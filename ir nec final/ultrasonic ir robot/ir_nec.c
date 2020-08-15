#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>

#include "ir_nec.h"

volatile struct IR_nec_packet_TypeDef
IR_nec_current_packet =
{
	.addr_l = 0,
	.addr_h = 0,
	.cmd = 0,
	.cmd_inv = 0
};

volatile struct IR_circular_q_TypeDef
IR_icr_buffer =
{
	.buffer = {0},
	.read_index = 0,
	.write_index = 0
};

volatile enum  IR_nec_status_TypeDef IR_nec_current_status = IDLE;
volatile enum IR_circular_q_status_TypeDef IR_icr_buffer_status = EMPTY;
volatile bool IR_nec_new_data = false;

static volatile uint8_t nec_current_state = 0;
static volatile uint8_t repeat_timer = 0;

void IR_Initialize()
{
	TCCR1B = (1 << ICNC1) | (1 << CS11);
	//PRESCALER SET TO 8, NEGATIVE EDGE DETECTION ENABLE AND NOISE CANCELLER ENABLED
	
	TIMSK1 = (1 << ICIE1) | (1 << TOIE1);
	//ENABLE INPUT CAPTURE AND TIMER OVERFLOW INTERUPT
}

void IR_DeInitialize()
{
	TCCR1B = 0;
	TIMSK1 = 0;
}

ISR (TIMER1_CAPT_vect)
{
	TCNT1 = 0;
	repeat_timer = 0;
	IR_WriteBuffer(ICR1);
}

ISR(TIMER1_OVF_vect)
{
	if (IR_nec_current_status == PACKET_RECVD_N_REPEAT_RECVNG)
	repeat_timer++;
	
	if (repeat_timer == 4)
	{
		nec_current_state = 0;
		IR_nec_current_status = IDLE;
	}
}

void IR_FlushBuffer(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		memset((void *)IR_icr_buffer.buffer,0,sizeof(IR_icr_buffer));
		IR_icr_buffer.read_index = 0;
		IR_icr_buffer.write_index = 0;
		IR_icr_buffer_status = EMPTY;
		nec_current_state = 0;
	}
}

void IR_WriteBuffer(uint16_t data)
{
	IR_icr_buffer.buffer[IR_icr_buffer.write_index] = data;
	
	if (IR_icr_buffer.write_index == IR_ICR_BUFFER_SIZE - 1)
	IR_icr_buffer.write_index = 0;
	else
	IR_icr_buffer.write_index++;
	
	if (IR_icr_buffer.write_index == IR_icr_buffer.read_index)
	IR_icr_buffer_status = OVERFLOW;
	else
	IR_icr_buffer_status = OK;
}

uint16_t IR_ReadBuffer()
{
	uint16_t icr_copy;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		icr_copy = IR_icr_buffer.buffer[IR_icr_buffer.read_index];
		
		if (IR_icr_buffer.read_index == IR_ICR_BUFFER_SIZE - 1)
		IR_icr_buffer.read_index = 0;
		else
		IR_icr_buffer.read_index++;
		
		if (IR_icr_buffer.read_index == IR_icr_buffer.write_index)
		IR_icr_buffer_status = EMPTY;
		else
		IR_icr_buffer_status = OK;
	}
	
	return icr_copy;
}

void IR_ProcessData()
{
	uint16_t icr_copy;
	
	if (IR_icr_buffer_status == OK)
	{
		icr_copy = IR_ReadBuffer();
		nec_current_state++;
		
		if (nec_current_state == 2)
		{
			if (!(icr_copy <= IR_START_TIME_H && icr_copy >= IR_START_TIME_L))
			{
				nec_current_state = 0;
				IR_nec_current_status = TIMING_ERR;
			}
		}
		else if (nec_current_state <= 34 && nec_current_state >=  3)
		{
			if (icr_copy <= IR_HIGH_TIME_H && icr_copy >= IR_HIGH_TIME_L)
			{
				if (nec_current_state <= 10)
				IR_nec_current_packet.addr_l |= (1 << (nec_current_state - 3));
				else if (nec_current_state <= 18)
				IR_nec_current_packet.addr_h |= (1 << (nec_current_state - 11));
				else if (nec_current_state <= 26)
				IR_nec_current_packet.cmd |= (1 << (nec_current_state  - 19));
				else
				IR_nec_current_packet.cmd_inv |= (1 << (nec_current_state - 27));
			}
			else if (icr_copy <= IR_LOW_TIME_H && icr_copy >= IR_LOW_TIME_L)
			{
				if (nec_current_state <= 10)
				IR_nec_current_packet.addr_l &= ~(1 << (nec_current_state - 3));
				else if (nec_current_state <= 18)
				IR_nec_current_packet.addr_h &= ~(1 << (nec_current_state - 11));
				else if (nec_current_state <= 26)
				IR_nec_current_packet.cmd &= ~(1 << (nec_current_state - 19));
				else
				IR_nec_current_packet.cmd_inv &= ~(1 << (nec_current_state - 27));
			}
			else
			{
				nec_current_state = 0;
				IR_nec_current_status = TIMING_ERR;
			}
		}
		else if (nec_current_state == 35)
		{
			if (IR_nec_current_packet.cmd == (unsigned char)~(IR_nec_current_packet.cmd_inv))
			{
				IR_nec_current_status = PACKET_RECVD;
				IR_nec_new_data = true;
			}
			else
			{
				nec_current_state = 0;
				IR_nec_current_status = CHKSM_ERR;
			}
		}
		else if (nec_current_state == 36)
		{
			if (!(icr_copy <= IR_REPEAT1_TIME_H && icr_copy >= IR_REPEAT1_TIME_L))
			{
				if (icr_copy <= IR_START_TIME_H && icr_copy >= IR_START_TIME_L)
				{
					nec_current_state = 2;
				}
				else
				{
					nec_current_state = 0;
					IR_nec_current_status = TIMING_ERR;
				}
			}
		}
		else if (nec_current_state == 37)
		{
			if (icr_copy <= IR_REPEAT2_TIME_H && icr_copy >= IR_REPEAT2_TIME_L)
			{
				nec_current_state = 35;
				IR_nec_current_status = PACKET_RECVD_N_REPEAT_RECVNG;
			}
			else if (icr_copy <= IR_START_TIME_H && icr_copy >= IR_START_TIME_L)
			{
				nec_current_state = 2;
			}
			else
			{
				nec_current_state = 0;
				IR_nec_current_status = TIMING_ERR;
			}
		}
	}
	else if (IR_icr_buffer_status == OVERFLOW)
	{
		IR_FlushBuffer();
	}
}
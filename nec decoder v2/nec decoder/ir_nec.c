#include "ir_nec.h"

inline void IR_Initialize()
{
	TCCR1B = (1 << ICNC1) | (1 << CS11); 
	//PRESCALER SET TO 8, NEGATIVE EDGE DETECTION ENABLE AND NOISE CANCELLER ENABLED
	
	TIMSK1 = (1 << ICIE1) | (1 << TOIE1);
	//ENABLE INPUT CAPTURE AND TIMER OVERFLOW INTERUPT
}

inline void IR_DeInitialize()
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
	repeat_timer++;
	if (repeat_timer == 3)
	{
		nec_current_state = 0;
		nec_current_status = IDLE;
	}
}

void IR_FlushBuffer(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		memset((void *)icr_buffer.buffer,0,sizeof(icr_buffer));
		icr_buffer.read_index = 0;
		icr_buffer.write_index = 0;
		icr_buffer_status = EMPTY;
		nec_current_state = 0;
	}
}

inline void IR_WriteBuffer(uint16_t data)
{
	icr_buffer.buffer[icr_buffer.write_index] = data;
	
	if (icr_buffer.write_index == 9)
		icr_buffer.write_index = 0;
	else
		icr_buffer.write_index++;
		
	if (icr_buffer.write_index == icr_buffer.read_index)
		icr_buffer_status = OVERFLOW;
	else
		icr_buffer_status = OK;			
}

uint16_t IR_ReadBuffer()
{
	uint16_t icr_copy;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		icr_copy = icr_buffer.buffer[icr_buffer.read_index];
		
		if (icr_buffer.read_index == 9)
		icr_buffer.read_index = 0;
		else
		icr_buffer.read_index++;
		
		if (icr_buffer.read_index == icr_buffer.write_index)
		icr_buffer_status = EMPTY;
		else
		icr_buffer_status = OK;
	}	
	
	return icr_copy;
}

void IR_ProcessData()
{
	uint16_t icr_copy;
	
	if (icr_buffer_status == OK)
	{
		icr_copy = IR_ReadBuffer();
		nec_current_state++;
		
		if (nec_current_state == 2)
		{
			if (!(icr_copy <= IR_START_TIME_H && icr_copy >= IR_START_TIME_L))
			{
				nec_current_state = 0;
				nec_current_status = TIMING_ERR;
			}
		}
		else if (nec_current_state <= 34 && nec_current_state >=  3)
		{
			if (icr_copy <= IR_HIGH_TIME_H && icr_copy >= IR_HIGH_TIME_L)
			{
				if (nec_current_state <= 10)
					nec_current_packet.addr_l |= (1 << (nec_current_state - 3));
				else if (nec_current_state <= 18)
					nec_current_packet.addr_h |= (1 << (nec_current_state - 11));
				else if (nec_current_state <= 26)
					nec_current_packet.cmd |= (1 << (nec_current_state  - 19));
				else
					nec_current_packet.cmd_inv |= (1 << (nec_current_state - 27));
			}
			else if (icr_copy <= IR_LOW_TIME_H && icr_copy >= IR_LOW_TIME_L)
			{
				if (nec_current_state <= 10)
					nec_current_packet.addr_l &= ~(1 << (nec_current_state - 3));
				else if (nec_current_state <= 18)
					nec_current_packet.addr_h &= ~(1 << (nec_current_state - 11));
				else if (nec_current_state <= 26)
					nec_current_packet.cmd &= ~(1 << (nec_current_state - 19));
				else
					nec_current_packet.cmd_inv &= ~(1 << (nec_current_state - 27));
			}
			else
			{
				nec_current_state = 0;
				nec_current_status = TIMING_ERR;
			}
		}
		else if (nec_current_state == 35)
		{
			if (nec_current_packet.cmd == (unsigned char)~(nec_current_packet.cmd_inv))
			{
				nec_current_status = PACKET_RECVD;
				nec_new_data = true;
			}
			else
			{
				nec_current_state = 0;
				nec_current_status = CHKSM_ERR;
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
					nec_current_status = TIMING_ERR;
				}
			}
		}
		else if (nec_current_state == 37)
		{
			if (icr_copy <= IR_REPEAT2_TIME_H && icr_copy >= IR_REPEAT2_TIME_L)
			{
				nec_current_state = 35;
				nec_current_status = PACKET_RECVD_N_REPEAT_RECVNG;
			}
			else if (icr_copy <= IR_START_TIME_H && icr_copy >= IR_START_TIME_L)
			{
				nec_current_state = 2;
			}
			else
			{
				nec_current_state = 0;
				nec_current_status = TIMING_ERR;
			}
		}
	}
	else if (icr_buffer_status == OVERFLOW)
	{
		IR_FlushBuffer();
	}
}
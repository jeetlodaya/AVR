#include "ir_nec.h"

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
	nec_current_state += 1;
	repeat_timer = 0;
	
	if (nec_current_state == 2)
	{
		if (!(ICR1 <= IR_START_TIME_H && ICR1 >= IR_START_TIME_L))
		{
			nec_current_state = 0;
			nec_current_status = TIMING_ERR;
		}
	}
	else if (nec_current_state <= 34 && nec_current_state >=  3)
	{
		if (ICR1 <= IR_HIGH_TIME_H && ICR1 >= IR_HIGH_TIME_L)
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
		else if (ICR1 <= IR_LOW_TIME_H && ICR1 >= IR_LOW_TIME_L)
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
			nec_data_counter++;
		}
		else
		{
			nec_current_state = 0;
			nec_current_status = CHKSM_ERR;
		}	
	}
	else if (nec_current_state == 36)
	{
		if (!(ICR1 <= IR_REPEAT1_TIME_H && ICR1 >= IR_REPEAT1_TIME_L))
		{
			if (ICR1 <= IR_START_TIME_H && ICR1 >= IR_START_TIME_L)
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
		if (ICR1 <= IR_REPEAT2_TIME_H && ICR1 >= IR_REPEAT2_TIME_L)
		{
			nec_current_state = 35;
			nec_current_status = PACKET_RECVD_N_REPEAT_RECVNG;	
		}
		else if (ICR1 <= IR_START_TIME_H && ICR1 >= IR_START_TIME_L)
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

ISR(TIMER1_OVF_vect)
{
	repeat_timer++;
	if (repeat_timer == 5)
	{
		nec_current_state = 0;
		nec_current_status = IDLE;
	}
}
#include "complementary_filter.h"

uint8_t MPU6050_RawData[12];
volatile float acx,gyy,anglex = 0,error,setpoint = 8,kp = 8,kd = 0,ki = 0,previous_error = 0,sum_error = 0,dt = 0.00064,p_term,i_term,d_term,pid_total;

void COMPFILT_Initialize()
{
	TCCR2A = (WGM21);
	//CTC 0 TO OCR2A
	TCCR2B = (CS22) | (CS21) | (CS20);
	//1024 PRESCALER
	
	OCR2A = 10;
	//1562.5 hz sample rate
	TIMSK2 = (1 << OCIE2A);
	//OCR2A COMAPARE INTTERUPT
}

ISR(TIMER2_COMPA_vect)
{	
	MPU6050_ReadByteS(ACXH,&MPU6050_RawData[0],6);
	MPU6050_ReadByteS(GYXH,&MPU6050_RawData[6],6);
	
	acx = (360.0/3.14159265)*asin((MPU6050_RawData[0] << 8 | MPU6050_RawData[1])/32768.0);	
	gyy = 250.0*((MPU6050_RawData[8] << 8 | MPU6050_RawData[9])/32768.0);
	anglex = 0.98*(anglex + gyy*dt) + 0.02*acx;
	//-----------------------------------------------------------------------------
	error = setpoint - anglex;
	sum_error = sum_error + error;
	p_term = kp*error;
	i_term = ki*sum_error*dt;
	d_term = kd*(error - previous_error)/dt;
	previous_error = error;
	//-----------------------------------------------------------------------------
	if (p_term >= 255.0)
	{
		p_term = 255.0;
	} 
	else if (p_term <= -255.0)
	{
		p_term = -255.0;
	}
	//-------------------------------------------------------------------------------
	if ((i_term > 0.0 && error < 0.0) || (i_term < 0.0 && error > 0.0))
	{
		i_term = 0.0;
		sum_error = 0.0;
	}
	
	if(sum_error >= 255.0/(ki*dt))
	{
		sum_error = 255.0/(ki*dt);
	}else if (sum_error <= -255.0/(ki*dt))
	{
		sum_error = -255.0/(ki*dt);
	}
	//---------------------------------------------------------------------------------	
	if (d_term >= 255.0)
	{	
		d_term = 255.0;
	}	
	else if (d_term <= -255.0)
	{
		d_term = -255.0;
	}	
	//----------------------------------------------------------------------------------
	pid_total = p_term + i_term + d_term;
	if (pid_total >= 255.0)
	{
		pid_total = 255.0;
	}
	else if (pid_total <= -255.0)
	{
		pid_total = -255.0;
	}
	//-----------------------------------------------------------------------------------
	if (pid_total >= 0.0)
	{
		MOTOR_Backward((uint8_t)pid_total,(uint8_t)pid_total);	
	}else
	{
		pid_total = -pid_total;
		MOTOR_Forward((uint8_t)pid_total,(uint8_t)pid_total);
	}
}
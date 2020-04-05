/*
 * gesture_controlled_robot_rx.c
 *
 * Created: 10-04-2019 12:31:08
 *  Author: jeet
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "motor driver.c"
#include "nrf24.c"

int main(void)
{
	uint8_t MPU6050_data[12];
	uint16_t acx,acy,acz;
	
	MOTOR_Initialize();
	NRF24_Initialize();

	
		
    while(1)
    {
		NRF24_ReadRxFifo(MPU6050_data,12);
		acx = (MPU6050_data[0] << 8) | (MPU6050_data[1]);
		acy = (MPU6050_data[2] << 8) | (MPU6050_data[3]);
		acz = (MPU6050_data[4] << 8) | (MPU6050_data[5]);
		
		if (acx & 0x8000)
		{
			acx = ~acx + 1;
			acx = acx >> 7;
			
			if (acy & 0x8000)
			{
				acy = ~acy + 1;
				acy = acy >> 7;
				if (acx <= acy)
				{
					MOTOR_Forward(0,acx  + 9);
				}
				else
				{
					MOTOR_Forward(acx - acy,acx  + 9);
				}
			}
			else
			{
				acy = acy >> 7;
				if (acx <= acy)
				{
					MOTOR_Forward(acx,9);
				}
				else
				{
					MOTOR_Forward(acx,acx - acy + 9);
				}
			}	
		}
		
		else
		{
			acx = acx >> 7;
			if (acy & 0x8000)
			{
				acy = ~acy + 1;
				acy = acy >> 6;
				if (acx <= acy)
				{
					MOTOR_Backward(acx,9);
				}
				else
				{
					MOTOR_Backward(acx ,acx - acy + 9);
				}
			}
			else
			{
				acy = acy >> 7;
				if (acx <= acy)
				{
					MOTOR_Backward(0,acx + 9);
				}
				else
				{
					MOTOR_Backward(acx - acy,acx + 9);
				}
			}
		}
	}		
}
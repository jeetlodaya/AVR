/*
 * gesture_tx.c
 *
 * Created: 29-04-2019 09:45:27
 *  Author: jeet
 */ 


#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "lcd16x2.c"
#include "nrf24.c"
#include "mpu6050.c"

int main(void)
{
	uint8_t MPU6050_data[12];
	
	LCD_Initialize();
	MPU6050_Intialize();
	NRF24_Initialize();
	
	while(1)
	{
		MPU6050_ReadByteS(ACXH,MPU6050_data,6);
		MPU6050_ReadByteS(GYXH,(MPU6050_data+5),6);
		NRF24_WriteTxFifo(MPU6050_data,12);
		LCD_Integer2String((MPU6050_data[0] << 8)| MPU6050_data[1],5,0,0);
		LCD_Integer2String((MPU6050_data[2] << 8)| MPU6050_data[3],5,0,8);
		LCD_Integer2String((MPU6050_data[4] << 8)| MPU6050_data[5],5,1,0);
	}
}
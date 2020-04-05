#include "mpu6050.h"

inline void MPU6050_Intialize(void)
{
	I2C_Initialize();
	MPU6050_WriteByte(PWRMGMT1,0x01);
	MPU6050_WriteByte(CONFIG,0x06);
}

void MPU6050_WriteByte(uint8_t address,uint8_t data)
{
	I2C_Start() ;
	I2C_Write(MPU6050_WRITE);
	I2C_Write(address);
	I2C_Write(data);
	I2C_Stop();
}

uint8_t MPU6050_ReadByte(uint8_t address)
{
	uint8_t data;
	I2C_Start();
	I2C_Write(MPU6050_WRITE);
	I2C_Write(address);
	I2C_Start();
	I2C_Write(MPU6050_READ);
	data = I2C_ReadNoAsk();
	I2C_Stop();
	return data;
}

void MPU6050_WriteByteS(uint8_t address,uint8_t *arr,uint8_t size)
{
	I2C_Start() ;
	I2C_Write(MPU6050_WRITE);
	I2C_Write(address);
	
	do 
	{
		I2C_Write(*(arr++));
		
	} while (--size);
	
	I2C_Stop();
	
}

void MPU6050_ReadByteS(uint8_t address,uint8_t *arr,uint8_t size)
{
	I2C_Start();
	I2C_Write(MPU6050_WRITE);
	I2C_Write(address);
	I2C_Start();
	I2C_Write(MPU6050_READ);
	
	do 
	{
		if(size == 1)
			*arr = I2C_ReadNoAsk();
		else	
			*(arr++) = I2C_ReadAsk();
		
	} while (--size);

	I2C_Stop();
}



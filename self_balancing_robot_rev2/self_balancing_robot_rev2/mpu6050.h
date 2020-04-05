#ifndef MPU6050_H
#define MPU6050_H

#include <avr/io.h>
#include "i2c.c"

#define	MPU6050_READ 0xd1
#define MPU6050_WRITE 0xd0
#define PWRMGMT1 0x6b
#define GYRO_CONFIG 0x1b
#define CONFIG 0x1a
#define ACXH 0x3b
#define TEMPH 0x41
#define GYXH 0x43

inline void MPU6050_Intialize(void);
void MPU6050_WriteByte(uint8_t address,uint8_t data);
uint8_t MPU6050_ReadByte(uint8_t address);
void MPU6050_WriteByteS(uint8_t address,uint8_t *arr,uint8_t size);
void MPU6050_ReadByteS(uint8_t start_address,uint8_t *arr,uint8_t size);

#endif

#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/twi.h>

inline void I2C_Initialize(void);
void I2C_Start(void);
uint8_t I2C_ReadAsk(void);
uint8_t I2C_ReadNoAsk(void);
void I2C_Write(uint8_t data);
void I2C_Stop(void);
uint8_t I2C_GetStatus(void);

#endif




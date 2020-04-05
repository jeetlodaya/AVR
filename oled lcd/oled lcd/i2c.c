
#include "i2c.h"

inline void I2C_Initialize()
{
	TWBR = 12;//400khz
}

void I2C_Start()
{
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
}

void I2C_Stop()
{
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

uint8_t I2C_ReadAsk()
{
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);	
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t I2C_ReadNoAsk()
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}

void I2C_Write(uint8_t address)
{
	TWDR = address;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
}

uint8_t I2C_GetStatus()
{
	return (TWSR & 0b11111000);
}
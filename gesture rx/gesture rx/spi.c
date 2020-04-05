
#include "spi.h"

void SPI_Initialize()
{
	SPI_DDR |= (1 << SS) | (1 << MOSI) | (1 << SCK);
	SPCR = (1 << SPE) | (1 << MSTR);	
	SPSR = (1 << SPI2X); // F = FOS/2
	SPI_PORT |= (1 << SS);	
}

uint8_t SPI_Transciever(uint8_t data)
{   
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}

void SPI_CsClear()
{
	SPI_PORT &= ~(1 << SS);
}

void SPI_CsSet()
{
	SPI_PORT |= (1 << SS);
}




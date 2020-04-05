#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK 5
#define MOSI 3
#define SS 2

void SPI_Initialize(void);
uint8_t SPI_Transciever(uint8_t data);
void SPI_CsSet(void);
void SPI_CsClear(void);

#endif




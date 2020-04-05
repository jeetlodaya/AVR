#ifndef UART_H
#define UART_H

#define BAUD 9600
#define UBBR_VALUE (uint16_t)(F_CPU/(16UL*BAUD) - 1)

#include <avr/io.h>

inline void UART_Initialize(void);
uint8_t UART_Read(void);
void UART_Write(uint8_t data);

#endif

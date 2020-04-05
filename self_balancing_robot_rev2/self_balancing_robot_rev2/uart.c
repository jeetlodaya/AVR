
#include "uart.h"

inline void UART_Initialize()
{
	UBRR0H = (uint8_t) (UBBR_VALUE >> 8);
	UBRR0L = (uint8_t) UBBR_VALUE;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UPM01) | (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0); 
	//STOP BIT 2 DATA 8 BIT PARITY EVEN 
}




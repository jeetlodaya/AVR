
#include "uart.h"

inline void UART_Initialize()
{
	UBRRH = (uint8_t) (UBBR_VALUE >> 8);
	UBRRL = (uint8_t) UBBR_VALUE;
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << UPM1) | (1 << UCSZ1) | (1 << UCSZ0) | (1 << USBS)  ; //STOP BIT 2 DATA 8 BIT PARITY EVEN 
}

void UART_Write(uint8_t data)
{
	UDR = data;
	while ( !(UCSRA & (1<<UDRE)) );
}

uint8_t UART_Read()
{
	while ( !(UCSRA & (1<<RXC)) );
	return UDR;
}
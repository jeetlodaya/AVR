/*
 * nec decoder.c
 *
 * Created: 11-05-2020 20:13:54
 * Author : jeet
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.c"
#include "ir_nec.c"

static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}


int main(void)
{
	UART_Initialize();
	stdout = &mystdout;
	
	IR_Initialize();
	
	sei();
	
	while (1)
	{
		IR_ProcessData();	
		if (nec_new_data == true || nec_current_status == PACKET_RECVD_N_REPEAT_RECVNG)
		{
			nec_new_data = false;
			printf("nec_addrl=%u nec_addrh=%u nec_cmd=%u nec_cmdinv=%u\n",nec_current_packet.addr_h,nec_current_packet.addr_l,nec_current_packet.cmd,nec_current_packet.cmd_inv);
		}
	}
	
}


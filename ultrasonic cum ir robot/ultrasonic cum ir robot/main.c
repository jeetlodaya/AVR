#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/eeprom.h>
#include "uart.h"
#include "ir_nec.h"

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
	
	MOTOR_Initialize();
	IR_Initialize();
	
	sei();
	
	while (1)
	{
		IR_ProcessData();
		if (IR_nec_new_data == true || IR_nec_current_status == PACKET_RECVD_N_REPEAT_RECVNG)
		{
			IR_nec_new_data = false;
			printf("nec_addrl=%u nec_addrh=%u nec_cmd=%u nec_cmdinv=%u\n",IR_nec_current_packet.addr_h,IR_nec_current_packet.addr_l,IR_nec_current_packet.cmd,IR_nec_current_packet.cmd_inv);
		}
	}
	
}
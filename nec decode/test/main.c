#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "uart.c"
#include "ir_nec.c"
#include "motor driver.c"

#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED_1 4
#define LED_2 5

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

uint8_t EEMEM eep_left_motor_speed,eep_right_motor_speed,eep_led;

int main(void)
{
	uint8_t left_motor_speed,right_motor_speed;
	
	left_motor_speed = eeprom_read_byte(&eep_left_motor_speed);
	right_motor_speed = eeprom_read_byte(&eep_right_motor_speed);
	
	UART_Initialize();
	stdout = &mystdout;
	
	MOTOR_Initialize();
	IR_Initialize();
	
	LED_DDR |= (1 << LED_1) | (1 << LED_2);
	LED_PORT |= eeprom_read_byte(&eep_led);
	
	sei();
	
	while (1)
	{
		if (nec_current_status == PACKET_RECVD || nec_current_status == PACKET_RECVD_N_REPEAT_RECVNG)
		{	
			if (nec_current_packet.cmd == 202 || nec_current_packet.cmd == 147)
				MOTOR_Forward(left_motor_speed,right_motor_speed);
			else if (nec_current_packet.cmd == 210 || nec_current_packet.cmd == 139)
				MOTOR_Backward(left_motor_speed,right_motor_speed);
			else if (nec_current_packet.cmd == 193 || nec_current_packet.cmd == 200)
				MOTOR_HardRight(left_motor_speed,right_motor_speed);	
			else if (nec_current_packet.cmd == 153 || nec_current_packet.cmd == 142)
				MOTOR_HardLeft(left_motor_speed,right_motor_speed);
			else if (nec_current_packet.cmd == 146)
				MOTOR_SoftFrontLeft(right_motor_speed);
			else if (nec_current_packet.cmd == 204)
				MOTOR_SoftFrontRight(left_motor_speed);	
			else if (nec_current_packet.cmd == 138)
				MOTOR_SoftBackLeft(left_motor_speed);
			else if (nec_current_packet.cmd == 196)
				MOTOR_SoftBackRight(right_motor_speed);			
		}
		else if (nec_current_status == IDLE)
			MOTOR_Stop();
			
		if(nec_data_counter >= 1)
		{	
			nec_data_counter = 0;
			if(nec_current_packet.cmd == 221)
			{
				if (left_motor_speed != 255)
					left_motor_speed += 5;
			}
			else if (nec_current_packet.cmd == 140)
			{
				if (left_motor_speed != 0)
					left_motor_speed -= 5;
			}
			else if (nec_current_packet.cmd == 152)
			{
				if (right_motor_speed != 255)
					right_motor_speed += 5 ;
			}
			else if (nec_current_packet.cmd == 151)
			{
				if (right_motor_speed != 0)
					right_motor_speed -= 5;
			}
			else if (nec_current_packet.cmd == 220)
			{
				LED_PORT ^= (1 << LED_1) | (1 << LED_2);	
			}
			else if (nec_current_packet.cmd == 131)
			{
				left_motor_speed = 255;
				right_motor_speed = 255;
			}
			eeprom_write_byte (&eep_led,LED_PORT);
			eeprom_write_byte (&eep_left_motor_speed, left_motor_speed);
			eeprom_write_byte (&eep_right_motor_speed, right_motor_speed);
		}	
		printf("%u\n",nec_current_packet.cmd);
	}
	
}

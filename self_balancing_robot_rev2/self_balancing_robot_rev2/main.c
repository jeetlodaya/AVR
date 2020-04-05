/*
 * self_balancing_robot_rev2.c
 *
 * Created: 24-01-2020 22:05:51
 * Author : jeet
 */ 
#define F_CPU 16000000UL 

#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <avr/interrupt.h>
#include "mpu6050.c"
#include "uart.c"
#include "motor driver.c"
#include "complementary_filter.c"



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
	MPU6050_Intialize();
	COMPFILT_Initialize();
	
	sei();
	
	while (1)
	{
		
	}
	
}
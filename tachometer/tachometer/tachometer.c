/*
 * tachometer.c
 *
 * Created: 01-02-2019 09:26:22
 *  Author: jeet
 */ 

#define F_CPU 16000000U

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd16x2.c"

volatile uint16_t rps ;

void timers_init(void)
{
	//timer0 1 second
	TIMSK = (1 << TOIE0) ;
	TCCR0 = (1 << CS02) | (1 << CS00) ;
	 
	//timer1 counter rising edge
	TCCR1B = (1 << CS12) | (1 << CS11) | (1 << CS10) ;
	
}

ISR(TIMER0_OVF_vect)
{
	 //overflow timer1 61 times to obtain 1 second
	static uint8_t count_1sec = 61 ;
	
	if (!(--count_1sec))
	{   
		rps = TCNT1 ;
		TCNT1 = 0 ;
		count_1sec = 61 ;
	}
	
}

int main(void)
{	
	timers_init();
	LCD_Initialize();
	
	sei();
	
	LCD_String("RPS :") ;
	
    while(1)
    {
		LCD_Integer2String(rps,6,0,5) ;
    }
}
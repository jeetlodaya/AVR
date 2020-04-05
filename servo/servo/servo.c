/*
 * servo.c
 *
 * Created: 21-10-2018 10:40:17
 *  Author: jeet
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "lcd.h"

#define SERVO_MIN 1200                                                                 //0.6mS
#define SERVO_180 4800                                                                 //2.4mS
#define SERVO_FREE 40000                                                               //100% DC
#define SERVO_ANGLE(deg) (uint16_t)(OCR1A =(SERVO_MIN + ((SERVO_180 - SERVO_MIN)/180)*deg))     //macro to convert degrees to OCR

void servo_initialization(void){
	DDRB |= (1 << PINB1) ;                                                             // OCR1A OUTPUT                                                                 
	ICR1 = 40000 ;                                                                     //0-40000 count to get 50hz || 16mhz/8/40000 = 50hz 
	OCR1A = 40000 ;                                                                    //free servo initially
	TCCR1A = (1 << COM1A1) | (1 << WGM11) ;                                            //low after compare
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) ;                               //div by 8 , 0 - ICR fast PWM 
	
}

int main(void)
{   lcd_init() ;
	servo_initialization() ;
    SERVO_ANGLE(20) ;
	lcd_string("CAN YOU SEE THIS RAW DATA!");
	
	
    while(1)
    {
       
    }
}
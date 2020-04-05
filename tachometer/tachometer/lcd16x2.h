/*
 * lcd.c
 *
 * Created: 21-06-2018 18:57:38
 *  Author: jeet 
 * CODE FOR LCD IN 4 BIT MODE WITH BUSY FLAG
 * NOTE ALL THE PINS OF LCD SHOULD BE CONNECTED TO 1 PORT 
 
 
 
           ___________________                     ____________________   
		  |                   |                   |                    |
		  |                RS |------------------>| PD4                |
		  |                   |                   |                    |
		  |                RW |------------------>| PD6                |
		  |      L            |                   |           A        |
		  |                EN |------------------>| PD7                |
		  |                   |                   |                    |
		  |      C         D4 |------------------>| PD0       V        |
		  |                   |                   |                    |
		  |                D5 |------------------>| PD1                |
		  |      D            |                   |           R        |                   
		  |                D6 |------------------>| PD2                |
		  |                   |                   |                    |
		  |                D7 |------------------>| PD3                |
		  |___________________|                   |____________________|
		  


*/

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DDR DDRD
#define LCD_PORT PORTD
#define LCD_READ PIND

#define RS 4
#define RW 6
#define	EN 7
#define D4 0
#define D5 1
#define D6 2
#define D7 3

#define LCD_CLEAR 0x01;
#define LCD_HOME 0x02;

inline void LCD_Initialize(void);
static void Enable(void);
static void BusyFlag(void);
void LCD_Command(uint8_t data);
void LCD_Ascii(uint8_t data);
void LCD_Cursor(uint8_t row,uint8_t column);
void LCD_String(char *str);
void LCD_Integer2String(uint16_t integer ,uint8_t size,uint8_t row,uint8_t column);

#endif

  
 



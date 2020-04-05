/*
 * lcd.c
 *
 * Created: 21-06-2018 18:57:38
 *  Author: jeet 
 * CODE FOR LCD IN 4 BIT MODE WITH BUSY FLAG
 * NOTE ALL THE PINS OF LCD SHOULD BE CONNECTED TO 1 PORT 
 
 
 
           ___________________                     ____________________   
		  |                   |                   |                    |
		  |                RS |------------------>| PD0                |
		  |                   |                   |                    |
		  |                RW |------------------>| PD1                |
		  |      L            |                   |           A        |
		  |                EN |------------------>| PD2                |
		  |                   |                   |                    |
		  |      C         D4 |------------------>| PD3       V        |
		  |                   |                   |                    |
		  |                D5 |------------------>| PD4                |
		  |      D            |                   |           R        |                   
		  |                D6 |------------------>| PD5                |
		  |                   |                   |                    |
		  |                D7 |------------------>| PD6                |
		  |___________________|                   |____________________|
		  


*/

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DDR DDRD
#define LCD_PORT PORTD
#define LCD_READ PIND

#define RS 0
#define RW 1
#define	EN 2
#define D4 3
#define D5 4
#define D6 5
#define D7 6

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

  
 



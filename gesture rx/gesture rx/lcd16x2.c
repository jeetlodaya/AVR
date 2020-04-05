
#include "lcd16x2.h"

static void Enable()
{
	LCD_PORT |= (1 << EN);
	asm volatile ("nop");
	asm volatile ("nop"); 
	asm volatile ("nop"); 
	LCD_PORT &= ~(1 << EN);
}

static void BusyFlag(void)
{
	uint8_t temp;
	LCD_DDR &= ~((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4));
	LCD_PORT |= (1 << RW) ;
	LCD_PORT &= ~((1 << RS) | (1 << D7) | (1 << D6) | (1 << D5) | (1 << D4)) ;
	do{
		Enable();
		temp = LCD_READ & (1 << D7);
		Enable();
	} while(temp);
	LCD_PORT &= ~(1 << RW);
	LCD_DDR |= (1 << D7) | (1 << D6) | (1 << D5) | (1 << D4);
}


void LCD_Command(uint8_t data)
{
	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4));
	LCD_PORT |= (data & 0xf0) >> (7 - D7);
	LCD_PORT &=  ~(1 << RS);
	Enable();
	
	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4));
	LCD_PORT |= (data & 0x0f) << D4 ;
	Enable();
	BusyFlag();
}

void LCD_Ascii(uint8_t data )
{
	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4));
	LCD_PORT |= ((data & 0xf0) >> (7 - D7)) | (1 << RS);
	Enable();

	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << D5) | (1 << D4));
	LCD_PORT |= ((data & 0x0f) << D4);
	Enable();
	BusyFlag();
}

inline void LCD_Initialize()
{
	LCD_DDR |= (1 << D7) | (1 << D6) | (1 << D5) | (1 << D4) | (1 << EN) | (1 << RS) | (1 << RW);
	_delay_ms(16);
	
	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << RS) | (1 << RW));
	LCD_PORT |= (1 << D5) | (1 << D4);
	Enable();
	_delay_us(50);
	
	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << RS) | (1 << RW));
	LCD_PORT |= (1 << D5) | (1 << D4);
	Enable();
	_delay_us(50);
	
	LCD_PORT &= ~((1 << D7) | (1 << D6) | (1 << D4) | (1 << RS) | (1 << RW));
	LCD_PORT |= (1 << D5);
	Enable();
	BusyFlag();
	
	LCD_Command(0x28);
	LCD_Command(0x0c);
	LCD_Command(0x01);
}


void LCD_Cursor(uint8_t row , uint8_t column)
{
	if(row == 0)
	LCD_Command(0x80 + column);
	else
	LCD_Command(0xc0 + column);
}

void LCD_String(char *str)
{
	do
	{
		LCD_Ascii(*(str++));
		
	} while (*str);
	
}

void LCD_Integer2String(uint16_t integer,uint8_t size,uint8_t row,uint8_t column)
{
	LCD_Cursor(row,column);
	char arr[6];

	arr[size--] = '\0';
	
	do
	{
		arr[size] = integer%10 + '0';
		integer /= 10 ;
		
	} while (size-- != 0);
	
	LCD_String(arr);
}


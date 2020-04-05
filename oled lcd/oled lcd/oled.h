
//PRINT TEXT FAST ONLY WORKS WITH 8X6 MIKRO GLCD
//#define OLED_FRACIONALSIZE //for fractional size text(using floating point math)
//filled polygon only works with right angle rectangles

#ifndef OLED_H
#define OLED_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "i2c.c"
#include "font.h"

#define OLED_FONTY 8
#define OLED_FONTX 6

#define OLED_FONTCOLUMNSIZEINBYTES (uint8_t) ceil(OLED_FONTY/8.0)
#define OLED_FONTTOTALCHARACTERSIZEINBYTES (uint8_t) (OLED_FONTCOLUMNSIZEINBYTES*OLED_FONTX + 1) 

#define OLED_WRITEADR 0x78
#define OLED_READADR 0x79
#define OLED_WRITEDATA 0x40
#define OLED_WRITECOMMAND 0x00 

struct OLED_FrameCursor
{
	uint8_t y;
	uint8_t x;
};

struct OLED_FrameCursor OLED_CurrentCordinates = {0,0};

uint8_t OLED_FrameBuffer[8][128];

inline void OLED_Initialize(void);
void OLED_WriteFlashArray(const uint8_t *arr,uint16_t size);
static void OLED_SendCommand(void);
static void OLED_SendData(void);
void OLED_SetContrast(uint8_t data);
void OLED_SetCursor(uint8_t page,uint8_t x);
void OLED_SetFrameCursor(uint8_t y,uint8_t x);
void OLED_PrintTextFast(char *arr);
#ifdef	OLED_FRACTIONALSIZE
void OLED_PrintTextSlow(char *arr,float size);
#else
void OLED_PrintTextSlow(char *arr,uint8_t size);
#endif
void OLED_DrawPixel(int8_t y,int8_t x);
void OLED_DrawLine(int8_t y1,int8_t x1,int8_t y2,int8_t x2);
void OLED_DrawPolygon(int8_t arr[10][2],uint8_t no_sides);
void OLED_DrawFilledPolygon(int8_t arr[10][2],uint8_t no_sides);
void OLED_DrawCircle(int8_t k,int8_t h,int8_t radius);
void OLED_DrawFilledCircle(int8_t k,int8_t h,int8_t radius);
void OLED_DrawBitMap(int8_t y,int8_t x,const uint8_t *arr,uint8_t no_of_rows,uint8_t no_of_column);
void OLED_InvertDisplay(uint8_t enable);
static void OLED_ClearScreenBuffer(void);
void OLED_ClearScreen(void);

static uint8_t OLED_ReadFrameBuffer(uint8_t y,uint8_t x);

#endif
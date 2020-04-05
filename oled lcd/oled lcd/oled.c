#include "oled.h"


inline void OLED_Initialize()
{
	static const uint8_t OLED_CommandArray[] PROGMEM = 
	{
		0x00,						//lower nibble of column address **
		0x10,						//higher nibble  of column address **
		0x20,0x00,					//horizontal address mode **
		0x21,0x00,0xFF,				//start and end address of column
		0x22,0x00,0xFF,				//page start and end address
		0xB0,						//set page address **
		0x00,						//set display start line register
		0xA1,						//segment remappig **
		0xA8,0xFF,                  //mux ratio 64
		0xC8,                       //com scan direction 64 to 0 **
		0xD3,0x00,					//display offset 0
		0xDA,0x12,                  //sequential com pin **
		0xD5,0xF0,					//display clock and oscillator freq
		0xD9,0x22,	                //set pre charge period
		0xDB,0x20,                  //Vcomh deselect
		0x81,0xFF,                  //set contrast
		0xA4,                       //display ram content
		0xA6,                       //normal mode 
		0x8D,0x14,                  //enable charge pump **
		0x2E,
		0xAF                        //entire display on
	};
	
	I2C_Initialize();
	OLED_SendCommand();
	OLED_WriteFlashArray(OLED_CommandArray,34);
	I2C_Stop();
	
	OLED_ClearScreen();
}

static void OLED_SendCommand()
{
	I2C_Start();
	I2C_Write(OLED_WRITEADR);
	I2C_Write(OLED_WRITECOMMAND);
}

static void OLED_SendData()
{
	I2C_Start();
	I2C_Write(OLED_WRITEADR);
	I2C_Write(OLED_WRITEDATA);
}

void OLED_WriteFlashArray(const uint8_t *arr,uint16_t size)
{
	uint8_t buffer;
	do 
	{
		buffer = pgm_read_byte(arr++);
		I2C_Write(buffer);
		
	} while (--size);	
}

void OLED_SetCursor(uint8_t page,uint8_t x)
{
	OLED_SendCommand();
	I2C_Write(x & 0x0F);
	I2C_Write((x & 0xF0) >> 4 | 0x10);
	I2C_Write(page | 0xB0);
	I2C_Stop();
}

void OLED_SetFrameCursor(uint8_t y,uint8_t x)
{
	OLED_CurrentCordinates.y = y;
	OLED_CurrentCordinates.x = x;
}

void OLED_SetContrast(uint8_t data)
{
	OLED_SendCommand();
	I2C_Write(0x81);
	I2C_Write(data);
	I2C_Stop();
}

void OLED_PrintTextFast(char *arr)
{
	uint16_t index;
	uint8_t buffer;
	
	do 
	{
		index = (*(arr++) - ' ')*6;
		
		OLED_SendData();
		
		for (uint8_t i = 0;i <= 5;i++)
		{
			buffer = pgm_read_byte(&OLED_Font[index + i]);
			I2C_Write(buffer);
		}
		I2C_Stop();
				
	} while (*arr != '\0');
}


#ifdef OLED_FRACTIONALSIZE
void OLED_PrintTextSlow(char *arr,float size)
{
	uint16_t index;
	uint8_t buffer;
	
	size = size + 1.0;
	
	do 
	{
		index =  (*(arr++) - ' ')*OLED_FONTTOTALCHARACTERSIZEINBYTES;
		
		for (uint8_t y = 0;y <= (uint8_t)OLED_FONTY*size - 1;y++)
		{
			for (uint8_t x = 0; x <= (uint8_t)OLED_FONTX*size - 1;x++)
			{	
				buffer = pgm_read_byte(&OLED_Font[(uint8_t)(y/size)/8 + (uint8_t)(x/size)*OLED_FONTCOLUMNSIZEINBYTES + 1 + index]);
				if (((buffer >> (uint8_t)(y/size)%8) & 0x01) == 0x01)
				{
					OLED_DrawPixel(OLED_CurrentCordinates.y + y,OLED_CurrentCordinates.x + x);
				}	
			}
		}
		
		OLED_CurrentCordinates.x += (uint8_t)OLED_FONTX*size;
		
		if ((OLED_CurrentCordinates.x >= (uint8_t)(132.0 - size*OLED_FONTX))
		{
			OLED_CurrentCordinates.x = 0;
			OLED_CurrentCordinates.y += (uint8_t)OLED_FONTY*size;
		}
		
	} while (*arr != '\0');
}
#else

void OLED_PrintTextSlow(char *arr,uint8_t size)
{
	uint16_t index;
	uint8_t buffer;
	
	size++;
	
	do
	{
		index =  (*(arr++) - ' ')*OLED_FONTTOTALCHARACTERSIZEINBYTES;
		
		for (uint8_t y = 0;y <= (OLED_FONTY * size) - 1;y++)
		{
			for (uint8_t x = 0; x <= (OLED_FONTX * size) -  1;x++)
			{
				buffer = pgm_read_byte(&OLED_Font[(y / size)/8 + (x / size)*OLED_FONTCOLUMNSIZEINBYTES + 1 + index]);
				if (((buffer >> (y / size)%8) & 0x01) == 0x01)
				{
					OLED_DrawPixel(OLED_CurrentCordinates.y + y,OLED_CurrentCordinates.x + x);
				}
			}
		}
		
		OLED_CurrentCordinates.x += (OLED_FONTX * size);
		
		if (OLED_CurrentCordinates.x >= 132 - size*OLED_FONTX)
		{
			OLED_CurrentCordinates.x = 0;
			OLED_CurrentCordinates.y += (OLED_FONTY * size);
		}
		
	} while (*arr != '\0');
}

#endif

void OLED_DrawPixel(int8_t y,int8_t x)
{	
	if (y <= 63 && x <= 127)
	{
		OLED_SetCursor(y/8,x);
		OLED_SendData();
		OLED_FrameBuffer[y/8][x] |= 1 << (y%8);
		I2C_Write(OLED_FrameBuffer[y/8][x]);
		I2C_Stop();
	}	
}

void OLED_DrawLine(int8_t y1,int8_t x1,int8_t y2,int8_t x2)
{
	uint8_t y,x,x_lowerlimit,x_upperlimit,y_upperlimit,y_lowerlimit;
	
	if (x1 <= x2)
	{
		x_lowerlimit = x1;
		x_upperlimit = x2;
	} 
	else
	{
		x_lowerlimit = x2;
		x_upperlimit = x1;
	}
	
	if (y1 <= y2)
	{
		y_lowerlimit = y1;
		y_upperlimit = y2;
	}
	else
	{
		y_lowerlimit = y2;
		y_upperlimit = y1;
	}
	
	if ((x_upperlimit - x_lowerlimit >= y_upperlimit - y_lowerlimit) && (x2 - x1) != 0)
	{
		for (x = x_lowerlimit;x <= x_upperlimit;x++)
		{
			y = (x*(y2 - y1) + y1*(x2 - x1) + x1*(y1 - y2))/(x2 - x1);
			OLED_DrawPixel(y,x);
		}
	} 
	else if ((y1 - y2) != 0)
	{
		for (y = y_lowerlimit;y <= y_upperlimit;y++)
		{
			x = (y*(x1 - x2) + y1*(x2 - x1) + x1*(y1 - y2))/(y1 - y2);
			OLED_DrawPixel(y,x);
		}
	}
}

void OLED_DrawPolygon(int8_t arr[10][2],uint8_t no_sides)
{
	for (uint8_t y = 0;y <= no_sides - 1;y++)
	{
		OLED_DrawLine(arr[y][0],arr[y][1],arr[y + 1][0],arr[y + 1][1]);
	}
}

void OLED_DrawFilledPolygon(int8_t arr[10][2],uint8_t no_sides)
{
	OLED_ClearScreenBuffer();
	
	OLED_DrawPolygon(arr,no_sides);
	
	uint8_t pixel_counter = 0,y1,x1,y2,x2;
	
	for(uint8_t y = 0;y <= 63;y++)
	{
		for (uint8_t x = 0;x <= 127;x++)
		{
			if (OLED_ReadFrameBuffer(y,x) == 1)
			{
				pixel_counter++;
				
				if (pixel_counter == 1)
				{
					y1 = y;
					x1 = x;
				} 
				else
				{
					y2 = y;
					x2 = x;
				}
			}
		}
		if (pixel_counter >= 2)
		{
			OLED_DrawLine(y1,x1,y2,x2);
		}
		pixel_counter = 0;
	}
}

void OLED_DrawCircle(int8_t k,int8_t h,int8_t radius)
{
	for (uint8_t y = 0;y <= 63;y++)
	{
		for (uint8_t x = 0;x <=  127;x++)
		{
			if ((x - h)*(x - h) + (y - k)*(y - k) >= radius*radius - radius && (x - h)*(x - h) + (y - k)*(y - k) <= radius*radius + radius )
			{
				OLED_DrawPixel(y,x);
			}
		}
	}
}

void OLED_DrawFilledCircle(int8_t k,int8_t h,int8_t radius)
{
	for (uint8_t y = 0;y <= 63;y++)
	{
		for (uint8_t x = 0;x <=  127;x++)
		{
			if ((x - h)*(x - h) + (y - k)*(y - k) <= radius*radius)
			{
				OLED_DrawPixel(y,x);
			}
		}
	}
}

void OLED_DrawBitMap(int8_t y,int8_t x,const uint8_t *arr,uint8_t no_of_rows,uint8_t no_of_column)
{
	uint8_t buffer;
	for(uint8_t i = 0;i <= no_of_rows - 1;i++)
	{
		for (uint8_t j = 0;j <= no_of_column - 1;j++)
		{
			buffer = pgm_read_byte(&arr[j/8 + i*no_of_column/8]);
			if (((buffer >> (7 - j%8)) & 0x01) == 0x01)
			{
				OLED_DrawPixel(i + y,j + x);
			}
		}
	}
}

void OLED_ClearScreen()
{
	OLED_SendData();
	for(uint16_t i = 0;i <= 1023;i++)
	{
		I2C_Write(0x00);
	}
	I2C_Stop();
	
	OLED_ClearScreenBuffer();
	
	OLED_CurrentCordinates.x = 0;
	OLED_CurrentCordinates.y = 0;
}

void OLED_InvertDisplay(uint8_t enable)
{
	OLED_SendCommand();
	if (enable == 1)
		I2C_Write(0xA7);
	else
		I2C_Write(0xA6);
	I2C_Stop();
}

static void OLED_ClearScreenBuffer()
{
	for (uint8_t i = 0;i <= 7;i++)
	{
		for (uint8_t j = 0;j <= 127;j++)
		{
			OLED_FrameBuffer[i][j] = 0x00;
		}
	}
}

static uint8_t OLED_ReadFrameBuffer(uint8_t y,uint8_t x)
{
	uint8_t buffer;
	buffer = OLED_FrameBuffer[y/8][x];
	return ((buffer >> y%8) & 0x01);
}



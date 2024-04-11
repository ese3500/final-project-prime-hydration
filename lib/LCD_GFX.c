/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 

#include "LCD_GFX.h"
#include "ST7735.h"

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if ((pixels>>j)&1==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
	uint8_t t1 = radius / 16;
	uint8_t x = radius;
	uint8_t y = 0;
	while (x >= y) {
		LCD_drawLine(x0-y, y0-x, x0+y, y0-x, color);
		LCD_drawLine(x0-x, y0-y, x0+x, y0-y, color);
		LCD_drawLine(x0-x, y0+y, x0+x, y0+y, color);
		LCD_drawLine(x0-y, y0+x, x0+y, y0+x, color);
		y++;
		t1 += y;
		if (t1 - x >= 0) {
			t1 -= x;
			x--;
		}
	}
}

/* 
Midpoint Circle Algorithm - Jesko's Algorithm (Wikipedia)

t1 = r / 16
x = r
y = 0
Repeat Until x < y
    Pixel (x, y) and all symmetric pixels are colored (8 times)
    y = y + 1
    t1 = t1 + y
    t2 = t1 - x
    If t2 >= 0
        t1 = t2
        x = x - 1
*/

/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)  
{
	// constraint: x0 < x1 - TODO: fix
	short dx = x1 - x0;
	short dy = y1 - y0;
	short D = 2 * dy - dx;
	short y = y0;
	for (short x = x0; x < x1; x++) {
		LCD_drawPixel(x, y, c);
		if (D > 0) {
			y++;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}
}

/*
Bresenham’s Line Algorithm (Wikipedia)

plotLine(x0, y0, x1, y1)
	dx = x1 - x0
	dy = y1 - y0
	D = 2*dy - dx
	y = y0

	for x from x0 to x1
		plot(x, y)
		if D > 0
			y = y + 1
			D = D - 2*dx
		end if
		D = D + 2*dy
*/


/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
	for (uint8_t x = x0; x < x1; x++) {
		for (uint8_t y = y0; y < y1; y++) {
			LCD_drawPixel(x, y, color);
		}
	}
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
	for (uint8_t x = 0; x < LCD_WIDTH; x++) {
		for (uint8_t y = 0; y < LCD_HEIGHT; y++) {
			LCD_drawPixel(x, y, color);
		}
	}
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	for (int i = 0; str[i] != 0; i++) {
		LCD_drawChar(x + i * 5, y, str[i], fg, bg);
	}
}
/*
	Raspberry pi KS0108 LCD interface
	Author: Christian Isenberg
	
	With help from the following amazing libraries :D : 
		- Universal KS0108 driver library by  Rados³aw Kwiecieñ, radek@dxp.pl
		- BCM 2835 library by Mike McCauley
*/

#include "KS0108.h"
#include <bcm2835.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>


#define KS0108_RS	7
#define KS0108_EN	11
#define KS0108_CS1	25
#define KS0108_CS2	8
#define KS0108_CS3	9

#define  KS0108_D0	2
#define  KS0108_D1	3
#define  KS0108_D2	4
#define  KS0108_D3	17
#define  KS0108_D4	27
#define  KS0108_D5	22
#define  KS0108_D6	10
#define  KS0108_D7	9

#define KS0108_SCREEN_WIDTH		128
#define KS0108_SCREEN_HEIGHT	64

uint8_t screen_x, screen_y;
uint8_t framebuffer[1024]; // 128 colunas * 64 linhas /8 bits

void GLCD_ClearBuffer()
{
	memset (framebuffer, 0, sizeof(framebuffer));
}

void GLCD_SyncBuffer(){
	GLCD_Bitmap(framebuffer);
}

void GLCD_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
	for(int nx=x; nx<x+w ; nx++){
		for(int ny=y; ny<y+h ; ny++){
			GLCD_SetPixel(nx,ny);
		}
	}
}


void GLCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		GLCD_SetPixel(x0,y0);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}

}

void GLCD_WriteChar(uint8_t x, uint8_t y, char charToWrite, uint8_t* font)
{
	int firstChar = font[4];
	int charCount = font[5];
	int charHeight = font[3];
	int charWidth = font[2];
	int sum= 6;
	int fixed_width = 1;

	if( (font[0] + font [1]) != 0x00){
		fixed_width  = 0;
	}


	if( !fixed_width ){
		charWidth = font[6+(charToWrite-firstChar)];
		sum += charCount;
	}
	
	//jumps to the char data position on the array.
	for(int i=firstChar; i<charToWrite; i++){
		if( !fixed_width )
			sum += font[6+i-firstChar] * ceil(charHeight/8.0);
		else
			sum += charWidth * ceil(charHeight/8.0);
	}

	for(int line=0; line < charHeight; line+=8){
		for(int col=0; col<charWidth; col++){
			GLCD_SetPixels(x+col, ceil(y+line),
				font[sum + col + (int)ceil(charWidth*line/8.0)]
			);
		}
	}
		
}



//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(uint8_t x, uint8_t y, char * stringToWrite, uint8_t* font)
{
	while(*stringToWrite){
		GLCD_WriteChar(x,y,*stringToWrite++, font);
		x+=font[2]+1;
	}
}



//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(uint8_t x, uint8_t y)
{
	int idx = (KS0108_SCREEN_WIDTH * (y/8)) + x;
	framebuffer[idx] |= 1 << y%8;
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixels(uint8_t x, uint8_t y, uint8_t byte)
{
	int idx = (KS0108_SCREEN_WIDTH * (y/8)) + x;
	int idx2 = (KS0108_SCREEN_WIDTH * ( (y/8)+1) ) + x;
	uint8_t rest = y%8;
	framebuffer[idx] |= ( byte << y%8 );
	if(rest)
		framebuffer[idx2] |= byte >> (8-y%8);

}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(uint8_t * bmp)
{
	uint8_t i, j;
	int counter = 0;
	for(j = 0; j < 8; j++)
	{
		GLCD_GoTo(0,j);
		for(i = 0; i < KS0108_SCREEN_WIDTH; i++) 
		GLCD_WriteData((uint8_t)bmp[counter++]);
	}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_Initalize(void)
{
	uint8_t i;
	GLCD_InitalizePorts();
	for(i = 0; i < 3; i++)
		GLCD_WriteCommand((DISPLAY_ON_CMD | ON), i);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_GoTo(uint8_t x, uint8_t y)
{
	uint8_t i;
	screen_x = x;
	screen_y = y;
	for(i = 0; i < KS0108_SCREEN_WIDTH/64; i++)
	{
		GLCD_WriteCommand(DISPLAY_SET_Y | 0,i);
		GLCD_WriteCommand(DISPLAY_SET_X | y,i);
		GLCD_WriteCommand(DISPLAY_START_LINE | 0,i);
	}
	GLCD_WriteCommand(DISPLAY_SET_Y | (x % 64), (x / 64));
	GLCD_WriteCommand(DISPLAY_SET_X | y, (x / 64));
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearScreen(void)
{
	uint8_t i, j;
	for(j = 0; j < KS0108_SCREEN_HEIGHT/8; j++)
	{
		GLCD_GoTo(0,j);
		for(i = 0; i < KS0108_SCREEN_WIDTH; i++)
		GLCD_WriteData(0x00);
	}
}

//-------------------------------------------------------------------------------------------------
// Puts byte into Data Lines
//-------------------------------------------------------------------------------------------------
void RPI_PutData(uint8_t data)
{
	bcm2835_gpio_write(KS0108_D0, (data >> 0) & 1 ) ;
	bcm2835_gpio_write(KS0108_D1, (data >> 1) & 1 ) ;
	bcm2835_gpio_write(KS0108_D2, (data >> 2) & 1 ) ;
	bcm2835_gpio_write(KS0108_D3, (data >> 3) & 1 ) ;
	bcm2835_gpio_write(KS0108_D4, (data >> 4) & 1 ) ;
	bcm2835_gpio_write(KS0108_D5, (data >> 5) & 1 ) ;
	bcm2835_gpio_write(KS0108_D6, (data >> 6) & 1 ) ;
	bcm2835_gpio_write(KS0108_D7, (data >> 7) & 1 ) ;
}
//-------------------------------------------------------------------------------------------------
// Delay function
//-------------------------------------------------------------------------------------------------
void GLCD_Delay(void)
{
	bcm2835_delayMicroseconds(3);
}
//-------------------------------------------------------------------------------------------------
// Enalbe Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_EnableController(uint8_t controller)
{
	switch(controller){
		case 0 : bcm2835_gpio_write(KS0108_CS1, HIGH); break;
		case 1 : bcm2835_gpio_write(KS0108_CS2, HIGH); break;
		case 2 : bcm2835_gpio_write(KS0108_CS3, HIGH); break;
	}
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_DisableController(uint8_t controller)
{
	switch(controller){
		case 0 : bcm2835_gpio_write(KS0108_CS1, LOW); break;
		case 1 : bcm2835_gpio_write(KS0108_CS2, LOW); break;
		case 2 : bcm2835_gpio_write(KS0108_CS3, LOW); break;
	}
}

//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------
void GLCD_WriteCommand(uint8_t commandToWrite, uint8_t controller)
{
	GLCD_Delay();
	bcm2835_gpio_write(KS0108_RS, LOW);
	GLCD_EnableController(controller);
	RPI_PutData(commandToWrite);
	bcm2835_gpio_write(KS0108_EN, HIGH);
	GLCD_Delay();
	bcm2835_gpio_write(KS0108_EN, LOW);
	GLCD_DisableController(controller);
}

//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------
void GLCD_WriteData(uint8_t dataToWrite)
{
	GLCD_Delay();
	bcm2835_gpio_write(KS0108_RS, HIGH);
	RPI_PutData(dataToWrite);
	GLCD_EnableController(screen_x / 64);
	bcm2835_gpio_write(KS0108_EN, HIGH);
	GLCD_Delay();
	bcm2835_gpio_write(KS0108_EN, LOW);
	GLCD_DisableController(screen_x / 64);
	screen_x++;
}
//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
int GLCD_InitalizePorts(void)
{
	if (!bcm2835_init()){
		printf("Falha ao iniciar bcm2835...\n");
		return 1;
	}
	printf("Iniciou bcm2835...\n");

	//sets all pins as output
	bcm2835_gpio_fsel(KS0108_EN, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_RS, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_CS1, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_CS2,	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D0, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D1, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D2, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D3, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D4, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D5, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D6, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(KS0108_D7, 	BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(30, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(30 , 1);

	//zera framebuffer
	memset (framebuffer, 0, sizeof(framebuffer));

	return 0;
}

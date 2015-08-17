/*
	Raspberry pi KS0108 LCD interface
	Author: Christian Isenberg
	
	With help from the following amazing libraries :D : 
		- Universal KS0108 driver library by  Rados³aw Kwiecieñ, radek@dxp.pl
		- BCM 2835 library by Mike McCauley
*/

#include "KS0108.h"
#include "font5x8.h"
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
			GLCD_SetPixel(nx,ny,1);
		}
	}
}

void GLCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	float deltax = x1 - x0;
	float deltay = y1 - y0;
	float error = 0;
	float deltaerr = abs (deltay / deltax);    // Assume deltax != 0 (line is not vertical),
	// note that this division needs to be done in a way that preserves the fractional part
	int y = y0;
	for (int x=x0; x<x1; x++){
		GLCD_SetPixel(x,y,1);
		error = error + deltaerr;
		while(error >= 0.5){
			GLCD_SetPixel(x,y,1);
			y = y - signbit(y1 - y0);
			error = error - 1.0;
		}
	}
 
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(char charToWrite)
{
	int i;
	charToWrite -= 32; 
	for(i = 0; i < 5; i++) 
		GLCD_WriteData( font5x8[(5 * charToWrite) + i] ); 
	GLCD_WriteData(0x00);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(char * stringToWrite)
{
	while(*stringToWrite)
		GLCD_WriteChar(*stringToWrite++);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
	int idx = (KS0108_SCREEN_WIDTH * (y/8)) + x;
//	framebuffer[idx] = framebuffer[idx] ^= (-color ^ framebuffer[idx]) & (1 << (y%8));
	framebuffer[idx] |= 1 << y%8;

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

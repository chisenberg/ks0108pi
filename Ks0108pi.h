#ifndef __KS0108PI_H_INCLUDED__
#define __KS0108PI_H_INCLUDED__

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

class Ks0108pi
{
private:

	// static ks0108 commands
	static const uint8_t DISPLAY_SET_Y = 0x40;
	static const uint8_t DISPLAY_SET_X = 0xB8;
	static const uint8_t DISPLAY_START_LINE = 0xC0;
	static const uint8_t DISPLAY_ON_CMD = 0x3E;
	static const uint8_t ON = 0x01;
	static const uint8_t OFF = 0x00;

	// pins
	uint8_t PIN_RS;
	uint8_t PIN_EN;
	uint8_t PIN_CS1;
	uint8_t PIN_CS2;
	uint8_t PIN_CS3;
	uint8_t PIN_D0;
	uint8_t PIN_D1;
	uint8_t PIN_D2;
	uint8_t PIN_D3;
	uint8_t PIN_D4;
	uint8_t PIN_D5;
	uint8_t PIN_D6;
	uint8_t PIN_D7;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	uint8_t screen_x, screen_y;
	uint8_t *framebuffer;
	int framebuffer_size;

	// methods
	void goTo(uint8_t , uint8_t );
	void putData(uint8_t);
	void writeData(uint8_t );
	void writeCommand(uint8_t , uint8_t );
	void lcdDelay();
	void enableController(uint8_t);
	void disableController(uint8_t);



public:

	enum DrawStyle
	{
		STYLE_WHITE_BG = 1 << 0,
		STYLE_BLACK_BG = 1 << 1,
		STYLE_GRAY_BG = 1 << 2,
		STYLE_WHITE_BORDER = 1 << 3,
		STYLE_BLACK_BORDER = 1 << 4,
	};

	Ks0108pi();
	int init();
	void clearScreen();
	void clearBuffer();
	void syncBuffer();
	void wait(unsigned int);

	void setPixel(uint8_t , uint8_t);
	void clearPixel(uint8_t , uint8_t);
	void setPixels(uint8_t , uint8_t, uint8_t);

	void drawRect(uint8_t , uint8_t , uint8_t , uint8_t, uint8_t);

	void drawLine(uint8_t , uint8_t , uint8_t , uint8_t);

	void writeChar(uint8_t, uint8_t, char , uint8_t* );
	void writeString(uint8_t, uint8_t, char *, uint8_t* );

	void shiftBufferHorizontal(int);
};

#endif // __KS0108PI_H_INCLUDED__

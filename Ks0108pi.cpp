/*
	Raspberry pi KS0108 LCD interface
	Author: Christian Isenberg

	With help from the following amazing libraries :D :
		- Universal KS0108 driver library by  Rados³aw Kwiecieñ, radek@dxp.pl
		- BCM 2835 library by Mike McCauley
*/

#include "Ks0108pi.h"




//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
Ks0108pi::Ks0108pi(void)
{

}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
int Ks0108pi::init(void)
{
	if (!bcm2835_init()){
		printf("Falha ao iniciar bcm2835...\n");
		return 1;
	}

	// pins
	Ks0108pi::PIN_RS = 7;
	Ks0108pi::PIN_EN = 11;
	Ks0108pi::PIN_CS1 = 25;
	Ks0108pi::PIN_CS2 = 8;
	Ks0108pi::PIN_CS3 = 9;
	Ks0108pi::PIN_D0 = 2;
	Ks0108pi::PIN_D1 = 3;
	Ks0108pi::PIN_D2 = 4;
	Ks0108pi::PIN_D3 = 17;
	Ks0108pi::PIN_D4 = 27;
	Ks0108pi::PIN_D5 = 22;
	Ks0108pi::PIN_D6 = 10;
	Ks0108pi::PIN_D7 = 9;
	// other
	Ks0108pi::screen_x=0;
	Ks0108pi::screen_y=0;
	Ks0108pi::SCREEN_WIDTH = 128;
	Ks0108pi::SCREEN_HEIGHT = 64;

	//sets all pins as output
	bcm2835_gpio_fsel(PIN_EN, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_RS, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_CS1, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_CS2,	BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(PIN_D0, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D1, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D2, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D3, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D4, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D5, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D6, 	BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_D7, 	BCM2835_GPIO_FSEL_OUTP);

	for(int i = 0; i < 3; i++)
		writeCommand((DISPLAY_ON_CMD | ON), i);

	//zera framebuffer
	framebuffer_size = (SCREEN_WIDTH * SCREEN_HEIGHT)/8;
	framebuffer = new uint8_t[framebuffer_size];
	std::fill_n(framebuffer,framebuffer_size, 0);

	printf("Iniciou bcm2835...\n");
	return 0;
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::goTo(uint8_t x, uint8_t y)
{
	uint8_t i;
	screen_x = x;
	screen_y = y;
	for(i = 0; i < SCREEN_WIDTH/64; i++)
	{
		writeCommand(DISPLAY_SET_Y | 0,i);
		writeCommand(DISPLAY_SET_X | y,i);
		writeCommand(DISPLAY_START_LINE | 0,i);
	}
	writeCommand(DISPLAY_SET_Y | (x % 64), (x / 64));
	writeCommand(DISPLAY_SET_X | y, (x / 64));
}

void Ks0108pi::putData(uint8_t data)
{
	bcm2835_gpio_write(PIN_D0, (data >> 0) & 1 ) ;
	bcm2835_gpio_write(PIN_D1, (data >> 1) & 1 ) ;
	bcm2835_gpio_write(PIN_D2, (data >> 2) & 1 ) ;
	bcm2835_gpio_write(PIN_D3, (data >> 3) & 1 ) ;
	bcm2835_gpio_write(PIN_D4, (data >> 4) & 1 ) ;
	bcm2835_gpio_write(PIN_D5, (data >> 5) & 1 ) ;
	bcm2835_gpio_write(PIN_D6, (data >> 6) & 1 ) ;
	bcm2835_gpio_write(PIN_D7, (data >> 7) & 1 ) ;
}

//-------------------------------------------------------------------------------------------------
// Write data to current position
//-------------------------------------------------------------------------------------------------
void Ks0108pi::writeData(uint8_t dataToWrite)
{
	lcdDelay();
	bcm2835_gpio_write(PIN_RS, HIGH);
	putData(dataToWrite);
	enableController(screen_x / 64);
	bcm2835_gpio_write(PIN_EN, HIGH);
	lcdDelay();
	bcm2835_gpio_write(PIN_EN, LOW);
	disableController(screen_x / 64);
	screen_x++;
}

//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------
void Ks0108pi::writeCommand(uint8_t commandToWrite, uint8_t controller)
{
	lcdDelay();
	bcm2835_gpio_write(PIN_RS, LOW);
	enableController(controller);
	putData(commandToWrite);
	bcm2835_gpio_write(PIN_EN, HIGH);
	lcdDelay();
	bcm2835_gpio_write(PIN_EN, LOW);
	disableController(controller);
}

//-------------------------------------------------------------------------------------------------
// lcdDelay function
//-------------------------------------------------------------------------------------------------
void Ks0108pi::lcdDelay(void)
{
	bcm2835_delayMicroseconds(3);
}

//-------------------------------------------------------------------------------------------------
// Enalbe Controller (0-2)
//-------------------------------------------------------------------------------------------------
void Ks0108pi::enableController(uint8_t controller)
{
	switch(controller){
		case 0 : bcm2835_gpio_write(PIN_CS1, HIGH); break;
		case 1 : bcm2835_gpio_write(PIN_CS2, HIGH); break;
		case 2 : bcm2835_gpio_write(PIN_CS3, HIGH); break;
	}
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------
void Ks0108pi::disableController(uint8_t controller)
{
	switch(controller){
		case 0 : bcm2835_gpio_write(PIN_CS1, LOW); break;
		case 1 : bcm2835_gpio_write(PIN_CS2, LOW); break;
		case 2 : bcm2835_gpio_write(PIN_CS3, LOW); break;
	}
}




//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::clearScreen()
{
	clearBuffer();
	syncBuffer();
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::clearBuffer()
{
	std::fill_n(framebuffer,framebuffer_size, 0);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::syncBuffer()
{
	uint8_t i, j;
	int counter = 0;
	for(j = 0; j < 8; j++)
	{
		goTo(0,j);
		for(i = 0; i < SCREEN_WIDTH; i++)
		writeData((uint8_t)framebuffer[counter++]);
	}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::wait(unsigned int millis)
{
	bcm2835_delay(millis);
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::setPixel(uint8_t x, uint8_t y)
{
	int idx = (SCREEN_WIDTH * (y/8)) + x;
	framebuffer[idx] |= 1 << y%8;
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::setPixels(uint8_t x, uint8_t y, uint8_t byte)
{
	int idx = (SCREEN_WIDTH * (y/8)) + x;
	int idx2 = (SCREEN_WIDTH * ( (y/8)+1) ) + x;
	uint8_t rest = y%8;
	framebuffer[idx] |= ( byte << y%8 );
	if(rest)
		framebuffer[idx2] |= byte >> (8-y%8);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h){
	for(int nx=x; nx < x+w ; nx++){
		for(int ny=y; ny < y+h ; ny++){
			setPixel(nx,ny);
		}
	}
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		setPixel(x0,y0);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}

}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::writeChar(uint8_t x, uint8_t y, char charToWrite, uint8_t* font)
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
			setPixels(x+col, ceil(y+line),
				font[sum + col + (int)ceil(charWidth*line/8.0)]
			);
		}
	}

}



//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void Ks0108pi::writeString(uint8_t x, uint8_t y, char * stringToWrite, uint8_t* font)
{
	while(*stringToWrite){
		writeChar(x,y,*stringToWrite++, font);
		x+=font[2]+1;
	}
}

void Ks0108pi::shiftBufferHorizontal(int x)
{
	uint8_t *originalfb = new uint8_t[framebuffer_size];

	//backup of current framebuffer
	std::copy(framebuffer, framebuffer+framebuffer_size, originalfb);
	this->clearBuffer();

	int x_original;
	int x_new;


	// line scan
	for(int y=0; y<SCREEN_HEIGHT/8; y++)
	{
		//x scan
		x_original = x < 0 ? x*-1 : 0;
		x_new = x < 0 ? 0 : x ;
		while(x_original < SCREEN_WIDTH && x_new < SCREEN_WIDTH)
		{
			this->setPixels(x_new, y*8, originalfb[ (y*SCREEN_WIDTH) + x_original ] );
			x_original ++;
			x_new ++;
		}
	}


}

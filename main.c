#include <stdio.h>
#include <stdlib.h>
#include "Ks0108pi.h"
//
#include "fonts/metric01.h"
#include "fonts/metric02.h"
#include "fonts/metric04.h"
//
#include "fonts/pbar.h"
#include "fonts/pbar2.h"

#include "bmps/chuva.h"
#include "fonts/batticon.h"

#include <math.h>
#define PI 3.14159265

// void drawPbar(uint8_t x, uint8_t y, uint8_t percent){
//
// 	char string[30];
// 	int tiles = ((120.0*percent)/100.0)/4;
// 	for(int i=0; i<30; i++){
// 		if 		(i==0)string[0] = tiles < 1 ? PBAR_INI : PBAR_INI_FILL;
// 		else if (i==29)string[29] = tiles < 30 ? PBAR_END : PBAR_END_FILL;
// 		else if (i==tiles) string[i] = PBAR_FILL_LAST;
// 		else string[i] = i <= tiles ? PBAR_FILL : PBAR_EMPTY;
// 	}
// 	for(int i=0; i<30; i++){
// 		GLCD_WriteChar(x,y,string[i], pbar);
// 		x+=pbar[2];
// 	}
// }

int main(int argc, char** argv){
	Ks0108pi *kspi = new Ks0108pi();
	kspi->init();

	for(int i=0; i< 100; i++)
	{
		kspi->clearScreen();
		kspi->writeString(50+i,i/2,"TESTE",metric02);
		kspi->writeChar(84,18,0x00,batticon);
		kspi->syncBuffer();
		bcm2835_delay(500);
	}

	// while(1)
	// {
	// 	GLCD_ClearBuffer();
	// 	GLCD_WriteString(0,0,"TESTE",metric02);
	// 	GLCD_WriteString(60,0,"USADO",metric01);
	// 	GLCD_WriteString(60,7,"HOJE",metric01);
	// 	GLCD_WriteString(90,0,"2.3",metric02);
	// 	GLCD_WriteString(116,7,"kWh",metric01);
	// 	GLCD_WriteString(0,18,"50135",metric04);
	// 	GLCD_WriteString(0,52,"21.5*",metric02);
	// 	GLCD_WriteChar(84,18,0x00,batticon);
	// 	GLCD_SyncBuffer();
	// }
}

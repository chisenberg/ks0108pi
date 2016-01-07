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
void tela(Ks0108pi *kspi)
{
	char string[256];
	kspi->writeString(0,0,"TESTE",metric02);
	kspi->writeString(60,0,"USADO",metric01);
	kspi->writeString(60,7,"HOJE",metric01);
	kspi->writeString(90,0,"2.3",metric02);
	kspi->writeString(116,7,"kWh",metric01);
	sprintf(string,"%d",12);
	kspi->writeString(0,18,string,metric04);
	kspi->writeString(0,52,"21.5*",metric02);
	kspi->writeChar(84,18,0x00,batticon);
}

int main(int argc, char** argv){
	Ks0108pi *kspi = new Ks0108pi();
	kspi->init();

	while(1)
	{
		kspi->clearBuffer();

		//kspi->wait(1500);

		for(int space=5; space>0; space--){
			for(int i=0; i<128; ){
				for(int j=0; j<64; )
				{
					kspi->setPixel(i,j);
					j+=space;
				}
				i+=space;
			}
			kspi->wait(250);
			kspi->syncBuffer();
		}



	}
}

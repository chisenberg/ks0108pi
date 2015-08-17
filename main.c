#include <stdio.h>
#include <bcm2835.h>
#include "KS0108.h" 
#include "mowslogo.h" 
#include "speed.h" 

#include <math.h>
#define PI 3.14159265

int main(int argc, char** argv){
	GLCD_Initalize();
	printf("aqui...\n");
	GLCD_ClearScreen();

	GLCD_ClearBuffer();
	//GLCD_DrawRect(32,32,32,32);
	

int Fs = 128;
int f = 2;
int sample = 128;
float y=0;

int shift=0;
while(1){
	//if(++shift > 128) shift=1;
	GLCD_ClearBuffer();
	GLCD_DrawLine(0,0,128,64);
	for(int x=0; x<sample; x++){
		y = sin(2 * PI * f * x / Fs);
		GLCD_SetPixel(x,32+y*32,1);
		GLCD_SyncBuffer();
		bcm2835_delay(10);
	}
	
}





	
/*	while(1){
		for(int y = 0; y<8; y++){
			GLCD_GoTo(0,y);
			for(int x=0; x<128; x++)
				GLCD_WriteData(0xFF);
		}
		bcm2835_delay(250);
		GLCD_ClearScreen();
		bcm2835_delay(250);
	}
	
	*/

}


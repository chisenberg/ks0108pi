# KS0108 LCD Raspberry Pi
This is a C/C++ library/driver for the popular ks0108 lcd graphic ( 128x64 ) displays.

DISCLAIMER: WORK IN PROGRESS!

 Pin manipulation is done with BCM 2835 library by Mike McCauley.
This is the only aditional library required for this to work.
( more info: http://www.airspayce.com/mikem/bcm2835/ )

The following functions already work but need further testing:
  - Set single pixel
  - Set a byte of vertical pixels
  - Clear screen
  - Draw line
  - Draw filled rect
  - Draw char / string

 Pinout is hardcoded and can be changed inside the Ks0108pi::init() function.
The pin numbers dont represent the board phisical numbering, but the GPIO-pin format
( more info: http://elinux.org/images/5/5c/Pi-GPIO-header.png )

Current values are:

- Ks0108pi::PIN_RS = 7;
- Ks0108pi::PIN_EN = 11;
- Ks0108pi::PIN_CS1 = 25;
- Ks0108pi::PIN_CS2 = 8;
- Ks0108pi::PIN_CS3 = 9;
- Ks0108pi::PIN_D0 = 2;
- Ks0108pi::PIN_D1 = 3;
- Ks0108pi::PIN_D2 = 4;
- Ks0108pi::PIN_D3 = 17;
- Ks0108pi::PIN_D4 = 27;
- Ks0108pi::PIN_D5 = 22;
- Ks0108pi::PIN_D6 = 10;
- Ks0108pi::PIN_D7 = 9;

More observations:	
  - the main.c file contains some examples.
  - just run make inside the folder and you can run the example with "./main"
  - font files are done based on the GLCD font "standard" found on the arduino library.
	
	

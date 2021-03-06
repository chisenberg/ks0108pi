#ifndef PBAR_H
#define PBAR_H

#define PBAR_INI 		0x00
#define PBAR_INI_FILL	0x01
#define PBAR_END 		0x02
#define PBAR_END_FILL	0x03
#define PBAR_EMPTY		0x04
#define PBAR_FILL 		0x05
#define PBAR_FILL_LAST 	0x06


static uint8_t pbar[] = {
	0x0, 0x0,	// size of zero indicates fixed width font
	4,			// width
	8,			// height
	0x00,		// first char
	6,			// char count
	
	0x3c, 0x42, 0x81, 0x81, 	// 0x00 start
	0x3c, 0x42, 0x99, 0xbd, 	// 0x01 start fill
	0x81, 0x81, 0x42, 0x3c, 	// 0x02 end
	0xbd, 0x99, 0x42, 0x3c, 	// 0x03 end fill
	0x81, 0x81, 0x81, 0x81, 	// 0x04 empty
	0xbd, 0xbd, 0xbd, 0xbd,  	// 0x05 fill
	0xbd, 0xbd, 0x99, 0x81, 	// 0x06 fill last
 };
#endif


#include <stdint.h>

#define DISPLAY_SET_Y       0x40
#define DISPLAY_SET_X       0xB8
#define DISPLAY_START_LINE  0xC0
#define DISPLAY_ON_CMD		0x3E
  #define ON	0x01
  #define OFF	0x00
#define DISPLAY_STATUS_BUSY	0x80

void GLCD_Initalize(void);
int GLCD_InitalizePorts(void);

void GLCD_WriteData(uint8_t );
void GLCD_WriteCommand(uint8_t , uint8_t );
void GLCD_ClearScreen(void);
void GLCD_GoTo(uint8_t, uint8_t);

void GLCD_ClearBuffer();
void GLCD_SyncBuffer();

void GLCD_Bitmap(uint8_t *);
void GLCD_SetPixel(uint8_t , uint8_t , uint8_t );
void GLCD_DrawRect(uint8_t, uint8_t, uint8_t, uint8_t);
void GLCD_DrawLine(uint8_t , uint8_t , uint8_t , uint8_t);

void GLCD_WriteString(char *);
void GLCD_WriteChar(char );



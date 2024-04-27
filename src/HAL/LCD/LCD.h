#ifndef LCD_H_
#define LCD_H_

#include "STD_TYPES.h"
#include "port.h"

typedef struct {
	void * port;
	uint32_t pin;
}LCD_pin_Cfg;

typedef enum{
		D0_PIN,
		D1_PIN,
		D2_PIN,
		D3_PIN,
		D4_PIN,
		D5_PIN,
		D6_PIN,
		D7_PIN,
		RS_PIN,
		RW_PIN,
		EN_PIN,
		NUMBER_OF_LCD_PINS
	}LCD_eDataPins;


void LCD_Init_Asynch();

void LCD_Write_String_NoCopy(uint8_t * str,uint32_t length);

void LCD_Wrtite_Number_Asynch(uint32_t Copy_number);

void LCD_Clear_Display_Asynch();

void LCD_Set_Cursor_Asynch(uint8_t Copy_u8Row, uint8_t Copy_u8Column);

//void LCD_Write_Command_NoCopy(uint8_t Command);

#endif

#ifndef _KEYPAD_CFG_H_
#define _KEYPAD_CFG_H_


#include "std_types.h"


#define NO_OF_KEYPADS           1
#define FIRST_KEYPAD            0

typedef enum
{
    Row0=0,
    Row1,
    Row2,
    /*No OF Desired rows*/
    No_of_Desired_Rows
}Keypad_Rows_t;


typedef enum
{
    Column0=0,
    Column1,
    Column2,
    /*No OF Desired rows*/
    No_of_Desired_Columns
}Keypad_Columns_t;


typedef struct
{
	u32 PIN;
	void* PORT;
}KeyPad_Connection_CFG_t;


typedef struct
{
    KeyPad_Connection_CFG_t KeyPad_Rows_Connection_CFG[No_of_Desired_Rows];
    u32 ROWS_Default_state;
    KeyPad_Connection_CFG_t KeyPad_Columns_Connection_CFG[No_of_Desired_Columns];
    u32 Columns_Default_state;
    u32 ROWS_Column_Output_state;//basedon ur keypad configuration

}KeyPad_CFG_t;


#endif

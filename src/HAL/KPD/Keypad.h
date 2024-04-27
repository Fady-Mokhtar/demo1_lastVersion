
#ifndef _KEYPAD_H_
#define _KEYPAD_H_


//#include"HAL/LED/led.h"
#include"usart.h"
//#include"LCD.h"
//#include"dma.h"


#define IDLE_MESSAGE    0X01
#define START_OF_FRAME  0x55

#define CHANGE_MODE     0x10
#define EDIT_MODE       0x20
#define START           0x30
#define STOP            0x40
#define LEFT            0x50
#define RIGHT           0x60
#define INCREMENT       0x70
#define DECREMENT       0x80
#define OK              0x90



extern USART_ManagerStruct usart1Manager;
// extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
// extern DMA_HandleTypeDef dma2Manager_stream7_usart1_tx;


typedef enum
{
    KEYPAD_OK,
    KEYPAD_NOK,
    KEYPAD_NULLPTR
}Keypad_Errorstates_t;

void keypad_runnable(void);


#endif

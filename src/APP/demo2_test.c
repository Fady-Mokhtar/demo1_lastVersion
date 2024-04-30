/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#define demo2_test
#ifdef demo2_test

/* Includes ------------------------------------------------------------------*/
#include "std_types.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "Sched.h"
#include "nvic.h"
#include "led.h"
#include "lcd.h"
#include "keypad.h"
#include "Systick.h"
#include <string.h>


/********************************************************************************************************/
/************************************************Defines************************************************/
/********************************************************************************************************/

//-----------init configuration sets------
extern GPIO_StrCfg_t Loc_DMAGpios[__NUM_OF_PINS_DMA];

//------------managers & handlers----------
extern USART_ManagerStruct usart1Manager;

//------------global variables----------

const uint8_t ledon[8] = {'L', 'e', 'd', 'i', 's', '_', 'O', 'n'};
const uint8_t ledoff[8] = {'L', 'e', 'd', 'i', 's', 'O', 'f', 'f'};
const uint8_t error[8] = {'E', 'r', 'r', 'o', 'r', '!', '!', '!'};

char rx_data[8] = {0};

void HAL_UART_RxCpltCallback(USART_ManagerStruct *usart1Manager)
{
    if (strcmp((char *)rx_data, "switchon") == 0)
    {
        LED_SetStatus(LED_1, LED_ON);
        USART_startTransmit_IT(&usart1Manager, ledon, 0x08);
    }
    else if (strcmp((char *)rx_data, "switchof") == 0)
    {
        LED_SetStatus(LED_1, LED_OFF);
        USART_startTransmit_IT(&usart1Manager, ledoff, 0x08);
    }
    else
    {
        LED_SetStatus(LED_1, LED_OFF);
        USART_startTransmit_IT(&usart1Manager, error, 0x08);
    }
    
    //USART_startReceive_IT(&usart1Manager, rx_data, 8); // Restart UART receive
}


int main(void)
{

    Enable_Interrupts();

    /* Initialize all configured peripherals */
    RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_GPIOA);
    RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_GPIOB);

    GPIO_Init(Loc_DMAGpios, __NUM_OF_PINS_DMA);
    LED_Init();
    USART_Init(&usart1Manager);
    
    
    while (1)
    {
        USART_startReceive_IT(&usart1Manager, rx_data, 8); // Start UART receive in interrupt mode

    }

    return 0;
}

#endif
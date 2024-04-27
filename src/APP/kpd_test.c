/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#define kpd_test
#ifdef kpd_test

/* Includes ------------------------------------------------------------------*/
#include "std_types.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "Sched.h"
#include "nvic.h"
#include "lcd.h"
#include "keypad.h"
#include "Systick.h"


/********************************************************************************************************/
/************************************************Defines************************************************/
/********************************************************************************************************/

//-----------init configuration sets------
extern GPIO_StrCfg_t Loc_DMAGpios[__NUM_OF_PINS_DMA];

// keypad
extern volatile uint8_t UART_Buffer[2];
extern volatile u8 UARTFARME[2];

//------------managers & handlers----------
extern USART_ManagerStruct usart1Manager;
extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
extern DMA_HandleTypeDef dma2Manager_stream7_usart1_tx;

//------------global variables----------

#define receive_state 0x00
#define transmit_state 0x01

static uint8_t req_state = receive_state;

void Uart_runnable(void)
{
    switch (req_state)
		{
			case receive_state:
                req_state = transmit_state;
				USART_startReceive_IT(&usart1Manager, &UART_Buffer, (uint16_t)0x02);
				break;
            case transmit_state:
                req_state = receive_state;
				USART_startTransmit_IT(&usart1Manager, &UARTFARME, (uint16_t)0x02);
				break;
			
			default:
				break;
		}
    
    
}

void Uart_receive_runnable()
{

    //USART_startReceive_IT(&usart1Manager, &UART_Buffer, (uint16_t)0x02);
    USART_Receive_DMA(&usart1Manager, &UART_Buffer, (uint16_t)0x02);
}

void Uart_transmit_runnable()
{

    //USART_startTransmit_IT(&usart1Manager, &UARTFARME, (uint16_t)0x02);
    USART_Transmit_DMA(&usart1Manager, &UARTFARME, (uint16_t)0x02);
}

int main(void)
{

    /* Initialize all configured peripherals */
    GPIO_Init(Loc_DMAGpios, __NUM_OF_PINS_DMA);
    Enable_Interrupts();
    
    RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_GPIOA);
    RCC_enableAHB1Peripheral(RCC_AHB1PERIPHERAL_GPIOB);
    USART_Init(&usart1Manager);


    DMA_Init(&dma2Manager_stream2_usart1_rx);
    DMA_Init(&dma2Manager_stream7_usart1_tx);

    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    
    LCD_Init_Asynch();
    Sched_Init();
    Sched_Start();
    while (1)
    {
        
    }
    

    
    


    return 0;
}

#endif
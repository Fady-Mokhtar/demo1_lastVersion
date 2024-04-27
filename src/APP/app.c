/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

//#define demo_test
#ifdef demo_test

/* Includes ------------------------------------------------------------------*/
#include "std_types.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "stk.h"
#include "nvic.h"
//#include "lcd.h"
#include "keypad.h"
#include "Schedular.h"
#include "MCAL/PORT/port.h"
/********************************************************************************************************/
/************************************************Defines************************************************/
/********************************************************************************************************/
// #define Start_Of_Frame  0x55

// #define Change_Mode     0x10
// #define Edit_Mode       0x20
// #define Start           0x30
// #define Stop            0x40
// #define Left            0x50
// #define Right           0x60
// #define Increment       0x70
// #define Decrement       0x80
// #define Ok              0x90

//-----------init configuration sets------
extern GPIO_StrCfg_t Loc_arrStrGpios[__NUM_OF_PINS_DEMO];
extern GPIO_StrCfg_t Loc_DMAGpios[__NUM_OF_PINS_DMA];

// keypad
extern volatile uint8_t UARTFARME[2];
// lcd
extern volatile  uint8_t UART_Buffer[2];
//------------managers & handlers----------
extern USART_ManagerStruct usart1Manager;
extern DMA_HandleTypeDef dma2Manager_stream2_usart1_rx;
extern DMA_HandleTypeDef dma2Manager_stream7_usart1_tx;


//------------global variables----------
uint8_t g_lcdCommand = 0;

void Uart_receive_runnable()
{
    USART_Receive_DMA(&usart1Manager, &UART_Buffer, (uint16_t)0x02);
    //for(int i=0; i<800000; i++);
}

void Uart_transmit_runnable()
{
    if (UARTFARME[1] != IDLE_MESSAGE)
    {
        //for(int i=0; i<200000; i++);
        USART_Transmit_DMA(&usart1Manager, &UARTFARME, (uint16_t)0x02);
    }
}

int main(void)
{

    /* Initialize all configured peripherals */
    //GPIO_Init(Loc_arrStrGpios, __NUM_OF_PINS_DEMO);
    GPIO_Init(Loc_DMAGpios, __NUM_OF_PINS_DMA);
   // LCD_Init_Asynch();
    Enable_Interrupts();

    USART_Init(&usart1Manager);

    DMA_Init(&dma2Manager_stream2_usart1_rx);
    DMA_Init(&dma2Manager_stream7_usart1_tx);

    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);

    sched_init();
    sched_start();
    while (1)
    {
        
    }
    

    // transmit
    //uint8_t tx_frame[2] = {Start_Of_Frame, Change_Mode};
    for(int i=0; i<200000; i++);
    USART_Transmit_DMA(&usart1Manager, &UARTFARME, (uint16_t)0x02);
    // while(1)
    // {
    
    // }
    // receive
    //uint8_t rx_frame[2] = {0};
    USART_Receive_DMA(&usart1Manager, &UART_Buffer, (uint16_t)0x02);
    for(int i=0; i<800000; i++);
    
    // while (1)
    // {
    //     if (rx_frame[0] == Start_Of_Frame)
    //     {
    //         switch (rx_frame[1])
    //         {
    //         case Change_Mode:
    //             // change mode
    //             g_lcdCommand = Change_Mode;
    //             break;
    //         case Edit_Mode:
    //             // edit mode
    //             g_lcdCommand = Edit_Mode;
    //             break;
    //         case Start:
    //             // start
    //             g_lcdCommand = Start;
    //             break;
    //         case Stop:
    //             // stop
    //             g_lcdCommand = Stop;
    //             break;
    //         case Left:
    //             // left
    //             g_lcdCommand = Left;
    //             break;
    //         case Right:
    //             // right
    //             g_lcdCommand = Right;
    //             break;
    //         case Increment:
    //             // increment
    //             g_lcdCommand = Increment;
    //             break;
    //         case Decrement:
    //             // decrement
    //             g_lcdCommand = Decrement;
    //             break;
    //         case Ok:
    //             // ok
    //             break;

    //         default:
    //             while(1)
    //             {
    //                 // wrong input
    //             }
    //             break;
    //         }
    //         // success
    //         while (1)
    //         {

    //         }
    //     }
    // }

    return 0;
}

#endif
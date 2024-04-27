#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include "Systick_cfg.h"


#define SYSTICK_MODE_PERIODIC               0  
#define SYSTICK_MODE_ONETIME                1
#define SYSTICK_CLK_SOURCE_AHB              0x4
#define SYSTICK_CLK_SOURCE_AHB_DIV_8_       0x0
#define SYSTICK_ASSERT_ON                   0x2
#define SYSTICK_ASSERT_OFF                  0x0

typedef void (*SYSTICKCB_t)(void);
typedef enum
{
    systick_OK=0,
    systick_NOK
}STK_ErrorState_t;

STK_ErrorState_t SYSTICK_SetConfig(u8 SYSTICK_MODE,u8 SYSTICK_CLK_SOURCE,u8 SYSTICK_ASSERT_CFG);
STK_ErrorState_t SYSTICK_SetTimeMS(u32 Copy_of_time);
STK_ErrorState_t SYSTICK_start();
STK_ErrorState_t SYSTICK_Stop();
STK_ErrorState_t SYSTICK_setCallBackFN(SYSTICKCB_t CB);



#endif
#ifndef _PORT_H_
#define _PORT_H_
#include "std_types.h"


#define GPIO_PIN0 	0x00000000
#define GPIO_PIN1 	0x00000001
#define GPIO_PIN2 	0x00000002
#define GPIO_PIN3 	0x00000003
#define GPIO_PIN4 	0x00000004
#define GPIO_PIN5 	0x00000005
#define GPIO_PIN6 	0x00000006
#define GPIO_PIN7 	0x00000007
#define GPIO_PIN8 	0x00000008
#define GPIO_PIN9	0x00000009
#define GPIO_PIN10 	0x0000000A
#define GPIO_PIN11 	0x0000000B
#define GPIO_PIN12 	0x0000000C
#define GPIO_PIN13 	0x0000000D
#define GPIO_PIN14 	0x0000000E
#define GPIO_PIN15 	0x0000000F

#define GPIOA_BASEADD  (void*)(0x40020000)
#define GPIOB_BASEADD  (void*)(0x40020400)
#define GPIOC_BASEADD  (void*)(0x40020800)
#define GPIOD_BASEADD  (void*)(0x40020C00)
#define GPIOE_BASEADD  (void*)(0x40021000)
#define GPIOH_BASEADD  (void*)(0x40021C00)

#define GPIO_INPUT_FL					0x00000000
#define GPIO_INPUT_PU					0x00000008
#define GPIO_INPUT_PD					0x00000010

#define GPIO_OUTPUT_PP 					0x00000001
#define GPIO_OUTPUT_PP_PU				0x00000009
#define GPIO_OUTPUT_PP_PD				0x00000011
#define GPIO_OUTPUT_OD					0x00000005
#define GPIO_OUTPUT_OD_PU				0x0000000D
#define GPIO_OUTPUT_OD_PD				0x00000015

#define GPIO_ANALOG 					0x00000003

#define GPIO_AF_PP 						0x00000002
#define GPIO_AF_PP_PU					0x0000000A
#define GPIO_AF_PP_PD					0x00000012
#define GPIO_AF_OD						0x00000006
#define GPIO_AF_OD_PU					0x0000000E
#define GPIO_AF_OD_PD					0x00000016

#define GPIO_SET_PIN 					0x00000001
#define GPIO_RESET_PIN 					0x00010000
/********************Macros for the SPEED********************/
#define GPIO_LOW_SPEED			0x00000000
#define GPIO_MEDIUM_SPEED		0x00000001
#define GPIO_HIGH_SPEED			0x00000002
#define GPIO_VERY_HIGH_SPEED	0x00000003

#define GPIO_HIGH               0x00000001
#define GPIO_LOW                0x00000000

#define BSSR_BIT_VALUE          0x00000001
#define BSSR_OFFSET             0x00000010
typedef struct
{

	u32       Pin;
	void* 	  Port;
	u32       Speed;
	u32       Mode;

}GPIO_PinConfig_t;

typedef enum
{
	GPIO_enuOk ,
	GPIO_enuNOk,
	GPIO_NULLPOINTER,
	GPIO_WRONGPortORPin,
	GPIO_WRONGStatus
}GPIO_enuErrorStatus_t;


GPIO_enuErrorStatus_t GPIO_InitPin (GPIO_PinConfig_t *ptrToPinConfig);
GPIO_enuErrorStatus_t GPIO_Set_PinValue(void *Port , u32 PinNum , u32 PinStatus );
GPIO_enuErrorStatus_t GPIO_GetPinValue(void *Port , u32 PinNum,  u32 * PinStatus) ;
 //GPIO_enuErrorStatus_t GPIO_GetPinValue(void *Port , u8 connection,u32 PinNum,  u32 * PinStatus);


#endif

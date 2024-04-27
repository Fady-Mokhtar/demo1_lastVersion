
#ifndef LED_H_
#define LED_H_

#include "std_types.h"
#include "LED_Cfg.h"

#define LED_ON            0b1
#define LED_OFF           0b0

#define LED_FORWARD       0x00000000
#define LED_REVERSE       0x00000001


typedef struct
{
	u32 PIN;
	void* PORT;
	u32 Connection;
	u32 Default_state;

}LED_CFG_t;

typedef enum

{
	LED_enuOK,
	LED_enuNOK


}LED_enuErrorStatus_t;


LED_enuErrorStatus_t LED_Init(void);


LED_enuErrorStatus_t LED_SetStatus(LEDS_t LED, u32 LEDStatus);

void LED_Toggle();


#endif /* LED_H_ */
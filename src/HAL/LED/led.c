
#include "LED.h"
#include "port.h"

extern LED_CFG_t LEDS [_LED_NUM];
u8 state_1=LED_ON;
u8 state_2=LED_ON;



LED_enuErrorStatus_t LED_Init(void)

{
	LED_enuErrorStatus_t Ret_enuErrorStatusLED = LED_enuOK;

	GPIO_PinConfig_t Led;
	Led.Mode=GPIO_OUTPUT_PP;
	Led.Speed=GPIO_HIGH_SPEED;
	u8 Loc_iter=0;

	for (Loc_iter=0;Loc_iter<_LED_NUM;Loc_iter++)
	{
		Led.Pin=LEDS[Loc_iter].PIN;
		Led.Port=LEDS[Loc_iter].PORT;
		GPIO_InitPin(&Led);
		GPIO_Set_PinValue(LEDS[Loc_iter].PORT, LEDS[Loc_iter].PIN, (LEDS[Loc_iter].Connection)^(LEDS[Loc_iter].Default_state));

	}

	return Ret_enuErrorStatusLED;


}



LED_enuErrorStatus_t LED_SetStatus(LEDS_t LED, u32 LEDStatus)
{
	LED_enuErrorStatus_t Ret_enuErrorStatusLED = LED_enuOK;

	GPIO_Set_PinValue(LEDS[LED].PORT, LEDS[LED].PIN, (LEDS[LED].Connection)^LEDStatus);

return Ret_enuErrorStatusLED;
}


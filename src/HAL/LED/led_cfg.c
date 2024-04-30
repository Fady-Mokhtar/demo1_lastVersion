

#include "port.h"
#include "LED.h"

LED_CFG_t LEDS [_LED_NUM]=

{
		[LED_1]= {
				.PIN=GPIO_PIN0,
				.PORT=GPIOB_BASEADD,
				.Connection=LED_FORWARD,
				.Default_state=LED_OFF
		}

};
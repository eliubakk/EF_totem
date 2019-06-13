/*
 * Driver for WS2812B LEDs on the Raspberry Pi 2
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-10
 *
 */

#include "WS2812B_LED_Drv.h"

void send_data(uint8_t pin, uint32_t data) {
	uint8_t bit_count;
	//volatile uint32_t timer_count;

	GPIO_clear(pin);
	
	for(bit_count = 0; bit_count < 24; ++bit_count) {
		if(data & 1) {
			//Set value to be loaded into timer after time high is finished
			//PUT32(ARM_TIMER_RLD, LED_ONE_LOW);

			//Set pin
			GPIO_set(pin);
			
			//Clear interrupt and start timer
			PUT32(ARM_TIMER_CLI, 0);
			PUT32(ARM_TIMER_LOD, LED_ONE_HIGH-1);
			//timer_count = SYSTIMER_COUNT;
			//SYSTIMER_C_SET(0, timer_count + LED_ONE_HIGH);
			//SYSTIMER_MATCH_CLR(0);

			//Waiting for timer to finish
			while(1) if(GET32(ARM_TIMER_RIS)) break;
			//while(1) if(SYSTIMER_MATCH(0)) break;

			//Clear pin
			GPIO_clear(pin);

			//Clear interrupt and start timer
			PUT32(ARM_TIMER_CLI, 0);
			PUT32(ARM_TIMER_LOD, LED_ONE_LOW-1);
			//timer_count = SYSTIMER_COUNT;
			//SYSTIMER_C_SET(0, timer_count + LED_ONE_LOW);
			//SYSTIMER_MATCH_CLR(0);

			//Waiting for timer to finish
			while(1) if(GET32(ARM_TIMER_RIS)) break;
			//while(1) if(SYSTIMER_MATCH(0)) break;
		} else {
			//Set value to be loaded into timer after time high is finished
			//PUT32(ARM_TIMER_RLD, LED_ZERO_LOW);

			//Set pin
			GPIO_set(pin);

			//Clear interrupt and start timer
			PUT32(ARM_TIMER_CLI, 0);
			PUT32(ARM_TIMER_LOD, LED_ZERO_HIGH-1);
			//timer_count = SYSTIMER_COUNT;
			//SYSTIMER_C_SET(0, timer_count + LED_ZERO_HIGH);
			//SYSTIMER_MATCH_CLR(0);

			//Waiting for timer to finish
			while(1) if(GET32(ARM_TIMER_RIS)) break;
			//while(1) if(SYSTIMER_MATCH(0)) break;

			//Clear pin
			GPIO_clear(pin);

			//Clear interrupt and start timer
			PUT32(ARM_TIMER_CLI, 0);
			PUT32(ARM_TIMER_LOD, LED_ZERO_LOW-1);
			//timer_count = SYSTIMER_COUNT;
			//SYSTIMER_C_SET(0, timer_count + LED_ZERO_LOW);
			//SYSTIMER_MATCH_CLR(0);

			//Waiting for timer to finish
			while(1) if(GET32(ARM_TIMER_RIS)) break;
			//while(1) if(SYSTIMER_MATCH(0)) break;
		}
		data = data >> 1;
	}
}
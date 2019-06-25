/*
 * For use with the BCM2836 Broadcom chip on the Raspberry Pi 2
 *
 * Functions for initializing and using the PWM peripheral
 *
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-18
 *
 */

#include "pwm.h"
#include "BCM2836_HW.h"

uint8_t pwm_serial_init(uint32_t freq, uint8_t channel, uint8_t pin) {
	uint32_t div;

	/* Configure GPIO */
	if(pwm_GPIO_init(channel, pin) != 0) {
		return 1;
	}

	/* Turn off PWM if already init */
	pwm_deinit();

	/* Set up PWM clock */
	div = 8; //OSC_FREQ / (3 * freq);
	CM_CLK_PWMDIV = CM_CLK_DIV_PASSWD | CM_CLK_DIV_DIVI(div);
	CM_CLK_PWMCTL = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC;
	CM_CLK_PWMCTL = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC | CM_CLK_CTL_ENAB;
	usleep(10);

	while(1) if(CM_CLK_PWMCTL & CM_CLK_CTL_BUSY) break;

	/* Set up PWM module for serial data transfer */
	PWM_RNG1 = 32;
	usleep(10);
	PWM_CTL = PWM_CTL_CLRF1;
	usleep(10);
	switch(channel){
		case 0: {
			PWM_CTL = PWM_CTL_USEF1 | PWM_CTL_MODE1;
			usleep(10);
			PWM_CTL |= PWM_CTL_PWEN1;
			break;
		}
		case 1: {
			PWM_CTL = PWM_CTL_USEF2 | PWM_CTL_MODE2;
			usleep(10);
			PWM_CTL |= PWM_CTL_PWEN2;
			break;
		}
		default:
			return 1;
	};

	return 0;
}

void pwm_deinit( void ) {
	/* Turn off PWM */
	PWM_CTL = 0;
	usleep(10);

	/* Kill PWM clock */
	CM_CLK_PWMCTL = CM_CLK_CTL_PASSWD | CM_CLK_CTL_KILL;
	usleep(10);
	while(1) if(!(CM_CLK_PWMCTL & CM_CLK_CTL_BUSY)) break;
}

/* 	Configure pwm output pin
	returns: 0 - Success 
			 1 - Bad channel
			 2 - Bad pin */
uint8_t pwm_GPIO_init(uint8_t channel, uint8_t pin) {
	uint8_t i;
	pin_config_t pwm_pins[2][4];
	pwm_pins[0][0] = (pin_config_t){.pin = 12, .alt = GPIO_ALT0};
	pwm_pins[0][1] = (pin_config_t){.pin = 18, .alt = GPIO_ALT5};
	pwm_pins[0][2] = (pin_config_t){.pin = 40, .alt = GPIO_ALT0};
	pwm_pins[0][3] = (pin_config_t){.pin = 52, .alt = GPIO_ALT1};
	pwm_pins[1][0] = (pin_config_t){.pin = 13, .alt = GPIO_ALT0};
	pwm_pins[1][1] = (pin_config_t){.pin = 19, .alt = GPIO_ALT5};
	pwm_pins[1][2] = (pin_config_t){.pin = 41, .alt = GPIO_ALT0};
	pwm_pins[1][3] = (pin_config_t){.pin = 45, .alt = GPIO_ALT0};

	/* Check if channel is allowed */
	if(channel > 1) {
		return 1;
	}

	/* Check if pin is allowed for given channel, and configure GPIO */
	for(i = 0; i < 4; ++i) {
		if(pwm_pins[channel][i].pin == pin){
			GPIO_configure(pwm_pins[channel][i]);
			return 0;
		}
	}

	/* Pin not found for channel */
	return 2;
}

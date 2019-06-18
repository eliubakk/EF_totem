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

uint8_t pwm_serial_init(uint32_t freq, uint8_t channel) {

	PWM_RNG1 = 32;
	PWM_CTL = PWM_CTL_CLRF1;
	switch(channel){
		case 0: {
			PWM_CTL = PWM_CTL_USEF1 | PWM_CTL_MODE1;
			PWM_CTL |= PWM_CTL_PWEN1;
		}
		case 1: {
			PWM_CTL = PWM_CTL_USEF2 | PWM_CTL_MODE2;
			PWM_CTL |= PWM_CTL_PWEN2;
		}
	}

	return 0;
}
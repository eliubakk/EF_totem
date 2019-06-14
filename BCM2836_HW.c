/*
 * Peripheral srf defines and macros for BCM2836 Broadcom chip
 * for the Raspberry Pi 2
 *
 * Functions for initiallizing and using peripherals
 *
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-10
 *
 */

#include "BCM2836_HW.h"


void GPIO_configure(uint8_t pin, uint8_t mode) {
	unsigned int reg;

	reg = GPFSEL(pin / 10);

	/* clear mode bits */
	reg &= ~(7 << (3 * (pin % 10))); 

	/* set mode bits */
    reg |= (mode << (3 * (pin % 10)));

    GPFSEL(pin / 10) = reg;
}

void GPIO_set(uint8_t pin) {
	if(pin < 32) {
		GPSET0 = (1 << pin);
	} else {
		GPSET1 = (1 << (pin - 32));
	}
}

void GPIO_clear(uint8_t pin) {
	if(pin < 32) {
		GPCLR0 = (1 << pin);
	} else {
		GPCLR1 = (1 << (pin - 32));
	}
}

uint8_t GPIO_level(uint8_t pin) {
	if(pin < 32) {
		return (1 & (GPLEV0 >> pin));
	} else {
		return (1 & (GPLEV1 >> (pin - 32)));
	}
}

void GPIO_toggle(uint8_t pin) {
	if(GPIO_level(pin)){
		GPIO_clear(pin);
	} else {
		GPIO_set(pin);
	}
}


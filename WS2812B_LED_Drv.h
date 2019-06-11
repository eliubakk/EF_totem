/*
 * Driver for WS2812B LEDs on the Raspberry Pi 2
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-10
 *
 */

#ifndef __WS2812B_LED_DRV_H__
#define __WS2812B_LED_DRV_H__

#include "BCM2836_HW.h"

#define LED_FREQ_HZ		800000
#define LED_ZERO_HIGH	100		/* 0.4us / 0.004us = 100 clock periods */
#define LED_ONE_HIGH	200		/* 0.8us / 0.004us = 200 clock periods */
#define LED_ZERO_LOW	212		/* 0.85us / 0.004us = 212.5 clock periods */
#define LED_ONE_LOW		112		/* 0.45us / 0.004us = 112.5 clock periods */
#define LED_RESET		12500	/* low more than 50us, 50us / 0.004us = 12500 clock periods */

#endif
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
#define LED_ZERO_HIGH	70 //250000000		/* 0.4us / 0.004us = 100 clock periods, 400ns / 4ns = 100 clock periods */
#define LED_ONE_HIGH	200 //500000000		/* 0.8us / 0.004us = 200 clock periods, 800ns / 4ns = 200 clock periods */
#define LED_ZERO_LOW	212 //250000000		/* 0.85us / 0.004us = 212.5 clock periods, 850ns / 4ns = 212.5 clock periods */
#define LED_ONE_LOW		112 //500000000		/* 0.45us / 0.004us = 112.5 clock periods, 450ns / 4ns = 112.5 clock periods */
#define LED_RESET		12500	/* low more than 50us, 50us / 0.004us = 12500 clock periods */

#define DATA_MASK_B0 	0x00800000
#define DATA_MASK_B1 	0x00400000
#define DATA_MASK_B2 	0x00200000
#define DATA_MASK_B3 	0x00100000
#define DATA_MASK_B4 	0x00080000
#define DATA_MASK_B5 	0x00040000
#define DATA_MASK_B6 	0x00020000
#define DATA_MASK_B7 	0x00010000
#define DATA_MASK_B8 	0x00008000
#define DATA_MASK_B9 	0x00004000
#define DATA_MASK_B10 	0x00002000
#define DATA_MASK_B11 	0x00001000
#define DATA_MASK_B12 	0x00000800
#define DATA_MASK_B13 	0x00000400
#define DATA_MASK_B14 	0x00000200
#define DATA_MASK_B15 	0x00000100
#define DATA_MASK_B16 	0x00000080
#define DATA_MASK_B17 	0x00000040
#define DATA_MASK_B18 	0x00000020
#define DATA_MASK_B19 	0x00000010
#define DATA_MASK_B20 	0x00000008
#define DATA_MASK_B21 	0x00000004
#define DATA_MASK_B22 	0x00000002
#define DATA_MASK_B23 	0x00000001

#define COLOR_BLUE 		0x00000020
#define COLOR_YELLOW	0x0083A400
#define COLOR_GREEN		0x00200000
#define COLOR_RED		0x00002000

#define HIGH_PIN_SEND_ONE(mask) ({ \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ONE_HIGH); \
	GPSET1 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR1 = mask; break; }\
	ARM_TIMER_LOD = (LED_ONE_LOW); \
	ARM_TIMER_CLI = 0; \
	while(1) if(ARM_TIMER_RIS) break; \
})

#define LOW_PIN_SEND_ONE(mask) ({ \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ONE_HIGH); \
	GPSET0 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR0 = mask; break; } \
	ARM_TIMER_LOD = (LED_ONE_LOW); \
	ARM_TIMER_CLI = 0; \
	while(1) if(ARM_TIMER_RIS) break; \
})

#define HIGH_PIN_SEND_ZERO(mask) ({ \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ZERO_HIGH); \
	GPSET1 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR1 = mask; break; } \
	ARM_TIMER_LOD = (LED_ZERO_LOW); \
	ARM_TIMER_CLI = 0; \
	while(1) if(ARM_TIMER_RIS) break; \
})

#define LOW_PIN_SEND_ZERO(mask) ({ \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ZERO_HIGH); \
	GPSET0 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR0 = mask; break; } \
	ARM_TIMER_LOD = (LED_ZERO_LOW); \
	ARM_TIMER_CLI = 0; \
	while(1) if(ARM_TIMER_RIS) break; \
})

void send_data(uint8_t pin, uint32_t data);

#endif
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

#define LED_FREQ_MHZ	0.8
#define LED_ZERO_HIGH	15 //250000000		/* 0.4us / 0.004us = 100 clock periods, 400ns / 4ns = 100 clock periods */
#define LED_ONE_HIGH	170 //500000000		/* 0.8us / 0.004us = 200 clock periods, 800ns / 4ns = 200 clock periods */
#define LED_ZERO_LOW	150 //250000000		/* 0.85us / 0.004us = 212.5 clock periods, 850ns / 4ns = 212.5 clock periods */
#define LED_ONE_LOW		40 //500000000		/* 0.45us / 0.004us = 112.5 clock periods, 450ns / 4ns = 112.5 clock periods */
#define LED_RESET_US	55	/* low more than 50us */
#define LED_COLOR_BITS	24  /* Three colors, 8 bits each */

// PWM Symbol definitions
#define PWM_ONE		0x6  /* 1 1 0 */
#define PWM_ZERO	0x4  /* 1 0 0 */

#define NUM_LED 1

// #define DATA_MASK_B0 	0x00800000
// #define DATA_MASK_B1 	0x00400000
// #define DATA_MASK_B2 	0x00200000
// #define DATA_MASK_B3 	0x00100000
// #define DATA_MASK_B4 	0x00080000
// #define DATA_MASK_B5 	0x00040000
// #define DATA_MASK_B6 	0x00020000
// #define DATA_MASK_B7 	0x00010000
// #define DATA_MASK_B8 	0x00008000
// #define DATA_MASK_B9 	0x00004000
// #define DATA_MASK_B10 	0x00002000
// #define DATA_MASK_B11 	0x00001000
// #define DATA_MASK_B12 	0x00000800
// #define DATA_MASK_B13 	0x00000400
// #define DATA_MASK_B14 	0x00000200
// #define DATA_MASK_B15 	0x00000100
// #define DATA_MASK_B16 	0x00000080
// #define DATA_MASK_B17 	0x00000040
// #define DATA_MASK_B18 	0x00000020
// #define DATA_MASK_B19 	0x00000010
// #define DATA_MASK_B20 	0x00000008
// #define DATA_MASK_B21 	0x00000004
// #define DATA_MASK_B22 	0x00000002
// #define DATA_MASK_B23 	0x00000001

#define NP_COLOUR_BLUE 0x00200088
#define NP_COLOUR_YELLOW 0x0083A400
#define NP_COLOUR_GREEN 0x00901030
#define NP_COLOUR_RED 0x00209925

#define COLOR_BLUE 		0x00000020
#define COLOR_YELLOW	0x0083A400
#define COLOR_GREEN		0x00200000
#define COLOR_RED		0x00002000

#define HIGH_PIN_SEND_ONE(mask) ({ \
	ARM_TIMER_CTL = 0x003E0002; \
	ARM_TIMER_LOD = (LED_ONE_HIGH); \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_CTL = 0x003E0082; \
	GPSET1 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR1 = mask; break; }\
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ONE_LOW); \
	while(1) if(ARM_TIMER_RIS) break; \
})

#define LOW_PIN_SEND_ONE(mask) ({ \
	ARM_TIMER_CTL = 0x003E0002; \
	ARM_TIMER_LOD = (LED_ONE_HIGH); \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_CTL = 0x003E0082; \
	GPSET0 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR0 = mask; break; } \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ONE_LOW); \
	while(1) if(ARM_TIMER_RIS) break; \
})

#define HIGH_PIN_SEND_ZERO(mask) ({ \
	ARM_TIMER_CTL = 0x003E0002; \
	ARM_TIMER_LOD = (LED_ZERO_HIGH); \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_CTL = 0x003E0082; \
	GPSET1 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR1 = mask; break; } \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ZERO_LOW); \
	while(1) if(ARM_TIMER_RIS) break; \
})

#define LOW_PIN_SEND_ZERO(mask) ({ \
	ARM_TIMER_CTL = 0x003E0002; \
	ARM_TIMER_LOD = (LED_ZERO_HIGH); \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_CTL = 0x003E0082; \
	GPSET0 = mask; \
	while(1) if(ARM_TIMER_RIS) { GPCLR0 = mask; break; } \
	ARM_TIMER_CLI = 0; \
	ARM_TIMER_LOD = (LED_ZERO_LOW); \
	while(1) if(ARM_TIMER_RIS) break; \
})

typedef struct {
	uint8_t		channel;	/* PWM channel */
	uint8_t 	pin;		/* GPIO pin for PWM */
	uint32_t	num_led;	/* Number of LEDS */
} WS2812B_LED_config_t;

/* 3 PWM bits needed for each bit of led color, and zero for 55us to reset */
#define PWM_BIT_COUNT(leds, freq) 	(((leds) * LED_COLOR_BITS * 3) \
									+ ((LED_RESET_US * ((freq) * 3)) / 1000000))

// Pad out to the nearest uint32 + 32-bits for idle low/high times the number of channels
#define PWM_BYTE_COUNT(leds, freq)	(((((PWM_BIT_COUNT(leds, freq) >> 3) & ~0x7) + 4) + 4))

#define PWM_DMA_CHANNEL 5

volatile extern uint8_t led_raw[PWM_BYTE_COUNT(NUM_LED, LED_FREQ_HZ)];
volatile extern dma_cb_t dma_cb;

#define LED_RAW_ADDR 0xC0019000
#define DMA_CB_ADDR  0xC0018000

uint8_t pwm_serial_init(uint32_t freq, WS2812B_LED_config_t config);

void pwm_deinit( void );

uint8_t pwm_GPIO_init(uint8_t channel, uint8_t pin);

void dma_start( void );

uint8_t dma_wait( void );

uint8_t WS2812B_LED_init(WS2812B_LED_config_t config);

uint8_t WS2812B_LED_refresh(WS2812B_LED_config_t config, uint32_t *leds);

//void send_data(uint8_t pin, uint32_t data);

#endif
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

#ifndef __BCM2836_HW__
#define __BCM2836_HW__

#include "stdint.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

/* Base peripheral address */
#define P_BASE 0x3F000000

/* GPIO register adderesses */
#define GPIO_BASE	(P_BASE + 0x00200000)
#define GPFSEL(n)	(GPIO_BASE + ((n) * 0x4))	/* GPIO Function Select 0-5 */
#define GPSET0		(GPIO_BASE + 0x1C)			/* GPIO Pin Output Set 0 */
#define GPSET1		(GPIO_BASE + 0x20)			/* GPIO Pin Output Set 1 */
#define GPCLR0		(GPIO_BASE + 0x28)			/* GPIO Pin Output Clear 0 */
#define GPCLR1		(GPIO_BASE + 0x2C)			/* GPIO Pin Output Clear 1 */
#define GPLEV0		(GPIO_BASE + 0x34)			/* GPIO Pin Level 0 */
#define GPLEV1		(GPIO_BASE + 0x38)			/* GPIO Pin Level 1 */
#define GPEDS0		(GPIO_BASE + 0x40)			/* GPIO Pin Event Detect Status 0 */
#define GPEDS1		(GPIO_BASE + 0x44)			/* GPIO Pin Event Detect Status 1 */
#define GPREN0		(GPIO_BASE + 0x4C)			/* GPIO Pin Rising Edge Detect Enable 0 */
#define GPREN1		(GPIO_BASE + 0x50)			/* GPIO Pin Rising Edge Detect Enable 1 */
#define GPFEN0		(GPIO_BASE + 0x58)			/* GPIO Pin Falling Edge Detect Enable 0 */
#define GPFEN1		(GPIO_BASE + 0x5C)			/* GPIO Pin Falling Edge Detect Enable 1 */
#define GPHEN0		(GPIO_BASE + 0x64)			/* GPIO Pin High Detect Enable 0 */
#define GPHEN1		(GPIO_BASE + 0x68)			/* GPIO Pin High Detect Enable 1 */
#define GPLEN0		(GPIO_BASE + 0x70)			/* GPIO Pin Low Detect Enable 0 */
#define GPLEN1		(GPIO_BASE + 0x74)			/* GPIO Pin Low Detect Enable 1 */
#define GPAREN0		(GPIO_BASE + 0x7C)			/* GPIO Pin Async. Rising Edge Detect 0 */
#define GPAREN1		(GPIO_BASE + 0x80)			/* GPIO Pin Async. Rising Edge Detect 1 */
#define GPAFEN0		(GPIO_BASE + 0x88)			/* GPIO Pin Async. Falling Edge Detect 0 */
#define GPAFEN1		(GPIO_BASE + 0x8C)			/* GPIO Pin Async. Falling Edge Detect 1 */
#define GPPUD		(GPIO_BASE + 0x94)			/* GPIO Pin Pull-up/down Enable */
#define GPPUDCLK0	(GPIO_BASE + 0x98)			/* GPIO Pin Pull-up/down Enable Clock 0 */
#define GPPUDCLK1	(GPIO_BASE + 0x9C)			/* GPIO Pin Pull-up/down Enable Clock 1 */

/* GPIO Fuction select values */
#define GPIO_INPUT		0
#define GPIO_OUTPUT		1
#define GPIO_ALT0		4
#define GPIO_ALT1		5
#define GPIO_ALT2		6
#define GPIO_ALT3		7
#define GPIO_ALT4		3
#define GPIO_ALT5		2

void GPIO_configure(uint8_t pin, uint8_t mode);

void GPIO_set(uint8_t pin);

void GPIO_clear(uint8_t pin);

uint8_t GPIO_level(uint8_t pin);

#define ARM_TIMER_LOD 0x3F00B400
#define ARM_TIMER_VAL 0x3F00B404
#define ARM_TIMER_CTL 0x3F00B408
#define ARM_TIMER_CLI 0x3F00B40C
#define ARM_TIMER_RIS 0x3F00B410
#define ARM_TIMER_MIS 0x3F00B414
#define ARM_TIMER_RLD 0x3F00B418
#define ARM_TIMER_DIV 0x3F00B41C
#define ARM_TIMER_CNT 0x3F00B420

#define SYSTIMERCLO 0x3F003004

#endif
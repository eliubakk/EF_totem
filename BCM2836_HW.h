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

#ifndef __BCM2836_HW_H__
#define __BCM2836_HW_H__

#include "stdint.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

/* Base peripheral address */
#define P_BASE 0x3F000000

/* ARM base register address */
#define ARM_BASE 0x40000000

/* GPIO register addresses */
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

/* GPIO Function Definitions */
void GPIO_configure(uint8_t pin, uint8_t mode);

void GPIO_set(uint8_t pin);

void GPIO_clear(uint8_t pin);

uint8_t GPIO_level(uint8_t pin);

void GPIO_toggle(uint8_t pin);

/* System Timer addresses */
#define SYSTIMER_FREQ		250000000	/* 250MHz */
#define SYSTIMER_PERIOD_US	0.004		/* 0.004us/period */
#define SYSTIMER_BASE	(P_BASE + 0x3000) 
#define SYSTIMER_CS		(SYSTIMER_BASE)			/* System Timer Control/Status */
#define SYSTIMER_CLO	(SYSTIMER_BASE + 0x4)	/* System Timer Counter Lower 32 bits */
#define SYSTIMER_CHI	(SYSTIMER_BASE + 0x8)	/* System Timer Counter Higher 32 bits */
#define SYSTIMER_C(n)	(SYSTIMER_BASE + 0xC + ((n) * 0x4))	/* System Timer Compare 0-3 */ 		

/* System Timer Function Definitions */
#define SYSTIMER_MATCH(n)	(1 & (GET32(SYSTIMER_CS) >> (n)))	/* Match detected for timers 0-3 */
#define SYSTIMER_MATCH_CLR(n) PUT32(SYSTIMER_CS, 1 << (n))
#define SYSTIMER_COUNT	GET32(SYSTIMER_CLO)
#define SYSTIMER_C_SET(n, val) PUT32(SYSTIMER_C(n), (val))

/* ARM register adderesses */
#define ARM_CORE_TMR_CTL 	(ARM_BASE)					/* Control register */
#define ARM_CORE_TMR_SCALE	(ARM_BASE + 0x8)			/* Core timer pre-scaler */
#define ARM_CORE_TMR_LSB	(ARM_BASE + 0x1C)			/* Core timer LS 32 bits */
#define ARM_CORE_TMR_MSB	(ARM_BASE + 0x20)			/* Core timer MS 32 bits */
#define ARM_GPU_INT			(ARM_BASE + 0xC)			/* GPU interrupt routing */
#define ARM_PMU_INT_SET		(ARM_BASE + 0x10)			/* PMU interrupt routing write-set */
#define ARM_PMU_INT_CLR		(ARM_BASE + 0x14)			/* PMU interrupt routing write-clear */
#define ARM_CORE_TMRIRQ(n)	(ARM_BASE + 0x40 + ((n)*0x4))	/* Core 0-3 Timers interrupt control */
#define ARM_CORE_MBOXIRQ(n)	(ARM_BASE + 0x50 + ((n)*0x4))	/* Core 0-3 Mailboxes interrupt control */
#define ARM_CORE_IRQSRC(n)	(ARM_BASE + 0x60 + ((n)*0x4))	/* Core 0-3 interrupt source */
#define ARM_CORE_FIQSRC(n)	(ARM_BASE + 0x70 + ((n)*0x4))	/* Core 0-3 fast interrupt source */
#define ARM_LOCAL_TMR_CTL	(ARM_BASE + 0x34)			/* Local timer control & status */
#define ARM_LOCAL_TMR_IRQ	(ARM_BASE + 0x38)			/* Local timer IRQ clear & reload */
#define ARM_LOCAL_INT		(ARM_BASE + 0x24)			/* Local interrupt routing */



/* Arm Timer addresses */
#define ARM_TIMER_BASE	(P_BASE + 0xB000)
#define ARM_TIMER_LOD	(ARM_TIMER_BASE + 0x400)	/* Load */
#define ARM_TIMER_VAL	(ARM_TIMER_BASE + 0x404)	/* Value */
#define ARM_TIMER_CTL	(ARM_TIMER_BASE + 0x408)	/* Control */
#define ARM_TIMER_CLI	(ARM_TIMER_BASE + 0x40C)	/* IRQ Clear/Ack */
#define ARM_TIMER_RIS	(ARM_TIMER_BASE + 0x410)	/* RAW IRQ */
#define ARM_TIMER_MIS	(ARM_TIMER_BASE + 0x414)	/* Masked IRQ */
#define ARM_TIMER_RLD	(ARM_TIMER_BASE + 0x418)	/* Reload */
#define ARM_TIMER_DIV	(ARM_TIMER_BASE + 0x41C)	/* Pre-divider */
#define ARM_TIMER_CNT	(ARM_TIMER_BASE + 0x420)	/* Free running counter */

#endif
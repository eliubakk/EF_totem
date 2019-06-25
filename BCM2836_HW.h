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

/* Macro to directly access registers */
#define HW_IO(n)	(*(volatile uint32_t*)(n))

/***************************************
 *      Peripheral Base Addresses      *
 ***************************************/
/* Base peripheral address */
#define P_BASE		0x3F000000

/* Peripheral base addresses */
#define GPIO_BASE	(P_BASE + 0x00200000)
#define SPI0_BASE	(P_BASE + 0x204000)
#define PWM_BASE	(P_BASE + 0x20c000)
#define CM_CLK_BASE	(P_BASE + 0x101070)

/* Timer base adderesses */
#define SYS_TIMER_BASE	(P_BASE + 0x3000) 
#define ARM_TIMER_BASE	(P_BASE + 0xB000)

/* ARM base address */
#define ARM_BASE 0x40000000

/****************************
 *      GPIO Registers      *
 ****************************/
#define GPFSEL(n)	HW_IO(GPIO_BASE + ((n) * 0x4))	/* GPIO Function Select 0-5 */
#define GPSET0		HW_IO(GPIO_BASE + 0x1C)			/* GPIO Pin Output Set 0 */
#define GPSET1		HW_IO(GPIO_BASE + 0x20)			/* GPIO Pin Output Set 1 */
#define GPCLR0		HW_IO(GPIO_BASE + 0x28)			/* GPIO Pin Output Clear 0 */
#define GPCLR1		HW_IO(GPIO_BASE + 0x2C)			/* GPIO Pin Output Clear 1 */
#define GPLEV0		HW_IO(GPIO_BASE + 0x34)			/* GPIO Pin Level 0 */
#define GPLEV1		HW_IO(GPIO_BASE + 0x38)			/* GPIO Pin Level 1 */
#define GPEDS0		HW_IO(GPIO_BASE + 0x40)			/* GPIO Pin Event Detect Status 0 */
#define GPEDS1		HW_IO(GPIO_BASE + 0x44)			/* GPIO Pin Event Detect Status 1 */
#define GPREN0		HW_IO(GPIO_BASE + 0x4C)			/* GPIO Pin Rising Edge Detect Enable 0 */
#define GPREN1		HW_IO(GPIO_BASE + 0x50)			/* GPIO Pin Rising Edge Detect Enable 1 */
#define GPFEN0		HW_IO(GPIO_BASE + 0x58)			/* GPIO Pin Falling Edge Detect Enable 0 */
#define GPFEN1		HW_IO(GPIO_BASE + 0x5C)			/* GPIO Pin Falling Edge Detect Enable 1 */
#define GPHEN0		HW_IO(GPIO_BASE + 0x64)			/* GPIO Pin High Detect Enable 0 */
#define GPHEN1		HW_IO(GPIO_BASE + 0x68)			/* GPIO Pin High Detect Enable 1 */
#define GPLEN0		HW_IO(GPIO_BASE + 0x70)			/* GPIO Pin Low Detect Enable 0 */
#define GPLEN1		HW_IO(GPIO_BASE + 0x74)			/* GPIO Pin Low Detect Enable 1 */
#define GPAREN0		HW_IO(GPIO_BASE + 0x7C)			/* GPIO Pin Async. Rising Edge Detect 0 */
#define GPAREN1		HW_IO(GPIO_BASE + 0x80)			/* GPIO Pin Async. Rising Edge Detect 1 */
#define GPAFEN0		HW_IO(GPIO_BASE + 0x88)			/* GPIO Pin Async. Falling Edge Detect 0 */
#define GPAFEN1		HW_IO(GPIO_BASE + 0x8C)			/* GPIO Pin Async. Falling Edge Detect 1 */
#define GPPUD		HW_IO(GPIO_BASE + 0x94)			/* GPIO Pin Pull-up/down Enable */
#define GPPUDCLK0	HW_IO(GPIO_BASE + 0x98)			/* GPIO Pin Pull-up/down Enable Clock 0 */
#define GPPUDCLK1	HW_IO(GPIO_BASE + 0x9C)			/* GPIO Pin Pull-up/down Enable Clock 1 */

/* GPIO Fuction select values */
#define GPIO_INPUT		0
#define GPIO_OUTPUT		1
#define GPIO_ALT0		4
#define GPIO_ALT1		5
#define GPIO_ALT2		6
#define GPIO_ALT3		7
#define GPIO_ALT4		3
#define GPIO_ALT5		2

typedef struct {
	uint8_t pin;
	uint8_t alt;
} pin_config_t;

/************************************
 *      System Timer Registers      *
 ************************************/
#define SYS_TIMER_CS	HW_IO(SYS_TIMER_BASE)						/* System Timer Control/Status */
#define SYS_TIMER_CLO	HW_IO(SYS_TIMER_BASE + 0x4)					/* System Timer Counter Lower 32 bits */
#define SYS_TIMER_CHI	HW_IO(SYS_TIMER_BASE + 0x8)					/* System Timer Counter Higher 32 bits */
#define SYS_TIMER_C(n)	HW_IO(SYS_TIMER_BASE + 0xC + ((n) * 0x4))	/* System Timer Compare 0-3 */ 		

/* System Clock values */
#define SYS_TIMER_FREQ		250000000	/* 250MHz */
#define SYS_TIMER_PERIOD_US	0.004		/* 0.004us/period */
#define OSC_FREQ			19200000	/* 19.2MHz Crystal Oscillator */

/*********************************
 *      Arm Timer Registers      *
 *********************************/
#define ARM_TIMER_LOD	HW_IO(ARM_TIMER_BASE + 0x400)	/* Load */
#define ARM_TIMER_VAL	HW_IO(ARM_TIMER_BASE + 0x404)	/* Value */
#define ARM_TIMER_CTL	HW_IO(ARM_TIMER_BASE + 0x408)	/* Control */
#define ARM_TIMER_CLI	HW_IO(ARM_TIMER_BASE + 0x40C)	/* IRQ Clear/Ack */
#define ARM_TIMER_RIS	HW_IO(ARM_TIMER_BASE + 0x410)	/* RAW IRQ */
#define ARM_TIMER_MIS	HW_IO(ARM_TIMER_BASE + 0x414)	/* Masked IRQ */
#define ARM_TIMER_RLD	HW_IO(ARM_TIMER_BASE + 0x418)	/* Reload */
#define ARM_TIMER_DIV	HW_IO(ARM_TIMER_BASE + 0x41C)	/* Pre-divider */
#define ARM_TIMER_CNT	HW_IO(ARM_TIMER_BASE + 0x420)	/* Free running counter */

/**************************************
 *      Arm Core Timer Registers      *
 **************************************/
#define ARM_CORE_TMR_CTL 	HW_IO(ARM_BASE)						/* Control register */
#define ARM_CORE_TMR_SCALE	HW_IO(ARM_BASE + 0x8)				/* Core timer pre-scaler */
#define ARM_CORE_TMR_LSB	HW_IO(ARM_BASE + 0x1C)				/* Core timer LS 32 bits */
#define ARM_CORE_TMR_MSB	HW_IO(ARM_BASE + 0x20)				/* Core timer MS 32 bits */
#define ARM_GPU_INT			HW_IO(ARM_BASE + 0xC)				/* GPU interrupt routing */
#define ARM_PMU_INT_SET		HW_IO(ARM_BASE + 0x10)				/* PMU interrupt routing write-set */
#define ARM_PMU_INT_CLR		HW_IO(ARM_BASE + 0x14)				/* PMU interrupt routing write-clear */
#define ARM_CORE_TMRIRQ(n)	HW_IO(ARM_BASE + 0x40 + ((n)*0x4))	/* Core 0-3 Timers interrupt control */
#define ARM_CORE_MBOXIRQ(n)	HW_IO(ARM_BASE + 0x50 + ((n)*0x4))	/* Core 0-3 Mailboxes interrupt control */
#define ARM_CORE_IRQSRC(n)	HW_IO(ARM_BASE + 0x60 + ((n)*0x4))	/* Core 0-3 interrupt source */
#define ARM_CORE_FIQSRC(n)	HW_IO(ARM_BASE + 0x70 + ((n)*0x4))	/* Core 0-3 fast interrupt source */
#define ARM_LOCAL_TMR_CTL	HW_IO(ARM_BASE + 0x34)				/* Local timer control & status */
#define ARM_LOCAL_TMR_IRQ	HW_IO(ARM_BASE + 0x38)				/* Local timer IRQ clear & reload */
#define ARM_LOCAL_INT		HW_IO(ARM_BASE + 0x24)				/* Local interrupt routing */

/***************************
 *      PWM Registers      *
 ***************************/
#define PWM_CTL		HW_IO(PWM_BASE)			/* PWM Control */
#define PWM_STA		HW_IO(PWM_BASE + 0x4)	/* PWM Status */
#define PWM_DMAC	HW_IO(PWM_BASE + 0x8)	/* PWM DMA Configuration */
#define PWM_RNG1	HW_IO(PWM_BASE + 0x10)	/* PWM Channel 1 Range */
#define PWM_DAT1	HW_IO(PWM_BASE + 0x14)	/* PWM Channel 1 Data */
#define PWM_FIF1	HW_IO(PWM_BASE + 0x18)	/* PWM FIFO Input */
#define PWM_RNG2	HW_IO(PWM_BASE + 0x20)	/* PWM Channel 2 Range */
#define PWM_DAT2	HW_IO(PWM_BASE + 0x24)	/* PWM Channel 2 Data */

/* PWM Control bits */
#define PWM_CTL_MSEN2	(1 << 15)	/* Channel 2 M/S Enable */
#define PWM_CTL_USEF2	(1 << 13)	/* Channel 2 Use Fifo */
#define PWM_CTL_POLA2	(1 << 12)	/* Channel 2 Polarity */
#define PWM_CTL_SBIT2	(1 << 11)	/* Channel 2 Silence Bit */
#define PWM_CTL_RPTL2	(1 << 10)	/* Channel 2 Repeat Last Data */
#define PWM_CTL_MODE2	(1 << 9)	/* Channel 2 Mode */
#define PWM_CTL_PWEN2	(1 << 8)	/* Channel 2 Enable */
#define PWM_CTL_MSEN1	(1 << 7)	/* Channel 1 M/S Enable */
#define PWM_CTL_CLRF1	(1 << 6)	/* Clear Fifo */
#define PWM_CTL_USEF1	(1 << 5)	/* Channel 1 Use Fifo */
#define PWM_CTL_POLA1	(1 << 4)	/* Channel 1 Polarity */
#define PWM_CTL_SBIT1	(1 << 3)	/* Channel 1 Silence Bit */
#define PWM_CTL_RPTL1	(1 << 2)	/* Channel 1 Repeat Last Data */
#define PWM_CTL_MODE1	(1 << 1)	/* Channel 1 Mode */
#define PWM_CTL_PWEN1	(1)			/* Channel 1 Enable */

#define PWM_STA_EMPT1	(1 << 1)	/* PWM FIFO Empty Flag */
#define PWM_STA_FULL1	(1 << 0)	/* PWM FIFO Full Flag */

/*************************************
 *      Clock Manager Registers      *
 *************************************/
#define CM_CLK_GP0CTL	HW_IO(CM_CLK_BASE)			/* Clock Manager General Purpose Clocks Control 0 */
#define CM_CLK_GP1CTL	HW_IO(CM_CLK_BASE + 0x8)	/* Clock Manager General Purpose Clocks Control 1 */
#define CM_CLK_GP2CTL	HW_IO(CM_CLK_BASE + 0x10)	/* Clock Manager General Purpose Clocks Control 2 */
#define CM_CLK_GP0DIV	HW_IO(CM_CLK_BASE + 0x4)	/* Clock Manager General Purpose Clocks Divisor 0 */
#define CM_CLK_GP1DIV	HW_IO(CM_CLK_BASE + 0xC)	/* Clock Manager General Purpose Clocks Divisor 1 */
#define CM_CLK_GP2DIV	HW_IO(CM_CLK_BASE + 0x14)	/* Clock Manager General Purpose Clocks Divisor 2 */
#define CM_CLK_PCMCTL	HW_IO(CM_CLK_BASE + 0x28)	/* Clock Manager Audio Clocks Control PCM */
#define CM_CLK_PWMCTL	HW_IO(CM_CLK_BASE + 0x30)	/* Clock Manager Audio Clocks Control PWM */
#define CM_CLK_PCMDIV	HW_IO(CM_CLK_BASE + 0x2C)	/* Clock Manager Audio Clock Divisors PCM */
#define CM_CLK_PWMDIV	HW_IO(CM_CLK_BASE + 0x34)	/* Clock Manager Audio Clock Divisors PWM */

/* CM control bits */
#define CM_CLK_CTL_PASSWD		(0x5a << 24)
#define CM_CLK_CTL_MASH(val)	(((val) & 0x3) << 9)
#define CM_CLK_CTL_FLIP			(1 << 8)
#define CM_CLK_CTL_BUSY			(1 << 7)
#define CM_CLK_CTL_KILL			(1 << 5)
#define CM_CLK_CTL_ENAB			(1 << 4)
#define CM_CLK_CTL_SRC_GND		(0 << 0)
#define CM_CLK_CTL_SRC_OSC		(1 << 0)
#define CM_CLK_CTL_SRC_TSTDBG0	(2 << 0)
#define CM_CLK_CTL_SRC_TSTDBG1	(3 << 0)
#define CM_CLK_CTL_SRC_PLLA		(4 << 0)
#define CM_CLK_CTL_SRC_PLLC		(5 << 0)
#define CM_CLK_CTL_SRC_PLLD		(6 << 0)
#define CM_CLK_CTL_SRC_HDMIAUX	(7 << 0)

#define CM_CLK_DIV_PASSWD		(0x5a << 24)
#define CM_CLK_DIV_DIVI(val)	(((val) & 0xfff) << 12)
#define CM_CLK_DIV_DIVF(val)	(((val) & 0xfff) << 0)

/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/

/**********************************
 *      Functions and Macros      *
 **********************************/
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

/* System Timer Macro Definitions */
#define SYS_TIMER_MATCH(n)		(1 & (SYS_TIMER_CS >> (n)))	/* Match detected for timers 0-3 */
#define SYS_TIMER_MATCH_CLR(n)	SYS_TIMER_CS = (1 << (n))
#define SYS_TIMER_COUNT			SYS_TIMER_CLO
#define SYS_TIMER_C_SET(n, val)	SYS_TIMER_C(n) = (val)

/* System Timer Finction Definitions */
void usleep(uint32_t usec);

/* GPIO Function Definitions */
void GPIO_configure(pin_config_t config);

void GPIO_set(uint8_t pin);

void GPIO_clear(uint8_t pin);

uint8_t GPIO_level(uint8_t pin);

void GPIO_toggle(uint8_t pin);

#endif
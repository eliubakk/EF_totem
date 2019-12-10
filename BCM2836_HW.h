/****************************
 * Written by Erik Liubakka *
 ****************************/

/********************************************************************
 *                                                                  *
 * Peripheral srf defines and macros for BCM2836 Broadcom chip      *
 * for the Raspberry Pi 2                                           *
 *                                                                  *
 * Registers and functions for the following Peripherals            *
 *      - GPIO                                                      *
 *      - System Timer                                              *
 *      - ARM IRQ/PIT Timer                                         *
 *      - ARM Core Timer                                            *
 *      - Clock Manager                                             *
 *                                                                  *
 ********************************************************************/

/********************************************************************
 *                          UPDATE HISTORY                          *
 ********************************************************************
 *     DATE     |                   WHAT CHANGED                    *
 ********************************************************************
 *  2019-06-10  | Initial Revision                                  *
 *  2019-12-10  | Refactored to split peripherals to other files    *
 ********************************************************************/

#ifndef __BCM2836_HW_H__
#define __BCM2836_HW_H__

/*************************************************************************************
 *    INCLUDES                                                                       *
 *************************************************************************************/

#include "stdint.h"

/*************************************************************************************
 *    DEFINES                                                                        *
 *************************************************************************************/

/* Peripheral Base Address (non-ARM) */
#define P_BASE		0x3F000000

/* ARM Base Address */
#define ARM_BASE 	0x40000000

/* BSC */
/* EXTERNAL MASS MEDIA */
/* INTERRUPT */
/* PCM/I2S */
#define SPI0_BASE			(P_BASE + 0x204000)
/* SPI/BCS SLAVE */
/* UART */
/* USB */

/* Default Clock Source Values */
#define OSC_FREQ_MHZ		19.2		/* 19.2MHz Crystal Oscillator */
#define OSC_PERIOD			
#define SYS_TIMER_FREQ_MHZ	250	    	/* 250MHz */
#define SYS_TIMER_PERIOD_US	0.004		/* 0.004us/period */


/*************************************************************************************
 *    MACROS                                                                         *
 *************************************************************************************/

/* Register Access Macros */
#define HW_IO_TYPE(type, addr)	(*(volatile type*)(n))

/* Type Specific Register Access Macros */
#define HW_IO_U8(n)		HW_IO_TYPE(uint8_t, n)
#define HW_IO_U16(n)	HW_IO_TYPE(uint16_t, n)
#define HW_IO_U32(n)	HW_IO_TYPE(uint32_t, n)
#define HW_IO_S8(n)		HW_IO_TYPE(int8_t, n)
#define HW_IO_S16(n)	HW_IO_TYPE(int16_t, n)
#define HW_IO_S32(n)	HW_IO_TYPE(int32_t, n)
#define HW_IO(n)		HW_IO_U32(n)

/* System Timer Macro Definitions */
#define SYS_TIMER_MATCH(n)		(1 & (SYS_TIMER_CS >> (n)))	/* Match detected for timers 0-3 */
#define SYS_TIMER_MATCH_CLR(n)	SYS_TIMER_CS = (1 << (n))
#define SYS_TIMER_COUNT			SYS_TIMER_CLO
#define SYS_TIMER_C_SET(n, val)	SYS_TIMER_C(n) = (val)

/*************************************************************************************
 *    TYPE/ENUM DEFINITIONS                                                          *
 *************************************************************************************/

/**************** CLOCK MANAGER ****************/

/* Peripherials */
typedef enum cm_periph
{
	CM_GP0 = 0,	/* 0 - General Purpose Clock 0 */
	CM_GP1,		/* 1 - General Purpose Clock 1 */
	CM_GP2,		/* 2 - General Purpose Clock 2 */
	/* 3 */		/* unknown at this point */
	/* 4 */		/* unknown at this point */
	CM_PCM = 5,	/* 5 - Audio Clock, PCM */
	CM_PWM		/* 6 - Audio Clock, PWM */
} CM_periph_t;

/* Clock Sources */
typedef enum cm_clk_src
{
	SRC_GND = 0,	/* 0 */
	SRC_OSC,		/* 1 - 19.2MHz Crystal Oscillator */
	SRC_TSTDBG0,	/* 2 - ? */
	SRC_TSTDBG1,	/* 3 - ? */
	SRC_PLLA,		/* 4 */
	SRC_PLLC,		/* 5 */
	SRC_PLLD,		/* 6 */
	SRC_HDMIAUX,	/* 5 */
} CM_clk_src_t;

typedef unsigned float	CM_period_t;

/* Peripheral Clock Config */
typedef struct cm_cfg
{
	CM_periph_t		periph;		/* Peripheral clock to configure */
	CM_clk_src_t	clk_src;	/* Source of clock */
	CM_freq_t		period;		/* Period of Output clock in microseconds */
} CM_config_t;

/**************** GPIO ****************/

/* Function Select Values */
typedef enum gpio_alt
{
	GPIO_INPUT = 0,	/* 0 */
	GPIO_OUTPUT,	/* 1 */
	GPIO_ALT5,		/* 2 */
	GPIO_ALT4,		/* 3 */
	GPIO_ALT0,		/* 4 */
	GPIO_ALT1,		/* 5 */
	GPIO_ALT2,		/* 6 */
	GPIO_ALT3		/* 7 */
} GPIO_alt_t;

/* Pin Type */
typedef struct gpio_pin
{
	uint8_t		num;
	GPIO_alt_t	alt;
} GPIO_pin_t;

/*************************************************************************************
 *    GLOBAL VARIABLES                                                               *
 *************************************************************************************/


/*************************************************************************************
 *    GLOBAL FUNCTION PROTOTYPES                                                     *
 *************************************************************************************/

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

/**************** SYSTEM TIMER ****************/
void usleep(uint32_t usec);

/**************** CLOCK MANAGER ****************/
uint8_t CM_init(CM_config_t cfg);

void CM_deinit(CM_config_t cfg);


/**************** GPIO ****************/
void GPIO_configure(GPIO_pin_t p);

void GPIO_set(uint8_t pin);

void GPIO_clear(uint8_t pin);

uint8_t GPIO_level(uint8_t pin);

void GPIO_toggle(uint8_t pin);

/*************************************************************************************/

#endif
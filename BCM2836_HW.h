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

/* System Clock values */
#define SYS_TIMER_FREQ		250000000	/* 250MHz */
#define SYS_TIMER_PERIOD_US	0.004		/* 0.004us/period */
#define OSC_FREQ			19200000	/* 19.2MHz Crystal Oscillator */

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

/* GPIO Function Select Values */
typedef enum 
{
	GPIO_INPUT,		/* 0 */
	GPIO_OUTPUT,	/* 1 */
	GPIO_ALT5,		/* 2 */
	GPIO_ALT4,		/* 3 */
	GPIO_ALT0,		/* 4 */
	GPIO_ALT1,		/* 5 */
	GPIO_ALT2,		/* 6 */
	GPIO_ALT3		/* 7 */
} pin_alt_t;

typedef struct {
	uint8_t		pin;
	pin_alt_t	alt;
} pin_config_t;

/*************************************************************************************
 *    GLOBAL VARIABLES                                                               *
 *************************************************************************************/


/*************************************************************************************
 *    GLOBAL FUNCTION PROTOTYPES                                                     *
 *************************************************************************************/

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

/* System Timer Finction Definitions */
void usleep(uint32_t usec);

/* GPIO Function Definitions */
void GPIO_configure(pin_config_t config);

void GPIO_set(uint8_t pin);

void GPIO_clear(uint8_t pin);

uint8_t GPIO_level(uint8_t pin);

void GPIO_toggle(uint8_t pin);

/*************************************************************************************/

#endif
/****************************
 * Written by Erik Liubakka *
 ****************************/

/********************************************************************
 *                                                                  *
 * For use with the BCM2836 Broadcom chip on the Raspberry Pi 2     *
 *                                                                  *
 * Registers and functions for the following Peripherals            *
 *      - GPIO                                                      *
 *                                                                  *
 ********************************************************************/

/********************************************************************
 *                          UPDATE HISTORY                          *
 ********************************************************************
 *     DATE     |                   WHAT CHANGED                    *
 ********************************************************************
 *  2019-12-10  | Initial Revision, moved from BCM2836_HW.*         *
 ********************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__

/*************************************************************************************
 *    INCLUDES                                                                       *
 *************************************************************************************/

#include "BCM2836_HW.h"

/*************************************************************************************
 *    DEFINES                                                                        *
 *************************************************************************************/

#define GPIO_BASE			(P_BASE + 0x200000)

/* GPIO Registers */
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

/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/
/*******************************
 *            Types            *
 *******************************/

typedef struct {
	uint8_t pin;
	uint8_t alt;
} pin_config_t;

/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/
/*******************************
 *          Functions          *
 *******************************/

void GPIO_configure(pin_config_t config);

void GPIO_set(uint8_t pin);

void GPIO_clear(uint8_t pin);

uint8_t GPIO_level(uint8_t pin);

void GPIO_toggle(uint8_t pin);

#endif
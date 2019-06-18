/*
 * For use with the BCM2836 Broadcom chip on the Raspberry Pi 2
 *
 * Functions for initializing and using the PWM peripheral
 *
 *
 * Written by Erik Liubakka
 * Initial Revision: 2019-06-18
 *
 */

#ifndef __PWM_H__
#define __PWM_H__

#include "BCM2836_HW.h"

uint8_t pwm_serial_init(uint32_t freq, uint8_t channel);

#endif
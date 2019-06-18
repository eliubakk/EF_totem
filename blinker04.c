
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void dummy ( unsigned int );

#include "BCM2836_HW.h"
#include "WS2812B_LED_Drv.h"

//#define TIMEOUT 20000000
#define TIMEOUT 250000000
#define NUM_LED 25

//-------------------------------------------------------------------------
int notmain ( void )
{
    //volatile uint32_t timer_count;
    uint32_t led_i;

    //GPIO 47 is Green LED
    //GPIO 35 is Red LED
    GPIO_configure(35, GPIO_OUTPUT);
    GPIO_configure(17, GPIO_OUTPUT);

    //GPIO_clear(47);
    GPIO_clear(35);
    GPIO_set(35);


    ARM_TIMER_CTL = 0x003E0002;
    ARM_TIMER_LOD = (TIMEOUT - 1);
    //ARM_TIMER_RLD = (TIMEOUT-1);
    //PUT32(ARM_TIMER_DIV, 0x000000F9);
    //PUT32(ARM_TIMER_CTL, 0x00000002);
    ARM_TIMER_DIV = 0x00000000;
    ARM_TIMER_CLI = 0;
    ARM_TIMER_CTL = 0x003E0082;

    //PUT32(ARM_CORE_TMR_SCALE, 0x80000000);
    GPIO_clear(17);
    ARM_TIMER_LOD = (LED_RESET);
    ARM_TIMER_CLI = 0;
    while(1) if(ARM_TIMER_RIS) break;
    ARM_TIMER_CLI = 0;

    while(1)
    {
        //GPIO_toggle(35);
        //send_data(47, 0x0000FF00);
        //GPIO_toggle(35);
        //timer_count = SYSTIMER_COUNT;
        //SYSTIMER_C_SET(1, timer_count + TIMEOUT);
        //SYSTIMER_MATCH_CLR(1);
        //while(1) if(SYSTIMER_MATCH(1)) break;
        for(led_i = 0; led_i < NUM_LED; ++led_i){
            send_data(17, COLOR_RED);
        }
        ARM_TIMER_LOD = (TIMEOUT-1);
        ARM_TIMER_CLI = 0;
        while(1) if(ARM_TIMER_RIS) break;
        ARM_TIMER_CLI = 0;
        GPIO_toggle(35);
        //GPIO_toggle(47);

        //while(1) if(ARM_TIMER_RIS) break;
        //ARM_TIMER_CLI = 0;
        //GPIO_toggle(35);
        //GPIO_toggle(47);

        //GPIO_toggle(47);
        //GPIO_toggle(35);
        //while(1) if(GET32(ARM_TIMER_RIS)) break;
        //PUT32(ARM_TIMER_CLI, 0);
        //GPIO_toggle(47);
        //GPIO_toggle(35);
        //while(1) if(GET32(ARM_TIMER_RIS)) break;
        //PUT32(ARM_TIMER_CLI, 0);
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

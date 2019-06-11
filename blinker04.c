
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void dummy ( unsigned int );

#include "BCM2836_HW.h"

//#define TIMEOUT 20000000
#define TIMEOUT 4000000

//-------------------------------------------------------------------------
int notmain ( void )
{
    GPIO_configure(35, GPIO_OUTPUT);
    GPIO_configure(47, GPIO_OUTPUT);

    PUT32(ARM_TIMER_CTL, 0x003E0000);
    PUT32(ARM_TIMER_LOD, TIMEOUT-1);
    PUT32(ARM_TIMER_RLD, TIMEOUT-1);
    PUT32(ARM_TIMER_DIV, 0x000000F9);
    PUT32(ARM_TIMER_CLI, 0);
    PUT32(ARM_TIMER_CTL, 0x003E0082);

    while(1)
    {
        GPIO_set(47);
        GPIO_clear(35);
        while(1) if(GET32(ARM_TIMER_RIS)) break;
        PUT32(ARM_TIMER_CLI, 0);
        GPIO_clear(47);
        GPIO_set(35);
        while(1) if(GET32(ARM_TIMER_RIS)) break;
        PUT32(ARM_TIMER_CLI, 0);
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

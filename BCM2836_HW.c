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

/*************************************************************************************
 *    INCLUDES                                                                       *
 *************************************************************************************/

#include "BCM2836_HW.h"

/*************************************************************************************
 *    LOCAL DEFINES                                                                  *
 *************************************************************************************/

/* Base Adderesses */
#define GPIO_BASE		(P_BASE + 0x200000)
#define SYS_TIMER_BASE	(P_BASE + 0x3000) 
#define ARM_TIMER_BASE	(P_BASE + 0xB000)
#define CM_CLK_BASE		(P_BASE + 0x101070) /* CLOCK MANAGER */

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

/* System Timer Registers */
#define SYS_TIMER_CS	HW_IO(SYS_TIMER_BASE)						/* System Timer Control/Status */
#define SYS_TIMER_CLO	HW_IO(SYS_TIMER_BASE + 0x4)					/* System Timer Counter Lower 32 bits */
#define SYS_TIMER_CHI	HW_IO(SYS_TIMER_BASE + 0x8)					/* System Timer Counter Higher 32 bits */
#define SYS_TIMER_C(n)	HW_IO(SYS_TIMER_BASE + 0xC + ((n) * 0x4))	/* System Timer Compare 0-3 */ 		

/* ARM Timer Registers */
#define ARM_TIMER_LOD	HW_IO(ARM_TIMER_BASE + 0x400)	/* Load */
#define ARM_TIMER_VAL	HW_IO(ARM_TIMER_BASE + 0x404)	/* Value */
#define ARM_TIMER_CTL	HW_IO(ARM_TIMER_BASE + 0x408)	/* Control */
#define ARM_TIMER_CLI	HW_IO(ARM_TIMER_BASE + 0x40C)	/* IRQ Clear/Ack */
#define ARM_TIMER_RIS	HW_IO(ARM_TIMER_BASE + 0x410)	/* RAW IRQ */
#define ARM_TIMER_MIS	HW_IO(ARM_TIMER_BASE + 0x414)	/* Masked IRQ */
#define ARM_TIMER_RLD	HW_IO(ARM_TIMER_BASE + 0x418)	/* Reload */
#define ARM_TIMER_DIV	HW_IO(ARM_TIMER_BASE + 0x41C)	/* Pre-divider */
#define ARM_TIMER_CNT	HW_IO(ARM_TIMER_BASE + 0x420)	/* Free running counter */

/* ARM Core Timer Registers */
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

/* Clock Manager Registers */
// #define CM_CLK_GP0CTL	HW_IO(CM_CLK_BASE)			/* Clock Manager General Purpose Clocks Control 0 */
// #define CM_CLK_GP1CTL	HW_IO(CM_CLK_BASE + 0x8)	/* Clock Manager General Purpose Clocks Control 1 */
// #define CM_CLK_GP2CTL	HW_IO(CM_CLK_BASE + 0x10)	/* Clock Manager General Purpose Clocks Control 2 */
// #define CM_CLK_GP0DIV	HW_IO(CM_CLK_BASE + 0x4)	/* Clock Manager General Purpose Clocks Divisor 0 */
// #define CM_CLK_GP1DIV	HW_IO(CM_CLK_BASE + 0xC)	/* Clock Manager General Purpose Clocks Divisor 1 */
// #define CM_CLK_GP2DIV	HW_IO(CM_CLK_BASE + 0x14)	/* Clock Manager General Purpose Clocks Divisor 2 */
// #define CM_CLK_PCMCTL	HW_IO(CM_CLK_BASE + 0x28)	/* Clock Manager Audio Clocks Control PCM */
// #define CM_CLK_PWMCTL	HW_IO(CM_CLK_BASE + 0x30)	/* Clock Manager Audio Clocks Control PWM */
// #define CM_CLK_PCMDIV	HW_IO(CM_CLK_BASE + 0x2C)	/* Clock Manager Audio Clock Divisors PCM */
// #define CM_CLK_PWMDIV	HW_IO(CM_CLK_BASE + 0x34)	/* Clock Manager Audio Clock Divisors PWM */
#define CM_CLK_CTL(n)	HW_IO(CM_CLK_BASE + ((n)*0x8))			/* Clock Manager Clocks Control */
#define CM_CLK_DIV(n)	HW_IO(CM_CLK_BASE + ((n)*0x8) + 0x4)	/* Clock Manager Clock Divisor */

/* CM Control Bits */
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

/*************************************************************************************
 *    LOCAL VARIABLES                                                                *
 *************************************************************************************/


/*************************************************************************************
 *    GLOBAL VARIABLE DECLARATIONS                                                   *
 *************************************************************************************/


/*************************************************************************************
 *    LOCAL FUNCTION PROTOTYPES                                                      *
 *************************************************************************************/


/*************************************************************************************
 *    CODE                                                                           *
 *************************************************************************************/

/**************** SYSTEM TIMER ****************/
void usleep(uint32_t usec)
{
	volatile uint32_t timer_count;

	timer_count = SYS_TIMER_COUNT;
    SYS_TIMER_C_SET(1, timer_count + usec);
    SYS_TIMER_MATCH_CLR(1);
    while(1) if(SYS_TIMER_MATCH(1)) break;
}

/**************** CLOCK MANAGER ****************/
uint8_t CM_init(CM_config_t cfg)
{
	uint32_t div;
	uint8_t ret;

	ret = 0;
	switch(cfg.clk_src)
	{
		case SRC_GND:
		{
			div = 1;
			break;
		}
		case SRC_OSC:
		{
			div = (uint32_t)( OSC_FREQ_MHZ * cfg.period );
			break;
		}
		case SRC_TSTDBG0:
		case SRC_TSTDBG1:
		case SRC_PLLA:
		case SRC_PLLC:
		case SRC_PLLD:
		case SRC_HDMIAUX:
		default:
		{
			/* Not implemented */
			div = 1;
			ret = 1;
			break;
		}
	};

	if(ret == 0)
	{
		/* Set peripheral clock divisor */
		CM_CLK_DIV(cfg.periph) = CM_CLK_DIV_PASSWD | CM_CLK_DIV_DIVI(div);
		usleep(100);

		/* Set peripheral clock source */ 
		CM_CLK_CTL(cfg.periph) = CM_CLK_CTL_PASSWD | cfg.clk_src;
		usleep(100);

		/* Enable peripheral clock */
		CM_CLK_CTL(cfg.periph) = CM_CLK_CTL_PASSWD | cfg.clk_src | CM_CLK_CTL_ENAB;
		usleep(100);

		/* Wait until clock is busy (running) */
		while(1) if(CM_CLK_CTL(cfg.periph) & CM_CLK_CTL_BUSY) break;
	}
	
	return ret;
}

void CM_deinit(CM_config_t cfg)
{
	/* Kill Clock */
	CM_CLK_CTL(cfg.periph) = CM_CLK_CTL_PASSWD | CM_CLK_CTL_KILL;
	usleep(10);

	/* Wait until clock is not busy (off) */
	while(1) if(!(CM_CLK_CTL(cfg.periph) & CM_CLK_CTL_BUSY)) break;
}


/**************** GPIO ****************/
void GPIO_configure(GPIO_pin_t p)
{
	unsigned int reg;

	reg = GPFSEL(p.num / 10);

	/* clear mode bits */
	reg &= ~(7 << (3 * (p.num % 10))); 

	/* set mode bits */
    reg |= (p.alt << (3 * (p.num % 10)));

    GPFSEL(p.num / 10) = reg;
}

void GPIO_set(uint8_t pin)
{
	if(pin < 32)
	{
		GPSET0 = (1 << pin);
	}
	else
	{
		GPSET1 = (1 << (pin - 32));
	}
}

void GPIO_clear(uint8_t pin)
{
	if(pin < 32)
	{
		GPCLR0 = (1 << pin);
	}
	else
	{
		GPCLR1 = (1 << (pin - 32));
	}
}

uint8_t GPIO_level(uint8_t pin)
{
	if(pin < 32)
	{
		return (1 & (GPLEV0 >> pin));
	}
	else
	{
		return (1 & (GPLEV1 >> (pin - 32)));
	}
}

void GPIO_toggle(uint8_t pin)
{
	if(GPIO_level(pin))
	{
		GPIO_clear(pin);
	}
	else
	{
		GPIO_set(pin);
	}
}

/*************************************************************************************/
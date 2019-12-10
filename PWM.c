/****************************
 * Written by Erik Liubakka *
 ****************************/

/********************************************************************
 *                                                                  *
 * For use with the BCM2836 Broadcom chip on the Raspberry Pi 2     *
 *                                                                  *
 * Registers and functions for the following Peripherals            *
 *      - PWM                                                       *
 *                                                                  *
 ********************************************************************/

/********************************************************************
 *                          UPDATE HISTORY                          *
 ********************************************************************
 *     DATE     |                   WHAT CHANGED                    *
 ********************************************************************
 *  2019-06-18  | Initial Revision                                  *
 *  2019-12-10  | Moved PWM definitions from BCM2836_HW.*           *
 ********************************************************************/

/*************************************************************************************
 *    INCLUDES                                                                       *
 *************************************************************************************/

#include "PWM.h"

/*************************************************************************************
 *    LOCAL DEFINES                                                                  *
 *************************************************************************************/

/* PWM Registers */
#define PWM_CTL		HW_IO(PWM_BASE)			/* PWM Control */
#define PWM_STA		HW_IO(PWM_BASE + 0x4)	/* PWM Status */
#define PWM_DMAC	HW_IO(PWM_BASE + 0x8)	/* PWM DMA Configuration */
#define PWM_RNG1	HW_IO(PWM_BASE + 0x10)	/* PWM Channel 1 Range */
#define PWM_DAT1	HW_IO(PWM_BASE + 0x14)	/* PWM Channel 1 Data */
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

#define PWM_DMAC_ENAB			(1 << 31)
#define PWM_DMAC_PANIC(val)		(((val) & 0xff) << 8)
#define PWM_DMAC_DREQ(val)		(((val) & 0xff) << 0)

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

uint8_t PWM_init(PWM_config_t cfg)
{
	uint8_t i;
	uint8_t ret;
	GPIO_pin_t pwm_pins[2][4];
	CM_config_t clk_cfg;

	ret = 0;

	pwm_pins[0][0] = (GPIO_pin_t){.num = 12, .alt = GPIO_ALT0};
	pwm_pins[0][1] = (GPIO_pin_t){.num = 18, .alt = GPIO_ALT5};
	pwm_pins[0][2] = (GPIO_pin_t){.num = 40, .alt = GPIO_ALT0};
	pwm_pins[0][3] = (GPIO_pin_t){.num = 52, .alt = GPIO_ALT1};
	pwm_pins[1][0] = (GPIO_pin_t){.num = 13, .alt = GPIO_ALT0};
	pwm_pins[1][1] = (GPIO_pin_t){.num = 19, .alt = GPIO_ALT5};
	pwm_pins[1][2] = (GPIO_pin_t){.num = 41, .alt = GPIO_ALT0};
	pwm_pins[1][3] = (GPIO_pin_t){.num = 45, .alt = GPIO_ALT0};

	/* Check if channel is allowed */
	if(cfg.channel > 1)
	{
		ret = 1;
	}

	/* Check if pin is allowed for given channel, and configure GPIO */
	if(ret == 0)
	{
		ret = 1;
		for(i = 0; i < 4; ++i)
		{
			if(pwm_pins[cfg.channel][i].num == cfg.port)
			{
				GPIO_configure(pwm_pins[cfg.channel][i]);
				ret = 0;
				break;
			}
		}
	}

	/* Enable clock if pin was configured */
	if(ret == 0)
	{
		clk_cfg.periph = CM_PWM;
		clk_cfg.clk_src = SRC_OSC;
		clk_cfg.period = cfg.period;
		ret = CM_init(clk_cfg);
	}

	/* TODO: Add option for non-serial data transfer? */
	if(ret == 0)
	{
		/* Set up module for serial data transfer */
		PWM_RNG1 = 32;
		usleep(10);

		PWM_CTL = PWM_CTL_CLRF1;
		usleep(10);

		PWM_DMAC = PWM_DMAC_ENAB | PWM_DMAC_PANIC(7) | PWM_DMAC_DREQ(3);
		usleep(10);

		switch(cfg.channel){
			case 0: {
				PWM_CTL = PWM_CTL_USEF1 | PWM_CTL_MODE1;
				usleep(10);

				PWM_CTL |= PWM_CTL_PWEN1;
				break;
			}
			case 1: {
				PWM_CTL = PWM_CTL_USEF2 | PWM_CTL_MODE2;
				usleep(10);

				PWM_CTL |= PWM_CTL_PWEN2;
				break;
			}
			default:
				ret = 1;
		};
	}
	
	return ret;
}

void PWM_deinit(void)
{
	CM_config_t clk_cfg;

	clk_cfg.periph = CM_PWM;

	/* Turn off PWM */
	PWM_CTL = 0;
	usleep(10);

	/* Turn off Clock */
	CM_deinit(clk_cfg);
}

/*************************************************************************************/
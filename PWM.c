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

/* Base Address */
#define PWM_BASE	(P_BASE + 0x20C000)

/* PWM Registers */
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


/*************************************************************************************/
/****************************
 * Written by Erik Liubakka *
 ****************************/

/********************************************************************
 *                                                                  *
 * For use with the BCM2836 Broadcom chip on the Raspberry Pi 2     *
 *                                                                  *
 * Registers and functions for the following AUX Peripherals        *
 *      - UART                                                      *
 *      - SPI0                                                      *
 *      - SPI1                                                      *
 *                                                                  *
 ********************************************************************/

/********************************************************************
 *                          UPDATE HISTORY                          *
 ********************************************************************
 *     DATE     |                   WHAT CHANGED                    *
 ********************************************************************
 *  2019-12-10  | Initial Revision                                  *
 ********************************************************************/

/*************************************************************************************
 *    INCLUDES                                                                       *
 *************************************************************************************/

#include "aux_periph.h"

/*************************************************************************************
 *    LOCAL DEFINES                                                                  *
 *************************************************************************************/

/* Base Adderesses */
#define AUX_BASE			(P_BASE + 0x215000) 			/* Auxiliary Peripherals Base */
#define AUX_MU_BASE			(AUX_BASE + 0x40)				/* Auxiliary Mini UART Base */ 
#define AUX_SPI_BASE(n)		(AUX_BASE + 0x80 + (n)*0x40)	/* Auxiliary SPI0, SPI1 Base */

/* Global AUX Reg Bits */
#define AUX_IRQ_MU		(1)
#define AUX_IRQ_SPI0	(1 << 1)
#define AUX_IRQ_SPI1	(1 << 2)

#define AUX_EN_MU		(1)
#define AUX_EN_SPI0		(1 << 1)
#define AUX_EN_SPI1		(1 << 2)

/* Global AUX Registers */
#define AUX_IRQ			HW_IO_U8(AUX_BASE)			/* Auxiliary Interrupt Status */
#define AUX_ENABLES		HW_IO_U8(AUX_BASE + 0x4)	/* Auxiliary Enables */

/* Mini Uart AUX Registers */
#define AUX_MU_IO		HW_IO_U8(AUX_MU_BASE)			/* I/O Data */
#define AUX_MU_IER		HW_IO_U8(AUX_MU_BASE + 0x4)		/* Interrupt Enable */
#define AUX_MU_IIR		HW_IO_U8(AUX_MU_BASE + 0x8)		/* Interrupt Identify */
#define AUX_MU_LCR		HW_IO_U8(AUX_MU_BASE + 0xC)		/* Line Control */
#define AUX_MU_MCR		HW_IO_U8(AUX_MU_BASE + 0x10)	/* Modem Control */
#define AUX_MU_LSR		HW_IO_U8(AUX_MU_BASE + 0x14)	/* Line Status */
#define AUX_MU_MSR		HW_IO_U8(AUX_MU_BASE + 0x18)	/* Modem Status */
#define AUX_MU_SCRATCH	HW_IO_U8(AUX_MU_BASE + 0x1C)	/* Scratch */
#define AUX_MU_CNTL		HW_IO_U8(AUX_MU_BASE + 0x20)	/* Extra Control */
#define AUX_MU_STAT		HW_IO_U32(AUX_MU_BASE + 0x24)	/* Extra Status */
#define AUX_MU_BAUD		HW_IO_U16(AUX_MU_BASE + 0x28)	/* Baudrate */

/* SPI AUX Registers */
#define AUX_SPI_CNTL0(n)	HW_IO_U32(AUX_SPI((n)))			/* Control Register 0 */
#define AUX_SPI_CNTL1(n)	HW_IO_U8(AUX_SPI((n)) + 0x4)	/* Control Register 1 */
#define AUX_SPI_STAT(n)		HW_IO_U32(AUX_SPI((n)) + 0x8)	/* Status */
#define AUX_SPI_IO(n)		HW_IO_U32(AUX_SPI((n)) + 0x10)	/* Data */
#define AUX_SPI_PEEK(n)		HW_IO_U16(AUX_SPI((n)) + 0x14)	/* Peek */

/*************************************************************************************
 *    GLOBAL VARIABLE DECLARATIONS                                                   *
 *************************************************************************************/


/*************************************************************************************
 *    LOCAL VARIABLES                                                                *
 *************************************************************************************/


/*************************************************************************************
 *    LOCAL FUNCTION PROTOTYPES                                                      *
 *************************************************************************************/


/*************************************************************************************
 *    CODE                                                                           *
 *************************************************************************************/


/*************************************************************************************/
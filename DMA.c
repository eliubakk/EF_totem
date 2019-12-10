/****************************
 * Written by Erik Liubakka *
 ****************************/

/********************************************************************
 *                                                                  *
 * For use with the BCM2836 Broadcom chip on the Raspberry Pi 2     *
 *                                                                  *
 * Registers and functions for the following Peripherals            *
 *      - DMA                                                       *
 *                                                                  *
 ********************************************************************/

/********************************************************************
 *                          UPDATE HISTORY                          *
 ********************************************************************
 *     DATE     |                   WHAT CHANGED                    *
 ********************************************************************
 *  2019-12-10  | Initial Revision, moved from BCM2836_HW.*         *
 ********************************************************************/

/*************************************************************************************
 *    INCLUDES                                                                       *
 *************************************************************************************/

#include "DMA.h"

/*************************************************************************************
 *    LOCAL DEFINES                                                                  *
 *************************************************************************************/

/* Base Address */
#define DMA_BASE(channel)	(P_BASE + 0x7000 + ((channel) * 0x100))

/* DMA Registers */
#define DMA_CS(channel)			HW_IO(DMA_BASE(channel)) 		/* DMA Channel Control and Status */
#define DMA_CONBLK_AD(channel)	HW_IO(DMA_BASE(channel) + 0x4) 	/* DMA Channel Control Block Address */
#define DMA_TI(channel)			HW_IO(DMA_BASE(channel) + 0x8) 	/* DMA Channel CB Word 0 (Transfer Information) */
#define DMA_SOURCE_AD(channel)	HW_IO(DMA_BASE(channel) + 0xC) 	/* DMA Channel CB Word 1 (Source Address) */
#define DMA_DEST_AD(channel)	HW_IO(DMA_BASE(channel) + 0x10) /* DMA Channel CB Word 2 (Destination Address) */
#define DMA_TXFR_LEN(channel)	HW_IO(DMA_BASE(channel) + 0x14) /* DMA Channel CB Word 3 (Transfer Length) */
#define DMA_STRIDE(channel)		HW_IO(DMA_BASE(channel) + 0x18) /* DMA Channel CB Word 4 (2D Stride) */
#define DMA_NEXTCONBK(channel)	HW_IO(DMA_BASE(channel) + 0x1C)	/* DMA Channel CB Word 5 (Next CB Address) */
#define DMA_DEBUG(channel)		HW_IO(DMA_BASE(channel) + 0x20)	/* DMA Channel Debug */

/* DMA control bits */
#define DMA_CS_RESET						(1 << 31)
#define DMA_CS_ABORT						(1 << 30)
#define DMA_CS_DISDEBUG						(1 << 29)
#define DMA_CS_WAIT_OUTSTANDING_WRITES		(1 << 28)
#define DMA_CS_PANIC_PRIORITY(val)			(((val) & 0xf) << 20)                
#define DMA_CS_PRIORITY(val)				(((val) & 0xf) << 16)
#define DMA_CS_ERROR						(1 << 8)
#define DMA_CS_WAITING_OUTSTANDING_WRITES	(1 << 6)
#define DMA_CS_DREQ_STOPS_DMA				(1 << 5)
#define DMA_CS_PAUSED						(1 << 4)
#define DMA_CS_DREQ							(1 << 3)
#define DMA_CS_INT							(1 << 2)
#define DMA_CS_END							(1 << 1)
#define DMA_CS_ACTIVE						(1 << 0)

/* DMA transfer information bits */
#define DMA_TI_NO_WIDE_BURSTS				(1 << 26)
#define DMA_TI_WAITS(val)					(((val) & 0x1f) << 21)
#define DMA_TI_PREMAP(val)					(((val) & 0x1f) << 16)
#define DMA_TI_BURST_LENGTH(val)			(((val) & 0xf) << 12)
#define DMA_TI_SRC_IGNORE					(1 << 11)
#define DMA_TI_SRC_DREQ						(1 << 10)
#define DMA_TI_SRC_WIDTH					(1 << 9)
#define DMA_TI_SRC_INC						(1 << 8)
#define DMA_TI_DEST_IGNORE					(1 << 7)
#define DMA_TI_DEST_DREQ					(1 << 6)
#define DMA_TI_DEST_WIDTH					(1 << 5)
#define DMA_TI_DEST_INC						(1 << 4)
#define DMA_TI_WAIT_RESP					(1 << 3)
#define DMA_TI_TDMODE						(1 << 1)
#define DMA_TI_INTEN						(1 << 0)

/* DMA DREQ Peripheral Assignments */
#define DMA_DREQ_PWM						5

/* DMA transfer length bits */
#define DMA_TXFR_LEN_YLENGTH(val)			(((val) & 0xffff) << 16)
#define DMA_TXFR_LEN_XLENGTH(val)			(((val) & 0xffff) << 0)

/* DMA stride bits */
#define DMA_STRIDE_D_STRIDE(val)			(((val) & 0xffff) << 16)
#define DMA_STRIDE_S_STRIDE(val)			(((val) & 0xffff) << 0)

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
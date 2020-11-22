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

/* DMA transfer length bits */
#define DMA_TXFR_LEN_YLENGTH(val)			(((val) & 0xffff) << 16)
#define DMA_TXFR_LEN_XLENGTH(val)			(((val) & 0xffff) << 0)

/* DMA stride bits */
#define DMA_STRIDE_D_STRIDE(val)			(((val) & 0xffff) << 16)
#define DMA_STRIDE_S_STRIDE(val)			(((val) & 0xffff) << 0)


#define DMA_CB_ADDR  0xC0018000

/*************************************************************************************
 *    LOCAL VARIABLES                                                                *
 *************************************************************************************/


/*************************************************************************************
 *    GLOBAL VARIABLE DECLARATIONS                                                   *
 *************************************************************************************/

volatile dma_cb_t dma_cb __attribute__((section(".DMA_CB")));

/*************************************************************************************
 *    LOCAL FUNCTION PROTOTYPES                                                      *
 *************************************************************************************/


/*************************************************************************************
 *    CODE                                                                           *
 *************************************************************************************/

uint8_t DMA_init( DMA_channel_t channel, uint32_t source_addr, uint32_t dest_addr, uint32_t tx_len )
{
	if(channel != DMA_DREQ_PWM)
	{
		/* Only support PWM for now */
		return 1;
	}

	dma_cb.ti = (uint32_t)(DMA_TI_NO_WIDE_BURSTS |
						   DMA_TI_WAIT_RESP |
						   DMA_TI_DEST_DREQ |
						   DMA_TI_PREMAP(channel) |
						   DMA_TI_SRC_INC);

	dma_cb.source_ad = source_addr;
	dma_cb.dest_ad = dest_addr;
	dma_cb.txfr_len = tx_len;
	dma_cb.stride = 0;
	dma_cb.nextconbk = 0;
	dma_cb.reserved[0] = 0;
	dma_cb.reserved[1] = 0;

	DMA_CS(channel) = 0;
	DMA_TXFR_LEN(channel) = 0;

	return 0;
}

void DMA_start( DMA_channel_t channel )
{
	DMA_CS(channel) = DMA_CS_RESET;
	usleep(10);

	DMA_CS(channel) = DMA_CS_INT | DMA_CS_END;
	usleep(10);

	DMA_CONBLK_AD(channel) = (uint32_t)DMA_CB_ADDR;
	DMA_DEBUG(channel) = 7;
	DMA_CS(channel) = DMA_CS_WAIT_OUTSTANDING_WRITES |
						DMA_CS_PANIC_PRIORITY(15) |
						DMA_CS_PRIORITY(15) |
						DMA_CS_ACTIVE;
}

uint8_t DMA_wait( DMA_channel_t channel )
{
	while(    ( DMA_CS(channel) & DMA_CS_ACTIVE )
		  && !( DMA_CS(channel) & DMA_CS_ERROR  ) )
	{
		usleep(10);
	}

	if( DMA_CS(channel) & DMA_CS_ERROR )
	{
		return 1;
	}

	return 0;
}

/*************************************************************************************/
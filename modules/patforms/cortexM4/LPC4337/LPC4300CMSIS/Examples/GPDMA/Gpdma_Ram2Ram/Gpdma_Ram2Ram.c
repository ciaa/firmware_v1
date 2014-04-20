/**********************************************************************
* $Id$		Gpdma_Ram2Ram.c		2011-06-02
*//**
* @file		Gpdma_Ram2Ram.c
* @brief	This example used to test GPDMA Ram-to-Ram mode
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc43xx_gpdma.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Gpdma_Ram2Ram	Gpdma_Ram2Ram
 * @ingroup GPDMA_Examples
 * @{
 */


/************************** PRIVATE MACROS *************************/
/** Define SDRAM address */
#define LPC_AHB_SRAM0_BASE		0x20000000
/** DMA transfer size */
#define DMA_SIZE		0x100UL
/** DMA Source Address is AHBRAM1_BASE that used for USB RAM purpose, but
 * it is not used in this example, so this memory section can be used for general purpose
 * memory
 */
#define DMA_SRC			LPC_AHB_SRAM0_BASE
/** DMA Source Address is (AHBRAM1_BASE + DMA_SIZE) that used for USB RAM purpose, but
 * it is not used in this example, so this memory section can be used for general purpose
 * memory
 */
#define DMA_DST			(DMA_SRC+DMA_SIZE)


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"GPDMA demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 - 115200bps \n\r"
" This example will transfer 2 blocks of data from memory boundary \n\r"
" to the other memory boundary on RAM using GPDMA module with interrupt \n\r"
"********************************************************************************\n\r";
uint8_t menu2[] = "Demo terminated! \n\r";
uint8_t err_menu[] = "Buffer Check fail!";
uint8_t compl_menu[] = "Buffer Check success!";

// Terminal Counter flag for Channel 0
__IO uint32_t Channel0_TC;

// Error Counter flag for Channel 0
__IO uint32_t Channel0_Err;


/************************** PRIVATE FUNCTIONS *************************/
void DMA_IRQHandler (void);

void print_menu(void);
void Buffer_Init(void);
void Error_Loop(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		GPDMA interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void DMA_IRQHandler (void)
{
	// check GPDMA interrupt on channel 0
	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0)){ //check interrupt status on channel 0
		// Check counter terminal status
		if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0)){
			// Clear terminate counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
				Channel0_TC++;
		}
		if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)){
			// Clear error counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);
			Channel0_Err++;
		}
	}
}

/*-------------------------PRIVATE FUNCTIONS-----------------------------*/
/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}

/*********************************************************************//**
 * @brief		Initialize buffer
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Buffer_Init(void)
{
	uint8_t i;
	uint32_t *src_addr = (uint32_t *)DMA_SRC;
	uint32_t *dest_addr = (uint32_t *)DMA_DST;

	 for ( i = 0; i < DMA_SIZE/4; i++ )
	{
		*src_addr++ = i;
		*dest_addr++ = 0;
	}
}


/*********************************************************************//**
 * @brief		Verify buffer
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Buffer_Verify(void)
{
	uint8_t i;
	uint32_t *src_addr = (uint32_t *)DMA_SRC;
	uint32_t *dest_addr = (uint32_t *)DMA_DST;

	for ( i = 0; i < DMA_SIZE/4; i++ )
	{
		if ( *src_addr++ != *dest_addr++ )
		{
			/* Call Error Loop */
			Error_Loop();
		}
	}
}


/*********************************************************************//**
 * @brief		Error Loop (called by Buffer_Verify() if any error)
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Error_Loop(void)
{
	_DBG(err_menu);

	/* Loop forever */
	while (1);
}


/*-------------------------MAIN FUNCTION--------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	GPDMA_Channel_CFG_Type GPDMACfg;

	SystemInit();
	CGU_Init();
	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();

	/* GPDMA block section -------------------------------------------- */
	/* Initialize buffer */
	_DBG_("Initialize Buffer...");
	Buffer_Init();

    /* Disable GPDMA interrupt */
    NVIC_DisableIRQ(DMA_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));

    /* Initialize GPDMA controller */
	GPDMA_Init();

	// Setup GPDMA channel --------------------------------
	// channel 0
	GPDMACfg.ChannelNum = 0;
	// Source memory
	GPDMACfg.SrcMemAddr = DMA_SRC;
	// Destination memory
	GPDMACfg.DstMemAddr = DMA_DST;
	// Transfer size
	GPDMACfg.TransferSize = DMA_SIZE;
	// Transfer width
	GPDMACfg.TransferWidth = GPDMA_WIDTH_WORD;
	// Transfer type
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2M_CONTROLLER_DMA;
	// Source connection - unused
	GPDMACfg.SrcConn = 0;
	// Destination connection - unused
	GPDMACfg.DstConn = 0;
	// Linker List Item - unused
	GPDMACfg.DMALLI = 0;
	// Setup channel with given parameter
	GPDMA_Setup(&GPDMACfg);

	/* Reset terminal counter */
	Channel0_TC = 0;
	/* Reset Error counter */
	Channel0_Err = 0;

	_DBG_("Start transfer...");

	// Enable GPDMA channel 0
	GPDMA_ChannelCmd(0, ENABLE);

	/* Enable GPDMA interrupt */
	NVIC_EnableIRQ(DMA_IRQn);

	/* Wait for GPDMA processing complete */
	while ((Channel0_TC == 0) && (Channel0_Err == 0));

	/* Verify buffer */
	Buffer_Verify();

	_DBG(compl_menu);

    /* Loop forever */
    while(1);
}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}


#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
 */

/**********************************************************************
* $Id$		Emc_Sdram.c		2011-06-02
*//**
* @file		Emc_Sdram.c
* @brief	This example describes how to use EMC to program SDRAM
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
#include "debug_frmwrk.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Emc_Sdram	Emc_Sdram
 * @ingroup EMC_Examples
 * @{
 */

extern void SDRAM_Init(void);

/************************** PRIVATE DEFINITIONS *************************/
/* SDRAM Address Base for DYCS0*/
#define SDRAM_ADDR_BASE		0x28000000
/* SDRAM test size 512kBytes*/
#define SDRAM_SIZE			(512*1024)
/* EMC MAX Clock */
#define MAX_EMC_CLK			120000000


/************************** PRIVATE VARIABLES ***********************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Ex SDRAM Demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M4 \n\r"
"\t - Communicate via: UART0 -  115200 bps \n\r"
" This example will fill then check the SDRAM content on LPC4300 Eval Board \n\r"
"********************************************************************************\n\r";


/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void) {                       /* Main Program */
	uint32_t *ramdata;
	int i;
	SystemInit();
	CGU_Init();

	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	/* print welcome screen */
	print_menu();

	/* print Core and EMC frequency */
	_DBG("Core M4 Clk = ");
	_DBD32(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE));

	if(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) > MAX_EMC_CLK){
		_DBG("\r\nCore M4 Clk too high, SDRAM operation may wrong! Should set lower!!\r\n");
	}
	/* Init the SDRAM Controller */
	_DBG("Initialize the SDRAM...\r\n");
	SDRAM_Init();

	_DBG("Fill RAM...\r\n");
	ramdata = (uint32_t *)SDRAM_ADDR_BASE;
	for(i=0;i<SDRAM_SIZE/4;i++){
		*ramdata = i;
		ramdata++;
	}

	_DBG("Check RAM...\r\n");
	ramdata = (uint32_t *)SDRAM_ADDR_BASE;
	for(i=0;i<SDRAM_SIZE/4;i++){
		if(*ramdata != i) {
			_DBG("ramdata = ");
			_DBH32(*ramdata);
			_DBG("; i = ");
			_DBH32(i);
			while(1);
		}
		ramdata++;
	}
	_DBG("RAM Check Finish...\r\n");

	_DBG("Clear RAM content...\r\n");
	ramdata = (uint32_t *)SDRAM_ADDR_BASE;
	for(i=0;i<SDRAM_SIZE/4;i++){
		*ramdata = 0;
		ramdata++;
	}
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

/**********************************************************************
* $Id$		Emc_NorFlash.c		2011-06-02
*//**
* @file		Emc_NorFlash.c
* @brief	This example describes how to use EMC to program SST39VF320
*			Nor Flash
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
#include "SST39VF320.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Emc_NorFlash	Emc_NorFlash
 * @ingroup EMC_Examples
 * @{
 */

/************************** PRIVATE VARIABLES ***********************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"NOR Flash demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 -  115200 bps \n\r"
" This example will program the SST39VF3201 Nor Flash on Hitex LPC1800 Board \n\r"
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

	_DBG("Initialize the Flash...\r\n");

	Init(0x1C000000);
	/* Erase Sector 0 */
	_DBG("Press 1 to Erase Sector 0...\r\n");
	while(_DG!='1');
	EraseSector(0x1C000000 + 0x20000);
	_DBG("Press 2 to Program Menu data to Flash...\r\n");
	while(_DG!='2');
	if(ProgramPage(0x1C000000 + 0x20000, sizeof(menu1)+1, menu1) != 0){
		_DBG("Error ProgramPage !!!");
		while(1);
	}
	_DBG("Press 3 to Print menu data from exNOR...\r\n");
	while(_DG!='3');
	_DBG((void*)(0x1C000000 + 0x20000));
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

/**********************************************************************
* $Id: Hitex_Fast_Startup.c 8763 2011-12-08 00:45:50Z nxp21346 $		LPC18xx_LPC43xx_emc.c		2011-12-07
*//**
* @file		LPC18xx_LPC43xx_emc.c
* @brief	Contains all functions support for Clock Generation and Control
* 			firmware library on LPC18xx_LPC43xx
* @version	1.0
* @date		07. December. 2011
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

#include "LPC43xx.h"

#include "lpc43xx_cgu.h"
#include "lpc43xx_emc.h"
#include "lpc43xx_scu.h"


void Hitex_CGU_Init(void)
{

	__disable_irq();
	MemoryPinInit(); // Make sure EMC is in high-speed pin mode

	LPC_SCU->SFSCLK_3 = MD_PUP | MD_EHS | FUNC1;   /* CLKOUT on CLK3 pin */
	LPC_SCU->SFSCLK_1 = MD_PUP | MD_EHS | FUNC1;   /* CLKOUT on CLK1 pin */


 	/* Set the XTAL oscillator frequency to 12MHz*/
	CGU_SetXTALOSC(__CRYSTAL);
	CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_M4);
	
	/* Set PL160M 12*15 = 180 MHz */
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL1);
	CGU_SetPLL1(15);
	CGU_EnableEntity(CGU_CLKSRC_PLL1, ENABLE);

	/* Run extclock 3 from PL160M, /256 */
	CGU_EnableEntity(CGU_CLKSRC_IDIVE, DISABLE);
	CGU_SetDIV(CGU_CLKSRC_IDIVE, 256);
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVE);
	CGU_EnableEntity(CGU_CLKSRC_IDIVE, ENABLE);

	CGU_UpdateClock();	

#if defined LPC18XX
	LPC_CCU1->CLK_M3_EMCDIV_CFG |=    (1<<0) |  (1<<5);		// Turn on clock / 2
	LPC_CREG->CREG6 |= (1<<16);	// EMC divided by 2
    LPC_CCU1->CLK_M3_EMC_CFG |= (1<<0);		// Turn on clock
#endif
#if defined LPC43XX
	LPC_CCU1->CLK_M4_EMCDIV_CFG |=    (1<<0) |  (1<<5);		// Turn on clock / 2
	LPC_CREG->CREG6 |= (1<<16);	// EMC divided by 2
    LPC_CCU1->CLK_M4_EMC_CFG |= (1<<0);		// Turn on clock
#endif

	/* Run base M3 clock from PL160M, no division */
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_M4);

	emc_WaitMS(10);

	// output IDIVE	for debug
	LPC_CGU->BASE_OUT_CLK = (0x10 << 24) | (0x1 << 11);  

   #ifdef EXT_FLASH
	EMCFlashInit();
	#endif

	__enable_irq();

}

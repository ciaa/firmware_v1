/**********************************************************************
* $Id$		system_lpc43xx.c			2011-06-02
*//**
* @file		system_lpc43xx.c
* @brief	Cortex-M3 Device System Source File for NXP lpc43xx Series.
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
**********************************************************************/

#include <stdint.h>

#include "system_LPC43xx.h"


__attribute__((section("IRAM_SYSTEM_DATA"), zero_init))
uint32_t SystemCoreClock;   /* System Clock Frequency (Core Clock) */

/* bootloader sets pll at 96 MHz */
#define CPU_SPEED_AFTER_BOOTLOADER (96000000UL)    


__weak
void SystemInit (void)	{

	SystemCoreClock = CPU_SPEED_AFTER_BOOTLOADER;

}


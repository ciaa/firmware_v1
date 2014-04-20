/**********************************************************************
* $Id$		system_lpc43xx.c			2012-05-21
*//**
* @file		system_lpc43xx.c
* @brief	Cortex-M3 Device System Source File for NXP lpc43xx Series.
* @version	1.0
* @date		21. May. 2011
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

#include "lpc43xx.h"
#include "fpu_enable.h"
#include "lpc43xx_scu.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __IRC            (12000000UL)    /* IRC Oscillator frequency          */

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __IRC;		/*!< System Clock Frequency (Core Clock)*/

extern uint32_t getPC(void);

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
    uint32_t org;

#if defined(CORE_M4) && defined(USE_FPU)
        fpuEnable();
#endif
        
#if !defined(CORE_M0)
// Set up Cortex_M3 or M4 VTOR register to point to vector table
// This code uses a toolchain defined symbol to locate the vector table
// If this is not completed, interrupts are likely to cause an exception.
	unsigned int * pSCB_VTOR = (unsigned int *) 0xE000ED08;
#if defined(__IAR_SYSTEMS_ICC__)
	extern void *__vector_table;

	org = *pSCB_VTOR = (unsigned int)&__vector_table;
#elif defined(__CODE_RED)
	extern void *g_pfnVectors;

	org = *pSCB_VTOR = (unsigned int)&g_pfnVectors;
#elif defined(__ARMCC_VERSION)
	extern void *__Vectors;

	org = *pSCB_VTOR = (unsigned int)&__Vectors;
#else
#error Unknown compiler
#endif
#else
// Cortex M0?
	#error Cannot configure VTOR on Cortex_M0
#endif

// LPC18xx/LPC43xx ROM sets the PLL to run from IRC and drive the part
// at 96 MHz out of reset
    SystemCoreClock = 96000000;

// In case we are running from internal flash, we configure the flash
// accelerator. This is a conservative value that should work up to 204
// MHz on the LPC43xx or 180 MHz on the LPC18xx. This value may change
// as the chips are characterized and should also change based on
// core clock speed.
#define FLASH_ACCELERATOR_SPEED 6
#ifdef INTERNAL_FLASH
	{
		volatile uint32_t *MAM;
		uint32_t t;

		// Set up flash controller for both banks
		// Bank A
		MAM = (volatile uint32_t *)(LPC_CREG_BASE + 0x120);
		t=*MAM;
		t &= ~(0xF<<12);
		*MAM = t | (FLASH_ACCELERATOR_SPEED<<12);
		// Bank B
		MAM = (volatile uint32_t *)(LPC_CREG_BASE + 0x124);
		t=*MAM;
		t &= ~(0xF<<12);
		*MAM = t | (FLASH_ACCELERATOR_SPEED<<12);
	}
#endif
#ifdef EXTERNAL_PARALLEL_FLASH
// In case we are running from external flash, (booted by boot rom)
// We enable the EMC buffer to improve performance.
  /*Enable Buffer for External Flash*/
//  LPC_EMC->STATICCONFIG0 |= 1<<19;

	// Also: We must enable address lines A14 and above. The boot rom only enables them to A13.
	scu_pinmux(0x6,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_8 - EXTBUS_A14 — External memory address line 14 */
	scu_pinmux(0x6,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_7 - EXTBUS_A15 — External memory address line 15 */	
	scu_pinmux(0xD, 16,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_16 - EXTBUS_A16 — External memory address line 16 */
	scu_pinmux(0xD, 15,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_15 - EXTBUS_A17 — External memory address line 17 */	
	scu_pinmux(0xE,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_0 - EXTBUS_A18 — External memory address line 18 */
	scu_pinmux(0xE,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_1 - EXTBUS_A19 — External memory address line 19 */
	scu_pinmux(0xE,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_2 - EXTBUS_A20 — External memory address line 20 */
	scu_pinmux(0xE,  3,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_3 - EXTBUS_A21 — External memory address line 21 */
	scu_pinmux(0xE,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_4 - EXTBUS_A22 — External memory address line 22 */	
	scu_pinmux(0xA,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PA_4 - EXTBUS_A23 — External memory address line 23 */

#endif
}

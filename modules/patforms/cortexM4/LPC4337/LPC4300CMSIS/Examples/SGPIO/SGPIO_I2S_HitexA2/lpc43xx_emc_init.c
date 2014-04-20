/**********************************************************************
* $Id: lpc43xx_emc_init.c 
*//**
* @file		lpc43xx_emc_init.c
* @brief	Initializes the memory controller for usage of external 
*           memory devices
* @version	1.0
* @date		09. March 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
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
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "LPC43xx.h"
#include "lpc43xx_emc_init.h"
#include "lpc43xx_scu.h"



void MemoryPinInit(void)
{
  /* DATA LINES 0..15 */
    scu_pinmux(0x1,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_7: D0   */
    scu_pinmux(0x1,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_8: D1   */
    scu_pinmux(0x1,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_9: D2   */
    scu_pinmux(0x1,  10, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_10: D3  */
    scu_pinmux(0x1,  11, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_11: D4  */
    scu_pinmux(0x1,  12, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_12: D5  */
    scu_pinmux(0x1,  13, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_13: D6  */
    scu_pinmux(0x1,  14, (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_14: D7  */
    scu_pinmux(0x5,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_4: D8   */
    scu_pinmux(0x5,  5,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_5: D9   */
    scu_pinmux(0x5,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_6: D10  */
    scu_pinmux(0x5,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_7: D11  */
    scu_pinmux(0x5,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_0: D12  */
    scu_pinmux(0x5,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_1: D13  */
    scu_pinmux(0x5,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_2: D14  */
    scu_pinmux(0x5,  3,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* P5_3: D15  */

  /* ADDRESS LINES  */
	scu_pinmux(0x2,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_9 - EXTBUS_A0   */
	scu_pinmux(0x2, 10,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_10 - EXTBUS_A1  */	
	scu_pinmux(0x2, 11,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_11 - EXTBUS_A2  */	
	scu_pinmux(0x2, 12,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_12 - EXTBUS_A3  */
	scu_pinmux(0x2, 13,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_13 - EXTBUS_A4  */	
	scu_pinmux(0x1,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P1_0 - EXTBUS_A5   */
	scu_pinmux(0x1,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P1_1 - EXTBUS_A6   */	
	scu_pinmux(0x1,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P1_2 - EXTBUS_A7   */	
	scu_pinmux(0x2,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_8 - EXTBUS_A8   */
	scu_pinmux(0x2,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P2_7 - EXTBUS_A9   */	
	scu_pinmux(0x2,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_6 - EXTBUS_A10  */
	scu_pinmux(0x2,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_2 - EXTBUS_A11  */
	scu_pinmux(0x2,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_1 - EXTBUS_A12  */
	scu_pinmux(0x2,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* P2_0 - EXTBUS_A13  */	
	scu_pinmux(0x6,  8,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_8 - EXTBUS_A14  */
	scu_pinmux(0x6,  7,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_7 - EXTBUS_A15  */	
	scu_pinmux(0xD, 16,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_16 - EXTBUS_A16 */
	scu_pinmux(0xD, 15,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_15 - EXTBUS_A17 */	
	scu_pinmux(0xE,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_0 - EXTBUS_A18  */
	scu_pinmux(0xE,  1,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_1 - EXTBUS_A19  */
	scu_pinmux(0xE,  2,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_2 - EXTBUS_A20  */
	scu_pinmux(0xE,  3,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_3 - EXTBUS_A21  */
	scu_pinmux(0xE,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PE_4 - EXTBUS_A22  */	
	scu_pinmux(0xA,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* PA_4 - EXTBUS_A23  */

  /* CONTROL SIGNALS */
	scu_pinmux(0x1,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);	/* P1_4 - EXTBUS_BLS0 — LOW active Byte Lane select signal 0 */
	scu_pinmux(0x6,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC1);	/* P6_6 - EXTBUS_BLS1 — LOW active Byte Lane select signal 1 */	
	scu_pinmux(0xD, 13,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_13 - EXTBUS_BLS2 — LOW active Byte Lane select signal 2 */
	scu_pinmux(0xD, 10,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);	/* PD_10 - EXTBUS_BLS3 — LOW active Byte Lane select signal 3 */		
    
    scu_pinmux(0x6,  9,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P6_9: EXTBUS_DYCS0   */
    scu_pinmux(0x1,  6,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P1_6: WE             */
    scu_pinmux(0x6,  4,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P6_4: CAS            */
    scu_pinmux(0x6,  5,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P6_5: RAS            */

	LPC_SCU_CLK(0) = 0 + (MD_PLN | MD_EZI | MD_ZI | MD_EHS);  /* SFSCLK0: EXTBUS_CLK0   */
    LPC_SCU_CLK(1) = 0 + (MD_PLN | MD_EZI | MD_ZI | MD_EHS);  /* SFSCLK1: EXTBUS_CLK1   */
    LPC_SCU_CLK(2) = 0 + (MD_PLN | MD_EZI | MD_ZI | MD_EHS);  /* SFSCLK2: EXTBUS_CLK2   */
    LPC_SCU_CLK(3) = 0 + (MD_PLN | MD_EZI | MD_ZI | MD_EHS);  /* SFSCLK3: EXTBUS_CLK3   */

    scu_pinmux(0x6, 11,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P6_11: CKEOUT0  */
    scu_pinmux(0x6, 12,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P6_12: DQMOUT0  */
    scu_pinmux(0x6, 10,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* P6_10: DQMOUT1  */
    scu_pinmux(0xD,  0,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC2);  /* PD_0:  DQMOUT2  */
    scu_pinmux(0xE, 13,  (MD_PLN | MD_EZI | MD_ZI | MD_EHS), FUNC3);  /* PE_13: DQMOUT3  */

	scu_pinmux(	0x1, 3, MD_PLN_FAST, FUNC3);	/* OE   */
	scu_pinmux(	0x1, 4, MD_PLN_FAST, FUNC3);	/* BLS0 */
	scu_pinmux(	0x1, 5, MD_PLN_FAST, FUNC3);	/* CS0  */
	scu_pinmux(	0x1, 6, MD_PLN_FAST, FUNC3);	/* WE   */
}



void EMCFlashInit(void)
{
    LPC_EMC->STATICWAITRD0 = 14;   // 14 waitstates for read access @ 204MHz core frequency and 70ns flash type 
	LPC_EMC->STATICWAITPAG0 = 14;
	LPC_EMC->STATICWAITOEN0 = 0;

	LPC_EMC->CONTROL = 0x01;
	LPC_EMC->STATICCONFIG0 = (1UL<<7) | (1UL);

    /* Enable Buffer for EMC I/Os */
    LPC_EMC->STATICCONFIG0 |= 1<<19;
}


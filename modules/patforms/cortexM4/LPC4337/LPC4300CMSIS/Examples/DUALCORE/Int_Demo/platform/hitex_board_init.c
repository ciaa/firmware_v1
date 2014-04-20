/**********************************************************************
* $Id$		hitex_board_init.c			2012-03-16
*//**
* @file		hitex_board_init.c
* @brief	Hitex board specific initialization 
* @version	1.0
* @date		03. March. 2012
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
**********************************************************************/
#include "LPC43xx.h"
#include "fpu_enable.h"
#include "platform_init.h"

#include "hitex_board_init.h"

#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_emc.h"
#include "spifi_rom_api.h"

/*----------------------------------------------------------------------------
  Initialize board specific IO
 *----------------------------------------------------------------------------*/
void hitex_ioInit(void)
{	
	/* assigned to M4 core */
	/* P9.3 : GPIO4_15 */
	scu_pinmux(0x9 , 3 , MD_PDN, FUNC0); 	
	LPC_GPIO_PORT->DIR[4] |= (1UL << 15);

	/* assigned to M0 core */
	/* P8.1 : GPIO4_1 */
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	
	LPC_GPIO_PORT->DIR[4] |= (1UL << 1);

}

static void emc_WaitUS(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000) / 3;
	while(us--);
}

/*----------------------------------------------------------------------------
  Initialize clocks
 *----------------------------------------------------------------------------*/
void hitex_clockInit(void)
{

	/* after boot CPU runs at 96 MHz */
	/* cpu runs from: IRC (12MHz) >> PLL M = 24, FCCO @ 288 MHz direct mode >> IDIVC = 4 >> 96 MHz */
	
	/* enable the crystal oscillator */
	CGU_SetXTALOSC(__CRYSTAL);
	CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);

	/* connect the cpu to the xtal */
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_M4);
	
	/* connect the PLL to the xtal */
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL1);

	/* configure the PLL to 108 MHz */
	CGU_SetPLL1(9);
	
	/* enable the PLL */
	CGU_EnableEntity(CGU_CLKSRC_PLL1, ENABLE);

	/* connect to the CPU core */
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_M4);

	SystemCoreClock = 180000000;

	/* wait one msec */
	emc_WaitUS(1000);

	/* Change the clock to 204 MHz without disconnecting the CPU */
	/* uses direct mode */
	CGU_SetPLL1(17);

	SystemCoreClock = 204000000;

	/* configure the EMC to run at CLK/2 */
	LPC_CCU1->CLK_M4_EMCDIV_CFG |=    (1<<0) |  (1<<5);		// Turn on clock / 2
	LPC_CREG->CREG6 |= (1<<16);								// EMC divided by 2
    LPC_CCU1->CLK_M4_EMC_CFG |= (1<<0);						// Turn on clock

	CGU_UpdateClock();
}


void hitex_emcPinMuxInit(void) {

	/* configure pin multiplexing for external memory */
	/* Nor Flash on Hitex board is 16-bit mode */
  	/* DATA LINES 0..31 > D0..D31 */
  	/* P1_7 - EXTBUS_D0 — External memory data line 0 */
	#define PLAIN_FAST_MODE (MD_PLN | MD_EZI | MD_ZI | MD_EHS)

    scu_pinmux(0x1,  7,  PLAIN_FAST_MODE, FUNC3);  /* P1_7: 	D0 	*/
    scu_pinmux(0x1,  8,  PLAIN_FAST_MODE, FUNC3);  /* P1_8: 	D1 	*/
    scu_pinmux(0x1,  9,  PLAIN_FAST_MODE, FUNC3);  /* P1_9: 	D2 	*/
    scu_pinmux(0x1,  10, PLAIN_FAST_MODE, FUNC3);  /* P1_10: 	D3 	*/
    scu_pinmux(0x1,  11, PLAIN_FAST_MODE, FUNC3);  /* P1_11: 	D4 	*/
    scu_pinmux(0x1,  12, PLAIN_FAST_MODE, FUNC3);  /* P1_12: 	D5 	*/
    scu_pinmux(0x1,  13, PLAIN_FAST_MODE, FUNC3);  /* P1_13: 	D6 	*/
    scu_pinmux(0x1,  14, PLAIN_FAST_MODE, FUNC3);  /* P1_14: 	D7 	*/
    scu_pinmux(0x5,  4,  PLAIN_FAST_MODE, FUNC2);  /* P5_4: 	D8 	*/
    scu_pinmux(0x5,  5,  PLAIN_FAST_MODE, FUNC2);  /* P5_5: 	D9 	*/
    scu_pinmux(0x5,  6,  PLAIN_FAST_MODE, FUNC2);  /* P5_6: 	D10 */
    scu_pinmux(0x5,  7,  PLAIN_FAST_MODE, FUNC2);  /* P5_7: 	D11 */
    scu_pinmux(0x5,  0,  PLAIN_FAST_MODE, FUNC2);  /* P5_0: 	D12 */
    scu_pinmux(0x5,  1,  PLAIN_FAST_MODE, FUNC2);  /* P5_1: 	D13 */
    scu_pinmux(0x5,  2,  PLAIN_FAST_MODE, FUNC2);  /* P5_2: 	D14 */
    scu_pinmux(0x5,  3,  PLAIN_FAST_MODE, FUNC2);  /* P5_3: 	D15 */

#if 0	   	/* these settings are provided as template */
			/* they are not routed on Hitex board */
    scu_pinmux(0xD,  2,  PLAIN_FAST_MODE, FUNC2);  /* PD_2: D16 	*/
    scu_pinmux(0xD,  3,  PLAIN_FAST_MODE, FUNC2);  /* PD_3: D17 	*/
    scu_pinmux(0xD,  4,  PLAIN_FAST_MODE, FUNC2);  /* PD_4: D18  	*/
    scu_pinmux(0xD,  5,  PLAIN_FAST_MODE, FUNC2);  /* PD_5: D19 	*/
    scu_pinmux(0xD,  6,  PLAIN_FAST_MODE, FUNC2);  /* PD_6: D20 	*/
    scu_pinmux(0xD,  7,  PLAIN_FAST_MODE, FUNC2);  /* PD_7: D21 	*/
    scu_pinmux(0xD,  8,  PLAIN_FAST_MODE, FUNC2);  /* PD_8: D22 	*/
    scu_pinmux(0xD,  9,  PLAIN_FAST_MODE, FUNC2);  /* PD_9: D23 	*/
    scu_pinmux(0xE,  5,  PLAIN_FAST_MODE, FUNC3);  /* PE_5: D24 	*/
    scu_pinmux(0xE,  6,  PLAIN_FAST_MODE, FUNC3);  /* PE_6: D25 	*/
    scu_pinmux(0xE,  7,  PLAIN_FAST_MODE, FUNC3);  /* PE_7: D26 	*/
    scu_pinmux(0xE,  8,  PLAIN_FAST_MODE, FUNC3);  /* PE_8: D27 	*/
    scu_pinmux(0xE,  9,  PLAIN_FAST_MODE, FUNC3);  /* PE_9: D28 	*/
    scu_pinmux(0xE, 10,  PLAIN_FAST_MODE, FUNC3);  /* PE_10: D29 	*/
    scu_pinmux(0xE, 11,  PLAIN_FAST_MODE, FUNC3);  /* PE_11: D30 	*/
    scu_pinmux(0xE, 12,  PLAIN_FAST_MODE, FUNC3);  /* PE_12: D31 	*/
#endif

  	/* ADDRESS LINES A0..A14 */
	/* common to all configurations */
	scu_pinmux(0x2,  9,  PLAIN_FAST_MODE, FUNC3);	/* P2_9 - EXTBUS_A0 — External memory address line 0 */
	scu_pinmux(0x2, 10,  PLAIN_FAST_MODE, FUNC3);	/* P2_10 - EXTBUS_A1 — External memory address line 1 */	
	scu_pinmux(0x2, 11,  PLAIN_FAST_MODE, FUNC3);	/* P2_11 - EXTBUS_A2 — External memory address line 2 */	
	scu_pinmux(0x2, 12,  PLAIN_FAST_MODE, FUNC3);	/* P2_12 - EXTBUS_A3 — External memory address line 3 */
	scu_pinmux(0x2, 13,  PLAIN_FAST_MODE, FUNC3);	/* P2_13 - EXTBUS_A4 — External memory address line 4 */	
	scu_pinmux(0x1,  0,  PLAIN_FAST_MODE, FUNC2);	/* P1_0 - EXTBUS_A5 — External memory address line 5 */
	scu_pinmux(0x1,  1,  PLAIN_FAST_MODE, FUNC2);	/* P1_1 - EXTBUS_A6 — External memory address line 6 */	
	scu_pinmux(0x1,  2,  PLAIN_FAST_MODE, FUNC2);	/* P1_2 - EXTBUS_A7 — External memory address line 7 */	
	scu_pinmux(0x2,  8,  PLAIN_FAST_MODE, FUNC3);	/* P2_8 - EXTBUS_A8 — External memory address line 8 */
	scu_pinmux(0x2,  7,  PLAIN_FAST_MODE, FUNC3);	/* P2_7 - EXTBUS_A9 — External memory address line 9 */	
	scu_pinmux(0x2,  6,  PLAIN_FAST_MODE, FUNC2);	/* P2_6 - EXTBUS_A10 — External memory address line 10 */
	scu_pinmux(0x2,  2,  PLAIN_FAST_MODE, FUNC2);	/* P2_2 - EXTBUS_A11 — External memory address line 11 */
	scu_pinmux(0x2,  1,  PLAIN_FAST_MODE, FUNC2);	/* P2_1 - EXTBUS_A12 — External memory address line 12 */
	scu_pinmux(0x2,  0,  PLAIN_FAST_MODE, FUNC2);	/* P2_0 - EXTBUS_A13 — External memory address line 13 */	
	scu_pinmux(0x6,  8,  PLAIN_FAST_MODE, FUNC1);	/* P6_8 - EXTBUS_A14 — External memory address line 14 */

	scu_pinmux(0x1,	6,	PLAIN_FAST_MODE, FUNC3);	/* Write enable		*/

	/* NOR FLASH specific pins */
	#if (EXT_NOR_FLASH == YES)

	/* configure additional address lines */
	scu_pinmux(0x6,  7,  PLAIN_FAST_MODE, FUNC1);	/* P6_7 - EXTBUS_A15 — External memory address line 15 */	
	scu_pinmux(0xD, 16,  PLAIN_FAST_MODE, FUNC2);	/* PD_16 - EXTBUS_A16 — External memory address line 16 */
	scu_pinmux(0xD, 15,  PLAIN_FAST_MODE, FUNC2);	/* PD_15 - EXTBUS_A17 — External memory address line 17 */	
	scu_pinmux(0xE,  0,  PLAIN_FAST_MODE, FUNC3);	/* PE_0 - EXTBUS_A18 — External memory address line 18 */
	scu_pinmux(0xE,  1,  PLAIN_FAST_MODE, FUNC3);	/* PE_1 - EXTBUS_A19 — External memory address line 19 */
	scu_pinmux(0xE,  2,  PLAIN_FAST_MODE, FUNC3);	/* PE_2 - EXTBUS_A20 — External memory address line 20 */
	scu_pinmux(0xE,  3,  PLAIN_FAST_MODE, FUNC3);	/* PE_3 - EXTBUS_A21 — External memory address line 21 */
	scu_pinmux(0xE,  4,  PLAIN_FAST_MODE, FUNC3);	/* PE_4 - EXTBUS_A22 — External memory address line 22 */	
	scu_pinmux(0xA,  4,  PLAIN_FAST_MODE, FUNC3);	/* PA_4 - EXTBUS_A23 — External memory address line 23 */

	/* configure additional control signals */
	scu_pinmux(0x1, 5,	PLAIN_FAST_MODE, FUNC3);	/* Chip Select 0 */
	scu_pinmux(0x1,	3,	PLAIN_FAST_MODE, FUNC3);	/* Output enable */

	#endif

	/* SDRAM specific pins */
	#if(EXT_SDRAM == YES)

	/* configure additional clock */
	LPC_SCU_CLK(0) = 0 + PLAIN_FAST_MODE; /* SFSCLK0: EXTBUS_CLK0  */

	/* not routed to sdram on Hitex Board but must be enabled to use 16-bit and 32-bit sdram interfaces */
	LPC_SCU_CLK(1) = 0 + PLAIN_FAST_MODE; /* SFSCLK1: EXTBUS_CLK1  */
    LPC_SCU_CLK(2) = 0 + PLAIN_FAST_MODE; /* SFSCLK2: EXTBUS_CLK2  */
    LPC_SCU_CLK(3) = 0 + PLAIN_FAST_MODE; /* SFSCLK3: EXTBUS_CLK3  */

   
	/* configure additional control signals */
    scu_pinmux(0x6,  9,  PLAIN_FAST_MODE, FUNC3);  /* P6_9: EXTBUS_DYCS0 */
    scu_pinmux(0x6,  4,  PLAIN_FAST_MODE, FUNC3);  /* P6_4: CAS  */
    scu_pinmux(0x6,  5,  PLAIN_FAST_MODE, FUNC3);  /* P6_5: RAS  */

    scu_pinmux(0x6, 11,  PLAIN_FAST_MODE, FUNC3);  /* P6_11: CKEOUT0  */
    scu_pinmux(0x6, 12,  PLAIN_FAST_MODE, FUNC3);  /* P6_12: DQMOUT0  */
    scu_pinmux(0x6, 10,  PLAIN_FAST_MODE, FUNC3);  /* P6_10: DQMOUT1  */
    scu_pinmux(0xD,  0,  PLAIN_FAST_MODE, FUNC2);  /* PD_0: DQMOUT2   */
    scu_pinmux(0xE, 13,  PLAIN_FAST_MODE, FUNC3);  /* PE_13: DQMOUT3  */

	#endif


#if 0
	/* routed for SRAM device, not used */
	/* BYTE ENABLES */
	scu_pinmux(0x1,  4,  PLAIN_FAST_MODE, FUNC3);	/* P1_4 - EXTBUS_BLS0 — LOW active Byte Lane select signal 0 */
	scu_pinmux(0x6,  6,  PLAIN_FAST_MODE, FUNC1);	/* P6_6 - EXTBUS_BLS1 — LOW active Byte Lane select signal 1 */	
	
	/* configure additional address lines if not already configured by NOR flash initialization */
	#if(EXT_NOR_FLASH == NO)

	scu_pinmux(0x6,  7,  PLAIN_FAST_MODE, FUNC1);	/* P6_7 - EXTBUS_A15 — External memory address line 15 */	
	scu_pinmux(0xD, 16,  PLAIN_FAST_MODE, FUNC2);	/* PD_16 - EXTBUS_A16 — External memory address line 16 */
	scu_pinmux(0xD, 15,  PLAIN_FAST_MODE, FUNC2);	/* PD_15 - EXTBUS_A17 — External memory address line 17 */	
	scu_pinmux(0xE,  0,  PLAIN_FAST_MODE, FUNC3);	/* PE_0 - EXTBUS_A18 — External memory address line 18 */


	#endif

	/*not routed on Hitex board */
	// scu_pinmux(0xD, 13,  PLAIN_FAST_MODE, FUNC2);	/* PD_13 - EXTBUS_BLS2 — LOW active Byte Lane select signal 2 */
	// scu_pinmux(0xD, 10,  PLAIN_FAST_MODE, FUNC2);	/* PD_10 - EXTBUS_BLS3 — LOW active Byte Lane select signal 3 */		

#endif

}


/* Hitex board SST39VF3201B Flash */
const norFlashConfig norFlashEmc102Mhz = {

	.waitRd = (9 - 1),	/* 7,14 -> 8, plus one WS to account for margins  > 9 */
	.waitOutEn = 4 		/* 3,57-> 4 */
};

/* configure NOR flash */
void hitex_norFlashInit(void)
{	
	/* Read Cycle Time 70 nS minimum */
	LPC_EMC->STATICWAITRD0 = norFlashEmc102Mhz.waitRd; 

	/* Toe 35 ns max */
	LPC_EMC->STATICWAITOEN0 = norFlashEmc102Mhz.waitOutEn; 

	/* set to 16 bit mode, use WE, no page */
	LPC_EMC->STATICCONFIG0 = (1UL<<7) | (1UL);

    /* Enable Buffers for External Flash */
    LPC_EMC->STATICCONFIG0 |= 1<<19;
}



/* initialize external memory interface */
void hitex_externalMemoryInit(void) {

	hitex_emcPinMuxInit();
	
	/* enable the controller and enable CS0 & DYCS0 access */
	LPC_EMC->CONTROL = 0x01;

	#if (EXT_NOR_FLASH == YES)
	hitex_norFlashInit();
	#endif

	#if (EXT_SDRAM == YES)
	hitex_sdramInit();
	#endif


}




#define SPIFI_CLK_MODE 	(MD_PLN | MD_EHS | MD_ZI)
#define SPIFI_DATA_MODE (MD_PLN | MD_EZI | MD_ZI )
#define SPIFI_CS_MODE 	(MD_PUP | MD_ZI )
static SPIFIobj sobj;

/* configure SPIFI if needed */
void hitex_externalSpifiInit(void) {

	volatile uint32_t u32REG, u32Val;
	volatile int waitForMe = 1;
	int status;

	/* Read the SPIFI status */
	/* Notice, this is a read only register !!! */
	u32REG = LPC_RGU->RESET_ACTIVE_STATUS1;
	u32Val = 0x0;

	/* if 0 M0 is in reset, so need to keep this asserted */
	if(!(u32REG & (0x1 << 24))) u32Val = (0x1 << 24); 
	/* now add the SPIFI reset */
	u32Val |= (0x1 << 21); 

	/* reset the SPIFI now */
   	LPC_RGU->RESET_CTRL1 = u32Val;
	
	/* Run SPIFI from PLL out /3 = 68 MHz */
	/* In quad mode, on-board qspi flash can go up to 80 MHz */	
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVA);
	CGU_EnableEntity(CGU_CLKSRC_IDIVA, ENABLE);
	CGU_SetDIV(CGU_CLKSRC_IDIVA, 3);
	CGU_EntityConnect(CGU_CLKSRC_IDIVA, CGU_BASE_SPIFI);

	/* configure pinmuxing if not booting from SPIFI */
	#if(MASTER_BOOT_MODE != SPIFI_BOOT)

	scu_pinmux(0x3,  3,  SPIFI_CLK_MODE, FUNC3); /* SCLK 0xF3 */
	scu_pinmux(0x3,  4,  SPIFI_CLK_MODE, FUNC3); /* DATA 0xD3 */
	scu_pinmux(0x3,  5,  SPIFI_CLK_MODE, FUNC3); 
	scu_pinmux(0x3,  6,  SPIFI_CLK_MODE, FUNC3); 
	scu_pinmux(0x3,  7,  SPIFI_CLK_MODE, FUNC3); 
	scu_pinmux(0x3,  8,  SPIFI_CS_MODE,  FUNC3);  /* CS 0x83 */

	#endif
	
	/* re-init SPIFI for the new 68 MHz frequency */
	status = spifi_init(&sobj, 7, S_RCVCLK | S_FULLCLK, 68);
	

}


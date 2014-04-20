/***********************************************************************
 *
 * Project: LPC43xx SGPIO emulation
 *
 * Description: Main file including a test application for the frame sync
 * + 32-bit data emulation
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
#include "config.h"

#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"

#include "sgpio_frame.h"

void ClockInit(void);


/*----------------------------------------------------------------------------
  Initialize clocks
 *----------------------------------------------------------------------------*/
static void emc_WaitUS(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000) / 3;
	while(us--);
}


void ClockInit(void) {
	
	/* after boot CPU runs at 96 MHz */
	/* cpu runs from: IRC (12MHz) >> PLL M = 24, FCCO @ 288 MHz direct mode >> IDIVC = 4 >> 96 MHz */
	
	/* enable the crystal oscillator */
	CGU_SetXTALOSC(XTAL_FREQ);
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

	/* connect to the SGPIO block @ 204 MHz */
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_PERIPH);

	SystemCoreClock = 108000000;

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
	
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_CLKOUT);
	LPC_SCU->SFSCLK_0 = MD_EHS | PUP_ENABLE | FUNC1;
}




/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
volatile uint32_t fault = 0;

int main (void)
{	
	uint32_t c;
	uint8_t shift;
		
	SystemInit();
	ClockInit();
	
	// P8.1 : GPIO4_1 D10 (LED)
	// Systick heartbeat
	scu_pinmux(0x8 ,1 , PUP_ENABLE, FUNC0);	
	LPC_GPIO_PORT->DIR[4] |= (1UL << 1);

	// P9.1 : GPIO 4_13 SGPIO1 - LED
	scu_pinmux(0x9 ,1 , PUP_ENABLE, FUNC0);	
	LPC_GPIO_PORT->DIR[4] |= (1UL << 13);

	SysTick_Config(204000000/1000);
	
	// init the interface
	SGPIO_frameInit();
	
	// open frame interface
	SGPIO_frameOpen();
	
	if(EMU_OK != SGPIO_frameCheckErrors()) while(1);
		
	c = 0x1;
	
	// test a walking one pattern 
	while(1) {
		
		
		while(!isFrameReady());
		
		SGPIO_frameWrite(c);									
		
		if(c == 0) {
			c = 0x1; // for the 32-bit wrap
		} else {
			c = c << 1;		
		};

	};
	
}




volatile uint32_t i = 0;
void SysTick_Handler(void) {
	
	i++;
	
	if (i==1000) {
		
		if(fault == 0) LPC_GPIO_PORT->NOT[4] |= (1UL << 1); 
		i = 0;
	}; 
	
}


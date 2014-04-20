/***********************************************************************
 * $Id: main.c 3858 2010-07-15 15:33:12Z nxp27266 $
 *
 * Project: LPC18xx Validation
 *
 * Description: SGPIO Tests
 *
 * Copyright(C) 2010, NXP Semiconductor
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

#include "sgpio_spi.h"


/* clock phase 0 tests */

const SpiParam spiMaster0Setup_chpa0_low_4bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_4,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_4)	

};

const SpiParam spiMaster0Setup_chpa0_low_8bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_8,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_8)	

};

const SpiParam spiMaster0Setup_chpa0_low_16bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_16,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_16)	

};

const SpiParam spiMaster0Setup_chpa0_low_30bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_30,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_30)	

};

const SpiParam spiMaster0Setup_chpa0_low_32bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_32,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_32)

};

const SpiParam spiMaster0Setup_chpa0_high_4bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_4,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_4)	

};

const SpiParam spiMaster0Setup_chpa0_high_8bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_8,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_8)	

};

const SpiParam spiMaster0Setup_chpa0_high_16bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_16,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_16)	

};

const SpiParam spiMaster0Setup_chpa0_high_30bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_30,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_30)	

};

const SpiParam spiMaster0Setup_chpa0_high_32bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_32,
	.clockPhase = CPHA_0,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_32)	

};

/* clock phase 1 tests */

const SpiParam spiMaster0Setup_chpa1_low_4bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_4,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_4)	

};

const SpiParam spiMaster0Setup_chpa1_low_8bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_8,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_8)	

};

const SpiParam spiMaster0Setup_chpa1_low_16bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_16,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_16)	

};

const SpiParam spiMaster0Setup_chpa1_low_30bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_30,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_30)	

};

const SpiParam spiMaster0Setup_chpa1_low_32bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_32,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_LOW,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_32)

};

const SpiParam spiMaster0Setup_chpa1_high_4bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_4,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_4)	

};

const SpiParam spiMaster0Setup_chpa1_high_8bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_8,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_8)	

};

const SpiParam spiMaster0Setup_chpa1_high_16bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_16,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_16)	

};

const SpiParam spiMaster0Setup_chpa1_high_30bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_30,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_30)	

};

const SpiParam spiMaster0Setup_chpa1_high_32bit = {

	.spiMode = MASTER,
	.wordLenght = DBIT_32,
	.clockPhase = CPHA_1,
	.clockPolarity = IDLE_HIGH,
	.bitRateHz = 1000000,
	.chipSelect	= CHIP_SELECT_PATTERN(DBIT_32)

};


void ClockInit(void);
SpiParam const * test;

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
	
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_CLKOUT);
	LPC_SCU->SFSCLK_0 = MD_EHS | PUP_ENABLE | FUNC1;
}




/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
volatile uint32_t fault = 0;

int main (void)
{	
	uint32_t c = 0x1, value = 0x0;
	
	
	SystemInit();
	ClockInit();
	
	// P8.1 : GPIO4_1 D10 (LED)
	// Systick heartbeat
	scu_pinmux(0x8 ,1 , PUP_ENABLE, FUNC0);	
	LPC_GPIO_PORT->DIR[4] |= (1UL << 1);

	// P9.1 : GPIO 4_13 SGPIO1 - LED
	scu_pinmux(0x9 ,1 , PUP_ENABLE, FUNC0);	
	LPC_GPIO_PORT->DIR[4] |= (1UL << 13);

	SysTick_Config(120000000/1000);


	
	// pick the test mode
	test = &spiMaster0Setup_chpa0_low_32bit;
	
	// init the interface
	SGPIO_spiInit();
	
	// open master 0
	SGPIO_spiOpen(SPI_MASTER_0, test);
	
	if(SPIEMU_OK != SGPIO_spiEmuCheckErrors()) while(1);
		
	
	// test a walking one pattern in loopback
	while(1) {
		
		// check the interface is ready
		if(isSpiReady(SPI_MASTER_0)) {
			
			// write one item, read back data goes @ value address
			SGPIO_spiWrite(SPI_MASTER_0, c, &value);
			while(!isSpiReady(SPI_MASTER_0));
			
			if(c != value) {
				fault = 1;
				while(1);
			};
			
			c = c << 1;
			if(c & (1<<(test->wordLenght))) c=1; // limit up to the word length
			if(c == 0) c=1; // for the 32-bit case
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


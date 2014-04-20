/*****************************************************************************
 * $Id:: main.c                       
 *
 * Project: USB Audio Example for LPC18xx and LPC43xx
 *
 * Description:
 *   Implements a USB audio profile, the stereo data is forwarded to overall
 *   four I2S interfaces, realized with the SGPIO peripheral.
 *
 *----------------------------------------------------------------------------
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
 *****************************************************************************/

/*****************************************************************************
 *
 *  Pin access for the SGPIO - I2S channels on the Hitex rev A2 board:
 *  (applies to this software example only!!)
 *
 *  P1.18: SGPIO12: SV3.11            left/right
 *	P9.6 : SGPIO8:  SV6.9             audio bit clock
 *	P9.1 : SGPIO1:  SV6.7             stereo audio channel #1
 *	P9.5 : SGPIO3:  SV3.5             stereo audio channel #3  
 *	P9.3 : SGPIO9:  SV6.15            stereo audio channel #2
 *	P1.17: SGPIO11: SV3.3             stereo audio channel #4
 *****************************************************************************/

/*****************************************************************************
 *
 *  The UDA1380 is on board, so it can be used to test the I2S signals which 
 *  are coming from the SGPIOs:
 *
 *  Required connections:  SV6-9  --> SV9-9    Bit clock
 *                         SV3-11 --> SV9-7    WS
 *                         SV3-3  --> SV9-5    Data #4
 *                         SV3-5  --> SV9-5    Data #3
 *                         SV6-15 --> SV9-5    Data #2
 *                         SV6-7  --> SV9-5    Data #1
 *
 *  Jumpers:     SV9 1-2 17-18 19-20
 *  No jumpers: 
 *
 *****************************************************************************/

#ifndef CORE_M4
	  #error "Build Error: please define CORE_M4 in the project settings" 
#endif

#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_emc_init.h"
#include "uda1380.h"


/**********************************************************************
 ** Function prototypes
 **********************************************************************/
void vIOInit(void);
void AudioClockInit(void);

static volatile uint32_t u32Milliseconds = 1000;
static volatile uint32_t counter;


void sound_main(void);
extern void Hitex_CGU_Init(void);


/************************** PRIVATE DEFINITIONS *************************/
#define LED1_BIT			1   //LEDUSB
#define LED1_PORT			4

/************************** PRIVATE VARIABLES *************************/
uint32_t msec;


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 1000Hz
{
	if(msec)msec--;
}



/******************************************************************************
 *  Main Program
 *
 *
 *
 ******************************************************************************/
int main (void) 
{                   
    extern unsigned long __Vectors;	    
	SCB->VTOR = (unsigned long)&__Vectors;

	SystemInit();

    /* Set the XTAL oscillator frequency to 12 MHz */
	CGU_SetXTALOSC(12000000);
	CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_M4);
		
	#ifdef CLOCK_HALFRATE
	  LPC_CCU1->CLK_M4_EMCDIV_CFG |= (1<<0) |  (1<<5);	// Turn on clock / 2
	  LPC_CREG->CREG6 |= (1<<16);	                    // Clock for EMC gets divided by 2
      LPC_CCU1->CLK_M4_EMC_CFG |= (1<<0);		        // Turn on clock
    #endif
    
	// Set PL160M @ 12*9=108 MHz
	CGU_SetPLL1(9);
    
	// Run base M4 clock from PL160M, no division
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_M4);

	// Now change the clock to 204 MHz
	CGU_SetPLL1(17);

    CGU_UpdateClock();

    #ifdef EXT_STAT_MEM
      EMCFlashInit();
    #endif

    // Set a start signal
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : USB0_IND1 LED, set jumper JP3
	GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), 1);

	/* Turn on the USB phy */
    LPC_CREG->CREG0 &= ~(1 << 5);

    /* enable clocks and pinmux for usb0 */
	CGU_SetPLL0();
	CGU_EnableEntity(CGU_CLKSRC_PLL0, ENABLE);
	CGU_EntityConnect(CGU_CLKSRC_PLL0, CGU_BASE_USB0);
	
	AudioClockInit();

	/* Generate interrupt @ 1000 Hz */
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000);

	sound_main();

    #ifdef UDA1380
      // The UDA1380 is on board, so it can be used to test the I2S signals which are coming from the SGPIOs
      // Required connections:  SV6-9  --> SV9-9    Audio bit clock
      //                        SV3-11 --> SV9-7    WS
      //                        SV3-3  --> SV9-5    Data #4
      //                        SV3-5  --> SV9-5    Data #3
      //                        SV8-3  --> SV9-5    Data #2
      //                        SV8-5  --> SV9-5    Data #2
      Audio_Init();
    #endif

	while(1)
	{
		msec = 100;
		while(msec)
			__WFI();
		GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));     // clear USB0_IND1 LED
		msec = 100;
		while(msec)
			__WFI();
		GPIO_SetValue(LED1_PORT,(1<<LED1_BIT));       // set USB0_IND1 LED
	}	
}





/******************************************************************************
 *  Initialize board specific IO
 *
 *
 *
 ******************************************************************************/
 /*
void vIOInit(void)
{	
	#if (PLATFORM == NXP_VALIDATION_BOARD)
		// P9.2 : GPIO4_14: LD11 (LED)
		scu_pinmux(0x9 ,2 , MD_PDN, FUNC0); 	
		LPC_GPIO_PORT->DIR[4] |= (1UL << 14);
	#endif
	#if (PLATFORM == HITEX_A2_BOARD)    // BF: goes to LCDVD12
		// P8.3 : GPIO4_3
		scu_pinmux(0x8 , 3 , MD_PDN, FUNC0); 	
		LPC_GPIO_PORT->DIR[4] |= (1UL << 3);
	#endif
    #if (PLATFORM == HITEX_A4_BOARD)    // BF: put on P9.0 = LED D15 (green). However, it is not used in the code
		// P9.0 : GPIO4_12
		scu_pinmux(0x9 , 0 , MD_PDN, FUNC0); 	
		LPC_GPIO_PORT->DIR[4] |= (1UL << 12);
	#endif
    	
}
*/
	

  
/******************************************************************************
 *  Initialize clocks
 *
 *
 *
 ******************************************************************************/
void AudioClockInit(void)
{
	CGU_SetPLL0audio();
	
	CGU_EnableEntity(CGU_CLKSRC_PLL0_AUDIO, ENABLE);
	CGU_EntityConnect(CGU_CLKSRC_PLL0_AUDIO, CGU_BASE_CLKOUT);

    // set clock1 pin to output CLKOUT
    //LPC_SCU->SFSCLK_0 = MD_EHS | FUNC1;
    LPC_SCU->SFSCLK_1 = MD_EHS | FUNC1;       
    LPC_SCU->SFSCLK_2 = MD_EHS | FUNC1; 
    //LPC_SCU->SFSCLK_3 = MD_EHS | FUNC1;       

	CGU_EntityConnect(CGU_CLKSRC_PLL0_AUDIO, CGU_BASE_PERIPH);
	CGU_EnableEntity(CGU_BASE_PERIPH, ENABLE); 		
}




/**********************************************************************
* $Id$		Pwr_DeepSleep.c				2011-06-02
*//**
* @file		Pwr_DeepSleep.c
* @brief	This example describes how to enter the system in deep sleep
* 			mode and wake-up by using WAKEUP0 pin
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
#include "lpc43xx_evrt.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_pwr.h"
#include "debug_frmwrk.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Pwr_DeepSleep	Pwr_DeepSleep
 * @ingroup PWR_Examples
 * @{
 */

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"Power control demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART1 - 115200 bps \n\r"
	"This example used to enter system in deep sleep mode and wake up it\n\r"
	"by using WAKEUP0 pin\n\r"
	"********************************************************************************\n\r";
/************************** PRIVATE FUNCTION *************************/
void EVRT_IRQHandler(void);
void print_menu(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		EVRT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0))
	{
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0);
		NVIC_DisableIRQ(EVENTROUTER_IRQn);
	}
	LPC_EVENTROUTER->CLR_STAT = LPC_EVENTROUTER->STATUS;	
}
/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu);
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void)
{

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

	// print welcome screen
	print_menu();

	// Initiate EVRT
	EVRT_Init(LPC_EVENTROUTER);

	// Configure interrupt signal from WAKEUP0 pin to EVRT
#if defined(HITEX_BOARD)
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,EVRT_SRC_ACTIVE_RISING_EDGE);
#elif defined(KEIL_BOARD)
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,EVRT_SRC_ACTIVE_FALLING_EDGE);
#endif

	// Enable interrupt signal from WAKEUP0 pin to EVRT
	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,ENABLE);

    /* Disable EVRT interrupt */
    NVIC_DisableIRQ(EVENTROUTER_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x01<<3)|0x01));
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0);
    /* Enable RTC interrupt */
    NVIC_EnableIRQ(EVENTROUTER_IRQn);

    _DBG("\n\rPress '1' to start demo...");
    while(_DG != '1');

	_DBG("\n\rEnter deep sleep...connect WAKEUP0 pin to 3.3V to exit");
	CGU_EntityConnect(CGU_CLKSRC_IRC, CGU_BASE_M4);
  PWR_DeepSleep();

	// Re-initiate system
	CGU_Init();
	/* Re-Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

    _DBG("\n\rWaked up from deep sleep");

	while (1);
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

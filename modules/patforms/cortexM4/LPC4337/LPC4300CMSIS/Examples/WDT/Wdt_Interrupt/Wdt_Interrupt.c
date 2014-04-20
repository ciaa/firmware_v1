/**********************************************************************
* $Id$		Wdt_Interrupt.c			2011-06-02
*//**
* @file		Wdt_Interrupt.c
* @brief	This example describes how to use Watch-dog timer application
*           in interrupt mode
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
#include "lpc43xx_wwdt.h"
#include "lpc43xx_evrt.h"
#include "lpc43xx_cgu.h"
#include "debug_frmwrk.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Wdt_Interrupt	Wdt_Interrupt
 * @ingroup WDT_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS *************************/
//Watchdog time out in 5 seconds
#define WDT_INTERRUPT_TIMEOUT 	5000000 	// max value is (0xFFFFFF*4)/12000000 = 5.59s
//Watchdog warn in 3 seconds
#define WDT_WARNING_VALUE 		2000000
/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Watch dog timer interrupt (test or debug mode) demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: Cortex M3 \n\r"
"\t - Communicate via: UART1 -  115200 bps \n\r"
"********************************************************************************\n\r";
Bool wdt_feeding = TRUE;
uint32_t cnt=0;
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
	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER,EVRT_SRC_WWDT))
	{
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_WWDT);
		if(WWDT_GetStatus(WWDT_WARNINT_FLAG))
		{
			WWDT_ClearStatusFlag(WWDT_WARNINT_FLAG);
		}
		if(!wdt_feeding)
		{
			WWDT_Start();
			_DBG("Warning...watchdog timeout!\n\r");
		}
	}
}
/*********************************************************************//**
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 1000Hz
{
	if(cnt>=500)
	{
		if(wdt_feeding)
		{
			WWDT_Feed();
		}
		cnt = 0;
	}
	cnt++;
}
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
 * @brief		Main WDT program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	uint8_t ch;
	st_Wdt_Config wdtCfg;

	SystemInit();
	CGU_Init();
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000);			// Generate interrupt @ 1000 Hz

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

	//////////////////Configure WWDT////////////////////
	// Initiate WWDT
	WWDT_Init();
	// Configure WWDT
	wdtCfg.wdtReset = DISABLE;
	wdtCfg.wdtProtect = DISABLE;
	wdtCfg.wdtTmrConst = WDT_INTERRUPT_TIMEOUT;
	wdtCfg.wdtWarningVal = WDT_WARNING_VALUE;
	wdtCfg.wdtWindowVal = WWDT_WINDOW_MAX;
	WWDT_Configure(wdtCfg);
	//////////////////Configure EVRT/////////////////////
	// Initiate EVRT
	EVRT_Init(LPC_EVENTROUTER);
	// Configure interrupt signal from WWDT to EVRT
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_WWDT,EVRT_SRC_ACTIVE_RISING_EDGE);
	// Enable interrupt signal from WWDT to EVRT
	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_WWDT,ENABLE);

    _DBG("Watchdog is frequently fed by SysTick_Handler\n\r");
	_DBG("Press '1' to disable feeding Watchdog timer\n\r");
	_DBG("Press '2' to enable feeding Watchdog timer\n\r");

	// Start watchdog
	WWDT_Start();

    /* Disable EVRT interrupt */
    NVIC_DisableIRQ(EVENTROUTER_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x01<<3)|0x01));
    /* Enable RTC interrupt */
	NVIC_ClearPendingIRQ(EVENTROUTER_IRQn);
    NVIC_EnableIRQ(EVENTROUTER_IRQn);

	while(1)
	{
		do
		{
		 	ch = _DG;
		}
		while((ch !='1') && (ch != '2'));
		if(ch == '2')
		{
			wdt_feeding = TRUE;
			_DBG("Enable feeding\n\r");
		}
		if(ch == '1')
		{
			wdt_feeding = FALSE;
			_DBG("Disable feeding\n\r");
		}
	}

}

/* Support required entry point for other toolchain */
int main (void)
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

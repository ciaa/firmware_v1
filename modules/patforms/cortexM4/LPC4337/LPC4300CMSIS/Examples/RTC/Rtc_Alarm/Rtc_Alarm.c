/**********************************************************************
* $Id$		Rtc_Alarm.c			2011-06-02
*//**
* @file		Rtc_Alarm.c
* @brief	This example describes how to use RTC to generate interrupt
* 			in Second Counter Increment Interrupt (1s) and generate
*           alarm interrupt at 10s
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
#include "lpc43xx_rtc.h"
#include "lpc43xx_cgu.h"
#include "debug_frmwrk.h"
#include "lpc43xx_evrt.h"
#include "lpc43xx_libcfg.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Rtc_Alarm	Rtc_Alarm
 * @ingroup RTC_Examples
 * @{
 */


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" RTC demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM CORTEX-M3 \n\r"
"\t - Communicate via: UART1 - 115200 bps \n\r"
" A simple RTC example. \n\r"
" To generate interrupt in minute Counter Increment Interrupt (1min) \n\r"
" and generate Alarm interrupt at 30s \n\r"
"********************************************************************************\n\r";

/************************** PRIVATE FUNCTION *************************/
void EVRT_IRQHandler(void);
void RTC_IRQHandler(void);
void print_menu(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RTC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void RTC_IRQHandler(void)
{
#if 1
	uint32_t minval;

	/* This is increment counter interrupt*/
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
	{
		// Clear pending interrupt
		RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);

		minval = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_MINUTE);
		/* Send debug information */
		_DBG ("Minute: "); _DBD(minval);
		_DBG_("");
	}

	/* Continue to check the Alarm match*/
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_ALARM))
	{
		// Clear pending interrupt
		RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);

		/* Send debug information */
		_DBG_ ("ALARM 30s matched!");
	}
#else
	RTC_TIME_Type RTCFullTime;

	RTC_GetFullTime (LPC_RTC, &RTCFullTime);
	_DBG( "\n\rCurrent time : ");
	_DBD((RTCFullTime.HOUR)); _DBG (":");
	_DBD ((RTCFullTime.MIN)); _DBG (":");
	_DBD ((RTCFullTime.SEC)); _DBG("  ");
	_DBD ((RTCFullTime.DOM)); _DBG("/");
	_DBD ((RTCFullTime.MONTH)); _DBG("/");
	_DBD16 ((RTCFullTime.YEAR)); _DBG_("");
	interrupt = TRUE;
	RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
	RTC_ClearIntPending(LPC_RTC, RTC_INT_ALARM);
#endif
}

/*********************************************************************//**
 * @brief		EVRT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER,EVRT_SRC_RTC))
	{
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_RTC);
		RTC_IRQHandler();
	}
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
 * @brief		Main RTC program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	uint32_t sec,pre_sec;

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

	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_RTC,DISABLE);

	/* RTC Block section ------------------------------------------------------ */
	// Init RTC module
	RTC_Init(LPC_RTC);

	/* Set current time for RTC */
	// Current time is 8:00:00PM, 2009-04-24
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, 20);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 4);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2009);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 24);

	/* Set ALARM time for second */
	RTC_SetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND, 30);

	/* Set the AMR for 30s match alarm interrupt */
	RTC_AlarmIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);

	/* Set the CIIR for minute counter interrupt*/
	RTC_CntIncrIntConfig (LPC_RTC, RTC_TIMETYPE_MINUTE, ENABLE);

	/* EVRT Block section ------------------------------------------------------ */
	EVRT_Init(LPC_EVENTROUTER);
	/* Configure RTC signal to EVRT is rising edge*/
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_RTC,EVRT_SRC_ACTIVE_RISING_EDGE);
	/* Mount RTC interrupt to EVRT*/
	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_RTC,ENABLE);
	/* Clear pending interrupt from RTC if any*/
	EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_RTC);

    /* Disable EVRT interrupt */
    NVIC_DisableIRQ(EVENTROUTER_IRQn);
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x01<<3)|0x01));
    /* Enable RTC interrupt */
    NVIC_EnableIRQ(EVENTROUTER_IRQn);

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_Cmd(LPC_RTC, ENABLE);
	sec=pre_sec=0;
    /* Loop forever */
    while(1)
	{
		sec =  RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
		if(sec!=pre_sec)
		{
		 	pre_sec = sec;
			_DBG("*");
		}
	}
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

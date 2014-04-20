/**********************************************************************
* $Id$		Timer_Capture.c			2011-06-02
*//**
* @file		Timer_Capture.c
* @brief	This example describes how to use TIMER capture function
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
#include "lpc43xx_timer.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Timer_Capture	Timer_Capture
 * @ingroup TIMER_Examples
 * @{
 */

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Timer Match interrupt demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART1 -  115200 bps \n\r"
" Using Timer 0 to take a snapshot of the timer value when an input signal \n\r"
" on CAP0.2 transitions \n\r"
"********************************************************************************\n\r";
/************************** PRIVATE DEFINITIONS **********************/
#define CAPTURE_CHANNEL		1

// UART Configuration structure variable
UART_CFG_Type UARTConfigStruct;
uint32_t MatchCount;

//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;
uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;

/************************** PRIVATE FUNCTIONS *************************/
/* Interrupt service routines */
void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler(void);

void print_menu(void);
/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		TIMER0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TIMER0_IRQHandler(void)
{
	if (TIM_GetIntCaptureStatus(LPC_TIMER0,(TIM_INT_TYPE)CAPTURE_CHANNEL))
	{
		TIM_ClearIntCapturePending(LPC_TIMER0,(TIM_INT_TYPE)CAPTURE_CHANNEL);
		_DBG("Time capture: ");
		_DBH32(TIM_GetCaptureValue(LPC_TIMER0,(TIM_COUNTER_INPUT_OPT)CAPTURE_CHANNEL));
		_DBG_("");
	}
}

/*********************************************************************//**
 * @brief		TIMER1 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TIMER1_IRQHandler(void)
{
	if (TIM_GetIntCaptureStatus(LPC_TIMER1,(TIM_INT_TYPE)CAPTURE_CHANNEL))
	{
		TIM_ClearIntCapturePending(LPC_TIMER1,(TIM_INT_TYPE)CAPTURE_CHANNEL);
		_DBG("Time capture: ");
		_DBH32(TIM_GetCaptureValue(LPC_TIMER1,(TIM_COUNTER_INPUT_OPT)CAPTURE_CHANNEL));_DBG_("");
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
 * @brief		Main TIMER program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{

	//SystemInit();
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

	// Setup pin P7_3 (pin 16) for Cap 1.1 input
	scu_pinmux(0x7,3,MD_PLN_FAST, FUNC1);

	// Initialize timer 1, prescale count time of 1000000uS = 1S
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1000000;

	// use channel 0, CAPn.CAPTURE_CHANNEL
	TIM_CaptureConfigStruct.CaptureChannel = CAPTURE_CHANNEL;
	// Enable capture on CAPn.CAPTURE_CHANNEL rising edge
	TIM_CaptureConfigStruct.RisingEdge = ENABLE;
	// Enable capture on CAPn.CAPTURE_CHANNEL falling edge
	TIM_CaptureConfigStruct.FallingEdge = ENABLE;
	// Generate capture interrupt
	TIM_CaptureConfigStruct.IntOnCaption = ENABLE;


	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIMER1, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigCapture(LPC_TIMER1, &TIM_CaptureConfigStruct);
	TIM_ResetCounter(LPC_TIMER1);


	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER1_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER1_IRQn);
	// To start timer 1
	TIM_Cmd(LPC_TIMER1,ENABLE);

	while (1);
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

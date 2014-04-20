/**********************************************************************
* $Id$		Nvic_VectorTableRelocation.c			2011-06-02
*//**
* @file		Nvic_VectorTableRelocation.c
* @brief	This example used to test NVIC Vector Table relocation
* 			function
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
#include <stdio.h>
#include "debug_frmwrk.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_nvic.h"
#include "lpc43xx_timer.h"
#include "lpc43xx_libcfg.h"

#include <string.h>
/* Example group ----------------------------------------------------------- */
/** @defgroup Nvic_VectorTableRelocation	Nvic_VectorTableRelocation
 * @ingroup NVIC_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS ***********************/
/* Vector Table Offset */
#define VTOR_OFFSET		0x20000000

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"Privileged demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART0 - 115200 bps \n\r"
	"This example used to test NVIC Vector Table Relocation function\n\r"
	"********************************************************************************\n\r";
//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
//uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;
//FunctionalState LEDStatus = ENABLE;

/************************** PRIVATE FUNCTIONS *************************/
/* Interrupt service routine */
void TIMER0_IRQHandler(void);
void print_menu(void);
extern uint32_t getPC(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		TIMER0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TIMER0_IRQHandler(void)
{
	if (TIM_GetIntStatus(LPC_TIMER0, TIM_MR0_INT)== SET)
	{
		_DBG_("Match interrupt occur...");
	}
	TIM_ClearIntPending(LPC_TIMER0, TIM_MR0_INT);
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

	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();

	_DBG("Remapping Vector Table at address: ");

	_DBH32(VTOR_OFFSET); _DBG_("");

	NVIC_SetVTOR(VTOR_OFFSET);

	/* Copy Vector Table from initialized address to new address
	 * In Hitex Flash mode: Vector Table will be initialized at 0x1C000000
	 * In Internal SRAM mode: Vector Table will be initialized at 0x10000000
	 * In SPIFI 64MB Debug mode: Vector Table will be initialized at 0x14000000
	 * In SPIFI 128MB mode: Vector Table will be initialized at 0x80000000
	 * Aligned: 256 words
	 */

  memcpy((void *)VTOR_OFFSET, (const void *)(getPC()& 0xFF000000), 256*4);

	_DBG_("If Vector Table remapping is successful, a message will be printed every second...");

	// Initialize timer 0, prescale count time of 100uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 100;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz)
	TIM_MatchConfigStruct.MatchValue   = 10000;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIMER0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIMER0,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);
	// To start timer 0
	TIM_Cmd(LPC_TIMER0,ENABLE);

	while (1);
}

/* With ARM and GHS toolsets, the entry point is main() - this will
 allow the linker to generate wrapper code to setup stacks, allocate
 heap area, and initialize and copy code and data segments. For GNU
 toolsets, the entry point is through __start() in the crt0_gnu.asm
 file, and that startup code will setup stacks and data */
int main(void) {
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

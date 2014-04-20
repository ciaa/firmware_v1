/**********************************************************************
* $Id$		Rit_Interrupt.c			2011-06-02
*//**
* @file		Rit_Interrupt.c
* @brief	This example used RIT to generate interrupt each 1s
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
#include "lpc43xx_rit.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Rit_Interrupt	Rit_Interrupt
 * @ingroup RIT_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS ***********************/

#define LED1_PORT		4
#define LED1_BIT		1

#define TIME_INTERVAL 	1000

/************************** PRIVATE VARIABLE ***********************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	" RIT demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART1 - 115200 bps \n\r"
	" Use RIT as a timer to generate interrupt to turn on/off LED each 1s \n\r"
	"********************************************************************************\n\r";
FunctionalState LEDStatus = ENABLE;

/************************** PRIVATE FUNCTION *************************/
void RIT_IRQHandler(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RIT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void RIT_IRQHandler(void)
{
	RIT_GetIntStatus(LPC_RITIMER); //call this to clear interrupt flag
	if(LEDStatus == ENABLE)
	{
		LEDStatus = DISABLE;
		//turn off LED
		GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));
	}
	else
	{
		LEDStatus = ENABLE;
		//turn off LED
		GPIO_SetValue(LED1_PORT,(1<<LED1_BIT));
	}
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main RIT program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void) {

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
	 _DBG(menu);

	RIT_Init(LPC_RITIMER);
	/* Configure time_interval for RIT
	 * In this case: time_interval = 1000 ms = 1s
	 * So, RIT will generate interrupt each 1s
	 */
	RIT_TimerConfig(LPC_RITIMER,TIME_INTERVAL);

	_DBG("The time interval is: ");
	_DBD32(TIME_INTERVAL); _DBG_(" millisecond..");

	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// USB_IND1 LED

	/* Using LED for testing */
	FIO_SetDir(LED1_PORT,(1<<LED1_BIT),1);
	FIO_SetValue(LED1_PORT,(1<<LED1_BIT));
	NVIC_EnableIRQ(RITIMER_IRQn);

	while(1);
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

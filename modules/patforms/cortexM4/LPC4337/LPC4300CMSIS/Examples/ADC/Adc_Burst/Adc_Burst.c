/**********************************************************************
* $Id$		Adc_Burst.c		2011-06-02
*//**
* @file		Adc_Burst.c
* @brief	This example describes how to use ADC conversion in
* 			burst mode
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
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc43xx_adc.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Adc_Burst		Adc_Burst
 * @ingroup ADC_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS ***********************/

#define _ADC_INT_1			ADC_ADINTEN1
#define _ADC_CHANNEL_1	ADC_CHANNEL_1


#define _ADC_INT_2			ADC_ADINTEN2
#define _ADC_CHANNEL_2	ADC_CHANNEL_2


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" ADC burst demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM CORTEX-M4 \n\r"
"\t - Communicate via: UART0 - 115200 bps \n\r"
" Use ADC with 12-bit resolution rate of 200KHz, running burst mode (single or multiple input)\n\r"
#if defined(HITEX_BOARD)
" Display ADC value via UART0\n\r"
#elif defined(KEIL_BOARD)
" Display ADC value via UART3\n\r"
#endif
" Turn the potentiometer to see how ADC value changes\n\r"
"********************************************************************************\n\r";


/************************** PRIVATE FUNCTION *************************/
void print_menu(void);

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
 * @brief		Main ADC program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{

	uint32_t tmp;
	uint32_t adc_value;
	LPC_SCU_Type *p;

	SystemInit();
	CGU_Init();
	/* Initialize debug via UART
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();
	/* ADC initialize
	 * - ADC conversion rate: 200K
	 * - Channel: 2
	 */
	ADC_Init(LPC_ADC0, 200000, 10);
	ADC_ChannelCmd(LPC_ADC0,_ADC_CHANNEL_1,ENABLE);
	ADC_ChannelCmd(LPC_ADC0,_ADC_CHANNEL_2,ENABLE);

	//Start burst conversion
	ADC_BurstCmd(LPC_ADC0,ENABLE);
	
	//Select function ADC1_1
	p =   (LPC_SCU_Type*)(LPC_SCU_BASE);
	p->ENAIO1 |= 0x02;
	//Select function ADC1_2
	p->ENAIO1 |= 0x04;
	while(1)
	{
		adc_value =  ADC_ChannelGetData(LPC_ADC0,ADC_CHANNEL_1);
		_DBG("ADC value on channel 1 (Keil board potentiometer): ");
		_DBD32(adc_value);
		_DBG_("");

		adc_value =  ADC_ChannelGetData(LPC_ADC0,ADC_CHANNEL_2);
		_DBG("ADC value on channel 2 (Hitex board potentiometer): ");
		_DBD32(adc_value);
		_DBG_("");

		// Wait for a while
		for(tmp = 0; tmp < 1500000; tmp++);
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

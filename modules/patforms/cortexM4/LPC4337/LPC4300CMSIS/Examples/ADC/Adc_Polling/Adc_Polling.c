/**********************************************************************
* $Id$		Adc_Polling.c		2011-06-02
*//**
* @file		Adc_Polling.c
* @brief	This example describes how to use ADC conversion in polling mode
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
#include "lpc43xx_adc.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Adc_Polling	Adc_Polling
 * @ingroup ADC_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS ***********************/
#if defined(HITEX_BOARD)
#define _ADC_INT		ADC_ADINTEN2
#define _ADC_CHANNEL	ADC_CHANNEL_2
#elif defined(KEIL_BOARD)
#define _ADC_INT		ADC_ADINTEN1
#define _ADC_CHANNEL	ADC_CHANNEL_1
#endif


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" ADC demo \n\r"
"\t - MCU: LPC4300 \n\r"
"\t - Core: ARM CORTEX-M4 \n\r"
"\t - Communicate via: UART - 115200 bps \n\r"
" Use ADC with 10-bit resolution rate of 200KHz, read in polling mode\n\r"
" To get ADC value and display via UART\n\r"
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
	volatile uint32_t adc_value, tmp;
	LPC_SCU_Type *p;

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

	/* ADC initialize */
	ADC_Init(LPC_ADC0, 200000, 10);
	ADC_IntConfig(LPC_ADC0,_ADC_INT,DISABLE);
	ADC_ChannelCmd(LPC_ADC0,_ADC_CHANNEL,ENABLE);

	//Select function ADC1_2
	p =   (LPC_SCU_Type*)(LPC_SCU_BASE);
	p->ENAIO1 |= 0x04;
	
	while(1)
	{
		// Start conversion
		ADC_StartCmd(LPC_ADC0,ADC_START_NOW);
		//Wait conversion complete
		while (!(ADC_ChannelGetStatus(LPC_ADC0,_ADC_CHANNEL,ADC_DATA_DONE)));
		adc_value = ADC_ChannelGetData(LPC_ADC0,_ADC_CHANNEL);
		//Display the result of conversion on the UART0
		lpc_printf("ADC value on channel %d: ",_ADC_CHANNEL);
		_DBD32(adc_value);
		_DBG_("");
		//delay
		for(tmp = 0; tmp < 1000000; tmp++);
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

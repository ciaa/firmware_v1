/**********************************************************************
* $Id$		Rtc_Calibration.c			2011-06-02
*//**
* @file		Rtc_Calibration.c
* @brief	This example describes how to calibrate real time clock.
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
#include "debug_frmwrk.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_rtc.h"
#include "lpc43xx_libcfg.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Rtc_Calibration	Rtc_Calibration
 * @ingroup RTC_Examples
 * @{
 */

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"RTC Calibration demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART1 - 115200 bps \n\r"
	"This example describes how to calibrate RTC \n\r"
	"********************************************************************************\n\r";

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);

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
	uint32_t cur_sec, pre_sec;

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

	/* In this example:
	 * Suppose that the RTC need periodically adjust after each 5 second.
	 * And the time counter need by incrementing the counter by 2 instead of 1
	 * We will observe timer counter after calibration via serial display
	 */
	// Init RTC module
	RTC_Init(LPC_RTC);

	//Set current time = 0
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	cur_sec = pre_sec = 0;
	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_Cmd(LPC_RTC, ENABLE);

	/* Setting Timer calibration
	 * Calibration value =  5s;
	 * Direction = Forward calibration
	 * So after each 5s, calibration logic can periodically adjust the time counter by
	 * incrementing the counter by 2 instead of 1
	 */
	RTC_CalibConfig(LPC_RTC, 5, RTC_CALIB_DIR_FORWARD);
	RTC_CalibCounterCmd(LPC_RTC, ENABLE);
	//RTC_Cmd(LPC_RTC, ENABLE);
    /* Loop forever */
    while(1)
    {
    	cur_sec = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_SECOND);
    	if(cur_sec != pre_sec)
    	{
    		if((cur_sec>pre_sec)&&((cur_sec - pre_sec)== 2))
    			_DBG("\n\rCalibrated!\n\r");
    		pre_sec = cur_sec;
    		_DBD(pre_sec);
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

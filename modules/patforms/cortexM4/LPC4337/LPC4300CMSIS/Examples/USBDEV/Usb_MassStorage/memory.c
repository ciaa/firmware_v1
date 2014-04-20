/**********************************************************************
* $Id$		memory.c				2011-06-02
*//**
* @file		memory.c
* @brief	This file contains the initialization of the USB Mass Storage Demo
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
#include <string.h>
#include "lpc43xx.H"                        /* LPC23xx definitions */
#include "lpc_types.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_uart.h"
#include <stdio.h>

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbuser.h"
#include "mscuser.h"

#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup USBDEV_USBMSC	USBMSC
 * @ingroup USBDEV_Examples
 * @{
 */

/***********************************************************************/
/* Select USB0 or USB1 device in usb.h                                 */

volatile uint32_t systicks=0;

extern DQH_T ep_QH[EP_NUM_MAX];
extern DTD_T ep_TD[EP_NUM_MAX];
extern uint32_t DevStatusFS2HS;

extern void USB_EndPoint0 (uint32_t event);


void IOInit(void);

/* Main Program */

int main (void) {
	LPC_USBDRV_INIT_T usb_cb;
	volatile uint32_t i=0, CNT_100ms=0, CNT_1S=0;

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

	/* initilize call back structures */
	memset((void*)&usb_cb, 0, sizeof(LPC_USBDRV_INIT_T));
	usb_cb.USB_Reset_Event = USB_Reset_Event;
	usb_cb.USB_P_EP[0] = USB_EndPoint0;
	usb_cb.USB_P_EP[1] = USB_EndPoint1;
	usb_cb.ep0_maxp = USB_MAX_PACKET0;

	/* USB Initialization */
	USB_Init(&usb_cb);
	USB_Connect(TRUE);                        /* USB Connect */


	while (1);                                /* Loop forever */

}


/*----------------------------------------------------------------------------
  SysTick IRQ: Executed periodically
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 500Hz
{
	static uint32_t cnt;
	systicks++;

	if ( cnt < systicks )	   					// 0.5s
	{
	  	cnt = systicks +50;

		if(LPC_GPIO_PORT->PIN[4]&(1<<14))
		  LPC_GPIO_PORT->CLR[4] = (1<<14);				// Toggle led
		else
		  LPC_GPIO_PORT->SET[4] = (1<<14);
	}
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

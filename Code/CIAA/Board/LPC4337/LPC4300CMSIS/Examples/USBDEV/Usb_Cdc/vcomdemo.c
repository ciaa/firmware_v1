/*----------------------------------------------------------------------------
 *      Name:    vcomdemo.c
 *      Purpose: USB virtual COM port Demo
 *      Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

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
#include "cdc.h"
#include "cdcuser.h"
#include "usbuser.h"

#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"
#include "serial.h"
#include "vcomdemo.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup USBDEV_USBCDC	USBCDC
 * @ingroup USBDEV_Examples
 * @{
 */
volatile uint32_t systicks=0;

extern DQH_T ep_QH[EP_NUM_MAX];
extern DTD_T ep_TD[EP_NUM_MAX];
extern uint32_t DevStatusFS2HS;

extern void USB_EndPoint0 (uint32_t event);

void IOInit(void);
/*----------------------------------------------------------------------------
 Initialises the VCOM port.
 Call this function before using VCOM_putchar or VCOM_getchar
 *---------------------------------------------------------------------------*/
void VCOM_Init(void) {
#if PORT_NUM
  CDC_Init (1);
#else
  CDC_Init (0);
#endif
}


/*----------------------------------------------------------------------------
  Reads character from serial port buffer and writes to USB buffer
 *---------------------------------------------------------------------------*/
void VCOM_Serial2Usb(void) {
#ifdef __ICCARM__
#pragma data_alignment=4
  static char serBuf [USB_CDC_BUFSIZE];
#else
#ifdef __GNUC__
#define __align(x) __attribute__((aligned(x)))
#endif
  __align(4) static char serBuf [USB_CDC_BUFSIZE];
#endif
         int  numBytesRead, numAvailByte;

  ser_AvailChar (&numAvailByte);
  if (numAvailByte > 0) {
    if (CDC_DepInEmpty) {
      numBytesRead = ser_Read (&serBuf[0], &numAvailByte);

      CDC_DepInEmpty = 0;
	  USB_WriteEP (CDC_DEP_IN, (unsigned char *)&serBuf[0], numBytesRead);
    }
  }

}

/*----------------------------------------------------------------------------
  Reads character from USB buffer and writes to serial port buffer
 *---------------------------------------------------------------------------*/
void VCOM_Usb2Serial(void) {
  #ifdef __ICCARM__
#pragma data_alignment=4
    static char serBuf [32];
#else
#ifdef __GNUC__
#define __align(x) __attribute__((aligned(x)))
#endif
  __align(4) static char serBuf [32];
#endif
         int  numBytesToRead, numBytesRead, numAvailByte;

  CDC_OutBufAvailChar (&numAvailByte);
  if (numAvailByte > 0) {
      numBytesToRead = numAvailByte > 32 ? 32 : numAvailByte;
      numBytesRead = CDC_RdOutBuf (&serBuf[0], &numBytesToRead);
#if PORT_NUM
      ser_Write (1, &serBuf[0], &numBytesRead);
#else
      ser_Write (0, &serBuf[0], &numBytesRead);
#endif
  }

}


/*----------------------------------------------------------------------------
  checks the serial state and initiates notification
 *---------------------------------------------------------------------------*/
void VCOM_CheckSerialState (void) {
         unsigned short temp;
  static unsigned short serialState;

  temp = CDC_GetSerialState();
  if (serialState != temp) {
     serialState = temp;
     CDC_NotificationIn();                  // send SERIAL_STATE notification
  }
}

/*----------------------------------------------------------------------------
  Main Program
 *---------------------------------------------------------------------------*/
int main (void) {

	LPC_USBDRV_INIT_T usb_cb;
	SystemInit();
	CGU_Init();
	IOInit();

	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	//debug_frmwrk_init();

	/* initilize call back structures */
	memset((void*)&usb_cb, 0, sizeof(LPC_USBDRV_INIT_T));
	usb_cb.USB_Reset_Event = USB_Reset_Event;
	usb_cb.USB_P_EP[0] = USB_EndPoint0;
	usb_cb.USB_P_EP[1] = USB_EndPoint1;
	usb_cb.USB_P_EP[2] = USB_EndPoint2;
	usb_cb.ep0_maxp = USB_MAX_PACKET0;
  VCOM_Init();                              // VCOM Initialization

  USB_Init(&usb_cb);                               // USB Initialization
  USB_Connect(TRUE);                        // USB Connect

  while (!USB_Configuration) ;              // wait until USB is configured

  while (1) {                               // Loop forever
    VCOM_Serial2Usb();                      // read serial port and initiate USB event
    VCOM_CheckSerialState();
	VCOM_Usb2Serial();
  } // end while
} // end main ()

void IOInit(void)
{
	scu_pinmux(0x9 ,2 , MD_PDN, FUNC2); 	// GPIO4_14: LD11
	scu_pinmux(0xA ,4 , MD_PDN, FUNC2); 	// GPIO4_11: LD10
	scu_pinmux(0x6 ,10 , MD_PUP, FUNC3);	// GPIO3_6: button 0
	scu_pinmux(0x4 ,0 , MD_PUP, FUNC0); 	// GPIO2_0: button 1

	LPC_GPIO_PORT->DIR[4] |= (1<<14);			// LD11 = output
	LPC_GPIO_PORT->DIR[4] |= (1<<11);			// LD11 = output
	LPC_GPIO_PORT->DIR[3] &= ~(1<<6);			// Button 0 = input
	LPC_GPIO_PORT->DIR[2] &= ~(1<<0);			// Button 1 = input
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

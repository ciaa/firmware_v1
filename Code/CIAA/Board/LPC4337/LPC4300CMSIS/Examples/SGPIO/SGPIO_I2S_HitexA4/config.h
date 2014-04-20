/*****************************************************************************
*   config.h:  Header file for LPC18xx/43xx Family Microprocessors
*
*   Copyright(C) 2012, NXP Semiconductor
*   All rights reserved.
*
*
******************************************************************************
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
******************************************************************************/

#ifndef __CONFIG_H 
#define __CONFIG_H

#include <stdint.h>

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define RTC_CLK		(   32768UL)	/* RTC oscillator frequency           */
#define IRC_OSC		(12000000UL)	/* Internal RC oscillator frequency   */
#define IRC_TRIM_VAL 0x34A			/* IRC trim value for 12MHz output    */
#define XTAL_FREQ	(12000000UL)	/* Frequency of external xtal */
#define EXT_FREQ	(12000000UL)	/* Frequency of external clock on EXT_TCK, ENET_RX_CLK or ENET_TX_CLK */

/*----------------------------------------------------------------------------
  Retarget selection
 *----------------------------------------------------------------------------*/

typedef enum {
	RETARGET_USART0	= 0,
	RETARGET_UART1	= 1,
	RETARGET_USART2	= 2,
	RETARGET_USART3	= 3
} RETARGET_Type;

#define RETARGET_UART_BUFSIZE		0x40
extern volatile uint32_t UART0Count;
extern volatile uint8_t UART0Buffer[RETARGET_UART_BUFSIZE];
extern volatile uint32_t UART1Count;
extern volatile uint8_t UART1Buffer[RETARGET_UART_BUFSIZE];
extern volatile uint32_t UART2Count;
extern volatile uint8_t UART2Buffer[RETARGET_UART_BUFSIZE];
extern volatile uint32_t UART3Count;
extern volatile uint8_t UART3Buffer[RETARGET_UART_BUFSIZE];
extern void RetargetInit(RETARGET_Type retarget, uint32_t baudrate);

/*----------------------------------------------------------------------------
  Board selection
 *----------------------------------------------------------------------------*/
//Eagle validation board with BGA256 socket 
//This board setup will be selected when none of the following defines are applied.

//Eagle validation board with BGA504 socket 
//#define USE_BGA504 

//12b vADC validation board
//#define USE_ADC_VAL

//Falcon/Typhoon characterization/test board with BGA256 socket
//#define USE_TEST256

//NXP LPC1800/LPC4300 evaluation board
//#define USE_NXP_EVAL

//Hitex LPC1850 evaluation board
#define USE_HITEX_LPC1850_EVAL

#endif /* end __CONFIG_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

/**********************************************************************
* $Id$		config.h			2011-06-02
*//**
* @file		config.h
* @brief	Configuration file
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
#ifndef __CONFIG_H
#define __CONFIG_H

/**********************************************************************
 ** Define clocks
**********************************************************************/
#define RTC_CLK     (32768UL)		/* RTC oscillator frequency 			*/
#define IRC_OSC     (12000000UL)    /* Internal RC oscillator frequency 	*/
#define IRC_TRIM_VAL 0x350 			/* IRC trim value for 12MHz output 		*/
#define XTAL_FREQ	(10000000UL)	/* Frequency of external xtal 			*/
#define USE_XTAL	1				/* Select clock source 1=XTAL, 0=IRC 	*/

/**********************************************************************
 ** Board selection
**********************************************************************/
//#define USE_NXP_EVAL

/**********************************************************************
 ** USB defines
**********************************************************************/
#define USE_USB0  1
#define USB_MAX_IF_NUM  8
#define USB_MAX_EP_NUM  6
#define USB_MAX_PACKET0 64
#define HID_EP_IN   0x82
#define HID_EP_OUT  0x02

#define HID_RPRT_DESC_SZ  0x21

#endif /* end __CONFIG_H */

/**********************************************************************
 **                            End Of File
 **********************************************************************/

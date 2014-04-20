/*****************************************************************************
 * $Id:: sgpio_i2s.h
 *
 * Project: USB Audio Example for LPC18xx and LPC43xx
 *
 * Description: Configuration of six SGPIOs to construct an I2S interface
 *              with 4 stereo channels
 *
 *----------------------------------------------------------------------------
 *  Copyright(C) 2012, NXP Semiconductor
 *  All rights reserved.
 *
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
 *****************************************************************************/
#ifndef SGPIO_I2S_H
#define SGPIO_I2S_H

////////////////////////////////////////////////////////////////////////////////
//
//	start of bte's hacks
//
//  basically I'm going to let the i2s run in parallel gathering data from USB
//  then grabing a copy of that data into a circular buffer for the sGPIO interrupt
//
//
#define SGPIO_BUF_SIZE	128			// has to be 2 ^ x   space
#define SGPIO_BUF_MASK	(SGPIO_BUF_SIZE-1)

#include "adc_user.h"

int doRBIT(int);		// sGPIO does LSB first, need to flip it -- this is in startup.S


void SGPIO_IRQHandler(void) ;
	
void SGPIO_Codec_IRQHandler(USB_ADC_CTRL_T* pAdcCtrl) ;

////////////////////////// setup sGPIO ///////////////////////
//
//
void setupSGPIO(void) ;

//
//  bte hacks
//
//////////////////////////////////////////////////////////////////////////////////////////

#endif

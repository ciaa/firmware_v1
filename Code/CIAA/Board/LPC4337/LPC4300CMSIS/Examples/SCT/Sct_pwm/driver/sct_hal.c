/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		sct_hal.c
* @brief	implementation of the HAL for the SCT
* @version	1.0
* @date		19. June. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
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
#include "sct_hal.h"

volatile intCallback_t risingEdgeCallbackFunction;
volatile intCallback_t fallingEdgeCallbackFunction;
volatile intCallback_t periodCallbackFunction;

void SCT_IRQHandler(void) {
	
	uint32_t flagRegister = LPC_SCT->EVFLAG;
	uint16_t activeInterrupts = LPC_SCT->EVEN;
	
	// need to check which interrupts are active since the flags will always be set when the
	// events are fired
	if(flagRegister & RISING_INT & activeInterrupts) {
			
		/* quit the interrupt */
		LPC_SCT->EVFLAG = RISING_INT;
		
		/* call the interrupt callback function */
		(*risingEdgeCallbackFunction)();
	}
	
	if(flagRegister & FALLING_INT & activeInterrupts) {
			
		/* quit the interrupt */
		LPC_SCT->EVFLAG = FALLING_INT;
		
		/* call the interrupt callback function */
		(*fallingEdgeCallbackFunction)();
	}

	if(flagRegister & PERIOD_INT & activeInterrupts) {
			
		/* quit the interrupt */
		LPC_SCT->EVFLAG = PERIOD_INT;
		
		/* call the interrupt callback function */
		(*periodCallbackFunction)();
	}

}


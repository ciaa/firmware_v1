/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_debugOutput.c
* @brief	implementation of a debug event used to mark the period begin 
* 			on SCT_OUT 15 
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

void makeDebugOutput(void) {

	SCT_eventCntrlReg eventCntrl;
	
	SCT_setOutputForEvent(EV_DEBUG_UP, OUT_DEBUG_UP);
	SCT_clearOutputForEvent(EV_DEBUG_DOWN, OUT_DEBUG_DOWN);
	SCT_setMatchRegister(MATCHREG_DEBUG_UP, 0);
	SCT_setMatchRegister(MATCHREG_DEBUG_DOWN, 1);	
	SCT_setMatchReloadRegister(MATCHREG_DEBUG_UP, 0);
	SCT_setMatchReloadRegister(MATCHREG_DEBUG_DOWN, 1);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_DEBUG_UP, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_DEBUG_UP, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_DEBUG_UP, SCT_SMASK_0);                  

	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_DEBUG_DOWN, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_DEBUG_DOWN, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_DEBUG_DOWN, SCT_SMASK_0);                  

}




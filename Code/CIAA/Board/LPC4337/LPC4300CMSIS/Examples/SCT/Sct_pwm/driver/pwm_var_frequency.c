/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_singlePhase.c
* @brief	configuration modules for single phase mode 
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

#include "sct_pwm.h"


extern volatile intCallback_t risingEdgeCallbackFunction;
extern volatile intCallback_t fallingEdgeCallbackFunction;
extern volatile intCallback_t periodCallbackFunction;
static uint16_t	fixedDutyCycle;

PWM_STATUS makePwmVarFrequencyConfig(PWM_VarFrequency* config) {
	
	
	uint16_t channelIdx, i, channelMask, activeInterrupts;
	PWM_STATUS status;
	uint32_t matchClear, matchPeriod;
	SCT_eventCntrlReg eventCntrl;
	PwmClockFrequency pwmFreq;
	
	// save duty cycle for later use
	fixedDutyCycle = config->dutyCycle;
	
	// setup output channels to be driven
	channelIdx = config->outputMask;
	channelMask = 0x1;
	
	// iterate on all 16 outputs, configure which ones are toggled
	for(i=0;i<16;i++){
				
		// configure output if active
		if(channelIdx & channelMask) {
			
			// now configure for the polarity
			if(config->polarity == ACTIVE_HIGH) {
		
				// rising edge event sets the output
				// falling edge event clears the output						
				SCT_setOutputForEvent(EV_RISING, (SCT_OUT) i);
				SCT_clearOutputForEvent(EV_PERIOD, (SCT_OUT) i);
				SCT_clearOutputForEvent(EV_FALLING, (SCT_OUT) i);
				SCT_setOutput((SCT_OUT) i);
				// in case an event is simultaneous, clear output
				// for rising and period end
				SCT_setConflictReg((SCT_OUT) i, SETCLR_CLR);
				
			} else {

				// rising edge event clears the output
				// falling edge event sets the output
				SCT_clearOutputForEvent(EV_RISING, (SCT_OUT) i);
				SCT_setOutputForEvent(EV_PERIOD, (SCT_OUT) i);
				SCT_setOutputForEvent(EV_FALLING, (SCT_OUT) i);
				SCT_clrOutput((SCT_OUT) i);
				// in case an event is simultaneous, set output
				// for rising and period end
				SCT_setConflictReg((SCT_OUT) i, SETCLR_SET);
			};
		}
		
		channelMask = channelMask << 1;	
	}
			
	// setup match registers for PWM waveform
			
	///////////////////////////////////////////////////////////////
	// period event
	///////////////////////////////////////////////////////////////
	
	// configure match values
	status = SCT_pwmIoctl(GET_PWM_FREQUENCY, &pwmFreq);
	matchPeriod = pwmFreq / (config->period);
	SCT_setMatchRegister(MATCHREG_PERIOD, matchPeriod-1);
	SCT_setMatchReloadRegister(MATCHREG_PERIOD, matchPeriod-1);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_PERIOD, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_PERIOD, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_PERIOD, SCT_SMASK_0);                  

	// define this event for limiting the timer
	SCT_setLimitEvent(EV_PERIOD);

	///////////////////////////////////////////////////////////////
	// rising edge event
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_RISING, 0);
	SCT_setMatchReloadRegister(MATCHREG_RISING, 0);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING, SCT_SMASK_0);                  
	
	///////////////////////////////////////////////////////////////
	// falling edge event
	///////////////////////////////////////////////////////////////
	
	// duty cycle is expressed in per mil
	matchClear = (matchPeriod*fixedDutyCycle) / 1000;
	
	SCT_setMatchRegister(MATCHREG_FALLING, matchClear);
	SCT_setMatchReloadRegister(MATCHREG_FALLING, matchClear);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_FALLING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING, SCT_SMASK_0);                  
			
	// plug the interrupt functions 
	risingEdgeCallbackFunction = config->risingEdgeInt;
	fallingEdgeCallbackFunction = config->fallingEdgeInt;	
	periodCallbackFunction = config->periodInt;
	activeInterrupts = config->interruptMask;
	
	// setup which interrupts shall be driven
	SCT_clearEventInterrupts(activeInterrupts);	
	SCT_setEventInterrupts(activeInterrupts);
	
	// enable IRQ at NVIC level
	NVIC_EnableIRQ(SCT_IRQn);
	
	status = PWM_OK;
	return(status);
}

extern PWM_STATUS changePwmFrequency(uint32_t freq) {

	PWM_STATUS status;
	PwmClockFrequency pwmFreq;
	uint32_t newMatchClear, newMatchPeriod;

	status = SCT_pwmIoctl(GET_PWM_FREQUENCY, &pwmFreq);
	
	// need to have at least two ticks in a period to toggle
	if(freq >= (pwmFreq >>2)) {
	
		status = PWM_INVALID_FREQ;
		return(status);
	}
	
	newMatchPeriod = pwmFreq / freq;

	// duty cycle is expressed in per mil
	newMatchClear = (newMatchPeriod*fixedDutyCycle) / 1000;

	if(newMatchClear != 0) {
	
		status = SCT_pwmIoctl(BLOCK_RELOAD, 0);
	
		SCT_setMatchReloadRegister(MATCHREG_PERIOD, newMatchPeriod-1);
		SCT_setMatchReloadRegister(MATCHREG_FALLING, newMatchClear);	
	
		status = SCT_pwmIoctl(ENABLE_RELOAD, 0);

		status = PWM_OK;
	}
	else {
		status = PWM_INVALID_DC;
	};
	
	return(status);
}





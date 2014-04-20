/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_pulse_skipping.c
* @brief	configuration modules for pulse skipping mode 
* @version	1.0
* @date		18. July. 2012
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

PWM_STATUS makePwmPulseSkippingConfig(PWM_PulseSkipping* config) {
	
	
	uint16_t channelIdx, i, channelMask, dutyCycle, activeInterrupts;
	PWM_STATUS status;
	uint32_t matchSet, matchClear, matchPeriod;
	SCT_eventCntrlReg eventCntrl;
	PwmClockFrequency pwmFreq;
	
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
				SCT_clearOutputForEvent(EV_FALLING, (SCT_OUT) i);
				SCT_clearOutputForEvent(EV_PERIOD, (SCT_OUT) i);
				
				SCT_setOutput((SCT_OUT) i);
				
				// in case an event is simultaneous, clear output
				SCT_setConflictReg((SCT_OUT) i, SETCLR_CLR);
				
				// this event clears the output in sync with the period boundary
				SCT_clearOutputForEvent(EV_START_SKIP, (SCT_OUT) i);								
				
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
				
				// this event disables the outputs in sync with the period boundary				
				SCT_setOutputForEvent(EV_START_SKIP, (SCT_OUT) i);								
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
		STATEV_DONTCARE,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_PERIOD, eventCntrl);
	
	// configure state mask, period event fires in all defined states
	SCT_setEventActiveState(EV_PERIOD, (SCT_STATE_MASK) 
		((1u << STATE_SKIP_OFF) | 
		(1u << STATE_SKIP_ON))
		);                  

	// define this event for limiting the timer
	SCT_setLimitEvent(EV_PERIOD);

	///////////////////////////////////////////////////////////////
	// rising edge event
	///////////////////////////////////////////////////////////////
	matchSet = 0; 
	SCT_setMatchRegister(MATCHREG_RISING, matchSet);
	SCT_setMatchReloadRegister(MATCHREG_RISING, matchSet);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_JUMP,
		STATE_SKIP_OFF,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING, (SCT_STATE_MASK) (1u << STATE_SKIP_OFF));                  
	
	///////////////////////////////////////////////////////////////
	// falling edge event
	///////////////////////////////////////////////////////////////
	dutyCycle = config->dutyCycle;
	
	// duty cycle is expressed in per mil
	matchClear = (matchPeriod*dutyCycle) / 1000;
	
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
		EVCTRL_STATELD_JUMP,
		STATE_SKIP_OFF,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING, (SCT_STATE_MASK) (1u << STATE_SKIP_OFF));                  
	

	///////////////////////////////////////////////////////////////
	// pulse skipping control
	///////////////////////////////////////////////////////////////
	/* these events need to have higher priority than the period event */
	/* to make a state change */

	/* this event lets the state machine jump into the inactive mode */
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_PERIOD, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_INPUT,
		(SCT_IoSel) config->asyncEnableInput,
		EVCTRL_IOCOND_LOW,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_JUMP,
		STATE_SKIP_ON,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_START_SKIP, eventCntrl);
	
	// configure state mask, event configured while in SKIP INACTIVE state
	SCT_setEventActiveState(EV_START_SKIP, (SCT_STATE_MASK) (1u << STATE_SKIP_OFF));                  
	
	/* this event lets the state machine jump back into the SKIP INACTIVE (normal) mode */
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_PERIOD, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_INPUT,
		(SCT_IoSel) config->asyncEnableInput,
		EVCTRL_IOCOND_HIGH,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_JUMP,
		STATE_SKIP_OFF,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_STOP_SKIP, eventCntrl);
	
	// configure state mask, event configured while in SKIP ACTIVE state
	SCT_setEventActiveState(EV_STOP_SKIP, (SCT_STATE_MASK) (1u << STATE_SKIP_ON));  

	SCT_setLimitEvent(EV_STOP_SKIP); 
	
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


PWM_STATUS changePwmPulseSkippingDc(uint16_t dc) {
	
	uint32_t matchPeriod, matchClear;
	
	if(dc > 1000) return(PWM_INVALID_TIMING);
	
	// get the actual period match	
	// add one sice the period N is set from 0 to N-1
	matchPeriod = SCT_getMatchReloadRegister(MATCHREG_PERIOD) + 1;
				
	// compute the new time match based on the duty cycle
	matchClear = (matchPeriod * dc) / 1000;
				
	SCT_setMatchReloadRegister(MATCHREG_FALLING, matchClear);
		
	return(PWM_OK);
}



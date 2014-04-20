/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_pulse_skipping_complementary.c
* @brief	configuration modules for pulse skipping complementary mode 
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

PWM_STATUS makePwmPulseSkippingComplementaryConfig(PWM_ComplementaryPulseSkipping* config) {
	
	
	uint16_t i, dutyCycle, activeInterrupts;
	PWM_STATUS status;
	uint32_t matchSet, matchClear, matchPeriod, matchSetDb, matchClearDb;

	SCT_eventCntrlReg eventCntrl;
	PwmClockFrequency pwmFreq;
	uint8_t phaseIndex;
	PWM_pulseSkipOutputPair* outputPair;
	
	// setup output channels to be driven
	phaseIndex = config->pwmOutputs.numPhases;
	outputPair = config->pwmOutputs.phasePairs;

	// configure match values
	status = SCT_pwmIoctl(GET_PWM_FREQUENCY, &pwmFreq);
	
	// subtract one to make for the exact frequency
	matchPeriod = (pwmFreq / (config->period));
	
	// match at the beginning of the cycle
	matchSet = 0; 

	// delayed match with db for rising edge event
	// convert from nanoseconds in timer ticks
	matchSetDb = config->risingEdgeDeadband; 

	// duty cycle is expressed 'per mil'
	dutyCycle = config->dutyCycle;
	
	// take into account the rising edge dead band as an offset shift
	matchClear = ((matchPeriod*dutyCycle) / 1000) + matchSetDb;

	// configure the matchClear for the complementary falling edge event
	matchClearDb = matchClear + (config->fallingEdgeDeadband);
	
	// check the preprogrammed limits to see if the sum fits in the period
	if(matchClearDb >= matchPeriod) {
		status = PWM_INVALID_TIMING;
		return(status);
	};

	// iterate on all 16 outputs, configure which ones are toggled
	for(i=0;i<phaseIndex;i++){
						
		// now configure for the polarity
		if(config->polarity == ACTIVE_HIGH) {
	
			// rising event: clear complementary output
			SCT_clearOutputForEvent(EV_RISING, outputPair->complementaryOutput);
			
			// rising event + db: set primary output
			SCT_setOutputForEvent(EV_RISING_DB, outputPair->directOutput);
			
			// falling event: clear primary output
			SCT_clearOutputForEvent(EV_FALLING, outputPair->directOutput);

			// falling event + db: set complementary output
			SCT_setOutputForEvent(EV_FALLING_DB, outputPair->complementaryOutput);
			
			// period event: clear complementary output
			//SCT_clearOutputForEvent(EV_PERIOD, outputPair->complementaryOutput);
			
			// preset the outputs
			SCT_clrOutput(outputPair->complementaryOutput);			
			SCT_setOutput(outputPair->directOutput);

			// in case an event is simultaneous, clear output
			// for rising and period end
			SCT_setConflictReg(outputPair->directOutput, SETCLR_CLR);
			SCT_setConflictReg(outputPair->complementaryOutput, SETCLR_SET);	

			// this event clears / sets the output in sync with the period boundary
			SCT_clearOutputForEvent(EV_START_SKIP, outputPair->directOutput);			
			SCT_setOutputForEvent(EV_START_SKIP, outputPair->complementaryOutput);						
			
			
		} else {

			// rising event: clear direct output
			SCT_clearOutputForEvent(EV_RISING, outputPair->directOutput);
			
			// rising event + db: set complementary output
			SCT_setOutputForEvent(EV_RISING_DB, outputPair->complementaryOutput);
			
			// falling event: clear complementary output
			SCT_clearOutputForEvent(EV_FALLING, outputPair->complementaryOutput);

			// falling event + db: set direct output
			SCT_setOutputForEvent(EV_FALLING_DB, outputPair->directOutput);
			
			// period event: clear direct output
			// SCT_clearOutputForEvent(EV_PERIOD, outputPair->directOutput);
			
			// preset the outputs
			SCT_setOutput(outputPair->complementaryOutput);			
			SCT_clrOutput(outputPair->directOutput);

			// in case an event is simultaneous, clear output
			// for rising and period end
			SCT_setConflictReg(outputPair->directOutput, SETCLR_SET);
			SCT_setConflictReg(outputPair->complementaryOutput, SETCLR_CLR);

			// this event disables the outputs in sync with the period boundary				
			SCT_setOutputForEvent(EV_START_SKIP, outputPair->directOutput);				
			SCT_clearOutputForEvent(EV_START_SKIP, outputPair->complementaryOutput);	

		};
		
		outputPair++;	// advance index
	}

	// setup match registers for PWM waveform
			
	///////////////////////////////////////////////////////////////
	// period event
	///////////////////////////////////////////////////////////////	
	// subtract one to make for the exact frequency
	SCT_setMatchRegister(MATCHREG_PERIOD, matchPeriod - 1 );
	SCT_setMatchReloadRegister(MATCHREG_PERIOD, matchPeriod - 1);
	
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
	
	// configure state mask
	SCT_setEventActiveState(EV_PERIOD, (SCT_STATE_MASK)
		((1u << STATE_SKIP_OFF) | 
		(1u << STATE_SKIP_ON))
		);                   

	// define this event for limiting the timer, fires in all defined states
	SCT_setLimitEvent(EV_PERIOD);
		
	///////////////////////////////////////////////////////////////
	// rising edge event
	///////////////////////////////////////////////////////////////
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
	// complementary rising edge event
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_RISING_DB, matchSetDb);
	SCT_setMatchReloadRegister(MATCHREG_RISING_DB, matchSetDb);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING_DB, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_JUMP,
		STATE_SKIP_OFF,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING_DB, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING_DB, (SCT_STATE_MASK) (1u << STATE_SKIP_OFF));                  

	///////////////////////////////////////////////////////////////
	// falling edge event
	///////////////////////////////////////////////////////////////	
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
	// complementary falling edge event
	///////////////////////////////////////////////////////////////	
	SCT_setMatchRegister(MATCHREG_FALLING_DB, matchClearDb);
	SCT_setMatchReloadRegister(MATCHREG_FALLING_DB, matchClearDb);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_FALLING_DB, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_DONTCARE,
		EVCTRL_IOSEL_DONTCARE,
		EVCTRL_IOCOND_DONTCARE,
		EVCTRL_IOCOMB_MATCH,
		EVCTRL_STATELD_JUMP,
		STATE_SKIP_OFF,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING_DB, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING_DB, (SCT_STATE_MASK) (1u << STATE_SKIP_OFF));                  


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


PWM_STATUS changePwmCompPulseSkippingDc(uint16_t dc) {

	uint32_t setDb, clearDb, matchClear, matchPeriod, matchClearDb;

	if(dc > 1000) return(PWM_INVALID_TIMING);
	
	// get the actual period match	
	// add one sice the period N is set from 0 to N-1
	matchPeriod = SCT_getMatchReloadRegister(MATCHREG_PERIOD) + 1;
	
	// first set event is always at zero
	setDb = SCT_getMatchReloadRegister(MATCHREG_RISING_DB);
	
	clearDb = SCT_getMatchReloadRegister(MATCHREG_FALLING_DB) - SCT_getMatchReloadRegister(MATCHREG_FALLING);

	// take into account the rising edge dead band as an offset shift
	matchClear = ((matchPeriod * dc) / 1000) + setDb;

	// configure the matchClear for the complementary falling edge event
	matchClearDb = matchClear + clearDb;
	
	// check the preprogrammed limits to see if the sum fits in the period
	if(matchClearDb >= matchPeriod) return(PWM_INVALID_TIMING);	
	
	// reload the clear registers			
	SCT_changeMode(SCT_NO_RELOAD);
	
	SCT_setMatchReloadRegister(MATCHREG_FALLING, matchClear);
	SCT_setMatchReloadRegister(MATCHREG_FALLING_DB, matchClearDb);	
	
	SCT_changeMode(SCT_MATCH_RELOAD);
		
	return(PWM_OK);
}



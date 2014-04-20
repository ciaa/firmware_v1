/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_push_pull_complementary.c
* @brief	configuration modules for push pull complementary mode 
* @version	1.0
* @date		16. July. 2012
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

PWM_STATUS makePwmPushPullComplementaryConfig(PWM_ComplementaryPushPull* config) {
	
	uint32_t matchClear, matchPeriod, matchSetDb, matchClearDb;
	uint16_t i, dutyCycle, activeInterrupts;
	uint8_t phaseIndex;
	PWM_STATUS status;		
	SCT_eventCntrlReg eventCntrl;
	PwmClockFrequency pwmFreq;
	PWM_pushPullOutputPair* outputPair;

	
	

	// configure match values
	status = SCT_pwmIoctl(GET_PWM_FREQUENCY, &pwmFreq);
	
	// subtract one to make for the exact frequency
	matchPeriod = (pwmFreq / (config->period));
	
	// match at the beginning of the cycle
	// matchSet is constant 0

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

	// setup output channels to be driven, first the odd channels
	phaseIndex = config->pwmPhasesOdd.numPhases;
	outputPair = config->pwmPhasesOdd.phasePairs;
	
	// iterate on the output pairs, configure which ones are toggled
	for(i=0;i<phaseIndex;i++){
				
		// now configure for the polarity
		if(config->polarity == ACTIVE_HIGH) {
	
			// rising event: clear complementary output
			SCT_clearOutputForEvent(EV_RISING_ODD, outputPair->complementaryOutput);
			
			// rising event + db: set primary output
			SCT_setOutputForEvent(EV_RISING_ODD_DB, outputPair->directOutput);
			
			// falling event: clear primary output
			SCT_clearOutputForEvent(EV_FALLING_ODD, outputPair->directOutput);

			// falling event + db: set complementary output
			SCT_setOutputForEvent(EV_FALLING_ODD_DB, outputPair->complementaryOutput);
			
			// period event: clear direct output
			SCT_clearOutputForEvent(EV_PERIOD_ODD, outputPair->directOutput);
			
			// preset the outputs
			SCT_clrOutput(outputPair->complementaryOutput);			
			SCT_setOutput(outputPair->directOutput);

			// in case an event is simultaneous, clear output
			// for rising and period end
			SCT_setConflictReg(outputPair->directOutput, SETCLR_CLR);
			SCT_setConflictReg(outputPair->complementaryOutput, SETCLR_SET);
			
		} else {

			// rising event: clear direct output
			SCT_clearOutputForEvent(EV_RISING_ODD, outputPair->directOutput);
			
			// rising event + db: set complementary output
			SCT_setOutputForEvent(EV_RISING_ODD_DB, outputPair->complementaryOutput);
			
			// falling event: clear complementary output
			SCT_clearOutputForEvent(EV_FALLING_ODD, outputPair->complementaryOutput);

			// falling event + db: set direct output
			SCT_setOutputForEvent(EV_FALLING_ODD_DB, outputPair->directOutput);
			
			// period event: clear complementary output
			SCT_clearOutputForEvent(EV_PERIOD_ODD, outputPair->directOutput);
			
			// preset the outputs
			SCT_setOutput(outputPair->complementaryOutput);			
			SCT_clrOutput(outputPair->directOutput);

			// in case an event is simultaneous, clear output
			// for rising and period end
			SCT_setConflictReg(outputPair->directOutput, SETCLR_SET);
			SCT_setConflictReg(outputPair->complementaryOutput, SETCLR_CLR);
		};
				
		outputPair++;	// advance index
	}
			
	// setup output channels to be driven, now for the even channels
	phaseIndex = config->pwmPhasesEven.numPhases;
	outputPair = config->pwmPhasesEven.phasePairs;
	
	// iterate on the output pairs, configure which ones are toggled
	for(i=0;i<phaseIndex;i++){
				
		// now configure for the polarity
		if(config->polarity == ACTIVE_HIGH) {
	
			// rising event: clear complementary output
			SCT_clearOutputForEvent(EV_RISING_EVEN, outputPair->complementaryOutput);
			
			// rising event + db: set primary output
			SCT_setOutputForEvent(EV_RISING_EVEN_DB, outputPair->directOutput);
			
			// falling event: clear primary output
			SCT_clearOutputForEvent(EV_FALLING_EVEN, outputPair->directOutput);

			// falling event + db: set complementary output
			SCT_setOutputForEvent(EV_FALLING_EVEN_DB, outputPair->complementaryOutput);
			
			// period event: clear direct output
			SCT_clearOutputForEvent(EV_PERIOD, outputPair->directOutput);
			
			// preset the outputs
			SCT_clrOutput(outputPair->complementaryOutput);			
			SCT_setOutput(outputPair->directOutput);

			// in case an event is simultaneous, clear output
			// for rising and period end
			SCT_setConflictReg(outputPair->directOutput, SETCLR_CLR);
			SCT_setConflictReg(outputPair->complementaryOutput, SETCLR_SET);
			
		} else {

			// rising event: clear direct output
			SCT_clearOutputForEvent(EV_RISING_EVEN, outputPair->directOutput);
			
			// rising event + db: set complementary output
			SCT_setOutputForEvent(EV_RISING_EVEN_DB, outputPair->complementaryOutput);
			
			// falling event: clear complementary output
			SCT_clearOutputForEvent(EV_FALLING_EVEN, outputPair->complementaryOutput);

			// falling event + db: set direct output
			SCT_setOutputForEvent(EV_FALLING_EVEN_DB, outputPair->directOutput);
			
			// period event: clear complementary output
			SCT_clearOutputForEvent(EV_PERIOD, outputPair->directOutput);
			
			// preset the outputs
			SCT_setOutput(outputPair->complementaryOutput);			
			SCT_clrOutput(outputPair->directOutput);

			// in case an event is simultaneous, clear output
			// for rising and period end
			SCT_setConflictReg(outputPair->directOutput, SETCLR_SET);
			SCT_setConflictReg(outputPair->complementaryOutput, SETCLR_CLR);
		};
				
		outputPair++;	// advance index
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

	// configure the signal to toggle every period
	SCT_clearOutputForEvent(EV_PERIOD, config->oddPeriod);
	SCT_setOutputForEvent(EV_PERIOD, config->oddPeriod);
	SCT_setConflictReg(config->oddPeriod, SETCLR_TOGGLE);
	
	// start from count zero, even period signalled with low level
	SCT_clrOutput(config->oddPeriod);
	
	///////////////////////////////////////////////////////////////
	// rising edge event odd
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_RISING, 0);
	SCT_setMatchReloadRegister(MATCHREG_RISING, 0);
	
	// configure event control
	// triggers when match is valid and oddPeriod signal is high	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_HIGH,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING_ODD, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING_ODD, SCT_SMASK_0);                  

	///////////////////////////////////////////////////////////////
	// complementary rising edge event odd
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_RISING_DB, matchSetDb);
	SCT_setMatchReloadRegister(MATCHREG_RISING_DB, matchSetDb);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING_DB, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_HIGH,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING_ODD_DB, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING_ODD_DB, SCT_SMASK_0);  
	
	///////////////////////////////////////////////////////////////
	// falling edge event odd
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_FALLING, matchClear);
	SCT_setMatchReloadRegister(MATCHREG_FALLING, matchClear);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_FALLING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_HIGH,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING_ODD, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING_ODD, SCT_SMASK_0);                  

	///////////////////////////////////////////////////////////////
	// complementary falling edge event odd
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_FALLING_DB, matchClearDb);
	SCT_setMatchReloadRegister(MATCHREG_FALLING_DB, matchClearDb);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_FALLING_DB, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_HIGH,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING_ODD_DB, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING_ODD_DB, SCT_SMASK_0);                  


	///////////////////////////////////////////////////////////////
	// rising edge event even
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_RISING, 0);
	SCT_setMatchReloadRegister(MATCHREG_RISING, 0);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_LOW,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING_EVEN, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING_EVEN, SCT_SMASK_0);                  

	///////////////////////////////////////////////////////////////
	// complementary rising edge event even
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_RISING_DB, matchSetDb);
	SCT_setMatchReloadRegister(MATCHREG_RISING_DB, matchSetDb);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_RISING_DB, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_LOW,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_RISING_EVEN_DB, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_RISING_EVEN_DB, SCT_SMASK_0);                  

	///////////////////////////////////////////////////////////////
	// falling edge event even
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_FALLING, matchClear);
	SCT_setMatchReloadRegister(MATCHREG_FALLING, matchClear);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_FALLING, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_LOW,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING_EVEN, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING_EVEN, SCT_SMASK_0);                  
			
	///////////////////////////////////////////////////////////////
	// complementary falling edge event even
	///////////////////////////////////////////////////////////////
	SCT_setMatchRegister(MATCHREG_FALLING_DB, matchClearDb);
	SCT_setMatchReloadRegister(MATCHREG_FALLING_DB, matchClearDb);
	
	// configure event control	
	eventCntrl = SCT_makeEventCntrlReg(
		MATCHREG_FALLING_DB, 
		EVCTRL_HEVENT_U,
		EVCTRL_OUTSEL_OUTPUT,
		(SCT_IoSel) config->oddPeriod,
		EVCTRL_IOCOND_LOW,
		EVCTRL_IOCOMB_AND,
		EVCTRL_STATELD_DONTCARE,
		STATEV_0,
		EVCTRL_MATCHMEM_DONTCARE,
		EVCTRL_EVENTDIR_DONTCARE);
			
	SCT_writeEventCntrlReg(EV_FALLING_EVEN_DB, eventCntrl);
	
	// configure state mask
	SCT_setEventActiveState(EV_FALLING_EVEN_DB, SCT_SMASK_0); 			


			
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


PWM_STATUS changePwmPushPullComplementaryDc(uint16_t dc) {
	
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



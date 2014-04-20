/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		sct_pwm.c
* @brief	API functions implementation for the SCT PWM emulation
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

/* local variables for keeping the state */
static PwmClockFrequency 	pwmFreq;
static SctClockFrequency 	sctIpFreq;
static PwmType				pwmMode;

/* initialize the emulated PWM peripheral */
PWM_STATUS SCT_pwmInit(PwmGlobalParams* config) {

	SCT_CfgReg 		cfgReg;
	SCT_CntrlReg	cntrlReg;
	uint32_t		prescaler;
	
	// disable IRQ at NVIC level
	NVIC_DisableIRQ(SCT_IRQn);

	// reset the block
	SCT_changeMode(SCT_RESET);	

	/* quit any outstanding interrupt sources */
	LPC_SCT->EVFLAG = RISING_INT | FALLING_INT | PERIOD_INT;

	// clear pending IRQ at NVIC level
	NVIC_ClearPendingIRQ(SCT_IRQn);
	
	/* keep track of the frequency for later use */
	pwmFreq = config->pwmFrequency;
	sctIpFreq = config->sctFrequency;
	prescaler = sctIpFreq / pwmFreq;
	if(prescaler > 256) return(PWM_INVALID_BASE_FREQUENCY);
	
	
	// reset to invalid type, not opened yet
	pwmMode = INVALID_TYPE;
	
	cfgReg = SCT_makeCfgReg(
		CONFIG_UNIFIED,
		config->clock,
		config->clockIn,
		CONFIG_NORELOAD_DEFAULT,
		CONFIG_NORELOAD_DEFAULT,
		CONFIG_INSYC_DEFAULT		
	);
	
	SCT_writeCfgReg(cfgReg);

	/* only the L is used since counter is always unified mode */
	cntrlReg = SCT_makeCntrlReg(
		CTRL_STOP_DEFAULT,
		CTRL_HALT_DEFAULT,
		CTRL_CLRCNTR_ENABLE,
		CTRL_BIDIR_DEFAULT,
		(SCT_Prescale)(prescaler - 1),
		CTRL_STOP_DEFAULT,
		CTRL_HALT_DEFAULT,
		CTRL_CLRCNTR_ENABLE,
		CTRL_BIDIR_DEFAULT,
		SCT_PRESCALE_DEFAULT);
	
	SCT_writeCntrlReg(cntrlReg);
	
	return(PWM_OK);
}


/* definitions of the configuration functions */


#define DEBUG_MODE

#ifdef DEBUG_MODE

extern void makeDebugOutput(void);

#endif

/* open the simulated PWM interface */
PWM_STATUS SCT_pwmOpen(PwmType mode, const void* config) {
	
	PWM_STATUS status = PWM_INVALID_CONFIGURATION;
	
	#ifdef DEBUG_MODE
	makeDebugOutput();
	#endif
	switch(mode) {
		case SINGLE_PHASE:
					
			status = makePwmSinglePhaseConfig((PWM_SinglePhase*)config);
			break;
	
		case COMP_SINGLE_PHASE:
			
			status = makePwmSinglePhaseComplementaryConfig((PWM_ComplementarySinglePhase*)config);
			break;
		
		case PUSH_PULL:
			
			status = makePwmPushPullConfig((PWM_PushPull*)config);
			break;

		case COMP_PUSH_PULL:
			status = makePwmPushPullComplementaryConfig((PWM_ComplementaryPushPull*)config);
			break;

		case PULSE_SKIPPING:
			status = makePwmPulseSkippingConfig((PWM_PulseSkipping*)config);
			break;
		
		case VARIABLE_F_FIXED_DC:
			status = makePwmVarFrequencyConfig((PWM_VarFrequency*)config);
			break;

		case COMP_PULSE_SKIPPING:
			status = makePwmPulseSkippingComplementaryConfig((PWM_ComplementaryPulseSkipping*)config);
			break;
		
		/* not supported yet */

		default:
			
			status = PWM_INVALID_CONFIGURATION;
			break;
	}	

	// keep track of the mode
	if(status == PWM_OK) pwmMode = mode;
	return(status);
}


/* close the simulated PWM interface */
void SCT_pwmClose(void) {

	// reset the block
	SCT_changeMode(SCT_RESET);	
	
	pwmFreq = 0;
	sctIpFreq = 0;
	
	// reset to invalid type
	pwmMode = INVALID_TYPE;
	
};




/* modify PWM configuration parameters dynamically */
PWM_STATUS SCT_pwmIoctl(const PwmIoctl cmd, void* param) {

	uint32_t paramCast;
	PWM_STATUS status = PWM_INVALID_COMMAND;
	
	switch(cmd) {
		
		case GET_PWM_FREQUENCY:
			
			*((PwmClockFrequency*)param) = pwmFreq;
			status = PWM_OK;
		
		break;
		
		case START_PWM:
			
			SCT_changeMode(SCT_SYNC_STOP_REMOVE);
			SCT_changeMode(SCT_SYNC_HALT_REMOVE);
			SCT_changeMode(SCT_RUN);		
			status = PWM_OK;
		
		break;
		
		case SYNC_STOP_PWM:
			
			SCT_changeMode(SCT_SYNC_STOP);
			break;
		
		case STOP_PWM:
			
			SCT_changeMode(SCT_STOP);		
			status = PWM_OK;
		
		case SYNC_HALT_PWM:
			
			SCT_changeMode(SCT_SYNC_HALT);
			break;
		
		case HALT_PWM:
			
			SCT_changeMode(SCT_HALT);		
			status = PWM_OK;

		break;

		case SET_DC:
			
			switch(pwmMode) {
				
				case SINGLE_PHASE:					
					status = changePwmSinglePhaseDc(*((uint16_t *)param));					
				break;
				
				case COMP_SINGLE_PHASE:
					status = changePwmCompSinglePhaseDc(*((uint16_t *)param));
				break;
				
				case PUSH_PULL:					
					status = changePwmPushPullDc(*((uint16_t *)param));
				break;
				
				case COMP_PUSH_PULL:					
					status = changePwmPushPullComplementaryDc(*((uint16_t *)param));
				break;

				case PULSE_SKIPPING:					
					status = changePwmPulseSkippingDc(*((uint16_t *)param));
				break;
				
				case VARIABLE_F_FIXED_DC:
					// not supported
					break;				
			};
			
		break;
			
		case SET_FREQ:
			
			switch(pwmMode) {
						
				case VARIABLE_F_FIXED_DC:
					status = changePwmFrequency(*((uint32_t *)param));
					break;				
			};
			
		break;

		case ENABLE_INT:
			
			// cast to uint16_t interrupt mask
			paramCast = *((uint16_t *)param);
			
			// apply new mask
			SCT_setEventInterrupts(paramCast);
		
			status = PWM_OK;
		
		break;	
		
		case BLOCK_RELOAD:
		
			SCT_changeMode(SCT_NO_RELOAD);
		 
		break;
		
		case ENABLE_RELOAD:
		
			SCT_changeMode(SCT_MATCH_RELOAD);
		 
		break;
		
	};
	
	return(status);
}

















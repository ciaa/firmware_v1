/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		sct_pwm_test.c
* @brief	This example shows some test cases for PWM generation 
*			using the SCT
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

#include <stdlib.h>

/* include the SCT PWM emulation api */
#include "sct_pwm.h"

/* simple BSP utilities */
#include "mcb4300_lpc4350.h"



/* global configuration related to clocks and clock sources */
PwmGlobalParams globalConfig = {
	
	/* which clock is used */
	.clock = CONFIG_CLKMOD_INTERNAL,	
	
	/* which pin is sampled if not from INTERNAL_BUS */
	.clockIn = CONFIG_IN_0_DONTCARE,	
	
	/* at which base frequency the PWM waveform should be produced */
	.pwmFrequency = 2000000,			
	
	/* reference clock (internal) for the SCT */
	.sctFrequency = 180000000			
};




/* prototypes of the interrupt handling functions */
void fallingEdgeCallback(void);
void risingEdgeCallback(void);
void periodCallback(void);
	
/**********************************************************************
 * configuration for a single phase test 
 **********************************************************************/ 
PWM_SinglePhase singlePhaseTestConfig = {
	
	/* which outputs of type SCT_OUT should be steered */
	.outputMask = SCT_OUTMASK_0 | SCT_OUTMASK_1 | SCT_OUTMASK_2 | SCT_OUTMASK_3,		
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT ,		
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,				
	/* value in Hz for the PWM period */	
	.period = 1000,				
	/* active high or active low */
	.polarity = ACTIVE_HIGH,	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback
};



/**********************************************************************
 * configuration for a single phase test with complementary output
 **********************************************************************/
PWM_outputPair phaseArray[] = {
	{SCT_OUT_0, SCT_OUT_1},
	{SCT_OUT_2, SCT_OUT_3},
	{SCT_OUT_4, SCT_OUT_5}
};

PWM_ComplementarySinglePhase singlePhaseComplementaryTestConfig = {

	/* which outputs of type SCT_OUT should be steered */
	.pwmOutputs = {
		.numPhases = 3,
		.phasePairs = &phaseArray[0]
	},						
	
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT,			
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,				
	/* value in Hz for the PWM period */	
	.period = 1000,					
	/* active high or active low */				
	.polarity = ACTIVE_HIGH,		
	/* in PWM clocks */	
	.risingEdgeDeadband = 100, 	
	/* in PWM clocks */
	.fallingEdgeDeadband = 100,  	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback			

};

/**********************************************************************
 * configuration for a push pull test 
 **********************************************************************/

PWM_PushPull pushPullTestConfig = {

	/* which outputs of type SCT_OUT should be steered */
	.outputMaskEven = SCT_OUTMASK_0 | SCT_OUTMASK_1 | SCT_OUTMASK_2 | SCT_OUTMASK_3,
	.outputMaskOdd = SCT_OUTMASK_4 | SCT_OUTMASK_5 | SCT_OUTMASK_6 | SCT_OUTMASK_7,
	
	/* which internal signal is used to track the period number (odd/even) */
	.oddPeriod = SCT_OUT_8,
	
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT ,	
	
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,				
	
	/* value in Hz for the PWM period */	
	.period = 1000,				
	
	/* active high or active low */
	.polarity = ACTIVE_HIGH,	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback			

};


/**********************************************************************
 * configuration for a push pull test with complementary output
 **********************************************************************/
PWM_pushPullOutputPair phaseArrayOdd[] = {
	{SCT_OUT_0, SCT_OUT_1},
	{SCT_OUT_2, SCT_OUT_3},
	{SCT_OUT_4, SCT_OUT_5}
};

PWM_pushPullOutputPair phaseArrayEven[] = {
	{SCT_OUT_6, SCT_OUT_7},
	{SCT_OUT_9, SCT_OUT_10},
	{SCT_OUT_11, SCT_OUT_12}
};

PWM_ComplementaryPushPull pushPullComplementaryTestConfig = {

	/* which outputs of type SCT_OUT should be steered on odd periods */
	.pwmPhasesOdd = {
		.numPhases = 3,
		.phasePairs = &phaseArrayOdd[0]
	},	

	/* which outputs of type SCT_OUT should be steered on odd periods */
	.pwmPhasesEven = {
		.numPhases = 3,
		.phasePairs = &phaseArrayEven[0]
	},	
	
	/* which internal signal is used to track the period number (odd/even) */
	.oddPeriod = SCT_OUT_8,
	
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT,	
	
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,				

	/* value in Hz for the PWM period */	
	.period = 1000,					

	/* active high or active low */				
	.polarity = ACTIVE_HIGH,		

	/* in PWM clocks */	
	.risingEdgeDeadband = 100, 	

	/* in PWM clocks */
	.fallingEdgeDeadband = 100,  	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback			

};

/**********************************************************************
 * configuration for a pulse skipping PWM
 **********************************************************************/
PWM_PulseSkipping pulseSkippingTestConfig = {

	/* which outputs of type SCT_OUT should be steered */
	.outputMask = SCT_OUTMASK_0 | SCT_OUTMASK_1 | SCT_OUTMASK_2 | SCT_OUTMASK_3,
	
	/* which input signal is used to enable / disable the outputs */		
	.asyncEnableInput = SCT_IN_0,
 	
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT,
	
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,	
	
	/* value in Hz */	
	.period = 1000,		
	
	/* active high or active low */
	.polarity = ACTIVE_HIGH,	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback

};

/**********************************************************************
 * configuration for a complementary pulse skipping PWM
 **********************************************************************/
PWM_pulseSkipOutputPair pskipPhaseArray[] = {
	{SCT_OUT_0, SCT_OUT_1},
	{SCT_OUT_2, SCT_OUT_3},
	{SCT_OUT_4, SCT_OUT_5}
};


PWM_ComplementaryPulseSkipping complPulseSkippingTestConfig = {

	/* which outputs of type SCT_OUT should be steered on odd periods */
	.pwmOutputs = {
		.numPhases = 3,
		.phasePairs = &pskipPhaseArray[0]
	},	

	
	/* which input signal is used to enable / disable the outputs */		
	.asyncEnableInput = SCT_IN_0,
 	
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT,
	
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,	
	
	/* value in Hz */	
	.period = 1000,		
	
	/* active high or active low */
	.polarity = ACTIVE_HIGH,	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback

};


/**********************************************************************
 * configuration for a variable frequency test 
 **********************************************************************/ 
PWM_VarFrequency varFrequencyTestConfig = {
	
	/* which outputs of type SCT_OUT should be steered */
	.outputMask = SCT_OUTMASK_0 | SCT_OUTMASK_1 | SCT_OUTMASK_2 | SCT_OUTMASK_3,		
	/* which interrupts should be active */	
	.interruptMask = PERIOD_INT ,		
	/* value in percentage per mil (n/1000) */
	.dutyCycle = 500,				
	/* value in Hz for the PWM period */	
	.period = 1000,				
	/* active high or active low */
	.polarity = ACTIVE_HIGH,	
	
	.risingEdgeInt = &risingEdgeCallback,
	.fallingEdgeInt = &fallingEdgeCallback,
	.periodInt = &periodCallback
};


/**********************************************************************
 * @brief		Main SCT program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int main(void)
{
	PWM_STATUS status = PWM_INVALID_STATUS;
	
	setupMCB4350();

	
	/* Global configuration of the SCT */
	status = SCT_pwmInit(&globalConfig);
	while(status != PWM_OK);
	
	/* Open the emulated peripheral */
	status = SCT_pwmOpen(PULSE_SKIPPING, &pulseSkippingTestConfig);
	while(status != PWM_OK);

	/* start the peripheral */
	status = SCT_pwmIoctl(START_PWM, NULL);
	while(status != PWM_OK);
	
	SysTick_Config(SystemCoreClock/1000);
	
	// run cpu in an endless loop
	while (1) {
	};
}



/**********************************************************************
 * this function can be used to test periodically the start / stop 
 * behavior
 **********************************************************************/
int16_t periods = 5;
volatile uint8_t startStopFlag = 0;
void startStop(void) {
	
	if(--periods == 0) {			
			periods = 5; 		
			
		if(startStopFlag == 0) {
			startStopFlag = 1;
			/* ignore the return status */
			SCT_pwmIoctl(SYNC_STOP_PWM,NULL);
		} else {
			startStopFlag = 0;
			/* ignore the return status */
			SCT_pwmIoctl(START_PWM,NULL);
		}
	};	

}

/**********************************************************************
 * system tick handler, useful for triggering periodic events
 **********************************************************************/
uint16_t msec = 1000;
void SysTick_Handler (void) {
	
	if(msec >0) {
			msec--;
	}
	else {
		toggleLed();
		
		/* test sync start and stop of the PWM */
		// startStop();
		msec = 1000;
	}
	
	
}

/**********************************************************************
 * rising edge event interrupt function
 **********************************************************************/
volatile uint32_t numRisingEdgeInts = 0;
void risingEdgeCallback(void) {

	numRisingEdgeInts++;	
}

/**********************************************************************
 * falling edge event interrupt function
 **********************************************************************/
volatile uint32_t numFallingEdgeInts = 0;
void fallingEdgeCallback(void) {
	
	numFallingEdgeInts++;
}



/**********************************************************************
 * this function could be used to sweep the duty cycle over the full
 * period
 **********************************************************************/
int16_t dutyCycle = 500;
volatile uint8_t up = 1;
void changeDc(void) {
	
	if(up) {
		
		dutyCycle++;
		if(dutyCycle == 1001) {			
			up = 0; 		
			dutyCycle = 1000;
		};
		

	} else {

		dutyCycle--;
		if(dutyCycle < 0) {
			up = 1; 				
			dutyCycle = 0;
		};
	};
	
	/* ignore the return status */
	SCT_pwmIoctl(SET_DC, (void*)&dutyCycle);	
}

/**********************************************************************
 * this function could be used to sweep the frequency over the full
 * range
 **********************************************************************/
int32_t frequency = 1000;
void changeFreq(void) {
	
	if(up) {
		
		frequency++;
		if(frequency == globalConfig.pwmFrequency - 1) {			
			up = 0; 		
		};
		

	} else {

		frequency--;
		if(frequency == 1) {
			up = 1; 				
		};
	};
	
	/* ignore the return status */
	SCT_pwmIoctl(SET_FREQ, (void*)&frequency);	
}

/**********************************************************************
 * period event interrupt function
 **********************************************************************/
volatile uint32_t numPeriodInts = 0;
void periodCallback(void) {

	/* enable different interrupts */
	// SCT_pwmIoctl(ENABLE_INT, RISING_INT | FALLING_INT | PERIOD_INT);

	/* sweep the duty cycle */
	// changeDc();

	/* sweep the frequency */
	changeFreq();

	numPeriodInts++;
}




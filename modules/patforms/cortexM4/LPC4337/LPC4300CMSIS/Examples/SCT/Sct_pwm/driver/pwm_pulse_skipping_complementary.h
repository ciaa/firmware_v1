/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_pulse_skipping_complementary.h
* @brief	structure definition for the SCT PWM pulse skipping
* 			complementary configuration mode
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
#ifndef __SCT_PWM_PULSE_SKIPPING_COMPLEMENTARY_H__
#define __SCT_PWM_PULSE_SKIPPING_COMPLEMENTARY_H__

/******************************************************************************
 * Simple PWM with single phase output and complementary phase output
 * programmable dead band option
 * identical PWM waveform output to all configured pins
 * rising edge, falling edge, period interrupt supported
 * pulses are skipped if beginning of the period a selected input signal is 
 * detected LOW
 *****************************************************************************/
typedef struct PWM_pulseSkipOutputPair {
	
	SCT_OUT directOutput;
	SCT_OUT complementaryOutput;	
	
} PWM_pulseSkipOutputPair;

typedef struct PWM_pskipPhases {
	
	uint8_t 					numPhases;
	PWM_pulseSkipOutputPair* 	phasePairs; 
	
} PWM_pskipPhases;

typedef struct PWM_ComplementaryPulseSkipping {

	PWM_pskipPhases	pwmOutputs;				/* which outputs of type SCT_OUT should be steered */				
	SCT_IN 			asyncEnableInput; 		/* which input signal is used to enable / disable the outputs */	
	uint16_t  		interruptMask;			/* which interrupts should be active */	
	uint16_t 		dutyCycle;				/* value in percentage per mil (n/1000) */
	uint32_t 		period;					/* value in Hz */
	uint16_t		risingEdgeDeadband; 	/* dead band applied on rising edge */
	uint16_t		fallingEdgeDeadband;	/* dead band applied on rising edge */
	PWM_polarity	polarity;				/* active high or active low */
	intCallback_t	risingEdgeInt;			/* rising edge, falling edge, period event interrupt callbacks */
	intCallback_t	fallingEdgeInt;		
	intCallback_t	periodInt;			

} PWM_ComplementaryPulseSkipping;



extern PWM_STATUS makePwmPulseSkippingComplementaryConfig(PWM_ComplementaryPulseSkipping* config);
extern PWM_STATUS changePwmCompPulseSkippingDc(uint16_t dc);

#endif




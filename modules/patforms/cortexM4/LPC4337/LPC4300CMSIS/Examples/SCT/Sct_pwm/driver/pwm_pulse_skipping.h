/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_pulse_skipping.h
* @brief	structure definition for the SCT PWM pulse skipping
* 			configuration mode
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
#ifndef __SCT_PWM_PULSE_SKIPPING_H__
#define __SCT_PWM_PULSE_SKIPPING_H__

/******************************************************************************
 * Simple PWM with single phase output 
 * no dead band available
 * identical PWM waveform output to all configured pins
 * rising edge, falling edge, period interrupt supported
 * pulses are skipped if beginnig of the period a selected input signal is 
 * detected LOW
 *****************************************************************************/
typedef struct PWM_PulseSkipping {

	uint16_t 		outputMask;			/* which outputs of type SCT_OUT should be steered */
	SCT_IN 			asyncEnableInput; 	/* which input signal is used to enable / disable the outputs */	
	uint16_t  		interruptMask;		/* which interrupts should be active */	
	uint16_t 		dutyCycle;			/* value in percentage per mil (n/1000) */
	uint32_t 		period;				/* value in Hz */	
	PWM_polarity	polarity;			/* active high or active low */
	intCallback_t	risingEdgeInt;		/* rising edge event interrupt callback */
	intCallback_t	fallingEdgeInt;		/* falling edge event interrupt callback */
	intCallback_t	periodInt;			/* period event interrupt callback */	

} PWM_PulseSkipping;


extern PWM_STATUS makePwmPulseSkippingConfig(PWM_PulseSkipping* config);
extern PWM_STATUS changePwmPulseSkippingDc(uint16_t dc);

#endif


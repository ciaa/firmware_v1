/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		pwm_push_pull_complementary.h
* @brief	structure definition for the SCT PWM push pull
* 			complementary configuration mode
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
#ifndef __SCT_PWM_PUSH_PULL_COMPLEMENTARY_H__
#define __SCT_PWM_PUSH_PULL_COMPLEMENTARY_H__

/******************************************************************************
 * PWM with push pull output and complementary phase output
 * programmable dead band option
 * identical PWM waveform output to all configured pins
 * rising edge, falling edge, period interrupt supported
 *****************************************************************************/
typedef struct PWM_pushPullOutputPair {
	
	SCT_OUT directOutput;
	SCT_OUT complementaryOutput;	
	
} PWM_pushPullOutputPair;

typedef struct PWM_pushPullPhases {
	
	uint8_t 				numPhases;
	PWM_pushPullOutputPair* phasePairs; 
	
} PWM_pushPullPhases;

typedef struct PWM_ComplementaryPushPull {

	/* which outputs of type SCT_OUT should be steered in the odd period */
	PWM_pushPullPhases		pwmPhasesOdd;				
		
	/* which outputs of type SCT_OUT should be steered in the even period */
	PWM_pushPullPhases		pwmPhasesEven;				

	/* on which SCT output (internal) the period shall be tracked */
	/* this signal is reserved and cannot be used for the outputs */
	SCT_OUT 		oddPeriod;
	
	/* which interrupts should be active */	
	uint16_t  		interruptMask;			
	
	/* value in percentage per mil (n/1000) */
	uint16_t 		dutyCycle;				
	
	/* value in Hz */
	uint32_t 		period;					
	
	/* dead band applied on rising edge */
	uint16_t		risingEdgeDeadband; 	
	
	/* dead band applied on rising edge */
	uint16_t		fallingEdgeDeadband;	
	
	/* active high or active low */
	PWM_polarity	polarity;				
	
	/* rising edge, falling edge, period event interrupt callbacks */
	intCallback_t	risingEdgeInt;			
	intCallback_t	fallingEdgeInt;		
	intCallback_t	periodInt;			

} PWM_ComplementaryPushPull;



extern PWM_STATUS makePwmPushPullComplementaryConfig(PWM_ComplementaryPushPull* config);
extern PWM_STATUS changePwmPushPullComplementaryDc(uint16_t dc);

#endif




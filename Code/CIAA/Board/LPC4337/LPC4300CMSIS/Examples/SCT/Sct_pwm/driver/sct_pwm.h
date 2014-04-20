/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		sct_pwm.h
* @brief	API functions definition for the SCT PWM emulation
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
#ifndef __SCT_PWM_H__
#define __SCT_PWM_H__

#include <stdint.h>
//#include <stdbool.h>

#include "sct_hal.h"
#include "sct_pwm_error.h"

/* definition of supported waveform types */
typedef enum PwmType {

	SINGLE_PHASE = (int) (1u << 31),
	COMP_SINGLE_PHASE = 1u << 30,	
	PUSH_PULL = 1u << 29,	
	COMP_PUSH_PULL = 1u << 28,		
	PULSE_SKIPPING = 1u << 27,	
	COMP_PULSE_SKIPPING = 1u << 26,		
	VARIABLE_F_FIXED_DC = 1u << 25,		
	INVALID_TYPE = 0
	
} PwmType;

/* polarity of the PWM waveform */
typedef enum PWM_polarity {
	
	ACTIVE_HIGH = 0,
	ACTIVE_LOW
	
} PWM_polarity;



/******************************************************************************
 * PWM waveform parameter structures
 *****************************************************************************/
#include "pwm_single_phase.h"
#include "pwm_single_phase_complementary.h"
#include "pwm_push_pull.h"
#include "pwm_push_pull_complementary.h"
#include "pwm_pulse_skipping.h"
#include "pwm_pulse_skipping_complementary.h"
#include "pwm_var_frequency.h"


/******************************************************************************
 * Control commands used to change PWM parameters and behavior at runtime
 *
 * identical PWM waveform output to all configured pins
 * rising edge, falling edge, period interrupt supported
 *****************************************************************************/
typedef enum PwmIoctl {

	CMD_EMPTY = 0,		// no effect
	GET_PWM_FREQUENCY,	// return current clock base for the PWM
	START_PWM,			// start the generation of the signals
	SET_DC,				// change duty cycle
	SET_FREQ,			// change the period frequency
	ENABLE_INT,			// enable the interrupts 
	SYNC_STOP_PWM,		// stop the counter at the next period boundary
	STOP_PWM,			// stop the counter immediately
	SYNC_HALT_PWM,		// halt the counter at the next period boundary
	HALT_PWM,			// halt the counter immediately
	BLOCK_RELOAD,		// stop reloading the match registers
	ENABLE_RELOAD		// enable reloading the match registers
	
} PwmIoctl;



/******************************************************************************
 * Configuration parameters common at the module level
 *****************************************************************************/
typedef struct PwmGlobalParams {
	
	/* which clock is used */
	PwmClockBase			clock;			
	
	/* which pin is sampled if not from INTERNAL_BUS */
	PwmClockInput			clockIn;		
	
	/* at which base frequency the PWM waveform should be produced */
	PwmClockFrequency		pwmFrequency;	
	
	/* frequency at which the SCT is clocked in the system */
	SctClockFrequency		sctFrequency;	
	
} PwmGlobalParams;



/******************************************************************************
 * PWM emulation API calls
 *****************************************************************************/

/* initialize the emulated PWM peripheral with global settings */
PWM_STATUS SCT_pwmInit(PwmGlobalParams *config);

/* open the simulated PWM interface */
PWM_STATUS SCT_pwmOpen(PwmType mode, const void* config);

/* close the simulated PWM interface */
void SCT_pwmClose(void);

/* modify PWM configuration parameters dynamically */
PWM_STATUS SCT_pwmIoctl(const PwmIoctl cmd, void* param) __attribute__((always_inline));
























#endif


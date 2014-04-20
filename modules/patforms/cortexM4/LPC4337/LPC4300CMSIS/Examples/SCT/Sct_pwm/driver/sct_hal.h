/**********************************************************************
* $Id$		nxp28536			2012-06-19
*//**
* @file		sct_hal.h
* @brief	definition and inline implementation of HAL functions to
* 			program the SCT at the register level
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
#ifndef __SCT_HAL_H__
#define __SCT_HAL_H__


#include "lpc43xx.h"


/* definition of the output pins for the sct */
typedef enum SCT_OUT_MASK {

	SCT_OUTMASK_0 = 1u << 0,
	SCT_OUTMASK_1 = 1u << 1,
	SCT_OUTMASK_2 = 1u << 2,
	SCT_OUTMASK_3 = 1u << 3,
	SCT_OUTMASK_4 = 1u << 4,
	SCT_OUTMASK_5 = 1u << 5,
	SCT_OUTMASK_6 = 1u << 6,
	SCT_OUTMASK_7 = 1u << 7,
	SCT_OUTMASK_8 = 1u << 8,
	SCT_OUTMASK_9 = 1u << 9,
	SCT_OUTMASK_10 = 1u << 10,
	SCT_OUTMASK_11 = 1u << 11,
	SCT_OUTMASK_12 = 1u << 12,
	SCT_OUTMASK_13 = 1u << 13,
	SCT_OUTMASK_14 = 1u << 14,
	SCT_OUTMASK_15 = 1u << 15,
	
} SCT_OUT_MASK;

typedef enum SCT_OUT {

	SCT_OUT_0 = 0,
	SCT_OUT_1,
	SCT_OUT_2,
	SCT_OUT_3,
	SCT_OUT_4,
	SCT_OUT_5,
	SCT_OUT_6,
	SCT_OUT_7,
	SCT_OUT_8,
	SCT_OUT_9,
	SCT_OUT_10,
	SCT_OUT_11,
	SCT_OUT_12,
	SCT_OUT_13,
	SCT_OUT_14,
	SCT_OUT_15
	
} SCT_OUT;

#define OUT_DEBUG_UP	SCT_OUT_15
#define OUT_DEBUG_DOWN	SCT_OUT_15


/* definition of the input pins for the sct */
typedef enum SCT_IN {

	SCT_IN_0 = 0,
	SCT_IN_1 = 1,
	SCT_IN_2 = 2,
	SCT_IN_3 = 3,
	SCT_IN_4 = 4,
	SCT_IN_5 = 5,
	SCT_IN_6 = 6,
	SCT_IN_7 = 7,
	SCT_IN_8 = 8,
	SCT_IN_9 = 9,
	SCT_IN_10 = 10,
	SCT_IN_11 = 11,
	SCT_IN_12 = 12,
	SCT_IN_13 = 13,
	SCT_IN_14 = 14,
	SCT_IN_15 = 15,
	
} SCT_IN;

/* definition of the events */
typedef enum SCT_EVENT {

	SCT_EV_0 = 0,
	SCT_EV_1,
	SCT_EV_2,
	SCT_EV_3,
	SCT_EV_4,
	SCT_EV_5,
	SCT_EV_6,
	SCT_EV_7,
	SCT_EV_8,
	SCT_EV_9,
	SCT_EV_10,
	SCT_EV_11,
	SCT_EV_12,
	SCT_EV_13,
	SCT_EV_14,
	SCT_EV_15
	
} SCT_EVENT;

typedef enum SCT_EVENT_MASK {

	SCT_EVMASK_0 = 1u << 0,
	SCT_EVMASK_1 = 1u << 1,
	SCT_EVMASK_2 = 1u << 2,
	SCT_EVMASK_3 = 1u << 3,
	SCT_EVMASK_4 = 1u << 4,
	SCT_EVMASK_5 = 1u << 5,
	SCT_EVMASK_6 = 1u << 6,
	SCT_EVMASK_7 = 1u << 7,
	SCT_EVMASK_8 = 1u << 8,
	SCT_EVMASK_9 = 1u << 9,
	SCT_EVMASK_10 = 1u << 10,
	SCT_EVMASK_11 = 1u << 11,
	SCT_EVMASK_12 = 1u << 12,
	SCT_EVMASK_13 = 1u << 13,
	SCT_EVMASK_14 = 1u << 14,
	SCT_EVMASK_15 = 1u << 15,
	
} SCT_EVENT_MASK;

/* definition of the match registers */
typedef enum SCT_MATCHREG {

	SCT_MATCH_0 = 0,
	SCT_MATCH_DEFAULT = SCT_MATCH_0,
	SCT_MATCH_1,
	SCT_MATCH_2,
	SCT_MATCH_3,
	SCT_MATCH_4,
	SCT_MATCH_5,
	SCT_MATCH_6,
	SCT_MATCH_7,
	SCT_MATCH_8,
	SCT_MATCH_9,
	SCT_MATCH_10,
	SCT_MATCH_11,
	SCT_MATCH_12,
	SCT_MATCH_13,
	SCT_MATCH_14,
	SCT_MATCH_15
	
} SCT_MATCHREG;


typedef enum SCT_STATE_NUM { 

	SCT_STATE_0 = 0,
	SCT_STATE_1,
	SCT_STATE_2,
	SCT_STATE_3,
	SCT_STATE_4,
	SCT_STATE_5,
	SCT_STATE_6,
	SCT_STATE_7,
	SCT_STATE_8,
	SCT_STATE_9,
	SCT_STATE_10,
	SCT_STATE_11,
	SCT_STATE_12,
	SCT_STATE_13,
	SCT_STATE_14,
	SCT_STATE_15,
	SCT_STATE_16,
	SCT_STATE_17,
	SCT_STATE_18,
	SCT_STATE_19,
	SCT_STATE_20,
	SCT_STATE_21,
	SCT_STATE_22,
	SCT_STATE_23,
	SCT_STATE_24,
	SCT_STATE_25,
	SCT_STATE_26,
	SCT_STATE_27,
	SCT_STATE_28,
	SCT_STATE_29,
	SCT_STATE_30,
	SCT_STATE_31,
	
} SCT_STATE_NUM;

typedef enum SCT_STATE_MASK { 

	SCT_SMASK_0 = 1u << 0,
	SCT_SMASK_1 = 1u << 1,
	SCT_SMASK_2	= 1u << 2,
	SCT_SMASK_3	= 1u << 3,
	SCT_SMASK_4	= 1u << 4,
	SCT_SMASK_5	= 1u << 5,
	SCT_SMASK_6	= 1u << 6,
	SCT_SMASK_7	= 1u << 7,
	SCT_SMASK_8	= 1u << 8,
	SCT_SMASK_9	= 1u << 9,
	SCT_SMASK_10 = 1u << 10,
	SCT_SMASK_11 = 1u << 11,
	SCT_SMASK_12 = 1u << 12,
	SCT_SMASK_13 = 1u << 13,
	SCT_SMASK_14 = 1u << 14,
	SCT_SMASK_15 = 1u << 15,
	SCT_SMASK_16 = 1u << 16,
	SCT_SMASK_17 = 1u << 17,
	SCT_SMASK_18 = 1u << 18,
	SCT_SMASK_19 = 1u << 19,
	SCT_SMASK_20 = 1u << 20,
	SCT_SMASK_21 = 1u << 21,
	SCT_SMASK_22 = 1u << 22,
	SCT_SMASK_23 = 1u << 23,
	SCT_SMASK_24 = 1u << 24,
	SCT_SMASK_25 = 1u << 25,
	SCT_SMASK_26 = 1u << 26,
	SCT_SMASK_27 = 1u << 27,
	SCT_SMASK_28 = 1u << 28,
	SCT_SMASK_29 = 1u << 29,
	SCT_SMASK_30 = 1u << 30,
	SCT_SMASK_31 = (int) (1u << 31)
	
} SCT_STATE_MASK;


/* internal mapping of events */
/* internal mapping of match interrupts */




/* used by several configurations*/
#define EV_PERIOD 		SCT_EV_0
#define PERIOD_INT 	SCT_EVMASK_0


/* used by normal pwm */
#define EV_RISING 		SCT_EV_1
#define RISING_INT 		SCT_EVMASK_1

#define EV_FALLING		SCT_EV_2
#define FALLING_INT		SCT_EVMASK_2

/* used by complementary pwm */
#define EV_RISING_DB 	SCT_EV_3
#define EV_FALLING_DB	SCT_EV_4

/* use by push pull */
#define EV_RISING_ODD	SCT_EV_1
#define EV_FALLING_ODD	SCT_EV_2
#define EV_RISING_EVEN	SCT_EV_3
#define EV_FALLING_EVEN	SCT_EV_4

/* use by complementary push pull */
#define EV_PERIOD_ODD	SCT_EV_0
#define EV_PERIOD_EVEN	SCT_EV_9

#define EV_RISING_ODD_DB	SCT_EV_5
#define EV_FALLING_ODD_DB	SCT_EV_6
#define EV_RISING_EVEN_DB	SCT_EV_7
#define EV_FALLING_EVEN_DB	SCT_EV_8

#define EV_START_SKIP		SCT_EV_10
#define EV_STOP_SKIP		SCT_EV_11
// #define EV_TRACK_SKIP_SIGNAL	SCT_EV_12

#define EV_DEBUG_UP		SCT_EV_14
#define EV_DEBUG_DOWN	SCT_EV_15

/*********************************************************/
/* config register */
/*********************************************************/
typedef enum SCT_Unify {
	
	CONFIG_SPLIT			= 0,
	CONFIG_UNIFIED			= 1,
	CONFIG_UNIFY_DEFAULT 	= CONFIG_SPLIT,
	CONFIG_UNIFY_DONTCARE 	= CONFIG_SPLIT,

} SCT_Unify;

typedef enum SCT_ClockBase {
	
	CONFIG_CLKMOD_INTERNAL	= 0x0,
	CONFIG_CLKMODE_DEFAULT	= CONFIG_CLKMOD_INTERNAL,
	CONFIG_CLKMODE_DONTCARE	= CONFIG_CLKMOD_INTERNAL,	
	CONFIG_SAMPLED_HIGHPERF = 0x1,
	CONFIG_SAMPLED_LOWPOWER	= 0x2,
	CONFIG_CLKSEL_INPUT		= 0x3

} SCT_ClockBase;

typedef enum SCT_ClockPin {

	CONFIG_IN_0_RISING 		= 1u << 0,
	CONFIG_IN_0_DEFAULT		= CONFIG_IN_0_RISING,
	CONFIG_IN_0_DONTCARE	= CONFIG_IN_0_RISING,	
	CONFIG_IN_0_FALLING		= 1u << 1,
	CONFIG_IN_1_RISING 		= 1u << 2,
	CONFIG_IN_1_FALLING 	= 1u << 3,
	CONFIG_IN_2_RISING 		= 1u << 4,
	CONFIG_IN_2_FALLING 	= 1u << 5,
	CONFIG_IN_3_RISING 		= 1u << 6,
	CONFIG_IN_3_FALLING 	= 1u << 7,
	CONFIG_IN_4_RISING 		= 1u << 8,
	CONFIG_IN_4_FALLING 	= 1u << 9,
	CONFIG_IN_5_RISING 		= 1u << 10,
	CONFIG_IN_5_FALLING		= 1u << 11,
	CONFIG_IN_6_RISING		= 1u << 12,
	CONFIG_IN_6_FALLING		= 1u << 13,
	CONFIG_IN_7_RISING		= 1u << 14,
	CONFIG_IN_7_FALLING		= 1u << 15, 
	
} SCT_ClockPin;

typedef enum SCT_NoReload {

	CONFIG_NORELOAD_DISABLE		= 0,
	CONFIG_NORELOAD_DEFAULT 	= CONFIG_NORELOAD_DISABLE,
	CONFIG_NORELOAD_DONTCARE	= CONFIG_NORELOAD_DISABLE,
	CONFIG_NORELOAD_ENABLE		= 1,
	
} SCT_NoReload;

typedef SCT_NoReload	SCT_NoReloadL;
typedef SCT_NoReload	SCT_NoReloadH;


typedef uint8_t	SCT_inSync;
#define CONFIG_INSYC_DEFAULT 	0xFF
#define CONFIG_INSYC_DONTCARE	0xFF

typedef uint32_t SCT_CfgReg;

/* function to make the config register */
static SCT_CfgReg SCT_makeCfgReg (
	SCT_Unify			unifyMode,
	SCT_ClockBase 		clockBase, 
	SCT_ClockPin		clockSel,
	SCT_NoReloadL		noReloadL,
	SCT_NoReloadH		noReloadH,
	SCT_inSync			inSync
)  __attribute__((always_inline));

static SCT_CfgReg SCT_makeCfgReg (
	SCT_Unify			unifyMode,
	SCT_ClockBase 		clockBase, 
	SCT_ClockPin		clockSel,
	SCT_NoReloadL		noReloadL,
	SCT_NoReloadH		noReloadH,
	SCT_inSync			inSync)
{
    return (SCT_CfgReg)(
        (unifyMode << 0)	|
        (clockBase << 1)	|
        (clockSel << 3)		|
		(noReloadL << 7)	|	
		(noReloadH << 8)	|	
        ((inSync & 0xFF) << 9)     	
	);
}

static void SCT_writeCfgReg(SCT_CfgReg cfgReg) __attribute__((always_inline));

static void SCT_writeCfgReg(SCT_CfgReg cfgReg) {

	LPC_SCT->CONFIG = cfgReg;
}



/*********************************************************/
/* control register */
/*********************************************************/

typedef enum SCT_Stop {
	
	CTRL_STOP_DISABLE	= 0,
	CTRL_STOP_DEFAULT 	= CTRL_STOP_DISABLE,
	CTRL_STOP_DONTCARE 	= CTRL_STOP_DISABLE,
	CTRL_STOP_ENABLE	= 1,

} SCT_Stop;

typedef SCT_Stop	SCT_StopH;
typedef SCT_Stop	SCT_StopL;

typedef enum SCT_Halt {
	
	CTRL_HALT_DISABLE	= 0,
	CTRL_HALT_ENABLE	= 1,
	CTRL_HALT_DEFAULT 	= CTRL_HALT_ENABLE,

} SCT_Halt;

typedef SCT_Halt	SCT_HaltH;
typedef SCT_Halt	SCT_HaltL;

typedef enum SCT_ClearCntr {
	
	CTRL_CLRCNTR_DISABLE	= 0,
	CTRL_CLRCNTR_DEFAULT 	= CTRL_CLRCNTR_DISABLE,
	CTRL_CLRCNTR_DONTCARE 	= CTRL_CLRCNTR_DISABLE,
	CTRL_CLRCNTR_ENABLE		= 1,

} SCT_ClearCntr;

typedef SCT_ClearCntr	SCT_ClearCntrL;
typedef SCT_ClearCntr	SCT_ClearCntrH;

typedef enum SCT_Bidir {
	
	CTRL_BIDIR_DISABLE	= 0,
	CTRL_BIDIR_DEFAULT 	= CTRL_BIDIR_DISABLE,
	CTRL_BIDIR_DONTCARE = CTRL_BIDIR_DISABLE,
	CTRL_BIDIR_ENABLE	= 1,

} SCT_Bidir;

typedef SCT_Bidir	SCT_BidirH;
typedef SCT_Bidir	SCT_BidirL;

typedef uint8_t			SCT_Prescale;
typedef SCT_Prescale	SCT_PrescaleL;
typedef SCT_Prescale	SCT_PrescaleH;
#define SCT_PRESCALE_DEFAULT 0

/* function to make the control reg register */
typedef uint32_t SCT_CntrlReg;
typedef uint16_t SCT_CntrlRegL;
typedef uint16_t SCT_CntrlRegH;

static SCT_CntrlReg SCT_makeCntrlReg (
		SCT_StopL 		stopL,
		SCT_HaltL 		haltL,
		SCT_ClearCntrL 	clrCntrL,
		SCT_BidirL		bidirL, 
		SCT_PrescaleL	prescaleL,
		SCT_StopH 		stopH,
		SCT_HaltH 		haltH,
		SCT_ClearCntrH	clrCntrH,
		SCT_BidirH		bidirH,
		SCT_PrescaleH	prescaleH
)  __attribute__((always_inline));

static SCT_CntrlReg SCT_makeCntrlReg (
		SCT_StopL 		stopL,
		SCT_HaltL 		haltL,
		SCT_ClearCntrL 	clrCntrL,
		SCT_BidirL		bidirL, 
		SCT_PrescaleL	prescaleL,
		SCT_StopH 		stopH,
		SCT_HaltH 		haltH,
		SCT_ClearCntrH	clrCntrH,
		SCT_BidirH		bidirH,
		SCT_PrescaleH	prescaleH)
{
    return (SCT_CntrlReg)(
		(stopL << 1) 				|
		(haltL << 2) 				|
		(clrCntrL << 3)				|
		(bidirL << 4)				|
		((prescaleL & 0xFF)<< 5)	|
		(stopH << 17) 				|
		(haltH << 18) 				|
		(clrCntrH << 19)			|
		(bidirH << 20)				|
		((prescaleH & 0xFF)<< 21)	
	);
}


/* function to configure the control register */
static void SCT_writeCntrlReg (const SCT_CntrlReg ctrlReg) __attribute__((always_inline));

/* supports only the unified mode for now */
static void SCT_writeCntrlReg(const SCT_CntrlReg ctrlReg) {

	LPC_SCT->CTRL_U = ctrlReg;
}



/*********************************************************/
/* event control register */
/*********************************************************/

typedef enum SCT_hEvent {
	
	EVCTRL_HEVENT_L = 0,
	EVCTRL_HEVENT_U = EVCTRL_HEVENT_L,
	EVCTRL_HEVENT_H = 1

} SCT_hEvent;

typedef enum SCT_outSel {
	
	EVCTRL_OUTSEL_INPUT = 0,
	EVCTRL_OUTSEL_DONTCARE = EVCTRL_OUTSEL_INPUT,
	EVCTRL_OUTSEL_DEFAULT = EVCTRL_OUTSEL_INPUT,	
	EVCTRL_OUTSEL_OUTPUT = 1

} SCT_outSel;

typedef enum SCT_IoSel {
	
	EVCTRL_IOSEL_0 = 0,
	EVCTRL_IOSEL_DONTCARE = EVCTRL_IOSEL_0,
	EVCTRL_IOSEL_DEFAULT = EVCTRL_IOSEL_0,	
	EVCTRL_IOSEL_1,
	EVCTRL_IOSEL_2,
	EVCTRL_IOSEL_3,
	EVCTRL_IOSEL_4,
	EVCTRL_IOSEL_5,
	EVCTRL_IOSEL_6,
	EVCTRL_IOSEL_7,
	EVCTRL_IOSEL_8,
	EVCTRL_IOSEL_9,	
	EVCTRL_IOSEL_10,
	EVCTRL_IOSEL_11,
	EVCTRL_IOSEL_12,
	EVCTRL_IOSEL_13,
	EVCTRL_IOSEL_14,
	EVCTRL_IOSEL_15,

} SCT_IoSel;

typedef enum SCT_IoCond {
	
	EVCTRL_IOCOND_LOW = 0,
	EVCTRL_IOCOND_DONTCARE = EVCTRL_IOCOND_LOW,
	EVCTRL_IOCOND_DEFAULT = EVCTRL_IOCOND_LOW,	
	EVCTRL_IOCOND_RISE = 1,
	EVCTRL_IOCOND_FALL = 2,
	EVCTRL_IOCOND_HIGH = 3	

} SCT_IoCond;

typedef enum SCT_combMode {
	
	EVCTRL_IOCOMB_OR = 0,
	EVCTRL_IOCOMB_DEFAULT = EVCTRL_IOCOMB_OR,
	EVCTRL_IOCOMB_DONTCARE = EVCTRL_IOCOMB_OR,	
	EVCTRL_IOCOMB_MATCH = 1,
	EVCTRL_IOCOMB_IO = 2,
	EVCTRL_IOCOMB_AND = 3	

} SCT_combMode;

typedef enum SCT_stateLd {
	
	EVCTRL_STATELD_ADD = 0,
	EVCTRL_STATELD_DONTCARE = EVCTRL_STATELD_ADD,	
	EVCTRL_STATELD_DEFAULT = EVCTRL_STATELD_ADD,	
	EVCTRL_STATELD_JUMP = 1

} SCT_stateLd;

typedef enum SCT_matchMem {
	
	EVCTRL_MATCHMEM_EQUAL = 0,
	EVCTRL_MATCHMEM_DONTCARE = EVCTRL_MATCHMEM_EQUAL,	
	EVCTRL_MATCHMEM_DEFAULT = EVCTRL_MATCHMEM_EQUAL,	
	EVCTRL_STATELD_GT_UPCOUNT = 1,
	EVCTRL_STATELD_LT_DOWNCOUNT = EVCTRL_STATELD_GT_UPCOUNT
	
} SCT_matchMem;

typedef enum SCT_eventDirection {
	
	EVCTRL_EVENTDIR_ALWAYS = 0,
	EVCTRL_EVENTDIR_DONTCARE = EVCTRL_EVENTDIR_ALWAYS,	
	EVCTRL_EVENTDIR_DEFAULT = EVCTRL_EVENTDIR_ALWAYS,	
	EVCTRL_EVENTDIR_UPCOUNT = 1,
	EVCTRL_EVENTDIR_DOWNCOUNT = 2
	
} SCT_eventDirection;

typedef enum SCT_stateV {
	
	STATEV_0 = 0,
	STATEV_DONTCARE = STATEV_0,
	STATEV_1,
	STATEV_2,
	STATEV_3,
	STATEV_4,
	STATEV_5,
	STATEV_6,
	STATEV_7,
	STATEV_8,
	STATEV_9,
	STATEV_10,
	STATEV_11,
	STATEV_12,
	STATEV_13,
	STATEV_14,
	STATEV_15,
	STATEV_16,
	STATEV_17,
	STATEV_18,
	STATEV_19,
	STATEV_20,
	STATEV_21,
	STATEV_22,
	STATEV_23,
	STATEV_24,
	STATEV_25,
	STATEV_26,
	STATEV_27,
	STATEV_28,
	STATEV_29,
	STATEV_30,
	STATEV_31

} SCT_stateV;


/* function to make the event control reg register */
typedef uint32_t SCT_eventCntrlReg;

static SCT_eventCntrlReg SCT_makeEventCntrlReg (
	SCT_MATCHREG		matchSel, 
	SCT_hEvent			hEvent,
	SCT_outSel			outSel,
	SCT_IoSel			ioSel,
	SCT_IoCond			ioCond,
	SCT_combMode		combMode,
	SCT_stateLd			stateLd,
	SCT_stateV			sctStateNum,
	SCT_matchMem		matchMem,
	SCT_eventDirection 	evDir
	
)  __attribute__((always_inline));

static SCT_eventCntrlReg SCT_makeEventCntrlReg (
	SCT_MATCHREG 		matchSel, 
	SCT_hEvent			hEvent,
	SCT_outSel			outSel,
	SCT_IoSel			ioSel,
	SCT_IoCond			ioCond,
	SCT_combMode		combMode,
	SCT_stateLd			stateLd,
	SCT_stateV			sctStateNum,
	SCT_matchMem		matchMem,
	SCT_eventDirection 	evDir)
{
    return (SCT_eventCntrlReg)(
		(matchSel << 0) 		|
		(hEvent << 4) 			|
		(outSel << 5)			|
		(ioSel << 6)			|
		(ioCond << 10)			|
		(combMode << 12) 		|
		(stateLd << 14) 		|
		(sctStateNum << 15)		|
		(matchMem << 20)		|
		(evDir << 21)
	);
}

#define STATE_SKIP_ON		STATEV_1
#define STATE_SKIP_OFF		STATEV_0


/* internal mapping of match registers */
#define MATCHREG_PERIOD 		SCT_MATCH_0
#define MATCHREG_RISING 		SCT_MATCH_1
#define MATCHREG_FALLING		SCT_MATCH_2
#define MATCHREG_RISING_DB		SCT_MATCH_3
#define MATCHREG_FALLING_DB		SCT_MATCH_4


#define MATCHREG_DEBUG_UP		SCT_MATCH_14
#define MATCHREG_DEBUG_DOWN		SCT_MATCH_15

/* function to configure the event control register */
static void SCT_writeEventCntrlReg (const SCT_EVENT eventId, const SCT_eventCntrlReg ctrlReg) __attribute__((always_inline));

static void SCT_writeEventCntrlReg(const SCT_EVENT eventId, const SCT_eventCntrlReg ctrlReg) {

	LPC_SCT->EVENT[eventId].CTRL = ctrlReg;
}


/* function to configure the event state register */
static void SCT_setEventActiveState(const SCT_EVENT eventId, SCT_STATE_MASK eventStateMask)  __attribute__((always_inline));                  
static void SCT_setEventActiveState(const SCT_EVENT eventId, SCT_STATE_MASK eventStateMask)  {

	LPC_SCT->EVENT[eventId].STATE = eventStateMask;
}

/* function to configure the match register */
static void SCT_setMatchRegister(const SCT_MATCHREG matchId, uint32_t matchRegValue)  __attribute__((always_inline));                  
static void SCT_setMatchRegister(const SCT_MATCHREG matchId, uint32_t matchValue)  {

	LPC_SCT->MATCH[matchId].U = matchValue;

}

/* function to configure the match reload register */
static void SCT_setMatchReloadRegister(const SCT_MATCHREG matchId, uint32_t matchRegValue)  __attribute__((always_inline));                  
static void SCT_setMatchReloadRegister(const SCT_MATCHREG matchId, uint32_t matchValue)  {

	LPC_SCT->MATCHREL[matchId].U = matchValue;

}

static uint32_t SCT_getMatchReloadRegister(const SCT_MATCHREG matchId)  __attribute__((always_inline));                  
static uint32_t SCT_getMatchReloadRegister(const SCT_MATCHREG matchId)  {

	return(LPC_SCT->MATCHREL[matchId].U);

}



/* definition of interrupt callback function */
typedef void (*intCallback_t) (void);


/* function to configure the event interrupts */
static void SCT_setEventInterrupts(uint16_t interruptMask)  __attribute__((always_inline));                  
static void SCT_setEventInterrupts(uint16_t interruptMask)  {

	LPC_SCT->EVEN = interruptMask; 

}

static void SCT_clearEventInterrupts(uint16_t interruptMask)  __attribute__((always_inline));                  
static void SCT_clearEventInterrupts(uint16_t interruptMask) {
	
	LPC_SCT->EVFLAG = interruptMask;
}

/* function to configure the output set register */
static void SCT_setOutputForEvent(const SCT_EVENT eventId, const SCT_OUT outputId)  __attribute__((always_inline));                  
static void SCT_setOutputForEvent(const SCT_EVENT eventId, const SCT_OUT outputId)  {

	LPC_SCT->OUT[outputId].SET |= (1u << eventId);
}

/* function to configure the output clear register */
static void SCT_clearOutputForEvent(const SCT_EVENT eventId, const SCT_OUT outputId)  __attribute__((always_inline));                  
static void SCT_clearOutputForEvent(const SCT_EVENT eventId, const SCT_OUT outputId)  {

	LPC_SCT->OUT[outputId].CLR |= (1u << eventId);
}

/* function to configure the output clear register */
static void SCT_setOutput(const SCT_OUT outputId)  __attribute__((always_inline));                  
static void SCT_setOutput(const SCT_OUT outputId)  {

	LPC_SCT->OUTPUT |= (1u << outputId);
}

static void SCT_clrOutput(const SCT_OUT outputId)  __attribute__((always_inline));                  
static void SCT_clrOutput(const SCT_OUT outputId)  {

	LPC_SCT->OUTPUT &= (~(1u << outputId));
}


typedef enum SCT_SETCLR {
	
	SETCLR_NOCHANGE = 0,
	SETCLR_DEFAULT = SETCLR_NOCHANGE,
	SETCLR_DONTCARE = SETCLR_NOCHANGE,	
	SETCLR_SET = 1,
	SETCLR_CLR = 2,
	SETCLR_TOGGLE = 3		
} SCT_SETCLR;


static void SCT_setConflictReg(const SCT_OUT outputId, const SCT_SETCLR mode)  __attribute__((always_inline));                  
static void SCT_setConflictReg(const SCT_OUT outputId, const SCT_SETCLR mode)  {

	LPC_SCT->RES |= (mode) << (outputId*2);
}

static void SCT_setLimitEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_setLimitEvent(const SCT_EVENT eventId)  {

	LPC_SCT->LIMIT_L |= (1u << eventId);
}

static void SCT_setHaltEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_setHaltEvent(const SCT_EVENT eventId)  {

	LPC_SCT->HALT_L |= (1u << eventId);
}

static void SCT_setStopEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_setStopEvent(const SCT_EVENT eventId)  {

	LPC_SCT->STOP_L |= (1u << eventId);
}

static void SCT_setStartEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_setStartEvent(const SCT_EVENT eventId)  {

	LPC_SCT->START_L |= (1u << eventId);
}

static void SCT_clrLimitEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_clrLimitEvent(const SCT_EVENT eventId)  {

	LPC_SCT->LIMIT_L &= ~(1u << eventId);
}

static void SCT_clrHaltEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_clrHaltEvent(const SCT_EVENT eventId)  {

	LPC_SCT->HALT_L &= ~(1u << eventId);
}

static void SCT_clrStopEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_clrStopEvent(const SCT_EVENT eventId)  {

	LPC_SCT->STOP_L &= ~(1u << eventId);
}

static void SCT_clrStartEvent(const SCT_EVENT eventId)  __attribute__((always_inline));                  
static void SCT_clrStartEvent(const SCT_EVENT eventId)  {

	LPC_SCT->START_L &= ~(1u << eventId);
}


typedef enum SCT_MODE {
	
	SCT_HALT,					/* halt the SCT */
	SCT_RELEASE_KEEP_STOPPED,	/* release but keep in stop (I/O events active) */
	SCT_STOP,					/* stop the SCT immediately */
	SCT_RUN,					/* run counter */
	SCT_NO_RELOAD,				/* disable match reloading */
	SCT_MATCH_RELOAD,			/* enable match reloading */
	SCT_RESET,					/* reset the whole block */
	SCT_SYNC_STOP,				/* allows sync stop on next period boundary */
	SCT_SYNC_STOP_REMOVE,		/* removes the stop condition */
	SCT_SYNC_HALT,				/* allows sync halt on next period boundary */
	SCT_SYNC_HALT_REMOVE		/* removes the halt condition */

} SCT_MODE;


static void SCT_changeMode(const SCT_MODE mode)  __attribute__((always_inline));                  
static void SCT_changeMode(const SCT_MODE mode)  {

	uint32_t reg;
	
	switch(mode) {
		
		case SCT_HALT:
			
			LPC_SCT->CTRL_U |= (1u << 2); // set halt bit
			break;
		
		case SCT_RELEASE_KEEP_STOPPED:
			
			reg = LPC_SCT->CTRL_U;
			reg |= (1u << 1); 		// set stop bit			
			reg &= ~(1u << 2);		// clear halt bit
			LPC_SCT->CTRL_U = reg;	
			break;

		case SCT_STOP:
			
			LPC_SCT->CTRL_U |= (1u << 1); // set stop bit
			break;

		case SCT_RUN:
			
			LPC_SCT->CTRL_U &= ~((1u << 2) | (1u << 1)); // clear halt bit and stop bit
			break;
		
		case SCT_NO_RELOAD:
			
			LPC_SCT->CONFIG |= (1u << 7); // prevent from reloading
			break;
		
		case SCT_MATCH_RELOAD:
			
			LPC_SCT->CONFIG &= ~(1u << 7);	// allow reloading		
			break;
		
		case SCT_RESET:
			
			reg = LPC_RGU->RESET_ACTIVE_STATUS1;
			/* If the M0 is being held in reset, do not release it */
			/* status register: 1 = no reset, 0 = reset asserted */
			/* control register: write 1 to keep in reset, 0 to release */
			if(reg & (1u << 24)) {
				// if not in reset, write a zero to keep bit clear
				LPC_RGU->RESET_CTRL1 = (1u << 5); 
			} 
			else { 
				// if in reset, write a one to keep in reset state
				LPC_RGU->RESET_CTRL1 = (1u << 24) | (1u << 5) ; 
			};	
			
			break;

		case SCT_SYNC_STOP:
			
			SCT_setStopEvent(EV_PERIOD);
			break;

		case SCT_SYNC_STOP_REMOVE:
			
			SCT_clrStopEvent(EV_PERIOD);
			break;

		case SCT_SYNC_HALT:
			
			SCT_setHaltEvent(EV_PERIOD);
			break;

		case SCT_SYNC_HALT_REMOVE:
			
			SCT_clrHaltEvent(EV_PERIOD);
			break;
	};
}




typedef uint32_t		SctClockFrequency;
typedef SCT_ClockBase	PwmClockBase;
typedef SCT_ClockPin	PwmClockInput;
typedef uint32_t		PwmClockFrequency;


#define MSEC_FREQUENCY	(1000000000)

#endif



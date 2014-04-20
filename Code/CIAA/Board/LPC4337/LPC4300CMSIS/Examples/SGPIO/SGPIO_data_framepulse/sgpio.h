/***********************************************************************
 * $Id: sgpio.h 3858 2010-07-15 15:33:12Z nxp28536 $
 *
 * Project: LPC43xx SGPIO emulation
 *
 * Description: HAL for SGPIO peripheral
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
 **********************************************************************/

#ifndef	__SGPIO_H__
#define __SGPIO_H__

#include "LPC43xx.h"
#include <stdint.h>
#include <stdbool.h>

/* assume running from PLL1 @ 204 MHz */
#define SGPIO_IP_CLOCK (204000000UL)


/* SGPIO slice definitions */
typedef enum SGPIO_Slice { 
	A = 0, 
	B, 
	C, 
	D, 
	E, 
	F, 
	G, 
	H, 
	I, 
	J, 
	K, 
	L, 
	M, 
	N, 
	O, 
	P, 
	NUM_SGPIO_SLICES 
	
} SGPIO_Slice;

/* SGPIO pin definitions */
typedef enum SGPIO_Pin { 	
	SGPIO_0 = 0, 
	SGPIO_1,
	SGPIO_2,
	SGPIO_3,
	SGPIO_4,
	SGPIO_5,
	SGPIO_6,
	SGPIO_7,
	SGPIO_8,
	SGPIO_9,
	SGPIO_10,
	SGPIO_11,
	SGPIO_12,
	SGPIO_13,
	SGPIO_14,
	SGPIO_15,
	NUM_SGPIO_PIN
} SGPIO_Pin;


/* types of signals defined for the pin */
typedef enum SGPIO_PinFunction { SGPIO_INPUT_PIN, SGPIO_OUTPUT_PIN, SGPIO_INTERNAL } SGPIO_PinFunction;


/******************************************************************************
 * SLICE_MUX_CFG register definitions 
 *****************************************************************************/
typedef enum SMC_MatchMode { 

	SMC_DONT_MATCH 		= 0x0, 
	SMC_MMODE_DEFAULT 	= SMC_DONT_MATCH,
	SMC_MMODE_DONTCARE 	= SMC_DONT_MATCH,	
	SMC_MATCH_DATA 		= 0x1

} SMC_MatchMode;

typedef enum SMC_ClkCaptureMode { 

	SMC_CLKCAP_RISING 	= 0x0, 
	SMC_CLKCAP_DEFAULT 	= SMC_CLKCAP_RISING,
	SMC_CLKCAP_DONTCARE = SMC_CLKCAP_RISING,	
	SMC_CLKCAP_FALLING 	= 0x1

} SMC_ClkCaptureMode;

typedef enum SMC_ClkGenMode { 

	SMC_CLKGEN_COUNTER 		= 0x0,
	SMC_CLKGEN_DEFAULT 		= SMC_CLKGEN_COUNTER,
	SMC_CLKGEN_DONTCARE 	= SMC_CLKGEN_COUNTER,			
	SMC_CLKGEN_SLICE_OR_PIN = 0x1

} SMC_ClkGenMode;


typedef enum SMC_InvOutClk { 

	SMC_INVOUT_NORMAL 	= 0x0, 
	SMC_INVOUT_DEFAULT 	= SMC_INVOUT_NORMAL,
	SMC_INVOUT_DONTCARE = SMC_INVOUT_NORMAL,	
	SMC_INVERTED 		= 0x1
	
} SMC_InvOutClk;

typedef	enum SMC_DataCapMode { 

	SMC_DATACAP_RISING 		= 0x0, 
	SMC_DATACAP_DEFAULT 	= SMC_DATACAP_RISING,
	SMC_DATACAP_DONTCARE 	= SMC_DATACAP_RISING,	
	SMC_DATACAP_FALLING 	= 0x1, 
	SMC_DATACAP_LOW 		= 0x2, 
	SMC_DATACAP_HIGH 		= 0x3 

} SMC_DataCapMode;

typedef	enum SMC_ParallelMode { 

	SMC_PAR_1BPCK 		= 0x0, 
	SMC_PAR_DEFAULT 	= SMC_PAR_1BPCK,
	SMC_PAR_DONTCARE 	= SMC_PAR_1BPCK,	
	SMC_PAR_2BPCK 		= 0x1, 
	SMC_PAR_4BPCK 		= 0x2, 
	SMC_PAR_8BPCK 		= 0x3 
	
} SMC_ParallelMode;

typedef enum SMC_InvQualifier { 

	SMC_INVQUAL_NORMAL 		= 0x0, 
	SMC_INVQUAL_DEFAULT 	= SMC_INVQUAL_NORMAL,
	SMC_INVQUAL_DONTCARE 	= SMC_INVQUAL_NORMAL,	
	SMC_INVQUAL_INVERTED 	= 0x1

} SMC_InvQualifier;

typedef uint32_t SGPIO_SliceMuxConfig;


/* function to make the slice mux register contents */
static SGPIO_SliceMuxConfig SGPIO_makeSliceMuxConfig (
	SMC_MatchMode 		matchMode, 
	SMC_ClkCaptureMode	clkCaptureMode,
	SMC_ClkGenMode		clkGenMode,
	SMC_InvOutClk		invOutClk,
	SMC_DataCapMode		dataCaptureMode,
	SMC_ParallelMode	parallelMode,
	SMC_InvQualifier	invQualifier)  __attribute__((always_inline));

static SGPIO_SliceMuxConfig SGPIO_makeSliceMuxConfig (
	SMC_MatchMode 		matchMode, 
	SMC_ClkCaptureMode	clkCaptureMode,
	SMC_ClkGenMode		clkGenMode,
	SMC_InvOutClk		invOutClk,
	SMC_DataCapMode		dataCaptureMode,
	SMC_ParallelMode	parallelMode,
	SMC_InvQualifier	invQualifier)
{
    return (SGPIO_SliceMuxConfig)(
        (matchMode << 0)		|
        (clkCaptureMode << 1) 	|
        (clkGenMode << 2)     	|
        (invOutClk << 3)  		|
        (dataCaptureMode << 4)	|        
		(parallelMode << 6)  	|
		(invQualifier << 8)
	);
}


/*****************************************************************************/


/******************************************************************************
 * SGPIO_MUX_CFG register definitions 
 *****************************************************************************/

typedef enum MC_ExtClkEnable { 

	MC_CLK_INTERNAL	= 0x0, 
	MC_CLK_DEFAULT 	= MC_CLK_INTERNAL,
	MC_CLK_DONTCARE = MC_CLK_INTERNAL,	
	MC_CLK_PIN 		= 0x1

} MC_ExtClkEnable;

typedef enum MC_ClkSrcPin { 

	MC_CLKSRC_SGPIO_8 		= 0x0, 
	MC_CLKSRC_PIN_DEFAULT 	= MC_CLKSRC_SGPIO_8,
	MC_CLKSRC_PIN_DONTCARE 	= MC_CLKSRC_SGPIO_8,	
	MC_CLKSRC_SGPIO_9 		= 0x1, 
	MC_CLKSRC_SGPIO_10 		= 0x2, 
	MC_CLKSRC_SGPIO_11 		= 0x3 

} MC_ClkSrcPin;

typedef enum MC_ClkSrcSlice {

	MC_CLKSRC_SLICE_D 			= 0x0, 
	MC_CLKSRC_SLICE_DEFAULT 	= MC_CLKSRC_SLICE_D,
	MC_CLKSRC_SLICE_DONTCARE 	= MC_CLKSRC_SLICE_D,	
	MC_CLKSRC_SLICE_H 			= 0x1, 
	MC_CLKSRC_SLICE_O 			= 0x2, 
	MC_CLKSRC_SLICE_P 			= 0x3 
 
 } MC_ClkSrcSlice;

typedef enum MC_QualMode { 

	MC_QUALMODE_ENABLE 		= 0x0, 
	MC_QUALMODE_DEFAULT		= MC_QUALMODE_ENABLE, 	
	MC_QUALMODE_DONTCARE	= MC_QUALMODE_ENABLE, 	
	MC_QUALMODE_DISABLE 	= 0x1, 
	MC_QUALMODE_SLICE 		= 0x2, 
	MC_QUALMODE_EXTPIN 		= 0x3

} MC_QualMode;

typedef enum MC_QualPinMode { 

	MC_QUALPIN_SGPIO_8 	= 0x0,
	MC_QUALPIN_DEFAULT 	= MC_QUALPIN_SGPIO_8,	
	MC_QUALPIN_DONTCARE = MC_QUALPIN_SGPIO_8,
	MC_QUALPIN_SGPIO_9 	= 0x1, 
	MC_QUALPIN_SGPIO_10 = 0x2, 
	MC_QUALPIN_SGPIO_11 = 0x3 

} MC_QualPinMode;

typedef enum MC_QualSliceMode { 

	MC_QUALSLICE_A_D 		= 0x0,
	MC_QUALSLICE_DEFAULT 	= MC_QUALSLICE_A_D,
	MC_QUALSLICE_DONTCARE 	= MC_QUALSLICE_A_D,	
	MC_QUALSLICE_H_O 		= 0x1, 
	MC_QUALSLICE_I_D 		= 0x2, 
	MC_QUALSLICE_P_O 		= 0x3 

} MC_QualSliceMode;

typedef enum MC_ConcatEnable { 

	MC_EXT_DATA_PIN 		= 0x0, 
	MC_CONCATEN_DEFAULT 	= MC_EXT_DATA_PIN,
	MC_CONCATEN_DONTCARE 	= MC_EXT_DATA_PIN,	
	MC_CONCAT_DATA 			= 0x1 

} MC_ConcatEnable;

typedef enum MC_ConcatOrder { 

	MC_CONCAT_SELF_LOOP 		= 0x0, 
	MC_CONCAT_ORDER_DEFAULT 	= MC_CONCAT_SELF_LOOP,
	MC_CONCAT_ORDER_DONTCARE	= MC_CONCAT_SELF_LOOP,
	MC_CONCAT_TWO_SLICES 		= 0x1, 
	MC_CONCAT_FOUR_SLICES 		= 0x2, 
	MC_CONCAT_EIGHT_SLICES 		= 0x3 

} MC_ConcatOrder;

typedef uint32_t SGPIO_MuxConfig;


/* function to make the mux register contents */
static SGPIO_MuxConfig SGPIO_makeMuxConfig (
	MC_ExtClkEnable 	extClkEnable,
	MC_ClkSrcPin 		clkSrcPin,
	MC_ClkSrcSlice 		clkSrcSlice,
	MC_QualMode 		qualMode,
	MC_QualPinMode 		qualPinMode,
	MC_QualSliceMode 	qualSliceMode,
	MC_ConcatEnable 	concatEnable,
	MC_ConcatOrder 	concatOrder)  __attribute__((always_inline));

static SGPIO_SliceMuxConfig SGPIO_makeMuxConfig (
	MC_ExtClkEnable 	extClkEnable,
	MC_ClkSrcPin 		clkSrcPin,
	MC_ClkSrcSlice 		clkSrcSlice,
	MC_QualMode 		qualMode,
	MC_QualPinMode 		qualPinMode,
	MC_QualSliceMode 	qualSliceMode,
	MC_ConcatEnable 	concatEnable,
	MC_ConcatOrder 		concatOrder)
{
    return (SGPIO_SliceMuxConfig)(
        (extClkEnable << 0)	|
        (clkSrcPin << 1) 	|
        (clkSrcSlice << 3)  |
        (qualMode << 5)  	|
        (qualPinMode << 7)	|        
		(qualSliceMode << 9)|
		(concatEnable << 11)|
		(concatOrder << 12)  	
	);
}


/*****************************************************************************/

/*****************************************************************************/
/* OUT_MUX_CFG register */
/*****************************************************************************/
typedef enum OMC_PinOutCfg { 

	OMC_DOUTM1 			= 0x0, 
	OMC_PINOUT_DEFAULT 	= OMC_DOUTM1,
	OMC_PINOUT_DONTCARE = OMC_DOUTM1,
	OMC_DOUTM2A 		= 0x1, 
	OMC_DOUTM2B 		= 0x2,
	OMC_DOUTM2C 		= 0x3,
	OMC_GPIO_OUT		= 0x4,
	OMC_DOUTM4A 		= 0x5, 
	OMC_DOUTM4B 		= 0x6,
	OMC_DOUTM4C 		= 0x7,
	OMC_CLKOUT 			= 0x8,
	OMC_DOUTM8A 		= 0x9, 
	OMC_DOUTM8B 		= 0xA,
	OMC_DOUTM8C 		= 0xB

} OMC_PinOutCfg;

typedef enum OMC_PinOeCfg { 

	OMC_GPIO_OE			= 0x0, 
	OMC_PINOE_DEFAULT 	= OMC_GPIO_OE,
	OMC_PINOE_DONTCARE 	= OMC_GPIO_OE,
	OMC_1BIT_OE 		= 0x4, 
	OMC_2BIT_OE 		= 0x5,
	OMC_4BIT_OE 		= 0x6,
	OMC_8BIT_OE			= 0x7

} OMC_PinOeCfg;

typedef uint32_t SGPIO_OutMuxConfig;


/* function to make the out mux register contents */
static SGPIO_OutMuxConfig SGPIO_makeOutMuxConfig (
	OMC_PinOutCfg 	pinOutCfg,
	OMC_PinOeCfg 	pinOeCfg
)  __attribute__((always_inline));
				
static SGPIO_OutMuxConfig SGPIO_makeOutMuxConfig (
	OMC_PinOutCfg 	pinOutCfg,
	OMC_PinOeCfg 	pinOeCfg)
{
    return (SGPIO_OutMuxConfig)(
        (pinOutCfg << 0)	|
        (pinOeCfg << 4)
	);
}

/*****************************************************************************/


typedef uint32_t 	dataReg32;
typedef uint8_t		sliceFunc;

/* these define the register values that can be changed dynamically via ioctl */
typedef enum  sliceIoctl { 
	RD_COUNT, 
	WR_COUNT, 
	RD_COUNT_RELOAD, 
	WR_COUNT_RELOAD, 
	RD_BIT_COUNT, 
	WR_BIT_COUNT, 
	RD_DATA, 
	WR_DATA, 
	RD_DATASS, 
	WR_DATASS 

} sliceIoctl;
	
/*****************************************************************************/
/* basic slice configuration */
/*****************************************************************************/
typedef struct SgpioSliceCfg  {
	
	SGPIO_Slice			sliceId;  		// slice identification
	SGPIO_PinFunction	sliceFunc;		// slice function
	SGPIO_Pin			pinId;			// which device pin
		
} SgpioSliceCfg;



/* functions to write the configuration registers */

/* function to configure the out mux register */
static void SGPIO_configOutMuxReg (const SGPIO_Pin pinId, const SGPIO_OutMuxConfig config) __attribute__((always_inline));

/* function to configure the mux register */
static void SGPIO_configMuxReg (const SGPIO_Slice sliceId, const SGPIO_MuxConfig config) __attribute__((always_inline));

/* function to configure the slice mux register */
static void SGPIO_configSliceMuxReg (const SGPIO_Slice sliceId, const SGPIO_SliceMuxConfig config) __attribute__((always_inline));


/* miscellaneous internal functions to handle start, stop, output enable for the slices */
static void SGPIO_setOeReg (const SGPIO_Pin pinId, const SGPIO_OutMuxConfig config) __attribute__((always_inline));
static void SGPIO_clearOeReg (const SGPIO_Pin pinId) __attribute__((always_inline));
static void SGPIO_disableSlices(const uint32_t sliceMask) __attribute__((always_inline));
static void SGPIO_clearDisableSlices(const uint32_t sliceMask) __attribute__((always_inline));
static void SGPIO_enableSlices(const bool stop, const uint32_t sliceMask) __attribute__((always_inline));
static void SGPIO_disableSlice(SGPIO_Slice sliceId) __attribute__((always_inline));
static void SGPIO_enableSlice(const bool stop, const SGPIO_Slice sliceId) __attribute__((always_inline));

/* miscellanaous internal functions to write the counter and data registers */
static void SGPIO_setCountReg(const SGPIO_Slice sliceId, const uint16_t param) __attribute__((always_inline));
static void SGPIO_setCountReloadReg(const SGPIO_Slice sliceId, const uint16_t param) __attribute__((always_inline));
static void SGPIO_setBitCountReg(const SGPIO_Slice sliceId, const uint16_t param) __attribute__((always_inline));
static void SGPIO_writeDataReg(const SGPIO_Slice sliceId, const uint32_t param) __attribute__((always_inline));
static void SGPIO_readDataReg(const SGPIO_Slice sliceId, const uint8_t wordLength, uint32_t* param) __attribute__((always_inline));
static void SGPIO_writeDataShadowReg(const SGPIO_Slice sliceId, const uint32_t param) __attribute__((always_inline));
static void SGPIO_readDataShadowReg(const SGPIO_Slice sliceId, const uint8_t wordLength, uint32_t* param) __attribute__((always_inline));



#define STOP 		true
#define DONTSTOP	false

/* function to configure the out mux register */

static void SGPIO_configMuxReg (const SGPIO_Slice sliceId, const SGPIO_MuxConfig config) {
	
	LPC_SGPIO->SGPIO_MUX_CFG[sliceId] = config;	

}


static void SGPIO_configSliceMuxReg (const SGPIO_Slice sliceId, const SGPIO_MuxConfig config) {
	
	LPC_SGPIO->SLICE_MUX_CFG[sliceId] = config;	

}

static void SGPIO_configOutMuxReg (const SGPIO_Pin pinId, const SGPIO_MuxConfig config) {
	
	LPC_SGPIO->OUT_MUX_CFG[pinId] = config;	

}


#define GPIO_OE_MASK (0x70)

static void SGPIO_setOeReg (const SGPIO_Pin pinId, const SGPIO_OutMuxConfig config) {
	
	if((config & GPIO_OE_MASK) == OMC_GPIO_OE) 
		LPC_SGPIO->GPIO_OENREG |= (0x1 << pinId);
}

static void SGPIO_clearOeReg (const SGPIO_Pin pinId) {
	
	LPC_SGPIO->GPIO_OENREG &= (~(0x1 << pinId));
}


static void SGPIO_disableSlices(const uint32_t sliceMask) {
	
	LPC_SGPIO->CTRL_ENABLED	 &= (~sliceMask);

}

static void SGPIO_clearDisableSlices(const uint32_t sliceMask) {
	
	LPC_SGPIO->CTRL_DISABLED &= (~sliceMask);

}

static void SGPIO_enableSlices(const bool stop, const uint32_t sliceMask) {
	
	LPC_SGPIO->CTRL_ENABLED	 |= sliceMask;
	if(stop == true) LPC_SGPIO->CTRL_DISABLED |= sliceMask;

}

static void SGPIO_disableSlice(SGPIO_Slice sliceId) {
	
	LPC_SGPIO->CTRL_ENABLED	 &= (~(1 << sliceId));
	LPC_SGPIO->CTRL_DISABLED	 &= (~(1 << sliceId));

}

static void SGPIO_enableSlice(const bool stop, const SGPIO_Slice sliceId) {
	
	LPC_SGPIO->CTRL_ENABLED |= (1 << sliceId);
	if(stop == true) LPC_SGPIO->CTRL_DISABLED |= (1 << sliceId);

}

#define BITPOS_MIN	1
#define BITPOS_MAX	256

#define COUNT_RELOAD_MAX 4096
#define COUNT_RELOAD_MIN 1

#define COUNT_MAX 4096

/* status types for the spi emulation */
typedef enum emuStatus { EMU_OK = 0, EMU_INVALID_ID, ERR_SGPIO_BITPOS, ERR_SGPIO_COUNT_RELOAD, ERR_SGPIO_COUNT, EMU_ERROR } emuStatus;

extern emuStatus emuError;

static void SGPIO_setCountReg(const SGPIO_Slice sliceId, const uint16_t param) {
	
	/* static range check, shall be compiled out */
	if((uint32_t) param > COUNT_MAX) {
		emuError = ERR_SGPIO_COUNT;
		return;
	}
		
		
	LPC_SGPIO->COUNT[sliceId] = (uint32_t) param & 0xFFF;
}

static void SGPIO_setCountReloadReg(const SGPIO_Slice sliceId, const uint16_t param) {
	
	/* static range check, shall be compiled out when included in the same module */
	if((((uint32_t) param) < COUNT_RELOAD_MIN) || (((uint32_t)param) > COUNT_RELOAD_MAX)) {
		emuError = ERR_SGPIO_COUNT_RELOAD;
		return;
	}
	
	LPC_SGPIO->PRESET[sliceId] = ((uint32_t)param & 0x1FFF) - 1;
}

static void SGPIO_setBitCountReg(const SGPIO_Slice sliceId, const uint16_t param) {
	
	/* static range check, shall be compiled out when included in the same module */
	if((((uint32_t) param) < BITPOS_MIN) || ((uint32_t)param > BITPOS_MAX)) {
		emuError = ERR_SGPIO_BITPOS;
		return;
	}
	
	LPC_SGPIO->POS[sliceId] = ((((uint32_t)param & 0x1FF) - 1) << 8) | ((uint32_t)param & 0x1FF) - 1;
}

static void SGPIO_writeDataReg(const SGPIO_Slice sliceId, const uint32_t param) {
	
	LPC_SGPIO->REG[sliceId] = param;
}

static void SGPIO_readDataReg(const SGPIO_Slice sliceId, const uint8_t wordLength, uint32_t* param) {
	
	*((uint32_t*)param) = LPC_SGPIO->REG[sliceId] >> (32 - (wordLength));

}

static void SGPIO_writeDataShadowReg(const SGPIO_Slice sliceId, const uint32_t param) {
	
	LPC_SGPIO->REG_SS[sliceId] = (uint32_t) param;
}

static void SGPIO_readDataShadowReg(const SGPIO_Slice sliceId, const uint8_t wordLength, uint32_t* param) {
	
	*((uint32_t*)param) = LPC_SGPIO->REG_SS[sliceId] >> (32 - (wordLength));
}



#endif

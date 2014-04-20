/***********************************************************************
 * $Id: SGPIO_makeFrame0Config.c 3858 2010-07-15 15:33:12Z nxp28536 $
 *
 * Project: LPC43xx SGPIO emulation
 *
 * Description: configuration for the frame sync + 32-bit data emulation
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
#include <stdint.h>
#include "sgpio_frame.h"

extern const SgpioSliceCfg FRAME_CFG0;
extern const SgpioSliceCfg DATA_CFG0;
extern const SgpioSliceCfg CLOCK_CFG0;
extern const SgpioSliceCfg INTERNAL_FRAME_CFG0;


/* configure desired serial line frequency */
#define FRAME_CLOCK (51000000UL)

/* configure desired serial line frequency */
#define CLOCK_PATTERN (0xAAAAAAAA)

/* configure desired serial line frequency */
#define NUM_BITS (32)


void SGPIO_makeFrame0Config() {

	SGPIO_SliceMuxConfig 	sliceMuxCfg;
	SGPIO_MuxConfig 		muxCfg;
	SGPIO_OutMuxConfig		outMuxCfg;

	/**********************************************************************
	* make the static configuration for the pulse, slice I		
	*
	**********************************************************************/
	SGPIO_disableSlice(FRAME_CFG0.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_RISING,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_DONTCARE);
	
	SGPIO_configSliceMuxReg(FRAME_CFG0.sliceId, sliceMuxCfg);
	
	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_DONTCARE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_DONTCARE,
		MC_CONCAT_DATA,
		MC_CONCAT_SELF_LOOP);
	
	SGPIO_configMuxReg(FRAME_CFG0.sliceId, muxCfg);
	
	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);

	SGPIO_configOutMuxReg(FRAME_CFG0.pinId, outMuxCfg);

	if(FRAME_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(FRAME_CFG0.pinId, outMuxCfg);

	SGPIO_setBitCountReg(FRAME_CFG0.sliceId, NUM_BITS);

	// shift bits at half speed
	SGPIO_setCountReloadReg(FRAME_CFG0.sliceId, SGPIO_IP_CLOCK / FRAME_CLOCK);

	// data is looping back forever
	SGPIO_writeDataReg(FRAME_CFG0.sliceId, 0x1);
	
	
	/**********************************************************************
	* make the static configuration for the Clock, slice D		
	*
	**********************************************************************/
	SGPIO_disableSlice(CLOCK_CFG0.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_RISING,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_DONTCARE);
	
	SGPIO_configSliceMuxReg(CLOCK_CFG0.sliceId, sliceMuxCfg);
	
	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_DONTCARE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_DONTCARE,
		MC_CONCAT_DATA,
		MC_CONCAT_SELF_LOOP);
	
	SGPIO_configMuxReg(CLOCK_CFG0.sliceId, muxCfg);
	
	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);

	SGPIO_configOutMuxReg(CLOCK_CFG0.pinId, outMuxCfg);

	if(CLOCK_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(CLOCK_CFG0.pinId, outMuxCfg);

	SGPIO_setBitCountReg(CLOCK_CFG0.sliceId, 2*NUM_BITS );

	// clock pattern is shifted twice as fast for the serial clock
	SGPIO_setCountReloadReg(CLOCK_CFG0.sliceId, SGPIO_IP_CLOCK/(2*FRAME_CLOCK));
	
	SGPIO_writeDataReg(CLOCK_CFG0.sliceId, CLOCK_PATTERN);
	
	/**********************************************************************
	* make the static configuration for the Data out pin, slice A		
	*
	**********************************************************************/
	SGPIO_disableSlice(DATA_CFG0.sliceId);

	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_DONT_MATCH,
		SMC_CLKCAP_RISING,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_DONTCARE);
	
	SGPIO_configSliceMuxReg(DATA_CFG0.sliceId, sliceMuxCfg);
	
	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_DONTCARE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_DONTCARE,
		MC_CONCATEN_DONTCARE,
		MC_CONCAT_ORDER_DONTCARE);

	SGPIO_configMuxReg(DATA_CFG0.sliceId, muxCfg);
	
	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);
	// change for testing to clk output
	// outMuxCfg = SGPIO_makeOutMuxConfig(OMC_CLKOUT, OMC_GPIO_OE);
	
	SGPIO_configOutMuxReg(DATA_CFG0.pinId, outMuxCfg);

	if(DATA_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(DATA_CFG0.pinId, outMuxCfg);
			
	SGPIO_setBitCountReg(DATA_CFG0.sliceId, NUM_BITS);

	SGPIO_setCountReloadReg(DATA_CFG0.sliceId, SGPIO_IP_CLOCK / FRAME_CLOCK);
	
	// preset to one bit time so that it does not shift immediately
	SGPIO_setCountReg(DATA_CFG0.sliceId, SGPIO_IP_CLOCK / FRAME_CLOCK);
	
	SGPIO_writeDataReg(DATA_CFG0.sliceId, 0x0);
	SGPIO_writeDataShadowReg(DATA_CFG0.sliceId, 0x0);	
	
}



#include <stdint.h>

#include "sgpio_spi.h"

extern CsPattern cs0Waveform;
extern CsPattern cs0upWaveform;
extern const CsPattern ChipSelectWaveform[NUM_CS_IDX];

extern const SgpioSliceCfg CHIP_SELECT_CFG0;
extern const SgpioSliceCfg CHIP_SELECT_CFG0_INTERNAL;
extern const SgpioSliceCfg DATA_OUT_CFG0;
extern const SgpioSliceCfg CLOCK_CFG0;
extern const SgpioSliceCfg DATA_IN_CFG0;

void SGPIO_makeMaster0Cpha1LowConfig(SpiParam const *config) {

	SGPIO_SliceMuxConfig 	sliceMuxCfg;
	SGPIO_MuxConfig 		muxCfg;
	SGPIO_OutMuxConfig		outMuxCfg;
		
	/**********************************************************************
	* make the static configuration for the Chip Select, slice A + I		
	*
	**********************************************************************/
	cs0Waveform = ChipSelectWaveform[config->chipSelect];

	if(config->wordLenght == DBIT_30) {
		
		// for the 30-bit case
		cs0upWaveform = 0x1;				
		
	} else if (config->wordLenght == DBIT_32) {
			
		// for the 32-bit case
		cs0upWaveform = 0x4;
	};

	SGPIO_disableSlice(CHIP_SELECT_CFG0.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_DONTCARE,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_INVERTED);

	SGPIO_configSliceMuxReg(CHIP_SELECT_CFG0.sliceId, sliceMuxCfg);
	
	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_SLICE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_A_D,
		MC_CONCAT_DATA,
		MC_CONCAT_TWO_SLICES);

	SGPIO_configMuxReg(CHIP_SELECT_CFG0.sliceId, muxCfg);

	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);

	SGPIO_configOutMuxReg(CHIP_SELECT_CFG0.pinId, outMuxCfg);

	if(CHIP_SELECT_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(CHIP_SELECT_CFG0.pinId, outMuxCfg);

	SGPIO_spiIoctl(CHIP_SELECT_CFG0.sliceId, WR_BIT_COUNT, (void*)((config->wordLenght)+2));
	SGPIO_spiIoctl(CHIP_SELECT_CFG0.sliceId, WR_COUNT_RELOAD, (void*)(SGPIO_IP_CLOCK/(2*(config->bitRateHz))));
	SGPIO_spiIoctl(CHIP_SELECT_CFG0.sliceId, WR_DATA, (void*)cs0Waveform);			
	
	// Slice I is part of the Chip Select
	SGPIO_disableSlice(CHIP_SELECT_CFG0_INTERNAL.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_DONTCARE,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_INVERTED);
	
	SGPIO_configSliceMuxReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, sliceMuxCfg);

	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_SLICE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_I_D,
		MC_CONCAT_DATA,
		MC_CONCAT_TWO_SLICES);
	
	SGPIO_configMuxReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, muxCfg);
	
	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);
	SGPIO_configOutMuxReg(CHIP_SELECT_CFG0_INTERNAL.pinId, outMuxCfg);
	
	/* is an internal pin, could be skipped */
	if(CHIP_SELECT_CFG0_INTERNAL.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(CHIP_SELECT_CFG0_INTERNAL.pinId, outMuxCfg);

	SGPIO_spiIoctl(CHIP_SELECT_CFG0_INTERNAL.sliceId, WR_BIT_COUNT, (void*)((config->wordLenght)+2));
	SGPIO_spiIoctl(CHIP_SELECT_CFG0_INTERNAL.sliceId, WR_COUNT_RELOAD, (void*) (SGPIO_IP_CLOCK/(2*(config->bitRateHz))));
	SGPIO_spiIoctl(CHIP_SELECT_CFG0_INTERNAL.sliceId, WR_DATA, (void*)cs0upWaveform);		
	
	/**********************************************************************
	* make the static configuration for the Clock, slice D		
	*
	**********************************************************************/
	SGPIO_disableSlice(CLOCK_CFG0.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_DONTCARE,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_INVERTED);
	
	SGPIO_configSliceMuxReg(CLOCK_CFG0.sliceId, sliceMuxCfg);

	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_SLICE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_A_D,
		MC_CONCAT_DATA,
		MC_CONCAT_SELF_LOOP);
	
	SGPIO_configMuxReg(CLOCK_CFG0.sliceId, muxCfg);

	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);
	SGPIO_configOutMuxReg(CLOCK_CFG0.pinId, outMuxCfg);

	if(CLOCK_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(CLOCK_CFG0.pinId, outMuxCfg);
	
	SGPIO_spiIoctl(CLOCK_CFG0.sliceId, WR_DATA, (void*)0xAAAAAAAA); 	
	SGPIO_spiIoctl(CLOCK_CFG0.sliceId, WR_BIT_COUNT, (void*)((config->wordLenght)*2));
	SGPIO_spiIoctl(CLOCK_CFG0.sliceId, WR_COUNT_RELOAD, (void*)(SGPIO_IP_CLOCK/(2*(config->bitRateHz))));
	
	/**********************************************************************
	* make the static configuration for the Data out pin, slice E		
	*
	**********************************************************************/
	SGPIO_disableSlice(DATA_OUT_CFG0.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_FALLING,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_INVERTED);
	
	SGPIO_configSliceMuxReg(DATA_OUT_CFG0.sliceId, sliceMuxCfg);

	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_PIN,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_SLICE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_A_D,
		MC_CONCATEN_DONTCARE,
		MC_CONCAT_ORDER_DONTCARE);
	
	SGPIO_configMuxReg(DATA_OUT_CFG0.sliceId, muxCfg);

	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);
	// just for testing
	// outMuxCfg = SGPIO_makeOutMuxConfig(OMC_CLKOUT, OMC_GPIO_OE);
	SGPIO_configOutMuxReg(DATA_OUT_CFG0.pinId, outMuxCfg);
	
	if(DATA_OUT_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(DATA_OUT_CFG0.pinId, outMuxCfg);
			
	SGPIO_spiIoctl(DATA_OUT_CFG0.sliceId, WR_BIT_COUNT, (void*)((config->wordLenght)-1));
	
	// zero out just to start from clean
	SGPIO_spiIoctl(DATA_OUT_CFG0.sliceId, WR_DATA, (void*)0x0);
	SGPIO_spiIoctl(DATA_OUT_CFG0.sliceId, WR_DATASS, (void*)0x0);
	SGPIO_spiIoctl(DATA_OUT_CFG0.sliceId, WR_COUNT_RELOAD, (void*)((SGPIO_IP_CLOCK/(config->bitRateHz))));
	SGPIO_spiIoctl(DATA_OUT_CFG0.sliceId, WR_COUNT, (void*)((SGPIO_IP_CLOCK/(config->bitRateHz))));	

	/**********************************************************************
	* make the static configuration for the Data in pin, slice F		
	*
	**********************************************************************/
	SGPIO_disableSlice(DATA_IN_CFG0.sliceId);
	
	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
		SMC_MATCH_DATA,
		SMC_CLKCAP_RISING,
		SMC_CLKGEN_COUNTER,
		SMC_INVOUT_DONTCARE,
		SMC_DATACAP_DONTCARE,
		SMC_PAR_1BPCK,
		SMC_INVQUAL_INVERTED);
	
	SGPIO_configSliceMuxReg(DATA_IN_CFG0.sliceId, sliceMuxCfg);

	muxCfg = SGPIO_makeMuxConfig(
		MC_CLK_INTERNAL,
		MC_CLKSRC_PIN_DONTCARE,
		MC_CLKSRC_SLICE_DONTCARE,
		MC_QUALMODE_SLICE,
		MC_QUALPIN_DONTCARE,
		MC_QUALSLICE_A_D,
		MC_EXT_DATA_PIN,
		MC_CONCAT_ORDER_DONTCARE);
	
	SGPIO_configMuxReg(DATA_IN_CFG0.sliceId, muxCfg);

	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);
	SGPIO_configOutMuxReg(DATA_IN_CFG0.pinId, outMuxCfg);
	// just for testing
// 	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_CLKOUT, OMC_GPIO_OE);
// 	SGPIO_configOutMuxReg((SGPIO_Pin)5, outMuxCfg);

	if(DATA_IN_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
		SGPIO_setOeReg(DATA_IN_CFG0.pinId, outMuxCfg);
	// just for testing
// 	SGPIO_setOeReg((SGPIO_Pin)5, outMuxCfg);
	
	SGPIO_spiIoctl(DATA_IN_CFG0.sliceId, WR_BIT_COUNT, (void*)config->wordLenght);
	SGPIO_spiIoctl(DATA_IN_CFG0.sliceId, WR_DATA, (void*) 0x0);
	SGPIO_spiIoctl(DATA_IN_CFG0.sliceId, WR_DATASS, (void*) 0x0);
	SGPIO_spiIoctl(DATA_IN_CFG0.sliceId, WR_COUNT_RELOAD, (void*)((SGPIO_IP_CLOCK/(config->bitRateHz))));	

// 	/**********************************************************************
// 	* generate the qualifier for delaying the first clock pulse, slice M		
// 	*
// 	**********************************************************************/
// 	SGPIO_disableSlice(DATA_DELAY_CPHA1_CFG0.sliceId);
// 	
// 	sliceMuxCfg = SGPIO_makeSliceMuxConfig(
// 		SMC_MATCH_DATA,
// 		SMC_CLKCAP_FALLING,
// 		SMC_CLKGEN_COUNTER,
// 		SMC_INVOUT_DONTCARE,
// 		SMC_DATACAP_DONTCARE,
// 		SMC_PAR_1BPCK,
// 		SMC_INVQUAL_NORMAL);
// 	
// 	SGPIO_configSliceMuxReg(DATA_DELAY_CPHA1_CFG0.sliceId, sliceMuxCfg);

// 	muxCfg = SGPIO_makeMuxConfig(
// 		MC_CLK_INTERNAL,
// 		MC_CLKSRC_PIN_DONTCARE,
// 		MC_CLKSRC_SLICE_DONTCARE,
// 		MC_QUALMODE_DONTCARE,
// 		MC_QUALPIN_DONTCARE,
// 		MC_QUALSLICE_DONTCARE,
// 		MC_CONCATEN_DONTCARE,
// 		MC_CONCAT_ORDER_DONTCARE);
// 	
// 	SGPIO_configMuxReg(DATA_DELAY_CPHA1_CFG0.sliceId, muxCfg);

// 	outMuxCfg = SGPIO_makeOutMuxConfig(OMC_DOUTM1, OMC_GPIO_OE);
// 	SGPIO_configOutMuxReg(DATA_DELAY_CPHA1_CFG0.pinId, outMuxCfg);

// 	if(DATA_DELAY_CPHA1_CFG0.sliceFunc == SGPIO_OUTPUT_PIN)
// 		SGPIO_setOeReg(DATA_DELAY_CPHA1_CFG0.pinId, outMuxCfg);
// 			
// 	SGPIO_spiIoctl(DATA_DELAY_CPHA1_CFG0.sliceId, WR_BIT_COUNT, (void*)3);
// 	SGPIO_spiIoctl(DATA_DELAY_CPHA1_CFG0.sliceId, WR_COUNT_RELOAD, (void*)(SGPIO_IP_CLOCK/(2*(config->bitRateHz))));
// 	
// 	// bit delay pattern b110
// 	SGPIO_spiIoctl(DATA_DELAY_CPHA1_CFG0.sliceId, WR_DATA, (void*)0x3);	
	
}

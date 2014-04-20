/***********************************************************************
 * $Id: sgpio_frame.c 3858 2010-07-15 15:33:12Z nxp28536 $
 *
 * Project: LPC43xx SGPIO emulation
 *
 * Description: 32-bit wide data transmission with frame sync pulse
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
#include <stdbool.h>

#include "lpc43xx_scu.h"

#include "sgpio_frame.h"

/******************************************************************************
* USER SLICE ASSIGNMENT SECTION
******************************************************************************/ 


/******************************************************************************
* SLICE B is used to generate the clock
******************************************************************************/
const SgpioSliceCfg CLOCK_CFG0 = {

	.sliceId = B,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_8
};


/******************************************************************************
* SLICE I is used to generate the pulse
******************************************************************************/
const SgpioSliceCfg FRAME_CFG0 = {

	.sliceId = I,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_1

};

/******************************************************************************
* SLICE A is used to send the data
******************************************************************************/
const SgpioSliceCfg DATA_CFG0 = {

	.sliceId = A,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_0

};



/******************************************************************************
* END OF USER SLICE ASSIGNMENT SECTION
* DO NOT MODIFY BELOW THIS LINE
******************************************************************************/ 


static volatile uint32_t frame0Mask;
extern void SGPIO_makeFrame0Config(void);


/* used to track configuration errors, shall be compiled out */
emuStatus emuError = EMU_OK;


/**********************************************************************
 ** Function name: SgpioIrqInit
 **
 ** Description:		
 **
 ** Parameters:			
 **
 ** Returned value:		
 **********************************************************************/
static void SGPIO_frameEmuIrqInit(void) {

	NVIC_DisableIRQ(SGPIO_IINT_IRQn);
	
	LPC_SGPIO->CTR_STATUS_1 = (1<<DATA_CFG0.sliceId);		// clear capture interrupt status
	while(LPC_SGPIO->STATUS_1 & (1<<DATA_CFG0.sliceId));		// wait for status to clear	
	
	// Enable SGPIO interrupt (shift/capture/match/input are or'ed)
	NVIC_EnableIRQ(SGPIO_IINT_IRQn);
}

/**********************************************************************
 ** Function name: EVRT_IRQHandler		
 **
 ** Description:		
 **							
 ** Parameters:			
 **
 ** Returned value:		
 **********************************************************************/
static int8_t volatile frame0Ready = 0;

bool isFrameReady(void) {
	
	return(frame0Ready);
}

void SGPIO_IRQHandler(void) 
{

	// for debug purpose
	LPC_GPIO_PORT->NOT[4] |= (1UL << 13);

	// sgpio capture interrrupt flag from chip select
	if((LPC_SGPIO->STATUS_1) & (1<<DATA_CFG0.sliceId))								
	{					
		LPC_SGPIO->CTR_STATUS_1 = (1<<DATA_CFG0.sliceId);						
		frame0Ready = 1;
	}
	
	// just for debug
	LPC_GPIO_PORT->NOT[4] |= (1UL << 13);
}

emuStatus SGPIO_frameIoctl(const SGPIO_Slice sliceId, const sliceIoctl cmd, const void* param) {
	
	switch(cmd) {
	
		case WR_COUNT:
			
			/* static range check, will be compiled out */
			if((uint32_t) param > COUNT_MAX) return(ERR_SGPIO_COUNT);
		
			LPC_SGPIO->COUNT[sliceId] = (uint32_t) param & 0xFFF;
		
		break;
	
		case WR_COUNT_RELOAD:
			
			/* static range check, shall be compiled out when included in the same module */
			if((((uint32_t) param) < COUNT_RELOAD_MIN) || (((uint32_t)param) > COUNT_RELOAD_MAX)) return(ERR_SGPIO_COUNT_RELOAD);

			LPC_SGPIO->PRESET[sliceId] = ((uint32_t)param & 0x1FFF) - 1;
		
		break;

		case WR_BIT_COUNT:
		
			/* static range check, shall be compiled out when included in the same module */
			if((((uint32_t) param) < BITPOS_MIN) || ((uint32_t)param > BITPOS_MAX)) return(ERR_SGPIO_BITPOS);
			
			LPC_SGPIO->POS[sliceId] = ((((uint32_t)param & 0x1FF) - 1) << 8) | ((uint32_t)param & 0x1FF) - 1;
		
		break;

		case WR_DATA:
			
			LPC_SGPIO->REG[sliceId] = (uint32_t) param;
		
		break;

		
		case WR_DATASS:
			
			LPC_SGPIO->REG_SS[sliceId] = (uint32_t) param;
		
		break;
				
	};
	
	return(EMU_OK);
}


void SGPIO_frameWrite(uint32_t dataWrite) {

	frame0Ready = 0;

	SGPIO_writeDataShadowReg(DATA_CFG0.sliceId, dataWrite);			
}


void SGPIO_frameInit(void) {
	
	// reset the SGPIO block
	LPC_RGU->RESET_CTRL1 = (LPC_RGU->RESET_ACTIVE_STATUS1) | (1 << 25); 
	
	frame0Ready = 0;
	frame0Mask = 0;

	// init interrupts
	SGPIO_frameEmuIrqInit();
}



void SGPIO_frameOpen(void) {
	
		
	/* Apply SGPIO configuration for Keil MCB1800/4300 board */				
	// CLOCK, P1.1 : SGPIO8 - A6
	// activate input to propagate the clock back into the chip on
	// SGPIO pin 8
	scu_pinmux(0x1 ,1 , PUP_ENABLE | INBUF_ENABLE , FUNC3);	

	// PULSE, P0.1 : SGPIO1 - TX EN
	scu_pinmux(0x0 ,1 , PUP_ENABLE, FUNC3);	

	// DATA, P9.0 : SGPIO0 - LED
	scu_pinmux(0x9 ,0 , PUP_ENABLE, FUNC6);	

	// DELAY, P2.3: SGPIO12 - <Uart3 Tx		
	// visible for debug purpose, internal signal
	scu_pinmux(0x2 ,3 , PUP_ENABLE, FUNC0);
		
	// prepare the frame mask for the enable / disable steps		
	frame0Mask = (	(1U << FRAME_CFG0.sliceId) |\
					(1U << CLOCK_CFG0.sliceId) |\
					(1U << DATA_CFG0.sliceId));
	
	// apply configuration to SGPIO
	SGPIO_makeFrame0Config();

	// enable capture interrupt for slice DATA
	LPC_SGPIO->SET_EN_1 = (1<<DATA_CFG0.sliceId);	

	// enable the slices
	SGPIO_enableSlices(DONTSTOP, frame0Mask);	

}

	
void SGPIO_frameClose(void) {

	SGPIO_SliceMuxConfig 	sliceMuxCfg_default;
	SGPIO_MuxConfig 		muxCfg_default;
	SGPIO_OutMuxConfig		outMuxCfg_default;
			
	sliceMuxCfg_default = SGPIO_makeSliceMuxConfig(
			SMC_MMODE_DEFAULT,
			SMC_CLKCAP_DEFAULT,
			SMC_CLKGEN_DEFAULT,
			SMC_INVOUT_DEFAULT,
			SMC_DATACAP_DEFAULT,
			SMC_PAR_DEFAULT,
			SMC_INVQUAL_DEFAULT);
		
	muxCfg_default = SGPIO_makeMuxConfig(
			MC_CLK_DEFAULT,
			MC_CLKSRC_PIN_DEFAULT,
			MC_CLKSRC_SLICE_DEFAULT,
			MC_QUALMODE_DEFAULT,
			MC_QUALPIN_DEFAULT,
			MC_QUALSLICE_DEFAULT,
			MC_CONCATEN_DEFAULT,
			MC_CONCAT_ORDER_DEFAULT);
		
	outMuxCfg_default = SGPIO_makeOutMuxConfig(OMC_PINOUT_DEFAULT, OMC_PINOE_DEFAULT);

	SGPIO_disableSlices(frame0Mask);	
		
	// revert the static configuration
	SGPIO_configSliceMuxReg(FRAME_CFG0.sliceId, sliceMuxCfg_default);
	SGPIO_configMuxReg(FRAME_CFG0.sliceId, muxCfg_default);
	SGPIO_configOutMuxReg(FRAME_CFG0.pinId, outMuxCfg_default);
	SGPIO_clearOeReg(FRAME_CFG0.pinId);
	
	SGPIO_configSliceMuxReg(INTERNAL_FRAME_CFG0.sliceId, sliceMuxCfg_default);
	SGPIO_configMuxReg(INTERNAL_FRAME_CFG0.sliceId, muxCfg_default);
	SGPIO_configOutMuxReg(INTERNAL_FRAME_CFG0.pinId, outMuxCfg_default);
	SGPIO_clearOeReg(INTERNAL_FRAME_CFG0.pinId);

	SGPIO_configSliceMuxReg(CLOCK_CFG0.sliceId, sliceMuxCfg_default);
	SGPIO_configMuxReg(CLOCK_CFG0.sliceId, muxCfg_default);
	SGPIO_configOutMuxReg(CLOCK_CFG0.pinId, outMuxCfg_default);
	SGPIO_clearOeReg(CLOCK_CFG0.pinId);
	
	SGPIO_configSliceMuxReg(DATA_CFG0.sliceId, sliceMuxCfg_default);
	SGPIO_configMuxReg(DATA_CFG0.sliceId, muxCfg_default);
	SGPIO_configOutMuxReg(DATA_CFG0.pinId, outMuxCfg_default);
	SGPIO_clearOeReg(DATA_CFG0.pinId);
	
	SGPIO_setBitCountReg(FRAME_CFG0.sliceId, 0x0);
	SGPIO_setBitCountReg(INTERNAL_FRAME_CFG0.sliceId, 0x0);	
	SGPIO_setBitCountReg(CLOCK_CFG0.sliceId, 0x0);
	SGPIO_setBitCountReg(DATA_CFG0.sliceId, 0x0);
	
	SGPIO_setCountReloadReg(FRAME_CFG0.sliceId, 0x0);
	SGPIO_setCountReloadReg(INTERNAL_FRAME_CFG0.sliceId, 0x0);	
	SGPIO_setCountReloadReg(CLOCK_CFG0.sliceId, 0x0);
	SGPIO_setCountReloadReg(DATA_CFG0.sliceId, 0x0);
	
	SGPIO_writeDataReg(FRAME_CFG0.sliceId, 0);
	SGPIO_writeDataReg(INTERNAL_FRAME_CFG0.sliceId, 0);	
	SGPIO_writeDataReg(CLOCK_CFG0.sliceId, 0);
	SGPIO_writeDataReg(DATA_CFG0.sliceId, 0);
	
	SGPIO_writeDataShadowReg(FRAME_CFG0.sliceId, 0);
	SGPIO_writeDataShadowReg(INTERNAL_FRAME_CFG0.sliceId, 0);	
	SGPIO_writeDataShadowReg(CLOCK_CFG0.sliceId, 0);
	SGPIO_writeDataShadowReg(DATA_CFG0.sliceId, 0);

	SGPIO_setCountReg(FRAME_CFG0.sliceId, 0);
	SGPIO_setCountReg(INTERNAL_FRAME_CFG0.sliceId, 0);	
	SGPIO_setCountReg(CLOCK_CFG0.sliceId, 0);
	SGPIO_setCountReg(DATA_CFG0.sliceId, 0);

	// clear capture interrupt status
	while(LPC_SGPIO->STATUS_1 & (1<<DATA_CFG0.sliceId))
		LPC_SGPIO->CTR_STATUS_1 = (1<<DATA_CFG0.sliceId);

}


emuStatus SGPIO_frameCheckErrors (void) {
    
	return emuError;
}


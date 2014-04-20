#include <stdint.h>
#include <stdbool.h>

#include "lpc43xx_scu.h"

#include "sgpio_spi.h"

/******************************************************************************
* USER SLICE ASSIGNMENT SECTION
******************************************************************************/ 
/******************************************************************************
* SLICE E is used to transmit the data for MASTER 0
******************************************************************************/
const SgpioSliceCfg DATA_OUT_CFG0 = {

	.sliceId = E,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_2

};

/******************************************************************************
* SLICE F is used to receive the data for MASTER 0
******************************************************************************/
const SgpioSliceCfg DATA_IN_CFG0 = {

	.sliceId = F,
	.sliceFunc = SGPIO_INPUT_PIN,	
	.pinId = SGPIO_6

};


/******************************************************************************
* SLICE J is used to transmit the data for master 1
******************************************************************************/
const SgpioSliceCfg DATA_OUT_CFG1 = {

	.sliceId = J,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_3

};

/******************************************************************************
* SLICE K is used to receive the data for master 1
******************************************************************************/
const SgpioSliceCfg DATA_IN_CFG1 = {

	.sliceId = K,
	.sliceFunc = SGPIO_INPUT_PIN,	
	.pinId = SGPIO_5

};


/******************************************************************************
* END OF USER SLICE ASSIGNMENT SECTION
* DO NOT MODIFY BELOW THIS LINE
******************************************************************************/ 



SpiParam const * spiMaster0Cfg;
SpiParam const * spiMaster1Cfg;

CsPattern cs0Waveform;
CsPattern cs1Waveform;
CsPattern cs0upWaveform;
CsPattern cs1upWaveform;

static volatile uint32_t Master0Mask;
static volatile uint32_t Master1Mask;



extern void SGPIO_makeMaster0Cpha0LowConfig(SpiParam const *config);
extern void SGPIO_makeMaster0Cpha0HighConfig(SpiParam const *config);
extern void SGPIO_makeMaster0Cpha1LowConfig(SpiParam const *config);
extern void SGPIO_makeMaster0Cpha1HighConfig(SpiParam const *config);
extern void SGPIO_makeMaster1Cpha0LowConfig(SpiParam const *config);
extern void SGPIO_makeMaster1Cpha0HighConfig(SpiParam const *config);
extern void SGPIO_makeMaster1Cpha1LowConfig(SpiParam const *config);
extern void SGPIO_makeMaster1Cpha1HighConfig(SpiParam const *config);


const CsPattern ChipSelectWaveform[NUM_CS_IDX] = { 	
	(1u << 6)  | 1u, 
	(1u << 10)  | 1u,
	(1u << 12) | 1u,
	(1u << 14) | 1u,
	(1u << 16) | 1u,
	(1u << 18) | 1u,
	(1u << 20) | 1u,
	(1u << 22) | 1u,
	(1u << 24) | 1u,
	(1u << 26) | 1u,
	(1u << 28) | 1u,
	(1u << 30) | 1u,
	1u,
	1u
};


/******************************************************************************
* SLICE A is used to generate a slave select signal 
* can be concatenated with slice I for 32 bit mode 
******************************************************************************/
const SgpioSliceCfg CHIP_SELECT_CFG0 = {

	.sliceId = A,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_0
};

/******************************************************************************
* SLICE I is concatenated to slice A for generating the slave select signal 
* used in 32 bit mode 
******************************************************************************/
const SgpioSliceCfg CHIP_SELECT_CFG0_INTERNAL = {	

	.sliceId = I,
	.sliceFunc = SGPIO_INTERNAL,	
	.pinId = SGPIO_1
	
};	

/******************************************************************************
* SLICE D is used to generate a clock signal 
* the configuration is the same for each SPI mode 
******************************************************************************/
const SgpioSliceCfg CLOCK_CFG0 = {

	.sliceId = D,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_12

};



/******************************************************************************
* SLICE P is used to generate a slave select signal 
* can be concatenated with slice H for 32 bit mode 
******************************************************************************/
const SgpioSliceCfg CHIP_SELECT_CFG1 = {

	.sliceId = P,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_15
};
	

/******************************************************************************
* SLICE H is concatenated to slice P for generating the slave select signal 
* used in 32 bit mode 
******************************************************************************/
const SgpioSliceCfg CHIP_SELECT_CFG1_INTERNAL = {	

	.sliceId = H,
	.sliceFunc = SGPIO_INTERNAL,	
	.pinId = SGPIO_14
	
};			



/******************************************************************************
* SLICE O is used to generate a clock signal 
* the configuration is the same for each SPI mode 
******************************************************************************/
const SgpioSliceCfg CLOCK_CFG1 = {

	.sliceId = O,
	.sliceFunc = SGPIO_OUTPUT_PIN,	
	.pinId = SGPIO_13

};


/* used to track configuration errors, shall be compiled out */
spiEmuStatus spiEmuError = SPIEMU_OK;


/**********************************************************************
 ** Function name: SgpioIrqInit
 **
 ** Description:		
 **
 ** Parameters:			
 **
 ** Returned value:		
 **********************************************************************/
static void SGPIO_spiEmuIrqInit(void) {

	NVIC_DisableIRQ(SGPIO_IINT_IRQn);
	
	LPC_SGPIO->CTR_STATUS_1 = (1<<CHIP_SELECT_CFG0.sliceId);		// clear capture interrupt status
	while(LPC_SGPIO->STATUS_1 & (1<<CHIP_SELECT_CFG0.sliceId));		// wait for status to clear	
	
	LPC_SGPIO->CTR_STATUS_1 = (1<<CHIP_SELECT_CFG1.sliceId);		// clear capture interrupt status
	while(LPC_SGPIO->STATUS_1 & (1<<CHIP_SELECT_CFG1.sliceId));		// wait for status to clear	

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
static bool spiMaster0Ready = true;
static bool spiMaster1Ready = true;

bool isSpiReady(spiNum_t spiId) {
	
	if(spiId == SPI_MASTER_0) return(spiMaster0Ready);
	
	if(spiId == SPI_MASTER_1) return(spiMaster1Ready);
	
	return(false);
}

uint32_t* volatile dataReadPointer0;
uint32_t* volatile dataReadPointer1;

void SGPIO_IRQHandler(void) 
{

	// for debug purpose
	LPC_GPIO_PORT->NOT[4] |= (1UL << 13);

	// sgpio capture interrrupt flag from chip select
	if((LPC_SGPIO->STATUS_1) & (1<<CHIP_SELECT_CFG0.sliceId))								
	{			
		
		SGPIO_disableSlices(Master0Mask);
		LPC_SGPIO->CTR_STATUS_1 = (1<<CHIP_SELECT_CFG0.sliceId);					
		
		SGPIO_writeDataReg(CHIP_SELECT_CFG0.sliceId, cs0Waveform);

		// for the 30+ bit case, could eventually be conditional
		SGPIO_writeDataReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, cs0upWaveform);
		
		SGPIO_readDataReg(DATA_IN_CFG0.sliceId, spiMaster0Cfg->wordLenght, dataReadPointer0);
		SGPIO_writeDataReg(DATA_IN_CFG0.sliceId, 0x0);
		
		SGPIO_setCountReg(CHIP_SELECT_CFG0.sliceId, 0x0);
		SGPIO_setCountReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, 0x0);
		SGPIO_setCountReg(CLOCK_CFG0.sliceId, 0x0);		
		SGPIO_setCountReg(DATA_OUT_CFG0.sliceId, 0);
		SGPIO_setCountReg(DATA_IN_CFG0.sliceId, 0);
	
		spiMaster0Ready = 1;
	}

	// sgpio capture interrrupt flag from chip select
	if((LPC_SGPIO->STATUS_1) & (1<<CHIP_SELECT_CFG1.sliceId))								
	{			
		
		SGPIO_disableSlices(Master1Mask);
		LPC_SGPIO->CTR_STATUS_1 = (1<<CHIP_SELECT_CFG1.sliceId);					
		
		SGPIO_writeDataReg(CHIP_SELECT_CFG1.sliceId, cs1Waveform);

		// for the 30+ bit case, could eventually be conditional
		SGPIO_writeDataReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, cs1upWaveform);
		
		SGPIO_readDataReg(DATA_IN_CFG1.sliceId, spiMaster1Cfg->wordLenght, dataReadPointer1);
		SGPIO_writeDataReg(DATA_IN_CFG1.sliceId, 0x0);
		
		SGPIO_setCountReg(CHIP_SELECT_CFG1.sliceId, 0x0);
		SGPIO_setCountReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, 0x0);
		SGPIO_setCountReg(CLOCK_CFG1.sliceId, 0x0);
		SGPIO_setCountReg(DATA_OUT_CFG1.sliceId, 0x0);
		SGPIO_setCountReg(DATA_IN_CFG1.sliceId, 0x0);	
		
		spiMaster1Ready = 1;
	}
	
	// just for debug
	LPC_GPIO_PORT->NOT[4] |= (1UL << 13);
}

spiEmuStatus SGPIO_spiIoctl(const SGPIO_Slice sliceId, const sliceIoctl cmd, const void* param) {
	
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

		case RD_DATA:
			
			*((uint32_t*)param) = LPC_SGPIO->REG[sliceId] >> (32 - (spiMaster0Cfg->wordLenght));
		break;
		
		case WR_DATASS:
			
			LPC_SGPIO->REG_SS[sliceId] = (uint32_t) param;
		
		break;
		
		case RD_DATASS:
			
			*((uint32_t*)param) = LPC_SGPIO->REG_SS[sliceId] >> (32 - (spiMaster0Cfg->wordLenght));
		break;
		
	};
	
	return(SPIEMU_OK);
}


void SGPIO_spiWrite(spiNum_t spiId, uint32_t dataWrite, uint32_t* dataRead) {

	if(spiId == SPI_MASTER_0) {
		
		// LPC_SGPIO->REG[DATA_OUT_CFG0.sliceId] = dataWrite; 
		SGPIO_writeDataReg(DATA_OUT_CFG0.sliceId, dataWrite);	
		
		dataReadPointer0 = dataRead;
	
		SGPIO_enableSlices(STOP,Master0Mask);
		spiMaster0Ready = 0;
	};
	
	if(spiId == SPI_MASTER_1) {
		
		// LPC_SGPIO->REG[DATA_OUT_CFG1.sliceId] = dataWrite; 
		SGPIO_writeDataReg(DATA_OUT_CFG1.sliceId, dataWrite);	
		
		dataReadPointer1 = dataRead;
	
		SGPIO_enableSlices(STOP,Master1Mask);
		spiMaster1Ready = 0;
	};
}


void SGPIO_spiInit(void) {
	
	if(LPC_RGU->RESET_ACTIVE_STATUS1 & (1u << 24)) {
		// M0 not in reset, write back a '0'
		LPC_RGU->RESET_CTRL1 =  (1 << 25); // reset the SGPIO block
	}
	else {
		// M0 in reset, write back a '1'
		LPC_RGU->RESET_CTRL1 = (1u << 24) | (1 << 25); // reset the SGPIO block
	};
	
	spiMaster0Ready = 1;
	spiMaster1Ready = 1;	
	
	spiMaster0Cfg = 0;
	spiMaster1Cfg = 0;

	cs0Waveform = 0;
	cs1Waveform = 0;
	cs0upWaveform = 0;
	cs1upWaveform = 0;

	Master0Mask = 0;
	Master1Mask = 0;
	SGPIO_spiEmuIrqInit();
}



void SGPIO_spiOpen(const spiNum_t spiId,  SpiParam const *config) {
	
	if(spiId == SPI_MASTER_1) {
		
		spiMaster1Cfg = config;
		
		/* Apply SGPIO configuration for Keil MCB1800/4300 board */				
		/* OUTPUTS */
		
		// CHIP SELECT 1, P4.10 : SGPIO15 - LCDVD10
		scu_pinmux(0x4 , 10 , PUP_ENABLE, FUNC7);	
		
		// MOSI1 - DATA TX, PF.9 : SGPIO3 
		scu_pinmux(0xF ,9 , PUP_ENABLE , FUNC6);	

		// CLOCK1, PC.14 : SGPIO13 - Joystick right 
		// has already a pull up
		scu_pinmux(0xC , 14 , PLAIN_ENABLE, FUNC0);	
		
		/* INPUTS */
		
		// MISO1 - DATA RX, PF.6 : SGPIO5 - TRACEDATA1
		scu_pinmux(0xF ,6 , PLAIN_ENABLE | INBUF_ENABLE, FUNC6);		
		
		Master1Mask = (	(1U << CHIP_SELECT_CFG1.sliceId) |\
						(1U << CHIP_SELECT_CFG1_INTERNAL.sliceId)|\
						(1U << CLOCK_CFG1.sliceId)|\
						(1U << DATA_OUT_CFG1.sliceId)|\
						(1U << DATA_IN_CFG1.sliceId));
		
		if(config->clockPhase == CPHA_0) {
			
			if(config->clockPolarity == IDLE_LOW) {
				
				SGPIO_makeMaster1Cpha0LowConfig(config);
			} 
			else if (config->clockPolarity == IDLE_HIGH) {
			
				SGPIO_makeMaster1Cpha0HighConfig(config);
			}
			
		} 
		else if (config->clockPhase == CPHA_1) {
			
			if(config->clockPolarity == IDLE_LOW) {
				
				SGPIO_makeMaster1Cpha1LowConfig(config);
			} 
			else if (config->clockPolarity == IDLE_HIGH) {
			
				SGPIO_makeMaster1Cpha1HighConfig(config);		
							
			}		
		};		
		
		// enable capture interrupt for slice CHIP SELECT
		LPC_SGPIO->SET_EN_1 = (1<<CHIP_SELECT_CFG1.sliceId);		

		return;

	}
	
	if(spiId == SPI_MASTER_0) {

		spiMaster0Cfg = config;
		
		/* Apply SGPIO configuration for Keil MCB1800/4300 board */				
		/* OUTPUTS */
		
		// CHIP SELECT 0, P9.0 : SGPIO0 - LED
		scu_pinmux(0x9 ,0 , PUP_ENABLE, FUNC6);	
		
		// MOSI 0 - DATA TX, P9.2 : SGPIO2 - LED
		scu_pinmux(0x9 ,2 , PUP_ENABLE , FUNC6);	

		// CLOCK 0, P2.3: SGPIO12 - <Uart3 Tx			
		scu_pinmux(0x2 ,3 , PUP_ENABLE | INBUF_ENABLE, FUNC0);	
 
		
		/* INPUTS */

		// MISO - DATA RX, P7.2 : SGPIO6 - 
		scu_pinmux(0x7 ,2 , PLAIN_ENABLE | INBUF_ENABLE, FUNC7);		
		

		Master0Mask = (	(1U << CHIP_SELECT_CFG0.sliceId) |\
						(1U << CHIP_SELECT_CFG0_INTERNAL.sliceId)|\
						(1U << CLOCK_CFG0.sliceId)|\
						(1U << DATA_OUT_CFG0.sliceId)|\
						(1U << DATA_IN_CFG0.sliceId));

		if(config->clockPhase == CPHA_0) {
			
			if(config->clockPolarity == IDLE_LOW) {
				
				SGPIO_makeMaster0Cpha0LowConfig(config);
			} 
			else if (config->clockPolarity == IDLE_HIGH) {
			
				SGPIO_makeMaster0Cpha0HighConfig(config);
			}
			
		} 
		else if (config->clockPhase == CPHA_1) {
			
		if(config->clockPolarity == IDLE_LOW) {
				
				SGPIO_makeMaster0Cpha1LowConfig(config);
			} 
			else if (config->clockPolarity == IDLE_HIGH) {
			
				SGPIO_makeMaster0Cpha1HighConfig(config);		
							
			}		
		};		
		
		// enable capture interrupt for slice CHIP SELECT
		LPC_SGPIO->SET_EN_1 = (1<<CHIP_SELECT_CFG0.sliceId);		

		return;
	}
	
	spiEmuError = SPIEMU_INVALID_ID;
}

	
void SGPIO_spiClose(spiNum_t spiId) {

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

	if(spiId == SPI_MASTER_0) {

		SGPIO_disableSlices(Master0Mask);	
			
		// revert the static configuration
		SGPIO_configSliceMuxReg(CHIP_SELECT_CFG0.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(CHIP_SELECT_CFG0.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(CHIP_SELECT_CFG0.pinId, outMuxCfg_default);
		SGPIO_clearOeReg(CHIP_SELECT_CFG0.pinId);
		
		SGPIO_configSliceMuxReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(CHIP_SELECT_CFG0_INTERNAL.pinId, outMuxCfg_default);
		
		SGPIO_configSliceMuxReg(CLOCK_CFG0.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(CLOCK_CFG0.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(CLOCK_CFG0.pinId, outMuxCfg_default);
		SGPIO_clearOeReg(CLOCK_CFG0.pinId);
		
		SGPIO_configSliceMuxReg(DATA_OUT_CFG0.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(DATA_OUT_CFG0.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(DATA_OUT_CFG0.pinId, outMuxCfg_default);
		SGPIO_clearOeReg(DATA_OUT_CFG0.pinId);
		
		SGPIO_setBitCountReg(CHIP_SELECT_CFG0.sliceId, 0x0);
		SGPIO_setBitCountReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, 0x0);
		SGPIO_setBitCountReg(CLOCK_CFG0.sliceId, 0x0);
		SGPIO_setBitCountReg(DATA_OUT_CFG0.sliceId, 0x0);
		SGPIO_setBitCountReg(DATA_IN_CFG0.sliceId, 0x0);
		
		SGPIO_setCountReloadReg(CHIP_SELECT_CFG0.sliceId, 0x0);
		SGPIO_setCountReloadReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, 0x0);
		SGPIO_setCountReloadReg(CLOCK_CFG0.sliceId, 0x0);
		SGPIO_setCountReloadReg(DATA_OUT_CFG0.sliceId, 0x0);
		SGPIO_setCountReloadReg(DATA_IN_CFG0.sliceId, 0x0);
		
		SGPIO_writeDataReg(CHIP_SELECT_CFG0.sliceId, 0);
		SGPIO_writeDataReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, 0);
		SGPIO_writeDataReg(CLOCK_CFG0.sliceId, 0);
		SGPIO_writeDataReg(DATA_OUT_CFG0.sliceId, 0);
		SGPIO_writeDataReg(DATA_IN_CFG0.sliceId, 0);
		
		SGPIO_writeDataShadowReg(CHIP_SELECT_CFG0.sliceId, 0);
		SGPIO_writeDataShadowReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, 0);
		SGPIO_writeDataShadowReg(CLOCK_CFG0.sliceId, 0);
		SGPIO_writeDataShadowReg(DATA_OUT_CFG0.sliceId, 0);
		SGPIO_writeDataShadowReg(DATA_IN_CFG0.sliceId, 0);

		SGPIO_setCountReg(CHIP_SELECT_CFG0.sliceId, 0);
		SGPIO_setCountReg(CHIP_SELECT_CFG0_INTERNAL.sliceId, 0);
		SGPIO_setCountReg(CLOCK_CFG0.sliceId, 0);
		SGPIO_setCountReg(DATA_OUT_CFG0.sliceId, 0);
		SGPIO_setCountReg(DATA_IN_CFG0.sliceId, 0);

		// clear capture interrupt status
		while(LPC_SGPIO->STATUS_1 & (1<<CHIP_SELECT_CFG0.sliceId))
			LPC_SGPIO->CTR_STATUS_1 = (1<<CHIP_SELECT_CFG0.sliceId);
	};
	
	if(spiId == SPI_MASTER_1) {

		SGPIO_disableSlices(Master1Mask);

		// revert the static configuration
		SGPIO_configSliceMuxReg(CHIP_SELECT_CFG1.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(CHIP_SELECT_CFG1.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(CHIP_SELECT_CFG1.pinId, outMuxCfg_default);
		SGPIO_clearOeReg(CHIP_SELECT_CFG1.pinId);
		
		SGPIO_configSliceMuxReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(CHIP_SELECT_CFG1_INTERNAL.pinId, outMuxCfg_default);
		
		SGPIO_configSliceMuxReg(CLOCK_CFG1.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(CLOCK_CFG1.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(CLOCK_CFG1.pinId, outMuxCfg_default);
		SGPIO_clearOeReg(CLOCK_CFG1.pinId);
		
		SGPIO_configSliceMuxReg(DATA_OUT_CFG1.sliceId, sliceMuxCfg_default);
		SGPIO_configMuxReg(DATA_OUT_CFG1.sliceId, muxCfg_default);
		SGPIO_configOutMuxReg(DATA_OUT_CFG1.pinId, outMuxCfg_default);
		SGPIO_clearOeReg(DATA_OUT_CFG1.pinId);
		
		SGPIO_setBitCountReg(CHIP_SELECT_CFG1.sliceId, 0x0);
		SGPIO_setBitCountReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, 0x0);
		SGPIO_setBitCountReg(CLOCK_CFG1.sliceId, 0x0);
		SGPIO_setBitCountReg(DATA_OUT_CFG1.sliceId, 0x0);
		SGPIO_setBitCountReg(DATA_IN_CFG1.sliceId, 0x0);

		
		SGPIO_setCountReloadReg(CHIP_SELECT_CFG1.sliceId, 0x0);
		SGPIO_setCountReloadReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, 0x0);
		SGPIO_setCountReloadReg(CLOCK_CFG1.sliceId, 0x0);
		SGPIO_setCountReloadReg(DATA_OUT_CFG1.sliceId, 0x0);
		SGPIO_setCountReloadReg(DATA_IN_CFG1.sliceId, 0x0);
		
		SGPIO_writeDataReg(CHIP_SELECT_CFG1.sliceId, 0);
		SGPIO_writeDataReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, 0);
		SGPIO_writeDataReg(CLOCK_CFG1.sliceId, 0);
		SGPIO_writeDataReg(DATA_OUT_CFG1.sliceId, 0);
		SGPIO_writeDataReg(DATA_IN_CFG1.sliceId, 0);

		
		SGPIO_writeDataShadowReg(CHIP_SELECT_CFG1.sliceId, 0);
		SGPIO_writeDataShadowReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, 0);
		SGPIO_writeDataShadowReg(CLOCK_CFG1.sliceId, 0);
		SGPIO_writeDataShadowReg(DATA_OUT_CFG1.sliceId, 0);
		SGPIO_writeDataShadowReg(DATA_IN_CFG1.sliceId, 0);

		SGPIO_setCountReg(CHIP_SELECT_CFG1.sliceId, 0);
		SGPIO_setCountReg(CHIP_SELECT_CFG1_INTERNAL.sliceId, 0);
		SGPIO_setCountReg(CLOCK_CFG1.sliceId, 0);
		SGPIO_setCountReg(DATA_OUT_CFG1.sliceId, 0);
		SGPIO_setCountReg(DATA_IN_CFG1.sliceId, 0);

		// clear capture interrupt status
		while(LPC_SGPIO->STATUS_1 & (1<<CHIP_SELECT_CFG1.sliceId))			
			LPC_SGPIO->CTR_STATUS_1 = (1<<CHIP_SELECT_CFG1.sliceId);
	};

}


spiEmuStatus SGPIO_spiEmuCheckErrors (void) {
    
	return spiEmuError;
}


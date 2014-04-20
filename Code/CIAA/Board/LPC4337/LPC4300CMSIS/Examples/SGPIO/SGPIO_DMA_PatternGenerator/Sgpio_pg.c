
//#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
//#include "lpc43xx_scu.h"
//#include "lpc43xx_libcfg.h"
//#include "debug_frmwrk.h"
#include "LPC43xx.h"
#include "Sgpio_pg.h"
#include "lpc43xx_gpdma.h"
//#include "core_cmInstr.h"



void start_m0()
{	
	SCB->SCR |= 0x2;						// sleep-on-exit mode
	setupSGPIO();							// setup SGPIO slices and pinmux	
	/* Initialize GPDMA controller */
	GPDMA_Init();
	/* Setting GPDMA & SGPIO interrupt */
	NVIC_DisableIRQ(DMA_IRQn);				// Disable interrupt for DMA	
	NVIC_EnableIRQ(M0_SGPIO_IRQn);		// Enable SGPIO interrupt
	Setup_DMAChl();
	while(LPC_GPDMA->ENBLDCHNS != 0);		// wait until channels are all disabled
	while(1); 
}


int x = 0;

uint32_t *p = (uint32_t*) 0x40002120, *q = (uint32_t*) 0x40002160; // p: channel 1, q: channel 3
void SGPIO_IRQHandler()
{
	uint32_t *swap;
	q[4] = 1|(1<<14)|(1<<15);
	LPC_SGPIO->CTR_STATUS_1 = 0xFFFF;

	x &= 0x3;
	p[0] = ((uint32_t)&(DMA_MEM->REG_SSdata[0])+ x*64);		// CSRCADDR
	p[1] = (uint32_t) & (LPC_SGPIO->REG_SS[0]);				// CDESTADDR
	p[3] = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);		// CCONTROL
	swap = q;
	q = p;
	p = swap;	 
	x++;		
}


void Setup_DMAChl()
{
	uint32_t SrcAddr, DesAddr;

	LPC_GPDMA->INTTCCLEAR = 0xFF;
	LPC_GPDMA->INTERRCLR = 0xFF;
	LPC_GPDMA->CONFIG = GPDMA_DMACConfig_E;				// enable DMA controller
	while (!(LPC_GPDMA->CONFIG & GPDMA_DMACConfig_E));

	SrcAddr = (uint32_t) & (DMA_MEM->REG_SSdata[0]);
	DesAddr = (uint32_t) & (LPC_SGPIO->REG_SS[0]);
	// Setup GPDMA channel 1 : REG_SS (1) --------------------------------
	LPC_GPDMA->C1SRCADDR = SrcAddr;
	LPC_GPDMA->C1DESTADDR =	DesAddr;
	LPC_GPDMA->C1LLI = 0;
	LPC_GPDMA->C1CONTROL = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);


	SrcAddr = (uint32_t) & (DMA_MEM->EN_Slices);
	DesAddr = (uint32_t) & (LPC_SGPIO->CTRL_ENABLED);
	// Setup GPDMA channel 2 : Enable SLices--------------------------
	LPC_GPDMA->C2SRCADDR = SrcAddr;
	LPC_GPDMA->C2DESTADDR =	DesAddr;
	LPC_GPDMA->C2LLI = 0;
	LPC_GPDMA->C2CONTROL = (0x1)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);


	SrcAddr = (uint32_t) & (DMA_MEM->REG_SSdata[0]);
	DesAddr = (uint32_t) & (LPC_SGPIO->REG_SS[0]);
	// Setup GPDMA channel 3 : REG_SS (2) ----------------------------------
	LPC_GPDMA->C3SRCADDR = SrcAddr;
	LPC_GPDMA->C3DESTADDR =	DesAddr;
	LPC_GPDMA->C3LLI = 0;
	LPC_GPDMA->C3CONTROL = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);


	// Enable GPDMA channel 1 
	GPDMA_ChannelCmd(1, ENABLE);
	// Enable GPDMA channel 2 
	GPDMA_ChannelCmd(2, ENABLE);
}



/*-------------------------SETUP SGPIO--------------------------------*/
void setupSGPIO(void)
{
#define CONCAT_ENABLE (1<<11)						// concatenated enable and self loop
#define EXTERNAL_DATA_PIN 0
#define preset 2			      

	LPC_SGPIO->CLR_EN_0 = 0xFFFF;					// clear shift clock interrupt
	LPC_SGPIO->CTRL_ENABLED = 0;					// disable all counters in this state
	LPC_SGPIO->GPIO_OENREG = 0xFFFF;				// output enable all slices
	LPC_SGPIO->SET_EN_1 = 0xFFFF;


    /* set frequency of shift clocks */	 
	LPC_SGPIO->PRESET[0] = preset;
	LPC_SGPIO->PRESET[1] = preset;
	LPC_SGPIO->PRESET[2] = preset;
	LPC_SGPIO->PRESET[3] = preset;
	LPC_SGPIO->PRESET[4] = preset;
	LPC_SGPIO->PRESET[5] = preset;
	LPC_SGPIO->PRESET[6] = preset;
	LPC_SGPIO->PRESET[7] = preset;
	LPC_SGPIO->PRESET[8] = preset;
	LPC_SGPIO->PRESET[9] = preset;
	LPC_SGPIO->PRESET[10] = preset;
	LPC_SGPIO->PRESET[11] = preset;
	LPC_SGPIO->PRESET[12] = preset;					
	LPC_SGPIO->PRESET[13] = preset;				
	LPC_SGPIO->PRESET[14] = preset;
	LPC_SGPIO->PRESET[15] = preset;	


	/* set phase of shift clocks*/
	LPC_SGPIO->COUNT[0] = 0;
	LPC_SGPIO->COUNT[1] = 0;
	LPC_SGPIO->COUNT[2] = 0;
	LPC_SGPIO->COUNT[3] = 0;
	LPC_SGPIO->COUNT[4] = 0;
	LPC_SGPIO->COUNT[5] = 0;
	LPC_SGPIO->COUNT[6] = 0;
	LPC_SGPIO->COUNT[7] = 0;
	LPC_SGPIO->COUNT[8] = 0;
	LPC_SGPIO->COUNT[9] = 0;
	LPC_SGPIO->COUNT[10] = 0;
	LPC_SGPIO->COUNT[11] = 0;
	LPC_SGPIO->COUNT[12] = 0;
	LPC_SGPIO->COUNT[13] = 0;
	LPC_SGPIO->COUNT[14] = 0;
	LPC_SGPIO->COUNT[15] = 0;

	/**********************************/
	/*  Program SGPIO 0 as Channel 1  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[0] = 0x0;				/* output is sliceA(0), output enable is set by GPIO_OEREG */ 
	LPC_SGPIO->SGPIO_MUX_CFG[0] = CONCAT_ENABLE;
	LPC_SGPIO->POS[0] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 1 as Channel 2  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[1] = 0x0;				// output is sliceI(8), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[8] = CONCAT_ENABLE;
	LPC_SGPIO->POS[8] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 2 as Channel 3  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[2] = 0x0;				/* output is sliceE(4), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[4] = CONCAT_ENABLE;
	LPC_SGPIO->POS[4] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 3 as Channel 4  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[3] = 0x0;				// output is sliceJ(9), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[9] = CONCAT_ENABLE;
	LPC_SGPIO->POS[9] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 4 as Channel 5  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[4] = 0x0;				/* output is sliceC(2), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[2] = CONCAT_ENABLE;
	LPC_SGPIO->POS[2] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 5 as Channel 6  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[5] = 0x0;				// output is sliceK(10), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[10] = CONCAT_ENABLE;
	LPC_SGPIO->POS[10] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 6 as Channel 7  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[6] = 0x0;				/* output is sliceF(5), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[5] = CONCAT_ENABLE;
	LPC_SGPIO->POS[5] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 7 as Channel 8  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[7] = 0x0;				// output is sliceL(11), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[11] = CONCAT_ENABLE;
	LPC_SGPIO->POS[11] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 8 as Channel 9  */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[8] = 0x0;				/* output is sliceB(1), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[1] = CONCAT_ENABLE;
	LPC_SGPIO->POS[1] = 0x1F1F;

	/**********************************/
	/*  Program SGPIO 9 as Channel 10 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[9] = 0x0;				// output is sliceM(12), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[12] = CONCAT_ENABLE;
	LPC_SGPIO->POS[12] = 0x1F1F;

	/**********************************/
	/* Program SGPIO 10 as Channel 11 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[10] = 0x0;				/* output is sliceG(6), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[6] = CONCAT_ENABLE;
	LPC_SGPIO->POS[6] = 0x1F1F;

	/**********************************/
	/* Program SGPIO 11 as Channel 12 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[11] = 0x0;				// output is sliceN(13), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[13] = CONCAT_ENABLE;
	LPC_SGPIO->POS[13] = 0x1F1F;

	/**********************************/
	/* Program SGPIO 12 as Channel 13 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[12] = 0x0;				/* output is sliceD(3), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[3] = CONCAT_ENABLE;
	LPC_SGPIO->POS[3] = 0x1F1F;

	/**********************************/
	/* Program SGPIO 13 as Channel 14 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[13] = 0x0;				// output is sliceO(14), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[14] = CONCAT_ENABLE;
	LPC_SGPIO->POS[14] = 0x1F1F;

	/**********************************/
	/* Program SGPIO 14 as Channel 15 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[14] = 0x0;				/* output is sliceH(7), output enable is set by GPIO_OEREG */
	LPC_SGPIO->SGPIO_MUX_CFG[7] = CONCAT_ENABLE;
	LPC_SGPIO->POS[7] = 0x1F1F;

	/**********************************/
	/* Program SGPIO 15 as Channel 16 */
	/**********************************/
	LPC_SGPIO->OUT_MUX_CFG[15] = 0x0;				// output is sliceP(15), output enable is set by GPIO_OEREG 
	LPC_SGPIO->SGPIO_MUX_CFG[15] = CONCAT_ENABLE;
	LPC_SGPIO->POS[15] = 0x1F1F;



///////////setup sgpio pins//////////////////////////////////////////////
	LPC_SCU->SFSP0_0 = 0xF3;		// A2A4: function 3, plain, ehs on, ezi active, zi selected, SGPIO0
//	LPC_SCU->SFSP9_1 = 0xF6;		// A2: function 6, plain, ehs on, ezi active, zi selected, SGPIO1
//	LPC_SCU->SFSP1_15 = 0xF2;		// A2: function 2, plain, ehs on, ezi active, zi selected, SGPIO2
//	LPC_SCU->SFSP1_16 = 0xF2;		// A2: function 2, plain, ehs on, ezi active, zi selected, SGPIO3
//	LPC_SCU->SFSP2_0 = 0xF0;		// A2: function 0, plain, ehs on, ezi active, zi selected, SGPIO4
//	LPC_SCU->SFSP2_1 = 0xF0;		// A2: function 0, plain, ehs on, ezi active, zi selected, SGPIO5
//	LPC_SCU->SFSP7_2 = 0xF7;		// A2: function 7, plain, ehs on, ezi active, zi selected, SGPIO6
//	LPC_SCU->SFSP7_7 = 0xF7;		// A2: function 7, plain, ehs on, ezi active, zi selected, SGPIO7
//	LPC_SCU->SFSP9_6 = 0xF6;		// A2: function 6, plain, ehs on, ezi active, zi selected, SGPIO8
//	LPC_SCU->SFSP9_3 = 0xF6;		// A2: function 6, plain, ehs on, ezi active, zi selected, SGPIO9
//	LPC_SCU->SFSP1_3 = 0xF2;		// A2: function 2, plain, ehs on, ezi active, zi selected, SGPIO10
//	LPC_SCU->SFSP4_5 = 0xF7;		// A2: function 7, plain, ehs on, ezi active, zi selected, SGPIO11
//	LPC_SCU->SFSP1_18 = 0xF6;		// A2: function 6, plain, ehs on, ezi active, zi selected, SGPIO12
//	LPC_SCU->SFSP1_20 = 0xF6; 		// A2: function 6, plain, ehs on, ezi active, zi selected, SGPIO13
//    LPC_SCU->SFSP4_9 = 0xF7; 		// A2: function 7, plain, ehs on, ezi active, zi selected, SGPIO14
	LPC_SCU->SFSP4_10 = 0xD7; 		// A4: function 7, plain, ehs off, ezi active, zi selected, SGPIO15 // A2: pin2_8,function0
	                                                                                                                                                                                      
}

#include "LPC43xx.h"
#include "sgpio.h"

/**********************************************************************
 ** Function prototypes
 **********************************************************************/



/**********************************************************************
 ** Function name: SgpioIrqInit
 **
 ** Description:		
 **
 ** Parameters:			
 **
 ** Returned value:		
 **********************************************************************/
// void SGPIO_irqInit(void)
// {
// 	LPC_SGPIO->CTR_STAT1 = (1<<0);		// clear capture interrupt status
// 	while(LPC_SGPIO->STATUS1 & (1<<0));	// wait for status to clear	
// 	LPC_SGPIO->SET_EN1 = (1<<0);		// enable capture interrupt for slice A (CHIP SELECT)

// 	// Enable SGPIO interrupt (shift/capture/match/input are or'ed)
// 	NVIC_EnableIRQ(SGPIO_IRQn);
// }


/*----------------------------------------------------------------------------
  Initialize specific IO
 *----------------------------------------------------------------------------*/
// void setSgpio(void) {

// 	/* MCB board */
// 	// CHIP SELECT
// 	scu_pinmux(0x9 ,0 , PUP_ENABLE, FUNC6);	// P9.0 : SGPIO0 - LED

// 	
// 	scu_pinmux(0x9 ,1 , PUP_ENABLE | INBUF_ENABLE, FUNC6);	// P9.1 : SGPIO1 - LED

// 	// MOSI - DATA TX
// 	scu_pinmux(0x9 ,2 , PUP_ENABLE , FUNC6);	// P9.2 : SGPIO2 - LED

// 	// CLOCK
//  	scu_pinmux(0x2 ,3 , PUP_ENABLE | INBUF_ENABLE, FUNC0);	// P2.3: SGPIO12 - <Uart3 Tx			
//  
// 	// FEEDBACK CLOCK to SGPIO 9
//  scu_pinmux(0x9 ,3 , PUP_ENABLE | INBUF_ENABLE, FUNC6);		// P9.3 : SGPIO9 - USB1_IND1 LED

// // qualifier clock to SGPIO8 for supporting mode 1
//  scu_pinmux(0x9 ,6 , PUP_ENABLE | INBUF_ENABLE, FUNC6);		// P9.6 : SGPIO8 - USB1_PWR_FLT
// 	
// P1_4 SGPIO11 - Connector
// P8_1 SGPIO9 - USB0_IND1
// P8_2 SGPIO10 - USB0_IND0	
// P9_6 SGPIO8 - USB1_PWR_FLT

	// P0_0 SGPIO0 - ETH RXD1
	// P0_1 SGPIO1 - ETH TX EN
	// P1_0 SGPIO7 - A5
	// P1_1 SGPIO8 - A6 
	// P1_2 SGPIO9 - A7
	// P1_3 SGPIO10 - OE	
	// P1_4 SGPIO11 - Connector
	// P1_5 SGPIO15 - CS0
	// P1_6 SGPIO14 - WE
	// P1_12 SGPIO8 - D5
	// P1_13 SGPIO9 - D6
	// P1_14 SGPIO10 - D7
 // P1_16 SGPIO3 - ETH RX_DV
// P1_17 SGPIO11 - ETH MDIO
// P1_18 SGPIO12 - ETH TXD0
// P1_20 SGPIO13 - ETH TDX1
// P2_0 SGPIO4 - A13, Uart 0 TX
// P2_1 SGPIO5 - A12, Uart0 rx
// P2_2 SGPIO6 - A11
// P2_4 SGPIO13 - UART3 rx
// P2_5 SGPIO14 - usb1 VBUS
// P2_6 SGPIO7 - A10
// P2_8 SGPIO15 - A8
// P4_2 SGPIO8 - LCDVD3
// P4_3 SGPIO9 - PUSHBUTTON 
// P4_4 SGPIO10 - PUSHBUTTON
// P4_5 SGPIO11 - LCDVSYNC
// P4_6 SGPIO12 - LCDEN
// P4_9 SGPIO14 - LCDVD11
// P4_10 SGPIO15 - LCDVD10
// P6_6 SGPIO5 - USB0_PWR_FLT
// P6_7 SGPIO6 - A15
// P6_8 SGPIO7 - A14
// P7_0 SGPIO 4 - LCD_BL_EN
// P7_1 SGPIO 6 - AUDWSI
// P7_2 SGPIO6 - AUDDATA1
// P7_7 SGPIO7 - 
// P8_0 SGPIO8 - UDA Reset
// P8_1 SGPIO9 - USB0_IND1
// P8_2 SGPIO10 - USB0_IND0
// 

// P9_6 SGPIO8 - USB1_PWR_FLT

// // 	scu_pinmux(0x9 ,4 , PUP_ENABLE, FUNC6);		// P9.4  : SGPIO6  - USB1_IND0 LED	
// // 	

// 	
// // 	scu_pinmux(0xD ,12 , PUP_ENABLE, FUNC5);	// PD.12 : SGPIO11 - LED	
// // 	scu_pinmux(0xD ,13 , PUP_ENABLE, FUNC5);	// PD.13 : SGPIO12 - LED	
// // 	scu_pinmux(0xD ,14 , PUP_ENABLE, FUNC5);	// PD.14 : SGPIO13 - LED	
// 	
// /*
// 	scu_pinmux(0x9 ,5 , PUP_ENABLE, FUNC6);	// P9.5 : SGPIO3 - USB1_VBUS_EN SV5-4	OK

// 	scu_pinmux(0x7 ,0 , PUP_ENABLE, FUNC7);	// P7.0 : SGPIO4 - LCDLE (SV19 - 1)		OK
// 	// scu_pinmux(0x9 ,4 , PUP_ENABLE, FUNC6);	// P9.4 : SGPIO4  - Not routed on Hitex board

// 	scu_pinmux(0x7 ,1 , PUP_ENABLE, FUNC7);	// P7.1 : SGPIO5 - LCDVD19 X11-22  OK

// 	scu_pinmux(0x7 ,2 , PUP_ENABLE, FUNC7);	// P7.2 : SGPIO6 - LCDVD18 X11 - 21 OK

// 	scu_pinmux(0x7 ,7 , PUP_ENABLE, FUNC7);	// P7.7 : SGPIO7 - LCDPWR - X11 - 34 

// 	// scu_pinmux(0x9 ,6 , PUP_ENABLE, FUNC6);	// P9.6 : SGPIO8 - USB1_PWR_FAULT (SV5 - 6)	 OK
// 	scu_pinmux(0x4 ,2 , PUP_ENABLE, FUNC7);	// P4.2 : SGPIO8 - LCDVD3 X11-7	  OK
// 	


// 	scu_pinmux(0x4 ,3 , PUP_ENABLE, FUNC7);	// P4.3 : SGPIO9  - LCDVD2 X11-6  OK

// 	scu_pinmux(0x4 ,4 , PUP_ENABLE, FUNC7);	// P4.4 : SGPIO10 - LCDVEEN X11-36, JP22.2  OK

// 	scu_pinmux(0x4 ,5 , PUP_ENABLE, FUNC7);	// P4.5 : SGPIO11 - LCDFP X11-28  OK
// 	
// 	scu_pinmux(0x4 ,6 , PUP_ENABLE, FUNC7);	// P4.6 : SGPIO12 LCDAC X11-35  OK
// 	
// 	scu_pinmux(0x2 ,4 , PUP_ENABLE, FUNC0);	// P2.4 : SGPIO13 - U3_TXD X3.14
// 		
// 	scu_pinmux(0x4 ,9 , PUP_ENABLE, FUNC7);	// P4.9 : SGPIO14 - LCDVD11 - X11 -14

// 	scu_pinmux(0x4 ,10 , PUP_ENABLE, FUNC7);	// P4.9 : SGPIO15 - LCDVD10 - X11 -13

// */


// 	// LPC_SCU->SFSCLK_0 = 0x01;
// }





void SGPIO_configMuxReg (const SGPIO_Slice sliceId, const SGPIO_MuxConfig config) {
	
	LPC_SGPIO->SGPIO_MUX_CFG[sliceId] = config;	

}


void SGPIO_configSliceMuxReg (const SGPIO_Slice sliceId, const SGPIO_MuxConfig config) {
	
	LPC_SGPIO->SLICE_MUX_CFG[sliceId] = config;	

}

void SGPIO_configOutMuxReg (const SGPIO_Pin pinId, const SGPIO_MuxConfig config) {
	
	LPC_SGPIO->OUT_MUX_CFG[pinId] = config;	

}

#define GPIO_OE_MASK (0x70)
void SGPIO_setOeReg (const SGPIO_Pin pinId, const SGPIO_OutMuxConfig config) {
	
	if((config & GPIO_OE_MASK) == OMC_GPIO_OE) LPC_SGPIO->GPIO_OENREG |= (0x1 << pinId);
}

void SGPIO_clearOeReg (const SGPIO_Pin pinId) {
	
	LPC_SGPIO->GPIO_OENREG &= (~(0x1 << pinId));
}


void SGPIO_disableSlices(const uint32_t sliceMask) {
	
	LPC_SGPIO->CTRL_ENABLED	 &= (~sliceMask);
	LPC_SGPIO->CTRL_DISABLED	 &= (~sliceMask);

}

void SGPIO_enableSlices(const bool stop, const uint32_t sliceMask) {
	
	LPC_SGPIO->CTRL_ENABLED	 |= sliceMask;
	if(stop == true) LPC_SGPIO->CTRL_DISABLED	 |= sliceMask;

}

void SGPIO_disableSlice(SGPIO_Slice sliceId) {
	
	LPC_SGPIO->CTRL_ENABLED	 &= (~(1 << sliceId));
	LPC_SGPIO->CTRL_DISABLED	 &= (~(1 << sliceId));

}

void SGPIO_enableSlice(const bool stop, const SGPIO_Slice sliceId) {
	
	LPC_SGPIO->CTRL_ENABLED |= (1 << sliceId);
	if(stop == true) LPC_SGPIO->CTRL_DISABLED |= (1 << sliceId);

}





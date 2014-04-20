/*****************************************************************************
 * $Id:: sgpio_i2s.c
 *
 * Project: USB Audio Example for LPC18xx and LPC43xx
 *
 * Description: Configuration of six SGPIOs to construct an I2S interface
 *              with 4 stereo channels
 *
 *----------------------------------------------------------------------------
 *  Copyright(C) 2012, NXP Semiconductor
 *  All rights reserved.
 *
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
 *****************************************************************************/

#include "LPC43xx.h"
#include "config.h"
#include "lpc43xx_scu.h"
#include "board.h"
#include "sgpio_i2s.h"
#include "adc_user.h"


/*
unsigned int sGPIO_buffer(SGPIO_BUF_SIZE];
int into_sGPIO_buffer, outta_sGPIO_buffer, shiftcount;
*/


void processSample(int sample);
uint16_t* Codec_get_sample(USB_ADC_CTRL_T* pAdcCtrl, ADC_SUBSTREAM_T* pSubs) ;



/******************************************************************************
 *
 *
 *
 *
 ******************************************************************************/
void SGPIO_IRQloadData(void) {
	
	
	LPC_SGPIO->CTR_STATUS_1 = 0xffff;	// shotgun clear them all -- capture clock
	
	LPC_GPIO_PORT->SET[7] = 1<<18;		   // for scope debugging
	

	LPC_SGPIO->REG_SS[8] =	( sGPIO_buffer(outta_sGPIO_buffer + 768) );
	LPC_SGPIO->REG_SS[12] =	( sGPIO_buffer(outta_sGPIO_buffer + 512) );
	LPC_SGPIO->REG_SS[9] =  ( sGPIO_buffer(outta_sGPIO_buffer + 256) );
	LPC_SGPIO->REG_SS[13] =	( sGPIO_buffer(outta_sGPIO_buffer++) );
	
	LPC_SGPIO->REG_SS[0] =  ( sGPIO_buffer(outta_sGPIO_buffer + 768) );
	LPC_SGPIO->REG_SS[1] =  ( sGPIO_buffer(outta_sGPIO_buffer + 512) );
	LPC_SGPIO->REG_SS[4] =  ( sGPIO_buffer(outta_sGPIO_buffer + 256) );
	LPC_SGPIO->REG_SS[6] =	( sGPIO_buffer(outta_sGPIO_buffer++) );    

	LPC_GPIO_PORT->CLR[7] = 1<<18;
}



/******************************************************************************
 *
 *
 *
 *
 ******************************************************************************/
void SGPIO_Codec_IRQHandler(USB_ADC_CTRL_T* pAdcCtrl)
{
    uint32_t cur_level = 0;
    uint16_t* pData;
    ADC_SUBSTREAM_T* pSubs;
	
        cur_level = 2;
        /* get playback substream pointer */ 
        pSubs =  &pAdcCtrl->subs[SUBS_SPEAKER];
        /* check we are playing and also copy data from USB buffer if mute is
        disabled. Else copy mute data. */
        while (cur_level--) {
            if (pAdcCtrl->flags & ADC_PLAY_DATAVALID) {
                if ( pAdcCtrl->flags & ADC_PLAY_MUTE) {
                    /* for mute playback */
                    processSample(0);
                } 
                else {
                    pData = Codec_get_sample(pAdcCtrl, pSubs);

                    processSample( pData[1] | (pData[0] << 16) );
                }
            } 
            else
                /* for dummy playback */
                processSample(0);
                    
            pSubs->rd_count++;
        }
    
	SGPIO_IRQloadData();        
}



/******************************************************************************
 *
 *
 *
 *
 ******************************************************************************/
#ifdef CORE_M4

__asm int doRBIT(int x) {	//;		// sGPIO does LSB first, need to flip it 
 	RBIT r0, r0
	bx	 lr
} 	
#else

// do an RBIT is discrete code -- courtesy of bit twiddling hacks from the web
unsigned int doRBIT(unsigned int x) { 

    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));

}
#endif


/******************************************************************************
 *
 *
 *
 *
 ******************************************************************************/
void processSample(int sample) 
{
	// set to 1 to send input to output for debug  -- bte
	sGPIO_buffer(into_sGPIO_buffer+768) =
	sGPIO_buffer(into_sGPIO_buffer+512) =
	sGPIO_buffer(into_sGPIO_buffer+256) =
	sGPIO_buffer(into_sGPIO_buffer++) = doRBIT( sample );
}



#define INV_OUT_CLK   (1<<3)

#define CONCAT_ENABLE (1<<11)
#define SLICE_2		  (1<<12)
#define SLICE_4		  (2<<12)
#define SLICE_8		  (3<<12)

/******************************************************************************
 *
 *
 *
 *
 ******************************************************************************/
void setupSGPIO(void) {

    LPC_SGPIO->CLR_EN_0	= 0xFFFF;		// disable interrupting on clock
    LPC_SGPIO->CLR_EN_1	= 0xFFFF;		// disable interrupting on clock

	/* Disable all counters (should be in this state after reset but just make sure) */
	LPC_SGPIO->CTRL_ENABLED = 0;	

	/* Set all SGPIO pins as output */
	LPC_SGPIO->GPIO_OENREG = 0xFFFF;

	LPC_SGPIO->PRESET[3] = 0x0F;				// left/right word select
	
	LPC_SGPIO->PRESET[0] = 0x0F;				// audio bit clock
	LPC_SGPIO->PRESET[8] = 0x0F;				
	LPC_SGPIO->PRESET[4] = 0x0F;				
	LPC_SGPIO->PRESET[9] = 0x0F;				// 2 slice buffer
	
	LPC_SGPIO->PRESET[1] = 0x0F;
	LPC_SGPIO->PRESET[12] = 0x0F;			
	LPC_SGPIO->PRESET[6] = 0x0F;
	LPC_SGPIO->PRESET[13] = 0x0F;		// 2 slice buffer
		
	/* Load counter with initial value */

	LPC_SGPIO->COUNT[3] = 0;				
	LPC_SGPIO->COUNT[0] = 0;			
	LPC_SGPIO->COUNT[8] = 0;				
	LPC_SGPIO->COUNT[4] = 0;	
	LPC_SGPIO->COUNT[9] = 0;		
	LPC_SGPIO->COUNT[1] = 0;
	LPC_SGPIO->COUNT[12] = 0;			
	LPC_SGPIO->COUNT[6] = 0;
	LPC_SGPIO->COUNT[13] = 0;	
	
	
	
	/*************************************/
	/* Program slice D(3) as left-right  */
	/*************************************/

	LPC_SGPIO->SGPIO_MUX_CFG[3] = CONCAT_ENABLE;	
	
	LPC_SGPIO->REG_SS[3] = 0x7FFF8000;			// 16 bit mode 
	LPC_SGPIO->REG[3] =    0x7FFF8000;			// word shift leads data by 1 clock
	
	LPC_SGPIO->POS[3] = ((0x020 - 1) << 8) | (0x20 - 1);	// get the phase of POS counter to 0x1f
	
	LPC_SGPIO->OUT_MUX_CFG[12] = 0;				            // output the left-right signal on SGPIO12

	/*******************************************/
	/* Program slice A(0) as clock and stereo1 */
	/*******************************************/

	LPC_SGPIO->SGPIO_MUX_CFG[0] = 	
	LPC_SGPIO->SGPIO_MUX_CFG[8] = CONCAT_ENABLE | SLICE_2;	
	
	LPC_SGPIO->POS[0] = 	
	LPC_SGPIO->POS[8] = ((0x020*2 - 1) << 8) | (0x020*2 - 1);

	//LPC_SGPIO->SLICE_MUX_CFG[0] = INV_OUT_CLK;
	
	LPC_SGPIO->OUT_MUX_CFG[8] = (0x8 << 0);			// output the bit clock from A
	LPC_SGPIO->OUT_MUX_CFG[1] = 0;				    // output the data bit on SGPIO1
		
	/*************************************/
	/* Program slice B(1) as stereo2     */
	/*************************************/

	LPC_SGPIO->SGPIO_MUX_CFG[1] = 	
	LPC_SGPIO->SGPIO_MUX_CFG[12] = CONCAT_ENABLE | SLICE_2;	
	
	LPC_SGPIO->POS[1] = 	
	LPC_SGPIO->POS[12] = ((0x020*2 - 1) << 8) | (0x020*2 - 1);
	
	LPC_SGPIO->OUT_MUX_CFG[9] = 0;				// output the data bit on SGPIO9

	/*******************************************/
	/* Program slice E(4) as clock and stereo3 */
	/*******************************************/

	LPC_SGPIO->SGPIO_MUX_CFG[4] = 	
	LPC_SGPIO->SGPIO_MUX_CFG[9] = CONCAT_ENABLE | SLICE_2;	
	
	LPC_SGPIO->POS[4] = 	
	LPC_SGPIO->POS[9] = ((0x020*2 - 1) << 8) | (0x020*2 - 1);

	LPC_SGPIO->SLICE_MUX_CFG[0] = INV_OUT_CLK;
	
	LPC_SGPIO->OUT_MUX_CFG[3] = 0;				// output the data bit on SGPIO3	

	/*************************************/
	/* Program slice G(6) as stereo4     */
	/*************************************/

	LPC_SGPIO->SGPIO_MUX_CFG[6] = 	
	LPC_SGPIO->SGPIO_MUX_CFG[13] = CONCAT_ENABLE | SLICE_2;	
	
	LPC_SGPIO->POS[6] = 	
	LPC_SGPIO->POS[13] = ((0x020*2 - 1) << 8) | (0x020*2 - 1);
	
	LPC_SGPIO->OUT_MUX_CFG[11] = 0;				// output the data bit on SGPIO11
	

	// Enable counter under test to start bit stream generation
	LPC_SGPIO->CTRL_ENABLED = (1<<3)
							| (1<<0) | (1<<8) | (1<<4) | (1<<9)
							| (1<<1) | (1<<12) | (1<<6) | (1<<13) ;
							
	LPC_SCU->SFSCLK_2 = 1;	 // CLKOUT2 -- 24.576 clock for some codecs
    LPC_SCU->SFSCLK_3 = 1;	 // CLKOUT3 -- 24.576 clock for some codecs
	

    //	Pin mux setup for SGPIO pins
	scu_pinmux(0x9, 2, MD_PDN, FUNC0); 			// GPIO4[13] / ENET_RX_ERR / LED D12  -- a line to wiggle for debug

	scu_pinmux(0x1 ,18, MD_PDN, FUNC6);	// P1.18: SGPIO12:  
	scu_pinmux(0x9 ,6 , MD_PDN, FUNC6);	// P9.6 : SGPIO8: 
	scu_pinmux(0x9, 1 , MD_PDN, FUNC6);	// P9.1 : SGPIO1: 
	scu_pinmux(0x9 ,5 , MD_PDN, FUNC6);	// P9.5 : SGPIO3: 
	scu_pinmux(0x9 ,3 , MD_PDN, FUNC6);	// P9.3 : SGPIO9: 
	scu_pinmux(0x1, 17, MD_PDN, FUNC6);	// P1.17: SGPIO11: 
							
    // reset pointers, actually only byte values, but writing as int makes it easier to see in debug
    sGPIO_buffer(-1) = 0;		//outta_sGPIO_buffer
   	sGPIO_buffer(-2) = 0;	//into_sGPIO_buffer

	
}




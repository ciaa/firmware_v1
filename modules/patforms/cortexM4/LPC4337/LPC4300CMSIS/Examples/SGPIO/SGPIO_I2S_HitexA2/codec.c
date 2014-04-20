/***********************************************************************
* $Id:: codec.c 
*
* Project: USB audio device class demo
*
* Description: Routines to handle codec control 
*
***********************************************************************
*   Copyright(C) 2012, NXP Semiconductor
*   All rights reserved.
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
**********************************************************************/
#include <stdio.h>
#include "LPC43xx.h"
#include "adc_user.h"
#include "config.h"
#include "sgpio_i2s.h"


/*******************************************************************************
* Function: I2S_audio_playback
*******************************************************************************/
void Codec_play(USB_ADC_CTRL_T* pAdcCtrl, uint32_t enable)
{
    ADC_SUBSTREAM_T* pSubs = &pAdcCtrl->subs[SUBS_SPEAKER];
    if (enable) {
        pSubs->i2s_pos  = 0;
        pAdcCtrl->flags &= ~ADC_PLAY_DATAVALID;
        pAdcCtrl->flags |= ADC_PLAYING;
        ADC_start_xfr(pAdcCtrl, pSubs, ADC_USB_WRITER);

	NVIC_EnableIRQ(SGPIO_IINT_IRQn);
    	LPC_SGPIO->SET_EN_1	= 1 << 0;		// start interrupting -- capture clock slice A
    } else {
        pAdcCtrl->flags &= ~(ADC_PLAY_DATAVALID | ADC_PLAYING);
        ADC_stop_xfr(pAdcCtrl, pSubs, ADC_USB_WRITER);
        /* disable tx if both playback & capture are off */
        if ((pAdcCtrl->flags & ADC_RECORDING) == 0) 
	{ 

        }
    	LPC_SGPIO->CLR_EN_1	= 0xffff;		// stop interrupting -- all sourcesm though only slice A clock used for now
    }
    pSubs->i2s_pos  = 0;
    pSubs->rd_idx  = 0;
}

/*******************************************************************************
* Function: I2S_audio_record	 -- disabled
*******************************************************************************/
void Codec_record(USB_ADC_CTRL_T* pAdcCtrl, uint32_t enable)
{
}

uint16_t* Codec_get_sample(USB_ADC_CTRL_T* pAdcCtrl, ADC_SUBSTREAM_T* pSubs)
{
    uint16_t* ret;
    DTD_T* pDTD = &pSubs->head_TD[pSubs->rd_idx];

    ret = (uint16_t*) (pDTD->buffer2 + pSubs->i2s_pos);
    pSubs->i2s_pos += 4;

    if (pSubs->i2s_pos >= pDTD->buffer3) {
        /* reset the packet length */
        pDTD->buffer3 = 0;
        pSubs->rd_idx++;
        pSubs->rd_idx &= (NUM_DTDS - 1);
        pSubs->i2s_pos = 0;
        if (pSubs->rd_idx == pSubs->wr_idx) {
            pSubs->wr_valid_idx = 0;
            pAdcCtrl->flags &= ~ADC_PLAY_DATAVALID;
        }
    }
        
    return ret;
}

uint32_t* Codec_Sync_next_wr(void* ctrl, ADC_SUBSTREAM_T* pSubs)
{
    USB_ADC_CTRL_T* pAdcCtrl = (USB_ADC_CTRL_T*)ctrl;
    uint32_t* pBuf = (uint32_t*)(pSubs->buf + (pSubs->i2s_pos << 2));

    if (++pSubs->i2s_pos >= pSubs->buf_sz) { 
        pSubs->i2s_pos = 0; 
    }
    
    if ( ((pAdcCtrl->flags & ADC_REC_DATAVALID) == 0) && 
        (pSubs->i2s_pos >= (pSubs->buf_sz/2)) ) {
        pAdcCtrl->flags |= ADC_REC_DATAVALID; 
        pAdcCtrl->pRegs->endptprime |= pSubs->ep_bit;   /*start USB DMA */
    }
        
    return pBuf;
}

uint32_t* Codec_Async_next_wr(void* ctrl, ADC_SUBSTREAM_T* pSubs)
{
    USB_ADC_CTRL_T* pAdcCtrl = (USB_ADC_CTRL_T*)ctrl;
    DTD_T* pDTD = &pSubs->head_TD[pSubs->wr_idx];
    uint32_t* ret;

    ret = (uint32_t*) (pDTD->buffer2 + pSubs->i2s_pos);
    pSubs->i2s_pos += 4;

    if (pSubs->i2s_pos == (pSubs->maxp - 4)) {
        pSubs->i2s_pos = 0;
        /* set the packet length */
        if (pSubs->delta_samples > 0){
            pDTD->buffer3 = pSubs->maxp;
            pSubs->delta_samples--;
            pSubs->wr_idx++;
            pSubs->wr_idx &= (NUM_DTDS - 1);
        } else {
            pSubs->wr_idx++;
            pSubs->wr_idx &= (NUM_DTDS - 1);
            if (pSubs->delta_samples < 0) {
                pDTD->buffer3 = pSubs->maxp - 8;
                pSubs->delta_samples++;
    
                pSubs->i2s_pos = 4;
                /* move sample to next TD */
                ret--;
                *((uint32_t*)pSubs->head_TD[pSubs->wr_idx].buffer2) = *ret;

            } else {
                pDTD->buffer3 = pSubs->maxp - 4;
            }
            ret = (uint32_t*) (pSubs->head_TD[pSubs->wr_idx].buffer2 + pSubs->i2s_pos);
        }
        pDTD->total_bytes = (pDTD->buffer3 << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC;
        if ( ((pAdcCtrl->flags & ADC_REC_DATAVALID) == 0) && 
            ((pSubs->wr_idx - pSubs->rd_idx) >= (NUM_DTDS/2)) ) {
            pAdcCtrl->flags |= ADC_REC_DATAVALID; 
            pAdcCtrl->pRegs->endptprime |= pSubs->ep_bit;   /*start USB DMA */
        }
    }
        
    return ret;
}





/*******************************************************************************
*                    END of FILE
*******************************************************************************/


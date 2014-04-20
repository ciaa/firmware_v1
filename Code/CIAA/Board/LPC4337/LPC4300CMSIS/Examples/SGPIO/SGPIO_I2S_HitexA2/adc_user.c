/***********************************************************************
* $Id:: adc_user.c
*
* Project: USB Audio class
*
* Description:
*     USB Audio Device Class Custom User Module.
*
***********************************************************************
*   Copyright(C) 2011, NXP Semiconductor
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
#include <string.h>
#include "LPC43xx.h"
/* USB header files */
#include "usbd/usbd_core.h"
#include "usbd/usbd_hw.h"
#include "usbd/usbd_rom_api.h"
#include "usbd_config.h"
#include "adc_user.h"
#include "board.h"



/*
 *  Audio Device Class Interface Get Request Callback
 *   Called automatically on ADC Interface Get Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    LPC_OK - Success, ERR_USBD_INVALID_REQ - Error
 */

ErrorCode_t ADC_IF_GetRequest (USB_ADC_CTRL_T* pAdcCtrl, USB_CORE_CTRL_T* pCtrl) {

/*
  Interface = SetupPacket.wIndex.WB.L;
  EntityID  = SetupPacket.wIndex.WB.H;
  Request   = SetupPacket.bRequest;
  Value     = SetupPacket.wValue.W;
  ...
*/
    ErrorCode_t ret = ERR_USBD_INVALID_REQ;

    if (pCtrl->SetupPacket.wIndex.W == 0x0200) {
        /* Feature Unit: Interface = 0, ID = 2 */
        if (pCtrl->SetupPacket.wValue.WB.L == 0) {
            /* Master Channel */
            switch (pCtrl->SetupPacket.wValue.WB.H) {
            case AUDIO_MUTE_CONTROL:
                if (pCtrl->SetupPacket.bRequest == AUDIO_REQUEST_GET_CUR) {
                    pCtrl->EP0Buf[0] = (pAdcCtrl->flags & ADC_PLAY_MUTE)?1:0;
                    ret = LPC_OK;
                }
                break;
            case AUDIO_VOLUME_CONTROL:
                switch (pCtrl->SetupPacket.bRequest) {
                case AUDIO_REQUEST_GET_CUR:
                    *((__packed uint16_t *)pCtrl->EP0Buf) = pAdcCtrl->curr_vol;
                    ret = LPC_OK;
                    break;
                case AUDIO_REQUEST_GET_MIN:
                    *((__packed uint16_t *)pCtrl->EP0Buf) = VOLUME_MIN;
                    ret = LPC_OK;
                    break;
                case AUDIO_REQUEST_GET_MAX:
                    *((__packed uint16_t *)pCtrl->EP0Buf) = VOLUME_MAX;
                    ret = LPC_OK;
                    break;
                case AUDIO_REQUEST_GET_RES:
                    *((__packed uint16_t *)pCtrl->EP0Buf) = VOLUME_RES;
                    ret = LPC_OK;
                    break;
                }
                break;
            }
        }
    }
    
    return (ret);  /* Not Supported */
}


/*
 *  Audio Device Class Interface Set Request Callback
 *   Called automatically on ADC Interface Set Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    LPC_OK - Success, ERR_USBD_INVALID_REQ - Error
 */

ErrorCode_t ADC_IF_SetRequest (USB_ADC_CTRL_T* pAdcCtrl, USB_CORE_CTRL_T* pCtrl) {

/*
  Interface = SetupPacket.wIndex.WB.L;
  EntityID  = SetupPacket.wIndex.WB.H;
  Request   = SetupPacket.bRequest;
  Value     = SetupPacket.wValue.W;
  ...
*/
    ErrorCode_t ret = ERR_USBD_INVALID_REQ;
    if (pCtrl->SetupPacket.wIndex.W == 0x0200) {
        /* Feature Unit: Interface = 0, ID = 2 */
        if ((pCtrl->SetupPacket.wValue.WB.L == 0) &&
            (pCtrl->SetupPacket.bRequest == AUDIO_REQUEST_SET_CUR)) {
            /* Master Channel */
            switch (pCtrl->SetupPacket.wValue.WB.H) {
            case AUDIO_MUTE_CONTROL:
                if (pCtrl->EP0Buf[0])
                    pAdcCtrl->flags |= ADC_PLAY_MUTE;
                else
                    pAdcCtrl->flags &= ~ADC_PLAY_MUTE;
                
                ret = (LPC_OK);
                break;
            case AUDIO_VOLUME_CONTROL:
                //codec_set_volume(*((__packed uint16_t *)pCtrl->EP0Buf));
                pAdcCtrl->curr_vol = *((__packed uint16_t *)pCtrl->EP0Buf);              
                ret =  (LPC_OK);
                break;
            }
        }
    }

    return ret;  /* Not Supported */
}


/*
 *  Audio Device Class EndPoint Get Request Callback
 *   Called automatically on ADC EndPoint Get Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    LPC_OK - Success, ERR_USBD_INVALID_REQ - Error
 */

ErrorCode_t ADC_EP_GetRequest (USB_ADC_CTRL_T* pAdcCtrl, USB_CORE_CTRL_T* pCtrl) {

/*
  EndPoint = SetupPacket.wIndex.WB.L;
  Request  = SetupPacket.bRequest;
  Value    = SetupPacket.wValue.W;
  ...
*/
    int dir = SUBS_SPEAKER;
    ErrorCode_t ret = ERR_USBD_INVALID_REQ;
    
    switch(pCtrl->SetupPacket.wIndex.W) {
    case USB_ADC_IN_EP: 
        dir = SUBS_MIC;
    case USB_ADC_OUT_EP: 
        /* Feature Unit: Interface = 0, ID = 2 */
        if (pCtrl->SetupPacket.wValue.WB.L == 0) {
            /* Master Channel */
            if ((pCtrl->SetupPacket.wValue.WB.H == AUDIO_CONTROL_SAMPLING_FREQ) &&
                (pCtrl->SetupPacket.bRequest == AUDIO_REQUEST_GET_CUR) ) {
                pCtrl->EP0Buf[0] = (uint8_t)(pAdcCtrl->subs[dir].sample_rate & 0xFF);
                pCtrl->EP0Buf[1] = (uint8_t)((pAdcCtrl->subs[dir].sample_rate >> 8) & 0xFF);
                pCtrl->EP0Buf[2] = (uint8_t)((pAdcCtrl->subs[dir].sample_rate >> 16) & 0xFF);
                ret = (LPC_OK);
            }
        }
        break;
    default:
        break;
    }
    return ret;  /* Not Supported */
}


/*
 *  Audio Device Class EndPoint Set Request Callback
 *   Called automatically on ADC EndPoint Set Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    LPC_OK - Success, ERR_USBD_INVALID_REQ - Error
 */

ErrorCode_t ADC_EP_SetRequest (USB_ADC_CTRL_T* pAdcCtrl, USB_CORE_CTRL_T* pCtrl) {

/*
  EndPoint = SetupPacket.wIndex.WB.L;
  Request  = SetupPacket.bRequest;
  Value    = SetupPacket.wValue.W;
  ...
*/
    int dir = SUBS_SPEAKER;
    uint32_t rate;
    ErrorCode_t ret = ERR_USBD_INVALID_REQ;
    
    switch(pCtrl->SetupPacket.wIndex.W) {
    case USB_ADC_IN_EP: 
        dir = SUBS_MIC;
    case USB_ADC_OUT_EP: 
        /* Feature Unit: Interface = 0, ID = 2 */
        if (pCtrl->SetupPacket.wValue.WB.L == 0) {
            /* Master Channel */
            if (pCtrl->SetupPacket.wValue.WB.H == AUDIO_CONTROL_SAMPLING_FREQ) {
                rate = pCtrl->EP0Buf[0] | (pCtrl->EP0Buf[1] << 8) | (pCtrl->EP0Buf[2] << 16);
                if (pCtrl->SetupPacket.bRequest == AUDIO_REQUEST_SET_CUR) {
                    if (rate == pAdcCtrl->subs[dir].sample_rate) {
                        ret = LPC_OK;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    return (ret);  /* Not Supported */
}

ErrorCode_t ADC_Interface_Event (USBD_HANDLE_T hUsb /*, */) 
{
    USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);
    uint16_t wIndex = pCtrl->SetupPacket.wIndex.W;
    uint16_t wValue = pCtrl->SetupPacket.wValue.W;


    /* write code to enable/disable audo capture when interface 
    ALT setting is changed */
    if (wIndex == USB_ADC_SIF2_NUM) {
        Codec_record(pAdcCtrl, (wValue == 0x0001));
    }
    
    /* write code to enable/disable audo playback when interface 
    ALT setting is changed */
    if (wIndex == USB_ADC_SIF1_NUM) {
        Codec_play(pAdcCtrl, (wValue == 0x0001));
    }

    if (pAdcCtrl->flags & (ADC_PLAYING | ADC_RECORDING)) {
        USBD_API->hw->EnableEvent (hUsb, 0, USB_EVT_SOF, 1);
    } else {
        USBD_API->hw->EnableEvent (hUsb, 0, USB_EVT_SOF, 0);
        pAdcCtrl->sof_count = 0;
        pAdcCtrl->subs[SUBS_SPEAKER].rd_count = pAdcCtrl->subs[SUBS_SPEAKER].wr_count = 0;
        pAdcCtrl->subs[SUBS_MIC].rd_count = pAdcCtrl->subs[SUBS_MIC].wr_count = 0;
    }

    return LPC_OK;
}

ErrorCode_t ADC_ep0_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
    USB_ADC_CTRL_T* pAdcCtrl = (USB_ADC_CTRL_T*) data;
    ErrorCode_t ret = ERR_USBD_UNHANDLED;
    
    if (pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_CLASS) {
        switch (event) {
        case USB_EVT_SETUP:
            if ((pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE) &&
            ((pCtrl->SetupPacket.wIndex.WB.L == USB_ADC_CIF_NUM)  ||       /* IF number correct? */
            (pCtrl->SetupPacket.wIndex.WB.L == USB_ADC_SIF1_NUM) ||
            (pCtrl->SetupPacket.wIndex.WB.L == USB_ADC_SIF2_NUM)) ) {
                switch (pCtrl->SetupPacket.bRequest) {
                case AUDIO_REQUEST_GET_CUR:
                case AUDIO_REQUEST_GET_MIN:
                case AUDIO_REQUEST_GET_MAX:
                case AUDIO_REQUEST_GET_RES:
                    
                    ret = ADC_IF_GetRequest(pAdcCtrl, pCtrl);                 
                    if (ret == LPC_OK) {
                        pCtrl->EP0Data.pData = pCtrl->EP0Buf;                     /* point to data to be sent */
                        USBD_API->core->DataInStage(pCtrl);                       /* send requested data */
                    }
                    break;
                case AUDIO_REQUEST_SET_CUR:
                    //                case AUDIO_REQUEST_SET_MIN:
                    //                case AUDIO_REQUEST_SET_MAX:
                    //                case AUDIO_REQUEST_SET_RES:
                    pCtrl->EP0Data.pData = pCtrl->EP0Buf;                              /* data to be received */ 
                    ret = LPC_OK;
                    break;
                }
            } else if (pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_ENDPOINT) {
                switch (pCtrl->SetupPacket.bRequest) {
                case AUDIO_REQUEST_GET_CUR:
                case AUDIO_REQUEST_GET_MIN:
                case AUDIO_REQUEST_GET_MAX:
                case AUDIO_REQUEST_GET_RES:
                    ret = ADC_EP_GetRequest(pAdcCtrl, pCtrl);
                    if (ret == LPC_OK) {
                        pCtrl->EP0Data.pData = pCtrl->EP0Buf;                              /* point to data to be sent */
                        USBD_API->core->DataInStage(pCtrl);                                   /* send requested data */
                    }
                    break;
                case AUDIO_REQUEST_SET_CUR:
                    //              case AUDIO_REQUEST_SET_MIN:
                    //              case AUDIO_REQUEST_SET_MAX:
                    //              case AUDIO_REQUEST_SET_RES:
                    pCtrl->EP0Data.pData = pCtrl->EP0Buf;                                /* data to be received */ 
                    ret = LPC_OK;
                    break;
                }
            } 
            break;
        case USB_EVT_OUT:
            if ((pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE) &&
            ((pCtrl->SetupPacket.wIndex.WB.L == USB_ADC_CIF_NUM)  ||       /* IF number correct? */
            (pCtrl->SetupPacket.wIndex.WB.L == USB_ADC_SIF1_NUM) ||
            (pCtrl->SetupPacket.wIndex.WB.L == USB_ADC_SIF2_NUM)) ) {
                switch (pCtrl->SetupPacket.bRequest) {
                case AUDIO_REQUEST_SET_CUR:
                    //                      case AUDIO_REQUEST_SET_MIN:
                    //                      case AUDIO_REQUEST_SET_MAX:
                    //                      case AUDIO_REQUEST_SET_RES:
                    ret = ADC_IF_SetRequest(pAdcCtrl, pCtrl);
                    if (ret == LPC_OK) {
                        USBD_API->core->StatusInStage(pCtrl);                         /* send Acknowledge */
                    }
                    break;
                }
            } else if (pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_ENDPOINT) {
                switch (pCtrl->SetupPacket.bRequest) {
                case AUDIO_REQUEST_SET_CUR:
                    //                    case AUDIO_REQUEST_SET_MIN:
                    //                    case AUDIO_REQUEST_SET_MAX:
                    //                    case AUDIO_REQUEST_SET_RES:
                    ret = ADC_EP_SetRequest(pAdcCtrl, pCtrl);
                    if (ret == LPC_OK) {
                        USBD_API->core->StatusInStage(pCtrl);                           /* send Acknowledge */
                    }
                    break;
                }
            }
            break;
        
        default:
            break;
        }
    }  
    return ret;
}


void ADC_start_xfr(USB_ADC_CTRL_T* pAdcCtrl, ADC_SUBSTREAM_T* pSubs, uint32_t mode)
{
    int i;
    DQH_T *pQH = pSubs->ep_QH;
    DTD_T *pDTD = pSubs->head_TD;
    uint32_t  ptrBuff =  (uint32_t)pSubs->buf;
    uint32_t total_bytes =  ((pSubs->maxp << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC);
    
    /* reinit TDs */
    for (i = 0; i < NUM_DTDS; i++) {
        /* set the link*/
        pDTD[i].next_dTD = (uint32_t) &pDTD[i + 1];
        pDTD[i].total_bytes = total_bytes;
        /* allocate  buffers each 1msec worth */
        pDTD[i].buffer0 = ptrBuff;
        pDTD[i].buffer1 = (ptrBuff + 0x1000) & 0xfffff000;
        pDTD[i].buffer2 = ptrBuff;
        ptrBuff += pSubs->maxp;
    }
    /* update the end TD to terminate linked list */
    pDTD[NUM_DTDS - 1].next_dTD = TD_NEXT_TERMINATE; //(uint32_t) &pDTD[0];
    pSubs->tail_TD = &pDTD[NUM_DTDS - 1];
    /* reset the current TD pointer to head*/
    if (mode == ADC_USB_WRITER) {
        pSubs->wr_idx = pSubs->wr_valid_idx = 0;
        /* queue sync packet for asynchrounus/adaptive mode */
        if (pAdcCtrl->flags & ADC_ASYNC_MODE) {
            pDTD = pSubs->sync_TD;
            /* set the link*/
            pDTD->next_dTD = TD_NEXT_TERMINATE;
            pDTD->total_bytes = ((USB_ADC_SYNC_MAXP << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC);
            /* allocate  buffers each 1msec worth */
            pDTD->buffer0 = (uint32_t)&pSubs->sync_data;
            pDTD->buffer1 = (pDTD->buffer0 + 0x1000) & 0xfffff000;
            pDTD->buffer2 = pDTD->buffer3 = pDTD->buffer4 = 0;
            /* Initialize the Queue head */
            pSubs->sync_ep_QH->next_dTD = (uint32_t)pDTD;
            pSubs->sync_ep_QH->cap = QH_MULT(1) | QH_MAXP(USB_ADC_SYNC_MAXP);
            pSubs->sync_ep_QH->total_bytes &= (~0xC0);
            pAdcCtrl->pRegs->endptprime |= pSubs->sync_ep_bit;    /* Prime the endpoint for read/write*/
        }
    } else {
        pSubs->rd_idx = 0;
    }
    
    /* Initialize the Queue head */
    pQH->next_dTD = (uint32_t)pSubs->head_TD;
    pQH->cap = QH_MULT(1) | QH_MAXP(pSubs->maxp);
    pQH->total_bytes &= (~0xC0);
    /* prime only when USB recives data first */
    if (mode == ADC_USB_WRITER) 
        pAdcCtrl->pRegs->endptprime |= pSubs->ep_bit;    /* Prime the endpoint for read/write*/
}

void ADC_stop_xfr(USB_ADC_CTRL_T* pAdcCtrl, ADC_SUBSTREAM_T* pSubs, uint32_t mode)
{
    /* flush EP by calling reset */
    USBD_API->hw->ResetEP((USBD_HANDLE_T)pAdcCtrl->pUsbCtrl, pSubs->ep_num);
    if (pAdcCtrl->flags & ADC_ASYNC_MODE) {
        /* flush EP by calling reset */
        USBD_API->hw->ResetEP((USBD_HANDLE_T)pAdcCtrl->pUsbCtrl, pSubs->sync_ep_num);
    }
}

ErrorCode_t ADC_iso_in_hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    ADC_SUBSTREAM_T* pSubs = (ADC_SUBSTREAM_T*)data;
    DTD_T *pDTD = &pSubs->head_TD[pSubs->rd_idx];
    uint32_t total_bytes =  ((pSubs->maxp << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC);

    if (event == USB_EVT_IN) {
        while ( (pDTD->total_bytes & TD_ACTIVE) == 0) {
            pDTD = &pSubs->head_TD[pSubs->rd_idx];
            /* update read index */
            pSubs->rd_idx++;
            pSubs->rd_idx &= (NUM_DTDS - 1);
            /* store samples sent */
            pSubs->rd_count += ((pDTD->buffer0 - pDTD->buffer2) >> 2);

            /* clear and re-queue the buffer */
            pDTD->buffer0 = pDTD->buffer2;
            pDTD->buffer1 &= ~0xFFF;
            pDTD->next_dTD = TD_NEXT_TERMINATE;
            pDTD->total_bytes = total_bytes;
            /* re-queue the td at the end of list */
            pSubs->tail_TD->next_dTD = (uint32_t)pDTD;
            pSubs->tail_TD = pDTD;
        }
    }
    return LPC_OK;
}

ErrorCode_t ADC_iso_out_hdlr (USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    int32_t diff;
    ADC_SUBSTREAM_T* pSubs = (ADC_SUBSTREAM_T*)data;
    DTD_T *pDTD = &pSubs->head_TD[pSubs->wr_idx];
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);
    uint32_t total_bytes =  ((pSubs->maxp << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC);

    if (event == USB_EVT_OUT) {
        while ( (pDTD->total_bytes & TD_ACTIVE) == 0) {
            /* current write TD */
            pDTD = &pSubs->head_TD[pSubs->wr_idx];
            /* incremnet write index */
            pSubs->wr_idx++;
            /* re-use buffer3 to store length of received pkt */
            pDTD->buffer3 = pDTD->buffer0 - pDTD->buffer2;
            pSubs->wr_count += (pDTD->buffer3 >> 2);
            /* check if we recived data */
            if (pDTD->buffer3 > 0) {
             
                if((pAdcCtrl->flags & ADC_PLAY_DATAVALID) == 0) {

                    if (pSubs->wr_valid_idx == 0)
                        pSubs->wr_valid_idx = pSubs->wr_idx;
                    /* check if we buffered enough data to set the data valid flag */
                    if (pSubs->wr_idx > pSubs->wr_valid_idx) {
                        diff = pSubs->wr_idx - pSubs->wr_valid_idx;
                    } else {
                        diff = pSubs->wr_idx + (NUM_DTDS - pSubs->wr_valid_idx);
                    }
                    if ( diff >= NUM_DTDS/2) {
                        pAdcCtrl->flags |= ADC_PLAY_DATAVALID;
                        pSubs->rd_idx = pSubs->wr_valid_idx - 1;
                    }
                }
            } else {
                /* reset valid index and data valid flag */
                pSubs->wr_valid_idx = 0;
                pAdcCtrl->flags &= ~ADC_PLAY_DATAVALID;
            } 
            pSubs->wr_idx &= (NUM_DTDS - 1);

            /* clear and re-queue the buffer */
            pDTD->buffer0 = pDTD->buffer2;
            pDTD->buffer1 &= ~0xFFF;
            pDTD->next_dTD = TD_NEXT_TERMINATE;
            pDTD->total_bytes = total_bytes;
            /* re-queue the td at the end of list */
            pSubs->tail_TD->next_dTD = (uint32_t)pDTD;
            pSubs->tail_TD = pDTD;
        }
    }
    return LPC_OK;
}

/* check if any adjustment to the step is needed */
void adjust_step(USB_ADC_CTRL_T* pAdcCtrl, uint8_t curr_dir, int32_t curr_diff)
{
    /* double the step size if prev step is in same direction*/
    if (pAdcCtrl->prev_dir == curr_dir) {
        if ( pAdcCtrl->dir_count++ > 4)  {
            pAdcCtrl->prev_step <<= 1;
        }
    } else {
        pAdcCtrl->dir_count = 0;
        if ( pAdcCtrl->prev_step != FRAC_MIN_STEP)
            pAdcCtrl->prev_step = (pAdcCtrl->prev_step + (pAdcCtrl->prev_step >> 1)) >> 1;
    }
}

ErrorCode_t ADC_SyncSOF_Event (USBD_HANDLE_T hUsb)
{
    int32_t diff;
    int32_t chng = 0;
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);
    ADC_SUBSTREAM_T* pSubsSpkr = &pAdcCtrl->subs[SUBS_SPEAKER];

    /* check rate adjustment */
    if(++pAdcCtrl->sof_count >= 1000) {
        
    	/* do adjustment only if sample rate is in resaonable range. 
    	Takes care when audio starts within 1 sec. */
    	if ( (pSubsSpkr->rd_count < 48144) && 
            (pSubsSpkr->rd_count > 47856)) {
            
            diff = (48000 - pSubsSpkr->rd_count);
    		/* track rolling  average of samples over/under */
            pAdcCtrl->total_diff += diff; 
    
            if ( diff > 0) {
                /* rendering less samples so speed up */
                /* speedup only if the average also says we are slower*/
                if (pAdcCtrl->total_diff > 0)   {
                    adjust_step(pAdcCtrl, PLL_CHANGE_INC, diff);
                    pAdcCtrl->prev_dir = PLL_CHANGE_INC;
                    pAdcCtrl->cur_frac += pAdcCtrl->prev_step;
                    chng = 1;   
                }
    
            } else if ( diff < 0) {
                /* rendering more samples so slow down */
                /* slowdown only if the average also says we are faster*/
                if (pAdcCtrl->total_diff < 0) {
                    adjust_step(pAdcCtrl, PLL_CHANGE_DEC, diff);
                    pAdcCtrl->prev_dir = PLL_CHANGE_DEC;
                    pAdcCtrl->cur_frac -= pAdcCtrl->prev_step;
                    chng = 1;   
                }
            }
    
    
            if (chng ) {
                /* change the frequency */
                LPC_CGU->PLL0AUDIO_FRAC = pAdcCtrl->cur_frac;
                /* send the fraction update request */
                LPC_CGU->PLL0AUDIO_CTRL |= _BIT(12);
            }
        }
        /* read and reset WS counter */
        pAdcCtrl->sof_count = 0;
        pAdcCtrl->subs[SUBS_SPEAKER].rd_count = pAdcCtrl->subs[SUBS_SPEAKER].wr_count = 0;
        pAdcCtrl->subs[SUBS_MIC].rd_count = pAdcCtrl->subs[SUBS_MIC].wr_count = 0;
    }

    return LPC_OK;
}

ErrorCode_t ADC_Reset_Event (USBD_HANDLE_T hUsb)
{
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);

    pAdcCtrl->cur_frac = (86 << 15) | _BIT(8) | _BIT(3) | _BIT(2);
    pAdcCtrl->prev_step = FRAC_MIN_STEP;
    pAdcCtrl->total_diff = 0;
    pAdcCtrl->sof_count = 0;
    pAdcCtrl->subs[SUBS_SPEAKER].rd_count = pAdcCtrl->subs[SUBS_SPEAKER].wr_count = 0;
    pAdcCtrl->subs[SUBS_MIC].rd_count = pAdcCtrl->subs[SUBS_MIC].wr_count = 0;
        
    /* change the frequency */
    LPC_CGU->PLL0AUDIO_FRAC = pAdcCtrl->cur_frac;
    /* send the fraction update request */
    LPC_CGU->PLL0AUDIO_CTRL |= _BIT(12);

    return LPC_OK;        
}

ErrorCode_t ADC_out_sync_hdlr (USBD_HANDLE_T hUsb, void* data, uint32_t event) 
{
    ADC_SUBSTREAM_T* pSubs = (ADC_SUBSTREAM_T*)data;
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);
    DTD_T *pDTD = pSubs->sync_TD;

    if (event == USB_EVT_IN) {
        pDTD = pSubs->sync_TD;
        /* set the link*/
        pDTD->next_dTD = TD_NEXT_TERMINATE;
        pDTD->total_bytes = ((USB_ADC_SYNC_MAXP << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC);
        /* allocate  buffers each 1msec worth */
        pDTD->buffer0 = (uint32_t)&pSubs->sync_data;
        pDTD->buffer1 = (pDTD->buffer0 + 0x1000) & 0xfffff000;
        pDTD->buffer2 = pDTD->buffer3 = pDTD->buffer4 = 0;
        /* Initialize the Queue head */
        pSubs->sync_ep_QH->next_dTD = (uint32_t)pDTD;
        pSubs->sync_ep_QH->total_bytes &= (~0xC0);
        pAdcCtrl->pRegs->endptprime |= pSubs->sync_ep_bit;    /* Prime the endpoint for read/write*/
    }
    return LPC_OK;
}

ErrorCode_t ADC_in_sync_hdlr (USBD_HANDLE_T hUsb, void* data, uint32_t event) 
{
    ADC_SUBSTREAM_T* pSubs = (ADC_SUBSTREAM_T*)data;
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);
    DTD_T *pDTD = pSubs->sync_TD;

    if (event == USB_EVT_OUT) {
        pDTD = pSubs->sync_TD;
        /* set the link*/
        pDTD->next_dTD = TD_NEXT_TERMINATE;
        pDTD->total_bytes = ((USB_ADC_SYNC_MAXP << 16) | TD_MULTO(1) | TD_ACTIVE | TD_IOC);
        /* allocate  buffers each 1msec worth */
        pDTD->buffer0 = (uint32_t)&pSubs->sync_data;
        pDTD->buffer1 = (pDTD->buffer0 + 0x1000) & 0xfffff000;
        pDTD->buffer2 = pDTD->buffer3 = pDTD->buffer4 = 0;
        /* Initialize the Queue head */
        pSubs->sync_ep_QH->next_dTD = (uint32_t)pDTD;
        pSubs->sync_ep_QH->total_bytes &= (~0xC0);
        pAdcCtrl->pRegs->endptprime |= pSubs->sync_ep_bit;    /* Prime the endpoint for read/write*/
    }
    return LPC_OK;
}

ErrorCode_t ADC_ASyncSOF_Event (USBD_HANDLE_T hUsb)
{
    uint32_t range, fast = 0, temp;
    USB_ADC_CTRL_T* pAdcCtrl = GET_ADC_CTRL(hUsb);
    ADC_SUBSTREAM_T* pSubs;
    
    ++pAdcCtrl->sof_count;
    if (pAdcCtrl->sof_count >= (1 << USB_ADC_SYNC_REFRESH)) {

        if (pAdcCtrl->flags & ADC_PLAYING) {
            /* get speaker control */
            pSubs = &pAdcCtrl->subs[SUBS_SPEAKER];
            /* set the current sync data to be sent to ideal value */
            pSubs->sync_data = DEF_SYNC_VALUE;
            
            /*check if the sample count is in range */
            range = DEF_SYNC_VALUE >> (14 - USB_ADC_SYNC_REFRESH);
            if (pSubs->rd_count < range) {
                range -= pSubs->rd_count;
                fast = 0;
            } else {
                range = pSubs->rd_count - range;
                fast = 1;
            }
            /* update sync data only if valid. Max difference should be
            less than 100/32 =  3.125%. So divide range by 32(shift right 5)*/
            if (range < (DEF_SYNC_VALUE >> (14 - USB_ADC_SYNC_REFRESH + 5)) ) {
    
                /* include current packet transfer for calculation*/
                temp  = (pSubs->maxp - (pSubs->ep_QH->total_bytes >> 16)) >> 2;
                pSubs->wr_count += temp;      
                /* check the buffer pointers. Do they also indicate similar
                data rate trend. */
                if ( ((pSubs->wr_count < pSubs->rd_count) && (fast == 1)) ||
                    ((pSubs->wr_count > pSubs->rd_count) && (fast == 0) ) ) {
                    /* calculate the number of samples rendered in 1msec
                    in Q10.14 format */
                    pSubs->sync_data = pSubs->rd_count << (14 - USB_ADC_SYNC_REFRESH);
                }
            }
            else {
                /* why are we here ??*/
                pSubs->sync_data = DEF_SYNC_VALUE;
            } 
        }
        if (pAdcCtrl->flags & ADC_RECORDING) {
            /* get mic control */
            pSubs = &pAdcCtrl->subs[SUBS_SPEAKER];

            /* include current packet transfer for calculation*/
            //temp  = (pSubs->ep_QH->buffer3 - (pSubs->ep_QH->total_bytes >> 16)) >> 2;
            //pSubs->rd_count += temp;      
            /* compute fractional/extra samples to send/reduce to be sent to host */
            pSubs->delta_samples += (pSubs->wr_count - pSubs->rd_count);
        }
        
        /* reset 512ms sample counter */
        pAdcCtrl->sof_count = 0;
        pAdcCtrl->subs[SUBS_SPEAKER].rd_count = pAdcCtrl->subs[SUBS_SPEAKER].wr_count = 0;
        pAdcCtrl->subs[SUBS_MIC].rd_count = pAdcCtrl->subs[SUBS_MIC].wr_count = 0;
    }
    return LPC_OK;        
}

/*----------------------------------------------------------------------------
  USB device audio class init routine
 *----------------------------------------------------------------------------*/
ErrorCode_t ADC_init(USBD_HANDLE_T hUsb, USBD_API_INIT_PARAM_T* param, 
    USB_ADC_CTRL_T** ppAdcCtrl, uint32_t sync_mode)
{
    ErrorCode_t ret = LPC_OK;
    USB_ADC_CTRL_T* pAdcCtrl;
    uint32_t ep_indx = 0;
    uint32_t maxp = DEF_MAX_PKT_SZ;
    uint32_t mem_required;
    int i;
    /* calculate MAXP depending on sync mode */
    if (sync_mode & ADC_ASYNC_MODE)
        maxp += SAMPLE_SIZE;
    
    /* calculate memory required */
    mem_required = (sizeof(USB_ADC_CTRL_T) + ((2*NUM_DTDS) * (maxp + sizeof(DTD_T))));
    mem_required += (MAX_SUBSTREAMS * 0x1F); /* pading mem used for alignment */
        
    /* check for memory alignment */
    if ((param->mem_base &  0x3) && 
        (param->mem_size < mem_required) )
        return ERR_USBD_BAD_MEM_BUF;
    
    /* allocate memory for the control data structure */
    pAdcCtrl = (USB_ADC_CTRL_T*)(param->mem_base);
    param->mem_base += sizeof(USB_ADC_CTRL_T);
    param->mem_size -= sizeof(USB_ADC_CTRL_T);
    /* Init control structures with passed params */
    memset((void*)pAdcCtrl, 0, sizeof(USB_ADC_CTRL_T));
    pAdcCtrl->pUsbCtrl = (USB_CORE_CTRL_T*)hUsb; 
    *ppAdcCtrl = pAdcCtrl;
    pAdcCtrl->pRegs = (USB_OTG_REGS_T*)param->usb_reg_base;
    /* force full speed for USB1 */
    pAdcCtrl->pRegs->portsc1 |= (1<<24);

    pAdcCtrl->i2s_next_wr = Codec_Sync_next_wr;
    /* set the synchronization mode flag*/
    if (sync_mode & ADC_ASYNC_MODE) {
        pAdcCtrl->flags |= ADC_ASYNC_MODE;
        pAdcCtrl->i2s_next_wr = Codec_Async_next_wr;
    }

    /* init speaker substream */
    pAdcCtrl->subs[SUBS_SPEAKER].ep_QH = USB_GetEpQH(hUsb, USB_ADC_OUT_EP);/*endpoint queue head */
    pAdcCtrl->subs[SUBS_SPEAKER].ep_bit = _BIT(USB_ADC_OUT_EP & 0xF);   /* endpoint control bit */
    pAdcCtrl->subs[SUBS_SPEAKER].ep_num = USB_ADC_OUT_EP;               /* endpoint number */
    pAdcCtrl->subs[SUBS_SPEAKER].sample_rate = DEF_SAMPLE_RATE;         /* default sample rate */
    pAdcCtrl->subs[SUBS_SPEAKER].maxp = DEF_MAX_PKT_SZ;                 /* MAX packet size */
    if ( sync_mode & ADC_ASYNC_MODE) {
        pAdcCtrl->subs[SUBS_SPEAKER].maxp += SAMPLE_SIZE;               /* Asyn can recive 1 extra sample */
        pAdcCtrl->subs[SUBS_SPEAKER].sync_ep_QH = USB_GetEpQH(hUsb, 
                                                    USB_ADC_OUTSYNC_EP);/* sync endpoint queue head */
        pAdcCtrl->subs[SUBS_SPEAKER].sync_ep_bit = 
                                _BIT((USB_ADC_OUTSYNC_EP & 0xF) + 16);  /* endpoint control bit */
        pAdcCtrl->subs[SUBS_SPEAKER].sync_ep_num = USB_ADC_OUTSYNC_EP;  /* endpoint number */
    }

    /* init mic/linein substream */
    pAdcCtrl->subs[SUBS_MIC].ep_QH =  USB_GetEpQH(hUsb, USB_ADC_IN_EP); /* endpoint queue head */
    pAdcCtrl->subs[SUBS_MIC].ep_bit = _BIT((USB_ADC_IN_EP & 0xF) + 16); /* endpoint control bit */
    pAdcCtrl->subs[SUBS_MIC].ep_num = USB_ADC_IN_EP;                    /* endpoint number */
    pAdcCtrl->subs[SUBS_MIC].sample_rate = DEF_SAMPLE_RATE;             /* default sample rate */
    pAdcCtrl->subs[SUBS_MIC].maxp = DEF_MAX_PKT_SZ;                     /* MAX packet size */
    if ( sync_mode & ADC_ASYNC_MODE) {
        pAdcCtrl->subs[SUBS_MIC].maxp += SAMPLE_SIZE;                   /* Asyn can recive 1 extra sample */
        pAdcCtrl->subs[SUBS_MIC].sync_ep_QH = USB_GetEpQH(hUsb, 
                                                    USB_ADC_INSYNC_EP); /* sync endpoint queue head */
        pAdcCtrl->subs[SUBS_MIC].sync_ep_bit = 
                                _BIT((USB_ADC_INSYNC_EP & 0xF));        /* endpoint control bit */
        pAdcCtrl->subs[SUBS_MIC].sync_ep_num = USB_ADC_INSYNC_EP;       /* endpoint number */
    }

    /* initialize substreams - speaker and linein/mic */
    for (i = 0; i < MAX_SUBSTREAMS; i++) {
        /* align to 32 byte boundary before allocating TDs */
        while (param->mem_base & 0x1F) {
            (param->mem_base)++;
            (param->mem_size)--;
        }
        /* allocate transfer descriptors */
        pAdcCtrl->subs[i].head_TD = (DTD_T*)(param->mem_base);
        /* Zero out the device transfer descriptors */
        memset((void*)param->mem_base, 0, NUM_DTDS * sizeof(DTD_T));
        param->mem_base += (NUM_DTDS * sizeof(DTD_T));
        param->mem_size -= (NUM_DTDS * sizeof(DTD_T));
        /* allocate another TD for feedback ep associated with this substream */
        if ( sync_mode & ADC_ASYNC_MODE) {
            /* allocate transfer descriptors */
            pAdcCtrl->subs[i].sync_TD = (DTD_T*)(param->mem_base);
            /* Zero out the device transfer descriptors */
            memset((void*)param->mem_base, 0, sizeof(DTD_T));
            param->mem_base += sizeof(DTD_T);
            param->mem_size -= sizeof(DTD_T);
        }
        /* allocate  buffers each 1msec worth */
        pAdcCtrl->subs[i].buf = param->mem_base;
        pAdcCtrl->subs[i].buf_sz = (NUM_DTDS * pAdcCtrl->subs[i].maxp)/SAMPLE_SIZE;
        param->mem_base += (NUM_DTDS * pAdcCtrl->subs[i].maxp);
        param->mem_size -= (NUM_DTDS * pAdcCtrl->subs[i].maxp);
    }

    /* register ep0 handler */
    ret = USBD_API->core->RegisterClassHandler(hUsb, ADC_ep0_hdlr, pAdcCtrl);
    if (ret == LPC_OK) {

        /* register ISO OUT endpoint interrupt handler */
        ep_indx = ((USB_ADC_OUT_EP & 0x0F) << 1);
        ret = USBD_API->core->RegisterEpHandler (hUsb, ep_indx, ADC_iso_out_hdlr, &pAdcCtrl->subs[SUBS_SPEAKER]);
        /* register ISO IN endpoint interrupt handler */
        ep_indx = ((USB_ADC_IN_EP & 0x0F) << 1) + 1;
        ret = USBD_API->core->RegisterEpHandler (hUsb, ep_indx, ADC_iso_in_hdlr, &pAdcCtrl->subs[SUBS_MIC]);
        /* register Synch endpoint interrupt handlers in Async mode */
        if (sync_mode & ADC_ASYNC_MODE) {
            /* register endpoint interrupt handler for SyncEP associated with ISO OUT ep*/
            ep_indx = ((USB_ADC_OUTSYNC_EP & 0x0F) << 1) + 1;
            ret = USBD_API->core->RegisterEpHandler (hUsb, ep_indx, ADC_out_sync_hdlr, &pAdcCtrl->subs[SUBS_SPEAKER]);
            /* register endpoint interrupt handler for SyncEP associated with ISO IN ep*/
            ep_indx = ((USB_ADC_INSYNC_EP & 0x0F) << 1);
            ret = USBD_API->core->RegisterEpHandler (hUsb, ep_indx, ADC_in_sync_hdlr, &pAdcCtrl->subs[SUBS_MIC]);
        }
    }
    return ret;
}

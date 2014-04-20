/***********************************************************************
* $Id:: adc_user.h
*
* Project: USB Audio class
 *
 * Description: USB Audio Device Class Custom User Module
 *
 * Copyright(C) 2011, NXP Semiconductor
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
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
 **********************************************************************/
#include "usbd_config.h"
#include "usbd/error.h"
#include "usbd/usbd.h"
#include "usbd/usbd_core.h"
#include "usbd/usbd_rom_api.h"
#include "usbd/hw_usbd.h"
#include "adc_def.h"

#ifndef __ADC_USER_H__
#define __ADC_USER_H__


#define VOLUME_MIN          0x0000
#define VOLUME_MAX          0x003F
#define VOLUME_RES          0x0001
#define VOLUME_DEF          0x003D      /* default valume level */
#define DEF_SAMPLE_RATE     48000       /* default sample rate */
#define DEF_SYNC_VALUE      (48 << 14)  /* Samples per msec in 10.14 format */
#define DEF_MAX_PKT_SZ      (4 * 48)
#define NUM_DTDS            8           /* should always be 2^n value */
#define SAMPLE_SIZE         4           /* 4 bytes 16 bit per channel */
#define SILENCE_VAL         0x00000000  /* silence playback value */

/* controller flags */
#define ADC_PLAY_MUTE       0x0001
#define ADC_PLAY_DATAVALID  0x0002
#define ADC_PLAYING         0x0008
#define ADC_REC_DATAVALID   0x0010
#define ADC_RECORDING       0x0080  
#define ADC_ASYNC_MODE      0x8000
/* substream types */
#define SUBS_SPEAKER        0
#define SUBS_MIC            1
#define MAX_SUBSTREAMS      2   
/* PLL change direction defines */
#define PLL_CHANGE_NON      0 
#define PLL_CHANGE_INC      1 
#define PLL_CHANGE_DEC      2 
#define FRAC_MIN_STEP       _BIT(0)
/* ADC USB stream mode defines */
#define ADC_USB_WRITER      0
#define ADC_USB_READER      1


/* structure to store Sub-stream control  data */ 
typedef struct _ADC_SUBSTREAM_T
{
    DQH_T* ep_QH;           /* EP queue head associated with stream*/
    DTD_T* head_TD;         /* head of TD list */
    DTD_T* tail_TD;         /* tail of TD list*/
    uint32_t sample_rate;   /* stream rate */
    uint32_t maxp;          /* Max packet size */
    uint32_t buf;           /* stream buffer */

    uint16_t buf_sz;        /* stream buffer size */
    uint8_t  wr_idx;        /* writer index */ 
    uint8_t  wr_valid_idx;  /* index+1 of TD from where we started reciving valid data*/
    uint16_t rd_idx;        /* reader index */ 
    uint16_t i2s_pos;       /* current i2s position stream buffer*/
    uint32_t ep_bit;        /* bit mask for the associated EP in control regs */
    uint32_t ep_num;        /* endpoint number per USB spec */
    /* following variables are used in Asynchrounous mode */
    DQH_T* sync_ep_QH;      /* EP queue head associated with sync endpoint*/
    DTD_T* sync_TD;         /* TD for feedback sync EP */
	uint32_t sync_data;     /* feedback rate data. Only lower 3 bytes are valid */
    uint32_t sync_ep_bit;   /* bit mask for the associated EP in control regs */
    uint32_t sync_ep_num;   /* endpoint number per USB spec */
    int32_t delta_samples; /* Average fractional smaples left to be transfered */

    uint32_t wr_count;      /* sample received/written to the buffer */
    uint32_t rd_count;      /* rendered sample count*/
} ADC_SUBSTREAM_T;

/* Structure to store Audio class driver control data */
typedef struct _ADC_CTRL_T
{
    USB_CORE_CTRL_T*  pUsbCtrl; /* handle to ROM stack */
    USB_OTG_REGS_T* pRegs;      /* USB registers pointer */
    ADC_SUBSTREAM_T subs[2];    /* pointers to sub-stream strcutres */
    uint16_t curr_vol;          /* Volume Current Value */
    uint16_t flags;             /* controller state flags */
    uint32_t sof_count;         /* SOF counter to track rendered sample count */
	/* following variables are used in Synchronous mode */
    int32_t total_diff;         /* rolling average of clock difference between source & sink*/
    uint32_t cur_frac;          /* current fraction divider  */
    uint16_t prev_step;         /* previous adjustment step size */
    uint16_t prev_dir;          /* Direction in which we changed the step size */
    uint32_t dir_count;         /* Number of consecutive time we wer requested to change
								   clock in same direction */

    /* I2S get next pointer callback routines */
    uint32_t* (*i2s_next_wr) (void* pAdcCtrl, ADC_SUBSTREAM_T* pSubs);

} USB_ADC_CTRL_T;

/* structure to hold USB handle to ADC control lookup table */
 typedef struct _ADC_USBH_2_CTRL_T
{
    USBD_HANDLE_T hUsb;
    USB_ADC_CTRL_T* pAdcCtrl;
    
} ADC_USBH_2_CTRL_T;



/* exported data */
extern ADC_USBH_2_CTRL_T g_usbh_2_adcctrl[2];
/* inline routines/macros to get audio control associated with USB instance */
__inline USB_ADC_CTRL_T* GET_ADC_CTRL(USBD_HANDLE_T hUsb) 
{
    if ( hUsb == g_usbh_2_adcctrl[1].hUsb)
        return g_usbh_2_adcctrl[1].pAdcCtrl;

    return g_usbh_2_adcctrl[0].pAdcCtrl;
}     


/* exported routines */
extern ErrorCode_t ADC_init(USBD_HANDLE_T hUsb, USBD_API_INIT_PARAM_T* param, 
    USB_ADC_CTRL_T** ppAdcCtrl, uint32_t sync_mode);
extern void ADC_start_xfr(USB_ADC_CTRL_T* pAdcCtrl, ADC_SUBSTREAM_T* pSubs, uint32_t mode);
extern void ADC_stop_xfr(USB_ADC_CTRL_T* pAdcCtrl, ADC_SUBSTREAM_T* pSubs, uint32_t mode);
extern ErrorCode_t ADC_Interface_Event (USBD_HANDLE_T hUsb);
extern ErrorCode_t ADC_Reset_Event (USBD_HANDLE_T hUsb);

/* synchronization mode routines */
extern ErrorCode_t ADC_SyncSOF_Event (USBD_HANDLE_T hUsb);
extern ErrorCode_t ADC_ASyncSOF_Event (USBD_HANDLE_T hUsb);
uint32_t* Codec_Sync_next_wr(void* ctrl, ADC_SUBSTREAM_T* pSubs);
uint32_t* Codec_Async_next_wr(void* ctrl, ADC_SUBSTREAM_T* pSubs);

/* connector routines */
extern void Codec_record(USB_ADC_CTRL_T* pAdcCtrl, uint32_t enable);
extern void Codec_play(USB_ADC_CTRL_T* pAdcCtrl, uint32_t enable);
extern void Codec_set_volume(USB_ADC_CTRL_T* pAdcCtrl, uint32_t volume);
extern void Codec_IRQHandler(USB_ADC_CTRL_T* pAdcCtrl);



#endif  /* __ADC_USER_H__ */

/***********************************************************************
* $Id:: adc_user.c 165 2011-04-14 17:41:11Z usb10131                   $
*
* Project: USB audio streaming example project.
*
* Description:
*     USB Audio Device Class Custom User Module.
*
*  Revision History:
*
*  09/22/11 -- v0.0.1  
*          Initial version
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
#include "config.h"
#include "lpc43xx_scu.h"
#include "board.h"
#include "usbd_config.h"
#include "adc_user.h"
#include "codec.h"  /* codec board header files */
#include "sgpio_i2s.h"


//#define SYNC_MODE

#ifndef _BIT
#define _BIT(n) (((uint32_t)(1ul)) << (n))
#endif

#if defined(__DEBUG_FLASH) || defined(__DEBUG_RAM)
extern void DebugInit(LPC_USARTn_Type* pRegs, uint32_t baudrate, uint32_t src_clk);
#endif


/*----------------------------------------------------------------------------
 ** Extern global data 
 *----------------------------------------------------------------------------*/
extern uint8_t USB_DeviceDescriptor[];
extern uint8_t ADC_FsSyncConfigDescriptor[];
extern uint8_t ADC_FsAsyncConfigDescriptor[];
extern uint8_t USB_StringDescriptor[];
extern uint8_t USB_DeviceQualifier[];

/*----------------------------------------------------------------------------
 ** Function prototypes
 *----------------------------------------------------------------------------*/
static void vCatchError(uint8_t u8Error);

/*----------------------------------------------------------------------------
 ** Global data 
 *----------------------------------------------------------------------------*/
//volatile uint32_t g_sys_ticks = 0;
ADC_USBH_2_CTRL_T g_usbh_2_adcctrl[2];
uint32_t g_usb0_configured = 0;
uint32_t g_usb1_configured = 0;
USB_ADC_CTRL_T* g_i2s_ctrl = 0;


/*----------------------------------------------------------------------------
  Configure GIMA to capture SOF and WS counters
 *----------------------------------------------------------------------------*/
void init_ws_cntrs(void)
{
#if 0
    /* use Timer3_cap2 to count SOF on USB0 */
    LPC_GIMA->CAP3_2_IN = (0x2 << 4)    /* SOF0 */
              //  | _BIT(3) // pulse
              //  | _BIT(2) // Synch
              //  | _BIT(1) // edge detect
              //  | _BIT(0) // invert
                ;
#else
    /* use Timer3_cap1 to count I2S_TX_MWS  */
    LPC_GIMA->CAP3_1_IN = (0x2 << 4)    /* I2S_TX_MWS */
                | _BIT(3) // pulse
                | _BIT(2) // Synch
                | _BIT(1) // edge detect
                | _BIT(0) // invert
                ;
#endif
    /* configure Timer3 to count SOF */
	LPC_TIMER3->IR = 0x0F;          /* Clear MATx interrupt include DMA request */ 
	LPC_TIMER3->PR  = 0; 
	LPC_TIMER3->MR[0] = 0; 
	LPC_TIMER3->MR[1] = 0; 
	LPC_TIMER3->CCR = 0;
    LPC_TIMER3->CTCR = 0x1 | (0x2 << 2);
    LPC_TIMER3->TCR =  0x1;
}


ErrorCode_t USB0_Config_Event (USBD_HANDLE_T hUsb)
{
    g_usb0_configured = 1;
    if (g_i2s_ctrl == 0)
        g_i2s_ctrl = g_usbh_2_adcctrl[0].pAdcCtrl;
    return LPC_OK;
}

ErrorCode_t USB1_Config_Event (USBD_HANDLE_T hUsb)
{
    g_usb1_configured = 1;
    if (g_i2s_ctrl == 0)
        g_i2s_ctrl = g_usbh_2_adcctrl[1].pAdcCtrl;
    return LPC_OK;
}





/*----------------------------------------------------------------------------
  Main routine
 *----------------------------------------------------------------------------*/
int sound_main (void) 
{
    USBD_API_INIT_PARAM_T usb_param;
    USB_CORE_DESCS_T desc;
    ErrorCode_t ret = LPC_OK;
    
    setupSGPIO();

    /* initilize call back structures */
    memset((void*)&usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
    usb_param.usb_reg_base = LPC_USB0_BASE;
    usb_param.max_num_ep = 6;
    usb_param.mem_base = 0x20000000;
    usb_param.mem_size = 0x2000;
    usb_param.USB_Interface_Event = ADC_Interface_Event;
    usb_param.USB_SOF_Event = ADC_ASyncSOF_Event;
    usb_param.USB_Reset_Event = ADC_Reset_Event;
    usb_param.USB_Configure_Event = USB0_Config_Event;

    /* Set the USB descriptors */
    desc.device_desc = USB_DeviceDescriptor;
    desc.string_desc = USB_StringDescriptor;
    desc.full_speed_desc = desc.high_speed_desc = ADC_FsAsyncConfigDescriptor; 
    desc.device_qualifier = USB_DeviceQualifier;

    /* USB Initialization */
    ret = USBD_API->hw->Init(&g_usbh_2_adcctrl[0].hUsb, &desc, &usb_param);  
    if (ret == LPC_OK) {

        /* init audio class controller */
        ret = ADC_init(g_usbh_2_adcctrl[0].hUsb, &usb_param, 
            &g_usbh_2_adcctrl[0].pAdcCtrl, ADC_ASYNC_MODE);

        if (ret == LPC_OK) {

            NVIC_EnableIRQ(USB0_IRQn); //  enable USB0 interrrupts 
            /* now connect */
            USBD_API->hw->Connect(g_usbh_2_adcctrl[0].hUsb, 1); 
        }
    }
    /* now init USB1 in Sync mode */
    if ( ret == LPC_OK) {

        /* initilize call back structures */
        usb_param.usb_reg_base = LPC_USB1_BASE;
        usb_param.max_num_ep = 4;
        usb_param.mem_base = 0x20002000;
        usb_param.mem_size = 0x2000;
        usb_param.USB_SOF_Event = ADC_SyncSOF_Event;
        usb_param.USB_Reset_Event = ADC_Reset_Event;
        usb_param.USB_Configure_Event = USB1_Config_Event;
    
        /* Set the USB descriptors */
        desc.full_speed_desc = desc.high_speed_desc = ADC_FsSyncConfigDescriptor;
    
        /* USB Initialization */
        ret = USBD_API->hw->Init(&g_usbh_2_adcctrl[1].hUsb, &desc, &usb_param);  
        if (ret == LPC_OK) {
    
            /* init audio class controller */
            ret = ADC_init(g_usbh_2_adcctrl[1].hUsb, &usb_param, 
                &g_usbh_2_adcctrl[1].pAdcCtrl, 0);
    
            if (ret == LPC_OK) {
    
                NVIC_EnableIRQ(USB1_IRQn); //  enable USB1 interrrupts 
                /* now connect */
                USBD_API->hw->Connect(g_usbh_2_adcctrl[1].hUsb, 1); 
            }
    
        }
    }
                           
    if (ret != LPC_OK) {
        vCatchError(1); //"\r\nhwUSB_Init error!!!"
    }   
    
	return 0;		// happy now??    
}

/*----------------------------------------------------------------------------
  System tick handler
 *----------------------------------------------------------------------------*/
//void SysTick_Handler (void)                     
//{ 
//    g_sys_ticks++;
//}


/*----------------------------------------------------------------------------
  IRQ handler redirectors
 *----------------------------------------------------------------------------*/
void USB0_IRQHandler(void)
{
//    if (g_usb1_configured == 0)
        USBD_API->hw->ISR(g_usbh_2_adcctrl[0].hUsb);
}

void USB1_IRQHandler(void)
{
//    if (g_usb0_configured == 0)
        USBD_API->hw->ISR(g_usbh_2_adcctrl[1].hUsb);
}


void SGPIO_IRQHandler(void)
{
    SGPIO_Codec_IRQHandler(g_i2s_ctrl);       
}

/*----------------------------------------------------------------------------
  Error handler
 *----------------------------------------------------------------------------*/
static void vCatchError(uint8_t u8Error)
{
    volatile uint8_t u8theError = u8Error;

    while(1);
}


/**********************************************************************
 **                            End Of File
 **********************************************************************/

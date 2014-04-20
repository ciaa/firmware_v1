/**********************************************************************
* $Id$		usbd_hid.c			2011-06-02
*//**
* @file		usbd_hid.c
* @brief	USB HID example project.
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
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
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "usbd/usbd_hid.h"
#include "usbd/usbd_rom_api.h"
#include "usbd/usbd_hiduser.h"

extern uint32_t copy_hid_descriptors(uint8_t** pDesc, uint32_t* mem_base, uint32_t* mem_size);


static uint8_t* loopback_report;

ErrorCode_t USB_Configure_Event (USBD_HANDLE_T hUsb) {

  USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;
  if (pCtrl->config_value) {                   /* Check if USB is configured */
    USBD_API->hw->WriteEP(hUsb, HID_EP_IN, loopback_report, 1);
  }
  return LPC_OK;
}

/*
 *  HID Get Report Request Callback
 *   Called automatically on HID Get Report Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

ErrorCode_t HID_GetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* plength)
{
  /* ReportID = SetupPacket.wValue.WB.L; */
  switch (pSetup->wValue.WB.H) {
    case HID_REPORT_INPUT:
      *pBuffer[0] = *loopback_report;
      *plength = 1;
      break;
    case HID_REPORT_OUTPUT:
      return (ERR_USBD_STALL);          /* Not Supported */
    case HID_REPORT_FEATURE:
      /* EP0Buf[] = ...; */
      /* break; */
      return (ERR_USBD_STALL);          /* Not Supported */
  }
  return (LPC_OK);
}


/*
 *  HID Set Report Request Callback
 *   Called automatically on HID Set Report Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */
ErrorCode_t HID_SetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length)
{
  /* we will reuse standard EP0Buf */
  if (length == 0)
    return LPC_OK;
  /* ReportID = SetupPacket.wValue.WB.L; */
  switch (pSetup->wValue.WB.H) {
    case HID_REPORT_INPUT:
      return (ERR_USBD_STALL);          /* Not Supported */
    case HID_REPORT_OUTPUT:
      *loopback_report = **pBuffer;
      break;
    case HID_REPORT_FEATURE:
      return (ERR_USBD_STALL);          /* Not Supported */
  }
  return (LPC_OK);
}

/*
 *  USB Endpoint 1 Event Callback
 *   Called automatically on USB Endpoint 1 Event
 *    Parameter:       event
 */

ErrorCode_t HID_EpIn_Hdlr (USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
  USB_HID_CTRL_T* pHidCtrl = (USB_HID_CTRL_T*)data;

  switch (event) {
    case USB_EVT_IN:
      USBD_API->hw->WriteEP(hUsb, pHidCtrl->epin_adr, loopback_report, 1);
      break;
  }
  return LPC_OK;
}

/* Main Program */
ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
  USBD_HID_INIT_PARAM_T hid_param;
  USB_HID_REPORT_T reports_data[1];
  ErrorCode_t ret = LPC_OK;


  memset((void*)&hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
  /* HID paramas */
  hid_param.max_reports = 1;
  /* Init reports_data */
  reports_data[0].len = copy_hid_descriptors(&reports_data[0].desc, mem_base, mem_size);
  reports_data[0].idle_time = 0;

  if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE))
    return ERR_FAILED;

  hid_param.mem_base = *mem_base;
  hid_param.mem_size = *mem_size;
  hid_param.intf_desc = (uint8_t*)pIntfDesc;
  /* user defined functions */
  hid_param.HID_GetReport = HID_GetReport;
  hid_param.HID_SetReport = HID_SetReport;
  hid_param.HID_EpIn_Hdlr = HID_EpIn_Hdlr;
  hid_param.report_data  = reports_data;

  ret = USBD_API->hid->init(hUsb, &hid_param);
  /* allocate USB accessable memory space for report data */
  loopback_report =  (uint8_t*)hid_param.mem_base;
  hid_param.mem_base += 4;
  hid_param.mem_size += 4;
  /* update memory variables */
  *mem_base = hid_param.mem_base;
  *mem_size = hid_param.mem_size;
  return ret;
}



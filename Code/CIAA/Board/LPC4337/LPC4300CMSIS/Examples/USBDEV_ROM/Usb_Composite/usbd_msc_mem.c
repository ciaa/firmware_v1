/**********************************************************************
* $Id$		usbd_msc_mem.c			2011-06-02
*//**
* @file		usbd_msc_mem.c
* @brief	USB mass storage functions for composite device example project.
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
#include "usbd/usbd_msc.h"
#include "usbd/usbd_rom_api.h"

/* MSC Disk Image Definitions */
/* Mass Storage Memory Layout */
#define MSC_BlockSize       512
#define MSC_MemorySize      ((uint32_t)(32 * 1024))
#define MSC_BlockCount      (MSC_MemorySize / MSC_BlockSize)



const uint8_t InquiryStr[] = {'N','X','P',' ',' ',' ',' ',' ',     \
                           'L','P','C',' ','M','e','m',' ',     \
                           'D','i','s','k',' ',' ',' ',' ',     \
                           '1','.','0',' ',};


uint8_t* Memory = (uint8_t*)0x20004000;

void translate_rd( uint32_t offset, uint8_t** buff_adr, uint32_t length)
{
  //memcpy((void*)*buff_adr, (void*)&Memory[offset], length);
  *buff_adr =  &Memory[offset];
}

void translate_wr( uint32_t offset, uint8_t** buff_adr, uint32_t length)
{
  //memcpy((void*)&Memory[offset], *src, length);
  *buff_adr =  &Memory[offset + length];
}
void translate_GetWrBuf( uint32_t offset, uint8_t** buff_adr, uint32_t length)
{
  //memcpy((void*)&Memory[offset], *src, length);
  *buff_adr =  &Memory[offset];
}

ErrorCode_t translate_verify( uint32_t offset, uint8_t* src, uint32_t length)
{
  if (memcmp((void*)&Memory[offset], src, length))
    return ERR_FAILED;

  return LPC_OK;
}

/* Main Program */

ErrorCode_t usb_msc_mem_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
  USBD_MSC_INIT_PARAM_T msc_param;
  ErrorCode_t ret = LPC_OK;

  memset((void*)&msc_param, 0, sizeof(USBD_MSC_INIT_PARAM_T));
  msc_param.mem_base = *mem_base;
  msc_param.mem_size = *mem_size;
  /* mass storage paramas */
  msc_param.InquiryStr = (uint8_t*)InquiryStr;
  msc_param.BlockCount = MSC_MemorySize / MSC_BlockSize;
  msc_param.BlockSize = MSC_BlockSize;
  msc_param.MemorySize = MSC_MemorySize;

  if ((pIntfDesc == 0) ||
    (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_STORAGE) ||
        (pIntfDesc->bInterfaceSubClass != MSC_SUBCLASS_SCSI) )
    return ERR_FAILED;

  msc_param.intf_desc = (uint8_t*)pIntfDesc;
  /* user defined functions */
  msc_param.MSC_Write = translate_wr;
  msc_param.MSC_Read = translate_rd;
  msc_param.MSC_Verify = translate_verify;
  msc_param.MSC_GetWriteBuf = translate_GetWrBuf;


  ret = USBD_API->msc->init(hUsb, &msc_param);
  /* update memory variables */
  *mem_base = msc_param.mem_base;
  *mem_size = msc_param.mem_size;

  return ret;
}


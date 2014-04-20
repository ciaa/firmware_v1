/**********************************************************************
* $Id$		usbd_dfu.c			2011-06-02
*//**
* @file		usbd_dfu.c
* @brief	USB DFU functions for composite device example project.
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
#include "usbd/usbd_dfu.h"
#include "usbd/usbd_rom_api.h"

/* DFU boot definitions */
#define DFU_DEST_BASE         0x20004000//0x10080000
#define DFU_MAX_IMAGE_LEN     (32 * 1024)
#define DFU_XFER_BLOCK_SZ     (2048)
#define DFU_MAX_BLOCKS        (DFU_MAX_IMAGE_LEN/DFU_XFER_BLOCK_SZ)

extern void dfu_detach(USBD_HANDLE_T hUsb);


/* return
* DFU_STATUS_ : values defined in usbd/usbd_dfu.h in case of errors
* 0 : If end of memory reached
* length : Amount of data copied to destination buffer
*/
uint32_t dfu_rd( uint32_t block_num, uint8_t** pBuff, uint32_t length)
{
  uint32_t src_addr = DFU_DEST_BASE;

  if (block_num == DFU_MAX_BLOCKS)
    return 0;

  if (block_num > DFU_MAX_BLOCKS)
    return DFU_STATUS_errADDRESS;

  src_addr += (block_num * DFU_XFER_BLOCK_SZ);
  *pBuff = (uint8_t*)src_addr;
  //memcpy((void*)src, (void*)src_addr, length);

  return length;
}

/* return DFU_STATUS_ values defined in usbd/usbd_dfu.h */
uint8_t dfu_wr( uint32_t block_num, uint8_t** pBuff, uint32_t length)
{
  uint32_t dest_addr = DFU_DEST_BASE;

  if (block_num >= DFU_MAX_BLOCKS)
    return DFU_STATUS_errADDRESS;

  dest_addr += (block_num * DFU_XFER_BLOCK_SZ);
  dest_addr += length;
  *pBuff = (uint8_t*)dest_addr;

  //memcpy((void*)dest_addr, src, length);

  return DFU_STATUS_OK;
}

void dfu_done(void)
{

}

/* Main Program */
ErrorCode_t usb_dfu_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
  USBD_DFU_INIT_PARAM_T dfu_param;
  ErrorCode_t ret = LPC_OK;

  memset((void*)&dfu_param, 0, sizeof(USBD_DFU_INIT_PARAM_T));
  dfu_param.mem_base = *mem_base;
  dfu_param.mem_size = *mem_size;
  /* DFU paramas */
  dfu_param.wTransferSize = 2048;

  if ((pIntfDesc == 0) ||
    (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_APP) ||
        (pIntfDesc->bInterfaceSubClass != USB_DFU_SUBCLASS) )
    return ERR_FAILED;

  dfu_param.intf_desc = (uint8_t*)pIntfDesc;
  /* user defined functions */
  dfu_param.DFU_Write = dfu_wr;
  dfu_param.DFU_Read = dfu_rd;
  dfu_param.DFU_Done = dfu_done;
  dfu_param.DFU_Detach = dfu_detach;

  ret = USBD_API->dfu->init(hUsb, &dfu_param);
  /* update memory variables */
  *mem_base = dfu_param.mem_base;
  *mem_size = dfu_param.mem_size;
  return ret;
}



/**********************************************************************
* $Id$		usbd_rom_api.h				2011-06-02
*//**
* @file		usbd_rom_api.h
* @brief	USB base API structure definitions.
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
**********************************************************************/
#ifndef __MW_USBD_ROM_API_H
#define __MW_USBD_ROM_API_H
/** \file
 *  \brief ROM API for USB device stack.
 *
 *  Definition of functions exported by ROM based USB device stack.
 *
 */

#include "error.h"
#include "usbd.h"
#include "usbd_hw.h"
#include "usbd_core.h"
#include "usbd_mscuser.h"
#include "usbd_dfuuser.h"
#include "usbd_hiduser.h"

/** \brief Main USBD API functions structure.
 *  \ingroup Group_USBD
 *
 *  This structure contains pointer to various USB Device stack's sub-module
 *  function tables. This structure is used as main entry point to access
 *  various methods (grouped in sub-modules) exposed by ROM based USB device
 *  stack.
 *
 */
typedef struct USBD_API
{
  const USBD_HW_API_T* hw; /**< Pointer to function table which exposes functions
                           which interact directly with USB device stack's core
                           layer.*/
  const USBD_CORE_API_T* core; /**< Pointer to function table which exposes functions
                           which interact directly with USB device controller
                           hardware.*/
  const USBD_MSC_API_T* msc; /**< Pointer to function table which exposes functions
                           provided by MSC function driver module.
                           */
  const USBD_DFU_API_T* dfu; /**< Pointer to function table which exposes functions
                           provided by DFU function driver module.
                           */
  const USBD_HID_API_T* hid; /**< Pointer to function table which exposes functions
                           provided by HID function driver module.
                           */
} USBD_API_T;

/* A table of pointers to the USBD functions contained in ROM is located at the
   address contained at this location */
#define USBD_FUNCTION_TABLE_PTR_ADDR			(0x1040011CUL)

#define USBD_API (*((USBD_API_T**)USBD_FUNCTION_TABLE_PTR_ADDR))


#endif /*__MW_USBD_ROM_API_H*/


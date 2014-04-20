/***********************************************************************
 * $Id: usbd_config.h 8018 2011-08-31 16:19:55Z usb10131 $
 *
 * Project: USB application config
 *
 * Description:
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
#ifndef __USBD_CONFIG_H
#define __USBD_CONFIG_H


/**********************************************************************
 ** Board selection
**********************************************************************/
//#define USE_NXP_EVAL

/**********************************************************************
 ** USB defines
**********************************************************************/
#define CURR_USB_PORT    LPC_USB0_BASE
#define USB1_ULPI_PHY   1

#define USB_MAX_IF_NUM  8
#define USB_MAX_EP_NUM  6
#define USB_MAX_PACKET0 64
/* Max In/Out Packet Size */
#define USB_FS_MAX_BULK_PACKET  64
#define USB_HS_MAX_BULK_PACKET  512
#define MSC_EP_IN   0x81
#define MSC_EP_OUT  0x01

/* MSC Disk Image Definitions */
/* Mass Storage Memory Layout */
#define MSC_BlockSize       512
#define MSC_MemorySize      ((uint32_t)(32 * 1024)) 
#define MSC_BlockCount      (MSC_MemorySize / MSC_BlockSize)

#endif /* end __USBD_CONFIG_H */

/**********************************************************************
 **                            End Of File
 **********************************************************************/

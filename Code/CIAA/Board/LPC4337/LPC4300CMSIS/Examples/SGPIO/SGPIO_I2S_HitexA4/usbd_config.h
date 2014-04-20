
 /***********************************************************************
* $Id:: usbd_config.h
*
* Project: USB Audio class
 *
 * Description: USB Audio Device Class configuration
 *
 * Copyright(C) 2012, NXP Semiconductor
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
#ifndef __USBD_CONFIG_H
#define __USBD_CONFIG_H

/**********************************************************************
 ** USB defines
**********************************************************************/
#define CURR_USB_PORT    		LPC_USB0_BASE

#define USB_MAX_IF_NUM  		8
#define USB_MAX_EP_NUM  		6
#define USB_MAX_PACKET0 		64
#define USB_ADC_SYNC_MAXP       3
/* Max In/Out Packet Size */
#define USB_FS_MAX_BULK_PACKET  64
#define USB_HS_MAX_BULK_PACKET  512

/* interface numbers */
#define USB_ADC_CIF_NUM     	0
#define USB_ADC_SIF1_NUM    	1
#define USB_ADC_SIF2_NUM    	2
/* streaming endpoints */
#define USB_ADC_IN_EP      		0x81
#define USB_ADC_OUT_EP     		0x02
/* Audio streaming interface endpoints */
#define USB_ADC_INSYNC_EP   	0x01
#define USB_ADC_OUTSYNC_EP  	0x82
#define USB_ADC_SYNC_REFRESH  	8    /* should be atleast 6 or more due to range calculate used in algo. */


#endif /* end __USBD_CONFIG_H */

/**********************************************************************
 **                            End Of File
 **********************************************************************/

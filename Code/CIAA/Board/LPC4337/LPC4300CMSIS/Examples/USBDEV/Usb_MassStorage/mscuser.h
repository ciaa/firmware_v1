/**********************************************************************
* $Id$		mscuser.h				2011-06-02
*//**
* @file		mscuser.h
* @brief	Mass Storage Class Custom User Definitions
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
#ifndef __MSCUSER_H__
#define __MSCUSER_H__


/* Mass Storage Memory Layout */
/* MSC Disk Image Definitions */
/* Mass Storage Memory Layout */
#define DIR_ENTRY           32
#define BLOCKS_PER_CLUSTER  16
#define MSC_BlockSize       512
#define BOOT_SECT_SIZE 	    MSC_BlockSize
#define ROOT_DIR_SIZE       (MSC_BlockSize * 1)
#define FAT_SIZE	        (MSC_BlockSize * 2)

#define MSC_ImageSize	    (BOOT_SECT_SIZE + FAT_SIZE + ROOT_DIR_SIZE + 12*MSC_BlockSize)
#define MSC_MemorySize    MSC_ImageSize
#define MSC_BlockCount    (MSC_MemorySize / MSC_BlockSize)
#define NO_OF_CLUSTERS 	  (MSC_BlockCount/BLOCKS_PER_CLUSTER)


/* Max In/Out Packet Size */
#define MSC_FS_MAX_PACKET  64
#define MSC_HS_MAX_PACKET  224 /* < 256 work */

/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x81
#define MSC_EP_OUT      0x01

//extern uint8_t*  Memory;  /* MSC RAM */
extern uint8_t DiskImage[MSC_ImageSize];  /* Initial fat table & MBR */

/* MSC Requests Callback Functions */
extern uint32_t MSC_Reset(void);
extern uint32_t MSC_GetMaxLUN (void);

/* MSC Bulk Callback Functions */
extern void MSC_GetCBW (void);
extern void MSC_SetCSW (void);
extern void MSC_BulkIn (void);
extern void MSC_BulkOut(void);
extern void MSC_BulkOutNak(void);

#endif  /* __MSCUSER_H__ */

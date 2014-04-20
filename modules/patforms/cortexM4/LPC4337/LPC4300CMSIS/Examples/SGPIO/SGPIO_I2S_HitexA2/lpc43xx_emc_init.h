/***********************************************************************
* $Id: lpc43xx_emc_init.h
*
* Project: USB audio device class demo
*
* Description: Initializes the memory controller for usage of external 
*               memory devices
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
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  
* This copyright, permission, and disclaimer notice must appear in all 
* copies of this code.
**********************************************************************/


void MemoryPinInit(void);
void EMCFlashInit(void);

/* SDRAM Address Base for DYCS0*/
#define SDRAM_BASE_ADDR 0x28000000
#define FLASH_BASE_ADDR 0x1C000000

#define EMC_SDRAM_WIDTH_8_BITS		0
#define EMC_SDRAM_WIDTH_16_BITS		1
#define EMC_SDRAM_WIDTH_32_BITS		2

#define EMC_SDRAM_SIZE_16_MBITS		0
#define EMC_SDRAM_SIZE_64_MBITS		1
#define EMC_SDRAM_SIZE_128_MBITS	2
#define EMC_SDRAM_SIZE_256_MBITS	3
#define EMC_SDRAM_SIZE_512_MBITS	4

#define EMC_SDRAM_DATA_BUS_16_BITS	0
#define EMC_SDRAM_DATA_BUS_32_BITS	1

#define EMC_B_ENABLE 					(1 << 19)
#define EMC_ENABLE 						(1 << 0)
#define EMC_CE_ENABLE 					(1 << 0)
#define EMC_CS_ENABLE 					(1 << 1)
#define EMC_CLOCK_DELAYED_STRATEGY 		(0 << 0)
#define EMC_COMMAND_DELAYED_STRATEGY 	(1 << 0)
#define EMC_COMMAND_DELAYED_STRATEGY2 	(2 << 0)
#define EMC_COMMAND_DELAYED_STRATEGY3 	(3 << 0)
#define EMC_INIT(i) 					((i) << 7)
#define EMC_NORMAL 						(0)
#define EMC_MODE 						(1)
#define EMC_PRECHARGE_ALL 				(2)
#define EMC_NOP 						(3)






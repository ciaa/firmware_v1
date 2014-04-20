/**********************************************************************
* $Id$		SST39VF320.h			2011-06-02
*//**
* @file		SST39VF320.h
* @brief	Export function and macro for SST39VF320 Flash programming API
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

#include <lpc_types.h>
#define M8(adr)  (*((volatile uint8_t  *) (adr)))
#define M16(adr) (*((volatile uint16_t *) (adr)))
#define M32(adr) (*((volatile uint32_t  *) (adr)))


#define STATUS_POLLING	(1<<7)
#define STATUS_TOGGLE	(1<<6)

// Flash Programming Functions
extern          void  Init        (uint32_t adr);
extern          void  EraseChip   (void);               // Erase complete Device
extern          void  EraseSector (uint32_t adr);  // Erase Sector Function
extern          uint32_t  ProgramPage (uint32_t adr,   // Program Page Function
									uint32_t sz,
                                  uint8_t *buf);

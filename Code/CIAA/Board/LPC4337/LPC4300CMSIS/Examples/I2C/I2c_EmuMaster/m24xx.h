/**********************************************************************
* $Id$		m24xx.h	 2012-05-15
*//**
* @file		m24xx.h
* @brief	Definitions to talk to a M24xx EEPROM using GPIOs
* @version	1.0
* @date		15. May 2012
* @author	NXP MCU SW Application Team
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
* is used in conjunction with NXP Semiconductors microcontrollers.  
* This copyright, permission, and disclaimer notice must appear in all 
* copies of this code.
**********************************************************************/
#ifndef __M24XX_H__
#define __M24XX_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

 
/** I2C address of the M24C64 (as 7-bit address). */
#define I2C_ADDRESS_M24C64     (0x56)
 

bool m24xx_eeprom_read (uint8_t start, uint8_t length, uint8_t value[]);


#ifdef __cplusplus
}
#endif

#endif

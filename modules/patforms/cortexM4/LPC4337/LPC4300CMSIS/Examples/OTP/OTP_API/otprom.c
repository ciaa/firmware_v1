/**********************************************************************
* $Id$		otprom.c		2011-08-02
*//**
* @file		otprom.c
* @brief	OTP functions in boot rom
* @version	1.0
* @date		02. August. 2011
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

#include "LPC43xx.h"
#include "otprom.h"

#define BOOTROM_BASE		0x10400100
#define OTP_API_TABLE_OFFSET	0x1

unsigned long *BOOTROM_API_TABLE;

uint32_t (* otp_ProgBootSrc)(bootSrc_e BootSrc);
uint32_t (* otp_ProgJTAGDis)(void);
uint32_t (* otp_ProgUSBID)(uint32_t ProductID, uint32_t VendorID);
uint32_t (* otp_ProgGP0)(uint32_t Data, uint32_t Mask);
uint32_t (* otp_ProgGP1)(uint32_t Data, uint32_t Mask);
uint32_t (* otp_ProgGP2)(uint32_t Data, uint32_t Mask);
uint32_t (* otp_ProgKey1)(uint8_t *key);
uint32_t (* otp_ProgKey2)(uint8_t *key);
uint32_t (* otp_GenRand)(void);

uint32_t otp_Init(void)
{
  uint32_t (* ROM_otp_Init)(void);

  BOOTROM_API_TABLE = *((unsigned long **)BOOTROM_BASE + OTP_API_TABLE_OFFSET);
  
  ROM_otp_Init      = (uint32_t (*)(void))BOOTROM_API_TABLE[0];
  otp_ProgBootSrc   = (uint32_t (*)(bootSrc_e BootSrc))BOOTROM_API_TABLE[1];
  otp_ProgJTAGDis   = (uint32_t (*)(void))BOOTROM_API_TABLE[2];
  otp_ProgUSBID     = (uint32_t (*)(uint32_t ProductID, uint32_t VendorID))BOOTROM_API_TABLE[3];
  otp_ProgGP0       = (uint32_t (*)(uint32_t Data, uint32_t Mask))BOOTROM_API_TABLE[7];
  otp_ProgGP0       = (uint32_t (*)(uint32_t Data, uint32_t Mask))BOOTROM_API_TABLE[8];
  otp_ProgGP0       = (uint32_t (*)(uint32_t Data, uint32_t Mask))BOOTROM_API_TABLE[9];
  otp_ProgKey1      = (uint32_t (*)(uint8_t *key))BOOTROM_API_TABLE[10];
  otp_ProgKey1      = (uint32_t (*)(uint8_t *key))BOOTROM_API_TABLE[11];
  otp_GenRand       = (uint32_t (*)(void))BOOTROM_API_TABLE[12];

  return ROM_otp_Init();
}


/**********************************************************************
* $Id$		SST39VF320.c			2011-06-02
*//**
* @file		SST39VF320.c
* @brief	Flash Programming Functions adapted for SST39VF3201 (16-bit Bus)
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
#include "SST39VF320.H"
#include "lpc43xx.h"
#include "lpc_types.h"
#include "lpc43xx_scu.h"

uint16_t flash_status;

uint32_t base_adr;

/*********************************************************************//**
 * @brief		Check if Program/Erase completed
 * @param[in]	adr  Block Start Address
 * @return 		None
 **********************************************************************/
void Polling (uint32_t adr) {
  uint32_t q6;

  /* Check Toggle Bit */
  do {
	flash_status = M16(adr);
    q6 = (flash_status & STATUS_TOGGLE);
    flash_status = M16(adr);
  } while ((flash_status & STATUS_TOGGLE) != q6);

}

/*********************************************************************//**
 * @brief		Initialize Flash
 * @param[in]	baseadr  Device Base Address
 * @return 		None
 **********************************************************************/
void Init (uint32_t baseadr) {
	base_adr = baseadr;

 	/* Set up EMC pin */
	scu_pinmux(	1	,	0	,	MD_PLN_FAST	,	2	);	//A5
	scu_pinmux(	1	,	1	,	MD_PLN_FAST	,	2	);	//A6
	scu_pinmux(	1	,	2	,	MD_PLN_FAST	,	2	);	//A7
	scu_pinmux(	1	,	3	,	MD_PLN_FAST	,	3	);	//OE
	scu_pinmux(	1	,	4	,	MD_PLN_FAST	,	3	);	//BLS0
	scu_pinmux(	1	,	5	,	MD_PLN_FAST	,	3	);	//CS0
	scu_pinmux(	1	,	6	,	MD_PLN_FAST	,	3	);	//WE
	scu_pinmux(	1	,	7	,	MD_PLN_FAST	,	3	);//D0
	scu_pinmux(	1	,	8	,	MD_PLN_FAST	,	3	);//D1
	scu_pinmux(	1	,	9	,	MD_PLN_FAST	,	3	);//D2
	scu_pinmux(	1	,	10	,	MD_PLN_FAST	,	3	);//D3
	scu_pinmux(	1	,	11	,	MD_PLN_FAST	,	3	);//D4
	scu_pinmux(	1	,	12	,	MD_PLN_FAST	,	3	);//D5
	scu_pinmux(	1	,	13	,	MD_PLN_FAST	,	3	);//D6
	scu_pinmux(	1	,	14	,	MD_PLN_FAST	,	3	);//D7
	scu_pinmux(	2	,	0	,	MD_PLN_FAST	,	2	);//A13
	scu_pinmux(	2	,	1	,	MD_PLN_FAST	,	2	);//A12
	scu_pinmux(	2	,	2	,	MD_PLN_FAST	,	2	);//A11
	scu_pinmux(	2	,	6	,	MD_PLN_FAST	,	2	);//A10
	scu_pinmux(	2	,	7	,	MD_PLN_FAST	,	3	);//A9
	scu_pinmux(	2	,	8	,	MD_PLN_FAST	,	3	);//A8
	scu_pinmux(	2	,	9	,	MD_PLN_FAST	,	3	);//A0
	scu_pinmux(	2	,	10	,	MD_PLN_FAST	,	3	);//A1
	scu_pinmux(	2	,	11	,	MD_PLN_FAST	,	3	);//A2
	scu_pinmux(	2	,	12	,	MD_PLN_FAST	,	3	);//A3
	scu_pinmux(	2	,	13	,	MD_PLN_FAST	,	3	);//A4
	scu_pinmux(	5	,	0	,	MD_PLN_FAST	,	2	);//D12
	scu_pinmux(	5	,	1	,	MD_PLN_FAST	,	2	);//D13
	scu_pinmux(	5	,	2	,	MD_PLN_FAST	,	2	);//D14
	scu_pinmux(	5	,	3	,	MD_PLN_FAST	,	2	);//D15
	scu_pinmux(	5	,	4	,	MD_PLN_FAST	,	2	);//D8
	scu_pinmux(	5	,	5	,	MD_PLN_FAST	,	2	);//D9
	scu_pinmux(	5	,	6	,	MD_PLN_FAST	,	2	);//D10
	scu_pinmux(	5	,	7	,	MD_PLN_FAST	,	2	);//D11
	scu_pinmux(	6	,	3	,	MD_PLN_FAST	,	3	);//CS1
	scu_pinmux(	6	,	4	,	MD_PLN_FAST	,	3	);//CAS
	scu_pinmux(	6	,	5	,	MD_PLN_FAST	,	3	);//RAS
	scu_pinmux(	6	,	6	,	MD_PLN_FAST	,	1	);//A15
	scu_pinmux(	6	,	7	,	MD_PLN_FAST	,	1	);//A15
	scu_pinmux(	6	,	8	,	MD_PLN_FAST	,	1	);//A14
	scu_pinmux(	6	,	9	,	MD_PLN_FAST	,	3	);//DYCS0
	scu_pinmux(	6	,	10	,	MD_PLN_FAST	,	3	);//DQMOUT1
	scu_pinmux(	6	,	11	,	MD_PLN_FAST	,	3	);//CKEOUT0
	scu_pinmux(	6	,	12	,	MD_PLN_FAST	,	3	);//DQMOUT0
	scu_pinmux(	10	,	4	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	13	,	0	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	2	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	3	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	4	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	5	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	6	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	7	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	8	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	9	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	10	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	12	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	13	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	15	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	13	,	16	,	MD_PLN_FAST	,	2	);
	scu_pinmux(	14	,	0	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	1	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	2	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	3	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	4	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	5	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	6	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	7	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	8	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	9	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	10	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	11	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	12	,	MD_PLN_FAST	,	3	);
	scu_pinmux(	14	,	13	,	MD_PLN_FAST	,	3	);

	/* Set up EMC Controller */
	LPC_EMC->STATICWAITRD0 = 0x06;
	LPC_EMC->CONTROL = 0x01;
	LPC_EMC->STATICCONFIG0 = 0x81;
	LPC_EMC->STATICWAITOEN0 = 0x01;

    M32(0x40086400) = 3;
    M32(0x40086404) = 3;
    M32(0x40086408) = 3;
    M32(0x4008640C) = 3;
    M32(0x40086410) = 3;
    M32(0x40086414) = 3;
    M32(0x40086418) = 3;
    M32(0x4008641C) = 3;

    //M8(0x400F0080) = 0xFF;
    //M8(0x400F009C) = 0xFF;
}

/*********************************************************************//**
 * @brief		Erase all Nor Flash chip
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EraseChip (void) {

  /* Start Chip Erase Command */
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(base_adr + (0x5555 << 1)) = 0x80;
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(base_adr + (0x5555 << 1)) = 0x10;

  /* Waiting Completed Status */
  Polling(base_adr);
}

/*********************************************************************//**
 * @brief		Erase single sector
 * @param[in]	adr sector address
 * @return 		None
 **********************************************************************/
void EraseSector (uint32_t adr) {

  /* Start Erase Sector Command */
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(base_adr + (0x5555 << 1)) = 0x80;
  M16(base_adr + (0x5555 << 1)) = 0xAA;
  M16(base_adr + (0x2AAA << 1)) = 0x55;
  M16(adr) = 0x30;

  /* Waiting Completed Status */
  Polling(adr);
}

/*********************************************************************//**
 * @brief		Program Page in Nor Flash
 * @param[in]	adr	Page Start Address
 * @param[in]	sz	Page Size
 * @param[in]	buf	Page Data
 * @return 		None
 **********************************************************************/
uint32_t ProgramPage (uint32_t adr, uint32_t sz, uint8_t *buf) {
  int i;

  for (i = 0; i < ((sz+1)/2); i++)  {
    /* Start Program Command */
    M16(base_adr + (0x5555 << 1)) = 0xAA;
    M16(base_adr + (0x2AAA << 1)) = 0x55;
    M16(base_adr + (0x5555 << 1)) = 0xA0;
    M16(adr) = *((uint16_t *) buf);
    /* Wait word program complete */
	Polling(adr);
    buf += 2;
    adr += 2;
  }
  return (0);
}


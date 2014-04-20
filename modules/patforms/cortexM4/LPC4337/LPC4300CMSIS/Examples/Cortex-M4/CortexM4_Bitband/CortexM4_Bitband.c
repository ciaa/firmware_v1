/**********************************************************************
* $Id$		CortexM3_Bitband.c		2011-06-02
*//**
* @file		CortexM3_Bitband.c
* @brief	This example used to test bit-banding feature of Cortex-M3
* 			processor.
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
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup CortexM3_Bitband	CortexM3_Bitband
 * @ingroup Cortex_M3_Examples
 * @{
 */

/************************** PRIVATE DEFINTIONS*************************/
/* The processor memory map includes two bit-band regions. These occupy the lowest
 * 1MB of the SRAM and peripheral memory regions respectively.
 * + SRAM: 	Bit-band region: 	0x20000000 - 0x20100000
 * 			Bit-band alias:		0x22000000 - 0x23FFFFFF
 * + PERI:	Bit-band region:	0x40000000 - 0x40100000
 * 			Bit-band alias:		0x42000000 - 0x43FFFFFF
 * The mapping formula:
 * 		bit_word_offset = (byte_offset * 32) + (bit_number * 4)
 * 		bit_word_address = bit_band_base + bit_word_offset
 * where:
 * 	+ bit_word_offset: the position of the target bit in the bit-band memory region
 * 	+ bit_word_addr: the address of the word in the alias memory region that maps to the target bit
 *  + bit_band_base: the starting address of the alias region
 *  + byte_offset: the number of byte in the bit-band region that contains the targeted bit
 *  + bit_number: is the bit position (0-7) of the targeted bit
 *
 * Note: The fact, the AHB SRAM on lpc43xx just available in ranges:
 * 	+ 0x20000000 - 0x2000FFFF: for AHB SRAM
 */

/* Bit band SRAM definitions */
#define BITBAND_SRAM_REF   0x20000000
#define BITBAND_SRAM_BASE  0x22000000

#define BITBAND_SRAM(a,b) ((BITBAND_SRAM_BASE + ((a-BITBAND_SRAM_REF)<<5) + (b<<2)))  // Convert SRAM address

/* Bit band PERIPHERAL definitions */
#define BITBAND_PERI_REF   0x40000000
#define BITBAND_PERI_BASE  0x42000000

#define BITBAND_PERI(a,b) ((BITBAND_PERI_BASE + ((a-BITBAND_PERI_REF)<<5) + (b<<2)))  // Convert PERI address

/* Basic bit band function definitions */
#define BITBAND_SRAM_ClearBit(a,b)	(*(volatile uint32_t *) (BITBAND_SRAM(a,b)) = 0)
#define BITBAND_SRAM_SetBit(a,b)	(*(volatile uint32_t *) (BITBAND_SRAM(a,b)) = 1)
#define BITBAND_SRAM_GetBit(a,b)	(*(volatile uint32_t *) (BITBAND_SRAM(a,b)))

#define BITBAND_PERI_ClearBit(a,b)	(*(volatile uint32_t *) (BITBAND_PERI(a,b)) = 0)
#define BITBAND_PERI_SetBit(a,b)	(*(volatile uint32_t *) (BITBAND_PERI(a,b)) = 1)
#define BITBAND_PERI_GetBit(a,b)	(*(volatile uint32_t *) (BITBAND_PERI(a,b)))

/* Variable address in SRAM
 * should be in one of two range:
 * 	+ 0x20000000 - 0x2000FFFF: for AHB SRAM
 */
#define VAR_ADDRESS		0x20000000
#define VAR_BIT			3 //Bit 3

/* Peripheral address
 * should be in range: 0x40000000 - 0x40100000
 */
#define PERI_ADDRESS	0x40083000 // SSSP0CR Control Register (S0SPCR)
#define PERI_BIT		5 //bit 5


/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"Bit-banding demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART0 - 115200 bps \n\r"
	"This example used to test Bit-banding feature of Cortex-M3 processor\n\r"
	"********************************************************************************\n\r";


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		main function
 * @param[in]	none
 * @return 		int
 **********************************************************************/
int c_entry(void) { /* Main Program */

	uint32_t temp;

	SystemInit();
	CGU_Init();
	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();
	//print menu
	_DBG(menu);

	/* test Bit-banding SRAM first --------------------------------------------------------- */
	_DBG_("Test bit-band SRAM... ");

	//read variable at VAR_ADDRESS
	temp = (*(volatile uint32_t *)(VAR_ADDRESS));
	//print temp value
	_DBG("The value at address "); _DBH32(VAR_ADDRESS); _DBG(": ");
	_DBH32(temp); _DBG_("");

	/* Use bitband function to read VAR_BIT value */
	temp = BITBAND_SRAM_GetBit(VAR_ADDRESS,VAR_BIT);
	_DBG("Use bit-band function to get value at bit 3: ");_DBG_("");
	_DBH32(temp); _DBG_("");

	/* clear bit */
	BITBAND_SRAM_ClearBit(VAR_ADDRESS,VAR_BIT);
	/* re-read temp value */
	temp = (*(volatile uint32_t *)(VAR_ADDRESS));
	_DBG("Value after clear bit 3 value by using bit-band function: ");_DBG_("");
	_DBH32(temp); _DBG_("");

	/* Set bit */
	BITBAND_SRAM_SetBit(VAR_ADDRESS,VAR_BIT);
	/* re-read temp value */
	temp = (*(volatile uint32_t *)(VAR_ADDRESS));
	_DBG("Value after set bit 3 value by using bit-band function: ");_DBG_("");
	_DBH32(temp); _DBG_("");
	_DBG_("");

	/* Then, test Bit-banding PERI --------------------------------------------------------- */
	_DBG_("Test bit-band PERIPHERAL... ");

	LPC_SSP0->CR0 =(1<<5);

	/* Read the current value of peripheral register */
	temp = (*(volatile uint32_t *)(PERI_ADDRESS));
	_DBG("The value of peripheral register at "); _DBH32(PERI_ADDRESS); _DBG(": ");_DBG_("");
	_DBH32(temp); _DBG_("");

	/* Use bitband function to read VAR_BIT value */
	temp = BITBAND_PERI_GetBit(PERI_ADDRESS,PERI_BIT);
	_DBG("Use bit-band function to get value at bit 5: ");_DBG_(""); _DBH32(temp); _DBG_("");

	/* clear bit */
	BITBAND_PERI_ClearBit(PERI_ADDRESS,PERI_BIT);
	/* re-read temp value */
	temp = (*(volatile uint32_t *)(PERI_ADDRESS));
	_DBG("Peripheral register after clear bit 5 value by using bit-band function: ");_DBG_("");
	_DBH32(temp); _DBG_("");

	/* Set bit */
	BITBAND_PERI_SetBit(PERI_ADDRESS,PERI_BIT);
	/* re-read temp value */
	temp = (*(volatile uint32_t *)(PERI_ADDRESS));
	_DBG("Peripheral register after set bit 5 value by using bit-band function: ");_DBG_("");
	_DBH32(temp); _DBG_("");

	while (1);
}

/* With ARM and GHS toolsets, the entry point is main() - this will
 allow the linker to generate wrapper code to setup stacks, allocate
 heap area, and initialize and copy code and data segments. For GNU
 toolsets, the entry point is through __start() in the crt0_gnu.asm
 file, and that startup code will setup stacks and data */
int main(void) {
	return c_entry();
}

#ifdef  DEBUG
/*******************************************************************************
 * @brief		Reports the name of the source file and the source line number
 * 				where the CHECK_PARAM error has occurred.
 * @param[in]	file Pointer to the source file name
 * @param[in]    line assert_param error line source number
 * @return		None
 *******************************************************************************/
void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
		;
}
#endif

/**
 * @}
 */

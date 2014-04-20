/**********************************************************************
* $Id$		Spifi_ReadWrite.c	2011-06-02
*//**
* @file		Spifi_ReadWrite.c
* @brief	This example describes how to use GPIO to drive LEDs
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
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_cgu.h"
#include "spifi_rom_api.h"
#include "debug_frmwrk.h"
#include <string.h>

/* Example group ----------------------------------------------------------- */
/** @defgroup Spifi_ReadWrite	Spifi_ReadWrite
 * @ingroup SPIFI_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
#define LED1_BIT			1 //LEDUSB
#define LED1_PORT			4

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" SPIFI demo \n\r"
"\t - MCU: LPC4300 \n\r"
"\t - Core: ARM CORTEX-M4 \n\r"
"\t - Communicate via: UART1 - 115200 bps \n\r"
"********************************************************************************\n\r";
SPIFIobj obj;
SPIFI_RTNS * pSpifi;
SPIFIopers opers;
unsigned char ProgramData[PROG_SIZE];

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/

int c_entry (void) {                       /* Main Program                       */

    uint32_t i;
	SystemInit();
	CGU_Init();

	/* set SPIFI clock */
	LPC_CGU->BASE_SPIFI_CLK = 1<<24 | 1<<11; /* IRC 12 MHz is good enough for us */

	/* set up SPIFI I/O (undocumented bit 7 set as 1, Aug 2 2011) */
	LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
	/* IO pins */
	LPC_SCU->SFSP3_4=LPC_SCU->SFSP3_5=LPC_SCU->SFSP3_6=LPC_SCU->SFSP3_7 = 0xD3;
	LPC_SCU->SFSP3_8 = 0x13; /* CS doesn't need feedback */

	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	// Set up LED
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : USB0_IND1 LED

	GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), 1);

	// print welcome screen
	print_menu();

#ifdef USE_SPIFI_LIB
    pSpifi = &spifi_table;
#else
    pSpifi = (SPIFI_RTNS *)(SPIFI_ROM_TABLE);
#endif

	_DBG("Initializing SPIFI driver...");
	/* Initialize SPIFI driver */
	if (pSpifi->spifi_init(&obj, 3, S_RCVCLK | S_FULLCLK, 12)) while (1);

	_DBG("OK\r\nErasing QSPI device...");
		/* Erase Entire SPIFI Device if required */
	for ( i = 0 ; i < obj.memSize / 4; i+=4 )
	{
	    if ( *((uint32_t *)(obj.base+i)) != 0xFFFFFFFF )
		{
		    opers.dest = (char *)(obj.base);
		    opers.length = obj.memSize;
      	    opers.scratch = NULL;
			opers.options = S_VERIFY_ERASE;
		    /* Erase Device */
			if (pSpifi->spifi_erase(&obj, &opers)) while (1);
			break;
		}
	}

	_DBG("OK\r\nProgramming + verifying QSPI device...");
	for(i=0;i<PROG_SIZE;i++)
		ProgramData[i] = i%256;

	opers.length = PROG_SIZE;
	opers.scratch = NULL;
	opers.protect = 0;
	opers.options = S_CALLER_ERASE;
	for ( i = 0 ; i < obj.memSize / PROG_SIZE; i++ )
	{
        /* Write */
	    opers.dest = (char *)( obj.base + (i*PROG_SIZE) );
	    if (pSpifi->spifi_program (&obj, (char *)ProgramData, &opers)) while (1);
	    /* Verify */
	    if (memcmp((void *)opers.dest,(void *)ProgramData,PROG_SIZE)) while (1);
	}
	_DBG("OK!\r\n");

	GPIO_SetValue(LED1_PORT,(1<<LED1_BIT)); // Light LED

	while (1)
	{                           					// Loop forever
	}
}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
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
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
 */

/**********************************************************************
* $Id$		Eeprom_Demo.c	2011-06-02
*//**
* @file		Eeprom_Demo.c
* @brief	This example describes how to use internal EEPROM
* @version	1.0
* @date		09. December. 2011
* @author	NXP MCU MCU Validation Team
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
**********************************************************************/

#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_scu.h"
#include "debug_frmwrk.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "eeprom.h"

#define PAGE_OFFSET			0x10
#define PAGE_ADDR			0x01

static uint32_t u32Milliseconds;

void vIOInit(void);
void ClockInit(void);
void SysTick_Handler (void);
void HardFault_Handler (void);

/* Example group ----------------------------------------------------------- */
/** @defgroup EEPROM_Demo	EEPROM Demo
 * @ingroup EEPROM_Examples
 * @{
 */

/************************** PRIVATE VARIABLES ***********************/
uint8_t menu1[]=
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"EEPROM demo example \n\r"
"\t - MCU: LPC18xx/43xx \n\r"
"\t - Core: ARM CORTEX-M3/M4 \n\r"
"\t - Communicate via: UART0\1\2 - 115200 bps \n\r"
"This example used to demo EEPROM operation on LPC18xx/43xx.\n\r"
"A 'Hello' sentence will be written into EEPROM memory, then read back and check.\n\r"
"********************************************************************************\n\r";
//uint8_t read_buffer[];
uint8_t read_buffer[]="NXP Semiconductor LPC18xx/43xx-CortexM3/M4 \n\r\t--- HELLO WORLD!!!---\n\r";


/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);

/*-------------------------PRIVATE FUNCTIONS-----------------------------*/
/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG_(menu1);
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void) {                       /* Main Program */

	uint32_t i, k;
	uint32_t j=0;
	volatile uint32_t *sram_pointer = (uint32_t *)SRAM_BASE_1;
	volatile uint32_t *eeprom_pointer = (uint32_t *)EEPROM_BASE;
	volatile uint8_t *eightbit_pointer = (uint8_t *)SRAM_BASE_1;
	char * count;
	
	SystemInit();
	CGU_Init();

	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	/* Configure the IO's for the LEDs */ 
	vIOInit();	
	
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000);  

			
	scu_pinmux(0x2,0,MD_PLN,FUNC1);		/* PF_10 U0_TXD */
	scu_pinmux(0x2,1,MD_PLN|MD_EZI,FUNC1);	/* PF_11 U0_RXD */

	EEPROM_Init(CGU_GetPCLKFrequency(CGU_PERIPHERAL_EEPROM));
	print_menu();
	count = (char *)&read_buffer;	
	lpc_printf("Read_buffer address: %x\n\r",count);
	lpc_printf("EEPROM Base_Address: %x\n\r",EEPROM_BASE);

  for(i=0;i<0x4000;i++)		   //16k bytes
	{
		*(eightbit_pointer++)=(uint8_t)(i & 0x000000FF);		   //fill 16k EEPROM
	}
	lpc_printf("EEPROM_pointer = %x\n\r",eeprom_pointer);

	for(k=0;k<5;k++)
	{
		if(!(k%2))
			lpc_printf("Writing pattern from SRAM...\n\r");
		else
			lpc_printf("Writing 0xFFFFFFFF pattern...\n\r");
		
		lpc_printf("EEPROM page: ");
		for(i=0;i<128;i++)		// 128 pages of EEPROM
		{
			eeprom_pointer = (uint32_t *)EEPROM_BASE;
			eeprom_pointer = &eeprom_pointer[i*32];
			sram_pointer = (uint32_t *)SRAM_BASE_1;
			sram_pointer = &sram_pointer[i*32];
			if(i)
				lpc_printf(", ");
			lpc_printf("%d ",i);

			for(j=0;j<32;j++)  // 32 words (128 bytes) per page
			{
				if(!(k%2))
					eeprom_pointer[j] = sram_pointer[j];		// copy SRAM page -> EEPROM
				else
					eeprom_pointer[j] =0xFFFFFFFF ;		// Write 0xFFFFFFFFs into EEPROM instead
			}
			LPC_EEPROM->CMD = 0x6; 					   // program EEPROM page
			while (!(LPC_EEPROM->INTSTAT&(1<<2)));			   // Wait until programming is done
			LPC_EEPROM->INTSTATCLR = 1<<2;			   // Clear Program int status bit
			
			for(j=0;j<32;j++) // verify 32 words
			{
				if(!(k%2))
				{
					if(sram_pointer[j] != eeprom_pointer[j])
					{
						lpc_printf("***BAD***");
						break;
					}
				} else {
					if(0xFFFFFFFF != eeprom_pointer[j])
					{
						lpc_printf("***BAD***");
						break;
					}
				}
			}
			if(j==32)			
				lpc_printf("OK");
		}
		lpc_printf("\n\r");
	}

	while(1)
	{                           				
		u32Milliseconds = 100;
		
		/* Wait... */
		while(u32Milliseconds);

		#ifdef USE_NXP_EVAL
			/* Clear the GPIOs, turn off the LEDs*/ 
			LPC_GPIO4->CLR = 0xFF;
			/* Set the GPIO, turn on the LEDs*/ 
			LPC_GPIO4->SET = (1<<u8Ledcntr);

			if (u8Ledcntr++ == 8)
			{
				u8Ledcntr = 0;
			}		
	    #elif defined USE_HITEX_LPC1850_EVAL
		    /* Toggle LEDs */
			LPC_GPIO4->NOT = (1UL << 1) | (1UL << 15);
		#else /* validation board with 256BGA */
			/* Toggle LED state */
				LPC_GPIO_PORT->NOT[4] = (1UL << 14);
		#endif
	}

}

/*----------------------------------------------------------------------------
  Initialize /FalconEagle validation board specific IO
 *----------------------------------------------------------------------------*/
void vIOInit(void)
{
		scu_pinmux(0x9 ,2 , MD_PDN, FUNC0); 	// P9.2 : GPIO4_14: LD11 (LED)
LPC_GPIO_PORT->DIR[4]	= (1UL << 14);
	LPC_SCU->SFSCLK_0 = 0x01;
}

void HardFault_Handler (void)
{
	if (CoreDebug->DHCSR & 1)
	{
		__breakpoint(0);
	}	
	while(1);

}
/**********************************************************************
 ** Function name:		
 **
 ** Description:		
 **						
 ** Parameters:			
 **
 ** Returned value:		
 **********************************************************************/
void SysTick_Handler (void) 					
{           
	if(u32Milliseconds > 0)
	{
		u32Milliseconds--; 
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
	 ex: _DBG_("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
*/

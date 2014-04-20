/**********************************************************************
* $Id$		I2c_EmuMaster.c	  2012-05-15
*//**
* @file		I2c_EmuMaster.c
* @brief	This example describes how to configure GPIOs as I2C master emulation
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
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"
#include "i2c_bitbanging.h"
#include "pca9532.h"
#include "m24xx.h"



/* Example group ----------------------------------------------------------- */
/** @defgroup I2c_Master	I2c_Master
 * @ingroup I2C_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/



/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"I2C emulator demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M4 \n\r"
"\t - This example configures 2 GPIOs to work as I2C in master mode, enabling the 7 inch TFT display module\n\r"
"********************************************************************************\n\r";



/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);
//void Buffer_Init(uint8_t type);

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
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
  unsigned char lcd_configdata[64];
	volatile uint32_t i;

	SystemInit();
	CGU_Init();
	
	for(i = 0; i < 64; i++)
	{
    lcd_configdata[i] = 0; 
	}
	

	/* Initialize message output via UART0 with 115200bps, 8N1, no flow control */

	debug_frmwrk_init();

	print_menu();
	
	// Configure the port pin which is used as reset signal on the display module.
  // The pulldown keeps the PCA9532 on the 7" module in reset state. 
	scu_pinmux(0x07, 3, MD_PDN, FUNC0);	 // LCD_VD_17 @ P7.3 = TFT_RESET 
	LPC_GPIO_PORT->DIR[3] |= 1 << 11;  
	
	// Configure the 2 pins used for the I2C emulation
  // LCDVD9 = P4_8 = GPIO5_12 (function 4) = SDA                                 
	// LCDVD8 = P7_5 = GPIO3_13 (function 0) = SCL                                     
 	scu_pinmux(4, 8, (MD_PUP | MD_EZI), FUNC4);
	scu_pinmux(7, 5, (MD_PUP | MD_EZI), FUNC0);

  i2cemu_init();
	
	_DBG_("Press '1' to write display power-on commands through I2C emulation...");
	while (_DG != '1');

  pca9532_init();
	

	_DBG_("Press '2' to read EEPROM data through I2C emulation ...");
	while (_DG != '2');

  m24xx_eeprom_read(0, 64, &lcd_configdata[0]);
	
	// Print received data
	_DBG_("TFT module config data:");
	for(i = 0; i < 64; i++)
	{
    if(lcd_configdata[i] < 32)
    {	    
			lcd_configdata[i] = 46;   // replace non-printable characters with a '.'
    }
		_DBC(lcd_configdata[i]);
	}
	_DBG_(" ");
	

	_DBG_("Press '0' to switch off the display ...");
	while (_DG != '0');

  i2cemu_deinit();
  pca9532_deinit();
	
	_DBG_("Finished.");
	
  /* Loop forever */
  while(1);
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

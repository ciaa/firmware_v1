/**********************************************************************
* $Id$		I2c_Master.c	2011-06-02
*//**
* @file		I2c_Master.c
* @brief	This example describes how to configure I2C as master device
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
#include "lpc43xx_i2c.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"



/* Example group ----------------------------------------------------------- */
/** @defgroup I2c_Master	I2c_Master
 * @ingroup I2C_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
#define USEDI2CDEV_M		0

#define I2CDEV_UDA1380_ADDR		(0x34>>1)
#define UDA1380_CLOCK_CONFIGURE	0x00
/** Max buffer length */
#define BUFFER_SIZE			0x10

#if (USEDI2CDEV_M == 0)
#define I2CDEV_M LPC_I2C0
#elif (USEDI2CDEV_M == 2)
#define I2CDEV_M LPC_I2C2
#else
#error "Master I2C device not defined!"
#endif

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"I2C demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M4 \n\r"
"\t - This example configures I2C as master mode, write 2 bytes to UDA1380's 0x00_register\n\r"
"\t then read back to verify \n\r"
"********************************************************************************\n\r";

/** These global variables below used in interrupt mode - Slave device ----------------*/
uint8_t Master_Buf[BUFFER_SIZE];
uint8_t master_test[3];

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);
void Buffer_Init(uint8_t type);

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

/*********************************************************************//**
 * @brief		Initialize buffer
 * @param[in]	type:
 * 				- 0: Initialize Master_Buf with 0
 * 					Fill all member in Slave_Buf with 0
 * 				- 1: Initialize Slave_Buf with increment value from 0
 * 					Fill all member in Master_Buf with 0
 * @return 		None
 **********************************************************************/
void Buffer_Init(uint8_t type)
{
	uint8_t i;

	if (type)
	{
		for (i = 0; i < BUFFER_SIZE; i++) {
			Master_Buf[i] = i;
		}
	}
	else
	{
		for (i = 0; i < BUFFER_SIZE; i++) {
			Master_Buf[i] = 0;
		}
	}
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	I2C_M_SETUP_Type transferMCfg;
//	uint32_t tempp;
//	uint8_t *pdat;

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

	print_menu();

	/* Reset UDA1380 on board Hitex */
	scu_pinmux(8,2,MD_PUP, FUNC0);
	GPIO_SetDir(4, 1<<2, 1);
	GPIO_ClearValue(4, 1<<2);

	/* I2C block ------------------------------------------------------------------- */
	// Initialize Slave I2C peripheral
	I2C_Init(I2CDEV_M, 100000);
	/* Enable Slave I2C operation */
	I2C_Cmd(I2CDEV_M, ENABLE);

	/* Transmit -------------------------------------------------------- */
	_DBG_("Press '1' to transmit 2 bytes to UDA1380's 0x00_register...");
	while (_DG != '1');

	/* Initialize buffer */
	master_test[0] = UDA1380_CLOCK_CONFIGURE;
	master_test[1] = 0x05; /* EN_INT & EN_DEC */
	master_test[2] = 0x02; /* PLL1            */

	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
	transferMCfg.tx_data = master_test ;
	transferMCfg.tx_length = 3;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = 3;
	I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING);

	/* Transmit and receive -------------------------------------------------------- */
	_DBG_("Press '2' to read UDA1380's 0x00_register...");
	while (_DG != '2');

	/* Initialize buffer */
	Buffer_Init(0);
	master_test[0] = UDA1380_CLOCK_CONFIGURE;
	master_test[1] = 0x05; /* EN_INT & EN_DEC */
	master_test[2] = 0x02; /* PLL1            */

	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
	transferMCfg.tx_data = master_test ;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = Master_Buf;
	transferMCfg.rx_length = 2;
	transferMCfg.retransmissions_max = 3;
	I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING);

	// Verify
	if(
		(Master_Buf[0]!=master_test[1])
		||(Master_Buf[1]!=master_test[2])
	)
		_DBG_("Verify error");
	else
		_DBG_("Verify successfully");

	I2C_DeInit(I2CDEV_M);
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

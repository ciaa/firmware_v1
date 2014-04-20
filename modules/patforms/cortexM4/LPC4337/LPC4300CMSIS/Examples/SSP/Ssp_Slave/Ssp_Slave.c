/**********************************************************************
* $Id$		Ssp_Slave.c			2011-06-02
*//**
* @file		Ssp_Slave.c
* @brief	This example describes how to use SPP in slave mode
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
#include "lpc43xx_ssp.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"


/* Example group ----------------------------------------------------------- */
/** @defgroup Ssp_Slave	Ssp_Slave
 * @ingroup SSP_Examples
 * @{
 */

/************************** PRIVATE DEFINTIONS *************************/
/** Max buffer length */
#define BUFFER_SIZE			0x40


/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"SSP demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 - 115200bps \n\r"
"\t An example of SSP using interrupt mode to test the SSP driver \n\r"
" This example uses SSP in SPI mode as slave to communicate with an SSP master device \n\r"
" The master and slave transfer together a number of data byte \n\r"
"********************************************************************************\n\r";
uint8_t menu2[] = "Demo terminated! \n\r";

// SSP Configuration structure variable
SSP_CFG_Type SSP_ConfigStruct;
//SSP Data setup structure variable
SSP_DATA_SETUP_Type xferConfig;

// Tx buffer
uint8_t Tx_Buf[BUFFER_SIZE];
// Rx buffer
uint8_t Rx_Buf[BUFFER_SIZE];

/* Status Flag indicates current SSP transmission complete or not */
__IO FlagStatus complete;

/************************** PRIVATE FUNCTIONS *************************/
void SSP1_IRQHandler(void);

void print_menu(void);
void Buffer_Init(void);
void Error_Loop(void);
void Buffer_Verify(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief 		SSP1 Interrupt used for reading and writing handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void SSP1_IRQHandler(void)
{
	SSP_DATA_SETUP_Type *xf_setup;
	uint16_t tmp;
	uint8_t dataword;

	// Disable interrupt
	LPC_SSP1->IMSC = 0;

	if(SSP_GetDataSize(LPC_SSP1) > SSP_DATABIT_8)
		dataword = 1;
	else
		dataword = 0;
	xf_setup = &xferConfig;
	// save status
	tmp = LPC_SSP1->RIS;
	xf_setup->status = tmp;

	// Check overrun error
	if (tmp & SSP_RIS_ROR){
		// Clear interrupt
		LPC_SSP1->ICR = SSP_RIS_ROR;
		// update status
		xf_setup->status |= SSP_STAT_ERROR;
		// Set Complete Flag
		complete = SET;
		return;
	}

	if ((xf_setup->tx_cnt != xf_setup->length) || (xf_setup->rx_cnt != xf_setup->length)){
		/* check if RX FIFO contains data */
		while ((LPC_SSP1->SR & SSP_SR_RNE) && (xf_setup->rx_cnt != xf_setup->length)){
			// Read data from SSP data
			tmp = SSP_ReceiveData(LPC_SSP1);

			// Store data to destination
			if (xf_setup->rx_data != NULL)
			{
				if (dataword == 0){
					*(uint8_t *)((uint32_t)xf_setup->rx_data + xf_setup->rx_cnt) = (uint8_t) tmp;
				} else {
					*(uint16_t *)((uint32_t)xf_setup->rx_data + xf_setup->rx_cnt) = (uint16_t) tmp;
				}
			}
			// Increase counter
			if (dataword == 0){
				xf_setup->rx_cnt++;
			} else {
				xf_setup->rx_cnt += 2;
			}
		}

		while ((LPC_SSP1->SR & SSP_SR_TNF) && (xf_setup->tx_cnt != xf_setup->length)){
			// Write data to buffer
			if(xf_setup->tx_data == NULL){
				if (dataword == 0){
					SSP_SendData(LPC_SSP1, 0xFF);
					xf_setup->tx_cnt++;
				} else {
					SSP_SendData(LPC_SSP1, 0xFFFF);
					xf_setup->tx_cnt += 2;
				}
			} else {
				if (dataword == 0){
					SSP_SendData(LPC_SSP1, (*(uint8_t *)((uint32_t)xf_setup->tx_data + xf_setup->tx_cnt)));
					xf_setup->tx_cnt++;
				} else {
					SSP_SendData(LPC_SSP1, (*(uint16_t *)((uint32_t)xf_setup->tx_data + xf_setup->tx_cnt)));
					xf_setup->tx_cnt += 2;
				}
			}

			// Check overrun error
			if ((tmp = LPC_SSP1->RIS) & SSP_RIS_ROR){
				// update status
				xf_setup->status |= SSP_STAT_ERROR;
				// Set Complete Flag
				complete = SET;
				return;
			}

			// Check for any data available in RX FIFO
			while ((LPC_SSP1->SR & SSP_SR_RNE) && (xf_setup->rx_cnt != xf_setup->length)){
				// Read data from SSP data
				tmp = SSP_ReceiveData(LPC_SSP1);

				// Store data to destination
				if (xf_setup->rx_data != NULL)
				{
					if (dataword == 0){
						*(uint8_t *)((uint32_t)xf_setup->rx_data + xf_setup->rx_cnt) = (uint8_t) tmp;
					} else {
						*(uint16_t *)((uint32_t)xf_setup->rx_data + xf_setup->rx_cnt) = (uint16_t) tmp;
					}
				}
				// Increase counter
				if (dataword == 0){
					xf_setup->rx_cnt++;
				} else {
					xf_setup->rx_cnt += 2;
				}
			}
		}
	}

	// If there more data to sent or receive
	if ((xf_setup->rx_cnt != xf_setup->length) || (xf_setup->tx_cnt != xf_setup->length)){
		// Enable all interrupt
		LPC_SSP1->IMSC = SSP_IMSC_BITMASK;
	} else {
		// Save status
		xf_setup->status = SSP_STAT_DONE;
		// Set Complete Flag
		complete = SET;
	}
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Initialize buffer
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Buffer_Init(void)
{
	uint8_t i;

	for (i = 0; i < BUFFER_SIZE; i++) {
		Tx_Buf[i] = i;
		Rx_Buf[i] = 0;
	}
}

/*********************************************************************//**
 * @brief		Error Loop (called by Buffer_Verify() if any error)
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Error_Loop(void)
{
	/* Loop forever */
	while (1);
}

/*********************************************************************//**
 * @brief		Verify buffer
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Buffer_Verify(void)
{
	uint8_t i;
	uint8_t *src_addr = (uint8_t *) &Tx_Buf[0];
	uint8_t *dest_addr = (uint8_t *) &Rx_Buf[0];

	for ( i = 0; i < BUFFER_SIZE; i++ )
	{
		if ( *src_addr++ != *dest_addr++ )
		{
			_DBG_("Verify error");
			/* Call Error Loop */
			Error_Loop();
		}
	}
}

/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main SSP program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
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

	// print welcome screen
	print_menu();

	/* Configure SSP1 pins*/
	scu_pinmux(0xF,4,MD_PLN_FAST,FUNC0);	// 	func2=SSP1 SCK0
 	scu_pinmux(0xF,5,MD_PLN_FAST,FUNC2);	// 	func2=SSP1 SSEL0
 	scu_pinmux(0xF,6,MD_PLN_FAST,FUNC2);	// 	func2=SSP1 MISO0
 	scu_pinmux(0xF,7,MD_PLN_FAST,FUNC2);	// 	func2=SSP1 MOSI0

	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_ConfigStruct.Mode = SSP_SLAVE_MODE;
	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(SSP1_IRQn, ((0x01<<3)|0x01));
    /* Enable SSP1 interrupt */
    NVIC_EnableIRQ(SSP1_IRQn);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP1, ENABLE);

	_DBG_("Init buffer");
	/* Initialize Buffer */
	Buffer_Init();

	_DBG_("Wait for master transfer...");
	complete = RESET;
	xferConfig.tx_data = Tx_Buf;
	xferConfig.rx_data = Rx_Buf;
	xferConfig.length = BUFFER_SIZE;
	SSP_ReadWrite(LPC_SSP1, &xferConfig, SSP_TRANSFER_INTERRUPT);
	while(complete == RESET);

	// Verify buffer after transferring
	Buffer_Verify();
	_DBG_("Verify complete!");

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

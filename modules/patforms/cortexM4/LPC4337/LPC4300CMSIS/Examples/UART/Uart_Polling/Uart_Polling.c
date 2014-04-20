/**********************************************************************
* $Id$		Uart_Polling.c			2011-06-02
*//**
* @file		Uart_Polling.c
* @brief	This example describes how to using UART in polling mode
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
#include "lpc43xx_uart.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_scu.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Uart_Polling	Uart_Polling
 * @ingroup UART_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS *************************/
#if defined(HITEX_BOARD)
	#define TEST_UART (LPC_USARTn_Type *)LPC_USART0
#elif defined(KEIL_BOARD)
	#define TEST_UART (LPC_USARTn_Type *)LPC_USART3
#else
	#error #define KEIL_BOARD or HITEX_BOARD please
#endif
	

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] = "Hello NXP Semiconductors \n\r";
uint8_t menu2[] = "UART polling mode demo \n\r\t MCU lpc43xx - ARM Cortex-M3 \n\r"
#if defined(HITEX_BOARD)
"\t UART0 - 9600bps \n\r";
#elif defined(KEIL_BOARD)
"\t UART3 - 9600bps \n\r";
#endif
uint8_t menu3[] = "UART demo terminated!";

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(void);

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	UART_Send(TEST_UART, menu1, sizeof(menu1), BLOCKING);
	UART_Send(TEST_UART, menu2, sizeof(menu2), BLOCKING);
}



/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main UART program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;

	uint32_t idx, len;
	__IO FlagStatus exitflag;
	uint8_t buffer[10];

	SystemInit();
	CGU_Init();

	/*
	 * Initialize UART0 pin connect
	 */

	#if defined(HITEX_BOARD)
	// PF.10 : UART0_TXD (on Hitex board Rev. A4)
	scu_pinmux(0xF ,10 , MD_PDN, FUNC1); 	
	// PF.11 : UART0_RXD (on Hitex board Rev. A4)
	scu_pinmux(0xF ,11 , MD_PLN|MD_EZI|MD_ZI, FUNC1); 	
	#elif defined(KEIL_BOARD)
	// Need to use UART3- UART0 interferes with EMC (SDRAM/Async Flash) on Keil board
	// P2.3 : UART3_TXD (on Keil MCB board)
	scu_pinmux(0x2,3,MD_PLN,FUNC2);
	// P2.4 : UART3_RXD (on Keil MCB board)
	scu_pinmux(0x2,4,MD_PLN|MD_EZI,FUNC2);
	#else
		#error #define KEIL_BOARD or HITEX_BOARD please
	#endif

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init(TEST_UART, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig(TEST_UART, &UARTFIFOConfigStruct);


	// Enable UART Transmit
	UART_TxCmd(TEST_UART, ENABLE);

	// print welcome screen
	print_menu();

	// Reset exit flag
	exitflag = RESET;

    /* Read some data from the buffer */
    while (exitflag == RESET)
    {
       len = 0;
        while (len == 0)
        {
            len = UART_Receive(TEST_UART, buffer, sizeof(buffer), NONE_BLOCKING);
        }

        /* Got some data */
        idx = 0;
        while (idx < len)
        {
            if (buffer[idx] == 27)
            {
                /* ESC key, set exit flag */
            	UART_Send(TEST_UART, menu3, sizeof(menu3), BLOCKING);
                exitflag = SET;
            }
            else if (buffer[idx] == 'r')
            {
                print_menu();
            }
            else
            {
                /* Echo it back */
            	UART_Send(TEST_UART, &buffer[idx], 1, BLOCKING);
            }
            idx++;
        }
    }

    // wait for current transmission complete - THR must be empty
    while (UART_CheckBusy(TEST_UART) == SET);

    // DeInitialize UART0 peripheral
    UART_DeInit(TEST_UART);

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

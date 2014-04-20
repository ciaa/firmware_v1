/**********************************************************************
* $Id$		Uart_Autobaud.c			2011-06-02
*//**
* @file		Uart_Autobaud.c
* @brief	This example describes how to configure UART using auto-baud
* 			rate in interrupt mode
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
#include "lpc43xx_uart.h"
#include "lpc43xx_libcfg.h"
#include "lpc43xx_scu.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Uart_Autobaud	Uart_Autobaud
 * @ingroup UART_Examples
 * @{
 */


/************************** PRIVATE VARIABLES *************************/
uint8_t syncmenu[] = "AutoBaudrate Status: Synchronous! \n\r";
uint8_t menu1[] = "Hello NXP Semiconductors \n\r";
uint8_t menu2[] =
"UART Auto Baudrate demo\n\r\t "
"MCU lpc43xx - ARM Cortex-M3 \n\r\t "
#if defined(HITEX_BOARD)
"UART0 - Auto Baud rate mode used \n\r";
#elif defined(KEIL_BOARD)
"UART3 - Auto Baud rate mode used \n\r";
#endif
uint8_t menu3[] = "UART demo terminated!\n";

/* Synchronous Flag */
__IO FlagStatus Synchronous;

/************************** PRIVATE FUNCTIONS *************************/
#if defined(HITEX_BOARD)
void UART0_IRQHandler(void);
#elif defined(KEIL_BOARD)
void UART3_IRQHandler(void);
#endif

void print_menu(LPC_USARTn_Type *myUART);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief	UART0 interrupt handler sub-routine
 * @param	None
 * @return	None
 **********************************************************************/
#if defined(HITEX_BOARD)
void UART0_IRQHandler(void)
#elif defined(KEIL_BOARD)
void UART3_IRQHandler(void)
#endif
{
	// Call Standard UART 0 interrupt handler
	uint32_t intsrc, tmp, tmp1;
	LPC_USARTn_Type *myUART;

#if defined(HITEX_BOARD)
	myUART = (LPC_USARTn_Type *)LPC_USART0;
#elif defined(KEIL_BOARD)
	myUART = (LPC_USARTn_Type *)LPC_USART3;
#endif
	
	/* Determine the interrupt source */
	intsrc = myUART->IIR;
	tmp = intsrc & UART_IIR_INTID_MASK;


	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = myUART->LSR;
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {

			while(tmp1){
				; //implement error handling here
			}
		}
	}


	intsrc &= (UART_IIR_ABEO_INT | UART_IIR_ABTO_INT);
	// Check if End of auto-baudrate interrupt or Auto baudrate time out
	if (intsrc){
		// Clear interrupt pending
		myUART->ACR |= ((intsrc & UART_IIR_ABEO_INT) ? UART_ACR_ABEOINT_CLR : 0) \
						| ((intsrc & UART_IIR_ABTO_INT) ? UART_ACR_ABTOINT_CLR : 0);
			if (Synchronous == RESET)
			{
				/* Interrupt caused by End of auto-baud */
				if (intsrc & UART_AUTOBAUD_INTSTAT_ABEO){
					// Disable AB interrupt
					UART_IntConfig(myUART, UART_INTCFG_ABEO, DISABLE);
					// Set Sync flag
					Synchronous = SET;
				}

				/* Auto-Baudrate Time-Out interrupt (not implemented) */
				if (intsrc & UART_AUTOBAUD_INTSTAT_ABTO) {
					/* Just clear this bit - Add your code here */
					myUART->ACR |= (1<<9);
				}
			}
	}
}


/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(LPC_USARTn_Type *myUART)
{
	UART_Send(myUART, menu1, sizeof(menu1), BLOCKING);
	UART_Send(myUART, menu2, sizeof(menu2), BLOCKING);
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
	// Auto baudrate configuration structure
	UART_AB_CFG_Type ABConfig;
	uint32_t idx, len;
	__IO FlagStatus exitflag;
	uint8_t buffer[10];
	
	LPC_USARTn_Type *myUART;

	SystemInit();
	CGU_Init();

#if defined(HITEX_BOARD)
	myUART = (LPC_USARTn_Type *)LPC_USART0;
	// PF.10 : UART0_TXD (on Hitex board Rev. A4)
	scu_pinmux(0xF ,10 , MD_PDN, FUNC1);
	// PF.11 : UART0_RXD (on Hitex board Rev. A4)
	scu_pinmux(0xF ,11 , MD_PLN|MD_EZI|MD_ZI, FUNC1);
#elif defined(KEIL_BOARD)
	myUART = (LPC_USARTn_Type *)LPC_USART3;
	// Need to use UART3- UART0 interferes with EMC (SDRAM/Async Flash) on Keil board
	// P2.3 : UART3_TXD (on Keil MCB board)
	scu_pinmux(0x2,3,MD_PLN,FUNC2);
	// P2.4 : UART3_RXD (on Keil MCB board)
	scu_pinmux(0x2,4,MD_PLN|MD_EZI,FUNC2);
#else
#error
	#define KEIL_BOARD or HITEX_BOARD please
#endif
	

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	/* Initialize UART0 peripheral with given to corresponding parameter
	 * in this case, don't care the baudrate value UART initialized
	 * since this will be determine when running auto baudrate
	 */
	UART_Init(myUART, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig(myUART, &UARTFIFOConfigStruct);


	// Enable UART Transmit
	UART_TxCmd(myUART, ENABLE);


    /* Enable UART End of Auto baudrate interrupt */
	UART_IntConfig(myUART, UART_INTCFG_ABEO, ENABLE);
	/* Enable UART Auto baudrate timeout interrupt */
	UART_IntConfig(myUART, UART_INTCFG_ABTO, ENABLE);

    /* preemption = 1, sub-priority = 1 */
#if defined(HITEX_BOARD)
	NVIC_SetPriority(USART0_IRQn, ((0x01<<3)|0x01));
#elif defined(KEIL_BOARD)
	NVIC_SetPriority(USART3_IRQn, ((0x01<<3)|0x01));
#endif
/* Enable Interrupt for UART0 channel */
#if defined(HITEX_BOARD)
	NVIC_EnableIRQ(USART0_IRQn);
#elif defined(KEIL_BOARD)
	NVIC_EnableIRQ(USART3_IRQn);
#endif


/* ---------------------- Auto baud rate section ----------------------- */
	// Reset Synchronous flag for auto-baudrate mode
	Synchronous = RESET;

	// Configure Auto baud rate mode
    ABConfig.ABMode = UART_AUTOBAUD_MODE0;
    ABConfig.AutoRestart = ENABLE;

    // Start auto baudrate mode
    UART_ABCmd(myUART, &ABConfig, ENABLE);
    print_menu(myUART);

    /* Loop until auto baudrate mode complete */
    while (Synchronous == RESET);


    // Print status of auto baudrate
    UART_Send(myUART, syncmenu, sizeof(syncmenu), BLOCKING);
/* ---------------------- End of Auto baud rate section ----------------------- */

	// print welcome screen
	print_menu(myUART);

	// reset exit flag
	exitflag = RESET;

    /* Read some data from the buffer */
    while (exitflag == RESET)
    {
       len = 0;
        while (len == 0)
        {
            len = UART_Receive(myUART, buffer, sizeof(buffer), NONE_BLOCKING);
        }

        /* Got some data */
        idx = 0;
        while (idx < len)
        {
            if (buffer[idx] == 27)
            {
                /* ESC key, set exit flag */
            	UART_Send(myUART, menu3, sizeof(menu3), BLOCKING);
                exitflag = SET;
            }
            else if (buffer[idx] == 'r')
            {
                print_menu(myUART);
            }
            else
            {
                /* Echo it back */
            	UART_Send(myUART, &buffer[idx], 1, BLOCKING);
            }
            idx++;
        }
    }

    // wait for current transmission complete - THR must be empty
    while (UART_CheckBusy(myUART) == SET);

    // DeInitialize UART0 peripheral
    UART_DeInit(myUART);

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

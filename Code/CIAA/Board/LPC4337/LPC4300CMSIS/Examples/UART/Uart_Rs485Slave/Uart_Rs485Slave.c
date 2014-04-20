/**********************************************************************
* $Id$		Uart_Rs485Slave.c			2011-06-02
*//**
* @file		Uart_Rs485Slave.c
* @brief	This example used to test RS485 functionality on UART0 of
* 			lpc43xx.In this case, RS485 function on UART0 acts as SLave
* 			on RS485 bus.
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
/** @defgroup Uart_Rs485Slave	Uart_Rs485Slave
 * @ingroup UART_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
/*
 * These following defines can be modified:
 * - RECEIVER_ALWAYS_EN (0/1)
 * - AUTO_SLVADDR_DETECT (0/1) in case RECEIVER_ALWAYS_EN is set to 0
 */

/* Receiver always be enabled to receive any data frame on RS485 bus,
 * regardless that frame is data frame or slave address frame (9-bit).
 * - When receiving a data frame, slave will display that data frame content
 * via UART1.
 * - When receiving a slave address frame (9bit mode), line error interrupt
 * - 0: Receiver is not always enabled, only slave address frame can trigger
 * an interrupt event to allow slave handle.
 * - 1: Receiver always be enabled */
#define RECEIVER_ALWAYS_EN	0


#if (RECEIVER_ALWAYS_EN == 0)
/* Enable/Disable Auto Slave Address Detection
 * - In case of '0': any received data bytes will be ignored and will not
 * be stored in the RXFIFO. When an address byte is detected (parity bit
 * = '1') it will be placed into the RXFIFO and an Rx Data Ready Interrupt
 * will be generated. The interrupt handler can then read the address byte
 * and decide whether or not to enable the receiver to accept the following data.
 * - In case of '1': any received byte will be discarded if it is either a
 * data byte OR an address byte which fails to match the slave address configured
 * when initializing RS485. When a matching address character is detected it will
 * be pushed onto the RXFIFO along with the parity bit, and the receiver will
 * be automatically enabled (RS485CTRL bit 1 will be cleared by hardware).
 * The receiver will also generate an Rx Data Ready Interrupt */
#define AUTO_SLVADDR_DETECT 1
#define SLAVE_ADDR 'A'
#endif

/* buffer size definition */
#define UART_RING_BUFSIZE 256

/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)	(bufidx=0)
#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)

/************************** PRIVATE TYPES *************************/
/** @brief UART Ring buffer structure */
typedef struct
{
    __IO uint32_t tx_head;                /*!< UART Tx ring buffer head index */
    __IO uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
    __IO uint32_t rx_head;                /*!< UART Rx ring buffer head index */
    __IO uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
    __IO uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
    __IO uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
} UART_RING_BUFFER_T;

/************************** PRIVATE VARIABLES *************************/
#if (RECEIVER_ALWAYS_EN)
uint8_t menu1[] =
"Hello NXP Semiconductors \n\r" \
"RS485 demo in Slave mode \n\r" \
"Slave's Receiver always enabled \n\r";
#else
#if (AUTO_SLVADDR_DETECT == 0)
uint8_t menu1[] =
"Hello NXP Semiconductors \n\r" \
"RS485 demo in Slave mode \n\r" \
"Slave's Receiver is not always enabled - Auto Address Detection is disabled\n\r";
#else
uint8_t menu1[] =
"Hello NXP Semiconductors \n\r" \
"RS485 demo in Slave mode \n\r" \
"Slave's Receiver is not always enabled - Auto Address Detection is enabled\n\r";
#endif
#endif

uint8_t send_msg[] = "Sending... \n\r";
uint8_t recv_msg[] = "Receive: ";
uint8_t p_err_menu[] = "Parity error";
uint8_t addr_menu[] = " - Slv Addr Frm received\n\r";
uint8_t addr_acc[] = " - Slave Addr accepted\n\r";
uint8_t addr_una[] = " - Slave Addr unaccepted\n\r";
uint8_t addr_auto[] = "Slave Addr detected!\n\r";
uint8_t f_err_menu[] = "Frame error \n\r";
uint8_t nextline[] = "\n\r";

uint8_t ack_msg[] = "ACK";
uint8_t terminator = 13;

// UART Ring buffer
UART_RING_BUFFER_T rb;

/************************** PRIVATE FUNCTIONS *************************/
void UART0_IRQHandler(void);
void UART_IntReceive(void);
void UART_IntErr(uint8_t bLSErrType);

uint32_t UARTReceive(LPC_USARTn_Type *UARTPort, uint8_t *rxbuf, uint8_t buflen);
void print_menu(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		UART0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART0_IRQHandler(void)
{
	// Call Standard UART 0 interrupt handler
	uint32_t intsrc, tmp, tmp1;


	/* Determine the interrupt source */
	intsrc = LPC_USART0->IIR;
	tmp = intsrc & UART_IIR_INTID_MASK;


	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = LPC_USART0->LSR;
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
			UART_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
		UART_IntReceive();
	}
}


/********************************************************************//**
 * @brief 		UART receive function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_IntReceive(void)
{

#if (RECEIVER_ALWAYS_EN)
	uint8_t tmpc;
	uint32_t rLen;

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive(LPC_USART0, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}
#else
#if (AUTO_SLVADDR_DETECT == 0)

	uint8_t tmpc;
	uint32_t rLen;

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive(UART0, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}
#else

	uint8_t tmpc;
	uint32_t rLen;

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive(LPC_USART0, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}
#endif
#endif
}


/*********************************************************************//**
 * @brief		UART Line Status Error
 * @param[in]	bLSErrType	UART Line Status Error Type
 * @return		None
 **********************************************************************/
void UART_IntErr(uint8_t bLSErrType)
{
	uint8_t tmp;
#if (RECEIVER_ALWAYS_EN)
	uint8_t tmpc;

	if (bLSErrType & UART_LSR_PE){
		// Parity error means the latest frame receive is slave address frame,
		// Value of slave address is read and trimmed out.
		UART_Send((LPC_USARTn_Type*)LPC_UART1, p_err_menu, sizeof(p_err_menu), BLOCKING);
		UART_Send((LPC_USARTn_Type*)LPC_UART1, addr_menu, sizeof(addr_menu), BLOCKING);
		UART_Receive(LPC_USART0, &tmpc, 1, NONE_BLOCKING);
	}

	if (bLSErrType & UART_LSR_FE){
		UART_Send((LPC_USARTn_Type*)LPC_UART1, f_err_menu, sizeof(f_err_menu), BLOCKING);
	}
#else
#if (AUTO_SLVADDR_DETECT == 0)
	uint8_t tmp;
	// Check if this interrupt caused by parity error,
	// that means the last received frame is address frame,
	// if this address is matched with its own address,
	// continue to receive following data frame.
	if (bLSErrType & UART_LSR_PE){
		UART_Receive(UART0, &tmp, 1, NONE_BLOCKING);
		UART_Send((LPC_USARTn_Type*)LPC_UART1, p_err_menu, sizeof(p_err_menu), BLOCKING);
		if (tmp == SLAVE_ADDR){
			UART_RS485ReceiverCmd(UART0, ENABLE);
			UART_Send((LPC_USARTn_Type*)LPC_UART1, addr_acc, sizeof(addr_acc), BLOCKING);
		} else {
			// Disable receiver
			UART_RS485ReceiverCmd(UART0, DISABLE);
			UART_Send((LPC_USARTn_Type*)LPC_UART1, addr_una, sizeof(addr_una), BLOCKING);
		}
	}
#else

	// Check if this interrupt caused by parity error,
	// that means the last received frame is address frame,
	// if this address is matched with its own address,
	// continue to receive following data frame.
	if (bLSErrType & UART_LSR_PE){
		UART_Receive(LPC_USART0, &tmp, 1, NONE_BLOCKING);
		UART_Send((LPC_USARTn_Type*)LPC_UART1, addr_auto, sizeof(addr_auto), BLOCKING);
	}
#endif
#endif
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		UART read function for interrupt mode (using ring buffers)
 * @param[in]	UARTPort	Selected UART peripheral used to send data,
 * 				should be UART0
 * @param[out]	rxbuf Pointer to Received buffer
 * @param[in]	buflen Length of Received buffer
 * @return 		Number of bytes actually read from the ring buffer
 **********************************************************************/
uint32_t UARTReceive(LPC_USARTn_Type *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;

	/* Temporarily lock out UART receive interrupts during this
	   read so the UART receive interrupt won't cause problems
	   with the index values */
	UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

	/* Loop until receive buffer ring is empty or
		until max_bytes expires */
	while ((buflen > 0) && (!(__BUF_IS_EMPTY(rb.rx_head, rb.rx_tail))))
	{
		/* Read data from ring buffer into user buffer */
		*data = rb.rx[rb.rx_tail];
		data++;

		/* Update tail pointer */
		__BUF_INCR(rb.rx_tail);

		/* Increment data count and decrement buffer size count */
		bytes++;
		buflen--;
	}

	/* Re-enable UART interrupts */
	UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}

/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	UART_Send((LPC_USARTn_Type*)LPC_UART1, menu1, sizeof(menu1), BLOCKING);
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main UART-RS485 program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	// RS485 configuration
	UART_RS485_CTRLCFG_Type rs485cfg;
	uint32_t idx, len;
	uint8_t buffer[10];
	uint32_t tmp;

	SystemInit();
	CGU_Init();

	// UART1 section ----------------------------------------------------
	/*
	 * Initialize UART1 pin connect
	 */
	scu_pinmux(0xC ,13 , MD_PDN, FUNC2); 	// PC.13 : UART1_TXD
	scu_pinmux(0xC ,14 , MD_PLN|MD_EZI|MD_ZI, FUNC2); 	// PC.14 : UART1_RXD

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 115200 bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 115200;

	// Initialize UART1 peripheral with given to corresponding parameter
	UART_Init((LPC_USARTn_Type*)LPC_UART1, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART1 peripheral
	UART_FIFOConfig((LPC_USARTn_Type*)LPC_UART1, &UARTFIFOConfigStruct);

	// Enable UART Transmit
	UART_TxCmd((LPC_USARTn_Type*)LPC_UART1, ENABLE);

	// print welcome screen
	print_menu();

	// UART0 - RS485 section -------------------------------------------------

	/*
	 * Initialize UART0 pin connect
	 */
	scu_pinmux(0xF ,10 , MD_PDN, FUNC1); 	// PF.10 : UART0_TXD
	scu_pinmux(0xF ,11 , MD_PLN|MD_EZI|MD_ZI, FUNC1); 	// PF.11 : UART0_RXD
	/*
	 * Initialize UART0 pin connect
	 */
	// DIR0 - P6_2
	scu_pinmux(0x6 ,2 , MD_PDN, FUNC2);

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600 bps
	 * 8 data bit
	 * 1 Stop bit
	 * Parity: None
	 * Note: Parity will be enabled later in UART_RS485Config() function.
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init(LPC_USART0, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig(LPC_USART0, &UARTFIFOConfigStruct);

	// Configure RS485
	/*
	 * - Auto Direction in Tx/Rx driving is enabled
	 * - Direction control pin is set to DIR0
	 * - Direction control pole is set to "1" that means direction pin
	 * will drive to high state before transmit data.
	 * - Multidrop mode is disable
	 * - Auto detect address is disabled
	 * - Receive state is enable
	 */
	rs485cfg.AutoDirCtrl_State = ENABLE;
	rs485cfg.DirCtrlPin = UART_RS485_DIRCTRL_DIR;
	rs485cfg.DirCtrlPol_Level = SET;
	rs485cfg.DelayValue = 50;
	rs485cfg.NormalMultiDropMode_State = ENABLE;
#if AUTO_SLVADDR_DETECT
	rs485cfg.AutoAddrDetect_State = ENABLE;
	rs485cfg.MatchAddrValue = SLAVE_ADDR;
#else
	rs485cfg.AutoAddrDetect_State = DISABLE;
#endif
#if RECEIVER_ALWAYS_EN
	rs485cfg.Rx_State = ENABLE;
#else
	rs485cfg.Rx_State = DISABLE;
#endif
	UART_RS485Config(LPC_USART0, &rs485cfg);

    /* Enable UART Rx interrupt */
	UART_IntConfig(LPC_USART0, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig(LPC_USART0, UART_INTCFG_RLS, ENABLE);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(USART0_IRQn, ((0x01<<3)|0x01));
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(USART0_IRQn);

	// Enable UART Transmit
	UART_TxCmd(LPC_USART0, ENABLE);

	// for testing...
	while (1){
		len = 0;
		while (len == 0)
		{
			len = UARTReceive(LPC_USART0, buffer, sizeof(buffer));
		}

		/* Got some data */
		idx = 0;
		while (idx < len)
		{
			if (buffer[idx] == 13){
				for (tmp = 0; tmp < 1000000; tmp++);
				UART_RS485SendData(LPC_USART0, ack_msg, sizeof(ack_msg));
				UART_Send((LPC_USARTn_Type*)LPC_UART1, nextline, sizeof(nextline), BLOCKING);
				UART_RS485SendData(LPC_USART0, &terminator, 1);
			} else {
				/* Echo it back */
				UART_Send((LPC_USARTn_Type*)LPC_UART1, &buffer[idx], 1, BLOCKING);
			}
			idx++;
		}
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

/**********************************************************************
* $Id$		Ssp_Ti.c			2011-06-02
*//**
* @file		Ssp_Ti.c
* @brief	This example describes how to use SPP using TI frame format
* 			(interrupt mode)
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
/** @defgroup Ssp_Ti	Ssp_Ti
 * @ingroup SSP_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS ***********************/
/* Idle char */
#define IDLE_CHAR	0xFF

/** Used SSP device as master definition */
#define USEDSSPDEV_M		0

/** Used SSP device as slave definition */
#define USEDSSPDEV_S		1

/** Max buffer length */
#define BUFFER_SIZE			0x40

#if (USEDSSPDEV_M == USEDSSPDEV_S)
#error "Master and Slave SSP device are duplicated!"
#endif

#if (USEDSSPDEV_M == 0)
#define SSPDEV_M LPC_SSP0
#elif (USEDSSPDEV_M == 1)
#define SSPDEV_M LPC_SSP1
#else
#error "Master SSP device not defined!"
#endif

#if (USEDSSPDEV_S == 0)
#define SSPDEV_S LPC_SSP0
#elif (USEDSSPDEV_S == 1)
#define SSPDEV_S LPC_SSP1
#else
#error "Slave SSP device not defined!"
#endif

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"SSP demo \n\r"
"\t - MCU: lpc43xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 - 115.200 kbps \n\r"
" This example uses two SSP peripherals in TI frame format \n\r"
" \t one is set as master mode and the other is set as slave mode. \n\r"
"\t The master and slave transfer a number of data bytes together \n\r"
"\t in interrupt mode \n\r"
"********************************************************************************\n\r";
uint8_t menu2[] = "Demo terminated! \n\r";

// SSP Configuration structure variable
SSP_CFG_Type SSP_ConfigStruct;

/* These variable below are used in Master SSP interrupt mode -------------------- */
/* Read data pointer */
uint8_t *pRdBuf_M;
/* Write data pointer */
uint8_t *pWrBuf_M;
/* Index of read data mode */
uint32_t RdIdx_M;
/* Index of write data mode */
uint32_t WrIdx_M;
/* Length of data */
uint32_t DatLen_M;
/* Status Flag indicates current SSP transmission complete or not */
__IO Bool complete_M;
/* Master Tx Buffer */
uint8_t Master_Tx_Buf[BUFFER_SIZE];
/* Master Rx Buffer */
uint8_t Master_Rx_Buf[BUFFER_SIZE];


/* These variable below are used in Slave SSP interrupt mode -------------------- */
/* Read data pointer */
uint8_t *pRdBuf_S;
/* Write data pointer */
uint8_t *pWrBuf_S;
/* Index of read data mode */
uint32_t RdIdx_S;
/* Index of write data mode */
uint32_t WrIdx_S;
/* Length of data */
uint32_t DatLen_S;
/* Status Flag indicates current SSP transmission complete or not */
__IO Bool complete_S;
/* Slave Tx Buffer */
uint8_t Slave_Tx_Buf[BUFFER_SIZE];
/* Slave Rx Buffer */
uint8_t Slave_Rx_Buf[BUFFER_SIZE];


/************************** PRIVATE FUNCTIONS *************************/
void SSP0_IRQHandler(void);
void SSP1_IRQHandler(void);
void ssp_Master_IntHandler(void);
void ssp_Slave_IntHandler(void);

int32_t ssp_MasterReadWrite (LPC_SSPn_Type *SSPx,
	                 void *rbuffer,
	                 void *wbuffer,
	                 uint32_t length);
int32_t ssp_SlaveReadWrite (LPC_SSPn_Type *SSPx,
	                 void *rbuffer,
	                 void *wbuffer,
	                 uint32_t length);
void print_menu(void);
void Buffer_Init(void);
void Buffer_Verify(void);
void Error_Loop(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief 		SSP Master Interrupt sub-routine used for reading
 * 				and writing handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void ssp_Master_IntHandler(void)
{
	uint16_t tmp;

	/* Clear all interrupt */
	SSP_ClearIntPending(SSPDEV_M, SSP_INTCLR_ROR);
	SSP_ClearIntPending(SSPDEV_M, SSP_INTCLR_RT);

	/* check if RX FIFO contains data */
	while (SSP_GetStatus(SSPDEV_M, SSP_STAT_RXFIFO_NOTEMPTY) == SET)
	{
		tmp = SSP_ReceiveData(SSPDEV_M);
		if ((pRdBuf_M!= NULL) && (RdIdx_M < DatLen_M))
		{
			*(pRdBuf_M + RdIdx_M) = (uint8_t) tmp;
		}
		RdIdx_M++;
	}

	/* Check if TX FIFO is not full */
	while ((SSP_GetStatus(SSPDEV_M, SSP_STAT_TXFIFO_NOTFULL) == SET)
			&& (WrIdx_M < DatLen_M))
	{
		/* check if RX FIFO contains data */
		while (SSP_GetStatus(SSPDEV_M, SSP_STAT_RXFIFO_NOTEMPTY) == SET)
		{
			tmp = SSP_ReceiveData(SSPDEV_M);
			if ((pRdBuf_M!= NULL) && (RdIdx_M < DatLen_M))
			{
				*(pRdBuf_M + RdIdx_M) = (uint8_t) tmp;
			}
			RdIdx_M++;
		}

		if (pWrBuf_M != NULL)
		{
			SSP_SendData(SSPDEV_M, (uint16_t)(*(pWrBuf_M + WrIdx_M)));
		}
		else
		{
			SSP_SendData(SSPDEV_M, IDLE_CHAR);
		}
		WrIdx_M++;
	}

	/* There're more data to send */
	if (WrIdx_M < DatLen_M)
	{
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_TX, ENABLE);
	}
	/* Otherwise */
	else
	{
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_TX, DISABLE);
	}

	/* There're more data to receive */
	if (RdIdx_M < DatLen_M)
	{
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_ROR, ENABLE);
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_RT, ENABLE);
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_RX, ENABLE);
	}
	/* Otherwise */
	else
	{
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_ROR, DISABLE);
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_RT, DISABLE);
		SSP_IntConfig(SSPDEV_M, SSP_INTCFG_RX, DISABLE);
	}

	/* Set Flag if both Read and Write completed */
	if ((WrIdx_M == DatLen_M) && (RdIdx_M == DatLen_M))
	{
		complete_M = TRUE;
	}
}



/*********************************************************************//**
 * @brief 		SSP Slave Interrupt sub-routine used for reading
 * 				and writing handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void ssp_Slave_IntHandler(void)
{
	uint16_t tmp;

	/* Clear all interrupt */
	SSP_ClearIntPending(SSPDEV_S, SSP_INTCLR_ROR);
	SSP_ClearIntPending(SSPDEV_S, SSP_INTCLR_RT);

	/* check if RX FIFO contains data */
	while (SSP_GetStatus(SSPDEV_S, SSP_STAT_RXFIFO_NOTEMPTY) == SET)
	{
		tmp = SSP_ReceiveData(SSPDEV_S);
		if ((pRdBuf_S!= NULL) && (RdIdx_S < DatLen_S))
		{
			*(pRdBuf_S + RdIdx_S) = (uint8_t) tmp;
		}
		RdIdx_S++;
	}

	/* Check if TX FIFO is not full */
	while ((SSP_GetStatus(SSPDEV_S, SSP_STAT_TXFIFO_NOTFULL) == SET)
			&& (WrIdx_S < DatLen_S))
	{
		/* check if RX FIFO contains data */
		while (SSP_GetStatus(SSPDEV_S, SSP_STAT_RXFIFO_NOTEMPTY) == SET)
		{
			tmp = SSP_ReceiveData(SSPDEV_S);
			if ((pRdBuf_S!= NULL) && (RdIdx_S < DatLen_S))
			{
				*(pRdBuf_S + RdIdx_S) = (uint8_t) tmp;
			}
			RdIdx_S++;
		}

		if (pWrBuf_S != NULL)
		{
			SSP_SendData(SSPDEV_S, (uint16_t)(*(pWrBuf_S + WrIdx_S)));
		}
		else
		{
			SSP_SendData(SSPDEV_S, IDLE_CHAR);
		}
		WrIdx_S++;
	}

	/* There're more data to send */
	if (WrIdx_S < DatLen_S)
	{
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_TX, ENABLE);
	}
	/* Otherwise */
	else
	{
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_TX, DISABLE);
	}

	/* There're more data to receive */
	if (RdIdx_S < DatLen_S)
	{
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_ROR, ENABLE);
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_RT, ENABLE);
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_RX, ENABLE);
	}
	/* Otherwise */
	else
	{
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_ROR, DISABLE);
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_RT, DISABLE);
		SSP_IntConfig(SSPDEV_S, SSP_INTCFG_RX, DISABLE);
	}

	/* Set Flag if both Read and Write completed */
	if ((WrIdx_S == DatLen_S) && (RdIdx_S == DatLen_S))
	{
		complete_S = TRUE;
	}
}


/*********************************************************************//**
 * @brief 		SSP0 Interrupt used for reading and writing handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void SSP0_IRQHandler(void)
{
#if (USEDSSPDEV_M == 0)
	ssp_Master_IntHandler();
#endif

#if (USEDSSPDEV_S == 0)
	ssp_Slave_IntHandler();
#endif
}

/*********************************************************************//**
 * @brief 		SSP1 Interrupt used for reading and writing handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void SSP1_IRQHandler(void)
{
#if (USEDSSPDEV_M == 1)
	ssp_Master_IntHandler();
#endif

#if (USEDSSPDEV_S == 1)
	ssp_Slave_IntHandler();
#endif
}


/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief 		SSP Read write in polling mode function (Master mode)
 * @param[in]	SSPx: Pointer to SSP device
 * @param[out]	rbuffer: pointer to read buffer
 * @param[in]	wbuffer: pointer to write buffer
 * @param[in]	length: length of data to read and write
 * @return 		0 if there no data to send, otherwise return 1
 ***********************************************************************/
int32_t ssp_MasterReadWrite (LPC_SSPn_Type *SSPx,
	                 void *rbuffer,
	                 void *wbuffer,
	                 uint32_t length)
{
	pRdBuf_M = (uint8_t *) rbuffer;
    pWrBuf_M = (uint8_t *) wbuffer;
    DatLen_M = length;
    RdIdx_M = 0;
    WrIdx_M = 0;

    // wait for current SSP activity complete
    while (SSP_GetStatus(SSPx, SSP_STAT_BUSY));

	/* Clear all remaining data in RX FIFO */
	while (SSP_GetStatus(SSPx, SSP_STAT_RXFIFO_NOTEMPTY))
	{
		SSP_ReceiveData(SSPx);
	}

#if (USEDSSPDEV_M == 0)
	if (length != 0)
	{
		SSP0_IRQHandler();
	}
#endif
#if (USEDSSPDEV_M == 1)
	if (length != 0)
	{
		SSP1_IRQHandler();
	}
#endif

	// Return 0
	return 0;
}

/*********************************************************************//**
 * @brief 		SSP Read write in polling mode function (Slave mode)
 * @param[in]	SSPx: Pointer to SSP device
 * @param[out]	rbuffer: pointer to read buffer
 * @param[in]	wbuffer: pointer to write buffer
 * @param[in]	length: length of data to read and write
 * @return 		0 if there no data to send, otherwise return 1
 ***********************************************************************/
int32_t ssp_SlaveReadWrite (LPC_SSPn_Type *SSPx,
	                 void *rbuffer,
	                 void *wbuffer,
	                 uint32_t length)
{
	pRdBuf_S = (uint8_t *) rbuffer;
    pWrBuf_S = (uint8_t *) wbuffer;
    DatLen_S = length;
    RdIdx_S = 0;
    WrIdx_S = 0;

    // wait for current SSP activity complete
    while (SSP_GetStatus(SSPx, SSP_STAT_BUSY))
    {
    	SSP_ReceiveData(SSPx);
    }

	/* Clear all remaining data in RX FIFO */
	while (SSP_GetStatus(SSPx, SSP_STAT_RXFIFO_NOTEMPTY))
	{
		SSP_ReceiveData(SSPx);
	}
#if (USEDSSPDEV_S == 0)
	if (length != 0)
	{
		SSP0_IRQHandler();
	}
#endif
#if (USEDSSPDEV_S == 1)
	if (length != 0)
	{
		SSP1_IRQHandler();
	}
#endif

	// Return 0
	return 0;
}


/*********************************************************************//**
 * @brief		Initialize buffer
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Buffer_Init(void)
{
	uint8_t i;

	for (i = 0; i < BUFFER_SIZE; i++) {
		Master_Tx_Buf[i] = i;
		Slave_Tx_Buf[i] = i;
		Master_Rx_Buf[i] = 0;
		Slave_Rx_Buf[i] = 0;
	}

}

/*********************************************************************//**
 * @brief		Verify buffer
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Buffer_Verify(void)
{
	uint8_t i;
	uint8_t *pMTx = (uint8_t *) &Master_Tx_Buf[0];
	uint8_t *pSTx = (uint8_t *) &Slave_Tx_Buf[0];
	uint8_t *pMRx = (uint8_t *) &Master_Rx_Buf[0];
	uint8_t *pSRx = (uint8_t *) &Slave_Rx_Buf[0];

	for ( i = 0; i < BUFFER_SIZE; i++ )
	{
		if ((*pSRx++ != *pMTx++) || (*pMRx++ != *pSTx++))
		{
			/* Call Error Loop */
			Error_Loop();
		}
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
	_DBG_("Verify fail!\n\r");
	while (1);
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
 * @brief		Main TI program body
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

	/* Configure SSP0 pins*/
	scu_pinmux(0x3,3,MD_PLN_FAST,FUNC2);	// P3.3 connected to SCL/SCLK	func2=SSP0 SCK0
 	scu_pinmux(0x3,6,MD_PLN_FAST,FUNC2);	// P3.6 connected to nCS		func2=SSP0 SSEL0
 	scu_pinmux(0x3,7,MD_PLN_FAST,FUNC2);	// P3.7 connected to SO			func2=SSP0 MISO0
 	scu_pinmux(0x3,8,MD_PLN_FAST,FUNC2);	// P3.8 connected to nSI		func2=SSP0 MOSI0
 	/* Configure SSP1 pins*/
	scu_pinmux(0xF,4,MD_PLN_FAST,FUNC0);	//func0=SSP1 SCK1
 	scu_pinmux(0xF,5,MD_PLN_FAST,FUNC2);	//func2=SSP1 SSEL1
 	scu_pinmux(0xF,6,MD_PLN_FAST,FUNC2);	//func2=SSP1 MISO1
 	scu_pinmux(0xF,7,MD_PLN_FAST,FUNC2);	//func2=SSP1 MOSI1

	/* Initializing Master SSP device section ------------------------------------------- */
	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	// Re-configure SSP to TI frame format
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_TI;
	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(SSPDEV_M, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(SSPDEV_M, ENABLE);


    /* Initializing Slave SSP device section ------------------------------------------- */
	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);
	/* Re-configure mode for SSP device */
	SSP_ConfigStruct.Mode = SSP_SLAVE_MODE;
	// Re-configure SSP to TI frame format
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_TI;
	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(SSPDEV_S, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(SSPDEV_S, ENABLE);

	/* Interrupt configuration section ------------------------------------------------- */
#if ((USEDSSPDEV_S == 0) || (USEDSSPDEV_M == 0))
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(SSP0_IRQn, ((0x01<<3)|0x01));
	/* Enable SSP0 interrupt */
	NVIC_EnableIRQ(SSP0_IRQn);
#endif
#if ((USEDSSPDEV_S == 1) || (USEDSSPDEV_M == 1))
	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(SSP1_IRQn, ((0x01<<3)|0x01));
	/* Enable SSP0 interrupt */
	NVIC_EnableIRQ(SSP1_IRQn);
#endif

	/* Initializing Buffer section ------------------------------------------------- */
	Buffer_Init();

	/* Start Transmit/Receive between Master and Slave ----------------------------- */
	complete_S = FALSE;
	complete_M = FALSE;

	/* Slave must be ready first */
	ssp_SlaveReadWrite(SSPDEV_S, Slave_Rx_Buf, Slave_Tx_Buf, BUFFER_SIZE);
	/* Then Master can start its transferring */
	ssp_MasterReadWrite(SSPDEV_M, Master_Rx_Buf, Master_Tx_Buf, BUFFER_SIZE);

	/* Wait for complete */
	while ((complete_S == FALSE) || (complete_M == FALSE));

	/* Verify buffer */
	Buffer_Verify();

	_DBG_("Verify success!\n\r");
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

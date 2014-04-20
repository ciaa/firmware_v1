/**********************************************************************
* $Id$		CCan_SimpleTxRx.c		2011-06-02
*//**
* @file		CCan_SimpleTxRx.c
* @brief	This example used to test C CAN feature of Cortex-M3
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
#include "lpc43xx_can.h"
#include "lpc43xx_evrt.h"
#include "lpc43xx_scu.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup CCan_SimpleTxRx	CCan_SimpleTxRx
 * @ingroup C_CAN_Examples
 * @{
 */

/************************** PRIVATE DEFINTIONS*************************/
#define RX_MSG_ID			0x100
#define RX_EXT_MSG_ID		0x00100000

#define TX_MSG_ID			(0x200)
#define TX_EXT_MSG_ID		0x00200000


/************************** PRIVATE VARIABLES *************************/
message_object recv_obj;
volatile uint32_t btn0=FALSE;

/* Extern variables from can.c */
extern volatile uint32_t BOffCnt;
extern volatile uint32_t EWarnCnt;
extern volatile uint32_t StuffErrCnt;
extern volatile uint32_t FormErrCnt;
extern volatile uint32_t AckErrCnt;
extern volatile uint32_t Bit1ErrCnt;
extern volatile uint32_t Bit0ErrCnt;
extern volatile uint32_t CRCErrCnt;
extern volatile uint32_t ND1ErrCnt;

uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"Bit-banding demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART1 - 115200 bps \n\r"
	"This example used to test Bit-banding feature of Cortex-M3 processor\n\r"
	"********************************************************************************\n\r";


/************************** PRIVATE FUNCTION *************************/
void EVRT_IRQHandler(void);
void RX_callback(uint32_t msg_no);
void TX_callback(uint32_t msg_no);
void Print_Errors(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		Event Router IRQ Handler
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	uint32_t reg;
	reg = LPC_EVENTROUTER->STATUS;
	if(reg & (1<<WAKEUP0_ERIn)) 						// WAKEUP0_ERIn interrupt flag
	{
		LPC_EVENTROUTER->CLR_STAT = (1<<WAKEUP0_ERIn);	// clear Eventrouter WAKEUP0_ERIn interrupt flag
		btn0 = TRUE;
	}
	if(reg & (1<<CAN_ERIn)) 							// CAN_ERIn interrupt flag
	{
		LPC_EVENTROUTER->CLR_STAT = (1<<CAN_ERIn);		// clear Eventrouter CAN_ERIn interrupt flag
		CAN_IRQHandler();
	}
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Call back function when device received data
 * @param[in]	msg_no message object number
 * @return 		None
 **********************************************************************/
void RX_callback(uint32_t msg_no){
	CAN_ReadMsg(msg_no, &recv_obj);
	recv_obj.id = 0x200;
	lpc_printf("Message object %d RX STD\n\r", msg_no);

	/* resend message using slot 17 */
	CAN_Send(17, (uint32_t *)&recv_obj);
	lpc_printf("Message object %d TX configured STD\n\r", 17);
}

/*********************************************************************//**
 * @brief		Callback function when device sent data
 * @param[in]	msg_no message object number
 * @return 		None
 **********************************************************************/
void TX_callback(uint32_t msg_no){
	lpc_printf("Message object %d TX complete\n\r", msg_no);
}

/*********************************************************************//**
 * @brief		Monitor Error on Bus
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Print_Errors(void)
{
	uint32_t tmp_ec;

	static uint32_t can_tec=0;				// CAN transmit error counter
	static uint32_t can_rec=0;				// CAN receive error counter
	static Bool can_RP = FALSE;				// CAN Receive error passive

	/* Local copy of previous global error counter values */
	static uint32_t PrevBOffCnt = 0;		// Buss off status counter
	static uint32_t PrevEWarnCnt = 0;		// Warning status counter. 	At least one of the error counters
											// in the EML has reached the error warning limit of 96
	static uint32_t PrevStuffErrCnt = 0;	// More than 5 equal bits in a sequence in received message
	static uint32_t PrevFormErrCnt = 0;		// Wrong format of fixed format part of a received frame
	static uint32_t PrevAckErrCnt = 0;		// Transmitted message not acknowledged.
	static uint32_t PrevBit1ErrCnt = 0;		// Send a HIGH/recessive level, but monitored LOW/dominant
	static uint32_t PrevBit0ErrCnt = 0;		// Send a LOW/dominant level, but monitored HIGH/recessive
	static uint32_t PrevCRCErrCnt = 0;		// The CRC checksum was incorrect in the message received
	static uint32_t PrevND1ErrCnt = 0; 		// Message object new data error counter

	tmp_ec = LPC_C_CAN0->EC;

	if(can_tec != (tmp_ec&0xFF))
	{
		can_tec = (tmp_ec&0xFF);
		lpc_printf("ERR: Transmit error cnt: %d\n\r", can_tec);
	}

	if(can_rec != ((tmp_ec>>8)&0x7F))
	{
		can_rec = ((tmp_ec>>8)&0x7F);
		lpc_printf("ERR: Receive error cnt: %d\n\r", can_rec);
	}

	if(can_RP != (tmp_ec>>15)&1)
	{
		can_RP = (Bool)((tmp_ec>>15)&1);
		if(can_RP)
		{
			lpc_printf("ERR: Passive level reached\n\r");
		}
		else
		{
			lpc_printf("Passive level cleared\n\r");
		}
	}

	if(BOffCnt != PrevBOffCnt)
	{
		PrevBOffCnt = BOffCnt;
		lpc_printf("ERR: Buss off status cnt: %d\n\r", BOffCnt);
	}

	if(EWarnCnt != PrevEWarnCnt)
	{
		PrevEWarnCnt = EWarnCnt;
		lpc_printf("ERR: Warning status cnt: %d\n\r", EWarnCnt);
	}

	if(StuffErrCnt != PrevStuffErrCnt)
	{
		PrevStuffErrCnt = StuffErrCnt;
		lpc_printf("ERR: Stuff error cnt: %d\n\r", StuffErrCnt);
	}

	if(FormErrCnt != PrevFormErrCnt)
	{
		PrevFormErrCnt = FormErrCnt;
		lpc_printf("ERR: Form error cnt: %d\n\r", FormErrCnt);
	}

	if(AckErrCnt != PrevAckErrCnt)
	{
		PrevAckErrCnt = AckErrCnt;
		lpc_printf("ERR: Ack error cnt: %d\n\r", AckErrCnt);
	}

	if(Bit1ErrCnt != PrevBit1ErrCnt)
	{
		PrevBit1ErrCnt = Bit1ErrCnt;
		lpc_printf("ERR: Bit1 error cnt: %d\n\r", Bit1ErrCnt);
	}

	if(Bit0ErrCnt != PrevBit0ErrCnt)
	{
		PrevBit0ErrCnt = Bit0ErrCnt;
		lpc_printf("ERR: Bit0 error cnt: %d\n\r", Bit0ErrCnt);
	}

	if(CRCErrCnt != PrevCRCErrCnt)
	{
		PrevCRCErrCnt = CRCErrCnt;
		lpc_printf("ERR: CRC error cnt: %d\n\r", CRCErrCnt);
	}

	if(ND1ErrCnt != PrevND1ErrCnt)
	{
		PrevND1ErrCnt = ND1ErrCnt;
		lpc_printf("ERR: Message object new data error cnt: %d\n\r", ND1ErrCnt);
	}
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: main function
 * @param[in]	none
 * @return 		int
 **********************************************************************/
int c_entry(void) { /* Main Program */

	message_object send_obj;

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

	/* Setup IO pin */
	scu_pinmux(0x3,2,MD_PLN,FUNC2);		/* PE.3 CAN TD1, FUNC1 */
	scu_pinmux(0x3,1,MD_PLN | MD_EZI ,FUNC2);		/* PE.2	CAN RD1, FUNC1 */

        /** set CAN Peripheral Clock = 12MHz */
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_APB3);

	/* Init EVRT */
	EVRT_Init(LPC_EVENTROUTER);
	/*Clear C CAN Source */
	EVRT_ClrPendIntSrc(LPC_EVENTROUTER, EVRT_SRC_CCAN);
	/*Clear Wake Up 0 Source */
	EVRT_ClrPendIntSrc(LPC_EVENTROUTER, EVRT_SRC_WAKEUP0);
	/* Init CAN */
	
	CAN_Init( CAN_BITRATE500K12MHZ, CLKDIV1 , TX_callback, RX_callback);

	lpc_printf("CAN init\n\r");

	#if(BASIC_MODE | SILENT_MODE | LOOPBACK_MODE)
		LPC_C_CAN0->CNTL |= CTRL_TEST;				// Enable test mode
		#if BASIC_MODE
	 		LPC_C_CAN0->TEST |= TEST_BASIC;			// Enable basic mode
		#endif
		#if SILENT_MODE
	 		LPC_C_CAN0->TEST |= TEST_SILENT;			// Enable silent mode
		#endif
		#if LOOPBACK_MODE
	  		LPC_C_CAN0->TEST |= TEST_LBACK;			// Enable Loopback mode
		#endif
	#endif

	SCB->SCR = 0; 									// Disable SLEEPDEEP

	/* Enable Eventrouter in order to be able to read the C_CAN and Wakeup interrupt */
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER, EVRT_SRC_CCAN, EVRT_SRC_ACTIVE_RISING_EDGE);
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER, EVRT_SRC_WAKEUP0, EVRT_SRC_ACTIVE_RISING_EDGE);
	EVRT_SetUpIntSrc(LPC_EVENTROUTER, EVRT_SRC_CCAN, ENABLE);
	EVRT_SetUpIntSrc(LPC_EVENTROUTER, EVRT_SRC_WAKEUP0, ENABLE);
	NVIC_EnableIRQ(EVENTROUTER_IRQn);

	recv_obj.id = RX_MSG_ID;
	CAN_Recv(1, (uint32_t*)&recv_obj, FALSE);
	send_obj.id = TX_MSG_ID;
	send_obj.dlc = 8;
	CAN_Send(17, (uint32_t *)&send_obj);
	lpc_printf("Message object %d TX configured\n\r", 17);

	while (1)                              			// Loop forever
	{
		Print_Errors();					   			// Use UART1 for can error counter display

		if(btn0)
		{
			btn0=FALSE;
			send_obj.dlc     = 0x04;
			send_obj.id	   = TX_MSG_ID;
		   	send_obj.data[0] = 0xDE;
			send_obj.data[1] = 0xAD;
			send_obj.data[2] = 0xBE;
			send_obj.data[3] = 0xEF;
			CAN_Send(17, (uint32_t *)&send_obj);
			lpc_printf("Message object %d TX configured\n\r", 17);
		}

		__WFI();									// Enter normal sleep mode
	}
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

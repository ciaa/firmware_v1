/*********************************************************************** 
 * $Id: main.c 8651 2011-11-18 10:07:27Z nxp28536 $
 * 
 * Project: Mailbox M4<->M0 Application Example for LPC43xx 
 * 
 * Description: Implements a Mailbox application
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
#ifndef CORE_M0
	  #error "Build Error: please define CORE_M0 in the project settings" 
#endif

#include "LPC43xx.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"

#include "debug_frmwrk.h"

/* ipc comms */
#include "ipc_queue.h"
#include "platform_init.h"
#include "platform_check.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define COUNTER_VAL 100 // check every half second	for sending

static volatile uint32_t counter = COUNTER_VAL ;


void initRiTimer(uint32_t timerInterval);
void initTimer0(uint32_t TimerInterval);

volatile uint32_t numMsgs = 0;
volatile uint32_t numTaskResponses = 0;
#define BUFSIZE	80
char printBuffer[BUFSIZE]; 
char lowerCaseString[] __attribute__ ((section ("lowerCaseString_sec"),used)) = "lpc4300 has two cores inside"  ;
char directString[] __attribute__ ((section ("directString_sec"), used)) = "heureka"  ;

char* returnString;
uint32_t returnMessages;
uint32_t returnPower;
uint32_t localBase = 0;

volatile uint8_t sendA = 0;
volatile uint8_t sendB = 0;
volatile uint8_t sendC = 0;

#define writeString(xyz) _DBG(xyz)

/*****************************************************************************
**   Main Function  M0_main()
*****************************************************************************/
#define CLOCK_SPEED (204000000)
void applicationCode(void);

int main (void)  {

   	/* other library or drivers initialization calls might be done here */
	debug_frmwrk_init_clk(12000000);

	/* SysTick is not implemented, use another timer like RITIMER if needed */
	initRiTimer(CLOCK_SPEED/1000);

	initTimer0(CLOCK_SPEED/1000);   // Khz tick

	/* now the platform is ready, jump to the application */
	applicationCode();
}


/* possible messages */
srvMsg 		serviceMessage;
rdStsMsg 	readStatusMessage;
wrStsMsg 	writeStatusMessage;	  
rdMsg		readMessage;

/* possible commands */
maxCmd_t maxCommand;
cmd_t cmdType;
rdCmd* readCommand;
wrCmd* writeCommand;
uint16_t taskId;
uint16_t argument;
uint32_t parameter;

/* used for tracking statistics */
uint32_t validCmds = 0;
uint32_t receivedCmds = 0;
uint32_t sentMsgs = 0;

uint32_t qFull = 0;
uint32_t qEmpty = 0;
uint32_t qError = 0;

qStat status;


void applicationCode(void)
{		

	writeString("\n\r--- M0 Started ---\n\r\n\r");
	
	// init local queue system 
	IPC_slaveInitQueue();

	// signal to the M0 being ready via a message
	MAKE_SRV_MSG_HEADER(serviceMessage,TSK_MASTER_CONTROL,NOTIFY_SLAVE_STARTED);
	do {
		status = IPC_slavePushMsg(&serviceMessage);
		if(QFULL == status) qFull++;
		if(QERROR == status) qError++;
	}
	while(status != QINSERT);
	sentMsgs++;

	// now notify the master
	IPC_msgNotifyMaster();

	while(1)
	{					
		/* wait for a message to arrive */
		__WFI();
		
		if(counter > 0) {
			
			counter--; 
			
			// check for command pending from M4
			status = IPC_slavePopCmd(&maxCommand);
			
			// track some stats
			if(QEMPTY != status)  {
				receivedCmds++; 
			
				if(QVALID == status) { 
					validCmds++;
				} 
				else if (QERROR == status) {
					qError++;
				};
			}
			else {
			  	qEmpty++;
			};			
			
			// if something valid was received
			if(QVALID == status) {

				cmdType = IPC_getCmdType((cmdToken*)&maxCommand);

				// check which command it is
				if(CMD_RD_ID == cmdType) {

					// not used in this example
					readCommand = (rdCmd*) &maxCommand;

				}
				else if (CMD_WR_ID == cmdType) {

					writeCommand = (wrCmd*) &maxCommand;
					taskId = writeCommand->cmdToken.cmdClass.parsedCmd.command.id;
					argument = writeCommand->cmdToken.cmdClass.parsedCmd.command.argument;
					parameter = writeCommand->param;

					switch(taskId) {

						case TSK_SLAVE_CONTROL:

							if(PRINT_NUM_MESSAGES == argument) {
								
								returnMessages = (uint32_t) parameter;					
								memset(&printBuffer[0], 0, BUFSIZE);
								sprintf(printBuffer,"\t\n\r--- M0 : CM4 processed %d calls ---",returnMessages);
								strcat(printBuffer,"\n\r\n\r");
								writeString(printBuffer);	
							}
							else if (PRINT_WELCOME_MESSAGE == argument) {
								
								returnString = (char*) parameter;
								memset(&printBuffer[0], 0, BUFSIZE);
								strcat(printBuffer,(char*) returnString);
								writeString((char*) printBuffer);	
							}
							
						break;

						case TASK_D:
							
							// get the converted string
							returnString = (char*) parameter;
							numTaskResponses++;	
							memset(&printBuffer[0], 0, BUFSIZE);
							sprintf(printBuffer,"\t[ M4 :");
							strcat(printBuffer,(char*) returnString);
							strcat(printBuffer," ]\n\r\n\r");
							writeString((char*) printBuffer);	

						break;
					
						case TASK_E:
							
							returnPower = parameter;					
							memset(&printBuffer[0], 0, BUFSIZE);
							sprintf(printBuffer,"\t[ M4: %d ^ 3 = %d ]\n\r\n\r",localBase,returnPower);
							localBase++;
							writeString(printBuffer);

						break;

						case TASK_F:

							returnString = (char*) parameter;
							numTaskResponses++;	
							memset(&printBuffer[0], 0, BUFSIZE);
							sprintf(printBuffer,"\t[ M4:");
							strcat(printBuffer,(char*)returnString);
							strcat(printBuffer," ]\n\r\n\r");
							writeString((char*) printBuffer);

						break;
					}; 
				};

			}
		}
		else {

			// checked on a different timescale
			if(sendA) {

		   		memset(&printBuffer[0], 0, BUFSIZE);
				strcat(printBuffer,"> M0 Sending: ");
				strcat(printBuffer,lowerCaseString);
				strcat(printBuffer,"\n\r");
				writeString(printBuffer);
				
				// push another message
				MAKE_RD_MSG_HEADER(readMessage,TASK_MASTER_A,TASK_A_SERVICE);
				readMessage.param = (uint32_t) &lowerCaseString[0];
				do {
					status = IPC_slavePushMsg((msgToken*)&readMessage);
					if(QFULL == status) qFull++;
					if(QERROR == status) qError++;
				} while(status != QINSERT);
				
				// now notify the master
				IPC_msgNotifyMaster();

				sentMsgs++;				
				sendA = 0;				
			}
			
			if(sendB) {

				memset(&printBuffer[0], 0, BUFSIZE);					
				sprintf(printBuffer,"> M0 Sending: request for pow(%d,3)\n\r",localBase);
				writeString(printBuffer);			

				// push another message
				MAKE_RD_MSG_HEADER(readMessage,TASK_MASTER_B,TASK_B_SERVICE);
				readMessage.param = localBase;
				do {
					status = IPC_slavePushMsg((msgToken*)&readMessage);
					if(QFULL == status) qFull++;
					if(QERROR == status) qError++;
				} while(status != QINSERT);
				
				// now notify the master
				IPC_msgNotifyMaster();

				sentMsgs++;				
				sendB = 0;			
			}
			
			if(sendC) {

	   			memset(&printBuffer[0], 0, BUFSIZE);
				strcat(printBuffer,"> M0 Sending: ");
				strcat(printBuffer,directString);
				strcat(printBuffer,"\n\r");
				writeString(printBuffer);			

				// push another message
				MAKE_RD_MSG_HEADER(readMessage,TASK_MASTER_C,TASK_C_SERVICE);
				readMessage.param = (uint32_t) &directString[0];
				do {
					status = IPC_slavePushMsg((msgToken*)&readMessage);
					if(QFULL == status) qFull++;
					if(QERROR == status) qError++;
				} while(status != QINSERT);
				
				// now notify the master
				IPC_msgNotifyMaster();

				sentMsgs++;								
				sendC = 0;						
			}			
						
			counter = COUNTER_VAL;
		};

	}
		
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
static volatile uint32_t u32Milliseconds = 1000;
void M0_RIT_OR_WWDT_IRQHandler (void) 					
{           
	LPC_RITIMER->CTRL |=(1<<0);	// Clear interrupt flag
	if(u32Milliseconds > 0)
	{
		u32Milliseconds--; 
	}
	else
	{
		#if (PLATFORM == HITEX_BOARD)	
		LPC_GPIO_PORT->NOT[4] |= (1UL << 1);    
		#endif
		#if (PLATFORM == KEIL_BOARD)			
		LPC_GPIO_PORT->NOT[4] = (1UL << 13);	// P.4.13
		#endif		
		u32Milliseconds = 1000;
	}
}

void initRiTimer(uint32_t TimerInterval)
{
	NVIC_DisableIRQ(M0_RITIMER_OR_WWDT_IRQn);
	NVIC_ClearPendingIRQ(M0_RITIMER_OR_WWDT_IRQn);

	LPC_RITIMER->COMPVAL = TimerInterval; 			// Set match value
	LPC_RITIMER->COUNTER=0;							// Set count value to 0
	LPC_RITIMER->CTRL = (1<<3)|(1<<2)|(1<<1);		// Enable, enable break, reset on mask compare, clear interrupt flag

	NVIC_EnableIRQ(M0_RITIMER_OR_WWDT_IRQn);

}

#define HZ1 1000
#define PERIOD	5
void initTimer0(uint32_t TimerInterval)
{
	
	NVIC_DisableIRQ(TIMER0_IRQn);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);

	LPC_TIMER0->PR = TimerInterval; // scale down to 1KHZ (204000)
	
	// period 5 sec, reset
	LPC_TIMER0->MR[3] = PERIOD * HZ1; 

	// these give interrupts
	LPC_TIMER0->MR[2] = 3*HZ1;
	LPC_TIMER0->MR[1] = 2*HZ1;
	LPC_TIMER0->MR[0] = 1*HZ1;

	LPC_TIMER0->MCR = (((0x2) << 9)) | (((0x1) << 6)) | (((0x1) << 3)) | ((0x1) << 0);

	LPC_TIMER0->TC = 0;	

	LPC_TIMER0->IR = 0xFF;	

	NVIC_EnableIRQ(TIMER0_IRQn);
	
	LPC_TIMER0->TCR = 0x1;	 // start the timer
}

void TIMER0_IRQHandler(void)
{
	// check which match sent the interrupt
	if(LPC_TIMER0->IR & 0x1)
	{
		LPC_TIMER0->IR = 0x1;
		sendA = 1;
	}

	if(LPC_TIMER0->IR & 0x2)
	{
		LPC_TIMER0->IR = 0x2;
		sendB = 1;
	}

	if(LPC_TIMER0->IR & 0x4)
	{
		LPC_TIMER0->IR = 0x4;
		sendC = 1;
	}
}

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

/*****************************************************************************
**                            End Of File
*****************************************************************************/

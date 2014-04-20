/*****************************************************************************
 * $Id:: main.c 8651 2011-11-18 10:07:27Z nxp28536                           $
 *
 * Project: Mailbox M4<->M0 Application Example for LPC43xx
 *
 * Description:
 *   Implements a Mailbox application
 *----------------------------------------------------------------------------
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
 *****************************************************************************/
#ifndef CORE_M4
	  #error "Build Error: please define CORE_M4 in the project settings" 
#endif

#include "LPC43xx.h"
#include "platform_init.h"

#include "ipc_queue.h"

#include "platform_check.h"

#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "ctype.h"
#include "debug_frmwrk.h"

static volatile uint32_t u32Milliseconds = 1500;
static volatile uint32_t counter;

char welcomeMessage[] = "*************************************\n\r** LPC4300 = Cortex M4 + Cortex M0 **\n\r*************************************\n\r";

char lowerToUpper[41] __attribute__ ((section ("lowerToUpper_sec")));
char reversed[41] __attribute__ ((section ("reversed_sec")));
uint32_t power3 __attribute__ ((section ("power3_sec")));


#define CLOCK_SPEED (204000000UL)
#define COUNTER_VAL 15000 // used to print the stats

/* used to place the CM0_IMAGE array within the "SLAVE IMAGE" section */
#pragma arm section rodata = "M0_SLAVE_IMAGE"
/* fromelf.exe always generates an unsigned char LR0[] type of array */
const	/* DO NOT REMOVE THIS CONST QUALIFIER, IS USED TO PLACE THE IMAGE IN M4 ROM */
#include SLAVE_IMAGE_FILE

#pragma arm section rodata
/* return to default scheme */

#define writeString(xyz) _DBG(xyz)





void applicationCode(void);

int main (void)  {

   	/* other library or drivers initialization calls might be done here */

	/* now the platform is ready, jump to the application */
	applicationCode();
}


/* messages got from the M0 */
maxMsg_t msgBack;
srvMsg* serviceMessage;
rdStsMsg* readStatusMessage;
wrStsMsg* writeStatusMessage;
rdMsg* readMessage;
msg_t msgType;

/* used for the received messages */
uint16_t taskId;
uint16_t argument;
uint8_t statusInfo;
uint32_t parameter;

/* used for the commands sent for the M0 */
wrCmd writeCommand;

/* used for the statistics */
uint32_t validMsgs = 0;
uint32_t receivedMsgs = 0;
uint32_t numData_Messages = 0;
uint32_t numDataA_Messages = 0;
uint32_t numDataB_Messages = 0;
uint32_t numDataC_Messages = 0;
uint32_t sentCmds = 0;
uint32_t qFull = 0;
uint32_t qEmpty = 0;
uint32_t qError = 0;
uint8_t slaveStarted = 0;



qStat status;

void applicationCode (void)  {

	uint32_t lenght = 0;
	uint32_t i;
	char *c, *paramString;

	// just in case
	IPC_haltSlave();
		
	/* setup the queue system */
	IPC_masterInitQueue(&_hostCmdBufferData[0], MASTER_CMDBUF_SIZE, &_hostMsgBufferData[0], SLAVE_MSGBUF_SIZE);
		
	/* download the cpu image */
	#if (SLAVE_ROM_START == 0x1B000000)
	// pass zero as size to avoid downloading since it is in flash
	// still sets the shadow pointer!
	IPC_downloadSlaveImage(SLAVE_ROM_START, &LR0[0], 0);
	#else
	IPC_downloadSlaveImage(SLAVE_ROM_START, &LR0[0], sizeof(LR0));
	#endif

	/* start the remote processor */
	IPC_startSlave();

	SysTick_Config(CLOCK_SPEED/1000); // generate interrupts at 1Khz / 1msec
	
	// wait for the M0 to signal being ready via a message to the command queue
	while(!IPC_msgPending()) __WFI();
	status = IPC_masterPopMsg(&msgBack);

	// if something valid was received
	if(QVALID == status) {

		msgType = IPC_getMsgType((msgToken*)&msgBack);

		// check which message it is
		if(MSG_SRV == msgType) {

			serviceMessage = (srvMsg*) &msgBack;
			taskId = serviceMessage->msgClass.parsedMsg.servicingAnswer.id;
			statusInfo = serviceMessage->msgClass.parsedMsg.servicingAnswer.ss;

			switch(taskId) {

				case TSK_MASTER_CONTROL:
					
					if(NOTIFY_SLAVE_STARTED == statusInfo) {
					
						slaveStarted = 1;   /* just track the M0 is alive */
			
						/* ask the slave to print the welcome message */
						MAKE_WRITE_CMD_HEADER(writeCommand,TSK_SLAVE_CONTROL,PRINT_WELCOME_MESSAGE);
						writeCommand.param = (uint32_t) &welcomeMessage;

						do {
							status = IPC_masterPushCmd((cmdToken*)&writeCommand);
							if(QFULL == status) qFull++;
							if(QERROR == status) qError++;
						} while(status != QINSERT);
				
						// now notify the slave
						IPC_cmdNotifySlave();
							
						sentCmds++;

					};

				break;
			}

		}
	};
	
	counter = COUNTER_VAL;
			
	while(1)
	{

		/* wait for a message to arrive, wake up every msec due to systick */
		__WFI();

		if(counter > 0) {
			
			counter--; 
			
			// check for message pending from M0
			status = IPC_masterPopMsg(&msgBack);
			
			// track some stats
			if(QEMPTY != status)  {
				receivedMsgs++; 
			
				if(QVALID == status) { 
					validMsgs++;
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

				msgType = IPC_getMsgType((msgToken*)&msgBack);

				// check which message it is
				if(MSG_SRV == msgType) {

					// not used in this example
					serviceMessage = (srvMsg*) &msgBack;
					taskId = serviceMessage->msgClass.parsedMsg.servicingAnswer.id;
					statusInfo = serviceMessage->msgClass.parsedMsg.servicingAnswer.ss;
				}
				else if (MSG_RD == msgType) {
					
					numData_Messages++;
					readMessage = (rdMsg*) &msgBack;

					// find out which is it: for TASK_A, B or C
					taskId = readMessage->msgToken.msgClass.parsedMsg.readResponse.id;
					argument = readMessage->msgToken.msgClass.parsedMsg.readResponse.ppp;
					parameter = readMessage->param;

					switch(taskId) {

						case TASK_MASTER_A:

							// convert to uppercase
							memset(&lowerToUpper[0], 0, 41); 
							lenght = strlen((char*) parameter);
							paramString = (char*) parameter; 

							for(i=0;i<lenght;i++)  {
								
								c = (char*) paramString;
								lowerToUpper[i] = toupper(*c);
								c++;
								paramString++;
							}
								
							numDataA_Messages++;

							// send the answer back to the M0
							MAKE_WRITE_CMD_HEADER(writeCommand,TASK_D,TASK_D_ANSWER);
							writeCommand.param = (uint32_t) &lowerToUpper[0];

							do {
								status = IPC_masterPushCmd((cmdToken*)&writeCommand);
								if(QFULL == status) qFull++;
								if(QERROR == status) qError++;
							} while(status != QINSERT);
				
							// now notify the slave
							IPC_cmdNotifySlave();
							
							sentCmds++;

						break;

						case TASK_MASTER_B:

							power3 = (uint32_t) pow(parameter, 3);
							
							numDataB_Messages++;

							// send the answer back to the M0
							MAKE_WRITE_CMD_HEADER(writeCommand,TASK_E,TASK_E_ANSWER);
							writeCommand.param = power3;

							do {
								status = IPC_masterPushCmd((cmdToken*)&writeCommand);
								if(QFULL == status) qFull++;
								if(QERROR == status) qError++;
							} while(status != QINSERT);
				
							// now notify the slave
							IPC_cmdNotifySlave();
							
							sentCmds++;

						break;

						case TASK_MASTER_C:

							// reverse the string
							memset(&reversed[0], 0, 41); 
							lenght = strlen((char*) parameter);
							paramString = (char*) parameter; 
							c = (char*) paramString;
							for(i=0;i<lenght;i++)  {
								  
								  reversed[lenght - i - 1] = *c;
								  c++;
							}
		
							numDataC_Messages++;

							// send the answer back to the M0
							MAKE_WRITE_CMD_HEADER(writeCommand,TASK_F,TASK_F_ANSWER);
							writeCommand.param = (uint32_t) &reversed[0];

							do {
								status = IPC_masterPushCmd((cmdToken*)&writeCommand);
								if(QFULL == status) qFull++;
								if(QERROR == status) qError++;
							} while(status != QINSERT);
				
							// now notify the slave
							IPC_cmdNotifySlave();
							
							sentCmds++;

						break;
					};

				}
				else if (MSG_RD_STS == msgType) {
					
					readStatusMessage = (rdStsMsg*) &msgBack;
					
					// not used in this example
					taskId = readStatusMessage->msgClass.parsedMsg.readStatus.id;
					argument = readStatusMessage->msgClass.parsedMsg.readStatus.ppp;
					statusInfo = readStatusMessage->msgClass.parsedMsg.readStatus.rStat;

				}
				else if (MSG_WR_STS == msgType) {
					
					writeStatusMessage = (wrStsMsg*) &msgBack;
					
					// not used in this example
					taskId = writeStatusMessage->msgClass.parsedMsg.writeStatus.id;
					argument = writeStatusMessage->msgClass.parsedMsg.writeStatus.ppp;
					statusInfo = writeStatusMessage->msgClass.parsedMsg.writeStatus.wStat;

				};									
			};
		}
		else {
			
			// do this on a different time base than the queue check, every COUNTER_VAL msec		
			// if there is space on the command queue, send a new print request to the M0
			MAKE_WRITE_CMD_HEADER(writeCommand,TSK_SLAVE_CONTROL,PRINT_NUM_MESSAGES);
			writeCommand.param = numData_Messages;

			do {
				status = IPC_masterPushCmd((cmdToken*)&writeCommand);
				if(QFULL == status) qFull++;
				if(QERROR == status) qError++;
			} while(status != QINSERT);
				
			// now notify the master
			IPC_cmdNotifySlave();
							
			sentCmds++;

			// refresh the software counter
			counter = COUNTER_VAL;
		};
		
	};

}




void SysTick_Handler (void) 					
{           
	if(u32Milliseconds > 0)
	{
		u32Milliseconds--; 
	}
	else
	{
		#if (PLATFORM == HITEX_BOARD)					
		LPC_GPIO_PORT->NOT[4] |= (1UL << 15);    
		#endif
		#if (PLATFORM == KEIL_BOARD)								
		LPC_GPIO_PORT->NOT[4] |= (1UL << 12); // P 4.12   
		#endif
		u32Milliseconds = 1000;	
	}
}



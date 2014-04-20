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

#include "ipc_mbx.h"

#include "platform_check.h"

#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "ctype.h"
#include "debug_frmwrk.h"

static volatile uint32_t u32Milliseconds = 1500;
static volatile uint32_t counter;

uint8_t slaveStarted = 0;

uint32_t numData_Messages = 0;
uint32_t numDataA_Messages = 0;
uint32_t numDataB_Messages = 0;
uint32_t numDataC_Messages = 0;

mbxParam_t parameterA = 0;
mbxParam_t parameterB = 0;
mbxParam_t parameterC = 0;

msgId_t idA;
msgId_t idB;
msgId_t idC;

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

void applicationCode (void) 
{

	uint32_t lenght = 0;
	uint32_t i;
	char *c, *paramString;

	// just in case
	IPC_haltSlave();
		
	/* setup the local master mailbox system */
	IPC_initMasterMbx(&Master_CbackTable[0], (Mbx*) MASTER_MBX_START, (Mbx*) SLAVE_MBX_START);
		
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
	while(mbxFlags[MASTER_MBX_CMD] != MSG_PENDING) __WFE();

	if(NOTIFY_SLAVE_STARTED == IPC_getMsgType(MASTER_MBX_CMD)) {
		
		slaveStarted = 1;   /* just track the M0 is alive */

		/* free our mbx */
		IPC_freeMbx(MASTER_MBX_CMD);

		// if there is space on the remote mailbox, send a new msg to the M0
		if(IPC_queryRemoteMbx(SLAVE_MBX_CMD) == READY) {
			IPC_sendMsg(SLAVE_MBX_CMD, PRINT_WELCOME_MESSAGE, (msgId_t) 0xF, (mbxParam_t) &welcomeMessage);	
		};	

	};
	
	counter = COUNTER_VAL;
			
	while(1)
	{

		/* wait for a message to arrive, wake up every msec due to systick */
		__WFI();
		if(counter > 0) {
			
			counter--; 
			
			//////////////////////////////////////////////////////
			// check if we got a data  message from M0 for task A
			//////////////////////////////////////////////////////
			if(mbxFlags[MASTER_MBX_TASKA] == MSG_PENDING)
			{
				IPC_lockMbx(MASTER_MBX_TASKA);
	
				/* check the query we got */
				if(REQUEST_PROCESS_DATA == IPC_getMsgType(MASTER_MBX_TASKA))
				{
					idA = IPC_getMsgId(MASTER_MBX_TASKA);				
					parameterA = IPC_getMbxParameter(MASTER_MBX_TASKA);
					
					// convert to uppercase
					memset(&lowerToUpper[0], 0, 41); 
					lenght = strlen((char*) parameterA);
					paramString = (char*) parameterA; 

					for(i=0;i<lenght;i++)  {
						  c = (char*) paramString;
						  lowerToUpper[i] = toupper(*c);
						  c++;
						  paramString++;
					}
					
				

					numDataA_Messages++;
					numData_Messages++;
				};
	
				IPC_freeMbx(MASTER_MBX_TASKA);
	
				// if there is space on the remote mailbox, send a new msg to the M0
				if(IPC_queryRemoteMbx(SLAVE_MBX_TASKD) == READY) {
						IPC_sendMsg(SLAVE_MBX_TASKD, DATA_RESULT, (msgId_t) 0x1, (mbxParam_t) &lowerToUpper[0]);	
				};	
			};

			//////////////////////////////////////////////////////
			// check if we got a data  message from M0 for task B
			//////////////////////////////////////////////////////
			if(mbxFlags[MASTER_MBX_TASKB] == MSG_PENDING)
			{
				IPC_lockMbx(MASTER_MBX_TASKB);
	
				/* check the query we got */
				if(REQUEST_PROCESS_DATA == IPC_getMsgType(MASTER_MBX_TASKB))
				{
					idB = IPC_getMsgId(MASTER_MBX_TASKB);				
					parameterB = IPC_getMbxParameter(MASTER_MBX_TASKB);
					
					power3 = (uint32_t) pow(parameterB, 3);
					
					numDataB_Messages++;
					numData_Messages++;
				};
	
				IPC_freeMbx(MASTER_MBX_TASKB);
	
				// if there is space on the remote mailbox, send a new msg to the M0
				if(IPC_queryRemoteMbx(SLAVE_MBX_TASKE) == READY) {
						IPC_sendMsg(SLAVE_MBX_TASKE, DATA_RESULT, (msgId_t) 0x2, (mbxParam_t) power3);	
				};	
			};
			
			
			//////////////////////////////////////////////////////
			// check if we got a data  message from M0 for task C
			//////////////////////////////////////////////////////
			if(mbxFlags[MASTER_MBX_TASKC] == MSG_PENDING)
			{
				IPC_lockMbx(MASTER_MBX_TASKC);
	
				/* check the query we got */
				if(REQUEST_PROCESS_DATA == IPC_getMsgType(MASTER_MBX_TASKC))
				{
					idC = IPC_getMsgId(MASTER_MBX_TASKC);				
					parameterC = IPC_getMbxParameter(MASTER_MBX_TASKC);
					
					// reverse the string
					memset(&reversed[0], 0, 41); 
					lenght = strlen((char*) parameterC);
					paramString = (char*) parameterC; 
					c = (char*) paramString;
					for(i=0;i<lenght;i++)  {
						  
						  reversed[lenght - i - 1] = *c;
						  c++;
					}

					numDataC_Messages++;
					numData_Messages++;
				};
	
				IPC_freeMbx(MASTER_MBX_TASKC);
	
				// if there is space on the remote mailbox, send a new msg to the M0
				if(IPC_queryRemoteMbx(SLAVE_MBX_TASKF) == READY) {
						IPC_sendMsg(SLAVE_MBX_TASKF, DATA_RESULT, (msgId_t) 0x3, (mbxParam_t) &reversed[0]);	
				};	
			};	

//		 	__WFI();
//
		}
		else {
			
			// do this on a different time base than the mailbox check, every COUNTER_VAL msec		
			// if there is space on the remote mailbox, send a new print request to the M0
			if(IPC_queryRemoteMbx(SLAVE_MBX_CMD) == READY) {
					IPC_sendMsg(SLAVE_MBX_CMD, PRINT_NUM_MESSAGES, (msgId_t) 0xA, (mbxParam_t) numData_Messages);	
			};			

			// refresh the software counter
			counter = COUNTER_VAL;
		};
	}

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



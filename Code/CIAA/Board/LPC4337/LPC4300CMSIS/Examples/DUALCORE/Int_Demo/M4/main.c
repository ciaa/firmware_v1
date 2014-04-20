/*****************************************************************************
 * $Id:: main.c 9200 2012-02-22 08:44:47Z nxp28536                           $
 *
 * Project: Interrupt M4<->M0 Application Example for LPC43xx
 *
 * Description:
 *   Implements an Interrupt application
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
#ifndef IPC_MASTER
	  #error "Build Error: please define IPC_MASTER in the project settings" 
#endif

#include "platform_init.h"
#include "ipc_int.h"
#include "platform_check.h"



static volatile uint32_t u32Milliseconds;
static volatile uint32_t counter;
static volatile uint32_t sendMsg = 1;

uint8_t slaveStarted = 0;

#define CLOCK_SPEED (204000000UL)


/* used to place the CM0_IMAGE array within the "SLAVE IMAGE" section */
#pragma arm section rodata = "M0_SLAVE_IMAGE"

/* fromelf.exe always generates an unsigned char LR0[] type of array */
const	/* DO NOT REMOVE THIS CONST QUALIFIER, IS USED TO PLACE THE IMAGE IN M4 ROM */
#include SLAVE_IMAGE_FILE

#pragma arm section rodata
/* return to default scheme */


void applicationCode(void);

/******************************************************************************
 main application entry point 
******************************************************************************/
int main (void)  {

   	/* other library or drivers initialization calls might be done here */


	/* now the platform is ready, jump to the application */
	applicationCode();
}

/******************************************************************************
 this is the user application 
******************************************************************************/
void applicationCode(void)   {
   
	LPC_GPIO_PORT->NOT[4] = (1UL << 12);    

	IPC_haltSlave();

	/* setup the local master interrupt system */
	IPC_masterInitInterrupt(masterInterruptCallback);

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
	
	/* generate interrupts at 1Khz / 1msec */
	SysTick_Config(CLOCK_SPEED/1000); 
	
	/* wait for the M0 to signal being ready via a message */
	while(intFlag != MSG_PENDING) __WFE();

	slaveStarted = 1;   /* just track the M0 is alive */

	/* acknowledge the interrupt */
	IPC_resetIntFlag();
	
	while(1)
	{
		/* check if we got a message from M0 */
		if(intFlag == MSG_PENDING)
		{
			/* blink when we got the response */				
			#if (PLATFORM == HITEX_BOARD)								
			LPC_GPIO_PORT->NOT[4] |= (1UL << 15); // P 4.15   
			#endif

			#if (PLATFORM == KEIL_BOARD)								
			LPC_GPIO_PORT->NOT[4] |= (1UL << 12); // P 4.12   
			#endif

			IPC_resetIntFlag();
		};

		/* periodically send an interrupt */
		if(sendMsg) {
	
			IPC_sendInterrupt();	
			sendMsg = 0;
		};

		/* wait for event, either systick or interrupt */
		__WFE();	  
	};
}


/* systick handler is used just as a down counter */
void SysTick_Handler (void) 					
{           
	if(u32Milliseconds > 0)
	{
		u32Milliseconds--; 
	} 
	else {
		sendMsg = 1;
		u32Milliseconds = 500;
	}

}



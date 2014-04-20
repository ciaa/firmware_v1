/*********************************************************************** 
 * $Id: main.c 9200 2012-02-22 08:44:47Z nxp28536 $
 * 
 * Project: Interrupt M4<->M0 Application Example for LPC43xx 
 * 
 * Description: Implements an Interrupt application
 * 
 * Copyright(C) 2012, NXP Semiconductor
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
#ifndef IPC_SLAVE
	  #error "Build Error: please define IPC_SLAVE in the project settings" 
#endif

#include "LPC43xx.h"
#include "lpc43xx_scu.h"
#include "lpc_types.h"


/* ipc comms APIs */
#include "ipc_int.h"
#include "platform_check.h"


static volatile uint32_t u32Milliseconds = 1000;
static volatile uint32_t counter;
static volatile uint32_t sendResponse = 0;

#define COUNTER_VAL 10000

void initRiTimer(uint32_t timerInterval);

#define CLOCK_SPEED (204000000)


/*****************************************************************************
 Main Function  
*****************************************************************************/
int main(void) {		

		
	/* SysTick is not implemented, need to use another timer like RITIMER if needed */
	/* for periodic timings */
	// initRiTimer(CLOCK_SPEED/1000);

	/* init local interrupt system */
	IPC_slaveInitInterrupt(slaveInterruptCallback);

	/* signal back to M4 we are now ready */
	IPC_sendInterrupt();

   	counter = COUNTER_VAL;

	while(1)
	{					
		/* check for interrupt pending */
		if(intFlag == MSG_PENDING)
		{
			#if (PLATFORM == HITEX_BOARD)			
			LPC_GPIO_PORT->NOT[4] = (1UL << 1);	// P.4.1
			#endif

			#if (PLATFORM == KEIL_BOARD)			
			LPC_GPIO_PORT->NOT[4] = (1UL << 13);	// P.4.13
			#endif

			sendResponse = 1;

			IPC_resetIntFlag();
		}

		/* query if we can send back a response to M4 */
		if(sendResponse) {

			IPC_sendInterrupt();
			sendResponse = 0;
		};

		/* wait for the next interrupt to arrive */
		__WFE();
			
	}
		
}



void RIT_IRQHandler (void) 					
{           
	// Clear interrupt flag
	LPC_RITIMER->CTRL |=(1<<0);	
	
	if(u32Milliseconds > 0)
	{
		u32Milliseconds--; 
	}
	else
	{
		u32Milliseconds = 1000;
	}
}

void initRiTimer(uint32_t TimerInterval)
{
	// Set match value
	LPC_RITIMER->COMPVAL = TimerInterval; 			
	
	LPC_RITIMER->COUNTER=0;							
	
	// Enable, enable break, reset on mask compare, clear interrupt flag
	LPC_RITIMER->CTRL = (1<<3)|(1<<2)|(1<<1);		

	NVIC_EnableIRQ(M0_RITIMER_OR_WWDT_IRQn);

}

/*****************************************************************************
**                            End Of File
*****************************************************************************/

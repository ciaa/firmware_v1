/**********************************************************************
* $Id$		SCT_TrafficLight.c	2011-06-02
*//**
* @file		SCT_TrafficLight.c
* @brief	This example describes how to use the SCT peripheral to 
*           simulate a simple traffic light with LEDs
* @version	1.0
* @date		12. January. 2012
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
#include "lpc43xx_sct.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"
#include "sct_fsm.h"
#include "lpc43xx_libcfg.h"





/************************** PRIVATE DEFINITIONS *************************/

/************************** PRIVATE VARIABLES *************************/


/************************** PRIVATE FUNCTIONS *************************/


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/

#define MCSEL_BIT			23 
#define MCSEL_PORT			6
#include "stdlib.h"
static uint16_t delay;
extern void Hitex_CGU_Init(void);

int c_entry (void) {                       /* Main Program                       */


	Hitex_CGU_Init();

	// enable signals on MC connector X19
	scu_pinmux (0xD, 9, MD_PUP, FUNC4);   /* PD_9:  GPIO 6.23, MCSEL */
	GPIO_SetDir(MCSEL_PORT,(1<<MCSEL_BIT), 1);
	GPIO_ClearValue(MCSEL_PORT,(1<<MCSEL_BIT));


	/* Pin configuration for SCT */
	/* PD_13:  SCTIN_0 used for the CROSS_REQUEST input (button) */
	scu_pinmux (0xD, 13, MD_PDN | MD_EZI, FUNC1);  			

	/* PD_10:  SCTIN_1, used for SCT clock input */
	/* connect JP13 pin 1 with connector X19  pin2 */ 
	scu_pinmux (0xD, 10, MD_PUP | MD_EZI | MD_EHS, FUNC1);  

	scu_pinmux (0xE, 6, MD_PLN, FUNC1);   /* PE_6:  SCTOUT_2 connected to RGB green */	
	scu_pinmux (0xE, 5, MD_PLN, FUNC1);   /* PE_5:  SCTOUT_3 connected to RGB red */	   
	scu_pinmux (0xE, 8, MD_PLN, FUNC1);   /* P4_4:  SCTOUT_4 connected to RGB blue */	   
	scu_pinmux (0xE, 7, MD_PLN, FUNC1);   /* P4_3:  SCTOUT_5 */
	scu_pinmux (0xD, 3, MD_PLN, FUNC1);   /* PD_3:  SCTOUT_6 */

	/* Global configuration of the SCT */
	// use external clock, rising edge, on CTIN1
	// external clock is CPU clock /256 = 180 MHz / 256 = 703.125 KHz
	LPC_SCT->CONFIG = SCT_CONFIG_16BIT_COUNTER | (0x2 << 1) | (0x2 << 3);  

	SCT_ControlSet(SCT_CTRL_CLRCTR_L|SCT_CTRL_HALT_L|SCT_CTRL_PRE_L(256-1)
					|SCT_CTRL_HALT_H|SCT_CTRL_CLRCTR_H|SCT_CTRL_PRE_H(256-1),
					ENABLE);

	/* Now use the FSM code to configure the state machine */
	sct_fsm_init();

	/* initialize random seed: */
  	srand(NULL);

  	/* generate delay from 1 to 23 seconds for the next start */
	/* 180000000 / 256 / 256 = 2746 Hz */
	/* 65535 / 2746 =~ 23 */
	/* generate a random delay from 1 to 23 seconds */
  	delay = (rand() % 23 + 1) * 2746;

	NVIC_ClearPendingIRQ(SCT_IRQn);
	NVIC_EnableIRQ(SCT_IRQn);

	/* Start the SCT */
	SCT_ControlSet(SCT_CTRL_STOP_L|SCT_CTRL_HALT_L|SCT_CTRL_STOP_H|SCT_CTRL_HALT_H,DISABLE);

	while(1)
		__WFI();
}

/******************************************************************************
 * SCT interrupt handler
 * 
 *
 *  
 ******************************************************************************/
void SCT_IRQHandler (void)
{
    /* Acknowledge the interrupt source */
    if(LPC_SCT->EVFLAG & (1 << SCT_IRQ_EVENT_crossingTimeout)) {

		LPC_SCT->EVFLAG |= (1 << SCT_IRQ_EVENT_crossingTimeout);
    
    	/* determine the captured RPM measurement */
    	delay = (rand() % 23 + 1) * 2746;

		reload_cross_timeout(delay);
	};
	
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

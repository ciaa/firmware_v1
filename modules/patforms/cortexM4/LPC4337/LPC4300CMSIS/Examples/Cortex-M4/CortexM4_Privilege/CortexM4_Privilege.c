/**********************************************************************
* $Id$		CortexM3_Privilege.c		2011-06-02
*//**
* @file		CortexM3_Privilege.c
* @brief	This example used to test privilege feature of Cortex-M3
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
#include "lpc43xx_gpio.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_libcfg.h"
#include "debug_frmwrk.h"


/* Example group ----------------------------------------------------------- */
/** @defgroup CortexM3_Privilege	CortexM3_Privilege
 * @ingroup Cortex_M3_Examples
 * @{
 */

/************************** PRIVATE DEFINTIONS*************************/
/* CONTROL register controls the privileged level for software execution
 * when the processor is in Thread mode
 */
#define	THREAD_PRIVILEGED			0
#define THREAD_UNPRIVILEGED			1
#define THREAD_PRIVILEDGED_MASK		0xFFFFFFFE


/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"Privileged demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART0 - 115200 bps \n\r"
	"This example used to test Privileged feature of Cortex-M3 processor\n\r"
	"********************************************************************************\n\r";

/************************** SYSTEM CALL EXCEPTION *********************/
#if defined ( __CC_ARM   )
__ASM void __SVC(void)
{
  SVC 0x01
  BX R14
}
#elif defined ( __ICCARM__ )
static __INLINE  void __SVC()                     { __ASM ("svc 0x01");}
#elif defined   (  __GNUC__  )
static __INLINE void __SVC()                      { __ASM volatile ("svc 0x01");}
#endif
/************************** PRIVATE FUNCTIONS *************************/
void SVC_Handler(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
  * @brief  SVC Hander
  * @param  None
  * @retval None
 **********************************************************************/
void SVC_Handler(void)
{
  /* Change Thread mode to privileged */
  __set_CONTROL(2);
}


/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: main function
 * @param[in]	none
 * @return 		int
 **********************************************************************/
int c_entry(void)			/* Main Program */
{
	uint32_t priviledge_status;

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

	/* Thread mode is privilege out of reset */
	/* First checking Thread mode is privilege or un-privilege */
	priviledge_status = __get_CONTROL();
	if((priviledge_status & (~THREAD_PRIVILEDGED_MASK))==THREAD_PRIVILEGED)
	{
		_DBG_("Thread mode is privileged!");
	}
	else
	{
		_DBG_("Theard mode is unprivileged! It's not compliant with Cortex-M3 Technical");
		while(1); //Error loop
	}
	/* Wait user press '1' character to change to un-privilege mode */
	_DBG_("Press '1' to change to unprivilege mode ...\n\r");
	while(_DG !='1');

	/* Changing to un-privilege mode */
	__set_CONTROL((priviledge_status & THREAD_PRIVILEDGED_MASK)|THREAD_UNPRIVILEGED);
	_DBG_("Changed to unprivilege mode!");

	/* Check */
	priviledge_status = __get_CONTROL();
	if((priviledge_status & ~THREAD_PRIVILEDGED_MASK)==THREAD_UNPRIVILEGED)
	{
		_DBG_("Check: Thread mode change to unprivilege successful!");
	}
	else
	{
		_DBG_("Check: Thread mode is still privileged! ERROR...");
		while(1); //Error loop
	}
	/* Wait user press '1' character to change to un-privilege mode */
	_DBG_("Press '2' to change to privilege mode by calling system call exception...\n\r");
	while(_DG !='2');
	/* Call system call exception to re-change Thread mode into privilege */
	__SVC();
	_DBG_("Called system call exception!");

	/* Check */
	priviledge_status = __get_CONTROL();
	if((priviledge_status & ~THREAD_PRIVILEDGED_MASK)==THREAD_PRIVILEGED)
	{
		_DBG_("Check: Thread mode change to privilege successful!");
	}
	else
	{
		_DBG_("Check: Theard mode is still unprivileged! ERROR...");
		while(1); //Error loop
	}
	_DBG_("Demo terminate!");
	while (1);
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

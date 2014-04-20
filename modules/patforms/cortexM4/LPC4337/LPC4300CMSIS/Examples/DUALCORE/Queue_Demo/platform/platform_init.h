/**********************************************************************
* $Id$		platform_init.h			2012-03-16
*//**
* @file		platform_init.h
* @brief	LPC43xx Dual Core Interrupt configuration 
* @version	1.0
* @date		03. March. 2012
* @author	NXP MCU SW Application Team
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
#ifndef __PLATFORM_CONFIG_H 
#define __PLATFORM_CONFIG_H

#include "stdint.h"
#include "project_init.h"

/****************************************************/
/* USER CONFIGURATION SECTION						*/
/****************************************************/

/* choose the platform you want to build against 	*/
#define PLATFORM KEIL_BOARD

/* choose the device you want to build against 	*/
#define DEVICE	LPC43xx

/* define which boot mode is used */
#ifdef BOOT_FROM_NOR
#define MASTER_BOOT_MODE	(NOR_BOOT)
#elif BOOT_FROM_SPIFI
#define MASTER_BOOT_MODE	(SPIFI_BOOT)
#else
#define MASTER_BOOT_MODE	(OTHER_BOOT)
#endif


/* specify the filename used for the slave image */
#define SLAVE_IMAGE_FILE "CM0_image.c"

/* specify the maximum number of items which the buffer can hold before getting full */
/* messages might use 1 or multiple items in case of additional parameters */
#define MASTER_CMDBUF_SIZE		(10)

/* configure which priority the IPC interrupt should have on the host (M4) side */
/* uses cmsis definition, priority from 0 to 7  */
#define MASTER_QUEUE_PRIORITY		(0)

/* specify the maximum number of items which the buffer can hold before getting full */
/* messages might use 1 or multiple items in case of additional parameters */
#define SLAVE_MSGBUF_SIZE		(10)

/* configure which priority the IPC interrupt should have on the slave (M0) side */
/* uses cmsis definition, priority from 0 to 7 */
#define SLAVE_QUEUE_PRIORITY		(0)

/* these addresses specify where the IPC queues shall be located */
#define MASTER_CMD_BLOCK_START	0x20008000
#define SLAVE_MSG_BLOCK_START	0x2000A000

#ifdef CORE_M4


/* defined if BOOT_FROM_SPIFI is set as build rule */
#if (BOOT_MODE == SPIFI_BOOT)

	/* slave runs from internal ram @ 0x10080000 */
	#define SLAVE_ROM_START	0x10080000
	
	/* relocate vector table in ram? */
	/* note: (only meaningful for standalone flash projects) */
	#define RELOCATE_IRQ_TABLE	(YES)

/* defined if BOOT_FROM_NORFLASH is set as build rule */
#elif (BOOT_MODE == NOR_BOOT)

	/* slave runs from internal ram @ 0x10080000 */
	#define SLAVE_ROM_START	0x10080000
	
	/* relocate vector table in ram? */
	/* note: (only meaningful for standalone flash projects) */
	#define RELOCATE_IRQ_TABLE	(YES)

#elif (BOOT_MODE == FLASH_BOOT)
	
	/* slave runs from internal flash @ 0x1B000000 */
	#define SLAVE_ROM_START	0x1B000000
	
	/* relocate vector table in ram? */
	/* note: (only meaningful for standalone flash projects) */
	#define RELOCATE_IRQ_TABLE	(NO)	
	
#else 

	/* slave runs from internal ram @ 0x10080000 */
	#define SLAVE_ROM_START	0x10080000
	
	/* executes already from ram, do not change */
	#define RELOCATE_IRQ_TABLE	(NO)

#endif


#endif	  /* ifdef CORE_M4 */


/* definition of task types */

// tasks running on the Master
#define TSK_MASTER_CONTROL		0x1000
#define NOTIFY_SLAVE_STARTED 	0x01

#define TASK_MASTER_A	0x0010
#define TASK_MASTER_B	0x0020
#define TASK_MASTER_C	0x0030


#define TASK_A_SERVICE	0x10
#define TASK_B_SERVICE	0x20
#define TASK_C_SERVICE	0x30

/* tasks running on the Slave */
// control task running on slave
#define TSK_SLAVE_CONTROL		0x2000
// arguments for TSK_SLAVE_CONTROL
#define PRINT_NUM_MESSAGES		0x02
#define PRINT_WELCOME_MESSAGE	0x03

#define TASK_D	0x0050
#define TASK_D_ANSWER	0x40

#define TASK_E	0x0060	
#define TASK_F	0x0070




#define TASK_E_ANSWER	0x50
#define TASK_F_ANSWER	0x60

/****************************************************/
/* END OF USER CONFIGURATION 						*/
/* DO NOT EDIT BELOW THIS LINE						*/
/****************************************************/





/* assign the roles for the devices */
#if (DEVICE==LPC43xx)

#include "LPC43xx.h"

/* these definitions are used in case the interrupt vector table */
/* gets relocated into internal ram */
/* total of system + user defined interrupts, 68 vectors in total */
#define LPC4300_SYSTEM_INTERRUPTS 	(16)
#define LPC4300_USER_INTERRUPTS 	(53)
#define LPC4300_TOTAL_INTERRUPTS 	(LPC4300_SYSTEM_INTERRUPTS + LPC4300_USER_INTERRUPTS)

#define MASTER_CPU 	CORE_M4
#define SLAVE_CPU 	CORE_M0

#define MASTER_IRQn (1)
#define SLAVE_IRQn 	(1)

#define MASTER_TXEV_QUIT()	{ LPC_CREG->M4TXEVENT = 0x0; }
#define SLAVE_TXEV_QUIT() 	{ LPC_CREG->M0TXEVENT = 0x0; }

#define SET_SLAVE_SHADOWREG(n) {LPC_CREG->M0APPMEMMAP = n; }

#define MASTER_IPC_TABLE	MASTER_MBX_START
#define SLAVE_IPC_TABLE		SLAVE_MBX_START

#endif

/****************************************************/
/* platform wise initialization function 			*/
/****************************************************/
void platformInit(void);



#endif /* __PLATFORM_CONFIG_H */


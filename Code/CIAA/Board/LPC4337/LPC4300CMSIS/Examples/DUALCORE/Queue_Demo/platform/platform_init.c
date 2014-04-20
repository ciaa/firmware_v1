/**********************************************************************
* $Id$		platform_int.c			2012-03-16
*//**
* @file		platform_init.c
* @brief	LPC43xx platform initialization 
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
#include "LPC43xx.h"
#include "fpu_enable.h"
#include "platform_init.h"


#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_emc.h"


/**********************************************************************
 ** Function prototypes
 **********************************************************************/

/* this function copies the system setup code into internal ram */
void copyStartupCode2IntRam(void) __attribute__ ((section("INIT_CODE_COPY")));
 
/* this function relocated the interrupt vector table to internal ram */
/* also configures the VTOR in NVIC */
#if (RELOCATE_IRQ_TABLE == YES)
void copyIntVectorTable2Ram(void) __attribute__ ((section("VECTOR_TABLE_COPY")));
#endif

/* include board specific setup functions */
#if (PLATFORM == HITEX_BOARD)

	#include "hitex_board_init.h" 

#endif

#if (PLATFORM == KEIL_BOARD)

	#include "keil_board_init.h" 

#endif

/* this declaration is used to initialize the platform with system level settings */
const uint32_t CGU_ClockSourceFrequencyDefault[CGU_CLKSRC_NUM] = {0,12000000,0,0,0,0, 0, 480000000,0,0,0,0,0,0,0,0,0};
extern uint32_t CGU_ClockSourceFrequency[CGU_CLKSRC_NUM];

/* this function is called at startup before main, to initialize the platform */
void platformInit(void) {
	
	int i;
	
	/* manually initialize the structure, since this will not be done by uLib scatter loading */
	/* it is placed in a "UNINIT" section by purpose! */
	for(i=0;i<CGU_CLKSRC_NUM; i++) 
		CGU_ClockSourceFrequency[i] = CGU_ClockSourceFrequencyDefault[i];
	
	/* setup the clock */
	clockInit();

	#if (INITIALIZE_EMC == YES)
	 
	/* configure EMC if used */
	externalMemoryInit();
	
	#endif

	#if (EXT_SPIFI_FLASH == YES) 
	 
	/* configure SPIFI if used */
	externalSpifiInit();
	
	#endif
	
    #if (RELOCATE_IRQ_TABLE == YES)
	
	/* move interrupt vector table to ram if needed */
	copyIntVectorTable2Ram(); 
	
	#endif

	/* configure the io */
	ioInit();

}

//#if (INITIALIZE_EMC == YES)
/* these define the locations of the system code copied to ram */
extern unsigned long Image$$ER_IRAM_SYSTEM_CODE$$Base;
extern unsigned long Load$$ER_IRAM_SYSTEM_CODE$$RO$$Base;
extern unsigned long Load$$ER_IRAM_SYSTEM_CODE$$RO$$Limit;

/* these define the locations of the system code copied to ram */
extern unsigned long Image$$ER_IRAM_INIT_CODE$$Base;
extern unsigned long Load$$ER_IRAM_INIT_CODE$$RO$$Base;
extern unsigned long Load$$ER_IRAM_INIT_CODE$$RO$$Limit;
// #endif

/* this copies the system and the platform configuration code into ram */
/* needed for reconfiguring the external memory interfaces */
void copyStartupCode2IntRam(void)
{
	unsigned long *pulSrc;
	
	unsigned long *pulSrcBase = (unsigned long*) &Load$$ER_IRAM_SYSTEM_CODE$$RO$$Base;
    unsigned long *pulSrcLimit = (unsigned long*) &Load$$ER_IRAM_SYSTEM_CODE$$RO$$Limit;
    unsigned long *pulRamBase = (unsigned long*) &Image$$ER_IRAM_SYSTEM_CODE$$Base;

    unsigned long *relocBase = pulRamBase;

    /* copy the "system code" section over to ram */
    /* ER_IRAM_SYSTEM_CODE */
	for(pulSrc = pulSrcBase; pulSrc < pulSrcLimit; )
    {
        *relocBase++ = *pulSrc++;
    }


    /* copy the "initialization code" over to ram */
	/* ER_IRAM_INIT_CODE */
 	pulSrcBase = (unsigned long*) &Load$$ER_IRAM_INIT_CODE$$RO$$Base;
    pulSrcLimit = (unsigned long*) &Load$$ER_IRAM_INIT_CODE$$RO$$Limit;
    pulRamBase = (unsigned long*) &Image$$ER_IRAM_INIT_CODE$$Base;

    relocBase = pulRamBase;

    for(pulSrc = pulSrcBase; pulSrc < pulSrcLimit; )
    {
        *relocBase++ = *pulSrc++;
    }


}




#if (RELOCATE_IRQ_TABLE == YES)

/* these two symbols define the boundaries of the interrupt vector table */
extern unsigned long Image$$ER_FLASH_ROM_RESET_VECTOR_TABLE$$Base;
extern unsigned long Image$$ER_FLASH_ROM_RESET_VECTOR_TABLE$$Limit;

extern unsigned long Image$$ER_IRAM_VECTOR_TABLE$$Base;
/* have to take into account all existing user interrupts + reset vector */
unsigned int ramInterruptVectorTable[LPC4300_TOTAL_INTERRUPTS] 
__attribute__ ((section("IRAM_ISR_TABLE"), zero_init, used));
#endif

/* this routine relocates the interrupt vector table to internal ram 
 memory at address M4_RAM_START 
 it reserves space for a number of interrupt vectors defined in platform_config.h
*/
#if (RELOCATE_IRQ_TABLE == YES)
void copyIntVectorTable2Ram(void)
{

	unsigned long *pulSrc, *relocBase;

    unsigned long *pulRamBase = (unsigned long *) &Image$$ER_IRAM_VECTOR_TABLE$$Base;
	unsigned long *pulSrcBase = (unsigned long *) &Image$$ER_FLASH_ROM_RESET_VECTOR_TABLE$$Base;
    unsigned long *pulSrcLimit = (unsigned long *) &Image$$ER_FLASH_ROM_RESET_VECTOR_TABLE$$Limit;

    /* copy the table over */
    for(pulSrc = pulSrcBase, relocBase = pulRamBase; pulSrc < pulSrcLimit; )
    {
        *relocBase++ = *pulSrc++;
    }

	/* disable interrupts for safety while switching the VTOR */
	__disable_irq();

    SCB->VTOR = (uint32_t) pulRamBase;
	__ISB();
	
	__enable_irq();
}

#endif



#ifndef __PLATFORM_CHECK_H
#define __PLATFORM_CHECK_H

#warning "************ PLATFORM CONFIG ************"

/* this is used to check the build platform */
#if (PLATFORM == HITEX_BOARD)
	#warning "HITEX LPC4350EVA platform ***"	

#elif (PLATFORM == KEIL_BOARD)
	#warning "KEIL MCB4357 platform ***"	
	
#else
	#error "!!! Platform not supported, check platform_init.h !!!"
#endif

/* this is used to check the build device */
#if (DEVICE == LPC43xx) 
	#warning "LPC43xx device"	
#else
	#error "!!! Device not supported, check platform_init.h !!!"
#endif

#ifdef IPC_MASTER

/* this is to ensure memory ranges are defined for the slave */
#ifndef SLAVE_ROM_START
	#error "!!! M0_ROM_START not defined, check platform_init.h !!!"
#endif


/* check the build rules */
#if (EXT_NOR_FLASH == YES)
	#warning "Parallel NOR flash support: YES"
#if (RELOCATE_IRQ_TABLE == YES)
	#warning "Relocate IRQ table in ram: YES"
#elif  (RELOCATE_IRQ_TABLE == NO)
	#warning "Relocate IRQ table in ram: NO"
#endif

#elif (EXT_NOR_FLASH == NO)
	#warning "Parallel NOR flash support: NO"
#endif

#if (EXT_SPIFI_FLASH == YES)
	#warning "SPIFI support: YES"
#if (RELOCATE_IRQ_TABLE == YES)
	#warning "Relocate IRQ table in ram: YES"
#elif  (RELOCATE_IRQ_TABLE == NO)
	#warning "Relocate IRQ table in ram: NO"
#endif
#elif (EXT_SPIFI_FLASH == NO)
	#warning "SPIFI support: NO"
#endif

#if (EXT_SDRAM == YES)
	#warning "SDRAM support: YES"
#if (RELOCATE_IRQ_TABLE == YES)
	#warning "Relocate IRQ table in ram: YES"
#elif  (RELOCATE_IRQ_TABLE == NO)
	#warning "Relocate IRQ table in ram: NO"
#endif
#elif (EXT_SDRAM == NO)
	#warning "SDRAM support: NO"
#endif

#endif /* IPC_MASTER */

#warning "************ PLATFORM CONFIG ************"



#endif /* platform check */





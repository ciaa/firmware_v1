#ifndef __PROJECT_INIT_H 
#define __PROJECT_INIT_H

#include "stdint.h"

/****************************************************/
/* supported platforms list							*/
/* DO NOT CHANGE THESE DEFINITIONS 					*/
#define HITEX_BOARD          (1)
#define KEIL_BOARD			 (2)
/****************************************************/

/****************************************************/
/* supported devices list							*/
/* DO NOT CHANGE THESE DEFINITIONS 					*/
#define LPC43xx	(1)
/****************************************************/

/****************************************************/
/* used for the configuration checks */
/* DO NOT CHANGE THESE DEFINITIONS 					*/
/****************************************************/
#define YES	(1)
#define NO	(2)

/****************************************************/
/* used for the boot mode checks */
/* DO NOT CHANGE THESE DEFINITIONS 					*/
/****************************************************/
#define NOR_BOOT	(1)
#define SPIFI_BOOT	(2)
#define FLASH_BOOT  (3)
#define OTHER_BOOT	(4)

/****************************************************/
/* supported speed configurations for cpu & emc		*/
/* DO NOT CHANGE THESE DEFINITIONS 					*/
/****************************************************/

typedef struct norFlashConfig_tag {

	uint32_t waitRd;
	uint32_t waitOutEn;

} norFlashConfig;


typedef struct sdramConfig_tag {

	uint8_t rasCas; // TBD

} sdramConfig;


#ifdef USE_SPIFI_FLASH
#define	EXT_SPIFI_FLASH	(YES)
#else 
#define EXT_SPIFI_FLASH	(NO)
#endif

/* these definitions are being taken from the project build rules */
/* BOOT_FROM_NORFLASH, BOOT_FROM_SPIFI */
#ifdef BOOT_FROM_NORFLASH
	#define BOOT_MODE	NOR_BOOT
#elif BOOT_FROM_SPIFI
	#define BOOT_MODE	SPIFI_BOOT
	#ifdef EXT_SPIFI_FLASH
		#undef EXT_SPIFI_FLASH
		#define EXT_SPIFI_FLASH	(YES)
	#endif
#elif BOOT_FROM_INTERNAL_FLASH
	#define BOOT_MODE FLASH_BOOT
#else
	#define BOOT_MODE	OTHER_BOOT
#endif



/* these definitions are being taken from the project build rules */
/* USE_NOR_FLASH, USE SPIFI_FLASH, USE_SDRAM */
#ifdef USE_NOR_FLASH
#define	EXT_NOR_FLASH	(YES)
#else 
#define EXT_NOR_FLASH	(NO)
#endif

#ifdef USE_SDRAM
#define	EXT_SDRAM	(YES)
#else 
#define EXT_SDRAM	(NO)
#endif

#if ((EXT_NOR_FLASH == YES) || (EXT_SDRAM == YES) || (BOOT_MODE == NOR_BOOT))
#define INITIALIZE_EMC (YES)
#else
#define INITIALIZE_EMC (NO)
#endif


#endif /* __PLATFORM_CONFIG_H */


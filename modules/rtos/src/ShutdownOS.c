

/** \brief FreeOSEK ShutdownOS Implementation File
 **
 ** This file implements the ShutdownOS API
 **
 ** \file ShutdownOS.c
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * KLi          Kang Li
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090418 v0.1.5 MaCe bugfix function name to ShutdownOS
 * 20090417 v0.1.4 MaCe upadte license
 * 20090408 v0.1.3 MaCe add ShutdownOs_Arch service call
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080810 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void ShutdownOS
(
	StatusType Error
)
{
	/* \req OSEK_SYS_3.26 The system service void
	 * ShutdownOS ( StatusType Error ) shall be defined */

	/* achitecture dependent shutdown service */
	ShutdownOs_Arch();

#if (HOOK_SHUTDOWNHOOK == ENABLE)
	/* \req OSEK_ERR_1.2 The ShutdownHook hook routine shall be called when a
	 * system shutdown is requested by the application or by the operating
	 * system in case of a severe error */
	ShutdownHook();
#endif

	/* this function shall never return */
	while(1);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


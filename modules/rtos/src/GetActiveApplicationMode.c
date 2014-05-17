

/** \brief FreeOSEK Os GetActiveApplicationMode Implementation File
 **
 ** This file implements the GetActiveApplicationMode API
 **
 ** \file GetActiveApplicationMode.c
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
 * 20090128 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080713 v0.1.0 MaCe initial version
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
AppModeType GetActiveApplicationMode
(
	void
)
{
	/* \req OSEK_SYS_3.24 The system service AppModeType
	 ** GetActiveApplicationMode ( void ) shall be defined */

	/* \req OSEK_SYS_3.24.1 This service shall return the current
	 ** application mode */

	/* \req OSEK_ERR_1.1.1 The macro GetActiveApplicationMode()
	 ** shall be provided */

	return ApplicationMode;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


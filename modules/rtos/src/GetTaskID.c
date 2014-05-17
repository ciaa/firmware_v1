

/** \brief FreeOSEK GetTaskID Implementation File
 **
 ** This file implements the GetTaskID API
 **
 ** \file GetTaskID.c
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
 * 20090417 v0.1.3 MaCe update license
 * 20090128 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080713 v0.1.0 MaCe	initial version
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
StatusType GetTaskID
(
	TaskRefType TaskID
)
{
	/* \req OSEK_SYS_3.5 The system service StatusType
	 ** GetTaskID ( TaskRefType TaskID ) shall be defined */

	/* \req OSEK_SYS_3.5.1 Shall returns the information about the TaskID of
	 ** the task which is currently running */
	/* \req OSEK_SYS_3.5.2 If no task is currently running, the service returns
	 ** INVALID_TASK as TaskType */
	*TaskID = GetRunningTask();

	/* \req OSEK_SYS_3.5.3 This function returns always E_OK */
	return E_OK;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


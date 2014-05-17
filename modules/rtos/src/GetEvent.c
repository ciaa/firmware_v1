

/** \brief FreeOSEK Os GetEvent Implementation File
 **
 ** This file implements the GetEvent API
 **
 ** \file GetEvent.c
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
 * 20090330 v0.1.3 MaCe add NO_EVENTS evaluation
 * 20090128 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080814 v0.1.0 MaCe	initial version
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
#if (NO_EVENTS == DISABLE)
StatusType GetEvent
(
	TaskType TaskID,
	EventMaskRefType Event
)
{
	/* \req OSEK_SYS_3.17 The system service StatusType
	 ** GetEvent ( TaskType TaskID, EventMaskRefType Event ) shall be defined*/

	/* \req OSEK_SYS_3.17.3 Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if ( TaskID >= TASKS_COUNT )
	{
		/* \req OSEK_SYS_3.17.4-1/3 Extra possible return values in Extended mode are
		 ** E_OS_ID, E_OS_ACCESS, E_OS_STATE */
		ret = E_OS_ID;
	}
	else if ( !TasksConst[TaskID].ConstFlags.Extended )
	{
		/* \req OSEK_SYS_3.17.4-2/3 Extra possible return values in Extended mode are
		 ** E_OS_ID, E_OS_ACCESS, E_OS_STATE */
		ret = E_OS_ACCESS;
	}
	else if ( TasksVar[TaskID].Flags.State == TASK_ST_SUSPENDED )
	{
		/* \req OSEK_SYS_3.17.4-3/3 Extra possible return values in Extended mode are
		 ** E_OS_ID, E_OS_ACCESS, E_OS_STATE */
		ret = E_OS_STATE;
	}
	else
#endif
	{
		/* \req OSEK_SYS_3.17.1 This service shall return the current state of
		 ** all event bits of the task TaskID, not the events that the task is
		 ** waiting for */
		/* \req OSEK_SYS_3.17.2 The current status of the event mask of task
		 ** TaskID shall be copied to Event */
		*Event = TasksVar[TaskID].Events;
	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
		(HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-10/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-10/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_SetEvent);
		SetError_Param1(TaskID);
		SetError_Param2((size_t)Event);
		SetError_Ret(ret);
		SetError_Msg("ActivateTask returns != than E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}
#endif /* #if (NO_EVENTS == DISABLE) */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/




/** \brief FreeOSEK GetTaskState Implementation File
 **
 ** This file implements the GetTaskState API
 **
 ** \file GetTaskState.c
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
StatusType GetTaskState
(
	TaskType TaskID,
	TaskStateRefType State
)
{
	/* \req OSEK_SYS_3.6 The system service StatusType
	 ** GetTaskState ( TaskType TaskID, TaskStateRefType State ) shall be
	 ** defined. */

	/* \req OSEK_SYS_3.6.3Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	/* check if TaskID is in a valid range */
	if ( TaskID >= TASKS_COUNT )
	{
		/* TaskID is out of range */
		/* return E_OS_ID */
		/* \req OSEK_SYS_3.6.4 Extra possible return values in Extended mode is
		 ** E_OS_ID */
		ret = E_OS_ID;

		/* set invalid task value to the state */
		*State = TASK_ST_INVALID;
	}
	else
#endif
	{

		/* \req OSEK_SYS_3.6.1 The service shall returns the state of a task
		 ** (running, ready, waiting, suspended) at the time of calling
		 ** GetTaskState */
		/* \req OSEK_SYS_3.6.2 When the service is called for a task, which is
		 ** activated more than once, the state is set to running if any instance
		 ** of the task is running. */
		*State = (TaskStateType) TasksVar[TaskID].Flags.State;
	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
		(HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-5/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-5/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_GetTaskState);
		SetError_Param1(TaskID);
		SetError_Param2((size_t)State);
		SetError_Ret(ret);
		SetError_Msg("GetTaskState returns != than E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


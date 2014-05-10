/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of FreeOSEK.
 *
 * FreeOSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Linking FreeOSEK statically or dynamically with other modules is making a
 * combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of FreeOSEK give
 * you permission to combine FreeOSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with FreeOSEK solely through the FreeOSEK defined interface.
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for FreeOSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of FreeOSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 *
 * FreeOSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** \brief FreeOSEK Os ChainTask Implementation File
 **
 ** This file implements the ChainTask API
 **
 ** \file ChainTask.c
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
 * 20090418 v0.1.5 MaCe add PostTaskHook handling
 * 20090406 v0.1.4 MaCe add support to RES_SCHEDULER
 * 20090330 v0.1.3 MaCe add set actual context to SYS when chaining task
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
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
StatusType ChainTask
(
	TaskType taskid
)
{
	/* \req OSEK_SYS_3.3 The system service StatusType
	 ** ChainTask ( TaskType TaskID ) shall be defined. */


	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if ( taskid >= TASKS_COUNT )
	{
		/* \req OSEK_SYS_3.3.10-1/3 Added possible return values in Extended mode
		 ** are E_OS_ID, E_OS_RESOURCE, E_OS_CALLEVEL */
		ret = E_OS_ID;
	}
	else if ( GetCallingContext() != CONTEXT_TASK )
	{
		/* \req OSEK_SYS_3.3.10-2/3 Added possible return values in Extended mode
		 ** are E_OS_ID, E_OS_RESOURCE, E_OS_CALLEVEL */
		ret = E_OS_CALLEVEL;
	}
#if ( (RESOURCES_COUNT != 0) || (NO_RES_SCHEDULER == DISABLE) )
	/* check if any resource is still reserved for this task */
	else if (
#if (RESOURCES_COUNT != 0)
				 ( TasksVar[GetRunningTask()].Resources != 0 )
#endif /* #if (RESOURCES_COUNT != 0) */
#if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == DISABLE) )
					||
#endif /* #if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == DISABLE) ) */
#if (NO_RES_SCHEDULER == DISABLE)
				 ( TasksVar[GetRunningTask()].ActualPriority == TASK_MAX_PRIORITY )
#endif /* #if (NO_RES_SCHEDULER == DISABLE) */
			  )
	{
		/* \req OSEK_SYS_3.3.10-3/3 Added possible return values in Extended mode
		 ** are E_OS_ID, E_OS_RESOURCE, E_OS_CALLEVEL */
		ret = E_OS_RESOURCE;
	}
#endif /* #if ( (RESOURCES_COUNT != 0) || (NO_RES_SCHEDULER == DISABLE) ) */
	else
#endif
	if ( ( (TasksVar[taskid].Activations + 1) > TasksConst[taskid].MaxActivations) &&
		  ( taskid != GetRunningTask()) )
	{
		/* \req OSEK_SYS_3.3.8 If E_OS_LIMIT is returned the activation is ignored */
		/* \req OSEK_SYS_3.3.9-1/2 Possible return values in Standard mode are:
		 ** no return or E_OS_LIMIT */
		ret = E_OS_LIMIT;
	}
	else
	{

#if (HOOK_POSTTASKHOOK == ENABLE)
		PostTaskHook();
#endif /* #if (HOOK_POSTTASKHOOK == ENABLE) */

		IntSecure_Start();

		/* release internal resources */
		/* \req OSEK_SYS_3.3.4 If an internal resource is assigned to the calling
		 ** task it shall be automatically released, even if the succeeding task is
		 ** identical with the current task. */
		ReleaseInternalResources();

		/* decrement activations for this task */
		TasksVar[GetRunningTask()].Activations--;

		if (TasksVar[GetRunningTask()].Activations == 0)
		{
			/* if no more activations set state to suspended */
			/* \req OSEK_SYS_3.3.1-1/2 This service causes the termination of the calling task. */
			TasksVar[GetRunningTask()].Flags.State = TASK_ST_SUSPENDED;
		}
		else
		{
			/* if more activations set state to ready */
			/* \req OSEK_SYS_3.3.1-2/2 This service causes the termination of the calling task. */
			TasksVar[GetRunningTask()].Flags.State = TASK_ST_READY;
		}

		/* set entry point for this task again */
		/* \req OSEK_SYS_3.1.2-1/3 The operating system shall ensure that the task
         * code is being executed from the first statement. */
		SetEntryPoint(GetRunningTask());
		/* remove ready list */
		RemoveTask(GetRunningTask());
		/* set running task to invalid */
		SetRunningTask(INVALID_TASK);
		/* set actual context task */
		SetActualContext(CONTEXT_SYS);
		/* increment activations */
		TasksVar[taskid].Activations++;
		/* activate task */
		/* \req OSEK_SYS_3.3.2 After termination of the calling task a succeeding
		 **  task TaskID shall be activated. */
		/* \req OSEK_SYS_3.3.3 If the succeeding task is identical with the current
		 ** task, this does not result in multiple requests. The task is not
		 ** transferred to the suspended state, but will immediately become ready
		 ** again. */
		AddReady(taskid);

		if(TasksVar[taskid].Flags.State ==  TASK_ST_SUSPENDED)
		{
			/* \req OSEK_SYS_3.3.7 When an extended task is transferred from suspended
			 ** state into ready state all its events are cleared.*/
			TasksVar[taskid].Events = 0;
		}

		IntSecure_End();

		/* call scheduler, never returns */
		/* \req OSEK_SYS_3.3.5 If called successfully, ChainTask does not return
		 ** to the call level and the status can not be evaluated. */
		/* \req OSEK_SYS_3.3.6 If the service ChainTask is called successfully,
		 ** this enforces a rescheduling. */
		/* \req OSEK_SYS_3.3.9-2/2 Possible return values in Standard mode are:
		 ** no return or E_OS_LIMIT */
		(void)Schedule();

	}

#if (HOOK_ERRORHOOK == ENABLE)
   /* \req OSEK_ERR_1.3-3/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
   /* \req OSEK_ERR_1.3.1-3/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
	if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_ChainTask);
		SetError_Param1(taskid);
		SetError_Ret(ret);
		SetError_Msg("ChainTask returns != than E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


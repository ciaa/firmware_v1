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

/** \brief FreeOSEK Os TerminateTask Implementation File
 **
 ** This file implements the TerminateTask API
 **
 ** \file TerminateTask.c
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
 * 20090418 v0.1.7 MaCe add PostTaskHook handling
 * 20090417 v0.1.6 MaCe update license
 * 20090406 v0.1.5 MaCe add support to RES_SCHEDULER
 * 20090330 v0.1.4 MaCe set system context when terminate task
 * 20090130 v0.1.3 MaCe add OSEK_MEMMAP check
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
StatusType TerminateTask
(
	void
)
{
	/* \req OSEK_SYS_3.2 The system service StatusType TerminateTask ( void )
	 ** shall be defined. */

	StatusType ret = E_OK;

	/* \req OSEK_SYS_3.2.4 If the version with extended status is used, the
	 ** service returns in case of error, and provides a status which can be
	 ** evaluated in the application. */
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if ( GetCallingContext() != CONTEXT_TASK )
	{
		/* \req OSEK_SYS_3.2.7-1/2 Possibly return values in Extended mode are
		 ** E_OS_RESOURCE or E_OS_CALLEVEL */
		ret = E_OS_CALLEVEL;
	}

#if ( (RESOURCES_COUNT != 0) || (NO_RES_SCHEDULER == DISABLE) )
	/* check if on or more resources are ocupied */
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
		/* \req OSEK_SYS_3.2.7-2/2 Possibly return values in Extended mode are
		 ** E_OS_RESOURCE or E_OS_CALLEVEL */
		ret = E_OS_RESOURCE;
	}
#endif /* #if ( (RESOURCES_COUNT != 0) || (NO_RES_SCHEDULER == DISABLE) ) */
	else
#endif /* #if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) */
	{

#if (HOOK_POSTTASKHOOK == ENABLE)
		PostTaskHook();
#endif /* #if (HOOK_POSTTASKHOOK == ENABLE) */

		IntSecure_Start();

		/* release internal resources */
		/* \req OSEK_SYS_3.2.2 If an internal resource is assigned to the calling
		 ** task, it is automatically released. */
		ReleaseInternalResources();

		/* decrement activations for this task */
		TasksVar[GetRunningTask()].Activations--;

		if (TasksVar[GetRunningTask()].Activations == 0)
		{
			/* if no more activations set state to suspended */
			/* \req OSEK_SYS_3.2.1 The calling task shall be transferred from the
			 ** running state into the suspended state. */
			TasksVar[GetRunningTask()].Flags.State = TASK_ST_SUSPENDED;
		}
		else
		{
			/* if more activations set state to ready */
         TasksVar[GetRunningTask()].Flags.State = TASK_ST_READY;
		}

		/* set entry point for this task again */
		/* \req OSEK_SYS_3.1.2-3/3 The operating system shall ensure that the task
		 ** code is being executed from the first statement. */
		SetEntryPoint(GetRunningTask());
		/* remove ready list */
		RemoveTask(GetRunningTask());
		/* set running task to invalid */
		SetRunningTask(INVALID_TASK);
		/* set actual context SYS */
		SetActualContext(CONTEXT_SYS);

		IntSecure_End();

		/* call scheduler, never returns */
		/* \req OSEK_SYS_3.2.3 If the call was successful, TerminateTask does not
		 ** return to the call level and the status can not be evaluated. */
		/* \req OSEK_SYS_3.2.5 If the service TerminateTask is called
		 ** successfully, it enforces a rescheduling. */
		/* \req OSEK_SYS_3.2.6 This function will never returns in Standard
		 ** mode. */
		(void)Schedule();
	}

#if (HOOK_ERRORHOOK == ENABLE)
	/* \req OSEK_ERR_1.3-2/xx The ErrorHook hook routine shall be called if a
	 ** system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-2/xx The hook routine ErrorHook is not called if a
	 ** system service is called from the ErrorHook itself. */
	if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_TerminateTask);
		SetError_Ret(ret);
		SetError_Msg("Terminate Task returns != than E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}

/** @} doxygen endVar group definition */
/** @} doxygen endVar group definition */
/** @} doxygen endVar group definition */
/*==================[end of file]============================================*/

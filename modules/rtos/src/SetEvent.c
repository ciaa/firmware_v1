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

/** \brief FreeOSEK Os SetEvent Implementation File
 **
 ** This file implements the SetEvent API
 **
 ** \file SetEvent.c
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
 * 20090417 v0.1.5 MaCe correct task status when seting it ready and update GPL
 *						 link exception
 * 20090330 v0.1.4 MaCe add NO_EVENTS evaluation
 * 20090330 v0.1.3 MaCe add support to NON_PREEMPTIVE systems and add non
 *								preemptive check
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
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
StatusType SetEvent
(
	TaskType TaskID,
	EventMaskType Mask
)
{
	/* \req OSEK_SYS_3.15 The system service StatusType
	 * SetEvent ( TaskType TaskID, EventMaskType Mask ) shall be defined */

	/* \req OSEK_SYS_3.15.2: Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if ( TaskID >= TASKS_COUNT )
	{
		/* \req OSEK_SYS_3.15.3-1/3 Extra possible return values in Extended mode
		 * are E_OS_ID, E_OS_ACCESS, E_OS_STATE */
		ret = E_OS_ID;
	}
	else if ( !TasksConst[TaskID].ConstFlags.Extended )
	{
		/* \req OSEK_SYS_3.15.3-2/3 Extra possible return values in Extended mode
		 * are E_OS_ID, E_OS_ACCESS, E_OS_STATE */
		ret = E_OS_ACCESS;
	}
	else if ( TasksVar[TaskID].Flags.State == TASK_ST_SUSPENDED )
	{
		/* \req OSEK_SYS_3.15.3-3/3 Extra possible return values in Extended mode
		 * are E_OS_ID, E_OS_ACCESS, E_OS_STATE */
		ret = E_OS_STATE;
	}
	else
#endif
	{
		/* enter to critical code */
		IntSecure_Start();

		/* the event shall be set only if the task is running ready or waiting */
		if ( 	( TasksVar[TaskID].Flags.State == TASK_ST_RUNNING ) ||
				( TasksVar[TaskID].Flags.State == TASK_ST_READY ) ||
				( TasksVar[TaskID].Flags.State == TASK_ST_WAITING) )
		{
			/* set the events */
			/* \req OSEK_SYS_3.15.1-1/3 The events of task TaskID are set according to the
			 * event mask Mask. Calling SetEvent causes the task TaskID to be
			 * transferred to the ready state, if it was waiting for at least one
			 * of the events specified in Mask */
			TasksVar[TaskID].Events |= ( Mask & TasksConst[TaskID].EventsMask );

			/* if the task is waiting and one waiting event occurrs set it to ready */
			if (	( TasksVar[TaskID].Flags.State == TASK_ST_WAITING ) &&
					( TasksVar[TaskID].EventsWait & TasksVar[TaskID].Events ) )
			{
				/* \req OSEK_SYS_3.15.1-2/3 The events of task TaskID are set according to the
				 * event mask Mask. Calling SetEvent causes the task TaskID to be
				 * transferred to the ready state, if it was waiting for at least one
				 * of the events specified in Mask */
				AddReady(TaskID);

				/* \req OSEK_SYS_3.15.1-3/3 The events of task TaskID are set according to the
				 * event mask Mask. Calling SetEvent causes the task TaskID to be
				 * transferred to the ready state, if it was waiting for at least one
				 * of the events specified in Mask */
				TasksVar[TaskID].Flags.State = TASK_ST_READY;

				IntSecure_End();

#if (NON_PREEMPTIVE == DISABLE)
				/* check if called from a Task Context */
				if ( GetCallingContext() ==  CONTEXT_TASK )
				{
					if ( ( TasksConst[GetRunningTask()].ConstFlags.Preemtive ) &&
						  ( ret == E_OK )	)
					{
						/* \req OSEK_SYS_3.15.4 Rescheduling shall take place only if called from a
						 * preemptable task. */
						(void)Schedule();
					}
				}
#endif /* #if (NON_PREEMPTIVE == DISABLE) */

			}
			else
			{
				IntSecure_End();
			}
		}
		else
		{
			IntSecure_End();
		}
	}


#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
		(HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-8/xx The ErrorHook hook routine shall be called if a
	 * system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-8/xx The hook routine ErrorHook is not called if a
	 * system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_SetEvent);
		SetError_Param1(TaskID);
		SetError_Param2(Mask);
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


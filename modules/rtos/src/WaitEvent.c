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

/** \brief FreeOSEK Os WaitEvent Implementation File
 **
 ** This file implements the WaitEvent API
 **
 ** \file WaitEvent.c
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
StatusType WaitEvent
(
	EventMaskType Mask
)
{
	/* \req OSEK_SYS_3.18: The system service StatusType
	  * WaitEvent ( EventMaskType Mask ) shall be defined */

	volatile uint8	flag = 1;

	/* \req OSEK_SYS_3.18.3 Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if ( !TasksConst[GetRunningTask()].ConstFlags.Extended )
	{
		/* \req OSEK_SYS_3.18.4-1/3 Extra possible return values in Extended mode
		  * are E_OS_ACCESS, E_OS_RESOURCE, E_OS_CALLEVEL */
		ret = E_OS_ACCESS;
	}
	else if ( GetCallingContext() != CONTEXT_TASK )
	{
		/* \req OSEK_SYS_3.18.4-2/3 Extra possible return values in Extended mode
		  * are E_OS_ACCESS, E_OS_RESOURCE, E_OS_CALLEVEL */
		ret = E_OS_CALLEVEL;
	}
	else if ( TasksVar[GetRunningTask()].Resources != 0 )
	{
		/* \req OSEK_SYS_3.18.4-3/3 Extra possible return values in Extended mode
		  * are E_OS_ACCESS, E_OS_RESOURCE, E_OS_CALLEVEL */
		ret = E_OS_RESOURCE;
	}
	else
#endif
	{
		/* enter to critical code */
		IntSecure_Start();

		if ( Mask & TasksVar[GetRunningTask()].Events )
		{
			/* finish cirtical code */
	      IntSecure_End();
		}
		else
		{
			/* \req OSEK_SYS_3.18.1 The state of the calling task is set to waiting,
			  * unless at least one of the events specified in Mask has already been
			  * set */
			TasksVar[GetRunningTask()].Flags.State = TASK_ST_WAITING;

			/* set wait mask */
			TasksVar[GetRunningTask()].EventsWait = Mask;

			/* save actual task context */
			SaveContext(GetRunningTask());

			if (flag)
			{

				/* execute this code only ones */
				flag = 0;

				/* remove of the Ready List */
				RemoveTask(GetRunningTask());

				/* set running task to invalid */
         	SetRunningTask(INVALID_TASK);

				/* finish cirtical code */
		      IntSecure_End();

				/* OSEK_SYS_3.18.2 This call enforces rescheduling, if the wait
				  * condition occurs. If rescheduling takes place, the internal
				  * resource of the task is released while the task is in the
				  * waiting state */
				(void)Schedule();
			}
			else
			{
				/* finish critical code */
				IntSecure_End();
			}
		}
	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
      (HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-11/xx The ErrorHook hook routine shall be called if a
	  * system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-11/xx The hook routine ErrorHook is not called if a
	  * system service is called from the ErrorHook itself. */
	if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_WaitEvent);
		SetError_Param1(Mask);
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


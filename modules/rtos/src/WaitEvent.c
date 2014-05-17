

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


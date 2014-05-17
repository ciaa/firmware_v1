

/** \brief FreeOSEK Os StartOS Implementation File
 **
 ** This file implements the StartOS API
 **
 ** \file StartOS.c
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
 * 20090418 v0.1.4 MaCe bugfix function name to StartOS
 * 20090330 v0.1.3 MaCe use ActivateTast instead of AddReady
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
void StartOS
(
	AppModeType Mode
)
{
	/* \req OSEK_SYS_3.25 The system service void
	 ** StartOS ( AppModeType Mode ) shall be defined */

	/* \req OSEK_SYS_3.25.1 This system service shall starts the operating
	 ** system */
	uint8f loopi;

	IntSecure_Start();

	/* save the aplication mode */
	ApplicationMode = Mode;

	/* StartOs_Arch */
	StartOs_Arch();


	/* init every task */
   for( loopi = 0; loopi < TASKS_COUNT; loopi++)
	{
		/* \req OSEK_SYS_3.1.2-2/3 The operating system shall ensure that the task
		 ** code is being executed from the first statement. */
		SetEntryPoint(loopi); /* set task entry point */
	}

	/* set sys context */
	SetActualContext(CONTEXT_SYS);

	/* set actual task to invalid task */
	SetRunningTask(INVALID_TASK);

	/* add to ready the corresponding tasks for this
    * Application Mode */
	for (loopi = 0; loopi < AutoStart[Mode].TotalTasks; loopi++)
	{
		/* activate task */
		ActivateTask(AutoStart[Mode].TasksRef[loopi]);
	}

	for (loopi = 0; loopi < ALARM_AUTOSTART_COUNT; loopi++)
	{
		if (AutoStartAlarm[loopi].Mode == Mode)
		{
			(void)SetRelAlarm(AutoStartAlarm[loopi].Alarm, AutoStartAlarm[loopi].AlarmTime, AutoStartAlarm[loopi].AlarmCycleTime);
		}
	}

#if (HOOK_STARTUPHOOK == ENABLE)
	StartupHook();
#endif

	IntSecure_End();

	/* enable all OS interrupts */
	EnableOSInterrupts();

	/* enable interrupts */
	EnableInterrupts();

	/* call Scheduler */
	(void)Schedule();

	/* this function shall never return */
	while(1);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen endVar group definition */
/*==================[end of file]============================================*/


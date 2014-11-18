/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

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

#if (HOOK_STARTUPHOOK == OSEK_ENABLE)
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


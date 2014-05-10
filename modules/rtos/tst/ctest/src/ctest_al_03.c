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

/** \brief FreeOSEK Os Conformance Test for the Alarms, Test Sequence 3
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_03.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_03 Test Sequence 3
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090420 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_al_03.h"	/* include test header file */
#include "ctest.h"			/* include ctest header file */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
const uint32f SequenceCounterOk = MAX_SEQUENCE;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int main
(
	void
)
{
	/* start OS in AppMode 1 */
	StartOS(AppMode1);

	/* shall never return */
	while(1);

	return 0;
}

TASK(Task1)
{
	StatusType ret;
	TickType Tick;

	Sequence(1);
	InitAlarmCounter();

	Sequence(2);
	/* \treq AL_24 nmf B1B2E1E2 se call SetAbsAlarm() for alarm which will
	 * set an event on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetAbsAlarm(Alarm1, 1, 1);
	ASSERT(AL_24, ret != E_OK);

	Sequence(3);
	/* \treq AL_18 nmf B1B2E1E2 se call SetAbsAlarm() for already activated
	 * alarm which will set an event on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetAbsAlarm(Alarm1, 3, 0);
	ASSERT(AL_18, ret != E_OS_STATE);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	Sequence(4);
	/* \treq AL_34 nmf B1B2E1E2 se Expiration of alarm wich sets an event while
	 * running task is non-preemptive. Task which owns the event is ot waiting
	 * for this event and not supsended
	 *
	 * \result Event is set. Task which is owner of the event becomes ready. No
	 * preemption of running task
	 */
	IncAlarmCounter();

	Sequence (5);

	/* force scheduling */
	Schedule();
#else
	Sequence(4);
	Sequence (5);
	IncAlarmCounter();
	/* force scheduling is not necessary */
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(8);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif

	Sequence(11);
	/* \treq AL_28 nmf B1B2E1E2 se Call CancelAlarm() for already activated
	 * alarm which will set an event on expiration
	 *
	 * \result Alarm is cancelled. Service returns E_OK
	 */
	ret = CancelAlarm(Alarm1);
	ASSERT(AL_28, ret != E_OK);

	Sequence(12);
	/* \treq AL_15 nmf B1B2E1E2 se Call SetRelAlarm() for alarm which will
	 * set an event on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(AL_15, ret != E_OK);

	Sequence(13);
	/* \treq AL_09 nmf B1B2E1E2 se Call SetRelAlarm() for already activated
	 * alarm which will set an event on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetRelAlarm(Alarm1, 3, 0);
	ASSERT(AL_09, ret != E_OS_STATE);

	Sequence(14);
	/* \treq AL_06 nmf B1B2E1E2 se Call GetAlarm() for alarm which will set an
	 * event on expriation
	 *
	 * \result Return number of ticks until expiration. Service returns E_OK
	 */
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(AL_06, ret != E_OK);
	ASSERT(AL_06, Tick != 1);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	Sequence(15);
	/* \treq AL_34 nmf B1B2E1E2 se Expiration of alarm wich sets an event while
	 * running task is non-preemptive. Task which owns the event is ot waiting
	 * for this event and not supsended
	 *
	 * \result Event is set. Task which is owner of the event becomes ready. No
	 * preemption of running task
	 */
	IncAlarmCounter();
	ASSERT(AL_34, 0);

	Sequence (16);

	/* force scheduling */
	Schedule();
#else
	Sequence(15);
	Sequence (16);
	IncAlarmCounter();
	/* force scheduling is not necessary */
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(18);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	StatusType ret;

	Sequence(0);
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(6);
	ret = ClearEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(7);
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(9);
	ret = ClearEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(10);
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(17);
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


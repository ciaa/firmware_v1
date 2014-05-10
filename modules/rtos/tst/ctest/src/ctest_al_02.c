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

/** \brief FreeOSEK Os Conformance Test for the Alarms, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_02 Test Sequence 2
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
#include "ctest_al_02.h"	/* include test header file */
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
	AlarmBaseType AlarmBase;
	TickType Tick;

	ASSERT(OTHER, 0);

	Sequence(0);
	/* \treq AL_02 nmf B1B2E1E2 se call GetAlarmBase()
	 *
	 * \result Return alarm characteristics. Service returns E_OK
	 */
	ret = GetAlarmBase(Alarm1, &AlarmBase);
	ASSERT(AL_02, ret != E_OK);

	Sequence(1);
	/* \treq AL_04 nmf B1B2E1E2 se call GetAlarm() for alarm which is currently
	 * not in use
	 *
	 * \result Service returns E_OS_NOFUNC
	 */
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(AL_04, ret != E_OS_NOFUNC);

	Sequence(2);
	/* \treq AL_26 nmf B1B2E1E2 se call CancelAlarm() for alarm which is currently
	 * not in use
	 *
	 * \result Service returns E_OS_NOFUNC
	 */
	ret = CancelAlarm(Alarm1);
	ASSERT(AL_26, ret != E_OS_NOFUNC);

	Sequence(3);
	InitAlarmCounter();

	Sequence(4);
	/* \treq AL_23 nmf B1B2E1E2 se call SetAbsAlarm() for alarm which will
	 * activate a task on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetAbsAlarm(Alarm1, 1, 1);
	ASSERT(AL_23, ret != E_OK);

	Sequence(5);
	/* \treq AL_17 nmf B1B2E1E2 se call SetAbsAlarm() for already activated
	 * alarm which will activate a task on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetAbsAlarm(Alarm1, 3, 0);
	ASSERT(AL_17, ret != E_OS_STATE);

	Sequence(6);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(8);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(10);
	/* \treq AL_27 nmf B1B2E1E2 se call CancelAlarm() for already activated
	 * alarm which will activate a task on expiration
	 *
	 * \result Alarm is cancelled. Service returns E_OK
	 */
	ret = CancelAlarm(Alarm1);
	ASSERT(AL_27, ret != E_OK);

	Sequence(11);
	/* \treq AL_14 nmf B1B2E1E2 se call SetRelAlarm() for alarm which will
	 * activate a task on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(AL_14, ret != E_OK);

	Sequence(12);
	/* \treq AL_08 nmf B1B2E1E2 se call SetRelAlarm() for already activated
	 * alarm which will activate a task on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetRelAlarm(Alarm1, 2, 0);
	ASSERT(AL_08, ret != E_OS_STATE);

	Sequence(13);
	/* \treq AL_05 nmf B1B2E1E2 se call GetAlarm() for alarm which will activate
	 * a task on expiration
	 *
	 * \result Return number of ticks until expiration. Service returns E_OK
	 */
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(AL_05, ret != E_OK);
	ASSERT(AL_05, Tick != 1);

	Sequence(14);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(16);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	static uint8 count = 0;

	switch (count)
	{
		case 0:
			/* increment call counter */
			count++;

			Sequence(7);
			TerminateTask();
			break;
		case 1:
			/* increment call counter */
			count++;

			Sequence(9);
			TerminateTask();
			break;
		case 2:
			/* increment call counter */
			count++;

			Sequence(15);
			TerminateTask();
			break;
		default:
			while(1);
			break;
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


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

/** \brief FreeOSEK Os Conformance Test for the Event Mechanism, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_em_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EM Event Mechanism
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EM_02 Test Sequence 2
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090417 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_em_02.h"	/* include test header file */
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
	EventMaskType EventMask;

	Sequence(3);
	/* \treq EM_20 nmf E1E2 se Call GetEvent() for waiting extended task
	 *
	 * \result Return current state of all event bits. Service returns E_OK
	 */
	ret = GetEvent(Task2, &EventMask);
	ASSERT(EM_20, ret != E_OK);
	ASSERT(EM_20, EventMask != 0);

	Sequence(4);
	ret = SetEvent(Task1, Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(5);
	/* \treq EM_18 nmf E1E2 se Call GetEvent() for running extended task
	 *
	 * \result Return current state of all event bits. Service returns E_OK
	 */
	ret = GetEvent(Task1, &EventMask);
	ASSERT(EM_18, ret != E_OK);
	ASSERT(EM_18, EventMask != Event1);

	Sequence(6);
	/* \treq EM_26 nmf E1E2 se Call WaitEvent() for extended task. At least one
	 * of the event waited for is already set
	 *
	 * \result No preemption of running task. Service returns E_OK
	 */
	ret = WaitEvent(Event1);
	ASSERT(EM_26, ret != E_OK);

	Sequence(7);
	/* \treq EM_14 nmf E1E2 se Call ClearEvent() for extended task
	 *
	 * \result Requested events are cleared. Service returns E_OK
	 */
	ret = ClearEvent(Event1);
	ASSERT(EM_14, ret != E_OK);

	Sequence(8);
	ret = GetEvent(Task1, &EventMask);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, EventMask != 0);

	Sequence(9);
	ret = SetEvent(Task2, Event2);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(11);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	StatusType ret;
	EventMaskType EventMask;

	Sequence(0);
	ret = ActivateTask(Task1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(1);
	/* \treq EM_19 nmf E1E2 se Call GetEvent() from ready extended
	 *
	 * \result Return current state of all vent bits. Service returns E_OK
	 */
	ret = GetEvent(Task1, &EventMask);
	ASSERT(EM_19, ret != E_OK);
	ASSERT(EM_19, EventMask != 0);

	Sequence(2);
	/* \treq EM_25 nmf E1E2 se Call WaitEvent() from ready extended task.
	 * None of the events waited for is set
	 *
	 * \result Running task becomes waiting and ready task with highest priority
	 * is executed. Service returns E_OS_OK
	 */
	ret = WaitEvent(Event2);
	ASSERT(EM_25, ret != E_OK);

	Sequence(10);
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


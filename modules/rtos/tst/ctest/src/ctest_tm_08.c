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

/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 8
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_08.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_08 Test Sequence 8
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090409 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_tm_08.h"	/* include test header file */
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
	static uint8 count = 0;

	switch (count)
	{
		case 0:
			/* increment task call counter */
			count++;

			ASSERT(OTHER, 0);

			Sequence(0);
			/* \treq TM_05 mf B1E2 s,e Call ActivateTask() from preemptive
			 * task on suspended basic task which has equal priority as running
			 * task
			 *
			 * \result Non preemption of running task. Activated task becomes ready.
			 * Service returns E_OK
			 */
			ret = ActivateTask(Task2);
			ASSERT(TM_05, ret != E_OK);

			Sequence(5);
			TerminateTask();
			break;
		case 1:
			/* increment task call counter */
			count++;

			Sequence(6);
			/* \treq TM_18 mf B1E2 s,e Call ActivateTask() from preemptive
			 * task on running basic task which has not reached max. number of
			 * activations
			 *
			 * \result Non preemption of running task. Activated task becomes ready.
			 * Service returns E_OK
			 */
			ret = ActivateTask(Task1);
			ASSERT(TM_18, ret != E_OK);

			Sequence(7);
			TerminateTask();
			break;
		case 2:
			/* increment task call counter */
			count++;

			Sequence(8);

			/* evaluate conformance tests */
			ConfTestEvaluation();

			/* finish the conformance test */
			ConfTestFinish();
			break;
		default:
			while(1);
			break;
	}

}

TASK(Task2)
{
	StatusType ret;

	Sequence(1);
	/* \treq TM_13 mf B1E2 s,e Call ActivateTask() from preemptive
	 * task on ready basic task which has not reached max. number of activations
	 * and has lower priority than the running task
	 *
	 * \result Non preemption of running task. Activation request is queued in
	 * ready list. Service returns E_OK
	 */
	ret = ActivateTask(Task1);
	ASSERT(TM_13, ret != E_OK);

	Sequence(2);
	/* \treq TM_14 mf B1E2 s,e Call ActivateTask() from preemptive
	 * task on ready basic task which has not reached max. number of activations
	 * and has equal priority than the running task
	 *
	 * \result Non preemption of running task. Activation request is queued in
	 * ready list. Service returns E_OK
	 */
	ret = ActivateTask(Task3);
	ASSERT(TM_14, ret != E_OK);

	Sequence(3);
	TerminateTask();
}

TASK(Task3)
{
	Sequence(4);
	TerminateTask();
}

/* This task is not used, only to change the scheduling police */
TASK(Task4)
{
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


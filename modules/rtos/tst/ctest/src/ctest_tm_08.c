

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


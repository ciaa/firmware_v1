

/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_02 Test Sequence 2
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090413 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_tm_02.h"	/* include test header file */
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

	ASSERT(OTHER, 0);

	Sequence(0);
	/* \treq TM_02 nm B1B2E1E2 se Call ActivateTask() from non-preemptive
 	 * task on suspend basic task
	 *
	 * \result No preemption of running task. Activated task becomes ready.
	 * Service returns E_OK
	 */
	ret = ActivateTask(Task2);
	ASSERT(TM_02, ret != E_OK);

	Sequence(1);
	/* \treq TM_02 nm B1B2E1E2 se Call ActivateTask() from non-preemptive
 	 * task on suspend basic task
	 *
	 * \result No preemption of running task. Activated task becomes ready.
	 * Service returns E_OK
	 */
	ret = ActivateTask(Task3);
	ASSERT(TM_02, ret != E_OK);

	Sequence(2);
	/* \treq TM_34 nmf B1B2E1E2 se Call Schedule() from task
	 *
	 * \result Ready task with highest priority is executed. Service returns E_OK
	 */
	ret = Schedule();
	ASSERT(TM_34, ret != E_OK);

	Sequence(5);
	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	Sequence(4);
	TerminateTask();
}

TASK(Task3)
{
	Sequence(3);
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


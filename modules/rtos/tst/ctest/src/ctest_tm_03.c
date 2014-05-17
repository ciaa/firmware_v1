

/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 3
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_03.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_03 Test Sequence 3
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090413 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_tm_03.h"	/* include test header file */
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
	/* \treq TM_03 mf B1B2E1E2 se Call ActivateTask() from preemptive
 	 * task on suspend basic task which has higher priority than running
	 * task
	 *
	 * \result Runnin task is preempted. Activated task becomes ready.
	 * Service returns E_OK
	 */
	ret = ActivateTask(Task3);
	ASSERT(TM_03, ret != E_OK);

	Sequence(4);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	Sequence(3);
	TerminateTask();
}

TASK(Task3)
{
	StatusType ret;

	Sequence(1);
	/* \treq TM_04 mf B1B2E1E2 se Call ActivateTask() from preemptive
 	 * task on suspend basic task which has lower priority than running
	 * task
	 *
	 * \result Non preemption of running task. Activated task becomes ready.
	 * Service returns E_OK
	 */
	ret = ActivateTask(Task2);
	ASSERT(TM_04, ret != E_OK);

	Sequence(2);
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


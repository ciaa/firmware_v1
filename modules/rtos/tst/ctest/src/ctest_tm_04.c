

/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 4
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_04.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_04 Test Sequence 4
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
#include "ctest_tm_04.h"	/* include test header file */
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
	EventMaskType event;

	Sequence(0);
	/* \treq TM_06 nm E1E2 se Call ActivateTask() from non-preemptive
 	 * task on suspend extended task
	 *
	 * \result No preemption of running task. Activated task becomes ready
	 * and it's events become cleared. Service Returns E_OK
	 */
	ret = ActivateTask(Task2);
	ASSERT(TM_06, ret != E_OK);

	Sequence(1);
	ret = GetEvent(Task1, &event);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, event != 0);

	Sequence(2);
	ret = GetEvent(Task2, &event);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, event != 0);

	Sequence(3);
	ret = Schedule();
	ASSERT(OTHER, ret != E_OK);

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

/* This task is not used, only to change the scheduling police */
TASK(Task3)
{
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/




/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 6
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_06.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_06 Test Sequence 6
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
#include "ctest_tm_06.h"	/* include test header file */
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
	TaskStateType state;

	Sequence(0);
	ret = ActivateTask(Task2);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(4);
	ret = GetTaskState(Task2, &state);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, state != WAITING);

	Sequence(5);
	/* \treq TM_19 nmf E1E2 e Call ActivateTask() on waiting extended task
	 *
	 * \result Service returns E_OS_LIMIT
	 */
	ret = ActivateTask(Task2);
	ASSERT(TM_19,ret != E_OS_LIMIT);

	Sequence(6);
	/* \treq TM_33 nmf E1E2 e Call ChainTask() on waiting extended task
	 *
	 * \result Service returns E_OS_LIMIT
	 */
	ret = ChainTask(Task2);
	ASSERT(TM_33,ret != E_OS_LIMIT);

	Sequence(7);
	ret = SetEvent(Task2,Event2);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(10);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	StatusType ret;

	Sequence(1);
	/* \treq TM_11 nmf B1B2E1E2 e Call ActivateTask() on ready extended task
	 *
	 * \result Service returns E_OS_LIMIT
	 */
	ret = ActivateTask(Task1);
	ASSERT(TM_11,ret != E_OS_LIMIT);

	Sequence(2);
	/* \treq TM_16 nmf B1B2E1E2 e Call ActivateTask() on running extended task
	 *
	 * \result Service returns E_OS_LIMIT
	 */
	ret = ActivateTask(Task2);
	ASSERT(TM_16,ret != E_OS_LIMIT);

	Sequence(3);
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(8);
	/* \treq TM_31 nmf E1E2 e Call ChainTask() on ready extended task
	 *
	 * \result Running task is not terminated. Service returns E_OS_LIMIT
	 */
	ret = ChainTask(Task1);
	ASSERT(TM_31,ret != E_OS_LIMIT);

	Sequence(9);
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


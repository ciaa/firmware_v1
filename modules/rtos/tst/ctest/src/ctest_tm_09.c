

/** \brief FreeOSEK Os Conformance Test for the Task Managment, Test Sequence 9
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_tm_09.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM Task Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_TM_09 Test Sequence 9
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
#include "ctest_tm_09.h"	/* include test header file */
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
	TaskType TaskID;
	TaskStateType TaskState;

		Sequence(0);
		/* \treq TM_39 nmf B1B2E1E2 se Call GetTaskID() from task
		 *
		 * \result Return task ID of cruently running task. Service returns E_OK
		 */
		ret = GetTaskID(&TaskID);
		ASSERT(TM_39, TaskID != Task1);
		ASSERT(TM_39, ret != E_OK);

		Sequence(1);
		/* \treq TM_41 nmf B1B2E1E2 se Call GetTaskState()
		 *
		 * \result Returns state of queried task. Service returns E_OK
		 */
		ret = GetTaskState(Task1, &TaskState);
		ASSERT(TM_41, TaskState != RUNNING);
		ASSERT(TM_41, ret != E_OK);

		Sequence(2);
		/* \treq TM_41 nmf B1B2E1E2 se Call GetTaskState()
		 *
		 * \result Returns state of queried task. Service returns E_OK
		 */
		ret = GetTaskState(Task2, &TaskState);
		ASSERT(TM_41, TaskState != SUSPENDED);
		ASSERT(TM_41, ret != E_OK);

		Sequence(3);
		ret = ActivateTask(Task2);
		ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
		/* force scheduling */
		ret = Schedule();
		ASSERT(OTHER, ret != E_OK);
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

		Sequence(6);
		ASSERT(TM_28, 0);
		/* \treq TM_28 nmf B1B2E1E2 se Call ChainTask() on suspended task
		 *
		 * \result Running task is terminated, chained task becomes ready and
		 * and ready task with higher priority is executed
		 */
		ChainTask(Task3);
		ASSERT(TM_28, 1);
		while(1);
}

TASK(Task2)
{
	StatusType ret;
	TaskStateType TaskState;

	Sequence(4);
	/* \treq TM_41 nmf B1B2E1E2 se Call GetTaskState()
	 *
	 * \result Returns state of queried task. Service returns E_OK
	 */
	ret = GetTaskState(Task1, &TaskState);
	ASSERT(TM_41, TaskState != READY);
	ASSERT(TM_41, ret != E_OK);

	Sequence(5);
	ASSERT(TM_23, 0);
	/* \treq TM_23 nmf B1B2E1E2 se Call TerminateTask()
	 *
	 * \result Running task is terminated, and ready task with highest priority
	 * is executed
	 */
	/** TODO this test case is performed out of order */
	TerminateTask();
	ASSERT(TM_23, 1);
	while(1);
}

TASK(Task3)
{
	static uint8 count = 0;

	switch (count)
	{
		case 0:
			/* increment task call counter */
			count++;

			Sequence(7);
			ASSERT(TM_29, 0);
			/* \treq TM_29 nmf B1B2E1E2 se Call ChainTask() on running task
			 *
			 * \result Running task is terminated, chained task becomes ready and
			 * and ready task with higher priority is executed
			 */
			ChainTask(Task3);
			ASSERT(TM_29, 0);
			while(1);
			break;
		case 1:
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

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/




/** \brief FreeOSEK Os Conformance Test for the Event Mechanism, Test Sequence 4
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_em_04.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EM Event Mechanism
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EM_04 Test Sequence 4
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
#include "ctest_em_04.h"	/* include test header file */
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
	TaskStateType TaskState;

	Sequence(1);
	/* \treq EM_08 mf E1E2 se Call SetEvent() from preemptive task
	 * on waiting extended task which is not waiting for any of the requested
	 * events
	 *
	 * \result Requested events are set. Running task is not preempted. Waiting
	 * task doesn't becomes ready. Service returns E_OK
	 */
	ret = SetEvent(Task2, Event2);
	ASSERT(EM_08, ret != E_OK);

	Sequence(2);
	ret = GetTaskState(Task2, &TaskState);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, TaskState != WAITING);

	Sequence(3);
	ret = GetEvent(Task2, &EventMask);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, EventMask != Event2);

	Sequence(4);
	ret = ActivateTask(Task3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(5);
	ret = GetTaskState(Task3, &TaskState);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, TaskState != READY);

	Sequence(6);
	/* \treq EM_10 nm E1E2 se Call SetEvent() from preemptive task
	 * on ready extended task
	 *
	 * \result Requested events are set. Running task is not preempted. Service
	 * returns E_OK
	 */
	ret = SetEvent(Task3, Event3);
	ASSERT(EM_10, ret != E_OK);

	Sequence(7);
	ret = GetEvent(Task3, &EventMask);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, EventMask != Event3);

	Sequence(8);
	/* \treq EM_06 mf E1E2 se Call SetEvent() from preemptive task
	 * on waiting extended task which is waiting for at least one of the
	 * requested events and has higher priority than running tas
	 *
	 * \result Requested events are set. Running task ready (is preempted).
	 * Waiting becomes running. Service returns E_OK
	 */
	ret = SetEvent(Task2, Event1);
	ASSERT(EM_06, ret != E_OK);

	Sequence(11);
	ret = ActivateTask(Task4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(16);
	TerminateTask();
}

TASK(Task2)
{
	StatusType ret;

	Sequence(0);
	ret = WaitEvent(Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(9);
	ret = ClearEvent(Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(10);
	ret = WaitEvent(Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(15);
	TerminateTask();
}

TASK(Task3)
{
	Sequence(17);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}


TASK(Task4)
{
	StatusType ret;
	TaskStateType TaskState;

	Sequence(12);
	/* \treq EM_07 mf E1E2 se Call SetEvent() from preemptive task
	 * on waiting extended task which is waiting for at least one of the
	 * requested events and has equal or lower priority than running task
	 *
	 * \result Requested events are set. Running task is not preempted. Waiting
	 * task becomes ready. Service returns E_OK
	 */
	ret = SetEvent(Task2, Event1);
	ASSERT(EM_07, ret != E_OK);

	Sequence(13);
	ret = GetTaskState(Task2, &TaskState);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, TaskState != READY);

	Sequence(14);
	TerminateTask();
}

/* This task is not used, only to change the scheduling police */
TASK(Task5)
{
	TerminateTask();
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


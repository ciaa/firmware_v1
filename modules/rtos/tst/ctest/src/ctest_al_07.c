

/** \brief FreeOSEK Os Conformance Test for the Alarms, Test Sequence 7
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_07.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_07 Test Sequence 7
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090424 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_al_07.h"	/* include test header file */
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

	Sequence(1);
	ret = ActivateTask(Task3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(13);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	StatusType ret;

	Sequence(0);
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(6);
	ret = ClearEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(7);
	ret = ActivateTask(Task4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(12);
	TerminateTask();
}

TASK(Task3)
{
	StatusType ret;
	TaskStateType TaskState;

	Sequence(2);
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(OTHER, ret != E_OK);

	Sequence(3);
	/* \treq AL_36 mf E1E2 se Expiration of alarm wich sets an event
	 * while running task is preemptive. Task which owns the event is
	 * waiting for this event
	 *
	 * \result Event is set. Task which is owner of this event becomes ready.
	 * Task with highest priority is executed (Rescheduling)
	 */
	IncAlarmCounter();
	ASSERT(AL_36, 0);

	Sequence(4);
	ret = GetTaskState(Task2, &TaskState);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, TaskState != READY);

	Sequence(5);
	TerminateTask();
}

TASK(Task4)
{
	StatusType ret;
	EventMaskType EventMask;

	Sequence(8);
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(OTHER, ret != E_OK);

	Sequence(9);
	/* \treq AL_35 mf E1E2 se Expiration of alarm wich sets an event
	 * while running task is preemptive. Task which owns the event is not
	 * waiting for this event and not suspended
	 *
	 * \result Event is set.
	 */
	IncAlarmCounter();
	ASSERT(AL_35, 0);

	Sequence(10);
	ret = GetEvent(Task2, &EventMask);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, EventMask != Event2);

	Sequence(11);
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




/** \brief FreeOSEK OS Conformance Test for the Alarms, Test Sequence 5
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_05.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_05 Test Sequence 5
 ** @{ */


/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090420 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_al_05.h"	/* include test header file */
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

	Sequence(0);

	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(OTHER, ret != E_OK);

	Sequence(1);
	/* \treq AL_31 mf B1B2E1E2 se Expiration of alarm wich activates a task
	 * with higher priority than running task while running task is preemptive
	 *
	 * \result Task is activated. Task with highest priority is executed
	 */
	IncAlarmCounter();
	ASSERT(AL_31, 0);

	Sequence(3);
	ChainTask(Task3);
}

TASK(Task2)
{
	static uint8 count = 0;

	switch (count)
	{
		case 0:
			/* increment call count */
			count++;

			Sequence(2);
			TerminateTask();
		case 1:
			/* increment call count */
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

TASK(Task3)
{
	StatusType ret;
	TaskStateType TaskState;


	Sequence(4);
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(OTHER, ret != E_OK);

	Sequence(5);
	/* \treq AL_32 mf B1B2E1E2 se Expiration of alarm wich activates a task
	 * with lower priority than running task while running task is preemptive
	 *
	 * \result Task is activated. No preemption of running task
	 */
	IncAlarmCounter();
	ASSERT(AL_32, 0);

	Sequence(6);
	ret = GetTaskState(Task2, &TaskState);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, TaskState != READY);

	Sequence(7);
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


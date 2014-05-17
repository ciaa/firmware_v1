

/** \brief FreeOSEK Os Conformance Test for the Resource Managment, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_rm_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM Resource Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM_02 Test Sequence 2
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090414 v0.1.0 MaCe initial version based on old moduletest
 */

/*==================[inclusions]=============================================*/
#include "os.h"				/* include os header file */
#include "ctest_rm_02.h"	/* include test header file */
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
	/* \treq RM_06 nm B1B2E1E2 se Test Priority Ceiling Protocol: Call
	 * GetResource() rom non-preemptive task, activate task with priority
	 * higher than the running task but lower than ceiling priority, and force
	 * rescheduling
	 *
	 * \result Resource is occupied and running task's priority is set to the
	 * resource's ceiling priority. Service returns E_OK. No preemption occurrs
	 * after activating the task with higher priotiy and rescheduling
	 */
	ret = GetResource(Resource1);
	ASSERT(RM_06, ret != E_OK);

	Sequence(1);
	ret = ActivateTask(Task2);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(2);
	ret = ActivateTask(Task3);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(4);
	/* \treq RM_13 nm B1B2E1E2 se Call ReleaseResource() from a non-preemptive
	 * task
	 *
	 * \result Resource is released and running task's priority is reset. No
	 * preemption of running task. Service returns E_OK
	 */
	ret = ReleaseResource(Resource1);
	ASSERT(RM_13, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(6);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	Sequence(5);
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


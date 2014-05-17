

/** \brief FreeOSEK Os Conformance Test for the Resource Managment, Test Sequence 4
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_rm_04.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM Resource Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM_04 Test Sequence 4
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
#include "ctest_rm_04.h"	/* include test header file */
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
	/* \treq RM_08 nmf B1B2E1E2 se Call GetResource() for resource RES_SCHEDULER
	 *
	 * \result Resource is occupied and running task's priority is set to
	 * resource's ceiling priority. Service returns E_OK
	 */
	ret = GetResource(RES_SCHEDULER);
	ASSERT(RM_08, ret != E_OK);

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

	Sequence(3);
	/* \treq RM_15 nm B1B2E1E2 se Call ReleaseResource() from non-preemptive
	 * task for resource RES_SCHEDULER
	 *
	 * \result Resource is released and running task's priority is reset. No
	 * preemption of running task. Service Returns E_OK
	 */
	/* \treq RM_16 mf B1B2E1E2 se Call ReleaseResource() from preemptive
	 * task for resource RES_SCHEDULER
	 *
	 * \result Resource is released and running task's priority is reset. Ready
	 * task with highest priority is executed. Service returns E_OK
	 */
	ret = ReleaseResource(RES_SCHEDULER);
	ASSERT(RM_15, ret != E_OK);
	ASSERT(RM_16, ret != E_OK);

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


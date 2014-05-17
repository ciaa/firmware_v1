

/** \brief FreeOSEK Os Conformance Test for the Resource Managment, Test Sequence 1
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_rm_01.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM Resource Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM_01 Test Sequence 1
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
#include "ctest_rm_01.h"	/* include test header file */
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
	/* enable interrupts ISR2 and ISR3 */
	/* nothing to do */

	Sequence(1);
	/* \treq RM_01 nmf B1B2E1E2 e Call GetResource() from a task which has no
	 * access to this resource
	 *
	 * \result Service returns E_OS_ACCESS
	 */
	ret = GetResource(ResourceA);
	ASSERT(RM_01, ret != E_OS_ACCESS);

	Sequence(2);
	/* \treq RM_02 nmf B1B2E1E2 e Call GetResource() from a task with invalid
	 * resource ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = GetResource(INVALID_RESOURCE);
	ASSERT(RM_02, ret != E_OS_ID);

	Sequence(3);
	ret = GetResource(Resource1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(4);
	ret = GetResource(Resource2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(5);
	ret = GetResource(Resource3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(6);
	ret = GetResource(Resource4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(7);
	ret = GetResource(Resource5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(8);
	/* \treq RM_05 nmf B1B2E1E2 e Call GetResource() from a task with too many
	 * resources occupied in parallel. This req do not affect FreeOSEK whera upto 32
	 * resources can be occupied by a task
	 *
	 * \result Service returns E_OK
	 */
	ret = GetResource(Resource6);
	ASSERT(RM_05, ret != E_OK);

	Sequence(9);
	ret = ReleaseResource(Resource6);
	ASSERT(OTHER, ret != E_OK);

	Sequence(10);
	ret = ReleaseResource(Resource5);
	ASSERT(OTHER, ret != E_OK);

	Sequence(11);
	ret = ReleaseResource(Resource4);
	ASSERT(OTHER, ret != E_OK);

	Sequence(12);
	ret = ReleaseResource(Resource3);
	ASSERT(OTHER, ret != E_OK);

	Sequence(13);
	ret = ReleaseResource(Resource2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(14);
	ret = ReleaseResource(Resource1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(15);
	/*  trigger ISR 2 */
	TriggerISR2();

#if ( ISR_CATEGORY_3 == ENABLE )
	/* trigger ISR 3 */
	TriggerISR3();
#endif /* #if ( ISR_CATEGORY_3 == ENABLE ) */

	Sequence(18);
	/* \treq RM_12 nmf B1B2E1E2 e Call ReleaseResource() from a task with a resource
	 * which is not occupied
	 *
	 * \result Service returns E_OS_NOFUNC
	 */
	ret = ReleaseResource(Resource1);
	ASSERT(RM_12, ret != E_OS_NOFUNC);

	Sequence(19);
	/* \treq RM_09 nmf B1B2E1E2 e Call ReleaseResource() from a task with an
	 * invalid resource ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = ReleaseResource(INVALID_RESOURCE);
	ASSERT(RM_09, ret != E_OS_ID);

	Sequence(20);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	while(1);
	TerminateTask();
}

ISR(ISR2)
{
	StatusType ret;

	Sequence(16);
	/* \treq RM_03 nmf B1B2E1E2 e Call GetResource() from ISR category 2
	 *
	 * \result Service returns E_OS_CALLEVEL
	 */
    /* this test is an error in the specification see #40
     * Issue in TestPlan RM_03 Call GetResource and ReleaseResource from ISR2
     */
	ret = GetResource(Resource1);
	ASSERT(RM_03, ret != E_OK);

	Sequence(17);
	/* \treq RM_10 nmf B1B2E1E2 e Call ReleaseResource() from ISR category 2
	 *
	 * \result Service returns E_OS_CALLEVEL
	 */
    /* this test is an error in the specification see #40
     * Issue in TestPlan Call GetResource and ReleaseResource from ISR2
     */
	ret = ReleaseResource(Resource1);
	ASSERT(RM_10, ret != E_OK);
}

#if ( ISR_CATEGORY_3 == ENABLE )
ISR(ISR3)
{
	StatusType ret;

	EnterISR();

	/* ISR3 are not supported by FreeOSEK OS*/

	LeaveISR();
}
#endif /* #if ( ISR_CATEGORY_3 == ENABLE ) */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


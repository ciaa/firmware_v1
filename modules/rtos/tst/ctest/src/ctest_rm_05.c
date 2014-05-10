/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of FreeOSEK.
 *
 * FreeOSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Linking FreeOSEK statically or dynamically with other modules is making a
 * combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of FreeOSEK give
 * you permission to combine FreeOSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with FreeOSEK solely through the FreeOSEK defined interface.
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for FreeOSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of FreeOSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 *
 * FreeOSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** \brief FreeOSEK Os Conformance Test for the Resource Managment, Test Sequence 5
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_rm_05.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM Resource Management
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_RM_05 Test Sequence 5
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
#include "ctest_rm_05.h"	/* include test header file */
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
	/* \treq RM_02 nmf B1B2E1E2 e Call GetResource() from a task with invalid
	 * resource ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = GetResource(INVALID_RESOURCE);
	ASSERT(RM_02, ret != E_OS_ID);

	Sequence(2);
	/*  trigger ISR 2 */
	TriggerISR2();

#if ( ISR_CATEGORY_3 == ENABLE )
	/* trigger ISR 3 */
	TriggerISR3();
#endif /* #if ( ISR_CATEGORY_3 == ENABLE ) */

	Sequence(5);
	/* \treq RM_12 nmf B1B2E1E2 e Call ReleaseResource() from a task with a resource
	 * which is not occupied
	 *
	 * \result Service returns E_OS_NOFUNC
	 */
	ret = ReleaseResource(Resource1);
	ASSERT(RM_12, ret != E_OS_NOFUNC);

	Sequence(6);
	/* \treq RM_09 nmf B1B2E1E2 e Call ReleaseResource() from a task with an
	 * invalid resource ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = ReleaseResource(INVALID_RESOURCE);
	ASSERT(RM_09, ret != E_OS_ID);

	ASSERT(OTHER, 0);

	Sequence(7);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

/* This task is not used, only to change the scheduling police */
TASK(Task2)
{
	while(1);
	TerminateTask();
}

ISR(ISR2)
{
	StatusType ret;

	Sequence(3);
    /* \treq RM_03 nmf B1B2E1E2 e Call GetResource() from ISR category 2
	 *
	 * \result Service returns E_OS_CALLEVEL
	 */
    /* this test is an error in the specification see #40
     * Issue in TestPlan RM_03 Call GetResource and ReleaseResource from ISR2
     */
	ret = GetResource(Resource1);
	ASSERT(RM_03, ret != E_OK);

	Sequence(4);
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


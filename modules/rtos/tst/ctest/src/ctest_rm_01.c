/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

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


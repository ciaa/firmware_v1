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


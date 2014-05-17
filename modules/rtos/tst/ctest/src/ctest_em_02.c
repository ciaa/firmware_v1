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

/** \brief FreeOSEK Os Conformance Test for the Event Mechanism, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_em_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EM Event Mechanism
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EM_02 Test Sequence 2
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
#include "ctest_em_02.h"	/* include test header file */
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

	Sequence(3);
	/* \treq EM_20 nmf E1E2 se Call GetEvent() for waiting extended task
	 *
	 * \result Return current state of all event bits. Service returns E_OK
	 */
	ret = GetEvent(Task2, &EventMask);
	ASSERT(EM_20, ret != E_OK);
	ASSERT(EM_20, EventMask != 0);

	Sequence(4);
	ret = SetEvent(Task1, Event1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(5);
	/* \treq EM_18 nmf E1E2 se Call GetEvent() for running extended task
	 *
	 * \result Return current state of all event bits. Service returns E_OK
	 */
	ret = GetEvent(Task1, &EventMask);
	ASSERT(EM_18, ret != E_OK);
	ASSERT(EM_18, EventMask != Event1);

	Sequence(6);
	/* \treq EM_26 nmf E1E2 se Call WaitEvent() for extended task. At least one
	 * of the event waited for is already set
	 *
	 * \result No preemption of running task. Service returns E_OK
	 */
	ret = WaitEvent(Event1);
	ASSERT(EM_26, ret != E_OK);

	Sequence(7);
	/* \treq EM_14 nmf E1E2 se Call ClearEvent() for extended task
	 *
	 * \result Requested events are cleared. Service returns E_OK
	 */
	ret = ClearEvent(Event1);
	ASSERT(EM_14, ret != E_OK);

	Sequence(8);
	ret = GetEvent(Task1, &EventMask);
	ASSERT(OTHER, ret != E_OK);
	ASSERT(OTHER, EventMask != 0);

	Sequence(9);
	ret = SetEvent(Task2, Event2);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(11);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	StatusType ret;
	EventMaskType EventMask;

	Sequence(0);
	ret = ActivateTask(Task1);
	ASSERT(OTHER, ret != E_OK);

	Sequence(1);
	/* \treq EM_19 nmf E1E2 se Call GetEvent() from ready extended
	 *
	 * \result Return current state of all vent bits. Service returns E_OK
	 */
	ret = GetEvent(Task1, &EventMask);
	ASSERT(EM_19, ret != E_OK);
	ASSERT(EM_19, EventMask != 0);

	Sequence(2);
	/* \treq EM_25 nmf E1E2 se Call WaitEvent() from ready extended task.
	 * None of the events waited for is set
	 *
	 * \result Running task becomes waiting and ready task with highest priority
	 * is executed. Service returns E_OS_OK
	 */
	ret = WaitEvent(Event2);
	ASSERT(EM_25, ret != E_OK);

	Sequence(10);
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


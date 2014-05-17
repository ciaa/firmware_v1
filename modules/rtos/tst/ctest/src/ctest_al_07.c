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


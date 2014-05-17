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

/** \brief FreeOSEK Os Conformance Test for the Alarms, Test Sequence 3
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_03.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_03 Test Sequence 3
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
#include "ctest_al_03.h"	/* include test header file */
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
	TickType Tick;

	Sequence(1);
	InitAlarmCounter();

	Sequence(2);
	/* \treq AL_24 nmf B1B2E1E2 se call SetAbsAlarm() for alarm which will
	 * set an event on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetAbsAlarm(Alarm1, 1, 1);
	ASSERT(AL_24, ret != E_OK);

	Sequence(3);
	/* \treq AL_18 nmf B1B2E1E2 se call SetAbsAlarm() for already activated
	 * alarm which will set an event on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetAbsAlarm(Alarm1, 3, 0);
	ASSERT(AL_18, ret != E_OS_STATE);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	Sequence(4);
	/* \treq AL_34 nmf B1B2E1E2 se Expiration of alarm wich sets an event while
	 * running task is non-preemptive. Task which owns the event is ot waiting
	 * for this event and not supsended
	 *
	 * \result Event is set. Task which is owner of the event becomes ready. No
	 * preemption of running task
	 */
	IncAlarmCounter();

	Sequence (5);

	/* force scheduling */
	Schedule();
#else
	Sequence(4);
	Sequence (5);
	IncAlarmCounter();
	/* force scheduling is not necessary */
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(8);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif

	Sequence(11);
	/* \treq AL_28 nmf B1B2E1E2 se Call CancelAlarm() for already activated
	 * alarm which will set an event on expiration
	 *
	 * \result Alarm is cancelled. Service returns E_OK
	 */
	ret = CancelAlarm(Alarm1);
	ASSERT(AL_28, ret != E_OK);

	Sequence(12);
	/* \treq AL_15 nmf B1B2E1E2 se Call SetRelAlarm() for alarm which will
	 * set an event on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(AL_15, ret != E_OK);

	Sequence(13);
	/* \treq AL_09 nmf B1B2E1E2 se Call SetRelAlarm() for already activated
	 * alarm which will set an event on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetRelAlarm(Alarm1, 3, 0);
	ASSERT(AL_09, ret != E_OS_STATE);

	Sequence(14);
	/* \treq AL_06 nmf B1B2E1E2 se Call GetAlarm() for alarm which will set an
	 * event on expriation
	 *
	 * \result Return number of ticks until expiration. Service returns E_OK
	 */
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(AL_06, ret != E_OK);
	ASSERT(AL_06, Tick != 1);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	Sequence(15);
	/* \treq AL_34 nmf B1B2E1E2 se Expiration of alarm wich sets an event while
	 * running task is non-preemptive. Task which owns the event is ot waiting
	 * for this event and not supsended
	 *
	 * \result Event is set. Task which is owner of the event becomes ready. No
	 * preemption of running task
	 */
	IncAlarmCounter();
	ASSERT(AL_34, 0);

	Sequence (16);

	/* force scheduling */
	Schedule();
#else
	Sequence(15);
	Sequence (16);
	IncAlarmCounter();
	/* force scheduling is not necessary */
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(18);

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
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(9);
	ret = ClearEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(10);
	ret = WaitEvent(Event2);
	ASSERT(OTHER, ret != E_OK);

	Sequence(17);
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


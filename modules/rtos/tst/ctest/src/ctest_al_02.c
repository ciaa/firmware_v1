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

/** \brief FreeOSEK Os Conformance Test for the Alarms, Test Sequence 2
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_02.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_02 Test Sequence 2
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
#include "ctest_al_02.h"	/* include test header file */
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
	AlarmBaseType AlarmBase;
	TickType Tick;

	ASSERT(OTHER, 0);

	Sequence(0);
	/* \treq AL_02 nmf B1B2E1E2 se call GetAlarmBase()
	 *
	 * \result Return alarm characteristics. Service returns E_OK
	 */
	ret = GetAlarmBase(Alarm1, &AlarmBase);
	ASSERT(AL_02, ret != E_OK);

	Sequence(1);
	/* \treq AL_04 nmf B1B2E1E2 se call GetAlarm() for alarm which is currently
	 * not in use
	 *
	 * \result Service returns E_OS_NOFUNC
	 */
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(AL_04, ret != E_OS_NOFUNC);

	Sequence(2);
	/* \treq AL_26 nmf B1B2E1E2 se call CancelAlarm() for alarm which is currently
	 * not in use
	 *
	 * \result Service returns E_OS_NOFUNC
	 */
	ret = CancelAlarm(Alarm1);
	ASSERT(AL_26, ret != E_OS_NOFUNC);

	Sequence(3);
	InitAlarmCounter();

	Sequence(4);
	/* \treq AL_23 nmf B1B2E1E2 se call SetAbsAlarm() for alarm which will
	 * activate a task on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetAbsAlarm(Alarm1, 1, 1);
	ASSERT(AL_23, ret != E_OK);

	Sequence(5);
	/* \treq AL_17 nmf B1B2E1E2 se call SetAbsAlarm() for already activated
	 * alarm which will activate a task on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetAbsAlarm(Alarm1, 3, 0);
	ASSERT(AL_17, ret != E_OS_STATE);

	Sequence(6);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(8);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(10);
	/* \treq AL_27 nmf B1B2E1E2 se call CancelAlarm() for already activated
	 * alarm which will activate a task on expiration
	 *
	 * \result Alarm is cancelled. Service returns E_OK
	 */
	ret = CancelAlarm(Alarm1);
	ASSERT(AL_27, ret != E_OK);

	Sequence(11);
	/* \treq AL_14 nmf B1B2E1E2 se call SetRelAlarm() for alarm which will
	 * activate a task on expiration
	 *
	 * \result Alarms is activated. Service returns E_OK
	 */
	ret = SetRelAlarm(Alarm1, 1, 0);
	ASSERT(AL_14, ret != E_OK);

	Sequence(12);
	/* \treq AL_08 nmf B1B2E1E2 se call SetRelAlarm() for already activated
	 * alarm which will activate a task on expiration
	 *
	 * \result Service returns E_OS_STATE
	 */
	ret = SetRelAlarm(Alarm1, 2, 0);
	ASSERT(AL_08, ret != E_OS_STATE);

	Sequence(13);
	/* \treq AL_05 nmf B1B2E1E2 se call GetAlarm() for alarm which will activate
	 * a task on expiration
	 *
	 * \result Return number of ticks until expiration. Service returns E_OK
	 */
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(AL_05, ret != E_OK);
	ASSERT(AL_05, Tick != 1);

	Sequence(14);
	IncAlarmCounter();

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(16);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

TASK(Task2)
{
	static uint8 count = 0;

	switch (count)
	{
		case 0:
			/* increment call counter */
			count++;

			Sequence(7);
			TerminateTask();
			break;
		case 1:
			/* increment call counter */
			count++;

			Sequence(9);
			TerminateTask();
			break;
		case 2:
			/* increment call counter */
			count++;

			Sequence(15);
			TerminateTask();
			break;
		default:
			while(1);
			break;
	}
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


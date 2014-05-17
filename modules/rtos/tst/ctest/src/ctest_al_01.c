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

/** \brief FreeOSEK Os Conformance Test for the Alarms, Test Sequence 1
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_al_01.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL Alarms
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_AL_01 Test Sequence 1
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
#include "ctest_al_01.h"	/* include test header file */
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

	Sequence(0);
	/* \treq AL_01 nmf B1B2E1E2 e call GetAlarmBase() with invalid alarm ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = GetAlarmBase(INVALID_ALARM, &AlarmBase);
	ASSERT(AL_01, ret != E_OS_ID);

	Sequence(1);
	/* \treq AL_03 nmf B1B2E1E2 e call GetAlarm() with invalid alarm ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = GetAlarm(INVALID_ALARM, &Tick);
	ASSERT(AL_03, ret != E_OS_ID);

	Sequence(2);
	ret = GetAlarmBase(Alarm1, &AlarmBase);
	ASSERT(OTHER, ret != E_OK);

	Sequence(3);
	/* \treq AL_07 nmf B1B2E1E2 e call SetRelAlarm() with invalid alarm ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = SetRelAlarm(INVALID_ALARM, 0, 0);
	ASSERT(AL_07, ret != E_OS_ID);

	Sequence(4);
	/* \treq AL_10 nmf B1B2E1E2 e call SetRelAlarm() with icrement value lower
	 * than zero
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetRelAlarm(Alarm1, -1, 0);
	ASSERT(AL_10, ret != E_OS_VALUE);

	Sequence(5);
	/* \treq AL_11 nmf B1B2E1E2 e call SetRelAlarm() with icrement value greater
	 * than maxallowedvalue
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetRelAlarm(Alarm1, AlarmBase.maxallowedvalue+1, 0);
	ASSERT(AL_11, ret != E_OS_VALUE);

	Sequence(6);
	/* \treq AL_12 nmf B1B2E1E2 e call SetRelAlarm() with cycle value lower
	 * than mincycle
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetRelAlarm(Alarm1, 0, AlarmBase.mincycle-1);
	ASSERT(AL_12, ret != E_OS_VALUE);

	Sequence(7);
	/* \treq AL_13 nmf B1B2E1E2 e call SetRelAlarm() with cycle value greater
	 * than maxallowedvalue
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetRelAlarm(Alarm1, 0, AlarmBase.maxallowedvalue+1);
	ASSERT(AL_13, ret != E_OS_VALUE);

	Sequence(8);
	/* \treq AL_16 nmf B1B2E1E2 e call SetAbsAlarm() with invalid alarm ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = SetAbsAlarm(INVALID_ALARM, 0, 0);
	ASSERT(AL_16, ret != E_OS_ID);

	Sequence(9);
	/* \treq AL_19 nmf B1B2E1E2 e call SetAbsAlarm() with increment value lower
	 * than zero
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetAbsAlarm(Alarm1, -1, 0);
	ASSERT(AL_19, ret != E_OS_VALUE);

	Sequence(10);
	/* \treq AL_20 nmf B1B2E1E2 e call SetAbsAlarm() with increment value
	 * greater than maxallowedvalue
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetAbsAlarm(Alarm1, AlarmBase.maxallowedvalue+1, 0);
	ASSERT(AL_20, ret != E_OS_VALUE);

	Sequence(11);
	/* \treq AL_21 nmf B1B2E1E2 e call SetAbsAlarm() with cycle value lower
	 * than mincycle
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetAbsAlarm(Alarm1, 0, AlarmBase.mincycle-1);
	ASSERT(AL_21, ret != E_OS_VALUE);

	Sequence(12);
	/* \treq AL_22 nmf B1B2E1E2 e call SetAbsAlarm() with cycle value greater
	 * than maxallowedvalue
	 *
	 * \result Service returns E_OS_VALUE
	 */
	ret = SetAbsAlarm(Alarm1, 0, AlarmBase.maxallowedvalue+1);
	ASSERT(AL_22, ret != E_OS_VALUE);

	Sequence(13);
	/* \treq AL_25 nmf B1B2E1E2 e call CancelAlaarm() with invalid alarm ID
	 *
	 * \result Service returns E_OS_ID
	 */
	ret = CancelAlarm(INVALID_ALARM);
	ASSERT(AL_25, ret != E_OS_ID);

	Sequence(14);

	/* evaluate conformance tests */
	ConfTestEvaluation();

	/* finish the conformance test */
	ConfTestFinish();
}

/* This task is not used, only to change the scheduling police */
TASK(Task2)
{
	TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


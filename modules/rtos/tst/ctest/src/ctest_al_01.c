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


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

/** \brief FreeOSEK Os Conformance Test for the Error Handling, Test Sequence 1
 **
 ** \file FreeOSEK/Os/tst/ctest/src/ctest_eh_01.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT Conformance Test
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EH Error Handling
 ** @{ */
/** \addtogroup FreeOSEK_Os_CT_EH_01 Test Sequence 1
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
#include "ctest_eh_01.h"	/* include test header file */
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

void StartupHook(void)
{
	StatusType ret;

	Sequence(0);
	/* \treq EH_06 nmf B1B2E1E2 se Check StartupHook: Start OS
	 *
	 * \result StartupHook is called after initialisation of OS
	 */
	ret = ActivateTask(Task1);
	ASSERT(EH_06, ret != E_OK);
}

void PreTaskHook(void)
{
	static uint8 count = 0;
	StatusType ret;
	TaskType TaskID;
	TaskStateType TaskState;

	switch (count)
	{
		case 0:
			/* increment pre task hook call count */
			count++;

			Sequence(1);
			/* \treq EH_04 nmf B1B2E1E2 se Check PreTaskHook/PostTaskHook:
			 * Force rescheduling
			 *
			 * \result PreTaskHook is called before executing the new task,
			 * but after the transition to running state. PostTaskHook is called
			 * after existing the current task but before leaving the task's
			 * running state.
			 */
			ret = GetTaskID(&TaskID);
			ASSERT(EH_04, ret != E_OK);
			ASSERT(EH_04, TaskID != Task1);

			Sequence(2);
			ret = GetTaskState(Task1, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != RUNNING);

			Sequence(3);
			ret = GetTaskState(Task2, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != SUSPENDED);
			break;
		case 1:
			/* increment pre task hook call count */
			count++;

			Sequence(8);
			/* \treq EH_04 nmf B1B2E1E2 se Check PreTaskHook/PostTaskHook:
			 * Force rescheduling
			 *
			 * \result PreTaskHook is called before executing the new task,
			 * but after the transition to running state. PostTaskHook is called
			 * after existing the current task but before leaving the task's
			 * running state.
			 */
			ret = GetTaskID(&TaskID);
			ASSERT(EH_04, ret != E_OK);
			ASSERT(EH_04, TaskID != Task2);

			Sequence(9);
			ret = GetTaskState(Task1, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != READY);

			Sequence(10);
			ret = GetTaskState(Task2, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != RUNNING);
			break;
		case 2:
			/* increment pre task hook call count */
			count++;

			Sequence(17);
			/* \treq EH_04 nmf B1B2E1E2 se Check PreTaskHook/PostTaskHook:
			 * Force rescheduling
			 *
			 * \result PreTaskHook is called before executing the new task,
			 * but after the transition to running state. PostTaskHook is called
			 * after existing the current task but before leaving the task's
			 * running state.
			 */
			ret = GetTaskID(&TaskID);
			ASSERT(EH_04, ret != E_OK);
			ASSERT(EH_04, TaskID != Task1);

			Sequence(18);
			ret = GetTaskState(Task1, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != RUNNING);

			Sequence(19);
			ret = GetTaskState(Task2, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != SUSPENDED);
			break;
		default:
			while(1);
			break;
	}
}

void PostTaskHook(void)
{
	static uint8 count = 0;
	StatusType ret;
	TaskType TaskID;
	TaskStateType TaskState;

	switch (count)
	{
		case 0:
			/* increment post task hook call count */
			count++;

			Sequence(5);
			/* \treq EH_04 nmf B1B2E1E2 se Check PreTaskHook/PostTaskHook:
			 * Force rescheduling
			 *
			 * \result PreTaskHook is called before executing the new task,
			 * but after the transition to running state. PostTaskHook is called
			 * after existing the current task but before leaving the task's
			 * running state.
			 */
			ret = GetTaskID(&TaskID);
			ASSERT(EH_04, ret != E_OK);
			ASSERT(EH_04, TaskID != Task1);

			Sequence(6);
			ret = GetTaskState(Task1, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != RUNNING);

			Sequence(7);
			ret = GetTaskState(Task2, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != READY);
			break;
		case 1:
			/* increment post task hook call count */
			count++;

			Sequence(14);
			/* \treq EH_04 nmf B1B2E1E2 se Check PreTaskHook/PostTaskHook:
			 * Force rescheduling
			 *
			 * \result PreTaskHook is called before executing the new task,
			 * but after the transition to running state. PostTaskHook is called
			 * after existing the current task but before leaving the task's
			 * running state.
			 */
			ret = GetTaskID(&TaskID);
			ASSERT(EH_04, ret != E_OK);
			ASSERT(EH_04, TaskID != Task2);

			Sequence(15);
			ret = GetTaskState(Task1, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != READY);

			Sequence(16);
			ret = GetTaskState(Task2, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != RUNNING);
			break;
		case 2:
			/* increment post task hook call count */
			count++;

			Sequence(21);
			/* \treq EH_04 nmf B1B2E1E2 se Check PreTaskHook/PostTaskHook:
			 * Force rescheduling
			 *
			 * \result PreTaskHook is called before executing the new task,
			 * but after the transition to running state. PostTaskHook is called
			 * after existing the current task but before leaving the task's
			 * running state.
			 */
			ret = GetTaskID(&TaskID);
			ASSERT(EH_04, ret != E_OK);
			ASSERT(EH_04, TaskID != Task1);

			Sequence(22);
			ret = GetTaskState(Task1, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != RUNNING);

			Sequence(23);
			ret = GetTaskState(Task2, &TaskState);
			ASSERT(OTHER, ret != E_OK);
			ASSERT(OTHER, TaskState != SUSPENDED);

			Sequence(24);

			/* evaluate conformance tests */
			ConfTestEvaluation();

			/* finish the conformance test */
			ConfTestFinish();
			break;
		default:
			while(1);
			break;
	}
}

void ErrorHook(void)
{
	Sequence(12);
	/* \treq EH_05 nmf B1B2E1E2 se Check ErrorHook:
	 * Force error
	 *
	 * \result Errorhook is called at the end of a system service which has a
	 * return value not equal to E_OK
	 *
	 */
	ASSERT(EH_05,0);
}

TASK(Task1)
{
	StatusType ret;

	Sequence(4);
	ret = ActivateTask(Task2);
	ASSERT(OTHER, ret != E_OK);

#if (CT_SCHEDULING_Task1 == CT_NON_PREEMPTIVE)
	/* force scheduling */
	Schedule();
#endif /* #if (CT_SCHEDULING_TASK1 == CT_NON_PREEMPTIVE) */

	Sequence(20);
	TerminateTask();
}

TASK(Task2)
{
	StatusType ret;
	TickType Tick;

	Sequence(11);
	ret = GetAlarm(Alarm1, &Tick);
	ASSERT(OTHER, ret != E_OS_NOFUNC);

	Sequence(13);
	TerminateTask();
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


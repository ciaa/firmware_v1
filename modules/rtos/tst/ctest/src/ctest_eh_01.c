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


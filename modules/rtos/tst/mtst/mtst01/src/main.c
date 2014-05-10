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

/** \brief Project Main Entry Point
 **
 ** main function implementation file
 **
 ** \file mtst01/src/main.c
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_MT
 ** @{ */
/** \addtogroup FreeOSEK_Os_MT_01
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090130 v0.1.1 MaCe add MAIN_MEMMAP check
 * 20080812 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "TestSuite.h"
#include "stdio.h"
#include "main.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
unsigned int mt_ErrorHook_Counter;
unsigned int mt_ErrorHook_Api;
unsigned int mt_ErrorHook_Param1;
unsigned int mt_ErrorHook_Param2;
unsigned int mt_ErrorHook_Param3;
unsigned int mt_ErrorHook_Ret;

unsigned int mt_TestCase;
unsigned int mt_StateCounter;

TS_ResultType mt_TaskAssert;

#endif

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
#endif

int main
(
	void
)
{
	StartOS(AppMode1);

	/* shall never return */
	while(1);

	return 0;
}

void ErrorHook(void)
{
	mt_ErrorHook_Counter++;
	mt_ErrorHook_Api = OSErrorGetServiceId();
	mt_ErrorHook_Param1 = OSErrorGetParam1();
	mt_ErrorHook_Param2 = OSErrorGetParam2();
	mt_ErrorHook_Param3 = OSErrorGetParam3();
	mt_ErrorHook_Ret = OSErrorGetRet();
}

TEST(OS_0001)
{
	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0001);

	ASSERT(OSServiceId_ActivateTask					!= 1);
	ASSERT(OSServiceId_TerminateTask					!= 2);
	ASSERT(OSServiceId_ChainTask						!= 3);
	ASSERT(OSServiceId_Schedule						!= 4);
	ASSERT(OSServiceId_GetTaskID						!= 5);
	ASSERT(OSServiceId_GetTaskState					!= 6);
	ASSERT(OSServiceId_DisableAllInterrupts		!= 7);
	ASSERT(OSServiceId_EnableAllInterrupts			!= 8);
	ASSERT(OSServiceId_SuspendAllInterrupts		!= 9);
	ASSERT(OSServiceId_ResumeAllInterrupts			!= 10);
	ASSERT(OSServiceId_SuspendOSInterrupts			!= 11);
	ASSERT(OSServiceId_ResumeOSInterrupts			!= 12);
	ASSERT(OSServiceId_GetResource					!= 13);
	ASSERT(OSServiceId_ReleaseResource				!= 14);
	ASSERT(OSServiceId_SetEvent						!= 15);
	ASSERT(OSServiceId_ClearEvent						!= 16);
	ASSERT(OSServiceId_GetEvent						!= 17);
	ASSERT(OSServiceId_WaitEvent						!= 18);
	ASSERT(OSServiceId_GetAlarmBase					!= 19);
	ASSERT(OSServiceId_GetAlarm						!= 20);
	ASSERT(OSServiceId_SetRelAlarm					!= 21);
	ASSERT(OSServiceId_SetAbsAlarm					!= 22);
	ASSERT(OSServiceId_CancelAlarm					!= 23);
	ASSERT(OSServiceId_GetActiveApplicationMode	!= 24);
	ASSERT(OSServiceId_StartOS							!= 25);
	ASSERT(OSServiceId_ShutdownOS						!= 26);

	return TS_OK;
}

TEST(OS_0002)
{
	TaskStateType State;
	StatusType ret;

	mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0002);
	mt_SetTaskAssert(TS_OK);

	mt_SetMark(0);

	/* \reqt OSEK_SYS_3.1.1-1/2 */
	GetTaskState(TaskA, &State);
	ASSERT(State != SUSPENDED);
	GetTaskState(InitTask, &State);
	ASSERT(State != RUNNING);

	/* \reqt OSEK_SYS_3.1 */
	ret = ActivateTask(TaskA);
	mt_SetMark(2);
	/* \reqt OSEK_SYS_3.1.3-1/2 */
   ASSERT(mt_ErrorHook_Counter != 0);
	/* \reqt OSEK_SYS_3.1.4-1/2 */
	ASSERT(mt_GetMark() != 2);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	/* \reqt OSEK_SYS_3.1.7-1/2 */
	ASSERT(ret != E_OK)

	ret = ActivateTask(TaskB);
	mt_SetMark(3);
	ASSERT(mt_GetMark() != 3);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	/* add wait for an alamr */
	/* ASSERT(mt_GetMark() != 4); */

	ret = ActivateTask(TaskB);
   ASSERT(mt_GetTaskAssert() != TS_OK);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	/* \reqt OSEK_SYS_3.1.7-2/2 */
	ret = ActivateTask(TaskB);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	ASSERT(ret != E_OS_LIMIT);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ActivateTask);
	ASSERT(mt_ErrorHook_Param1 != TaskB);
	ASSERT(mt_ErrorHook_Ret != E_OS_LIMIT);

	ret = ActivateTask(INVALID_TASK);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ActivateTask);
	ASSERT(mt_ErrorHook_Param1 != INVALID_TASK);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);
	/* \reqt OSEK_SYS_3.1.8 */
	ASSERT(ret != E_OS_ID)

	return TS_OK;
}

TEST(OS_0003)
{
	StatusType ret;
	TaskStateType State;

	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0003);
	mt_SetTaskAssert(TS_OK);
	mt_SetMark(0);

	ActivateTask(TaskA);
	mt_SetMark(2);
	GetTaskState(TaskA, &State);
	/* \reqt OSEK_SYS_3.2.1-2/2 */
	ASSERT(State != SUSPENDED);
	ASSERT(mt_GetMark() != 2);

	GetResource(Resource1);
	/* \reqt OSEK_SYS_3.2.7-1/2 */
	ret = TerminateTask();
   ASSERT(ret != E_OS_RESOURCE);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_TerminateTask);
	ASSERT(mt_ErrorHook_Ret != E_OS_RESOURCE);

	ReleaseResource(Resource1);

	return TS_OK;
}

TEST(OS_0004)
{
	StatusType ret;

	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0004);
	mt_SetTaskAssert(TS_OK);
	mt_SetMark(0);

	ActivateTask(TaskA);
	ASSERT(mt_GetMark() != 2);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	mt_SetMark(3);
	ActivateTask(TaskD);
	ASSERT(mt_GetMark() != 5);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	ret = ChainTask(TaskB);
	ASSERT(ret != E_OS_LIMIT);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ChainTask);
	ASSERT(mt_ErrorHook_Param1 != TaskB);
	ASSERT(mt_ErrorHook_Ret != E_OS_LIMIT);

	ret = ChainTask(INVALID_TASK);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ChainTask);
	ASSERT(mt_ErrorHook_Param1 != INVALID_TASK);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

	GetResource(Resource1);
	ret = ChainTask(TaskA);
	ASSERT(ret != E_OS_RESOURCE);
	ASSERT(mt_ErrorHook_Counter != 3);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ChainTask);
	ASSERT(mt_ErrorHook_Param1 != TaskA);
	ASSERT(mt_ErrorHook_Ret != E_OS_RESOURCE);
	ReleaseResource(Resource1);

	return TS_OK;
}

TEST(OS_0005)
{
	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0005);
	mt_SetTaskAssert(TS_OK);
	mt_SetMark(0);

	return TS_OK;
}

TEST(OS_0006)
{
	StatusType ret;
	TaskType TaskID = 0;

	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0006);
	mt_SetTaskAssert(TS_OK);
	mt_SetMark(0);

	ret = GetTaskID(&TaskID);
	ASSERT(mt_ErrorHook_Counter != 0);
	ASSERT(ret != E_OK);
	ASSERT(TaskID != InitTask);

	return TS_OK;
}

TEST(OS_0007)
{
	StatusType ret;
	TaskStateType State;

	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0007);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	ret = GetTaskState(InitTask, &State);
	ASSERT(mt_ErrorHook_Counter != 0);
	ASSERT(ret != E_OK);
	ASSERT(State != RUNNING);

	ret = GetTaskState(TaskB, &State);
	ASSERT(mt_ErrorHook_Counter != 0);
	ASSERT(ret != E_OK);
	ASSERT(State != READY);

	ret = GetTaskState(TaskC, &State);
	ASSERT(mt_ErrorHook_Counter != 0);
	ASSERT(ret != E_OK);
	ASSERT(State != SUSPENDED);

	ret = GetTaskState(INVALID_TASK, &State);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetTaskState);
	ASSERT(mt_ErrorHook_Param1 != INVALID_TASK);
	ASSERT(mt_ErrorHook_Param2 != (unsigned int)&State);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);
	ASSERT(ret != E_OS_ID);

	return TS_OK;
}

TEST(OS_0008)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0008);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	DisableAllInterrupts();

	EnableAllInterrupts();

   return TS_OK;
}

TEST(OS_0009)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0009);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

   DisableAllInterrupts();

	EnableAllInterrupts();

   return TS_OK;
}

TEST(OS_0010)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0010);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	SuspendAllInterrupts();

	ResumeAllInterrupts();

   return TS_OK;
}

TEST(OS_0011)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0011);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	SuspendAllInterrupts();

	ResumeAllInterrupts();


   return TS_OK;
}

TEST(OS_0012)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0012);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	SuspendOSInterrupts();

	ResumeOSInterrupts();

   return TS_OK;
}

TEST(OS_0013)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0013);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	SuspendOSInterrupts();

	ResumeOSInterrupts();

   return TS_OK;
}

TEST(OS_0014)
{
	StatusType ret;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0014);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	ret = GetResource(Resource1);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	ret = GetResource(Resource1);
	ASSERT(ret != E_OS_ACCESS);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetResource);
	ASSERT(mt_ErrorHook_Param1 != Resource1);
	ASSERT(mt_ErrorHook_Ret != E_OS_ACCESS);

	ReleaseResource(Resource1);

	ret = GetResource(Resource2);
	ASSERT(ret != E_OS_ACCESS);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetResource);
	ASSERT(mt_ErrorHook_Param1 != Resource2);
	ASSERT(mt_ErrorHook_Ret != E_OS_ACCESS);

	ret = GetResource(INVALID_RESOURCE);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 3);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetResource);
	ASSERT(mt_ErrorHook_Param1 != INVALID_RESOURCE);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

   return TS_OK;
}

TEST(OS_0015)
{
   StatusType ret;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0015);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	GetResource(Resource1);
	ret = ReleaseResource(Resource1);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	ret = ReleaseResource(INVALID_RESOURCE);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ReleaseResource);
	ASSERT(mt_ErrorHook_Param1 != INVALID_RESOURCE);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

	ret = ReleaseResource(Resource1);
	ASSERT(ret != E_OS_NOFUNC);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_ReleaseResource);
	ASSERT(mt_ErrorHook_Param1 != Resource1);
	ASSERT(mt_ErrorHook_Ret != E_OS_NOFUNC);

	return TS_OK;
}

TEST(OS_0016)
{
	StatusType ret;
	mt_ErrorHook_Counter = 0;
	mt_SetTestCase(OS_0016);
	mt_SetTaskAssert(TS_OK);
	mt_SetMark(0);

	ret = SetEvent(INVALID_TASK, Event1);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_SetEvent);
	ASSERT(mt_ErrorHook_Param1 != INVALID_TASK);
	ASSERT(mt_ErrorHook_Param2 != Event1);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

	ret = SetEvent(TaskB, Event2);
	ASSERT(ret != E_OS_ACCESS);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_SetEvent);
	ASSERT(mt_ErrorHook_Param1 != TaskB);
	ASSERT(mt_ErrorHook_Param2 != Event2);
	ASSERT(mt_ErrorHook_Ret != E_OS_ACCESS);

	ret = SetEvent(TaskA, Event1);
	ASSERT(ret != E_OS_STATE);
	ASSERT(mt_ErrorHook_Counter != 3);
	ASSERT(mt_ErrorHook_Api != OSServiceId_SetEvent);
	ASSERT(mt_ErrorHook_Param1 != TaskA);
	ASSERT(mt_ErrorHook_Param2 != Event1);
	ASSERT(mt_ErrorHook_Ret != E_OS_STATE);

	mt_SetMark(1);
	ActivateTask(TaskA);
	ASSERT(mt_GetMark() != 2);
	mt_SetMark(3);
	ret = SetEvent(TaskA, Event1);
	ASSERT(ret != E_OK);
	ASSERT(mt_GetMark() != 4);

	return TS_OK;
}

TEST(OS_0017)
{
   StatusType ret;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0017);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	ret = ClearEvent(Event1);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	mt_SetMark(0);
	ActivateTask(TaskC);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	ASSERT(mt_GetMark() != 1);

	return TS_OK;
}

TEST(OS_0018)
{
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0018);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	mt_SetMark(0);
	ActivateTask(TaskC);
	ASSERT(mt_GetMark() != 1);
	ASSERT(mt_GetTaskAssert() != TS_OK);

	mt_SetMark(0);
	ActivateTask(TaskA);
	ASSERT(mt_GetTaskAssert() != TS_OK);
	mt_SetMark(3);
	ASSERT(mt_GetMark() != 3);
	SetEvent(TaskA, Event1);
	ASSERT(mt_GetMark() != 4);
	ASSERT(mt_GetTaskAssert() != TS_OK);

   return TS_OK;
}

TEST(OS_0019)
{
	StatusType ret;
   AlarmBaseType Info;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0019);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	ret = GetAlarmBase(WakeTaskE, &Info);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	ret = GetAlarmBase(INVALID_ALARM, &Info);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetAlarmBase);
	ASSERT(mt_ErrorHook_Param1 != INVALID_ALARM);
	ASSERT(mt_ErrorHook_Param2 != (unsigned int)&Info);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

	return TS_OK;
}

TEST(OS_0020)
{
	StatusType ret;
	TickType Tick;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0020);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	ret = GetAlarm(WakeTaskE, &Tick);
	ASSERT(ret != E_OS_NOFUNC);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetAlarm);
	ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
	ASSERT(mt_ErrorHook_Param2 != (unsigned int)&Tick);
	ASSERT(mt_ErrorHook_Ret != E_OS_NOFUNC);

	ret = GetAlarm(INVALID_ALARM, &Tick);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_GetAlarm);
	ASSERT(mt_ErrorHook_Param1 != INVALID_ALARM);
	ASSERT(mt_ErrorHook_Param2 != (unsigned int )&Tick);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);


   return TS_OK;
}

TEST(OS_0021)
{
   StatusType ret;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0021);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	SetRelAlarm(SetEventInitTask, 15, 0);
	ret = SetRelAlarm(WakeTaskE, 10, 10);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);
	mt_SetMark(1);
	WaitEvent(Event1);
	mt_SetMark(3);
	ASSERT(mt_GetMark() != 3);

	ret = SetRelAlarm(WakeTaskE, 100, 150);
	ASSERT(ret != E_OS_STATE);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_SetRelAlarm);
	ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
	ASSERT(mt_ErrorHook_Param2 != 100);
	ASSERT(mt_ErrorHook_Param3 != 150);
	ASSERT(mt_ErrorHook_Ret != E_OS_STATE);

	ret = SetRelAlarm(INVALID_ALARM, 100, 150);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_SetRelAlarm);
	ASSERT(mt_ErrorHook_Param1 != INVALID_ALARM);
	ASSERT(mt_ErrorHook_Param2 != 100);
	ASSERT(mt_ErrorHook_Param3 != 150);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

	CancelAlarm(WakeTaskE);
	ret = SetRelAlarm(WakeTaskE, -1, 150);
	ASSERT(ret != E_OS_VALUE);
	ASSERT(mt_ErrorHook_Counter != 3);
	ASSERT(mt_ErrorHook_Api != OSServiceId_SetRelAlarm);
	ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
	ASSERT(mt_ErrorHook_Param2 != -1);
	ASSERT(mt_ErrorHook_Param3 != 150);
	ASSERT(mt_ErrorHook_Ret != E_OS_VALUE);

   ret = SetRelAlarm(WakeTaskE, 100, 1);
   ASSERT(ret != E_OS_VALUE);
   ASSERT(mt_ErrorHook_Counter != 4);
   ASSERT(mt_ErrorHook_Api != OSServiceId_SetRelAlarm);
   ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
   ASSERT(mt_ErrorHook_Param2 != 100);
   ASSERT(mt_ErrorHook_Param3 != 1);
   ASSERT(mt_ErrorHook_Ret != E_OS_VALUE);

	return TS_OK;
};

TEST(OS_0022)
{
   StatusType ret;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0022);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	ret = SetAbsAlarm(WakeTaskE, 100, 100);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

   ret = SetAbsAlarm(WakeTaskE, 100, 150);
   ASSERT(ret != E_OS_STATE);
   ASSERT(mt_ErrorHook_Counter != 1);
   ASSERT(mt_ErrorHook_Api != OSServiceId_SetAbsAlarm);
   ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
   ASSERT(mt_ErrorHook_Param2 != 100);
   ASSERT(mt_ErrorHook_Param3 != 150);
   ASSERT(mt_ErrorHook_Ret != E_OS_STATE);

   ret = SetAbsAlarm(INVALID_ALARM, 100, 150);
   ASSERT(ret != E_OS_ID);
   ASSERT(mt_ErrorHook_Counter != 2);
   ASSERT(mt_ErrorHook_Api != OSServiceId_SetAbsAlarm);
   ASSERT(mt_ErrorHook_Param1 != INVALID_ALARM);
   ASSERT(mt_ErrorHook_Param2 != 100);
   ASSERT(mt_ErrorHook_Param3 != 150);
   ASSERT(mt_ErrorHook_Ret != E_OS_ID);

   CancelAlarm(WakeTaskE);
   ret = SetAbsAlarm(WakeTaskE, -1, 150);
   ASSERT(ret != E_OS_VALUE);
   ASSERT(mt_ErrorHook_Counter != 3);
   ASSERT(mt_ErrorHook_Api != OSServiceId_SetAbsAlarm);
   ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
   ASSERT(mt_ErrorHook_Param2 != -1);
   ASSERT(mt_ErrorHook_Param3 != 150);
   ASSERT(mt_ErrorHook_Ret != E_OS_VALUE);

   ret = SetAbsAlarm(WakeTaskE, 100, 1);
   ASSERT(ret != E_OS_VALUE);
   ASSERT(mt_ErrorHook_Counter != 4);
   ASSERT(mt_ErrorHook_Api != OSServiceId_SetAbsAlarm);
   ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
   ASSERT(mt_ErrorHook_Param2 != 100);
   ASSERT(mt_ErrorHook_Param3 != 1);
   ASSERT(mt_ErrorHook_Ret != E_OS_VALUE);

	return TS_OK;

}

TEST(OS_0023)
{
   StatusType ret;
   mt_ErrorHook_Counter = 0;
   mt_SetTestCase(OS_0023);
   mt_SetTaskAssert(TS_OK);
   mt_SetMark(0);

	SetRelAlarm(WakeTaskE, 100, 100);
	ret = CancelAlarm(WakeTaskE);
	ASSERT(ret != E_OK);
	ASSERT(mt_ErrorHook_Counter != 0);

	ret = CancelAlarm(WakeTaskE);
	ASSERT(ret != E_OS_NOFUNC);
	ASSERT(mt_ErrorHook_Counter != 1);
	ASSERT(mt_ErrorHook_Api != OSServiceId_CancelAlarm);
	ASSERT(mt_ErrorHook_Param1 != WakeTaskE);
	ASSERT(mt_ErrorHook_Ret != E_OS_NOFUNC);

	ret = CancelAlarm(INVALID_ALARM);
	ASSERT(ret != E_OS_ID);
	ASSERT(mt_ErrorHook_Counter != 2);
	ASSERT(mt_ErrorHook_Api != OSServiceId_CancelAlarm);
	ASSERT(mt_ErrorHook_Param1 != INVALID_ALARM);
	ASSERT(mt_ErrorHook_Ret != E_OS_ID);

	return TS_OK;
}

#endif

#endif

ISR(CanRx)
{
}

ISR(CanTx)
{
}

ISR(NMI)
{
}

TASK(InitTask)
{
	TS_RunTestSuite();

	ShutdownOS(0);
	TerminateTask();
}

TASK(TaskA)
{
	StatusType ret;
	TaskStateType State;
	EventMaskType Events;
	unsigned int test = 0;

	test++;

	switch(mt_GetTestCase())
	{
		case OS_0002:
			mt_SetMark(1);
			/* \reqt OSEK_SYS_3.1.1-2/2 */
			GetTaskState(TaskA, &State);
			TASKASSERT(State != RUNNING);
			GetTaskState(InitTask, &State);
			TASKASSERT(State != READY);
			/* \reqt OSEK_SYS_3.1.2 */
			TASKASSERT(test != 1);
			GetEvent(TaskA, &Events);
			/* \reqt OSEK_SYS_3.1.6 */
			TASKASSERT(Events != 0);
			TerminateTask();
			break;
		case OS_0003:
			mt_SetMark(1);
			GetTaskState(TaskA, &State);
			/* \reqt OSEK_SYS_3.2.1-1/2 */
			TASKASSERT(State != RUNNING);
			/* \reqt OSEK_SYS_3.2 */
			TerminateTask();
			break;
		case OS_0004:
			mt_SetMark(1);
			ChainTask(TaskC);
			break;
		case OS_0016:
			mt_SetMark(2);
			WaitEvent(Event1);
			mt_SetMark(4);
			TerminateTask();
		case OS_0018:
			mt_SetMark(1);
			SetEvent(TaskA, Event1);
			ret = WaitEvent(Event1);
			TASKASSERT(ret != E_OK);
			mt_SetMark(2);
			ClearEvent(Event1);
			WaitEvent(Event1);
			mt_SetMark(4);
			TerminateTask();
		default:
			TASKASSERT(1);
			TerminateTask();
			break;
	}

	TASKASSERT(1);
	TerminateTask();
}

TASK(TaskB)
{
	switch(mt_GetTestCase())
	{
		case OS_0002:
			mt_SetMark(4);
			TerminateTask();
			break;
		default:
			TerminateTask();
			break;
	}

	TASKASSERT(1);
	TerminateTask();
}

TASK(TaskC)
{
	StatusType ret;

	switch(mt_GetTestCase())
	{
		case OS_0004:
			mt_SetMark(2);
			TerminateTask();
			break;
		case OS_0017:
			ret = ClearEvent(Event1);
			TASKASSERT(ret != E_OS_ACCESS);
			TASKASSERT(mt_ErrorHook_Counter != 1);
			TASKASSERT(mt_ErrorHook_Api != OSServiceId_ClearEvent);
			TASKASSERT(mt_ErrorHook_Param1 != Event1);
			TASKASSERT(mt_ErrorHook_Ret != E_OS_ACCESS);
			mt_SetMark(1);
			TerminateTask();
		case OS_0018:
			ret = WaitEvent(Event1);
			TASKASSERT(ret != E_OS_ACCESS);
			TASKASSERT(mt_ErrorHook_Counter != 1);
			TASKASSERT(mt_ErrorHook_Api != OSServiceId_WaitEvent);
			TASKASSERT(mt_ErrorHook_Param1 != Event1);
			TASKASSERT(mt_ErrorHook_Ret != E_OS_ACCESS);
			mt_SetMark(1);
			TerminateTask();
		default:
			TASKASSERT(1);
			break;
	}

	TASKASSERT(1);
	TerminateTask();
}

TASK(TaskD)
{
	switch(mt_GetTestCase())
	{
		case OS_0004:
			if (mt_GetMark() == 3)
			{
				mt_SetMark(4);
				ChainTask(TaskD);
			}
			else
			{
				mt_SetMark(5);
				TerminateTask();
			}
			break;
		default:
			TASKASSERT(1);
			TerminateTask();
			break;
	}

	TerminateTask();
}

TASK(TaskE)
{
	switch(mt_GetTestCase())
	{
		case OS_0021:
			mt_SetMark(2);
			TerminateTask();
			break;
		default:
			TASKASSERT(1);
			TerminateTask();
			break;
	}

	TerminateTask();
}

ALARMCALLBACK(AlarmCallback)
{

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


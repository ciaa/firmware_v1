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

/** \brief FreeOSEK Os Internal Arch Implementation File
 **
 ** \file arm7/Os_Internal_Arch.c
 ** \arch arm7
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * KT           Tamas Kenderes
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20111015 v0.1.3 KT	renamed interrupt handlers, modified
 * 							TimerIrqHandler_Arch() to use ClearTimerInterrupt_Cpu()
 * 20090719 v0.1.2 MaCe rename file to Os_
 * 20090330 v0.1.1 MaCe add NO_EVENTS and NON_PREEMPTIVE evaluation and
 *								improvement of FIQ_Routine
 * 20081116 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
void* Osek_NewTaskPtr_Arch;

void* Osek_OldTaskPtr_Arch;

/*==================[internal data definition]===============================*/

TaskType TerminatingTask = INVALID_TASK;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void ReturnHook_Arch(void)
{
	/* Tasks shouldn't return here... */
	while(1) osekpause();

}

void CheckTerminatingTask_Arch(void)
{
	if(TerminatingTask != INVALID_TASK)
	{
//		int i;
//		for(i=0; i<TasksConst[TerminatingTask].StackSize/4; i++)
//			((uint32 *)TasksConst[TerminatingTask].StackPtr)[i] = 0;
		InitStack_Arch(TerminatingTask);
	}
	TerminatingTask = INVALID_TASK;
}

/* Task Stack Initialization */
void InitStack_Arch(uint8 TaskID)
{

	uint32 * taskStack = (uint32 *)TasksConst[TaskID].StackPtr;
	int taskStackSizeWords = TasksConst[TaskID].StackSize/4;

	taskStack[taskStackSizeWords-1] = 1<<24; /* xPSR.T = 1 */
	taskStack[taskStackSizeWords-2] = (uint32) TasksConst[TaskID].EntryPoint; /*PC*/
	taskStack[taskStackSizeWords-3] = (uint32) ReturnHook_Arch; /* stacked LR */
	taskStack[taskStackSizeWords-9] = 0xFFFFFFF9; /*current LR*/

	*(TasksConst[TaskID].TaskContext) = &(taskStack[taskStackSizeWords - 17]);

}

/* Periodic Interrupt Timer, included in all Cortex-M4 processors */
void SysTick_Handler(void)
{
#if (ALARMS_COUNT != 0)
	/* to save the context during the interrupt */
	ContextType context;
	/* counter increment */
	static CounterIncrementType CounterIncrement = 1;

	/* increment the disable interrupt conter to avoid enable the interrupts */
	IntSecure_Start();

	/* save actual context */
	context = GetCallingContext();

	/* set context to CONTEXT_DBG */
	SetActualContext(CONTEXT_DBG);

	/* call counter interrupt handler */
	CounterIncrement = IncrementCounter(0, 1 /* CounterIncrement */); /* TODO FIXME */

	/* interrupt has to be called first after so many CounterIncrement */
	/* SetCounterTime(CounterIncrement); */ /* TODO FIXME */

	/* set context back */
	SetActualContext(context);

	/* set the disable interrupt counter back */
	IntSecure_End();

#endif /* #if (ALARMS_COUNT != 0) */

	/* clear timer interrupt flag */
	//not necessary for Cortex-M3
	//ClearTimerInterrupt_Cpu();

#if 0 /* TODO */
#if (NON_PREEMPTIVE == DISABLE)
		/* check if interrupt a Task Context */
		if ( GetCallingContext() ==  CONTEXT_TASK )
		{
			if ( TasksConst[GetRunningTask()].ConstFlags.Preemtive )
			{
				/* \req TODO Rescheduling shall take place only if interrupt a
				 * preemptable task. */
				(void)Schedule();
			}
		}
#endif /* #if (NON_PREEMPTIVE == ENABLE) */
#endif
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


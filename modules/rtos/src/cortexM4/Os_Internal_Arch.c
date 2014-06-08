/* Copyright 2014, Pablo Ridolfi (UTN-FRBA)
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

/** \brief FreeOSEK Os Internal Arch Implementation File
 **
 ** \file cortexM4/Os_Internal_Arch.c
 ** \arch cortexM4
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
 * PR		Pablo Ridolfi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140608 v0.1.0 PR	First version for Cortex-M processors.
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


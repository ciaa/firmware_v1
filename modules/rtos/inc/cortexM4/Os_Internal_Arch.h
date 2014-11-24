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


#ifndef _OS_INTERNAL_ARCH_H_
#define _OS_INTERNAL_ARCH_H_

/** \brief FreeOSEK Internal Architecture Dependent Header File
 **
 ** \file cortexM4/Os_Internal_Arch.h
 ** \arch cortexM4
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*****************************************************************************
 * add your name to the developers and create for you a nick with
 * 3 or 4 letters. Please do not use any given nick.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/*
 * Initials     Name
 * ---------------------------
 * PR		Pablo Ridolfi
 */

/*****************************************************************************
 * add a new version to this file, add the date, your initials and the main
 * changes, only main changes shall be listed here. A detailed message log
 * is saved in svn log and on the tracker system since every svn login message
 * shalle indicate the related tracker id.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140608 v0.1.1 PR	First version for Cortex-M processors.
 */

/*==================[inclusions]=============================================*/
//#include "lpc43xx_cgu.h"

/*==================[macros]=================================================*/

/** \brief Osek_Internal_Arch_Cpu.h inclusion needed macro
 **
 ** This define makes the Osek_Internal.h file to include the
 ** Osek_Internal_Arch_Cpu file which is not standard for all architectures.
 ** If for the actual architecture no Osek_Internal_Arch_Cpu.h is neede
 ** remove the macro and this comment.
 **/
#define OSEK_INLCUDE_INTERNAL_ARCH_CPU

extern void * Osek_OldTaskPtr_Arch;
extern void * Osek_NewTaskPtr_Arch;
extern TaskType TerminatingTask;

/** \brief Interrupt Secure Start Macro
 **
 ** This macro will be used internaly by the OS in any part of code that
 ** has to be executed atomic.
 **/
#define IntSecure_Start() SuspendAllInterrupts()

/** \brief Interrupt Secure End Macro
 **
 ** This macro is the countra part of IntSecure_Start()
 **/
#define IntSecure_End() ResumeAllInterrupts()

/** \brief osekpause
 **
 ** This macro is called by the scheduler when not task has to be executed.
 ** If a background task is configured by the user (a full preemptive task
 ** with lower priority and which never ends) this macro will never be called.
 ** In other case the macro will be called any time that the OS has nothing
 ** else to execute. The macro may sleep the cpu for a short time to avoid
 ** overheating and full power consumption or may halt the processor always
 ** that all wakeup reasons are right configured. If nothing is running
 ** nothing my activate any task so we will keep sleeping until anything
 ** occurs, like for example an interrupt.
 **
 **/
#define osekpause()	__asm volatile("wfi")

/** \brief Call to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#define CallTask(actualtask, nexttask)										\
{																			\
	Osek_OldTaskPtr_Arch = (void*)TasksConst[(actualtask)].TaskContext;		\
	Osek_NewTaskPtr_Arch = (void*)TasksConst[(nexttask)].TaskContext;		\
	__asm__ __volatile__ (													\
		/* Call PendSV */													\
		"push {r0,r1}												\n\t"	\
		/* Activate bit PENDSVSET in Interrupt Control State Register (ICSR) */ \
		"ldr r0,=0xE000ED04											\n\t"	\
		"ldr r1,[r0]												\n\t"	\
		"orr r1,1<<28												\n\t"	\
		"str r1,[r0]												\n\t"	\
		"pop {r0,r1}												\n\t"	\
	);																		\
}

/** \brief Jmp to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#define JmpTask(task)														            \
{									                                                \
   extern TaskType WaitingTask;                                            \
   if(WaitingTask != INVALID_TASK)                                         \
   {                                                                       \
      Osek_OldTaskPtr_Arch = (void*)TasksConst[WaitingTask].TaskContext;   \
      WaitingTask = INVALID_TASK;                                          \
   }                                                                       \
   else                                                                    \
   {                                                                       \
      Osek_OldTaskPtr_Arch = (void*)0;                                     \
   }                                                                       \
	Osek_NewTaskPtr_Arch = (void*)TasksConst[(task)].TaskContext;			   \
	__asm__ __volatile__ (													            \
		/* Call PendSV */													               \
		"push {r0,r1}												\n\t"	               \
		/* Activate bit PENDSVSET in Interrupt Control State Register (ICSR) */ \
		"ldr r0,=0xE000ED04											\n\t"	            \
		"ldr r1,[r0]												\n\t"	               \
		"orr r1,1<<28												\n\t"	               \
		"str r1,[r0]												\n\t"	               \
		"pop {r0,r1}												\n\t"	               \
	);																		                  \
}

/** \brief Save context */
#define SaveContext(task)                                                  \
{                                                                          \
   extern TaskType WaitingTask;                                            \
   if(TasksVar[GetRunningTask()].Flags.State == TASK_ST_WAITING)           \
   {                                                                       \
      WaitingTask = GetRunningTask();                                      \
   }                                                                       \
   flag = 0;                                                               \
   /* remove of the Ready List */                                          \
   RemoveTask(GetRunningTask());                                           \
   /* set system context */                                                \
   SetActualContext(CONTEXT_SYS);                                          \
   /* set running task to invalid */                                       \
   SetRunningTask(INVALID_TASK);                                           \
   /* finish cirtical code */                                              \
   IntSecure_End();                                                        \
   /* call scheduler */                                                    \
   Schedule();                                                             \
   /* add this call in order to maintain counter balance when returning */ \
   IntSecure_Start();                                                      \
}

/** \brief */
#define ResetStack(task)		\
{								      \
	TerminatingTask = (task);	\
}

/** \brief Set the entry point for a task */
#define SetEntryPoint(task)   \
{								      \
	TerminatingTask = (task);	\
}

/** \brief Enable OS Interruptions
 **
 ** Enable OS configured interrupts (ISR1 and ISR2). This macro
 ** is called only ones in StartUp.c function.
 **/
#define EnableOSInterrupts() __asm volatile("cpsie i")

/** \brief Enable Interruptions
 **
 ** Enable not OS configured interrupts (ISR1 and ISR2). This macro
 ** is called only ones in StartUp.c function.
 **
 ** This macro may be empty. Maybe will be removed on the future,
 ** please use it only if necessary, in other case use EnableOSInterrupts.
 **/
#define EnableInterrupts()	EnableOSInterrupts()


/** \brief Disable OS Interruptions
 **
 ** Disable OS configured interrupts (ISR1 and ISR2).
 **/
#define DisableOSInterrupts() __asm volatile("cpsid i")

/** \brief Disable Interruptions
 **
 ** Disable not OS configured interrupts (ISR1 and ISR2). This macro
 ** is called only ones in StartUp.c function.
 **
 ** This macro may be empty. Maybe will be removed on the future,
 ** please use it only if necessary, in other case use DisableOSInterrupts.
 **/
#define DisableInterrupts()	DisableOSInterrupts()

/** \brief Get Counter Actual Value
 **
 ** This macro returns the actual value of the a counter
 **
 ** \param[in] CounterID id of the counter to be readed
 ** \return Actual value of the counter
 **/
#define GetCounter_Arch(CounterID) (CountersVar[CounterID].Time)

/** \brief Pre ISR Macro
 **
 ** This macro is called every time that an ISR Cat 2 is started
 **/
#define PreIsr2_Arch(isr)

/** \brief Post ISR Macro
 **
 ** This macro is called every time that an ISR Cat 2 is finished
 **/
#define PostIsr2_Arch(isr) Schedule()

/** \brief ShutdownOs Arch service
 **
 ** This macro is called on the ShutdownOS to perform the architecture
 ** dependent shutdown actions.
 **/
#define ShutdownOs_Arch()

/*==================[typedef]================================================*/
/*****************************************************************************
 * Please define here all needed types that will be used only internal by
 * the OS and only for this architecture and which will not depend on the
 * configuraiton. Normaly this section shall be empty.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external data declaration]==============================*/
/*****************************************************************************
 * Please declare here all exported data defined in Osek_Internal_Arch.c
 * that will be visible only internal to the OS for this architectire.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions declaration]=========================*/
void InitStack_Arch(uint8 TaskID);


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_H_ */


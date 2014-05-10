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

#ifndef _OS_INTERNAL_ARCH_H_
#define _OS_INTERNAL_ARCH_H_
/** \brief FreeOSEK Os Internal Architecture Dependent Header File
 **
 ** \file win/Os_Internal_Arch.h
 ** \arch win
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
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090408 v0.1.1 Mace add ShutdownOs_Arch
 * 20080725 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/
#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/shm.h"
#include "unistd.h"
#include "signal.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"

/*==================[macros]=================================================*/
/** \brief Lenght of the Message Queue */
#define MESSAGE_QUEUE_LENGTH  32

/** \brief Interrupt Secure Start Macro
 **
 ** This macro shall be used to disable the interrupts
 **/
#define IntSecure_Start() SuspendAllInterrupts()

/** \brief Interrupt Secure End Macro
 **
 ** This macro shall be used to restore the interrupts
 **/
#define IntSecure_End() ResumeAllInterrupts()

/** \brief osekpause
 **
 **/
#define osekpause()  		\
   {                 		\
		PreCallService();	\
	   (void)usleep(1);		\
		PostCallService();	\
   }

/** \brief Jmp to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#define JmpTask(task) \
   {                                                                                            \
      __asm__ __volatile__ ("movl %0, %%esp;" \
         "movl %2, %%eax;" \
         "movl %1, %%ebp;" \
         "jmp *%%eax;" \
         : :  "g" (TasksConst[(task)].TaskContext->tss_esp), "g" (TasksConst[(task)].TaskContext->tss_ebp) , "g" (TasksConst[(task)].TaskContext->tss_eip) : "%eax"); \
   }

/** \brief Call to an other Task
 **
 ** This function jmps to the indicated task.
 **/
/* #define CallTask(task)															\
	{																					\
		SaveContext(GetRunningTask());                              \
      *//* set as running task *//*											   \
		SetRunningTask(task);												   \
	   JmpTask(task);                                              \
		RestoreContext();                                           \
	}*/

/** \brief Save context */
#define SaveContext(task) \
   {                                   \
      /* save actual esp */            \
      __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_esp) : : "%eax" ); \
                                       \
      /* save actual ebp */            \
      __asm__ __volatile__ ("movl %%ebp, %%eax; movl %%eax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_ebp) : : "%eax" );  \
                                       \
      /* save return eip */            \
      __asm__ __volatile__ ("movl $_next, %%eax; movl %%eax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_eip) : : "%eax"); \
      __asm__ __volatile__ ("_next:"); \
   }

#define SaveWinStack() \
   {                                   \
      /* save actual win esp */        \
      __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (WinStack) : : "eax"); \
   }

/** \brief Set the entry point for a task */
#define SetEntryPoint(task)   { TasksConst[(task)].TaskContext->tss_eip = (uint32)TasksConst[(task)].EntryPoint; }

/** \brief */
#define ResetStack(task)																													      \
	{																																				      \
		TasksConst[loopi].TaskContext->tss_esp = (uint32)( TasksConst[loopi].StackPtr + TasksConst[loopi].StackSize - 4);  \
   }

#define ISR_NMI      0
#define ISR_CTR      1
#define ISR_CANRX    2
#define ISR_CANTX    3

#define EnableOSInterrupts()															\
	{																							\
		InterruptMask &= (InterruptFlagsType)~(OSEK_OS_INTERRUPT_MASK);	\
	}

#define EnableInterrupts()		\
	{									\
		InterruptState = 1;		\
	}

/** \brief Get Counter Actual Value
 **
 ** This macro returns the actual value of the counter
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
#define PostIsr2_Arch(isr)

/** \brief Pre Call Service
 **
 ** This macro shall be called before calling any posix system service
 **/
#define PreCallService()      \
	{                          \
		/* save osek stack */   \
		__asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (OsekStack) : : "eax"); \
		/* get windows stack */ \
		__asm__ __volatile__ ("movl %0, %%esp;" : : "g" (WinStack) ); \
	}

/** \brief Post Call Service
 **
 ** This macro shall be called after calling any posix system service
 **/
#define PostCallService()     \
	{                          \
		/* get windows stack */ \
		__asm__ __volatile__ ("movl %0, %%esp;" : : "g" (OsekStack) ); \
	}

/** \brief ShutdownOs Arch service
 **/
#define	ShutdownOs_Arch()

/*==================[typedef]================================================*/
/** \brief Message Queue type definition */
typedef struct {
   uint8 Lock;
   uint8 Buffer[MESSAGE_QUEUE_LENGTH];
   uint8 Last;
   uint8 First;
   uint8 Count;
} MessageQueueType;

/*==================[external data declaration]==============================*/
/** \brief Interrupt Falg
 **
 ** This variable indicate the state of the posix interrupts. If bit 0 is set
 ** interrupt 0 has been activated, if bit 1 is set interrupt 1 has been
 ** activated, and so on.
 **/
extern InterruptFlagsType InterruptFlag;

/** \brief Message Signal
 **/
extern struct sigaction MessageSignal;

/** \brief Signal Event
 **/
/* extern struct sigevent SignalEvent; */

/** \brief Osek Hardware Timer 0
 **/
/* extern uint32 OsekHWTimer0; */

extern MessageQueueType *MessageQueue;

extern int SharedMemory;

extern uint32 WinStack;

extern uint32 OsekStack;

/*==================[external functions declaration]=========================*/
extern void CallTask(TaskType OldTask, TaskType NewTask);

extern StatusType SetEvent(TaskType TaskID, EventMaskType Mask);

extern void PosixInterruptHandler(int status);

extern void HWTimerFork(uint8 timer);

extern void OsekKillSigHandler(int status);

extern void OSEK_ISR_HWTimer0(void);

extern void OSEK_ISR_HWTimer1(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_H_ */


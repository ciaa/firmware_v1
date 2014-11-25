/* Copyright 2008, 2009, 2014 Mariano Cerdeiro
 * Copyright 2014, Juan Cecconi
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

#ifndef _OS_INTERNAL_ARCH_H_
#define _OS_INTERNAL_ARCH_H_
/** \brief FreeOSEK Os Internal Architecture Dependent Header File
 **
 ** \file x86/Os_Internal_Arch.h
 ** \arch x86
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
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.3 MaCe rename file to Os_
 * 20090408 v0.1.2 Mace add ShutdownOs_Arch
 * 20090330 v0.1.2 MaCe update CallTask macro
 * 20090130 v0.1.1 MaCe remove std type definitions, now in Types.h
 * 20080725 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaLibs_CircBuf.h"
#include "stdio.h"      /* used to print debug information */
#include "signal.h"     /* used to simulate interrupts */
#include "unistd.h"     /* used to create a fork to poll the interrupts */
#include "stdlib.h"     /* used to call exit to terminate the process */
#include "time.h"       /* used to simulate the hardware timer */
#include "string.h"     /* used to call the function strerror */

/*==================[macros]=================================================*/
#define TASK_STACK_ADDITIONAL_SIZE      10000

/*==================[cputype macros]=========================================*/
/** \brief ia32 cputype definition */
#define ia32        1
/** \brief ia64 cputype definition */
#define ia64        2

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
#define osekpause()     \
{                       \
   PreCallService();    \
   (void)usleep(1);     \
   PostCallService();   \
}

/** \brief Jmp to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#if ( CPUTYPE == ia64 )
#define JmpTask(task) \
{                                                                                            \
   __asm__ __volatile__ ("movq %0, %%rsp;"                                                   \
         "movq %2, %%rax;"                                                                         \
         "movq %1, %%rbp;"                                                                         \
         "jmp *%%rax;"                                                                             \
         : :  "g" (TasksConst[(task)].TaskContext->tss_rsp), "g" (TasksConst[(task)].TaskContext->tss_rbp) , "g" (TasksConst[(task)].TaskContext->tss_rip) : "%rax"); \
}
#elif ( CPUTYPE == ia32 )
#define JmpTask(task) \
{                                                                                            \
   __asm__ __volatile__ ("movl %0, %%esp;"                                                   \
         "movl %2, %%eax;"                                                                         \
         "movl %1, %%ebp;"                                                                         \
         "jmp *%%eax;"                                                                             \
         : :  "g" (TasksConst[(task)].TaskContext->tss_esp), "g" (TasksConst[(task)].TaskContext->tss_ebp) , "g" (TasksConst[(task)].TaskContext->tss_eip) : "%eax"); \
}
#endif

/** \brief Call to an other Task
 **
 ** This function jmps to the indicated task.
 **/
#if ( CPUTYPE == ia64 )
#define CallTask(OldTask, NewTask) \
{                                                                                                                                \
   /* save actual rsp */                                                                                                         \
   __asm__ __volatile__ ("movq %%rsp, %%rax; movq %%rax, %0;" : "=g" (TasksConst[(OldTask)].TaskContext->tss_rsp) : : "%rax" );  \
   /* save actual rbp */                                                                                                         \
   __asm__ __volatile__ ("movq %%rbp, %%rax; movq %%rax, %0;" : "=g" (TasksConst[(OldTask)].TaskContext->tss_rbp) : : "%rax" );  \
   /* save return rip */                                                                                                         \
   __asm__ __volatile__ ("movq $_next, %%rax; movq %%rax, %0;" : "=g" (TasksConst[(OldTask)].TaskContext->tss_rip) : : "%rax");  \
   /* load new stack pointer */                                                                                                  \
   __asm__ __volatile__ ("movq %0, %%rsp;" : : "g" (TasksConst[(NewTask)].TaskContext->tss_rsp));                                \
   /* load new rbp and jmp to the new task */                                                                                    \
   __asm__ __volatile__ ("movq %0, %%rbx; movq %1, %%rbp; jmp *%%rbx;" : : "g" (TasksConst[(NewTask)].TaskContext->tss_rip), "g" (TasksConst[(NewTask)].TaskContext->tss_rbp));                                                                                                       \
   __asm__ __volatile__ ("_next:");                                                                                              \
}
#elif ( CPUTYPE == ia32 )
#define CallTask(OldTask, NewTask) \
{                                                                                                                                \
   /* save actual esp */                                                                                                         \
   __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (TasksConst[OldTask].TaskContext->tss_esp) : : "%eax" );    \
   /* save actual ebp */                                                                                                         \
   __asm__ __volatile__ ("movl %%ebp, %%eax; movl %%eax, %0;" : "=g" (TasksConst[OldTask].TaskContext->tss_ebp) : : "%eax" );    \
   /* save return eip */                                                                                                         \
   __asm__ __volatile__ ("movl $_next, %%eax; movl %%eax, %0" : "=g" (TasksConst[OldTask].TaskContext->tss_eip) : : "%eax");     \
   /* load new stack pointer */                                                                                                  \
   __asm__ __volatile__ ("movl %0, %%esp;" : : "g" (TasksConst[NewTask].TaskContext->tss_esp));                                  \
   /* load new ebp and jmp to the new task */                                                                                    \
   __asm__ __volatile__ ("movl %0, %%ebx; movl %1, %%ebp; jmp *%%ebx;" : : "g" (TasksConst[NewTask].TaskContext->tss_eip), "g" (TasksConst[NewTask].TaskContext->tss_ebp));                                                                                                       \
   __asm__ __volatile__ ("_next:");                                                                                              \
}
#endif

/** \brief Save context */
#if ( CPUTYPE == ia64 )
#define SaveContext(task)                                                                                                       \
{                                                                                                                               \
   /* save actual rsp */                                                                                                        \
   __asm__ __volatile__ ("movq %%rsp, %%rax; movq %%rax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_rsp) : : "%rax" );    \
   /* save actual rbp */                                                                                                        \
   __asm__ __volatile__ ("movq %%rbp, %%rax; movq %%rax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_rbp) : : "%rax" );    \
   /* save return rip */                                                                                                        \
   __asm__ __volatile__ ("movq $_next, %%rax; movq %%rax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_rip) : : "%rax");    \
   __asm__ __volatile__ ("_next:");                                                                                             \
}
#elif ( CPUTYPE == ia32 )
#define SaveContext(task)                                                                                                       \
{                                                                                                                               \
   /* save actual esp */                                                                                                        \
   __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_esp) : : "%eax" );    \
   /* save actual ebp */                                                                                                        \
   __asm__ __volatile__ ("movl %%ebp, %%eax; movl %%eax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_ebp) : : "%eax" );    \
   /* save return eip */                                                                                                        \
   __asm__ __volatile__ ("movl $_next, %%eax; movl %%eax, %0;" : "=g" (TasksConst[(task)].TaskContext->tss_eip) : : "%eax");    \
   __asm__ __volatile__ ("_next:");                                                                                             \
}
#endif

/** \brief Set the entry point for a task */
#if ( CPUTYPE == ia64 )
#define SetEntryPoint(task)                                                                                           \
{                                                                                                                     \
   TasksConst[(task)].TaskContext->tss_rip = (uint64)TasksConst[(task)].EntryPoint;                                   \
}
#elif ( CPUTYPE == ia32 )
#define SetEntryPoint(task)                                                                                           \
{                                                                                                                     \
   TasksConst[(task)].TaskContext->tss_eip = (uint32)TasksConst[(task)].EntryPoint;                                   \
}
#endif
/** \brief */
#if ( CPUTYPE == ia64 )
#define ResetStack(task)                                                                                              \
{                                                                                                                     \
   TasksConst[task].TaskContext->tss_rsp = (uint64)( TasksConst[task].StackPtr + TasksConst[task].StackSize - 8);  \
}
#elif ( CPUTYPE == ia32 )
#define ResetStack(task)                                                                                              \
{                                                                                                                     \
   TasksConst[task].TaskContext->tss_esp = (uint32)( TasksConst[task].StackPtr + TasksConst[task].StackSize - 4);  \
}
#endif

#define ISR_NMI      0
#define ISR_CTR      1
#define ISR_CANRX    2
#define ISR_CANTX    3

#define EnableOSInterrupts()                                         \
{                                                                    \
   InterruptMask &= (InterruptFlagsType)~(OSEK_OS_INTERRUPT_MASK);   \
}

#define EnableInterrupts() \
{                       \
   InterruptState = 1;  \
}

#define DisableOSInterrupts()                                        \
{                                                                    \
   InterruptMask |= (InterruptFlagsType)(OSEK_OS_INTERRUPT_MASK);    \
}

#define DisableInterrupts() \
{                       \
   InterruptState = 0;  \
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
#define PostIsr2_Arch(isr) \
   Schedule();

#if ( CPUTYPE == ia64 )
#define SaveOsStack()                                                                      \
{                                                                                           \
   /* save actual Os stack */                                                              \
   __asm__ __volatile__ ("movq %%rsp, %%rax; movq %%rax, %0;" : "=g" (OsStack) : : "rax"); \
}
#elif ( CPUTYPE == ia32 )
#define SaveOsStack()                                                                      \
{                                                                                           \
   /* save actual Os esp */                                                                \
   __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (OsStack) : : "eax"); \
}
#endif

/** \brief Pre Call Service
 **
 ** This macro shall be called before calling any Os system service
 **/
#if ( CPUTYPE == ia64 )
#define PreCallService()                                                                     \
{                                                                                            \
   /* save osek stack */                                                                     \
   __asm__ __volatile__ ("movq %%rsp, %%rax; movq %%rax, %0;" : "=g" (OsekStack) : : "rax"); \
   /* get Os stack */                                                                       \
   __asm__ __volatile__ ("movq %0, %%rsp;" : : "g" (OsStack) );                             \
}
#elif ( CPUTYPE == ia32 )
#define PreCallService()                                                                     \
{                                                                                            \
   /* save osek stack */                                                                     \
   __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (OsekStack) : : "eax"); \
   /* get Os stack */                                                                       \
   __asm__ __volatile__ ("movl %0, %%esp;" : : "g" (OsStack) );                             \
}
#endif

/** \brief Post Call Service
 **
 ** This macro shall be called after calling any Os system service
 **/
#if ( CPUTYPE == ia64 )
#define PostCallService()                                                                       \
{                                                                                               \
   /* save actual Os stack */                                                                  \
   __asm__ __volatile__ ("movq %%rsp, %%rax; movq %%rax, %0;" : "=g" (OsStack) : : "rax");     \
   /* get osek stack */                                                                         \
   __asm__ __volatile__ ("movq %0, %%rsp;" : : "g" (OsekStack) );                               \
}
#elif ( CPUTYPE == ia32 )
#define PostCallService()                                                                       \
{                                                                                               \
   /* save actual Os stack */                                                                  \
   __asm__ __volatile__ ("movl %%esp, %%eax; movl %%eax, %0;" : "=g" (OsStack) : : "eax");     \
   /* get osek stack */                                                                         \
   __asm__ __volatile__ ("movl %0, %%esp;" : : "g" (OsekStack) );                               \
}
#endif

/** \brief ShutdownOs Arch service
 **/
#define ShutdownOs_Arch()  \
{                          \
   PreCallService();       \
   PostCallService();      \
}

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
/** \brief Interrupt Falg
 **
 ** This variable indicate the state of the Os interrupts. If bit 0 is set
 ** interrupt 0 has been activated, if bit 1 is set interrupt 1 has been
 ** activated, and so on.
 **/
extern InterruptFlagsType InterruptFlag;

/** \brief Osek Hardware Timer 0
 **/
extern uint32 OsekHWTimer0;

extern uint8 * OSEK_IntCircBuffer;

extern ciaaLibs_CircBufType * OSEK_IntCircBuf;

extern bool Os_Terminate_Flag;

extern pthread_t Os_Thread_Timer;

/** \brief Os Stack
 **
 ** This variable is used to save the Os stack used to call the system
 ** (linux) services from FreeOSEK
 **/
#ifdef CPUTYPE
#if ( CPUTYPE == ia64 )
extern uint64 OsStack;
#elif ( CPUTYPE == ia32 )
extern uint32 OsStack;
#else /* #if ( CPUTYPE == ia64 ) */
#error Unknown CPUTYPE for ARCH x86
#endif /* #if ( CPUTYPE == ia64 ) */
#else /* #ifdef CPUTYPE */
#error CPUTPYE is not defined
#endif /* #idef CPUTYPE */


/** \brief Osek Stack
 **
 ** This variable is used to save the Osek stack while calling a Os
 ** service
 **/
#if ( CPUTYPE == ia64 )
extern uint64 OsekStack;
#elif ( CPUTYPE == ia32 )
extern uint32 OsekStack;
#else /* #if ( CPUTYPE == ia64 ) */
#error Unknown CPUTYPE for ARCH x86
#endif /* #if ( CPUTYPE == ia64 ) */

/*==================[external functions declaration]=========================*/
/** \brief Os Interrupt Handler
 **
 ** This function is called every time when a interrupt message is received.
 **/
extern void OsInterruptHandler(int status);

extern void* HWTimerThread(void *pThread_Arg);

extern void OsekKillSigHandler(int status);

extern void OSEK_ISR_HWTimer0(void);

extern void OSEK_ISR_HWTimer1(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_H_ */


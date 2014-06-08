/*****************************************************************************
 * add your copyright notice
 * Copyright <year>, <your name>
 *
 * Please do not change the license on a single file, if you want to change it
 * discuss it with the development team.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

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

/*****************************************************************************
 * update the tmparch directory on the \file and \arch doxygen comment with your
 * architecture.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief FreeOSEK Internal Architecture Dependent Header File
 **
 ** \file tmparch/Os_Internal_Arch.h
 ** \arch tmparch
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
 * MaCe			 Mariano Cerdeiro
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
 * 20090526 v0.1.0 MaCe FreeOSEK architecture template file
 */

/*==================[inclusions]=============================================*/



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
#define JmpTask(task)														\
{																			\
	Osek_OldTaskPtr_Arch = (void*)0;										\
	Osek_NewTaskPtr_Arch = (void*)TasksConst[(task)].TaskContext;			\
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

/** \brief Save context */
#define SaveContext(task) 													\
{																			\
	flag = 0;																\
	/* remove of the Ready List */											\
	RemoveTask(GetRunningTask());											\
	/* finish cirtical code */												\
	IntSecure_End();														\
	/* call scheduler */													\
	Schedule();																\
}

/** \brief */
#define ResetStack(task)		\
{								\
	TerminatingTask = (task);	\
}

/** \brief Set the entry point for a task */
#define SetEntryPoint(task)		\
{								\
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

/** \brief Get Counter Actual Value
 **
 ** This macro returns the actual value of the a counter
 **
 ** \param[in] CounterID id of the counter to be readed
 ** \return Actual value of the counter
 **/
#define GetCounter_Arch(CounterID) (CountersVar[CounterID].Time)

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


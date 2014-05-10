#error this is a remember to remove the comment on the following line
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

#error this is a remember to remove the comment on the following line
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

#error this is a remember to remove the comment on the following line
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

#error this is a remember to remove the comment on the following line
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
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * add any needed include, please take into account that normaly NO INCLUDE
 * shall be included here, but in case you can do it, but discuss the reason
 * with the project manager.
 * Normaly nothing shall be included here
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[macros]=================================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please define here all needed macros that will be visible to the OS user
 * for this architecutre. This means that anyone including os.h will have
 * access to this definitions if the actual architecutre is used.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

#error update the following macro and remove this comment
/** \brief Osek_Internal_Arch_Cpu.h inclusion needed macro
 **
 ** This define makes the Osek_Internal.h file to include the
 ** Osek_Internal_Arch_Cpu file which is not standard for all architectures.
 ** If for the actual architecture no Osek_Internal_Arch_Cpu.h is neede
 ** remove the macro and this comment.
 **/
#define OSEK_INLCUDE_INTERNAL_ARCH_CPU

#error update the following macro and remove this comment
/** \brief Interrupt Secure Start Macro
 **
 ** This macro will be used internaly by the OS in any part of code that
 ** has to be executed atomic.
 **/
#define IntSecure_Start() SuspendAllInterrupts()

#error update the following macro and remove this comment
/** \brief Interrupt Secure End Macro
 **
 ** This macro is the countra part of IntSecure_Start()
 **/
#define IntSecure_End() ResumeAllInterrupts()

#error update the following macro and remove this comment
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
#define osekpause()			\
	{								\
	}

#error update the following macro and remove this comment
/** \brief Call to an other Task
 **
 ** This function jmps to the indicated nexttask and saves the running context
 ** on the actualtask context.
 **
 ** The Tasks context can be found under the following variables:
 **	- TasksConst[actualtask].TaskContext
 ** 	- TasksConst[nexttask].TaskContext
 **
 ** \param[in] actualtask actual running task id
 ** \param[in] nexttask next task task id
 **/
#define CallTask(actualtask, nexttask)														\
	{																									\
	}

#error update the following macro and remove this comment
/** \brief Jmp to an other Task
 **
 ** This function jmps to the indicated task. Same as call task but is not
 ** necessary to save the actual task state since the task was finished.
 **
 ** The Tasks context can be found under the following variable:
 **	- TasksConst[task].TaskContext
 **
 ** \param[in] task task id of the task to be executed
 **/
#define JmpTask(task)															\
	{																					\
	}

#error update the following macro and remove this comment
/** \brief Save context
 **
 ** This function has to save the context when calling it. This is used
 ** for example when a function calls waitevent. The task will be resumed
 ** with a JmpTask or CallTask macro.
 **
 ** The task context can be found under the following variable:
 **	- TasksConst[task].TaskContext
 **
 ** \param[in] task task id to save the task context
 **/
#define SaveContext(task) 														\
	{																					\
	}

#error update the following macro and remove this comment
/** \brief Reset the entry point for a task
 **
 ** This macro reset the entry point of a specific task. The entry
 ** point of a task is updated every time when a task is completed.
 **
 ** The task context can be found under the following variable:
 **	- TasksConst[task].TaskContext
 **
 ** The start configured task entry point can be found on the following
 ** variable:
 **	- TasksConst[task].EntryPoint
 **
 ** \param[in] task task id to reset the task entry point
 **/
#define SetEntryPoint(task)							\
	{															\
	}

#error update the following macro and remove this comment
/** \brief Reset stack task
 **
 ** This macro reset the stack of a specific task. The stack
 ** point of a task is updated every time when a task is completed.
 **
 ** The task context can be found under the following variable:
 **	- TasksConst[task].TaskContext
 **
 ** The start configured stack can be found on the following
 ** variables:
 **	- TasksConst[task].StackPtr
 **	- TasksConst[task].StackSize
 **
 ** \param[in] task task id to reset the stack
 **/
#define ResetStack(task)											\
	{																		\
   }

#error update the following macro and remove this comment
/** \brief Enable OS Interruptions
 **
 ** Enable OS configured interrupts (ISR1 and ISR2). This macro
 ** is called only ones in StartUp.c function.
 **/
#define EnableOSInterrupts()															\
	{																							\
	}

#error update the following macro and remove this comment
/** \brief Enable Interruptions
 **
 ** Enable not OS configured interrupts (ISR1 and ISR2). This macro
 ** is called only ones in StartUp.c function.
 **
 ** This macro may be empty. Maybe will be removed on the future,
 ** please use it only if necessary, in other case use EnableOSInterrupts.
 **/
#define EnableInterrupts()		\
	{									\
	}

#error update the following macro and remove this comment
/** \brief Get Counter Actual Value
 **
 ** This macro returns the actual value of the a counter
 **
 ** \param[in] CounterID id of the counter to be readed
 ** \return Actual value of the counter
 **/
#define GetCounter_Arch(CounterID) (CountersVar[CounterID].Time)

#error update the following macro and remove this comment
/** \brief ShutdownOs Arch service
 **
 ** This macro is called on the ShutdownOS to perform the architecture
 ** dependent shutdown actions.
 **/
#define ShutdownOs_Arch()

/*==================[typedef]================================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please define here all needed types that will be used only internal by
 * the OS and only for this architecture and which will not depend on the
 * configuraiton. Normaly this section shall be empty.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external data declaration]==============================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please declare here all exported data defined in Osek_Internal_Arch.c
 * that will be visible only internal to the OS for this architectire.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions declaration]=========================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please declare here all exported functions defined in Osek_Internal_Arch.c
 * that will be visible only internaly to the OS user for this architectire.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_H_ */


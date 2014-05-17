

#ifndef _OS_ARCH_H_
#define _OS_ARCH_H_
/** \brief FreeOSEK Os Architecture Dependent Header File
 **
 ** This file is included form os.h and defines macros
 ** and types which depends on the architecture.
 **
 ** \file posix/Os_Arch.h
 ** \arch posix
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.1 MaCe rename file to Os_
 * 20080725 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief Enable All Interrupts Arch */
#define EnableAllInterrupts_Arch()	ResumeAllInterrupts_Arch()

/** \brief Disable All Interrupts Arch */
#define DisableAllInterrupts_Arch() SuspendAllInterrupts_Arch()

/** \brief Resume All Interrupts Arch */
#define ResumeAllInterrupts_Arch()						\
	{																\
		InterruptState = ((InterruptStateType)1U);	\
		ScheduleInterrupts();								\
	}

/** \brief Suspend All Interrupts Arch */
#define SuspendAllInterrupts_Arch()						\
	{																\
		InterruptState = ((InterruptStateType)0U);	\
	}

/** \brief Resume OS Interrupts Arch */
#define ResumeOSInterrupts_Arch()													\
	{																							\
		InterruptMask &= (InterruptFlagsType)~(OSEK_OS_INTERRUPT_MASK);	\
	}


/** \brief Suspend OS Interrupts Arch */
#define SuspendOSInterrupts_Arch()					\
	{															\
		InterruptMask |= OSEK_OS_INTERRUPT_MASK;	\
	}

/*==================[typedef]================================================*/
/** \brief Interrupt type definition */
typedef unsigned int InterruptFlagsType;

/** \brief Interrupt state type definition */
typedef unsigned char InterruptStateType;

/*==================[external data declaration]==============================*/
/** \brief Interrupt Mask
 **
 ** This variable mask the interrupts. Interrupts which are masked are
 ** not going to be executed until the mask is cleared.
 **/
extern InterruptFlagsType InterruptMask;

/** \brief Interrupt State
 **
 ** If this variable is set the intterupts are enable, if it is 0
 ** interrupts are disable.
 **/
extern InterruptStateType InterruptState;

/*==================[external functions declaration]=========================*/
extern void ScheduleInterrupts(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_ARCH_H_ */


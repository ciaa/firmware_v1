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

#ifndef _OS_H_
#define _OS_H_
/** \brief FreeOSEK Os Header File
 **
 ** This file shall be included by all files using any FreeOSEK Os API
 ** or implementing any FreeOSEK Os ISR or TASK.
 **
 ** \file os.h
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os Operating System
 ** @{ */
/** \addtogroup FreeOSEK_Os_Global Global
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 * KLi          Kang Li
 * AsKa         Asnoka
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.7 MaCe rename file to Os_
 * 20090420 v0.1.6 MaCe correct AlarmBaseType members to lower cases
 * 20090418 v0.1.5 MaCe bugfix correct StartOS and ShutdownOS services names
 * 20090329 v0.1.4 MaCe add RES_SCHEDULER
 * 20090129 v0.1.3 MaCe add OSEK_MEMMAP check
 * 20081214 v0.1.2 AsKa Fix mistaken comments for SetRelAlarm
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Types.h"
#include "Os_Cfg.h"
#include "Os_Arch.h"

/*==================[macros]=================================================*/
/** \brief TASK macro definition
 **
 ** This macro shall be used to start the definition of all tasks.
 **
 ** \param[in] name name of the task to be implemented.
 **/
#define TASK(name)   void OSEK_TASK_ ## name (void)

/** \brief ISR macro definition
 **
 ** This macro shall be used to start the definition of all ISR category 1 or 2.
 **
 ** \param[in] name name of the ISR category 1 or 2.
 **/
#define ISR(name)   void OSEK_ISR_ ## name (void)

/** \brief ALARMCALLBACK macro definition
 **/
#define ALARMCALLBACK(name)	void OSEK_CALLBACK_ ## name (void)

/** \brief SUSPEND Task State */
#define SUSPENDED 0U
/** \brief READY Task State */
#define READY 1U
/** \brief RUNNING Task State */
#define RUNNING 2U
/** \brief WAITING Task State */
#define WAITING 3U
/** \brief INVALID Task State */
#define INVALID_STATE 4U

/** \brief Definition return value E_OK */
/* \req OSEK_SYS_1.1.1 */
#define E_OK					((StatusType)0U)
/** \brief Definition return value E_OS_ACCESS */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_ACCESS			((StatusType)1U)
/** \brief Definition return value E_OS_CALLEVEL */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_CALLEVEL		((StatusType)2U)
/** \brief Definition return value E_OS_ID */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_ID				((StatusType)3U)
/** \brief Definition return value E_OS_LIMIT */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_LIMIT			((StatusType)4U)
/** \brief Definition return value E_OS_NOFUNC */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_NOFUNC			((StatusType)5U)
/** \brief Definition return value E_OS_RESOURCE */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_RESOURCE		((StatusType)6U)
/** \brief Definition return value E_OS_STATE */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_STATE			((StatusType)7U)
/** \brief Definition return value E_OS_VALUE */
/* \req OSEK_SYS_1.1.1 */
#define E_OS_VALUE			((StatusType)8U)

/** \brief Enable All Interrupts
 **
 ** This system service enables all interruptions disabled by
 ** the DisableAllInterrups service.
 **
 ** If the service DisablleAllInterrupt is called n times
 ** time, only the n call to EnablleAllInterrupts will enable
 ** all disabled interrupts.
 **
 ** \return none
 **/
/* \req OSEK_SYS_3.7 The system service void EnableAllInterrupts ( void )
 *  shall be defined
 * \req OSEK_SYS_3.7.1 This service restores the state saved by
 * DisableAllInterrupts
 */
#define EnableAllInterrupts()						\
	{														\
		DisableAllInterrupts_Counter--;			\
		if(DisableAllInterrupts_Counter <=		\
			((InterruptCounterType)0U))			\
		{													\
			DisableAllInterrupts_Counter =		\
				((InterruptCounterType)0U);		\
			EnableAllInterrupts_Arch();			\
		}													\
	}

/** \brief Disable All Interrupts
 **
 ** This system service disables all interrupts.
 **
 ** If this service is called n times, EnableAllInterrupts has to be
 ** called n times to enable the interrupts back.
 **
 ** \return none
 **/
/* \req OSEK_SYS_3.8 The system service void DisableAllInterrupts ( void )
 *  shall be defined
 * \req OSEK_SYS_3.8.1 This service disables all interrupts for which the
 *  hardware supports disabling
 * \req OSEK_SYS_3.8.2 The state before is saved for the EnableAllInterrupts
 *  call
 */
#define DisableAllInterrupts()			\
	{												\
		DisableAllInterrupts_Counter++;	\
		DisableAllInterrupts_Arch();		\
	}

/** \brief Resume All Interrupts
 **
 ** This API resume all Interruptions suspended by the last call to
 ** SuspendAllInterrupts.
 **/
/* \req OSEK_SYS_3.9 The system service void ResumeAllInterrupts ( void )
 *  shall be defined
 * \req OSEK_SYS_3.9.1 This service restores the recognition status of all
 *  interrupts saved by the SuspendAllInterrupts service
 * \req OSEK_SYS_3.9.2 SuspendAllInterrupts/ResumeAllInterrupts can be nested.
 *  In case of nesting pairs of the calls SuspendAllInterrupts and
 *  ResumeAllInterrupts the interrupt recognition status saved by the first
 *  call of SuspendAllInterrupts is restored by the last call of the
 *  ResumeAllInterrupts service.
 **/
#define ResumeAllInterrupts() 					\
	{														\
		SuspendAllInterrupts_Counter--;			\
		if(SuspendAllInterrupts_Counter <=		\
			((InterruptCounterType)0U))			\
		{													\
			SuspendAllInterrupts_Counter =		\
				((InterruptCounterType)0U);		\
			ResumeAllInterrupts_Arch();			\
		}													\
	}

/** \brief Suspend All Interrupts
 **
 ** This API suspend all Interruptions and saves the interruptions state for
 ** the EnableAllInterrupts API.
 **/
/* \req OSEK_SYS_3.10 The system service void SuspendAllInterrupts ( void )
 *  shall be defined
 * \req OSEK_SYS_3.10.1 This service shall save the recognition status of all
 *  interrupts
 * \req OSEK_SYS_3.10.2 and disables all interrupts for which the hardware
 *  supports disabling
 */
#define SuspendAllInterrupts()					\
	{														\
		SuspendAllInterrupts_Counter++;			\
		SuspendAllInterrupts_Arch();				\
	}

/** \brief Resume OS Interrupts
 **
 ** This service restores the recognition status of interrupts saved by the
 ** SuspendOSInterrupts service.
 **/
/* \req OSEK_SYS_3.11 The system service void ResumeOSInterrupts ( void )
 *  shall be defined
 * \req OSEK_SYS_3.11.1 This service restores the recognition status of
 *  interrupts saved by the SuspendOSInterrupts service
 * \req OSEK_SYS_3.11.2 SuspendOSInterrupts/ResumeOSInterrupts can be nested.
 *  In case of nesting pairs of the calls SuspendOSInterrupts and
 *  ResumeOSInterrupts the interrupt recognition status saved by the first call
 *  of SuspendOSInterrupts is restored by the last call of the
 *  ResumeOSInterrupts service
 */
#define ResumeOSInterrupts()						\
	{														\
		SuspendOSInterrupts_Counter--;			\
		if(SuspendOSInterrupts_Counter <= 		\
			((InterruptCounterType)0U))			\
		{													\
			SuspendOSInterrupts_Counter =			\
				((InterruptCounterType)0U);		\
			ResumeOSInterrupts_Arch();				\
		}													\
	}

/** \brief Suspend OS Interrupts
 **
 ** SuspendOSInterrupts/ResumeOSInterrupts can be nested. In case of nesting pairs of
 ** the calls SuspendOSInterrupts and ResumeOSInterrupts the interrupt recognition
 ** status saved by the first call of SuspendOSInterrupts is restored by the last
 ** call of the ResumeOSInterrupts service.
 **/
/* \req OSEK_SYS_3.12 The system service void SuspendOSInterrupts ( void )
 *  shall be defined
 * \req OSEK_SYS_3.12.1 This service shall save the recognition status of
 *  interrupts of category 2
 * \req OSEK_SYS_3.12.2 and disables the recognition of these interrupts
 */
#define SuspendOSInterrupts()						\
	{														\
		SuspendOSInterrupts_Counter++;			\
		SuspendOSInterrupts_Arch();				\
	}

#define OSServiceId_ActivateTask						1
#define OSServiceId_TerminateTask					2
#define OSServiceId_ChainTask							3
#define OSServiceId_Schedule							4
#define OSServiceId_GetTaskID							5
#define OSServiceId_GetTaskState						6
#define OSServiceId_DisableAllInterrupts			7
#define OSServiceId_EnableAllInterrupts			8
#define OSServiceId_SuspendAllInterrupts			9
#define OSServiceId_ResumeAllInterrupts			10
#define OSServiceId_SuspendOSInterrupts			11
#define OSServiceId_ResumeOSInterrupts				12
#define OSServiceId_GetResource						13
#define OSServiceId_ReleaseResource					14
#define OSServiceId_SetEvent							15
#define OSServiceId_ClearEvent						16
#define OSServiceId_GetEvent							17
#define OSServiceId_WaitEvent							18
#define OSServiceId_GetAlarmBase						19
#define OSServiceId_GetAlarm							20
#define OSServiceId_SetRelAlarm						21
#define OSServiceId_SetAbsAlarm						22
#define OSServiceId_CancelAlarm						23
#define OSServiceId_GetActiveApplicationMode		24
#define OSServiceId_StartOS							25
#define OSServiceId_ShutdownOS						26

/** \brief Resource Scheduler */
#define RES_SCHEDULER									255

/*==================[typedef]================================================*/
/** \brief Type definition of StatusType
 **
 ** This type is used to represent the status returned by all FreeOSEK APIs
 **/
/* \req OSEK_SYS_1.1 */
typedef unsigned char StatusType;

/** \brief Type definition of TaskType
 **
 ** This type is used to represent the Task IDs
 **/
typedef unsigned char TaskType;

/** \brief Type definition of TaskRefType
 **
 ** This type is used to represent a pointer to a Task ID Type
 **/
typedef TaskType* TaskRefType;

/** \brief Type definition of TaskStateType
 **
 ** This type is used to represent the State of a Task
 **/
typedef unsigned char TaskStateType;

/** \brief Type definition of TaskStateRefType
 **
 ** This type is used to represent a pointer to a TaskStateType
 **/
typedef TaskStateType* TaskStateRefType;

/** \brief Type definition of AppModeType
 **
 ** This type is used to represent an Application Mode
 **/
typedef unsigned char AppModeType;

/** \brief Type definition of ResourceType
 **
 ** This type is used to represent a Resorce
 **/
typedef unsigned char ResourceType;

/** \brief Type definition of Event Mask
 **
 ** This type is used to represent Events
 **/
typedef unsigned int EventMaskType;

/** \brief Type definition of EventMaskRefType
 **
 ** This type is used to represent references to Events
 **/
typedef EventMaskType* EventMaskRefType;

/** \brief Type definition of AlarmType
 **
 ** This type is used to represent references to Alarms
 **/
typedef unsigned char AlarmType;

/** \brief Type definition of TickType
 **
 ** This type is used to represent references to Ticks
 **/
typedef unsigned int TickType;

/** \brief Type definition of TickRefType
 **
 ** This type is used to represent references to TickType
 **/
typedef TickType* TickRefType;


/** \brief Type definition of AlarmBaseType
 **
 ** This type is used to represent references to AlarmBase
 **/
typedef struct {
	TickType maxallowedvalue;
	TickType ticksperbase;
	TickType mincycle;
} AlarmBaseType;

/** \brief Type definition of AlarmBaseRefType
 **
 ** This type is used to represent references to AlarmBase
 **/
typedef AlarmBaseType* AlarmBaseRefType;

/** \brief Interrupt Counter type definition */
typedef signed char InterruptCounterType;

/*==================[external data declaration]==============================*/
/** \brief Suspend OS interrupts counter */
extern InterruptCounterType SuspendOSInterrupts_Counter;

/** \brief Disable All interrupts counter */
extern InterruptCounterType DisableAllInterrupts_Counter;

/** \brief Suspend All interrupts counter */
extern InterruptCounterType SuspendAllInterrupts_Counter;

/*==================[external functions declaration]=========================*/
/** \brief Activate the specified Task
 **
 ** This system service activates the task with id indicated in the TaskID
 ** parameter.
 **
 ** This service can be used to activate any FreeOSEK Tasks. By the activation
 ** the task will be transfered from the suspended to the ready state. Basic
 ** tasks may be activated more tha one time, if configured. A second or n
 ** activation puts the task for a second or n time on the ready list. A
 ** maximal count of activation shall be defined on the OIL configuration for
 ** every basic tasks. Extended task can only be activated ones.
 **
 ** FreeOSEK generates macros with the names of the tasks as defined on the on
 ** the OIL configuration.
 **
 ** Rescheduling takes place before returning. Activating a higher priority
 ** task preemts the running task (only if preemtable) and start the
 ** activated task.
 **
 ** This function may return E_OK or E_OS_LIMIT in standard mode.
 ** Extra in extended mode E_OS_ID may be returned.
 **
 ** If the return code is other than E_OK, the ErrorHook is enable, and
 ** ErrorHook is not beeing executed the ErrorHook function will be called.
 **
 ** \param[in] TaskID TaskID of the task to be activated.
 ** \return E_OK if no error occurrs
 ** \return E_OS_LIMIT if to many task activations of TaskID
 ** \return E_OS_ID if the TaskID is invalid, only in extended mode.
 **/
extern StatusType ActivateTask(TaskType TaskID);

/** \brief Terminate the calling Task
 **
 ** This API shall finish the execution of the calling task. This
 ** API never returns in standard mode. In extended mode
 ** the following errors can be returned.
 **
 ** \return E_OS_RESOURCE if the Task still occupies resources
 ** \return E_OS_CALLEVEL if called at interrupt level
 **/
extern StatusType TerminateTask(void);

/** \brief Terminate the calling Task and Activate the specified Task
 **
 ** This system service finishs the execution of the calling task and
 ** activates the indicated task. If no error occurrs this service never
 ** returns.
 **
 ** This service can be used to continue executing a task terminating the
 ** calling task.
 **
 ** ChainTask service may be used to referencint the actual task. This
 ** will case the actual task to be restarted.
 **
 ** FreeOSEK generates macros with the names of the tasks as defined on the on
 ** the OIL configuration.
 **
 ** Rescheduling takes place before returning.
 **
 ** This function may return E_OS_LIMIT in standard mode.
 ** Extra in extended mode E_OS_ID, E_OS_CALLEVEL, or E_OS_RESOURCE may be
 ** returned.
 **
 ** If any error occurs, and the ErrorHook is enable, and ErrorHook is not
 ** beeing executed, the ErrorHook function will be called.
 **
 ** If no error occurrs the function doesn't return.
 **
 ** \param[in] TaskID task to be activatied after finishing the actial task.
 ** \return E_OS_LIMIT if too many task activations of TaskID
 ** \return E_OS_ID if the TaskID is invalid
 ** \return E_OS_RESOURCE if the calling task still occupies resources
 ** \return E_OS_CALLEVEL if call at interrupt level
 **/
extern StatusType ChainTask(TaskType TaskID);

/** \brief Schedule this Task if higher priority Task are Active
 **
 ** This API shall Schedule the calling Task if a higher priority Task
 ** is active. This API shall only be used from non preemtive tasks.
 **
 ** \return E_OK if no error
 ** \return E_OS_CALLEVEL if call at interrupt level
 ** \return E_OS_RESOURCE if the calling task occupies resources
 **/
extern StatusType Schedule(void);

/** \brief Get Task ID of the calling Task
 **
 ** This API returns the TaskID of the calling Task
 **
 ** \param[out] TaskID TaskID of the actual task
 ** \return E_OK always
 **/
extern StatusType GetTaskID(TaskRefType TaskID);

/** \brief Get the Task State of the specified Task
 **
 ** This API returns the Task State of the indicated Task
 **
 ** \param[in] TaskID TaskID of the Task which State shall be returned
 ** \param[out] State State of the Task corresponding with the specified TaskID
 ** \return E_OK if no errors
 ** \return E_OS_ID if the Task TaskID is invalid
 **/
extern StatusType GetTaskState(TaskType TaskID, TaskStateRefType State);

/** \brief Get a Resource
 **
 ** This call serves to enter critical sections in the code that are assigned
 ** to the resource referenced by ResID.
 **
 ** \param[in] ResID Resorce ID to be reserved.
 ** \return E_OK if no error
 ** \return E_OS_ID if the Resource ResID is invalid
 ** \return E_OS_ACCESS if attempt to get a resource which is already occupied
 **         by any task or ISR
 **
 **/
extern StatusType GetResource(ResourceType ResID);

/** \brief Release a Resource
 **
 ** ReleaseResource is the counterpart of GetResource and serves to leave critical
 ** sections in the code that are assigned to the resource referenced by ResID.
 **
 ** \param[in] ResID Resorce ID to be freed
 ** \return E_OK if no error
 ** \return E_OS_ID if the Resource ResID is invalid
 ** \return E_OS_NOFUNC if attempt to release a resource which is not occupied by
 **         any task or ISR, or another resource shall be released before
 ** \return E_OS_ACCESS if attempt to release a resource which has a lower ceiling
 **         priority than the statically assigned priority of interrupt routine.
 **
 **/
extern StatusType ReleaseResource(ResourceType ResID);

/** \brief Start the FreeOSEK Operating System
 **
 ** This Interface shall be used to start the Operating System. It may be
 ** called directly from the main function after performing any neccesary
 ** hardware initializations.
 **
 ** This function shall never return.
 **
 ** \param[in] Mode Application Mode
 ** \return never
 **/
extern void StartOS(AppModeType Mode);

/** \brief Set Event
 **
 ** This Interface can be used to set one or more events of the specified
 ** Task.
 **
 ** \param[in] TaskID TaskID of the task to set the Events
 ** \param[in] Mask Events to be set on the specified task
 **/
extern StatusType SetEvent(TaskType TaskID, EventMaskType Mask);

/** \brief Clear Event
 **
 ** This system service clears one or more events of the calling task.
 **
 ** This service can be used to remove events set to the calling task by any
 ** other task.
 **
 ** FreeOSEK generates macros with the names of the events as defined on the OIL
 ** configuration.
 **
 ** This function returns E_OK in standard mode. Extra in extended mode
 ** E_OS_ACCESS and E_OS_CALLEVEL may be returned.
 **
 ** If any error occurs, and the ErrorHook is enable, and ErrorHook is not
 ** beeing executed, the ErrorHook function will be called.
 **
 ** \param[in] Mask Events to be cleared.
 ** \return E_OK if no error occurs
 ** \return E_OS_ACCESS if called from a basic task
 ** \return E_OS_CALLEVEL if called from a context other than a task
 **/
extern StatusType ClearEvent(EventMaskType Mask);

/** \brief Get Event
 **
 ** This Interface can be used to get the events states of a specified task
 **
 ** \param[in] TaskID TaskID of the task to get the Events
 ** \param[in] Event reference to the Event status.
 **/
extern StatusType GetEvent(TaskType TaskID, EventMaskRefType Event);

/** \brief Wait Event
 **
 ** This put the task in wait state until one or more of the indicated
 ** events occurs.
 **
 ** \param[in] Mask events to wait for
 **/
extern StatusType WaitEvent(EventMaskType Mask);

/** \brief ShutdownOS
 **
 ** This api stops the os.
 **
 ** \param[in] Error shutdown error
 **/
extern void ShutdownOS(StatusType Error);

/** \brief Get Active Application Mode
 **
 ** This API returns the Application Mode
 **/
extern AppModeType GetActiveApplicationMode(void);

/** \brief Get Alarm Base
 **
 ** The system service GetAlarmBase reads the alarm base characteristics.
 ** The return value Info is a structure in which the information of data type
 ** AlarmBaseType is stored.
 **
 ** \param[in] AlarmID Alamar ID to be readed
 ** \param[out] Info Reference to an Alarm Base Type
 **/
extern StatusType GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);

/** \brief Get Alarm
 **
 ** The system service GetAlarm shall return the relative value in ticks before the
 ** alarm AlarmID expires.
 **
 ** \param[in] AlarmID Alamar ID to be readed
 ** \param[out] Tick count of tick before alarm expires.
 **/
extern StatusType GetAlarm(AlarmType AlarmID, TickRefType Tick);

/** \brief Set Relative Alarm
 **
 ** The system service occupies the alarm AlarmID element.
 ** After increment ticks have elapsed, the task assigned to the
 ** alarm AlarmID is activated or the assigned event (only for
 ** extended tasks) is set or the alarm-callback routine is called.
 **
 ** \param[in] AlarmID Alamar ID to be readed
 ** \param[in] Increment
 ** \param[in] Cycle
 **/
extern StatusType SetRelAlarm(AlarmType AlarmID, TickType Increment, TickType Cycle);

/** \brief Set Absolute Alarm
 **
 ** The system service occupies the alarm AlarmID element. When Start ticks
 ** are reached, the task assigned to the alarm AlarmID is activated or the
 ** assigned event (only for extended tasks) is set or the alarm-callback
 ** routine is called.
 **
 ** \param[in] AlarmID Alamar ID to be readed
 ** \param[in] Start
 ** \param[in] Cycle
 **/
extern StatusType SetAbsAlarm(AlarmType AlarmID, TickType Start, TickType Cycle);

/** \brief Cancel Alarm
 **
 ** The system service cancels the alarm indicated in the parameter AlarmID.
 **
 ** This service can be used to cancel a running alarm activated before
 ** with the service SetRelAlarm or SetAbsAlarm or started automaticaly
 ** for the actual application mode.
 **
 ** FreeOSEK generates macros with the names of the alarms as defined on the on
 ** the OIL configuration.
 **
 ** This service may return E_OK, or E_OS_NOFUNC in standard mode. Extra
 ** in extended mode E_OS_ID may be returned.
 **
 ** If the return code is other than E_OK, the ErrorHook is enable, and
 ** ErrorHook is not beeing executed the ErrorHook function will be called.
 **
 ** \param[in] AlarmID Alamar ID to be readed
 ** \return E_OK if no error occurs
 ** \return E_OS_ID if an invalid AlamrID is provided
 ** \return E_NO_FUNC if the alarm is not running
 **/
extern StatusType CancelAlarm(AlarmType AlarmID);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_H_ */


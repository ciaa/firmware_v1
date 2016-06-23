/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2008, 2009 Mariano Cerdeiro
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

#ifndef _OS_INTERNAL_CFG_H_
#define _OS_INTERNAL_CFG_H_
/** \brief FreeOSEK Os Generated Internal Configuration Header File
 **
 ** This file content the internal generated configuration of FreeOSEK Os
 **
 ** \file Os_Internal_Cfg.h
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */
/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief ERROR_CHECKING_STANDARD */
#define ERROR_CHECKING_STANDARD   1

/** \brief ERROR_CHECKING_EXTENDED */
#define ERROR_CHECKING_EXTENDED   2

/** \brief Count of task */
#define TASKS_COUNT 2U

/** \brief Remote tasks count */
#define REMOTE_TASKS_COUNT 0U

/** \brief Count of resources */
#define RESOURCES_COUNT 1

/** \brief Error Checking Type */
#define ERROR_CHECKING_TYPE ERROR_CHECKING_EXTENDED
/** \brief pre task hook enable-disable macro */
#define HOOK_PRETASKHOOK OSEK_DISABLE
/** \brief post task hook enable-disable macro */
#define HOOK_POSTTASKHOOK OSEK_DISABLE
/** \brief error hook enable-disable macro */
#define HOOK_ERRORHOOK OSEK_ENABLE
/** \brief startup hook enable-disable macro */
#define HOOK_STARTUPHOOK OSEK_DISABLE
/** \brief shutdown hook enable-disable macro */
#define HOOK_SHUTDOWNHOOK OSEK_DISABLE

#define READYLISTS_COUNT 2
#define SetError_Api(api)   ( Osek_ErrorApi = (api) )
#define SetError_Param1(param1) ( Osek_ErrorParam1 = (param1) )
#define SetError_Param2(param2) ( Osek_ErrorParam2 = (param2) )
#define SetError_Param3(param3) ( Osek_ErrorParam3 = (param3) )
#define SetError_Ret(ret) ( Osek_ErrorRet = (uint32)(ret) )
#define SetError_Msg(msg)
/* { printf ("Error found in file: \"%s\" line \"%d\" ", __FILE__, __LINE__); printf(msg); } */
#define SetError_ErrorHook()          \
   {                                  \
      ErrorHookRunning = (uint8)1U;   \
      ErrorHook();                    \
      ErrorHookRunning = (uint8)0U;   \
   }

#define ALARM_AUTOSTART_COUNT 0

#define OSEK_COUNTER_HardwareCounter 0
/** \brief ALARMS_COUNT define */
#define ALARMS_COUNT 1

/** \brief NON_PREEMPTIVE macro definition */
#define NON_PREEMPTIVE OSEK_ENABLE

/** \brief NO_EVENTS macro definition */
#define NO_EVENTS OSEK_DISABLE

/** \brief NO_RES_SCHEDULER macro definition */
#define NO_RES_SCHEDULER OSEK_ENABLE


/*==================[typedef]================================================*/
typedef unsigned char TaskPriorityType;

typedef struct {
   unsigned int Extended : 1;
   unsigned int Preemtive : 1;
   unsigned int State : 2;
} TaskFlagsType;

typedef uint8 TaskActivationsType;

typedef uint32 TaskEventsType;

typedef uint32 TaskResourcesType;

typedef uint8* StackPtrType;

typedef uint16 StackSizeType;

typedef void (* EntryPointType)(void);

typedef void (* CallbackType)(void);

typedef uint8 TaskTotalType;

typedef uint8 TaskCoreType;

/** \brief Task Constant type definition
 **
 ** This structure defines all constants and constant pointers
 ** needed to manage a task
 **
 ** \param EntryPoint pointer to the entry point for this task
 ** \param Priority static priority of this task
 ** \param MaxActivations maximal activations for this task
 **/
typedef struct {
   EntryPointType EntryPoint;
   TaskContextRefType TaskContext;
   StackPtrType StackPtr;
   StackSizeType StackSize;
   TaskPriorityType StaticPriority;
   TaskActivationsType MaxActivations;
   TaskFlagsType ConstFlags;
   TaskEventsType EventsMask;
   TaskResourcesType ResourcesMask;
   TaskCoreType TaskCore;
} TaskConstType;

/** \brief Task Variable type definition
 **
 ** This structure defines all variables needed to manage a task
 **
 ** \param ActualPriority actual priority of this task
 ** \param Activations actual activations on this task
 ** \param Flags flags variable of this task
 ** \param Events of this task
 ** \param Resource of this task
 **/
typedef struct {
   TaskPriorityType ActualPriority;
   TaskActivationsType Activations;
   TaskFlagsType Flags;
   TaskEventsType Events;
   TaskEventsType EventsWait;
   TaskResourcesType Resources;
} TaskVariableType;

/** \brief Auto Start Structure Type
 **
 ** \param Total taks on this application mode
 ** \param Reference to the tasks on this Application Mode
 **/
typedef struct {
   TaskTotalType TotalTasks;
   TaskRefType TasksRef;
} AutoStartType;

/** \brief Ready List Constatn Type
 **
 ** \param ListLength Lenght of the Ready List
 ** \param TaskRef Reference to the Ready Array for this Priority
 **/
typedef struct {
   TaskTotalType ListLength;
   TaskRefType TaskRef;
} ReadyConstType;

/** \brief Ready List Variable Type
 **
 ** \param ListStart first valid componet on the list
 ** \param ListCount count of valid components on this list
 **/
typedef struct {
   TaskTotalType ListStart;
   TaskTotalType ListCount;
} ReadyVarType;

/** \brief Alarm State
 **
 ** This type defines the possibly states of one alarm which are:
 ** 0 disable
 ** 1 enable
 **/
typedef uint8 AlarmStateType;

/** \brief Alarm Time */
typedef uint32 AlarmTimeType;

/** \brief Alarm Cycle Time */
typedef uint32 AlarmCycleTimeType;

/** \brief Counter Type */
typedef uint8 CounterType;

/** \brief Counter Increment Type */
typedef uint32f CounterIncrementType;

/** \brief Alarm Increment Type */
typedef uint32f AlarmIncrementType;

/** \brief Alarm Action Type */
typedef enum {
   ALARMCALLBACK = 0,
   SETEVENT = 1,
   ACTIVATETASK = 2,
   INCREMENT = 3
} AlarmActionType;

/** \brief Alarm Action Info Type
 **
 ** This type has extra information of the Alarm action
 **/
typedef struct {
   CallbackType CallbackFunction;
   TaskType TaskID;
   EventMaskType Event;
   CounterType Counter;
} AlarmActionInfoType;

/** \brief Alarm Variable Type */
typedef struct {
   AlarmStateType AlarmState;
   AlarmTimeType AlarmTime;
   AlarmCycleTimeType AlarmCycleTime;
} AlarmVarType;

/** \brief Alarm Constant Type */
typedef struct {
   CounterType Counter;
   AlarmActionType AlarmAction;
   AlarmActionInfoType AlarmActionInfo;
} AlarmConstType;

/** \brief Auto Start Alarm Type */
typedef struct {
   AppModeType Mode;
   AlarmType Alarm;
   AlarmTimeType AlarmTime;
   AlarmCycleTimeType AlarmCycleTime;
} AutoStartAlarmType;

typedef struct {
   uint8   AlarmsCount;
   AlarmType* AlarmRef;
   TickType MaxAllowedValue;
   TickType MinCycle;
   TickType TicksPerBase;
} CounterConstType;

typedef struct {
   TickType Time;
} CounterVarType;

/*==================[external data declaration]==============================*/
/** \brief ErrorHookRunning
 **
 ** This variable is used to check if the error hook is been executed.
 ** 0 ErrorHook is not been executed
 ** 1 ErrorHook is been executed.
 **/
extern uint8 ErrorHookRunning;

/** \brief Tasks Constants
 **
 ** Contents all constant and constant pointer needed to
 ** manage all FreeOSEK tasks
 **/
extern const TaskConstType TasksConst[TASKS_COUNT];

/** \brief Remote Tasks Core Number
 **
 ** Contents the core number for each remote task.
 **/
extern const TaskCoreType RemoteTasksCore[REMOTE_TASKS_COUNT];

/** \brief Tasks Variable
 **
 ** Contents all variables needed to manage all FreeOSEK tasks
 **/
extern TaskVariableType TasksVar[TASKS_COUNT];

/** \brief Application Mode
 **
 ** This variable contents the actual running application mode
 **/
extern uint8 ApplicationMode;

/** \brief List of Auto Start Tasks in Application Mode AppMode1 */
extern const TaskType TasksAppModeAppMode1[1];
/** \brief AutoStart Array */
extern const AutoStartType AutoStart[1];

/** \brief Resources Priorities */
extern const TaskPriorityType ResourcesPriority[1];

/** \brief Ready Const List */
extern const ReadyConstType ReadyConst[2];

/** \brief Ready Variable List */
extern ReadyVarType ReadyVar[2];

/** \brief Resources Priorities */
extern const TaskPriorityType ResourcesPriority[1];

/** \brief Alarms Variable Structure */
extern AlarmVarType AlarmsVar[1];

/** \brief Alarms Constant Structure */
extern const AlarmConstType AlarmsConst[1];

/** \brief Alarms Constant Structure */
extern const AutoStartAlarmType AutoStartAlarm[ALARM_AUTOSTART_COUNT];

/** \brief Counter Var Structure */
extern CounterVarType CountersVar[1];

/** \brief Counter Const Structure */
extern const CounterConstType CountersConst[1];
/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_CFG_H_ */

/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

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

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090719 v0.1.7 MaCe rename file to Os_
 * 20090331 v0.1.6 MaCe add USERESSCHEDULER evaluation
 * 20090330 v0.1.5 MaCe add NO_EVENTS macro
 * 20090327 v0.1.4 MaCe add declaration of the start task for the app. modes
 * 20090131 v0.1.3 MaCe add extern to CountersVar declaration
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20090128 v0.1.1 MaCe remove ENABLE and DISABLE macro, now defined in OpenGEN
 * 20080713 v0.1.0 MaCe	initial version
 */
<?php

function remove($a,$index)
{
if ($index < count($a))
{
for ($i = $index; $i < count($a)-1; $i++)
{
 $a[$i] = $a[$i+1];
}
array_pop($a);
}
return $a;
}

function remove_doubles($a)
{
sort($a);
$old = NULL;
for($loopi = 0; $loopi < count($a); $loopi++)
{
if ($old == $a[$loopi])
{
 /* if equal remove this element */
 $a = remove($a,$loopi);
 $loopi--;
}
else
{
 $old = $a[$loopi];
}
}

return $a;
}

/* get tasks */
$tasks = $config->getList("/OSEK","TASK");

/* convert config priority to real osek priority */
$priorities = array();
foreach ($tasks as $task)
{
$priorities[] = $config->getValue("/OSEK/" . $task, "PRIORITY");
}
$priorities = remove_doubles($priorities);
$count = 0;
$priority = array();
foreach ($priorities as $prio)
{
$priority[$prio] = $count++;
}
arsort($priority);

?>
/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief ERROR_CHECKING_STANDARD */
#define ERROR_CHECKING_STANDARD   1

/** \brief ERROR_CHECKING_EXTENDED */
#define ERROR_CHECKING_EXTENDED   2

/** \brief Count of task */
<?php
$taskscount = $config->getCount("/OSEK","TASK");
if ($taskscount<=0)
{
	error("No tasks found in the configuration.\n");
}
print "#define TASKS_COUNT	$taskscount" . "U\n\n";


/* Define the Resources */
$resources = $config->getList("/OSEK","RESOURCE");
if(count($resources)>31)
{
	error("more than 31 resources were defined");
}
else
{
	print "/** \brief Count of resources */\n";
	print "#define RESOURCES_COUNT " . count($resources) . "\n\n";
}

$os = $config->getList("/OSEK","OS");
if (count($os)>1)
{
	error("More than one OS defined on the configuration");
}
$osattr = $config->getValue("/OSEK/" . $os[0],"STATUS");
print	"/** \brief Error Checking Type */\n";
if ( $osattr == "EXTENDED" )
{
	print	"#define ERROR_CHECKING_TYPE	ERROR_CHECKING_EXTENDED\n";
}
elseif ( $osattr == "STANDARD" )
{
	print "#define ERROR_CHECKING_TYPE  ERROR_CHECKING_STANDARD\n";
}
else
{
	error("Wrong OS Status configuration");
}

/* PRETASKHOOK */
$pretaskhook=$config->getValue("/OSEK/" . $os[0],"PRETASKHOOK");
print "/** \brief pre task hook enable-disable macro */\n";
if($pretaskhook == "")
{
	warning("PRETASKHOOK isn't defined on the configuration, set disable as default");
	print "#define HOOK_PRETASKHOOK DISABLE\n";
}
elseif($pretaskhook == "TRUE")
{
	print "#define HOOK_PRETASKHOOK ENABLE\n";
}
elseif($pretaskhook == "FALSE")
{
   print "#define HOOK_PRETASKHOOK DISABLE\n";
}
else
{
	error("PRETASKHOOK set to an invalid value \"$pretaskhook\"");
}
/* POSTTAKHOOK */
$posttaskhook=$config->getValue("/OSEK/" . $os[0],"POSTTASKHOOK");
print "/** \brief post task hook enable-disable macro */\n";
if($posttaskhook == "")
{
   warning("POSTTASKHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_POSTTASKHOOK DISABLE\n";
}
elseif($posttaskhook == "TRUE")
{
   print "#define HOOK_POSTTASKHOOK ENABLE\n";
}
elseif($pretaskhook == "FALSE")
{
   print "#define HOOK_POSTTASKHOOK DISABLE\n";
}
else
{
   error("POSTTASKHOOK set to an invalid value \"$pretaskhook\"");
}
/* ERRORHOOK */
$errorhook=$config->getValue("/OSEK/" . $os[0],"ERRORHOOK");
print "/** \brief error hook enable-disable macro */\n";
if($errorhook == "")
{
   warning("ERRORHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_ERRORHOOK DISABLE\n";
}
elseif($errorhook == "TRUE")
{
   print "#define HOOK_ERRORHOOK ENABLE\n";
}
elseif($errorhook == "FALSE")
{
   print "#define HOOK_ERRORHOOK DISABLE\n";
}
else
{
   error("ERRORHOOK set to an invalid value \"$pretaskhook\"");
}
/* STARTUPHOOK */
$startuphook=$config->getValue("/OSEK/" . $os[0],"STARTUPHOOK");
print "/** \brief startup hook enable-disable macro */\n";
if($startuphook == "")
{
   warning("STARTUPHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_STARTUPHOOK DISABLE\n";
}
elseif($startuphook == "TRUE")
{
   print "#define HOOK_STARTUPHOOK ENABLE\n";
}
elseif($startuphook == "FALSE")
{
   print "#define HOOK_STARTUPHOOK DISABLE\n";
}
else
{
   error("STARTUPHOOK set to an invalid value \"$pretaskhook\"");
}
/* SHUTDOWNHOOK */
$shutdownhook=$config->getValue("/OSEK/" . $os[0],"SHUTDOWNHOOK");
print "/** \brief shutdown hook enable-disable macro */\n";
if($shutdownhook == "")
{
   warning("SHUTDOWNHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_SHUTDOWNHOOK DISABLE\n";
}
elseif($shutdownhook == "TRUE")
{
   print "#define HOOK_SHUTDOWNHOOK ENABLE\n";
}
elseif($shutdownhook == "FALSE")
{
   print "#define HOOK_SHUTDOWNHOOK DISABLE\n";
}
else
{
   error("SHUTDOWNHOOK set to an invalid value \"$pretaskhook\"");
}


?>

#define READYLISTS_COUNT <?php echo count($priority); ?>

#define SetError_Api(api)   ( Osek_ErrorApi = (api) )
#define SetError_Param1(param1) ( Osek_ErrorParam1 = (param1) )
#define SetError_Param2(param2) ( Osek_ErrorParam2 = (param2) )
#define SetError_Param3(param3) ( Osek_ErrorParam3 = (param3) )
#define SetError_Ret(ret) ( Osek_ErrorRet = (uint32)(ret) )
#define SetError_Msg(msg)
/* { printf ("Error found in file: \"%s\" line \"%d\" ", __FILE__, __LINE__); printf(msg); } */
#define SetError_ErrorHook()			\
	{											\
		ErrorHookRunning = (uint8)1U;	\
		ErrorHook();						\
		ErrorHookRunning = (uint8)0U;	\
	}

<?php
$alarms = $config->getList("/OSEK","ALARM");
$count = 0;
foreach ($alarms as $alarm)
{
	if ($config->getValue("/OSEK/" . $alarm, "AUTOSTART") == "TRUE")
	{
		$count++;
	}
}
?>
#define ALARM_AUTOSTART_COUNT <?php echo $count ?>


<?php
$counters = $config->getList("/OSEK","COUNTER");
$count = 0;
foreach ($counters as $counter)
{
	print "#define	OSEK_COUNTER_" . $counter . " " . $count . "\n";
	$count++;
}

$alarms = $config->getList("/OSEK","ALARM");
print "/** \brief ALARMS_COUNT define */\n";
print "#define ALARMS_COUNT " . count($alarms) . "\n\n";

$preemptive = false;
foreach($tasks as $task)
{
	$schedule = $config->getValue("/OSEK/" .$task, "SCHEDULE");
	if($schedule == "FULL")
	{
		$preemptive = true;
	}
}

print "/** \brief NON_PREEMPTIVE macro definition */\n";
if ($preemptive == false)
{
	print "#define NON_PREEMPTIVE	ENABLE\n\n";
}
else
{
	print "#define NON_PREEMPTIVE	DISABLE\n\n";
}

$events = $config->getList("/OSEK","EVENT");
print "/** \brief NO_EVENTS macro definition */\n";
if(count($events) == 0)
{
	print "#define NO_EVENTS ENABLE\n\n";
}
else
{
	print "#define NO_EVENTS DISABLE\n\n";
}

$schedulerpolicy = $config->getValue("/OSEK/" . $os[0],"USERESSCHEDULER");
print "/** \brief NO_RES_SCHEDULER macro definition */\n";
switch($schedulerpolicy)
{
	case "FALSE":
		print "#define NO_RES_SCHEDULER ENABLE\n\n";
		break;
	case "TRUE":
		print "#define NO_RES_SCHEDULER DISABLE\n\n";
		break;
	default :
		warning("USERESSCHEDULER not defined on the configuration, using FALSE as default");
		print "#define NO_RES_SCHEDULER ENABLE\n\n";
		break;
}


?>

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
	uint8	AlarmsCount;
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

<?php
$appmodes = $config->getList("/OSEK", "APPMODE");

foreach ($appmodes as $appmode)
{
	$tasksinmode = array();
	foreach($tasks as $task)
	{
		$taskappmodes = $config->getList("/OSEK/" . $task, "APPMODE");
		foreach ($taskappmodes as $taskappmode)
		{
			if ($taskappmode == $appmode)
			{
				$tasksinmode[] = $task;
			}
		}
	}
	if (count($tasksinmode) > 0)
	{
		$count = 0;
		print "/** \brief List of Auto Start Tasks in Application Mode $appmode */\n";
		print "extern const TaskType TasksAppMode" . $appmode . "[" . count($tasksinmode). "];\n";
	}
}

$appmodes = $config->getList("/OSEK","APPMODE");
print "/** \brief AutoStart Array */\n";
print "extern const AutoStartType AutoStart[" . count($appmodes) . "];\n\n";

/* Resources Priorities */
$resources = $config->getList("/OSEK","RESOURCE");
print "/** \brief Resources Priorities */\n";
print "extern const TaskPriorityType ResourcesPriority[" . count($resources) . "];\n\n";

print "/** \brief Ready Const List */\n";
print "extern const ReadyConstType ReadyConst[" . count($priority) .  "];\n\n";
print "/** \brief Ready Variable List */\n";
print "extern ReadyVarType ReadyVar[" . count($priority) . "];\n\n";

$resources = $config->getList("/OSEK","RESOURCE");
print "/** \brief Resources Priorities */\n";
print "extern const TaskPriorityType ResourcesPriority[" . count($resources) . "];\n\n";

$alarms = $config->getList("/OSEK","ALARM");
print "/** \brief Alarms Variable Structure */\n";
print "extern AlarmVarType AlarmsVar[" . count($alarms) . "];\n\n";

print "/** \brief Alarms Constant Structure */\n";
print "extern const AlarmConstType AlarmsConst[" . count($alarms) . "];\n\n";

print "/** \brief Alarms Constant Structure */\n";
print "extern const AutoStartAlarmType AutoStartAlarm[ALARM_AUTOSTART_COUNT];\n\n";

$counters = $config->getList("/OSEK","COUNTER");
print "/** \brief Counter Var Structure */\n";
print "extern CounterVarType CountersVar[" . count($counters) . "];\n\n";

print "/** \brief Counter Const Structure */\n";
print "extern const CounterConstType CountersConst[" . count($counters) . "];\n";

?>
/*==================[external functions declaration]=========================*/
<?php
$intnames = $config->getList("/OSEK","ISR");
foreach ($intnames as $int)
{
   $inttype = $config->getValue("/OSEK/" . $int,"INTERRUPT");
   $intcat = $config->getValue("/OSEK/" . $int,"CATEGORY");

   if ($intcat == 2)
   {
      print "/** \\brief Interrupt Cat 2 $int\n";
      print " **/\n";
      print "extern void OSEK_ISR2_$int(void);\n";
   }

}
?>

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_CFG_H_ */

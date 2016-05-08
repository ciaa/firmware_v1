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

/* remove soon
 * you can load the helper here with
   $this->loadHelper("modules/rtos/gen/ginc/Multicore.php");

 * or when calling generator.php with
   -H modules/rtos/gen/ginc/Multicore.php
 *
*/

/* get tasks */
$tasks = $this->helper->multicore->getLocalList("/OSEK", "TASK");

$priority = $this->config->priority2osekPriority($tasks);

?>
/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief ERROR_CHECKING_STANDARD */
#define ERROR_CHECKING_STANDARD   1

/** \brief ERROR_CHECKING_EXTENDED */
#define ERROR_CHECKING_EXTENDED   2

/** \brief Count of task */
<?php
$taskscount = count($this->helper->multicore->getLocalList("/OSEK", "TASK"));
$remotetaskscount = count($this->helper->multicore->getRemoteList("/OSEK", "TASK"));

if ($taskscount<=0)
{
   $this->log->error("No tasks found in the configuration.\n");
}
print "#define TASKS_COUNT $taskscount" . "U\n\n";

print "/** \brief Remote tasks count */\n";
print "#define REMOTE_TASKS_COUNT $remotetaskscount" . "U\n\n";

/* Define the Resources */
$resources = $this->config->getList("/OSEK","RESOURCE");
if(count($resources)>31)
{
   $this->log->error("more than 31 resources were defined");
}
else
{
   print "/** \brief Count of resources */\n";
   print "#define RESOURCES_COUNT " . count($resources) . "\n\n";
}

$os = $this->config->getList("/OSEK","OS");
if (count($os)>1)
{
   $this->log->error("More than one OS defined on the configuration");
}
$osattr = $this->config->getValue("/OSEK/" . $os[0],"STATUS");
print "/** \brief Error Checking Type */\n";
if ( $osattr == "EXTENDED" )
{
   print "#define ERROR_CHECKING_TYPE ERROR_CHECKING_EXTENDED\n";
}
elseif ( $osattr == "STANDARD" )
{
   print "#define ERROR_CHECKING_TYPE  ERROR_CHECKING_STANDARD\n";
}
else
{
   $this->log->error("Wrong OS Status configuration");
}

/* PRETASKHOOK */
$pretaskhook=$this->config->getValue("/OSEK/" . $os[0],"PRETASKHOOK");
print "/** \brief pre task hook enable-disable macro */\n";
if($pretaskhook == "")
{
   $this->log->warning("PRETASKHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_PRETASKHOOK OSEK_DISABLE\n";
}
elseif($pretaskhook == "TRUE")
{
   print "#define HOOK_PRETASKHOOK OSEK_ENABLE\n";
}
elseif($pretaskhook == "FALSE")
{
   print "#define HOOK_PRETASKHOOK OSEK_DISABLE\n";
}
else
{
   $this->log->error("PRETASKHOOK set to an invalid value \"$pretaskhook\"");
}
/* POSTTAKHOOK */
$posttaskhook=$this->config->getValue("/OSEK/" . $os[0],"POSTTASKHOOK");
print "/** \brief post task hook enable-disable macro */\n";
if($posttaskhook == "")
{
   $this->log->warning("POSTTASKHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_POSTTASKHOOK OSEK_DISABLE\n";
}
elseif($posttaskhook == "TRUE")
{
   print "#define HOOK_POSTTASKHOOK OSEK_ENABLE\n";
}
elseif($posttaskhook == "FALSE")
{
   print "#define HOOK_POSTTASKHOOK OSEK_DISABLE\n";
}
else
{
   $this->log->error("POSTTASKHOOK set to an invalid value \"$pretaskhook\"");
}
/* ERRORHOOK */
$errorhook=$this->config->getValue("/OSEK/" . $os[0],"ERRORHOOK");
print "/** \brief error hook enable-disable macro */\n";
if($errorhook == "")
{
   $this->log->warning("ERRORHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_ERRORHOOK OSEK_DISABLE\n";
}
elseif($errorhook == "TRUE")
{
   print "#define HOOK_ERRORHOOK OSEK_ENABLE\n";
}
elseif($errorhook == "FALSE")
{
   print "#define HOOK_ERRORHOOK OSEK_DISABLE\n";
}
else
{
   $this->log->error("ERRORHOOK set to an invalid value \"$pretaskhook\"");
}
/* STARTUPHOOK */
$startuphook=$this->config->getValue("/OSEK/" . $os[0],"STARTUPHOOK");
print "/** \brief startup hook enable-disable macro */\n";
if($startuphook == "")
{
   $this->log->warning("STARTUPHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_STARTUPHOOK OSEK_DISABLE\n";
}
elseif($startuphook == "TRUE")
{
   print "#define HOOK_STARTUPHOOK OSEK_ENABLE\n";
}
elseif($startuphook == "FALSE")
{
   print "#define HOOK_STARTUPHOOK OSEK_DISABLE\n";
}
else
{
   $this->log->error("STARTUPHOOK set to an invalid value \"$pretaskhook\"");
}
/* SHUTDOWNHOOK */
$shutdownhook=$this->config->getValue("/OSEK/" . $os[0],"SHUTDOWNHOOK");
print "/** \brief shutdown hook enable-disable macro */\n";
if($shutdownhook == "")
{
   $this->log->warning("SHUTDOWNHOOK isn't defined on the configuration, set disable as default");
   print "#define HOOK_SHUTDOWNHOOK OSEK_DISABLE\n";
}
elseif($shutdownhook == "TRUE")
{
   print "#define HOOK_SHUTDOWNHOOK OSEK_ENABLE\n";
}
elseif($shutdownhook == "FALSE")
{
   print "#define HOOK_SHUTDOWNHOOK OSEK_DISABLE\n";
}
else
{
   $this->log->error("SHUTDOWNHOOK set to an invalid value \"$pretaskhook\"");
}

/* MULTICORE */
$multicore = $this->config->getValue("/OSEK/" . $os[0], "MULTICORE");
if ($multicore == "TRUE")
{
   print "/** \brief multicore API */\n";
   print "#define OSEK_MULTICORE OSEK_ENABLE\n";
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
#define SetError_ErrorHook()          \
   {                                  \
      ErrorHookRunning = (uint8)1U;   \
      ErrorHook();                    \
      ErrorHookRunning = (uint8)0U;   \
   }

<?php
$alarms = $this->helper->multicore->getLocalList("/OSEK", "ALARM");
$count = 0;
foreach ($alarms as $alarm)
{
   if ($this->config->getValue("/OSEK/" . $alarm, "AUTOSTART") == "TRUE")
   {
      $count++;
   }
}
?>
#define ALARM_AUTOSTART_COUNT <?php echo $count ?>


<?php
$counters = $this->helper->multicore->getLocalList("/OSEK", "COUNTER");

foreach ($counters as $count => $counter)
{
   print "#define OSEK_COUNTER_" . $counter . " " . $count . "\n";
}

$alarms = $this->helper->multicore->getLocalList("/OSEK", "ALARM");
print "/** \brief ALARMS_COUNT define */\n";
print "#define ALARMS_COUNT " . count($alarms) . "\n\n";

$preemptive = false;
foreach($tasks as $task)
{
   $schedule = $this->config->getValue("/OSEK/" .$task, "SCHEDULE");
   if($schedule == "FULL")
   {
      $preemptive = true;
   }
}

print "/** \brief NON_PREEMPTIVE macro definition */\n";
if ($preemptive == false)
{
   print "#define NON_PREEMPTIVE OSEK_ENABLE\n\n";
}
else
{
   print "#define NON_PREEMPTIVE OSEK_DISABLE\n\n";
}

$events = $this->config->getList("/OSEK","EVENT");
print "/** \brief NO_EVENTS macro definition */\n";
if(count($events) == 0)
{
   print "#define NO_EVENTS OSEK_ENABLE\n\n";
}
else
{
   print "#define NO_EVENTS OSEK_DISABLE\n\n";
}

$schedulerpolicy = $this->config->getValue("/OSEK/" . $os[0],"USERESSCHEDULER");
print "/** \brief NO_RES_SCHEDULER macro definition */\n";
switch($schedulerpolicy)
{
   case "FALSE":
      print "#define NO_RES_SCHEDULER OSEK_ENABLE\n\n";
      break;
   case "TRUE":
      print "#define NO_RES_SCHEDULER OSEK_DISABLE\n\n";
      break;
   default :
      $this->log->warning("USERESSCHEDULER not defined on the configuration, using FALSE as default");
      print "#define NO_RES_SCHEDULER OSEK_ENABLE\n\n";
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

<?php
$appmodes = $this->config->getList("/OSEK", "APPMODE");

foreach ($appmodes as $appmode)
{
   $tasksinmode = array();
   foreach($tasks as $task)
   {
      $taskappmodes = $this->config->getList("/OSEK/" . $task, "APPMODE");
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

$appmodes = $this->config->getList("/OSEK","APPMODE");
print "/** \brief AutoStart Array */\n";
print "extern const AutoStartType AutoStart[" . count($appmodes) . "];\n\n";

/* Resources Priorities */
$resources = $this->config->getList("/OSEK","RESOURCE");
print "/** \brief Resources Priorities */\n";
print "extern const TaskPriorityType ResourcesPriority[" . count($resources) . "];\n\n";

print "/** \brief Ready Const List */\n";
print "extern const ReadyConstType ReadyConst[" . count($priority) .  "];\n\n";
print "/** \brief Ready Variable List */\n";
print "extern ReadyVarType ReadyVar[" . count($priority) . "];\n\n";

$resources = $this->config->getList("/OSEK","RESOURCE");
print "/** \brief Resources Priorities */\n";
print "extern const TaskPriorityType ResourcesPriority[" . count($resources) . "];\n\n";

$alarms = $this->helper->multicore->getLocalList("/OSEK", "ALARM");

print "/** \brief Alarms Variable Structure */\n";
print "extern AlarmVarType AlarmsVar[" . count($alarms) . "];\n\n";

print "/** \brief Alarms Constant Structure */\n";
print "extern const AlarmConstType AlarmsConst[" . count($alarms) . "];\n\n";

print "/** \brief Alarms Constant Structure */\n";
print "extern const AutoStartAlarmType AutoStartAlarm[ALARM_AUTOSTART_COUNT];\n\n";

$counters = $this->helper->multicore->getLocalList("/OSEK", "COUNTER");

print "/** \brief Counter Var Structure */\n";
print "extern CounterVarType CountersVar[" . count($counters) . "];\n\n";

print "/** \brief Counter Const Structure */\n";
print "extern const CounterConstType CountersConst[" . count($counters) . "];\n";

?>
/*==================[external functions declaration]=========================*/
<?php
$intnames = $this->helper->multicore->getLocalList("/OSEK", "ISR");
foreach ($intnames as $int)
{
   $inttype = $this->config->getValue("/OSEK/" . $int,"INTERRUPT");
   $intcat = $this->config->getValue("/OSEK/" . $int,"CATEGORY");

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

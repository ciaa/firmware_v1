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
<?php
/** \brief FreeOSEK File to be Generated
 **
 ** \file Os_Cfg.h.php
 **
 **/
?>

#ifndef _OS_CFG_H_
#define _OS_CFG_H_
/** \brief FreeOSEK Os Generated Configuration Header File
 **
 ** This file contents the generated configuration of FreeOSEK Os
 **
 ** \file Os_Cfg.h
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
 * 20090719 v0.1.3 MaCe rename file to Os_
 * 20090424 v0.1.2 MaCe add counters defines
 * 20090128 v0.1.1 MaCe add MEMMAP off configuration
 * 20080810 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
/** \brief Definition of the  DeclareTask Macro */
#define DeclareTask(name)	void OSEK_TASK_ ## name (void)

#define OSEK_OS_INTERRUPT_MASK ((InterruptFlagsType)0xFFFFFFFFU)

<?php
/* Definitions of Tasks */
$tasks = $config->getList("/OSEK","TASK");
$count = 0;
foreach ($tasks as $task)
{
	print "/** \brief Task Definition */\n";
	print "#define $task $count\n";
	$count++;
}
print "\n";

/* Define the Applications Modes */
$appmodes = $config->getList("/OSEK","APPMODE");
$count = 0;
foreach ($appmodes as $appmode)
{
	print "/** \brief Definition of the Application Mode $appmode */\n";
	print "#define " . $appmode . " " . $count++ . "\n";
}
print "\n";

/* Define the Events */
$events = $config->getList("/OSEK","EVENT");
$count = 0;
foreach ($events as $event)
{
	print "/** \brief Definition of the Event $event */\n";
	print "#define " . $event . " " . (1<<$count++) . "\n";
}
print "\n";

/* Define the Resources */
$resources = $config->getList("/OSEK","RESOURCE");
$count = 0;
foreach ($resources as $resource)
{
	print "/** \brief Definition of the resource $resource */\n";
	print "#define " . $resource . " " . $count++ . "\n";
}
print "\n";

/* Define the Alarms */
$alarms = $config->getList("/OSEK","ALARM");
$count = 0;
foreach ($alarms as $alarm)
{
   print "/** \brief Definition of the Alarm $alarm */\n";
   print "#define " . $alarm . " " . $count++ . "\n";
}
print "\n";

/* Define the Counters */
$counters = $config->getList("/OSEK","COUNTER");
$count = 0;
foreach ($counters as $counter)
{
   print "/** \brief Definition of the Counter $counter */\n";
   print "#define " . $counter . " " . $count++ . "\n";
}
print "\n";

$errorhook=$config->getValue("/OSEK/" . $os[0],"ERRORHOOK");
if ($errorhook == "TRUE")
{
?>
/** \brief OS Error Get Service Id */
/* \req OSEK_ERR_0.1 The macro OSErrorGetServiceId() shall provide the service
 * identifier with a OSServiceIdType type where the error has been risen
 * \req OSEK_ERR_0.1.1 Possibly return values are: OSServiceId_xxxx, where
 * xxxx is the name of the system service
 */
#define OSErrorGetServiceId() (Osek_ErrorApi)

#define OSErrorGetParam1() (Osek_ErrorParam1)

#define OSErrorGetParam2() (Osek_ErrorParam2)

#define OSErrorGetParam3() (Osek_ErrorParam3)

#define OSErrorGetRet() (Osek_ErrorRet)

<?php
}

$memmap = $config->getValue("/OSEK/" . $os[0],"MEMMAP");
print "/** \brief OSEK_MEMMAP macro (DISABLE not MemMap is used for FreeOSEK, ENABLE\n ** MemMap is used for FreeOSEK) */\n";
if ($memmap == "TRUE")
{
	print "#define OSEK_MEMMAP ENABLE\n";
}
elseif ($memmap == "FALSE")
{
	print "#define OSEK_MEMMAP DISABLE\n";
}
else
{
	warning("MEMMAP configuration not found in FreeOSEK configuration, disabling as default");
	print "#define OSEK_MEMMAP DISABLE\n";
}

?>

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
<?php
$errorhook=$config->getValue("/OSEK/" . $os[0],"ERRORHOOK");
if ($errorhook == "TRUE")
{
?>
/** \brief Error Api Variable
 **
 ** This variable contents the api which generate the last error
 **/
extern unsigned int Osek_ErrorApi;

/** \brief Error Param1 Variable
 **
 ** This variable contents the first parameter passed to the api which has
 ** generted the last error.
 **/
extern unsigned int Osek_ErrorParam1;

/** \brief Error Param2 Variable
 **
 ** This variable contents the second parameter passed to the api which has
 ** generted the last error.
 **/
extern unsigned int Osek_ErrorParam2;

/** \brief Error Param3 Variable
 **
 ** This variable contents the third parameter passed to the api which has
 ** generted the last error.
 **/
extern unsigned int Osek_ErrorParam3;

/** \brief Error Return Variable
 **
 ** This variable contents return value of the api which has generated
 ** the last error.
 **/
extern unsigned int Osek_ErrorRet;

<?php
}
?>

/*==================[external functions declaration]=========================*/
<?php
$pretaskhook=$config->getValue("/OSEK/" . $os[0],"PRETASKHOOK");
if ($pretaskhook == "TRUE")
{
	print "/** \brief Pre Task Hook */\n";
	print "extern void PreTaskHook(void);\n\n";
}
$posttaskhook=$config->getValue("/OSEK/" . $os[0],"POSTTASKHOOK");
if ($posttaskhook == "TRUE")
{
   print "/** \brief Post Task Hook */\n";
   print "extern void PostTaskHook(void);\n\n";
}
$shutdownhook=$config->getValue("/OSEK/" . $os[0],"SHUTDOWNHOOK");
if ($shutdownhook == "TRUE")
{
   print "/** \brief Shutdown Hook */\n";
   print "extern void ShutdownHook(void);\n\n";
}
$startuphook=$config->getValue("/OSEK/" . $os[0],"STARTUPHOOK");
if ($startuphook == "TRUE")
{
   print "/** \brief Startup Hook */\n";
   print "extern void StartupHook(void);\n\n";
}
$errorhook=$config->getValue("/OSEK/" . $os[0],"ERRORHOOK");
if ($errorhook == "TRUE")
{
   print "/** \brief Error Hook */\n";
   print "extern void ErrorHook(void);\n\n";
}

/* Declare Tasks */
$count = 0;
foreach ($tasks as $task)
{
	print "/** \brief Task Declaration of Task $task */\n";
	print "DeclareTask($task);\n";
}
print "\n";

$intnames = $config->getList("/OSEK","ISR");
foreach ($intnames as $int)
{
	print "/** \brief ISR Declaration */\n";
	print "extern void OSEK_ISR_$int(void); /* Interrupt Handler $int */\n";
}
print "\n";

$alarms = $config->getList("/OSEK","ALARM");
foreach ($alarms as $alarm)
{
	$action = $config->getValue("/OSEK/" . $alarm, "ACTION");
	if ($action == "ALARMCALLBACK")
	{
		print "/** \brief Alarm Callback declaration */\n";
		print "extern void OSEK_CALLBACK_" . $config->getValue("/OSEK/" . $alarm . "/ALARMCALLBACK", "CALLBACK") . "(void);\n";
	}
}
print "\n";

?>

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_CFG_H_ */


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

#ifndef _CTEST_CFG_H_
#define _CTEST_CFG_H_
/** \brief Free OSEK Conformance Test
 **
 ** \file FreeOSEK/tst/ctest/gen/inc/ctest_cfg.h
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_CT Conformance Test
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe			 Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090403 v0.1.0 MaCe	initial version based on module tests
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/
<?php
$os = $config->getList("/OSEK","OS");
$osattr = $config->getValue("/OSEK/" . $os[0],"STATUS");
print	"/** \brief Error Checking Type */\n";
if ( $osattr == "EXTENDED" )
{
	print	"#define CT_ERROR_CHECKING_TYPE	CT_ERROR_CHECKING_EXTENDED\n\n";
}
elseif ( $osattr == "STANDARD" )
{
	print "#define CT_ERROR_CHECKING_TYPE  CT_ERROR_CHECKING_STANDARD\n\n";
}

/* get tasks */
$tasks = $config->getList("/OSEK","TASK");

foreach($tasks as $task)
{
	print	"/** \brief Confromance Test Scheduling information for task $task */\n";
	$schedule = $config->getValue("/OSEK/" . $task, "SCHEDULE");
	if($schedule == "FULL")
	{
		print "#define CT_SCHEDULING_" . $task . "	CT_PREEMPTIVE\n\n";
	}
	elseif ($schedule == "NON")
	{
		print "#define CT_SCHEDULING_" . $task . "	CT_NON_PREEMPTIVE\n\n";
	}
	else
	{
		error("No Scheduling type was specified for task $task");
	}
}

?>

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CTEST_CFG_H_ */


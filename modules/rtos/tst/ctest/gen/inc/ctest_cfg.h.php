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


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

#ifndef _OS_ARCH_H_
#define _OS_ARCH_H_

/*****************************************************************************
 * update the tmparch directory on the \file and \arch doxygen comment with your
 * architecture.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief FreeOSEK Os Architecture Dependent Header File
 **
 ** This file is included form os.h and defines macros
 ** and types which depends on the architecture.
 **
 ** \file tmparch/Os_Arch.h
 ** \arch tmparch
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Global
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
/*****************************************************************************
 * add any needed include, please take into account that normaly NO INCLUDE
 * shall be included here, but in case you can do it, but discuss the reason
 * with the project manager.
 * Normaly nothing shall be included here
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[macros]=================================================*/
/*****************************************************************************
 * Please define here all needed macros that will be visible to the OS user
 * for this architecutre. This means that anyone including os.h will have
 * access to this definitions if the actual architecutre is used.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/** \brief Enable All Interrupts Arch */
#define EnableAllInterrupts_Arch()	ResumeAllInterrupts_Arch()

/** \brief Disable All Interrupts Arch */
#define DisableAllInterrupts_Arch() SuspendAllInterrupts_Arch()

/** \brief Resume All Interrupts Arch
 **
 ** This macro shall resume (enable) all interrupts.
 **/
#define ResumeAllInterrupts_Arch()						\
	{																\
	}

/** \brief Suspend All Interrupts Arch
 **
 ** This macro shall suspend (disable) all interrupts.
 **/
#define SuspendAllInterrupts_Arch()						\
	{																\
	}

/** \brief Resume OS Interrupts Arch
 **
 ** This macro shall resume (enable) all interrupts configured on the
 ** FreeOSEK OIL configuration file as ISR1 or ISR2.
 **/
#define ResumeOSInterrupts_Arch()						\
	{																\
	}

/** \brief Suspend OS Interrupts Arch
 **
 ** This macro shall suspend (disable) all interrupts configured on the
 ** FreeOSEK OIL configuration file as ISR1 or ISR2.
 **/
#define SuspendOSInterrupts_Arch()					\
	{															\
	}

/*==================[typedef]================================================*/
/*****************************************************************************
 * Please define here all needed types that will be visible to the OS user
 * for this architecutre. This means that anyone including os.h will have
 * access to these type definitions if the actual architecutre is used.
 * Normaly no type has to be declared here.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external data declaration]==============================*/
/*****************************************************************************
 * Please declare here all exported data defined in Osek_Arch.c that will
 * be visible to the OS user for this architectire. This means that anyone
 * including os.h will have access to these variables, if the actual
 * architecutre is used. Normaly no variables has to be declared here.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions declaration]=========================*/
/*****************************************************************************
 * Please declare here all exported functions defined in Osek_Arch.c that will
 * be visible to the OS user for this architectire. This means that anyone
 * including os.h will have access to these variables, if the actual
 * architecutre is used. Normaly no variables has to be declared here.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_ARCH_H_ */


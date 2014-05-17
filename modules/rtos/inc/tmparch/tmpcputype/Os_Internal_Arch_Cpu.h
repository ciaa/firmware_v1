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

#ifndef _OS_INTERNAL_ARCH_CPU_H_
#define _OS_INTERNAL_ARCH_CPU_H_

#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * update the tmparch and tmpcyputype directory on the \file and \arch doxygen
 * comment with your architecture.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief FreeOSEK Internal Architecture Cpu Dependent Header File
 **
 ** \file tmparch/tmpcputype/Os_Internal_Arch_Cpu.h
 ** \arch tmparch/tmpcputype
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

/*==================[cpu macros]=============================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please define here all cpu macros available for this cputype. arm7 and
 * cpy family lpc2xxx is given as example, please remove all arm7 and lpc2xxx
 * related macros
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief lpc2xxx cpu definition */
#define lpc2101	1
/** \brief lpc2xxx cpu definition */
#define lpc2102	2
/** \brief lpc2xxx cpu definition */
#define lpc2103	3
/* ... */
#define lpc2478	46
/** \brief lpc2xxx cpu definition */
#define lpc2880	47
/** \brief lpc2xxx cpu definition */
#define lpc2888	48

/*==================[inclusions]=============================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Include the right macros for every cpu. Please remove all arm7 and lpc2xxx
 * related macros.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
#ifndef CPU
#error CPU is not defined
#elif (CPU == lpc2468)
#include "lpc2468.h"
#elif (CPU == lpc2378)
#include "lpc23xx.h"
#else
#error not defined CPU
#endif

/*==================[macros]=================================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * use this section to defined any necessary macros for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[typedef]================================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * use this section to defined any necessary types for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external data declaration]==============================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * use this section to declare any necessary variables for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions declaration]=========================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * use this section to declare any necessary function for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_CPU_H_ */


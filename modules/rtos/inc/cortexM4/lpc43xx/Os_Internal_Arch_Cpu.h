/* Copyright 2014, ACSE & CADIEEL
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

/** \brief FreeOSEK Internal Architecture Cpu Dependent Header File
 **
 ** \file cortexM4/lpc4000/Os_Internal_Arch_Cpu.h
 ** \arch cortexM4/lpc4000
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
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
 * PR		Pablo Ridolfi
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
 * 20140608 v0.1.1 PR	First version for LPC4337 microcontroller.
 */

/*==================[cpu macros]=============================================*/
/*****************************************************************************
 * Please define here all cpu macros available for this cputype. arm7 and
 * cpy family lpc2xxx is given as example, please remove all arm7 and lpc2xxx
 * related macros
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief lpc4337 cpu definition */
#define lpc4337 	1

/*==================[inclusions]=============================================*/
/*****************************************************************************
 * Include the right macros for every cpu. Please remove all arm7 and lpc2xxx
 * related macros.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
#ifndef CPU
#error CPU is not defined
#elif (CPU == lpc4337)
#include "LPC43xx.h"
#include "core_cmFunc.h"
#else
#error not defined CPU
#endif

/*==================[macros]=================================================*/
/*****************************************************************************
 * use this section to defined any necessary macros for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[typedef]================================================*/
/*****************************************************************************
 * use this section to defined any necessary types for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external data declaration]==============================*/
/*****************************************************************************
 * use this section to declare any necessary variables for a specific cpu of
 * your family.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions declaration]=========================*/
void StartOs_Arch_Cpu(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_CPU_H_ */


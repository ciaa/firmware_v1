/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2014, Pablo Ridolfi (UTN-FRBA)
 * All rights reserved.
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

#ifndef _OS_INTERNAL_ARCH_CFG_H_
#define _OS_INTERNAL_ARCH_CFG_H_

/*****************************************************************************
 * update the tmparch directory on the \file doxygen comment with your
 * architecture
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief FreeOSEK Os Generated Internal Architecture Configuration Header File
 **
 ** This file content the internal generated architecture dependent
 ** configuration of FreeOSEK.
 **
 ** \file cortexM4/Os_Internal_Arch_Cfg.h
 ** \arch cortexM4
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
 * PR           Pablo Ridolfi
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
 * v0.1.1 20141130 PR   Added interrupt processing functions.
 * v0.1.0 20140608 PR   First version for Cortex-M processors.
 */

/*==================[inclusions]=============================================*/
/*****************************************************************************
 * Please define here all needed macros of your configuration that can
 * be configured or are used in Osek_Internal_Arch_Cfg.c.php
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[typedef]================================================*/

/** \brief Task Context Type */
typedef uint32 * TaskContextType;

/** \brief Task Context Type */
typedef TaskContextType* TaskContextRefType;

/*==================[external data declaration]==============================*/
/*****************************************************************************
 * Please declare here all exported data defined
 * Osek_Internal_Arch_Cfg.c.php
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions declaration]=========================*/

void Enable_User_ISRs(void);
void Enable_ISR2_Arch(void);
void Disable_ISR2_Arch(void);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _OS_INTERNAL_ARCH_CFG_H_ */


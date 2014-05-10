#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * add your copyright notice
 * Copyright <year>, <your name>
 *
 * Please do not change the license on a single file, if you want to change it
 * discuss it with the development team.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

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


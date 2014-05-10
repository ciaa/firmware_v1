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

#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * update the tmparch directory on the \file doxygen comment with your
 * architecture
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief FreeOSEK StartOs Architecture Dependece Implementation File
 **
 ** This file implements the StartOs Arch API
 **
 ** \file tmparch/StartOs_Arch.c
 ** \arch tmparch
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
 * v0.1.0 20090526 MaCe	FreeOSEK architecture template file
 */


/*==================[inclusions]=============================================*/
#include "Os_Internal.h"
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * add any needed include, please take into account that normaly NO INCLUDE
 * shall be included here other than Osek_Internal.h, but in case you can do
 * it, but discuss the reason with the project manager.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[macros and definitions]=================================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please try to keep empty this part, define all needed macros
 * in Osek_Internal_Arch.h
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[internal data declaration]==============================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please declare here all needed variables which are only used
 * in this file.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[internal functions declaration]=========================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please declare here all needed functions which are only used
 * in this file.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[internal data definition]===============================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please define here all needed variables which are only used
 * in this file.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external data definition]===============================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please declare here all needed variables which are
 * exported. Please define these variables in Osek_Internal_Arch.h
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[internal functions definition]==========================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please define here all needed functions which are only used
 * in this file.
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/

/*==================[external functions definition]==========================*/
#error this is a remember to remove the comment on the following line
/*****************************************************************************
 * Please declare here all needed functions which are
 * exported. Please define these functions in Osek_Internal_Arch.h
 *
 * PLEASE REMOVE THIS COMMENT
 *****************************************************************************/
/** \brief Architecture Dependnece Start Os function
 **
 ** This function is defined in Osek_Internal.h and has to be defined for
 ** every architecture. This function shall set for all tasks:
 **	- init the stack for all tasks.
 **	- init the entry point of all stack
 **	- init all other necessary registers to the init value for the first
 **	  time that the task will be executed.
 **	- if necessary to call StartOs_Arch_Cpu
 **
 ** Needed variables for this function will be:
 **	- TasksConst[].TaskContext
 **	- TasksConst[].StackPtr
 **	- TasksConst[].StackSize
 **	- TasksConst[].EntryPoint
 **
 ** Needed macros for this function will be:
 **	- TASKS_COUNT
 **
 **/
void StartOs_Arch(void)
{
	uint8f loopi;

	/* init every task */
	for( loopi = 0; loopi < TASKS_COUNT; loopi++)
	{
		/* init stack */
#error update here and remove this comment

		/* init entry point */
#error update here and remove this comment

		/* init registers */
#error update here and remove this comment
	}

	/* call CPU dependent initialisation */
#error update here and remove this comment

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


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

/** \brief FreeOSEK Os ReleaseResource Implementation File
 **
 ** This file implements the ReleaseResource API
 **
 ** \file ReleaseResource.c
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
 * MaCe         Mariano Cerdeiro
 * KLi          Kang Li
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20090414 v0.1.7 MaCe fix optmization for NO_RES_SCHEDULER
 * 20090414 v0.1.6 MaCe bugfix NON_PREEMPTIVE == DISABLE and not ENABLE
 * 20090331 v0.1.5 MaCe add support to NO_RES_SCHEDULER
 * 20090330 v0.1.5 MaCe correct errors done in v0.1.3
 * 20090330 v0.1.4 MaCe add support to NON_PREEMPTIVE systems and add non
 *								preemptive check
 * 20090329 v0.1.3 MaCe add RES_SCHEDULER and imp. code if RESOURCE_COUNT is
 *								different than 0
 * 20090130 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080909 v0.1.0 MaCe	initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/* only compile this function if RESOURCE_COUNT != 0 or if RES_SCHEDULER is
 * used */
#if ( (NO_RES_SCHEDULER == DISABLE) || (RESOURCES_COUNT != 0) )
StatusType ReleaseResource
(
	ResourceType ResID
)
{
	/* \req OSEK_SYS_3.13 The system service StatusType
	 * ReleaseResource ( ResourceType ResID ) shall be defined */

	/* \req OSEK_SYS_3.14.2: Possible return values in Standard mode is
	 * E_OK  */
	StatusType ret = E_OK;

#if (RESOURCES_COUNT != 0)
	uint8 loopi;
#endif /* #if (RESOURCES_COUNT != 0) */

	/* asign the static priority to the task */
	TaskPriorityType priority = TasksConst[GetRunningTask()].StaticPriority;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if (
/* only if one or more resources were defined */
#if (RESOURCES_COUNT != 0)
		  ( ResID > RESOURCES_COUNT )
#endif /* #if (RESOURCES_COUNT != 0) */
#if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == DISABLE) )
				&&
#endif /* #if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == DISABLE) ) */
/* check RES_SCHEDULER only if used */
#if (NO_RES_SCHEDULER == DISABLE)
		  ( ResID != RES_SCHEDULER )
#endif /* #if (NO_RES_SCHEDULER == DISABLE) */
		)
	{
		/* \req OSEK_SYS_3.14.3-1/2 Extra possible return values in Extended mode are
		 * E_OS_ID, E_OS_NOFUNC, E_OS_ACCESS */
		ret = E_OS_ID;
	}
	else
#if (NO_RES_SCHEDULER == DISABLE)
		if ( ResID != RES_SCHEDULER )
#endif /* #if (NO_RES_SCHEDULER == DISABLE) */
	{
		if ( ( TasksVar[GetRunningTask()].Resources & ( 1 << ResID ) ) == 0 )
		{
			/* \req OSEK_SYS_3.14.3-2/2 Extra possible return values in Extended mode are
			 ** E_OS_ID, E_OS_NOFUNC, E_OS_ACCESS */
			ret = E_OS_NOFUNC;
		}
	}
#if (NO_RES_SCHEDULER == DISABLE)
	else
	{
		/* nothing to do */
	}
#endif /* #if (NO_RES_SCHEDULER == DISABLE) */

	if ( ret == E_OK )
#endif /* #if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) */
	{
		IntSecure_Start();

#if (RESOURCES_COUNT != 0)
#if (NO_RES_SCHEDULER == DISABLE)
		if ( ResID != RES_SCHEDULER )
#endif /* #if (NO_RES_SCHEDULER == DISABLE) */
		{
			/* clear resource */
	   	TasksVar[GetRunningTask()].Resources &= ~( 1 << ResID );
		}

		for (loopi = 0; loopi < RESOURCES_COUNT; loopi++)
		{
			if ( TasksVar[GetRunningTask()].Resources & ( 1 << loopi ) )
			{
				if ( priority < ResourcesPriority[loopi] )
				{
					priority = ResourcesPriority[loopi];
				}
			}
		}
#endif /* #if (RESOURCES_COUNT != 0) */

		/* \req OSEK_SYS_3.14.1 ReleaseResource is the counterpart of GetResource
		 * and serves to leave critical sections in the code that are assigned to
		 * the resource referenced by ResID */
		TasksVar[GetRunningTask()].ActualPriority = priority;

		IntSecure_End();

#if (NON_PREEMPTIVE == DISABLE)
		/* check if called from a Task Context */
		if ( GetCallingContext() ==  CONTEXT_TASK )
		{
			if ( ( TasksConst[GetRunningTask()].ConstFlags.Preemtive ) &&
				  ( ret == E_OK )	)
			{
				/* \req OSEK_SYS_3.14.4 Rescheduling shall take place only if called from a
				 * preemptable task. */
				(void)Schedule();
			}
		}
#endif /* #if (NON_PREEMPTIVE == DISABLE) */

	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
		(HOOK_ERRORHOOK == ENABLE) )
	/* \req OSEK_ERR_1.3-7/xx The ErrorHook hook routine shall be called if a
	 * system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-7/xx The hook routine ErrorHook is not called if a
	 * system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
	{
		SetError_Api(OSServiceId_ReleaseResource);
		SetError_Param1(ResID);
		SetError_Ret(ret);
		SetError_Msg("ReleaseResource returns != E_OK");
		SetError_ErrorHook();
	}
#endif

	return ret;
}
#endif /* #if ( (NO_RES_SCHEDULER == DISABLE) || (RESOURCES_COUNT != 0) ) */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


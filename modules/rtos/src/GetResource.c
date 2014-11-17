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

/** \brief FreeOSEK Os GetResource Implementation File
 **
 ** This file implements the GetResource API
 **
 ** \file GetResource.c
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
 * 20090414 v0.1.6 MaCe fix optmization for NO_RES_SCHEDULER
 * 20090331 v0.1.5 MaCe add support to NO_RES_SCHEDULER
 * 20090330 v0.1.4 MaCe correct errors done in v0.1.3
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
#if ( (NO_RES_SCHEDULER == OSEK_DISABLE) || (RESOURCES_COUNT != 0) )
StatusType GetResource
(
	ResourceType ResID
)
{
	/* \req OSEK_SYS_3.13 The system service StatusType
	 * GetResource ( ResourceType ResID ) shall be defined */

	/* \req OSEK_SYS_3.13.2: Possible return values in Standard mode is E_OK */
	StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
	if (
/* only if one or more resources were defined */
#if (RESOURCES_COUNT != 0)
			( ResID > RESOURCES_COUNT )
#endif /* (RESOURCES_COUNT != 0) */
#if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == OSEK_DISABLE) )
				&&
#endif /* #if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == OSEK_DISABLE) ) */
/* check RES_SCHEDULER only if used */
#if (NO_RES_SCHEDULER == OSEK_DISABLE)
			( ResID != RES_SCHEDULER )
#endif /* #if (NO_RES_SCHEDULER == OSEK_DISABLE) */
		)
	{
		/* \req OSEK_SYS_3.13.3-1/2 Extra possible return values in Extended mode are
		 * E_OS_ID, E_OS_ACCESS */
		ret = E_OS_ID;
	}
	else
#if (NO_RES_SCHEDULER == OSEK_DISABLE)
		if ( ResID != RES_SCHEDULER )
#endif /* #if (NO_RES_SCHEDULER == OSEK_DISABLE) */
	{
		if ( ( TasksVar[GetRunningTask()].Resources & ( 1 << ResID ) ) ||
			  ( ( TasksConst[GetRunningTask()].ResourcesMask & ( 1 << ResID ) ) == 0 ) )
		{
			/* \req OSEK_SYS_3.13.3-2/2 Extra possible return values in Extended mode are
			 * E_OS_ID, E_OS_ACCESS */
			ret = E_OS_ACCESS;
		}
	}
#if (NO_RES_SCHEDULER == OSEK_DISABLE)
	else
	{
		/* nothing to do */
	}
#endif /* #if (NO_RES_SCHEDULER == OSEK_DISABLE) */

	if ( ret == E_OK )
#endif /* #if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) */
	{
		IntSecure_Start();

/* check RES_SCHEDULER only if used */
#if (NO_RES_SCHEDULER == OSEK_DISABLE)
		if ( ResID == RES_SCHEDULER )
		{
			TasksVar[GetRunningTask()].ActualPriority = TASK_MAX_PRIORITY;
		}
#endif /* #if (NO_RES_SCHEDULER == OSEK_DISABLE) */
#if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == OSEK_DISABLE) )
		else
#endif /* #if ( (RESOURCES_COUNT != 0) && (NO_RES_SCHEDULER == OSEK_DISABLE) ) */

/* only if one or more resources were defined */
#if (RESOURCES_COUNT != 0)
		{
			/* \req OSEK_SYS_3.13.1 This call serves to enter critical sections in
			 * the code that are assigned to the resource referenced by ResID */
			if ( TasksVar[GetRunningTask()].ActualPriority < ResourcesPriority[ResID])
			{
				TasksVar[GetRunningTask()].ActualPriority = ResourcesPriority[ResID];
			}

			/* mark resource as set */
			TasksVar[GetRunningTask()].Resources |= ( 1 << ResID );
		}
#endif /* #if (RESOURCES_COUNT != 0) */

		IntSecure_End();
	}

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
		(HOOK_ERRORHOOK == OSEK_ENABLE) )
	/* \req OSEK_ERR_1.3-6/xx The ErrorHook hook routine shall be called if a
	 * system service returns a StatusType value not equal to E_OK.*/
	/* \req OSEK_ERR_1.3.1-6/xx The hook routine ErrorHook is not called if a
	 * system service is called from the ErrorHook itself. */
   else if ( ( ErrorHookRunning != 1 ) )
	{
		SetError_Api(OSServiceId_GetResource);
		SetError_Param1(ResID);
		SetError_Ret(ret);
		SetError_Msg("GetResource returns != E_OK");
		SetError_ErrorHook();
	}
	else
	{
		/* nothing to do */
	}
#endif

	return ret;
}
#endif /* #if ( (NO_RES_SCHEDULER == OSEK_DISABLE) || (RESOURCES_COUNT != 0) ) */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


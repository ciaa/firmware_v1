/* Copyright 2008, 2009, 2014 Mariano Cerdeiro
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

/** \brief FreeOSEK Os ClearEvent Implementation File
 **
 ** This file implements the ClearEvent API
 **
 ** \file ClearEvent.c
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
 * 20141109 v0.1.6 MaCe ClearEvent shall E_OS_CALLEVEL is called from any
 *                      context != task
 * 20090330 v0.1.3 MaCe add NO_EVENTS evaluation
 * 20090128 v0.1.2 MaCe add OSEK_MEMMAP check
 * 20081113 v0.1.1 KLi  Added memory layout attribute macros
 * 20080814 v0.1.0 MaCe	initial version
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
#if (NO_EVENTS == OSEK_DISABLE)
StatusType ClearEvent
(
   EventMaskType Mask
)
{
   /* \req OSEK_SYS_3.16 The system service StatusType
    * ClearEvent ( EventMaskType Mask ) shall be defined */

   /* \req OSEK_SYS_3.16.2 Possible return values in Standard mode is E_OK */
   StatusType ret = E_OK;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if ( GetCallingContext() != CONTEXT_TASK )
   {
      /* \req OSEK_SYS_3.16.3-2/2 Extra possible return values in Extended
       * mode are E_OS_ACCESS, E_OS_CALLEVEL */
      ret = E_OS_CALLEVEL;
   }
   else if ( !TasksConst[GetRunningTask()].ConstFlags.Extended )
   {
      /* \req OSEK_SYS_3.16.3-1/2 Extra possible return values in Extended
       * mode are E_OS_ACCESS, E_OS_CALLEVEL */
      ret = E_OS_ACCESS;
   }
   else
#endif
   {
      /* enter to critical code */
      IntSecure_Start();

      /* \req OSEK_SYS_3.16.1 The events of the extended task calling ClearEvent
       * are cleared according to the event mask Mask */
      TasksVar[GetRunningTask()].Events &=
         (EventMaskType)~( Mask & TasksConst[GetRunningTask()].EventsMask );

      /* finish cirtical code */
      IntSecure_End();

   }

#if ( (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED) && \
      (HOOK_ERRORHOOK == OSEK_ENABLE) )
   /* \req OSEK_ERR_1.3-9/xx The ErrorHook hook routine shall be called if a
    * system service returns a StatusType value not equal to E_OK.*/
   /* \req OSEK_ERR_1.3.1-9/xx The hook routine ErrorHook is not called if a
    * system service is called from the ErrorHook itself. */
   if ( ( ret != E_OK ) && (ErrorHookRunning != 1))
   {
      SetError_Api(OSServiceId_ClearEvent);
      SetError_Param1(Mask);
      SetError_Ret(ret);
      SetError_Msg("ClearEvent returns != than E_OK");
      SetError_ErrorHook();
   }
#endif

   return ret;
}
#endif /* #if (NO_EVENTS == OSEK_DISABLE) */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


/* Copyright 2015, Diego Ezequiel Vommaro
 * Copyright 2015, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief This file implements the public interface of the Rtcs tool
 **
 ** This file implements the main functionality of the Rtcs tool
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementation
 ** @{ */

/*==================[inclusions]=============================================*/
#include "Rtcs_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern int8_t Rtcs_Init(void)
{
   int8_t i;
   uint32_t ret;

   if(Rtcs_state == UNINITIALIZED)
   {
      /* First execution of the all controllers */
      for (i = 0; i < CONTROLLERS_LIST_SIZE; i++)
      {
        Rtcs_controllers_list[i]->ControllerFirstRunFunc(Rtcs_controllers_list[i]->data);
      }

      /* Tool state changes to Active */
      Rtcs_state = ACTIVE;

      /* The return vale is OK */
      ret = RTCS_STATE_OK;
   }
   else
   {
      /* The current state is incorrect */
      ret = RTCS_STATE_ERROR;
   }

   return ret;
}

extern int8_t Rtcs_Start(void)
{
   int8_t ret;

   if(Rtcs_state == INACTIVE)
   {
      /* Activation of the Rtcs tool */
      Rtcs_state = ACTIVE;

      /* The return value is OK */
      ret = RTCS_STATE_OK;
   }
   else
   {
      /* The current state is incorrect */
      ret = RTCS_STATE_ERROR;
   }

   return ret;
}

extern int8_t Rtcs_Stop(void)
{
   int8_t ret;

   if(Rtcs_state == ACTIVE)
   {
      /* Deactivation of the Rtcs tool*/
      Rtcs_state = INACTIVE;

      /* The return value is OK */
      ret = RTCS_STATE_OK;
   }
   else
   {
      /* The current state is incorrect */
      ret = RTCS_STATE_ERROR;
   }

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief CIAA Devices source file
 **
 ** This header file describes the Devices.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */
/*
 * Initials     Name
 * ---------------------------
 * EzEs         Ezequiel Esposito
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140503 v0.0.2 MaCe implement all functions
 * 20140420 v0.0.1 EzEs initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDevices.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdbool.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdio.h"

/*==================[macros and definitions]=================================*/

/*==================[typedef]================================================*/
/** \brief Devices type */
typedef struct {
   ciaaDevices_deviceType * device[ciaaDevices_MAXDEVICES];
   uint8_t position;
} ciaaDevices_devicesType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief List of devices */
static ciaaDevices_devicesType ciaaDevices;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaDevices_init(void)
{
   /* reset position of the devices */
   ciaaDevices.position = 0;
}

extern void ciaaDevices_addDevice(ciaaDevices_deviceType * device)
{
   /* enter critical section */
   /* not needed, only 1 task running */

   /* check if positions are empty for more devices */
   if (ciaaDevices.position < (uint8_t)ciaaDevices_MAXDEVICES)
   {
      /* store the device in the list */
      ciaaDevices.device[ciaaDevices.position] = device;

      /* increment the device position */
      ciaaDevices.position++;
   }

   /* exit critical section */
   /* not needed, only 1 task running */
}

extern ciaaDevices_deviceType * ciaaDevices_getDevice(char const * const path)
{
   bool found = false;
   ciaaDevices_deviceType * ret = NULL;
   uint8_t device;

   /* search over all devices */
   for(device = 0; (device < ciaaDevices.position) && !found; device++)
   {
      /* if the same path is found */
      if (ciaaPOSIX_strncmp(path, ciaaDevices.device[device]->path,
               ciaaPOSIX_strlen(ciaaDevices.device[device]->path)) == 0)
      {
         /* return the device */
         ret = ciaaDevices.device[device];

         /* break the loop */
         found = true;
      }
   }

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


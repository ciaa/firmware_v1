/* Copyright 2014, 2015, Mariano Cerdeiro
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

/** \brief CIAA DIO Devices
 **
 ** Provides support for DIO devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140525 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDioDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_assert.h"
#include "ciaak.h"            /* <= ciaa kernel header */
#include "os.h"

/*==================[macros and definitions]=================================*/
#define ciaaDioDevices_MAXDEVICES   20

/*==================[typedef]================================================*/
typedef struct {
   ciaaDevices_deviceType const * device;
} ciaaDioDevices_deviceType;

/** \brief Dio Devices Type */
typedef struct {
   ciaaDioDevices_deviceType devstr[ciaaDioDevices_MAXDEVICES];
   uint8_t position;
} ciaaDioDevices_devicesType;

/*==================[internal data declaration]==============================*/
/** \brief List of devices */
ciaaDioDevices_devicesType ciaaDioDevices;

/** \brief Prefix of all Dio devices
 *
 * This prefix is used for all drevices which drivers are register against this
 * device.
 *
 * Eg. if a driver is 1 and it registers to this device type (/dev/dio)
 * the resulting path will be /dev/dio/1
 */
char const * const ciaaDioDevices_prefix = "/dev/dio";

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaDioDevices_init(void)
{
   /* reset position of the drivers */
   ciaaDioDevices.position = 0;
}

extern void ciaaDioDevices_addDriver(ciaaDevices_deviceType * driver)
{
   ciaaDevices_deviceType * newDevice;
   char * newDeviceName;
   uint8_t length;
   uint8_t position;

   /* enter critical section */
   /* not needed, only 1 task running */

   /* check if more drivers can be added */
   if (ciaaDioDevices_MAXDEVICES > ciaaDioDevices.position)
   {
      /* get position for nexxt device */
      position = ciaaDioDevices.position;

      /* increment position for next device */
      ciaaDioDevices.position++;

      /* exit critical section */
      /* not needed, only 1 task running */

      /* add driver */
      ciaaDioDevices.devstr[position].device = driver;

      /* allocate memory for new device */
      newDevice = (ciaaDevices_deviceType*) ciaak_malloc(sizeof(ciaaDevices_deviceType));

      /* set functions for this device */
      newDevice->open = ciaaDioDevices_open;
      newDevice->close = ciaaDioDevices_close;
      newDevice->ioctl = ciaaDioDevices_ioctl;
      newDevice->read = ciaaDioDevices_read;
      newDevice->write = ciaaDioDevices_write;

      /* store layers information information */
      newDevice->layer = (void*) &ciaaDioDevices.devstr[position];
      newDevice->loLayer = (void*) driver;

      /* store newDevice layer information in the lower layer */
      driver->upLayer = newDevice;

      /* create path string for this device */
      length = ciaaPOSIX_strlen(driver->path);
      length += ciaaPOSIX_strlen(ciaaDioDevices_prefix);
      length += 2; /* for the / and the termination null */

      /* create path for the new device */
      newDeviceName = (char*) ciaak_malloc(length);

      /* start a new string */
      *newDeviceName = (char)0;

      /* add prefix, / and the device name */
      /* return value of strcat is always the first parameter and can be ignored */
      (void)ciaaPOSIX_strcat(newDeviceName, ciaaDioDevices_prefix);
      (void)ciaaPOSIX_strcat(newDeviceName, "/");
      (void)ciaaPOSIX_strcat(newDeviceName, driver->path);
      /* add path to device structure */
      newDevice->path = newDeviceName;

      /* add device */
      ciaaDevices_addDevice(newDevice);
   }
   else
   {
      /* exit critical section */
      /* not needed, only 1 task running */
   }
}

extern ciaaDevices_deviceType * ciaaDioDevices_open(char const * path,
      ciaaDevices_deviceType * device,
      uint8_t const oflag)
{
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   /* serial devices does not support that the drivers update the device */
   /* the returned device shall be the same as passed */
   ciaaPOSIX_assert(drv->open(path, drv, oflag) == drv);

   return device;
}

extern int32_t ciaaDioDevices_close(ciaaDevices_deviceType const * const device)
{
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   return drv->close(drv);
}

extern int32_t ciaaDioDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param)
{
   int32_t ret;
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   ret = drv->ioctl(drv, request, param);

   return ret;
}

extern ssize_t ciaaDioDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte)
{
   ssize_t ret;
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   ret = drv->read(drv, buf, nbyte);

   return ret;
}

extern ssize_t ciaaDioDevices_write(ciaaDevices_deviceType const * const device, uint8_t const * buf, size_t nbyte)
{
   ssize_t ret;
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   ret = drv->write(drv, buf, nbyte);

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


/* Copyright 2015, Juan Pablo Vecchio, Joaquin Jorge Rodriguez
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

/** \brief CIAA GPIO Devices
 **
 ** Provides support for GPIO devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JPV         Juan Pablo Vecchio
 * JJR         Joaquin Jorge Rodriguez
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150612 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaKeyboardDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_assert.h"
#include "ciaak.h"            /* <= ciaa kernel header */
#include "os.h"

/*==================[macros and definitions]=================================*/
#define ciaaKeyboardDevices_MAXDEVICES   20       /* Actually, we just create one keyboard device */

/*==================[typedef]================================================*/
typedef struct {
   ciaaDevices_deviceType const * device;
} ciaaKeyboardDevices_deviceType;

/** \brief Keyboard Devices Type */
typedef struct {
   ciaaKeyboardDevices_deviceType devstr[ciaaKeyboardDevices_MAXDEVICES];
   uint8_t position;
} ciaaKeyboardDevices_devicesType;

/*==================[internal data declaration]==============================*/
/** \brief List of devices */
ciaaKeyboardDevices_devicesType ciaaKeyboardDevices;

/** \brief Prefix of all Keyboard devices
 *
 * This prefix is used for all devices which drivers are register against this
 * device.
 *
 * Eg. if a driver is 1 and it registers to this device type (/dev/keyb)
 * the resulting path will be /dev/keyb/1
 */
char const * const ciaaKeyboardDevices_prefix = "/dev/keyb";

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaKeyboardDevices_init(void)
{
   /* reset position of the drivers */
   ciaaKeyboardDevices.position = 0;
}

extern void ciaaKeyboardDevices_addDriver(ciaaDevices_deviceType * driver)
{
   ciaaDevices_deviceType * newDevice;
   char * newDeviceName;
   uint8_t length;
   uint8_t position;

   /* enter critical section */
   /* not needed, only 1 task running */

   /* check if more drivers can be added */
   if (ciaaKeyboardDevices_MAXDEVICES > ciaaKeyboardDevices.position)
   {
      /* get position for nexxt device */
      position = ciaaKeyboardDevices.position;

      /* increment position for next device */
      ciaaKeyboardDevices.position++;

      /* exit critical section */
      /* not needed, only 1 task running */

      /* add driver */
      ciaaKeyboardDevices.devstr[position].device = driver;

      /* allocate memory for new device */
      newDevice = (ciaaDevices_deviceType*) ciaak_malloc(sizeof(ciaaDevices_deviceType));

      /* set functions for this device */
      newDevice->open = ciaaKeyboardDevices_open;
      newDevice->close = ciaaKeyboardDevices_close;
      newDevice->ioctl = ciaaKeyboardDevices_ioctl;
      newDevice->read = ciaaKeyboardDevices_read;
      newDevice->write = ciaaKeyboardDevices_write;

      /* store layers information information */
      newDevice->layer = (void*) &ciaaKeyboardDevices.devstr[position];
      newDevice->loLayer = (void*) driver;

      /* store newDevice layer information in the lower layer */
      driver->upLayer = newDevice;

      /* create path string for this device */
      length = ciaaPOSIX_strlen(driver->path);
      length += ciaaPOSIX_strlen(ciaaKeyboardDevices_prefix);
      length += 2; /* for the / and the termination null */

      /* create path for the new device */
      newDeviceName = (char*) ciaak_malloc(length);

      /* start a new string */
      *newDeviceName = (char)0;

      /* add prefix, / and the device name */
      /* return value of strcat is always the first parameter and can be ignored */
      (void)ciaaPOSIX_strcat(newDeviceName, ciaaKeyboardDevices_prefix);
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

extern ciaaDevices_deviceType * ciaaKeyboardDevices_open(char const * path,
      ciaaDevices_deviceType * device,
      uint8_t const oflag)
{
   ciaaPOSIX_assert(
         device->open(path, (ciaaDevices_deviceType *)device->loLayer, oflag)
         == device->loLayer);

   return device;
}

extern int32_t ciaaKeyboardDevices_close(ciaaDevices_deviceType const * const device)
{
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   return drv->close(drv);
}

extern int32_t ciaaKeyboardDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param)
{
   int32_t ret;
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   ret = drv->ioctl(drv, request, param);

   return ret;
}

extern uint16_t ciaaKeyboardDevices_read(ciaaDevices_deviceType const * const device, uint16_t * const buf, size_t const nbyte)
{
	uint16_t ret;
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   ret = drv->read(drv, buf, nbyte);

   return ret;
}

extern uint16_t ciaaKeyboardDevices_write(ciaaDevices_deviceType const * const device, uint16_t const * buf, size_t nbyte)
{
	uint16_t ret;
   ciaaDevices_deviceType * drv = (ciaaDevices_deviceType*) device->loLayer;

   ret = drv->write(drv, buf, nbyte);

   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


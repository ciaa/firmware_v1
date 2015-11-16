/* Copyright 2015, Mariano Cerdeiro
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

/** \brief CIAA Block Devices
 **
 ** Provides support for block devices
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
 * 20150118 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_errno.h"
#include "ciaaLibs_CircBuf.h"
#include "ciaak.h"       /* <= ciaa kernel header */
#include "os.h"

/*==================[macros and definitions]=================================*/
#define ciaaBlockDevices_MAXDEVICES          20

/*==================[typedef]================================================*/
typedef struct {
   TaskType taskID;
   void * fct;
} ciaaBlockDevices_blockerType;

typedef struct {
   ciaaDevices_deviceType const * device;
   ciaaBlockDevices_blockerType blocked;
   uint8_t flags;
} ciaaBlockDevices_deviceType;

/** \brief Block Devices Type */
typedef struct {
   ciaaBlockDevices_deviceType devstr[ciaaBlockDevices_MAXDEVICES];
   uint8_t position;
} ciaaBlockDevices_devicesType;

/*==================[internal data declaration]==============================*/
/** \brief List of devices */
ciaaBlockDevices_devicesType ciaaBlockDevices;

/** \brief Prefix of all block devices
 *
 * This prefix is used for all drevices which drivers are register against this
 * device.
 *
 * Eg. if a driver is sd/1 and it registers to this device type (/dev/block)
 * the resulting path will be /dev/block/sd/1
 */
char const * const ciaaBlockDevices_prefix = "/dev/block";

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaBlockDevices_init(void)
{
   int32_t loopi = 0;

   /* reset position of the drivers */
   ciaaBlockDevices.position = 0;

   /* init the device structure */
   for(loopi = 0; ciaaBlockDevices_MAXDEVICES > loopi; loopi++)
   {
      /* set invalid task as default */
      ciaaBlockDevices.devstr[loopi].blocked.taskID = 255; /* TODO */
      ciaaBlockDevices.devstr[loopi].blocked.fct = NULL;
   }
}

extern void ciaaBlockDevices_addDriver(ciaaDevices_deviceType * driver)
{
   ciaaDevices_deviceType * newDevice;
   char * newDeviceName;
   uint8_t length;
   uint8_t position;

   /* enter critical section */
   /* not needed, only 1 task running */

   /* check if more drivers can be added */
   if (ciaaBlockDevices_MAXDEVICES > ciaaBlockDevices.position) {

      /* get position for next device */
      position = ciaaBlockDevices.position;

      /* increment position for next device */
      ciaaBlockDevices.position++;

      /* exit critical section */
      /* not needed, only 1 task running */

      /* add driver */
      ciaaBlockDevices.devstr[position].device = driver;

      /* initial flags */
      ciaaBlockDevices.devstr[position].flags = 0;

      /* allocate memory for new device */
      newDevice = (ciaaDevices_deviceType*) ciaak_malloc(sizeof(ciaaDevices_deviceType));

      /* set functions for this device */
      newDevice->open = ciaaBlockDevices_open;
      newDevice->close = ciaaBlockDevices_close;
      newDevice->ioctl = ciaaBlockDevices_ioctl;
      newDevice->read = ciaaBlockDevices_read;
      newDevice->write = ciaaBlockDevices_write;
      newDevice->lseek = ciaaBlockDevices_lseek;

      /* store layers information information */
      newDevice->layer = (void *) &ciaaBlockDevices.devstr[position];
      newDevice->loLayer = (void *) driver;

      /* store newDevice layer information in the lower layer */
      driver->upLayer = newDevice;

      /* create path string for this device */
      length = ciaaPOSIX_strlen(driver->path);
      length += ciaaPOSIX_strlen(ciaaBlockDevices_prefix);
      length += 2; /* for the / and the termination null */

      /* create path for the new device */
      newDeviceName = (char *) ciaak_malloc(length);

      /* start a new string */
      *newDeviceName = 0;

      /* add prefix, / and the device name */
      ciaaPOSIX_strcat(newDeviceName, ciaaBlockDevices_prefix);
      ciaaPOSIX_strcat(newDeviceName, "/");
      ciaaPOSIX_strcat(newDeviceName, driver->path);
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

extern ciaaDevices_deviceType * ciaaBlockDevices_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;

   /* block devices does not support that the drivers update the device */
   /* the returned device shall be the same as passed */
   ciaaPOSIX_assert(blockDevice->device->open(path, (ciaaDevices_deviceType *)device->loLayer, oflag) == device->loLayer);

   return device;
}

extern int32_t ciaaBlockDevices_close(ciaaDevices_deviceType const * const device)
{
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType *) device->layer;

   return blockDevice->device->close((ciaaDevices_deviceType *)device->loLayer);
}

extern off_t ciaaBlockDevices_lseek(ciaaDevices_deviceType const * const device, off_t const offset, uint8_t const whence)
{
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType *) device->layer;

   return blockDevice->device->lseek((ciaaDevices_deviceType *)device->loLayer, offset, whence);
}

extern int32_t ciaaBlockDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param)
{
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType *) device->layer;
   int32_t ret = 0;

   switch(request)
   {
      default:
         ret = blockDevice->device->ioctl(device->loLayer, request, param);
         break;
   }
   return ret;
}

extern ssize_t ciaaBlockDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;
   ssize_t ret = 0;

   /* trigger read data from lower layer */
   ret = blockDevice->device->read(device->loLayer, buf, nbyte);

   /* if no bytes have been read, wait for them */
   if (0 == ret)
   {
      /* get task id and function for waking up the task later */
      GetTaskID(&blockDevice->blocked.taskID);
      blockDevice->blocked.fct = (void*) ciaaBlockDevices_read;

      /* if no data wait for it */
#ifdef POSIXE
      WaitEvent(POSIXE);
      ClearEvent(POSIXE);
#else
#endif

      /* TODO improve this */
      /* for the first impl. the driver shall return as much bytes as requested */
      ret = nbyte;
   }

   return ret;
}

extern ssize_t ciaaBlockDevices_write(ciaaDevices_deviceType const * const device, uint8_t const * buf, size_t const nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;
   ssize_t ret = 0;

   /* trigger write data from lower layer */
   ret = blockDevice->device->write(device->loLayer, buf, nbyte);

   /* if no bytes couldnt be written, wait to complete the write */
   if (0 == ret)
   {
      /* get task id and function for waking up the task later */
      GetTaskID(&blockDevice->blocked.taskID);
      blockDevice->blocked.fct = (void*) ciaaBlockDevices_read;

      /* if no data wait for it */
#ifdef POSIXE
      WaitEvent(POSIXE);
      ClearEvent(POSIXE);
#else
#endif

      /* TODO improve this */
      /* for the first impl. the driver shall return as much bytes as requested */
      ret = nbyte;
   }

   return ret;
}

extern void ciaaBlockDevices_writeConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;

   TaskType taskID = blockDevice->blocked.taskID;

   if ( (255 != taskID) &&
        ( (void*)ciaaBlockDevices_write == blockDevice->blocked.fct) )
   {
      /* invalidate task id */
      blockDevice->blocked.taskID = 255; /* TODO add macro */
      /* reset blocked function */
      blockDevice->blocked.fct = NULL;

#ifdef POSIXE
      /* set task event */
      SetEvent(taskID, POSIXE);
#else
#endif
   } else {
      /* this shall not happens */
      ciaaPOSIX_assert(0);
   }
}

extern void ciaaBlockDevices_readIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;

   TaskType taskID = blockDevice->blocked.taskID;

   if ( (255 != taskID) &&
        ( (void*)ciaaBlockDevices_read == blockDevice->blocked.fct) )
   {
      /* invalidate task id */
      blockDevice->blocked.taskID = 255; /* TODO add macro */
      /* reset blocked function */
      blockDevice->blocked.fct = NULL;

#ifdef POSIXE
      /* set task event */
      SetEvent(taskID, POSIXE);
#else
#endif
   } else {
      /* this shall not happens */
      ciaaPOSIX_assert(0);
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


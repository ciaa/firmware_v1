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

   if(oflag & O_NONBLOCK)
   {
      blockDevice->flags |= ciaaBlockDevices_NONBLOCK_MODE;
   }
   return device;
}

extern int32_t ciaaBlockDevices_close(ciaaDevices_deviceType const * const device)
{
   return device->close((ciaaDevices_deviceType *)device->loLayer);
}

extern int32_t ciaaBlockDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param)
{
   int32_t ret = 0;

   ciaaBlockDevices_deviceType * blockDevice = (ciaaBlockDevices_deviceType *) device->layer;

   switch(request)
   {
      case ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE:
         if((bool)(intptr_t)param == false)
         {
            /* Blocking mode */
            blockDevice->flags &= ~ciaaBlockDevices_NONBLOCK_MODE;
         }
         else
         {
            /* NonBlocking Mode */
            blockDevice->flags |= ciaaBlockDevices_NONBLOCK_MODE;
         }
         ret = 0;
         break;

      default:
         ret = blockDevice->device->ioctl(device->loLayer, request, param);
         break;
   }
   return ret;
}

extern int32_t ciaaBlockDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, uint32_t nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;
   int32_t ret = 0;

   /* read data from lower layer */
   /* TODO */

   /* get task id and function for waking up the task later */
   GetTaskID(&blockDevice->blocked.taskID);
   blockDevice->blocked.fct = (void*) ciaaBlockDevices_read;

   /* if no data wait for it */
#ifdef POSIXE
   WaitEvent(POSIXE);
   ClearEvent(POSIXE);
#endif

   return ret;
}

extern int32_t ciaaBlockDevices_write(ciaaDevices_deviceType const * const device, uint8_t const * buf, uint32_t nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;
   int32_t ret = 0;
   int32_t total = 0;
   ciaaLibs_CircBufType * cbuf = &blockDevice->txBuf;
   int32_t head;
   uint32_t space;

   do
   {
      /* read head and space */
      head = cbuf->head;
      space = ciaaLibs_circBufSpace(cbuf, head);

      /* put bytes in the queue */
      ret = ciaaLibs_circBufPut(cbuf, buf, ciaaLibs_min(nbyte-total, space));
      /* update total of written bytes */
      total += ret;

      /* starts the transmission if not already ongoing */
      blockDevice->device->ioctl(
            device->loLayer,
            ciaaPOSIX_IOCTL_STARTTX,
            NULL);

      /* if not all bytes could be stored in the buffer */
      if (total < nbyte)
      {
         /* increment buffer */
         buf += ret;

         /* set the task to sleep until some data have been send */

         /* TODO improve this: https://github.com/ciaa/Firmware/issues/88 */
         blockDevice->device->ioctl(device->loLayer, ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT, (void*)false);
         /* get task id and function for waking up the task later */
         GetTaskID(&blockDevice->blocked.taskID);
         blockDevice->blocked.fct = (void*) ciaaBlockDevices_write;

         /* TODO improve this: https://github.com/ciaa/Firmware/issues/88 */
         blockDevice->device->ioctl(device->loLayer, ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT, (void*)true);
         /* wait to write all data or for the txConfirmation */
#ifdef POSIXE
         WaitEvent(POSIXE);
         ClearEvent(POSIXE);
#endif
      }
   }
   while (total < nbyte);

   return total;
}

extern void ciaaBlockDevices_txConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;
   uint32_t write = 0;
   ciaaLibs_CircBufType * cbuf = &blockDevice->txBuf;
   uint32_t tail = cbuf->tail;
   uint32_t rawCount = ciaaLibs_circBufRawCount(cbuf, tail);
   uint32_t count = ciaaLibs_circBufCount(cbuf, tail);
   TaskType taskID = blockDevice->blocked.taskID;

   /* if some data have to be transmitted */
   if (count > 0)
   {
      /* write data to the driver */
      write = blockDevice->device->write(device->loLayer, ciaaLibs_circBufReadPos(cbuf), rawCount);

      /* update buffer */
      ciaaLibs_circBufUpdateHead(cbuf, write);

      /* if all bytes were written and more data is available */
      if ( (write == rawCount) && (count > rawCount ) )
      {
         /* re calculate rawCount */
         rawCount = ciaaLibs_circBufRawCount(cbuf, tail);

         /* write more bytes */
         write = blockDevice->device->write(device->loLayer,
               ciaaLibs_circBufReadPos(cbuf), rawCount);

         if (write > 0)
         {
            /* update buffer */
            ciaaLibs_circBufUpdateHead(cbuf, write);
         }
      }
      /* if task is blocked and waiting for reception of this device */
      if ( (255 != taskID) &&
            (blockDevice->blocked.fct ==
             (void*) ciaaBlockDevices_write) )
      {
         /* invalidate task id */
         blockDevice->blocked.taskID = 255; /* TODO add a macro */

         /* reset function */
         blockDevice->blocked.fct = NULL;

         /* set task event */
#ifdef POSIXE
         SetEvent(taskID, POSIXE);
#endif
      }
   }
}

extern void ciaaBlockDevices_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* get block device */
   ciaaBlockDevices_deviceType * blockDevice =
      (ciaaBlockDevices_deviceType*) device->layer;
   ciaaLibs_CircBufType * cbuf = &blockDevice->rxBuf;
   uint32_t head = cbuf->head;
   uint32_t rawSpace = ciaaLibs_circBufRawSpace(cbuf, head);
   uint32_t space = ciaaLibs_circBufSpace(cbuf, head);
   uint32_t read = 0;
   TaskType taskID = blockDevice->blocked.taskID;

   read = blockDevice->device->read(device->loLayer, ciaaLibs_circBufWritePos(cbuf), rawSpace);

   /* if rawSpace is full but more space is avaialble */
   if ((read == rawSpace) && (space > rawSpace))
   {
      read += blockDevice->device->read(
            device->loLayer,
            &cbuf->buf[0],
            space - rawSpace);
   }
   else
   {
      if ((read == rawSpace) && (space <= rawSpace))
      {
         /* data may be lost because not place on the receive buffer */
         /* TODO */
      }
      else
      {
         /* read less bytes than provided */
         /* nothing to do */
      }
   }

   /* update tail */
   ciaaLibs_circBufUpdateTail(cbuf, read);

   /* if data has been read */
   if ( (0 < read) && (255 != taskID) &&
         (blockDevice->blocked.fct == (void*) ciaaBlockDevices_read ) )
   {
      /* invalidate task id */
      blockDevice->blocked.taskID = 255; /* TODO add macro */

      /* reset blocked function */
      blockDevice->blocked.fct = NULL;

#ifdef POSIXE
      /* set task event */
      SetEvent(taskID, POSIXE);
#endif
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


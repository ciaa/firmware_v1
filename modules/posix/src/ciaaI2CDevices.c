/* Copyright 2014, 2015 Mariano Cerdeiro
 * Copyright 2014, Juan Cecconi
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

/** \brief CIAA I2C Devices
 **
 ** Provides support for I2C devices
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
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140525 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaI2CDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_errno.h"
#include "ciaaLibs_CircBuf.h"
#include "ciaak.h"       /* <= ciaa kernel header */
#include "os.h"

/*==================[macros and definitions]=================================*/
#define ciaaI2CDevices_MAXDEVICES          20
#define ciaaI2CDevices_NONBLOCK_MODE       0x01

/*==================[typedef]================================================*/
typedef struct {
   TaskType taskID;
   void * fct;
} ciaaI2CDevices_blockerType;

typedef struct {
   ciaaDevices_deviceType const * device;
   ciaaI2CDevices_blockerType blocked;
   ciaaLibs_CircBufType rxBuf;
   ciaaLibs_CircBufType txBuf;
   uint8_t flags;
} ciaaI2CDevices_deviceType;

/** \brief I2C Devices Type */
typedef struct {
   ciaaI2CDevices_deviceType devstr[ciaaI2CDevices_MAXDEVICES];
   uint8_t position;
} ciaaI2CDevices_devicesType;

/*==================[internal data declaration]==============================*/
/** \brief List of devices */
ciaaI2CDevices_devicesType ciaaI2CDevices;

/** \brief Prefix of all I2C devices
 *
 * This prefix is used for all drevices which drivers are register against this
 * device.
 *
 * Eg. if a driver is uart/1 and it registers to this device type (/dev/I2C)
 * the resulting path will be /dev/I2C/uart/1
 */
char const * const ciaaI2CDevices_prefix = "/dev/I2C";

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaI2CDevices_init(void)
{
   int32_t loopi = 0;

   /* reset position of the drivers */
   ciaaI2CDevices.position = 0;

   /* init the device structure */
   for(loopi = 0; ciaaI2CDevices_MAXDEVICES > loopi; loopi++)
   {
      /* set invalid task as default */
      ciaaI2CDevices.devstr[loopi].blocked.taskID = 255; /* TODO */
      ciaaI2CDevices.devstr[loopi].blocked.fct = NULL;
   }
}

extern void ciaaI2CDevices_addDriver(ciaaDevices_deviceType * driver)
{
   ciaaDevices_deviceGroupType * newDevice;
   char * newDeviceName;
   uint8_t length;
   uint8_t position;

   /* enter critical section */
   /* not needed, only 1 task running */

   /* check if more drivers can be added */
   if (ciaaI2CDevices_MAXDEVICES > ciaaI2CDevices.position) {

      /* get position for next device */
      position = ciaaI2CDevices.position;

      /* increment position for next device */
      ciaaI2CDevices.position++;

      /* exit critical section */
      /* not needed, only 1 task running */

      /* add driver */
      ciaaI2CDevices.devstr[position].device = driver;

      /* configure rx and tx buffers */
      ciaaLibs_circBufInit(&ciaaI2CDevices.devstr[position].rxBuf, ciaak_malloc(64), 64);
      ciaaLibs_circBufInit(&ciaaI2CDevices.devstr[position].txBuf, ciaak_malloc(64), 64);

      /* initial flags */
      ciaaI2CDevices.devstr[position].flags = 0;

      /* allocate memory for new device */
      newDevice = (ciaaDevices_deviceGroupType*) ciaak_malloc(sizeof(ciaaDevices_deviceGroupType));

      /* set functions for this device */
      newDevice->open = ciaaI2CDevices_open;
      newDevice->close = ciaaI2CDevices_close;
      newDevice->ioctl = ciaaI2CDevices_ioctl;
      newDevice->read = ciaaI2CDevices_read;
      newDevice->write = ciaaI2CDevices_write;

      /* store layers information information */
      newDevice->layer = (void *) &ciaaI2CDevices.devstr[position];
      newDevice->loLayer = (void *) driver;

      /* store newDevice layer information in the lower layer */
      driver->upLayer = newDevice;

      /* create path string for this device */
      length = ciaaPOSIX_strlen(driver->path);
      length += ciaaPOSIX_strlen(ciaaI2CDevices_prefix);
      length += 2; /* for the / and the termination null */

      /* create path for the new device */
      newDeviceName = (char *) ciaak_malloc(length);

      /* start a new string */
      *newDeviceName = 0;

      /* add prefix, / and the device name */
      ciaaPOSIX_strcat(newDeviceName, ciaaI2CDevices_prefix);
      ciaaPOSIX_strcat(newDeviceName, "/");
      ciaaPOSIX_strcat(newDeviceName, driver->path);
      /* add path to device structure */
      newDevice->path = newDeviceName;

      /* add device */
      ciaaDevices_addDeviceGroup(newDevice);
   }
   else
   {
      /* exit critical section */
      /* not needed, only 1 task running */
   }
}

extern ciaaDevices_deviceType * ciaaI2CDevices_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaI2CDevices_deviceType * I2CDevice =
      (ciaaI2CDevices_deviceType*) device->layer;

   /* I2C devices does not support that the drivers update the device */
   /* the returned device shall be the same as passed */
   ciaaPOSIX_assert(I2CDevice->device->open(path, (ciaaDevices_deviceType *)device->loLayer, oflag) == device->loLayer);

   if(oflag & O_NONBLOCK)
   {
      I2CDevice->flags |= ciaaI2CDevices_NONBLOCK_MODE;
   }
   return device;
}

extern int32_t ciaaI2CDevices_close(ciaaDevices_deviceType const * const device)
{
   return device->close((ciaaDevices_deviceType *)device->loLayer);
}

extern int32_t ciaaI2CDevices_ioctl(ciaaDevices_deviceType const * const device, int32_t request, void* param)
{
   int32_t ret = 0;

   ciaaI2CDevices_deviceType * I2CDevice = (ciaaI2CDevices_deviceType *) device->layer;
   ciaaLibs_CircBufType * cbuf;
   uint32_t tail, head;

   switch(request)
   {
      case ciaaPOSIX_IOCTL_GET_RX_COUNT:
         cbuf = &I2CDevice->rxBuf;
         tail = cbuf->tail;
         *(uint32_t *)param = ciaaLibs_circBufCount(cbuf, tail);
         ret = 0;
         break;

      case ciaaPOSIX_IOCTL_GET_TX_SPACE:
         cbuf = &I2CDevice->txBuf;
         head = cbuf->head;
         *(uint32_t *)param = ciaaLibs_circBufSpace(cbuf, head);
         ret = 0;
         break;

      case ciaaPOSIX_IOCTL_RXINDICATION:
         break;

      case ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE:
         if((bool)(intptr_t)param == false)
         {
            /* Blocking mode */
            I2CDevice->flags &= ~ciaaI2CDevices_NONBLOCK_MODE;
         }
         else
         {
            /* NonBlocking Mode */
            I2CDevice->flags |= ciaaI2CDevices_NONBLOCK_MODE;
         }
         ret = 0;
         break;

      default:
         ret = I2CDevice->device->ioctl(device->loLayer, request, param);
         break;
   }
   return ret;
}

extern ssize_t ciaaI2CDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte)
{
   /* get I2C device */
   ciaaI2CDevices_deviceType * I2CDevice =
      (ciaaI2CDevices_deviceType*) device->layer;
   int32_t ret = 0;

   /* if the rx buffer is not empty */
   if (!ciaaLibs_circBufEmpty(&I2CDevice->rxBuf))
   {
      /* try to read nbyte from rxBuf and store it to the user buffer */
      ret = ciaaLibs_circBufGet(&I2CDevice->rxBuf,
            buf,
            nbyte);
   }
   else
   {
      /* There aren't data */
      if(I2CDevice->flags & ciaaI2CDevices_NONBLOCK_MODE)
      {
         /* We are in non blocking mode */
         /* We should do a blocking call...*/
         ciaaPOSIX_errno = EAGAIN; /* shall return -1 and set errno to [EAGAIN]. */
         ret = -1;
      }
      else
      {
         /* We are in blocking mode */
         /* TODO improve this: https://github.com/ciaa/Firmware/issues/88 */
         I2CDevice->device->ioctl(device->loLayer, ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT, (void*)false);

         /* get task id and function for waking up the task later */
         GetTaskID(&I2CDevice->blocked.taskID);
         I2CDevice->blocked.fct = (void*) ciaaI2CDevices_read;

         /* TODO improve this: https://github.com/ciaa/Firmware/issues/88 */
         I2CDevice->device->ioctl(device->loLayer, ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT, (void*)true);

         /* if no data wait for it */
#ifdef POSIXE
         WaitEvent(POSIXE);
         ClearEvent(POSIXE);
#endif

         /* after the wait is not needed to check if data is avaibale on the
          * buffer. The event will be set first after adding some data into it */

         /* try to read nbyte from rxBuf and store it to the user buffer */
         ret = ciaaLibs_circBufGet(&I2CDevice->rxBuf,
               buf,
               nbyte);
      }
   }
   return ret;
}

extern ssize_t ciaaI2CDevices_write(ciaaDevices_deviceType const * const device, uint8_t const * buf, size_t const nbyte)
{
   /* get I2C device */
   ciaaI2CDevices_deviceType * I2CDevice =
      (ciaaI2CDevices_deviceType*) device->layer;
   int32_t ret = 0;
   int32_t total = 0;
   ciaaLibs_CircBufType * cbuf = &I2CDevice->txBuf;
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
      I2CDevice->device->ioctl(
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
         I2CDevice->device->ioctl(device->loLayer, ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT, (void*)false);
         /* get task id and function for waking up the task later */
         GetTaskID(&I2CDevice->blocked.taskID);
         I2CDevice->blocked.fct = (void*) ciaaI2CDevices_write;

         /* TODO improve this: https://github.com/ciaa/Firmware/issues/88 */
         I2CDevice->device->ioctl(device->loLayer, ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT, (void*)true);
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

extern void ciaaI2CDevices_txConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* get I2C device */
   ciaaI2CDevices_deviceType * I2CDevice =
      (ciaaI2CDevices_deviceType*) device->layer;
   uint32_t write = 0;
   ciaaLibs_CircBufType * cbuf = &I2CDevice->txBuf;
   uint32_t tail = cbuf->tail;
   uint32_t rawCount = ciaaLibs_circBufRawCount(cbuf, tail);
   uint32_t count = ciaaLibs_circBufCount(cbuf, tail);
   TaskType taskID = I2CDevice->blocked.taskID;

   /* if some data have to be transmitted */
   if (count > 0)
   {
      /* write data to the driver */
      write = I2CDevice->device->write(device->loLayer, ciaaLibs_circBufReadPos(cbuf), rawCount);

      /* update buffer */
      ciaaLibs_circBufUpdateHead(cbuf, write);

      /* if all bytes were written and more data is available */
      if ( (write == rawCount) && (count > rawCount ) )
      {
         /* re calculate rawCount */
         rawCount = ciaaLibs_circBufRawCount(cbuf, tail);

         /* write more bytes */
         write = I2CDevice->device->write(device->loLayer,
               ciaaLibs_circBufReadPos(cbuf), rawCount);

         if (write > 0)
         {
            /* update buffer */
            ciaaLibs_circBufUpdateHead(cbuf, write);
         }
      }
      /* if task is blocked and waiting for reception of this device */
      if ( (255 != taskID) &&
            (I2CDevice->blocked.fct ==
             (void*) ciaaI2CDevices_write) )
      {
         /* invalidate task id */
         I2CDevice->blocked.taskID = 255; /* TODO add a macro */

         /* reset function */
         I2CDevice->blocked.fct = NULL;

         /* set task event */
#ifdef POSIXE
         SetEvent(taskID, POSIXE);
#endif
      }
   }
}

extern void ciaaI2CDevices_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* get I2C device */
   ciaaI2CDevices_deviceType * I2CDevice =
      (ciaaI2CDevices_deviceType*) device->layer;
   ciaaLibs_CircBufType * cbuf = &I2CDevice->rxBuf;
   uint32_t head = cbuf->head;
   uint32_t rawSpace = ciaaLibs_circBufRawSpace(cbuf, head);
   uint32_t space = ciaaLibs_circBufSpace(cbuf, head);
   uint32_t read = 0;
   TaskType taskID = I2CDevice->blocked.taskID;

   read = I2CDevice->device->read(device->loLayer, ciaaLibs_circBufWritePos(cbuf), rawSpace);

   /* if rawSpace is full but more space is avaialble */
   if ((read == rawSpace) && (space > rawSpace))
   {
      read += I2CDevice->device->read(
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
         (I2CDevice->blocked.fct == (void*) ciaaI2CDevices_read ) )
   {
      /* invalidate task id */
      I2CDevice->blocked.taskID = 255; /* TODO add macro */

      /* reset blocked function */
      I2CDevice->blocked.fct = NULL;

#ifdef POSIXE
      /* set task event */
      SetEvent(taskID, POSIXE);
#endif
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


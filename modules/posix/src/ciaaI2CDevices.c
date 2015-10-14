/* Copyright 2015, Sebastian Bedin para SUR Emprendimientos Tecnologicos S.R.L. / CONAE
 * Copyright 2015, Mariano Cerdeiro
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
 * SeBe         Sebastian Bedin
 * MaCe         Mariano Cerdeiro
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
   uint8_t registerAddWidth;
   uint32_t registerAdd;
} ciaaI2CDevices_masterDataType;

typedef struct {
   /* due to a rework some elements of this struct may not be needed */
   /* elements to review */
   ciaaDevices_deviceType const * device;
   ciaaI2CDevices_blockerType blocked;
   ciaaLibs_CircBufType rxBuf;
   ciaaLibs_CircBufType txBuf;
   ciaaI2CDevices_masterDataType masterData;

   uint8_t flags;

   /* confirmed elements */
   uint32_t curPos;     /* actual position on this device */
   uint32_t maxPos;     /* maximal position on this device */ /* TODO move to Flash */

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
 * Eg. if a driver is i2c/1 and it registers to this device type (/dev/i2c)
 * the resulting path will be /dev/i2c/1
 */
char const * const ciaaI2CDevices_prefix = "/dev/i2c";

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
      ciaaLibs_circBufInit(&ciaaI2CDevices.devstr[position].rxBuf, ciaak_malloc(256), 256);
      ciaaLibs_circBufInit(&ciaaI2CDevices.devstr[position].txBuf, ciaak_malloc(256), 256);

      /* initial flags */
      ciaaI2CDevices.devstr[position].flags = 0;

      /* allocate memory for new device */
      newDevice = (ciaaDevices_deviceGroupType*) ciaak_malloc(sizeof(ciaaDevices_deviceType));

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

   /* exit critical section */
   /* not needed, only 1 task running */

}

extern ciaaDevices_deviceType * ciaaI2CDevices_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaI2CDevices_deviceType * I2CDevice =
    (ciaaI2CDevices_deviceType*) device->layer;

   /* I2C devices does not support that the drivers update the device */
   /* the returned device shall be the same as passed */
   ciaaPOSIX_assert(I2CDevice->device->open(path, (ciaaDevices_deviceType *)device->loLayer, oflag) == device->loLayer);

   if(oflag & ciaaPOSIX_O_NONBLOCK)
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
   ciaaI2CDevices_masterDataType * pI2CMaster = (ciaaI2CDevices_masterDataType *) &I2CDevice->masterData;
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

      case ciaaPOSIX_IOCTL_SET_REGISTERADDWIDTH:
         pI2CMaster->registerAddWidth = (uint8_t)(intptr_t)param;
         ret = (int32_t)pI2CMaster->registerAddWidth;
         break;

      case ciaaPOSIX_IOCTL_SET_REGISTERADD:
         pI2CMaster->registerAdd = (uint32_t)(intptr_t)param;
         ret = (int32_t)pI2CMaster->registerAdd;
         break;

      default:
         ret = I2CDevice->device->ioctl(device->loLayer, request, param);
         break;
   }
   return ret;
}

extern ssize_t ciaaI2CDevices_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte)
{
   ciaaI2CDevices_deviceType * I2CDevice = (ciaaI2CDevices_deviceType*) device->layer;
   int32_t ret = 0;
   ciaaLibs_CircBufType * cbuftx = &I2CDevice->txBuf;
   ciaaLibs_CircBufType * cbufrx = &I2CDevice->rxBuf;
   uint32_t space;
   int i;
   uint8_t width;
   uint8_t * pByte;
   uint8_t val[4];

   space = ciaaLibs_circBufSpace(cbufrx, cbufrx->head);

   if ( nbyte <= space )
   {
   space = ciaaLibs_circBufSpace(cbuftx, cbuftx->head);

   if ( I2CDevice->masterData.registerAddWidth <= space )
   {
      width = I2CDevice->masterData.registerAddWidth;
      pByte = (uint8_t *)&I2CDevice->masterData.registerAdd;
      for( i = 0; i < width; i++ )
      {
         val[i] = pByte[width - 1 - i];
      }
      ciaaLibs_circBufPut(cbuftx, val, width);

      I2CDevice->device->write( device->loLayer, ciaaLibs_circBufReadPos(cbuftx), I2CDevice->masterData.registerAddWidth);

      ret = I2CDevice->device->read( device->loLayer, buf, nbyte);

      ciaaLibs_circBufClean(cbuftx);

      }
   }

   return ret;
}

extern ssize_t ciaaI2CDevices_write(ciaaDevices_deviceType const * const device, uint8_t const * buf, size_t const nbyte)
{
   ciaaI2CDevices_deviceType * I2CDevice = (ciaaI2CDevices_deviceType*) device->layer;
   int32_t ret = 0;
   ciaaLibs_CircBufType * cbuf = &I2CDevice->txBuf;
   uint32_t space;
   int i;
   uint8_t width;
   uint8_t * pByte;
   uint8_t val[4];

   space = ciaaLibs_circBufSpace(cbuf, cbuf->head);

   if ( (nbyte + I2CDevice->masterData.registerAddWidth) <= space )
   {
      width = I2CDevice->masterData.registerAddWidth;
      pByte = (uint8_t *)&I2CDevice->masterData.registerAdd;
      for( i = 0; i < width; i++ )
      {
         val[i] = pByte[width - 1 - i];
      }
      ciaaLibs_circBufPut(cbuf, val, width);

      ciaaLibs_circBufPut(cbuf, buf, nbyte);

      ret = I2CDevice->device->write(device->loLayer, ciaaLibs_circBufReadPos(cbuf), (nbyte + I2CDevice->masterData.registerAddWidth));

      ciaaLibs_circBufClean(cbuf);
   }

   return ret;
}

extern off_t ciaaI2CDevices_lseek(ciaaDevices_deviceType const * const device, off_t const offset, uint8_t const whence)
{
   off_t newPos = (off_t)-1;

   ciaaI2CDevices_deviceType * I2CDevice = (ciaaI2CDevices_deviceType*) device->layer;

   switch (whence) {
      case SEEK_CUR:
         newPos = I2CDevice->curPos += offset;
         break;

      case SEEK_SET:
         newPos = offset;
         break;

      case SEEK_END:
         newPos = I2CDevice->maxPos + offset;
         break;

#if CIAA_POSIX_I2C_DEBUG
      default:
         /* TODO */
         newPos = (off_t)-1;
         break;
#endif
   }

   if ((0 <= newPos) && (I2CDevice->maxPos >= newPos))
   {
      /* newPos is of the type off_t which is an integer, but
       * the position accept only possitive values. */
      I2CDevice->curPos = (uint32_t) newPos;
   }

   return newPos;
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


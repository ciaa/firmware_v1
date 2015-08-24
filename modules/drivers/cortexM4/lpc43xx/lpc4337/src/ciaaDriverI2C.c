/* Copyright 2015, Sebastian Bedin para SUR Emprendimientos Tecnologicos S.R.L. / CONAE
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

/** \brief CIAA I2C Driver for LPC4337
 **
 ** Implements the I2C Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup I2C I2C Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * SeBe         Sebastian Bedin
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150615 v0.0.1   SeBe first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverI2C.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverI2CConstType;

#define ciaaDriverI2C_RX_BUFFER_SIZE       (16)
#define ciaaDriverI2C_TX_BUFFER_SIZE       (16)

typedef struct {
   uint16_t slaveAdd;
   uint8_t buftx[4];
   uint8_t bufsize;
   I2C_ID_T i2c_id;
} ciaaDriverI2CControl;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverI2CControl i2cControl[2];

/** \brief Device for I2C 0 */
static ciaaDevices_deviceType ciaaDriverI2C_device0 = {
   "0",                    /** <= driver name */
   ciaaDriverI2C_open,     /** <= open function */
   ciaaDriverI2C_close,    /** <= close function */
   ciaaDriverI2C_read,     /** <= read function */
   ciaaDriverI2C_write,    /** <= write function */
   ciaaDriverI2C_ioctl,    /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   &(i2cControl[0]),       /** <= layer */
   LPC_I2C0                /** <= lower layer */
};

/** \brief Device for I2C 1 */
static ciaaDevices_deviceType ciaaDriverI2C_device1 = {
   "1",                    /** <= driver name */
   ciaaDriverI2C_open,     /** <= open function */
   ciaaDriverI2C_close,    /** <= close function */
   ciaaDriverI2C_read,     /** <= read function */
   ciaaDriverI2C_write,    /** <= write function */
   ciaaDriverI2C_ioctl,    /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   &(i2cControl[1]),       /** <= layer */
   LPC_I2C1                /** <= lower layer */
};

static ciaaDevices_deviceType * const ciaaI2CDevices[] = {
   &ciaaDriverI2C_device0,
   &ciaaDriverI2C_device1
};

static ciaaDriverI2CConstType const ciaaDriverI2CConst = {
   ciaaI2CDevices,
   1
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void ciaaDriverI2C_hwInit(void)
{
   /* I2C0 (I2C) */
   Chip_I2C_Init( I2C0 );
   Chip_I2C_SetClockRate( I2C0, 100000);
   Chip_SCU_I2C0PinConfig( I2C0_STANDARD_FAST_MODE );

#if 0
   /* I2C1 (conflicto con RS232)  */
   Chip_I2C_Init( I2C1 );
   Chip_I2C_SetClockRate( I2C1, 400000);
   Chip_SCU_PinMux(2, 3, MD_PLN, FUNC1);                 /* P2_3: I2C1_SDA */
   Chip_SCU_PinMux(2, 4, MD_PLN|MD_EZI|MD_ZI, FUNC1);    /* P2_4: I2C1_SCL */
#endif
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverI2C_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverI2C_close(ciaaDevices_deviceType const * const device)
{
   ciaaDriverI2CControl * pI2CControl;
   pI2CControl = (ciaaDriverI2CControl *)device->layer;
   Chip_I2C_Disable((I2C_ID_T)pI2CControl->i2c_id);
   return 0;
}

extern int32_t ciaaDriverI2C_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;
   ciaaDriverI2CControl * pI2CControl;

   if(	(device == ciaaDriverI2CConst.devices[0]) ||
         (device == ciaaDriverI2CConst.devices[1]) )
   {
      switch(request)
      {
      case ciaaPOSIX_IOCTL_SET_CLOCKRATE:
         pI2CControl = (ciaaDriverI2CControl *)device->layer;
         Chip_I2C_SetClockRate((I2C_ID_T)pI2CControl->i2c_id, (int32_t)param);
         ret = (int32_t)param;
         break;

      case ciaaPOSIX_IOCTL_SET_SLAVEADD:
         pI2CControl = (ciaaDriverI2CControl *)device->layer;
         pI2CControl->slaveAdd = (int8_t)param;
         ret = (int32_t)pI2CControl->slaveAdd;
         break;
      }
   }
   return ret;
}

extern ssize_t ciaaDriverI2C_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   ssize_t ret = -1;
   ciaaDriverI2CControl * pI2CControl;
   I2CM_XFER_T i2cmXferRec;

   if(size != 0)
   {
      if(	(device == ciaaDriverI2CConst.devices[0]) ||
            (device == ciaaDriverI2CConst.devices[1]) )
      {
         pI2CControl = (ciaaDriverI2CControl *)device->layer;

         i2cmXferRec.slaveAddr = pI2CControl->slaveAdd;
         i2cmXferRec.options = 0;
         i2cmXferRec.status = 0;
         i2cmXferRec.txSz = 0;
         i2cmXferRec.txBuff = NULL;
         i2cmXferRec.rxSz = size;
         i2cmXferRec.rxBuff = buffer;
         Chip_I2CM_XferBlocking((LPC_I2C_T *)device->loLayer, &i2cmXferRec);

         if(i2cmXferRec.status == I2CM_STATUS_OK)
         {
            ret = size;
         }
      }
   }
   return ret;
}

extern ssize_t ciaaDriverI2C_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = 0;
   ciaaDriverI2CControl * pI2CControl;
   I2CM_XFER_T i2cmXferRec;

   if(	(device == ciaaDriverI2CConst.devices[0]) ||
         (device == ciaaDriverI2CConst.devices[1]) )
   {
      pI2CControl = (ciaaDriverI2CControl *)device->layer;

      i2cmXferRec.slaveAddr = pI2CControl->slaveAdd;
      i2cmXferRec.options = 0;
      i2cmXferRec.status = 0;
      i2cmXferRec.txSz = size;
      i2cmXferRec.txBuff = buffer;
      i2cmXferRec.rxSz = 0;
      i2cmXferRec.rxBuff = NULL;
      Chip_I2CM_XferBlocking((LPC_I2C_T *)device->loLayer, &i2cmXferRec);

      if(i2cmXferRec.status == I2CM_STATUS_OK)
      {
         ret = size;
      }
   }
   return ret;
}

void ciaaDriverI2C_init(void)
{
   uint8_t loopi;

   /* init hardware */
   ciaaDriverI2C_hwInit();

   /* add i2c driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverI2CConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaI2CDevices_addDriver(ciaaDriverI2CConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

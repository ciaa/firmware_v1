/* Copyright 2016, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2016, Marcos Ziegler <ziegler.marcos.b@gmail.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2016, ACSE & CADIEEL & Marcos Ziegler <ziegler.marcos.b@gmail.com
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

/** \brief CIAA SSP Driver for LPC4337
 **
 ** Implements the SSP Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup SSP SSP drivers
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaDriverSSP_Internal.h"
#include "ciaaDriverSSP.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Device for SSP0 */
static ciaaDevices_deviceType ciaaDriverSSP_SSP0device = {
   "ssp/0",               /** <= driver name */
   ciaaDriverSSP_open,    /** <= open function */
   ciaaDriverSSP_close,   /** <= close function */
   ciaaDriverSSP_read,    /** <= read function */
   ciaaDriverSSP_write,   /** <= write function */
   ciaaDriverSSP_ioctl,   /** <= ioctl function */
   NULL,                  /** <= seek function is not provided */
   NULL,                  /** <= uper layer */
   &(ciaaDriverSSP_SSP0), /** <= layer */
   LPC_SSP0               /** <= lower layer */
};

/** \brief Device for SSP0 */
static ciaaDevices_deviceType ciaaDriverSSP_SSP1device = {
   "ssp/1",               /** <= driver name */
   ciaaDriverSSP_open,    /** <= open function */
   ciaaDriverSSP_close,   /** <= close function */
   ciaaDriverSSP_read,    /** <= read function */
   ciaaDriverSSP_write,   /** <= write function */
   ciaaDriverSSP_ioctl,   /** <= ioctl function */
   NULL,                  /** <= seek function is not provided */
   NULL,                  /** <= uper layer */
   &(ciaaDriverSSP_SSP1), /** <= layer */
   LPC_SSP1               /** <= lower layer */
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void ciaaDriverSSP_hwInit(void)
{
   int ret;
   ciaaDriverSSP_SSPDevType *SSP_dev;

   SSP_dev = ciaaDriverSSP_SSP1device->layer;
   ret = ciaaDriverSSP_channelInit(SSP_dev);
}

/*==================[external functions definition]==========================*/

extern ciaaDevices_deviceType * ciaaDriverSSP_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   /* Restart FIFOS */
   Chip_SSP_Int_FlushData((LPC_SSP_T *)device->loLayer);

   return device;
}

extern int32_t ciaaDriverSSP_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverSSP_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;
   ciaaDriverSSP_SSPDevType *pSSP_dev = NULL;

   if(device == ciaaDriverSPIConst.devices)
   {
      pSSP_dev = (ciaaDriverSSP_SSPDevType *)device->layer;
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTTX:
            break;

         case ciaaPOSIX_IOCTL_SET_BAUDRATE:
            pSSP_dev->set_ClockRate(pSSP_dev, (uint32_t) param);
            ret = 0;
            break;
         default:
            break;
      }
   }
   return ret;
}

extern ssize_t ciaaDriverSSP_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   ssize_t ret = -1;
   ciaaDriverSSP_SSPDevType *pSSP_dev = NULL;
   uint8_t i;

   if(size != 0)
   {
      if(device == ciaaDriverSPIConst.devices)
      {
         pSSP_dev = (ciaaDriverSSP_SSPDevType *)device->layer;

         for(i = 0; i < size; i++)
         {
            buffer[i] = pSSP_dev->RWByte(pSSP_dev, 0xFF);
         }
         if(i == size)
         {
            ret = size;
         }
      }
   }
   return ret;
}

extern ssize_t ciaaDriverSSP_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = 0;
   ciaaDriverSSP_SSPDevType *pSSP_dev = NULL;

   if(device == ciaaDriverSSPConst.devices)
   {
      pSSP_dev = device->layer;
      while(ret < size)
      {
         /* send first byte */
         pSSP_dev->RWByte(pSSP_dev, buffer[ret]);
         /* bytes written */
         ret++;
      }
   }
   return ret;
}

void ciaaDriverSSP_init(void)
{
   uint8_t loopi;

   /* init hardware */
   ciaaDriverSSP_hwInit();

   /* add SPI driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverSSPConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverSSPConst.devices[loopi]);
   }
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

/* Copyright 2014, 2015 Pablo Ridolfi (UTN-FRBA)
 * Copyright 2016, Esteban Volentini (LabMicro - UNT)
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

/** \brief CIAA Dio Driver for LPC4337
 **
 ** Implements the Digital Input/Output (Dio) Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup DIO DIO Drivers
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaDriverDio.h"
#include "board_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Device for DIO 0 */
static ciaaDevices_deviceType ciaaDriverDio_dio0 = {
   "gpio/0",                        /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   NULL,                           /** <= layer */
   (void*)&ciaaDriverDio_port0     /** <= port description as lowlayer */
};

#if (CIAA_DRIVER_DIO_DEVICE_COUNTER >= 2)
/** \brief Device for DIO 1 */
static ciaaDevices_deviceType ciaaDriverDio_dio1 = {
   "gpio/1",                        /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   NULL,                           /** <= layer */
   (void*)&ciaaDriverDio_port1     /** <= port description as lowlayer */
};
#endif

#if (CIAA_DRIVER_DIO_DEVICE_COUNTER >= 3)
/** \brief Device for DIO 2 */
static ciaaDevices_deviceType ciaaDriverDio_dio2 = {
   "gpio/2",                        /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   NULL,                           /** <= layer */
   (void*)&ciaaDriverDio_port2     /** <= port description as lowlayer */
};
#endif

#if (CIAA_DRIVER_DIO_DEVICE_COUNTER >= 4)
/** \brief Device for DIO 3 */
static ciaaDevices_deviceType ciaaDriverDio_dio3 = {
   "gpio/3",                        /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   NULL,                           /** <= layer */
   (void*)&ciaaDriverDio_port3     /** <= port description as lowlayer */
};
#endif

#if (CIAA_DRIVER_DIO_DEVICE_COUNTER == 4)
static ciaaDevices_deviceType * const ciaaDioDevices[] = {
   &ciaaDriverDio_dio0,
   &ciaaDriverDio_dio1,
   &ciaaDriverDio_dio2,
   &ciaaDriverDio_dio3
};
#elif (CIAA_DRIVER_DIO_DEVICE_COUNTER == 3)
static ciaaDevices_deviceType * const ciaaDioDevices[] = {
   &ciaaDriverDio_dio0,
   &ciaaDriverDio_dio1,
   &ciaaDriverDio_dio2
};
#elif (CIAA_DRIVER_DIO_DEVICE_COUNTER == 2)
static ciaaDevices_deviceType * const ciaaDioDevices[] = {
   &ciaaDriverDio_dio0,
   &ciaaDriverDio_dio1
};
#else
static ciaaDevices_deviceType * const ciaaDioDevices[] = {
   &ciaaDriverDio_dio0
};
#endif

static ciaaDriverConstType const ciaaDriverDioConst = {
   ciaaDioDevices,
   CIAA_DRIVER_DIO_DEVICE_COUNTER
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverDio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaDriverDio_portType const * port = device->loLayer;
   ciaaDriverDio_pinType const * pin = port->pins;

   uint32_t index;

   /* low level GPIO peripheral initialization */
   for(index = 0; index < port->count; index++) 
   {
      Chip_SCU_PinMuxSet(pin->scu.pingrp, pin->scu.pinnum, pin->scu.modefunc);
      Chip_GPIO_SetDir(LPC_GPIO_PORT, pin->gpio, (1 << pin->pin), pin->output);
      pin++; 
   }

   return device;
}

extern int32_t ciaaDriverDio_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverDio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   return -1;
}

extern ssize_t ciaaDriverDio_read(ciaaDevices_deviceType const * const device, uint8_t * buffer, size_t size)
{
   uint32_t index;
   uint32_t input = 0;
   ssize_t result = -1;
   uint8_t * pointer = buffer;
   ciaaDriverDio_portType const * port = device->loLayer;
   ciaaDriverDio_pinType const * pin = port->pins;

   if (sie > 0) 
   {
      ciaaPOSIX_memset(buffer, 0, size);
      /* if pointer size less than pin count then return only 8 * size first pins */
      if ((size << 3) > (port->count >> 3)) {
         result = port->count;
      } else {
         result = size << 3;
      }

      for(index = 0; index < result; index++) 
      {
         *pointer |= (Chip_GPIO_GetPinState(LPC_GPIO_PORT, pin->gpio, pin->pin) ? 1 : 0) << input;

         input = (input + 1) & 0x07;
         /* if input == 0 then pointer++ */
         pointer += (input ? 0 : 1);
         pin++; 
      }
      result >>= 3;
   }
   return result;
}

extern ssize_t ciaaDriverDio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   uint32_t index;
   uint32_t input = 0;
   ssize_t result = -1;
   uint8_t const * pointer = buffer;
   ciaaDriverDio_portType const * port = device->loLayer;
   ciaaDriverDio_pinType const * pin = port->pins;

   if (size > 0) 
   {
      /* if buffer size less than pin count then return only 8 * size first pins */
      if ((size << 3) > (port->count >> 3)) {
         result = port->count;
      } else {
         result = size << 3;
      }

      for(index = 0; index < result; index++) 
      {
         Chip_GPIO_SetPinState(LPC_GPIO_PORT, pin->gpio, pin->pin, *pointer & (1 << input));

         input = (input + 1) & 0x07;
         /* if input == 0 then buffer++ */
         pointer += (input ? 0 : 1);
         pin++; 
      }
      result >>= 3;
   }
   return result;
}

void ciaaDriverDio_init(void)
{
   uint8_t loopi;

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverDioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaDioDevices_addDriver(ciaaDriverDioConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

/* Copyright 2014, Mariano Cerdeiro
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

/** \brief CIAA Dio Posix Driver
 **
 ** Simulated DIO Driver for Posix for testing proposes
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup DIO DIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140528 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverDio.h"
#include "ciaaDriverDio_Internal.h"
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
/** \brief Device for in DIO 0 */
static ciaaDevices_deviceType ciaaDriverDio_device0 = {
   "in/0",                         /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverDio_dio0,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for out DIO 0 */
static ciaaDevices_deviceType ciaaDriverDio_device1 = {
   "out/0",                        /** <= driver name */
   ciaaDriverDio_open,             /** <= open function */
   ciaaDriverDio_close,            /** <= close function */
   ciaaDriverDio_read,             /** <= read function */
   ciaaDriverDio_write,            /** <= write function */
   ciaaDriverDio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverDio_dio1,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaDioDevices[] = {
   &ciaaDriverDio_device0,
   &ciaaDriverDio_device1
};

static ciaaDriverConstType const ciaaDriverDioConst = {
   ciaaDioDevices,
   2
};

/*==================[external data definition]===============================*/
/** \brief Dio 0 */
ciaaDriverDio_dioType ciaaDriverDio_dio0;

/** \brief Dio 1 */
ciaaDriverDio_dioType ciaaDriverDio_dio1;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverDio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
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

extern ssize_t ciaaDriverDio_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t size)
{
   return 0;
}

extern ssize_t ciaaDriverDio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   return 0;
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


/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


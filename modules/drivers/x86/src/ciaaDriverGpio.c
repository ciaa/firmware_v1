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

/** \brief CIAA Gpio Posix Driver
 **
 ** Simulated GPIO Driver for Posix for testing proposes
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup GPIO GPIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150715 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverGpio.h"
#include "ciaaDriverGpio_Internal.h"
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
/** \brief Device for in GPIO 0 */
static ciaaDevices_deviceType ciaaDriverGpio_device0 = {
   "in-out/0",                         /** <= driver name */
   ciaaDriverGpio_open,             /** <= open function */
   ciaaDriverGpio_close,            /** <= close function */
   ciaaDriverGpio_read,             /** <= read function */
   ciaaDriverGpio_write,            /** <= write function */
   ciaaDriverGpio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&ciaaDriverGpio_gpio0,     /** <= layer */
   NULL                            /** <= NULL no lower layer */
};


static ciaaDevices_deviceType * const ciaaGpioDevices[] = {
   &ciaaDriverGpio_device0
};

static ciaaDriverConstType const ciaaDriverGpioConst = {
   ciaaGpioDevices,
   1
};

/*==================[external data definition]===============================*/
/** \brief Gpio 0 */
ciaaDriverGpio_gpioType ciaaDriverGpio_gpio0;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverGpio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverGpio_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverGpio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   return -1;
}

extern ssize_t ciaaDriverGpio_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t size)
{
   return 0;
}

extern ssize_t ciaaDriverGpio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   return 0;
}

void ciaaDriverGpio_init(void)
{
   uint8_t loopi;

   /* add gpio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverGpioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaGpioDevices_addDriver(ciaaDriverGpioConst.devices[loopi]);
   }
}


/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


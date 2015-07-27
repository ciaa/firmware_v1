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

/** \brief CIAA Dio Driver for K60_120
 **
 ** Implements the Digital Input/Output (Dio) Driver for K60_120
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
 * PR           Pablo Ridolfi
 * Apermingeat  Alejandro Permingeat
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140913 v0.0.1 MaCe first stub version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverDio.h"
#include "ciaaDriverDio_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "fsl_port_hal.h"
#include "fsl_gpio_hal.h"

/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

#define CIAA_GPIO_INPUTS_NUMBER		2
#define CIAA_GPIO_OUTPUTS_NUMBER	   4

/*==================[internal data declaration]==============================*/
static ciaaDriverDio_pinType const ciaaGPIOinputPins[CIAA_GPIO_INPUTS_NUMBER] = {
      {PORTA, PTA, 19u}, {PORTE, PTE, 26u}
};
static ciaaDriverDio_pinType const ciaaGPIOoutputPins[CIAA_GPIO_OUTPUTS_NUMBER] = {
      {PORTA, PTA, 11u}, {PORTA, PTA, 28u}, {PORTA, PTA, 29u}, {PORTA, PTA, 10u}
};

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for DIO 0 */
static ciaaDevices_deviceType ciaaDriverDio_in0 = {
   "in/0",                          /** <= driver name */
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

/** \brief Device for DIO 1 */
static ciaaDevices_deviceType ciaaDriverDio_out0 = {
   "out/0",                          /** <= driver name */
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
   &ciaaDriverDio_in0,
   &ciaaDriverDio_out0
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
   uint8_t index;

   if (device == &ciaaDriverDio_in0) {
      for(index = 0; index < CIAA_GPIO_INPUTS_NUMBER; index++) {
         PORT_HAL_SetPassiveFilterCmd(ciaaGPIOinputPins[index].port, ciaaGPIOinputPins[index].pin, false);
         PORT_HAL_SetMuxMode(ciaaGPIOinputPins[index].port, ciaaGPIOinputPins[index].pin, kPortMuxAsGpio);
         PORT_HAL_SetPullMode(ciaaGPIOinputPins[index].port, ciaaGPIOinputPins[index].pin, kPortPullUp);
         PORT_HAL_SetPullCmd(ciaaGPIOinputPins[index].port, ciaaGPIOinputPins[index].pin, true);
         PORT_HAL_SetPinIntMode(ciaaGPIOinputPins[index].port, ciaaGPIOinputPins[index].pin, kPortIntDisabled);
         GPIO_HAL_SetPinDir(ciaaGPIOinputPins[index].gpio, ciaaGPIOinputPins[index].pin, kGpioDigitalInput);
      }
   } else if (device == &ciaaDriverDio_out0) {
      for(index = 0; index < CIAA_GPIO_OUTPUTS_NUMBER; index++) {
         PORT_HAL_SetMuxMode(ciaaGPIOoutputPins[index].port, ciaaGPIOoutputPins[index].pin, kPortMuxAsGpio);
         PORT_HAL_SetPullCmd(ciaaGPIOoutputPins[index].port, ciaaGPIOoutputPins[index].pin, false);
         GPIO_HAL_SetPinDir(ciaaGPIOoutputPins[index].gpio, ciaaGPIOoutputPins[index].pin, kGpioDigitalOutput);
         GPIO_HAL_ClearPinOutput(ciaaGPIOoutputPins[index].gpio, ciaaGPIOoutputPins[index].pin);
      }
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
  ssize_t ret = -1;

  /* Can't store read result in buffer. At least 1 byte required. */
  if(size != 0)
  {
     if(device == &ciaaDriverDio_in0)
     {
	 int32_t i;
	 buffer[0] = 0;
	 for(i = 0; i < CIAA_GPIO_INPUTS_NUMBER; i++)
	 {
	    //ciaa_lpc4337_writeOutput(i, buffer[0] & (1 << i));
	     buffer[0] |= GPIO_HAL_ReadPinInput(ciaaGPIOinputPins[i].gpio, ciaaGPIOinputPins[i].pin) << i;
	 }

        /* 1 byte read */
        ret = 1;
     }
     else if(device == &ciaaDriverDio_out0)
     {
        /* read actual output state from layer data */
        buffer[0] = (uint8_t)*((ciaaDriverDio_dioType *)device->layer);

        ret = 1;
     }
     else
     {
        /* Invalid device */
        ret = -1;
     }
  }
  return ret;
}

extern ssize_t ciaaDriverDio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
  ssize_t ret = -1;

  if(size != 0)
  {
     if(device == &ciaaDriverDio_in0)
     {
        /* Inputs can't be written. */
        ret = -1;
     }
     else if(device == &ciaaDriverDio_out0)
     {
        int32_t i;
        for(i = 0; i < CIAA_GPIO_OUTPUTS_NUMBER; i++)
        {
           GPIO_HAL_WritePinOutput(ciaaGPIOoutputPins[i].gpio, ciaaGPIOoutputPins[i].pin, (buffer[0] & (1 << i))>>i);
        }

        /* save actual output state in layer data */
        *((ciaaDriverDio_dioType *)device->layer) = buffer[0];

        /* 1 byte written */
        ret = 1;
     }
     else
     {
        ret = -1;
     }
  }
  return ret;
}

void ciaaDriverDio_init(void)
{
   uint8_t loopi;

   /* add dio driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverDioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaDioDevices_addDriver(ciaaDriverDioConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverDio_dioType *)ciaaDriverDioConst.devices[loopi]->layer) = 0;
   }
}

/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


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
 * AP           Alejandro Permingeat
 * EsVo         Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150829 v0.0.5 AP   Bug fixed in pinout definition for DOUT7
 * 20150803 v0.0.4 EsVo verify device received in open function
 * 20150801 v0.0.3 EsVo migration to KSDK 1.2 and remove of drivers files
 * 20150329 v0.0.2 AP   first operational version using KSDK 1.0
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

//#define CIAA_GPIO_INPUTS_NUMBER      2
//#define CIAA_GPIO_OUTPUTS_NUMBER     4

#define CIAA_GPIO_INPUTS_NUMBER        8
#define CIAA_GPIO_OUTPUTS_NUMBER    8

/*==================[internal data declaration]==============================*/
/*
static ciaaDriverDio_pinType const ciaaGPIOinputPins[CIAA_GPIO_INPUTS_NUMBER] = {
      {PORTA, 19u, PTA, kSimClockGatePortA}, {PORTE, 26u, PTE, kSimClockGatePortE}
};
static ciaaDriverDio_pinType const ciaaGPIOoutputPins[CIAA_GPIO_OUTPUTS_NUMBER] = {
      {PORTA, 11u, PTA, kSimClockGatePortA}, {PORTA, 28u, PTA, kSimClockGatePortA},
      {PORTA, 29u, PTA, kSimClockGatePortA}, {PORTA, 10u, PTA, kSimClockGatePortA}
};
*/
static ciaaDriverDio_pinType const ciaaGPIOinputPins[CIAA_GPIO_INPUTS_NUMBER] = {
      {PORTC,  4u, PTC, kSimClockGatePortC}, {PORTC,  5u, PTC, kSimClockGatePortC},
      {PORTC,  6u, PTC, kSimClockGatePortC}, {PORTC,  7u, PTC, kSimClockGatePortC},
      {PORTC,  8u, PTC, kSimClockGatePortC}, {PORTC,  9u, PTC, kSimClockGatePortC},
      {PORTC, 10u, PTC, kSimClockGatePortC}, {PORTC, 11u, PTC, kSimClockGatePortC}
};
static ciaaDriverDio_pinType const ciaaGPIOoutputPins[CIAA_GPIO_OUTPUTS_NUMBER] = {
      {PORTB, 16u, PTB, kSimClockGatePortB}, {PORTB,  6u, PTB, kSimClockGatePortB},
      {PORTB,  7u, PTB, kSimClockGatePortB}, {PORTB, 17u, PTB, kSimClockGatePortB},
      {PORTC, 15u, PTC, kSimClockGatePortC}, {PORTC, 14u, PTC, kSimClockGatePortC},
      {PORTC, 13u, PTC, kSimClockGatePortC}, {PORTC, 12u, PTC, kSimClockGatePortC}
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
   ciaaDriverDio_pinType const * dio;
   uint8_t index;

   if (device == &ciaaDriverDio_in0) {
      for(index = 0; index < CIAA_GPIO_INPUTS_NUMBER; index++) {
         dio = &ciaaGPIOinputPins[index];

         /* Enable clock for PORTs */
         SIM_HAL_EnableClock(SIM, dio->gate);

         PORT_HAL_SetPassiveFilterCmd(dio->port, dio->pin, false);
         PORT_HAL_SetMuxMode(dio->port, dio->pin, kPortMuxAsGpio);
         PORT_HAL_SetPullMode(dio->port, dio->pin, kPortPullUp);
         PORT_HAL_SetPullCmd(dio->port, dio->pin, true);
         PORT_HAL_SetPinIntMode(dio->port, dio->pin, kPortIntDisabled);

         GPIO_HAL_SetPinDir(dio->gpio, dio->pin, kGpioDigitalInput);
      }
   } else if (device == &ciaaDriverDio_out0) {
      for(index = 0; index < CIAA_GPIO_OUTPUTS_NUMBER; index++) {
         dio = &ciaaGPIOoutputPins[index];

         SIM_HAL_EnableClock(SIM, ciaaGPIOoutputPins[index].gate);

         PORT_HAL_SetMuxMode(dio->port, dio->pin, kPortMuxAsGpio);
         PORT_HAL_SetPullCmd(dio->port, dio->pin, false);

         GPIO_HAL_SetPinDir(dio->gpio, dio->pin, kGpioDigitalOutput);
         GPIO_HAL_ClearPinOutput(dio->gpio, dio->pin);

         /* Configure GPIO output features. */
         GPIO_HAL_WritePinOutput(dio->gpio, dio->pin, 0);
#if FSL_FEATURE_PORT_HAS_SLEW_RATE
         PORT_HAL_SetSlewRateMode(dio->port, dio->pin, kPortFastSlewRate);
#endif
#if FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH
         PORT_HAL_SetDriveStrengthMode(dio->port, dio->pin, kPortHighDriveStrength);
#endif
#if FSL_FEATURE_PORT_HAS_OPEN_DRAIN
         PORT_HAL_SetOpenDrainCmd(dio->port, dio->pin, false);
#endif

      }
   } else {
      device = NULL;
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
   }
}

/*==================[interrupt hanlders]=====================================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


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
#include "fsl_gpio_driver.h"

/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

#define CIAA_GPIO_INPUTS_NUMBER		8
#define CIAA_GPIO_OUTPUTS_NUMBER	8

/*==================[internal data declaration]==============================*/
gpio_input_pin_user_config_t ciaaGPIOinputCofig[CIAA_GPIO_INPUTS_NUMBER+1];
gpio_output_pin_user_config_t ciaaGPIOoutputCofig[CIAA_GPIO_OUTPUTS_NUMBER+1];

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
void ciaa_k60_120_gpio_init(void)
{


#if (BOARD == ciaa_fsl)

   /* Inputs */
   /*Digital In 0 ->    GPIO14    PTC4*/
   ciaaGPIOinputCofig[0].pinName = GPIO_MAKE_PIN(HW_GPIOC, 4);
   ciaaGPIOinputCofig[0].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[0].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[0].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[0].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
   /*Digital In 1 ->     GPIO12    PTC5*/
   ciaaGPIOinputCofig[1].pinName = GPIO_MAKE_PIN(HW_GPIOC, 5);
   ciaaGPIOinputCofig[1].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[1].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[1].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[1].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
   /*Digital In 2 ->     GPIO10    PTC6*/
   ciaaGPIOinputCofig[2].pinName = GPIO_MAKE_PIN(HW_GPIOC, 6);
   ciaaGPIOinputCofig[2].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[2].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[2].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[2].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
   /*Digital In 3 ->     GPIO8    PTC7*/
   ciaaGPIOinputCofig[3].pinName = GPIO_MAKE_PIN(HW_GPIOC, 7);
   ciaaGPIOinputCofig[3].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[3].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[3].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[3].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
    /*Digital In 4 ->     GPIO6    PTC8*/
   ciaaGPIOinputCofig[4].pinName = GPIO_MAKE_PIN(HW_GPIOC, 8);
   ciaaGPIOinputCofig[4].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[4].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[4].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[4].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
   /*Digital In 5 ->     GPIO4    PTC9*/
   ciaaGPIOinputCofig[5].pinName = GPIO_MAKE_PIN(HW_GPIOC, 9);
   ciaaGPIOinputCofig[5].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[5].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[5].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[5].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
    /*Digital In 6 ->     GPIO2    PTC10*/
   ciaaGPIOinputCofig[6].pinName = GPIO_MAKE_PIN(HW_GPIOC, 10);
   ciaaGPIOinputCofig[6].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[6].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[6].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[6].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/
   /*Digital In 7 ->     GPIO0    PTC11*/
   ciaaGPIOinputCofig[7].pinName = GPIO_MAKE_PIN(HW_GPIOC, 11);
   ciaaGPIOinputCofig[7].config.isPullEnable = true;             /*!< Enable or disable pull. */
   ciaaGPIOinputCofig[7].config.pullSelect = kPortPullUp;        /*!< Select internal pull(up/down) resistor.*/
   ciaaGPIOinputCofig[7].config.isPassiveFilterEnabled = false;  /*!< Enable or disable passive filter.*/
   ciaaGPIOinputCofig[7].config.interrupt = kPortIntDisabled;    /*!< Select interrupt/DMA request.*/

   //Note: This pinName must be defined here to indicate the end of array.
   ciaaGPIOinputCofig[8].pinName = GPIO_PINS_OUT_OF_RANGE,


   /* Outputs */

   /* Relays */
   /*Digital Out 0 -> PTB16*/
   ciaaGPIOoutputCofig[0].pinName = GPIO_MAKE_PIN(HW_GPIOB, 16);
   ciaaGPIOoutputCofig[0].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[0].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[0].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[0].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/
   /*Digital Out 1 -> PTB06*/
   ciaaGPIOoutputCofig[1].pinName = GPIO_MAKE_PIN(HW_GPIOB, 6);
   ciaaGPIOoutputCofig[1].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[1].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[1].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[1].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/
   /*Digital Out 2 -> PTB07*/
   ciaaGPIOoutputCofig[2].pinName = GPIO_MAKE_PIN(HW_GPIOB, 7);
   ciaaGPIOoutputCofig[2].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[2].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[2].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[2].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/
   /*Digital Out 3 -> PTB17*/
   ciaaGPIOoutputCofig[3].pinName = GPIO_MAKE_PIN(HW_GPIOB, 17);
   ciaaGPIOoutputCofig[3].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[3].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[3].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[3].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/


   /* MOSFETs */
   /*Digital Out 4 -> PTC15*/
   ciaaGPIOoutputCofig[4].pinName = GPIO_MAKE_PIN(HW_GPIOC, 15);
   ciaaGPIOoutputCofig[4].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[4].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[4].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[4].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/
   /*Digital Out 5 -> PTC14*/
   ciaaGPIOoutputCofig[5].pinName = GPIO_MAKE_PIN(HW_GPIOC, 14);
   ciaaGPIOoutputCofig[5].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[5].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[5].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[5].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/
   /*Digital Out 6 -> PTC13*/
   ciaaGPIOoutputCofig[6].pinName = GPIO_MAKE_PIN(HW_GPIOC, 13);
   ciaaGPIOoutputCofig[6].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[6].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[6].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[6].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/
   /*Digital Out 7 -> PTC12*/
   ciaaGPIOoutputCofig[7].pinName = GPIO_MAKE_PIN(HW_GPIOC, 12);
   ciaaGPIOoutputCofig[7].config.outputLogic = 0;                       /*!< Set default output logic.*/
   ciaaGPIOoutputCofig[7].config.slewRate = kPortFastSlewRate;          /*! Select fast/slow slew rate.*/
   ciaaGPIOoutputCofig[7].config.driveStrength = kPortHighDriveStrength;/*!< Select low/high drive strength.*/
   ciaaGPIOoutputCofig[7].config.isOpenDrainEnabled = false;             /*!< Enable or disable open drain.*/

   //Note: This pinName must be defined here to indicate the end of array.
   ciaaGPIOoutputCofig[8].pinName = GPIO_PINS_OUT_OF_RANGE,

   GPIO_DRV_Init(ciaaGPIOinputCofig, ciaaGPIOoutputCofig);

#else
   #error please define BOARD variable!
#endif
}

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

extern ssize_t ciaaDriverDio_read(ciaaDevices_deviceType const * const device, uint8_t * buffer, size_t size)
{
  ssize_t ret = -1;

  /* Can't store read result in buffer. At least 1 byte required. */
  if(size != 0)
  {
     if(device == ciaaDioDevices[0])
     {
	 int32_t i;
	 buffer[0] = 0;
	 for(i = 0; i < 8; i++)
	 {
	    //ciaa_lpc4337_writeOutput(i, buffer[0] & (1 << i));
	     buffer[0] |= GPIO_DRV_ReadPinInput(ciaaGPIOoutputCofig[i].pinName) << i;
	 }

        /* 1 byte read */
        ret = 1;
     }
     else if(device == ciaaDioDevices[1])
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
     if(device == ciaaDioDevices[0])
     {
        /* Inputs can't be written. */
        ret = -1;
     }
     else if(device == ciaaDioDevices[1])
     {
        int32_t i;
        for(i = 0; i < 8; i++)
        {
           GPIO_DRV_WritePinOutput(ciaaGPIOoutputCofig[i].pinName, (buffer[0] & (1 << i))>>i);
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

   ciaa_k60_120_gpio_init();

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


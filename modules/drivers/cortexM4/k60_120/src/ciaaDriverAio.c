/* Copyright 2014, Alejandro Permingeat
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

/** \brief CIAA Aio Driver for mk60fx512vlq15
 **
 ** Implements the A/D & D/A Drivers for mk60fx512vlq15
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup AIO AIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * AP           Alejandro Permingeat
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 *
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverAio.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

typedef uint8_t ContextType;

#define AIO_FIFO_SIZE       (16)

typedef struct {
} ciaaDriverAdcControlType;

typedef struct {
} ciaaDriverDacControlType;

typedef union {
   ciaaDriverAdcControlType adc;        /** <= ADC control */
   ciaaDriverDacControlType dac;        /** <= DAC control */
} ciaaDriverAdcDacControlType;

typedef struct {
   int32_t channel;                     /** <= current channel */
   uint8_t cnt;                         /** <= count */
   uint8_t hwbuf[AIO_FIFO_SIZE];        /** <= buffer */
   ciaaDriverAdcDacControlType adc_dac; /** <= ADC & DAC control */
} ciaaDriverAioControlType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverAioControlType aioControl[3];

/** \brief Device for ADC 0 */
static ciaaDevices_deviceType ciaaDriverAio_in0 = {
   "aio/in/0",                     /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   &(aioControl[0]),               /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for ADC 1 */
static ciaaDevices_deviceType ciaaDriverAio_in1 = {
   "aio/in/1",                     /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   &(aioControl[1]),               /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for DAC 0 */
static ciaaDevices_deviceType ciaaDriverAio_out0 = {
   "aio/out/0",                    /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   &(aioControl[2]),               /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaAioDevices[] = {
   &ciaaDriverAio_in0,
   &ciaaDriverAio_in1,
   &ciaaDriverAio_out0
};

static ciaaDriverConstType const ciaaDriverAioConst = {
   ciaaAioDevices,
   3
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[internal functions definition]==========================*/
static void ciaaDriverAio_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_rxIndication(device->upLayer, nbyte);
}

static void ciaaDriverAio_txConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_txConfirmation(device->upLayer, nbyte);
}

static void ciaaDriverAio_adcIRQHandler(ciaaDevices_deviceType const * const device)
{

}

static void ciaaDriverAio_dacIRQHandler(ciaaDevices_deviceType const * const device)
{
 }

void ciaa_lpc4337_aio_init(void)
{

}


/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverAio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverAio_close(ciaaDevices_deviceType const * const device)
{
   int32_t ret = -1;

   return ret;
}

extern int32_t ciaaDriverAio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;

   return ret;
}

extern ssize_t ciaaDriverAio_read(ciaaDevices_deviceType const * const device, uint8_t * const buffer, size_t const size)
{
   ssize_t ret = -1;

   return ret;
}

extern ssize_t ciaaDriverAio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = -1;
   return ret;
}

void ciaaDriverAio_init(void)
{

}


/*==================[interrupt hanlders]=====================================*/

ISR(ADC0_IRQHandler)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in0);
}

ISR(ADC1_IRQHandler)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in1);
}

ISR(DMA_IRQHandler)
{
   ciaaDriverAio_dacIRQHandler(&ciaaDriverAio_out0);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


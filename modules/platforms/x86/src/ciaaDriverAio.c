/* Copyright 2014, 2015, Mariano Cerdeiro
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

/** \brief CIAA AIO Posix Driver
 **
 ** Simulated AIO Driver for Posix for testing proposes
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup AIO Analog Input Output Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141101 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverAio.h"
#include "ciaaDriverAio_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "os.h"

/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for UART 0 */
static ciaaDevices_deviceType ciaaDriverAio_device0 = {
   "aio/0",                        /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                            /** <= seek function is not provided */
   NULL,                            /** <= upper layer */
   (void*)&ciaaDriverAio_uart0,    /** <= layer */
   NULL                             /** <= NULL no lower layer */
};

/** \brief Device for UART 1 */
static ciaaDevices_deviceType ciaaDriverAio_device1 = {
   "aio/1",                        /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                            /** <= seek function is not provided */
   NULL,                            /** <= upper layer */
   (void*)&ciaaDriverAio_uart1,    /** <= layer */
   NULL                             /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaAioDevices[] = {
   &ciaaDriverAio_device0,
   &ciaaDriverAio_device1
};

static ciaaDriverConstType const ciaaDriverAioConst = {
   ciaaAioDevices,
   2
};

/*==================[external data definition]===============================*/
/** \brief Aio 0 */
ciaaDriverAio_uartType ciaaDriverAio_uart0;

/** \brief Aio 1 */
ciaaDriverAio_uartType ciaaDriverAio_uart1;

/*==================[internal functions definition]==========================*/
static void ciaaDriverAio_rxIndication(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaDriverAio_uartType * uart = device->layer;

   ciaaSerialDevices_rxIndication(device->upLayer, uart->rxBuffer.length);
}

static void ciaaDriverAio_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaDriverAio_uartType * uart = device->layer;

   ciaaSerialDevices_txConfirmation(device->upLayer, uart->txBuffer.length);
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverAio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverAio_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverAio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   return -1;
}

extern ssize_t ciaaDriverAio_read(ciaaDevices_deviceType const * const device, uint8_t * const buffer, size_t const size)
{
   size_t ret = size;

   /* receive the data and forward to upper layer */
   ciaaDriverAio_uartType * uart = device->layer;

   if (size > uart->rxBuffer.length)
   {
      ret = uart->rxBuffer.length;
   }

   /* copy received bytes to upper layer */
   ciaaPOSIX_memcpy(buffer, &uart->rxBuffer.buffer[0], ret);

   return ret;
}

extern ssize_t ciaaDriverAio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   size_t ret = 0;

   /* write data */
   ciaaDriverAio_uartType * uart = device->layer;

   if (0 == uart->txBuffer.length)
   {
      /* copy data */
      ciaaPOSIX_memcpy(&uart->txBuffer.buffer, buffer, size);

      /* return lenght and set 0 for the next */
      ret = size;

      /* set length of the buffer */
      uart->txBuffer.length = size;
   }

   return ret;
}

void ciaaDriverAio_init(void)
{
   uint8_t loopi;

   /* add uart driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverAioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverAioConst.devices[loopi]);
   }
}


/*==================[interrupt hanlders]=====================================*/
ISR(ADC0_IRQHandler)
{
   ciaaDriverAio_rxIndication(&ciaaDriverAio_device0);
}

ISR(ADC1_IRQHandler)
{
   ciaaDriverAio_txConfirmation(&ciaaDriverAio_device0);
}

ISR(DMA_IRQHandler)
{
   ciaaDriverAio_txConfirmation(&ciaaDriverAio_device1);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/



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
 */

/** \brief CIAA Uart Posix Driver
 **
 ** Simulated UART Driver for Posix for testing proposes
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup UART UART Drivers
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
#include "ciaaDriverUart.h"
#include "ciaaDriverUart_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

#define ISR(name)   void OSEK_ISR_ ## name (void)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for UART 0 */
static ciaaDevices_deviceType ciaaDriverUart_device0 = {
   "uart/0",                        /** <= driver name */
   ciaaDriverUart_open,             /** <= open function */
   ciaaDriverUart_close,            /** <= close function */
   ciaaDriverUart_read,             /** <= read function */
   ciaaDriverUart_write,            /** <= write function */
   ciaaDriverUart_ioctl,            /** <= ioctl function */
   NULL,                            /** <= seek function is not provided */
   NULL,                            /** <= upper layer */
   (void*)&ciaaDriverUart_uart0,    /** <= layer */
   NULL                             /** <= NULL no lower layer */
};

/** \brief Device for UART 1 */
static ciaaDevices_deviceType ciaaDriverUart_device1 = {
   "uart/1",                        /** <= driver name */
   ciaaDriverUart_open,             /** <= open function */
   ciaaDriverUart_close,            /** <= close function */
   ciaaDriverUart_read,             /** <= read function */
   ciaaDriverUart_write,            /** <= write function */
   ciaaDriverUart_ioctl,            /** <= ioctl function */
   NULL,                            /** <= seek function is not provided */
   NULL,                            /** <= upper layer */
   (void*)&ciaaDriverUart_uart1,    /** <= layer */
   NULL                             /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaUartDevices[] = {
   &ciaaDriverUart_device0,
   &ciaaDriverUart_device1
};

static ciaaDriverConstType const ciaaDriverUartConst = {
   ciaaUartDevices,
   2
};

/*==================[external data definition]===============================*/
/** \brief Uart 0 */
ciaaDriverUart_uartType ciaaDriverUart_uart0;

/** \brief Uart 1 */
ciaaDriverUart_uartType ciaaDriverUart_uart1;

/*==================[internal functions definition]==========================*/
static void ciaaDriverUart_rxIndication(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaDriverUart_uartType * uart = device->layer;

   ciaaSerialDevices_rxIndication(device->upLayer, uart->rxBuffer.length);
}

static void ciaaDriverUart_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaDriverUart_uartType * uart = device->layer;

   ciaaSerialDevices_txConfirmation(device->upLayer, uart->txBuffer.length);
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   return -1;
}

extern int32_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, uint32_t size)
{
   /* receive the data and forward to upper layer */
   ciaaDriverUart_uartType * uart = device->layer;

   if (size > uart->rxBuffer.length)
   {
      size = uart->rxBuffer.length;
   }

   /* copy received bytes to upper layer */
   ciaaPOSIX_memcpy(buffer, &uart->rxBuffer.buffer[0], size);

   return size;
}

extern int32_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, uint32_t const size)
{
   int32_t ret = 0;

   /* write data */
   ciaaDriverUart_uartType * uart = device->layer;

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

void ciaaDriverUart_init(void)
{
   uint8_t loopi;

   /* add uart driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverUartConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverUartConst.devices[loopi]);
   }
}


/*==================[interrupt hanlders]=====================================*/
extern void ciaaDriverUart_uart0_rxIndication(void)
{
   ciaaDriverUart_rxIndication(&ciaaDriverUart_device0);
}

extern void ciaaDriverUart_uart0_txConfirmation(void)
{
   ciaaDriverUart_txConfirmation(&ciaaDriverUart_device0);
}

extern void ciaaDriverUart_uart1_rxIndication(void)
{
   ciaaDriverUart_rxIndication(&ciaaDriverUart_device1);
}

extern void ciaaDriverUart_uart1_txConfirmation(void)
{
   ciaaDriverUart_txConfirmation(&ciaaDriverUart_device1);
}

/* hardware stubs to avoid compilation errors due to handler definition in oil file */
ISR(UART0_IRQHandler)
{
}

ISR(UART2_IRQHandler)
{
}

ISR(UART3_IRQHandler)
{
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/



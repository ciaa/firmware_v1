/* Copyright 2014, 2015 Mariano Cerdeiro, Esteban Volentini
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

/** \brief CIAA UART Driver for K60_120
 **
 ** Implements the UART Driver for K60_120
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
 * EV			Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150403 v0.0.2 EV   first operational version
 * 20140913 v0.0.1 MaCe first stub version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverUart_Internal.h"
#include "ciaaDriverUart.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "os.h"

#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include "fsl_mcg_hal.h"
#include "fsl_port_hal.h"
#include "fsl_uart_hal.h"


/*==================[macros and definitions]=================================*/
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

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

/** \brief Device for UART 2 */
static ciaaDevices_deviceType ciaaDriverUart_device2 = {
   "uart/2",                        /** <= driver name */
   ciaaDriverUart_open,             /** <= open function */
   ciaaDriverUart_close,            /** <= close function */
   ciaaDriverUart_read,             /** <= read function */
   ciaaDriverUart_write,            /** <= write function */
   ciaaDriverUart_ioctl,            /** <= ioctl function */
   NULL,                            /** <= seek function is not provided */
   NULL,                            /** <= upper layer */
   (void*)&ciaaDriverUart_uart2,    /** <= layer */
   NULL                             /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaUartDevices[] = {
   &ciaaDriverUart_device0,
   &ciaaDriverUart_device1,
   &ciaaDriverUart_device2
};

static ciaaDriverConstType const ciaaDriverUartConst = {
   ciaaUartDevices,
   3
};

/*==================[internal functions declaration]=========================*/
/** \brief Uart 0 */
ciaaDriverUart_uartType ciaaDriverUart_uart0;

/** \brief Uart 1 */
ciaaDriverUart_uartType ciaaDriverUart_uart1;

/** \brief Uart 2 */
ciaaDriverUart_uartType ciaaDriverUart_uart2;

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void ciaaDriverUart_rxIndication(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   //ciaaDriverUart_uartType * uart = device->layer;

   //ciaaSerialDevices_rxIndication(device->upLayer, uart->rxBuffer.length);
}
static void ciaaDriverUart_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   //ciaaDriverUart_uartType * uart = device->layer;

   //ciaaSerialDevices_txConfirmation(device->upLayer, uart->txBuffer.length);
}

/** \brief Initialize configuration variables and hardware port  */
void ciaaDriverUart_configInit(ciaaDevices_deviceType * device, uint8_t index)
{
   ciaaDriverUart_uartType * uart = device->layer;

	uart->instance = index;
   uart->config.baudRate = 19200;
   uart->config.bitCountPerChar = kUart8BitsPerChar;
   uart->config.parityMode = kUartParityDisabled;
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
   uart->config.stopBitCount = kUartOneStopBit;
#endif

   uart->base = UART5;
}
/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaDriverUart_uartType * uart = device->layer;
   uint32_t uartSourceClock;

   if (uart->instance == 0) {
      /* enable clock for PORTs */
      SIM_HAL_EnableClock(SIM, kSimClockGatePortE);
      /* Affects PORTE_PCR9 register */
      PORT_HAL_SetMuxMode(PORTE,9u,kPortMuxAlt3);
      /* Affects PORTE_PCR8 register */
      PORT_HAL_SetMuxMode(PORTE,8u,kPortMuxAlt3);
   }

   UART_HAL_Init(uart->base);

   /* UART clock source is either system or bus clock depending on instance */
   uartSourceClock = CLOCK_SYS_GetUartFreq(uart->instance);

  /* Initialize UART baud rate, bit count, parity and stop bit. */
   UART_HAL_SetBaudRate(uart->base, uartSourceClock, uart->config.baudRate);
   UART_HAL_SetBitCountPerChar(uart->base, uart->config.bitCountPerChar);
   UART_HAL_SetParityMode(uart->base, uart->config.parityMode);
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
   UART_HAL_SetStopBitCount(uart->base, uart->config.stopBitCount);
#endif

   UART_HAL_EnableTransmitter(uart->base);
   UART_HAL_EnableReceiver(uart->base);
   NVIC_EnableIRQ(UART5_RX_TX_IRQn);

   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int32_t ret = -1;

   UART_HAL_DisableTransmitter(uart->base);
   UART_HAL_DisableReceiver(uart->base);
   ret = 0;

   return ret;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int32_t ret = -1;

   if((device == ciaaDriverUartConst.devices[0]) ||
      (device == ciaaDriverUartConst.devices[1]) ||
      (device == ciaaDriverUartConst.devices[2]))
   {
      switch(request)
      {
         /* signal to start transmition */
         case ciaaPOSIX_IOCTL_STARTTX:
            ciaaDriverUart_txConfirmation(device);
         break;

         /* set serial port baudrate */
         case ciaaPOSIX_IOCTL_SET_BAUDRATE:
            uart->config.baudRate = (uint32_t)(param);
            /** @todo Cambiar velocidad en el puerto fisico */
            break;
      }
   }
   return ret;
}

extern ssize_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t * const buffer, size_t const size)
{
  // ciaaDriverUart_uartType * uart = device->layer;
   ssize_t ret = -1;

   return ret;
}

extern ssize_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   //ciaaDriverUart_uartType * uart = device->layer;
   ssize_t ret = -1;

   return ret;
}

void ciaaDriverUart_init(void)
{
   uint8_t loopi;

   /* add uart driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverUartConst.countOfDevices; loopi++) {
      /* initialize configuration variables and port */
      ciaaDriverUart_configInit(ciaaDriverUartConst.devices[loopi], loopi);

      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverUartConst.devices[loopi]);
   }
}


ISR(UART0_IRQHandler)
{

}


ISR(UART2_IRQHandler)
{

}

ISR(UART3_IRQHandler)
{

}

/*==================[interrupt handlers]=====================================*/
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


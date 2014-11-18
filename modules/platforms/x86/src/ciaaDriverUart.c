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
 * EsVo         Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140528 v0.0.1 MaCe initial version
 * 20141116 v0.0.2 EsVo add uart emulation via sockets
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverUart.h"
#include "ciaaDriverUart_Internal.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "OS_Internal.h"
#include "fcntl.h"

/*==================[macros and definitions]=================================*/
/** \brief Cygwin only support deprecated macro FASYNC */
 #ifndef O_ASYNC
   #define O_ASYNC FASYNC
 #endif
 
/** \brief Cygwin only support deprecated macro FASYNC */
#ifndef O_NONBLOCK
   #define O_NONBLOCK FNONBLOCK
#endif
 
/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
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
   NULL                              /** <= server handlers as lower layer */
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
   NULL                              /** <= server handlers as lower layer */
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

#ifdef ENABLE_UART_EMULATION
/** \brief Configure server socket to operate in asyncronous mode*/
int ciaaDriverUart_configureScokect(int socket) {
   int result;

   result = fcntl(socket, F_SETOWN, getpid());
   if (result < 0)
   {
      perror("Error setting signal process owner: ");
   }
   if (result >= 0) {
      result = fcntl(socket, F_GETFL, 0);
      if (result < 0)
      {
         perror("Error getting socket flags: ");
      }
      else
      {
         result = fcntl(socket, F_SETFL, result | O_ASYNC | O_NONBLOCK);
         if (result < 0)
         {
            perror("Error setting socket asincronous flags: ");
         }
      }
   }
   return (result);
}
 
/** \brief Process an SIGIO from emulator sockets */
static void ciaaDriverUart_signalHandler(int signal, siginfo_t * info, void * context)
{
   ciaaDriverUart_uartType * uart;
   int index;
   int result;

   for(index = ciaaDriverUartConst.countOfDevices - 1; index >= 0; index--) 
   {
      uart = (ciaaDriverUart_uartType *) ciaaUartDevices[index]->layer;
      if (!uart->client.conected)
      {
         uart->client.addressSize = sizeof(uart->client.address);
         uart->client.socket = accept(uart->server.socket, (struct sockaddr *) &(uart->client.address), &(uart->client.addressSize));
         if (uart->client.socket > 0) 
         {
            ciaaDriverUart_configureScokect(uart->client.socket);
            printf("Client Conected\r\n");
            uart->client.conected = true;
            ciaaDriverUart_txConfirmation(ciaaUartDevices[index]);
         }
      }
 
      if (uart->client.conected)
      {
         if (uart->client.sending > 0) 
         {
            uart->client.sending = 0;
            ciaaDriverUart_txConfirmation(ciaaUartDevices[index]);
         }
         
         result = recv(uart->client.socket, uart->rxBuffer.buffer, sizeof(uart->rxBuffer), MSG_DONTWAIT);
         if (result == 0) {
            printf("Client disconected\r\n");
            uart->client.conected = false;
            uart->client.sending = 0;
         } else if (result > 0) {
            uart->rxBuffer.length = result;
            ciaaDriverUart_rxIndication(ciaaUartDevices[index]);
         } 
      }
   }
}
#endif // ENABLE_UART_EMULATION

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
 #ifdef ENABLE_UART_EMULATION  
   ciaaDriverUart_uartType * uart;
   struct sigaction signalAction;
   int index;
   int result;

   for(index = ciaaDriverUartConst.countOfDevices - 1; index >= 0; index--) {
      if (ciaaDriverUartConst.devices[index] == device) break;
   }

   PreCallService();
   if (index >= 0) {
      uart = (ciaaDriverUart_uartType *) device->layer;
      bzero(uart, sizeof(* uart));

      signalAction.sa_sigaction = &ciaaDriverUart_signalHandler;
      signalAction.sa_flags = SA_SIGINFO;

      result = sigaction(SIGIO, &signalAction, NULL);
      if (result < 0)
      {
         perror("Error setting signal handler: ");
      }
      else
      {
         result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
         if (result > 0)
         {
            uart->server.socket = result;
         }
         else
         {
            perror("Error creating server socket: ");
         }
      }

      if (uart->server.socket) {
         result = ciaaDriverUart_configureScokect(uart->server.socket);
      }

      if (result >= 0)
      {
         uart->server.address.sin_family = AF_INET;
         uart->server.address.sin_addr.s_addr = INADDR_ANY;
         uart->server.address.sin_port = htons(2000 + index);

         result = bind(uart->server.socket, (struct sockaddr *) &(uart->server.address), sizeof(uart->server.address));

         if (result < 0)
         {
            perror("Error binding socket address: ");
         }
         else
         {
            result = listen(uart->server.socket, 1);
            if (result < 0) 
            {
               perror("Error listen on socket: ");
            }
         }
      }
   }
   PostCallService();
#endif // ENABLE_UART_EMULATION
   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
#ifdef ENABLE_UART_EMULATION
   ciaaDriverUart_uartType * uart = device->layer;

   PreCallService();
   if (uart->client.conected) {
      close(uart->client.socket);
   }
   close(uart->server.socket);
   PostCallService();
#endif // ENABLE_UART_EMULATION   
   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;
#ifdef ENABLE_UART_EMULATION
   ciaaDriverUart_uartType * uart = device->layer;

   if((device == ciaaDriverUartConst.devices[0]) ||
      (device == ciaaDriverUartConst.devices[1]) )
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTTX:
            if (uart->client.conected) {
               ciaaDriverUart_txConfirmation(device);
            }
         break;
      }
   }
#endif // ENABLE_UART_EMULATION
   return ret;
}

extern int32_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, uint32_t size)
{
   ciaaDriverUart_uartType * uart = device->layer;

   /* receive the data and forward to upper layer */
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
   ciaaDriverUart_uartType * uart = device->layer;

   int32_t ret = 0;
#ifdef ENABLE_UART_EMULATION   
   int32_t result;
#endif // ENABLE_UART_EMULATION
   
   /* write data */
   if (0 == uart->txBuffer.length)
   {
      /* copy data */
      ciaaPOSIX_memcpy(&uart->txBuffer.buffer, buffer, size);

      /* return lenght and set 0 for the next */
      ret = size;

      /* set length of the buffer */
      uart->txBuffer.length = size;

#ifdef ENABLE_UART_EMULATION
      if (uart->client.conected) {
         result = send(uart->client.socket, uart->txBuffer.buffer, uart->txBuffer.length, MSG_DONTWAIT);
         uart->client.sending = uart->txBuffer.length;
         uart->txBuffer.length = 0;
      }
#endif // ENABLE_UART_EMULATION
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



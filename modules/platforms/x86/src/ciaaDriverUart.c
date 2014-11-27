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
 * 20141121 v0.0.3 EsVo add host uart support
 * 20141116 v0.0.2 EsVo add uart emulation via sockets
 * 20140528 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverUart.h"
#include "ciaaDriverUart_Internal.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "os.h"
 
#ifdef CIAADRVUART_ENABLE_TRANSMITION
   #include "os_internal.h" 
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION
   #include "os_internal.h" 
   #include "fcntl.h"
#endif /* CIAADRVUART_ENABLE_EMULATION */

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
static void ciaaDriverUart_serialHandler(ciaaDevices_deviceType const * const device);

static void ciaaDriverUart_serverHandler(ciaaDevices_deviceType const * const device);

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
#if defined(CIAADRVUART_ENABLE_TRANSMITION) || defined(CIAADRVUART_ENABLE_EMULATION)

/** \brief Process an SIGIO from serial transmition or emulation sockets */
static void ciaaDriverUart_signalHandler(int signal, siginfo_t * info, void * context)
{
   ciaaDevices_deviceType * device;
   int index;

   for(index = ciaaDriverUartConst.countOfDevices - 1; index >= 0; index--) 
   {
      device = (ciaaDevices_deviceType *) ciaaUartDevices[index];
      #ifdef CIAADRVUART_ENABLE_TRANSMITION
         ciaaDriverUart_serialHandler(device);
      #endif /* CIAADRVUART_ENABLE_TRANSMITION */

      #ifdef CIAADRVUART_ENABLE_EMULATION
         ciaaDriverUart_serverHandler(device);
      #endif /* CIAADRVUART_ENABLE_EMULATION */      
   }   
}
#endif /* CIAADRVUART_ENABLE_TRANSMITION || CIAADRVUART_ENABLE_EMULATION */

#ifdef CIAADRVUART_ENABLE_TRANSMITION
static void ciaaDriverUart_serialHandler(ciaaDevices_deviceType const * const device) 
{
   ciaaDriverUart_uartType * uart = device->layer;
   
   if (uart->fileDescriptor > 0) 
   {
      
   }   
}
int ciaaDriverUart_serialInit(ciaaDevices_deviceType * device, uint8_t index)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int ret = 0; 

   bzero(uart, sizeof(ciaaDriverUart_uartType));
   
   /* Set baudreate 115200 */
   cfsetspeed(&uart->deviceOptions, B115200);

   /* Set to 8 Data bits, Parity None, 1 Stop bit */
   uart->deviceOptions.c_cflag |= CS8; 
   uart->deviceOptions.c_cflag &= ~PARENB;
   uart->deviceOptions.c_cflag &= ~CSTOPB;
   
   
   /* Set without hardware flow control */
   uart->deviceOptions.c_cflag |= CLOCAL;
   uart->deviceOptions.c_cflag &= ~CRTSCTS;
   
   /* Set RAW mode */
   uart->deviceOptions.c_lflag &= ~(ICANON | ECHO | ISIG);   
   uart->deviceOptions.c_oflag &= ~OPOST;   
   
   return ret;
}

int ciaaDriverUart_serialOpen(ciaaDevices_deviceType * device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int ret; 
   
   PreCallService();
   ret = open(uart->deviceName, O_RDWR | O_NOCTTY | O_NDELAY);
   if (ret > 0)
   {
      uart->fileDescriptor = ret;
   }
   else
   {   
      uart->fileDescriptor = 0;
      perror("Error open serial port: ");      
   }
   if (uart->fileDescriptor) 
   {
      ret =  tcsetattr(uart->fileDescriptor, TCSANOW, &uart->deviceOptions);
   }
   PostCallService();   
   return ret;   
}

int ciaaDriverUart_serialClose(ciaaDevices_deviceType const * const device)
{
   ciaaDriverUart_uartType * uart = device->layer;

   if (uart->fileDescriptor > 0) 
   {
      PreCallService();
      close(uart->fileDescriptor);
      PostCallService();
      
   }
   return 0;
}
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION
/** \brief Configure server socket to operate in asyncronous mode*/
int ciaaDriverUart_configureSocket(int socket) {
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

static void ciaaDriverUart_serverHandler(ciaaDevices_deviceType const * const device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int ret;

   if (uart->server.socket > 0) 
   {
      if (!uart->client.conected)
      {
         uart->client.addressSize = sizeof(uart->client.address);
         uart->client.socket = accept(uart->server.socket, (struct sockaddr *) &(uart->client.address), &(uart->client.addressSize));
         if (uart->client.socket > 0) 
         {
            ciaaDriverUart_configureSocket(uart->client.socket);
            printf("Client Conected\r\n");
            uart->client.conected = true;
            ciaaDriverUart_txConfirmation(device);
         }
      }

      if (uart->client.conected)
      {
         if (uart->client.sending > 0) 
         {
            uart->client.sending = 0;
            ciaaDriverUart_txConfirmation(device);
         }
         
         ret = recv(uart->client.socket, uart->rxBuffer.buffer, sizeof(uart->rxBuffer), MSG_DONTWAIT);
         if (ret == 0) {
            printf("Client disconected\r\n");
            uart->client.conected = false;
            uart->client.sending = 0;
         } else if (ret > 0) {
            uart->rxBuffer.length = ret;
            ciaaDriverUart_rxIndication(device);
         }
      }
   }
}

/** \brief Start server emulation for serial ports */
int ciaaDriverUart_serverInit(ciaaDevices_deviceType * device, uint8_t index) 
{
   ciaaDriverUart_uartType * uart = device->layer;
   struct sigaction signalAction;
   int port = -1;
   int ret;

#ifdef CIAADRVUART_TCP_PORT_0
      if (0 == index) port = CIAADRVUART_TCP_PORT_0;
#endif
#ifdef CIAADRVUART_TCP_PORT_1
      if (1 == index) port = CIAADRVUART_TCP_PORT_1;
#endif

   bzero(&signalAction, sizeof(signalAction));
   signalAction.sa_sigaction = &ciaaDriverUart_signalHandler;
   signalAction.sa_flags = SA_SIGINFO;
   ret = sigaction(SIGIO, &signalAction, NULL);
   if (ret < 0)
   {
      perror("Error setting signal handler: ");
   }
   else 
   {
      bzero(uart, sizeof(ciaaDriverUart_uartType));
      if ((index >= 0) && (port > 0)) 
      {
         uart->server.address.sin_family = AF_INET;
         uart->server.address.sin_addr.s_addr = INADDR_ANY;
         uart->server.address.sin_port = htons(port);
         ret = 0;
      } 
      else
      {
         ret = -1;
      }
   }
   
   return ret;
}
   
int ciaaDriverUart_serverOpen(ciaaDevices_deviceType * device) 
{
   ciaaDriverUart_uartType * uart = device->layer;
   int ret;

   PreCallService();
   
   ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (ret > 0)
   {
      uart->server.socket = ret;
   }
   else
   {
      uart->server.socket = 0;
      perror("Error creating server socket: ");
   }

   if (uart->server.socket) 
   {
      ret = ciaaDriverUart_configureSocket(uart->server.socket);
   }

   if (ret >= 0)
   {
      ret = bind(uart->server.socket, (struct sockaddr *) &(uart->server.address), sizeof(uart->server.address));

      if (ret < 0)
      {
         perror("Error binding socket address: ");         
      }
      else
      {
         ret = listen(uart->server.socket, 1);
         if (ret < 0) 
         {
            perror("Error listen on socket: ");
         }
      }
      if (ret < 0) 
      {
         close(uart->server.socket);
         uart->server.socket = 0;
      }
   }   
   PostCallService();
   
   return ret;
}

int ciaaDriverUart_serverClose(ciaaDevices_deviceType const * const device) 
{
   ciaaDriverUart_uartType * uart = device->layer;

   if (uart->server.socket > 0) 
   {
      PreCallService();
      if (uart->client.conected) 
      {
         close(uart->client.socket);
      }
      close(uart->server.socket);
      PostCallService();
   }
   
   return 0;
}
#endif /* CIAADRVUART_ENABLE_EMULATION */

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
#ifdef CIAADRVUART_ENABLE_TRANSMITION  
   ciaaDriverUart_serialOpen(device);
#endif /* CIAADRVUART_ENABLE_TRANSMITION */
   
#ifdef CIAADRVUART_ENABLE_EMULATION  
   ciaaDriverUart_serverOpen(device); 
#endif /* CIAADRVUART_ENABLE_EMULATION */

   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
#ifdef CIAADRVUART_ENABLE_TRANSMITION  
   ciaaDriverUart_serialClose(device);
#endif /* CIAADRVUART_ENABLE_TRANSMITION */
   
#ifdef CIAADRVUART_ENABLE_EMULATION  
   ciaaDriverUart_serverClose(device);
#endif /* CIAADRVUART_ENABLE_EMULATION */

   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;
 
#if defined(CIAADRVUART_ENABLE_TRANSMITION) || defined(CIAADRVUART_ENABLE_EMULATION)
   ciaaDriverUart_uartType * uart = device->layer;

   if((device == ciaaDriverUartConst.devices[0]) ||
      (device == ciaaDriverUartConst.devices[1]) )
   {
      switch(request)
      {
#ifdef CIAADRVUART_ENABLE_TRANSMITION
         case ciaaPOSIX_IOCTL_SET_BAUDRATE:
            ret = cfsetspeed(&uart->deviceOptions, (speed_t)(param));            
            if ((0 == ret ) && (0 != uart->fileDescriptor))
            {
               ret = tcsetattr(uart->fileDescriptor, TCSANOW, &uart->deviceOptions);
            }
         break;
#endif /* CIAADRVUART_ENABLE_TRANSMITION */  

#ifdef CIAADRVUART_ENABLE_EMULATION
         case ciaaPOSIX_IOCTL_STARTTX:
            if (uart->client.conected) {
               ciaaDriverUart_txConfirmation(device);
            }
         break;
#endif /* CIAADRVUART_ENABLE_EMULATION */
      }
   }
#endif /* CIAADRVUART_ENABLE_TRANSMITION || CIAADRVUART_ENABLE_EMULATION */
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
#endif /* CIAADRVUART_ENABLE_EMULATION */
   
   /* write data */
   if (0 == uart->txBuffer.length)
   {
      /* copy data */
      ciaaPOSIX_memcpy(&uart->txBuffer.buffer, buffer, size);

      /* return lenght and set 0 for the next */
      ret = size;

      /* set length of the buffer */
      uart->txBuffer.length = size;

#ifdef CIAADRVUART_ENABLE_TRANSMITION
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef ENABLE_UART_EMULATION
      if (uart->client.conected) {
         result = send(uart->client.socket, uart->txBuffer.buffer, uart->txBuffer.length, MSG_DONTWAIT);
         uart->client.sending = uart->txBuffer.length;
         uart->txBuffer.length = 0;
      }
#endif /* CIAADRVUART_ENABLE_EMULATION */
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
      
#ifdef CIAADRVUART_ENABLE_TRANSMITION
      ciaaDriverUart_serialInit(ciaaDriverUartConst.devices[loopi], loopi);
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION
      ciaaDriverUart_serverInit(ciaaDriverUartConst.devices[loopi], loopi);
#endif /* CIAADRVUART_ENABLE_EMULATION */
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



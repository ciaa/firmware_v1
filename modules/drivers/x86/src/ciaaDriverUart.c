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
#include "ciaaDriverUart_Internal.h"
#ifdef CIAADRVUART_ENABLE_FUNCIONALITY
   #include <pthread.h>
   #include <fcntl.h>
   #include <signal.h>
   #include <stdio.h>
   #include <string.h>
   #include <unistd.h>
   #include <stdlib.h>
   #include <errno.h>
#endif /* CIAADRVUART_ENABLE_FUNCIONALITY */

#include "ciaaDriverUart.h"
#include "ciaaPOSIX_stdio.h"
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

#ifdef CIAADRVUART_ENABLE_TRANSMITION

/* Constant with filename of serial port maped to Uart 0 */
static const char ciaaDriverUart_name0[] = CIAADRVUART_PORT_SERIAL_0;

/* Constant with filename of serial port maped to Uart 1 */
static const char ciaaDriverUart_name1[] = CIAADRVUART_PORT_SERIAL_1;

/* Constant with filenames of host serial ports */
static const char * ciaaDriverUart_serialPorts[] = {
   ciaaDriverUart_name0,
   ciaaDriverUart_name1
};

#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION

/* Constant with TCP port to server serial emulation */
static const int ciaaDriverUart_serverPorts[] = {
   CIAADRVUART_TCP_PORT_0,
   CIAADRVUART_TCP_PORT_1
};

#endif /* CIAADRVUART_ENABLE_EMULATION */

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

#ifdef CIAADRVUART_ENABLE_TRANSMITION
/** \brief Initialize host serial port name and options */
void ciaaDriverUart_serialInit(ciaaDevices_deviceType * device, uint8_t index)
{
   ciaaDriverUart_uartType * uart = device->layer;

   uart->deviceName = ciaaDriverUart_serialPorts[index];

   /* Set RAW mode */
   cfmakeraw(&uart->deviceOptions);

   /* Set baudreate 115200 */
   cfsetspeed(&uart->deviceOptions, B115200);

   /* Set to 8 Data bits, Parity None, 1 Stop bit */
   uart->deviceOptions.c_cflag |= CS8;
   uart->deviceOptions.c_cflag &= ~PARENB;
   uart->deviceOptions.c_cflag &= ~CSTOPB;

   /* Set without hardware flow control */
   uart->deviceOptions.c_cflag |= CLOCAL;
   uart->deviceOptions.c_cflag &= ~CRTSCTS;
}

/** \brief Handle the serial port transmission and reception in a separated thread */
static void * ciaaDriverUart_serialHandler(ciaaDevices_deviceType const * const device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int result = 0;

   /* while not KILL signal is received */
   while (!result)
   {
      /* if data avaiable to send transmit it to host port */
      if (uart->txBuffer.length)
      {
         result = write(uart->fileDescriptor, uart->txBuffer.buffer, uart->txBuffer.length);
         uart->txBuffer.length -= result;
         if (0 == uart->txBuffer.length)
         {
            ciaaDriverUart_txConfirmation(device);
         }
      }
      /* try receive data from the host port */
      result = read(uart->fileDescriptor, uart->rxBuffer.buffer + uart->rxBuffer.length, sizeof(uart->rxBuffer) - uart->rxBuffer.length);
      if (result > 0) {
         uart->rxBuffer.length += result;
         ciaaDriverUart_rxIndication(device);
      }
      result = usleep(100);
   }

   return NULL;
}

/** \brief Open and configure the host port and create a thread to handle the comunication */
ciaaDevices_deviceType * ciaaDriverUart_serialOpen(ciaaDevices_deviceType * device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int result;

   /* if host serial port name is defined */
   if (0 != uart->deviceName[0])
   {
      /* open host serial port */
      result = open(uart->deviceName, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
      if (result > 0)
      {
         uart->fileDescriptor = result;
      }
      else
      {
         perror("Error open serial port: ");
      }
      if (uart->fileDescriptor) {
         /* configure serial port opstions */
         /* Issue #173, Under MAC OS X the function returns error even when the port is properly configured */
         #if 0
            /* This is the correct code, but in MAC OS X returns error if an thread was created previously to this call */
            result = tcsetattr(uart->fileDescriptor, TCSANOW, &uart->deviceOptions);
         #else
            /* This is a turn around to avoid the error on MAC OS X, in Linux it's unnecessary */
            result = 0;
            tcsetattr(uart->fileDescriptor, TCSANOW, &uart->deviceOptions);
         #endif
         if (result)
         {
            perror("Error setting serial port parameters: ");
         }

         /* create thread to handle serial port trasmission and reception */
         result += pthread_create(&uart->handlerThread, NULL, (void *) &ciaaDriverUart_serialHandler, device);
         if (result)
         {
            perror("Error creating handler thread: ");
         }
      }

      /* if error release was ocurred device pointer */
      if (result) {
         close(uart->fileDescriptor);
         device = NULL;
      }
   }
   return device;
}


#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION
/** \brief Initialize TCP server address and port */
void ciaaDriverUart_serverInit(ciaaDevices_deviceType * device, uint8_t index)
{
   ciaaDriverUart_uartType * uart = device->layer;

   uart->rxBuffer.length = 0;

   uart->serverAddress.sin_family = AF_INET;
   uart->serverAddress.sin_addr.s_addr = INADDR_ANY;
   uart->serverAddress.sin_port = htons(ciaaDriverUart_serverPorts[index]);
}

/** \brief Handle the server conections, transmission and reception in a separated thread */
static void * ciaaDriverUart_serverHandler(ciaaDevices_deviceType const * const device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   struct sockaddr_in clientAddress;
   socklen_t addressSize = sizeof(clientAddress);
   int clientSocket = 0;
   int result = 0;

   /* while not KILL signal is received */
   while (!result)
   {
      /* if not client conected try to accept a new client */
      if (clientSocket <= 0)
      {
         clientSocket = accept(uart->fileDescriptor, (struct sockaddr *) &clientAddress, &addressSize);
         if (clientSocket > 0)
         {
            printf("Client Conected\r\n");
         }
      }

      /* if a client is conected */
      if (clientSocket > 0)
      {
         /* if data avaiable tod send transmit it to client */
         if (uart->txBuffer.length > 0)
         {
            result = send(clientSocket, uart->txBuffer.buffer, uart->txBuffer.length, MSG_DONTWAIT);
            uart->txBuffer.length -= result;
            if (0 == uart->txBuffer.length)
            {
               ciaaDriverUart_txConfirmation(device);
            }
         }

         /* try to receive data from client */
         if(uart->rxBuffer.length < sizeof(uart->rxBuffer.buffer))
         {
            result = recv(clientSocket, uart->rxBuffer.buffer + uart->rxBuffer.length, sizeof(uart->rxBuffer) - uart->rxBuffer.length, MSG_DONTWAIT);
            if (0 == result)
            {
               /* the cliente was disconected */
               printf("Client disconected\r\n");
               close(clientSocket);
               clientSocket = 0;
            }
            else if (result > 0)
            {
               /* the cliente was send data */
               uart->rxBuffer.length += result;
               ciaaDriverUart_rxIndication(device);
            } else
            {
               /* nothing to do */
            }
         }
      }
      result = usleep(100);
   }

   /* close client conection if remains open */
   if (clientSocket > 0)
      close(clientSocket);

   return NULL;
}

/** \brief Start the server and create a thread to handle the comunication */
ciaaDevices_deviceType * ciaaDriverUart_serverOpen(ciaaDevices_deviceType * device)
{
   ciaaDriverUart_uartType * uart = device->layer;
   int result;

   /* if server port is defined */
   if (0 != uart->serverAddress.sin_port)
   {
      /* create a server socket */
      result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if (result > 0)
      {
         uart->fileDescriptor = result;
      }
      else
      {
         perror("Error creating server socket: ");
      }

      if (uart->fileDescriptor)
      {
         /* retrieve current flags of server sockets */
         result = fcntl(uart->fileDescriptor, F_GETFL, 0);
         if (result < 0)
         {
            perror("Error getting file descriptor flags: ");
         }

         /* set server flags to operate in non block mode */
         result = fcntl(uart->fileDescriptor, F_SETFL, result | O_NONBLOCK);
         if (result < 0) perror("Error setting file descriptor asincronous flags: ");

         /* bind socket to server address and port */
         result += bind(uart->fileDescriptor, (struct sockaddr *) &(uart->serverAddress), sizeof(uart->serverAddress));
         if (result)
         {
            perror("Error binding socket address: ");
         }

         /* start server to lisen client requests */
         result += listen(uart->fileDescriptor, 1);
         if (result < 0)
         {
            perror("Error listen on socket: ");
         }

          /* create thread to serves conections, trasmission and reception */
         result += pthread_create(&uart->handlerThread, NULL, (void *) &ciaaDriverUart_serverHandler, device);
         if (result)
         {
            perror("Error creating handler thread: ");
         }
      }

      /* if error release was ocurred device pointer */
      if (result)
      {
         close(uart->fileDescriptor);
         device = NULL;
      }
   }
   return device;
}
#endif /* CIAADRVUART_ENABLE_EMULATION */

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
#ifdef CIAADRVUART_ENABLE_TRANSMITION
   if (0 != device)
   {
      device = ciaaDriverUart_serialOpen(device);
   }
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION
   if (0 != device) {
      device = ciaaDriverUart_serverOpen(device);
   }
#endif /* CIAADRVUART_ENABLE_EMULATION */

   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
#ifdef CIAADRVUART_ENABLE_FUNCIONALITY
   ciaaDriverUart_uartType * uart = device->layer;

   if (uart->fileDescriptor > 0)
   {
      /* Signal thread to exit and wait it */
      pthread_kill(uart->handlerThread, SIGALRM);
      pthread_join(uart->handlerThread, NULL);

      /* Close serial port descriptor */
      close(uart->fileDescriptor);
   }
#endif /* CIAADRVUART_ENABLE_FUNCIONALITY */
   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;

#ifdef CIAADRVUART_ENABLE_FUNCIONALITY
   ciaaDriverUart_uartType * uart = device->layer;

   if((device == ciaaDriverUartConst.devices[0]) ||
      (device == ciaaDriverUartConst.devices[1]) )
   {
      switch(request)
      {
         /* signal to start transmition */
         case ciaaPOSIX_IOCTL_STARTTX:
            if (uart->fileDescriptor)
            {
               ciaaDriverUart_txConfirmation(device);
            }
         break;

#ifdef CIAADRVUART_ENABLE_TRANSMITION
         /* set serial port baudrate */
         case ciaaPOSIX_IOCTL_SET_BAUDRATE:
            ret = cfsetspeed(&uart->deviceOptions, (speed_t)(param));
            if ((0 == ret ) && (0 != uart->fileDescriptor))
            {
               ret = tcsetattr(uart->fileDescriptor, TCSANOW, &uart->deviceOptions);
            }
         break;
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

      }
   }
#endif /* CIAADRVUART_ENABLE_FUNCIONALITY */
   return ret;
}

extern ssize_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   int i;
   ciaaDriverUart_uartType * uart = device->layer;
   ssize_t ret = size;

   /* receive the data and forward to upper layer */
   if (size > uart->rxBuffer.length)
   {
      ret = uart->rxBuffer.length;
   }
   /* copy received bytes to upper layer */
   ciaaPOSIX_memcpy(buffer, &uart->rxBuffer.buffer[0], ret);


   uart->rxBuffer.length -= ret;
   /* TODO: here is a synchronization problem: if a new packet arrives then
    * buffer length changes and data may be overwritten */

   for(i = 0; i < uart->rxBuffer.length; ++i)
   {
      uart->rxBuffer.buffer[i] = uart->rxBuffer.buffer[i+ret];
   }

   return ret;
}

extern ssize_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ciaaDriverUart_uartType * uart = device->layer;

   int32_t ret = 0;

   /* write data */
   if (0 == uart->txBuffer.length)
   {
      /* copy data */
      ciaaPOSIX_memcpy(&uart->txBuffer.buffer[0], buffer, size);

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

#ifdef CIAADRVUART_ENABLE_TRANSMITION
      /* initialize host name and options port */
      ciaaDriverUart_serialInit(ciaaDriverUartConst.devices[loopi], loopi);
#endif /* CIAADRVUART_ENABLE_TRANSMITION */

#ifdef CIAADRVUART_ENABLE_EMULATION
      /* initialize server address and port */
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



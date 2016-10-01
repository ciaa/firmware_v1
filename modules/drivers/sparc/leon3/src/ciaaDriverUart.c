/* Copyright 2016, Gerardo Puga
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

/** \brief CIAA UART Driver for the SPARC/LEON3 architecture
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup UART UART Drivers
 ** @{ */

/*==================[inclusions]=============================================*/


#include "ciaaDriverUart.h"
#include "ciaaDriverUart_Internal.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_ioctl_serial.h"

#include "grlib.h"

/*==================[macros and definitions]=================================*/


#define SPARC_MAX_UART_COUNT      4

#define SPARC_DRIVER_UART_NOMINAL_DEVICE_NAME_STRING "uart/X"
#define SPARC_DRIVER_UART_NOMINAL_DEVICE_NAME_STRLEN 6


/*==================[internal data declaration]==============================*/




/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/


char sparcStaticUartDeviceNamesBuffer[(SPARC_DRIVER_UART_NOMINAL_DEVICE_NAME_STRLEN+1) * SPARC_MAX_UART_COUNT];

sparcDriverUartInfoType sparcDriverUartInfo[SPARC_MAX_UART_COUNT];


uint32 sparcDriverUartCount;


/*==================[external data definition]===============================*/


/*==================[internal functions definition]==========================*/


void sparcDriverUartInitQueue(sparcDriverUartQueueType *queue)
{
   queue->head = 0;
   queue->tail = 0;
}


sint32 sparcDriverUartQueueIsEmpty(sparcDriverUartQueueType *queue)
{
   sint32 queueIsEmptyAnswer;

   queueIsEmptyAnswer = 1;

   if (queue->head != queue->tail)
   {
      queueIsEmptyAnswer = 0;
   }

   return queueIsEmptyAnswer;
}


sint32 sparcDriverUartQueueIsFull(sparcDriverUartQueueType *queue)
{
   sint32 queueIsFullAnswer;
   uint32 nextTail;

   queueIsFullAnswer = 0;

   /* Determine if we have the space. In doing so
    * avoid doing division, because we might not
    * have the hardware resources to do it fast
    * (sparc mul/div instructions might be disabled)*/
   nextTail = queue->tail + 1;
   if (nextTail >= SPARC_DRIVER_QUEUE_LENGHT)
   {
      nextTail -= SPARC_DRIVER_QUEUE_LENGHT;
   }

   if (queue->head == nextTail)
   {
      queueIsFullAnswer = 1;
   }

   return queueIsFullAnswer;
}


void sparcDriverUartQueuePush(sparcDriverUartQueueType *queue, uint8 dataItem)
{
   uint32 nextTail;

   /* Determine if we have the space. In doing so
    * avoid doing division, because we might not
    * have the hardware resources to do it fast
    * (sparc mul/div instructions might be disabled)*/
   nextTail = queue->tail + 1;
   if (nextTail >= SPARC_DRIVER_QUEUE_LENGHT)
   {
      nextTail -= SPARC_DRIVER_QUEUE_LENGHT;
   }

   if (queue->head != nextTail)
   {
      /* the is at least one available space on the circular buffer.
       * the current next pointer value is the index of that position. */
      queue->buffer[queue->tail] = dataItem;

      /* Update the tail of the queue */
      queue->tail = nextTail;
   }
}


sint32 sparcDriverUartQueuePop(sparcDriverUartQueueType *queue, uint8 *dataItemPtr)
{
   sint32 returnValue;

   returnValue = -1;

   if (queue->head != queue->tail)
   {
      /* there is at least one element on the queue */
      returnValue = 0;

      *dataItemPtr = queue->buffer[queue->head];

      /* Avoid doing divisions, look elsewhere for the reason */
      queue->head++;
      if (queue->head >= SPARC_DRIVER_QUEUE_LENGHT)
      {
         queue->head -= SPARC_DRIVER_QUEUE_LENGHT;
      }
   }

   return returnValue;
}


void sparcDriverUartDetectCoreConfiguration()
{
   grDeviceRegisterValue controlRegisterValue;
   grPlugAndPlayAPBDeviceTableEntryType apbUartInfo;
   uint32 deviceIndex;

   /* Detect the number of UARTs available, and the base address of each
    * using the AMBA PnP system. */
   sparcDriverUartCount = 0;
   while ((sparcDriverUartCount< SPARC_MAX_UART_COUNT)
         && (grWalkPlugAndPlayAPBDeviceTable(GRLIB_PNP_VENDOR_ID_GAISLER_RESEARCH, GRLIB_PNP_DEVICE_ID_APBUART, &apbUartInfo, sparcDriverUartCount) >= 0))
   {
      sparcDriverUartInfo[sparcDriverUartCount].baseAddress = apbUartInfo.address;
      sparcDriverUartInfo[sparcDriverUartCount].irq         = apbUartInfo.irq;

      /* Detect the presence of FIFOS on this UART */
      controlRegisterValue = grRegisterRead(apbUartInfo.address, GRLIB_APBUART_CONTROL_REGISTER);
      if (controlRegisterValue & GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_FA)
      {
         sparcDriverUartInfo[sparcDriverUartCount].hasFIFOs = 1;
      } else {
         sparcDriverUartInfo[sparcDriverUartCount].hasFIFOs = 0;
      }

      sparcDriverUartCount++;
   }
}


void sparcDriverUpdateControlRegister(sparcDriverUartInfoType *uartDataStructPtr)
{
   grDeviceRegisterValue controlRegisterValue;

   controlRegisterValue = 0;

   /*
    * Validate configuration
    * */
   sparcAssert(uartDataStructPtr->numberOfStopBits >= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->numberOfStopBits <= 2, "Invalid configuration");
   sparcAssert(uartDataStructPtr->flowControl <= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->useParity <= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->useOddParity <= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->loopbackEnabled <= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->externalClkEnabled <= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->txInterruptEnabled <= 1, "Invalid configuration");
   sparcAssert(uartDataStructPtr->rxInterruptEnabled <= 1, "Invalid configuration");

   /*
    * Set user configuration flags
    * */
   if (uartDataStructPtr->numberOfStopBits == 2)   controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_NS;
   if (uartDataStructPtr->flowControl == 1)        controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_FL;
   if (uartDataStructPtr->useParity == 1)          controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_PE;
   if (uartDataStructPtr->useOddParity == 1)       controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_PS;
   if (uartDataStructPtr->loopbackEnabled == 1)    controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_LB;
   if (uartDataStructPtr->externalClkEnabled == 1) controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_EC;

   /*
    * Set the device control bits, and enable interrupts if
    * */

   /* Both the receiver and the transmitter are on at all times */
   controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TE; /* Transmitter enable */
   controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RE; /* Receiver enable */

   if (uartDataStructPtr->hasFIFOs == 1)
   {
      /*
       * Tx Interrupts
       * */
      if (uartDataStructPtr->txInterruptEnabled == 1)
      {
         /* Generate tx interrupts whenever the FIFO is at least half empty */
         controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TF; /* Transmitter FIFO interrupt enable */
      }

      /*
       * Rx Interrupts
       * */
      if (uartDataStructPtr->rxInterruptEnabled == 1)
      {
         /* Generate rx interrupts whenever the FIFO is at least half full, or if it is less than half full but no new characters have been
          * received for the last 4 and 1/2 character intervals. */
         controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RI; /* Receiver interrupt enable */
         controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_DI; /* Delayed interrupt enable */
         controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RF; /* Receiver FIFO interrupt enable */
      }

   } else {

      /*
       * Tx Interrupts
       * */
      if (uartDataStructPtr->txInterruptEnabled == 1)
      {
         /* Generate tx interrupts whenever the holding register goes from full to empty */
         controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TI; /* Transmitter interrupt enable */
      }

      /*
       * Rx Interrupts
       * */
      if (uartDataStructPtr->rxInterruptEnabled)
      {
         /* Generate rx interrupts whenever the holding register goes from empty to occupied */
         controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RI; /* Receiver interrupt enable */
      }
   }

   grRegisterWrite(uartDataStructPtr->baseAddress, GRLIB_APBUART_CONTROL_REGISTER, controlRegisterValue);
}


void sparcDriverUpdateScalerRegister(sparcDriverUartInfoType *uartDataStructPtr)
{
   uint32 quotientNumerator, quotientDenominator;
   uint32 scalerInputClockFrequency;
   uint32 clockCyclesPerScalerTick;
   grDeviceRegisterValue scalerRegisterValue;

   scalerInputClockFrequency = grGetSystemClockFrequencyValue();

   /* calculate the quotient, doing rounding */
   quotientNumerator        = scalerInputClockFrequency;
   quotientDenominator      = uartDataStructPtr->baudrate * 8;
   clockCyclesPerScalerTick = (quotientNumerator + (quotientDenominator / 2)) / quotientDenominator;

   /* Substract 1 from the reload value before writing to the scaler reload
      register, because the scaler reload happens when the scaler underflows,
      not when its value reaches 0 */
   scalerRegisterValue = clockCyclesPerScalerTick - 1;

   grRegisterWrite(uartDataStructPtr->baseAddress, GRLIB_APBUART_SCALER_REGISTER, scalerRegisterValue);
}


void sparcDriverEnableRxInterrupts(sparcDriverUartInfoType *uartDataStructPtr)
{
   uartDataStructPtr->rxInterruptEnabled = 1;

   sparcDriverUpdateControlRegister(uartDataStructPtr);
}


void sparcDriverDisableRxInterrupts(sparcDriverUartInfoType *uartDataStructPtr)
{
   uartDataStructPtr->rxInterruptEnabled = 0;

   sparcDriverUpdateControlRegister(uartDataStructPtr);
}


void sparcDriverEnableTxInterrupts(sparcDriverUartInfoType *uartDataStructPtr)
{
   uartDataStructPtr->txInterruptEnabled = 1;

   sparcDriverUpdateControlRegister(uartDataStructPtr);
}


void sparcDriverDisableTxInterrupts(sparcDriverUartInfoType *uartDataStructPtr)
{
   uartDataStructPtr->txInterruptEnabled = 0;

   sparcDriverUpdateControlRegister(uartDataStructPtr);
}


void sparcDriverKickstartTxInterrupts(sparcDriverUartInfoType *uartDataStructPtr)
{

   if (uartDataStructPtr->hasFIFOs == 1)
   {
      /*
       * Nothing to do here, because the the transmission FIFO interrupt is a level
       * interrupt: it will stay on for as long as the condition remains the same
       * (transmission FIFO half empty).
       *
       * As soon as the transmission interrupts are enabled, an interrupt will be
       * kicked to life and transmission will start.
       *
       * */

   } else {

      /*
       * Now here we are in some deep shit.
       *
       * The standard transmitter interrupt of the APBUART core is not a level interrupt,
       * but is only emitted when the holding register transitions from being full to empty.
       *
       * That means that in order for the transmission to start it is no enough to enable the
       * transmission interrupt, we must also manually load the holding register with the
       * first byte or otherwise no further interrupts will ever be generated and nothing
       * will be transmitted.
       *
       * */

      /* If interrupts are already enabled, no kickstart is needed because the
       * next transmitter interrupt will send the data anyway. */
      if (uartDataStructPtr->txInterruptEnabled == 0)
      {
         ciaaSerialDevices_txConfirmation(uartDataStructPtr->deviceDataStructure, 1);
      }
   }
}


sint32 sparcDriverSetBaudrate(sparcDriverUartInfoType *uartDataStructPtr, uint32 ciaaBaudrate)
{
   sint32 retValue;

   retValue = -1;

   switch (ciaaBaudrate) {
   case ciaaBAUDRATE_300 :   uartDataStructPtr->baudrate =    300; retValue = 0; break;
   case ciaaBAUDRATE_600 :   uartDataStructPtr->baudrate =    600; retValue = 0; break;
   case ciaaBAUDRATE_1200:   uartDataStructPtr->baudrate =   1200; retValue = 0; break;
   case ciaaBAUDRATE_1800:   uartDataStructPtr->baudrate =   1800; retValue = 0; break;
   case ciaaBAUDRATE_2400:   uartDataStructPtr->baudrate =   2400; retValue = 0; break;
   case ciaaBAUDRATE_4800:   uartDataStructPtr->baudrate =   4800; retValue = 0; break;
   case ciaaBAUDRATE_9600:   uartDataStructPtr->baudrate =   9600; retValue = 0; break;
   case ciaaBAUDRATE_14400:  uartDataStructPtr->baudrate =  14400; retValue = 0; break;
   case ciaaBAUDRATE_19200:  uartDataStructPtr->baudrate =  19200; retValue = 0; break;
   case ciaaBAUDRATE_38400:  uartDataStructPtr->baudrate =  38400; retValue = 0; break;
   case ciaaBAUDRATE_57600:  uartDataStructPtr->baudrate =  57600; retValue = 0; break;
   case ciaaBAUDRATE_115200: uartDataStructPtr->baudrate = 115200; retValue = 0; break;
   case ciaaBAUDRATE_230400: uartDataStructPtr->baudrate = 230400; retValue = 0; break;
   case ciaaBAUDRATE_460800: uartDataStructPtr->baudrate = 460800; retValue = 0; break;
   case ciaaBAUDRATE_921600: uartDataStructPtr->baudrate = 921600; retValue = 0; break;
   default:
      break;
   }

   if (retValue == 0)
   {
      sparcDriverUpdateScalerRegister(uartDataStructPtr);
   }

   return retValue;
}




void sparcDriverUartInitializeControlStructures()
{
   uint32 uartIndex;

   for (uartIndex = 0; uartIndex < sparcDriverUartCount; uartIndex++)
   {
      /* Set the initial configuration */
      sparcDriverUartInfo[uartIndex].numberOfStopBits      = 1;
      sparcDriverUartInfo[uartIndex].flowControl           = 0;
      sparcDriverUartInfo[uartIndex].useParity             = 0;
      sparcDriverUartInfo[uartIndex].useOddParity          = 0;
      sparcDriverUartInfo[uartIndex].loopbackEnabled       = 0;

      sparcDriverUartInfo[uartIndex].externalClkEnabled    = 0;
      sparcDriverUartInfo[uartIndex].externalClkFrequency  = 40000000; /* some value */

      sparcDriverUartInfo[uartIndex].baudrate              = SPARC_DRIVER_DEFAULT_BAUDRATE;

      sparcDriverUartInfo[uartIndex].txInterruptEnabled    = 0;
      sparcDriverUartInfo[uartIndex].rxInterruptEnabled    = 0;

      sparcDriverUartInfo[uartIndex].deviceDataStructure.path =
            sparcStaticUartDeviceNamesBuffer + uartIndex * (SPARC_DRIVER_UART_NOMINAL_DEVICE_NAME_STRLEN + 1);

      ciaaPOSIX_strcpy(sparcDriverUartInfo[uartIndex].deviceDataStructure.path, SPARC_DRIVER_UART_NOMINAL_DEVICE_NAME_STRING);

      /* Clearly, this naming scheme only works if sparcDriverUartCount < 10 */
      sparcAssert(uartIndex < 10, "Modify the device naming scheme!");
      sparcDriverUartInfo[uartIndex].deviceDataStructure.path[SPARC_DRIVER_UART_NOMINAL_DEVICE_NAME_STRLEN - 1] =
            '0' + uartIndex;

      sparcDriverUartInfo[uartIndex].deviceDataStructure.open    = ciaaDriverUart_open;
      sparcDriverUartInfo[uartIndex].deviceDataStructure.close   = ciaaDriverUart_close;
      sparcDriverUartInfo[uartIndex].deviceDataStructure.read    = ciaaDriverUart_read;
      sparcDriverUartInfo[uartIndex].deviceDataStructure.write   = ciaaDriverUart_write;
      sparcDriverUartInfo[uartIndex].deviceDataStructure.ioctl   = ciaaDriverUart_ioctl;
      sparcDriverUartInfo[uartIndex].deviceDataStructure.lseek   = NULL;
      sparcDriverUartInfo[uartIndex].deviceDataStructure.upLayer = NULL; /** <= upper layer data */
      sparcDriverUartInfo[uartIndex].deviceDataStructure.layer   = (void *)&sparcDriverUartInfo[uartIndex]; /** <= this layer's data */
      sparcDriverUartInfo[uartIndex].deviceDataStructure.loLayer = (void *)uartIndex; /** <= lower layer data */

      /* initially all the devices are closed */
      sparcDriverUartInfo[uartIndex].deviceIsOpened = 0;

      /* initialize the queues */
      sparcDriverUartInitQueue(&sparcDriverUartInfo[uartIndex].txQueue);
      sparcDriverUartInitQueue(&sparcDriverUartInfo[uartIndex].rxQueue);
   }
}


void sparcDriverUartInitializeHardware()
{
   uint32 uartIndex;

   for (uartIndex = 0; uartIndex < sparcDriverUartCount; uartIndex++)
   {
      sparcDriverUpdateScalerRegister(&sparcDriverUartInfo[uartIndex]);
      sparcDriverUpdateControlRegister(&sparcDriverUartInfo[uartIndex]);
   }
}


void sparcDriverUartRegisterDrivers()
{
   sint32 uartIndex;

   for(uartIndex = 0; uartIndex < sparcDriverUartCount; uartIndex++)
   {
      ciaaSerialDevices_addDriver(&sparcDriverUartInfo[uartIndex].deviceDataStructure);
   }
}


void sparcDriverServiceDevice(sparcDriverUartInfoType *uartDataStructPtr)
{
   grDeviceRegisterValue statusRegisterValue;

   sparcAssert(uartDataStructPtr->deviceDataStructure->upLayer != NULL, "Missing information from the upper layer");

   statusRegisterValue = grRegisterRead(uartDataStructPtr->baseAddress, GRLIB_APBUART_STATUS_REGISTER);

   if ((statusRegisterValue & GRLIB_FLAG_MASK_APBUART_STATUS_REGISTER_DR) != 0)
   {
      do
      {
         if ((statusRegisterValue & GRLIB_FLAG_MASK_APBUART_STATUS_REGISTER_DR) != 0)
         {
            sparcDriverUartQueuePush(&uartDataStructPtr->rxQueue, grRegisterRead(uartDataStructPtr->baseAddress, GRLIB_APBUART_DATA_REGISTER));
         }

         statusRegisterValue = grRegisterRead(uartDataStructPtr->baseAddress, GRLIB_APBUART_STATUS_REGISTER);

      } while ( ((statusRegisterValue & GRLIB_FLAG_MASK_APBUART_STATUS_REGISTER_DR) != 0)
            && (sparcDriverUartQueueIsFull(&uartDataStructPtr->rxQueue) == 0) );

      ciaaSerialDevices_rxConfirmation(uartDataStructPtr->deviceDataStructure->upLayer, sparcDriverUartQueueByteCount(&uartDataStructPtr->rxQueue));
   }

   if ( (uartDataStructPtr->txInterruptEnabled != 0)
         && ((statusRegisterValue & GRLIB_FLAG_MASK_APBUART_STATUS_REGISTER_TF) == 0) )
   {
      /* Call the POSIX layer to fill the output buffer */
      ciaaSerialDevices_txConfirmation(uartDataStructPtr->deviceDataStructure->upLayer, 1 );

      /* Check again the status register, we want to check if the POSIX layer had
       * enough bytes to fill the output buffer. */
      statusRegisterValue = grRegisterRead(uartDataStructPtr->baseAddress, GRLIB_APBUART_STATUS_REGISTER);

      if ((statusRegisterValue & GRLIB_FLAG_MASK_APBUART_STATUS_REGISTER_TF) == 0)
      {
         /* The buffer is not full, POSIX did not fill it. Thats because there are no more bytes
          * to send. Disable the transmission interrupt. */
         sparcDriverDisableTxInterrupts(uartDataStructPtr);
      }
   }

}


/*==================[external functions definition]==========================*/


extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   sparcDriverUartInfoType *deviceControlStructurePtr;

   deviceControlStructurePtr = (sparcDriverUartInfoType *)device->layer;

   /* if this is the first nested open of the device, make sure
    * the queues are flushed */
   if (deviceControlStructurePtr->deviceIsOpened == 0)
   {
      sparcDriverUartInitQueue(&deviceControlStructurePtr->txQueue);
      sparcDriverUartInitQueue(&deviceControlStructurePtr->rxQueue);
   }

   deviceControlStructurePtr->deviceIsOpened++;

   return device;
}


extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   sparcDriverUartInfoType *deviceControlStructurePtr;

   deviceControlStructurePtr = (sparcDriverUartInfoType *)device->layer;

   if (deviceControlStructurePtr->deviceIsOpened != 0)
   {
      deviceControlStructurePtr->deviceIsOpened--;
   }

   return 0;
}


extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   sparcDriverUartInfoType *deviceControlStructurePtr;
   int32_t returnValue;

   deviceControlStructurePtr = (sparcDriverUartInfoType *)device->layer;

   returnValue = -1;

   switch(request)
   {
   case ciaaPOSIX_IOCTL_STARTTX:
      sparcDriverKickstartTxInterrupts(deviceControlStructurePtr);
      returnValue = 0;
      break;

   case ciaaPOSIX_IOCTL_SET_BAUDRATE:
      returnValue = sparcDriverSetBaudrate(deviceControlStructurePtr, (int32_t)param);
      break;

   case ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT:
      if((bool)(intptr_t)param == false)
      {
         sparcDriverDisableTxInterrupts(deviceControlStructurePtr);
      } else {
         sparcDriverEnableTxInterrupts(deviceControlStructurePtr);
      }
      returnValue = 0;
      break;

   case ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT:
      if((bool)(intptr_t)param == false)
      {
         sparcDriverDisableRxInterrupts(deviceControlStructurePtr);
      } else {
         sparcDriverEnableRxInterrupts(deviceControlStructurePtr);
      }
      returnValue = 0;
      break;
   }

   return returnValue;
}


extern ssize_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   sparcDriverUartInfoType *deviceControlStructurePtr;
   int32_t ret;

   ret = -1;

   FILL THE VOIDS

   deviceControlStructurePtr = (sparcDriverUartInfoType *)device->layer;

   return ret;
}


extern ssize_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   sparcDriverUartInfoType *deviceControlStructurePtr;
   int32_t ret;

   ret = -1;

   /* enter critical */

   FILL THE VOIDS

   deviceControlStructurePtr = (sparcDriverUartInfoType *)device->layer;

   /* exit critical */

   return ret;
}


void ciaaDriverUart_init(void)
{
   /* Read important UART configuration data */
   sparcDriverUartDetectCoreConfiguration();

   /* Set the baseline configuration of each of the UARTS */
   sparcDriverUartInitializeControlStructures();

   /* Configure the initial baud rate, stop bits, etc. */
   sparcDriverUartInitializeHardware();

   /* Add drivers to the system */
   sparcDriverUartRegisterDrivers();
}


/*==================[interrupt handlers]=====================================*/


ISR(UART0_IRQHandler)
{
   sint32 uartIndex;

   for(uartIndex = 0; uartIndex < sparcDriverUartCount; uartIndex++)
   {
      sparcDriverServiceDevice(&sparcDriverUartInfo[uartIndex].deviceDataStructure);
   }
}


ISR(UART1_IRQHandler)
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


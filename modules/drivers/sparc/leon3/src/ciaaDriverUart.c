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
#include "grlib.h"

/*==================[macros and definitions]=================================*/


#define SPARC_MAX_UART_COUNT      4


/*==================[internal data declaration]==============================*/




/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/


sparcDriverUartInfoType sparcDriverUartInfo[SPARC_MAX_UART_COUNT];


uint32 sparcDriverUartCount;


/*==================[external data definition]===============================*/


/*==================[internal functions definition]==========================*/


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


void sparcDriverReprogramControlRegister(uint32 uartIndex)
{
   grDeviceRegisterValue controlRegisterValue;

   sparcAssert(sparcDriverUartInfo[uartIndex].numberOfStopBits >= 1, "Invalid configuration");
   sparcAssert(sparcDriverUartInfo[uartIndex].numberOfStopBits <= 2, "Invalid configuration");
   sparcAssert(sparcDriverUartInfo[uartIndex].flowControl <= 1, "Invalid configuration");
   sparcAssert(sparcDriverUartInfo[uartIndex].useParity <= 1, "Invalid configuration");
   sparcAssert(sparcDriverUartInfo[uartIndex].useOddParity <= 1, "Invalid configuration");
   sparcAssert(sparcDriverUartInfo[uartIndex].loopbackEnabled <= 1, "Invalid configuration");
   sparcAssert(sparcDriverUartInfo[uartIndex].externalClkEnabled <= 1, "Invalid configuration");

   controlRegisterValue = 0;

   /* Set user configuration flags */
   if (sparcDriverUartInfo[uartIndex].numberOfStopBits == 2)   controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_NS;
   if (sparcDriverUartInfo[uartIndex].flowControl == 1)        controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_FL;
   if (sparcDriverUartInfo[uartIndex].useParity == 1)          controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_PE;
   if (sparcDriverUartInfo[uartIndex].useOddParity == 1)       controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_PS;
   if (sparcDriverUartInfo[uartIndex].loopbackEnabled == 1)    controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_LB;
   if (sparcDriverUartInfo[uartIndex].externalClkEnabled == 1) controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_EC;

   /* Set other configuration flags */

   // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_SI; /* Transmitter shift empty interrupt enable */
   // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_BI; /* Break interrupt enable */
   // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_DB; /* FIFO debug mode enable */

   if (sparcDriverUartInfo[uartIndex].hasFIFOs == 1)
   {
      // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TI; /* Transmitter interrupt enable */
      // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RI; /* Receiver interrupt enable */
      controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_DI; /* Delayed interrupt enable */
      controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RF; /* Receiver FIFO interrupt enable */
      // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TF; /* Transmitter FIFO interrupt enable */
   } else {
      controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TI; /* Transmitter interrupt enable */
      controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RI; /* Receiver interrupt enable */
      // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_DI; /* Delayed interrupt enable */
      // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RF; /* Receiver FIFO interrupt enable */
      // controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TF; /* Transmitter FIFO interrupt enable */
   }

   controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_TE; /* Transmitter enable */
   controlRegisterValue |= GRLIB_FLAG_MASK_APBUART_CONTROL_REGISTER_RE; /* Receiver enable */

   grRegisterWrite(sparcDriverUartInfo[uartIndex].baseAddress, GRLIB_APBUART_CONTROL_REGISTER, controlRegisterValue);
}


void sparcDriverUartConfigureUarts()
{
   uint32 uartIndex;

   for (uartIndex = 0; uartIndex < sparcDriverUartCount; uartIndex++)
   {
      /* Set the initial configuration */
      sparcDriverUartInfo[uartIndex].numberOfStopBits   = 1;
      sparcDriverUartInfo[uartIndex].flowControl        = 0;
      sparcDriverUartInfo[uartIndex].useParity          = 0;
      sparcDriverUartInfo[uartIndex].useOddParity       = 0;
      sparcDriverUartInfo[uartIndex].loopbackEnabled    = 0;
      sparcDriverUartInfo[uartIndex].externalClkEnabled = 0;

      sparcDriverReprogramControlRegister(uartIndex);
   }
}


/*==================[external functions definition]==========================*/


extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{

   FILL THE VOIDS

   return device;
}


extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   FILL THE VOIDS

   return 0;
}


extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   FILL THE VOIDS

   return ret;
}


extern ssize_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   FILL THE VOIDS

   return ret;
}


extern ssize_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   FILL THE VOIDS

   return ret;
}


void ciaaDriverUart_init(void)
{
   /* Read important UART configuration data */
   sparcDriverUartDetectCoreConfiguration();

   /* Set the baseline configuration of each of the UARTS */
   sparcDriverUartConfigureUarts();

}


/*==================[interrupt handlers]=====================================*/


ISR(UART0_IRQHandler)
{


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


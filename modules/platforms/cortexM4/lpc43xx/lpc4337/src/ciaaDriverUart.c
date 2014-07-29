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

/** \brief CIAA UART Driver for LPC4337
 **
 ** Implements the UART Driver for LPC4337
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
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/

typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device for UART 0 */
static ciaaDevices_deviceType ciaaDriverUart_device0 = {
   "uart/0",               /** <= driver name */
   ciaaDriverUart_open,    /** <= open function */
   ciaaDriverUart_close,   /** <= close function */
   ciaaDriverUart_read,    /** <= read function */
   ciaaDriverUart_write,   /** <= write function */
   ciaaDriverUart_ioctl,   /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   NULL,                   /** <= layer */
   LPC_USART0              /** <= lower layer */
};

/** \brief Device for UART 1 */
static ciaaDevices_deviceType ciaaDriverUart_device1 = {
   "uart/1",               /** <= driver name */
   ciaaDriverUart_open,    /** <= open function */
   ciaaDriverUart_close,   /** <= close function */
   ciaaDriverUart_read,    /** <= read function */
   ciaaDriverUart_write,   /** <= write function */
   ciaaDriverUart_ioctl,   /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   NULL,                   /** <= layer */
   LPC_USART2              /** <= lower layer */
};

/** \brief Device for UART 2 */
static ciaaDevices_deviceType ciaaDriverUart_device2 = {
   "uart/2",               /** <= driver name */
   ciaaDriverUart_open,    /** <= open function */
   ciaaDriverUart_close,   /** <= close function */
   ciaaDriverUart_read,    /** <= read function */
   ciaaDriverUart_write,   /** <= write function */
   ciaaDriverUart_ioctl,   /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   NULL,                   /** <= layer */
   LPC_USART3              /** <= lower layer */
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

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void ciaaDriverUart_rxIndication(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_rxIndication(device->upLayer, 1 );
}

static void ciaaDriverUart_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_txConfirmation(device->upLayer, 1 );
}

static void ciaaDriverUart_hwInit(void)
{
   /* UART2 (USB-UART) */
   Chip_UART_Init(LPC_USART2);
   Chip_UART_SetBaud(LPC_USART2, 115200);

   Chip_UART_TXEnable(LPC_USART2);

   Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              // P7_1: UART2_TXD
   Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); // P7_2: UART2_RXD

   Chip_UART_IntEnable(LPC_USART2, UART_IER_RBRINT);

   NVIC_EnableIRQ(USART2_IRQn);


   /* UART3 (RS232) */
//   cfg.Baud_rate = 57600;
//   UART_Init(LPC_USART3, &cfg);
//   UART_TxCmd(LPC_USART3, ENABLE);
//
//   scu_pinmux(2, 3, MD_PDN, FUNC2);                // P2_3: UART3_TXD
//   scu_pinmux(2, 4, MD_PLN|MD_EZI|MD_ZI, FUNC2);      // P2_4: UART3_RXD
//
//   UART_IntConfig((LPC_USARTn_Type *)LPC_USART3, UART_INTCFG_RBR, ENABLE);
//   NVIC_EnableIRQ(USART3_IRQn);
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   if(device == ciaaDriverUartConst.devices[0])
   {

   }
   else if(device == ciaaDriverUartConst.devices[1])
   {
      if(request == ciaaPOSIX_IOCTL_STARTTX)
      {
         /* this one calls write */
         ciaaDriverUart_txConfirmation(&ciaaDriverUart_device1);
      }
   }
   else if(device == ciaaDriverUartConst.devices[2])
   {

   }
   return -1;
}

extern int32_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, uint32_t size)
{
   return -1;
}

extern int32_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, uint32_t const size)
{
   int32_t ret = 0;

   if(device == ciaaDriverUartConst.devices[0])
   {

   }
   else if(device == ciaaDriverUartConst.devices[1])
   {
      if(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE)
      {
         /* send first byte */
         Chip_UART_SendByte(LPC_USART2, *(uint8_t *)buffer);
         /* enable Tx Holding Register Empty interrupt */
         Chip_UART_IntEnable(LPC_USART2, UART_IER_THREINT);
         /* 1 byte written */
         ret = 1;
      }
   }
   else if(device == ciaaDriverUartConst.devices[2])
   {

   }

   return ret;
}

void ciaaDriverUart_init(void)
{
   uint8_t loopi;

   /* init hardware */
   ciaaDriverUart_hwInit();

   /* add uart driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverUartConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverUartConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/
void UART0_IRQHandler(void)
{
   /* TODO check and call only rx or tx as corresponding */
   ciaaDriverUart_rxIndication(&ciaaDriverUart_device0);
   ciaaDriverUart_txConfirmation(&ciaaDriverUart_device0);
}

void UART2_IRQHandler(void)
{
   uint8_t status = Chip_UART_ReadLineStatus(LPC_USART2);

   if(status & UART_LSR_RDR)
   {
      ciaaDriverUart_rxIndication(&ciaaDriverUart_device1);
   }
   if(status & UART_LSR_THRE)
   {
      /* disable THRE irq */
      Chip_UART_IntDisable(LPC_USART2, UART_IER_THREINT);
      /* tx confirmation, 1 byte sent */
      ciaaDriverUart_txConfirmation(&ciaaDriverUart_device1);
   }
}

void UART3_IRQHandler(void)
{
   /* TODO check and call only rx or tx as corresponding */
   ciaaDriverUart_rxIndication(&ciaaDriverUart_device2);
   ciaaDriverUart_txConfirmation(&ciaaDriverUart_device2);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


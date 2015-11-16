/* Copyright 2014, Pablo Ridolfi (UTN-FRBA)
 * Copyright 2014, Juan Cecconi
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
 * PaRi         Pablo Ridolfi
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140731 v0.0.1   PR first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverUart.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

#define UART_RX_FIFO_SIZE       (16)

typedef struct {
   uint8_t hwbuf[UART_RX_FIFO_SIZE];
   uint8_t rxcnt;
} ciaaDriverUartControl;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverUartControl uartControl[3];

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
   &(uartControl[0]),      /** <= layer */
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
   &(uartControl[1]),      /** <= layer */
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
   &(uartControl[2]),      /** <= layer */
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
static void ciaaDriverUart_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_rxIndication(device->upLayer, nbyte);
}

static void ciaaDriverUart_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_txConfirmation(device->upLayer, 1 );
}

static void ciaaDriverUart_hwInit(void)
{
   /* UART0 (RS485/Profibus) */
   Chip_UART_Init(LPC_USART0);
   Chip_UART_SetBaud(LPC_USART0, 115200);

   Chip_UART_SetupFIFOS(LPC_USART0, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0);

   Chip_UART_TXEnable(LPC_USART0);

   Chip_SCU_PinMux(9, 5, MD_PDN, FUNC7);              /* P9_5: UART0_TXD */
   Chip_SCU_PinMux(9, 6, MD_PLN|MD_EZI|MD_ZI, FUNC7); /* P9_6: UART0_RXD */

   Chip_UART_SetRS485Flags(LPC_USART0, UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_OINV_1);

   Chip_SCU_PinMux(6, 2, MD_PDN, FUNC2);              /* P6_2: UART0_DIR */

   /* UART2 (USB-UART) */
   Chip_UART_Init(LPC_USART2);
   Chip_UART_SetBaud(LPC_USART2, 115200);

   Chip_UART_SetupFIFOS(LPC_USART2, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0);

   Chip_UART_TXEnable(LPC_USART2);

   Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1: UART2_TXD */
   Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2: UART2_RXD */

   /* UART3 (RS232) */
   Chip_UART_Init(LPC_USART3);
   Chip_UART_SetBaud(LPC_USART3, 115200);

   Chip_UART_SetupFIFOS(LPC_USART3, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0);

   Chip_UART_TXEnable(LPC_USART3);

   Chip_SCU_PinMux(2, 3, MD_PDN, FUNC2);              /* P2_3: UART3_TXD */
   Chip_SCU_PinMux(2, 4, MD_PLN|MD_EZI|MD_ZI, FUNC2); /* P2_4: UART3_RXD */
}

/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   /* Restart FIFOS: set Enable, Reset content, set trigger level */
   Chip_UART_SetupFIFOS((LPC_USART_T *)device->loLayer, UART_FCR_FIFO_EN | UART_FCR_TX_RS | UART_FCR_RX_RS | UART_FCR_TRG_LEV0);
   /* dummy read */
   Chip_UART_ReadByte((LPC_USART_T *)device->loLayer);
   /* enable rx interrupt */
   Chip_UART_IntEnable((LPC_USART_T *)device->loLayer, UART_IER_RBRINT);

   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   /* disable tx and rx interrupt */
   Chip_UART_IntDisable((LPC_USART_T *)device->loLayer, UART_IER_THREINT | UART_IER_RBRINT);
   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;

   if((device == ciaaDriverUartConst.devices[0]) ||
      (device == ciaaDriverUartConst.devices[1]) ||
      (device == ciaaDriverUartConst.devices[2]) )
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTTX:
            /* disable THRE irq (TX) */
            Chip_UART_IntDisable((LPC_USART_T *)device->loLayer, UART_IER_THREINT);
            /* this one calls write */
            ciaaDriverUart_txConfirmation(device);
            /* enable THRE irq (TX) */
            Chip_UART_IntEnable((LPC_USART_T *)device->loLayer, UART_IER_THREINT);
            ret = 0;
            break;

         case ciaaPOSIX_IOCTL_SET_BAUDRATE:
            ret = Chip_UART_SetBaud((LPC_USART_T *)device->loLayer,  (int32_t)param);
            break;

         case ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL:
            Chip_UART_SetupFIFOS((LPC_USART_T *)device->loLayer,  UART_FCR_FIFO_EN | UART_FCR_TX_RS | UART_FCR_RX_RS | (int32_t)param);
            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               /* disable THRE irq (TX) */
               Chip_UART_IntDisable((LPC_USART_T *)device->loLayer, UART_IER_THREINT);
            }
            else
            {
               /* enable THRE irq (TX) */
               Chip_UART_IntEnable((LPC_USART_T *)device->loLayer, UART_IER_THREINT);
            }
            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               /* disable RBR irq (RX) */
               Chip_UART_IntDisable((LPC_USART_T *)device->loLayer, UART_IER_RBRINT);
            }
            else
            {
               /* enable RBR irq (RX) */
               Chip_UART_IntEnable((LPC_USART_T *)device->loLayer, UART_IER_RBRINT);
            }
            break;
      }
   }
   return ret;
}

extern ssize_t ciaaDriverUart_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   ssize_t ret = -1;
   ciaaDriverUartControl * pUartControl;
   uint8_t i;

   if(size != 0)
   {
      if((device == ciaaDriverUartConst.devices[0]) ||
         (device == ciaaDriverUartConst.devices[1]) ||
         (device == ciaaDriverUartConst.devices[2]) )
      {
         pUartControl = (ciaaDriverUartControl *)device->layer;

         if(size > pUartControl->rxcnt)
         {
            /* buffer has enough space */
            ret = pUartControl->rxcnt;
            pUartControl->rxcnt = 0;
         }
         else
         {
            /* buffer hasn't enough space */
            ret = size;
            pUartControl->rxcnt -= size;
         }
         for(i = 0; i < ret; i++)
         {
            buffer[i] = pUartControl->hwbuf[i];
         }
         if(pUartControl->rxcnt != 0)
         {
            /* We removed data from the buffer, it is time to reorder it */
            for(i = 0; i < pUartControl->rxcnt ; i++)
            {
               pUartControl->hwbuf[i] = pUartControl->hwbuf[i + ret];
            }
         }
      }
   }
   return ret;
}

extern ssize_t ciaaDriverUart_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = 0;

   if((device == ciaaDriverUartConst.devices[0]) ||
      (device == ciaaDriverUartConst.devices[1]) ||
      (device == ciaaDriverUartConst.devices[2]) )
   {
      while((Chip_UART_ReadLineStatus((LPC_USART_T *)device->loLayer) & UART_LSR_THRE) && (ret < size))
      {
         /* send first byte */
         Chip_UART_SendByte((LPC_USART_T *)device->loLayer, buffer[ret]);
         /* bytes written */
         ret++;
      }
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
ISR(UART0_IRQHandler)
{
   uint8_t status = Chip_UART_ReadLineStatus(LPC_USART0);

   if(status & UART_LSR_RDR)
   {
      do
      {
         uartControl[0].hwbuf[uartControl[0].rxcnt] = Chip_UART_ReadByte(LPC_USART0);
         uartControl[0].rxcnt++;
      }while((Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_RDR) &&
             (uartControl[0].rxcnt < UART_RX_FIFO_SIZE));

      ciaaDriverUart_rxIndication(&ciaaDriverUart_device0, uartControl[0].rxcnt);
   }
   if((status & UART_LSR_THRE) && (Chip_UART_GetIntsEnabled(LPC_USART0) & UART_IER_THREINT))
   {
      /* tx confirmation, 1 byte sent */
      ciaaDriverUart_txConfirmation(&ciaaDriverUart_device0);

      if(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE)
      {  /* There is not more bytes to send, disable THRE irq */
         Chip_UART_IntDisable(LPC_USART0, UART_IER_THREINT);
      }
   }
}

ISR(UART2_IRQHandler)
{
   uint8_t status = Chip_UART_ReadLineStatus(LPC_USART2);

   if(status & UART_LSR_RDR)
   {
      do
      {
         uartControl[1].hwbuf[uartControl[1].rxcnt] = Chip_UART_ReadByte(LPC_USART2);
         uartControl[1].rxcnt++;
      }while((Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_RDR) &&
             (uartControl[1].rxcnt < UART_RX_FIFO_SIZE));

      ciaaDriverUart_rxIndication(&ciaaDriverUart_device1, uartControl[1].rxcnt);
   }
   if((status & UART_LSR_THRE) && (Chip_UART_GetIntsEnabled(LPC_USART2) & UART_IER_THREINT))
   {
      /* tx confirmation, 1 byte sent */
      ciaaDriverUart_txConfirmation(&ciaaDriverUart_device1);

      if(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE)
      {  /* There is not more bytes to send, disable THRE irq */
         Chip_UART_IntDisable(LPC_USART2, UART_IER_THREINT);
      }
   }
}

ISR(UART3_IRQHandler)
{
   uint8_t status = Chip_UART_ReadLineStatus(LPC_USART3);

   if(status & UART_LSR_RDR)
   {
      do
      {
         uartControl[2].hwbuf[uartControl[2].rxcnt] = Chip_UART_ReadByte(LPC_USART3);
         uartControl[2].rxcnt++;
      }while((Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_RDR) &&
             (uartControl[2].rxcnt < UART_RX_FIFO_SIZE));

      ciaaDriverUart_rxIndication(&ciaaDriverUart_device2, uartControl[2].rxcnt);
   }
   if((status & UART_LSR_THRE) && (Chip_UART_GetIntsEnabled(LPC_USART3) & UART_IER_THREINT))
   {
      /* tx confirmation, 1 byte sent */
      ciaaDriverUart_txConfirmation(&ciaaDriverUart_device2);

      if(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_THRE)
      {  /* There is not more bytes to send, disable THRE irq */
         Chip_UART_IntDisable(LPC_USART3, UART_IER_THREINT);
      }
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


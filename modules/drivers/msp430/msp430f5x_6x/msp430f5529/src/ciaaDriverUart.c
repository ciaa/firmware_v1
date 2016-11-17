/* Copyright 2016, Franco Bucafusco
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

/** \brief CIAA UART Driver for MSP430F5529
 **
 ** Implements the UART Driver for MSP430F5529
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup UART UART Drivers
 ** @{ */
/*==================[inclusions]=============================================*/
#include "ciaaDriverUart.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "Os_Internal_Arch_Cpu.h"
#include "msp430.h"
#include "driverlib.h"

#include "os.h"

/*==================[macros and definitions]=================================*/

typedef struct
{
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

#define UART_RX_FIFO_SIZE       (16)

typedef struct
{
   uint8_t hwbuf[UART_RX_FIFO_SIZE];
   uint8_t rxcnt;
} ciaaDriverUartControl;

#define UART_DEFAULT_BAUDRATE    9600

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverUartControl uartControl[2];
/** \brief Device for UART 0 */
static ciaaDevices_deviceType ciaaDriverUart_device0 =
{
   "uart/0",               /** <= driver name */
   ciaaDriverUart_open,    /** <= open function */
   ciaaDriverUart_close,   /** <= close function */
   ciaaDriverUart_read,    /** <= read function */
   ciaaDriverUart_write,   /** <= write function */
   ciaaDriverUart_ioctl,   /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   &(uartControl[0]),      /** <= layer */
   USCI_A0_BASE            /** <= lower layer */
};

/** \brief Device for UART 1 */
static ciaaDevices_deviceType ciaaDriverUart_device1 =
{
   "uart/1",               /** <= driver name */
   ciaaDriverUart_open,    /** <= open function */
   ciaaDriverUart_close,   /** <= close function */
   ciaaDriverUart_read,    /** <= read function */
   ciaaDriverUart_write,   /** <= write function */
   ciaaDriverUart_ioctl,   /** <= ioctl function */
   NULL,                   /** <= seek function is not provided */
   NULL,                   /** <= uper layer */
   &(uartControl[1]),      /** <= layer */
   USCI_A1_BASE            /** <= lower layer */
};

static ciaaDevices_deviceType * const ciaaUartDevices[] =
{
   &ciaaDriverUart_device0,
   &ciaaDriverUart_device1
};

static ciaaDriverConstType const ciaaDriverUartConst =
{
   ciaaUartDevices,
   2
};


/*
default structure for configuring the usci.
for boards like msp-exp430f5529 the silicon revision of the uc is E, so it has UCS6 but, that limits the LPM or the BAUDRATE.
for launchpad 5529, the silicon revision is I son the UCS6 bug is no more present.
*/
USCI_A_UART_initParam uart_default_params =
{
#if BOARD==msp_exp430f5529
   USCI_A_UART_CLOCKSOURCE_ACLK, //selectClockSource
   (uint16)(((uint32)(REF_FREQUENCY_HZ))/UART_DEFAULT_BAUDRATE/16), // clockPrescalar   //UART_DEFAULT_BAUDRATE   //WORKING_FREQUENCY_MHZ*1000000
#else
   USCI_A_UART_CLOCKSOURCE_SMCLK, //selectClockSource
   (uint16)(((uint32)(WORKING_FREQUENCY_MHZ*1000000))/UART_DEFAULT_BAUDRATE/16), // clockPrescalar   //UART_DEFAULT_BAUDRATE   //WORKING_FREQUENCY_MHZ*1000000
#endif

   0, //firstModReg
   0, //secondModReg
   USCI_A_UART_NO_PARITY, //parity
   USCI_A_UART_LSB_FIRST, //msborLsbFirst
   USCI_A_UART_ONE_STOP_BIT, //numberofStopBits
   USCI_A_UART_MODE, //uartMode

#if BOARD==msp_exp430f5529
   USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION
#else
   USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  //overSampling
#endif
};


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
/*
sets the baudrate

*/
void SetBaudrate( uint16_t baseAddress , unsigned short baud   )
{
   unsigned short n ;
   unsigned short n_16 ;
   unsigned long temp;

   #if BOARD==msp_exp430f5529
      temp = REF_FREQUENCY_HZ;
   #else
      temp = ( unsigned long )WORKING_FREQUENCY_MHZ*1000000;
   #endif

   n =  ( temp )/( ( unsigned long )( baud * 100 )  );   //TODO: REMOVE MAGIC NUMBER 100 IS THE BAUDRATE SCALER. SHOULD BE USED WITHIN THE BLINKING_ECHO EXAMPLE.

#if BOARD==msp_exp430f5529

   HWREG16(baseAddress + OFS_UCAxBRW) = n;

   n_16 =  ((temp - n*baud *100 )*8)/(baud *100);

   HWREG16(baseAddress + OFS_UCAxMCTL) |=  (n_16*UCBRS0) ;

#else
   /* based on 34.3.10.2 Oversampling Baud-Rate Mode Setting from slau208m. */

   n_16 = n/16;

   HWREG16(baseAddress + OFS_UCAxBRW) = n_16;

   n_16 = n_16*16;

   n= n - n_16;

   HWREG16(baseAddress + OFS_UCAxMCTL) |=  (n*UCBRF0) ;
#endif
}


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
   /* UART0  (USB-UART) */

   /* Pin Muxes for UART0 */
   GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P4,  GPIO_PIN4 + GPIO_PIN5 );

   USCI_A_UART_disable( USCI_A0_BASE);

   USCI_A_UART_init( USCI_A0_BASE , &uart_default_params );

   //Enable Receive Interrupt
   USCI_A_UART_clearInterrupt(USCI_A0_BASE,  USCI_A_UART_RECEIVE_INTERRUPT_FLAG | USCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
   USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_TRANSMIT_INTERRUPT_FLAG);

   /* UART1  */
   /* Pin Muxes for UART0 */
   GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P3,  GPIO_PIN3 + GPIO_PIN4 );

   USCI_A_UART_disable( USCI_A1_BASE);

   USCI_A_UART_init( USCI_A1_BASE , &uart_default_params );


   //Enable Receive Interrupt
   USCI_A_UART_clearInterrupt(USCI_A1_BASE,  USCI_A_UART_RECEIVE_INTERRUPT_FLAG | USCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
   USCI_A_UART_enableInterrupt(USCI_A1_BASE, USCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
}
/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverUart_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   /* Enable de peripheral */
   USCI_A_UART_enable( (uint16_t) device->loLayer );

   /* dummy read */

   /* enable rx interrupt */
   USCI_A_UART_enableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
   return device;
}

extern int32_t ciaaDriverUart_close(ciaaDevices_deviceType const * const device)
{
   /* disable tx and rx interrupt */
   USCI_A_UART_clearInterrupt( (uint16_t) device->loLayer ,  USCI_A_UART_RECEIVE_INTERRUPT_FLAG | USCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
   USCI_A_UART_disable( (uint16_t) device->loLayer );
   return 0;
}

extern int32_t ciaaDriverUart_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;

   if((device == ciaaDriverUartConst.devices[0]) ||
         (device == ciaaDriverUartConst.devices[1])
     )
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTTX:
            /* disable THRE irq (TX) */
            //     USCI_A_UART_disableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_TRANSMIT_INTERRUPT_FLAG );

            /* this one calls write */
            ciaaDriverUart_txConfirmation(device);

            /* enable THRE irq (TX) */
            //    USCI_A_UART_enableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_TRANSMIT_INTERRUPT_FLAG );
            ret = 0;
            break;

         case ciaaPOSIX_IOCTL_SET_BAUDRATE:

            SetBaudrate( (uint16_t) device->loLayer  , param );

            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_TX_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               /* disable  irq (TX) */
               USCI_A_UART_disableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_TRANSMIT_INTERRUPT_FLAG );
            }
            else
            {
               /* enable  irq (TX) */
               USCI_A_UART_enableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_TRANSMIT_INTERRUPT_FLAG );
            }
            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               /* disable  irq (RX) */
               USCI_A_UART_disableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
            }
            else
            {
               /* enable  irq (RX) */
               USCI_A_UART_enableInterrupt( (uint16_t) device->loLayer , USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
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
            (device == ciaaDriverUartConst.devices[1])  )
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
      (device == ciaaDriverUartConst.devices[1])
     )
   {
      while( ret < size )
      {
         /* send first byte */
         USCI_A_UART_transmitData( (uint16_t) device->loLayer , buffer[ret] ) ;

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
   for(loopi = 0; loopi < ciaaDriverUartConst.countOfDevices; loopi++)
   {
      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverUartConst.devices[loopi]);
   }
}

/*==================[interrupt handlers]=====================================*/
ISR( USCI_A0_VECTOR )
{
   /* turn on the FLL */
   __bic_SR_register( SCG0 );

   switch(__even_in_range(UCA0IV,4))
   {
         //Vector 2 - RXIFG
      case 2:

         do
         {
            USCI_A_UART_clearInterrupt( USCI_A0_BASE ,  USCI_A_UART_RECEIVE_INTERRUPT_FLAG );

            uartControl[0].hwbuf[uartControl[0].rxcnt] = USCI_A_UART_receiveData(USCI_A0_BASE);
            uartControl[0].rxcnt++;
         }
         while( (USCI_A_UART_getInterruptStatus(USCI_A0_BASE , USCI_A_UART_RECEIVE_INTERRUPT_FLAG) == USCI_A_UART_RECEIVE_INTERRUPT_FLAG) &&
                (uartControl[0].rxcnt < UART_RX_FIFO_SIZE));

         ciaaDriverUart_rxIndication(&ciaaDriverUart_device0, uartControl[0].rxcnt);

         break;

         //Vector 2 - TXIFG
      case 4:

         /* tx confirmation, 1 byte sent */
         //no hace nada. se envia por poleo en el write
         /*ciaaDriverUart_txConfirmation(&ciaaDriverUart_device0);

         if(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE)
         {

            Chip_UART_IntDisable(LPC_USART0, UART_IER_THREINT);
         }*/
         break;

      default:
         break;
   }

}

ISR(USCI_A1_VECTOR)
{
   /* turn on the FLL */
   __bic_SR_register( SCG0 );

   switch(__even_in_range(UCA1IV,4))
   {
         //Vector 2 - RXIFG
      case 2:

         do
         {
            USCI_A_UART_clearInterrupt( USCI_A1_BASE ,  USCI_A_UART_RECEIVE_INTERRUPT_FLAG );

            uartControl[1].hwbuf[uartControl[1].rxcnt] = USCI_A_UART_receiveData(USCI_A1_BASE);
            uartControl[1].rxcnt++;
         }
         while((USCI_A_UART_getInterruptStatus(USCI_A1_BASE , USCI_A_UART_RECEIVE_INTERRUPT_FLAG) == USCI_A_UART_RECEIVE_INTERRUPT_FLAG) &&
               (uartControl[1].rxcnt < UART_RX_FIFO_SIZE));

         ciaaDriverUart_rxIndication(&ciaaDriverUart_device1, uartControl[1].rxcnt);


         break;

         //Vector 2 - TXIFG
      case 4:

         /* tx confirmation, 1 byte sent */
         //no hace nada. se envia por poleo en el write
         /*ciaaDriverUart_txConfirmation(&ciaaDriverUart_device0);

         if(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE)
         {

            Chip_UART_IntDisable(LPC_USART0, UART_IER_THREINT);
         }*/
         break;

      default:
         break;
   }
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

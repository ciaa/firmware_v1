/* Copyright 2014, Fernando Beunza
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

/** \brief CIAA Aio Driver for LPC4337
 **
 ** Implements the A/D & D/A Drivers for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup AIO AIO Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * FB           Fernando Beunza
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140911 v0.0.1   FB first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverAio.h"
#include "ciaaDriverAio_Internal.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/

#define CONTEXT_ISR2 3

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

typedef uint8_t ContextType;

#define UART_AIO_FIFO_SIZE       (16)

typedef struct {
   uint8_t hwbuf[UART_AIO_FIFO_SIZE];
   uint8_t aiocnt;
   union {
      struct {
         LPC_ADC_T *handler;          /** <= adc handler */
         int32_t interrupt;           /** <= adc interrupt */
         ADC_CLOCK_SETUP_T setup;     /** <= adc setup */
         ADC_RESOLUTION_T resolution; /** <= adc resolution */
         int32_t channel;             /** <= current adc channel */
         bool start;                  /** <= adc start conversion flag */
     } adc;
     struct {
         LPC_DAC_T *handler;          /** <= adc handler */
     } dac;
  };
} ciaaDriverAioControl;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverAioControl aioControl[3];

/** \brief Device for ADC 0 */
static ciaaDevices_deviceType ciaaDriverAio_in0 = {
   "aio/in/0",                     /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   &(aioControl[0]),               /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for ADC 1 */
static ciaaDevices_deviceType ciaaDriverAio_in1 = {
   "aio/in/1",                     /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   &(aioControl[1]),               /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

/** \brief Device for DAC 0 */
static ciaaDevices_deviceType ciaaDriverAio_out0 = {
   "aio/out/0",                    /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   &(aioControl[2]),               /** <= layer */
   NULL                            /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaAioDevices[] = {
   &ciaaDriverAio_in0,
   &ciaaDriverAio_in1,
   &ciaaDriverAio_out0
};

static ciaaDriverConstType const ciaaDriverAioConst = {
   ciaaAioDevices,
   3
};

/*==================[external data definition]===============================*/

extern ContextType ActualContext;

/** \brief Aio 0 */
ciaaDriverAio_aioType ciaaDriverAio_aio0;

/** \brief Aio 1 */
ciaaDriverAio_aioType ciaaDriverAio_aio1;

/** \brief Aio 2 */
ciaaDriverAio_aioType ciaaDriverAio_aio2;

/*==================[internal functions definition]==========================*/

/*==================[internal functions definition]==========================*/
static void ciaaDriverAio_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_rxIndication(device->upLayer, nbyte);
}

static void ciaaDriverAio_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_txConfirmation(device->upLayer, 1);
}

static void ciaaDriverAio_adcIRQHandler(ciaaDevices_deviceType const * const device)
{
   uint16_t *ptr;
   uint16_t dataADC;
   ciaaDriverAioControl *pAioControl;

   ContextType ctx = ActualContext;
   ActualContext = CONTEXT_ISR2;
   pAioControl = (ciaaDriverAioControl *) device->layer;

   /* Interrupt mode: Call the stream interrupt handler */
   NVIC_DisableIRQ(pAioControl->adc.interrupt);
   Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, pAioControl->adc.channel, DISABLE);
   Chip_ADC_ReadValue(pAioControl->adc.handler, pAioControl->adc.channel, &dataADC);

   if (pAioControl->aiocnt < UART_AIO_FIFO_SIZE)
   {
      ptr = (uint16_t *) &(pAioControl->hwbuf[pAioControl->aiocnt]);
      *ptr = dataADC;
      pAioControl->aiocnt += sizeof(dataADC);
   }
   ciaaDriverAio_rxIndication(device, pAioControl->aiocnt);

   NVIC_EnableIRQ(pAioControl->adc.interrupt);
   Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, pAioControl->adc.channel, ENABLE);

   ActualContext = ctx;
}

void ciaa_lpc4337_aio_init(void)
{
   /* ADC0 Init */
   aioControl[0].adc.handler = LPC_ADC0;
   aioControl[0].adc.interrupt = ADC0_IRQn;
   aioControl[0].adc.start = false;
   Chip_ADC_Init(aioControl[0].adc.handler, &(aioControl[0].adc.setup));
   aioControl[0].adc.channel = -1;
   Chip_ADC_SetBurstCmd(aioControl[0].adc.handler, DISABLE);

   /* ADC1 Init */
   aioControl[1].adc.handler = LPC_ADC1;
   aioControl[1].adc.interrupt = ADC1_IRQn;
   aioControl[1].adc.start = false;
   Chip_ADC_Init(aioControl[1].adc.handler, &(aioControl[1].adc.setup));
   aioControl[1].adc.channel = -1;
   Chip_ADC_SetBurstCmd(aioControl[1].adc.handler, DISABLE);

   /* DAC Init */
   aioControl[2].dac.handler = LPC_DAC;
   Chip_SCU_DAC_Analog_Config(); //select DAC function
   Chip_DAC_Init(aioControl[2].dac.handler); //initialize DAC
   Chip_DAC_SetDMATimeOut(aioControl[2].dac.handler, 0xFFFF);
   Chip_DAC_ConfigDAConverterControl(aioControl[2].dac.handler, (DAC_CNT_ENA | DAC_DMA_ENA));
}


/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverAio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverAio_close(ciaaDevices_deviceType const * const device)
{
   int32_t ret = -1;
   ciaaDriverAioControl *pAioControl;

   pAioControl = (ciaaDriverAioControl *) device->layer;

   /* Inputs */
   if ((device == ciaaDriverAioConst.devices[0]) || 
       (device == ciaaDriverAioConst.devices[1]))
   {
      NVIC_DisableIRQ(pAioControl->adc.interrupt);
      Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, pAioControl->adc.channel, DISABLE);
      ret = 0;
   }

   /* Outputs */   
   if (device == ciaaDriverAioConst.devices[2])
   {
      ret = 0;
   }

   return ret;
}

extern int32_t ciaaDriverAio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;
   ciaaDriverAioControl *pAioControl;

   pAioControl = (ciaaDriverAioControl *) device->layer;

   /* Inputs */
   if ((device == ciaaDriverAioConst.devices[0]) ||
       (device == ciaaDriverAioConst.devices[1]))
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_SET_CHANNEL:
            NVIC_DisableIRQ(pAioControl->adc.interrupt);
            Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, pAioControl->adc.channel, DISABLE);
            Chip_ADC_EnableChannel(pAioControl->adc.handler, pAioControl->adc.channel, DISABLE);
            switch((int32_t)param)
            {
                case ciaaCHANNEL_0:
                    pAioControl->adc.channel = ADC_CH1;
                    ret = 0;
                    break;
                case ciaaCHANNEL_1:
                    pAioControl->adc.channel = ADC_CH2;
                    ret = 0;
                    break;
                case ciaaCHANNEL_2:
                    pAioControl->adc.channel = ADC_CH3;
                    ret = 0;
                    break;
                case ciaaCHANNEL_3:
                    pAioControl->adc.channel = ADC_CH4;
                    ret = 0;
                    break;
            }
            if (ret == 0)
            {
                NVIC_EnableIRQ(pAioControl->adc.interrupt);
                Chip_ADC_EnableChannel(pAioControl->adc.handler, pAioControl->adc.channel, ENABLE);
                Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, pAioControl->adc.channel, ENABLE);
                pAioControl->adc.start = true;
            }
            break;

         case ciaaPOSIX_IOCTL_SET_SAMPLE_RATE:
            NVIC_DisableIRQ(pAioControl->adc.interrupt);
            Chip_ADC_SetSampleRate(pAioControl->adc.handler, &(pAioControl->adc.setup), (uint32_t)param);
            NVIC_EnableIRQ(pAioControl->adc.interrupt);
            break;

         case ciaaPOSIX_IOCTL_SET_RESOLUTION:
            NVIC_DisableIRQ(pAioControl->adc.interrupt);
            switch((int32_t)param)
            {
                case ciaaRESOLUTION_10BITS:
                    pAioControl->adc.resolution = ADC_10BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_9BITS:
                    pAioControl->adc.resolution = ADC_9BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_8BITS:
                    pAioControl->adc.resolution = ADC_8BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_7BITS:
                    pAioControl->adc.resolution = ADC_7BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_6BITS:
                    pAioControl->adc.resolution = ADC_6BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_5BITS:
                    pAioControl->adc.resolution = ADC_5BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_4BITS:
                    pAioControl->adc.resolution = ADC_4BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_3BITS:
                    pAioControl->adc.resolution = ADC_3BITS;
                    ret = 0;
                    break;
            }
            if (ret == 0)
            {
                Chip_ADC_SetResolution(pAioControl->adc.handler, &(pAioControl->adc.setup), pAioControl->adc.resolution);
                NVIC_EnableIRQ(pAioControl->adc.interrupt);
            }
            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               NVIC_DisableIRQ(pAioControl->adc.interrupt);
               Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, &(pAioControl->adc.setup), DISABLE);
               pAioControl->adc.start = false;
            }
            else
            {
               NVIC_EnableIRQ(pAioControl->adc.interrupt);
               Chip_ADC_Int_SetChannelCmd(pAioControl->adc.handler, &(pAioControl->adc.setup), ENABLE);
               pAioControl->adc.start = true;
            }
            break;
      }
      if (pAioControl->adc.start == true)
      {
         Chip_ADC_SetStartMode(pAioControl->adc.handler, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
      }
   }

   /* Outputs */
   if ((device == ciaaDriverAioConst.devices[2]))
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTTX:
            /* this one calls write */
            ciaaDriverAio_txConfirmation(device);
            ret = 0;
            break;

         case ciaaPOSIX_IOCTL_SET_CHANNEL:
            switch((int32_t)param)
            {
                case ciaaCHANNEL_0:
                    ret = 0;
                    break;
            }
            break;
      }
   }
   return ret;
}

extern int32_t ciaaDriverAio_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, uint32_t size)
{
   uint8_t i;
   int32_t ret = -1;
   ciaaDriverAioControl *pAioControl;

   if (size != 0)
   {
      /* Inputs */
      if ((device == ciaaDriverAioConst.devices[0]) ||
          (device == ciaaDriverAioConst.devices[1]))
      {
         pAioControl = (ciaaDriverAioControl *) device->layer;

         if (size > pAioControl->aiocnt)
         {
            /* buffer has enough space */
            ret = pAioControl->aiocnt;
            pAioControl->aiocnt = 0;
         }
         else
         {
            /* buffer hasn't enough space */
            ret = size;
            pAioControl->aiocnt -= size;
         }
         for(i = 0; i < ret; i++)
         {
            buffer[i] = pAioControl->hwbuf[i];
         }
         if (pAioControl->aiocnt != 0)
         {
            /* We removed data from the buffer, it is time to reorder it */
            for(i = 0; i < pAioControl->aiocnt ; i++)
            {
               pAioControl->hwbuf[i] = pAioControl->hwbuf[i + ret];
            }
         }
      }
      
      /* Outputs */
      if (device == ciaaDriverAioConst.devices[2])
      {
         /* Inputs can't be read. */
         ret = -1;
      }
   }
   return ret;
}

extern int32_t ciaaDriverAio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, uint32_t const size)
{
   int32_t count;
   uint16_t *ptr;
   int32_t ret = -1;
   ciaaDriverAioControl *pAioControl;

   if (size != 0)
   {
      /* Inputs */
      if ((device == ciaaDriverAioConst.devices[0]) ||
          (device == ciaaDriverAioConst.devices[1]))
      {
         /* Inputs can't be written. */
         ret = -1;
      }
      
      /* Outputs */
      if (device == ciaaDriverAioConst.devices[2])
      {
         pAioControl = (ciaaDriverAioControl *) device->layer;

         count = size;
         ptr = (uint16_t *) buffer;
         while(count > 1)
         {
            Chip_DAC_UpdateValue(pAioControl->dac.handler, *ptr);

            /* save actual output state in layer data */
            /* *((ciaaDriverAio_aioType *)device->layer) = *ptr; */

            while (!(Chip_DAC_GetIntStatus(pAioControl->dac.handler))) {}

            count -= 2;
            ptr ++;
         }

         /* 1 byte written */
         ret = size - count;
      }
   }
   return ret;
}

void ciaaDriverAio_init(void)
{
   uint8_t loopi;

   ciaa_lpc4337_aio_init();

   /* add adc/dac driver to the list of devices */
   for(loopi = 0; loopi < ciaaDriverAioConst.countOfDevices; loopi++) {
      /* add each device */
      ciaaSerialDevices_addDriver(ciaaDriverAioConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverAio_aioType *)ciaaDriverAioConst.devices[loopi]->layer) = 0;
   }
}


/*==================[interrupt hanlders]=====================================*/

void ADC0_IRQHandler(void)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in0);
}

void ADC1_IRQHandler(void)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in1);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


/* Copyright 2014, Fernando Beunza
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
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

typedef uint8_t ContextType;

#define AIO_FIFO_SIZE       (16)

typedef struct {
   LPC_ADC_T *handler;                  /** <= adc handler */
   int32_t interrupt;                   /** <= adc interrupt */
   ADC_CLOCK_SETUP_T setup;             /** <= adc setup */
   ADC_RESOLUTION_T resolution;         /** <= adc resolution */
   bool start;                          /** <= adc start conversion flag */
} ciaaDriverAdcControlType;

typedef struct {
   LPC_DAC_T *handler;                  /** <= dac handler */
   LPC_GPDMA_T *dma_handler;            /** <= dma handler */
   int32_t dma_interrupt;               /** <= dma interrupt */
   uint8_t dma_channel;                 /** <= dma channel */
} ciaaDriverDacControlType;

typedef union {
   ciaaDriverAdcControlType adc;        /** <= ADC control */
   ciaaDriverDacControlType dac;        /** <= DAC control */
} ciaaDriverAdcDacControlType;

typedef struct {
   int32_t channel;                     /** <= current channel */
   uint8_t cnt;                         /** <= count */
   uint8_t hwbuf[AIO_FIFO_SIZE];        /** <= buffer */
   ciaaDriverAdcDacControlType adc_dac; /** <= ADC & DAC control */
} ciaaDriverAioControlType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverAioControlType aioControl[3];

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

/*==================[internal functions definition]==========================*/

/*==================[internal functions definition]==========================*/
static void ciaaDriverAio_rxIndication(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_rxIndication(device->upLayer, nbyte);
}

static void ciaaDriverAio_txConfirmation(ciaaDevices_deviceType const * const device, uint32_t const nbyte)
{
   /* receive the data and forward to upper layer */
   ciaaSerialDevices_txConfirmation(device->upLayer, nbyte);
}

static void ciaaDriverAio_adcIRQHandler(ciaaDevices_deviceType const * const device)
{
   ciaaDriverAioControlType *pAioControl;
   uint16_t *ptr;
   uint16_t dataADC;

   pAioControl = (ciaaDriverAioControlType *) device->layer;

   /* Interrupt mode: Call the stream interrupt handler */
   NVIC_DisableIRQ(pAioControl->adc_dac.adc.interrupt);
   Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, pAioControl->channel, DISABLE);
   Chip_ADC_ReadValue(pAioControl->adc_dac.adc.handler, pAioControl->channel, &dataADC);

   if (pAioControl->cnt < AIO_FIFO_SIZE)
   {
      ptr = (uint16_t *) &(pAioControl->hwbuf[pAioControl->cnt]);
      *ptr = dataADC;
      pAioControl->cnt += sizeof(dataADC);
   }
   ciaaDriverAio_rxIndication(device, pAioControl->cnt);

   NVIC_EnableIRQ(pAioControl->adc_dac.adc.interrupt);
   Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, pAioControl->channel, ENABLE);
}

static void ciaaDriverAio_dacIRQHandler(ciaaDevices_deviceType const * const device)
{
   ciaaDriverAioControlType *pAioControl;

   pAioControl = (ciaaDriverAioControlType *) device->layer;
   NVIC_DisableIRQ(pAioControl->adc_dac.dac.dma_interrupt);

   if (Chip_GPDMA_Interrupt(pAioControl->adc_dac.dac.dma_handler, pAioControl->adc_dac.dac.dma_channel) == SUCCESS)
   {
      ciaaDriverAio_txConfirmation(device, pAioControl->cnt);
      Chip_GPDMA_Stop(pAioControl->adc_dac.dac.dma_handler, pAioControl->adc_dac.dac.dma_channel);
   }
   else
   {
       NVIC_EnableIRQ(pAioControl->adc_dac.dac.dma_interrupt);
   }
}

void ciaa_lpc4337_aio_init(void)
{
   /* ADC0 Init */
   aioControl[0].adc_dac.adc.handler = LPC_ADC0;
   aioControl[0].adc_dac.adc.interrupt = ADC0_IRQn;
   aioControl[0].adc_dac.adc.start = false;
   Chip_ADC_Init(aioControl[0].adc_dac.adc.handler, &(aioControl[0].adc_dac.adc.setup));
   aioControl[0].channel = -1;
   Chip_ADC_SetBurstCmd(aioControl[0].adc_dac.adc.handler, DISABLE);

   /* ADC1 Init */
   aioControl[1].adc_dac.adc.handler = LPC_ADC1;
   aioControl[1].adc_dac.adc.interrupt = ADC1_IRQn;
   aioControl[1].adc_dac.adc.start = false;
   Chip_ADC_Init(aioControl[1].adc_dac.adc.handler, &(aioControl[1].adc_dac.adc.setup));
   aioControl[1].channel = -1;
   Chip_ADC_SetBurstCmd(aioControl[1].adc_dac.adc.handler, DISABLE);


   /* DAC Init */
   aioControl[2].adc_dac.dac.handler = LPC_DAC;
   Chip_SCU_DAC_Analog_Config(); //select DAC function
   Chip_DAC_Init(aioControl[2].adc_dac.dac.handler); //initialize DAC
   Chip_DAC_SetBias(aioControl[2].adc_dac.dac.handler, DAC_MAX_UPDATE_RATE_400kHz);
   Chip_DAC_SetDMATimeOut(aioControl[2].adc_dac.dac.handler, 0xffff);
   Chip_DAC_ConfigDAConverterControl(aioControl[2].adc_dac.dac.handler, DAC_CNT_ENA | DAC_DMA_ENA);

   /* Initialize GPDMA controller */
   aioControl[2].adc_dac.dac.dma_handler = LPC_GPDMA;
   Chip_GPDMA_Init(aioControl[2].adc_dac.dac.dma_handler);

   /* Setup GPDMA interrupt */
   aioControl[2].adc_dac.dac.dma_interrupt = DMA_IRQn;
   NVIC_DisableIRQ(aioControl[2].adc_dac.dac.dma_interrupt);
   NVIC_SetPriority(aioControl[2].adc_dac.dac.dma_interrupt, ((0x01 << 3) | 0x01));
   NVIC_EnableIRQ(aioControl[2].adc_dac.dac.dma_interrupt);
}


/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverAio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
   return device;
}

extern int32_t ciaaDriverAio_close(ciaaDevices_deviceType const * const device)
{
   ciaaDriverAioControlType *pAioControl;
   int32_t ret = -1;

   pAioControl = (ciaaDriverAioControlType *) device->layer;

   /* Inputs */
   if ((device == ciaaDriverAioConst.devices[0]) ||
       (device == ciaaDriverAioConst.devices[1]))
   {
      NVIC_DisableIRQ(pAioControl->adc_dac.adc.interrupt);
      Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, pAioControl->channel, DISABLE);
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
   ciaaDriverAioControlType *pAioControl;
   uint32_t freq;
   uint32_t value;
   int32_t ret = -1;

   pAioControl = (ciaaDriverAioControlType *) device->layer;

   /* Inputs */
   if ((device == ciaaDriverAioConst.devices[0]) ||
       (device == ciaaDriverAioConst.devices[1]))
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_SET_CHANNEL:
            NVIC_DisableIRQ(pAioControl->adc_dac.adc.interrupt);
            Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, pAioControl->channel, DISABLE);
            Chip_ADC_EnableChannel(pAioControl->adc_dac.adc.handler, pAioControl->channel, DISABLE);

            ret = 0;
            switch((int32_t)param)
            {
               case ciaaCHANNEL_0:
#if(BOARD==ciaa_nxp)
                  pAioControl->channel = ADC_CH1;
#elif(BOARD==edu_ciaa_nxp)
                  ret = -1;
#endif
                  break;
               case ciaaCHANNEL_1:
#if(BOARD==ciaa_nxp)
                  pAioControl->channel = ADC_CH2;
#elif(BOARD==edu_ciaa_nxp)
                  pAioControl->channel = ADC_CH1;
#endif
                  break;
               case ciaaCHANNEL_2:
#if(BOARD==ciaa_nxp)
                  pAioControl->channel = ADC_CH3;
#elif(BOARD==edu_ciaa_nxp)
                  pAioControl->channel = ADC_CH2;
#endif
                  break;
               case ciaaCHANNEL_3:
#if(BOARD==ciaa_nxp)
                  pAioControl->channel = ADC_CH4;
#elif(BOARD==edu_ciaa_nxp)
                  pAioControl->channel = ADC_CH3;
#endif
                  break;
               default:
                  ret = -1;
                  break;
            }
            if (ret == 0)
            {
                NVIC_EnableIRQ(pAioControl->adc_dac.adc.interrupt);
                Chip_ADC_EnableChannel(pAioControl->adc_dac.adc.handler, pAioControl->channel, ENABLE);
                Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, pAioControl->channel, ENABLE);
                pAioControl->adc_dac.adc.start = true;
            }
            break;

         case ciaaPOSIX_IOCTL_SET_SAMPLE_RATE:
            NVIC_DisableIRQ(pAioControl->adc_dac.adc.interrupt);
            Chip_ADC_SetSampleRate(pAioControl->adc_dac.adc.handler, &(pAioControl->adc_dac.adc.setup), (uint32_t)param);
            NVIC_EnableIRQ(pAioControl->adc_dac.adc.interrupt);
            break;

         case ciaaPOSIX_IOCTL_SET_RESOLUTION:
            NVIC_DisableIRQ(pAioControl->adc_dac.adc.interrupt);
            switch((int32_t)param)
            {
                case ciaaRESOLUTION_10BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_10BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_9BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_9BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_8BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_8BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_7BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_7BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_6BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_6BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_5BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_5BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_4BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_4BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_3BITS:
                    pAioControl->adc_dac.adc.resolution = ADC_3BITS;
                    ret = 0;
                    break;
            }
            if (ret == 0)
            {
                Chip_ADC_SetResolution(pAioControl->adc_dac.adc.handler, &(pAioControl->adc_dac.adc.setup), pAioControl->adc_dac.adc.resolution);
                NVIC_EnableIRQ(pAioControl->adc_dac.adc.interrupt);
            }
            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_RX_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               NVIC_DisableIRQ(pAioControl->adc_dac.adc.interrupt);
               Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, &(pAioControl->adc_dac.adc.setup), DISABLE);
               pAioControl->adc_dac.adc.start = false;
            }
            else
            {
               NVIC_EnableIRQ(pAioControl->adc_dac.adc.interrupt);
               Chip_ADC_Int_SetChannelCmd(pAioControl->adc_dac.adc.handler, &(pAioControl->adc_dac.adc.setup), ENABLE);
               pAioControl->adc_dac.adc.start = true;
            }
            break;
      }
      if (pAioControl->adc_dac.adc.start == true)
      {
         Chip_ADC_SetStartMode(pAioControl->adc_dac.adc.handler, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
      }
   }

   /* Outputs */
   if ((device == ciaaDriverAioConst.devices[2]))
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTTX:
            /* this one calls write */
            ciaaDriverAio_txConfirmation(device, 1);
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

         case ciaaPOSIX_IOCTL_SET_SAMPLE_RATE:
            freq = (uint32_t) param;
            if (freq < 7)
            {
                value = 0xffff;
                Chip_DAC_SetBias(pAioControl->adc_dac.dac.handler, DAC_MAX_UPDATE_RATE_400kHz);
            }
            else if ((freq >= 7) && (freq < 400000))
            {
                value = 400000 / freq;
                Chip_DAC_SetBias(pAioControl->adc_dac.dac.handler, DAC_MAX_UPDATE_RATE_400kHz);
            }
            else
            {
                value = 10000000 / freq;
                Chip_DAC_SetBias(pAioControl->adc_dac.dac.handler, DAC_MAX_UPDATE_RATE_1MHz);
            }
            Chip_DAC_SetDMATimeOut(pAioControl->adc_dac.dac.handler, value);
            break;
      }
   }
   return ret;
}

extern ssize_t ciaaDriverAio_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   ciaaDriverAioControlType *pAioControl;
   ssize_t ret = -1;
   uint8_t i;

   if (size != 0)
   {
      /* Inputs */
      if ((device == ciaaDriverAioConst.devices[0]) ||
          (device == ciaaDriverAioConst.devices[1]))
      {
         pAioControl = (ciaaDriverAioControlType *) device->layer;

         if (size > pAioControl->cnt)
         {
            /* buffer has enough space */
            ret = pAioControl->cnt;
            pAioControl->cnt = 0;
         }
         else
         {
            /* buffer hasn't enough space */
            ret = size;
            pAioControl->cnt -= size;
         }
         for(i = 0; i < ret; i++)
         {
            buffer[i] = pAioControl->hwbuf[i];
         }
         if (pAioControl->cnt != 0)
         {
            /* We removed data from the buffer, it is time to reorder it */
            for(i = 0; i < pAioControl->cnt ; i++)
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

extern ssize_t ciaaDriverAio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ciaaDriverAioControlType *pAioControl;
   uint16_t *ptr;
   ssize_t ret = -1;
   int32_t count;
   uint32_t dmaBuffer[AIO_FIFO_SIZE];
   uint8_t samples;

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
         pAioControl = (ciaaDriverAioControlType *) device->layer;

         /* pre-format the data to DACR register */
         samples = 0;
         count = size;
         ptr = (uint16_t *) buffer;
         while((count > 1) && (samples < AIO_FIFO_SIZE))
         {
            dmaBuffer[samples] = (uint32_t) (DAC_VALUE(*ptr) | DAC_BIAS_EN);
            count -= 2;
            ptr ++;
            samples ++;
         }
         if (samples)
         {
            NVIC_DisableIRQ(pAioControl->adc_dac.dac.dma_interrupt);

            /* Get the free channel for DMA transfer */
            pAioControl->adc_dac.dac.dma_channel = Chip_GPDMA_GetFreeChannel(pAioControl->adc_dac.dac.dma_handler, GPDMA_CONN_DAC);

            /* Start DMA transfer */
            Chip_GPDMA_Transfer(pAioControl->adc_dac.dac.dma_handler, pAioControl->adc_dac.dac.dma_channel,
                                   (uint32_t) &dmaBuffer, GPDMA_CONN_DAC,
                                   GPDMA_TRANSFERTYPE_M2P_CONTROLLER_DMA, samples);

            /* Bytes transfered */
            pAioControl->cnt = size - count;
            ret = pAioControl->cnt;
         }
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
   }
}


/*==================[interrupt hanlders]=====================================*/

ISR(ADC0_IRQHandler)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in0);
}

ISR(ADC1_IRQHandler)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in1);
}

ISR(DMA_IRQHandler)
{
   ciaaDriverAio_dacIRQHandler(&ciaaDriverAio_out0);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


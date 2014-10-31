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
} ciaaDriverAioControl;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
ciaaDriverAioControl aioControl[3];

/** \brief Device for ADC 0 */
static ciaaDevices_deviceType ciaaDriverAio_in0 = {
   "in/0",                        /** <= driver name */
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
   "in/1",                        /** <= driver name */
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
   "out/0",                        /** <= driver name */
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

/** \brief adc0 setup */
static ADC_CLOCK_SETUP_T ciaaDriverAio_adc0_setup;

/** \brief adc1 setup */
static ADC_CLOCK_SETUP_T ciaaDriverAio_adc1_setup;

/** \brief current adc0 channel */
static int32_t ciaaDriverAio_adc0_channel;

/** \brief current adc1 channel */
static int32_t ciaaDriverAio_adc1_channel;

/** \brief adc0 start conversion flag */
static bool ciaaDriverAio_adc0_start;

/** \brief adc1 start conversion flag */
static bool ciaaDriverAio_adc1_start;


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
   ciaaAioDevices_rxIndication(device->upLayer, nbyte);
}

static void ciaaDriverAio_txConfirmation(ciaaDevices_deviceType const * const device)
{
   /* receive the data and forward to upper layer */
   ciaaAioDevices_txConfirmation(device->upLayer, 1);
}

void ciaa_lpc4337_aio_init(void)
{
   /* ADC0 Init */
   ciaaDriverAio_adc0_start = false;
   Chip_ADC_Init(LPC_ADC0, &ciaaDriverAio_adc0_setup);
   ciaaDriverAio_adc0_channel = -1;
   Chip_ADC_SetBurstCmd(LPC_ADC0, DISABLE);

   /* ADC1 Init */
   ciaaDriverAio_adc1_start = false;
   Chip_ADC_Init(LPC_ADC1, &ciaaDriverAio_adc1_setup);
   ciaaDriverAio_adc1_channel = -1;
   Chip_ADC_SetBurstCmd(LPC_ADC1, DISABLE);

   /* DAC Init */
   Chip_SCU_DAC_Analog_Config(); //select DAC function
   Chip_DAC_Init(LPC_DAC); //initialize DAC
   Chip_DAC_SetDMATimeOut(LPC_DAC, 0xFFFF); 
   Chip_DAC_ConfigDAConverterControl(LPC_DAC, (DAC_CNT_ENA | DAC_DMA_ENA));
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

   if (device == ciaaDriverAioConst.devices[0])
   {
      NVIC_DisableIRQ(ADC0_IRQn);
      Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ciaaDriverAio_adc0_channel, DISABLE);
      ret = 0;
   }
   else if (device == ciaaDriverAioConst.devices[1])
   {
      NVIC_DisableIRQ(ADC1_IRQn);
      Chip_ADC_Int_SetChannelCmd(LPC_ADC1, ciaaDriverAio_adc1_channel, DISABLE);
      ret = 0;
   }
   else if (device == ciaaDriverAioConst.devices[2])
   {
      ret = 0;
   }
   return ret;
}

extern int32_t ciaaDriverAio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;
   ADC_RESOLUTION_T resolution;
   int32_t *adc_channel;
   void *pADC;
   int32_t iADC;
   ADC_CLOCK_SETUP_T *adc_setup;
   bool *start_adc;

   if (device == ciaaDriverAioConst.devices[0])
   {
        pADC = LPC_ADC0;
        iADC = ADC0_IRQn;
        adc_setup = &ciaaDriverAio_adc0_setup;
        adc_channel = &ciaaDriverAio_adc0_channel;
        start_adc = &ciaaDriverAio_adc0_start;
   }
   if (device == ciaaDriverAioConst.devices[1])
   {
        pADC = LPC_ADC1;
        iADC = ADC1_IRQn;
        adc_setup = &ciaaDriverAio_adc1_setup;
        adc_channel = &ciaaDriverAio_adc1_channel;
        start_adc = &ciaaDriverAio_adc1_start;
   }

   /* Inputs */
   if ((device == ciaaDriverAioConst.devices[0]) ||
       (device == ciaaDriverAioConst.devices[1]))
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_SET_CHANNEL:
            NVIC_DisableIRQ(iADC);
            Chip_ADC_Int_SetChannelCmd((LPC_ADC_T *) pADC, *adc_channel, DISABLE);
            Chip_ADC_EnableChannel((LPC_ADC_T *) pADC, *adc_channel, DISABLE);
            switch((int32_t)param)
            {
                case ciaaCHANNEL_0:
                    *adc_channel = ADC_CH1;
                    ret = 0;
                    break;
                case ciaaCHANNEL_1:
                    *adc_channel = ADC_CH2;
                    ret = 0;
                    break;
                case ciaaCHANNEL_2:
                    *adc_channel = ADC_CH3;
                    ret = 0;
                    break;
                case ciaaCHANNEL_3:
                    *adc_channel = ADC_CH4;
                    ret = 0;
                    break;
            }
            if (ret == 0)
            {
                NVIC_EnableIRQ(iADC);
                Chip_ADC_EnableChannel((LPC_ADC_T *) pADC, *adc_channel, ENABLE);
                Chip_ADC_Int_SetChannelCmd((LPC_ADC_T *) pADC, *adc_channel, ENABLE);
                *start_adc = true;
            }
            break;

         case ciaaPOSIX_IOCTL_SET_SAMPLE_RATE:
            NVIC_DisableIRQ(iADC);
            Chip_ADC_SetSampleRate((LPC_ADC_T *) pADC, adc_setup, (uint32_t)param);
            NVIC_EnableIRQ(iADC);
            break;

         case ciaaPOSIX_IOCTL_SET_RESOLUTION:
            NVIC_DisableIRQ(iADC);
            switch((int32_t)param)
            {
                case ciaaRESOLUTION_10BITS:
                    resolution = ADC_10BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_9BITS:
                    resolution = ADC_9BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_8BITS:
                    resolution = ADC_8BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_7BITS:
                    resolution = ADC_7BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_6BITS:
                    resolution = ADC_6BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_5BITS:
                    resolution = ADC_5BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_4BITS:
                    resolution = ADC_4BITS;
                    ret = 0;
                    break;
                case ciaaRESOLUTION_3BITS:
                    resolution = ADC_3BITS;
                    ret = 0;
                    break;
            }
            if (ret == 0)
            {
                Chip_ADC_SetResolution((LPC_ADC_T *) pADC, adc_setup, resolution);
                NVIC_EnableIRQ(iADC);
            }
            break;

         case ciaaPOSIX_IOCTL_SET_ENABLE_AI_INTERRUPT:
            if((bool)(intptr_t)param == false)
            {
               NVIC_DisableIRQ(iADC);
               Chip_ADC_Int_SetChannelCmd((LPC_ADC_T *) pADC, adc_setup, DISABLE);
               *start_adc = false;
            }
            else
            {
               NVIC_EnableIRQ(iADC);
               Chip_ADC_Int_SetChannelCmd((LPC_ADC_T *) pADC, adc_setup, ENABLE);
               *start_adc = true;
            }
        	break;
      }
   }

   if (device == ciaaDriverAioConst.devices[0])
   {
      if (ciaaDriverAio_adc0_start == true)
      {
         Chip_ADC_SetStartMode((LPC_ADC_T *) pADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
      }
   }
   if (device == ciaaDriverAioConst.devices[1])
   {
      if (ciaaDriverAio_adc0_start == true)
      {
          Chip_ADC_SetStartMode((LPC_ADC_T *) pADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
      }
   }

   /* Outputs */
   if ((device == ciaaDriverAioConst.devices[2]) )
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_STARTAO:
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
   int32_t ret = -1;
   uint8_t i;
   ciaaDriverAioControl *pAioControl;

   if (size != 0)
   {
      if ((device == ciaaDriverAioConst.devices[0]) ||
          (device == ciaaDriverAioConst.devices[1]) )
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
      else if (device == ciaaDriverAioConst.devices[2])
      {
         /* Inputs can't be read. */
         ret = -1;
      }
      else
      {
         /* Invalid device */
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

   if (size != 0)
   {
      if (device == ciaaDriverAioConst.devices[0])
      {
         /* Inputs can't be written. */
         ret = -1;
      }
      else if (device == ciaaDriverAioConst.devices[1])
      {
         /* Inputs can't be written. */
         ret = -1;
      }
      else if(device == ciaaDriverAioConst.devices[2])
      {
         count = size;
         ptr = (uint16_t *) buffer;
         while(count > 1)
         {
            Chip_DAC_UpdateValue(LPC_DAC, *ptr);

            /* save actual output state in layer data */
            *((ciaaDriverAio_aioType *)device->layer) = *ptr;

            while (!(Chip_DAC_GetIntStatus(LPC_DAC))) {}

            count -= 2;
            ptr ++;
         }

         /* 1 byte written */
         ret = size - count;
      }
      else
      {
         ret = -1;
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
      ciaaAioDevices_addDriver(ciaaDriverAioConst.devices[loopi]);
      /* init layer data for each device */
      *((ciaaDriverAio_aioType *)ciaaDriverAioConst.devices[loopi]->layer) = 0;
   }
}


/*==================[interrupt hanlders]=====================================*/
void ADC0_IRQHandler(void)
{
   uint16_t dataADC;
   uint16_t *ptr;

   ContextType ctx = ActualContext;
   ActualContext = CONTEXT_ISR2;

   /* Interrupt mode: Call the stream interrupt handler */
   NVIC_DisableIRQ(ADC0_IRQn);
   Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ciaaDriverAio_adc0_channel, DISABLE);
   Chip_ADC_ReadValue(LPC_ADC0, ciaaDriverAio_adc0_channel, &dataADC);

   if (aioControl[0].aiocnt < UART_AIO_FIFO_SIZE)
   {
      ptr = &(aioControl[0].hwbuf[aioControl[0].aiocnt]);
      *ptr = dataADC;
      aioControl[0].aiocnt += sizeof(dataADC);
   }
   ciaaDriverAio_rxIndication(&ciaaDriverAio_in0, aioControl[0].aiocnt);

   NVIC_EnableIRQ(ADC0_IRQn);
   Chip_ADC_Int_SetChannelCmd(LPC_ADC0, ciaaDriverAio_adc0_channel, ENABLE);

   ActualContext = ctx;
}

void ADC1_IRQHandler(void)
{
   uint16_t dataADC;
   uint16_t *ptr;

   ContextType ctx = ActualContext;
   ActualContext = CONTEXT_ISR2;

   /* Interrupt mode: Call the stream interrupt handler */
   NVIC_DisableIRQ(ADC1_IRQn);
   Chip_ADC_Int_SetChannelCmd(LPC_ADC1, ciaaDriverAio_adc1_channel, DISABLE);
   Chip_ADC_ReadValue(LPC_ADC1, ciaaDriverAio_adc1_channel, &dataADC);

   if (aioControl[1].aiocnt < UART_AIO_FIFO_SIZE)
   {
      ptr = &(aioControl[1].hwbuf[aioControl[1].aiocnt]);
      *ptr = dataADC;
      aioControl[1].aiocnt += sizeof(dataADC);
   }
   ciaaDriverAio_rxIndication(&ciaaDriverAio_in1, aioControl[1].aiocnt);

   NVIC_EnableIRQ(ADC1_IRQn);
   Chip_ADC_Int_SetChannelCmd(LPC_ADC1, ciaaDriverAio_adc1_channel, ENABLE);

   ActualContext = ctx;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


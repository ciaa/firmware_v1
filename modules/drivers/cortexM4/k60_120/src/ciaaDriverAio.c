/* Copyright 2014, 2015 Alejandro Permingeat, Esteban Volentini
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

/** \brief CIAA Aio Driver for mk60fx512vlq15
 **
 ** Implements the A/D & D/A Drivers for mk60fx512vlq15
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
 * AP           Alejandro Permingeat
 * EsVo         Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150803 v0.0.2 EsVo verify device received in open function
 * 20150329 v0.0.1 AP   first stub version
 */

/*==================[inclusions]=============================================*/
#include "ciaaDriverAio.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "os.h"
#include "fsl_adc16_hal.h"
#include "fsl_uart_hal.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"

/*==================[macros and definitions]=================================*/

#define CIAA_AIO_INPUTS_PIN_NUMBER	8
#define CIAA_AIO_OUTPUTS_PIN_NUMBER	1

#define CIAA_AIO_INPUTS_CHANNELS_PER_ADC    4

/** \brief Pin port resource descriptor Type */
typedef struct ciaaDriverAio_pinStruct {
   PORT_Type * port;                      /** <= i/o port base address */
   uint32_t pin;                          /** <= pin number of i/o port */
   port_mux_t mux;                        /** <= function multiplexor value */
   sim_clock_gate_name_t gate;            /** <= Port clock gate name */
   int32_t channel;
} ciaaDriverAio_pinType;

/** \brief ADC port resource descriptor Type */
typedef struct ciaaDriverAdc_portStruct {
   ADC_Type * base;                      /** <= ADC port base address */
   uint32_t instance;                     /** <= ADC port instance */
   IRQn_Type irq;                         /** <= ADC port interrupt */
   sim_clock_gate_name_t gate;            /** <= ADC clock gate name */
} ciaaDriverAdc_portType;

/** \brief Pointer to Devices */
typedef struct  {
   ciaaDevices_deviceType * const * const devices;
   uint8_t countOfDevices;
} ciaaDriverConstType;

typedef uint8_t ContextType;

#define AIO_FIFO_SIZE       (16)

typedef struct {
    ADC_Type * base;                      /** <= ADC port base address */
    uint32_t instance;                     /** <= ADC port instance */
    IRQn_Type irq;                         /** <= ADC port interrupt */
    sim_clock_gate_name_t gate;            /** <= ADC clock gate name */
    uint32_t    pins_cnt;
    ciaaDriverAio_pinType pin[CIAA_AIO_INPUTS_CHANNELS_PER_ADC]; /** <= Max Channels per ADC */
} ciaaDriverAdcPortType;

typedef struct {
} ciaaDriverDacPortType;

typedef struct {
   int32_t channel;                     /** <= current channel */
   uint8_t cnt;                         /** <= count */
   uint8_t hwbuf[AIO_FIFO_SIZE];        /** <= buffer */
   adc16_converter_config_t config;     /** <= ADC config */
   ciaaDriverAdcPortType port;          /** <= ADC & DAC control */
} ciaaDriverAdcControlType;

typedef struct {
   int32_t channel;                     /** <= current channel */
   uint8_t cnt;                         /** <= count */
   uint8_t hwbuf[AIO_FIFO_SIZE];        /** <= buffer */
   int32_t config;                      /** <= DAC config */
   ciaaDriverDacPortType port;          /** <= DAC & DAC control */
} ciaaDriverDacControlType;

typedef union {
    ciaaDriverAdcControlType adc; /** <= ADC control */
    ciaaDriverDacControlType dac; /** <= DAC control */
} ciaaDriverAioControlType;

/*==================[internal data declaration]==============================*/

static ciaaDriverAdcPortType const ciaaDriverAdc_port0 = {
    ADC2, 0u, ADC2_IRQn, kSimClockGateAdc2, 4u, {
      { PORTA,  26u, kPortPinDisabled, kSimClockGatePortA, 15u },
      { PORTA,  27u, kPortPinDisabled, kSimClockGatePortA, 14u },
      { PORTA,  28u, kPortPinDisabled, kSimClockGatePortA, 13u },
      { PORTA,  29u, kPortPinDisabled, kSimClockGatePortA, 12u }
    }
};

static ciaaDriverAdcPortType const ciaaDriverAdc_port1 = {
    ADC1, 1u, ADC1_IRQn, kSimClockGateAdc1, 2u, {
      { PORTB,   4u, kPortPinDisabled, kSimClockGatePortB,  9u },
      { PORTB,   5u, kPortPinDisabled, kSimClockGatePortB, 10u }
    }
};

static ciaaDriverAdcPortType const ciaaDriverAdc_port2 = {
    ADC0, 2u, ADC0_IRQn, kSimClockGateAdc0, 2u, {
      { PORTB,   2u, kPortPinDisabled, kSimClockGatePortB, 12u },
      { PORTB,   3u, kPortPinDisabled, kSimClockGatePortB, 13u }
    }
};

static ciaaDriverDacPortType const ciaaDriverDac_port0;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief Buffers */
    ciaaDriverAioControlType aioControl[4];

/** \brief Device for ADC 2 */
static ciaaDevices_deviceType ciaaDriverAio_in0 = {
   "aio/in/0",                     /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&(aioControl[0]),        /** <= layer */
   NULL,                           /** <= NULL no lower layer */
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
   (void*)&(aioControl[1]),        /** <= layer */
   NULL,                           /** <= NULL no lower layer */
};

/** \brief Device for ADC 0 */
static ciaaDevices_deviceType ciaaDriverAio_in2 = {
   "aio/in/2",                     /** <= driver name */
   ciaaDriverAio_open,             /** <= open function */
   ciaaDriverAio_close,            /** <= close function */
   ciaaDriverAio_read,             /** <= read function */
   ciaaDriverAio_write,            /** <= write function */
   ciaaDriverAio_ioctl,            /** <= ioctl function */
   NULL,                           /** <= seek function is not provided */
   NULL,                           /** <= upper layer */
   (void*)&(aioControl[2]),        /** <= layer */
   NULL,                           /** <= NULL no lower layer */
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
   (void*)&(aioControl[3]),        /** <= layer */
   NULL,                           /** <= NULL no lower layer */
};

static ciaaDevices_deviceType * const ciaaAioDevices[] = {
   &ciaaDriverAio_in0,
   &ciaaDriverAio_in1,
   &ciaaDriverAio_in2,
   &ciaaDriverAio_out0
};

static ciaaDriverConstType const ciaaDriverAioConst = {
   ciaaAioDevices,
   4
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

}

static void ciaaDriverAio_dacIRQHandler(ciaaDevices_deviceType const * const device)
{

}


/*==================[external functions definition]==========================*/
extern ciaaDevices_deviceType * ciaaDriverAio_open(char const * path,
      ciaaDevices_deviceType * device, uint8_t const oflag)
{
      ciaaDriverAdcControlType * adc;
      ciaaDriverDacControlType * dacCtrl;
      ciaaDriverAioControlType * aio;
      ciaaDriverAioControlType * dac;
      uint8_t index;

   if ((device != &ciaaDriverAio_in0) && (device != &ciaaDriverAio_in1) && (device != &ciaaDriverAio_in2)&&
       (device != &ciaaDriverAio_out0))
   {
      device == NULL;
   }
   else
   {
      aio = device->layer;
      if ((device != &ciaaDriverAio_in0) || (device != &ciaaDriverAio_in1)|| (device != &ciaaDriverAio_in2))
	  {
         adc = &(aio->adc);
         for(index = 0; index < adc->port.pins_cnt; index++)
         {
            /*Initialize all aio pins for current ADC*/

            /* Enable clock for PORTs */
            SIM_HAL_EnableClock(SIM, adc->port.pin[index].gate);

            /*Select Mux Mode*/
            PORT_HAL_SetMuxMode(adc->port.pin[index].port, adc->port.pin[index].pin, adc->port.pin[index].mux);

         }

         /*Initialize the ADC module*/
         /* Enable clock for ADC. */
         SIM_HAL_EnableClock(SIM,adc->port.gate);

         /* Reset all the register to a known state. */
         ADC16_HAL_Init(adc->port.base);

         /* Prepare configuration for current ADC. */
         adc->config.lowPowerEnable = true;
         adc->config.clkDividerMode = kAdc16ClkDividerOf8;
         adc->config.longSampleTimeEnable = true;
         adc->config.resolution = kAdc16ResolutionBitOfSingleEndAs16;
         adc->config.clkSrc = kAdc16ClkSrcOfAsynClk;
         adc->config.asyncClkEnable = true;
         adc->config.highSpeedEnable = false;
         adc->config.longSampleCycleMode = kAdc16LongSampleCycleOf24;
         adc->config.hwTriggerEnable = false;
         adc->config.refVoltSrc = kAdc16RefVoltSrcOfVref;
         adc->config.continuousConvEnable = false;
     #if FSL_FEATURE_ADC16_HAS_DMA
         adc->config.dmaEnable = false;
     #endif /* FSL_FEATURE_ADC16_HAS_DMA */

         /*Apply the ADC prepared configuration*/
         ADC16_HAL_ConfigConverter(adc->port.base, &(adc->config));

         /*Initialize channel*/
         adc->channel=0;

         /*Initialize buffer*/
         adc->cnt=0;

       }
	   else
	   {
		   //DAC
	   }

   }
   return device;
}

extern int32_t ciaaDriverAio_close(ciaaDevices_deviceType const * const device)
{
   int32_t ret = -1;

   return ret;
}

extern int32_t ciaaDriverAio_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;

   return ret;
}

extern ssize_t ciaaDriverAio_read(ciaaDevices_deviceType const * const device, uint8_t * const buffer, size_t const size)
{
   ssize_t ret = -1;

   return ret;
}

extern ssize_t ciaaDriverAio_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = -1;
   return ret;
}

void ciaa_k60_120_aio_init(void)
{
    aioControl[0].adc.port = ciaaDriverAdc_port0;
    aioControl[1].adc.port = ciaaDriverAdc_port1;
    aioControl[2].adc.port = ciaaDriverAdc_port2;
    aioControl[3].dac.port = ciaaDriverDac_port0;
}

void ciaaDriverAio_init(void)
{
	   uint8_t loopi;

	   ciaa_k60_120_aio_init();

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

ISR(ADC2_IRQHandler)
{
   ciaaDriverAio_adcIRQHandler(&ciaaDriverAio_in2);
}

ISR(DMA_IRQHandler)
{
   ciaaDriverAio_dacIRQHandler(&ciaaDriverAio_out0);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


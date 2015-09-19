/* Copyright 2014, 2015 Alejandro Permingeat,
 * Copyright 2015 Esteban Volentini
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

#ifndef _CIAADRIVERAIO_INTERNAL_H_
#define _CIAADRIVERAIO_INTERNAL_H_
/** \brief Internal Header file of AIO Driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup AIO Drivers
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
 * 20150913 v0.0.3 AP   ADC reading implementation through interruption
 * 20150803 v0.0.2 EsVo verify device received in open function
 * 20150329 v0.0.1 AP   first stub version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "fsl_adc16_hal.h"
#include "fsl_uart_hal.h"
#include "fsl_port_hal.h"
#include "fsl_sim_hal.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Size of AIO buffer */
    #define AIO_FIFO_SIZE       (16)

/** \number of input pins (ADC inputs) */
#define CIAA_AIO_INPUTS_PIN_NUMBER  8

/** \number of output pins (ADC inputs) */
#define CIAA_AIO_OUTPUTS_PIN_NUMBER 1

/** \number of channels per ADC module */
#define CIAA_AIO_INPUTS_CHANNELS_PER_ADC    4

/*==================[typedef]================================================*/

/** \brief Pin port resource descriptor Type */
typedef struct ciaaDriverAio_pinStruct {
   PORT_Type * port;                      /** <= i/o port base address */
   uint32_t pin;                          /** <= pin number of i/o port */
   port_mux_t mux;                        /** <= function multiplexor value */
   sim_clock_gate_name_t gate;            /** <= port clock gate name */
   int32_t channel;                       /** <= module channel assigned to this pin */
} ciaaDriverAio_pinType;

/** \brief ADC port resource descriptor Type */
typedef struct ciaaDriverAdc_portStruct {
   ADC_Type * base;                       /** <= ADC port base address */
   uint32_t instance;                     /** <= ADC port instance */
   IRQn_Type irq;                         /** <= ADC port interrupt */
   sim_clock_gate_name_t gate;            /** <= ADC clock gate name */
} ciaaDriverAdc_portType;

typedef struct {
    ADC_Type * base;                      /** <= ADC port base address */
    uint32_t instance;                     /** <= ADC port instance */
    IRQn_Type irq;                         /** <= ADC port interrupt */
    sim_clock_gate_name_t gate;            /** <= ADC clock gate name */
    uint32_t    pins_cnt;
    ciaaDriverAio_pinType pin[CIAA_AIO_INPUTS_CHANNELS_PER_ADC]; /** <= Max Channels per ADC */
} ciaaDriverAdcPortType;

typedef struct {
    /** to be implemented */
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

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADRIVERAIO_INTERNAL_H_ */



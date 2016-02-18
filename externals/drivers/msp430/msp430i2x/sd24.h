/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// sd24.h - Driver for the SD24 Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_SD24_H__
#define __MSP430WARE_SD24_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_SD24__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "inc/hw_memmap.h"
//*****************************************************************************
//
//! \brief Used in the SD24_initConverterAdvanced() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct SD24_initConverterAdvancedParam
{
    //! Selects the converter that will be configured. Check check datasheet
    //! for available converters on device.
    //! \n Valid values are:
    //! - \b SD24_CONVERTER_0
    //! - \b SD24_CONVERTER_1
    //! - \b SD24_CONVERTER_2
    //! - \b SD24_CONVERTER_3
    uint8_t converter;
    //! Determines whether the converter will do continuous samples or a single
    //! sample
    //! \n Valid values are:
    //! - \b SD24_CONTINUOUS_MODE [Default]
    //! - \b SD24_SINGLE_MODE
    uint16_t conversionMode;
    //! \n Valid values are:
    //! - \b SD24_NOT_GROUPED
    //! - \b SD24_GROUPED [Default]
    uint8_t groupEnable;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b SD24_INPUT_CH_ANALOG
    //! - \b SD24_INPUT_CH_TEMPERATURE
    uint8_t inputChannel;
    //! Selects how the data format of the results
    //! \n Valid values are:
    //! - \b SD24_DATA_FORMAT_BINARY [Default]
    //! - \b SD24_DATA_FORMAT_2COMPLEMENT
    uint8_t dataFormat;
    //! Selects the delay for the interrupt
    //! \n Valid values are:
    //! - \b SD24_FIRST_SAMPLE_INTERRUPT
    //! - \b SD24_FOURTH_SAMPLE_INTERRUPT [Default]
    uint8_t interruptDelay;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b SD24_OVERSAMPLE_32
    //! - \b SD24_OVERSAMPLE_64
    //! - \b SD24_OVERSAMPLE_128
    //! - \b SD24_OVERSAMPLE_256
    uint16_t oversampleRatio;
    //! Selects the gain for the converter
    //! \n Valid values are:
    //! - \b SD24_GAIN_1 [Default]
    //! - \b SD24_GAIN_2
    //! - \b SD24_GAIN_4
    //! - \b SD24_GAIN_8
    //! - \b SD24_GAIN_16
    uint8_t gain;
} SD24_initConverterAdvancedParam;

//*****************************************************************************
//
// The following are values that can be passed to the referenceSelect parameter
// for functions: SD24_init().
//
//*****************************************************************************
#define SD24_REF_EXTERNAL                                                (0x00)
#define SD24_REF_INTERNAL                                            (SD24REFS)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: SD24_initConverterAdvanced(); the conversionMode parameter for
// functions: SD24_initConverter().
//
//*****************************************************************************
#define SD24_CONTINUOUS_MODE                                             (0x00)
#define SD24_SINGLE_MODE                                             (SD24SNGL)

//*****************************************************************************
//
// The following are values that can be passed to the converter parameter for
// functions: SD24_initConverter(), SD24_setConverterDataFormat(),
// SD24_startConverterConversion(), SD24_stopConverterConversion(),
// SD24_setInputChannel(), SD24_setInterruptDelay(), SD24_setOversampling(),
// SD24_setGain(), SD24_getResults(), SD24_getHighWordResults(),
// SD24_enableInterrupt(), SD24_disableInterrupt(), SD24_clearInterrupt(), and
// SD24_getInterruptStatus(); the param parameter for functions:
// SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_CONVERTER_0                                                      0
#define SD24_CONVERTER_1                                                      1
#define SD24_CONVERTER_2                                                      2
#define SD24_CONVERTER_3                                                      3

//*****************************************************************************
//
// The following are values that can be passed to the oversampleRatio parameter
// for functions: SD24_setOversampling(); the param parameter for functions:
// SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_OVERSAMPLE_32                                         (SD24OSR_32)
#define SD24_OVERSAMPLE_64                                         (SD24OSR_64)
#define SD24_OVERSAMPLE_128                                       (SD24OSR_128)
#define SD24_OVERSAMPLE_256                                       (SD24OSR_256)

//*****************************************************************************
//
// The following are values that can be passed to the inputChannel parameter
// for functions: SD24_setInputChannel(); the param parameter for functions:
// SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_INPUT_CH_ANALOG                                       (SD24INCH_0)
#define SD24_INPUT_CH_TEMPERATURE                                  (SD24INCH_6)

//*****************************************************************************
//
// The following are values that can be passed to the dataFormat parameter for
// functions: SD24_setConverterDataFormat(); the param parameter for functions:
// SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_DATA_FORMAT_BINARY                                          (0x00)
#define SD24_DATA_FORMAT_2COMPLEMENT                                   (SD24DF)

//*****************************************************************************
//
// The following are values that can be passed to the gain parameter for
// functions: SD24_setGain(); the param parameter for functions:
// SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_GAIN_1                                                (SD24GAIN_1)
#define SD24_GAIN_2                                                (SD24GAIN_2)
#define SD24_GAIN_4                                                (SD24GAIN_4)
#define SD24_GAIN_8                                                (SD24GAIN_8)
#define SD24_GAIN_16                                              (SD24GAIN_16)

//*****************************************************************************
//
// The following are values that can be passed to the interruptDelay parameter
// for functions: SD24_setInterruptDelay(); the param parameter for functions:
// SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_FIRST_SAMPLE_INTERRUPT                                (SD24INTDLY)
#define SD24_FOURTH_SAMPLE_INTERRUPT                                     (0x00)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: SD24_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_NOT_GROUPED                                                 (0x00)
#define SD24_GROUPED                                                  (SD24GRP)

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: SD24_enableInterrupt(), SD24_disableInterrupt(),
// SD24_clearInterrupt(), and SD24_getInterruptStatus() as well as returned by
// the SD24_getInterruptStatus() function.
//
//*****************************************************************************
#define SD24_CONVERTER_INTERRUPT                                      (SD24IFG)
#define SD24_CONVERTER_OVERFLOW_INTERRUPT                           (SD24OVIFG)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the SD24 Module
//!
//! This function initializes the SD24 module sigma-delta analog-to-digital
//! conversions. Specifically the function sets up the clock source for the
//! SD24 core to use for conversions. Upon completion of the initialization the
//! SD24 interrupt registers will be reset and the given parameters will be
//! set. The converter configuration settings are independent of this function.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param referenceSelect selects the reference source for the SD24 core
//!        Valid values are:
//!        - \b SD24_REF_EXTERNAL [Default]
//!        - \b SD24_REF_INTERNAL
//!        \n Modified bits are \b SD24REFS of \b SD24BCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_init(uint16_t baseAddress,
                      uint8_t referenceSelect);

//*****************************************************************************
//
//! \brief Configure SD24 converter
//!
//! This function initializes a converter of the SD24 module. Upon completion
//! the converter will be ready for a conversion and can be started with the
//! SD24_startConverterConversion(). Additional configuration such as data
//! format can be configured in SD24_setConverterDataFormat().
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be configured. Check check
//!        datasheet for available converters on device.
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param conversionMode determines whether the converter will do continuous
//!        samples or a single sample
//!        Valid values are:
//!        - \b SD24_CONTINUOUS_MODE [Default]
//!        - \b SD24_SINGLE_MODE
//!        \n Modified bits are \b SD24SNGL of \b SD24CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_initConverter(uint16_t baseAddress,
                               uint16_t converter,
                               uint16_t conversionMode);

//*****************************************************************************
//
//! \brief Configure SD24 converter - Advanced Configure
//!
//! This function initializes a converter of the SD24 module. Upon completion
//! the converter will be ready for a conversion and can be started with the
//! SD24_startConverterConversion().
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param param is the pointer to struct for converter advanced configuration.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_initConverterAdvanced(uint16_t baseAddress,
                                       SD24_initConverterAdvancedParam *param);

//*****************************************************************************
//
//! \brief Set SD24 converter data format
//!
//! This function sets the converter format so that the resulting data can be
//! viewed in either binary or 2's complement.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be configured. Check check
//!        datasheet for available converters on device.
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param dataFormat selects how the data format of the results
//!        Valid values are:
//!        - \b SD24_DATA_FORMAT_BINARY [Default]
//!        - \b SD24_DATA_FORMAT_2COMPLEMENT
//!        \n Modified bits are \b SD24DFx of \b SD24CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_setConverterDataFormat(uint16_t baseAddress,
                                        uint16_t converter,
                                        uint16_t dataFormat);

//*****************************************************************************
//
//! \brief Start Conversion for Converter
//!
//! This function starts a single converter.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be started
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//!        \n Modified bits are \b SD24SC of \b SD24CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_startConverterConversion(uint16_t baseAddress,
                                          uint8_t converter);

//*****************************************************************************
//
//! \brief Stop Conversion for Converter
//!
//! This function stops a single converter.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be stopped
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//!        \n Modified bits are \b SD24SC of \b SD24CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_stopConverterConversion(uint16_t baseAddress,
                                         uint8_t converter);

//*****************************************************************************
//
//! \brief Configures the input channel
//!
//! This function configures the input channel. For MSP430i2xx devices, users
//! can choose either analog input or internal temperature input.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param inputChannel selects oversampling ratio for the converter
//!        Valid values are:
//!        - \b SD24_INPUT_CH_ANALOG
//!        - \b SD24_INPUT_CH_TEMPERATURE
//!        \n Modified bits are \b SD24INCHx of \b SD24INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_setInputChannel(uint16_t baseAddress,
                                 uint8_t converter,
                                 uint8_t inputChannel);

//*****************************************************************************
//
//! \brief Configures the delay for an interrupt to trigger
//!
//! This function configures the delay for the first interrupt service request
//! for the corresponding converter. This feature delays the interrupt request
//! for a completed conversion by up to four conversion cycles allowing the
//! digital filter to settle prior to generating an interrupt request.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be stopped
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param interruptDelay selects the delay for the interrupt
//!        Valid values are:
//!        - \b SD24_FIRST_SAMPLE_INTERRUPT
//!        - \b SD24_FOURTH_SAMPLE_INTERRUPT [Default]
//!        \n Modified bits are \b SD24INTDLYx of \b SD24INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_setInterruptDelay(uint16_t baseAddress,
                                   uint8_t converter,
                                   uint8_t interruptDelay);

//*****************************************************************************
//
//! \brief Configures the oversampling ratio for a converter
//!
//! This function configures the oversampling ratio for a given converter.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param oversampleRatio selects oversampling ratio for the converter
//!        Valid values are:
//!        - \b SD24_OVERSAMPLE_32
//!        - \b SD24_OVERSAMPLE_64
//!        - \b SD24_OVERSAMPLE_128
//!        - \b SD24_OVERSAMPLE_256
//!        \n Modified bits are \b SD24OSRx of \b SD24OSRx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_setOversampling(uint16_t baseAddress,
                                 uint8_t converter,
                                 uint16_t oversampleRatio);

//*****************************************************************************
//
//! \brief Configures the gain for the converter
//!
//! This function configures the gain for a single converter.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param gain selects the gain for the converter
//!        Valid values are:
//!        - \b SD24_GAIN_1 [Default]
//!        - \b SD24_GAIN_2
//!        - \b SD24_GAIN_4
//!        - \b SD24_GAIN_8
//!        - \b SD24_GAIN_16
//!        \n Modified bits are \b SD24GAINx of \b SD24INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_setGain(uint16_t baseAddress,
                         uint8_t converter,
                         uint8_t gain);

//*****************************************************************************
//
//! \brief Returns the results for a converter
//!
//! This function gets the results from the SD24MEMx register for upper 16-bit
//! and lower 16-bit results, and concatenates them to form a long. The actual
//! result is a maximum 24 bits.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter who's results will be returned
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//!
//! \return Result of conversion
//
//*****************************************************************************
extern uint32_t SD24_getResults(uint16_t baseAddress,
                                uint8_t converter);

//*****************************************************************************
//
//! \brief Returns the high word results for a converter
//!
//! This function gets the upper 16-bit result from the SD24MEMx register and
//! returns it.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter selects the converter who's results will be returned
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//!
//! \return Result of conversion
//
//*****************************************************************************
extern uint16_t SD24_getHighWordResults(uint16_t baseAddress,
                                        uint8_t converter);

//*****************************************************************************
//
//! \brief Enables interrupts for the SD24 Module
//!
//! This function enables interrupts for the SD24 module. Does not clear
//! interrupt flags.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param mask is the bit mask of the converter interrupt sources to be
//!        enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_CONVERTER_INTERRUPT
//!        - \b SD24_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b SD24OVIEx of \b SD24BIE register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_enableInterrupt(uint16_t baseAddress,
                                 uint8_t converter,
                                 uint16_t mask);

//*****************************************************************************
//
//! \brief Disables interrupts for the SD24 Module
//!
//! This function disables interrupts for the SD24 module.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param mask is the bit mask of the converter interrupt sources to be
//!        disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_CONVERTER_INTERRUPT
//!        - \b SD24_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b SD24OVIEx of \b SD24BIE register.
//!
//! Modified bits of \b SD24BIE register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_disableInterrupt(uint16_t baseAddress,
                                  uint8_t converter,
                                  uint16_t mask);

//*****************************************************************************
//
//! \brief Clears interrupts for the SD24 Module
//!
//! This function clears interrupt flags for the SD24 module.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param mask is the bit mask of the converter interrupt sources to clear.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_CONVERTER_INTERRUPT
//!        - \b SD24_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b SD24OVIFGx of \b SD24BIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_clearInterrupt(uint16_t baseAddress,
                                uint8_t converter,
                                uint16_t mask);

//*****************************************************************************
//
//! \brief Returns the interrupt status for the SD24 Module
//!
//! This function returns interrupt flag statuses for the SD24 module.
//!
//! \param baseAddress is the base address of the SD24 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_CONVERTER_0
//!        - \b SD24_CONVERTER_1
//!        - \b SD24_CONVERTER_2
//!        - \b SD24_CONVERTER_3
//! \param mask is the bit mask of the converter interrupt sources to return.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_CONVERTER_INTERRUPT
//!        - \b SD24_CONVERTER_OVERFLOW_INTERRUPT
//!
//! \return Logical OR of any of the following:
//!         - \b SD24_CONVERTER_INTERRUPT
//!         - \b SD24_CONVERTER_OVERFLOW_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint16_t SD24_getInterruptStatus(uint16_t baseAddress,
                                        uint8_t converter,
                                        uint16_t mask);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_SD24_H__

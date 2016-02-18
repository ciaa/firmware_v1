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
// adc.h - Driver for the ADC Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_ADC_H__
#define __MSP430WARE_ADC_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_ADC__

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

//*****************************************************************************
//
// The following are values that can be passed to the clockSourceSelect
// parameter for functions: ADC_init().
//
//*****************************************************************************
#define ADC_CLOCKSOURCE_ADCOSC                                      (ADCSSEL_0)
#define ADC_CLOCKSOURCE_ACLK                                        (ADCSSEL_1)
#define ADC_CLOCKSOURCE_SMCLK                                       (ADCSSEL_2)

//*****************************************************************************
//
// The following are values that can be passed to the clockSourceDivider
// parameter for functions: ADC_init().
//
//*****************************************************************************
#define ADC_CLOCKDIVIDER_1                               (ADCDIV_0 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_2                               (ADCDIV_1 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_3                               (ADCDIV_2 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_4                               (ADCDIV_3 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_5                               (ADCDIV_4 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_6                               (ADCDIV_5 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_7                               (ADCDIV_6 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_8                               (ADCDIV_7 + ADCPDIV_0)
#define ADC_CLOCKDIVIDER_12                              (ADCDIV_2 + ADCPDIV_1)
#define ADC_CLOCKDIVIDER_16                              (ADCDIV_3 + ADCPDIV_1)
#define ADC_CLOCKDIVIDER_20                              (ADCDIV_4 + ADCPDIV_1)
#define ADC_CLOCKDIVIDER_24                              (ADCDIV_5 + ADCPDIV_1)
#define ADC_CLOCKDIVIDER_28                              (ADCDIV_6 + ADCPDIV_1)
#define ADC_CLOCKDIVIDER_32                              (ADCDIV_7 + ADCPDIV_1)
#define ADC_CLOCKDIVIDER_64                              (ADCDIV_0 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_128                             (ADCDIV_1 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_192                             (ADCDIV_2 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_256                             (ADCDIV_3 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_320                             (ADCDIV_4 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_384                             (ADCDIV_5 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_448                             (ADCDIV_6 + ADCPDIV_2)
#define ADC_CLOCKDIVIDER_512                             (ADCDIV_7 + ADCPDIV_2)

//*****************************************************************************
//
// The following are values that can be passed to the
// sampleHoldSignalSourceSelect parameter for functions: ADC_init().
//
//*****************************************************************************
#define ADC_SAMPLEHOLDSOURCE_SC                                      (ADCSHS_0)
#define ADC_SAMPLEHOLDSOURCE_1                                       (ADCSHS_1)
#define ADC_SAMPLEHOLDSOURCE_2                                       (ADCSHS_2)
#define ADC_SAMPLEHOLDSOURCE_3                                       (ADCSHS_3)

//*****************************************************************************
//
// The following are values that can be passed to the multipleSamplesEnabled
// parameter for functions: ADC_setupSamplingTimer().
//
//*****************************************************************************
#define ADC_MULTIPLESAMPLESDISABLE                                  (!(ADCMSC))
#define ADC_MULTIPLESAMPLESENABLE                                      (ADCMSC)

//*****************************************************************************
//
// The following are values that can be passed to the clockCycleHoldCount
// parameter for functions: ADC_setupSamplingTimer().
//
//*****************************************************************************
#define ADC_CYCLEHOLD_4_CYCLES                                       (ADCSHT_0)
#define ADC_CYCLEHOLD_8_CYCLES                                       (ADCSHT_1)
#define ADC_CYCLEHOLD_16_CYCLES                                      (ADCSHT_2)
#define ADC_CYCLEHOLD_32_CYCLES                                      (ADCSHT_3)
#define ADC_CYCLEHOLD_64_CYCLES                                      (ADCSHT_4)
#define ADC_CYCLEHOLD_96_CYCLES                                      (ADCSHT_5)
#define ADC_CYCLEHOLD_128_CYCLES                                     (ADCSHT_6)
#define ADC_CYCLEHOLD_192_CYCLES                                     (ADCSHT_7)
#define ADC_CYCLEHOLD_256_CYCLES                                     (ADCSHT_8)
#define ADC_CYCLEHOLD_384_CYCLES                                     (ADCSHT_9)
#define ADC_CYCLEHOLD_512_CYCLES                                    (ADCSHT_10)
#define ADC_CYCLEHOLD_768_CYCLES                                    (ADCSHT_11)
#define ADC_CYCLEHOLD_1024_CYCLES                                   (ADCSHT_12)

//*****************************************************************************
//
// The following are values that can be passed to the inputSourceSelect
// parameter for functions: ADC_configureMemory().
//
//*****************************************************************************
#define ADC_INPUT_VEREF_N                                           (ADCINCH_0)
#define ADC_INPUT_VEREF_P                                           (ADCINCH_1)
#define ADC_INPUT_A2                                                (ADCINCH_2)
#define ADC_INPUT_A3                                                (ADCINCH_3)
#define ADC_INPUT_A4                                                (ADCINCH_4)
#define ADC_INPUT_A5                                                (ADCINCH_5)
#define ADC_INPUT_A6                                                (ADCINCH_6)
#define ADC_INPUT_A7                                                (ADCINCH_7)
#define ADC_INPUT_A8                                                (ADCINCH_8)
#define ADC_INPUT_A9                                                (ADCINCH_9)
#define ADC_INPUT_TEMPSENSOR                                       (ADCINCH_12)
#define ADC_INPUT_REFVOLTAGE                                       (ADCINCH_13)
#define ADC_INPUT_DVSS                                             (ADCINCH_14)
#define ADC_INPUT_DVCC                                             (ADCINCH_15)

//*****************************************************************************
//
// The following are values that can be passed to the
// positiveRefVoltageSourceSelect parameter for functions:
// ADC_configureMemory().
//
//*****************************************************************************
#define ADC_VREFPOS_AVCC                               (!(ADCSREF0 + ADCSREF1))
#define ADC_VREFPOS_INT                                              (ADCSREF0)
#define ADC_VREFPOS_EXT_BUF                                          (ADCSREF1)
#define ADC_VREFPOS_EXT_NOBUF                             (ADCSREF1 + ADCSREF0)

//*****************************************************************************
//
// The following are values that can be passed to the
// negativeRefVoltageSourceSelect parameter for functions:
// ADC_configureMemory().
//
//*****************************************************************************
#define ADC_VREFNEG_AVSS                                          (!(ADCSREF2))
#define ADC_VREFNEG_EXT                                              (ADCSREF2)

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: ADC_enableInterrupt(), and ADC_disableInterrupt().
//
//*****************************************************************************
#define ADC_OVERFLOW_INTERRUPT                                        (ADCOVIE)
#define ADC_TIMEOVERFLOW_INTERRUPT                                   (ADCTOVIE)
#define ADC_ABOVETHRESHOLD_INTERRUPT                                  (ADCHIIE)
#define ADC_BELOWTHRESHOLD_INTERRUPT                                  (ADCLOIE)
#define ADC_INSIDEWINDOW_INTERRUPT                                    (ADCINIE)
#define ADC_COMPLETED_INTERRUPT                                        (ADCIE0)

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlagMask
// parameter for functions: ADC_clearInterrupt(), and ADC_getInterruptStatus().
//
//*****************************************************************************
#define ADC_OVERFLOW_INTERRUPT_FLAG                                  (ADCOVIFG)
#define ADC_TIMEOVERFLOW_INTERRUPT_FLAG                             (ADCTOVIFG)
#define ADC_ABOVETHRESHOLD_INTERRUPT_FLAG                            (ADCHIIFG)
#define ADC_BELOWTHRESHOLD_INTERRUPT_FLAG                            (ADCLOIFG)
#define ADC_INSIDEWINDOW_INTERRUPT_FLAG                              (ADCINIFG)
#define ADC_COMPLETED_INTERRUPT_FLAG                                  (ADCIFG0)

//*****************************************************************************
//
// The following are values that can be passed to the
// conversionSequenceModeSelect parameter for functions: ADC_startConversion().
//
//*****************************************************************************
#define ADC_SINGLECHANNEL                                         (ADCCONSEQ_0)
#define ADC_SEQOFCHANNELS                                         (ADCCONSEQ_1)
#define ADC_REPEATED_SINGLECHANNEL                                (ADCCONSEQ_2)
#define ADC_REPEATED_SEQOFCHANNELS                                (ADCCONSEQ_3)

//*****************************************************************************
//
// The following are values that can be passed to the preempt parameter for
// functions: ADC_disableConversions().
//
//*****************************************************************************
#define ADC_COMPLETECONVERSION                                            false
#define ADC_PREEMPTCONVERSION                                              true

//*****************************************************************************
//
// The following are values that can be passed to the resolutionSelect
// parameter for functions: ADC_setResolution().
//
//*****************************************************************************
#define ADC_RESOLUTION_8BIT                                         (!(ADCRES))
#define ADC_RESOLUTION_10BIT                                           (ADCRES)

//*****************************************************************************
//
// The following are values that can be passed to the invertedSignal parameter
// for functions: ADC_setSampleHoldSignalInversion().
//
//*****************************************************************************
#define ADC_NONINVERTEDSIGNAL                                      (!(ADCISSH))
#define ADC_INVERTEDSIGNAL                                            (ADCISSH)

//*****************************************************************************
//
// The following are values that can be passed to the readBackFormat parameter
// for functions: ADC_setDataReadBackFormat().
//
//*****************************************************************************
#define ADC_UNSIGNED_BINARY                                          (!(ADCDF))
#define ADC_SIGNED_2SCOMPLEMENT                                         (ADCDF)

//*****************************************************************************
//
// The following are values that can be passed to the samplingRateSelect
// parameter for functions: ADC_setReferenceBufferSamplingRate().
//
//*****************************************************************************
#define ADC_MAXSAMPLINGRATE_200KSPS                                  (!(ADCSR))
#define ADC_MAXSAMPLINGRATE_50KSPS                                      (ADCSR)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the ADC_isBusy() function.
//
//*****************************************************************************
#define ADC_NOTBUSY                                                      (0x00)
#define ADC_BUSY                                                      (ADCBUSY)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the ADC Module.
//!
//! This function initializes the ADC module to allow for analog-to-digital
//! conversions. Specifically this function sets up the sample-and-hold signal
//! and clock sources for the ADC core to use for conversions. Upon successful
//! completion of the initialization all of the ADC control registers will be
//! reset, excluding the memory controls and reference module bits, the given
//! parameters will be set, and the ADC core will be turned on (Note, that the
//! ADC core only draws power during conversions and remains off when not
//! converting).Note that sample/hold signal sources are device dependent. Note
//! that if re-initializing the ADC after starting a conversion with the
//! startConversion() function, the disableConversion() must be called BEFORE
//! this function can be called.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param sampleHoldSignalSourceSelect is the signal that will trigger a
//!        sample-and-hold for an input signal to be converted. This parameter
//!        is device specific and sources should be found in the device's
//!        datasheet.
//!        Valid values are:
//!        - \b ADC_SAMPLEHOLDSOURCE_SC [Default]
//!        - \b ADC_SAMPLEHOLDSOURCE_1
//!        - \b ADC_SAMPLEHOLDSOURCE_2
//!        - \b ADC_SAMPLEHOLDSOURCE_3
//!        \n Modified bits are \b ADCSHSx of \b ADCCTL1 register.
//! \param clockSourceSelect selects the clock that will be used by the ADC
//!        core and the sampling timer if a sampling pulse mode is enabled.
//!        Valid values are:
//!        - \b ADC_CLOCKSOURCE_ADCOSC [Default] - MODOSC 5 MHz oscillator from
//!           the clock system
//!        - \b ADC_CLOCKSOURCE_ACLK - The Auxilary Clock
//!        - \b ADC_CLOCKSOURCE_SMCLK - The Sub-Master Clock
//!        \n Modified bits are \b ADCSSELx of \b ADCCTL1 register.
//! \param clockSourceDivider selects the amount that the clock will be
//!        divided.
//!        Valid values are:
//!        - \b ADC_CLOCKDIVIDER_1 [Default]
//!        - \b ADC_CLOCKDIVIDER_2
//!        - \b ADC_CLOCKDIVIDER_3
//!        - \b ADC_CLOCKDIVIDER_4
//!        - \b ADC_CLOCKDIVIDER_5
//!        - \b ADC_CLOCKDIVIDER_6
//!        - \b ADC_CLOCKDIVIDER_7
//!        - \b ADC_CLOCKDIVIDER_8
//!        - \b ADC_CLOCKDIVIDER_12
//!        - \b ADC_CLOCKDIVIDER_16
//!        - \b ADC_CLOCKDIVIDER_20
//!        - \b ADC_CLOCKDIVIDER_24
//!        - \b ADC_CLOCKDIVIDER_28
//!        - \b ADC_CLOCKDIVIDER_32
//!        - \b ADC_CLOCKDIVIDER_64
//!        - \b ADC_CLOCKDIVIDER_128
//!        - \b ADC_CLOCKDIVIDER_192
//!        - \b ADC_CLOCKDIVIDER_256
//!        - \b ADC_CLOCKDIVIDER_320
//!        - \b ADC_CLOCKDIVIDER_384
//!        - \b ADC_CLOCKDIVIDER_448
//!        - \b ADC_CLOCKDIVIDER_512
//!        \n Modified bits are \b ADCDIVx of \b ADCCTL1 register; bits \b
//!        ADCPDIVx of \b ADCCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_init(uint16_t baseAddress,
                     uint16_t sampleHoldSignalSourceSelect,
                     uint8_t clockSourceSelect,
                     uint16_t clockSourceDivider);

//*****************************************************************************
//
//! \brief Enables the ADC block.
//!
//! This will enable operation of the ADC block.
//!
//! \param baseAddress is the base address of the ADC module.
//!
//! Modified bits are \b ADCON of \b ADCCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the ADC block.
//!
//! This will disable operation of the ADC block.
//!
//! \param baseAddress is the base address of the ADC module.
//!
//! Modified bits are \b ADCON of \b ADCCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets up and enables the Sampling Timer Pulse Mode.
//!
//! This function sets up the sampling timer pulse mode which allows the
//! sample/hold signal to trigger a sampling timer to sample-and-hold an input
//! signal for a specified number of clock cycles without having to hold the
//! sample/hold signal for the entire period of sampling. Note that if a
//! conversion has been started with the startConversion() function, then a
//! call to disableConversions() is required before this function may be
//! called.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param clockCycleHoldCount sets the amount of clock cycles to sample-and-
//!        hold for the memory buffer.
//!        Valid values are:
//!        - \b ADC_CYCLEHOLD_4_CYCLES [Default]
//!        - \b ADC_CYCLEHOLD_8_CYCLES
//!        - \b ADC_CYCLEHOLD_16_CYCLES
//!        - \b ADC_CYCLEHOLD_32_CYCLES
//!        - \b ADC_CYCLEHOLD_64_CYCLES
//!        - \b ADC_CYCLEHOLD_96_CYCLES
//!        - \b ADC_CYCLEHOLD_128_CYCLES
//!        - \b ADC_CYCLEHOLD_192_CYCLES
//!        - \b ADC_CYCLEHOLD_256_CYCLES
//!        - \b ADC_CYCLEHOLD_384_CYCLES
//!        - \b ADC_CYCLEHOLD_512_CYCLES
//!        - \b ADC_CYCLEHOLD_768_CYCLES
//!        - \b ADC_CYCLEHOLD_1024_CYCLES
//!        \n Modified bits are \b ADCSHTx of \b ADCCTL0 register.
//! \param multipleSamplesEnabled allows multiple conversions to start without
//!        a trigger signal from the sample/hold signal
//!        Valid values are:
//!        - \b ADC_MULTIPLESAMPLESDISABLE - a timer trigger will be needed to
//!           start every ADC conversion.
//!        - \b ADC_MULTIPLESAMPLESENABLE - during a sequenced and/or repeated
//!           conversion mode, after the first conversion, no sample/hold
//!           signal is necessary to start subsequent samples.
//!        \n Modified bits are \b ADCMSC of \b ADCCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_setupSamplingTimer(uint16_t baseAddress,
                                   uint16_t clockCycleHoldCount,
                                   uint16_t multipleSamplesEnabled);

//*****************************************************************************
//
//! \brief Disables Sampling Timer Pulse Mode.
//!
//! Disables the Sampling Timer Pulse Mode. Note that if a conversion has been
//! started with the startConversion() function, then a call to
//! disableConversions() is required before this function may be called.
//!
//! \param baseAddress is the base address of the ADC module.
//!
//! Modified bits are \b ADCSHP of \b ADCCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_disableSamplingTimer(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Configures the controls of the selected memory buffer.
//!
//! Maps an input signal conversion into the memory buffer, as well as the
//! positive and negative reference voltages for each conversion being stored
//! into the memory buffer. If the internal reference is used for the positive
//! reference voltage, the internal REF module has to control the voltage
//! level. Note that if a conversion has been started with the
//! startConversion() function, then a call to disableConversions() is required
//! before this function may be called.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param inputSourceSelect is the input that will store the converted data
//!        into the specified memory buffer.
//!        Valid values are:
//!        - \b ADC_INPUT_VEREF_N [Default]
//!        - \b ADC_INPUT_VEREF_P
//!        - \b ADC_INPUT_A2
//!        - \b ADC_INPUT_A3
//!        - \b ADC_INPUT_A4
//!        - \b ADC_INPUT_A5
//!        - \b ADC_INPUT_A6
//!        - \b ADC_INPUT_A7
//!        - \b ADC_INPUT_A8 - [Valid for FR4xx devices]
//!        - \b ADC_INPUT_A9 - [Valid for FR4xx devices]
//!        - \b ADC_INPUT_TEMPSENSOR
//!        - \b ADC_INPUT_REFVOLTAGE
//!        - \b ADC_INPUT_DVSS
//!        - \b ADC_INPUT_DVCC
//!        \n Modified bits are \b ADCINCHx of \b ADCMCTL0 register.
//! \param positiveRefVoltageSourceSelect is the reference voltage source to
//!        set as the upper limit for the conversion that is to be stored in
//!        the specified memory buffer.
//!        Valid values are:
//!        - \b ADC_VREFPOS_AVCC [Default]
//!        - \b ADC_VREFPOS_INT
//!        - \b ADC_VREFPOS_EXT_BUF
//!        - \b ADC_VREFPOS_EXT_NOBUF
//!        \n Modified bits are \b ADCSREF of \b ADCMCTL0 register.
//! \param negativeRefVoltageSourceSelect is the reference voltage source to
//!        set as the lower limit for the conversion that is to be stored in
//!        the specified memory buffer.
//!        Valid values are:
//!        - \b ADC_VREFNEG_AVSS [Default]
//!        - \b ADC_VREFNEG_EXT
//!        \n Modified bits are \b ADCSREF of \b ADCMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_configureMemory(uint16_t baseAddress,
                                uint8_t inputSourceSelect,
                                uint8_t positiveRefVoltageSourceSelect,
                                uint8_t negativeRefVoltageSourceSelect);

//*****************************************************************************
//
//! \brief Enables selected ADC interrupt sources.
//!
//! Enables the indicated ADC interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. <b>Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param interruptMask is the bit mask of the memory buffer interrupt sources
//!        to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC_OVERFLOW_INTERRUPT - Interrupts when a new conversion is
//!           about to overwrite the previous one
//!        - \b ADC_TIMEOVERFLOW_INTERRUPT - Interrupts when a new conversion
//!           is starting before the previous one has finished
//!        - \b ADC_ABOVETHRESHOLD_INTERRUPT - Interrups when the input signal
//!           has gone above the high threshold of the window comparator
//!        - \b ADC_BELOWTHRESHOLD_INTERRUPT - Interrupts when the input signal
//!           has gone below the low threshold of the low window comparator
//!        - \b ADC_INSIDEWINDOW_INTERRUPT - Interrupts when the input signal
//!           is in between the high and low thresholds of the window
//!           comparator
//!        - \b ADC_COMPLETED_INTERRUPT - Interrupt for new conversion data in
//!           the memory buffer
//!
//! Modified bits of \b ADCIE register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_enableInterrupt(uint16_t baseAddress,
                                uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected ADC interrupt sources.
//!
//! Disables the indicated ADC interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param interruptMask is the bit mask of the memory buffer interrupt sources
//!        to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC_OVERFLOW_INTERRUPT - Interrupts when a new conversion is
//!           about to overwrite the previous one
//!        - \b ADC_TIMEOVERFLOW_INTERRUPT - Interrupts when a new conversion
//!           is starting before the previous one has finished
//!        - \b ADC_ABOVETHRESHOLD_INTERRUPT - Interrups when the input signal
//!           has gone above the high threshold of the window comparator
//!        - \b ADC_BELOWTHRESHOLD_INTERRUPT - Interrupts when the input signal
//!           has gone below the low threshold of the low window comparator
//!        - \b ADC_INSIDEWINDOW_INTERRUPT - Interrupts when the input signal
//!           is in between the high and low thresholds of the window
//!           comparator
//!        - \b ADC_COMPLETED_INTERRUPT - Interrupt for new conversion data in
//!           the memory buffer
//!
//! Modified bits of \b ADCIE register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_disableInterrupt(uint16_t baseAddress,
                                 uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Clears ADC10B selected interrupt flags.
//!
//! The selected ADC interrupt flags are cleared, so that it no longer asserts.
//! The memory buffer interrupt flags are only cleared when the memory buffer
//! is accessed.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to be
//!        cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC_OVERFLOW_INTERRUPT_FLAG - Interrupt flag for when a new
//!           conversion is about to overwrite the previous one
//!        - \b ADC_TIMEOVERFLOW_INTERRUPT_FLAG - Interrupt flag for when a new
//!           conversion is starting before the previous one has finished
//!        - \b ADC_ABOVETHRESHOLD_INTERRUPT_FLAG - Interrup flag for when the
//!           input signal has gone above the high threshold of the window
//!           comparator
//!        - \b ADC_BELOWTHRESHOLD_INTERRUPT_FLAG - Interrupt flag for when the
//!           input signal has gone below the low threshold of the window
//!           comparator
//!        - \b ADC_INSIDEWINDOW_INTERRUPT_FLAG - Interrupt flag for when the
//!           input signal is in between the high and low thresholds of the
//!           window comparator
//!        - \b ADC_COMPLETED_INTERRUPT_FLAG - Interrupt flag for new
//!           conversion data in the memory buffer
//!
//! Modified bits of \b ADCIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_clearInterrupt(uint16_t baseAddress,
                               uint8_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Returns the status of the selected memory interrupt flags.
//!
//! Returns the status of the selected interrupt flags.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param interruptFlagMask is a bit mask of the interrupt flags status to be
//!        returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC_OVERFLOW_INTERRUPT_FLAG - Interrupt flag for when a new
//!           conversion is about to overwrite the previous one
//!        - \b ADC_TIMEOVERFLOW_INTERRUPT_FLAG - Interrupt flag for when a new
//!           conversion is starting before the previous one has finished
//!        - \b ADC_ABOVETHRESHOLD_INTERRUPT_FLAG - Interrup flag for when the
//!           input signal has gone above the high threshold of the window
//!           comparator
//!        - \b ADC_BELOWTHRESHOLD_INTERRUPT_FLAG - Interrupt flag for when the
//!           input signal has gone below the low threshold of the window
//!           comparator
//!        - \b ADC_INSIDEWINDOW_INTERRUPT_FLAG - Interrupt flag for when the
//!           input signal is in between the high and low thresholds of the
//!           window comparator
//!        - \b ADC_COMPLETED_INTERRUPT_FLAG - Interrupt flag for new
//!           conversion data in the memory buffer
//!
//! Modified bits of \b ADC10IFG register.
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************
extern uint8_t ADC_getInterruptStatus(uint16_t baseAddress,
                                      uint8_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Enables/Starts an Analog-to-Digital Conversion.
//!
//! This function enables/starts the conversion process of the ADC. If the
//! sample/hold signal source chosen during initialization was ADCOSC, then the
//! conversion is started immediately, otherwise the chosen sample/hold signal
//! source starts the conversion by a rising edge of the signal. Keep in mind
//! when selecting conversion modes, that for sequenced                 and/or
//! repeated modes, to keep the sample/hold-and-convert process continuing
//! without a trigger from the sample/hold signal source, the multiple samples
//! must be enabled using the ADC_setupSamplingTimer() function. Also note that
//! when a sequence conversion mode is selected, the first input channel is the
//! one mapped to the memory buffer, the next input channel selected for
//! conversion is one less than the input channel just converted (i.e. A1 comes
//! after A2), until A0 is reached, and if in repeating mode, then the next
//! input channel will again be the one mapped to the memory buffer. Note that
//! after this function is called, the ADC_stopConversions() has to be called
//! to re-initialize the ADC, reconfigure a memory buffer control,
//! enable/disable the sampling timer, or to change the internal reference
//! voltage.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param conversionSequenceModeSelect determines the ADC operating mode.
//!        Valid values are:
//!        - \b ADC_SINGLECHANNEL [Default] - one-time conversion of a single
//!           channel into a single memory buffer
//!        - \b ADC_SEQOFCHANNELS - one time conversion of multiple channels
//!           into the specified starting memory buffer and each subsequent
//!           memory buffer up until the conversion is stored in a memory
//!           buffer dedicated as the end-of-sequence by the memory's control
//!           register
//!        - \b ADC_REPEATED_SINGLECHANNEL - repeated conversions of one
//!           channel into a single memory buffer
//!        - \b ADC_REPEATED_SEQOFCHANNELS - repeated conversions of multiple
//!           channels into the specified starting memory buffer and each
//!           subsequent memory buffer up until the conversion is stored in a
//!           memory buffer dedicated as the end-of-sequence by the memory's
//!           control register
//!        \n Modified bits are \b ADCCONSEQx of \b ADCCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_startConversion(uint16_t baseAddress,
                                uint8_t conversionSequenceModeSelect);

//*****************************************************************************
//
//! \brief Disables the ADC from converting any more signals.
//!
//! Disables the ADC from converting any more signals. If there is a conversion
//! in progress, this function can stop it immediatly if the preempt parameter
//! is set as ADC_PREEMPTCONVERSION, by changing the conversion mode to single-
//! channel, single-conversion and disabling conversions. If the conversion
//! mode is set as single-channel, single-conversion and this function is
//! called without preemption, then the ADC core conversion status is polled
//! until the conversion is complete before disabling conversions to prevent
//! unpredictable data. If the ADC_startConversion() has been called, then this
//! function has to be called to re-initialize the ADC, reconfigure a memory
//! buffer control, enable/disable the sampling pulse mode, or change the
//! internal reference voltage.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param preempt specifies if the current conversion should be preemptly
//!        stopped before the end of the conversion
//!        Valid values are:
//!        - \b ADC_COMPLETECONVERSION - Allows the ADC to end the current
//!           conversion before disabling conversions.
//!        - \b ADC_PREEMPTCONVERSION - Stops the ADC10B immediatly, with
//!           unpredicatble results of the current conversion. Cannot be used
//!           with repeated conversion.
//!
//! Modified bits of \b ADCCTL0 register and bits of \b ADCCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_disableConversions(uint16_t baseAddress,
                                   bool preempt);

//*****************************************************************************
//
//! \brief Returns the raw contents of the specified memory buffer.
//!
//! Returns the raw contents of the specified memory buffer. The format of the
//! content depends on the read-back format of the data: if the data is in
//! signed 2's complement format then the contents in the memory buffer will be
//! left-justified with the least-siginificant bits as 0's, whereas if the data
//! is in unsigned format then the contents in the memory buffer will be right-
//! justified with the most-significant bits as 0's.
//!
//! \param baseAddress is the base address of the ADC module.
//!
//! \return A Signed Integer of the contents of the specified memory buffer.
//
//*****************************************************************************
extern int16_t ADC_getResults(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Use to change the resolution of the converted data.
//!
//! This function can be used to change the resolution of the converted data
//! from the default of 12-bits.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param resolutionSelect determines the resolution of the converted data.
//!        Valid values are:
//!        - \b ADC_RESOLUTION_8BIT
//!        - \b ADC_RESOLUTION_10BIT [Default]
//!        \n Modified bits are \b ADCRES of \b ADCCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_setResolution(uint16_t baseAddress,
                              uint8_t resolutionSelect);

//*****************************************************************************
//
//! \brief Use to invert or un-invert the sample/hold signal
//!
//! This function can be used to invert or un-invert the sample/hold signal.
//! Note that if a conversion has been started with the startConversion()
//! function, then a call to disableConversions() is required before this
//! function may be called.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param invertedSignal set if the sample/hold signal should be inverted
//!        Valid values are:
//!        - \b ADC_NONINVERTEDSIGNAL [Default] - a sample-and-hold of an input
//!           signal for conversion will be started on a rising edge of the
//!           sample/hold signal.
//!        - \b ADC_INVERTEDSIGNAL - a sample-and-hold of an input signal for
//!           conversion will be started on a falling edge of the sample/hold
//!           signal.
//!        \n Modified bits are \b ADCISSH of \b ADCCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_setSampleHoldSignalInversion(uint16_t baseAddress,
                                             uint16_t invertedSignal);

//*****************************************************************************
//
//! \brief Use to set the read-back format of the converted data
//!
//! Sets the format of the converted data: how it will be stored into the
//! memory buffer, and how it should be read back. The format can be set as
//! right-justified (default), which indicates that the number will be
//! unsigned, or left-justified, which indicates that the number will be signed
//! in 2's complement format. This change affects all memory buffers for
//! subsequent conversions.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param readBackFormat is the specified format to store the conversions in
//!        the memory buffer.
//!        Valid values are:
//!        - \b ADC_UNSIGNED_BINARY [Default]
//!        - \b ADC_SIGNED_2SCOMPLEMENT
//!        \n Modified bits are \b ADCDF of \b ADCCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_setDataReadBackFormat(uint16_t baseAddress,
                                      uint16_t readBackFormat);

//*****************************************************************************
//
//! \brief Use to set the reference buffer's sampling rate.
//!
//! Sets the reference buffer's sampling rate to the selected sampling rate.
//! The default sampling rate is maximum of 200-ksps, and can be reduced to a
//! maximum of 50-ksps to conserve power.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param samplingRateSelect is the specified maximum sampling rate.
//!        Valid values are:
//!        - \b ADC_MAXSAMPLINGRATE_200KSPS [Default]
//!        - \b ADC_MAXSAMPLINGRATE_50KSPS
//!        \n Modified bits are \b ADCSR of \b ADCCTL2 register.
//!
//! Modified bits of \b ADCCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_setReferenceBufferSamplingRate(uint16_t baseAddress,
                                               uint16_t samplingRateSelect);

//*****************************************************************************
//
//! \brief Sets the high and low threshold for the window comparator feature.
//!
//! Sets the high and low threshold for the window comparator feature. Use the
//! ADCHIIE, ADCINIE, ADCLOIE interrupts to utilize this feature.
//!
//! \param baseAddress is the base address of the ADC module.
//! \param highThreshold is the upper bound that could trip an interrupt for
//!        the window comparator.
//! \param lowThreshold is the lower bound that could trip on interrupt for the
//!        window comparator.
//!
//! Modified bits of \b ADCLO register and bits of \b ADCHI register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC_setWindowComp(uint16_t baseAddress,
                              uint16_t highThreshold,
                              uint16_t lowThreshold);

//*****************************************************************************
//
//! \brief Returns the address of the memory buffer for the DMA module.
//!
//! \param baseAddress is the base address of the ADC module.
//!
//! \return the address of the memory buffer. This can be used in conjunction
//!         with the DMA to store the converted data directly to memory.
//
//*****************************************************************************
extern uint32_t ADC_getMemoryAddressForDMA(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the busy status of the ADC core.
//!
//! Returns the status of the ADC core if there is a conversion currently
//! taking place.
//!
//! \param baseAddress is the base address of the ADC module.
//!
//! \return ADC_BUSY or ADC_NOTBUSY dependent if there is a conversion
//!         currently taking place.
//!         Return one of the following:
//!         - \b ADC_NOTBUSY
//!         - \b ADC_BUSY
//
//*****************************************************************************
extern uint8_t ADC_isBusy(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_ADC_H__

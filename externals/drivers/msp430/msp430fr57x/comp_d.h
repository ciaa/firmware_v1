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
// comp_d.h - Driver for the COMP_D Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_COMP_D_H__
#define __MSP430WARE_COMP_D_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_COMPD__

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
//! \brief Used in the Comp_D_init() function as the param parameter.
//
//*****************************************************************************
typedef struct Comp_D_initParam
{
    //! Selects the input to the positive terminal.
    //! \n Valid values are:
    //! - \b COMP_D_INPUT0 [Default]
    //! - \b COMP_D_INPUT1
    //! - \b COMP_D_INPUT2
    //! - \b COMP_D_INPUT3
    //! - \b COMP_D_INPUT4
    //! - \b COMP_D_INPUT5
    //! - \b COMP_D_INPUT6
    //! - \b COMP_D_INPUT7
    //! - \b COMP_D_INPUT8
    //! - \b COMP_D_INPUT9
    //! - \b COMP_D_INPUT10
    //! - \b COMP_D_INPUT11
    //! - \b COMP_D_INPUT12
    //! - \b COMP_D_INPUT13
    //! - \b COMP_D_INPUT14
    //! - \b COMP_D_INPUT15
    //! - \b COMP_D_VREF
    uint8_t positiveTerminalInput;
    //! Selects the input to the negative terminal.
    //! \n Valid values are:
    //! - \b COMP_D_INPUT0 [Default]
    //! - \b COMP_D_INPUT1
    //! - \b COMP_D_INPUT2
    //! - \b COMP_D_INPUT3
    //! - \b COMP_D_INPUT4
    //! - \b COMP_D_INPUT5
    //! - \b COMP_D_INPUT6
    //! - \b COMP_D_INPUT7
    //! - \b COMP_D_INPUT8
    //! - \b COMP_D_INPUT9
    //! - \b COMP_D_INPUT10
    //! - \b COMP_D_INPUT11
    //! - \b COMP_D_INPUT12
    //! - \b COMP_D_INPUT13
    //! - \b COMP_D_INPUT14
    //! - \b COMP_D_INPUT15
    //! - \b COMP_D_VREF
    uint8_t negativeTerminalInput;
    //! Controls the output filter delay state, which is either off or enabled
    //! with a specified delay level. This parameter is device specific and
    //! delay levels should be found in the device's datasheet.
    //! \n Valid values are:
    //! - \b COMP_D_FILTEROUTPUT_OFF [Default]
    //! - \b COMP_D_FILTEROUTPUT_DLYLVL1
    //! - \b COMP_D_FILTEROUTPUT_DLYLVL2
    //! - \b COMP_D_FILTEROUTPUT_DLYLVL3
    //! - \b COMP_D_FILTEROUTPUT_DLYLVL4
    uint8_t outputFilterEnableAndDelayLevel;
    //! Controls if the output will be inverted or not
    //! \n Valid values are:
    //! - \b COMP_D_NORMALOUTPUTPOLARITY [Default]
    //! - \b COMP_D_INVERTEDOUTPUTPOLARITY
    uint16_t invertedOutputPolarity;
} Comp_D_initParam;

//*****************************************************************************
//
// The following are values that can be passed to the
// outputFilterEnableAndDelayLevel parameter for functions: Comp_D_init(); the
// param parameter for functions: Comp_D_init().
//
//*****************************************************************************
#define COMP_D_FILTEROUTPUT_OFF                                            0x00
#define COMP_D_FILTEROUTPUT_DLYLVL1                            (CDF + CDFDLY_0)
#define COMP_D_FILTEROUTPUT_DLYLVL2                            (CDF + CDFDLY_1)
#define COMP_D_FILTEROUTPUT_DLYLVL3                            (CDF + CDFDLY_2)
#define COMP_D_FILTEROUTPUT_DLYLVL4                            (CDF + CDFDLY_3)

//*****************************************************************************
//
// The following are values that can be passed to the positiveTerminalInput
// parameter for functions: Comp_D_init(); the inputPort parameter for
// functions: Comp_D_enableInputBuffer(), and Comp_D_disableInputBuffer(); the
// negativeTerminalInput parameter for functions: Comp_D_init(); the param
// parameter for functions: Comp_D_init(), and Comp_D_init().
//
//*****************************************************************************
#define COMP_D_INPUT0                                               (CDIPSEL_0)
#define COMP_D_INPUT1                                               (CDIPSEL_1)
#define COMP_D_INPUT2                                               (CDIPSEL_2)
#define COMP_D_INPUT3                                               (CDIPSEL_3)
#define COMP_D_INPUT4                                               (CDIPSEL_4)
#define COMP_D_INPUT5                                               (CDIPSEL_5)
#define COMP_D_INPUT6                                               (CDIPSEL_6)
#define COMP_D_INPUT7                                               (CDIPSEL_7)
#define COMP_D_INPUT8                                               (CDIPSEL_8)
#define COMP_D_INPUT9                                               (CDIPSEL_9)
#define COMP_D_INPUT10                                             (CDIPSEL_10)
#define COMP_D_INPUT11                                             (CDIPSEL_11)
#define COMP_D_INPUT12                                             (CDIPSEL_12)
#define COMP_D_INPUT13                                             (CDIPSEL_13)
#define COMP_D_INPUT14                                             (CDIPSEL_14)
#define COMP_D_INPUT15                                             (CDIPSEL_15)
#define COMP_D_VREF                                                      (0x10)

//*****************************************************************************
//
// The following are values that can be passed to the invertedOutputPolarity
// parameter for functions: Comp_D_init(); the param parameter for functions:
// Comp_D_init().
//
//*****************************************************************************
#define COMP_D_NORMALOUTPUTPOLARITY                               (!(CDOUTPOL))
#define COMP_D_INVERTEDOUTPUTPOLARITY                                (CDOUTPOL)

//*****************************************************************************
//
// The following are values that can be passed to the
// supplyVoltageReferenceBase parameter for functions:
// Comp_D_setReferenceVoltage().
//
//*****************************************************************************
#define COMP_D_REFERENCE_AMPLIFIER_DISABLED                          (CDREFL_0)
#define COMP_D_VREFBASE1_5V                                          (CDREFL_1)
#define COMP_D_VREFBASE2_0V                                          (CDREFL_2)
#define COMP_D_VREFBASE2_5V                                          (CDREFL_3)

//*****************************************************************************
//
// The following are values that can be passed to the referenceAccuracy
// parameter for functions: Comp_D_setReferenceAccuracy().
//
//*****************************************************************************
#define COMP_D_ACCURACY_STATIC                                      (!CDREFACC)
#define COMP_D_ACCURACY_CLOCKED                                      (CDREFACC)

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: Comp_D_enableInterrupt(), and Comp_D_disableInterrupt().
//
//*****************************************************************************
#define COMP_D_INTERRUPT                                                 (CDIE)
#define COMP_D_INTERRUPT_INVERTED_POLARITY                              (CDIIE)

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlagMask
// parameter for functions: Comp_D_clearInterrupt(), and
// Comp_D_getInterruptStatus().
//
//*****************************************************************************
#define COMP_D_INTERRUPT_FLAG                                           (CDIFG)
#define COMP_D_INTERRUPT_FLAG_INVERTED_POLARITY                        (CDIIFG)

//*****************************************************************************
//
// The following are values that can be passed to the edgeDirection parameter
// for functions: Comp_D_setInterruptEdgeDirection().
//
//*****************************************************************************
#define COMP_D_FALLINGEDGE                                           (!(CDIES))
#define COMP_D_RISINGEDGE                                               (CDIES)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the Comp_D_outputValue() function.
//
//*****************************************************************************
#define COMP_D_HIGH                                                     (CDOUT)
#define COMP_D_LOW                                                     (!CDOUT)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the Comp_D Module.
//!
//! Upon successful initialization of the Comp_D module, this function will
//! have reset all necessary register bits and set the given options in the
//! registers. To actually use the Comp_D module, the Comp_D_enable() function
//! must be explicitly called before use. If a Reference Voltage is set to a
//! terminal, the Voltage should be set using the setReferenceVoltage()
//! function.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param param is the pointer to struct for initialization.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process
//
//*****************************************************************************
extern bool Comp_D_init(uint16_t baseAddress,
                        Comp_D_initParam *param);

//*****************************************************************************
//
//! \brief Generates a Reference Voltage to the terminal selected during
//! initialization.
//!
//! Use this function to generate a voltage to serve as a reference to the
//! terminal selected at initialization. The voltage is determined by the
//! equation: Vbase * (Numerator / 32). If the upper and lower limit voltage
//! numerators are equal, then a static reference is defined, whereas they are
//! different then a hysteresis effect is generated. Note that the "limit"
//! voltage is the voltage triggers a change in COMP_D value.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param supplyVoltageReferenceBase decides the source and max amount of
//!        Voltage that can be used as a reference.
//!        Valid values are:
//!        - \b COMP_D_REFERENCE_AMPLIFIER_DISABLED
//!        - \b COMP_D_VREFBASE1_5V
//!        - \b COMP_D_VREFBASE2_0V
//!        - \b COMP_D_VREFBASE2_5V
//!        \n Modified bits are \b CDREFL of \b CDCTL2 register.
//! \param lowerLimitSupplyVoltageFractionOf32 is the numerator of the equation
//!        to generate the reference voltage for the lower limit reference
//!        voltage.
//!        \n Modified bits are \b CDREF0 of \b CDCTL2 register.
//! \param upperLimitSupplyVoltageFractionOf32 is the numerator of the equation
//!        to generate the reference voltage for the upper limit reference
//!        voltage.
//!        \n Modified bits are \b CDREF1 of \b CDCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_setReferenceVoltage(uint16_t baseAddress,
                                       uint16_t supplyVoltageReferenceBase,
                                       uint16_t lowerLimitSupplyVoltageFractionOf32,
                                       uint16_t upperLimitSupplyVoltageFractionOf32);

//*****************************************************************************
//
//! \brief Sets the reference accuracy
//!
//! The reference accuracy is set to the desired setting. Clocked is better for
//! low power operations but has a lower accuracy.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param referenceAccuracy is the reference accuracy setting of the
//!        comparator. Clocked is for low power/low accuracy.
//!        Valid values are:
//!        - \b COMP_D_ACCURACY_STATIC
//!        - \b COMP_D_ACCURACY_CLOCKED
//!        \n Modified bits are \b CDREFACC of \b CDCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_setReferenceAccuracy(uint16_t baseAddress,
                                        uint16_t referenceAccuracy);

//*****************************************************************************
//
//! \brief Enables selected Comparator interrupt sources.
//!
//! Enables the indicated Comparator interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor. <b>Does not clear interrupt flags.</b>
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param interruptMask
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_D_INTERRUPT - Output interrupt
//!        - \b COMP_D_INTERRUPT_INVERTED_POLARITY - Output interrupt inverted
//!           polarity
//!
//
//*****************************************************************************
extern void Comp_D_enableInterrupt(uint16_t baseAddress,
                                   uint16_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected Comparator interrupt sources.
//!
//! Disables the indicated Comparator interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt; disabled sources
//! have no effect on the processor.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param interruptMask
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_D_INTERRUPT - Output interrupt
//!        - \b COMP_D_INTERRUPT_INVERTED_POLARITY - Output interrupt inverted
//!           polarity
//!
//
//*****************************************************************************
extern void Comp_D_disableInterrupt(uint16_t baseAddress,
                                    uint16_t interruptMask);

//*****************************************************************************
//
//! \brief Clears Comparator interrupt flags.
//!
//! The Comparator interrupt source is cleared, so that it no longer
//! asserts.The highest interrupt flag is automatically cleared when an
//! interrupt vector generator is used.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param interruptFlagMask
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_D_INTERRUPT_FLAG - Output interrupt flag
//!        - \b COMP_D_INTERRUPT_FLAG_INVERTED_POLARITY - Output interrupt flag
//!           inverted polarity
//!
//
//*****************************************************************************
extern void Comp_D_clearInterrupt(uint16_t baseAddress,
                                  uint16_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Gets the current Comparator interrupt status.
//!
//! This returns the interrupt status for the Comparator module based on which
//! flag is passed.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param interruptFlagMask
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_D_INTERRUPT_FLAG - Output interrupt flag
//!        - \b COMP_D_INTERRUPT_FLAG_INVERTED_POLARITY - Output interrupt flag
//!           inverted polarity
//!
//
//*****************************************************************************
extern uint8_t Comp_D_getInterruptStatus(uint16_t baseAddress,
                                         uint16_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Explicitly sets the edge direction that would trigger an interrupt.
//!
//! This function will set which direction the output will have to go, whether
//! rising or falling, to generate an interrupt based on a non-inverted
//! interrupt.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param edgeDirection determines which direction the edge would have to go
//!        to generate an interrupt based on the non-inverted interrupt flag.
//!        Valid values are:
//!        - \b COMP_D_FALLINGEDGE [Default] - sets the bit to generate an
//!           interrupt when the output of the comparator falls from HIGH to
//!           LOW if the normal interrupt bit is set(and LOW to HIGH if the
//!           inverted interrupt enable bit is set).
//!        - \b COMP_D_RISINGEDGE - sets the bit to generate an interrupt when
//!           the output of the comparator rises from LOW to HIGH if the normal
//!           interrupt bit is set(and HIGH to LOW if the inverted interrupt
//!           enable bit is set).
//!        \n Modified bits are \b CDIES of \b CDCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_setInterruptEdgeDirection(uint16_t baseAddress,
                                             uint16_t edgeDirection);

//*****************************************************************************
//
//! \brief Toggles the edge direction that would trigger an interrupt.
//!
//! This function will toggle which direction the output will have to go,
//! whether rising or falling, to generate an interrupt based on a non-inverted
//! interrupt. If the direction was rising, it is now falling, if it was
//! falling, it is now rising.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_toggleInterruptEdgeDirection(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Turns on the Comparator module.
//!
//! This function sets the bit that enables the operation of the Comparator
//! module.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Turns off the Comparator module.
//!
//! This function clears the CDON bit disabling the operation of the Comparator
//! module, saving from excess power consumption.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Shorts the two input pins chosen during initialization.
//!
//! This function sets the bit that shorts the devices attached to the input
//! pins chosen from the initialization of the comparator.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_shortInputs(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the short of the two input pins chosen during
//! initialization.
//!
//! This function clears the bit that shorts the devices attached to the input
//! pins chosen from the initialization of the comparator.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_unshortInputs(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the input buffer of the selected input port to effectively
//! allow for analog signals.
//!
//! This function sets the bit to disable the buffer for the specified input
//! port to allow for analog signals from any of the comparator input pins.
//! This bit is automatically set when the input is initialized to be used with
//! the comparator module. This function should be used whenever an analog
//! input is connected to one of these pins to prevent parasitic voltage from
//! causing unexpected results.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param inputPort is the port in which the input buffer will be disabled.
//!        Valid values are:
//!        - \b COMP_D_INPUT0 [Default]
//!        - \b COMP_D_INPUT1
//!        - \b COMP_D_INPUT2
//!        - \b COMP_D_INPUT3
//!        - \b COMP_D_INPUT4
//!        - \b COMP_D_INPUT5
//!        - \b COMP_D_INPUT6
//!        - \b COMP_D_INPUT7
//!        - \b COMP_D_INPUT8
//!        - \b COMP_D_INPUT9
//!        - \b COMP_D_INPUT10
//!        - \b COMP_D_INPUT11
//!        - \b COMP_D_INPUT12
//!        - \b COMP_D_INPUT13
//!        - \b COMP_D_INPUT14
//!        - \b COMP_D_INPUT15
//!        - \b COMP_D_VREF
//!        \n Modified bits are \b CDPDx of \b CDCTL3 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_disableInputBuffer(uint16_t baseAddress,
                                      uint8_t inputPort);

//*****************************************************************************
//
//! \brief Enables the input buffer of the selected input port to allow for
//! digital signals.
//!
//! This function clears the bit to enable the buffer for the specified input
//! port to allow for digital signals from any of the comparator input pins.
//! This should not be reset if there is an analog signal connected to the
//! specified input pin to prevent from unexpected results.
//!
//! \param baseAddress is the base address of the COMP_D module.
//! \param inputPort is the port in which the input buffer will be enabled.
//!        Valid values are:
//!        - \b COMP_D_INPUT0 [Default]
//!        - \b COMP_D_INPUT1
//!        - \b COMP_D_INPUT2
//!        - \b COMP_D_INPUT3
//!        - \b COMP_D_INPUT4
//!        - \b COMP_D_INPUT5
//!        - \b COMP_D_INPUT6
//!        - \b COMP_D_INPUT7
//!        - \b COMP_D_INPUT8
//!        - \b COMP_D_INPUT9
//!        - \b COMP_D_INPUT10
//!        - \b COMP_D_INPUT11
//!        - \b COMP_D_INPUT12
//!        - \b COMP_D_INPUT13
//!        - \b COMP_D_INPUT14
//!        - \b COMP_D_INPUT15
//!        - \b COMP_D_VREF
//!        \n Modified bits are \b CDPDx of \b CDCTL3 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_enableInputBuffer(uint16_t baseAddress,
                                     uint8_t inputPort);

//*****************************************************************************
//
//! \brief Toggles the bit that swaps which terminals the inputs go to, while
//! also inverting the output of the comparator.
//!
//! This function toggles the bit that controls which input goes to which
//! terminal. After initialization, this bit is set to 0, after toggling it
//! once the inputs are routed to the opposite terminal and the output is
//! inverted.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_D_swapIO(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the output value of the Comp_D module.
//!
//! Returns the output value of the Comp_D module.
//!
//! \param baseAddress is the base address of the COMP_D module.
//!
//! \return Comp_D_HIGH or Comp_D_LOW as the output value of the Comparator
//!         module.
//!         Return one of the following:
//!         - \b Comp_D_HIGH
//!         - \b Comp_D_LOW
//!         \n indicates the output should be normal
//
//*****************************************************************************
extern uint16_t Comp_D_outputValue(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_COMP_D_H__

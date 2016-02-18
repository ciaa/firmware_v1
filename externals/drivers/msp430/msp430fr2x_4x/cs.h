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
// cs.h - Driver for the CS Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_CS_H__
#define __MSP430WARE_CS_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_CS__

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
// The following are values that can be passed to the clockSourceDivider
// parameter for functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_CLOCK_DIVIDER_1                                              DIVM__1
#define CS_CLOCK_DIVIDER_2                                              DIVM__2
#define CS_CLOCK_DIVIDER_4                                              DIVM__4
#define CS_CLOCK_DIVIDER_8                                              DIVM__8
#define CS_CLOCK_DIVIDER_16                                            DIVM__16
#define CS_CLOCK_DIVIDER_32                                            DIVM__32
#define CS_CLOCK_DIVIDER_64                                            DIVM__64
#define CS_CLOCK_DIVIDER_128                                          DIVM__128
#define CS_CLOCK_DIVIDER_256                                                0x8
#define CS_CLOCK_DIVIDER_512                                                0x9

//*****************************************************************************
//
// The following are values that can be passed to the selectClock parameter for
// functions: CS_enableClockRequest(), and CS_disableClockRequest(); the
// selectedClockSignal parameter for functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_ACLK                                                            0x01
#define CS_MCLK                                                            0x02
#define CS_SMCLK                                                           0x04

//*****************************************************************************
//
// The following are values that can be passed to the selectedClockSignal
// parameter for functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_FLLREF                                                          0x08

//*****************************************************************************
//
// The following are values that can be passed to the selectClock parameter for
// functions: CS_enableClockRequest(), and CS_disableClockRequest().
//
//*****************************************************************************
#define CS_MODOSC                                                   MODCLKREQEN

//*****************************************************************************
//
// The following are values that can be passed to the clockSource parameter for
// functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_XT1CLK_SELECT                                          SELMS__XT1CLK
#define CS_VLOCLK_SELECT                                          SELMS__VLOCLK
#define CS_REFOCLK_SELECT                                        SELMS__REFOCLK
#define CS_DCOCLKDIV_SELECT                                    SELMS__DCOCLKDIV

//*****************************************************************************
//
// The following are values that can be passed to the xt1drive parameter for
// functions: CS_turnOnXT1(), and CS_turnOnXT1WithTimeout().
//
//*****************************************************************************
#define CS_XT1_DRIVE_0                                               XT1DRIVE_0
#define CS_XT1_DRIVE_1                                               XT1DRIVE_1
#define CS_XT1_DRIVE_2                                               XT1DRIVE_2
#define CS_XT1_DRIVE_3                                               XT1DRIVE_3

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: CS_getFaultFlagStatus(), and CS_clearFaultFlag() as well as
// returned by the CS_clearAllOscFlagsWithTimeout() function.
//
//*****************************************************************************
#define CS_XT1OFFG                                                      XT1OFFG
#define CS_DCOFFG                                                        DCOFFG
#define CS_FLLULIFG                                                    FLLULIFG

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Sets the external clock source
//!
//! This function sets the external clock sources XT1 crystal oscillator
//! frequency values. This function must be called if an external crystal XT1
//! is used and the user intends to call CS_getMCLK, CS_getSMCLK or CS_getACLK
//! APIs. If not, it is not necessary to invoke this API.
//!
//! \param XT1CLK_frequency is the XT1 crystal frequencies in Hz
//!
//! \return None
//
//*****************************************************************************
extern void CS_setExternalClockSource(uint32_t XT1CLK_frequency);

//*****************************************************************************
//
//! \brief Initalizes a clock signal
//!
//! This function initializes each of the clock signals. The user must ensure
//! that this function is called for each clock signal. If not, the default
//! state is assumed for the particular clock signal. Refer MSP430Ware
//! documentation for CS module or Device Family User's Guide for details of
//! default clock signal states. Note that the dividers for \b CS_FLLREF are
//! different from the available clock dividers. Some devices do not support
//! dividers setting for \b CS_FLLREF, please refer to device specific
//! datasheet for details.
//!
//! \param selectedClockSignal selected clock signal
//!        Valid values are:
//!        - \b CS_ACLK
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//!        - \b CS_FLLREF
//! \param clockSource is clock source for the selectedClockSignal
//!        Valid values are:
//!        - \b CS_XT1CLK_SELECT
//!        - \b CS_VLOCLK_SELECT
//!        - \b CS_REFOCLK_SELECT
//!        - \b CS_DCOCLKDIV_SELECT
//! \param clockSourceDivider selected the clock divider to calculate
//!        clocksignal from clock source.
//!        Valid values are:
//!        - \b CS_CLOCK_DIVIDER_1 [Default] - [Valid for CS_FLLREF, CS_MCLK,
//!           CS_ACLK, CS_SMCLK]
//!        - \b CS_CLOCK_DIVIDER_2 - [Valid for CS_MCLK, CS_SMCLK]
//!        - \b CS_CLOCK_DIVIDER_4 - [Valid for CS_MCLK, CS_SMCLK]
//!        - \b CS_CLOCK_DIVIDER_8 - [Valid for CS_MCLK, CS_SMCLK]
//!        - \b CS_CLOCK_DIVIDER_16 - [Valid for CS_MCLK]
//!        - \b CS_CLOCK_DIVIDER_32 - [Valid for CS_FLLREF, CS_MCLK]
//!        - \b CS_CLOCK_DIVIDER_64 - [Valid for CS_FLLREF, CS_MCLK]
//!        - \b CS_CLOCK_DIVIDER_128 - [Valid for CS_FLLREF, CS_MCLK]
//!        - \b CS_CLOCK_DIVIDER_256 - [Valid for CS_FLLREF]
//!        - \b CS_CLOCK_DIVIDER_512 - [Valid for CS_FLLREF]
//!
//! Modified bits of \b CSCTL3 register, bits of \b CSCTL5 register and bits of
//! \b CSCTL4 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_initClockSignal(uint8_t selectedClockSignal,
                               uint16_t clockSource,
                               uint16_t clockSourceDivider);

//*****************************************************************************
//
//! \brief Intializes the XT1 crystal oscillator in low frequency mode
//!
//! Initializes the XT1 crystal oscillator in low frequency mode. Loops until
//! all oscillator fault flags are cleared, with no timeout. See the device-
//! specific data sheet for appropriate drive settings.
//!
//! \param xt1drive is the target drive strength for the XT1 crystal
//!        oscillator.
//!        Valid values are:
//!        - \b CS_XT1_DRIVE_0
//!        - \b CS_XT1_DRIVE_1
//!        - \b CS_XT1_DRIVE_2
//!        - \b CS_XT1_DRIVE_3 [Default]
//!        \n Modified bits are \b XT1DRIVE of \b UCSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOnXT1(uint16_t xt1drive);

//*****************************************************************************
//
//! \brief Turn On SMCLK
//!
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOnSMCLK(void);

//*****************************************************************************
//
//! \brief Turn Off SMCLK
//!
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOffSMCLK(void);

//*****************************************************************************
//
//! \brief VLO is turned off when not used
//!
//!
//! \return None
//
//*****************************************************************************
extern void CS_enableVLOAutoOff(void);

//*****************************************************************************
//
//! \brief VLO is always on
//!
//!
//! \return None
//
//*****************************************************************************
extern void CS_disableVLOAutoOff(void);

//*****************************************************************************
//
//! \brief Bypass the XT1 crystal oscillator
//!
//! Bypasses the XT1 crystal oscillator. Loops until all oscillator fault flags
//! are cleared, with no timeout.
//!
//!
//! Modified bits of \b SFRIFG1 register, bits of \b CSCTL7 register and bits
//! of \b CSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_bypassXT1(void);

//*****************************************************************************
//
//! \brief Initializes the XT1 crystal oscillator in low frequency mode with
//! timeout
//!
//! Initializes the XT1 crystal oscillator in low frequency mode with timeout.
//! Loops until all oscillator fault flags are cleared or until a timeout
//! counter is decremented and equals to zero. See the device-specific
//! datasheet for appropriate drive settings.
//!
//! \param xt1drive is the target drive strength for the XT1 crystal
//!        oscillator.
//!        Valid values are:
//!        - \b CS_XT1_DRIVE_0
//!        - \b CS_XT1_DRIVE_1
//!        - \b CS_XT1_DRIVE_2
//!        - \b CS_XT1_DRIVE_3 [Default]
//! \param timeout is the count value that gets decremented every time the loop
//!        that clears oscillator fault flags gets executed.
//!
//! Modified bits of \b SFRIFG1 register, bits of \b CSCTL7 register and bits
//! of \b CSCTL6 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool CS_turnOnXT1WithTimeout(uint16_t xt1drive,
                                    uint16_t timeout);

//*****************************************************************************
//
//! \brief Bypasses the XT1 crystal oscillator with time out
//!
//! Bypasses the XT1 crystal oscillator with time out. Loops until all
//! oscillator fault flags are cleared or until a timeout counter is
//! decremented and equals to zero.
//!
//! \param timeout is the count value that gets decremented every time the loop
//!        that clears oscillator fault flags gets executed.
//!
//! Modified bits of \b SFRIFG1 register, bits of \b CSCTL7 register and bits
//! of \b CSCTL6 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool CS_bypassXT1WithTimeout(uint16_t timeout);

//*****************************************************************************
//
//! \brief Stops the XT1 oscillator using the XT1AUTOOFF bit.
//!
//!
//! Modified bits are \b XT1AUTOOFF of \b CSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOffXT1(void);

//*****************************************************************************
//
//! \brief Initializes the DCO to operate a frequency that is a multiple of the
//! reference frequency into the FLL
//!
//! Initializes the DCO to operate a frequency that is a multiple of the
//! reference frequency into the FLL. Loops until all oscillator fault flags
//! are cleared, with a timeout. If the frequency is greater than 16 MHz, the
//! function sets the MCLK and SMCLK source to the undivided DCO frequency.
//! Otherwise, the function sets the MCLK and SMCLK source to the DCOCLKDIV
//! frequency. This function executes a software delay that is proportional in
//! length to the ratio of the target FLL frequency and the FLL reference.
//!
//! \param fsystem is the target frequency for MCLK in kHz
//! \param ratio is the ratio x/y, where x = fsystem and y = FLL reference
//!        frequency.
//!
//! Modified bits of \b CSCTL1 register, bits of \b CSCTL0 register, bits of \b
//! CSCTL2 register, bits of \b CSCTL4 register, bits of \b CSCTL7 register and
//! bits of \b SFRIFG1 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_initFLLSettle(uint16_t fsystem,
                             uint16_t ratio);

//*****************************************************************************
//
//! \brief Initializes the DCO to operate a frequency that is a multiple of the
//! reference frequency into the FLL
//!
//! Initializes the DCO to operate a frequency that is a multiple of the
//! reference frequency into the FLL. Loops until all oscillator fault flags
//! are cleared, with no timeout. If the frequency is greater than 16 MHz, the
//! function sets the MCLK and SMCLK source to the undivided DCO frequency.
//! Otherwise, the function sets the MCLK and SMCLK source to the DCOCLKDIV
//! frequency.
//!
//! \param fsystem is the target frequency for MCLK in kHz
//! \param ratio is the ratio x/y, where x = fsystem and y = FLL reference
//!        frequency.
//!
//! Modified bits of \b CSCTL1 register, bits of \b CSCTL0 register, bits of \b
//! CSCTL2 register, bits of \b CSCTL4 register, bits of \b CSCTL7 register and
//! bits of \b SFRIFG1 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_initFLL(uint16_t fsystem,
                       uint16_t ratio);

//*****************************************************************************
//
//! \brief Enables conditional module requests
//!
//! \param selectClock selects specific request enables
//!        Valid values are:
//!        - \b CS_ACLK
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//!        - \b CS_MODOSC
//!
//! Modified bits of \b CSCTL8 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_enableClockRequest(uint8_t selectClock);

//*****************************************************************************
//
//! \brief Disables conditional module requests
//!
//! \param selectClock selects specific request disable
//!        Valid values are:
//!        - \b CS_ACLK
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//!        - \b CS_MODOSC
//!
//! Modified bits of \b CSCTL8 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_disableClockRequest(uint8_t selectClock);

//*****************************************************************************
//
//! \brief Gets the current CS fault flag status.
//!
//! \param mask is the masked interrupt flag status to be returned. Mask
//!        parameter can be either any of the following selection.
//!        Valid values are:
//!        - \b CS_XT1OFFG - XT1 oscillator fault flag
//!        - \b CS_DCOFFG - DCO fault flag
//!        - \b CS_FLLULIFG - FLL unlock interrupt flag
//!
//! Modified bits of \b CSCTL7 register.
//!
//! \return The current flag status for the corresponding masked bit
//
//*****************************************************************************
extern uint8_t CS_getFaultFlagStatus(uint8_t mask);

//*****************************************************************************
//
//! \brief Clears the current CS fault flag status for the masked bit.
//!
//! \param mask is the masked interrupt flag status to be returned. mask
//!        parameter can be any one of the following
//!        Valid values are:
//!        - \b CS_XT1OFFG - XT1 oscillator fault flag
//!        - \b CS_DCOFFG - DCO fault flag
//!        - \b CS_FLLULIFG - FLL unlock interrupt flag
//!
//! Modified bits of \b CSCTL7 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_clearFaultFlag(uint8_t mask);

//*****************************************************************************
//
//! \brief Get the current ACLK frequency
//!
//! Get the current ACLK frequency. The user of this API must ensure that
//! CS_setExternalClockSource API was invoked before in case XT1 is being used.
//!
//!
//! \return Current ACLK frequency in Hz
//
//*****************************************************************************
extern uint32_t CS_getACLK(void);

//*****************************************************************************
//
//! \brief Get the current SMCLK frequency
//!
//! Get the current SMCLK frequency. The user of this API must ensure that
//! CS_setExternalClockSource API was invoked before in case XT1 is being used.
//!
//!
//! \return Current SMCLK frequency in Hz
//
//*****************************************************************************
extern uint32_t CS_getSMCLK(void);

//*****************************************************************************
//
//! \brief Get the current MCLK frequency
//!
//! Get the current MCLK frequency. The user of this API must ensure that
//! CS_setExternalClockSource API was invoked before in case XT1 is being used.
//!
//!
//! \return Current MCLK frequency in Hz
//
//*****************************************************************************
extern uint32_t CS_getMCLK(void);

//*****************************************************************************
//
//! \brief Clears all the Oscillator Flags
//!
//! \param timeout is the count value that gets decremented every time the loop
//!        that clears oscillator fault flags gets executed.
//!
//! \return The mask of the oscillator flag status
//!         Return Logical OR of any of the following:
//!         - \b CS_XT1OFFG XT1 oscillator fault flag
//!         - \b CS_DCOFFG DCO fault flag
//!         - \b CS_FLLULIFG FLL unlock interrupt flag
//!         \n indicating the status of the osciallator fault flags
//
//*****************************************************************************
extern uint16_t CS_clearAllOscFlagsWithTimeout(uint16_t timeout);

//*****************************************************************************
//
//! \brief Enables XT1 automatic gain control
//!
//!
//! Modified bits of \b CSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_enableXT1AutomaticGainControl(void);

//*****************************************************************************
//
//! \brief Disables XT1 automatic gain control
//!
//!
//! Modified bits of \b CSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_disableXT1AutomaticGainControl(void);

//*****************************************************************************
//
//! \brief Enables FLL unlock interrupt.
//!
//!
//! Modified bits are \b FLLULIE of \b CSCTL7 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_enableFLLUnlock(void);

//*****************************************************************************
//
//! \brief Disables FLL unlock interrupt.
//!
//!
//! Modified bits are \b FLLULIE of \b CSCTL7 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_disableFLLUnlock(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_CS_H__

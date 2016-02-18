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
#define CS_MODOSC                                                   MODCLKREQEN

//*****************************************************************************
//
// The following are values that can be passed to the clockSource parameter for
// functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_XT1CLK_SELECT                                           SELM__XT1CLK
#define CS_VLOCLK_SELECT                                           SELM__VLOCLK
#define CS_DCOCLK_SELECT                                           SELM__DCOCLK
#define CS_XT2CLK_SELECT                                           SELM__XT2CLK

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
// The following are values that can be passed to the xt2drive parameter for
// functions: CS_turnOnXT2(), and CS_turnOnXT2WithTimeout().
//
//*****************************************************************************
#define CS_XT2_DRIVE_4MHZ_8MHZ                                       XT2DRIVE_0
#define CS_XT2_DRIVE_8MHZ_16MHZ                                      XT2DRIVE_1
#define CS_XT2_DRIVE_16MHZ_24MHZ                                     XT2DRIVE_2
#define CS_XT2_DRIVE_24MHZ_32MHZ                                     XT2DRIVE_3

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: CS_getFaultFlagStatus(), and CS_clearFaultFlag() as well as
// returned by the CS_getFaultFlagStatus() function.
//
//*****************************************************************************
#define CS_XT2OFFG                                                      XT2OFFG
#define CS_XT1OFFG                                                      XT1OFFG

//*****************************************************************************
//
// The following are values that can be passed to the dcorsel parameter for
// functions: CS_setDCOFreq().
//
//*****************************************************************************
#define CS_DCORSEL_0                                                  DCOFSEL_0
#define CS_DCORSEL_1                                                    DCORSEL

//*****************************************************************************
//
// The following are values that can be passed to the dcofsel parameter for
// functions: CS_setDCOFreq().
//
//*****************************************************************************
#define CS_DCOFSEL_0                                                  DCOFSEL_0
#define CS_DCOFSEL_1                                                  DCOFSEL_1
#define CS_DCOFSEL_2                                                  DCOFSEL_2
#define CS_DCOFSEL_3                                                  DCOFSEL_3

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Sets the external clock source
//!
//! This function sets the external clock sources XT1 and XT2 crystal
//! oscillator frequency values. This function must be called if an external
//! crystal XT1 or XT2 is used and the user intends to call CS_getMCLK,
//! CS_getSMCLK, CS_getACLK and turnOnXT1, XT1ByPass, turnOnXT1WithTimeout,
//! XT1ByPassWithTimeout.
//!
//! \param XT1CLK_frequency is the XT1 crystal frequencies in Hz
//! \param XT2CLK_frequency is the XT2 crystal frequencies in Hz
//!
//! \return None
//
//*****************************************************************************
extern void CS_setExternalClockSource(uint32_t XT1CLK_frequency,
                                      uint32_t XT2CLK_frequency);

//*****************************************************************************
//
//! \brief Initializes clock signal
//!
//! This function initializes each of the clock signals. The user must ensure
//! that this function is called for each clock signal. If not, the default
//! state is assumed for the particular clock signal. Refer to MSP430ware
//! documentation for CS module or Device Family User's Guide for details of
//! default clock signal states.
//!
//! \param selectedClockSignal is the selected clock signal
//!        Valid values are:
//!        - \b CS_ACLK
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//!        - \b CS_MODOSC
//! \param clockSource is Clock source for the selectedClock
//!        Valid values are:
//!        - \b CS_XT1CLK_SELECT
//!        - \b CS_VLOCLK_SELECT
//!        - \b CS_DCOCLK_SELECT
//!        - \b CS_XT2CLK_SELECT
//! \param clockSourceDivider selects the clock divider to calculate clock
//!        signal from clock source.
//!        Valid values are:
//!        - \b CS_CLOCK_DIVIDER_1 - [Default for ACLK]
//!        - \b CS_CLOCK_DIVIDER_2
//!        - \b CS_CLOCK_DIVIDER_4
//!        - \b CS_CLOCK_DIVIDER_8 - [Default for SMCLK and MCLK]
//!        - \b CS_CLOCK_DIVIDER_16
//!        - \b CS_CLOCK_DIVIDER_32
//!
//! Modified bits of \b CSCTL0 register, bits of \b CSCTL3 register and bits of
//! \b CSCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_initClockSignal(uint8_t selectedClockSignal,
                               uint16_t clockSource,
                               uint16_t clockSourceDivider);

//*****************************************************************************
//
//! \brief Initializes the XT1 crystal oscillator in low frequency mode.
//!
//! Loops until all oscillator fault flags are cleared, with no timeout. See
//! the device-specific data sheet for appropriate drive settings. IMPORTANT:
//! User must call CS_setExternalClockSource function to set frequency of
//! external clocks before calling this function.
//!
//! \param xt1drive is the target drive strength for the XT1 crystal
//!        oscillator.
//!        Valid values are:
//!        - \b CS_XT1_DRIVE_0
//!        - \b CS_XT1_DRIVE_1
//!        - \b CS_XT1_DRIVE_2
//!        - \b CS_XT1_DRIVE_3 [Default]
//!
//! Modified bits of \b CSCTL0 register, bits of \b CSCTL5 register, bits of \b
//! CSCTL4 register and bits of \b SFRIFG1 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOnXT1(uint16_t xt1drive);

//*****************************************************************************
//
//! \brief Turn on SMCLK
//!
//!
//! Modified bits of \b CSCTL4 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOnSMCLK(void);

//*****************************************************************************
//
//! \brief Turn off SMCLK
//!
//!
//! Modified bits of \b CSCTL4 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOffSMCLK(void);

//*****************************************************************************
//
//! \brief Bypasses the XT1 crystal oscillator.
//!
//! Loops until all oscillator fault flags are cleared, with no timeout.
//! IMPORTANT: User must call CS_setExternalClockSource function to set
//! frequency of external clocks before calling this function.
//!
//!
//! Modified bits of \b CSCTL0 register, bits of \b CSCTL5 register, bits of \b
//! CSCTL4 register and bits of \b SFRIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_bypassXT1(void);

//*****************************************************************************
//
//! \brief Initializes the XT1 crystal oscillator in low frequency mode with
//! timeout.
//!
//! Loops until all oscillator fault flags are cleared or until a timeout
//! counter is decremented and equals to zero. See the device-specific
//! datasheet for appropriate drive settings. IMPORTANT: User must call
//! CS_setExternalClockSource function to set frequency of external clocks
//! before calling this function.
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
//! Modified bits of \b CSCTL0 register, bits of \b CSCTL5 register, bits of \b
//! CSCTL4 register and bits of \b SFRIFG1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool CS_turnOnXT1WithTimeout(uint16_t xt1drive,
                                    uint32_t timeout);

//*****************************************************************************
//
//! \brief Bypasses the XT1 crystal oscillator with timeout.
//!
//! Loops until all oscillator fault flags are cleared or until a timeout
//! counter is decremented and equals to zero.IMPORTANT: User must call
//! CS_setExternalClockSource to set frequency of external clocks before
//! calling this function
//!
//! \param timeout is the count value that gets decremented every time the loop
//!        that clears oscillator fault flags gets executed.
//!
//! Modified bits of \b CSCTL0 register, bits of \b CSCTL5 register, bits of \b
//! CSCTL4 register and bits of \b SFRIFG register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool CS_bypassXT1WithTimeout(uint32_t timeout);

//*****************************************************************************
//
//! \brief Stops the XT1 oscillator using the XT1OFF bit.
//!
//!
//! Modified bits of \b CSCTL4 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOffXT1(void);

//*****************************************************************************
//
//! \brief Starts the XT2 crystal
//!
//! Initializes the XT2 crystal oscillator, which supports crystal frequencies
//! between 4 MHz and 32 MHz, depending on the selected drive strength. Loops
//! until all oscillator fault flags are cleared, with no timeout. See the
//! device-specific data sheet for appropriate drive settings. NOTE: User must
//! call CS_setExternalClockSource to set frequency of external clocks before
//! calling this function.
//!
//! \param xt2drive is the target drive strength for the XT2 crystal
//!        oscillator.
//!        Valid values are:
//!        - \b CS_XT2_DRIVE_4MHZ_8MHZ
//!        - \b CS_XT2_DRIVE_8MHZ_16MHZ
//!        - \b CS_XT2_DRIVE_16MHZ_24MHZ
//!        - \b CS_XT2_DRIVE_24MHZ_32MHZ [Default]
//!
//! Modified bits of \b CSCTL0 register, bits of \b CSCTL5 register, bits of \b
//! CSCTL4 register and bits of \b SFRIFG1 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOnXT2(uint16_t xt2drive);

//*****************************************************************************
//
//! \brief Bypasses the XT2 crystal oscillator
//!
//! Bypasses the XT2 crystal oscillator which supports crystal frequencies
//! between 4 MHz and 32 MHz. Loops until all oscillator fault flags are
//! cleared, with no timeout. NOTE: User must call CS_setExternalClockSource to
//! set frequency of external clocks before calling this function.
//!
//!
//! Modified bits of \b CSCTL5 register, bits of \b CSCTL4 register and bits of
//! \b SFRIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_bypassXT2(void);

//*****************************************************************************
//
//! \brief Initializes the XT2 crystal oscillator with timeout
//!
//! Initializes the XT2 crystal oscillator, which supports crystal frequencies
//! between 4 MHz and 32 MHz, depending on the selected drive strength. Loops
//! until all oscillator fault flags are cleared or until a timeout counter is
//! decremented and equals to zero. See the device-specific data sheet for
//! appropriate drive settings. NOTE: User must call CS_setExternalClockSource
//! to set frequency of external clocks before calling this function.
//!
//! \param xt2drive is the target drive strength for the XT2 crystal
//!        oscillator.
//!        Valid values are:
//!        - \b CS_XT2_DRIVE_4MHZ_8MHZ
//!        - \b CS_XT2_DRIVE_8MHZ_16MHZ
//!        - \b CS_XT2_DRIVE_16MHZ_24MHZ
//!        - \b CS_XT2_DRIVE_24MHZ_32MHZ [Default]
//! \param timeout is the count value that gets decremented every time the loop
//!        that clears oscillator fault flags gets executed.
//!
//! Modified bits of \b CSCTL5 register, bits of \b CSCTL4 register and bits of
//! \b SFRIFG1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool CS_turnOnXT2WithTimeout(uint16_t xt2drive,
                                    uint32_t timeout);

//*****************************************************************************
//
//! \brief Bypasses the XT2 crystal oscillator with timeout
//!
//! Bypasses the XT2 crystal oscillator with timeout, which supports crystal
//! frequencies between 4 MHz and 32 MHz. Loops until all oscillator fault
//! flags are cleared or until a timeout counter is decremented and equals to
//! zero. NOTE: User must call CS_setExternalClockSource to set frequency of
//! external clocks before calling this function.
//!
//! \param timeout is the count value that gets decremented every time the loop
//!        that clears oscillator fault flags gets executed.
//!
//! Modified bits of \b CSCTL5 register, bits of \b CSCTL4 register and bits of
//! \b SFRIFG1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool CS_bypassXT2WithTimeout(uint32_t timeout);

//*****************************************************************************
//
//! \brief Stops the XT2 oscillator using the XT2OFF bit.
//!
//!
//! Modified bits of \b CSCTL4 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_turnOffXT2(void);

//*****************************************************************************
//
//! \brief Enables conditional module requests
//!
//! \param selectClock selects specific request enables.
//!        Valid values are:
//!        - \b CS_ACLK
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//!        - \b CS_MODOSC
//!
//! Modified bits of \b CSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_enableClockRequest(uint8_t selectClock);

//*****************************************************************************
//
//! \brief Disables conditional module requests
//!
//! \param selectClock selects specific request enables.
//!        Valid values are:
//!        - \b CS_ACLK
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//!        - \b CS_MODOSC
//!
//! Modified bits of \b CSCTL6 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_disableClockRequest(uint8_t selectClock);

//*****************************************************************************
//
//! \brief Gets the current CS fault flag status.
//!
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b CS_XT2OFFG - XT2 oscillator fault flag
//!        - \b CS_XT1OFFG - XT2 oscillator fault flag (HF mode)
//!
//! \return Logical OR of any of the following:
//!         - \b CS_XT2OFFG XT2 oscillator fault flag
//!         - \b CS_XT1OFFG XT2 oscillator fault flag (HF mode)
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint8_t CS_getFaultFlagStatus(uint8_t mask);

//*****************************************************************************
//
//! \brief Clears the current CS fault flag status for the masked bit.
//!
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b CS_XT2OFFG - XT2 oscillator fault flag
//!        - \b CS_XT1OFFG - XT2 oscillator fault flag (HF mode)
//!
//! Modified bits of \b CSCTL5 register.
//!
//! \return None
//
//*****************************************************************************
extern void CS_clearFaultFlag(uint8_t mask);

//*****************************************************************************
//
//! \brief Get the current ACLK frequency.
//!
//! If a oscillator fault is set, the frequency returned will be based on the
//! fail safe mechanism of CS module. The user of this API must ensure that
//! CS_externalClockSourceInit API was invoked before in case XT1 or XT2 is
//! being used.
//!
//!
//! \return Current ACLK frequency in Hz
//
//*****************************************************************************
extern uint32_t CS_getACLK(void);

//*****************************************************************************
//
//! \brief Get the current SMCLK frequency.
//!
//! If a oscillator fault is set, the frequency returned will be based on the
//! fail safe mechanism of CS module. The user of this API must ensure that
//! CS_externalClockSourceInit API was invoked before in case XT1 or XT2 is
//! being used.
//!
//!
//! \return Current SMCLK frequency in Hz
//
//*****************************************************************************
extern uint32_t CS_getSMCLK(void);

//*****************************************************************************
//
//! \brief Get the current MCLK frequency.
//!
//! If a oscillator fault is set, the frequency returned will be based on the
//! fail safe mechanism of CS module. The user of this API must ensure that
//! CS_externalClockSourceInit API was invoked before in case XT1 or XT2 is
//! being used.
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
//! Modified bits of \b CSCTL5 register and bits of \b SFRIFG1 register.
//!
//! \return the mask of the oscillator flag status
//
//*****************************************************************************
extern uint16_t CS_clearAllOscFlagsWithTimeout(uint32_t timeout);

//*****************************************************************************
//
//! \brief Set DCO frequency
//!
//! \param dcorsel selects frequency range option. Valid options are:
//!        CS_DCORSEL_0 [Default] CS_DCORSEL_1
//!        Valid values are:
//!        - \b CS_DCORSEL_0
//!        - \b CS_DCORSEL_1
//! \param dcofsel selects valid frequency options based on dco frequency range
//!        selection (dcorsel).
//!        Valid values are:
//!        - \b CS_DCOFSEL_0 - Low frequency option 5.33MHZ. High frequency
//!           option 16MHz.
//!        - \b CS_DCOFSEL_1 - Low frequency option 6.67MHZ. High frequency
//!           option 20MHz.
//!        - \b CS_DCOFSEL_2 - Low frequency option 5.33MHZ. High frequency
//!           option 16MHz.
//!        - \b CS_DCOFSEL_3 [Default] - Low frequency option 8MHZ. High
//!           frequency option 24MHz.
//!
//! \return None
//
//*****************************************************************************
extern void CS_setDCOFreq(uint16_t dcorsel,
                          uint16_t dcofsel);

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

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
// rtc.h - Driver for the RTC Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_RTC_H__
#define __MSP430WARE_RTC_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_RTC__

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
// The following are values that can be passed to the clockSource parameter for
// functions: RTC_start().
//
//*****************************************************************************
#define RTC_CLOCKSOURCE_DISABLED                                      (RTCSS_0)
#define RTC_CLOCKSOURCE_SMCLK                                         (RTCSS_1)
#define RTC_CLOCKSOURCE_XT1CLK                                        (RTCSS_2)
#define RTC_CLOCKSOURCE_VLOCLK                                        (RTCSS_3)

//*****************************************************************************
//
// The following are values that can be passed to the clockPredivider parameter
// for functions: RTC_init().
//
//*****************************************************************************
#define RTC_CLOCKPREDIVIDER_1                                         (RTCPS_0)
#define RTC_CLOCKPREDIVIDER_10                                        (RTCPS_1)
#define RTC_CLOCKPREDIVIDER_100                                       (RTCPS_2)
#define RTC_CLOCKPREDIVIDER_1000                                      (RTCPS_3)
#define RTC_CLOCKPREDIVIDER_16                                        (RTCPS_4)
#define RTC_CLOCKPREDIVIDER_64                                        (RTCPS_5)
#define RTC_CLOCKPREDIVIDER_256                                       (RTCPS_6)
#define RTC_CLOCKPREDIVIDER_1024                                      (RTCPS_7)

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: RTC_enableInterrupt(), and RTC_disableInterrupt().
//
//*****************************************************************************
#define RTC_OVERFLOW_INTERRUPT                                          (RTCIE)

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlagMask
// parameter for functions: RTC_getInterruptStatus(), and RTC_clearInterrupt().
//
//*****************************************************************************
#define RTC_OVERFLOW_INTERRUPT_FLAG                                     (RTCIF)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the RTC.
//!
//! This function initializes the RTC for clock source and clock pre-divider.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param modulo is the modulo value to set to RTC.
//!        \n Modified bits of \b RTCMOD register.
//! \param clockPredivider is the clock pre-divider select for RTC.
//!        Valid values are:
//!        - \b RTC_CLOCKPREDIVIDER_1 [Default]
//!        - \b RTC_CLOCKPREDIVIDER_10
//!        - \b RTC_CLOCKPREDIVIDER_100
//!        - \b RTC_CLOCKPREDIVIDER_1000
//!        - \b RTC_CLOCKPREDIVIDER_16
//!        - \b RTC_CLOCKPREDIVIDER_64
//!        - \b RTC_CLOCKPREDIVIDER_256
//!        - \b RTC_CLOCKPREDIVIDER_1024
//!        \n Modified bits are \b RTCPS of \b RTCCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_init(uint16_t baseAddress,
                     uint16_t modulo,
                     uint16_t clockPredivider);

//*****************************************************************************
//
//! \brief Starts RTC running.
//!
//! This function starts the RTC by setting the clock source field (RTCSS).
//! When started, the RTC counter will begin counting at the rate described by
//! the clock source and pre-divider value. When the RTC counter reaches the
//! value in the modulo register, the RTC hardware sets the RTC's interrupt
//! flag bit (RTCIF). Please note, that the RTC actually compares the RTC
//! counter to the modulo shadow register. Since the RTC_start() function sets
//! the RTCSR (RTC software reset) bit, this forces the RTC to copy the value
//! from the Modulo register into the shadow register.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param clockSource is the clock source select for RTC.
//!        Valid values are:
//!        - \b RTC_CLOCKSOURCE_DISABLED [Default]
//!        - \b RTC_CLOCKSOURCE_SMCLK
//!        - \b RTC_CLOCKSOURCE_XT1CLK
//!        - \b RTC_CLOCKSOURCE_VLOCLK
//!        \n Modified bits are \b RTCSS of \b RTCCTL register.
//!
//! Modified bits are \b RTCSR of \b RTCCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_start(uint16_t baseAddress,
                      uint16_t clockSource);

//*****************************************************************************
//
//! \brief Stops RTC running.
//!
//! This function does software reset for RTC.
//!
//! \param baseAddress is the base address of the RTC module.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_stop(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets the modulo value.
//!
//! This function does software reset for RTC.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param modulo is the modulo value to set to RTC.
//!        \n Modified bits of \b RTCMOD register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_setModulo(uint16_t baseAddress,
                          uint16_t modulo);

//*****************************************************************************
//
//! \brief Enables selected RTC interrupt sources.
//!
//! This function enables the selected RTC interrupt source. Only the sources
//! that are enabled can be reflected to the processor interrupt; disabled
//! sources have no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param interruptMask is a bit mask of the interrupts to enable.
//!        Valid values are:
//!        - \b RTC_OVERFLOW_INTERRUPT - counter overflow interrupt
//!
//! Modified bits are \b RTCIE of \b RTCCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_enableInterrupt(uint16_t baseAddress,
                                uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected RTC interrupt sources.
//!
//! This function disables the selected RTC interrupt source.  Only the sources
//! that are enabled can be reflected to the processor interrupt; disabled
//! sources have no effect on the processor.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param interruptMask is a bit mask of the interrupts to disable.
//!        Valid values are:
//!        - \b RTC_OVERFLOW_INTERRUPT - counter overflow interrupt
//!
//! Modified bits are \b RTCIE of \b RTCCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_disableInterrupt(uint16_t baseAddress,
                                 uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Returns the status of the selected interrupts flags.
//!
//! This function returns the status of the interrupt flag for the selected
//! channel.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to return the
//!        status of.
//!        Valid values are:
//!        - \b RTC_OVERFLOW_INTERRUPT_FLAG - asserts when counter overflows
//!
//! \return A bit mask of the selected interrupt flag's status.
//
//*****************************************************************************
extern uint8_t RTC_getInterruptStatus(uint16_t baseAddress,
                                      uint8_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Clears selected RTC interrupt flags.
//!
//! This function clears the RTC interrupt flag is cleared, so that it no
//! longer asserts.
//!
//! \param baseAddress is the base address of the RTC module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to clear
//!        Valid values are:
//!        - \b RTC_OVERFLOW_INTERRUPT_FLAG - asserts when counter overflows
//!
//! Modified bits are \b RTCIF of \b RTCCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_clearInterrupt(uint16_t baseAddress,
                               int8_t interruptFlagMask);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_RTC_H__

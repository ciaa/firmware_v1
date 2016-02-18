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
// The following are values that can be passed to the mode parameter for
// functions: CS_setupDCO().
//
//*****************************************************************************
#define CS_INTERNAL_RESISTOR                                               0x00
#define CS_EXTERNAL_RESISTOR                                               DCOR
#define CS_BYPASS_MODE                                                   DCOBYP

//*****************************************************************************
//
// The following are values that can be passed to the clockSource parameter for
// functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_MCLK                                                            0x01
#define CS_SMCLK                                                           0x02

//*****************************************************************************
//
// The following are values that can be passed to the clockSourceDivider
// parameter for functions: CS_initClockSignal().
//
//*****************************************************************************
#define CS_CLOCK_DIVIDER_1                                               DIVM_0
#define CS_CLOCK_DIVIDER_2                                               DIVM_1
#define CS_CLOCK_DIVIDER_4                                               DIVM_2
#define CS_CLOCK_DIVIDER_8                                               DIVM_3
#define CS_CLOCK_DIVIDER_16                                              DIVM_4

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: CS_getFaultFlagStatus() as well as returned by the
// CS_getFaultFlagStatus() function.
//
//*****************************************************************************
#define CS_DCO_FAULT_FLAG                                                  DCOF

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Sets up the DCO using the selected mode
//!
//! Sets up the DCO using the selected mode. If the bypass mode is selected
//! than an external digital clock is required on the CLKIN pin to drive all
//! clocks on the device. ACLK frequency is not programmable and is fixed to
//! the bypass clock frequency divided by 512.  For external resistor mode a
//! 20kOhm resistor is recommended at the ROSC pin. External resistor mode
//! offers higer clock accuracy in terms of absolute tolerance and temperature
//! drift compared to the internal resistor mode. Please check your device
//! datasheet for details and ratings for the differnt modes.
//!
//! \param mode Mode to put the DCO into
//!        Valid values are:
//!        - \b CS_INTERNAL_RESISTOR - DCO operation with internal resistor
//!        - \b CS_EXTERNAL_RESISTOR - DCO operation with external resistor
//!        - \b CS_BYPASS_MODE - Bypass mode, provide external clock signal
//!
//! \return None
//
//*****************************************************************************
extern void CS_setupDCO(uint8_t mode);

//*****************************************************************************
//
//! \brief Initializes a clock singal with a divider
//!
//! Sets up a clock signal with a divider. If the DCO is in bypass mode the
//! frequency will be CLKIN / divider. If the DCO is not in bypass mode the
//! frequency will 16.384MHz / divider.
//!
//! \param clockSource Clock signal to initialize
//!        Valid values are:
//!        - \b CS_MCLK
//!        - \b CS_SMCLK
//! \param clockSourceDivider Divider setting for the selected clock signal
//!        Valid values are:
//!        - \b CS_CLOCK_DIVIDER_1
//!        - \b CS_CLOCK_DIVIDER_2
//!        - \b CS_CLOCK_DIVIDER_4
//!        - \b CS_CLOCK_DIVIDER_8
//!        - \b CS_CLOCK_DIVIDER_16
//!
//! \return None
//
//*****************************************************************************
extern void CS_initClockSignal(uint8_t clockSource,
                               uint8_t clockSourceDivider);

//*****************************************************************************
//
//! \brief Get the current ACLK frequency in Hz
//!
//! This API returns the current ACLK frequency in Hz. It does not work when
//! the device is setup in DCO bypass mode. Also, CS_setupDCO() should be
//! called before this API so that the DCO has been calibrated and this
//! calculation is accurate.
//!
//!
//! \return Current ACLK frequency in Hz, 0 when in bypass mode
//
//*****************************************************************************
extern uint32_t CS_getACLK(void);

//*****************************************************************************
//
//! \brief Get the current SMCLK frequency in Hz
//!
//! This API returns the current SMCLK frequency in Hz. It does not work when
//! the device is setup in DCO bypass mode. Also, CS_setupDCO() should be
//! called before this API so that the DCO has been calibrated and this
//! calculation is accurate.
//!
//!
//! \return Current SMCLK frequency in Hz, 0 when in bypass mode
//
//*****************************************************************************
extern uint32_t CS_getSMCLK(void);

//*****************************************************************************
//
//! \brief Get the current MCLK frequency in Hz
//!
//! This API returns the current MCLK frequency in Hz. It does not work when
//! the device is setup in DCO bypass mode. Also, CS_setupDCO() should be
//! called before this API so that the DCO has been calibrated and this
//! calculation is accurate.
//!
//!
//! \return Current MCLK frequency in Hz, 0 when in bypass mode
//
//*****************************************************************************
extern uint32_t CS_getMCLK(void);

//*****************************************************************************
//
//! \brief Get the DCO fault flag status
//!
//! Reads and returns DCO fault flag. The DCO fault flag is set when the DCO is
//! operating in external resistor mode and the DCO detects an abnormality. An
//! abnormality could be if the ROSC pin is left open or shorted to ground, or
//! if the resistance connected at the ROSC pin is far away from the
//! recommended value. If the fault persists the DCO automatically switches to
//! the internal resistor mode as a fail-safe mechanism.
//!
//! \param mask Mask of fault flags to check
//!        Mask value is the logical OR of any of the following:
//!        - \b CS_DCO_FAULT_FLAG
//!
//! \return Logical OR of any of the following:
//!         - \b CS_DCO_FAULT_FLAG
//!         \n indicating if the masked fault flags are set
//
//*****************************************************************************
extern uint8_t CS_getFaultFlagStatus(uint8_t mask);

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

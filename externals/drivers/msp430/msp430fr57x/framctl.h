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
// framctl.h - Driver for the FRAMCTL Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_FRAMCTL_H__
#define __MSP430WARE_FRAMCTL_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_FRAM_FR5XX__

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
// The following are values that can be passed to the interruptMask parameter
// for functions: FRAMCtl_enableInterrupt(), and FRAMCtl_disableInterrupt().
//
//*****************************************************************************
#define FRAMCTL_PUC_ON_UNCORRECTABLE_BIT                               UBDRSTEN
#define FRAMCTL_UNCORRECTABLE_BIT_INTERRUPT                              UBDIEN
#define FRAMCTL_CORRECTABLE_BIT_INTERRUPT                                CBDIEN
#define FRAMCTL_ACCESS_VIOLATION_INTERRUPT                               ACCVIE
#define FRAMCTL_ACCESS_TIME_ERROR_INTERRUPT                             ACCTEIE

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlagMask
// parameter for functions: FRAMCtl_getInterruptStatus() as well as returned by
// the FRAMCtl_getInterruptStatus() function.
//
//*****************************************************************************
#define FRAMCTL_ACCESS_TIME_ERROR_FLAG                                 ACCTEIFG
#define FRAMCTL_UNCORRECTABLE_BIT_FLAG                                   UBDIFG
#define FRAMCTL_CORRECTABLE_BIT_FLAG                                     CBDIFG
#define FRAMCTL_ACCESS_VIOLATION_FLAG                                   ACCVIFG

//*****************************************************************************
//
// The following are values that can be passed to the accessTime parameter for
// functions: FRAMCtl_configureWaitStateControl().
//
//*****************************************************************************
#define FRAMCTL_ACCESS_TIME_CYCLES_0                                  NACCESS_0
#define FRAMCTL_ACCESS_TIME_CYCLES_1                                  NACCESS_1
#define FRAMCTL_ACCESS_TIME_CYCLES_2                                  NACCESS_2
#define FRAMCTL_ACCESS_TIME_CYCLES_3                                  NACCESS_3
#define FRAMCTL_ACCESS_TIME_CYCLES_4                                  NACCESS_4
#define FRAMCTL_ACCESS_TIME_CYCLES_5                                  NACCESS_5
#define FRAMCTL_ACCESS_TIME_CYCLES_6                                  NACCESS_6
#define FRAMCTL_ACCESS_TIME_CYCLES_7                                  NACCESS_7

//*****************************************************************************
//
// The following are values that can be passed to the accessTime parameter for
// functions: FRAMCtl_configureWaitStateControl().
//
//*****************************************************************************
#define FRAMCTL_PRECHARGE_TIME_CYCLES_0                               NPRECHG_0
#define FRAMCTL_PRECHARGE_TIME_CYCLES_1                               NPRECHG_1
#define FRAMCTL_PRECHARGE_TIME_CYCLES_2                               NPRECHG_2
#define FRAMCTL_PRECHARGE_TIME_CYCLES_3                               NPRECHG_3
#define FRAMCTL_PRECHARGE_TIME_CYCLES_4                               NPRECHG_4
#define FRAMCTL_PRECHARGE_TIME_CYCLES_5                               NPRECHG_5
#define FRAMCTL_PRECHARGE_TIME_CYCLES_6                               NPRECHG_6
#define FRAMCTL_PRECHARGE_TIME_CYCLES_7                               NPRECHG_7

//*****************************************************************************
//
// The following are values that can be passed to the manualWaitState parameter
// for functions: FRAMCtl_configureWaitStateControl().
//
//*****************************************************************************
#define FRAMCTL_AUTO_MODE                                                 NAUTO
#define FRAMCTL_MANUAL_MODE                                                0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Write data into the fram memory in byte format.
//!
//! \param dataPtr is the pointer to the data to be written
//! \param framPtr is the pointer into which to write the data
//! \param numberOfBytes is the number of bytes to be written
//!
//! \return None
//
//*****************************************************************************
extern void FRAMCtl_write8(uint8_t *dataPtr,
                           uint8_t *framPtr,
                           uint16_t numberOfBytes);

//*****************************************************************************
//
//! \brief Write data into the fram memory in word format.
//!
//! \param dataPtr is the pointer to the data to be written
//! \param framPtr is the pointer into which to write the data
//! \param numberOfWords
//!
//
//*****************************************************************************
extern void FRAMCtl_write16(uint16_t *dataPtr,
                            uint16_t *framPtr,
                            uint16_t numberOfWords);

//*****************************************************************************
//
//! \brief Write data into the fram memory in long format, pass by reference
//!
//! \param dataPtr is the pointer to the data to be written
//! \param framPtr is the pointer into which to write the data
//! \param count
//!
//
//*****************************************************************************
extern void FRAMCtl_write32(uint32_t *dataPtr,
                            uint32_t *framPtr,
                            uint16_t count);

//*****************************************************************************
//
//! \brief Write data into the fram memory in long format, pass by value
//!
//! \param value is the value to written to FRAMCTL memory
//! \param framPtr is the pointer into which to write the data
//! \param count
//!
//
//*****************************************************************************
extern void FRAMCtl_memoryFill32(uint32_t value,
                                 uint32_t *framPtr,
                                 uint16_t count);

//*****************************************************************************
//
//! \brief Enables selected FRAMCtl interrupt sources.
//!
//! If header file contains legacy definitions: FRAMCtl_PUC_ON_DOUBLE_BIT_ERROR
//! - Enable PUC reset if FRAMCtl detects double bit error is detected.
//! FRAMCtl_DOUBLE_BIT_ERROR_INTERRUPT - Interrupts when a double bit error is
//! detected. FRAMCtl_SINGLE_BIT_ERROR_INTERRUPT - Interrupts when a single bit
//! error is detected.
//!
//! \param interruptMask is the bit mask of the memory buffer interrupt sources
//!        to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b FRAMCTL_PUC_ON_UNCORRECTABLE_BIT - Enable PUC reset if FRAMCtl
//!           uncorrectable bit error detected.
//!        - \b FRAMCTL_UNCORRECTABLE_BIT_INTERRUPT - Interrupts when an
//!           uncorrectable bit error is detected.
//!        - \b FRAMCTL_CORRECTABLE_BIT_INTERRUPT - Interrupts when a
//!           correctable bit error is detected.
//!        - \b FRAMCTL_ACCESS_VIOLATION_INTERRUPT - Interrupts when an access
//!           violation occurs.
//!        - \b FRAMCTL_ACCESS_TIME_ERROR_INTERRUPT - Interrupts when an access
//!           time error occurs.
//!
//! \return None
//
//*****************************************************************************
extern void FRAMCtl_enableInterrupt(uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Returns the status of the selected FRAMCtl interrupt flags.
//!
//! If header file contains legacy definitions: FRAMCtl_SINGLE_BIT_ERROR_FLAG -
//! Interrupt flag is set if a correctable bit error has been detected and
//! corrected in the FRAMCtl memory error detection logic.
//! FRAMCtl_DOUBLE_BIT_ERROR_FLAG .- Interrupt flag is set if an uncorrectable
//! bit error has been detected in the FRAMCtl memory error detection logic.
//!
//! \param interruptFlagMask is a bit mask of the interrupt flags status to be
//!        returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b FRAMCTL_ACCESS_TIME_ERROR_FLAG - Interrupt flag is set if a
//!           wrong setting for NPRECHG and NACCESS is set and FRAMCtl access
//!           time is not hold.
//!        - \b FRAMCTL_UNCORRECTABLE_BIT_FLAG - Interrupt flag is set if an
//!           uncorrectable bit error has been detected in the FRAMCtl memory
//!           error detection logic.
//!        - \b FRAMCTL_CORRECTABLE_BIT_FLAG - Interrupt flag is set if a
//!           correctable bit error has been detected and corrected in the
//!           FRAMCtl memory error detection logic.
//!        - \b FRAMCTL_ACCESS_VIOLATION_FLAG - Interrupt flag is set if an
//!           access violation is triggered.
//!
//! \return The current interrupt flag status for the corresponding mask.
//!         Return Logical OR of any of the following:
//!         - \b FRAMCtl_ACCESS_TIME_ERROR_FLAG Interrupt flag is set if a
//!         wrong setting for NPRECHG and NACCESS is set and FRAMCtl access
//!         time is not hold.
//!         - \b FRAMCtl_UNCORRECTABLE_BIT_FLAG Interrupt flag is set if an
//!         uncorrectable bit error has been detected in the FRAMCtl memory
//!         error detection logic.
//!         - \b FRAMCtl_CORRECTABLE_BIT_FLAG Interrupt flag is set if a
//!         correctable bit error has been detected and corrected in the
//!         FRAMCtl memory error detection logic.
//!         - \b FRAMCtl_ACCESS_VIOLATION_FLAG Interrupt flag is set if an
//!         access violation is triggered.
//!         \n indicating the status of the masked flags
//
//*****************************************************************************
extern uint8_t FRAMCtl_getInterruptStatus(uint16_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Disables selected FRAMCtl interrupt sources.
//!
//! If header file contains legacy definitions: FRAMCtl_PUC_ON_DOUBLE_BIT_ERROR
//! - Enable PUC reset if FRAMCtl detects double bit error is detected.
//! FRAMCtl_DOUBLE_BIT_ERROR_INTERRUPT - Interrupts when a double bit error is
//! detected. FRAMCtl_SINGLE_BIT_ERROR_INTERRUPT - Interrupts when a single bit
//! error is detected.
//!
//! \param interruptMask is the bit mask of the memory buffer interrupt sources
//!        to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b FRAMCTL_PUC_ON_UNCORRECTABLE_BIT - Enable PUC reset if FRAMCtl
//!           uncorrectable bit error detected.
//!        - \b FRAMCTL_UNCORRECTABLE_BIT_INTERRUPT - Interrupts when an
//!           uncorrectable bit error is detected.
//!        - \b FRAMCTL_CORRECTABLE_BIT_INTERRUPT - Interrupts when a
//!           correctable bit error is detected.
//!        - \b FRAMCTL_ACCESS_VIOLATION_INTERRUPT - Interrupts when an access
//!           violation occurs.
//!        - \b FRAMCTL_ACCESS_TIME_ERROR_INTERRUPT - Interrupts when an access
//!           time error occurs.
//!
//! \return None
//
//*****************************************************************************
extern void FRAMCtl_disableInterrupt(uint16_t interruptMask);

//*****************************************************************************
//
//! \brief Configures the wait state control of the FRAMCtl module
//! Configures the wait state control of the FRAM module. If using the
//! FRAMCtl_AUTO_MODE the values for accessTime and prechargeTime do not
//! matter.
//!
//! \param manualWaitState chooses if the wait state control is manual or
//!        automatic
//!        Valid values are:
//!        - \b FRAMCTL_AUTO_MODE
//!        - \b FRAMCTL_MANUAL_MODE
//! \param accessTime
//!        Valid values are:
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_0
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_1
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_2
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_3
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_4
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_5
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_6
//!        - \b FRAMCTL_ACCESS_TIME_CYCLES_7
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_0
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_1
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_2
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_3
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_4
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_5
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_6
//!        - \b FRAMCTL_PRECHARGE_TIME_CYCLES_7
//! \param prechargeTime
//!
//
//*****************************************************************************
extern void FRAMCtl_configureWaitStateControl(uint8_t manualWaitState,
                                              uint8_t accessTime,
                                              uint8_t prechargeTime);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_FRAMCTL_H__

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
// pmm.h - Driver for the PMM Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_PMM_H__
#define __MSP430WARE_PMM_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PMM_FRAM__

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
// The following are values that can be passed to the mask parameter for
// functions: PMM_clearInterrupt(), and PMM_getInterruptStatus() as well as
// returned by the PMM_getInterruptStatus() function.
//
//*****************************************************************************
#define PMM_BOR_INTERRUPT                                             PMMBORIFG
#define PMM_RST_INTERRUPT                                             PMMRSTIFG
#define PMM_POR_INTERRUPT                                             PMMPORIFG
#define PMM_SVSH_INTERRUPT                                              SVSHIFG
#define PMM_LPM5_INTERRUPT                                           PMMLPM5IFG
#define PMM_ALL                                                          (0xA7)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the PMM_getBandgapMode() function.
//
//*****************************************************************************
#define PMM_STATICMODE                                                    (0x0)
#define PMM_SAMPLEMODE                                                   BGMODE

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the PMM_isBandgapActive() function.
//
//*****************************************************************************
#define PMM_REFBG_INACTIVE                                                (0x0)
#define PMM_REFBG_ACTIVE                                               REFBGACT

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the PMM_isRefGenActive() function.
//
//*****************************************************************************
#define PMM_REFGEN_INACTIVE                                               (0x0)
#define PMM_REFGEN_ACTIVE                                             REFGENACT

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the PMM_getBufferedBandgapVoltageStatus() function.
//
//*****************************************************************************
#define PMM_REFBG_NOTREADY                                                (0x0)
#define PMM_REFBG_READY                                                REFBGRDY

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the PMM_getVariableReferenceVoltageStatus() function.
//
//*****************************************************************************
#define PMM_REFGEN_NOTREADY                                               (0x0)
#define PMM_REFGEN_READY                                              REFGENRDY

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Enables the low power reset. SVSH does not reset device, but
//! triggers a system NMI
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableLowPowerReset(void);

//*****************************************************************************
//
//! \brief Disables the low power reset. SVSH resets device.
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableLowPowerReset(void);

//*****************************************************************************
//
//! \brief Enables the high-side SVS circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSVSH(void);

//*****************************************************************************
//
//! \brief Disables the high-side SVS circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSVSH(void);

//*****************************************************************************
//
//! \brief Makes the low-dropout voltage regulator (LDO) remain ON when going
//! into LPM 3/4.
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_turnOnRegulator(void);

//*****************************************************************************
//
//! \brief Turns OFF the low-dropout voltage regulator (LDO) when going into
//! LPM3/4, thus the system will enter LPM3.5 or LPM4.5 respectively
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_turnOffRegulator(void);

//*****************************************************************************
//
//! \brief Calling this function will trigger a software Power On Reset (POR).
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_trigPOR(void);

//*****************************************************************************
//
//! \brief Calling this function will trigger a software Brown Out Rest (BOR).
//!
//!
//! Modified bits of \b PMMCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_trigBOR(void);

//*****************************************************************************
//
//! \brief Clears interrupt flags for the PMM
//!
//! \param mask is the mask for specifying the required flag
//!        Mask value is the logical OR of any of the following:
//!        - \b PMM_BOR_INTERRUPT - Software BOR interrupt
//!        - \b PMM_RST_INTERRUPT - RESET pin interrupt
//!        - \b PMM_POR_INTERRUPT - Software POR interrupt
//!        - \b PMM_SVSH_INTERRUPT - SVS high side interrupt
//!        - \b PMM_LPM5_INTERRUPT - LPM5 indication
//!        - \b PMM_ALL - All interrupts
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_clearInterrupt(uint16_t mask);

//*****************************************************************************
//
//! \brief Returns interrupt status
//!
//! \param mask is the mask for specifying the required flag
//!        Mask value is the logical OR of any of the following:
//!        - \b PMM_BOR_INTERRUPT - Software BOR interrupt
//!        - \b PMM_RST_INTERRUPT - RESET pin interrupt
//!        - \b PMM_POR_INTERRUPT - Software POR interrupt
//!        - \b PMM_SVSH_INTERRUPT - SVS high side interrupt
//!        - \b PMM_LPM5_INTERRUPT - LPM5 indication
//!        - \b PMM_ALL - All interrupts
//!
//! \return Logical OR of any of the following:
//!         - \b PMM_BOR_INTERRUPT Software BOR interrupt
//!         - \b PMM_RST_INTERRUPT RESET pin interrupt
//!         - \b PMM_POR_INTERRUPT Software POR interrupt
//!         - \b PMM_SVSH_INTERRUPT SVS high side interrupt
//!         - \b PMM_LPM5_INTERRUPT LPM5 indication
//!         - \b PMM_ALL All interrupts
//!         \n indicating  the status of the selected  interrupt flags
//
//*****************************************************************************
extern uint16_t PMM_getInterruptStatus(uint16_t mask);

//*****************************************************************************
//
//! \brief Unlock LPM5
//!
//! LPMx.5 configuration is not locked and defaults to its reset condition.
//! Disable the GPIO power-on default high-impedance mode to activate
//! previously configured port settings.
//!
//!
//! \return None
//
//*****************************************************************************
extern void PMM_unlockLPM5(void);

//*****************************************************************************
//
//! \brief Returns the bandgap mode of the PMM module.
//!
//! This function is used to return the bandgap mode of the PMM module,
//! requested by the peripherals using the bandgap. If a peripheral requests
//! static mode, then the bandgap mode will be static for all modules, whereas
//! if all of the peripherals using the bandgap request sample mode, then that
//! will be the mode returned. Sample mode allows the bandgap to be active only
//! when necessary to save on power consumption, static mode requires the
//! bandgap to be active until no peripherals are using it anymore.
//!
//!
//! \return The bandgap mode of the PMM module:
//!         Return Logical OR of any of the following:
//!         - \b PMM_STATICMODE if the bandgap is operating in static mode
//!         - \b PMM_SAMPLEMODE if the bandgap is operating in sample mode
//
//*****************************************************************************
extern uint16_t PMM_getBandgapMode(void);

//*****************************************************************************
//
//! \brief Returns the active status of the bandgap in the PMM module.
//!
//! This function is used to return the active status of the bandgap in the PMM
//! module. If the bandgap is in use by a peripheral, then the status will be
//! seen as active.
//!
//!
//! \return The bandgap active status of the PMM module:
//!         Return Logical OR of any of the following:
//!         - \b PMM_REFBG_INACTIVE if the bandgap is not being used at the
//!         time of query
//!         - \b PMM_REFBG_ACTIVE if the bandgap is being used at the time of
//!         query
//
//*****************************************************************************
extern uint16_t PMM_isBandgapActive(void);

//*****************************************************************************
//
//! \brief Returns the active status of the reference generator in the PMM
//! module.
//!
//! This function is used to return the active status of the reference
//! generator in the PMM module. If the reference generator is on and ready to
//! use, then the status will be seen as active.
//!
//!
//! \return The reference generator active status of the PMM module:
//!         Return Logical OR of any of the following:
//!         - \b PMM_REFGEN_INACTIVE if the reference generator is off and not
//!         operating
//!         - \b PMM_REFGEN_ACTIVE if the reference generator is on and ready
//!         to be used
//
//*****************************************************************************
extern uint16_t PMM_isRefGenActive(void);

//*****************************************************************************
//
//! \brief Returns the active status of the reference generator in the PMM
//! module.
//!
//! This function is used to return the ready status of the buffered bandgap
//! voltage in the PMM module. If the buffered bandgap voltage is ready to use,
//! the ready status will be returned.
//!
//!
//! \return The buffered bandgap voltage ready status of the PMM module:
//!         Return Logical OR of any of the following:
//!         - \b PMM_REFBG_NOTREADY if buffered bandgap voltage is NOT ready to
//!         be used
//!         - \b PMM_REFBG_READY if buffered bandgap voltage ready to be used
//
//*****************************************************************************
extern uint16_t PMM_getBufferedBandgapVoltageStatus(void);

//*****************************************************************************
//
//! \brief Returns the busy status of the variable reference voltage in the PMM
//! module.
//!
//! This function is used to return the ready status of the variable reference
//! voltage in the REFPMM module. If the reference generator is on and ready to
//! use, then the ready status will be returned.
//!
//!
//! \return The variable reference voltage active status of the PMM module:
//!         Return Logical OR of any of the following:
//!         - \b PMM_REFGEN_NOTREADY if variable reference voltage is NOT ready
//!         to be used
//!         - \b PMM_REFGEN_READY if variable reference voltage ready to be
//!         used
//
//*****************************************************************************
extern uint16_t PMM_getVariableReferenceVoltageStatus(void);

//*****************************************************************************
//
//! \brief Disables the internal temperature sensor to save power consumption.
//!
//! This function is used to turn off the internal temperature sensor to save
//! on power consumption. The temperature sensor is disabled by default.
//!
//!
//! Modified bits are \b TSENSOREN of \b PMMCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableTempSensor(void);

//*****************************************************************************
//
//! \brief Enables the internal temperature sensor.
//!
//! This function is used to turn on the internal temperature sensor to use by
//! other peripherals. The temperature sensor is disabled by default.
//!
//!
//! Modified bits are \b TSENSOREN of \b PMMCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableTempSensor(void);

//*****************************************************************************
//
//! \brief Disables the external reference output.
//!
//! This function is used to disable the external reference output. The
//! external reference is connected to a given external ADC channel. The
//! external reference is disabled by default.
//!
//!
//! Modified bits are \b EXTREFEN of \b PMMCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableExternalReference(void);

//*****************************************************************************
//
//! \brief Enables the external reference output.
//!
//! This function is used to enable the external reference output. The external
//! reference is connected to a given external ADC channel. The external
//! reference is disabled by default.
//!
//!
//! Modified bits are \b EXTREFEN of \b PMMCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableExternalReference(void);

//*****************************************************************************
//
//! \brief Disables the internal reference output.
//!
//! This function is used to disable the internal reference output. The
//! internal reference is internally connected to the ADC channel. The internal
//! reference is disabled by default.
//!
//!
//! Modified bits are \b INTREFEN of \b PMMCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableInternalReference(void);

//*****************************************************************************
//
//! \brief Enables the internal reference output.
//!
//! This function is used to enable the internal reference output. The internal
//! reference is internally connected to the ADC channel. The internal
//! reference is disabled by default.
//!
//!
//! Modified bits are \b INTREFEN of \b PMMCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableInternalReference(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_PMM_H__

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
// pmm.c - Driver for the pmm Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup pmm_api pmm
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PMM__
#include "pmm.h"

#include <assert.h>

void PMM_setupVoltageMonitor(uint8_t voltageMonitorLevel) {
    uint8_t currentStatus = VMONCTL;
    currentStatus &= ~(0x07);
    currentStatus |= voltageMonitorLevel;
    VMONCTL = currentStatus;
}

void PMM_calibrateReference(void) {
    REFCAL0 = HWREG8(TLV_START + TLV_CAL_REFCAL0);
    REFCAL1 = HWREG8(TLV_START + TLV_CAL_REFCAL1);
}

void PMM_setRegulatorStatus(uint8_t status) {
    uint8_t currentStatus = LPM45CTL;
    currentStatus &= ~(PMMREGOFF);
    currentStatus |= status;
    LPM45CTL = currentStatus;
}

void PMM_unlockIOConfiguration(void) {
    LPM45CTL &= ~(LOCKLPM45);
}

void PMM_enableInterrupt(uint8_t mask) {
    VMONCTL |= mask;
}

void PMM_disableInterrupt(uint8_t mask) {
    VMONCTL &= ~(mask);
}

uint8_t PMM_getInterruptStatus(uint8_t mask) {
    uint8_t result = 0x00;

    if((mask & PMM_VMON_INTERRUPT) && (VMONCTL & VMONIFG))
    {
        result |= PMM_VMON_INTERRUPT;
    }

    if(mask & PMM_LPM45_INTERRUPT)
    {
        result |= (LPM45CTL & LPM45IFG);
    }

    return(result);
}

void PMM_clearInterrupt(uint8_t mask) {
    LPM45CTL &= ~(mask);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for pmm_api
//! @}
//
//*****************************************************************************

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
// cs.c - Driver for the cs Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup cs_api cs
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_CS__
#include "cs.h"

#include <assert.h>

//*****************************************************************************
//
// DCO Frequency used for clock signal frequency calculations
//
//*****************************************************************************
#define CS_DCO_FREQ                                                    16384000

void CS_setupDCO(uint8_t mode) {
    CSCTL0 &= ~(DCOR | DCOBYP);
    CSCTL0 |= mode;

    // Set DCO calibration using TLV
    if(mode == CS_INTERNAL_RESISTOR)
    {
        CSIRFCAL = HWREG8(TLV_START + TLV_CAL_CSIRFCAL);
        CSIRTCAL = HWREG8(TLV_START + TLV_CAL_CSIRTCAL);
    }
    else if(mode == CS_EXTERNAL_RESISTOR)
    {
        CSERFCAL = HWREG8(TLV_START + TLV_CAL_CSERFCAL);
        CSERTCAL = HWREG8(TLV_START + TLV_CAL_CSERTCAL);
    }
}

void CS_initClockSignal(uint8_t clockSource,
                        uint8_t clockSourceDivider) {
    uint8_t clearBits = DIVM_7;

    if(clockSource == CS_SMCLK)
    {
        clockSourceDivider = clockSourceDivider << 4;
        clearBits = clearBits << 4;
    }

    CSCTL1 &= ~(clearBits);
    CSCTL1 |= clockSourceDivider;
}

uint32_t CS_getACLK(void) {
    return(32000);
}

uint32_t CS_getSMCLK(void) {
    uint8_t divider = (CSCTL1 & 0x70) >> 4;
    return(CS_DCO_FREQ >> divider);
}

uint32_t CS_getMCLK(void) {
    uint8_t divider = (CSCTL1 & 0x07);
    return(CS_DCO_FREQ >> divider);
}

uint8_t CS_getFaultFlagStatus(uint8_t mask) {
    return (CSCTL0 & mask);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for cs_api
//! @}
//
//*****************************************************************************

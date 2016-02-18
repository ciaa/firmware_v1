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
// comp_d.c - Driver for the comp_d Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup comp_d_api comp_d
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_COMPD__
#include "comp_d.h"

#include <assert.h>

bool Comp_D_init(uint16_t baseAddress,
                 Comp_D_initParam *param)
{
    bool retVal = STATUS_SUCCESS;

    //Reset COMPD Control 1 & Interrupt Registers for initialization (OFS_CDCTL3
    //is not reset because it controls the input buffers of the analog signals
    //and may cause parasitic effects if an analog signal is still attached and
    //the buffer is re-enabled
    HWREG16(baseAddress + OFS_CDCTL0) &= 0x0000;
    HWREG16(baseAddress + OFS_CDINT) &= 0x0000;

    //Clear reference voltage and reference source
    HWREG16(baseAddress + OFS_CDCTL2) &= ~(CDRS_3 | CDREFL_3);

    //Set the Positive Terminal
    if(COMP_D_VREF != param->positiveTerminalInput)
    {
        //Enable Positive Terminal Input Mux and Set it to the appropriate input
        HWREG16(baseAddress +
                OFS_CDCTL0) |= CDIPEN + param->positiveTerminalInput;

        //Disable the input buffer
        HWREG16(baseAddress +
                OFS_CDCTL3) |= (1 << param->positiveTerminalInput);
    }
    else
    {
        //Reset and Set COMPD Control 2 Register
        HWREG16(baseAddress + OFS_CDCTL2) &= ~(CDRSEL); //Set Vref to go to (+)terminal
    }

    //Set the Negative Terminal
    if(COMP_D_VREF != param->negativeTerminalInput)
    {
        //Enable Negative Terminal Input Mux and Set it to the appropriate input
        HWREG16(baseAddress +
                OFS_CDCTL0) |= CDIMEN + (param->negativeTerminalInput << 8);

        //Disable the input buffer
        HWREG16(baseAddress +
                OFS_CDCTL3) |= (1 << param->negativeTerminalInput);
    }
    else
    {
        //Reset and Set COMPD Control 2 Register
        HWREG16(baseAddress + OFS_CDCTL2) |= CDRSEL; //Set Vref to go to (-) terminal
    }

    //Reset and Set COMPD Control 1 Register
    HWREG16(baseAddress + OFS_CDCTL1) =
        +param->outputFilterEnableAndDelayLevel  //Set the filter enable bit and delay
        + param->invertedOutputPolarity; //Set the polarity of the output

    return (retVal);
}

void Comp_D_setReferenceVoltage(uint16_t baseAddress,
                                uint16_t supplyVoltageReferenceBase,
                                uint16_t lowerLimitSupplyVoltageFractionOf32,
                                uint16_t upperLimitSupplyVoltageFractionOf32)
{
    HWREG16(baseAddress + OFS_CDCTL1) &= ~(CDMRVS); //Set to VREF0

    //Reset COMPD Control 2 Bits (Except for CDRSEL which is set in Comp_Init() )
    HWREG16(baseAddress + OFS_CDCTL2) &= CDRSEL;

    //Set Voltage Source (Vcc | Vref, resistor ladder or not)
    if(COMP_D_REFERENCE_AMPLIFIER_DISABLED == supplyVoltageReferenceBase)
    {
        HWREG16(baseAddress + OFS_CDCTL2) |= CDRS_1; //Vcc with resistor ladder
    }
    else if(lowerLimitSupplyVoltageFractionOf32 == 32)
    {
        //If the lower limit is 32, then the upper limit has to be 32 due to the
        //assertion that upper must be >= to the lower limit. If the numerator is
        //equal to 32, then the equation would be 32/32 == 1, therefore no resistor
        //ladder is needed
        HWREG16(baseAddress + OFS_CDCTL2) |= CDRS_3; //Vref, no resistor ladder
    }
    else
    {
        HWREG16(baseAddress + OFS_CDCTL2) |= CDRS_2; //Vref with resistor ladder
    }

    //Set COMPD Control 2 Register
    HWREG16(baseAddress + OFS_CDCTL2) |=
        supplyVoltageReferenceBase //Set Supply Voltage Base
        + ((lowerLimitSupplyVoltageFractionOf32 - 1) << 8) //Set Supply Voltage Num.
        + (upperLimitSupplyVoltageFractionOf32 - 1);
}

void Comp_D_setReferenceAccuracy(uint16_t baseAddress,
                                 uint16_t referenceAccuracy)
{
    HWREG16(baseAddress + OFS_CDCTL2) &= ~(CDREFACC);
    HWREG16(baseAddress + OFS_CDCTL2) |= referenceAccuracy;
}

void Comp_D_enableInterrupt(uint16_t baseAddress,
                            uint16_t interruptMask)
{
    //Set the Interrupt enable bit
    HWREG16(baseAddress + OFS_CDINT) |= interruptMask;
}

void Comp_D_disableInterrupt(uint16_t baseAddress,
                             uint16_t interruptMask)
{
    HWREG16(baseAddress + OFS_CDINT) &= ~(interruptMask);
}

void Comp_D_clearInterrupt(uint16_t baseAddress,
                           uint16_t interruptFlagMask)
{
    HWREG16(baseAddress + OFS_CDINT) &= ~(interruptFlagMask);
}

uint8_t Comp_D_getInterruptStatus(uint16_t baseAddress,
                                  uint16_t interruptFlagMask)
{
    return (HWREG16(baseAddress + OFS_CDINT) & interruptFlagMask);
}

void Comp_D_setInterruptEdgeDirection(uint16_t baseAddress,
                                      uint16_t edgeDirection)
{
    //Set the edge direction that will trigger an interrupt
    if(COMP_D_RISINGEDGE == edgeDirection)
    {
        HWREG16(baseAddress + OFS_CDCTL1) |= CDIES;
    }
    else if(COMP_D_FALLINGEDGE == edgeDirection)
    {
        HWREG16(baseAddress + OFS_CDCTL1) &= ~(CDIES);
    }
}

void Comp_D_toggleInterruptEdgeDirection(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CDCTL1) ^= CDIES;
}

void Comp_D_enable(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CDCTL1) |= CDON;
}

void Comp_D_disable(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CDCTL1) &= ~(CDON);
}

void Comp_D_shortInputs(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CDCTL1) |= CDSHORT;
}

void Comp_D_unshortInputs(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CDCTL1) &= ~(CDSHORT);
}

void Comp_D_disableInputBuffer(uint16_t baseAddress,
                               uint8_t inputPort)
{
    HWREG16(baseAddress + OFS_CDCTL3) |= (1 << inputPort);
}

void Comp_D_enableInputBuffer(uint16_t baseAddress,
                              uint8_t inputPort)
{
    HWREG16(baseAddress + OFS_CDCTL3) &= ~(1 << inputPort);
}

void Comp_D_swapIO(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CDCTL1) ^= CDEX; //Toggle CDEX bit
}

uint16_t Comp_D_outputValue(uint16_t baseAddress)
{
    return (HWREG16(baseAddress + OFS_CDCTL1) & CDOUT);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for comp_d_api
//! @}
//
//*****************************************************************************

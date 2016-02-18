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
// sd24.c - Driver for the sd24 Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup sd24_api sd24
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_SD24__
#include "sd24.h"

#include <assert.h>

void SD24_init(uint16_t baseAddress,
               uint8_t referenceSelect)
{
    uint16_t address;
    uint8_t converter;

    // Reset all interrupts and flags, and turn off all groups
    for(converter = 0; converter < 4; converter++)
    {
        address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));
        HWREG16(address) &= ~(SD24SC + SD24IFG + SD24GRP);
    }

    // Turn off overflow interrupts, and configure SD24 reference
    HWREG16(baseAddress + OFS_SD24CTL) = referenceSelect;
    return;
}

void SD24_initConverter(uint16_t baseAddress,
                        uint16_t converter,
                        uint16_t conversionMode)
{
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(SD24SNGL);

    HWREG16(address) |= conversionMode;
}

void SD24_initConverterAdvanced(uint16_t baseAddress,
                                SD24_initConverterAdvancedParam *param)
{
    // Getting correct SD24CCTLx register
    uint16_t address = baseAddress +
                       (OFS_SD24CCTL0 + (param->converter * 0x02));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(SD24SNGL | SD24DF | SD24GRP | SD24OSR_32);

    HWREG16(address) |=
        (param->groupEnable | param->conversionMode | param->dataFormat
     | param->oversampleRatio);

    // Getting correct SD24INTCTLx register
    address = baseAddress + (OFS_SD24INCTL0 + (param->converter));

    // Clearing previous settings for configuration
    HWREG8(address) &= ~(SD24_GAIN_8 | SD24_GAIN_16 | SD24INTDLY | SD24INCH_7);

    HWREG8(address) |=
        (param->gain | param->interruptDelay | param->inputChannel);
}

void SD24_setConverterDataFormat(uint16_t baseAddress,
                                 uint16_t converter,
                                 uint16_t dataFormat)
{
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(SD24DF);

    HWREG16(address) |= dataFormat;
}

void SD24_startConverterConversion(uint16_t baseAddress,
                                   uint8_t converter)
{
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    assert(
        (SD24_CONVERTER_0 == converter) ||
        (SD24_CONVERTER_1 == converter) ||
        (SD24_CONVERTER_2 == converter) ||
        (SD24_CONVERTER_3 == converter)
        );

    // Setting SD24SC bit to start conversion
    HWREG16(address) |= SD24SC;
}

void SD24_stopConverterConversion(uint16_t baseAddress,
                                  uint8_t converter)
{
    assert(
        (SD24_CONVERTER_0 == converter) ||
        (SD24_CONVERTER_1 == converter) ||
        (SD24_CONVERTER_2 == converter) ||
        (SD24_CONVERTER_3 == converter)
        );

    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    // Setting SD24SC bit to start conversion
    HWREG16(address) &= ~(SD24SC);
}

void SD24_setInputChannel(uint16_t baseAddress,
                          uint8_t converter,
                          uint8_t inputChannel)
{
    uint16_t address = baseAddress + (OFS_SD24INCTL0 + (converter));

    // Clear previous settings
    HWREG8(address) &= ~(SD24INCH_7);

    HWREG8(address) |= inputChannel;
}

void SD24_setInterruptDelay(uint16_t baseAddress,
                            uint8_t converter,
                            uint8_t interruptDelay)
{
    uint16_t address = baseAddress + (OFS_SD24INCTL0 + (converter));

    assert(
        (SD24_CONVERTER_0 == converter) ||
        (SD24_CONVERTER_1 == converter) ||
        (SD24_CONVERTER_2 == converter) ||
        (SD24_CONVERTER_3 == converter)
        );

    // Clear previous settings
    HWREG16(address) &= ~(SD24INTDLY);

    HWREG16(address) |= interruptDelay;
}

void SD24_setOversampling(uint16_t baseAddress,
                          uint8_t converter,
                          uint16_t oversampleRatio)
{
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    assert(
        (SD24_CONVERTER_0 == converter) ||
        (SD24_CONVERTER_1 == converter) ||
        (SD24_CONVERTER_2 == converter) ||
        (SD24_CONVERTER_3 == converter)
        );

    // Clear previous settings
    HWREG16(address) &= ~(SD24OSR0 | SD24OSR1);

    HWREG16(address) |= oversampleRatio;
}

void SD24_setGain(uint16_t baseAddress,
                  uint8_t converter,
                  uint8_t gain)
{
    uint16_t address = baseAddress + (OFS_SD24INCTL0 + (converter));

    // Clear previous settings
    HWREG8(address) &= ~(SD24GAIN0 | SD24GAIN1 | SD24GAIN2);

    HWREG8(address) |= gain;
}

uint32_t SD24_getResults(uint16_t baseAddress,
                         uint8_t converter)
{
    volatile uint16_t OSR;

    // Calculate address of MEM results
    uint16_t address = baseAddress + (OFS_SD24MEM0 + (converter * 0x02));

    // Get high word result
    HWREG16(baseAddress +
            (OFS_SD24CCTL0 + (converter * 0x02))) &= ~(SD24LSBACC);
    uint32_t highResult = (uint32_t)HWREG16(address);

    // Get low word result
    HWREG16(baseAddress + (OFS_SD24CCTL0 + (converter * 0x02))) |= SD24LSBACC;
    uint16_t lowResult = HWREG16(address);
    HWREG16(baseAddress +
            (OFS_SD24CCTL0 + (converter * 0x02))) &= ~(SD24LSBACC);

    // Determine the OSR and combine the high and low result words as appropriate
    OSR =
        HWREG16(baseAddress +
                (OFS_SD24CCTL0 + (converter * 0x02))) & (SD24OSR0 | SD24OSR1);

    if(OSR == SD24_OVERSAMPLE_256)
    {
        return((highResult << 8) | lowResult);
    }
    else if(OSR == SD24_OVERSAMPLE_128)
    {
        return((highResult << 5) | lowResult);
    }
    else if(OSR == SD24_OVERSAMPLE_64)
    {
        return((highResult << 2) | lowResult);
    }
    else        // OSR = SD24_OVERSAMPLE_32
    {
        return (highResult);
    }
}

uint16_t SD24_getHighWordResults(uint16_t baseAddress,
                                 uint8_t converter)
{
    // Calculate address of MEM results
    uint16_t address = baseAddress + (OFS_SD24MEM0 + (converter * 0x02));

    // Get high word result
    HWREG16(baseAddress +
            (OFS_SD24CCTL0 + (converter * 0x02))) &= ~(SD24LSBACC);
    uint16_t highResult = HWREG16(address);

    return(highResult);
}

void SD24_enableInterrupt(uint16_t baseAddress,
                          uint8_t converter,
                          uint16_t mask)
{
    // Calculating address
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    //Clear Interrupt Flags
    HWREG16(address) &= ~(SD24IFG | SD24OVIFG);

    //Enable Interrupt
    if(mask & SD24_CONVERTER_INTERRUPT)
    {
        HWREG16(address) |= (SD24IE);
    }
    if(mask & SD24_CONVERTER_OVERFLOW_INTERRUPT)
    {
        HWREG16(baseAddress + OFS_SD24CTL) |= (SD24OVIE);
    }
}

void SD24_disableInterrupt(uint16_t baseAddress,
                           uint8_t converter,
                           uint16_t mask)
{
    // Calculating address
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    //Enable Interrupt
    if(mask & SD24_CONVERTER_INTERRUPT)
    {
        HWREG16(address) &= ~(SD24IE);
    }
    if(mask & SD24_CONVERTER_OVERFLOW_INTERRUPT)
    {
        HWREG16(baseAddress + OFS_SD24CTL) &= ~(SD24OVIE);
    }
}

void SD24_clearInterrupt(uint16_t baseAddress,
                         uint8_t converter,
                         uint16_t mask)
{
    // Calculating address
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    //Clear Interrupt Flags
    HWREG16(address) &= ~(mask);
}

uint16_t SD24_getInterruptStatus(uint16_t baseAddress,
                                 uint8_t converter,
                                 uint16_t mask)
{
    // Calculate address
    uint16_t address = baseAddress + (OFS_SD24CCTL0 + (converter * 0x02));

    // Read and return interrupt statuses
    return (HWREG16(address) & (mask) & (SD24IFG | SD24OVIFG));
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for sd24_api
//! @}
//
//*****************************************************************************

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
// lcd_e.c - Driver for the lcd_e Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup lcd_e_api lcd_e
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_LCD_E__
#include "lcd_e.h"

#include <assert.h>

//*****************************************************************************
//
//! Initialization parameter instance
//!
//! \param clockSource selects the clock that will be used by the LCD_E.
//!        Valid values are:
//!        - \b LCD_E_CLOCKSOURCE_XTCLK [Default] - The external
//!             oscillator clock.
//!        - \b LCD_E_CLOCKSOURCE_ACLK - The Auxilary Clock.
//!        - \b LCD_E_CLOCKSOURCE_VLOCLK - The internal low power and low
//!             frequency clock.
//!        \n Modified bits are \b LCDSSEL of \b LCDCTL0 register.
//! \param clockDivider selects the divider for LCD_E frequency.
//!        Valid values are:
//!        - \b LCD_E_CLOCKDIVIDER_1 [Default]
//!        - \b LCD_E_CLOCKDIVIDER_2
//!        - \b LCD_E_CLOCKDIVIDER_3
//!        - \b LCD_E_CLOCKDIVIDER_4
//!        - \b LCD_E_CLOCKDIVIDER_5
//!        - \b LCD_E_CLOCKDIVIDER_6
//!        - \b LCD_E_CLOCKDIVIDER_7
//!        - \b LCD_E_CLOCKDIVIDER_8
//!        - \b LCD_E_CLOCKDIVIDER_9
//!        - \b LCD_E_CLOCKDIVIDER_10
//!        - \b LCD_E_CLOCKDIVIDER_11
//!        - \b LCD_E_CLOCKDIVIDER_12
//!        - \b LCD_E_CLOCKDIVIDER_13
//!        - \b LCD_E_CLOCKDIVIDER_14
//!        - \b LCD_E_CLOCKDIVIDER_15
//!        - \b LCD_E_CLOCKDIVIDER_16
//!        - \b LCD_E_CLOCKDIVIDER_17
//!        - \b LCD_E_CLOCKDIVIDER_18
//!        - \b LCD_E_CLOCKDIVIDER_19
//!        - \b LCD_E_CLOCKDIVIDER_20
//!        - \b LCD_E_CLOCKDIVIDER_21
//!        - \b LCD_E_CLOCKDIVIDER_22
//!        - \b LCD_E_CLOCKDIVIDER_23
//!        - \b LCD_E_CLOCKDIVIDER_24
//!        - \b LCD_E_CLOCKDIVIDER_25
//!        - \b LCD_E_CLOCKDIVIDER_26
//!        - \b LCD_E_CLOCKDIVIDER_27
//!        - \b LCD_E_CLOCKDIVIDER_28
//!        - \b LCD_E_CLOCKDIVIDER_29
//!        - \b LCD_E_CLOCKDIVIDER_30
//!        - \b LCD_E_CLOCKDIVIDER_31
//!        - \b LCD_E_CLOCKDIVIDER_32
//!        \n Modified bits are \b LCDDIVx of \b LCDCTL0 register.
//! \param muxRate selects LCD_E mux rate.
//!        Valid values are:
//!        - \b LCD_E_STATIC [Default]
//!        - \b LCD_E_2_MUX
//!        - \b LCD_E_3_MUX
//!        - \b LCD_E_4_MUX
//!        - \b LCD_E_5_MUX
//!        - \b LCD_E_6_MUX
//!        - \b LCD_E_7_MUX
//!        - \b LCD_E_8_MUX
//!        \n Modified bits are \b LCDMXx of \b LCDCTL0 register.
//! \param waveforms selects LCD_E waveform mode.
//!        Valid values are:
//!        - \b LCD_E_STANDARD_WAVEFORMS [Default]
//!        - \b LCD_E_LOW_POWER_WAVEFORMS
//!        \n Modified bits are \b LCDLP of \b LCDCTL0 register.
//! \param segments sets LCD_E segment on/off.
//!        Valid values are:
//!        - \b LCD_E_SEGMENTS_DISABLED [Default]
//!        - \b LCD_E_SEGMENTS_ENABLED
//!        \n Modified bits are \b LCDSON of \b LCDCTL0 register.
//
//*****************************************************************************
const LCD_E_initParam LCD_E_INIT_PARAM = {
    LCD_E_CLOCKSOURCE_XTCLK,
    LCD_E_CLOCKDIVIDER_1,
    LCD_E_STATIC,
    LCD_E_STANDARD_WAVEFORMS,
    LCD_E_SEGMENTS_DISABLED
};

static void setLCDFunction(uint16_t baseAddress,
                           uint8_t index,
                           uint16_t value)
{
    switch(index)
    {
    case 0:
        HWREG16(baseAddress + OFS_LCDPCTL0) |= value;
        break;
    case 1:
        HWREG16(baseAddress + OFS_LCDPCTL1) |= value;
        break;
    case 2:
        HWREG16(baseAddress + OFS_LCDPCTL2) |= value;
        break;
    default: break;
    }
}

void LCD_E_init(uint16_t baseAddress,
                LCD_E_initParam *initParams)
{
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~(LCDMX0 | LCDMX1 | LCDMX2 | LCDSSEL_3
                                            | LCDLP | LCDSON | LCDDIV_31);
    HWREG16(baseAddress + OFS_LCDCTL0) |= initParams->muxRate;
    HWREG16(baseAddress + OFS_LCDCTL0) |= initParams->clockSource;
    HWREG16(baseAddress + OFS_LCDCTL0) |= initParams->waveforms;
    HWREG16(baseAddress + OFS_LCDCTL0) |= initParams->segments;
    HWREG16(baseAddress + OFS_LCDCTL0) |= initParams->clockDivider;
}

void LCD_E_on(uint16_t baseAddress)
{
    HWREG16(SYS_BASE + OFS_SYSCFG2) |= LCDPCTL;
    HWREG16(baseAddress + OFS_LCDCTL0) |= LCDON;
}

void LCD_E_off(uint16_t baseAddress)
{
    HWREG16(SYS_BASE + OFS_SYSCFG2) &= ~LCDPCTL;
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;
}

void LCD_E_clearInterrupt(uint16_t baseAddress,
                          uint16_t mask)
{
    HWREG8(baseAddress + OFS_LCDCTL1_L) &= ~(mask >> 8);
}

uint16_t LCD_E_getInterruptStatus(uint16_t baseAddress,
                                  uint16_t mask)
{
    return (HWREG8(baseAddress + OFS_LCDCTL1_L) & (mask >> 8));
}

void LCD_E_enableInterrupt(uint16_t baseAddress,
                           uint16_t mask)
{
    HWREG16(baseAddress + OFS_LCDCTL1) |= mask;
}

void LCD_E_disableInterrupt(uint16_t baseAddress,
                            uint16_t mask)
{
    HWREG16(baseAddress + OFS_LCDCTL1) &= ~mask;
}

void LCD_E_clearAllMemory(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDMEMCTL) |= LCDCLRM;
}

void LCD_E_clearAllBlinkingMemory(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDMEMCTL) |= LCDCLRBM;
}

void LCD_E_selectDisplayMemory(uint16_t baseAddress,
                               uint16_t displayMemory)
{
    HWREG16(baseAddress + OFS_LCDMEMCTL) &= ~LCDDISP;
    HWREG16(baseAddress + OFS_LCDMEMCTL) |= displayMemory;
}

void LCD_E_setBlinkingControl(uint16_t baseAddress,
                              uint16_t clockPrescalar,
                              uint16_t mode)
{
    HWREG16(baseAddress + OFS_LCDBLKCTL) &=
        ~(LCDBLKPRE2 | LCDBLKPRE1 | LCDBLKPRE0 | LCDBLKMOD_3);

    HWREG16(baseAddress + OFS_LCDBLKCTL) |= (clockPrescalar | mode);
}

void LCD_E_enableChargePump(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDVCTL) |= LCDCPEN;
}

void LCD_E_disableChargePump(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDVCTL) &= ~LCDCPEN;
}

void LCD_E_setChargePumpFreq(uint16_t baseAddress,
                             uint16_t freq)
{
    HWREG16(baseAddress + OFS_LCDVCTL) &=
        ~(LCDCPFSEL3 | LCDCPFSEL2 | LCDCPFSEL1 | LCDCPFSEL0);

    HWREG16(baseAddress + OFS_LCDVCTL) |= freq;
}

void LCD_E_setVLCDSource(uint16_t baseAddress,
                         uint16_t r13Source,
                         uint16_t r33Source)
{
    HWREG16(baseAddress + OFS_LCDVCTL) &= ~(LCDREFMODE | LCDSELVDD);

    HWREG16(baseAddress + OFS_LCDVCTL) |= (r13Source | r33Source);
}

void LCD_E_setVLCDVoltage(uint16_t baseAddress,
                          uint16_t voltage)
{
    HWREG16(baseAddress + OFS_LCDVCTL) &= ~(VLCD3 | VLCD2 | VLCD1 | VLCD0);

    HWREG16(baseAddress + OFS_LCDVCTL) |= voltage;
}

void LCD_E_setReferenceMode(uint16_t baseAddress,
                            uint16_t mode)
{
    HWREG16(baseAddress + OFS_LCDVCTL) &= ~(LCDREFMODE);

    HWREG16(baseAddress + OFS_LCDVCTL) |= mode;
}

void LCD_E_setPinAsLCDFunction(uint16_t baseAddress,
                               uint8_t pin)
{
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;

    uint8_t idx = pin >> 4;
    uint16_t val = 1 << (pin & 0xF);

    setLCDFunction(baseAddress, idx, val);
}

void LCD_E_setPinAsPortFunction(uint16_t baseAddress,
                                uint8_t pin)
{
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;

    uint8_t idx = pin >> 4;
    uint16_t val = 1 << (pin & 0xF);

    switch(idx)
    {
    case 0:
        HWREG16(baseAddress + OFS_LCDPCTL0) &= ~val;
        break;
    case 1:
        HWREG16(baseAddress + OFS_LCDPCTL1) &= ~val;
        break;
    case 2:
        HWREG16(baseAddress + OFS_LCDPCTL2) &= ~val;
        break;
    default: break;
    }
}

void LCD_E_setPinAsLCDFunctionEx(uint16_t baseAddress,
                                 uint8_t startPin,
                                 uint8_t endPin)
{
    uint8_t startIdx = startPin >> 4;
    uint8_t endIdx = endPin >> 4;
    uint8_t startPos = startPin & 0xF;
    uint8_t endPos = endPin & 0xF;
    uint16_t val = 0;
    uint8_t i = 0;

    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;

    if(startIdx == endIdx)
    {
        val = (0xFFFF >> (15 - endPos)) & (0xFFFF << startPos);

        setLCDFunction(baseAddress, startIdx, val);
    }
    else
    {
        val = 0xFFFF >> (15 - endPos);
        setLCDFunction(baseAddress, endIdx, val);

        for(i = endIdx - 1; i > startIdx; i--)
        {
            setLCDFunction(baseAddress, i, 0xFFFF);
        }

        val = 0xFFFF << startPos;
        setLCDFunction(baseAddress, startIdx, val);
    }
}

void LCD_E_setPinAsCOM(uint16_t baseAddress,
                       uint8_t pin,
                       uint8_t com)
{
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;

    uint8_t idx = pin >> 4;
    uint16_t val = 1 << (pin & 0xF);
    uint8_t muxRate =
        HWREG16(baseAddress + OFS_LCDCTL0) & (LCDMX2 | LCDMX1 | LCDMX0);

    switch(idx)
    {
    case 0:
        HWREG16(baseAddress + OFS_LCDCSSEL0) |= val;
        break;
    case 1:
        HWREG16(baseAddress + OFS_LCDCSSEL1) |= val;
        break;
    case 2:
        HWREG16(baseAddress + OFS_LCDCSSEL2) |= val;
        break;
    default: break;
    }

    // Set LCDMem to COM as well for corresponding pins
    if(muxRate <= (LCDMX1 | LCDMX0))
    {
        if(pin & 1)
        {
            HWREG8(baseAddress + OFS_LCDM0W + pin / 2) |= (com & 0xF) << 4;
            HWREG8(baseAddress + OFS_LCDBM0W + pin / 2) |= (com & 0xF) << 4;
        }
        else
        {
            HWREG8(baseAddress + OFS_LCDM0W + pin / 2) |= (com & 0xF);
            HWREG8(baseAddress + OFS_LCDBM0W + pin / 2) |= (com & 0xF);
        }
    }
    else
    {
        //5-mux, 6-mux, 7-mux, 8-mux
        HWREG8(baseAddress + OFS_LCDM0W + pin) |= com;
        HWREG8(baseAddress + OFS_LCDBM0W + pin) |= com;
    }
}

void LCD_E_setPinAsSEG(uint16_t baseAddress,
                       uint8_t pin)
{
    // Set LCDMem to COM as well for corresponding pins
    HWREG16(baseAddress + OFS_LCDCTL0) &= ~LCDON;

    uint8_t idx = pin >> 4;
    uint16_t val = 1 << (pin & 0xF);

    switch(idx)
    {
    case 0:
        HWREG16(baseAddress + OFS_LCDCSSEL0) &= ~val;
        break;
    case 1:
        HWREG16(baseAddress + OFS_LCDCSSEL1) &= ~val;
        break;
    case 2:
        HWREG16(baseAddress + OFS_LCDCSSEL2) &= ~val;
        break;
    default: break;
    }
}

void LCD_E_setMemory(uint16_t baseAddress,
                     uint8_t memory,
                     uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM0W + memory) = mask;
}

void LCD_E_updateMemory(uint16_t baseAddress,
                        uint8_t memory,
                        uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM0W + memory) |= mask;
}

void LCD_E_toggleMemory(uint16_t baseAddress,
                        uint8_t memory,
                        uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM0W + memory) ^= mask;
}

void LCD_E_clearMemory(uint16_t baseAddress,
                       uint8_t memory,
                       uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM0W + memory) &= ~mask;
}

void LCD_E_setBlinkingMemory(uint16_t baseAddress,
                             uint8_t memory,
                             uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDBM0W + memory) = mask;
}

void LCD_E_updateBlinkingMemory(uint16_t baseAddress,
                                uint8_t memory,
                                uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDBM0W + memory) |= mask;
}

void LCD_E_toggleBlinkingMemory(uint16_t baseAddress,
                                uint8_t memory,
                                uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDBM0W + memory) ^= mask;
}

void LCD_E_clearBlinkingMemory(uint16_t baseAddress,
                               uint8_t memory,
                               uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDBM0W + memory) &= ~mask;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for lcd_e_api
//! @}
//
//*****************************************************************************

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
// The following value is used by CS_getACLK, CS_getSMCLK, CS_getMCLK to
// determine the operating frequency based on the available DCO frequencies.
//
//*****************************************************************************
#define CS_DCO_FREQ_1                                                    5330000
#define CS_DCO_FREQ_2                                                    6670000
#define CS_DCO_FREQ_3                                                    8000000
#define CS_DCO_FREQ_4                                                   16000000
#define CS_DCO_FREQ_5                                                   20000000
#define CS_DCO_FREQ_6                                                   24000000

//*****************************************************************************
//
// Internal very low power VLOCLK, low frequency oscillator with 10kHz typical
// frequency, internal low-power oscillator MODCLK with 5 MHz typical
// frequency and LFMODCLK is MODCLK divided by 128.
//
//*****************************************************************************
#define CS_VLOCLK_FREQUENCY                                                10000
#define CS_MODCLK_FREQUENCY                                              5000000
#define CS_LFMODCLK_FREQUENCY                                              39062

//*****************************************************************************
//
// The following value is used by CS_turnOnXT1, CS_bypassXT1,
// CS_turnOnXT1WithTimeout, CS_bypassXT1WithTimeout to properly set the XTS bit.
// This frequnecy threshold is specified in the User's Guide
//
//*****************************************************************************
#define XT1_FREQUENCY_THRESHOLD                                         50000

//*****************************************************************************
//
// The XT1 crystal frequency. Should be set with CS_externalClockSourceInit if
// XT1 is used and user intends to invoke CS_getSMCLK, CS_getMCLK, CS_getACLK
// and turnOnXT1, XT1ByPass, turnOnXT1WithTimeout, XT1ByPassWithTimeout.
//
//*****************************************************************************
static uint32_t privateXT1ClockFrequency = 0;

//*****************************************************************************
//
// The XT2 crystal frequency. Should be set with CS_externalClockSourceInit if
// XT2 is used and user intends to invoke CS_getSMCLK, CS_getMCLK and
// CS_getACLK.
//
//*****************************************************************************
static uint32_t privateXT2ClockFrequency = 0;

static uint32_t privateCSSourceClockFromDCO(uint8_t clockdivider)
{
    uint32_t CLKFrequency = 0;

    if(HWREG16(CS_BASE + OFS_CSCTL1) & DCORSEL)
    {
        switch(HWREG16(CS_BASE + OFS_CSCTL1) & DCOFSEL_3)
        {
        case DCOFSEL_3:
            CLKFrequency = CS_DCO_FREQ_6 / clockdivider;
            break;
        case DCOFSEL_2:
            CLKFrequency = CS_DCO_FREQ_4 / clockdivider;
            break;
        case DCOFSEL_1:
            CLKFrequency = CS_DCO_FREQ_5 / clockdivider;
            break;
        default:
            CLKFrequency = CS_DCO_FREQ_4 / clockdivider;
            break;
        }
    }
    else
    {
        switch(HWREG16(CS_BASE + OFS_CSCTL1) & DCOFSEL_3)
        {
        case DCOFSEL_3:
            CLKFrequency = CS_DCO_FREQ_3 / clockdivider;
            break;
        case DCOFSEL_2:
            CLKFrequency = CS_DCO_FREQ_1 / clockdivider;
            break;
        case DCOFSEL_1:
            CLKFrequency = CS_DCO_FREQ_2 / clockdivider;
            break;
        default:
            CLKFrequency = CS_DCO_FREQ_1 / clockdivider;
            break;
        }
    }

    return (CLKFrequency);
}

static uint32_t privateCSComputeCLKFrequency(uint16_t CLKSource,
                                             uint16_t CLKSourceDivider)
{
    uint32_t CLKFrequency = 0;
    uint8_t CLKSourceFrequencyDivider = 1;
    uint8_t i = 0;

    // Determine Frequency divider
    for(i = 0; i < CLKSourceDivider; i++)
    {
        CLKSourceFrequencyDivider *= 2;
    }

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    // Determine clock source based on CLKSource
    switch(CLKSource)
    {
    // If XT1 is selected as clock source
    case SELM__XT1CLK:
        CLKFrequency = (privateXT1ClockFrequency /
                        CLKSourceFrequencyDivider);

        //Check if XT1OFFG is not set. If fault flag is set
        //VLO is used as source clock
        if(HWREG8(CS_BASE + OFS_CSCTL5) & XT1OFFG)
        {
            HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT1OFFG);
            //Clear OFIFG fault flag
            HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

            if(HWREG8(CS_BASE + OFS_CSCTL5) & XT1OFFG)
            {
                CLKFrequency = CS_VLOCLK_FREQUENCY;
            }
        }
        break;

    case SELM__VLOCLK:
        CLKFrequency =
            (CS_VLOCLK_FREQUENCY / CLKSourceFrequencyDivider);
        break;

    case SELM__DCOCLK:
        CLKFrequency =
            privateCSSourceClockFromDCO(CLKSourceFrequencyDivider);

        break;

    case SELM__XT2CLK:
        CLKFrequency =
            (privateXT2ClockFrequency / CLKSourceFrequencyDivider);

        if(HWREG8(CS_BASE + OFS_CSCTL5) & XT2OFFG)
        {
            HWREG8(CS_BASE + OFS_CSCTL5) &= ~XT2OFFG;
            //Clear OFIFG fault flag
            HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
        }

        if(HWREG8(CS_BASE + OFS_CSCTL5) & XT2OFFG)
        {
            CLKFrequency = CS_MODCLK_FREQUENCY;
        }
        break;
    }

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;

    return (CLKFrequency);
}

void CS_setExternalClockSource(uint32_t XT1CLK_frequency,
                               uint32_t XT2CLK_frequency)
{
    privateXT1ClockFrequency = XT1CLK_frequency;
    privateXT2ClockFrequency = XT2CLK_frequency;
}

void CS_initClockSignal(uint8_t selectedClockSignal,
                        uint16_t clockSource,
                        uint16_t clockSourceDivider)
{
    //Verify User has selected a valid Frequency divider
    assert(
        (CS_CLOCK_DIVIDER_1 == clockSourceDivider) ||
        (CS_CLOCK_DIVIDER_2 == clockSourceDivider) ||
        (CS_CLOCK_DIVIDER_4 == clockSourceDivider) ||
        (CS_CLOCK_DIVIDER_8 == clockSourceDivider) ||
        (CS_CLOCK_DIVIDER_16 == clockSourceDivider) ||
        (CS_CLOCK_DIVIDER_32 == clockSourceDivider)
        );

    // Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    switch(selectedClockSignal)
    {
    case CS_ACLK:
        //verify User has selected a valid clock source for ACLK
        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_VLOCLK_SELECT == clockSource) ||
            (CS_DCOCLK_SELECT == clockSource) ||
            (CS_XT2CLK_SELECT == clockSource)
            );
        clockSourceDivider = clockSourceDivider << 8;
        clockSource = clockSource << 8;

        HWREG16(CS_BASE + OFS_CSCTL2) &= ~(SELA_7);
        HWREG16(CS_BASE + OFS_CSCTL2) |= (clockSource);
        HWREG16(CS_BASE + OFS_CSCTL3) &= ~(DIVA0 + DIVA1 + DIVA2);
        HWREG16(CS_BASE + OFS_CSCTL3) |= clockSourceDivider;
        break;
    case CS_SMCLK:
        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_VLOCLK_SELECT == clockSource) ||
            (CS_DCOCLK_SELECT == clockSource) ||
            (CS_XT2CLK_SELECT == clockSource)
            );

        clockSource = clockSource << 4;
        clockSourceDivider = clockSourceDivider << 4;

        HWREG16(CS_BASE + OFS_CSCTL2) &= ~(SELS_7);
        HWREG16(CS_BASE + OFS_CSCTL2) |= clockSource;
        HWREG16(CS_BASE + OFS_CSCTL3) &= ~(DIVS0 + DIVS1 + DIVS2);
        HWREG16(CS_BASE + OFS_CSCTL3) |= clockSourceDivider;
        break;
    case CS_MCLK:
        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_VLOCLK_SELECT == clockSource) ||
            (CS_DCOCLK_SELECT == clockSource) ||
            (CS_XT2CLK_SELECT == clockSource)
            );

        HWREG16(CS_BASE + OFS_CSCTL2) &= ~(SELM_7);
        HWREG16(CS_BASE + OFS_CSCTL2) |= clockSource;
        HWREG16(CS_BASE + OFS_CSCTL3) &= ~(DIVM0 + DIVM1 + DIVM2);
        HWREG16(CS_BASE + OFS_CSCTL3) |= clockSourceDivider;
        break;
    }

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_turnOnXT1(uint16_t xt1drive)
{
    assert(privateXT1ClockFrequency != 0);

    assert((xt1drive == CS_XT1_DRIVE0) ||
           (xt1drive == CS_XT1_DRIVE1) ||
           (xt1drive == CS_XT1_DRIVE2) ||
           (xt1drive == CS_XT1_DRIVE3));

    // Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch ON XT1 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT1OFF;

    //Highest drive setting for turnOnXT1up
    HWREG16(CS_BASE + OFS_CSCTL4_L) |= XT1DRIVE1_L + XT1DRIVE0_L;

    //If user is using XT1 in HF mode
    if(privateXT1ClockFrequency > XT1_FREQUENCY_THRESHOLD)
    {
        HWREG16(CS_BASE + OFS_CSCTL4) |= XTS;
        HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT1BYPASS;
    }
    else
    {
        HWREG16(CS_BASE + OFS_CSCTL4) &= ~(XTS + XT1BYPASS);
    }

    //Wait for Crystal to stabilize
    while(HWREG8(CS_BASE + OFS_CSCTL5) & XT1OFFG)
    {
        //Clear OSC flaut Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT1OFFG);

        //Clear OFIFG fault flag
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    //set requested Drive mode
    HWREG16(CS_BASE + OFS_CSCTL4) = (HWREG16(CS_BASE + OFS_CSCTL4) &
                                     ~(XT1DRIVE_3)
                                     ) |
                                    (xt1drive);

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_turnOnSMCLK(void)
{
    // Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    HWREG16(CS_BASE + OFS_CSCTL4) &= ~SMCLKOFF;

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_turnOffSMCLK(void)
{
    // Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    HWREG16(CS_BASE + OFS_CSCTL4) |= SMCLKOFF;

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_bypassXT1(void)
{
    //Verify user has set frequency of XT1 with SetExternalClockSource
    assert(privateXT1ClockFrequency != 0);

    // Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    if(privateXT1ClockFrequency > XT1_FREQUENCY_THRESHOLD)
    {
        // Set XT1 in HF mode switch off XT1 Oscillator and enable BYPASS mode
        HWREG16(CS_BASE + OFS_CSCTL4) |= (XTS + XT1BYPASS + XT1OFF);
    }
    else
    {
        // Set XT1 in LF mode Switch off XT1 oscillator and enable BYPASS mode
        HWREG16(CS_BASE + OFS_CSCTL4) &= ~XTS;
        HWREG16(CS_BASE + OFS_CSCTL4) |= (XT1BYPASS + XT1OFF);
    }

    // Set XT1 in LF mode Switch off XT1 oscillator and enable BYPASS mode
    HWREG16(CS_BASE + OFS_CSCTL4) |= (XT1BYPASS + XT1OFF);

    //Wait until XT1 stabilizes
    while(HWREG8(CS_BASE + OFS_CSCTL5) & XT1OFFG)
    {
        //Clear OSC flaut Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT1OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }
    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

bool CS_turnOnXT1WithTimeout(uint16_t xt1drive,
                             uint32_t timeout)
{
    assert(privateXT1ClockFrequency != 0);

    assert((xt1drive == CS_XT1_DRIVE0) ||
           (xt1drive == CS_XT1_DRIVE1) ||
           (xt1drive == CS_XT1_DRIVE2) ||
           (xt1drive == CS_XT1_DRIVE3));

    assert(timeout > 0);

    // Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch ON XT1 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT1OFF;

    //Highest drive setting for turnOnXT1up
    HWREG16(CS_BASE + OFS_CSCTL4_L) |= XT1DRIVE1_L + XT1DRIVE0_L;

    //If user is using XT1 in HF mode
    if(privateXT1ClockFrequency > XT1_FREQUENCY_THRESHOLD)
    {
        HWREG16(CS_BASE + OFS_CSCTL4) |= XTS;
        HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT1BYPASS;
    }
    else
    {
        //If user is using XT1 in HF mode
        HWREG16(CS_BASE + OFS_CSCTL4) &= ~(XTS + XT1BYPASS);
    }

    while((HWREG8(CS_BASE + OFS_CSCTL5) & XT1OFFG) && --timeout)
    {
        //Clear OSC fault Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT1OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    if(timeout)
    {
        //set requested Drive mode
        HWREG16(CS_BASE + OFS_CSCTL4) = (HWREG16(CS_BASE + OFS_CSCTL4) &
                                         ~(XT1DRIVE_3)
                                         ) |
                                        (xt1drive);
        // Lock CS control register
        HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
        return (STATUS_SUCCESS);
    }
    else
    {
        // Lock CS control register
        HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
        return (STATUS_FAIL);
    }
}

bool CS_bypassXT1WithTimeout(uint32_t timeout)
{
    assert(privateXT1ClockFrequency != 0);

    assert(timeout > 0);

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    if(privateXT1ClockFrequency > XT1_FREQUENCY_THRESHOLD)
    {
        // Set XT1 in HF mode switch off XT1 Oscillator and enable BYPASS mode
        HWREG16(CS_BASE + OFS_CSCTL4) |= (XTS + XT1BYPASS + XT1OFF);
    }
    else
    {
        // Set XT1 in LF mode Switch off XT1 oscillator and enable BYPASS mode
        HWREG16(CS_BASE + OFS_CSCTL4) &= ~XTS;
        HWREG16(CS_BASE + OFS_CSCTL4) |= (XT1BYPASS + XT1OFF);
    }

    while((HWREG8(CS_BASE + OFS_CSCTL5) & XT1OFFG) && --timeout)
    {
        //Clear OSC fault Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT1OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;

    if(timeout)
    {
        return (STATUS_SUCCESS);
    }
    else
    {
        return (STATUS_FAIL);
    }
}

void CS_turnOffXT1(void)
{
    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;
    //Switch off XT1 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) |= XT1OFF;
    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_turnOnXT2(uint16_t xt2drive)
{
    assert(privateXT2ClockFrequency != 0);

    assert((xt2drive == CS_XT2_DRIVE_4MHZ_8MHZ) ||
           (xt2drive == CS_XT2_DRIVE_8MHZ_16MHZ) ||
           (xt2drive == CS_XT2_DRIVE_16MHZ_24MHZ)||
           (xt2drive == CS_XT2_DRIVE_24MHZ_32MHZ));

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch on XT2 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT2OFF;

    //Disable XT2BYPASS mode and Switch on XT2 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT2BYPASS;

    while(HWREG8(CS_BASE + OFS_CSCTL5) & XT2OFFG)
    {
        //Clear OSC flaut Flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT2OFFG);

        //Clear OFIFG fault flag
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    HWREG16(CS_BASE + OFS_CSCTL4) = (HWREG16(CS_BASE + OFS_CSCTL4) &
                                     ~(CS_XT2_DRIVE_24MHZ_32MHZ)
                                     ) | (xt2drive);

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_bypassXT2(void)
{
    //Verify user has initialized value of XT2Clock
    assert(privateXT2ClockFrequency != 0);

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch off XT2 oscillator and set it to BYPASS mode
    HWREG16(CS_BASE + OFS_CSCTL4) |= (XT2BYPASS + XT2OFF);

    while(HWREG8(CS_BASE + OFS_CSCTL5) & XT2OFFG)
    {
        //Clear OSC fault Flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT2OFFG);

        //Clear OFIFG fault flag
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

bool CS_turnOnXT2WithTimeout(uint16_t xt2drive,
                             uint32_t timeout)
{
    //Verify user has initialized value of XT2Clock
    assert(privateXT2ClockFrequency != 0);

    assert(timeout > 0);

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch on XT2 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT2OFF;

    // Disable XT2BYPASS mode
    HWREG16(CS_BASE + OFS_CSCTL4) &= ~XT2BYPASS;

    while((HWREG8(CS_BASE + OFS_CSCTL5) & XT2OFFG) && --timeout)
    {
        //Clear OSC fault Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT2OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    if(timeout)
    {
        //Set drive strength for XT2
        HWREG16(CS_BASE + OFS_CSCTL4) = (HWREG16(CS_BASE + OFS_CSCTL4) &
                                         ~(CS_XT2_DRIVE_24MHZ_32MHZ)
                                         ) |
                                        (xt2drive);

        // Lock CS control register
        HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;

        return (STATUS_SUCCESS);
    }
    else
    {
        return (STATUS_FAIL);
    }
}

bool CS_bypassXT2WithTimeout(uint32_t timeout)
{
    //Verify user has initialized value of XT2Clock
    assert(privateXT2ClockFrequency != 0);

    assert(timeout > 0);

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch off XT2 oscillator and enable BYPASS mode
    HWREG16(CS_BASE + OFS_CSCTL4) |= (XT2BYPASS + XT2OFF);

    while((HWREG8(CS_BASE + OFS_CSCTL5) & XT2OFFG) && --timeout)
    {
        //Clear OSC fault Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT2OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;

    if(timeout)
    {
        return (STATUS_SUCCESS);
    }
    else
    {
        return (STATUS_FAIL);
    }
}

void CS_turnOffXT2(void)
{
    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    //Switch off XT2 oscillator
    HWREG16(CS_BASE + OFS_CSCTL4) |= XT2OFF;

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_enableClockRequest(uint8_t selectClock)
{
    assert(
        (CS_ACLK == selectClock)||
        (CS_SMCLK == selectClock)||
        (CS_MCLK == selectClock)||
        (CS_MODOSC == selectClock));

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    HWREG8(CS_BASE + OFS_CSCTL6) |= selectClock;

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

void CS_disableClockRequest(uint8_t selectClock)
{
    assert(
        (CS_ACLK == selectClock)||
        (CS_SMCLK == selectClock)||
        (CS_MCLK == selectClock)||
        (CS_MODOSC == selectClock));

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    HWREG8(CS_BASE + OFS_CSCTL6) &= ~selectClock;

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

uint8_t CS_getFaultFlagStatus(uint8_t mask)
{
    assert(mask <= CS_turnOffXT2G);
    return (HWREG8(CS_BASE + OFS_CSCTL5) & mask);
}

void CS_clearFaultFlag(uint8_t mask)
{
    assert(mask <= CS_turnOffXT2G);

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    HWREG8(CS_BASE + OFS_CSCTL5) &= ~mask;

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

uint32_t CS_getACLK(void)
{
    //Find ACLK source
    uint16_t ACLKSource = (HWREG16(CS_BASE + OFS_CSCTL2) & SELA_7);
    ACLKSource = ACLKSource >> 8;

    //Find ACLK frequency divider
    uint16_t ACLKSourceDivider = HWREG16(CS_BASE + OFS_CSCTL3) & SELA_7;
    ACLKSourceDivider = ACLKSourceDivider >> 8;

    return (privateCSComputeCLKFrequency(
                ACLKSource,
                ACLKSourceDivider));
}

uint32_t CS_getSMCLK(void)
{
    //Find SMCLK source
    uint16_t SMCLKSource = HWREG8(CS_BASE + OFS_CSCTL2) & SELS_7;

    SMCLKSource = SMCLKSource >> 4;

    //Find SMCLK frequency divider
    uint16_t SMCLKSourceDivider = HWREG16(CS_BASE + OFS_CSCTL3) & SELS_7;
    SMCLKSourceDivider = SMCLKSourceDivider >> 4;

    return (privateCSComputeCLKFrequency(
                SMCLKSource,
                SMCLKSourceDivider)
            );
}

uint32_t CS_getMCLK(void)
{
    //Find MCLK source
    uint16_t MCLKSource = (HWREG16(CS_BASE + OFS_CSCTL2) & SELM_7);
    //Find MCLK frequency divider
    uint16_t MCLKSourceDivider = HWREG16(CS_BASE + OFS_CSCTL3) & SELM_7;

    return (privateCSComputeCLKFrequency(
                MCLKSource,
                MCLKSourceDivider)
            );
}

uint16_t CS_clearAllOscFlagsWithTimeout(uint32_t timeout)
{
    assert(timeout > 0);

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    do
    {
        // Clear all osc fault flags
        HWREG8(CS_BASE + OFS_CSCTL5) &= ~(XT1OFFG + XT2OFFG);

        // Clear the global osc fault flag.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

        // Check XT1 fault flags
    }
    while((HWREG8(SFR_BASE + OFS_SFRIFG1) & OFIFG) && --timeout);

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;

    return (HWREG8(CS_BASE + OFS_CSCTL5) & (XT1OFFG + XT2OFFG));
}

void CS_setDCOFreq(uint16_t dcorsel,
                   uint16_t dcofsel)
{
    //Verify User has selected a valid frequency option
    assert(
        (dcofsel == CS_DCOFSEL_0)||
        (dcofsel == CS_DCOFSEL_1)||
        (dcofsel == CS_DCOFSEL_2)||
        (dcofsel == CS_DCOFSEL_3));
    //Verify User has selected a valid frequency option

    //Verify user has selected a valid DCO Frequency Range option
    assert(
        (dcorsel == CS_DCORSEL_0)||
        (dcorsel == CS_DCORSEL_1));

    //Unlock CS control register
    HWREG16(CS_BASE + OFS_CSCTL0) = CSKEY;

    // Set user's frequency selection for DCO
    HWREG16(CS_BASE + OFS_CSCTL1) = (dcorsel + dcofsel);

    // Lock CS control register
    HWREG8(CS_BASE + OFS_CSCTL0_H) = 0x00;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for cs_api
//! @}
//
//*****************************************************************************

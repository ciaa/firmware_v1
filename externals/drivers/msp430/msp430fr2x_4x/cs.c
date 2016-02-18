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
// Internal very low power VLOCLK, low frequency oscillator with 10 kHz typical
// frequency
//
//*****************************************************************************
#define CS_VLOCLK_FREQUENCY                                               10000

//*****************************************************************************
//
// Internal, trimmed, low-frequency oscillator with 32768 Hz typical frequency
//
//*****************************************************************************
#define CS_REFOCLK_FREQUENCY                                              32768

//******************************************************************************
//
//The XT1 crystal frequency. Should be set with CS_externalClockSourceInit
//if XT1 is used and user intends to invoke CS_getSMCLK, CS_getMCLK or
//CS_getACLK
//
//******************************************************************************
static uint32_t privateXT1ClockFrequency = 0;

static uint32_t privateCSSourceClockFromDCO(uint16_t FLLRefCLKSource)
{
    assert((SELMS__DCOCLKDIV == FLLRefCLKSource));

    uint16_t N_value;
    uint16_t n_value = 1;
    uint32_t Fref_value;

    N_value = (HWREG16(CS_BASE + OFS_CSCTL2)) & 0x03FF;
    uint16_t tempDivider = HWREG8(CS_BASE + OFS_CSCTL3) & FLLREFDIV_7;

    if(tempDivider > 1)
    {
        n_value = 32 << (tempDivider - 1);
    }

    switch((HWREG8(CS_BASE + OFS_CSCTL3)) & SELREF_3)
    {
    case SELREF__XT1CLK:
        Fref_value = privateXT1ClockFrequency;

        if(XTS != (HWREG16(CS_BASE + OFS_CSCTL6) & XTS))
        {
            if(HWREG8(CS_BASE + OFS_CSCTL7) & XT1OFFG)
            {
                HWREG8(CS_BASE + OFS_CSCTL7) &= ~(XT1OFFG);
                //Clear OFIFG fault flag
                HWREG8(CS_BASE + OFS_SFRIFG1) &= ~OFIFG;

                if(HWREG8(CS_BASE + OFS_CSCTL7) & XT1OFFG)
                {
                    Fref_value = CS_REFOCLK_FREQUENCY;
                }
            }
        }
        break;
    case SELREF__REFOCLK:
        Fref_value = CS_REFOCLK_FREQUENCY;
        break;
    default: assert(0);
    }

    return (Fref_value * (N_value + 1) / n_value);
}

static uint32_t privateCSComputeCLKFrequency(uint16_t CLKSource,
                                             uint16_t CLKSourceDivider)
{
    uint32_t CLKFrequency = 0;
    uint8_t CLKSourceFrequencyDivider = 1;

    CLKSourceFrequencyDivider = 1 << CLKSourceDivider;

    switch(CLKSource)
    {
    case SELMS__XT1CLK:
        CLKFrequency = (privateXT1ClockFrequency /
                        CLKSourceFrequencyDivider);

        if(XTS != (HWREG16(CS_BASE + OFS_CSCTL6) & XTS))
        {
            if(HWREG8(CS_BASE + OFS_CSCTL7) & XT1OFFG)
            {
                HWREG8(CS_BASE + OFS_CSCTL7) &= ~(XT1OFFG);
                //Clear OFIFG fault flag
                HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

                if(HWREG8(CS_BASE + OFS_CSCTL7) & XT1OFFG)
                {
                    CLKFrequency = CS_REFOCLK_FREQUENCY;
                }
            }
        }
        break;

    case SELMS__VLOCLK:
        CLKFrequency =
            (CS_VLOCLK_FREQUENCY / CLKSourceFrequencyDivider);
        break;
    case SELMS__REFOCLK:
        CLKFrequency =
            (CS_REFOCLK_FREQUENCY / CLKSourceFrequencyDivider);
        break;
    case SELMS__DCOCLKDIV:
        CLKFrequency =
            privateCSSourceClockFromDCO(CLKSource)
            / CLKSourceFrequencyDivider;
        break;
    }

    return (CLKFrequency);
}

void CS_setExternalClockSource(uint32_t XT1CLK_frequency)
{
    privateXT1ClockFrequency = XT1CLK_frequency;
}

void CS_initClockSignal(uint8_t selectedClockSignal,
                        uint16_t clockSource,
                        uint16_t clockSourceDivider)
{
    switch(selectedClockSignal)
    {
    case CS_ACLK:

        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_REFOCLK_SELECT == clockSource)
            );
        assert(CS_CLOCK_DIVIDER_1 == clockSourceDivider);

        HWREG16(CS_BASE + OFS_CSCTL4) &= ~(SELA);

        if(clockSource == CS_XT1CLK_SELECT)
        {
            clockSource = 0x0;
        }
        clockSource = clockSource << 8;

        HWREG16(CS_BASE + OFS_CSCTL4) |= (clockSource);

        clockSourceDivider = clockSourceDivider << 8;
        break;
    case CS_SMCLK:

        assert(
            (CS_CLOCK_DIVIDER_1 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_2 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_4 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_8 == clockSourceDivider)
            );
        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_VLOCLK_SELECT == clockSource) ||
            (CS_REFOCLK_SELECT == clockSource) ||
            (CS_DCOCLKDIV_SELECT == clockSource)
            );

        HWREG16(CS_BASE + OFS_CSCTL4) &= ~(SELMS_7);
        HWREG16(CS_BASE + OFS_CSCTL4) |= (clockSource);

        HWREG16(CS_BASE + OFS_CSCTL5) &= ~(DIVS_3);
        clockSourceDivider = clockSourceDivider << 4;
        HWREG16(CS_BASE + OFS_CSCTL5) |= clockSourceDivider;
        break;
    case CS_MCLK:

        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_VLOCLK_SELECT == clockSource) ||
            (CS_REFOCLK_SELECT == clockSource) ||
            (CS_DCOCLKDIV_SELECT == clockSource)
            );

        assert(
            (CS_CLOCK_DIVIDER_1 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_2 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_4 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_8 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_16 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_32 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_64 == clockSourceDivider) ||
            (CS_CLOCK_DIVIDER_128 == clockSourceDivider)
            );

        HWREG16(CS_BASE + OFS_CSCTL4) &= ~(SELMS_7);
        HWREG16(CS_BASE + OFS_CSCTL4) |= (clockSource);

        HWREG16(CS_BASE + OFS_CSCTL5) &= ~(DIVM_7);
        HWREG16(CS_BASE + OFS_CSCTL5) |= clockSourceDivider;
        break;
    case CS_FLLREF:
        assert((clockSourceDivider == CS_CLOCK_DIVIDER_1)  ||
               (clockSourceDivider == CS_CLOCK_DIVIDER_32)  ||
               (clockSourceDivider == CS_CLOCK_DIVIDER_64)  ||
               (clockSourceDivider == CS_CLOCK_DIVIDER_128)  ||
               (clockSourceDivider == CS_CLOCK_DIVIDER_256)   ||
               (clockSourceDivider == CS_CLOCK_DIVIDER_512)
               );
        assert(
            (CS_XT1CLK_SELECT == clockSource) ||
            (CS_REFOCLK_SELECT == clockSource)
            );

        HWREG8(CS_BASE + OFS_CSCTL3) &= ~(SELREF_3);

        if(clockSource == CS_XT1CLK_SELECT)
        {
            clockSource = 0x0;
        }
        clockSource = clockSource << 4;
        HWREG8(CS_BASE + OFS_CSCTL3) |= (clockSource);

        HWREG8(CS_BASE + OFS_CSCTL3) &= ~(FLLREFDIV_7);
        //Note that dividers for FLLREF are slightly different
        //Hence handled differently from other CLK signals
        if(clockSourceDivider != CS_CLOCK_DIVIDER_1)
        {
            HWREG8(CS_BASE + OFS_CSCTL3) |= (clockSourceDivider - 4);
        }
        break;
    }
}

void CS_turnOnXT1(uint16_t xt1drive){
    assert((xt1drive == CS_XT1_DRIVE_0) ||
           (xt1drive == CS_XT1_DRIVE_1) ||
           (xt1drive == CS_XT1_DRIVE_2) ||
           (xt1drive == CS_XT1_DRIVE_3));

    //Switch ON XT1 oscillator
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~XT1AUTOOFF;

    //Highest drive setting for turnOnXT1
    HWREG16(CS_BASE + OFS_CSCTL6_L) |= XT1DRIVE1_L + XT1DRIVE0_L;

    //Enable LF mode and clear bypass
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~(XTS + XT1BYPASS);

    while(HWREG8(CS_BASE + OFS_CSCTL7) & XT1OFFG)
    {
        //Clear OSC fault flag
        HWREG8(CS_BASE + OFS_CSCTL7) &= ~(XT1OFFG);

        //Clear OFIFG fault flag
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    //set requested Drive mode
    HWREG16(CS_BASE + OFS_CSCTL6) = (HWREG16(CS_BASE + OFS_CSCTL6) &
                                     ~(XT1DRIVE_3)
                                     ) |
                                    (xt1drive);
}

void CS_turnOnSMCLK(void)
{
    //Turn on SMCLK
    HWREG16(CS_BASE + OFS_CSCTL5) &= ~SMCLKOFF;
}

void CS_turnOffSMCLK(void)
{
    //Turn off SMCLK
    HWREG16(CS_BASE + OFS_CSCTL5) |= SMCLKOFF;
}

void CS_enableVLOAutoOff(void)
{
    //Enable VLO Auto Off
    HWREG16(CS_BASE + OFS_CSCTL5) |= VLOAUTOOFF;
}

void CS_disableVLOAutoOff(void)
{
    //Disable VLO Auto Off
    HWREG16(CS_BASE + OFS_CSCTL5) &= ~VLOAUTOOFF;
}

void CS_bypassXT1(void)
{
    //Enable HF/LF mode
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~XTS;

    //Switch OFF XT1 oscillator and enable BYPASS mode
    HWREG16(CS_BASE + OFS_CSCTL6) |= (XT1BYPASS + XT1AUTOOFF);

    while(HWREG8(CS_BASE + OFS_CSCTL7) & (XT1OFFG))
    {
        //Clear OSC flaut Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL7) &= ~(XT1OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }
}

bool CS_turnOnXT1WithTimeout(uint16_t xt1drive,
                             uint16_t timeout)
{
    assert((xt1drive == CS_XT1_DRIVE_0) ||
           (xt1drive == CS_XT1_DRIVE_1) ||
           (xt1drive == CS_XT1_DRIVE_2) ||
           (xt1drive == CS_XT1_DRIVE_3));

    //Switch ON XT1 oscillator
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~XT1AUTOOFF;

    //Highest drive setting for turnOnXT1
    HWREG16(CS_BASE + OFS_CSCTL6_L) |= XT1DRIVE1_L + XT1DRIVE0_L;

    //Enable LF mode and clear bypass
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~(XTS + XT1BYPASS);

    do
    {
        HWREG8(CS_BASE + OFS_CSCTL7) &= ~(XT1OFFG);

        //Clear OFIFG fault flag
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }
    while((HWREG8(CS_BASE + OFS_CSCTL7) & XT1OFFG) && --timeout);

    if(timeout)
    {
        //set requested Drive mode
        HWREG16(CS_BASE + OFS_CSCTL6) = (HWREG16(CS_BASE + OFS_CSCTL6) &
                                         ~(XT1DRIVE_3)
                                         ) |
                                        (xt1drive);

        return (STATUS_SUCCESS);
    }
    else
    {
        return (STATUS_FAIL);
    }
}

bool CS_bypassXT1WithTimeout(uint16_t timeout)
{
    //Enable HF/LF mode
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~XTS;

    //Switch OFF XT1 oscillator  and enable bypass
    HWREG16(CS_BASE + OFS_CSCTL6) |= (XT1BYPASS + XT1AUTOOFF);

    do
    {
        //Clear OSC flaut Flags fault flags
        HWREG8(CS_BASE + OFS_CSCTL7) &= ~(XT1OFFG);

        // Clear the global fault flag. In case the XT1 caused the global fault
        // flag to get set this will clear the global error condition. If any
        // error condition persists, global flag will get again.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }
    while((HWREG8(CS_BASE + OFS_CSCTL7) & (XT1OFFG)) && --timeout);

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
    //Switch off XT1 oscillator
    HWREG16(CS_BASE + OFS_CSCTL6) |= XT1AUTOOFF;
}

void CS_initFLLSettle(uint16_t fsystem,
                      uint16_t ratio)
{
    volatile uint16_t x = ratio * 32;

    CS_initFLL(fsystem, ratio);

    while(x--)
    {
        __delay_cycles(30);
    }
}

void CS_initFLL(uint16_t fsystem,
                uint16_t ratio)
{
    uint16_t dco_FLLN, dco_FLLD = FLLD__1;

    //Save actual state of FLL loop control, then disable it. This is needed to
    //prevent the FLL from acting as we are making fundamental modifications to
    //the clock setup.
    uint16_t srRegisterState = __get_SR_register() & SCG0;

    //Have at least a divider of 2
    dco_FLLN = ratio;

    //Max DCODIV freq <=16MHz
    assert (fsystem <= 16000);
    assert (dco_FLLN <= 488);

    // Disable FLL
    __bis_SR_register(SCG0);

    //Set DCO to lowest Tap
    HWREG16(CS_BASE + OFS_CSCTL0) &= ~(DCO8 +
                                       DCO7 +
                                       DCO6 +
                                       DCO5 +
                                       DCO4 +
                                       DCO3 +
                                       DCO2 +
                                       DCO1 +
                                       DCO0
                                       );

    //Reset FN bits
    HWREG16(CS_BASE + OFS_CSCTL2) &= ~(0x03FF);
    HWREG16(CS_BASE + OFS_CSCTL2) = dco_FLLD | (dco_FLLN - 1);

    HWREG8(CS_BASE + OFS_CSCTL1) &= ~DCORSEL_7;
    if(fsystem <= 1000)             //fsystem <= 1MHz
    {
        HWREG8(CS_BASE + OFS_CSCTL1) |= DCORSEL_0;
    }
    else if(fsystem <= 2000)         //1MHz < fsystem <= 2MHz
    {
        HWREG8(CS_BASE + OFS_CSCTL1) |= DCORSEL_1;
    }
    else if(fsystem <= 4000)         //2MHz < fsystem <=  4MHz
    {
        HWREG8(CS_BASE + OFS_CSCTL1) |= DCORSEL_2;
    }
    else if(fsystem <= 8000)         //4MHz  < fsystem <=  8MHz
    {
        HWREG8(CS_BASE + OFS_CSCTL1) |= DCORSEL_3;
    }
    else if(fsystem <= 12000)        //8MHz    < fsystem <= 12MHz
    {
        HWREG8(CS_BASE + OFS_CSCTL1) |= DCORSEL_4;
    }
    else if(fsystem <= 16000)        //12MHz   < fsystem <=  16MHz
    {
        HWREG8(CS_BASE + OFS_CSCTL1) |= DCORSEL_5;
    }
    else
    {
        //exceeds 16MHz, not supported
        assert(0);
    }

    // Re-enable FLL
    __bic_SR_register(SCG0);

    while(HWREG8(CS_BASE + OFS_CSCTL7_L) & DCOFFG)
    {
        //Clear OSC flaut Flags
        HWREG8(CS_BASE + OFS_CSCTL7_L) &= ~(DCOFFG);

        //Clear OFIFG fault flag
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;
    }

    // Restore previous SCG0
    __bis_SR_register(srRegisterState);
}

void CS_enableClockRequest(uint8_t selectClock)
{
    HWREG8(CS_BASE + OFS_CSCTL8) |= selectClock;
}

void CS_disableClockRequest(uint8_t selectClock)
{
    HWREG8(CS_BASE + OFS_CSCTL8) &= ~selectClock;
}

uint8_t CS_getFaultFlagStatus(uint8_t mask)
{
    return (HWREG8(CS_BASE + OFS_CSCTL7) & mask);
}

void CS_clearFaultFlag(uint8_t mask)
{
    HWREG8(CS_BASE + OFS_CSCTL7) &= ~mask;
}

uint32_t CS_getACLK(void)
{
    //Find ACLK source
    uint16_t ACLKSource = (HWREG16(CS_BASE + OFS_CSCTL4) & SELA);

    ACLKSource = ACLKSource >> 8;

    if(ACLKSource == 0x0)
    {
        ACLKSource = SELMS__XT1CLK;
    }
    else
    {
        ACLKSource = SELMS__REFOCLK;
    }

    uint16_t ACLKSourceDivider = 1;
    ACLKSourceDivider = ACLKSourceDivider >> 8;

    return (privateCSComputeCLKFrequency(
                ACLKSource,
                ACLKSourceDivider)
            );
}

uint32_t CS_getSMCLK(void)
{
    uint16_t SMCLKSource = HWREG8(CS_BASE + OFS_CSCTL4_L) & SELMS_7;

    uint16_t SMCLKSourceDivider =
        HWREG16(CS_BASE + OFS_CSCTL5) & DIVS_3;
    SMCLKSourceDivider = SMCLKSourceDivider >> 4;

    return (privateCSComputeCLKFrequency(
                SMCLKSource,
                SMCLKSourceDivider)
            );
}

uint32_t CS_getMCLK(void)
{
    //Find AMCLK source
    uint16_t MCLKSource = (HWREG16(CS_BASE + OFS_CSCTL4) & SELMS_7);

    uint16_t MCLKSourceDivider = HWREG16(CS_BASE + OFS_CSCTL5) & DIVM_7;

    return (privateCSComputeCLKFrequency(
                MCLKSource,
                MCLKSourceDivider)
            );
}

uint16_t CS_clearAllOscFlagsWithTimeout(uint16_t timeout){
    do
    {
        // Clear all osc fault flags
        HWREG8(CS_BASE + OFS_CSCTL7) &= ~(DCOFFG + XT1OFFG);

        // Clear the global osc fault flag.
        HWREG8(SFR_BASE + OFS_SFRIFG1) &= ~OFIFG;

        // Check XT1 fault flags
    }
    while((HWREG8(SFR_BASE + OFS_SFRIFG1) & OFIFG) && --timeout);

    return (HWREG8(CS_BASE + OFS_CSCTL7) & (DCOFFG + XT1OFFG));
}

void CS_enableXT1AutomaticGainControl(void)
{
    HWREG16(CS_BASE + OFS_CSCTL6) &= ~XT1AGCOFF;
}

void CS_disableXT1AutomaticGainControl(void)
{
    HWREG16(CS_BASE + OFS_CSCTL6) |= XT1AGCOFF;
}

void CS_enableFLLUnlock(void)
{
    HWREG16(CS_BASE + OFS_CSCTL7) |= FLLULPUC;
}

void CS_disableFLLUnlock(void)
{
    HWREG16(CS_BASE + OFS_CSCTL7) &= ~FLLULPUC;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for cs_api
//! @}
//
//*****************************************************************************

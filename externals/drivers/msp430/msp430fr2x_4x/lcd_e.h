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
// lcd_e.h - Driver for the LCD_E Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_LCD_E_H__
#define __MSP430WARE_LCD_E_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_LCD_E__

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

#include "inc/hw_memmap.h"
//*****************************************************************************
//
//! \brief Used in the LCD_E_init() function as the initParams parameter.
//
//*****************************************************************************
typedef struct LCD_E_initParam
{
    //! Selects the clock that will be used by the LCD_E.
    //! \n Valid values are:
    //! - \b LCD_E_CLOCKSOURCE_XTCLK [Default]
    //! - \b LCD_E_CLOCKSOURCE_ACLK [Default]
    //! - \b LCD_E_CLOCKSOURCE_VLOCLK
    uint16_t clockSource;
    //! Selects the divider for LCD_E frequency.
    //! \n Valid values are:
    //! - \b LCD_E_CLOCKDIVIDER_1 [Default]
    //! - \b LCD_E_CLOCKDIVIDER_2
    //! - \b LCD_E_CLOCKDIVIDER_3
    //! - \b LCD_E_CLOCKDIVIDER_4
    //! - \b LCD_E_CLOCKDIVIDER_5
    //! - \b LCD_E_CLOCKDIVIDER_6
    //! - \b LCD_E_CLOCKDIVIDER_7
    //! - \b LCD_E_CLOCKDIVIDER_8
    //! - \b LCD_E_CLOCKDIVIDER_9
    //! - \b LCD_E_CLOCKDIVIDER_10
    //! - \b LCD_E_CLOCKDIVIDER_11
    //! - \b LCD_E_CLOCKDIVIDER_12
    //! - \b LCD_E_CLOCKDIVIDER_13
    //! - \b LCD_E_CLOCKDIVIDER_14
    //! - \b LCD_E_CLOCKDIVIDER_15
    //! - \b LCD_E_CLOCKDIVIDER_16
    //! - \b LCD_E_CLOCKDIVIDER_17
    //! - \b LCD_E_CLOCKDIVIDER_18
    //! - \b LCD_E_CLOCKDIVIDER_19
    //! - \b LCD_E_CLOCKDIVIDER_20
    //! - \b LCD_E_CLOCKDIVIDER_21
    //! - \b LCD_E_CLOCKDIVIDER_22
    //! - \b LCD_E_CLOCKDIVIDER_23
    //! - \b LCD_E_CLOCKDIVIDER_24
    //! - \b LCD_E_CLOCKDIVIDER_25
    //! - \b LCD_E_CLOCKDIVIDER_26
    //! - \b LCD_E_CLOCKDIVIDER_27
    //! - \b LCD_E_CLOCKDIVIDER_28
    //! - \b LCD_E_CLOCKDIVIDER_29
    //! - \b LCD_E_CLOCKDIVIDER_30
    //! - \b LCD_E_CLOCKDIVIDER_31
    //! - \b LCD_E_CLOCKDIVIDER_32
    uint16_t clockDivider;
    //! Selects LCD_E mux rate.
    //! \n Valid values are:
    //! - \b LCD_E_STATIC [Default]
    //! - \b LCD_E_2_MUX
    //! - \b LCD_E_3_MUX
    //! - \b LCD_E_4_MUX
    //! - \b LCD_E_5_MUX
    //! - \b LCD_E_6_MUX
    //! - \b LCD_E_7_MUX
    //! - \b LCD_E_8_MUX
    uint16_t muxRate;
    //! Selects LCD waveform mode.
    //! \n Valid values are:
    //! - \b LCD_E_STANDARD_WAVEFORMS [Default]
    //! - \b LCD_E_LOW_POWER_WAVEFORMS
    uint16_t waveforms;
    //! Sets LCD segment on/off.
    //! \n Valid values are:
    //! - \b LCD_E_SEGMENTS_DISABLED [Default]
    //! - \b LCD_E_SEGMENTS_ENABLED
    uint16_t segments;
} LCD_E_initParam;

extern const LCD_E_initParam LCD_E_INIT_PARAM;

//*****************************************************************************
//
// The following are values that can be passed to the initParams parameter for
// functions: LCD_E_init().
//
//*****************************************************************************
#define LCD_E_CLOCKSOURCE_XTCLK                                     (LCDSSEL_0)
#define LCD_E_CLOCKSOURCE_ACLK                                      (LCDSSEL_1)
#define LCD_E_CLOCKSOURCE_VLOCLK                                    (LCDSSEL_2)

//*****************************************************************************
//
// The following are values that can be passed to the initParams parameter for
// functions: LCD_E_init().
//
//*****************************************************************************
#define LCD_E_CLOCKDIVIDER_1                                         (LCDDIV_0)
#define LCD_E_CLOCKDIVIDER_2                                         (LCDDIV_1)
#define LCD_E_CLOCKDIVIDER_3                                         (LCDDIV_2)
#define LCD_E_CLOCKDIVIDER_4                                         (LCDDIV_3)
#define LCD_E_CLOCKDIVIDER_5                                         (LCDDIV_4)
#define LCD_E_CLOCKDIVIDER_6                                         (LCDDIV_5)
#define LCD_E_CLOCKDIVIDER_7                                         (LCDDIV_6)
#define LCD_E_CLOCKDIVIDER_8                                         (LCDDIV_7)
#define LCD_E_CLOCKDIVIDER_9                                         (LCDDIV_8)
#define LCD_E_CLOCKDIVIDER_10                                        (LCDDIV_9)
#define LCD_E_CLOCKDIVIDER_11                                       (LCDDIV_10)
#define LCD_E_CLOCKDIVIDER_12                                       (LCDDIV_11)
#define LCD_E_CLOCKDIVIDER_13                                       (LCDDIV_12)
#define LCD_E_CLOCKDIVIDER_14                                       (LCDDIV_13)
#define LCD_E_CLOCKDIVIDER_15                                       (LCDDIV_14)
#define LCD_E_CLOCKDIVIDER_16                                       (LCDDIV_15)
#define LCD_E_CLOCKDIVIDER_17                                       (LCDDIV_16)
#define LCD_E_CLOCKDIVIDER_18                                       (LCDDIV_17)
#define LCD_E_CLOCKDIVIDER_19                                       (LCDDIV_18)
#define LCD_E_CLOCKDIVIDER_20                                       (LCDDIV_19)
#define LCD_E_CLOCKDIVIDER_21                                       (LCDDIV_20)
#define LCD_E_CLOCKDIVIDER_22                                       (LCDDIV_21)
#define LCD_E_CLOCKDIVIDER_23                                       (LCDDIV_22)
#define LCD_E_CLOCKDIVIDER_24                                       (LCDDIV_23)
#define LCD_E_CLOCKDIVIDER_25                                       (LCDDIV_24)
#define LCD_E_CLOCKDIVIDER_26                                       (LCDDIV_25)
#define LCD_E_CLOCKDIVIDER_27                                       (LCDDIV_26)
#define LCD_E_CLOCKDIVIDER_28                                       (LCDDIV_27)
#define LCD_E_CLOCKDIVIDER_29                                       (LCDDIV_28)
#define LCD_E_CLOCKDIVIDER_30                                       (LCDDIV_29)
#define LCD_E_CLOCKDIVIDER_31                                       (LCDDIV_30)
#define LCD_E_CLOCKDIVIDER_32                                       (LCDDIV_31)

//*****************************************************************************
//
// The following are values that can be passed to the initParams parameter for
// functions: LCD_E_init().
//
//*****************************************************************************
#define LCD_E_STATIC                                                      (0x0)
#define LCD_E_2_MUX                                                    (LCDMX0)
#define LCD_E_3_MUX                                                    (LCDMX1)
#define LCD_E_4_MUX                                           (LCDMX1 | LCDMX0)
#define LCD_E_5_MUX                                                    (LCDMX2)
#define LCD_E_6_MUX                                           (LCDMX2 | LCDMX0)
#define LCD_E_7_MUX                                           (LCDMX2 | LCDMX1)
#define LCD_E_8_MUX                                  (LCDMX2 | LCDMX1 | LCDMX0)

//*****************************************************************************
//
// The following are values that can be passed to the initParams parameter for
// functions: LCD_E_init().
//
//*****************************************************************************
#define LCD_E_STANDARD_WAVEFORMS                                          (0x0)
#define LCD_E_LOW_POWER_WAVEFORMS                                       (LCDLP)

//*****************************************************************************
//
// The following are values that can be passed to the initParams parameter for
// functions: LCD_E_init().
//
//*****************************************************************************
#define LCD_E_SEGMENTS_DISABLED                                           (0x0)
#define LCD_E_SEGMENTS_ENABLED                                         (LCDSON)

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: LCD_E_clearInterrupt(), LCD_E_getInterruptStatus(),
// LCD_E_enableInterrupt(), and LCD_E_disableInterrupt() as well as returned by
// the LCD_E_getInterruptStatus() function.
//
//*****************************************************************************
#define LCD_E_BLINKING_SEGMENTS_ON_INTERRUPT                       (LCDBLKONIE)
#define LCD_E_BLINKING_SEGMENTS_OFF_INTERRUPT                     (LCDBLKOFFIE)
#define LCD_E_FRAME_INTERRUPT                                        (LCDFRMIE)

//*****************************************************************************
//
// The following are values that can be passed to the displayMemory parameter
// for functions: LCD_E_selectDisplayMemory().
//
//*****************************************************************************
#define LCD_E_DISPLAYSOURCE_MEMORY                                        (0x0)
#define LCD_E_DISPLAYSOURCE_BLINKINGMEMORY                            (LCDDISP)

//*****************************************************************************
//
// The following are values that can be passed to the clockPrescalar parameter
// for functions: LCD_E_setBlinkingControl().
//
//*****************************************************************************
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_4                                (0x0)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_8                         (LCDBLKPRE0)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_16                        (LCDBLKPRE1)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_32           (LCDBLKPRE1 | LCDBLKPRE0)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64                        (LCDBLKPRE2)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_128          (LCDBLKPRE2 | LCDBLKPRE0)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_256          (LCDBLKPRE2 | LCDBLKPRE1)
#define LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_512                                  \
    (LCDBLKPRE2 | LCDBLKPRE1 | LCDBLKPRE0)

//*****************************************************************************
//
// The following are values that can be passed to the mode parameter for
// functions: LCD_E_setBlinkingControl().
//
//*****************************************************************************
#define LCD_E_BLINK_MODE_DISABLED                                 (LCDBLKMOD_0)
#define LCD_E_BLINK_MODE_INDIVIDUAL_SEGMENTS                      (LCDBLKMOD_1)
#define LCD_E_BLINK_MODE_ALL_SEGMENTS                             (LCDBLKMOD_2)
#define LCD_E_BLINK_MODE_SWITCHING_BETWEEN_DISPLAY_CONTENTS       (LCDBLKMOD_3)

//*****************************************************************************
//
// The following are values that can be passed to the r13Source parameter for
// functions: LCD_E_setVLCDSource().
//
//*****************************************************************************
#define LCD_E_NON_INTERNAL_REFERENCE_VOLTAGE                              (0x0)
#define LCD_E_INTERNAL_REFERENCE_VOLTAGE                             (LCDREFEN)

//*****************************************************************************
//
// The following are values that can be passed to the r33Source parameter for
// functions: LCD_E_setVLCDSource().
//
//*****************************************************************************
#define LCD_E_EXTERNAL_SUPPLY_VOLTAGE                                     (0x0)
#define LCD_E_INTERNAL_SUPPLY_VOLTAGE                               (LCDSELVDD)

//*****************************************************************************
//
// The following are values that can be passed to the voltage parameter for
// functions: LCD_E_setVLCDVoltage().
//
//*****************************************************************************
#define LCD_E_REFERENCE_VOLTAGE_2_60V                                     (0x0)
#define LCD_E_REFERENCE_VOLTAGE_2_66V                                   (VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_2_72V                                   (VLCD1)
#define LCD_E_REFERENCE_VOLTAGE_2_78V                           (VLCD1 | VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_2_84V                                   (VLCD2)
#define LCD_E_REFERENCE_VOLTAGE_2_90V                           (VLCD2 | VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_2_96V                           (VLCD2 | VLCD1)
#define LCD_E_REFERENCE_VOLTAGE_3_02V                   (VLCD2 | VLCD1 | VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_3_08V                                   (VLCD3)
#define LCD_E_REFERENCE_VOLTAGE_3_14V                           (VLCD3 | VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_3_20V                           (VLCD3 | VLCD1)
#define LCD_E_REFERENCE_VOLTAGE_3_26V                   (VLCD3 | VLCD1 | VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_3_32V                           (VLCD3 | VLCD2)
#define LCD_E_REFERENCE_VOLTAGE_3_38V                   (VLCD3 | VLCD2 | VLCD0)
#define LCD_E_REFERENCE_VOLTAGE_3_44V                   (VLCD3 | VLCD2 | VLCD1)
#define LCD_E_REFERENCE_VOLTAGE_3_50V           (VLCD3 | VLCD2 | VLCD1 | VLCD0)

//*****************************************************************************
//
// The following are values that can be passed to the startPin parameter for
// functions: LCD_E_setPinAsLCDFunctionEx(); the endPin parameter for
// functions: LCD_E_setPinAsLCDFunctionEx(); the pin parameter for functions:
// LCD_E_setPinAsLCDFunction(), LCD_E_setPinAsPortFunction(),
// LCD_E_setPinAsCOM(), and LCD_E_setPinAsSEG().
//
//*****************************************************************************
#define LCD_E_SEGMENT_LINE_0                                                (0)
#define LCD_E_SEGMENT_LINE_1                                                (1)
#define LCD_E_SEGMENT_LINE_2                                                (2)
#define LCD_E_SEGMENT_LINE_3                                                (3)
#define LCD_E_SEGMENT_LINE_4                                                (4)
#define LCD_E_SEGMENT_LINE_5                                                (5)
#define LCD_E_SEGMENT_LINE_6                                                (6)
#define LCD_E_SEGMENT_LINE_7                                                (7)
#define LCD_E_SEGMENT_LINE_8                                                (8)
#define LCD_E_SEGMENT_LINE_9                                                (9)
#define LCD_E_SEGMENT_LINE_10                                              (10)
#define LCD_E_SEGMENT_LINE_11                                              (11)
#define LCD_E_SEGMENT_LINE_12                                              (12)
#define LCD_E_SEGMENT_LINE_13                                              (13)
#define LCD_E_SEGMENT_LINE_14                                              (14)
#define LCD_E_SEGMENT_LINE_15                                              (15)
#define LCD_E_SEGMENT_LINE_16                                              (16)
#define LCD_E_SEGMENT_LINE_17                                              (17)
#define LCD_E_SEGMENT_LINE_18                                              (18)
#define LCD_E_SEGMENT_LINE_19                                              (19)
#define LCD_E_SEGMENT_LINE_20                                              (20)
#define LCD_E_SEGMENT_LINE_21                                              (21)
#define LCD_E_SEGMENT_LINE_22                                              (22)
#define LCD_E_SEGMENT_LINE_23                                              (23)
#define LCD_E_SEGMENT_LINE_24                                              (24)
#define LCD_E_SEGMENT_LINE_25                                              (25)
#define LCD_E_SEGMENT_LINE_26                                              (26)
#define LCD_E_SEGMENT_LINE_27                                              (27)
#define LCD_E_SEGMENT_LINE_28                                              (28)
#define LCD_E_SEGMENT_LINE_29                                              (29)
#define LCD_E_SEGMENT_LINE_30                                              (30)
#define LCD_E_SEGMENT_LINE_31                                              (31)
#define LCD_E_SEGMENT_LINE_32                                              (32)
#define LCD_E_SEGMENT_LINE_33                                              (33)
#define LCD_E_SEGMENT_LINE_34                                              (34)
#define LCD_E_SEGMENT_LINE_35                                              (35)
#define LCD_E_SEGMENT_LINE_36                                              (36)
#define LCD_E_SEGMENT_LINE_37                                              (37)
#define LCD_E_SEGMENT_LINE_38                                              (38)
#define LCD_E_SEGMENT_LINE_39                                              (39)
#define LCD_E_SEGMENT_LINE_40                                              (40)
#define LCD_E_SEGMENT_LINE_41                                              (41)
#define LCD_E_SEGMENT_LINE_42                                              (42)
#define LCD_E_SEGMENT_LINE_43                                              (43)
#define LCD_E_SEGMENT_LINE_44                                              (44)
#define LCD_E_SEGMENT_LINE_45                                              (45)
#define LCD_E_SEGMENT_LINE_46                                              (46)
#define LCD_E_SEGMENT_LINE_47                                              (47)

//*****************************************************************************
//
// The following are values that can be passed to the memory parameter for
// functions: LCD_E_setMemory(), LCD_E_updateMemory(), LCD_E_toggleMemory(),
// LCD_E_clearMemory(), LCD_E_setBlinkingMemory(),
// LCD_E_updateBlinkingMemory(), LCD_E_toggleBlinkingMemory(), and
// LCD_E_clearBlinkingMemory().
//
//*****************************************************************************
#define LCD_E_MEMORY_BLINKINGMEMORY_0                                       (0)
#define LCD_E_MEMORY_BLINKINGMEMORY_1                                       (1)
#define LCD_E_MEMORY_BLINKINGMEMORY_2                                       (2)
#define LCD_E_MEMORY_BLINKINGMEMORY_3                                       (3)
#define LCD_E_MEMORY_BLINKINGMEMORY_4                                       (4)
#define LCD_E_MEMORY_BLINKINGMEMORY_5                                       (5)
#define LCD_E_MEMORY_BLINKINGMEMORY_6                                       (6)
#define LCD_E_MEMORY_BLINKINGMEMORY_7                                       (7)
#define LCD_E_MEMORY_BLINKINGMEMORY_8                                       (8)
#define LCD_E_MEMORY_BLINKINGMEMORY_9                                       (9)
#define LCD_E_MEMORY_BLINKINGMEMORY_10                                     (10)
#define LCD_E_MEMORY_BLINKINGMEMORY_11                                     (11)
#define LCD_E_MEMORY_BLINKINGMEMORY_12                                     (12)
#define LCD_E_MEMORY_BLINKINGMEMORY_13                                     (13)
#define LCD_E_MEMORY_BLINKINGMEMORY_14                                     (14)
#define LCD_E_MEMORY_BLINKINGMEMORY_15                                     (15)
#define LCD_E_MEMORY_BLINKINGMEMORY_16                                     (16)
#define LCD_E_MEMORY_BLINKINGMEMORY_17                                     (17)
#define LCD_E_MEMORY_BLINKINGMEMORY_18                                     (18)
#define LCD_E_MEMORY_BLINKINGMEMORY_19                                     (19)
#define LCD_E_MEMORY_BLINKINGMEMORY_20                                     (20)
#define LCD_E_MEMORY_BLINKINGMEMORY_21                                     (21)
#define LCD_E_MEMORY_BLINKINGMEMORY_22                                     (22)
#define LCD_E_MEMORY_BLINKINGMEMORY_23                                     (23)
#define LCD_E_MEMORY_BLINKINGMEMORY_24                                     (24)
#define LCD_E_MEMORY_BLINKINGMEMORY_25                                     (25)
#define LCD_E_MEMORY_BLINKINGMEMORY_26                                     (26)
#define LCD_E_MEMORY_BLINKINGMEMORY_27                                     (27)
#define LCD_E_MEMORY_BLINKINGMEMORY_28                                     (28)
#define LCD_E_MEMORY_BLINKINGMEMORY_29                                     (29)
#define LCD_E_MEMORY_BLINKINGMEMORY_30                                     (30)
#define LCD_E_MEMORY_BLINKINGMEMORY_31                                     (31)
#define LCD_E_MEMORY_BLINKINGMEMORY_32                                     (32)
#define LCD_E_MEMORY_BLINKINGMEMORY_33                                     (33)
#define LCD_E_MEMORY_BLINKINGMEMORY_34                                     (34)
#define LCD_E_MEMORY_BLINKINGMEMORY_35                                     (35)
#define LCD_E_MEMORY_BLINKINGMEMORY_36                                     (36)
#define LCD_E_MEMORY_BLINKINGMEMORY_37                                     (37)
#define LCD_E_MEMORY_BLINKINGMEMORY_38                                     (38)
#define LCD_E_MEMORY_BLINKINGMEMORY_39                                     (39)

//*****************************************************************************
//
// The following are values that can be passed to the com parameter for
// functions: LCD_E_setPinAsCOM().
//
//*****************************************************************************
#define LCD_E_MEMORY_COM0                                                (0x01)
#define LCD_E_MEMORY_COM1                                                (0x02)
#define LCD_E_MEMORY_COM2                                                (0x04)
#define LCD_E_MEMORY_COM3                                                (0x08)
#define LCD_E_MEMORY_COM4                                                (0x10)
#define LCD_E_MEMORY_COM5                                                (0x20)
#define LCD_E_MEMORY_COM6                                                (0x40)
#define LCD_E_MEMORY_COM7                                                (0x80)

//*****************************************************************************
//
// The following are values that can be passed to the freq parameter for
// functions: LCD_E_setChargePumpFreq().
//
//*****************************************************************************
#define LCD_E_CHARGEPUMP_FREQ_1                                        (0x0000)
#define LCD_E_CHARGEPUMP_FREQ_2                                    (LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_3                                    (LCDCPFSEL1)
#define LCD_E_CHARGEPUMP_FREQ_4                       (LCDCPFSEL1 | LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_5                                    (LCDCPFSEL2)
#define LCD_E_CHARGEPUMP_FREQ_6                       (LCDCPFSEL2 | LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_7                       (LCDCPFSEL2 | LCDCPFSEL1)
#define LCD_E_CHARGEPUMP_FREQ_8          (LCDCPFSEL2 | LCDCPFSEL1 | LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_9                                    (LCDCPFSEL3)
#define LCD_E_CHARGEPUMP_FREQ_10                      (LCDCPFSEL3 | LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_11                      (LCDCPFSEL3 | LCDCPFSEL1)
#define LCD_E_CHARGEPUMP_FREQ_12         (LCDCPFSEL3 | LCDCPFSEL1 | LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_13                      (LCDCPFSEL3 | LCDCPFSEL2)
#define LCD_E_CHARGEPUMP_FREQ_14         (LCDCPFSEL3 | LCDCPFSEL2 | LCDCPFSEL0)
#define LCD_E_CHARGEPUMP_FREQ_15         (LCDCPFSEL3 | LCDCPFSEL2 | LCDCPFSEL1)
#define LCD_E_CHARGEPUMP_FREQ_16                                              \
    (LCDCPFSEL3 | LCDCPFSEL2 | LCDCPFSEL1 | LCDCPFSEL0)

//*****************************************************************************
//
// The following are values that can be passed to the mode parameter for
// functions: LCD_E_setReferenceMode().
//
//*****************************************************************************
#define LCD_E_REFERENCE_MODE_STATIC                                       (0x0)
#define LCD_E_REFERENCE_MODE_SWITCHED                              (LCDREFMODE)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the LCD_E Module.
//!
//! This function initializes the LCD_E but without turning on. It bascially
//! setup the clock source, clock divider, mux rate, low-power waveform and
//! segments on/off. After calling this function, user can enable/disable
//! charge pump, internal reference voltage, or pin SEG/COM configurations.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param initParams is the pointer to LCD_InitParam structure. See the
//!        following parameters for each field.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_init(uint16_t baseAddress,
                       LCD_E_initParam *initParams);

//*****************************************************************************
//
//! \brief Turns on the LCD_E module.
//!
//! This function turns the LCD_E on.
//!
//! \param baseAddress is the base address of the LCD_E module.
//!
//! Modified bits are \b LCDPCTL of \b SYSCFG2 register; bits \b LCDON of \b
//! LCDCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_on(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Turns the LCD_E off.
//!
//! This function turns the LCD_E off.
//!
//! \param baseAddress is the base address of the LCD_E module.
//!
//! Modified bits are \b LCDPCTL of \b SYSCFG2 register; bits \b LCDON of \b
//! LCDCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_off(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears the LCD_E selected interrupt flags.
//!
//! This function clears the specified interrupt flags.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param mask is the masked interrupt flag to be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b LCD_E_BLINKING_SEGMENTS_ON_INTERRUPT
//!        - \b LCD_E_BLINKING_SEGMENTS_OFF_INTERRUPT
//!        - \b LCD_E_FRAME_INTERRUPT
//!        \n Modified bits are \b LCDBLKONIFG, \b LCDBLKOFFIFG and \b
//!        LCDFRMIFG of \b LCDCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_clearInterrupt(uint16_t baseAddress,
                                 uint16_t mask);

//*****************************************************************************
//
//! \brief Returns the status of the selected interrupt flags.
//!
//! This function returns the status of the selected interrupt flags.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param mask is the masked interrupt flags.
//!        Mask value is the logical OR of any of the following:
//!        - \b LCD_E_BLINKING_SEGMENTS_ON_INTERRUPT
//!        - \b LCD_E_BLINKING_SEGMENTS_OFF_INTERRUPT
//!        - \b LCD_E_FRAME_INTERRUPT
//!
//! \return The current interrupt flag status for the corresponding mask.
//!         Return Logical OR of any of the following:
//!         - \b LCD_E_BLINKING_SEGMENTS_ON_INTERRUPT
//!         - \b LCD_E_BLINKING_SEGMENTS_OFF_INTERRUPT
//!         - \b LCD_E_FRAME_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint16_t LCD_E_getInterruptStatus(uint16_t baseAddress,
                                         uint16_t mask);

//*****************************************************************************
//
//! \brief Enables selected LCD_E interrupt sources.
//!
//! This function enables the indicated LCD_E interrupt sources.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param mask is the interrupts to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b LCD_E_BLINKING_SEGMENTS_ON_INTERRUPT
//!        - \b LCD_E_BLINKING_SEGMENTS_OFF_INTERRUPT
//!        - \b LCD_E_FRAME_INTERRUPT
//!        \n Modified bits are \b LCDBLKONIE, \b LCDBLKOFFIE and \b LCDFRMIE
//!        of \b LCDCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_enableInterrupt(uint16_t baseAddress,
                                  uint16_t mask);

//*****************************************************************************
//
//! \brief Disables selected LCD_E interrupt sources.
//!
//! This function disables the indicated LCD_E interrupt sources.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param mask is the interrupts to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b LCD_E_BLINKING_SEGMENTS_ON_INTERRUPT
//!        - \b LCD_E_BLINKING_SEGMENTS_OFF_INTERRUPT
//!        - \b LCD_E_FRAME_INTERRUPT
//!        \n Modified bits are \b LCDBLKONIE, \b LCDBLKOFFIE and \b LCDFRMIE
//!        of \b LCDCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_disableInterrupt(uint16_t baseAddress,
                                   uint16_t mask);

//*****************************************************************************
//
//! \brief Clears all LCD_E memory registers.
//!
//! This function clears all LCD_E memory registers.
//!
//! \param baseAddress is the base address of the LCD_E module.
//!
//! Modified bits are \b LCDCLRM of \b LCDMEMCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_clearAllMemory(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears all LCD_E blinking memory registers.
//!
//! This function clears all LCD_E blinking memory registers.
//!
//! \param baseAddress is the base address of the LCD_E module.
//!
//! Modified bits are \b LCDCLRBM of \b LCDMEMCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_clearAllBlinkingMemory(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Selects display memory.
//!
//! This function selects display memory either from memory or blinking memory.
//! Please note if the blinking mode is selected as
//! LCD_E_BLINKMODE_INDIVIDUALSEGMENTS or LCD_E_BLINKMODE_ALLSEGMENTS or mux
//! rate >=5, display memory can not be changed. If
//! LCD_E_BLINKMODE_SWITCHDISPLAYCONTENTS is selected, display memory bit
//! reflects current displayed memory.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param displayMemory is the desired displayed memory.
//!        Valid values are:
//!        - \b LCD_E_DISPLAYSOURCE_MEMORY [Default]
//!        - \b LCD_E_DISPLAYSOURCE_BLINKINGMEMORY
//!        \n Modified bits are \b LCDDISP of \b LCDMEMCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_selectDisplayMemory(uint16_t baseAddress,
                                      uint16_t displayMemory);

//*****************************************************************************
//
//! \brief Sets the blinking control register.
//!
//! This function sets the blink control related parameter, including blink
//! clock frequency prescalar and blink mode.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param clockPrescalar is the clock pre-scalar for blinking frequency.
//!        Valid values are:
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_4 [Default]
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_8
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_16
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_32
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_128
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_256
//!        - \b LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_512
//!        \n Modified bits are \b LCDBLKPREx of \b LCDBLKCTL register.
//! \param mode is the select for blinking mode.
//!        Valid values are:
//!        - \b LCD_E_BLINK_MODE_DISABLED [Default]
//!        - \b LCD_E_BLINK_MODE_INDIVIDUAL_SEGMENTS
//!        - \b LCD_E_BLINK_MODE_ALL_SEGMENTS
//!        - \b LCD_E_BLINK_MODE_SWITCHING_BETWEEN_DISPLAY_CONTENTS
//!        \n Modified bits are \b LCDBLKMODx of \b LCDBLKCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setBlinkingControl(uint16_t baseAddress,
                                     uint16_t clockPrescalar,
                                     uint16_t mode);

//*****************************************************************************
//
//! \brief Enables the charge pump.
//!
//! This function enables the charge pump and config the charge pump frequency.
//!
//! \param baseAddress is the base address of the LCD_E module.
//!
//! Modified bits are \b LCDCPEN of \b LCDVCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_enableChargePump(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the charge pump.
//!
//! This function disables the charge pump.
//!
//! \param baseAddress is the base address of the LCD_E module.
//!
//! Modified bits are \b LCDCPEN of \b LCDVCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_disableChargePump(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets the charge pump frequency.
//!
//! This function sets the charge pump frequency. It takes effect once charge
//! pump is enabled by LCD_E_enableChargePump().
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param freq is the charge pump frequency to select.
//!        Valid values are:
//!        - \b LCD_E_CHARGEPUMP_FREQ_1 [Default]
//!        - \b LCD_E_CHARGEPUMP_FREQ_2
//!        - \b LCD_E_CHARGEPUMP_FREQ_3
//!        - \b LCD_E_CHARGEPUMP_FREQ_4
//!        - \b LCD_E_CHARGEPUMP_FREQ_5
//!        - \b LCD_E_CHARGEPUMP_FREQ_6
//!        - \b LCD_E_CHARGEPUMP_FREQ_7
//!        - \b LCD_E_CHARGEPUMP_FREQ_8
//!        - \b LCD_E_CHARGEPUMP_FREQ_9
//!        - \b LCD_E_CHARGEPUMP_FREQ_10
//!        - \b LCD_E_CHARGEPUMP_FREQ_11
//!        - \b LCD_E_CHARGEPUMP_FREQ_12
//!        - \b LCD_E_CHARGEPUMP_FREQ_13
//!        - \b LCD_E_CHARGEPUMP_FREQ_14
//!        - \b LCD_E_CHARGEPUMP_FREQ_15
//!        - \b LCD_E_CHARGEPUMP_FREQ_16
//!        \n Modified bits are \b LCDCPFSELx of \b LCDVCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setChargePumpFreq(uint16_t baseAddress,
                                    uint16_t freq);

//*****************************************************************************
//
//! \brief Sets LCD_E voltage source.
//!
//! Two voltage sources are set in this function: R13 and R33. For the R13, the
//! voltage source can be either internal reference voltage or non internal
//! reference voltage (Vext or Vdd). For the R33, it can be external supply
//! voltage (Vext) or internal supply voltage (Vdd).
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param r13Source is the voltage source for R13.
//!        Valid values are:
//!        - \b LCD_E_NON_INTERNAL_REFERENCE_VOLTAGE [Default]
//!        - \b LCD_E_INTERNAL_REFERENCE_VOLTAGE
//!        \n Modified bits are \b LCDREFEN of \b LCDVCTL register.
//! \param r33Source is the voltage source for R33.
//!        Valid values are:
//!        - \b LCD_E_EXTERNAL_SUPPLY_VOLTAGE  [Default]
//!        - \b LCD_E_INTERNAL_SUPPLY_VOLTAGE
//!        \n Modified bits are \b LCDSELVDD of \b LCDVCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setVLCDSource(uint16_t baseAddress,
                                uint16_t r13Source,
                                uint16_t r33Source);

//*****************************************************************************
//
//! \brief Sets LCD_E internal voltage for R13.
//!
//! This function sets the internal voltage for R13. The voltage is only
//! valuable when R13 voltage source is using internal reference voltage and
//! charge pump is enabled.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param voltage is the charge pump select.
//!        Valid values are:
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_60V [Default]
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_66V
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_72V
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_78V
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_84V
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_90V
//!        - \b LCD_E_REFERENCE_VOLTAGE_2_96V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_02V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_08V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_14V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_20V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_26V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_32V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_38V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_44V
//!        - \b LCD_E_REFERENCE_VOLTAGE_3_50V
//!        \n Modified bits are \b VLCDx of \b LCDVCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setVLCDVoltage(uint16_t baseAddress,
                                 uint16_t voltage);

//*****************************************************************************
//
//! \brief Sets the reference mode for R13.
//!
//! This function sets the reference mode for R13. In the switch mode, the Bias
//! Voltage Generator is on for 1 clock and off for 256 clock cycles to save
//! power. In the static mode, the Bias Voltage Generator is able to drive
//! larger LCD panels.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param mode is the reference mode on R13.
//!        Valid values are:
//!        - \b LCD_E_REFERENCE_MODE_STATIC [Default]
//!        - \b LCD_E_REFERENCE_MODE_SWITCHED
//!        \n Modified bits are \b LCDREFMODE of \b LCDVCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setReferenceMode(uint16_t baseAddress,
                                   uint16_t mode);

//*****************************************************************************
//
//! \brief Sets the LCD_E pins as LCD function pin.
//!
//! This function sets the LCD_E pins as LCD function pin.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param pin is the select pin set as LCD function.
//!        Valid values are:
//!        - \b LCD_E_SEGMENT_LINE_0
//!        - \b LCD_E_SEGMENT_LINE_1
//!        - \b LCD_E_SEGMENT_LINE_2
//!        - \b LCD_E_SEGMENT_LINE_3
//!        - \b LCD_E_SEGMENT_LINE_4
//!        - \b LCD_E_SEGMENT_LINE_5
//!        - \b LCD_E_SEGMENT_LINE_6
//!        - \b LCD_E_SEGMENT_LINE_7
//!        - \b LCD_E_SEGMENT_LINE_8
//!        - \b LCD_E_SEGMENT_LINE_9
//!        - \b LCD_E_SEGMENT_LINE_10
//!        - \b LCD_E_SEGMENT_LINE_11
//!        - \b LCD_E_SEGMENT_LINE_12
//!        - \b LCD_E_SEGMENT_LINE_13
//!        - \b LCD_E_SEGMENT_LINE_14
//!        - \b LCD_E_SEGMENT_LINE_15
//!        - \b LCD_E_SEGMENT_LINE_16
//!        - \b LCD_E_SEGMENT_LINE_17
//!        - \b LCD_E_SEGMENT_LINE_18
//!        - \b LCD_E_SEGMENT_LINE_19
//!        - \b LCD_E_SEGMENT_LINE_20
//!        - \b LCD_E_SEGMENT_LINE_21
//!        - \b LCD_E_SEGMENT_LINE_22
//!        - \b LCD_E_SEGMENT_LINE_23
//!        - \b LCD_E_SEGMENT_LINE_24
//!        - \b LCD_E_SEGMENT_LINE_25
//!        - \b LCD_E_SEGMENT_LINE_26
//!        - \b LCD_E_SEGMENT_LINE_27
//!        - \b LCD_E_SEGMENT_LINE_28
//!        - \b LCD_E_SEGMENT_LINE_29
//!        - \b LCD_E_SEGMENT_LINE_30
//!        - \b LCD_E_SEGMENT_LINE_31
//!        - \b LCD_E_SEGMENT_LINE_32
//!        - \b LCD_E_SEGMENT_LINE_33
//!        - \b LCD_E_SEGMENT_LINE_34
//!        - \b LCD_E_SEGMENT_LINE_35
//!        - \b LCD_E_SEGMENT_LINE_36
//!        - \b LCD_E_SEGMENT_LINE_37
//!        - \b LCD_E_SEGMENT_LINE_38
//!        - \b LCD_E_SEGMENT_LINE_39
//!        - \b LCD_E_SEGMENT_LINE_40
//!        - \b LCD_E_SEGMENT_LINE_41
//!        - \b LCD_E_SEGMENT_LINE_42
//!        - \b LCD_E_SEGMENT_LINE_43
//!        - \b LCD_E_SEGMENT_LINE_44
//!        - \b LCD_E_SEGMENT_LINE_45
//!        - \b LCD_E_SEGMENT_LINE_46
//!        - \b LCD_E_SEGMENT_LINE_47
//!
//! Modified bits are \b LCDSx of \b LCDPCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setPinAsLCDFunction(uint16_t baseAddress,
                                      uint8_t pin);

//*****************************************************************************
//
//! \brief Sets the LCD_E pins as port function pin.
//!
//! This function sets the LCD_E pins as port function pin.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param pin is the select pin set as Port function.
//!        Valid values are:
//!        - \b LCD_E_SEGMENT_LINE_0
//!        - \b LCD_E_SEGMENT_LINE_1
//!        - \b LCD_E_SEGMENT_LINE_2
//!        - \b LCD_E_SEGMENT_LINE_3
//!        - \b LCD_E_SEGMENT_LINE_4
//!        - \b LCD_E_SEGMENT_LINE_5
//!        - \b LCD_E_SEGMENT_LINE_6
//!        - \b LCD_E_SEGMENT_LINE_7
//!        - \b LCD_E_SEGMENT_LINE_8
//!        - \b LCD_E_SEGMENT_LINE_9
//!        - \b LCD_E_SEGMENT_LINE_10
//!        - \b LCD_E_SEGMENT_LINE_11
//!        - \b LCD_E_SEGMENT_LINE_12
//!        - \b LCD_E_SEGMENT_LINE_13
//!        - \b LCD_E_SEGMENT_LINE_14
//!        - \b LCD_E_SEGMENT_LINE_15
//!        - \b LCD_E_SEGMENT_LINE_16
//!        - \b LCD_E_SEGMENT_LINE_17
//!        - \b LCD_E_SEGMENT_LINE_18
//!        - \b LCD_E_SEGMENT_LINE_19
//!        - \b LCD_E_SEGMENT_LINE_20
//!        - \b LCD_E_SEGMENT_LINE_21
//!        - \b LCD_E_SEGMENT_LINE_22
//!        - \b LCD_E_SEGMENT_LINE_23
//!        - \b LCD_E_SEGMENT_LINE_24
//!        - \b LCD_E_SEGMENT_LINE_25
//!        - \b LCD_E_SEGMENT_LINE_26
//!        - \b LCD_E_SEGMENT_LINE_27
//!        - \b LCD_E_SEGMENT_LINE_28
//!        - \b LCD_E_SEGMENT_LINE_29
//!        - \b LCD_E_SEGMENT_LINE_30
//!        - \b LCD_E_SEGMENT_LINE_31
//!        - \b LCD_E_SEGMENT_LINE_32
//!        - \b LCD_E_SEGMENT_LINE_33
//!        - \b LCD_E_SEGMENT_LINE_34
//!        - \b LCD_E_SEGMENT_LINE_35
//!        - \b LCD_E_SEGMENT_LINE_36
//!        - \b LCD_E_SEGMENT_LINE_37
//!        - \b LCD_E_SEGMENT_LINE_38
//!        - \b LCD_E_SEGMENT_LINE_39
//!        - \b LCD_E_SEGMENT_LINE_40
//!        - \b LCD_E_SEGMENT_LINE_41
//!        - \b LCD_E_SEGMENT_LINE_42
//!        - \b LCD_E_SEGMENT_LINE_43
//!        - \b LCD_E_SEGMENT_LINE_44
//!        - \b LCD_E_SEGMENT_LINE_45
//!        - \b LCD_E_SEGMENT_LINE_46
//!        - \b LCD_E_SEGMENT_LINE_47
//!
//! Modified bits are \b LCDSx of \b LCDPCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setPinAsPortFunction(uint16_t baseAddress,
                                       uint8_t pin);

//*****************************************************************************
//
//! \brief Sets the LCD_E pins as LCD function pin.
//!
//! This function sets the LCD_E pins as LCD function pin. Instead of passing
//! the all the possible pins, it just requires the start pin and the end pin.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param startPin is the starting pin to be configed as LCD function pin.
//!        Valid values are:
//!        - \b LCD_E_SEGMENT_LINE_0
//!        - \b LCD_E_SEGMENT_LINE_1
//!        - \b LCD_E_SEGMENT_LINE_2
//!        - \b LCD_E_SEGMENT_LINE_3
//!        - \b LCD_E_SEGMENT_LINE_4
//!        - \b LCD_E_SEGMENT_LINE_5
//!        - \b LCD_E_SEGMENT_LINE_6
//!        - \b LCD_E_SEGMENT_LINE_7
//!        - \b LCD_E_SEGMENT_LINE_8
//!        - \b LCD_E_SEGMENT_LINE_9
//!        - \b LCD_E_SEGMENT_LINE_10
//!        - \b LCD_E_SEGMENT_LINE_11
//!        - \b LCD_E_SEGMENT_LINE_12
//!        - \b LCD_E_SEGMENT_LINE_13
//!        - \b LCD_E_SEGMENT_LINE_14
//!        - \b LCD_E_SEGMENT_LINE_15
//!        - \b LCD_E_SEGMENT_LINE_16
//!        - \b LCD_E_SEGMENT_LINE_17
//!        - \b LCD_E_SEGMENT_LINE_18
//!        - \b LCD_E_SEGMENT_LINE_19
//!        - \b LCD_E_SEGMENT_LINE_20
//!        - \b LCD_E_SEGMENT_LINE_21
//!        - \b LCD_E_SEGMENT_LINE_22
//!        - \b LCD_E_SEGMENT_LINE_23
//!        - \b LCD_E_SEGMENT_LINE_24
//!        - \b LCD_E_SEGMENT_LINE_25
//!        - \b LCD_E_SEGMENT_LINE_26
//!        - \b LCD_E_SEGMENT_LINE_27
//!        - \b LCD_E_SEGMENT_LINE_28
//!        - \b LCD_E_SEGMENT_LINE_29
//!        - \b LCD_E_SEGMENT_LINE_30
//!        - \b LCD_E_SEGMENT_LINE_31
//!        - \b LCD_E_SEGMENT_LINE_32
//!        - \b LCD_E_SEGMENT_LINE_33
//!        - \b LCD_E_SEGMENT_LINE_34
//!        - \b LCD_E_SEGMENT_LINE_35
//!        - \b LCD_E_SEGMENT_LINE_36
//!        - \b LCD_E_SEGMENT_LINE_37
//!        - \b LCD_E_SEGMENT_LINE_38
//!        - \b LCD_E_SEGMENT_LINE_39
//!        - \b LCD_E_SEGMENT_LINE_40
//!        - \b LCD_E_SEGMENT_LINE_41
//!        - \b LCD_E_SEGMENT_LINE_42
//!        - \b LCD_E_SEGMENT_LINE_43
//!        - \b LCD_E_SEGMENT_LINE_44
//!        - \b LCD_E_SEGMENT_LINE_45
//!        - \b LCD_E_SEGMENT_LINE_46
//!        - \b LCD_E_SEGMENT_LINE_47
//! \param endPin is the ending pin to be configed as LCD function pin.
//!        Valid values are:
//!        - \b LCD_E_SEGMENT_LINE_0
//!        - \b LCD_E_SEGMENT_LINE_1
//!        - \b LCD_E_SEGMENT_LINE_2
//!        - \b LCD_E_SEGMENT_LINE_3
//!        - \b LCD_E_SEGMENT_LINE_4
//!        - \b LCD_E_SEGMENT_LINE_5
//!        - \b LCD_E_SEGMENT_LINE_6
//!        - \b LCD_E_SEGMENT_LINE_7
//!        - \b LCD_E_SEGMENT_LINE_8
//!        - \b LCD_E_SEGMENT_LINE_9
//!        - \b LCD_E_SEGMENT_LINE_10
//!        - \b LCD_E_SEGMENT_LINE_11
//!        - \b LCD_E_SEGMENT_LINE_12
//!        - \b LCD_E_SEGMENT_LINE_13
//!        - \b LCD_E_SEGMENT_LINE_14
//!        - \b LCD_E_SEGMENT_LINE_15
//!        - \b LCD_E_SEGMENT_LINE_16
//!        - \b LCD_E_SEGMENT_LINE_17
//!        - \b LCD_E_SEGMENT_LINE_18
//!        - \b LCD_E_SEGMENT_LINE_19
//!        - \b LCD_E_SEGMENT_LINE_20
//!        - \b LCD_E_SEGMENT_LINE_21
//!        - \b LCD_E_SEGMENT_LINE_22
//!        - \b LCD_E_SEGMENT_LINE_23
//!        - \b LCD_E_SEGMENT_LINE_24
//!        - \b LCD_E_SEGMENT_LINE_25
//!        - \b LCD_E_SEGMENT_LINE_26
//!        - \b LCD_E_SEGMENT_LINE_27
//!        - \b LCD_E_SEGMENT_LINE_28
//!        - \b LCD_E_SEGMENT_LINE_29
//!        - \b LCD_E_SEGMENT_LINE_30
//!        - \b LCD_E_SEGMENT_LINE_31
//!        - \b LCD_E_SEGMENT_LINE_32
//!        - \b LCD_E_SEGMENT_LINE_33
//!        - \b LCD_E_SEGMENT_LINE_34
//!        - \b LCD_E_SEGMENT_LINE_35
//!        - \b LCD_E_SEGMENT_LINE_36
//!        - \b LCD_E_SEGMENT_LINE_37
//!        - \b LCD_E_SEGMENT_LINE_38
//!        - \b LCD_E_SEGMENT_LINE_39
//!        - \b LCD_E_SEGMENT_LINE_40
//!        - \b LCD_E_SEGMENT_LINE_41
//!        - \b LCD_E_SEGMENT_LINE_42
//!        - \b LCD_E_SEGMENT_LINE_43
//!        - \b LCD_E_SEGMENT_LINE_44
//!        - \b LCD_E_SEGMENT_LINE_45
//!        - \b LCD_E_SEGMENT_LINE_46
//!        - \b LCD_E_SEGMENT_LINE_47
//!
//! Modified bits are \b LCDSx of \b LCDPCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setPinAsLCDFunctionEx(uint16_t baseAddress,
                                        uint8_t startPin,
                                        uint8_t endPin);

//*****************************************************************************
//
//! \brief Sets the LCD_E pin as a common line.
//!
//! This function sets the LCD_E pin as a common line and assigns the
//! corresponding memory pin to a specific COM line.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param pin is the selected pin to be configed as common line.
//!        Valid values are:
//!        - \b LCD_E_SEGMENT_LINE_0
//!        - \b LCD_E_SEGMENT_LINE_1
//!        - \b LCD_E_SEGMENT_LINE_2
//!        - \b LCD_E_SEGMENT_LINE_3
//!        - \b LCD_E_SEGMENT_LINE_4
//!        - \b LCD_E_SEGMENT_LINE_5
//!        - \b LCD_E_SEGMENT_LINE_6
//!        - \b LCD_E_SEGMENT_LINE_7
//!        - \b LCD_E_SEGMENT_LINE_8
//!        - \b LCD_E_SEGMENT_LINE_9
//!        - \b LCD_E_SEGMENT_LINE_10
//!        - \b LCD_E_SEGMENT_LINE_11
//!        - \b LCD_E_SEGMENT_LINE_12
//!        - \b LCD_E_SEGMENT_LINE_13
//!        - \b LCD_E_SEGMENT_LINE_14
//!        - \b LCD_E_SEGMENT_LINE_15
//!        - \b LCD_E_SEGMENT_LINE_16
//!        - \b LCD_E_SEGMENT_LINE_17
//!        - \b LCD_E_SEGMENT_LINE_18
//!        - \b LCD_E_SEGMENT_LINE_19
//!        - \b LCD_E_SEGMENT_LINE_20
//!        - \b LCD_E_SEGMENT_LINE_21
//!        - \b LCD_E_SEGMENT_LINE_22
//!        - \b LCD_E_SEGMENT_LINE_23
//!        - \b LCD_E_SEGMENT_LINE_24
//!        - \b LCD_E_SEGMENT_LINE_25
//!        - \b LCD_E_SEGMENT_LINE_26
//!        - \b LCD_E_SEGMENT_LINE_27
//!        - \b LCD_E_SEGMENT_LINE_28
//!        - \b LCD_E_SEGMENT_LINE_29
//!        - \b LCD_E_SEGMENT_LINE_30
//!        - \b LCD_E_SEGMENT_LINE_31
//!        - \b LCD_E_SEGMENT_LINE_32
//!        - \b LCD_E_SEGMENT_LINE_33
//!        - \b LCD_E_SEGMENT_LINE_34
//!        - \b LCD_E_SEGMENT_LINE_35
//!        - \b LCD_E_SEGMENT_LINE_36
//!        - \b LCD_E_SEGMENT_LINE_37
//!        - \b LCD_E_SEGMENT_LINE_38
//!        - \b LCD_E_SEGMENT_LINE_39
//!        - \b LCD_E_SEGMENT_LINE_40
//!        - \b LCD_E_SEGMENT_LINE_41
//!        - \b LCD_E_SEGMENT_LINE_42
//!        - \b LCD_E_SEGMENT_LINE_43
//!        - \b LCD_E_SEGMENT_LINE_44
//!        - \b LCD_E_SEGMENT_LINE_45
//!        - \b LCD_E_SEGMENT_LINE_46
//!        - \b LCD_E_SEGMENT_LINE_47
//! \param com is the selected COM number for the common line.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_COM0
//!        - \b LCD_E_MEMORY_COM1
//!        - \b LCD_E_MEMORY_COM2
//!        - \b LCD_E_MEMORY_COM3
//!        - \b LCD_E_MEMORY_COM4 - only for 5-Mux/6-Mux/7-Mux/8-Mux
//!        - \b LCD_E_MEMORY_COM5 - only for 5-Mux/6-Mux/7-Mux/8-Mux
//!        - \b LCD_E_MEMORY_COM6 - only for 5-Mux/6-Mux/7-Mux/8-Mux
//!        - \b LCD_E_MEMORY_COM7 - only for 5-Mux/6-Mux/7-Mux/8-Mux
//!
//! Modified bits are \b LCDCSSx of \b LCDSSELx register; bits \b MBITx of \b
//! LCDBMx register; bits \b MBITx of \b LCDMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setPinAsCOM(uint16_t baseAddress,
                              uint8_t pin,
                              uint8_t com);

//*****************************************************************************
//
//! \brief Sets the LCD_E pin as a segment line.
//!
//! This function sets the LCD_E pin as segment line.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param pin is the selected pin to be configed as segment line.
//!        Valid values are:
//!        - \b LCD_E_SEGMENT_LINE_0
//!        - \b LCD_E_SEGMENT_LINE_1
//!        - \b LCD_E_SEGMENT_LINE_2
//!        - \b LCD_E_SEGMENT_LINE_3
//!        - \b LCD_E_SEGMENT_LINE_4
//!        - \b LCD_E_SEGMENT_LINE_5
//!        - \b LCD_E_SEGMENT_LINE_6
//!        - \b LCD_E_SEGMENT_LINE_7
//!        - \b LCD_E_SEGMENT_LINE_8
//!        - \b LCD_E_SEGMENT_LINE_9
//!        - \b LCD_E_SEGMENT_LINE_10
//!        - \b LCD_E_SEGMENT_LINE_11
//!        - \b LCD_E_SEGMENT_LINE_12
//!        - \b LCD_E_SEGMENT_LINE_13
//!        - \b LCD_E_SEGMENT_LINE_14
//!        - \b LCD_E_SEGMENT_LINE_15
//!        - \b LCD_E_SEGMENT_LINE_16
//!        - \b LCD_E_SEGMENT_LINE_17
//!        - \b LCD_E_SEGMENT_LINE_18
//!        - \b LCD_E_SEGMENT_LINE_19
//!        - \b LCD_E_SEGMENT_LINE_20
//!        - \b LCD_E_SEGMENT_LINE_21
//!        - \b LCD_E_SEGMENT_LINE_22
//!        - \b LCD_E_SEGMENT_LINE_23
//!        - \b LCD_E_SEGMENT_LINE_24
//!        - \b LCD_E_SEGMENT_LINE_25
//!        - \b LCD_E_SEGMENT_LINE_26
//!        - \b LCD_E_SEGMENT_LINE_27
//!        - \b LCD_E_SEGMENT_LINE_28
//!        - \b LCD_E_SEGMENT_LINE_29
//!        - \b LCD_E_SEGMENT_LINE_30
//!        - \b LCD_E_SEGMENT_LINE_31
//!        - \b LCD_E_SEGMENT_LINE_32
//!        - \b LCD_E_SEGMENT_LINE_33
//!        - \b LCD_E_SEGMENT_LINE_34
//!        - \b LCD_E_SEGMENT_LINE_35
//!        - \b LCD_E_SEGMENT_LINE_36
//!        - \b LCD_E_SEGMENT_LINE_37
//!        - \b LCD_E_SEGMENT_LINE_38
//!        - \b LCD_E_SEGMENT_LINE_39
//!        - \b LCD_E_SEGMENT_LINE_40
//!        - \b LCD_E_SEGMENT_LINE_41
//!        - \b LCD_E_SEGMENT_LINE_42
//!        - \b LCD_E_SEGMENT_LINE_43
//!        - \b LCD_E_SEGMENT_LINE_44
//!        - \b LCD_E_SEGMENT_LINE_45
//!        - \b LCD_E_SEGMENT_LINE_46
//!        - \b LCD_E_SEGMENT_LINE_47
//!
//! Modified bits are \b LCDCSSx of \b LCDSSELx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setPinAsSEG(uint16_t baseAddress,
                              uint8_t pin);

//*****************************************************************************
//
//! \brief Sets the LCD_E memory register.
//!
//! This function sets the entire one LCD_E memory register.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding memory.
//!
//! Modified bits are \b MBITx of \b LCDMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setMemory(uint16_t baseAddress,
                            uint8_t memory,
                            uint8_t mask);

//*****************************************************************************
//
//! \brief Updates the LCD_E memory register.
//!
//! This function updates the specific bits in the LCD_E memory register
//! according to the mask.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding memory.
//!
//! Modified bits are \b MBITx of \b LCDMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_updateMemory(uint16_t baseAddress,
                               uint8_t memory,
                               uint8_t mask);

//*****************************************************************************
//
//! \brief Toggles the LCD_E memory register.
//!
//! This function toggles the specific bits in the LCD_E memory register
//! according to the mask.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding memory.
//!
//! Modified bits are \b MBITx of \b LCDMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_toggleMemory(uint16_t baseAddress,
                               uint8_t memory,
                               uint8_t mask);

//*****************************************************************************
//
//! \brief Clears the LCD_E memory register.
//!
//! This function clears the specific bits in the LCD_E memory register
//! according to the mask.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding memory.
//!
//! Modified bits are \b MBITx of \b LCDMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_clearMemory(uint16_t baseAddress,
                              uint8_t memory,
                              uint8_t mask);

//*****************************************************************************
//
//! \brief Sets the LCD_E blinking memory register.
//!
//! This function sets the entire one LCD_E blinking memory register.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select blinking memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding blinking memory.
//!
//! Modified bits are \b MBITx of \b LCDBMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_setBlinkingMemory(uint16_t baseAddress,
                                    uint8_t memory,
                                    uint8_t mask);

//*****************************************************************************
//
//! \brief Updates the LCD_E blinking memory register.
//!
//! This function updates the specific bits in the LCD_E blinking memory
//! register according to the mask.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select blinking memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding blinking memory.
//!
//! Modified bits are \b MBITx of \b LCDBMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_updateBlinkingMemory(uint16_t baseAddress,
                                       uint8_t memory,
                                       uint8_t mask);

//*****************************************************************************
//
//! \brief Toggles the LCD_E blinking memory register.
//!
//! This function toggles the specific bits in the LCD_E blinking memory
//! register according to the mask.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select blinking memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding blinking memory.
//!
//! Modified bits are \b MBITx of \b LCDBMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_toggleBlinkingMemory(uint16_t baseAddress,
                                       uint8_t memory,
                                       uint8_t mask);

//*****************************************************************************
//
//! \brief Clears the LCD_E blinking memory register.
//!
//! This function clears the specific bits in the LCD_E blinking memory
//! register according to the mask.
//!
//! \param baseAddress is the base address of the LCD_E module.
//! \param memory is the select blinking memory for setting value.
//!        Valid values are:
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_0
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_1
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_2
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_3
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_4
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_5
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_6
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_7
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_8
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_9
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_10
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_11
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_12
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_13
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_14
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_15
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_16
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_17
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_18
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_19
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_20
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_21
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_22
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_23
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_24
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_25
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_26
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_27
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_28
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_29
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_30
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_31
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_32
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_33
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_34
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_35
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_36
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_37
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_38
//!        - \b LCD_E_MEMORY_BLINKINGMEMORY_39
//! \param mask is the designated value for the corresponding blinking memory.
//!
//! Modified bits are \b MBITx of \b LCDBMx register.
//!
//! \return None
//
//*****************************************************************************
extern void LCD_E_clearBlinkingMemory(uint16_t baseAddress,
                                      uint8_t memory,
                                      uint8_t mask);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_LCD_E_H__

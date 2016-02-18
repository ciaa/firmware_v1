/* ============================================================================ */
/* Copyright (c) 2015, Texas Instruments Incorporated                           */
/*  All rights reserved.                                                        */
/*                                                                              */
/*  Redistribution and use in source and binary forms, with or without          */
/*  modification, are permitted provided that the following conditions          */
/*  are met:                                                                    */
/*                                                                              */
/*  *  Redistributions of source code must retain the above copyright           */
/*     notice, this list of conditions and the following disclaimer.            */
/*                                                                              */
/*  *  Redistributions in binary form must reproduce the above copyright        */
/*     notice, this list of conditions and the following disclaimer in the      */
/*     documentation and/or other materials provided with the distribution.     */
/*                                                                              */
/*  *  Neither the name of Texas Instruments Incorporated nor the names of      */
/*     its contributors may be used to endorse or promote products derived      */
/*     from this software without specific prior written permission.            */
/*                                                                              */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" */
/*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,       */
/*  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      */
/*  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR            */
/*  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,       */
/*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,         */
/*  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; */
/*  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,    */
/*  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     */
/*  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,              */
/*  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                          */
/* ============================================================================ */

/********************************************************************
*
* Standard register and bit definitions for the Texas Instruments
* MSP430 microcontroller.
*
* This file supports assembler and C development for the
* MSP430F67491 device.
*
* Texas Instruments, Version 1.2
*
* Rev. 1.0, Initial Release
* Rev. 1.1, Removed not available Bits SD24BINCHx
* Rev. 1.2, Added Comp B to interrupt Vector Table
*
********************************************************************/

#ifndef __MSP430F67491
#define __MSP430F67491

#define __MSP430_HEADER_VERSION__ 1173

#define __MSP430_TI_HEADERS__

#ifdef __cplusplus
extern "C" {
#endif

#include <iomacros.h>

/************************************************************
* STANDARD BITS
************************************************************/

#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BITA                (0x0400)
#define BITB                (0x0800)
#define BITC                (0x1000)
#define BITD                (0x2000)
#define BITE                (0x4000)
#define BITF                (0x8000)

/************************************************************
* STATUS REGISTER BITS
************************************************************/

#define C                   (0x0001)
#define Z                   (0x0002)
#define N                   (0x0004)
#define V                   (0x0100)
#define GIE                 (0x0008)
#define CPUOFF              (0x0010)
#define OSCOFF              (0x0020)
#define SCG0                (0x0040)
#define SCG1                (0x0080)

/* Low Power Modes coded with Bits 4-7 in SR */

#ifndef __STDC__ /* Begin #defines for assembler */
#define LPM0                (CPUOFF)
#define LPM1                (SCG0+CPUOFF)
#define LPM2                (SCG1+CPUOFF)
#define LPM3                (SCG1+SCG0+CPUOFF)
#define LPM4                (SCG1+SCG0+OSCOFF+CPUOFF)
/* End #defines for assembler */

#else /* Begin #defines for C */
#define LPM0_bits           (CPUOFF)
#define LPM1_bits           (SCG0+CPUOFF)
#define LPM2_bits           (SCG1+CPUOFF)
#define LPM3_bits           (SCG1+SCG0+CPUOFF)
#define LPM4_bits           (SCG1+SCG0+OSCOFF+CPUOFF)

#include "in430.h"

#define LPM0      __bis_SR_register(LPM0_bits)         /* Enter Low Power Mode 0 */
#define LPM0_EXIT __bic_SR_register_on_exit(LPM0_bits) /* Exit Low Power Mode 0 */
#define LPM1      __bis_SR_register(LPM1_bits)         /* Enter Low Power Mode 1 */
#define LPM1_EXIT __bic_SR_register_on_exit(LPM1_bits) /* Exit Low Power Mode 1 */
#define LPM2      __bis_SR_register(LPM2_bits)         /* Enter Low Power Mode 2 */
#define LPM2_EXIT __bic_SR_register_on_exit(LPM2_bits) /* Exit Low Power Mode 2 */
#define LPM3      __bis_SR_register(LPM3_bits)         /* Enter Low Power Mode 3 */
#define LPM3_EXIT __bic_SR_register_on_exit(LPM3_bits) /* Exit Low Power Mode 3 */
#define LPM4      __bis_SR_register(LPM4_bits)         /* Enter Low Power Mode 4 */
#define LPM4_EXIT __bic_SR_register_on_exit(LPM4_bits) /* Exit Low Power Mode 4 */
#endif /* End #defines for C */

/************************************************************
* CPU
************************************************************/
#define __MSP430_HAS_MSP430XV2_CPU__  /* Definition to show that it has MSP430XV2 CPU */
#define __MSP430F5XX_6XX_FAMILY__

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/


/************************************************************
* ADC10_A
************************************************************/
#define __MSP430_HAS_ADC10_A__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_ADC10_A__ 0x0740
#define ADC10_A_BASE __MSP430_BASEADDRESS_ADC10_A__

#define ADC10CTL0_            0x0740    /* ADC10 Control 0 */
sfrb(ADC10CTL0_L , ADC10CTL0_);
sfrb(ADC10CTL0_H , ADC10CTL0_+1);
sfrw(ADC10CTL0, ADC10CTL0_);
#define ADC10CTL1_            0x0742    /* ADC10 Control 1 */
sfrb(ADC10CTL1_L , ADC10CTL1_);
sfrb(ADC10CTL1_H , ADC10CTL1_+1);
sfrw(ADC10CTL1, ADC10CTL1_);
#define ADC10CTL2_            0x0744    /* ADC10 Control 2 */
sfrb(ADC10CTL2_L , ADC10CTL2_);
sfrb(ADC10CTL2_H , ADC10CTL2_+1);
sfrw(ADC10CTL2, ADC10CTL2_);
#define ADC10LO_              0x0746    /* ADC10 Window Comparator High Threshold */
sfrb(ADC10LO_L , ADC10LO_);
sfrb(ADC10LO_H , ADC10LO_+1);
sfrw(ADC10LO, ADC10LO_);
#define ADC10HI_              0x0748    /* ADC10 Window Comparator High Threshold */
sfrb(ADC10HI_L , ADC10HI_);
sfrb(ADC10HI_H , ADC10HI_+1);
sfrw(ADC10HI, ADC10HI_);
#define ADC10MCTL0_           0x074A    /* ADC10 Memory Control 0 */
sfrb(ADC10MCTL0_L , ADC10MCTL0_);
sfrb(ADC10MCTL0_H , ADC10MCTL0_+1);
sfrw(ADC10MCTL0, ADC10MCTL0_);
#define ADC10MEM0_            0x0752    /* ADC10 Conversion Memory 0 */
sfrb(ADC10MEM0_L , ADC10MEM0_);
sfrb(ADC10MEM0_H , ADC10MEM0_+1);
sfrw(ADC10MEM0, ADC10MEM0_);
#define ADC10IE_              0x075A    /* ADC10 Interrupt Enable */
sfrb(ADC10IE_L , ADC10IE_);
sfrb(ADC10IE_H , ADC10IE_+1);
sfrw(ADC10IE, ADC10IE_);
#define ADC10IFG_             0x075C    /* ADC10 Interrupt Flag */
sfrb(ADC10IFG_L , ADC10IFG_);
sfrb(ADC10IFG_H , ADC10IFG_+1);
sfrw(ADC10IFG, ADC10IFG_);
#define ADC10IV_              0x075E    /* ADC10 Interrupt Vector Word */
sfrb(ADC10IV_L , ADC10IV_);
sfrb(ADC10IV_H , ADC10IV_+1);
sfrw(ADC10IV, ADC10IV_);

/* ADC10CTL0 Control Bits */
#define ADC10SC             (0x0001)  /* ADC10 Start Conversion */
#define ADC10ENC            (0x0002)  /* ADC10 Enable Conversion */
#define ADC10ON             (0x0010)  /* ADC10 On/enable */
#define ADC10MSC            (0x0080)  /* ADC10 Multiple SampleConversion */
#define ADC10SHT0           (0x0100)  /* ADC10 Sample Hold Select Bit: 0 */
#define ADC10SHT1           (0x0200)  /* ADC10 Sample Hold Select Bit: 1 */
#define ADC10SHT2           (0x0400)  /* ADC10 Sample Hold Select Bit: 2 */
#define ADC10SHT3           (0x0800)  /* ADC10 Sample Hold Select Bit: 3 */

/* ADC10CTL0 Control Bits */
#define ADC10SC_L           (0x0001)  /* ADC10 Start Conversion */
#define ADC10ENC_L          (0x0002)  /* ADC10 Enable Conversion */
#define ADC10ON_L           (0x0010)  /* ADC10 On/enable */
#define ADC10MSC_L          (0x0080)  /* ADC10 Multiple SampleConversion */

/* ADC10CTL0 Control Bits */
#define ADC10SHT0_H         (0x0001)  /* ADC10 Sample Hold Select Bit: 0 */
#define ADC10SHT1_H         (0x0002)  /* ADC10 Sample Hold Select Bit: 1 */
#define ADC10SHT2_H         (0x0004)  /* ADC10 Sample Hold Select Bit: 2 */
#define ADC10SHT3_H         (0x0008)  /* ADC10 Sample Hold Select Bit: 3 */

#define ADC10SHT_0          (0x0000) /* ADC10 Sample Hold Select 0 */
#define ADC10SHT_1          (0x0100) /* ADC10 Sample Hold Select 1 */
#define ADC10SHT_2          (0x0200) /* ADC10 Sample Hold Select 2 */
#define ADC10SHT_3          (0x0300) /* ADC10 Sample Hold Select 3 */
#define ADC10SHT_4          (0x0400) /* ADC10 Sample Hold Select 4 */
#define ADC10SHT_5          (0x0500) /* ADC10 Sample Hold Select 5 */
#define ADC10SHT_6          (0x0600) /* ADC10 Sample Hold Select 6 */
#define ADC10SHT_7          (0x0700) /* ADC10 Sample Hold Select 7 */
#define ADC10SHT_8          (0x0800) /* ADC10 Sample Hold Select 8 */
#define ADC10SHT_9          (0x0900) /* ADC10 Sample Hold Select 9 */
#define ADC10SHT_10         (0x0A00) /* ADC10 Sample Hold Select 10 */
#define ADC10SHT_11         (0x0B00) /* ADC10 Sample Hold Select 11 */
#define ADC10SHT_12         (0x0C00) /* ADC10 Sample Hold Select 12 */
#define ADC10SHT_13         (0x0D00) /* ADC10 Sample Hold Select 13 */
#define ADC10SHT_14         (0x0E00) /* ADC10 Sample Hold Select 14 */
#define ADC10SHT_15         (0x0F00) /* ADC10 Sample Hold Select 15 */

/* ADC10CTL1 Control Bits */
#define ADC10BUSY           (0x0001)    /* ADC10 Busy */
#define ADC10CONSEQ0        (0x0002)    /* ADC10 Conversion Sequence Select 0 */
#define ADC10CONSEQ1        (0x0004)    /* ADC10 Conversion Sequence Select 1 */
#define ADC10SSEL0          (0x0008)    /* ADC10 Clock Source Select 0 */
#define ADC10SSEL1          (0x0010)    /* ADC10 Clock Source Select 1 */
#define ADC10DIV0           (0x0020)    /* ADC10 Clock Divider Select 0 */
#define ADC10DIV1           (0x0040)    /* ADC10 Clock Divider Select 1 */
#define ADC10DIV2           (0x0080)    /* ADC10 Clock Divider Select 2 */
#define ADC10ISSH           (0x0100)    /* ADC10 Invert Sample Hold Signal */
#define ADC10SHP            (0x0200)    /* ADC10 Sample/Hold Pulse Mode */
#define ADC10SHS0           (0x0400)    /* ADC10 Sample/Hold Source 0 */
#define ADC10SHS1           (0x0800)    /* ADC10 Sample/Hold Source 1 */

/* ADC10CTL1 Control Bits */
#define ADC10BUSY_L         (0x0001)    /* ADC10 Busy */
#define ADC10CONSEQ0_L      (0x0002)    /* ADC10 Conversion Sequence Select 0 */
#define ADC10CONSEQ1_L      (0x0004)    /* ADC10 Conversion Sequence Select 1 */
#define ADC10SSEL0_L        (0x0008)    /* ADC10 Clock Source Select 0 */
#define ADC10SSEL1_L        (0x0010)    /* ADC10 Clock Source Select 1 */
#define ADC10DIV0_L         (0x0020)    /* ADC10 Clock Divider Select 0 */
#define ADC10DIV1_L         (0x0040)    /* ADC10 Clock Divider Select 1 */
#define ADC10DIV2_L         (0x0080)    /* ADC10 Clock Divider Select 2 */

/* ADC10CTL1 Control Bits */
#define ADC10ISSH_H         (0x0001)    /* ADC10 Invert Sample Hold Signal */
#define ADC10SHP_H          (0x0002)    /* ADC10 Sample/Hold Pulse Mode */
#define ADC10SHS0_H         (0x0004)    /* ADC10 Sample/Hold Source 0 */
#define ADC10SHS1_H         (0x0008)    /* ADC10 Sample/Hold Source 1 */

#define ADC10CONSEQ_0        (0x0000)      /* ADC10 Conversion Sequence Select: 0 */
#define ADC10CONSEQ_1        (0x0002)      /* ADC10 Conversion Sequence Select: 1 */
#define ADC10CONSEQ_2        (0x0004)      /* ADC10 Conversion Sequence Select: 2 */
#define ADC10CONSEQ_3        (0x0006)      /* ADC10 Conversion Sequence Select: 3 */

#define ADC10SSEL_0          (0x0000)      /* ADC10 Clock Source Select: 0 */
#define ADC10SSEL_1          (0x0008)      /* ADC10 Clock Source Select: 1 */
#define ADC10SSEL_2          (0x0010)      /* ADC10 Clock Source Select: 2 */
#define ADC10SSEL_3          (0x0018)      /* ADC10 Clock Source Select: 3 */

#define ADC10DIV_0           (0x0000)   /* ADC10 Clock Divider Select: 0 */
#define ADC10DIV_1           (0x0020)   /* ADC10 Clock Divider Select: 1 */
#define ADC10DIV_2           (0x0040)   /* ADC10 Clock Divider Select: 2 */
#define ADC10DIV_3           (0x0060)   /* ADC10 Clock Divider Select: 3 */
#define ADC10DIV_4           (0x0080)   /* ADC10 Clock Divider Select: 4 */
#define ADC10DIV_5           (0x00A0)   /* ADC10 Clock Divider Select: 5 */
#define ADC10DIV_6           (0x00C0)   /* ADC10 Clock Divider Select: 6 */
#define ADC10DIV_7           (0x00E0)   /* ADC10 Clock Divider Select: 7 */

#define ADC10SHS_0           (0x0000)  /* ADC10 Sample/Hold Source: 0 */
#define ADC10SHS_1           (0x0400)  /* ADC10 Sample/Hold Source: 1 */
#define ADC10SHS_2           (0x0800)  /* ADC10 Sample/Hold Source: 2 */
#define ADC10SHS_3           (0x0C00)  /* ADC10 Sample/Hold Source: 3 */

/* ADC10CTL2 Control Bits */
#define ADC10REFBURST       (0x0001)    /* ADC10 Reference Burst */
#define ADC10SR             (0x0004)    /* ADC10 Sampling Rate */
#define ADC10DF             (0x0008)    /* ADC10 Data Format */
#define ADC10RES            (0x0010)    /* ADC10 Resolution Bit */
#define ADC10PDIV0          (0x0100)    /* ADC10 predivider Bit: 0 */
#define ADC10PDIV1          (0x0200)    /* ADC10 predivider Bit: 1 */

/* ADC10CTL2 Control Bits */
#define ADC10REFBURST_L     (0x0001)    /* ADC10 Reference Burst */
#define ADC10SR_L           (0x0004)    /* ADC10 Sampling Rate */
#define ADC10DF_L           (0x0008)    /* ADC10 Data Format */
#define ADC10RES_L          (0x0010)    /* ADC10 Resolution Bit */

/* ADC10CTL2 Control Bits */
#define ADC10PDIV0_H        (0x0001)    /* ADC10 predivider Bit: 0 */
#define ADC10PDIV1_H        (0x0002)    /* ADC10 predivider Bit: 1 */

#define ADC10PDIV_0         (0x0000)    /* ADC10 predivider /1 */
#define ADC10PDIV_1         (0x0100)    /* ADC10 predivider /2 */
#define ADC10PDIV_2         (0x0200)    /* ADC10 predivider /64 */
#define ADC10PDIV_3         (0x0300)    /* ADC10 predivider reserved */

#define ADC10PDIV__1        (0x0000)    /* ADC10 predivider /1 */
#define ADC10PDIV__4        (0x0100)    /* ADC10 predivider /2 */
#define ADC10PDIV__64       (0x0200)    /* ADC10 predivider /64 */

/* ADC10MCTL0 Control Bits */
#define ADC10INCH0          (0x0001)    /* ADC10 Input Channel Select Bit 0 */
#define ADC10INCH1          (0x0002)    /* ADC10 Input Channel Select Bit 1 */
#define ADC10INCH2          (0x0004)    /* ADC10 Input Channel Select Bit 2 */
#define ADC10INCH3          (0x0008)    /* ADC10 Input Channel Select Bit 3 */
#define ADC10SREF0          (0x0010)    /* ADC10 Select Reference Bit 0 */
#define ADC10SREF1          (0x0020)    /* ADC10 Select Reference Bit 1 */
#define ADC10SREF2          (0x0040)    /* ADC10 Select Reference Bit 2 */

/* ADC10MCTL0 Control Bits */
#define ADC10INCH0_L        (0x0001)    /* ADC10 Input Channel Select Bit 0 */
#define ADC10INCH1_L        (0x0002)    /* ADC10 Input Channel Select Bit 1 */
#define ADC10INCH2_L        (0x0004)    /* ADC10 Input Channel Select Bit 2 */
#define ADC10INCH3_L        (0x0008)    /* ADC10 Input Channel Select Bit 3 */
#define ADC10SREF0_L        (0x0010)    /* ADC10 Select Reference Bit 0 */
#define ADC10SREF1_L        (0x0020)    /* ADC10 Select Reference Bit 1 */
#define ADC10SREF2_L        (0x0040)    /* ADC10 Select Reference Bit 2 */


#define ADC10INCH_0         (0)         /* ADC10 Input Channel 0 */
#define ADC10INCH_1         (1)         /* ADC10 Input Channel 1 */
#define ADC10INCH_2         (2)         /* ADC10 Input Channel 2 */
#define ADC10INCH_3         (3)         /* ADC10 Input Channel 3 */
#define ADC10INCH_4         (4)         /* ADC10 Input Channel 4 */
#define ADC10INCH_5         (5)         /* ADC10 Input Channel 5 */
#define ADC10INCH_6         (6)         /* ADC10 Input Channel 6 */
#define ADC10INCH_7         (7)         /* ADC10 Input Channel 7 */
#define ADC10INCH_8         (8)         /* ADC10 Input Channel 8 */
#define ADC10INCH_9         (9)         /* ADC10 Input Channel 9 */
#define ADC10INCH_10        (10)        /* ADC10 Input Channel 10 */
#define ADC10INCH_11        (11)        /* ADC10 Input Channel 11 */
#define ADC10INCH_12        (12)        /* ADC10 Input Channel 12 */
#define ADC10INCH_13        (13)        /* ADC10 Input Channel 13 */
#define ADC10INCH_14        (14)        /* ADC10 Input Channel 14 */
#define ADC10INCH_15        (15)        /* ADC10 Input Channel 15 */

#define ADC10SREF_0         (0x0000)    /* ADC10 Select Reference 0 */
#define ADC10SREF_1         (0x0010)    /* ADC10 Select Reference 1 */
#define ADC10SREF_2         (0x0020)    /* ADC10 Select Reference 2 */
#define ADC10SREF_3         (0x0030)    /* ADC10 Select Reference 3 */
#define ADC10SREF_4         (0x0040)    /* ADC10 Select Reference 4 */
#define ADC10SREF_5         (0x0050)    /* ADC10 Select Reference 5 */
#define ADC10SREF_6         (0x0060)    /* ADC10 Select Reference 6 */
#define ADC10SREF_7         (0x0070)    /* ADC10 Select Reference 7 */

/* ADC10IE Interrupt Enable Bits */
#define ADC10IE0            (0x0001)    /* ADC10_A Interrupt enable */
#define ADC10INIE           (0x0002)    /* ADC10_A Interrupt enable for the inside of window of the Window comparator */
#define ADC10LOIE           (0x0004)    /* ADC10_A Interrupt enable for lower threshold of the Window comparator */
#define ADC10HIIE           (0x0008)    /* ADC10_A Interrupt enable for upper threshold of the Window comparator */
#define ADC10OVIE           (0x0010)    /* ADC10_A ADC10MEM overflow Interrupt enable */
#define ADC10TOVIE          (0x0020)    /* ADC10_A conversion-time-overflow Interrupt enable */

/* ADC10IE Interrupt Enable Bits */
#define ADC10IE0_L          (0x0001)    /* ADC10_A Interrupt enable */
#define ADC10INIE_L         (0x0002)    /* ADC10_A Interrupt enable for the inside of window of the Window comparator */
#define ADC10LOIE_L         (0x0004)    /* ADC10_A Interrupt enable for lower threshold of the Window comparator */
#define ADC10HIIE_L         (0x0008)    /* ADC10_A Interrupt enable for upper threshold of the Window comparator */
#define ADC10OVIE_L         (0x0010)    /* ADC10_A ADC10MEM overflow Interrupt enable */
#define ADC10TOVIE_L        (0x0020)    /* ADC10_A conversion-time-overflow Interrupt enable */


/* ADC10IFG Interrupt Flag Bits */
#define ADC10IFG0           (0x0001)    /* ADC10_A Interrupt Flag */
#define ADC10INIFG          (0x0002)    /* ADC10_A Interrupt Flag for the inside of window of the Window comparator */
#define ADC10LOIFG          (0x0004)    /* ADC10_A Interrupt Flag for lower threshold of the Window comparator */
#define ADC10HIIFG          (0x0008)    /* ADC10_A Interrupt Flag for upper threshold of the Window comparator */
#define ADC10OVIFG          (0x0010)    /* ADC10_A ADC10MEM overflow Interrupt Flag */
#define ADC10TOVIFG         (0x0020)    /* ADC10_A conversion-time-overflow Interrupt Flag */

/* ADC10IFG Interrupt Flag Bits */
#define ADC10IFG0_L         (0x0001)    /* ADC10_A Interrupt Flag */
#define ADC10INIFG_L        (0x0002)    /* ADC10_A Interrupt Flag for the inside of window of the Window comparator */
#define ADC10LOIFG_L        (0x0004)    /* ADC10_A Interrupt Flag for lower threshold of the Window comparator */
#define ADC10HIIFG_L        (0x0008)    /* ADC10_A Interrupt Flag for upper threshold of the Window comparator */
#define ADC10OVIFG_L        (0x0010)    /* ADC10_A ADC10MEM overflow Interrupt Flag */
#define ADC10TOVIFG_L       (0x0020)    /* ADC10_A conversion-time-overflow Interrupt Flag */


/* ADC10IV Definitions */
#define ADC10IV_NONE        (0x0000)    /* No Interrupt pending */
#define ADC10IV_ADC10OVIFG  (0x0002)    /* ADC10OVIFG */
#define ADC10IV_ADC10TOVIFG (0x0004)    /* ADC10TOVIFG */
#define ADC10IV_ADC10HIIFG  (0x0006)    /* ADC10HIIFG */
#define ADC10IV_ADC10LOIFG  (0x0008)    /* ADC10LOIFG */
#define ADC10IV_ADC10INIFG  (0x000A)    /* ADC10INIFG */
#define ADC10IV_ADC10IFG    (0x000C)    /* ADC10IFG */

/************************************************************
* Auxilary Supply
************************************************************/
#define __MSP430_HAS_AUX_SUPPLY__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_AUX_SUPPLY__ 0x09E0
#define AUX_SUPPLY_BASE __MSP430_BASEADDRESS_AUX_SUPPLY__

#define AUXCTL0_              0x09E0    /* Auxiliary Supply Control register 0 */
sfrb(AUXCTL0_L , AUXCTL0_);
sfrb(AUXCTL0_H , AUXCTL0_+1);
sfrw(AUXCTL0, AUXCTL0_);
#define AUXCTL1_              0x09E2    /* Auxiliary Supply Control register 1 */
sfrb(AUXCTL1_L , AUXCTL1_);
sfrb(AUXCTL1_H , AUXCTL1_+1);
sfrw(AUXCTL1, AUXCTL1_);
#define AUXCTL2_              0x09E4    /* Auxiliary Supply Control register 2 */
sfrb(AUXCTL2_L , AUXCTL2_);
sfrb(AUXCTL2_H , AUXCTL2_+1);
sfrw(AUXCTL2, AUXCTL2_);

#define AUX2CHCTL_            0x09F2    /* AUX2 Charger Control register */
sfrb(AUX2CHCTL_L , AUX2CHCTL_);
sfrb(AUX2CHCTL_H , AUX2CHCTL_+1);
sfrw(AUX2CHCTL, AUX2CHCTL_);
#define AUX3CHCTL_            0x09F4    /* AUX3 Charger Control register */
sfrb(AUX3CHCTL_L , AUX3CHCTL_);
sfrb(AUX3CHCTL_H , AUX3CHCTL_+1);
sfrw(AUX3CHCTL, AUX3CHCTL_);
#define AUXADCCTL_            0x09F6    /* AUX ADC Control */
sfrb(AUXADCCTL_L , AUXADCCTL_);
sfrb(AUXADCCTL_H , AUXADCCTL_+1);
sfrw(AUXADCCTL, AUXADCCTL_);
#define AUXIFG_               0x09FA    /* AUX Interrupt Flag */
sfrb(AUXIFG_L , AUXIFG_);
sfrb(AUXIFG_H , AUXIFG_+1);
sfrw(AUXIFG, AUXIFG_);
#define AUXIE_                0x09FC    /* AUX Interrupt Enable */
sfrb(AUXIE_L , AUXIE_);
sfrb(AUXIE_H , AUXIE_+1);
sfrw(AUXIE, AUXIE_);
#define AUXIV_                0x09FE    /* AUX Interrupt Vector Word  */
sfrb(AUXIV_L , AUXIV_);
sfrb(AUXIV_H , AUXIV_+1);
sfrw(AUXIV, AUXIV_);


/* AUXCTL0 Control Bits */
#define LOCKAUX             (0x0001)  /* Lock auxiliary supply system. */
#define AUX0SW              (0x0002)  /* DVCC switch state. */
#define AUX1SW              (0x0004)  /* AUX1 switch state. */
#define AUX2SW              (0x0008)  /* AUX2 switch state. */

/* AUXCTL0 Control Bits */
#define LOCKAUX_L           (0x0001)  /* Lock auxiliary supply system. */
#define AUX0SW_L            (0x0002)  /* DVCC switch state. */
#define AUX1SW_L            (0x0004)  /* AUX1 switch state. */
#define AUX2SW_L            (0x0008)  /* AUX2 switch state. */


#define AUXKEY              (0xA500)  /* AUX Key */
#define AUXKEY_H            (0xA5)    /* AUX Key (high word access) */

/* AUXCTL1 Control Bits */
#define AUX0OK              (0x0001)  /* DVCC okay flag. */
#define AUX1OK              (0x0002)  /* AUX 1 supply okay flag. */
#define AUX2OK              (0x0004)  /* AUX 2 supply okay flag. */
#define AUX2PRIO            (0x0008)  /* Auxiliary supply AUX2 priority. */
#define AUX0MD              (0x0100)  /* DVCC supply mode. */
#define AUX1MD              (0x0200)  /* AUX1 supply mode. */
#define AUX2MD              (0x0400)  /* AUX2 supply mode. */

/* AUXCTL1 Control Bits */
#define AUX0OK_L            (0x0001)  /* DVCC okay flag. */
#define AUX1OK_L            (0x0002)  /* AUX 1 supply okay flag. */
#define AUX2OK_L            (0x0004)  /* AUX 2 supply okay flag. */
#define AUX2PRIO_L          (0x0008)  /* Auxiliary supply AUX2 priority. */

/* AUXCTL1 Control Bits */
#define AUX0MD_H            (0x0001)  /* DVCC supply mode. */
#define AUX1MD_H            (0x0002)  /* AUX1 supply mode. */
#define AUX2MD_H            (0x0004)  /* AUX2 supply mode. */

/* AUXCTL2 Control Bits */
#define AUX0LVL0            (0x0001)  /* DVCC supply threshold level Bit: 0 */
#define AUX0LVL1            (0x0002)  /* DVCC supply threshold level Bit: 1 */
#define AUX0LVL2            (0x0004)  /* DVCC supply threshold level Bit: 2 */
#define AUX1LVL0            (0x0010)  /* AUX1 supply threshold level Bit: 0 */
#define AUX1LVL1            (0x0020)  /* AUX1 supply threshold level Bit: 1 */
#define AUX1LVL2            (0x0040)  /* AUX1 supply threshold level Bit: 2 */
#define AUX2LVL0            (0x0100)  /* AUX2 supply threshold level Bit: 0 */
#define AUX2LVL1            (0x0200)  /* AUX2 supply threshold level Bit: 1 */
#define AUX2LVL2            (0x0400)  /* AUX2 supply threshold level Bit: 2 */
#define AUXMR0              (0x1000)  /* Auxiliary supply monitoring rate Bit: 0 */
#define AUXMR1              (0x2000)  /* Auxiliary supply monitoring rate Bit: 1 */

/* AUXCTL2 Control Bits */
#define AUX0LVL0_L          (0x0001)  /* DVCC supply threshold level Bit: 0 */
#define AUX0LVL1_L          (0x0002)  /* DVCC supply threshold level Bit: 1 */
#define AUX0LVL2_L          (0x0004)  /* DVCC supply threshold level Bit: 2 */
#define AUX1LVL0_L          (0x0010)  /* AUX1 supply threshold level Bit: 0 */
#define AUX1LVL1_L          (0x0020)  /* AUX1 supply threshold level Bit: 1 */
#define AUX1LVL2_L          (0x0040)  /* AUX1 supply threshold level Bit: 2 */

/* AUXCTL2 Control Bits */
#define AUX2LVL0_H          (0x0001)  /* AUX2 supply threshold level Bit: 0 */
#define AUX2LVL1_H          (0x0002)  /* AUX2 supply threshold level Bit: 1 */
#define AUX2LVL2_H          (0x0004)  /* AUX2 supply threshold level Bit: 2 */
#define AUXMR0_H            (0x0010)  /* Auxiliary supply monitoring rate Bit: 0 */
#define AUXMR1_H            (0x0020)  /* Auxiliary supply monitoring rate Bit: 1 */

#define AUX0LVL_0           (0x0000)  /* DVCC supply threshold level: 0 */
#define AUX0LVL_1           (0x0001)  /* DVCC supply threshold level: 1 */
#define AUX0LVL_2           (0x0002)  /* DVCC supply threshold level: 2 */
#define AUX0LVL_3           (0x0003)  /* DVCC supply threshold level: 3 */
#define AUX0LVL_4           (0x0004)  /* DVCC supply threshold level: 4 */
#define AUX0LVL_5           (0x0005)  /* DVCC supply threshold level: 5 */
#define AUX0LVL_6           (0x0006)  /* DVCC supply threshold level: 6 */
#define AUX0LVL_7           (0x0007)  /* DVCC supply threshold level: 7 */

#define AUX1LVL_0           (0x0000)  /* AUX1 supply threshold level: 0 */
#define AUX1LVL_1           (0x0010)  /* AUX1 supply threshold level: 1 */
#define AUX1LVL_2           (0x0020)  /* AUX1 supply threshold level: 2 */
#define AUX1LVL_3           (0x0030)  /* AUX1 supply threshold level: 3 */
#define AUX1LVL_4           (0x0040)  /* AUX1 supply threshold level: 4 */
#define AUX1LVL_5           (0x0050)  /* AUX1 supply threshold level: 5 */
#define AUX1LVL_6           (0x0060)  /* AUX1 supply threshold level: 6 */
#define AUX1LVL_7           (0x0070)  /* AUX1 supply threshold level: 7 */

#define AUX2LVL_0           (0x0000)  /* AUX2 supply threshold level: 0 */
#define AUX2LVL_1           (0x0100)  /* AUX2 supply threshold level: 1 */
#define AUX2LVL_2           (0x0200)  /* AUX2 supply threshold level: 2 */
#define AUX2LVL_3           (0x0300)  /* AUX2 supply threshold level: 3 */
#define AUX2LVL_4           (0x0400)  /* AUX2 supply threshold level: 4 */
#define AUX2LVL_5           (0x0500)  /* AUX2 supply threshold level: 5 */
#define AUX2LVL_6           (0x0600)  /* AUX2 supply threshold level: 6 */
#define AUX2LVL_7           (0x0700)  /* AUX2 supply threshold level: 7 */

#define AUXMR_0             (0x0000)  /* Auxiliary supply monitoring rate: 0 */
#define AUXMR_1             (0x1000)  /* Auxiliary supply monitoring rate: 1 */
#define AUXMR_2             (0x2000)  /* Auxiliary supply monitoring rate: 2 */
#define AUXMR_3             (0x3000)  /* Auxiliary supply monitoring rate: 3 */


/* AUXADCCTL Control Bits */
#define AUXADC              (0x0001)  /* Auxiliary supplies to ADC */
#define AUXADCSEL0          (0x0002)  /* Select supply to be measured with ADC Bit: 0 */
#define AUXADCSEL1          (0x0004)  /* Select supply to be measured with ADC Bit: 1 */
#define AUXADCR0            (0x0010)  /* Load resistance Bit: 0 */
#define AUXADCR1            (0x0020)  /* Load resistance Bit: 1 */

/* AUXADCCTL Control Bits */
#define AUXADC_L            (0x0001)  /* Auxiliary supplies to ADC */
#define AUXADCSEL0_L        (0x0002)  /* Select supply to be measured with ADC Bit: 0 */
#define AUXADCSEL1_L        (0x0004)  /* Select supply to be measured with ADC Bit: 1 */
#define AUXADCR0_L          (0x0010)  /* Load resistance Bit: 0 */
#define AUXADCR1_L          (0x0020)  /* Load resistance Bit: 1 */


#define AUXADCSEL_0         (0x0000)  /* Select supply to be measured with ADC: DVCC */
#define AUXADCSEL_1         (0x0002)  /* Select supply to be measured with ADC: AUXVCC1 */
#define AUXADCSEL_2         (0x0004)  /* Select supply to be measured with ADC: AUXVCC2 */
#define AUXADCSEL_3         (0x0006)  /* Select supply to be measured with ADC: AUXVCC3 */
#define AUXADCSEL__DVCC     (0x0000)  /* Select supply to be measured with ADC: DVCC */
#define AUXADCSEL__AUXVCC1  (0x0002)  /* Select supply to be measured with ADC: AUXVCC1 */
#define AUXADCSEL__AUXVCC2  (0x0004)  /* Select supply to be measured with ADC: AUXVCC2 */
#define AUXADCSEL__AUXVCC3  (0x0006)  /* Select supply to be measured with ADC: AUXVCC3 */

#define AUXADCR_0           (0x0000)  /* Load resistance: 0 */
#define AUXADCR_1           (0x0010)  /* Load resistance: 1 */
#define AUXADCR_2           (0x0020)  /* Load resistance: 2 */
#define AUXADCR_3           (0x0030)  /* Load resistance: 3 */


/* AUXxCHCTL Control Bits */
#define AUXCHEN             (0x0001)  /* Lock auxiliary supply system. */
#define AUXCHC0             (0x0002)  /* Charger charge current Bit: 0 */
#define AUXCHC1             (0x0004)  /* Charger charge current Bit: 1 */
#define AUXCHV0             (0x0010)  /* Charger end voltage Bit: 0 */
#define AUXCHV1             (0x0020)  /* Charger end voltage Bit: 1 */

/* AUXxCHCTL Control Bits */
#define AUXCHEN_L           (0x0001)  /* Lock auxiliary supply system. */
#define AUXCHC0_L           (0x0002)  /* Charger charge current Bit: 0 */
#define AUXCHC1_L           (0x0004)  /* Charger charge current Bit: 1 */
#define AUXCHV0_L           (0x0010)  /* Charger end voltage Bit: 0 */
#define AUXCHV1_L           (0x0020)  /* Charger end voltage Bit: 1 */


#define AUXCHKEY            (0x6900)  /* Charger Access Key */

#define AUXCHC_0            (0x0000)  /* Charger charge current: 0 */
#define AUXCHC_1            (0x0002)  /* Charger charge current: 1 */
#define AUXCHC_2            (0x0004)  /* Charger charge current: 2 */
#define AUXCHC_3            (0x0006)  /* Charger charge current: 3 */

#define AUXCHV_0            (0x0000)  /* Charger end voltage: 0 */
#define AUXCHV_1            (0x0010)  /* Charger end voltage: 1 */
#define AUXCHV_2            (0x0020)  /* Charger end voltage: 2 */
#define AUXCHV_3            (0x0030)  /* Charger end voltage: 3 */


/* AUXIFG Control Bits */
#define AUX0SWIFG           (0x0001)  /* Switched to DVCC interrupt flag */
#define AUX1SWIFG           (0x0002)  /* Switched to AUX1 interrupt flag */
#define AUX2SWIFG           (0x0004)  /* Switched to AUX2 interrupt flag */
#define AUX0DRPIFG          (0x0010)  /* DVCC dropped below its threshold interrupt flag */
#define AUX1DRPIFG          (0x0020)  /* AUX1 dropped below its threshold interrupt flag */
#define AUX2DRPIFG          (0x0040)  /* AUX2 dropped below its threshold interrupt flag */
#define AUXMONIFG           (0x0080)  /* Supply monitor interrupt flag */
#define AUXSWNMIFG          (0x0100)  /* Supplies switched (non-)maskable interrupt flag */


/* AUXIFG Control Bits */
#define AUX0SWIFG_L         (0x0001)  /* Switched to DVCC interrupt flag */
#define AUX1SWIFG_L         (0x0002)  /* Switched to AUX1 interrupt flag */
#define AUX2SWIFG_L         (0x0004)  /* Switched to AUX2 interrupt flag */
#define AUX0DRPIFG_L        (0x0010)  /* DVCC dropped below its threshold interrupt flag */
#define AUX1DRPIFG_L        (0x0020)  /* AUX1 dropped below its threshold interrupt flag */
#define AUX2DRPIFG_L        (0x0040)  /* AUX2 dropped below its threshold interrupt flag */
#define AUXMONIFG_L         (0x0080)  /* Supply monitor interrupt flag */


/* AUXIFG Control Bits */
#define AUXSWNMIFG_H        (0x0001)  /* Supplies switched (non-)maskable interrupt flag */



/* AUXIE Control Bits */
#define AUX0SWIE            (0x0001)  /* Switched to DVCC interrupt enable */
#define AUX1SWIE            (0x0002)  /* Switched to AUX1 interrupt enable */
#define AUX2SWIE            (0x0004)  /* Switched to AUX2 interrupt enable */
#define AUXSWGIE            (0x0008)  /* Global supply switched interrupt enable. */
#define AUX0DRPIE           (0x0010)  /* DVCC dropped below its threshold interrupt enable */
#define AUX1DRPIE           (0x0020)  /* AUX1 dropped below its threshold interrupt enable */
#define AUX2DRPIE           (0x0040)  /* AUX2 dropped below its threshold interrupt enable */
#define AUXMONIE            (0x0080)  /* Supply monitor interrupt enable */
#define AUXSWNMIE           (0x0100)  /* Supplies switched (non-)maskable interrupt enable */

/* AUXIE Control Bits */
#define AUX0SWIE_L          (0x0001)  /* Switched to DVCC interrupt enable */
#define AUX1SWIE_L          (0x0002)  /* Switched to AUX1 interrupt enable */
#define AUX2SWIE_L          (0x0004)  /* Switched to AUX2 interrupt enable */
#define AUXSWGIE_L          (0x0008)  /* Global supply switched interrupt enable. */
#define AUX0DRPIE_L         (0x0010)  /* DVCC dropped below its threshold interrupt enable */
#define AUX1DRPIE_L         (0x0020)  /* AUX1 dropped below its threshold interrupt enable */
#define AUX2DRPIE_L         (0x0040)  /* AUX2 dropped below its threshold interrupt enable */
#define AUXMONIE_L          (0x0080)  /* Supply monitor interrupt enable */

/* AUXIE Control Bits */
#define AUXSWNMIE_H         (0x0001)  /* Supplies switched (non-)maskable interrupt enable */


/* AUXIV Definitions */
#define AUXIV_NONE        (0x0000)    /* No Interrupt pending */
#define AUXIV_AUXSWNMIFG  (0x0002)    /* AUXSWNMIFG */
#define AUXIV_AUX0SWIFG   (0x0004)    /* AUX0SWIFG */
#define AUXIV_AUX1SWIFG   (0x0006)    /* AUX1SWIFG */
#define AUXIV_AUX2SWIFG   (0x0008)    /* AUX2SWIFG */
#define AUXIV_AUX0DRPIFG  (0x000A)    /* AUX0DRPIFG */
#define AUXIV_AUX1DRPIFG  (0x000C)    /* AUX1DRPIFG */
#define AUXIV_AUX2DRPIFG  (0x000E)    /* AUX2DRPIFG */
#define AUXIV_AUXMONIFG   (0x0010)    /* AUXMONIFG */

/*************************************************************
* Backup RAM Module
*************************************************************/
#define __MSP430_HAS_BACKUP_RAM__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_BACKUP_RAM__ 0x0480
#define BAK_RAM_BASE __MSP430_BASEADDRESS_BACKUP_RAM__

#define BAKMEM0_              0x0480    /* Battery Backup Memory 0 */
sfrb(BAKMEM0_L , BAKMEM0_);
sfrb(BAKMEM0_H , BAKMEM0_+1);
sfrw(BAKMEM0, BAKMEM0_);
#define BAKMEM1_              0x0482    /* Battery Backup Memory 1 */
sfrb(BAKMEM1_L , BAKMEM1_);
sfrb(BAKMEM1_H , BAKMEM1_+1);
sfrw(BAKMEM1, BAKMEM1_);
#define BAKMEM2_              0x0484    /* Battery Backup Memory 2 */
sfrb(BAKMEM2_L , BAKMEM2_);
sfrb(BAKMEM2_H , BAKMEM2_+1);
sfrw(BAKMEM2, BAKMEM2_);
#define BAKMEM3_              0x0486    /* Battery Backup Memory 3 */
sfrb(BAKMEM3_L , BAKMEM3_);
sfrb(BAKMEM3_H , BAKMEM3_+1);
sfrw(BAKMEM3, BAKMEM3_);

/************************************************************
* Comparator B
************************************************************/
#define __MSP430_HAS_COMPB__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_COMPB__ 0x08C0
#define COMP_B_BASE __MSP430_BASEADDRESS_COMPB__

#define CBCTL0_               0x08C0    /* Comparator B Control Register 0 */
sfrb(CBCTL0_L , CBCTL0_);
sfrb(CBCTL0_H , CBCTL0_+1);
sfrw(CBCTL0, CBCTL0_);
#define CBCTL1_               0x08C2    /* Comparator B Control Register 1 */
sfrb(CBCTL1_L , CBCTL1_);
sfrb(CBCTL1_H , CBCTL1_+1);
sfrw(CBCTL1, CBCTL1_);
#define CBCTL2_               0x08C4    /* Comparator B Control Register 2 */
sfrb(CBCTL2_L , CBCTL2_);
sfrb(CBCTL2_H , CBCTL2_+1);
sfrw(CBCTL2, CBCTL2_);
#define CBCTL3_               0x08C6    /* Comparator B Control Register 3 */
sfrb(CBCTL3_L , CBCTL3_);
sfrb(CBCTL3_H , CBCTL3_+1);
sfrw(CBCTL3, CBCTL3_);
#define CBINT_                0x08CC    /* Comparator B Interrupt Register */
sfrb(CBINT_L , CBINT_);
sfrb(CBINT_H , CBINT_+1);
sfrw(CBINT, CBINT_);
#define CBIV_                 0x08CE    /* Comparator B Interrupt Vector Word */
sfrw(CBIV, CBIV_);


/* CBCTL0 Control Bits */
#define CBIPSEL0            (0x0001)  /* Comp. B Pos. Channel Input Select 0 */
#define CBIPSEL1            (0x0002)  /* Comp. B Pos. Channel Input Select 1 */
#define CBIPSEL2            (0x0004)  /* Comp. B Pos. Channel Input Select 2 */
#define CBIPSEL3            (0x0008)  /* Comp. B Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010)  /* Comp. B */
//#define RESERVED            (0x0020)  /* Comp. B */
//#define RESERVED            (0x0040)  /* Comp. B */
#define CBIPEN              (0x0080)  /* Comp. B Pos. Channel Input Enable */
#define CBIMSEL0            (0x0100)  /* Comp. B Neg. Channel Input Select 0 */
#define CBIMSEL1            (0x0200)  /* Comp. B Neg. Channel Input Select 1 */
#define CBIMSEL2            (0x0400)  /* Comp. B Neg. Channel Input Select 2 */
#define CBIMSEL3            (0x0800)  /* Comp. B Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000)  /* Comp. B */
//#define RESERVED            (0x2000)  /* Comp. B */
//#define RESERVED            (0x4000)  /* Comp. B */
#define CBIMEN              (0x8000)  /* Comp. B Neg. Channel Input Enable */

/* CBCTL0 Control Bits */
#define CBIPSEL0_L          (0x0001)  /* Comp. B Pos. Channel Input Select 0 */
#define CBIPSEL1_L          (0x0002)  /* Comp. B Pos. Channel Input Select 1 */
#define CBIPSEL2_L          (0x0004)  /* Comp. B Pos. Channel Input Select 2 */
#define CBIPSEL3_L          (0x0008)  /* Comp. B Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010)  /* Comp. B */
//#define RESERVED            (0x0020)  /* Comp. B */
//#define RESERVED            (0x0040)  /* Comp. B */
#define CBIPEN_L            (0x0080)  /* Comp. B Pos. Channel Input Enable */
//#define RESERVED            (0x1000)  /* Comp. B */
//#define RESERVED            (0x2000)  /* Comp. B */
//#define RESERVED            (0x4000)  /* Comp. B */

/* CBCTL0 Control Bits */
//#define RESERVED            (0x0010)  /* Comp. B */
//#define RESERVED            (0x0020)  /* Comp. B */
//#define RESERVED            (0x0040)  /* Comp. B */
#define CBIMSEL0_H          (0x0001)  /* Comp. B Neg. Channel Input Select 0 */
#define CBIMSEL1_H          (0x0002)  /* Comp. B Neg. Channel Input Select 1 */
#define CBIMSEL2_H          (0x0004)  /* Comp. B Neg. Channel Input Select 2 */
#define CBIMSEL3_H          (0x0008)  /* Comp. B Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000)  /* Comp. B */
//#define RESERVED            (0x2000)  /* Comp. B */
//#define RESERVED            (0x4000)  /* Comp. B */
#define CBIMEN_H            (0x0080)  /* Comp. B Neg. Channel Input Enable */

#define CBIPSEL_0           (0x0000)  /* Comp. B V+ terminal Input Select: Channel 0 */
#define CBIPSEL_1           (0x0001)  /* Comp. B V+ terminal Input Select: Channel 1 */
#define CBIPSEL_2           (0x0002)  /* Comp. B V+ terminal Input Select: Channel 2 */
#define CBIPSEL_3           (0x0003)  /* Comp. B V+ terminal Input Select: Channel 3 */
#define CBIPSEL_4           (0x0004)  /* Comp. B V+ terminal Input Select: Channel 4 */
#define CBIPSEL_5           (0x0005)  /* Comp. B V+ terminal Input Select: Channel 5 */
#define CBIPSEL_6           (0x0006)  /* Comp. B V+ terminal Input Select: Channel 6 */
#define CBIPSEL_7           (0x0007)  /* Comp. B V+ terminal Input Select: Channel 7 */
#define CBIPSEL_8           (0x0008)  /* Comp. B V+ terminal Input Select: Channel 8 */
#define CBIPSEL_9           (0x0009)  /* Comp. B V+ terminal Input Select: Channel 9 */
#define CBIPSEL_10          (0x000A)  /* Comp. B V+ terminal Input Select: Channel 10 */
#define CBIPSEL_11          (0x000B)  /* Comp. B V+ terminal Input Select: Channel 11 */
#define CBIPSEL_12          (0x000C)  /* Comp. B V+ terminal Input Select: Channel 12 */
#define CBIPSEL_13          (0x000D)  /* Comp. B V+ terminal Input Select: Channel 13 */
#define CBIPSEL_14          (0x000E)  /* Comp. B V+ terminal Input Select: Channel 14 */
#define CBIPSEL_15          (0x000F)  /* Comp. B V+ terminal Input Select: Channel 15 */

#define CBIMSEL_0           (0x0000)  /* Comp. B V- Terminal Input Select: Channel 0 */
#define CBIMSEL_1           (0x0100)  /* Comp. B V- Terminal Input Select: Channel 1 */
#define CBIMSEL_2           (0x0200)  /* Comp. B V- Terminal Input Select: Channel 2 */
#define CBIMSEL_3           (0x0300)  /* Comp. B V- Terminal Input Select: Channel 3 */
#define CBIMSEL_4           (0x0400)  /* Comp. B V- Terminal Input Select: Channel 4 */
#define CBIMSEL_5           (0x0500)  /* Comp. B V- Terminal Input Select: Channel 5 */
#define CBIMSEL_6           (0x0600)  /* Comp. B V- Terminal Input Select: Channel 6 */
#define CBIMSEL_7           (0x0700)  /* Comp. B V- Terminal Input Select: Channel 7 */
#define CBIMSEL_8           (0x0800)  /* Comp. B V- terminal Input Select: Channel 8 */
#define CBIMSEL_9           (0x0900)  /* Comp. B V- terminal Input Select: Channel 9 */
#define CBIMSEL_10          (0x0A00)  /* Comp. B V- terminal Input Select: Channel 10 */
#define CBIMSEL_11          (0x0B00)  /* Comp. B V- terminal Input Select: Channel 11 */
#define CBIMSEL_12          (0x0C00)  /* Comp. B V- terminal Input Select: Channel 12 */
#define CBIMSEL_13          (0x0D00)  /* Comp. B V- terminal Input Select: Channel 13 */
#define CBIMSEL_14          (0x0E00)  /* Comp. B V- terminal Input Select: Channel 14 */
#define CBIMSEL_15          (0x0F00)  /* Comp. B V- terminal Input Select: Channel 15 */

/* CBCTL1 Control Bits */
#define CBOUT               (0x0001)  /* Comp. B Output */
#define CBOUTPOL            (0x0002)  /* Comp. B Output Polarity */
#define CBF                 (0x0004)  /* Comp. B Enable Output Filter */
#define CBIES               (0x0008)  /* Comp. B Interrupt Edge Select */
#define CBSHORT             (0x0010)  /* Comp. B Input Short */
#define CBEX                (0x0020)  /* Comp. B Exchange Inputs */
#define CBFDLY0             (0x0040)  /* Comp. B Filter delay Bit 0 */
#define CBFDLY1             (0x0080)  /* Comp. B Filter delay Bit 1 */
#define CBPWRMD0            (0x0100)  /* Comp. B Power Mode Bit 0 */
#define CBPWRMD1            (0x0200)  /* Comp. B Power Mode Bit 1 */
#define CBON                (0x0400)  /* Comp. B enable */
#define CBMRVL              (0x0800)  /* Comp. B CBMRV Level */
#define CBMRVS              (0x1000)  /* Comp. B Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000)  /* Comp. B */
//#define RESERVED            (0x4000)  /* Comp. B */
//#define RESERVED            (0x8000)  /* Comp. B */

/* CBCTL1 Control Bits */
#define CBOUT_L             (0x0001)  /* Comp. B Output */
#define CBOUTPOL_L          (0x0002)  /* Comp. B Output Polarity */
#define CBF_L               (0x0004)  /* Comp. B Enable Output Filter */
#define CBIES_L             (0x0008)  /* Comp. B Interrupt Edge Select */
#define CBSHORT_L           (0x0010)  /* Comp. B Input Short */
#define CBEX_L              (0x0020)  /* Comp. B Exchange Inputs */
#define CBFDLY0_L           (0x0040)  /* Comp. B Filter delay Bit 0 */
#define CBFDLY1_L           (0x0080)  /* Comp. B Filter delay Bit 1 */
//#define RESERVED            (0x2000)  /* Comp. B */
//#define RESERVED            (0x4000)  /* Comp. B */
//#define RESERVED            (0x8000)  /* Comp. B */

/* CBCTL1 Control Bits */
#define CBPWRMD0_H          (0x0001)  /* Comp. B Power Mode Bit 0 */
#define CBPWRMD1_H          (0x0002)  /* Comp. B Power Mode Bit 1 */
#define CBON_H              (0x0004)  /* Comp. B enable */
#define CBMRVL_H            (0x0008)  /* Comp. B CBMRV Level */
#define CBMRVS_H            (0x0010)  /* Comp. B Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000)  /* Comp. B */
//#define RESERVED            (0x4000)  /* Comp. B */
//#define RESERVED            (0x8000)  /* Comp. B */

#define CBFDLY_0           (0x0000)  /* Comp. B Filter delay 0 : 450ns */
#define CBFDLY_1           (0x0040)  /* Comp. B Filter delay 1 : 900ns */
#define CBFDLY_2           (0x0080)  /* Comp. B Filter delay 2 : 1800ns */
#define CBFDLY_3           (0x00C0)  /* Comp. B Filter delay 3 : 3600ns */

#define CBPWRMD_0           (0x0000)  /* Comp. B Power Mode 0 : High speed */
#define CBPWRMD_1           (0x0100)  /* Comp. B Power Mode 1 : Normal */
#define CBPWRMD_2           (0x0200)  /* Comp. B Power Mode 2 : Ultra-Low*/
#define CBPWRMD_3           (0x0300)  /* Comp. B Power Mode 3 : Reserved */


/* CBCTL2 Control Bits */
#define CBREF00             (0x0001)  /* Comp. B Reference 0 Resistor Select Bit : 0 */
#define CBREF01             (0x0002)  /* Comp. B Reference 0 Resistor Select Bit : 1 */
#define CBREF02             (0x0004)  /* Comp. B Reference 0 Resistor Select Bit : 2 */
#define CBREF03             (0x0008)  /* Comp. B Reference 0 Resistor Select Bit : 3 */
#define CBREF04             (0x0010)  /* Comp. B Reference 0 Resistor Select Bit : 4 */
#define CBRSEL              (0x0020)  /* Comp. B Reference select */
#define CBRS0               (0x0040)  /* Comp. B Reference Source Bit : 0 */
#define CBRS1               (0x0080)  /* Comp. B Reference Source Bit : 1 */
#define CBREF10             (0x0100)  /* Comp. B Reference 1 Resistor Select Bit : 0 */
#define CBREF11             (0x0200)  /* Comp. B Reference 1 Resistor Select Bit : 1 */
#define CBREF12             (0x0400)  /* Comp. B Reference 1 Resistor Select Bit : 2 */
#define CBREF13             (0x0800)  /* Comp. B Reference 1 Resistor Select Bit : 3 */
#define CBREF14             (0x1000)  /* Comp. B Reference 1 Resistor Select Bit : 4 */
#define CBREFL0             (0x2000)  /* Comp. B Reference voltage level Bit : 0 */
#define CBREFL1             (0x4000)  /* Comp. B Reference voltage level Bit : 1 */
#define CBREFACC            (0x8000)  /* Comp. B Reference Accuracy */

/* CBCTL2 Control Bits */
#define CBREF00_L           (0x0001)  /* Comp. B Reference 0 Resistor Select Bit : 0 */
#define CBREF01_L           (0x0002)  /* Comp. B Reference 0 Resistor Select Bit : 1 */
#define CBREF02_L           (0x0004)  /* Comp. B Reference 0 Resistor Select Bit : 2 */
#define CBREF03_L           (0x0008)  /* Comp. B Reference 0 Resistor Select Bit : 3 */
#define CBREF04_L           (0x0010)  /* Comp. B Reference 0 Resistor Select Bit : 4 */
#define CBRSEL_L            (0x0020)  /* Comp. B Reference select */
#define CBRS0_L             (0x0040)  /* Comp. B Reference Source Bit : 0 */
#define CBRS1_L             (0x0080)  /* Comp. B Reference Source Bit : 1 */

/* CBCTL2 Control Bits */
#define CBREF10_H           (0x0001)  /* Comp. B Reference 1 Resistor Select Bit : 0 */
#define CBREF11_H           (0x0002)  /* Comp. B Reference 1 Resistor Select Bit : 1 */
#define CBREF12_H           (0x0004)  /* Comp. B Reference 1 Resistor Select Bit : 2 */
#define CBREF13_H           (0x0008)  /* Comp. B Reference 1 Resistor Select Bit : 3 */
#define CBREF14_H           (0x0010)  /* Comp. B Reference 1 Resistor Select Bit : 4 */
#define CBREFL0_H           (0x0020)  /* Comp. B Reference voltage level Bit : 0 */
#define CBREFL1_H           (0x0040)  /* Comp. B Reference voltage level Bit : 1 */
#define CBREFACC_H          (0x0080)  /* Comp. B Reference Accuracy */

#define CBREF0_0            (0x0000)  /* Comp. B Int. Ref.0 Select 0 : 1/32 */
#define CBREF0_1            (0x0001)  /* Comp. B Int. Ref.0 Select 1 : 2/32 */
#define CBREF0_2            (0x0002)  /* Comp. B Int. Ref.0 Select 2 : 3/32 */
#define CBREF0_3            (0x0003)  /* Comp. B Int. Ref.0 Select 3 : 4/32 */
#define CBREF0_4            (0x0004)  /* Comp. B Int. Ref.0 Select 4 : 5/32 */
#define CBREF0_5            (0x0005)  /* Comp. B Int. Ref.0 Select 5 : 6/32 */
#define CBREF0_6            (0x0006)  /* Comp. B Int. Ref.0 Select 6 : 7/32 */
#define CBREF0_7            (0x0007)  /* Comp. B Int. Ref.0 Select 7 : 8/32 */
#define CBREF0_8            (0x0008)  /* Comp. B Int. Ref.0 Select 0 : 9/32 */
#define CBREF0_9            (0x0009)  /* Comp. B Int. Ref.0 Select 1 : 10/32 */
#define CBREF0_10           (0x000A)  /* Comp. B Int. Ref.0 Select 2 : 11/32 */
#define CBREF0_11           (0x000B)  /* Comp. B Int. Ref.0 Select 3 : 12/32 */
#define CBREF0_12           (0x000C)  /* Comp. B Int. Ref.0 Select 4 : 13/32 */
#define CBREF0_13           (0x000D)  /* Comp. B Int. Ref.0 Select 5 : 14/32 */
#define CBREF0_14           (0x000E)  /* Comp. B Int. Ref.0 Select 6 : 15/32 */
#define CBREF0_15           (0x000F)  /* Comp. B Int. Ref.0 Select 7 : 16/32 */
#define CBREF0_16           (0x0010)  /* Comp. B Int. Ref.0 Select 0 : 17/32 */
#define CBREF0_17           (0x0011)  /* Comp. B Int. Ref.0 Select 1 : 18/32 */
#define CBREF0_18           (0x0012)  /* Comp. B Int. Ref.0 Select 2 : 19/32 */
#define CBREF0_19           (0x0013)  /* Comp. B Int. Ref.0 Select 3 : 20/32 */
#define CBREF0_20           (0x0014)  /* Comp. B Int. Ref.0 Select 4 : 21/32 */
#define CBREF0_21           (0x0015)  /* Comp. B Int. Ref.0 Select 5 : 22/32 */
#define CBREF0_22           (0x0016)  /* Comp. B Int. Ref.0 Select 6 : 23/32 */
#define CBREF0_23           (0x0017)  /* Comp. B Int. Ref.0 Select 7 : 24/32 */
#define CBREF0_24           (0x0018)  /* Comp. B Int. Ref.0 Select 0 : 25/32 */
#define CBREF0_25           (0x0019)  /* Comp. B Int. Ref.0 Select 1 : 26/32 */
#define CBREF0_26           (0x001A)  /* Comp. B Int. Ref.0 Select 2 : 27/32 */
#define CBREF0_27           (0x001B)  /* Comp. B Int. Ref.0 Select 3 : 28/32 */
#define CBREF0_28           (0x001C)  /* Comp. B Int. Ref.0 Select 4 : 29/32 */
#define CBREF0_29           (0x001D)  /* Comp. B Int. Ref.0 Select 5 : 30/32 */
#define CBREF0_30           (0x001E)  /* Comp. B Int. Ref.0 Select 6 : 31/32 */
#define CBREF0_31           (0x001F)  /* Comp. B Int. Ref.0 Select 7 : 32/32 */

#define CBRS_0              (0x0000)  /* Comp. B Reference Source 0 : Off */
#define CBRS_1              (0x0040)  /* Comp. B Reference Source 1 : Vcc */
#define CBRS_2              (0x0080)  /* Comp. B Reference Source 2 : Shared Ref. */
#define CBRS_3              (0x00C0)  /* Comp. B Reference Source 3 : Shared Ref. / Off */

#define CBREF1_0            (0x0000)  /* Comp. B Int. Ref.1 Select 0 : 1/32 */
#define CBREF1_1            (0x0100)  /* Comp. B Int. Ref.1 Select 1 : 2/32 */
#define CBREF1_2            (0x0200)  /* Comp. B Int. Ref.1 Select 2 : 3/32 */
#define CBREF1_3            (0x0300)  /* Comp. B Int. Ref.1 Select 3 : 4/32 */
#define CBREF1_4            (0x0400)  /* Comp. B Int. Ref.1 Select 4 : 5/32 */
#define CBREF1_5            (0x0500)  /* Comp. B Int. Ref.1 Select 5 : 6/32 */
#define CBREF1_6            (0x0600)  /* Comp. B Int. Ref.1 Select 6 : 7/32 */
#define CBREF1_7            (0x0700)  /* Comp. B Int. Ref.1 Select 7 : 8/32 */
#define CBREF1_8            (0x0800)  /* Comp. B Int. Ref.1 Select 0 : 9/32 */
#define CBREF1_9            (0x0900)  /* Comp. B Int. Ref.1 Select 1 : 10/32 */
#define CBREF1_10           (0x0A00)  /* Comp. B Int. Ref.1 Select 2 : 11/32 */
#define CBREF1_11           (0x0B00)  /* Comp. B Int. Ref.1 Select 3 : 12/32 */
#define CBREF1_12           (0x0C00)  /* Comp. B Int. Ref.1 Select 4 : 13/32 */
#define CBREF1_13           (0x0D00)  /* Comp. B Int. Ref.1 Select 5 : 14/32 */
#define CBREF1_14           (0x0E00)  /* Comp. B Int. Ref.1 Select 6 : 15/32 */
#define CBREF1_15           (0x0F00)  /* Comp. B Int. Ref.1 Select 7 : 16/32 */
#define CBREF1_16           (0x1000)  /* Comp. B Int. Ref.1 Select 0 : 17/32 */
#define CBREF1_17           (0x1100)  /* Comp. B Int. Ref.1 Select 1 : 18/32 */
#define CBREF1_18           (0x1200)  /* Comp. B Int. Ref.1 Select 2 : 19/32 */
#define CBREF1_19           (0x1300)  /* Comp. B Int. Ref.1 Select 3 : 20/32 */
#define CBREF1_20           (0x1400)  /* Comp. B Int. Ref.1 Select 4 : 21/32 */
#define CBREF1_21           (0x1500)  /* Comp. B Int. Ref.1 Select 5 : 22/32 */
#define CBREF1_22           (0x1600)  /* Comp. B Int. Ref.1 Select 6 : 23/32 */
#define CBREF1_23           (0x1700)  /* Comp. B Int. Ref.1 Select 7 : 24/32 */
#define CBREF1_24           (0x1800)  /* Comp. B Int. Ref.1 Select 0 : 25/32 */
#define CBREF1_25           (0x1900)  /* Comp. B Int. Ref.1 Select 1 : 26/32 */
#define CBREF1_26           (0x1A00)  /* Comp. B Int. Ref.1 Select 2 : 27/32 */
#define CBREF1_27           (0x1B00)  /* Comp. B Int. Ref.1 Select 3 : 28/32 */
#define CBREF1_28           (0x1C00)  /* Comp. B Int. Ref.1 Select 4 : 29/32 */
#define CBREF1_29           (0x1D00)  /* Comp. B Int. Ref.1 Select 5 : 30/32 */
#define CBREF1_30           (0x1E00)  /* Comp. B Int. Ref.1 Select 6 : 31/32 */
#define CBREF1_31           (0x1F00)  /* Comp. B Int. Ref.1 Select 7 : 32/32 */

#define CBREFL_0            (0x0000)  /* Comp. B Reference voltage level 0 : None */
#define CBREFL_1            (0x2000)  /* Comp. B Reference voltage level 1 : 1.5V */
#define CBREFL_2            (0x4000)  /* Comp. B Reference voltage level 2 : 2.0V  */
#define CBREFL_3            (0x6000)  /* Comp. B Reference voltage level 3 : 2.5V  */


#define CBPD0               (0x0001)  /* Comp. B Disable Input Buffer of Port Register .0 */
#define CBPD1               (0x0002)  /* Comp. B Disable Input Buffer of Port Register .1 */
#define CBPD2               (0x0004)  /* Comp. B Disable Input Buffer of Port Register .2 */
#define CBPD3               (0x0008)  /* Comp. B Disable Input Buffer of Port Register .3 */
#define CBPD4               (0x0010)  /* Comp. B Disable Input Buffer of Port Register .4 */
#define CBPD5               (0x0020)  /* Comp. B Disable Input Buffer of Port Register .5 */
#define CBPD6               (0x0040)  /* Comp. B Disable Input Buffer of Port Register .6 */
#define CBPD7               (0x0080)  /* Comp. B Disable Input Buffer of Port Register .7 */
#define CBPD8               (0x0100)  /* Comp. B Disable Input Buffer of Port Register .8 */
#define CBPD9               (0x0200)  /* Comp. B Disable Input Buffer of Port Register .9 */
#define CBPD10              (0x0400)  /* Comp. B Disable Input Buffer of Port Register .10 */
#define CBPD11              (0x0800)  /* Comp. B Disable Input Buffer of Port Register .11 */
#define CBPD12              (0x1000)  /* Comp. B Disable Input Buffer of Port Register .12 */
#define CBPD13              (0x2000)  /* Comp. B Disable Input Buffer of Port Register .13 */
#define CBPD14              (0x4000)  /* Comp. B Disable Input Buffer of Port Register .14 */
#define CBPD15              (0x8000)  /* Comp. B Disable Input Buffer of Port Register .15 */

#define CBPD0_L             (0x0001)  /* Comp. B Disable Input Buffer of Port Register .0 */
#define CBPD1_L             (0x0002)  /* Comp. B Disable Input Buffer of Port Register .1 */
#define CBPD2_L             (0x0004)  /* Comp. B Disable Input Buffer of Port Register .2 */
#define CBPD3_L             (0x0008)  /* Comp. B Disable Input Buffer of Port Register .3 */
#define CBPD4_L             (0x0010)  /* Comp. B Disable Input Buffer of Port Register .4 */
#define CBPD5_L             (0x0020)  /* Comp. B Disable Input Buffer of Port Register .5 */
#define CBPD6_L             (0x0040)  /* Comp. B Disable Input Buffer of Port Register .6 */
#define CBPD7_L             (0x0080)  /* Comp. B Disable Input Buffer of Port Register .7 */

#define CBPD8_H             (0x0001)  /* Comp. B Disable Input Buffer of Port Register .8 */
#define CBPD9_H             (0x0002)  /* Comp. B Disable Input Buffer of Port Register .9 */
#define CBPD10_H            (0x0004)  /* Comp. B Disable Input Buffer of Port Register .10 */
#define CBPD11_H            (0x0008)  /* Comp. B Disable Input Buffer of Port Register .11 */
#define CBPD12_H            (0x0010)  /* Comp. B Disable Input Buffer of Port Register .12 */
#define CBPD13_H            (0x0020)  /* Comp. B Disable Input Buffer of Port Register .13 */
#define CBPD14_H            (0x0040)  /* Comp. B Disable Input Buffer of Port Register .14 */
#define CBPD15_H            (0x0080)  /* Comp. B Disable Input Buffer of Port Register .15 */


/* CBINT Control Bits */
#define CBIFG                (0x0001)  /* Comp. B Interrupt Flag */
#define CBIIFG               (0x0002)  /* Comp. B Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004)  /* Comp. B */
//#define RESERVED             (0x0008)  /* Comp. B */
//#define RESERVED             (0x0010)  /* Comp. B */
//#define RESERVED             (0x0020)  /* Comp. B */
//#define RESERVED             (0x0040)  /* Comp. B */
//#define RESERVED             (0x0080)  /* Comp. B */
#define CBIE                 (0x0100)  /* Comp. B Interrupt Enable */
#define CBIIE                (0x0200)  /* Comp. B Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400)  /* Comp. B */
//#define RESERVED             (0x0800)  /* Comp. B */
//#define RESERVED             (0x1000)  /* Comp. B */
//#define RESERVED             (0x2000)  /* Comp. B */
//#define RESERVED             (0x4000)  /* Comp. B */
//#define RESERVED             (0x8000)  /* Comp. B */

/* CBINT Control Bits */
#define CBIFG_L             (0x0001)  /* Comp. B Interrupt Flag */
#define CBIIFG_L            (0x0002)  /* Comp. B Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004)  /* Comp. B */
//#define RESERVED             (0x0008)  /* Comp. B */
//#define RESERVED             (0x0010)  /* Comp. B */
//#define RESERVED             (0x0020)  /* Comp. B */
//#define RESERVED             (0x0040)  /* Comp. B */
//#define RESERVED             (0x0080)  /* Comp. B */
//#define RESERVED             (0x0400)  /* Comp. B */
//#define RESERVED             (0x0800)  /* Comp. B */
//#define RESERVED             (0x1000)  /* Comp. B */
//#define RESERVED             (0x2000)  /* Comp. B */
//#define RESERVED             (0x4000)  /* Comp. B */
//#define RESERVED             (0x8000)  /* Comp. B */

/* CBINT Control Bits */
//#define RESERVED             (0x0004)  /* Comp. B */
//#define RESERVED             (0x0008)  /* Comp. B */
//#define RESERVED             (0x0010)  /* Comp. B */
//#define RESERVED             (0x0020)  /* Comp. B */
//#define RESERVED             (0x0040)  /* Comp. B */
//#define RESERVED             (0x0080)  /* Comp. B */
#define CBIE_H              (0x0001)  /* Comp. B Interrupt Enable */
#define CBIIE_H             (0x0002)  /* Comp. B Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400)  /* Comp. B */
//#define RESERVED             (0x0800)  /* Comp. B */
//#define RESERVED             (0x1000)  /* Comp. B */
//#define RESERVED             (0x2000)  /* Comp. B */
//#define RESERVED             (0x4000)  /* Comp. B */
//#define RESERVED             (0x8000)  /* Comp. B */

/* CBIV Definitions */
#define CBIV_NONE           (0x0000)    /* No Interrupt pending */
#define CBIV_CBIFG          (0x0002)    /* CBIFG */
#define CBIV_CBIIFG         (0x0004)    /* CBIIFG */

/*************************************************************
* CRC Module
*************************************************************/
#define __MSP430_HAS_CRC__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CRC__ 0x0150
#define CRC_BASE __MSP430_BASEADDRESS_CRC__

#define CRCDI_                0x0150    /* CRC Data In Register */
sfrb(CRCDI_L , CRCDI_);
sfrb(CRCDI_H , CRCDI_+1);
sfrw(CRCDI, CRCDI_);
#define CRCDIRB_              0x0152    /* CRC data in reverse byte Register */
sfrb(CRCDIRB_L , CRCDIRB_);
sfrb(CRCDIRB_H , CRCDIRB_+1);
sfrw(CRCDIRB, CRCDIRB_);
#define CRCINIRES_            0x0154    /* CRC Initialisation Register and Result Register */
sfrb(CRCINIRES_L , CRCINIRES_);
sfrb(CRCINIRES_H , CRCINIRES_+1);
sfrw(CRCINIRES, CRCINIRES_);
#define CRCRESR_              0x0156    /* CRC reverse result Register */
sfrb(CRCRESR_L , CRCRESR_);
sfrb(CRCRESR_H , CRCRESR_+1);
sfrw(CRCRESR, CRCRESR_);

/************************************************************
* DMA_X
************************************************************/
#define __MSP430_HAS_DMAX_3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_DMAX_3__ 0x0500
#define DMA_BASE __MSP430_BASEADDRESS_DMAX_3__

#define DMACTL0_              0x0500    /* DMA Module Control 0 */
sfrb(DMACTL0_L , DMACTL0_);
sfrb(DMACTL0_H , DMACTL0_+1);
sfrw(DMACTL0, DMACTL0_);
#define DMACTL1_              0x0502    /* DMA Module Control 1 */
sfrb(DMACTL1_L , DMACTL1_);
sfrb(DMACTL1_H , DMACTL1_+1);
sfrw(DMACTL1, DMACTL1_);
#define DMACTL2_              0x0504    /* DMA Module Control 2 */
sfrb(DMACTL2_L , DMACTL2_);
sfrb(DMACTL2_H , DMACTL2_+1);
sfrw(DMACTL2, DMACTL2_);
#define DMACTL3_              0x0506    /* DMA Module Control 3 */
sfrb(DMACTL3_L , DMACTL3_);
sfrb(DMACTL3_H , DMACTL3_+1);
sfrw(DMACTL3, DMACTL3_);
#define DMACTL4_              0x0508    /* DMA Module Control 4 */
sfrb(DMACTL4_L , DMACTL4_);
sfrb(DMACTL4_H , DMACTL4_+1);
sfrw(DMACTL4, DMACTL4_);
#define DMAIV_                0x050E    /* DMA Interrupt Vector Word */
sfrb(DMAIV_L , DMAIV_);
sfrb(DMAIV_H , DMAIV_+1);
sfrw(DMAIV, DMAIV_);

#define DMA0CTL_              0x0510    /* DMA Channel 0 Control */
sfrb(DMA0CTL_L , DMA0CTL_);
sfrb(DMA0CTL_H , DMA0CTL_+1);
sfrw(DMA0CTL, DMA0CTL_);
#define DMA0SA_               0x0512    /* DMA Channel 0 Source Address */
sfra(DMA0SA, DMA0SA_);
#define DMA0DA_               0x0516    /* DMA Channel 0 Destination Address */
sfra(DMA0DA, DMA0DA_);
#define DMA0SZ_               0x051A    /* DMA Channel 0 Transfer Size */
sfrw(DMA0SZ, DMA0SZ_);

#define DMA1CTL_              0x0520    /* DMA Channel 1 Control */
sfrb(DMA1CTL_L , DMA1CTL_);
sfrb(DMA1CTL_H , DMA1CTL_+1);
sfrw(DMA1CTL, DMA1CTL_);
#define DMA1SA_               0x0522    /* DMA Channel 1 Source Address */
sfra(DMA1SA, DMA1SA_);
#define DMA1DA_               0x0526    /* DMA Channel 1 Destination Address */
sfra(DMA1DA, DMA1DA_);
#define DMA1SZ_               0x052A    /* DMA Channel 1 Transfer Size */
sfrw(DMA1SZ, DMA1SZ_);

#define DMA2CTL_              0x0530    /* DMA Channel 2 Control */
sfrb(DMA2CTL_L , DMA2CTL_);
sfrb(DMA2CTL_H , DMA2CTL_+1);
sfrw(DMA2CTL, DMA2CTL_);
#define DMA2SA_               0x0532    /* DMA Channel 2 Source Address */
sfra(DMA2SA, DMA2SA_);
#define DMA2DA_               0x0536    /* DMA Channel 2 Destination Address */
sfra(DMA2DA, DMA2DA_);
#define DMA2SZ_               0x053A    /* DMA Channel 2 Transfer Size */
sfrw(DMA2SZ, DMA2SZ_);

/* DMACTL0 Control Bits */
#define DMA0TSEL0           (0x0001)    /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1           (0x0002)    /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2           (0x0004)    /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3           (0x0008)    /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4           (0x0010)    /* DMA channel 0 transfer select bit 4 */
#define DMA1TSEL0           (0x0100)    /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1           (0x0200)    /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2           (0x0400)    /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3           (0x0800)    /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4           (0x1000)    /* DMA channel 1 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA0TSEL0_L         (0x0001)    /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1_L         (0x0002)    /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2_L         (0x0004)    /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3_L         (0x0008)    /* DMA channel 0 transfer select bit 3 */
#define DMA0TSEL4_L         (0x0010)    /* DMA channel 0 transfer select bit 4 */

/* DMACTL0 Control Bits */
#define DMA1TSEL0_H         (0x0001)    /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1_H         (0x0002)    /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2_H         (0x0004)    /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3_H         (0x0008)    /* DMA channel 1 transfer select bit 3 */
#define DMA1TSEL4_H         (0x0010)    /* DMA channel 1 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0           (0x0001)    /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1           (0x0002)    /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2           (0x0004)    /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3           (0x0008)    /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4           (0x0010)    /* DMA channel 2 transfer select bit 4 */

/* DMACTL01 Control Bits */
#define DMA2TSEL0_L         (0x0001)    /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1_L         (0x0002)    /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2_L         (0x0004)    /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3_L         (0x0008)    /* DMA channel 2 transfer select bit 3 */
#define DMA2TSEL4_L         (0x0010)    /* DMA channel 2 transfer select bit 4 */


/* DMACTL4 Control Bits */
#define ENNMI               (0x0001)    /* Enable NMI interruption of DMA */
#define ROUNDROBIN          (0x0002)    /* Round-Robin DMA channel priorities */
#define DMARMWDIS           (0x0004)    /* Inhibited DMA transfers during read-modify-write CPU operations */

/* DMACTL4 Control Bits */
#define ENNMI_L             (0x0001)    /* Enable NMI interruption of DMA */
#define ROUNDROBIN_L        (0x0002)    /* Round-Robin DMA channel priorities */
#define DMARMWDIS_L         (0x0004)    /* Inhibited DMA transfers during read-modify-write CPU operations */



/* DMAxCTL Control Bits */
#define DMAREQ              (0x0001)    /* Initiate DMA transfer with DMATSEL */
#define DMAABORT            (0x0002)    /* DMA transfer aborted by NMI */
#define DMAIE               (0x0004)    /* DMA interrupt enable */
#define DMAIFG              (0x0008)    /* DMA interrupt flag */
#define DMAEN               (0x0010)    /* DMA enable */
#define DMALEVEL            (0x0020)    /* DMA level sensitive trigger select */
#define DMASRCBYTE          (0x0040)    /* DMA source byte */
#define DMADSTBYTE          (0x0080)    /* DMA destination byte */
#define DMASRCINCR0         (0x0100)    /* DMA source increment bit 0 */
#define DMASRCINCR1         (0x0200)    /* DMA source increment bit 1 */
#define DMADSTINCR0         (0x0400)    /* DMA destination increment bit 0 */
#define DMADSTINCR1         (0x0800)    /* DMA destination increment bit 1 */
#define DMADT0              (0x1000)    /* DMA transfer mode bit 0 */
#define DMADT1              (0x2000)    /* DMA transfer mode bit 1 */
#define DMADT2              (0x4000)    /* DMA transfer mode bit 2 */

/* DMAxCTL Control Bits */
#define DMAREQ_L            (0x0001)    /* Initiate DMA transfer with DMATSEL */
#define DMAABORT_L          (0x0002)    /* DMA transfer aborted by NMI */
#define DMAIE_L             (0x0004)    /* DMA interrupt enable */
#define DMAIFG_L            (0x0008)    /* DMA interrupt flag */
#define DMAEN_L             (0x0010)    /* DMA enable */
#define DMALEVEL_L          (0x0020)    /* DMA level sensitive trigger select */
#define DMASRCBYTE_L        (0x0040)    /* DMA source byte */
#define DMADSTBYTE_L        (0x0080)    /* DMA destination byte */

/* DMAxCTL Control Bits */
#define DMASRCINCR0_H       (0x0001)    /* DMA source increment bit 0 */
#define DMASRCINCR1_H       (0x0002)    /* DMA source increment bit 1 */
#define DMADSTINCR0_H       (0x0004)    /* DMA destination increment bit 0 */
#define DMADSTINCR1_H       (0x0008)    /* DMA destination increment bit 1 */
#define DMADT0_H            (0x0010)    /* DMA transfer mode bit 0 */
#define DMADT1_H            (0x0020)    /* DMA transfer mode bit 1 */
#define DMADT2_H            (0x0040)    /* DMA transfer mode bit 2 */

#define DMASWDW             (0x0000)  /* DMA transfer: source word to destination word */
#define DMASBDW             (0x0040)  /* DMA transfer: source byte to destination word */
#define DMASWDB             (0x0080)  /* DMA transfer: source word to destination byte */
#define DMASBDB             (0x00C0)  /* DMA transfer: source byte to destination byte */

#define DMASRCINCR_0        (0x0000)  /* DMA source increment 0: source address unchanged */
#define DMASRCINCR_1        (0x0100)  /* DMA source increment 1: source address unchanged */
#define DMASRCINCR_2        (0x0200)  /* DMA source increment 2: source address decremented */
#define DMASRCINCR_3        (0x0300)  /* DMA source increment 3: source address incremented */

#define DMADSTINCR_0        (0x0000)  /* DMA destination increment 0: destination address unchanged */
#define DMADSTINCR_1        (0x0400)  /* DMA destination increment 1: destination address unchanged */
#define DMADSTINCR_2        (0x0800)  /* DMA destination increment 2: destination address decremented */
#define DMADSTINCR_3        (0x0C00)  /* DMA destination increment 3: destination address incremented */

#define DMADT_0             (0x0000)  /* DMA transfer mode 0: Single transfer */
#define DMADT_1             (0x1000)  /* DMA transfer mode 1: Block transfer */
#define DMADT_2             (0x2000)  /* DMA transfer mode 2: Burst-Block transfer */
#define DMADT_3             (0x3000)  /* DMA transfer mode 3: Burst-Block transfer */
#define DMADT_4             (0x4000)  /* DMA transfer mode 4: Repeated Single transfer */
#define DMADT_5             (0x5000)  /* DMA transfer mode 5: Repeated Block transfer */
#define DMADT_6             (0x6000)  /* DMA transfer mode 6: Repeated Burst-Block transfer */
#define DMADT_7             (0x7000)  /* DMA transfer mode 7: Repeated Burst-Block transfer */

/* DMAIV Definitions */
#define DMAIV_NONE           (0x0000)    /* No Interrupt pending */
#define DMAIV_DMA0IFG        (0x0002)    /* DMA0IFG*/
#define DMAIV_DMA1IFG        (0x0004)    /* DMA1IFG*/
#define DMAIV_DMA2IFG        (0x0006)    /* DMA2IFG*/

#define DMA0TSEL_0          (0x0000)  /* DMA channel 0 transfer select 0 */
#define DMA0TSEL_1          (0x0001)  /* DMA channel 0 transfer select 1 */
#define DMA0TSEL_2          (0x0002)  /* DMA channel 0 transfer select 2 */
#define DMA0TSEL_3          (0x0003)  /* DMA channel 0 transfer select 3 */
#define DMA0TSEL_4          (0x0004)  /* DMA channel 0 transfer select 4 */
#define DMA0TSEL_5          (0x0005)  /* DMA channel 0 transfer select 5 */
#define DMA0TSEL_6          (0x0006)  /* DMA channel 0 transfer select 6 */
#define DMA0TSEL_7          (0x0007)  /* DMA channel 0 transfer select 7 */
#define DMA0TSEL_8          (0x0008)  /* DMA channel 0 transfer select 8 */
#define DMA0TSEL_9          (0x0009)  /* DMA channel 0 transfer select 9 */
#define DMA0TSEL_10         (0x000A) /* DMA channel 0 transfer select 10 */
#define DMA0TSEL_11         (0x000B) /* DMA channel 0 transfer select 11 */
#define DMA0TSEL_12         (0x000C) /* DMA channel 0 transfer select 12 */
#define DMA0TSEL_13         (0x000D) /* DMA channel 0 transfer select 13 */
#define DMA0TSEL_14         (0x000E) /* DMA channel 0 transfer select 14 */
#define DMA0TSEL_15         (0x000F) /* DMA channel 0 transfer select 15 */
#define DMA0TSEL_16         (0x0010) /* DMA channel 0 transfer select 16 */
#define DMA0TSEL_17         (0x0011) /* DMA channel 0 transfer select 17 */
#define DMA0TSEL_18         (0x0012) /* DMA channel 0 transfer select 18 */
#define DMA0TSEL_19         (0x0013) /* DMA channel 0 transfer select 19 */
#define DMA0TSEL_20         (0x0014) /* DMA channel 0 transfer select 20 */
#define DMA0TSEL_21         (0x0015) /* DMA channel 0 transfer select 21 */
#define DMA0TSEL_22         (0x0016) /* DMA channel 0 transfer select 22 */
#define DMA0TSEL_23         (0x0017) /* DMA channel 0 transfer select 23 */
#define DMA0TSEL_24         (0x0018) /* DMA channel 0 transfer select 24 */
#define DMA0TSEL_25         (0x0019) /* DMA channel 0 transfer select 25 */
#define DMA0TSEL_26         (0x001A) /* DMA channel 0 transfer select 26 */
#define DMA0TSEL_27         (0x001B) /* DMA channel 0 transfer select 27 */
#define DMA0TSEL_28         (0x001C) /* DMA channel 0 transfer select 28 */
#define DMA0TSEL_29         (0x001D) /* DMA channel 0 transfer select 29 */
#define DMA0TSEL_30         (0x001E) /* DMA channel 0 transfer select 30 */
#define DMA0TSEL_31         (0x001F) /* DMA channel 0 transfer select 31 */

#define DMA1TSEL_0          (0x0000)  /* DMA channel 1 transfer select 0 */
#define DMA1TSEL_1          (0x0100)  /* DMA channel 1 transfer select 1 */
#define DMA1TSEL_2          (0x0200)  /* DMA channel 1 transfer select 2 */
#define DMA1TSEL_3          (0x0300)  /* DMA channel 1 transfer select 3 */
#define DMA1TSEL_4          (0x0400)  /* DMA channel 1 transfer select 4 */
#define DMA1TSEL_5          (0x0500)  /* DMA channel 1 transfer select 5 */
#define DMA1TSEL_6          (0x0600)  /* DMA channel 1 transfer select 6 */
#define DMA1TSEL_7          (0x0007)  /* DMA channel 1 transfer select 7 */
#define DMA1TSEL_8          (0x0008)  /* DMA channel 1 transfer select 8 */
#define DMA1TSEL_9          (0x0900)  /* DMA channel 1 transfer select 9 */
#define DMA1TSEL_10         (0x0A00) /* DMA channel 1 transfer select 10 */
#define DMA1TSEL_11         (0x0B00) /* DMA channel 1 transfer select 11 */
#define DMA1TSEL_12         (0x0C00) /* DMA channel 1 transfer select 12 */
#define DMA1TSEL_13         (0x0D00) /* DMA channel 1 transfer select 13 */
#define DMA1TSEL_14         (0x0E00) /* DMA channel 1 transfer select 14 */
#define DMA1TSEL_15         (0x0F00) /* DMA channel 1 transfer select 15 */
#define DMA1TSEL_16         (0x1000) /* DMA channel 1 transfer select 16 */
#define DMA1TSEL_17         (0x1100) /* DMA channel 1 transfer select 17 */
#define DMA1TSEL_18         (0x1200) /* DMA channel 1 transfer select 18 */
#define DMA1TSEL_19         (0x1300) /* DMA channel 1 transfer select 19 */
#define DMA1TSEL_20         (0x1400) /* DMA channel 1 transfer select 20 */
#define DMA1TSEL_21         (0x1500) /* DMA channel 1 transfer select 21 */
#define DMA1TSEL_22         (0x1600) /* DMA channel 1 transfer select 22 */
#define DMA1TSEL_23         (0x1700) /* DMA channel 1 transfer select 23 */
#define DMA1TSEL_24         (0x1800) /* DMA channel 1 transfer select 24 */
#define DMA1TSEL_25         (0x1900) /* DMA channel 1 transfer select 25 */
#define DMA1TSEL_26         (0x1A00) /* DMA channel 1 transfer select 26 */
#define DMA1TSEL_27         (0x1B00) /* DMA channel 1 transfer select 27 */
#define DMA1TSEL_28         (0x1C00) /* DMA channel 1 transfer select 28 */
#define DMA1TSEL_29         (0x1D00) /* DMA channel 1 transfer select 29 */
#define DMA1TSEL_30         (0x1E00) /* DMA channel 1 transfer select 30 */
#define DMA1TSEL_31         (0x1F00) /* DMA channel 1 transfer select 31 */

#define DMA2TSEL_0          (0x0000)  /* DMA channel 2 transfer select 0 */
#define DMA2TSEL_1          (0x0001)  /* DMA channel 2 transfer select 1 */
#define DMA2TSEL_2          (0x0002)  /* DMA channel 2 transfer select 2 */
#define DMA2TSEL_3          (0x0003)  /* DMA channel 2 transfer select 3 */
#define DMA2TSEL_4          (0x0004)  /* DMA channel 2 transfer select 4 */
#define DMA2TSEL_5          (0x0005)  /* DMA channel 2 transfer select 5 */
#define DMA2TSEL_6          (0x0006)  /* DMA channel 2 transfer select 6 */
#define DMA2TSEL_7          (0x0007)  /* DMA channel 2 transfer select 7 */
#define DMA2TSEL_8          (0x0008)  /* DMA channel 2 transfer select 8 */
#define DMA2TSEL_9          (0x0009)  /* DMA channel 2 transfer select 9 */
#define DMA2TSEL_10         (0x000A) /* DMA channel 2 transfer select 10 */
#define DMA2TSEL_11         (0x000B) /* DMA channel 2 transfer select 11 */
#define DMA2TSEL_12         (0x000C) /* DMA channel 2 transfer select 12 */
#define DMA2TSEL_13         (0x000D) /* DMA channel 2 transfer select 13 */
#define DMA2TSEL_14         (0x000E) /* DMA channel 2 transfer select 14 */
#define DMA2TSEL_15         (0x000F) /* DMA channel 2 transfer select 15 */
#define DMA2TSEL_16         (0x0010) /* DMA channel 2 transfer select 16 */
#define DMA2TSEL_17         (0x0011) /* DMA channel 2 transfer select 17 */
#define DMA2TSEL_18         (0x0012) /* DMA channel 2 transfer select 18 */
#define DMA2TSEL_19         (0x0013) /* DMA channel 2 transfer select 19 */
#define DMA2TSEL_20         (0x0014) /* DMA channel 2 transfer select 20 */
#define DMA2TSEL_21         (0x0015) /* DMA channel 2 transfer select 21 */
#define DMA2TSEL_22         (0x0016) /* DMA channel 2 transfer select 22 */
#define DMA2TSEL_23         (0x0017) /* DMA channel 2 transfer select 23 */
#define DMA2TSEL_24         (0x0018) /* DMA channel 2 transfer select 24 */
#define DMA2TSEL_25         (0x0019) /* DMA channel 2 transfer select 25 */
#define DMA2TSEL_26         (0x001A) /* DMA channel 2 transfer select 26 */
#define DMA2TSEL_27         (0x001B) /* DMA channel 2 transfer select 27 */
#define DMA2TSEL_28         (0x001C) /* DMA channel 2 transfer select 28 */
#define DMA2TSEL_29         (0x001D) /* DMA channel 2 transfer select 29 */
#define DMA2TSEL_30         (0x001E) /* DMA channel 2 transfer select 30 */
#define DMA2TSEL_31         (0x001F) /* DMA channel 2 transfer select 31 */

#define DMA0TSEL__DMA_REQ   (0x0000)  /* DMA channel 0 transfer select 0:  DMA_REQ (sw) */
#define DMA0TSEL__TA0CCR0   (0x0001)  /* DMA channel 0 transfer select 1:  Timer0_A (TA0CCR0.IFG) */
#define DMA0TSEL__TA0CCR1   (0x0002)  /* DMA channel 0 transfer select 2:  Timer0_A (TA0CCR1.IFG) */
#define DMA0TSEL__TA1CCR0   (0x0003)  /* DMA channel 0 transfer select 3:  Timer1_A (TA1CCR0.IFG) */
#define DMA0TSEL__RES4      (0x0004)  /* DMA channel 0 transfer select 4:  Reserved */
#define DMA0TSEL__TA2CCR0   (0x0005)  /* DMA channel 0 transfer select 5:  Timer2_A (TA2CCR0.IFG) */
#define DMA0TSEL__RES6      (0x0006)  /* DMA channel 0 transfer select 6:  Reserved */
#define DMA0TSEL__TA3CCR0   (0x0007)  /* DMA channel 0 transfer select 7:  Timer3_A (TA3CCR0.IFG) */
#define DMA0TSEL__RES8      (0x0008)  /* DMA channel 0 transfer select 8:  Reserved */
#define DMA0TSEL__RES9      (0x0009)  /* DMA channel 0 transfer select 9:  Reserved */
#define DMA0TSEL__RES10     (0x000A) /* DMA channel 0 transfer select 10: Reserved */
#define DMA0TSEL__RES11     (0x000B) /* DMA channel 0 transfer select 11: Reserved */
#define DMA0TSEL__RES12     (0x000C) /* DMA channel 0 transfer select 12: Reserved */
#define DMA0TSEL__SD24IFG   (0x000D) /* DMA channel 0 transfer select 13: SD24IFG */
#define DMA0TSEL__RES14     (0x000E) /* DMA channel 0 transfer select 14: Reserved */
#define DMA0TSEL__RES15     (0x000F) /* DMA channel 0 transfer select 15: Reserved */
#define DMA0TSEL__USCIA0RX  (0x0010) /* DMA channel 0 transfer select 16: USCIA0 receive */
#define DMA0TSEL__USCIA0TX  (0x0011) /* DMA channel 0 transfer select 17: USCIA0 transmit */
#define DMA0TSEL__USCIA1RX  (0x0012) /* DMA channel 0 transfer select 18: USCIA1 receive */
#define DMA0TSEL__USCIA1TX  (0x0013) /* DMA channel 0 transfer select 19: USCIA1 transmit */
#define DMA0TSEL__USCIA2RX  (0x0014) /* DMA channel 0 transfer select 20: USCIA2 receive */
#define DMA0TSEL__USCIA2TX  (0x0015) /* DMA channel 0 transfer select 21: USCIA2 transmit */
#define DMA0TSEL__USCIB0RX  (0x0016) /* DMA channel 0 transfer select 22: USCIB0 receive */
#define DMA0TSEL__USCIB0TX  (0x0017) /* DMA channel 0 transfer select 23: USCIB0 transmit */
#define DMA0TSEL__ADC10IFG0 (0x0018) /* DMA channel 0 transfer select 24: ADC10IFG0 */
#define DMA0TSEL__USCIA3RX  (0x0019) /* DMA channel 0 transfer select 25: USCIA3 receive */
#define DMA0TSEL__USCIA3TX  (0x001A) /* DMA channel 0 transfer select 26: USCIA3 transmit */
#define DMA0TSEL__USCIB1RX  (0x001B) /* DMA channel 0 transfer select 27: USCIB1 receive */
#define DMA0TSEL__USCIB1TX  (0x001C) /* DMA channel 0 transfer select 28: USCIB1 transmit */
#define DMA0TSEL__MPY       (0x001D) /* DMA channel 0 transfer select 29: Multiplier ready */
#define DMA0TSEL__DMA2IFG   (0x001E) /* DMA channel 0 transfer select 30: previous DMA channel DMA2IFG */
#define DMA0TSEL__DMAE0     (0x001F) /* DMA channel 0 transfer select 31: ext. Trigger (DMAE0) */

#define DMA1TSEL__DMA_REQ   (0x0000)  /* DMA channel 1 transfer select 0:  DMA_REQ (sw) */
#define DMA1TSEL__TA0CCR0   (0x0100)  /* DMA channel 1 transfer select 1:  Timer0_A (TA0CCR0.IFG) */
#define DMA1TSEL__TA0CCR1   (0x0200)  /* DMA channel 1 transfer select 2:  Timer0_A (TA0CCR1.IFG) */
#define DMA1TSEL__TA1CCR0   (0x0300)  /* DMA channel 1 transfer select 3:  Timer1_A (TA1CCR0.IFG) */
#define DMA1TSEL__RES4      (0x0400)  /* DMA channel 1 transfer select 4:  Reserved */
#define DMA1TSEL__TA2CCR0   (0x0500)  /* DMA channel 1 transfer select 5:  Timer2_A (TA2CCR0.IFG) */
#define DMA1TSEL__RES6      (0x0600)  /* DMA channel 1 transfer select 6:  Reserved */
#define DMA1TSEL__TA3CCR0   (0x0007)  /* DMA channel 1 transfer select 7:  Timer3_A (TA3CCR0.IFG) */
#define DMA1TSEL__RES8      (0x0008)  /* DMA channel 1 transfer select 8:  Reserved */
#define DMA1TSEL__RES9      (0x0900)  /* DMA channel 1 transfer select 9:  Reserved */
#define DMA1TSEL__RES10     (0x0A00) /* DMA channel 1 transfer select 10: Reserved */
#define DMA1TSEL__RES11     (0x0B00) /* DMA channel 1 transfer select 11: Reserved */
#define DMA1TSEL__RES12     (0x0C00) /* DMA channel 1 transfer select 12: Reserved */
#define DMA1TSEL__SD24IFG   (0x0D00) /* DMA channel 1 transfer select 13: SD24IFG */
#define DMA1TSEL__RES14     (0x0E00) /* DMA channel 1 transfer select 14: Reserved */
#define DMA1TSEL__RES15     (0x0F00) /* DMA channel 1 transfer select 15: Reserved */
#define DMA1TSEL__USCIA0RX  (0x1000) /* DMA channel 1 transfer select 16: USCIA0 receive */
#define DMA1TSEL__USCIA0TX  (0x1100) /* DMA channel 1 transfer select 17: USCIA0 transmit */
#define DMA1TSEL__USCIA1RX  (0x1200) /* DMA channel 1 transfer select 18: USCIA1 receive */
#define DMA1TSEL__USCIA1TX  (0x1300) /* DMA channel 1 transfer select 19: USCIA1 transmit */
#define DMA1TSEL__USCIA2RX  (0x1400) /* DMA channel 1 transfer select 20: USCIA2 receive */
#define DMA1TSEL__USCIA2TX  (0x1500) /* DMA channel 1 transfer select 21: USCIA2 transmit */
#define DMA1TSEL__USCIB0RX  (0x1600) /* DMA channel 1 transfer select 22: USCIB0 receive */
#define DMA1TSEL__USCIB0TX  (0x1700) /* DMA channel 1 transfer select 23: USCIB0 transmit */
#define DMA1TSEL__ADC10IFG0 (0x1800) /* DMA channel 1 transfer select 24: ADC10IFG0 */
#define DMA1TSEL__USCIA3RX  (0x1900) /* DMA channel 1 transfer select 25: USCIA3 receive */
#define DMA1TSEL__USCIA3TX  (0x1A00) /* DMA channel 1 transfer select 26: USCIA3 transmit */
#define DMA1TSEL__USCIB1RX  (0x1B00) /* DMA channel 1 transfer select 27: USCIB1 receive */
#define DMA1TSEL__USCIB1TX  (0x1C00) /* DMA channel 1 transfer select 28: USCIB1 transmit */
#define DMA1TSEL__MPY       (0x1D00) /* DMA channel 1 transfer select 29: Multiplier ready */
#define DMA1TSEL__DMA0IFG   (0x1E00) /* DMA channel 1 transfer select 30: previous DMA channel DMA0IFG */
#define DMA1TSEL__DMAE0     (0x1F00) /* DMA channel 1 transfer select 31: ext. Trigger (DMAE0) */

#define DMA2TSEL__DMA_REQ   (0x0000)  /* DMA channel 2 transfer select 0:  DMA_REQ (sw) */
#define DMA2TSEL__TA0CCR0   (0x0001)  /* DMA channel 2 transfer select 1:  Timer0_A (TA0CCR0.IFG) */
#define DMA2TSEL__TA0CCR1   (0x0002)  /* DMA channel 2 transfer select 2:  Timer0_A (TA0CCR1.IFG) */
#define DMA2TSEL__TA1CCR0   (0x0003)  /* DMA channel 2 transfer select 3:  Timer1_A (TA1CCR0.IFG) */
#define DMA2TSEL__RES4      (0x0004)  /* DMA channel 2 transfer select 4:  Reserved */
#define DMA2TSEL__TA2CCR0   (0x0005)  /* DMA channel 2 transfer select 5:  Timer2_A (TA2CCR0.IFG) */
#define DMA2TSEL__RES6      (0x0006)  /* DMA channel 2 transfer select 6:  Reserved */
#define DMA2TSEL__TA3CCR0   (0x0007)  /* DMA channel 2 transfer select 7:  Timer3_A (TA3CCR0.IFG) */
#define DMA2TSEL__RES8      (0x0008)  /* DMA channel 2 transfer select 8:  Reserved */
#define DMA2TSEL__RES9      (0x0009)  /* DMA channel 2 transfer select 9:  Reserved */
#define DMA2TSEL__RES10     (0x000A) /* DMA channel 2 transfer select 10: Reserved */
#define DMA2TSEL__RES11     (0x000B) /* DMA channel 2 transfer select 11: Reserved */
#define DMA2TSEL__RES12     (0x000C) /* DMA channel 2 transfer select 12: Reserved */
#define DMA2TSEL__SD24IFG   (0x000D) /* DMA channel 2 transfer select 13: SD24IFG */
#define DMA2TSEL__RES14     (0x000E) /* DMA channel 2 transfer select 14: Reserved */
#define DMA2TSEL__RES15     (0x000F) /* DMA channel 2 transfer select 15: Reserved */
#define DMA2TSEL__USCIA0RX  (0x0010) /* DMA channel 2 transfer select 16: USCIA0 receive */
#define DMA2TSEL__USCIA0TX  (0x0011) /* DMA channel 2 transfer select 17: USCIA0 transmit */
#define DMA2TSEL__USCIA1RX  (0x0012) /* DMA channel 2 transfer select 18: USCIA1 receive */
#define DMA2TSEL__USCIA1TX  (0x0013) /* DMA channel 2 transfer select 19: USCIA1 transmit */
#define DMA2TSEL__USCIA2RX  (0x0014) /* DMA channel 2 transfer select 20: USCIA2 receive */
#define DMA2TSEL__USCIA2TX  (0x0015) /* DMA channel 2 transfer select 21: USCIA2 transmit */
#define DMA2TSEL__USCIB0RX  (0x0016) /* DMA channel 2 transfer select 22: USCIB0 receive */
#define DMA2TSEL__USCIB0TX  (0x0017) /* DMA channel 2 transfer select 23: USCIB0 transmit */
#define DMA2TSEL__ADC10IFG0 (0x0018) /* DMA channel 2 transfer select 24: ADC10IFG0 */
#define DMA2TSEL__USCIA3RX  (0x0019) /* DMA channel 2 transfer select 25: USCIA3 receive */
#define DMA2TSEL__USCIA3TX  (0x001A) /* DMA channel 2 transfer select 26: USCIA3 transmit */
#define DMA2TSEL__USCIB1RX  (0x001B) /* DMA channel 2 transfer select 27: USCIB1 receive */
#define DMA2TSEL__USCIB1TX  (0x001C) /* DMA channel 2 transfer select 28: USCIB1 transmit */
#define DMA2TSEL__MPY       (0x001D) /* DMA channel 2 transfer select 29: Multiplier ready */
#define DMA2TSEL__DMA1IFG   (0x001E) /* DMA channel 2 transfer select 30: previous DMA channel DMA1IFG */
#define DMA2TSEL__DMAE0     (0x001F) /* DMA channel 2 transfer select 31: ext. Trigger (DMAE0) */

/*************************************************************
* Flash Memory
*************************************************************/
#define __MSP430_HAS_FLASH__         /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_FLASH__ 0x0140
#define FLASH_BASE __MSP430_BASEADDRESS_FLASH__

#define FCTL1_                0x0140    /* FLASH Control 1 */
sfrb(FCTL1_L , FCTL1_);
sfrb(FCTL1_H , FCTL1_+1);
sfrw(FCTL1, FCTL1_);
//sfrbw    FCTL2                  (0x0142)  /* FLASH Control 2 */
#define FCTL3_                0x0144    /* FLASH Control 3 */
sfrb(FCTL3_L , FCTL3_);
sfrb(FCTL3_H , FCTL3_+1);
sfrw(FCTL3, FCTL3_);
#define FCTL4_                0x0146    /* FLASH Control 4 */
sfrb(FCTL4_L , FCTL4_);
sfrb(FCTL4_H , FCTL4_+1);
sfrw(FCTL4, FCTL4_);

#define FRPW                (0x9600)  /* Flash password returned by read */
#define FWPW                (0xA500)  /* Flash password for write */
#define FXPW                (0x3300)  /* for use with XOR instruction */
#define FRKEY               (0x9600)  /* (legacy definition) Flash key returned by read */
#define FWKEY               (0xA500)  /* (legacy definition) Flash key for write */
#define FXKEY               (0x3300)  /* (legacy definition) for use with XOR instruction */

/* FCTL1 Control Bits */
//#define RESERVED            (0x0001)  /* Reserved */
#define ERASE               (0x0002)  /* Enable bit for Flash segment erase */
#define MERAS               (0x0004)  /* Enable bit for Flash mass erase */
//#define RESERVED            (0x0008)  /* Reserved */
//#define RESERVED            (0x0010)  /* Reserved */
#define SWRT                (0x0020)  /* Smart Write enable */
#define WRT                 (0x0040)  /* Enable bit for Flash write */
#define BLKWRT              (0x0080)  /* Enable bit for Flash segment write */

/* FCTL1 Control Bits */
//#define RESERVED            (0x0001)  /* Reserved */
#define ERASE_L             (0x0002)  /* Enable bit for Flash segment erase */
#define MERAS_L             (0x0004)  /* Enable bit for Flash mass erase */
//#define RESERVED            (0x0008)  /* Reserved */
//#define RESERVED            (0x0010)  /* Reserved */
#define SWRT_L              (0x0020)  /* Smart Write enable */
#define WRT_L               (0x0040)  /* Enable bit for Flash write */
#define BLKWRT_L            (0x0080)  /* Enable bit for Flash segment write */


/* FCTL3 Control Bits */
#define BUSY                (0x0001)  /* Flash busy: 1 */
#define KEYV                (0x0002)  /* Flash Key violation flag */
#define ACCVIFG             (0x0004)  /* Flash Access violation flag */
#define WAIT                (0x0008)  /* Wait flag for segment write */
#define LOCK                (0x0010)  /* Lock bit: 1 - Flash is locked (read only) */
#define EMEX                (0x0020)  /* Flash Emergency Exit */
#define LOCKA               (0x0040)  /* Segment A Lock bit: read = 1 - Segment is locked (read only) */
//#define RESERVED            (0x0080)  /* Reserved */

/* FCTL3 Control Bits */
#define BUSY_L              (0x0001)  /* Flash busy: 1 */
#define KEYV_L              (0x0002)  /* Flash Key violation flag */
#define ACCVIFG_L           (0x0004)  /* Flash Access violation flag */
#define WAIT_L              (0x0008)  /* Wait flag for segment write */
#define LOCK_L              (0x0010)  /* Lock bit: 1 - Flash is locked (read only) */
#define EMEX_L              (0x0020)  /* Flash Emergency Exit */
#define LOCKA_L             (0x0040)  /* Segment A Lock bit: read = 1 - Segment is locked (read only) */
//#define RESERVED            (0x0080)  /* Reserved */


/* FCTL4 Control Bits */
#define VPE                 (0x0001)  /* Voltage Changed during Program Error Flag */
#define MGR0                (0x0010)  /* Marginal read 0 mode. */
#define MGR1                (0x0020)  /* Marginal read 1 mode. */
#define LOCKINFO            (0x0080)  /* Lock INFO Memory bit: read = 1 - Segment is locked (read only) */

/* FCTL4 Control Bits */
#define VPE_L               (0x0001)  /* Voltage Changed during Program Error Flag */
#define MGR0_L              (0x0010)  /* Marginal read 0 mode. */
#define MGR1_L              (0x0020)  /* Marginal read 1 mode. */
#define LOCKINFO_L          (0x0080)  /* Lock INFO Memory bit: read = 1 - Segment is locked (read only) */


/************************************************************
* LCD_C
************************************************************/
#define __MSP430_HAS_LCD_C__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_LCD_C__ 0x0A00
#define LCD_C_BASE __MSP430_BASEADDRESS_LCD_C__

#define LCDCCTL0_             0x0A00    /* LCD_C Control Register 0 */
sfrb(LCDCCTL0_L , LCDCCTL0_);
sfrb(LCDCCTL0_H , LCDCCTL0_+1);
sfrw(LCDCCTL0, LCDCCTL0_);
#define LCDCCTL1_             0x0A02    /* LCD_C Control Register 1 */
sfrb(LCDCCTL1_L , LCDCCTL1_);
sfrb(LCDCCTL1_H , LCDCCTL1_+1);
sfrw(LCDCCTL1, LCDCCTL1_);
#define LCDCBLKCTL_           0x0A04    /* LCD_C blinking control register */
sfrb(LCDCBLKCTL_L , LCDCBLKCTL_);
sfrb(LCDCBLKCTL_H , LCDCBLKCTL_+1);
sfrw(LCDCBLKCTL, LCDCBLKCTL_);
#define LCDCMEMCTL_           0x0A06    /* LCD_C memory control register */
sfrb(LCDCMEMCTL_L , LCDCMEMCTL_);
sfrb(LCDCMEMCTL_H , LCDCMEMCTL_+1);
sfrw(LCDCMEMCTL, LCDCMEMCTL_);
#define LCDCVCTL_             0x0A08    /* LCD_C Voltage Control Register */
sfrb(LCDCVCTL_L , LCDCVCTL_);
sfrb(LCDCVCTL_H , LCDCVCTL_+1);
sfrw(LCDCVCTL, LCDCVCTL_);
#define LCDCPCTL0_            0x0A0A    /* LCD_C Port Control Register 0 */
sfrb(LCDCPCTL0_L , LCDCPCTL0_);
sfrb(LCDCPCTL0_H , LCDCPCTL0_+1);
sfrw(LCDCPCTL0, LCDCPCTL0_);
#define LCDCPCTL1_            0x0A0C    /* LCD_C Port Control Register 1 */
sfrb(LCDCPCTL1_L , LCDCPCTL1_);
sfrb(LCDCPCTL1_H , LCDCPCTL1_+1);
sfrw(LCDCPCTL1, LCDCPCTL1_);
#define LCDCPCTL2_            0x0A0E    /* LCD_C Port Control Register 2 */
sfrb(LCDCPCTL2_L , LCDCPCTL2_);
sfrb(LCDCPCTL2_H , LCDCPCTL2_+1);
sfrw(LCDCPCTL2, LCDCPCTL2_);
#define LCDCCPCTL_            0x0A12    /* LCD_C Charge Pump Control Register 3 */
sfrb(LCDCCPCTL_L , LCDCCPCTL_);
sfrb(LCDCCPCTL_H , LCDCCPCTL_+1);
sfrw(LCDCCPCTL, LCDCCPCTL_);
#define LCDCIV_               0x0A1E    /* LCD_C Interrupt Vector Register */
sfrw(LCDCIV, LCDCIV_);

// LCDCCTL0
#define LCDON               (0x0001)  /* LCD_C LCD On */
#define LCDLP               (0x0002)  /* LCD_C Low Power Waveform */
#define LCDSON              (0x0004)  /* LCD_C LCD Segments On */
#define LCDMX0              (0x0008)  /* LCD_C Mux Rate Bit: 0 */
#define LCDMX1              (0x0010)  /* LCD_C Mux Rate Bit: 1 */
#define LCDMX2              (0x0020)  /* LCD_C Mux Rate Bit: 2 */
//#define RESERVED            (0x0040)  /* LCD_C RESERVED */
#define LCDSSEL             (0x0080)  /* LCD_C Clock Select */
#define LCDPRE0             (0x0100)  /* LCD_C LCD frequency pre-scaler Bit: 0 */
#define LCDPRE1             (0x0200)  /* LCD_C LCD frequency pre-scaler Bit: 1 */
#define LCDPRE2             (0x0400)  /* LCD_C LCD frequency pre-scaler Bit: 2 */
#define LCDDIV0             (0x0800)  /* LCD_C LCD frequency divider Bit: 0 */
#define LCDDIV1             (0x1000)  /* LCD_C LCD frequency divider Bit: 1 */
#define LCDDIV2             (0x2000)  /* LCD_C LCD frequency divider Bit: 2 */
#define LCDDIV3             (0x4000)  /* LCD_C LCD frequency divider Bit: 3 */
#define LCDDIV4             (0x8000)  /* LCD_C LCD frequency divider Bit: 4 */

// LCDCCTL0
#define LCDON_L             (0x0001)  /* LCD_C LCD On */
#define LCDLP_L             (0x0002)  /* LCD_C Low Power Waveform */
#define LCDSON_L            (0x0004)  /* LCD_C LCD Segments On */
#define LCDMX0_L            (0x0008)  /* LCD_C Mux Rate Bit: 0 */
#define LCDMX1_L            (0x0010)  /* LCD_C Mux Rate Bit: 1 */
#define LCDMX2_L            (0x0020)  /* LCD_C Mux Rate Bit: 2 */
//#define RESERVED            (0x0040)  /* LCD_C RESERVED */
#define LCDSSEL_L           (0x0080)  /* LCD_C Clock Select */

// LCDCCTL0
//#define RESERVED            (0x0040)  /* LCD_C RESERVED */
#define LCDPRE0_H           (0x0001)  /* LCD_C LCD frequency pre-scaler Bit: 0 */
#define LCDPRE1_H           (0x0002)  /* LCD_C LCD frequency pre-scaler Bit: 1 */
#define LCDPRE2_H           (0x0004)  /* LCD_C LCD frequency pre-scaler Bit: 2 */
#define LCDDIV0_H           (0x0008)  /* LCD_C LCD frequency divider Bit: 0 */
#define LCDDIV1_H           (0x0010)  /* LCD_C LCD frequency divider Bit: 1 */
#define LCDDIV2_H           (0x0020)  /* LCD_C LCD frequency divider Bit: 2 */
#define LCDDIV3_H           (0x0040)  /* LCD_C LCD frequency divider Bit: 3 */
#define LCDDIV4_H           (0x0080)  /* LCD_C LCD frequency divider Bit: 4 */

#define LCDPRE_0            (0x0000)  /* LCD_C LCD frequency pre-scaler: /1 */
#define LCDPRE_1            (0x0100)  /* LCD_C LCD frequency pre-scaler: /2 */
#define LCDPRE_2            (0x0200)  /* LCD_C LCD frequency pre-scaler: /4 */
#define LCDPRE_3            (0x0300)  /* LCD_C LCD frequency pre-scaler: /8 */
#define LCDPRE_4            (0x0400)  /* LCD_C LCD frequency pre-scaler: /16 */
#define LCDPRE_5            (0x0500)  /* LCD_C LCD frequency pre-scaler: /32 */
#define LCDPRE__1           (0x0000)  /* LCD_C LCD frequency pre-scaler: /1 */
#define LCDPRE__2           (0x0100)  /* LCD_C LCD frequency pre-scaler: /2 */
#define LCDPRE__4           (0x0200)  /* LCD_C LCD frequency pre-scaler: /4 */
#define LCDPRE__8           (0x0300)  /* LCD_C LCD frequency pre-scaler: /8 */
#define LCDPRE__16          (0x0400)  /* LCD_C LCD frequency pre-scaler: /16 */
#define LCDPRE__32          (0x0500)  /* LCD_C LCD frequency pre-scaler: /32 */

#define LCDDIV_0            (0x0000)  /* LCD_C LCD frequency divider: /1 */
#define LCDDIV_1            (0x0800)  /* LCD_C LCD frequency divider: /2 */
#define LCDDIV_2            (0x1000)  /* LCD_C LCD frequency divider: /3 */
#define LCDDIV_3            (0x1800)  /* LCD_C LCD frequency divider: /4 */
#define LCDDIV_4            (0x2000)  /* LCD_C LCD frequency divider: /5 */
#define LCDDIV_5            (0x2800)  /* LCD_C LCD frequency divider: /6 */
#define LCDDIV_6            (0x3000)  /* LCD_C LCD frequency divider: /7 */
#define LCDDIV_7            (0x3800)  /* LCD_C LCD frequency divider: /8 */
#define LCDDIV_8            (0x4000)  /* LCD_C LCD frequency divider: /9 */
#define LCDDIV_9            (0x4800)  /* LCD_C LCD frequency divider: /10 */
#define LCDDIV_10           (0x5000)  /* LCD_C LCD frequency divider: /11 */
#define LCDDIV_11           (0x5800)  /* LCD_C LCD frequency divider: /12 */
#define LCDDIV_12           (0x6000)  /* LCD_C LCD frequency divider: /13 */
#define LCDDIV_13           (0x6800)  /* LCD_C LCD frequency divider: /14 */
#define LCDDIV_14           (0x7000)  /* LCD_C LCD frequency divider: /15 */
#define LCDDIV_15           (0x7800)  /* LCD_C LCD frequency divider: /16 */
#define LCDDIV_16           (0x8000)  /* LCD_C LCD frequency divider: /17 */
#define LCDDIV_17           (0x8800)  /* LCD_C LCD frequency divider: /18 */
#define LCDDIV_18           (0x9000)  /* LCD_C LCD frequency divider: /19 */
#define LCDDIV_19           (0x9800)  /* LCD_C LCD frequency divider: /20 */
#define LCDDIV_20           (0xA000)  /* LCD_C LCD frequency divider: /21 */
#define LCDDIV_21           (0xA800)  /* LCD_C LCD frequency divider: /22 */
#define LCDDIV_22           (0xB000)  /* LCD_C LCD frequency divider: /23 */
#define LCDDIV_23           (0xB800)  /* LCD_C LCD frequency divider: /24 */
#define LCDDIV_24           (0xC000)  /* LCD_C LCD frequency divider: /25 */
#define LCDDIV_25           (0xC800)  /* LCD_C LCD frequency divider: /26 */
#define LCDDIV_26           (0xD000)  /* LCD_C LCD frequency divider: /27 */
#define LCDDIV_27           (0xD800)  /* LCD_C LCD frequency divider: /28 */
#define LCDDIV_28           (0xE000)  /* LCD_C LCD frequency divider: /29 */
#define LCDDIV_29           (0xE800)  /* LCD_C LCD frequency divider: /30 */
#define LCDDIV_30           (0xF000)  /* LCD_C LCD frequency divider: /31 */
#define LCDDIV_31           (0xF800)  /* LCD_C LCD frequency divider: /32 */
#define LCDDIV__1           (0x0000)  /* LCD_C LCD frequency divider: /1 */
#define LCDDIV__2           (0x0800)  /* LCD_C LCD frequency divider: /2 */
#define LCDDIV__3           (0x1000)  /* LCD_C LCD frequency divider: /3 */
#define LCDDIV__4           (0x1800)  /* LCD_C LCD frequency divider: /4 */
#define LCDDIV__5           (0x2000)  /* LCD_C LCD frequency divider: /5 */
#define LCDDIV__6           (0x2800)  /* LCD_C LCD frequency divider: /6 */
#define LCDDIV__7           (0x3000)  /* LCD_C LCD frequency divider: /7 */
#define LCDDIV__8           (0x3800)  /* LCD_C LCD frequency divider: /8 */
#define LCDDIV__9           (0x4000)  /* LCD_C LCD frequency divider: /9 */
#define LCDDIV__10          (0x4800)  /* LCD_C LCD frequency divider: /10 */
#define LCDDIV__11          (0x5000)  /* LCD_C LCD frequency divider: /11 */
#define LCDDIV__12          (0x5800)  /* LCD_C LCD frequency divider: /12 */
#define LCDDIV__13          (0x6000)  /* LCD_C LCD frequency divider: /13 */
#define LCDDIV__14          (0x6800)  /* LCD_C LCD frequency divider: /14 */
#define LCDDIV__15          (0x7000)  /* LCD_C LCD frequency divider: /15 */
#define LCDDIV__16          (0x7800)  /* LCD_C LCD frequency divider: /16 */
#define LCDDIV__17          (0x8000)  /* LCD_C LCD frequency divider: /17 */
#define LCDDIV__18          (0x8800)  /* LCD_C LCD frequency divider: /18 */
#define LCDDIV__19          (0x9000)  /* LCD_C LCD frequency divider: /19 */
#define LCDDIV__20          (0x9800)  /* LCD_C LCD frequency divider: /20 */
#define LCDDIV__21          (0xA000)  /* LCD_C LCD frequency divider: /21 */
#define LCDDIV__22          (0xA800)  /* LCD_C LCD frequency divider: /22 */
#define LCDDIV__23          (0xB000)  /* LCD_C LCD frequency divider: /23 */
#define LCDDIV__24          (0xB800)  /* LCD_C LCD frequency divider: /24 */
#define LCDDIV__25          (0xC000)  /* LCD_C LCD frequency divider: /25 */
#define LCDDIV__26          (0xC800)  /* LCD_C LCD frequency divider: /26 */
#define LCDDIV__27          (0xD000)  /* LCD_C LCD frequency divider: /27 */
#define LCDDIV__28          (0xD800)  /* LCD_C LCD frequency divider: /28 */
#define LCDDIV__29          (0xE000)  /* LCD_C LCD frequency divider: /29 */
#define LCDDIV__30          (0xE800)  /* LCD_C LCD frequency divider: /30 */
#define LCDDIV__31          (0xF000)  /* LCD_C LCD frequency divider: /31 */
#define LCDDIV__32          (0xF800)  /* LCD_C LCD frequency divider: /32 */


/* Display modes coded with Bits 2-4 */
#define LCDSTATIC           (LCDSON)
#define LCD2MUX             (LCDMX0+LCDSON)
#define LCD3MUX             (LCDMX1+LCDSON)
#define LCD4MUX             (LCDMX1+LCDMX0+LCDSON)
#define LCD5MUX             (LCDMX2+LCDSON)
#define LCD6MUX             (LCDMX2+LCDMX0+LCDSON)
#define LCD7MUX             (LCDMX2+LCDMX1+LCDSON)
#define LCD8MUX             (LCDMX2+LCDMX1+LCDMX0+LCDSON)

// LCDCCTL1
#define LCDFRMIFG           (0x0001)  /* LCD_C LCD frame interrupt flag */
#define LCDBLKOFFIFG        (0x0002)  /* LCD_C LCD blinking off interrupt flag, */
#define LCDBLKONIFG         (0x0004)  /* LCD_C LCD blinking on interrupt flag, */
#define LCDNOCAPIFG         (0x0008)  /* LCD_C No cpacitance connected interrupt flag */
#define LCDFRMIE            (0x0100)  /* LCD_C LCD frame interrupt enable */
#define LCDBLKOFFIE         (0x0200)  /* LCD_C LCD blinking off interrupt flag, */
#define LCDBLKONIE          (0x0400)  /* LCD_C LCD blinking on interrupt flag, */
#define LCDNOCAPIE          (0x0800)  /* LCD_C No cpacitance connected interrupt enable */

// LCDCCTL1
#define LCDFRMIFG_L         (0x0001)  /* LCD_C LCD frame interrupt flag */
#define LCDBLKOFFIFG_L      (0x0002)  /* LCD_C LCD blinking off interrupt flag, */
#define LCDBLKONIFG_L       (0x0004)  /* LCD_C LCD blinking on interrupt flag, */
#define LCDNOCAPIFG_L       (0x0008)  /* LCD_C No cpacitance connected interrupt flag */

// LCDCCTL1
#define LCDFRMIE_H          (0x0001)  /* LCD_C LCD frame interrupt enable */
#define LCDBLKOFFIE_H       (0x0002)  /* LCD_C LCD blinking off interrupt flag, */
#define LCDBLKONIE_H        (0x0004)  /* LCD_C LCD blinking on interrupt flag, */
#define LCDNOCAPIE_H        (0x0008)  /* LCD_C No cpacitance connected interrupt enable */

// LCDCBLKCTL
#define LCDBLKMOD0          (0x0001)  /* LCD_C Blinking mode Bit: 0 */
#define LCDBLKMOD1          (0x0002)  /* LCD_C Blinking mode Bit: 1 */
#define LCDBLKPRE0          (0x0004)  /* LCD_C Clock pre-scaler for blinking frequency Bit: 0 */
#define LCDBLKPRE1          (0x0008)  /* LCD_C Clock pre-scaler for blinking frequency Bit: 1 */
#define LCDBLKPRE2          (0x0010)  /* LCD_C Clock pre-scaler for blinking frequency Bit: 2 */
#define LCDBLKDIV0          (0x0020)  /* LCD_C Clock divider for blinking frequency Bit: 0 */
#define LCDBLKDIV1          (0x0040)  /* LCD_C Clock divider for blinking frequency Bit: 1 */
#define LCDBLKDIV2          (0x0080)  /* LCD_C Clock divider for blinking frequency Bit: 2 */

// LCDCBLKCTL
#define LCDBLKMOD0_L        (0x0001)  /* LCD_C Blinking mode Bit: 0 */
#define LCDBLKMOD1_L        (0x0002)  /* LCD_C Blinking mode Bit: 1 */
#define LCDBLKPRE0_L        (0x0004)  /* LCD_C Clock pre-scaler for blinking frequency Bit: 0 */
#define LCDBLKPRE1_L        (0x0008)  /* LCD_C Clock pre-scaler for blinking frequency Bit: 1 */
#define LCDBLKPRE2_L        (0x0010)  /* LCD_C Clock pre-scaler for blinking frequency Bit: 2 */
#define LCDBLKDIV0_L        (0x0020)  /* LCD_C Clock divider for blinking frequency Bit: 0 */
#define LCDBLKDIV1_L        (0x0040)  /* LCD_C Clock divider for blinking frequency Bit: 1 */
#define LCDBLKDIV2_L        (0x0080)  /* LCD_C Clock divider for blinking frequency Bit: 2 */


#define LCDBLKMOD_0         (0x0000)  /* LCD_C Blinking mode: Off */
#define LCDBLKMOD_1         (0x0001)  /* LCD_C Blinking mode: Individual */
#define LCDBLKMOD_2         (0x0002)  /* LCD_C Blinking mode: All */
#define LCDBLKMOD_3         (0x0003)  /* LCD_C Blinking mode: Switching */

#define LCDBLKPRE_0         (0x0000)  /* LCD_C Clock pre-scaler for blinking frequency: 0 */
#define LCDBLKPRE_1         (0x0004)  /* LCD_C Clock pre-scaler for blinking frequency: 1 */
#define LCDBLKPRE_2         (0x0008)  /* LCD_C Clock pre-scaler for blinking frequency: 2 */
#define LCDBLKPRE_3         (0x000C)  /* LCD_C Clock pre-scaler for blinking frequency: 3 */
#define LCDBLKPRE_4         (0x0010)  /* LCD_C Clock pre-scaler for blinking frequency: 4 */
#define LCDBLKPRE_5         (0x0014)  /* LCD_C Clock pre-scaler for blinking frequency: 5 */
#define LCDBLKPRE_6         (0x0018)  /* LCD_C Clock pre-scaler for blinking frequency: 6 */
#define LCDBLKPRE_7         (0x001C)  /* LCD_C Clock pre-scaler for blinking frequency: 7 */

#define LCDBLKPRE__512      (0x0000)  /* LCD_C Clock pre-scaler for blinking frequency: 512   */
#define LCDBLKPRE__1024     (0x0004)  /* LCD_C Clock pre-scaler for blinking frequency: 1024  */
#define LCDBLKPRE__2048     (0x0008)  /* LCD_C Clock pre-scaler for blinking frequency: 2048  */
#define LCDBLKPRE__4096     (0x000C)  /* LCD_C Clock pre-scaler for blinking frequency: 4096  */
#define LCDBLKPRE__8192     (0x0010)  /* LCD_C Clock pre-scaler for blinking frequency: 8192  */
#define LCDBLKPRE__16384    (0x0014)  /* LCD_C Clock pre-scaler for blinking frequency: 16384 */
#define LCDBLKPRE__32768    (0x0018)  /* LCD_C Clock pre-scaler for blinking frequency: 32768 */
#define LCDBLKPRE__65536    (0x001C)  /* LCD_C Clock pre-scaler for blinking frequency: 65536 */

#define LCDBLKDIV_0         (0x0000)  /* LCD_C Clock divider for blinking frequency: 0 */
#define LCDBLKDIV_1         (0x0020)  /* LCD_C Clock divider for blinking frequency: 1 */
#define LCDBLKDIV_2         (0x0040)  /* LCD_C Clock divider for blinking frequency: 2 */
#define LCDBLKDIV_3         (0x0060)  /* LCD_C Clock divider for blinking frequency: 3 */
#define LCDBLKDIV_4         (0x0080)  /* LCD_C Clock divider for blinking frequency: 4 */
#define LCDBLKDIV_5         (0x00A0)  /* LCD_C Clock divider for blinking frequency: 5 */
#define LCDBLKDIV_6         (0x00C0)  /* LCD_C Clock divider for blinking frequency: 6 */
#define LCDBLKDIV_7         (0x00E0)  /* LCD_C Clock divider for blinking frequency: 7 */

#define LCDBLKDIV__1        (0x0000)  /* LCD_C Clock divider for blinking frequency: /1 */
#define LCDBLKDIV__2        (0x0020)  /* LCD_C Clock divider for blinking frequency: /2 */
#define LCDBLKDIV__3        (0x0040)  /* LCD_C Clock divider for blinking frequency: /3 */
#define LCDBLKDIV__4        (0x0060)  /* LCD_C Clock divider for blinking frequency: /4 */
#define LCDBLKDIV__5        (0x0080)  /* LCD_C Clock divider for blinking frequency: /5 */
#define LCDBLKDIV__6        (0x00A0)  /* LCD_C Clock divider for blinking frequency: /6 */
#define LCDBLKDIV__7        (0x00C0)  /* LCD_C Clock divider for blinking frequency: /7 */
#define LCDBLKDIV__8        (0x00E0)  /* LCD_C Clock divider for blinking frequency: /8 */


// LCDCMEMCTL
#define LCDDISP             (0x0001)  /* LCD_C LCD memory registers for display */
#define LCDCLRM             (0x0002)  /* LCD_C Clear LCD memory */
#define LCDCLRBM            (0x0004)  /* LCD_C Clear LCD blinking memory */

// LCDCMEMCTL
#define LCDDISP_L           (0x0001)  /* LCD_C LCD memory registers for display */
#define LCDCLRM_L           (0x0002)  /* LCD_C Clear LCD memory */
#define LCDCLRBM_L          (0x0004)  /* LCD_C Clear LCD blinking memory */


// LCDCVCTL
#define LCD2B               (0x0001)  /* Selects 1/2 bias. */
#define VLCDREF0            (0x0002)  /* Selects reference voltage for regulated charge pump: 0 */
#define VLCDREF1            (0x0004)  /* Selects reference voltage for regulated charge pump: 1 */
#define LCDCPEN             (0x0008)  /* LCD Voltage Charge Pump Enable. */
#define VLCDEXT             (0x0010)  /* Select external source for VLCD. */
#define LCDEXTBIAS          (0x0020)  /* V2 - V4 voltage select. */
#define R03EXT              (0x0040)  /* Selects external connections for LCD mid voltages. */
#define LCDREXT             (0x0080)  /* Selects external connection for lowest LCD voltage. */
#define VLCD0               (0x0200)  /* VLCD select: 0 */
#define VLCD1               (0x0400)  /* VLCD select: 1 */
#define VLCD2               (0x0800)  /* VLCD select: 2 */
#define VLCD3               (0x1000)  /* VLCD select: 3 */
#define VLCD4               (0x2000)  /* VLCD select: 4 */
#define VLCD5               (0x4000)  /* VLCD select: 5 */

// LCDCVCTL
#define LCD2B_L             (0x0001)  /* Selects 1/2 bias. */
#define VLCDREF0_L          (0x0002)  /* Selects reference voltage for regulated charge pump: 0 */
#define VLCDREF1_L          (0x0004)  /* Selects reference voltage for regulated charge pump: 1 */
#define LCDCPEN_L           (0x0008)  /* LCD Voltage Charge Pump Enable. */
#define VLCDEXT_L           (0x0010)  /* Select external source for VLCD. */
#define LCDEXTBIAS_L        (0x0020)  /* V2 - V4 voltage select. */
#define R03EXT_L            (0x0040)  /* Selects external connections for LCD mid voltages. */
#define LCDREXT_L           (0x0080)  /* Selects external connection for lowest LCD voltage. */

// LCDCVCTL
#define VLCD0_H             (0x0002)  /* VLCD select: 0 */
#define VLCD1_H             (0x0004)  /* VLCD select: 1 */
#define VLCD2_H             (0x0008)  /* VLCD select: 2 */
#define VLCD3_H             (0x0010)  /* VLCD select: 3 */
#define VLCD4_H             (0x0020)  /* VLCD select: 4 */
#define VLCD5_H             (0x0040)  /* VLCD select: 5 */

/* Reference voltage source select for the regulated charge pump */
#define VLCDREF_0              (0x0000)       /* Internal */
#define VLCDREF_1              (0x0002)       /* External */
#define VLCDREF_2              (0x0004)       /* Reserved */
#define VLCDREF_3              (0x0006)       /* Reserved */


/* Charge pump voltage selections */
#define VLCD_0                 (0x0000)       /* Charge pump disabled */
#define VLCD_1                 (0x0200)       /* VLCD = 2.60V */
#define VLCD_2                 (0x0400)       /* VLCD = 2.66V */
#define VLCD_3                 (0x0600)       /* VLCD = 2.72V */
#define VLCD_4                 (0x0800)       /* VLCD = 2.78V */
#define VLCD_5                 (0x0A00)       /* VLCD = 2.84V */
#define VLCD_6                 (0x0C00)       /* VLCD = 2.90V */
#define VLCD_7                 (0x0E00)       /* VLCD = 2.96V */
#define VLCD_8                 (0x1000)       /* VLCD = 3.02V */
#define VLCD_9                 (0x1200)       /* VLCD = 3.08V */
#define VLCD_10                (0x1400)       /* VLCD = 3.14V */
#define VLCD_11                (0x1600)       /* VLCD = 3.20V */
#define VLCD_12                (0x1800)       /* VLCD = 3.26V */
#define VLCD_13                (0x1A00)       /* VLCD = 3.32V */
#define VLCD_14                (0x1C00)       /* VLCD = 3.38V */
#define VLCD_15                (0x1E00)       /* VLCD = 3.44V */

#define VLCD_DISABLED          (0x0000)       /* Charge pump disabled */
#define VLCD_2_60              (0x0200)       /* VLCD = 2.60V */
#define VLCD_2_66              (0x0400)       /* VLCD = 2.66V */
#define VLCD_2_72              (0x0600)       /* VLCD = 2.72V */
#define VLCD_2_78              (0x0800)       /* VLCD = 2.78V */
#define VLCD_2_84              (0x0A00)       /* VLCD = 2.84V */
#define VLCD_2_90              (0x0C00)       /* VLCD = 2.90V */
#define VLCD_2_96              (0x0E00)       /* VLCD = 2.96V */
#define VLCD_3_02              (0x1000)       /* VLCD = 3.02V */
#define VLCD_3_08              (0x1200)       /* VLCD = 3.08V */
#define VLCD_3_14              (0x1400)       /* VLCD = 3.14V */
#define VLCD_3_20              (0x1600)       /* VLCD = 3.20V */
#define VLCD_3_26              (0x1800)       /* VLCD = 3.26V */
#define VLCD_3_32              (0x1A00)       /* VLCD = 3.32V */
#define VLCD_3_38              (0x1C00)       /* VLCD = 3.38V */
#define VLCD_3_44              (0x1E00)       /* VLCD = 3.44V */


// LCDCPCTL0
#define LCDS0               (0x0001)  /* LCD Segment  0 enable. */
#define LCDS1               (0x0002)  /* LCD Segment  1 enable. */
#define LCDS2               (0x0004)  /* LCD Segment  2 enable. */
#define LCDS3               (0x0008)  /* LCD Segment  3 enable. */
#define LCDS4               (0x0010)  /* LCD Segment  4 enable. */
#define LCDS5               (0x0020)  /* LCD Segment  5 enable. */
#define LCDS6               (0x0040)  /* LCD Segment  6 enable. */
#define LCDS7               (0x0080)  /* LCD Segment  7 enable. */
#define LCDS8               (0x0100)  /* LCD Segment  8 enable. */
#define LCDS9               (0x0200)  /* LCD Segment  9 enable. */
#define LCDS10              (0x0400)  /* LCD Segment 10 enable. */
#define LCDS11              (0x0800)  /* LCD Segment 11 enable. */
#define LCDS12              (0x1000)  /* LCD Segment 12 enable. */
#define LCDS13              (0x2000)  /* LCD Segment 13 enable. */
#define LCDS14              (0x4000)  /* LCD Segment 14 enable. */
#define LCDS15              (0x8000)  /* LCD Segment 15 enable. */

// LCDCPCTL0
#define LCDS0_L             (0x0001)  /* LCD Segment  0 enable. */
#define LCDS1_L             (0x0002)  /* LCD Segment  1 enable. */
#define LCDS2_L             (0x0004)  /* LCD Segment  2 enable. */
#define LCDS3_L             (0x0008)  /* LCD Segment  3 enable. */
#define LCDS4_L             (0x0010)  /* LCD Segment  4 enable. */
#define LCDS5_L             (0x0020)  /* LCD Segment  5 enable. */
#define LCDS6_L             (0x0040)  /* LCD Segment  6 enable. */
#define LCDS7_L             (0x0080)  /* LCD Segment  7 enable. */

// LCDCPCTL0
#define LCDS8_H             (0x0001)  /* LCD Segment  8 enable. */
#define LCDS9_H             (0x0002)  /* LCD Segment  9 enable. */
#define LCDS10_H            (0x0004)  /* LCD Segment 10 enable. */
#define LCDS11_H            (0x0008)  /* LCD Segment 11 enable. */
#define LCDS12_H            (0x0010)  /* LCD Segment 12 enable. */
#define LCDS13_H            (0x0020)  /* LCD Segment 13 enable. */
#define LCDS14_H            (0x0040)  /* LCD Segment 14 enable. */
#define LCDS15_H            (0x0080)  /* LCD Segment 15 enable. */

// LCDCPCTL1
#define LCDS16              (0x0001)  /* LCD Segment 16 enable. */
#define LCDS17              (0x0002)  /* LCD Segment 17 enable. */
#define LCDS18              (0x0004)  /* LCD Segment 18 enable. */
#define LCDS19              (0x0008)  /* LCD Segment 19 enable. */
#define LCDS20              (0x0010)  /* LCD Segment 20 enable. */
#define LCDS21              (0x0020)  /* LCD Segment 21 enable. */
#define LCDS22              (0x0040)  /* LCD Segment 22 enable. */
#define LCDS23              (0x0080)  /* LCD Segment 23 enable. */
#define LCDS24              (0x0100)  /* LCD Segment 24 enable. */
#define LCDS25              (0x0200)  /* LCD Segment 25 enable. */
#define LCDS26              (0x0400)  /* LCD Segment 26 enable. */
#define LCDS27              (0x0800)  /* LCD Segment 27 enable. */
#define LCDS28              (0x1000)  /* LCD Segment 28 enable. */
#define LCDS29              (0x2000)  /* LCD Segment 29 enable. */
#define LCDS30              (0x4000)  /* LCD Segment 30 enable. */
#define LCDS31              (0x8000)  /* LCD Segment 31 enable. */

// LCDCPCTL1
#define LCDS16_L            (0x0001)  /* LCD Segment 16 enable. */
#define LCDS17_L            (0x0002)  /* LCD Segment 17 enable. */
#define LCDS18_L            (0x0004)  /* LCD Segment 18 enable. */
#define LCDS19_L            (0x0008)  /* LCD Segment 19 enable. */
#define LCDS20_L            (0x0010)  /* LCD Segment 20 enable. */
#define LCDS21_L            (0x0020)  /* LCD Segment 21 enable. */
#define LCDS22_L            (0x0040)  /* LCD Segment 22 enable. */
#define LCDS23_L            (0x0080)  /* LCD Segment 23 enable. */

// LCDCPCTL1
#define LCDS24_H            (0x0001)  /* LCD Segment 24 enable. */
#define LCDS25_H            (0x0002)  /* LCD Segment 25 enable. */
#define LCDS26_H            (0x0004)  /* LCD Segment 26 enable. */
#define LCDS27_H            (0x0008)  /* LCD Segment 27 enable. */
#define LCDS28_H            (0x0010)  /* LCD Segment 28 enable. */
#define LCDS29_H            (0x0020)  /* LCD Segment 29 enable. */
#define LCDS30_H            (0x0040)  /* LCD Segment 30 enable. */
#define LCDS31_H            (0x0080)  /* LCD Segment 31 enable. */

// LCDCPCTL2
#define LCDS32              (0x0001)  /* LCD Segment 32 enable. */
#define LCDS33              (0x0002)  /* LCD Segment 33 enable. */
#define LCDS34              (0x0004)  /* LCD Segment 34 enable. */
#define LCDS35              (0x0008)  /* LCD Segment 35 enable. */
#define LCDS36              (0x0010)  /* LCD Segment 36 enable. */
#define LCDS37              (0x0020)  /* LCD Segment 37 enable. */
#define LCDS38              (0x0040)  /* LCD Segment 38 enable. */
#define LCDS39              (0x0080)  /* LCD Segment 39 enable. */
#define LCDS40              (0x0100)  /* LCD Segment 40 enable. */
#define LCDS41              (0x0200)  /* LCD Segment 41 enable. */
#define LCDS42              (0x0400)  /* LCD Segment 42 enable. */
#define LCDS43              (0x0800)  /* LCD Segment 43 enable. */
#define LCDS44              (0x1000)  /* LCD Segment 44 enable. */
#define LCDS45              (0x2000)  /* LCD Segment 45 enable. */
#define LCDS46              (0x4000)  /* LCD Segment 46 enable. */
#define LCDS47              (0x8000)  /* LCD Segment 47 enable. */

// LCDCPCTL2
#define LCDS32_L            (0x0001)  /* LCD Segment 32 enable. */
#define LCDS33_L            (0x0002)  /* LCD Segment 33 enable. */
#define LCDS34_L            (0x0004)  /* LCD Segment 34 enable. */
#define LCDS35_L            (0x0008)  /* LCD Segment 35 enable. */
#define LCDS36_L            (0x0010)  /* LCD Segment 36 enable. */
#define LCDS37_L            (0x0020)  /* LCD Segment 37 enable. */
#define LCDS38_L            (0x0040)  /* LCD Segment 38 enable. */
#define LCDS39_L            (0x0080)  /* LCD Segment 39 enable. */

// LCDCPCTL2
#define LCDS40_H            (0x0001)  /* LCD Segment 40 enable. */
#define LCDS41_H            (0x0002)  /* LCD Segment 41 enable. */
#define LCDS42_H            (0x0004)  /* LCD Segment 42 enable. */
#define LCDS43_H            (0x0008)  /* LCD Segment 43 enable. */
#define LCDS44_H            (0x0010)  /* LCD Segment 44 enable. */
#define LCDS45_H            (0x0020)  /* LCD Segment 45 enable. */
#define LCDS46_H            (0x0040)  /* LCD Segment 46 enable. */
#define LCDS47_H            (0x0080)  /* LCD Segment 47 enable. */


// LCDCCPCTL
#define LCDCPDIS0           (0x0001)  /* LCD charge pump disable */
#define LCDCPDIS1           (0x0002)  /* LCD charge pump disable */
#define LCDCPDIS2           (0x0004)  /* LCD charge pump disable */
#define LCDCPDIS3           (0x0008)  /* LCD charge pump disable */
#define LCDCPDIS4           (0x0010)  /* LCD charge pump disable */
#define LCDCPDIS5           (0x0020)  /* LCD charge pump disable */
#define LCDCPDIS6           (0x0040)  /* LCD charge pump disable */
#define LCDCPDIS7           (0x0080)  /* LCD charge pump disable */
#define LCDCPCLKSYNC        (0x8000)  /* LCD charge pump clock synchronization */

// LCDCCPCTL
#define LCDCPDIS0_L         (0x0001)  /* LCD charge pump disable */
#define LCDCPDIS1_L         (0x0002)  /* LCD charge pump disable */
#define LCDCPDIS2_L         (0x0004)  /* LCD charge pump disable */
#define LCDCPDIS3_L         (0x0008)  /* LCD charge pump disable */
#define LCDCPDIS4_L         (0x0010)  /* LCD charge pump disable */
#define LCDCPDIS5_L         (0x0020)  /* LCD charge pump disable */
#define LCDCPDIS6_L         (0x0040)  /* LCD charge pump disable */
#define LCDCPDIS7_L         (0x0080)  /* LCD charge pump disable */

// LCDCCPCTL
#define LCDCPCLKSYNC_H      (0x0080)  /* LCD charge pump clock synchronization */

#define LCDM1_                0x0A20    /* LCD Memory 1 */
sfrb(LCDM1, LCDM1_);
#define LCDMEM_             LCDM1     /* LCD Memory */
#ifndef __STDC__
#define LCDMEM              LCDM1     /* LCD Memory (for assembler) */
#else
#define LCDMEM              ((volatile char*) &LCDM1) /* LCD Memory (for C) */
#endif
#define LCDM2_                0x0A21    /* LCD Memory 2 */
sfrb(LCDM2, LCDM2_);
#define LCDM3_                0x0A22    /* LCD Memory 3 */
sfrb(LCDM3, LCDM3_);
#define LCDM4_                0x0A23    /* LCD Memory 4 */
sfrb(LCDM4, LCDM4_);
#define LCDM5_                0x0A24    /* LCD Memory 5 */
sfrb(LCDM5, LCDM5_);
#define LCDM6_                0x0A25    /* LCD Memory 6 */
sfrb(LCDM6, LCDM6_);
#define LCDM7_                0x0A26    /* LCD Memory 7 */
sfrb(LCDM7, LCDM7_);
#define LCDM8_                0x0A27    /* LCD Memory 8 */
sfrb(LCDM8, LCDM8_);
#define LCDM9_                0x0A28    /* LCD Memory 9 */
sfrb(LCDM9, LCDM9_);
#define LCDM10_               0x0A29    /* LCD Memory 10 */
sfrb(LCDM10, LCDM10_);
#define LCDM11_               0x0A2A    /* LCD Memory 11 */
sfrb(LCDM11, LCDM11_);
#define LCDM12_               0x0A2B    /* LCD Memory 12 */
sfrb(LCDM12, LCDM12_);
#define LCDM13_               0x0A2C    /* LCD Memory 13 */
sfrb(LCDM13, LCDM13_);
#define LCDM14_               0x0A2D    /* LCD Memory 14 */
sfrb(LCDM14, LCDM14_);
#define LCDM15_               0x0A2E    /* LCD Memory 15 */
sfrb(LCDM15, LCDM15_);
#define LCDM16_               0x0A2F    /* LCD Memory 16 */
sfrb(LCDM16, LCDM16_);
#define LCDM17_               0x0A30    /* LCD Memory 17 */
sfrb(LCDM17, LCDM17_);
#define LCDM18_               0x0A31    /* LCD Memory 18 */
sfrb(LCDM18, LCDM18_);
#define LCDM19_               0x0A32    /* LCD Memory 19 */
sfrb(LCDM19, LCDM19_);
#define LCDM20_               0x0A33    /* LCD Memory 20 */
sfrb(LCDM20, LCDM20_);
#define LCDM21_               0x0A34    /* LCD Memory 21 */
sfrb(LCDM21, LCDM21_);
#define LCDM22_               0x0A35    /* LCD Memory 22 */
sfrb(LCDM22, LCDM22_);
#define LCDM23_               0x0A36    /* LCD Memory 23 */
sfrb(LCDM23, LCDM23_);
#define LCDM24_               0x0A37    /* LCD Memory 24 */
sfrb(LCDM24, LCDM24_);
#define LCDM25_               0x0A38    /* LCD Memory 25 */
sfrb(LCDM25, LCDM25_);
#define LCDM26_               0x0A39    /* LCD Memory 26 */
sfrb(LCDM26, LCDM26_);
#define LCDM27_               0x0A3A    /* LCD Memory 27 */
sfrb(LCDM27, LCDM27_);
#define LCDM28_               0x0A3B    /* LCD Memory 28 */
sfrb(LCDM28, LCDM28_);
#define LCDM29_               0x0A3C    /* LCD Memory 29 */
sfrb(LCDM29, LCDM29_);
#define LCDM30_               0x0A3D    /* LCD Memory 30 */
sfrb(LCDM30, LCDM30_);
#define LCDM31_               0x0A3E    /* LCD Memory 31 */
sfrb(LCDM31, LCDM31_);
#define LCDM32_               0x0A3F    /* LCD Memory 32 */
sfrb(LCDM32, LCDM32_);
#define LCDM33_               0x0A40    /* LCD Memory 33 */
sfrb(LCDM33, LCDM33_);
#define LCDM34_               0x0A41    /* LCD Memory 34 */
sfrb(LCDM34, LCDM34_);
#define LCDM35_               0x0A42    /* LCD Memory 35 */
sfrb(LCDM35, LCDM35_);
#define LCDM36_               0x0A43    /* LCD Memory 36 */
sfrb(LCDM36, LCDM36_);
#define LCDM37_               0x0A44    /* LCD Memory 37 */
sfrb(LCDM37, LCDM37_);
#define LCDM38_               0x0A45    /* LCD Memory 38 */
sfrb(LCDM38, LCDM38_);
#define LCDM39_               0x0A46    /* LCD Memory 39 */
sfrb(LCDM39, LCDM39_);
#define LCDM40_               0x0A47    /* LCD Memory 40 */
sfrb(LCDM40, LCDM40_);

#define LCDBM1_               0x0A40    /* LCD Blinking Memory 1 */
sfrb(LCDBM1, LCDBM1_);
#define LCDBMEM_            LCDBM1    /* LCD Blinking Memory */
#ifndef __STDC__
#define LCDBMEM             (LCDBM1)  /* LCD Blinking Memory (for assembler) */
#else
#define LCDBMEM             ((volatile char*) &LCDBM1) /* LCD Blinking Memory (for C) */
#endif
#define LCDBM2_               0x0A41    /* LCD Blinking Memory 2 */
sfrb(LCDBM2, LCDBM2_);
#define LCDBM3_               0x0A42    /* LCD Blinking Memory 3 */
sfrb(LCDBM3, LCDBM3_);
#define LCDBM4_               0x0A43    /* LCD Blinking Memory 4 */
sfrb(LCDBM4, LCDBM4_);
#define LCDBM5_               0x0A44    /* LCD Blinking Memory 5 */
sfrb(LCDBM5, LCDBM5_);
#define LCDBM6_               0x0A45    /* LCD Blinking Memory 6 */
sfrb(LCDBM6, LCDBM6_);
#define LCDBM7_               0x0A46    /* LCD Blinking Memory 7 */
sfrb(LCDBM7, LCDBM7_);
#define LCDBM8_               0x0A47    /* LCD Blinking Memory 8 */
sfrb(LCDBM8, LCDBM8_);
#define LCDBM9_               0x0A48    /* LCD Blinking Memory 9 */
sfrb(LCDBM9, LCDBM9_);
#define LCDBM10_              0x0A49    /* LCD Blinking Memory 10 */
sfrb(LCDBM10, LCDBM10_);
#define LCDBM11_              0x0A4A    /* LCD Blinking Memory 11 */
sfrb(LCDBM11, LCDBM11_);
#define LCDBM12_              0x0A4B    /* LCD Blinking Memory 12 */
sfrb(LCDBM12, LCDBM12_);
#define LCDBM13_              0x0A4C    /* LCD Blinking Memory 13 */
sfrb(LCDBM13, LCDBM13_);
#define LCDBM14_              0x0A4D    /* LCD Blinking Memory 14 */
sfrb(LCDBM14, LCDBM14_);
#define LCDBM15_              0x0A4E    /* LCD Blinking Memory 15 */
sfrb(LCDBM15, LCDBM15_);
#define LCDBM16_              0x0A4F    /* LCD Blinking Memory 16 */
sfrb(LCDBM16, LCDBM16_);
#define LCDBM17_              0x0A50    /* LCD Blinking Memory 17 */
sfrb(LCDBM17, LCDBM17_);
#define LCDBM18_              0x0A51    /* LCD Blinking Memory 18 */
sfrb(LCDBM18, LCDBM18_);
#define LCDBM19_              0x0A52    /* LCD Blinking Memory 19 */
sfrb(LCDBM19, LCDBM19_);
#define LCDBM20_              0x0A53    /* LCD Blinking Memory 20 */
sfrb(LCDBM20, LCDBM20_);

/* LCDCIV Definitions */
#define LCDCIV_NONE         (0x0000)    /* No Interrupt pending */
#define LCDCIV_LCDNOCAPIFG  (0x0002)    /* No capacitor connected */
#define LCDCIV_LCDCLKOFFIFG (0x0004)    /* Blink, segments off */
#define LCDCIV_LCDCLKONIFG  (0x0006)    /* Blink, segments on */
#define LCDCIV_LCDFRMIFG    (0x0008)    /* Frame interrupt */

/************************************************************
* HARDWARE MULTIPLIER 32Bit
************************************************************/
#define __MSP430_HAS_MPY32__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_MPY32__ 0x04C0
#define MPY32_BASE __MSP430_BASEADDRESS_MPY32__

#define MPY_                  0x04C0    /* Multiply Unsigned/Operand 1 */
sfrb(MPY_L , MPY_);
sfrb(MPY_H , MPY_+1);
sfrw(MPY, MPY_);
#define MPYS_                 0x04C2    /* Multiply Signed/Operand 1 */
sfrb(MPYS_L , MPYS_);
sfrb(MPYS_H , MPYS_+1);
sfrw(MPYS, MPYS_);
#define MAC_                  0x04C4    /* Multiply Unsigned and Accumulate/Operand 1 */
sfrb(MAC_L , MAC_);
sfrb(MAC_H , MAC_+1);
sfrw(MAC, MAC_);
#define MACS_                 0x04C6    /* Multiply Signed and Accumulate/Operand 1 */
sfrb(MACS_L , MACS_);
sfrb(MACS_H , MACS_+1);
sfrw(MACS, MACS_);
#define OP2_                  0x04C8    /* Operand 2 */
sfrb(OP2_L , OP2_);
sfrb(OP2_H , OP2_+1);
sfrw(OP2, OP2_);
#define RESLO_                0x04CA    /* Result Low Word */
sfrb(RESLO_L , RESLO_);
sfrb(RESLO_H , RESLO_+1);
sfrw(RESLO, RESLO_);
#define RESHI_                0x04CC    /* Result High Word */
sfrb(RESHI_L , RESHI_);
sfrb(RESHI_H , RESHI_+1);
sfrw(RESHI, RESHI_);
#define SUMEXT_               0x04CE    /* Sum Extend */
const_sfrb(SUMEXT_L , SUMEXT_);
const_sfrb(SUMEXT_H , SUMEXT_+1);
const_sfrw(SUMEXT, SUMEXT_);

#define MPY32L_               0x04D0    /* 32-bit operand 1 - multiply - low word */
sfrb(MPY32L_L , MPY32L_);
sfrb(MPY32L_H , MPY32L_+1);
sfrw(MPY32L, MPY32L_);
#define MPY32H_               0x04D2    /* 32-bit operand 1 - multiply - high word */
sfrb(MPY32H_L , MPY32H_);
sfrb(MPY32H_H , MPY32H_+1);
sfrw(MPY32H, MPY32H_);
#define MPYS32L_              0x04D4    /* 32-bit operand 1 - signed multiply - low word */
sfrb(MPYS32L_L , MPYS32L_);
sfrb(MPYS32L_H , MPYS32L_+1);
sfrw(MPYS32L, MPYS32L_);
#define MPYS32H_              0x04D6    /* 32-bit operand 1 - signed multiply - high word */
sfrb(MPYS32H_L , MPYS32H_);
sfrb(MPYS32H_H , MPYS32H_+1);
sfrw(MPYS32H, MPYS32H_);
#define MAC32L_               0x04D8    /* 32-bit operand 1 - multiply accumulate - low word */
sfrb(MAC32L_L , MAC32L_);
sfrb(MAC32L_H , MAC32L_+1);
sfrw(MAC32L, MAC32L_);
#define MAC32H_               0x04DA    /* 32-bit operand 1 - multiply accumulate - high word */
sfrb(MAC32H_L , MAC32H_);
sfrb(MAC32H_H , MAC32H_+1);
sfrw(MAC32H, MAC32H_);
#define MACS32L_              0x04DC    /* 32-bit operand 1 - signed multiply accumulate - low word */
sfrb(MACS32L_L , MACS32L_);
sfrb(MACS32L_H , MACS32L_+1);
sfrw(MACS32L, MACS32L_);
#define MACS32H_              0x04DE    /* 32-bit operand 1 - signed multiply accumulate - high word */
sfrb(MACS32H_L , MACS32H_);
sfrb(MACS32H_H , MACS32H_+1);
sfrw(MACS32H, MACS32H_);
#define OP2L_                 0x04E0    /* 32-bit operand 2 - low word */
sfrb(OP2L_L , OP2L_);
sfrb(OP2L_H , OP2L_+1);
sfrw(OP2L, OP2L_);
#define OP2H_                 0x04E2    /* 32-bit operand 2 - high word */
sfrb(OP2H_L , OP2H_);
sfrb(OP2H_H , OP2H_+1);
sfrw(OP2H, OP2H_);
#define RES0_                 0x04E4    /* 32x32-bit result 0 - least significant word */
sfrb(RES0_L , RES0_);
sfrb(RES0_H , RES0_+1);
sfrw(RES0, RES0_);
#define RES1_                 0x04E6    /* 32x32-bit result 1 */
sfrb(RES1_L , RES1_);
sfrb(RES1_H , RES1_+1);
sfrw(RES1, RES1_);
#define RES2_                 0x04E8    /* 32x32-bit result 2 */
sfrb(RES2_L , RES2_);
sfrb(RES2_H , RES2_+1);
sfrw(RES2, RES2_);
#define RES3_                 0x04EA    /* 32x32-bit result 3 - most significant word */
sfrb(RES3_L , RES3_);
sfrb(RES3_H , RES3_+1);
sfrw(RES3, RES3_);
#define MPY32CTL0_            0x04EC    /* MPY32 Control Register 0 */
sfrb(MPY32CTL0_L , MPY32CTL0_);
sfrb(MPY32CTL0_H , MPY32CTL0_+1);
sfrw(MPY32CTL0, MPY32CTL0_);

#define MPY_B               MPY_L      /* Multiply Unsigned/Operand 1 (Byte Access) */
#define MPYS_B              MPYS_L     /* Multiply Signed/Operand 1 (Byte Access) */
#define MAC_B               MAC_L      /* Multiply Unsigned and Accumulate/Operand 1 (Byte Access) */
#define MACS_B              MACS_L     /* Multiply Signed and Accumulate/Operand 1 (Byte Access) */
#define OP2_B               OP2_L      /* Operand 2 (Byte Access) */
#define MPY32L_B            MPY32L_L   /* 32-bit operand 1 - multiply - low word (Byte Access) */
#define MPY32H_B            MPY32H_L   /* 32-bit operand 1 - multiply - high word (Byte Access) */
#define MPYS32L_B           MPYS32L_L  /* 32-bit operand 1 - signed multiply - low word (Byte Access) */
#define MPYS32H_B           MPYS32H_L  /* 32-bit operand 1 - signed multiply - high word (Byte Access) */
#define MAC32L_B            MAC32L_L   /* 32-bit operand 1 - multiply accumulate - low word (Byte Access) */
#define MAC32H_B            MAC32H_L   /* 32-bit operand 1 - multiply accumulate - high word (Byte Access) */
#define MACS32L_B           MACS32L_L  /* 32-bit operand 1 - signed multiply accumulate - low word (Byte Access) */
#define MACS32H_B           MACS32H_L  /* 32-bit operand 1 - signed multiply accumulate - high word (Byte Access) */
#define OP2L_B              OP2L_L     /* 32-bit operand 2 - low word (Byte Access) */
#define OP2H_B              OP2H_L     /* 32-bit operand 2 - high word (Byte Access) */


/* MPY32CTL0 Control Bits */
#define MPYC                (0x0001)  /* Carry of the multiplier */
//#define RESERVED            (0x0002)  /* Reserved */
#define MPYFRAC             (0x0004)  /* Fractional mode */
#define MPYSAT              (0x0008)  /* Saturation mode */
#define MPYM0               (0x0010)  /* Multiplier mode Bit:0 */
#define MPYM1               (0x0020)  /* Multiplier mode Bit:1 */
#define OP1_32              (0x0040)  /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32              (0x0080)  /* Bit-width of operand 2 0:16Bit / 1:32Bit */
#define MPYDLYWRTEN         (0x0100)  /* Delayed write enable */
#define MPYDLY32            (0x0200)  /* Delayed write mode */

/* MPY32CTL0 Control Bits */
#define MPYC_L              (0x0001)  /* Carry of the multiplier */
//#define RESERVED            (0x0002)  /* Reserved */
#define MPYFRAC_L           (0x0004)  /* Fractional mode */
#define MPYSAT_L            (0x0008)  /* Saturation mode */
#define MPYM0_L             (0x0010)  /* Multiplier mode Bit:0 */
#define MPYM1_L             (0x0020)  /* Multiplier mode Bit:1 */
#define OP1_32_L            (0x0040)  /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32_L            (0x0080)  /* Bit-width of operand 2 0:16Bit / 1:32Bit */

/* MPY32CTL0 Control Bits */
//#define RESERVED            (0x0002)  /* Reserved */
#define MPYDLYWRTEN_H       (0x0001)  /* Delayed write enable */
#define MPYDLY32_H          (0x0002)  /* Delayed write mode */

#define MPYM_0              (0x0000)  /* Multiplier mode: MPY */
#define MPYM_1              (0x0010)  /* Multiplier mode: MPYS */
#define MPYM_2              (0x0020)  /* Multiplier mode: MAC */
#define MPYM_3              (0x0030)  /* Multiplier mode: MACS */
#define MPYM__MPY           (0x0000)  /* Multiplier mode: MPY */
#define MPYM__MPYS          (0x0010)  /* Multiplier mode: MPYS */
#define MPYM__MAC           (0x0020)  /* Multiplier mode: MAC */
#define MPYM__MACS          (0x0030)  /* Multiplier mode: MACS */

/************************************************************
* DIGITAL I/O Port1/2 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT1_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT1_R__ 0x0200
#define P1_BASE __MSP430_BASEADDRESS_PORT1_R__
#define __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT2_R__ 0x0200
#define P2_BASE __MSP430_BASEADDRESS_PORT2_R__
#define __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTA_R__ 0x0200
#define PA_BASE __MSP430_BASEADDRESS_PORTA_R__
#define __MSP430_HAS_P1SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P2SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PASEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P1SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P2SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PASEL1__         /* Define for DriverLib */

#define PAIN_                 0x0200    /* Port A Input */
const_sfrb(PAIN_L , PAIN_);
const_sfrb(PAIN_H , PAIN_+1);
const_sfrw(PAIN, PAIN_);
#define PAOUT_                0x0202    /* Port A Output */
sfrb(PAOUT_L , PAOUT_);
sfrb(PAOUT_H , PAOUT_+1);
sfrw(PAOUT, PAOUT_);
#define PADIR_                0x0204    /* Port A Direction */
sfrb(PADIR_L , PADIR_);
sfrb(PADIR_H , PADIR_+1);
sfrw(PADIR, PADIR_);
#define PAREN_                0x0206    /* Port A Resistor Enable */
sfrb(PAREN_L , PAREN_);
sfrb(PAREN_H , PAREN_+1);
sfrw(PAREN, PAREN_);
#define PADS_                 0x0208    /* Port A Drive Strenght */
sfrb(PADS_L , PADS_);
sfrb(PADS_H , PADS_+1);
sfrw(PADS, PADS_);
#define PASEL0_               0x020A    /* Port A Selection 0 */
sfrb(PASEL0_L , PASEL0_);
sfrb(PASEL0_H , PASEL0_+1);
sfrw(PASEL0, PASEL0_);
#define PASEL1_               0x020C    /* Port A Selection 1 */
sfrb(PASEL1_L , PASEL1_);
sfrb(PASEL1_H , PASEL1_+1);
sfrw(PASEL1, PASEL1_);
#define PAIES_                0x0218    /* Port A Interrupt Edge Select */
sfrb(PAIES_L , PAIES_);
sfrb(PAIES_H , PAIES_+1);
sfrw(PAIES, PAIES_);
#define PAIE_                 0x021A    /* Port A Interrupt Enable */
sfrb(PAIE_L , PAIE_);
sfrb(PAIE_H , PAIE_+1);
sfrw(PAIE, PAIE_);
#define PAIFG_                0x021C    /* Port A Interrupt Flag */
sfrb(PAIFG_L , PAIFG_);
sfrb(PAIFG_H , PAIFG_+1);
sfrw(PAIFG, PAIFG_);























#define P1IV_                 0x020E    /* Port 1 Interrupt Vector Word */
sfrw(P1IV, P1IV_);
#define P2IV_                 0x021E    /* Port 2 Interrupt Vector Word */
sfrw(P2IV, P2IV_);
#define P1IN                (PAIN_L)  /* Port 1 Input */
#define P1OUT               (PAOUT_L) /* Port 1 Output */
#define P1DIR               (PADIR_L) /* Port 1 Direction */
#define P1REN               (PAREN_L) /* Port 1 Resistor Enable */
#define P1DS                (PADS_L)  /* Port 1 Drive Strenght */
#define P1SEL0              (PASEL0_L) /* Port 1 Selection 0 */
#define P1SEL1              (PASEL1_L) /* Port 1 Selection 1 */
#define P1IES               (PAIES_L) /* Port 1 Interrupt Edge Select */
#define P1IE                (PAIE_L)  /* Port 1 Interrupt Enable */
#define P1IFG               (PAIFG_L) /* Port 1 Interrupt Flag */

//Definitions for P1IV
#define P1IV_NONE            (0x0000)    /* No Interrupt pending */
#define P1IV_P1IFG0          (0x0002)    /* P1IV P1IFG.0 */
#define P1IV_P1IFG1          (0x0004)    /* P1IV P1IFG.1 */
#define P1IV_P1IFG2          (0x0006)    /* P1IV P1IFG.2 */
#define P1IV_P1IFG3          (0x0008)    /* P1IV P1IFG.3 */
#define P1IV_P1IFG4          (0x000A)    /* P1IV P1IFG.4 */
#define P1IV_P1IFG5          (0x000C)    /* P1IV P1IFG.5 */
#define P1IV_P1IFG6          (0x000E)    /* P1IV P1IFG.6 */
#define P1IV_P1IFG7          (0x0010)    /* P1IV P1IFG.7 */


#define P2IN                (PAIN_H)  /* Port 2 Input */
#define P2OUT               (PAOUT_H) /* Port 2 Output */
#define P2DIR               (PADIR_H) /* Port 2 Direction */
#define P2REN               (PAREN_H) /* Port 2 Resistor Enable */
#define P2DS                (PADS_H)  /* Port 2 Drive Strenght */
#define P2SEL0              (PASEL0_H) /* Port 2 Selection 0 */
#define P2SEL1              (PASEL1_H) /* Port 2 Selection 1 */
#define P2IES               (PAIES_H) /* Port 2 Interrupt Edge Select */
#define P2IE                (PAIE_H)  /* Port 2 Interrupt Enable */
#define P2IFG               (PAIFG_H) /* Port 2 Interrupt Flag */

//Definitions for P2IV
#define P2IV_NONE            (0x0000)    /* No Interrupt pending */
#define P2IV_P2IFG0          (0x0002)    /* P2IV P2IFG.0 */
#define P2IV_P2IFG1          (0x0004)    /* P2IV P2IFG.1 */
#define P2IV_P2IFG2          (0x0006)    /* P2IV P2IFG.2 */
#define P2IV_P2IFG3          (0x0008)    /* P2IV P2IFG.3 */
#define P2IV_P2IFG4          (0x000A)    /* P2IV P2IFG.4 */
#define P2IV_P2IFG5          (0x000C)    /* P2IV P2IFG.5 */
#define P2IV_P2IFG6          (0x000E)    /* P2IV P2IFG.6 */
#define P2IV_P2IFG7          (0x0010)    /* P2IV P2IFG.7 */



/************************************************************
* DIGITAL I/O Port3/4 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT3_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT3_R__ 0x0220
#define P3_BASE __MSP430_BASEADDRESS_PORT3_R__
#define __MSP430_HAS_PORT4_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT4_R__ 0x0220
#define P4_BASE __MSP430_BASEADDRESS_PORT4_R__
#define __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTB_R__ 0x0220
#define PB_BASE __MSP430_BASEADDRESS_PORTB_R__
#define __MSP430_HAS_P3SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P4SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PBSEL0__         /* Define for DriverLib */

#define PBIN_                 0x0220    /* Port B Input */
const_sfrb(PBIN_L , PBIN_);
const_sfrb(PBIN_H , PBIN_+1);
const_sfrw(PBIN, PBIN_);
#define PBOUT_                0x0222    /* Port B Output */
sfrb(PBOUT_L , PBOUT_);
sfrb(PBOUT_H , PBOUT_+1);
sfrw(PBOUT, PBOUT_);
#define PBDIR_                0x0224    /* Port B Direction */
sfrb(PBDIR_L , PBDIR_);
sfrb(PBDIR_H , PBDIR_+1);
sfrw(PBDIR, PBDIR_);
#define PBREN_                0x0226    /* Port B Resistor Enable */
sfrb(PBREN_L , PBREN_);
sfrb(PBREN_H , PBREN_+1);
sfrw(PBREN, PBREN_);
#define PBDS_                 0x0228    /* Port B Drive Strenght */
sfrb(PBDS_L , PBDS_);
sfrb(PBDS_H , PBDS_+1);
sfrw(PBDS, PBDS_);
#define PBSEL0_               0x022A    /* Port B Selection 0 */
sfrb(PBSEL0_L , PBSEL0_);
sfrb(PBSEL0_H , PBSEL0_+1);
sfrw(PBSEL0, PBSEL0_);














#define P3IN                (PBIN_L)  /* Port 3 Input */
#define P3OUT               (PBOUT_L) /* Port 3 Output */
#define P3DIR               (PBDIR_L) /* Port 3 Direction */
#define P3REN               (PBREN_L) /* Port 3 Resistor Enable */
#define P3DS                (PBDS_L)  /* Port 3 Drive Strenght */
#define P3SEL0              (PBSEL0_L) /* Port 3 Selection 0 */


#define P4IN                (PBIN_H)  /* Port 4 Input */
#define P4OUT               (PBOUT_H) /* Port 4 Output */
#define P4DIR               (PBDIR_H) /* Port 4 Direction */
#define P4REN               (PBREN_H) /* Port 4 Resistor Enable */
#define P4DS                (PBDS_H)  /* Port 4 Drive Strenght */
#define P4SEL0              (PBSEL0_H) /* Port 4 Selection 0 */



/************************************************************
* DIGITAL I/O Port5/6 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT5_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT5_R__ 0x0240
#define P5_BASE __MSP430_BASEADDRESS_PORT5_R__
#define __MSP430_HAS_PORT6_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT6_R__ 0x0240
#define P6_BASE __MSP430_BASEADDRESS_PORT6_R__
#define __MSP430_HAS_PORTC_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTC_R__ 0x0240
#define PC_BASE __MSP430_BASEADDRESS_PORTC_R__
#define __MSP430_HAS_P5SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P6SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PCSEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P5SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_P6SEL1__         /* Define for DriverLib */
#define __MSP430_HAS_PCSEL1__         /* Define for DriverLib */

#define PCIN_                 0x0240    /* Port C Input */
const_sfrb(PCIN_L , PCIN_);
const_sfrb(PCIN_H , PCIN_+1);
const_sfrw(PCIN, PCIN_);
#define PCOUT_                0x0242    /* Port C Output */
sfrb(PCOUT_L , PCOUT_);
sfrb(PCOUT_H , PCOUT_+1);
sfrw(PCOUT, PCOUT_);
#define PCDIR_                0x0244    /* Port C Direction */
sfrb(PCDIR_L , PCDIR_);
sfrb(PCDIR_H , PCDIR_+1);
sfrw(PCDIR, PCDIR_);
#define PCREN_                0x0246    /* Port C Resistor Enable */
sfrb(PCREN_L , PCREN_);
sfrb(PCREN_H , PCREN_+1);
sfrw(PCREN, PCREN_);
#define PCDS_                 0x0248    /* Port C Drive Strenght */
sfrb(PCDS_L , PCDS_);
sfrb(PCDS_H , PCDS_+1);
sfrw(PCDS, PCDS_);
#define PCSEL0_               0x024A    /* Port C Selection 0 */
sfrb(PCSEL0_L , PCSEL0_);
sfrb(PCSEL0_H , PCSEL0_+1);
sfrw(PCSEL0, PCSEL0_);
#define PCSEL1_               0x024C    /* Port C Selection 1 */
sfrb(PCSEL1_L , PCSEL1_);
sfrb(PCSEL1_H , PCSEL1_+1);
sfrw(PCSEL1, PCSEL1_);
















#define P5IN                (PCIN_L)  /* Port 5 Input */
#define P5OUT               (PCOUT_L) /* Port 5 Output */
#define P5DIR               (PCDIR_L) /* Port 5 Direction */
#define P5REN               (PCREN_L) /* Port 5 Resistor Enable */
#define P5DS                (PCDS_L)  /* Port 5 Drive Strenght */
#define P5SEL0              (PCSEL0_L) /* Port 5 Selection 0 */
#define P5SEL1              (PCSEL1_L) /* Port 5 Selection 1 */


#define P6IN                (PCIN_H)  /* Port 6 Input */
#define P6OUT               (PCOUT_H) /* Port 6 Output */
#define P6DIR               (PCDIR_H) /* Port 6 Direction */
#define P6REN               (PCREN_H) /* Port 6 Resistor Enable */
#define P6DS                (PCDS_H)  /* Port 6 Drive Strenght */
#define P6SEL0              (PCSEL0_H) /* Port 6 Selection 0 */
#define P6SEL1              (PCSEL1_H) /* Port 6 Selection 1 */



/************************************************************
* DIGITAL I/O Port7/8 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT7_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT7_R__ 0x0260
#define P7_BASE __MSP430_BASEADDRESS_PORT7_R__
#define __MSP430_HAS_PORT8_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT8_R__ 0x0260
#define P8_BASE __MSP430_BASEADDRESS_PORT8_R__
#define __MSP430_HAS_PORTD_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTD_R__ 0x0260
#define PD_BASE __MSP430_BASEADDRESS_PORTD_R__
#define __MSP430_HAS_P7SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P8SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PDSEL0__         /* Define for DriverLib */

#define PDIN_                 0x0260    /* Port D Input */
const_sfrb(PDIN_L , PDIN_);
const_sfrb(PDIN_H , PDIN_+1);
const_sfrw(PDIN, PDIN_);
#define PDOUT_                0x0262    /* Port D Output */
sfrb(PDOUT_L , PDOUT_);
sfrb(PDOUT_H , PDOUT_+1);
sfrw(PDOUT, PDOUT_);
#define PDDIR_                0x0264    /* Port D Direction */
sfrb(PDDIR_L , PDDIR_);
sfrb(PDDIR_H , PDDIR_+1);
sfrw(PDDIR, PDDIR_);
#define PDREN_                0x0266    /* Port D Resistor Enable */
sfrb(PDREN_L , PDREN_);
sfrb(PDREN_H , PDREN_+1);
sfrw(PDREN, PDREN_);
#define PDDS_                 0x0268    /* Port D Drive Strenght */
sfrb(PDDS_L , PDDS_);
sfrb(PDDS_H , PDDS_+1);
sfrw(PDDS, PDDS_);
#define PDSEL0_               0x026A    /* Port D Selection 0 */
sfrb(PDSEL0_L , PDSEL0_);
sfrb(PDSEL0_H , PDSEL0_+1);
sfrw(PDSEL0, PDSEL0_);














#define P7IN                (PDIN_L)  /* Port 7 Input */
#define P7OUT               (PDOUT_L) /* Port 7 Output */
#define P7DIR               (PDDIR_L) /* Port 7 Direction */
#define P7REN               (PDREN_L) /* Port 7 Resistor Enable */
#define P7DS                (PDDS_L)  /* Port 7 Drive Strenght */
#define P7SEL0              (PDSEL0_L) /* Port 7 Selection 0 */


#define P8IN                (PDIN_H)  /* Port 8 Input */
#define P8OUT               (PDOUT_H) /* Port 8 Output */
#define P8DIR               (PDDIR_H) /* Port 8 Direction */
#define P8REN               (PDREN_H) /* Port 8 Resistor Enable */
#define P8DS                (PDDS_H)  /* Port 8 Drive Strenght */
#define P8SEL0              (PDSEL0_H) /* Port 8 Selection 0 */



/************************************************************
* DIGITAL I/O Port9/10 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT9_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT9_R__ 0x0280
#define P9_BASE __MSP430_BASEADDRESS_PORT9_R__
#define __MSP430_HAS_PORT10_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT10_R__ 0x0280
#define P10_BASE __MSP430_BASEADDRESS_PORT10_R__
#define __MSP430_HAS_PORTE_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTE_R__ 0x0280
#define PE_BASE __MSP430_BASEADDRESS_PORTE_R__
#define __MSP430_HAS_P9SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_P10SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PESEL0__         /* Define for DriverLib */

#define PEIN_                 0x0280    /* Port E Input */
const_sfrb(PEIN_L , PEIN_);
const_sfrb(PEIN_H , PEIN_+1);
const_sfrw(PEIN, PEIN_);
#define PEOUT_                0x0282    /* Port E Output */
sfrb(PEOUT_L , PEOUT_);
sfrb(PEOUT_H , PEOUT_+1);
sfrw(PEOUT, PEOUT_);
#define PEDIR_                0x0284    /* Port E Direction */
sfrb(PEDIR_L , PEDIR_);
sfrb(PEDIR_H , PEDIR_+1);
sfrw(PEDIR, PEDIR_);
#define PEREN_                0x0286    /* Port E Resistor Enable */
sfrb(PEREN_L , PEREN_);
sfrb(PEREN_H , PEREN_+1);
sfrw(PEREN, PEREN_);
#define PEDS_                 0x0288    /* Port E Drive Strenght */
sfrb(PEDS_L , PEDS_);
sfrb(PEDS_H , PEDS_+1);
sfrw(PEDS, PEDS_);
#define PESEL0_               0x028A    /* Port E Selection 0 */
sfrb(PESEL0_L , PESEL0_);
sfrb(PESEL0_H , PESEL0_+1);
sfrw(PESEL0, PESEL0_);














#define P9IN                (PEIN_L)  /* Port 9 Input */
#define P9OUT               (PEOUT_L) /* Port 9 Output */
#define P9DIR               (PEDIR_L) /* Port 9 Direction */
#define P9REN               (PEREN_L) /* Port 9 Resistor Enable */
#define P9DS                (PEDS_L)  /* Port 9 Drive Strenght */
#define P9SEL0              (PESEL0_L) /* Port 9 Selection 0 */


#define P10IN                (PEIN_H)  /* Port 10 Input */
#define P10OUT               (PEOUT_H) /* Port 10 Output */
#define P10DIR               (PEDIR_H) /* Port 10 Direction */
#define P10REN               (PEREN_H) /* Port 10 Resistor Enable */
#define P10DS                (PEDS_H)  /* Port 10 Drive Strenght */
#define P10SEL0              (PESEL0_H) /* Port 10 Selection 0 */



/************************************************************
* DIGITAL I/O Port11 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT11_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT11_R__ 0x02A0
#define P11_BASE __MSP430_BASEADDRESS_PORT11_R__
#define __MSP430_HAS_PORTF_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTF_R__ 0x02A0
#define PF_BASE __MSP430_BASEADDRESS_PORTF_R__
#define __MSP430_HAS_P11SEL0__         /* Define for DriverLib */
#define __MSP430_HAS_PFSEL0__         /* Define for DriverLib */

#define PFIN_                 0x02A0    /* Port F Input */
const_sfrb(PFIN_L , PFIN_);
const_sfrb(PFIN_H , PFIN_+1);
const_sfrw(PFIN, PFIN_);
#define PFOUT_                0x02A2    /* Port F Output */
sfrb(PFOUT_L , PFOUT_);
sfrb(PFOUT_H , PFOUT_+1);
sfrw(PFOUT, PFOUT_);
#define PFDIR_                0x02A4    /* Port F Direction */
sfrb(PFDIR_L , PFDIR_);
sfrb(PFDIR_H , PFDIR_+1);
sfrw(PFDIR, PFDIR_);
#define PFREN_                0x02A6    /* Port F Resistor Enable */
sfrb(PFREN_L , PFREN_);
sfrb(PFREN_H , PFREN_+1);
sfrw(PFREN, PFREN_);
#define PFDS_                 0x02A8    /* Port F Drive Strenght */
sfrb(PFDS_L , PFDS_);
sfrb(PFDS_H , PFDS_+1);
sfrw(PFDS, PFDS_);
#define PFSEL0_               0x02AA    /* Port F Selection 0 */
sfrb(PFSEL0_L , PFSEL0_);
sfrb(PFSEL0_H , PFSEL0_+1);
sfrw(PFSEL0, PFSEL0_);














#define P11IN                (PFIN_L)  /* Port 11 Input */
#define P11OUT               (PFOUT_L) /* Port 11 Output */
#define P11DIR               (PFDIR_L) /* Port 11 Direction */
#define P11REN               (PFREN_L) /* Port 11 Resistor Enable */
#define P11DS                (PFDS_L)  /* Port 11 Drive Strenght */
#define P11SEL0              (PFSEL0_L) /* Port 11 Selection0 */




/************************************************************
* DIGITAL I/O PortJ Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORTJ_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTJ_R__ 0x0320
#define PJ_BASE __MSP430_BASEADDRESS_PORTJ_R__
#define __MSP430_HAS_PJSEL__           /* Define for DriverLib */

#define PJIN_                 0x0320    /* Port J Input */
const_sfrb(PJIN_L , PJIN_);
const_sfrb(PJIN_H , PJIN_+1);
const_sfrw(PJIN, PJIN_);
#define PJOUT_                0x0322    /* Port J Output */
sfrb(PJOUT_L , PJOUT_);
sfrb(PJOUT_H , PJOUT_+1);
sfrw(PJOUT, PJOUT_);
#define PJDIR_                0x0324    /* Port J Direction */
sfrb(PJDIR_L , PJDIR_);
sfrb(PJDIR_H , PJDIR_+1);
sfrw(PJDIR, PJDIR_);
#define PJREN_                0x0326    /* Port J Resistor Enable */
sfrb(PJREN_L , PJREN_);
sfrb(PJREN_H , PJREN_+1);
sfrw(PJREN, PJREN_);
#define PJDS_                 0x0328    /* Port J Drive Strenght */
sfrb(PJDS_L , PJDS_);
sfrb(PJDS_H , PJDS_+1);
sfrw(PJDS, PJDS_);
#define PJSEL_                0x032A    /* Port J Selection */
sfrb(PJSEL_L , PJSEL_);
sfrb(PJSEL_H , PJSEL_+1);
sfrw(PJSEL, PJSEL_);













/************************************************************
* PORT MAPPING CONTROLLER
************************************************************/
#define __MSP430_HAS_PORT_MAPPING__   /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT_MAPPING__ 0x01C0
#define PMAP_CTRL_BASE __MSP430_BASEADDRESS_PORT_MAPPING__

#define PMAPKEYID_            0x01C0    /* Port Mapping Key register */
sfrb(PMAPKEYID_L , PMAPKEYID_);
sfrb(PMAPKEYID_H , PMAPKEYID_+1);
sfrw(PMAPKEYID, PMAPKEYID_);
#define PMAPCTL_              0x01C2    /* Port Mapping control register */
sfrb(PMAPCTL_L , PMAPCTL_);
sfrb(PMAPCTL_H , PMAPCTL_+1);
sfrw(PMAPCTL, PMAPCTL_);

#define  PMAPKEY             (0x2D52)  /* Port Mapping Key */
#define  PMAPPWD             PMAPKEYID /* Legacy Definition: Mapping Key register */
#define  PMAPPW              (0x2D52)  /* Legacy Definition: Port Mapping Password */

/* PMAPCTL Control Bits */
#define PMAPLOCKED          (0x0001)  /* Port Mapping Lock bit. Read only */
#define PMAPRECFG           (0x0002)  /* Port Mapping re-configuration control bit */

/* PMAPCTL Control Bits */
#define PMAPLOCKED_L        (0x0001)  /* Port Mapping Lock bit. Read only */
#define PMAPRECFG_L         (0x0002)  /* Port Mapping re-configuration control bit */


/************************************************************
* PORT 2 MAPPING CONTROLLER
************************************************************/
#define __MSP430_HAS_PORT2_MAPPING__   /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT2_MAPPING__ 0x01D0
#define P2MAP_BASE __MSP430_BASEADDRESS_PORT2_MAPPING__

#define P2MAP01_              0x01D0    /* Port P2.0/1 mapping register */
sfrb(P2MAP01_L , P2MAP01_);
sfrb(P2MAP01_H , P2MAP01_+1);
sfrw(P2MAP01, P2MAP01_);
#define P2MAP23_              0x01D2    /* Port P2.2/3 mapping register */
sfrb(P2MAP23_L , P2MAP23_);
sfrb(P2MAP23_H , P2MAP23_+1);
sfrw(P2MAP23, P2MAP23_);
#define P2MAP45_              0x01D4    /* Port P2.4/5 mapping register */
sfrb(P2MAP45_L , P2MAP45_);
sfrb(P2MAP45_H , P2MAP45_+1);
sfrw(P2MAP45, P2MAP45_);
#define P2MAP67_              0x01D6    /* Port P2.6/7 mapping register */
sfrb(P2MAP67_L , P2MAP67_);
sfrb(P2MAP67_H , P2MAP67_+1);
sfrw(P2MAP67, P2MAP67_);

#define  P2MAP0              P2MAP01_L /* Port P2.0 mapping register */
#define  P2MAP1              P2MAP01_H /* Port P2.1 mapping register */
#define  P2MAP2              P2MAP23_L /* Port P2.2 mapping register */
#define  P2MAP3              P2MAP23_H /* Port P2.3 mapping register */
#define  P2MAP4              P2MAP45_L /* Port P2.4 mapping register */
#define  P2MAP5              P2MAP45_H /* Port P2.5 mapping register */
#define  P2MAP6              P2MAP67_L /* Port P2.6 mapping register */
#define  P2MAP7              P2MAP67_H /* Port P2.7 mapping register */











/************************************************************
* PORT 3 MAPPING CONTROLLER
************************************************************/
#define __MSP430_HAS_PORT3_MAPPING__   /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT3_MAPPING__ 0x01D8
#define P3MAP_BASE __MSP430_BASEADDRESS_PORT3_MAPPING__

#define P3MAP01_              0x01D8    /* Port P3.0/1 mapping register */
sfrb(P3MAP01_L , P3MAP01_);
sfrb(P3MAP01_H , P3MAP01_+1);
sfrw(P3MAP01, P3MAP01_);
#define P3MAP23_              0x01DA    /* Port P3.2/3 mapping register */
sfrb(P3MAP23_L , P3MAP23_);
sfrb(P3MAP23_H , P3MAP23_+1);
sfrw(P3MAP23, P3MAP23_);
#define P3MAP45_              0x01DC    /* Port P3.4/5 mapping register */
sfrb(P3MAP45_L , P3MAP45_);
sfrb(P3MAP45_H , P3MAP45_+1);
sfrw(P3MAP45, P3MAP45_);
#define P3MAP67_              0x01DE    /* Port P3.6/7 mapping register */
sfrb(P3MAP67_L , P3MAP67_);
sfrb(P3MAP67_H , P3MAP67_+1);
sfrw(P3MAP67, P3MAP67_);

#define  P3MAP0              P3MAP01_L /* Port P3.0 mapping register */
#define  P3MAP1              P3MAP01_H /* Port P3.1 mapping register */
#define  P3MAP2              P3MAP23_L /* Port P3.2 mapping register */
#define  P3MAP3              P3MAP23_H /* Port P3.3 mapping register */
#define  P3MAP4              P3MAP45_L /* Port P3.4 mapping register */
#define  P3MAP5              P3MAP45_H /* Port P3.5 mapping register */
#define  P3MAP6              P3MAP67_L /* Port P3.6 mapping register */
#define  P3MAP7              P3MAP67_H /* Port P3.7 mapping register */











/************************************************************
* PORT 4 MAPPING CONTROLLER
************************************************************/
#define __MSP430_HAS_PORT4_MAPPING__   /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT4_MAPPING__ 0x01E0
#define P4MAP_BASE __MSP430_BASEADDRESS_PORT4_MAPPING__

#define P4MAP01_              0x01E0    /* Port P4.0/1 mapping register */
sfrb(P4MAP01_L , P4MAP01_);
sfrb(P4MAP01_H , P4MAP01_+1);
sfrw(P4MAP01, P4MAP01_);
#define P4MAP23_              0x01E2    /* Port P4.2/3 mapping register */
sfrb(P4MAP23_L , P4MAP23_);
sfrb(P4MAP23_H , P4MAP23_+1);
sfrw(P4MAP23, P4MAP23_);
#define P4MAP45_              0x01E4    /* Port P4.4/5 mapping register */
sfrb(P4MAP45_L , P4MAP45_);
sfrb(P4MAP45_H , P4MAP45_+1);
sfrw(P4MAP45, P4MAP45_);
#define P4MAP67_              0x01E6    /* Port P4.6/7 mapping register */
sfrb(P4MAP67_L , P4MAP67_);
sfrb(P4MAP67_H , P4MAP67_+1);
sfrw(P4MAP67, P4MAP67_);

#define  P4MAP0              P4MAP01_L /* Port P4.0 mapping register */
#define  P4MAP1              P4MAP01_H /* Port P4.1 mapping register */
#define  P4MAP2              P4MAP23_L /* Port P4.2 mapping register */
#define  P4MAP3              P4MAP23_H /* Port P4.3 mapping register */
#define  P4MAP4              P4MAP45_L /* Port P4.4 mapping register */
#define  P4MAP5              P4MAP45_H /* Port P4.5 mapping register */
#define  P4MAP6              P4MAP67_L /* Port P4.6 mapping register */
#define  P4MAP7              P4MAP67_H /* Port P4.7 mapping register */











#define PM_NONE       0
#define PM_UCA0RXD    1
#define PM_UCA0SOMI   1
#define PM_UCA0TXD    2
#define PM_UCA0SIMO   2
#define PM_UCA0CLK    3
#define PM_UCA0STE    4
#define PM_UCA1RXD    5
#define PM_UCA1SOMI   5
#define PM_UCA1TXD    6
#define PM_UCA1SIMO   6
#define PM_UCA1CLK    7
#define PM_UCA1STE    8
#define PM_UCA2RXD    9
#define PM_UCA2SOMI   9
#define PM_UCA2TXD    10
#define PM_UCA2SIMO   10
#define PM_UCA2CLK    11
#define PM_UCA2STE    12
#define PM_UCA3RXD    13
#define PM_UCA3SOMI   13
#define PM_UCA3TXD    14
#define PM_UCA3SIMO   14
#define PM_UCA3CLK    15
#define PM_UCA3STE    16
#define PM_UCB0SIMO   17
#define PM_UCB0SDA    17
#define PM_UCB0SOMI   18
#define PM_UCB0SCL    18
#define PM_UCB0CLK    19
#define PM_UCB0STE    20
#define PM_UCB1SIMO   21
#define PM_UCB1SDA    21
#define PM_UCB1SOMI   22
#define PM_UCB1SCL    22
#define PM_UCB1CLK    23
#define PM_UCB1STE    24
#define PM_TA0_0      25
#define PM_TA0_1      26
#define PM_TA0_2      27
#define PM_TA1_0      28
#define PM_TA2_0      29
#define PM_TA3_0      30
#define PM_ANALOG     31

/************************************************************
* PMM - Power Management System
************************************************************/
#define __MSP430_HAS_PMM__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PMM__ 0x0120
#define PMM_BASE __MSP430_BASEADDRESS_PMM__

#define PMMCTL0_              0x0120    /* PMM Control 0 */
sfrb(PMMCTL0_L , PMMCTL0_);
sfrb(PMMCTL0_H , PMMCTL0_+1);
sfrw(PMMCTL0, PMMCTL0_);
#define PMMCTL1_              0x0122    /* PMM Control 1 */
sfrb(PMMCTL1_L , PMMCTL1_);
sfrb(PMMCTL1_H , PMMCTL1_+1);
sfrw(PMMCTL1, PMMCTL1_);
#define SVSMHCTL_             0x0124    /* SVS and SVM high side control register */
sfrb(SVSMHCTL_L , SVSMHCTL_);
sfrb(SVSMHCTL_H , SVSMHCTL_+1);
sfrw(SVSMHCTL, SVSMHCTL_);
#define SVSMLCTL_             0x0126    /* SVS and SVM low side control register */
sfrb(SVSMLCTL_L , SVSMLCTL_);
sfrb(SVSMLCTL_H , SVSMLCTL_+1);
sfrw(SVSMLCTL, SVSMLCTL_);
#define SVSMIO_               0x0128    /* SVSIN and SVSOUT control register */
sfrb(SVSMIO_L , SVSMIO_);
sfrb(SVSMIO_H , SVSMIO_+1);
sfrw(SVSMIO, SVSMIO_);
#define PMMIFG_               0x012C    /* PMM Interrupt Flag */
sfrb(PMMIFG_L , PMMIFG_);
sfrb(PMMIFG_H , PMMIFG_+1);
sfrw(PMMIFG, PMMIFG_);
#define PMMRIE_               0x012E    /* PMM and RESET Interrupt Enable */
sfrb(PMMRIE_L , PMMRIE_);
sfrb(PMMRIE_H , PMMRIE_+1);
sfrw(PMMRIE, PMMRIE_);
#define PM5CTL0_              0x0130    /* PMM Power Mode 5 Control Register 0 */
sfrb(PM5CTL0_L , PM5CTL0_);
sfrb(PM5CTL0_H , PM5CTL0_+1);
sfrw(PM5CTL0, PM5CTL0_);

#define PMMPW               (0xA500)  /* PMM Register Write Password */
#define PMMPW_H             (0xA5)    /* PMM Register Write Password for high word access */

/* PMMCTL0 Control Bits */
#define PMMCOREV0           (0x0001)  /* PMM Core Voltage Bit: 0 */
#define PMMCOREV1           (0x0002)  /* PMM Core Voltage Bit: 1 */
#define PMMSWBOR            (0x0004)  /* PMM Software BOR */
#define PMMSWPOR            (0x0008)  /* PMM Software POR */
#define PMMREGOFF           (0x0010)  /* PMM Turn Regulator off */
#define PMMHPMRE            (0x0080)  /* PMM Global High Power Module Request Enable */

/* PMMCTL0 Control Bits */
#define PMMCOREV0_L         (0x0001)  /* PMM Core Voltage Bit: 0 */
#define PMMCOREV1_L         (0x0002)  /* PMM Core Voltage Bit: 1 */
#define PMMSWBOR_L          (0x0004)  /* PMM Software BOR */
#define PMMSWPOR_L          (0x0008)  /* PMM Software POR */
#define PMMREGOFF_L         (0x0010)  /* PMM Turn Regulator off */
#define PMMHPMRE_L          (0x0080)  /* PMM Global High Power Module Request Enable */


#define PMMCOREV_0          (0x0000)  /* PMM Core Voltage 0 (1.35V) */
#define PMMCOREV_1          (0x0001)  /* PMM Core Voltage 1 (1.55V) */
#define PMMCOREV_2          (0x0002)  /* PMM Core Voltage 2 (1.75V) */
#define PMMCOREV_3          (0x0003)  /* PMM Core Voltage 3 (1.85V) */


/* PMMCTL1 Control Bits */
#define PMMREFMD            (0x0001)  /* PMM Reference Mode */
#define PMMCMD0             (0x0010)  /* PMM Voltage Regulator Current Mode Bit: 0 */
#define PMMCMD1             (0x0020)  /* PMM Voltage Regulator Current Mode Bit: 1 */

/* PMMCTL1 Control Bits */
#define PMMREFMD_L          (0x0001)  /* PMM Reference Mode */
#define PMMCMD0_L           (0x0010)  /* PMM Voltage Regulator Current Mode Bit: 0 */
#define PMMCMD1_L           (0x0020)  /* PMM Voltage Regulator Current Mode Bit: 1 */


/* SVSMHCTL Control Bits */
#define SVSMHRRL0           (0x0001)  /* SVS and SVM high side Reset Release Voltage Level Bit: 0 */
#define SVSMHRRL1           (0x0002)  /* SVS and SVM high side Reset Release Voltage Level Bit: 1 */
#define SVSMHRRL2           (0x0004)  /* SVS and SVM high side Reset Release Voltage Level Bit: 2 */
#define SVSMHDLYST          (0x0008)  /* SVS and SVM high side delay status */
#define SVSHMD              (0x0010)  /* SVS high side mode */
#define SVSMHEVM            (0x0040)  /* SVS and SVM high side event mask */
#define SVSMHACE            (0x0080)  /* SVS and SVM high side auto control enable */
#define SVSHRVL0            (0x0100)  /* SVS high side reset voltage level Bit: 0 */
#define SVSHRVL1            (0x0200)  /* SVS high side reset voltage level Bit: 1 */
#define SVSHE               (0x0400)  /* SVS high side enable */
#define SVSHFP              (0x0800)  /* SVS high side full performace mode */
#define SVMHOVPE            (0x1000)  /* SVM high side over-voltage enable */
#define SVMHE               (0x4000)  /* SVM high side enable */
#define SVMHFP              (0x8000)  /* SVM high side full performace mode */

/* SVSMHCTL Control Bits */
#define SVSMHRRL0_L         (0x0001)  /* SVS and SVM high side Reset Release Voltage Level Bit: 0 */
#define SVSMHRRL1_L         (0x0002)  /* SVS and SVM high side Reset Release Voltage Level Bit: 1 */
#define SVSMHRRL2_L         (0x0004)  /* SVS and SVM high side Reset Release Voltage Level Bit: 2 */
#define SVSMHDLYST_L        (0x0008)  /* SVS and SVM high side delay status */
#define SVSHMD_L            (0x0010)  /* SVS high side mode */
#define SVSMHEVM_L          (0x0040)  /* SVS and SVM high side event mask */
#define SVSMHACE_L          (0x0080)  /* SVS and SVM high side auto control enable */

/* SVSMHCTL Control Bits */
#define SVSHRVL0_H          (0x0001)  /* SVS high side reset voltage level Bit: 0 */
#define SVSHRVL1_H          (0x0002)  /* SVS high side reset voltage level Bit: 1 */
#define SVSHE_H             (0x0004)  /* SVS high side enable */
#define SVSHFP_H            (0x0008)  /* SVS high side full performace mode */
#define SVMHOVPE_H          (0x0010)  /* SVM high side over-voltage enable */
#define SVMHE_H             (0x0040)  /* SVM high side enable */
#define SVMHFP_H            (0x0080)  /* SVM high side full performace mode */

#define SVSMHRRL_0          (0x0000)  /* SVS and SVM high side Reset Release Voltage Level 0 */
#define SVSMHRRL_1          (0x0001)  /* SVS and SVM high side Reset Release Voltage Level 1 */
#define SVSMHRRL_2          (0x0002)  /* SVS and SVM high side Reset Release Voltage Level 2 */
#define SVSMHRRL_3          (0x0003)  /* SVS and SVM high side Reset Release Voltage Level 3 */
#define SVSMHRRL_4          (0x0004)  /* SVS and SVM high side Reset Release Voltage Level 4 */
#define SVSMHRRL_5          (0x0005)  /* SVS and SVM high side Reset Release Voltage Level 5 */
#define SVSMHRRL_6          (0x0006)  /* SVS and SVM high side Reset Release Voltage Level 6 */
#define SVSMHRRL_7          (0x0007)  /* SVS and SVM high side Reset Release Voltage Level 7 */

#define SVSHRVL_0           (0x0000)  /* SVS high side Reset Release Voltage Level 0 */
#define SVSHRVL_1           (0x0100)  /* SVS high side Reset Release Voltage Level 1 */
#define SVSHRVL_2           (0x0200)  /* SVS high side Reset Release Voltage Level 2 */
#define SVSHRVL_3           (0x0300)  /* SVS high side Reset Release Voltage Level 3 */


/* SVSMLCTL Control Bits */
#define SVSMLRRL0           (0x0001)  /* SVS and SVM low side Reset Release Voltage Level Bit: 0 */
#define SVSMLRRL1           (0x0002)  /* SVS and SVM low side Reset Release Voltage Level Bit: 1 */
#define SVSMLRRL2           (0x0004)  /* SVS and SVM low side Reset Release Voltage Level Bit: 2 */
#define SVSMLDLYST          (0x0008)  /* SVS and SVM low side delay status */
#define SVSLMD              (0x0010)  /* SVS low side mode */
#define SVSMLEVM            (0x0040)  /* SVS and SVM low side event mask */
#define SVSMLACE            (0x0080)  /* SVS and SVM low side auto control enable */
#define SVSLRVL0            (0x0100)  /* SVS low side reset voltage level Bit: 0 */
#define SVSLRVL1            (0x0200)  /* SVS low side reset voltage level Bit: 1 */
#define SVSLE               (0x0400)  /* SVS low side enable */
#define SVSLFP              (0x0800)  /* SVS low side full performace mode */
#define SVMLOVPE            (0x1000)  /* SVM low side over-voltage enable */
#define SVMLE               (0x4000)  /* SVM low side enable */
#define SVMLFP              (0x8000)  /* SVM low side full performace mode */

/* SVSMLCTL Control Bits */
#define SVSMLRRL0_L         (0x0001)  /* SVS and SVM low side Reset Release Voltage Level Bit: 0 */
#define SVSMLRRL1_L         (0x0002)  /* SVS and SVM low side Reset Release Voltage Level Bit: 1 */
#define SVSMLRRL2_L         (0x0004)  /* SVS and SVM low side Reset Release Voltage Level Bit: 2 */
#define SVSMLDLYST_L        (0x0008)  /* SVS and SVM low side delay status */
#define SVSLMD_L            (0x0010)  /* SVS low side mode */
#define SVSMLEVM_L          (0x0040)  /* SVS and SVM low side event mask */
#define SVSMLACE_L          (0x0080)  /* SVS and SVM low side auto control enable */

/* SVSMLCTL Control Bits */
#define SVSLRVL0_H          (0x0001)  /* SVS low side reset voltage level Bit: 0 */
#define SVSLRVL1_H          (0x0002)  /* SVS low side reset voltage level Bit: 1 */
#define SVSLE_H             (0x0004)  /* SVS low side enable */
#define SVSLFP_H            (0x0008)  /* SVS low side full performace mode */
#define SVMLOVPE_H          (0x0010)  /* SVM low side over-voltage enable */
#define SVMLE_H             (0x0040)  /* SVM low side enable */
#define SVMLFP_H            (0x0080)  /* SVM low side full performace mode */

#define SVSMLRRL_0          (0x0000)  /* SVS and SVM low side Reset Release Voltage Level 0 */
#define SVSMLRRL_1          (0x0001)  /* SVS and SVM low side Reset Release Voltage Level 1 */
#define SVSMLRRL_2          (0x0002)  /* SVS and SVM low side Reset Release Voltage Level 2 */
#define SVSMLRRL_3          (0x0003)  /* SVS and SVM low side Reset Release Voltage Level 3 */
#define SVSMLRRL_4          (0x0004)  /* SVS and SVM low side Reset Release Voltage Level 4 */
#define SVSMLRRL_5          (0x0005)  /* SVS and SVM low side Reset Release Voltage Level 5 */
#define SVSMLRRL_6          (0x0006)  /* SVS and SVM low side Reset Release Voltage Level 6 */
#define SVSMLRRL_7          (0x0007)  /* SVS and SVM low side Reset Release Voltage Level 7 */

#define SVSLRVL_0           (0x0000)  /* SVS low side Reset Release Voltage Level 0 */
#define SVSLRVL_1           (0x0100)  /* SVS low side Reset Release Voltage Level 1 */
#define SVSLRVL_2           (0x0200)  /* SVS low side Reset Release Voltage Level 2 */
#define SVSLRVL_3           (0x0300)  /* SVS low side Reset Release Voltage Level 3 */


/* SVSMIO Control Bits */
#define SVMLOE              (0x0008)  /* SVM low side output enable */
#define SVMLVLROE           (0x0010)  /* SVM low side voltage level reached output enable */
#define SVMOUTPOL           (0x0020)  /* SVMOUT pin polarity */
#define SVMHOE              (0x0800)  /* SVM high side output enable */
#define SVMHVLROE           (0x1000)  /* SVM high side voltage level reached output enable */

/* SVSMIO Control Bits */
#define SVMLOE_L            (0x0008)  /* SVM low side output enable */
#define SVMLVLROE_L         (0x0010)  /* SVM low side voltage level reached output enable */
#define SVMOUTPOL_L         (0x0020)  /* SVMOUT pin polarity */

/* SVSMIO Control Bits */
#define SVMHOE_H            (0x0008)  /* SVM high side output enable */
#define SVMHVLROE_H         (0x0010)  /* SVM high side voltage level reached output enable */


/* PMMIFG Control Bits */
#define SVSMLDLYIFG         (0x0001)  /* SVS and SVM low side Delay expired interrupt flag */
#define SVMLIFG             (0x0002)  /* SVM low side interrupt flag */
#define SVMLVLRIFG          (0x0004)  /* SVM low side Voltage Level Reached interrupt flag */
#define SVSMHDLYIFG         (0x0010)  /* SVS and SVM high side Delay expired interrupt flag */
#define SVMHIFG             (0x0020)  /* SVM high side interrupt flag */
#define SVMHVLRIFG          (0x0040)  /* SVM high side Voltage Level Reached interrupt flag */
#define PMMBORIFG           (0x0100)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG           (0x0200)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG           (0x0400)  /* PMM Software POR interrupt flag */
#define SVSHIFG             (0x1000)  /* SVS low side interrupt flag */
#define SVSLIFG             (0x2000)  /* SVS high side interrupt flag */
#define PMMLPM5IFG          (0x8000)  /* LPM5 indication Flag */

/* PMMIFG Control Bits */
#define SVSMLDLYIFG_L       (0x0001)  /* SVS and SVM low side Delay expired interrupt flag */
#define SVMLIFG_L           (0x0002)  /* SVM low side interrupt flag */
#define SVMLVLRIFG_L        (0x0004)  /* SVM low side Voltage Level Reached interrupt flag */
#define SVSMHDLYIFG_L       (0x0010)  /* SVS and SVM high side Delay expired interrupt flag */
#define SVMHIFG_L           (0x0020)  /* SVM high side interrupt flag */
#define SVMHVLRIFG_L        (0x0040)  /* SVM high side Voltage Level Reached interrupt flag */

/* PMMIFG Control Bits */
#define PMMBORIFG_H         (0x0001)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG_H         (0x0002)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG_H         (0x0004)  /* PMM Software POR interrupt flag */
#define SVSHIFG_H           (0x0010)  /* SVS low side interrupt flag */
#define SVSLIFG_H           (0x0020)  /* SVS high side interrupt flag */
#define PMMLPM5IFG_H        (0x0080)  /* LPM5 indication Flag */

#define PMMRSTLPM5IFG       PMMLPM5IFG /* LPM5 indication Flag */


/* PMMIE and RESET Control Bits */
#define SVSMLDLYIE          (0x0001)  /* SVS and SVM low side Delay expired interrupt enable */
#define SVMLIE              (0x0002)  /* SVM low side interrupt enable */
#define SVMLVLRIE           (0x0004)  /* SVM low side Voltage Level Reached interrupt enable */
#define SVSMHDLYIE          (0x0010)  /* SVS and SVM high side Delay expired interrupt enable */
#define SVMHIE              (0x0020)  /* SVM high side interrupt enable */
#define SVMHVLRIE           (0x0040)  /* SVM high side Voltage Level Reached interrupt enable */
#define SVSLPE              (0x0100)  /* SVS low side POR enable */
#define SVMLVLRPE           (0x0200)  /* SVM low side Voltage Level reached POR enable */
#define SVSHPE              (0x1000)  /* SVS high side POR enable */
#define SVMHVLRPE           (0x2000)  /* SVM high side Voltage Level reached POR enable */

/* PMMIE and RESET Control Bits */
#define SVSMLDLYIE_L        (0x0001)  /* SVS and SVM low side Delay expired interrupt enable */
#define SVMLIE_L            (0x0002)  /* SVM low side interrupt enable */
#define SVMLVLRIE_L         (0x0004)  /* SVM low side Voltage Level Reached interrupt enable */
#define SVSMHDLYIE_L        (0x0010)  /* SVS and SVM high side Delay expired interrupt enable */
#define SVMHIE_L            (0x0020)  /* SVM high side interrupt enable */
#define SVMHVLRIE_L         (0x0040)  /* SVM high side Voltage Level Reached interrupt enable */

/* PMMIE and RESET Control Bits */
#define SVSLPE_H            (0x0001)  /* SVS low side POR enable */
#define SVMLVLRPE_H         (0x0002)  /* SVM low side Voltage Level reached POR enable */
#define SVSHPE_H            (0x0010)  /* SVS high side POR enable */
#define SVMHVLRPE_H         (0x0020)  /* SVM high side Voltage Level reached POR enable */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5            (0x0001)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5_L          (0x0001)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */

#define LOCKIO              LOCKLPM5  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */

/*************************************************************
* RAM Control Module
*************************************************************/
#define __MSP430_HAS_RC__             /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_RC__ 0x0158
#define RAM_BASE __MSP430_BASEADDRESS_RC__

#define RCCTL0_               0x0158    /* Ram Controller Control Register */
sfrb(RCCTL0_L , RCCTL0_);
sfrb(RCCTL0_H , RCCTL0_+1);
sfrw(RCCTL0, RCCTL0_);

/* RCCTL0 Control Bits */
#define RCRS0OFF            (0x0001)  /* RAM Controller RAM Sector 0 Off */
#define RCRS1OFF            (0x0002)  /* RAM Controller RAM Sector 1 Off */
#define RCRS2OFF            (0x0004)  /* RAM Controller RAM Sector 2 Off */
#define RCRS3OFF            (0x0008)  /* RAM Controller RAM Sector 3 Off */

/* RCCTL0 Control Bits */
#define RCRS0OFF_L          (0x0001)  /* RAM Controller RAM Sector 0 Off */
#define RCRS1OFF_L          (0x0002)  /* RAM Controller RAM Sector 1 Off */
#define RCRS2OFF_L          (0x0004)  /* RAM Controller RAM Sector 2 Off */
#define RCRS3OFF_L          (0x0008)  /* RAM Controller RAM Sector 3 Off */


#define RCKEY               (0x5A00)

/************************************************************
* Shared Reference
************************************************************/
#define __MSP430_HAS_REF__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_REF__ 0x01B0
#define REF_BASE __MSP430_BASEADDRESS_REF__

#define REFCTL0_              0x01B0    /* REF Shared Reference control register 0 */
sfrb(REFCTL0_L , REFCTL0_);
sfrb(REFCTL0_H , REFCTL0_+1);
sfrw(REFCTL0, REFCTL0_);

/* REFCTL0 Control Bits */
#define REFON               (0x0001)  /* REF Reference On */
#define REFOUT              (0x0002)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFTCOFF            (0x0008)  /* REF Temp.Sensor off */
#define REFVSEL0            (0x0010)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1            (0x0020)  /* REF Reference Voltage Level Select Bit:1 */
//#define RESERVED            (0x0040)  /* Reserved */
#define REFMSTR             (0x0080)  /* REF Master Control */
#define REFGENACT           (0x0100)  /* REF Reference generator active */
#define REFBGACT            (0x0200)  /* REF Reference bandgap active */
#define REFGENBUSY          (0x0400)  /* REF Reference generator busy */
#define BGMODE              (0x0800)  /* REF Bandgap mode */
//#define RESERVED            (0x1000)  /* Reserved */
//#define RESERVED            (0x2000)  /* Reserved */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

/* REFCTL0 Control Bits */
#define REFON_L             (0x0001)  /* REF Reference On */
#define REFOUT_L            (0x0002)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFTCOFF_L          (0x0008)  /* REF Temp.Sensor off */
#define REFVSEL0_L          (0x0010)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1_L          (0x0020)  /* REF Reference Voltage Level Select Bit:1 */
//#define RESERVED            (0x0040)  /* Reserved */
#define REFMSTR_L           (0x0080)  /* REF Master Control */
//#define RESERVED            (0x1000)  /* Reserved */
//#define RESERVED            (0x2000)  /* Reserved */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

/* REFCTL0 Control Bits */
//#define RESERVED            (0x0004)  /* Reserved */
//#define RESERVED            (0x0040)  /* Reserved */
#define REFGENACT_H         (0x0001)  /* REF Reference generator active */
#define REFBGACT_H          (0x0002)  /* REF Reference bandgap active */
#define REFGENBUSY_H        (0x0004)  /* REF Reference generator busy */
#define BGMODE_H            (0x0008)  /* REF Bandgap mode */
//#define RESERVED            (0x1000)  /* Reserved */
//#define RESERVED            (0x2000)  /* Reserved */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

#define REFVSEL_0           (0x0000)  /* REF Reference Voltage Level Select 1.5V */
#define REFVSEL_1           (0x0010)  /* REF Reference Voltage Level Select 2.0V */
#define REFVSEL_2           (0x0020)  /* REF Reference Voltage Level Select 2.5V */
#define REFVSEL_3           (0x0030)  /* REF Reference Voltage Level Select 2.5V */

/************************************************************
* Real Time Clock
************************************************************/
#define __MSP430_HAS_RTC_CE__         /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_RTC_CE__ 0x0C80
#define RTC_CE_BASE __MSP430_BASEADDRESS_RTC_CE__

#define RTCCTL0_              0x0C80    /* Real Timer Clock Control 0/Key */
sfrb(RTCCTL0_L , RTCCTL0_);
sfrb(RTCCTL0_H , RTCCTL0_+1);
sfrw(RTCCTL0, RTCCTL0_);
#define RTCCTL13_             0x0C82    /* Real Timer Clock Control 1/3 */
sfrb(RTCCTL13_L , RTCCTL13_);
sfrb(RTCCTL13_H , RTCCTL13_+1);
sfrw(RTCCTL13, RTCCTL13_);
#define RTCCTL1             RTCCTL13_L
#define RTCCTL3             RTCCTL13_H
#define RTCOCAL_              0x0C84    /* Real Timer Clock Offset Calibartion */
sfrb(RTCOCAL_L , RTCOCAL_);
sfrb(RTCOCAL_H , RTCOCAL_+1);
sfrw(RTCOCAL, RTCOCAL_);
#define RTCTCMP_              0x0C86    /* Real Timer Temperature Compensation */
sfrb(RTCTCMP_L , RTCTCMP_);
sfrb(RTCTCMP_H , RTCTCMP_+1);
sfrw(RTCTCMP, RTCTCMP_);
#define RTCPS0CTL_            0x0C88    /* Real Timer Prescale Timer 0 Control */
sfrb(RTCPS0CTL_L , RTCPS0CTL_);
sfrb(RTCPS0CTL_H , RTCPS0CTL_+1);
sfrw(RTCPS0CTL, RTCPS0CTL_);
#define RTCPS1CTL_            0x0C8A    /* Real Timer Prescale Timer 1 Control */
sfrb(RTCPS1CTL_L , RTCPS1CTL_);
sfrb(RTCPS1CTL_H , RTCPS1CTL_+1);
sfrw(RTCPS1CTL, RTCPS1CTL_);
#define RTCPS_                0x0C8C    /* Real Timer Prescale Timer Control */
sfrb(RTCPS_L , RTCPS_);
sfrb(RTCPS_H , RTCPS_+1);
sfrw(RTCPS, RTCPS_);
#define RTCIV_                0x0C8E    /* Real Time Clock Interrupt Vector */
sfrw(RTCIV, RTCIV_);
#define RTCTIM0_              0x0C90    /* Real Time Clock Time 0 */
sfrb(RTCTIM0_L , RTCTIM0_);
sfrb(RTCTIM0_H , RTCTIM0_+1);
sfrw(RTCTIM0, RTCTIM0_);
#define RTCTIM1_              0x0C92    /* Real Time Clock Time 1 */
sfrb(RTCTIM1_L , RTCTIM1_);
sfrb(RTCTIM1_H , RTCTIM1_+1);
sfrw(RTCTIM1, RTCTIM1_);
#define RTCDATE_              0x0C94    /* Real Time Clock Date */
sfrb(RTCDATE_L , RTCDATE_);
sfrb(RTCDATE_H , RTCDATE_+1);
sfrw(RTCDATE, RTCDATE_);
#define RTCYEAR_              0x0C96    /* Real Time Clock Year */
sfrb(RTCYEAR_L , RTCYEAR_);
sfrb(RTCYEAR_H , RTCYEAR_+1);
sfrw(RTCYEAR, RTCYEAR_);
#define RTCAMINHR_            0x0C98    /* Real Time Clock Alarm Min/Hour */
sfrb(RTCAMINHR_L , RTCAMINHR_);
sfrb(RTCAMINHR_H , RTCAMINHR_+1);
sfrw(RTCAMINHR, RTCAMINHR_);
#define RTCADOWDAY_           0x0C9A    /* Real Time Clock Alarm day of week/day */
sfrb(RTCADOWDAY_L , RTCADOWDAY_);
sfrb(RTCADOWDAY_H , RTCADOWDAY_+1);
sfrw(RTCADOWDAY, RTCADOWDAY_);
#define BIN2BCD_              0x0C9C    /* Real Time Binary-to-BCD conversion register */
sfrw(BIN2BCD, BIN2BCD_);
#define BCD2BIN_              0x0C9E    /* Real Time BCD-to-binary conversion register */
sfrw(BCD2BIN, BCD2BIN_);

#define RTCTCCTL0_            0x0CA0    /*  Real-Time Clock Time Capture Control Register 0   */
sfrb(RTCTCCTL0, RTCTCCTL0_);
#define RTCTCCTL1_            0x0CA1    /*  Real-Time Clock Time Capture Control Register 1   */
sfrb(RTCTCCTL1, RTCTCCTL1_);
#define RTCCAP0CTL_           0x0CA2    /*  Tamper Detect Pin 0 Control Register   */
sfrb(RTCCAP0CTL, RTCCAP0CTL_);
#define RTCCAP1CTL_           0x0CA3    /*  Tamper Detect Pin 1 Control Register   */
sfrb(RTCCAP1CTL, RTCCAP1CTL_);
#define RTCSECBAK0_           0x0CB0    /*  Real-Time Clock Seconds Backup Register 0   */
sfrb(RTCSECBAK0, RTCSECBAK0_);
#define RTCMINBAK0_           0x0CB1    /*  Real-Time Clock Minutes Backup Register 0   */
sfrb(RTCMINBAK0, RTCMINBAK0_);
#define RTCHOURBAK0_          0x0CB2    /*  Real-Time Clock Hours Backup Register 0   */
sfrb(RTCHOURBAK0, RTCHOURBAK0_);
#define RTCDAYBAK0_           0x0CB3    /*  Real-Time Clock Days Backup Register 0   */
sfrb(RTCDAYBAK0, RTCDAYBAK0_);
#define RTCMONBAK0_           0x0CB4    /*  Real-Time Clock Months Backup Register 0   */
sfrb(RTCMONBAK0, RTCMONBAK0_);
#define RTCYEARBAK0_          0x0CB6    /*  Real-Time Clock year Backup Register 0   */
sfrb(RTCYEARBAK0, RTCYEARBAK0_);
#define RTCSECBAK1_           0x0CB8    /*  Real-Time Clock Seconds Backup Register 1   */
sfrb(RTCSECBAK1, RTCSECBAK1_);
#define RTCMINBAK1_           0x0CB9    /*  Real-Time Clock Minutes Backup Register 1   */
sfrb(RTCMINBAK1, RTCMINBAK1_);
#define RTCHOURBAK1_          0x0CBA    /*  Real-Time Clock Hours Backup Register 1   */
sfrb(RTCHOURBAK1, RTCHOURBAK1_);
#define RTCDAYBAK1_           0x0CBB    /*  Real-Time Clock Days Backup Register 1   */
sfrb(RTCDAYBAK1, RTCDAYBAK1_);
#define RTCMONBAK1_           0x0CBC    /*  Real-Time Clock Months Backup Register 1   */
sfrb(RTCMONBAK1, RTCMONBAK1_);
#define RTCYEARBAK1_          0x0CBE    /*  Real-Time Clock Year Backup Register 1   */
sfrb(RTCYEARBAK1, RTCYEARBAK1_);

#define RTCSEC              RTCTIM0_L
#define RTCMIN              RTCTIM0_H
#define RTCHOUR             RTCTIM1_L
#define RTCDOW              RTCTIM1_H
#define RTCDAY              RTCDATE_L
#define RTCMON              RTCDATE_H
#define RTCYEARL            RTCYEAR_L
#define RT0PS               RTCPS_L
#define RT1PS               RTCPS_H
#define RTCAMIN             RTCAMINHR_L    /* Real Time Clock Alarm Min */
#define RTCAHOUR            RTCAMINHR_H    /* Real Time Clock Alarm Hour */
#define RTCADOW             RTCADOWDAY_L   /* Real Time Clock Alarm day of week */
#define RTCADAY             RTCADOWDAY_H   /* Real Time Clock Alarm day */

/* RTCCTL0 Control Bits */
#define RTCOFIE             (0x0080)   /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE            (0x0040)   /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE              (0x0020)   /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE            (0x0010)   /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG            (0x0008)   /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG           (0x0004)   /* RTC Time Event Interrupt Flag */
#define RTCAIFG             (0x0002)   /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG           (0x0001)   /* RTC Ready Interrupt Flag */

/* RTCCTL0 Control Bits */
#define RTCOFIE_L           (0x0080)   /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE_L          (0x0040)   /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE_L            (0x0020)   /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE_L          (0x0010)   /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG_L          (0x0008)   /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG_L         (0x0004)   /* RTC Time Event Interrupt Flag */
#define RTCAIFG_L           (0x0002)   /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG_L         (0x0001)   /* RTC Ready Interrupt Flag */


#define RTCKEY              (0xA500)   /* RTC Key for RTC write access */
#define RTCKEY_H            (0xA5)     /* RTC Key for RTC write access (high word) */


/* RTCCTL13 Control Bits */
#define RTCCALF1            (0x0200)   /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0            (0x0100)   /* RTC Calibration Frequency Bit 0 */
#define RTCBCD              (0x0080)   /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD             (0x0040)   /* RTC Hold */
#define RTCMODE             (0x0020)   /* RTC Mode 0:Counter / 1: Calendar */
#define RTCRDY              (0x0010)   /* RTC Ready */
#define RTCSSEL1            (0x0008)   /* RTC Source Select 1 */
#define RTCSSEL0            (0x0004)   /* RTC Source Select 0 */
#define RTCTEV1             (0x0002)   /* RTC Time Event 1 */
#define RTCTEV0             (0x0001)   /* RTC Time Event 0 */

/* RTCCTL13 Control Bits */
#define RTCBCD_L            (0x0080)   /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD_L           (0x0040)   /* RTC Hold */
#define RTCMODE_L           (0x0020)   /* RTC Mode 0:Counter / 1: Calendar */
#define RTCRDY_L            (0x0010)   /* RTC Ready */
#define RTCSSEL1_L          (0x0008)   /* RTC Source Select 1 */
#define RTCSSEL0_L          (0x0004)   /* RTC Source Select 0 */
#define RTCTEV1_L           (0x0002)   /* RTC Time Event 1 */
#define RTCTEV0_L           (0x0001)   /* RTC Time Event 0 */

/* RTCCTL13 Control Bits */
#define RTCCALF1_H          (0x0002)   /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0_H          (0x0001)   /* RTC Calibration Frequency Bit 0 */

#define RTCSSEL_0           (0x0000)   /* RTC Source Select ACLK */
#define RTCSSEL_1           (0x0004)   /* RTC Source Select SMCLK */
#define RTCSSEL_2           (0x0008)   /* RTC Source Select RT1PS */
#define RTCSSEL_3           (0x000C)   /* RTC Source Select RT1PS */
#define RTCSSEL__ACLK       (0x0000)   /* RTC Source Select ACLK */
#define RTCSSEL__SMCLK      (0x0004)   /* RTC Source Select SMCLK */
#define RTCSSEL__RT1PS      (0x0008)   /* RTC Source Select RT1PS */

#define RTCTEV_0            (0x0000)   /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV_1            (0x0001)   /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV_2            (0x0002)   /* RTC Time Event: 2 (12:00 changed) */
#define RTCTEV_3            (0x0003)   /* RTC Time Event: 3 (00:00 changed) */
#define RTCTEV__MIN         (0x0000)   /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV__HOUR        (0x0001)   /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV__0000        (0x0002)   /* RTC Time Event: 2 (00:00 changed) */
#define RTCTEV__1200        (0x0003)   /* RTC Time Event: 3 (12:00 changed) */

#define RTCCALF_0           (0x0000)   /* RTC Calibration Frequency: No Output */
#define RTCCALF_1           (0x0100)   /* RTC Calibration Frequency: 512 Hz */
#define RTCCALF_2           (0x0200)   /* RTC Calibration Frequency: 256 Hz */
#define RTCCALF_3           (0x0300)   /* RTC Calibration Frequency: 1 Hz */


/* RTCOCAL Control Bits */
#define RTCOCALS            (0x8000)   /* RTC Offset Calibration Sign */
#define RTCOCAL7            (0x0080)   /* RTC Offset Calibration Bit 7 */
#define RTCOCAL6            (0x0040)   /* RTC Offset Calibration Bit 6 */
#define RTCOCAL5            (0x0020)   /* RTC Offset Calibration Bit 5 */
#define RTCOCAL4            (0x0010)   /* RTC Offset Calibration Bit 4 */
#define RTCOCAL3            (0x0008)   /* RTC Offset Calibration Bit 3 */
#define RTCOCAL2            (0x0004)   /* RTC Offset Calibration Bit 2 */
#define RTCOCAL1            (0x0002)   /* RTC Offset Calibration Bit 1 */
#define RTCOCAL0            (0x0001)   /* RTC Offset Calibration Bit 0 */

/* RTCOCAL Control Bits */
#define RTCOCAL7_L          (0x0080)   /* RTC Offset Calibration Bit 7 */
#define RTCOCAL6_L          (0x0040)   /* RTC Offset Calibration Bit 6 */
#define RTCOCAL5_L          (0x0020)   /* RTC Offset Calibration Bit 5 */
#define RTCOCAL4_L          (0x0010)   /* RTC Offset Calibration Bit 4 */
#define RTCOCAL3_L          (0x0008)   /* RTC Offset Calibration Bit 3 */
#define RTCOCAL2_L          (0x0004)   /* RTC Offset Calibration Bit 2 */
#define RTCOCAL1_L          (0x0002)   /* RTC Offset Calibration Bit 1 */
#define RTCOCAL0_L          (0x0001)   /* RTC Offset Calibration Bit 0 */

/* RTCOCAL Control Bits */
#define RTCOCALS_H          (0x0080)   /* RTC Offset Calibration Sign */

/* RTCTCMP Control Bits */
#define RTCTCMPS            (0x8000)   /* RTC Temperature Compensation Sign */
#define RTCTCRDY            (0x4000)   /* RTC Temperature compensation ready */
#define RTCTCOK             (0x2000)   /* RTC Temperature compensation write OK */
#define RTCTCMP7            (0x0080)   /* RTC Temperature Compensation Bit 7 */
#define RTCTCMP6            (0x0040)   /* RTC Temperature Compensation Bit 6 */
#define RTCTCMP5            (0x0020)   /* RTC Temperature Compensation Bit 5 */
#define RTCTCMP4            (0x0010)   /* RTC Temperature Compensation Bit 4 */
#define RTCTCMP3            (0x0008)   /* RTC Temperature Compensation Bit 3 */
#define RTCTCMP2            (0x0004)   /* RTC Temperature Compensation Bit 2 */
#define RTCTCMP1            (0x0002)   /* RTC Temperature Compensation Bit 1 */
#define RTCTCMP0            (0x0001)   /* RTC Temperature Compensation Bit 0 */

/* RTCTCMP Control Bits */
#define RTCTCMP7_L          (0x0080)   /* RTC Temperature Compensation Bit 7 */
#define RTCTCMP6_L          (0x0040)   /* RTC Temperature Compensation Bit 6 */
#define RTCTCMP5_L          (0x0020)   /* RTC Temperature Compensation Bit 5 */
#define RTCTCMP4_L          (0x0010)   /* RTC Temperature Compensation Bit 4 */
#define RTCTCMP3_L          (0x0008)   /* RTC Temperature Compensation Bit 3 */
#define RTCTCMP2_L          (0x0004)   /* RTC Temperature Compensation Bit 2 */
#define RTCTCMP1_L          (0x0002)   /* RTC Temperature Compensation Bit 1 */
#define RTCTCMP0_L          (0x0001)   /* RTC Temperature Compensation Bit 0 */

/* RTCTCMP Control Bits */
#define RTCTCMPS_H          (0x0080)   /* RTC Temperature Compensation Sign */
#define RTCTCRDY_H          (0x0040)   /* RTC Temperature compensation ready */
#define RTCTCOK_H           (0x0020)   /* RTC Temperature compensation write OK */


#define RTCAE               (0x80)     /* Real Time Clock Alarm enable */




/* RTCPS0CTL Control Bits */
//#define Reserved          (0x8000)
//#define Reserved          (0x4000)
#define RT0PSDIV2           (0x2000)   /* RTC Prescale Timer 0 Clock Divide Bit: 2 */
#define RT0PSDIV1           (0x1000)   /* RTC Prescale Timer 0 Clock Divide Bit: 1 */
#define RT0PSDIV0           (0x0800)   /* RTC Prescale Timer 0 Clock Divide Bit: 0 */
//#define Reserved          (0x0400)
//#define Reserved          (0x0200)
#define RT0PSHOLD           (0x0100)   /* RTC Prescale Timer 0 Hold */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT0IP2              (0x0010)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1              (0x0008)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0              (0x0004)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE             (0x0002)   /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG            (0x0001)   /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x8000)
//#define Reserved          (0x4000)
//#define Reserved          (0x0400)
//#define Reserved          (0x0200)
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT0IP2_L            (0x0010)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1_L            (0x0008)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0_L            (0x0004)   /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE_L           (0x0002)   /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG_L          (0x0001)   /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x8000)
//#define Reserved          (0x4000)
#define RT0PSDIV2_H         (0x0020)   /* RTC Prescale Timer 0 Clock Divide Bit: 2 */
#define RT0PSDIV1_H         (0x0010)   /* RTC Prescale Timer 0 Clock Divide Bit: 1 */
#define RT0PSDIV0_H         (0x0008)   /* RTC Prescale Timer 0 Clock Divide Bit: 0 */
//#define Reserved          (0x0400)
//#define Reserved          (0x0200)
#define RT0PSHOLD_H         (0x0001)   /* RTC Prescale Timer 0 Hold */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)

#define RT0IP_0             (0x0000)   /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP_1             (0x0004)   /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP_2             (0x0008)   /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP_3             (0x000C)   /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP_4             (0x0010)   /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP_5             (0x0014)   /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP_6             (0x0018)   /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP_7             (0x001C)   /* RTC Prescale Timer 0 Interrupt Interval /256 */

#define RT0PSDIV_0          (0x0000)   /* RTC Prescale Timer 0 Clock Divide: /2 */
#define RT0PSDIV_1          (0x0800)   /* RTC Prescale Timer 0 Clock Divide: /4 */
#define RT0PSDIV_2          (0x1000)   /* RTC Prescale Timer 0 Clock Divide: /8 */
#define RT0PSDIV_3          (0x1800)   /* RTC Prescale Timer 0 Clock Divide: /16 */
#define RT0PSDIV_4          (0x2000)   /* RTC Prescale Timer 0 Clock Divide: /32 */
#define RT0PSDIV_5          (0x2800)   /* RTC Prescale Timer 0 Clock Divide: /64 */
#define RT0PSDIV_6          (0x3000)   /* RTC Prescale Timer 0 Clock Divide: /128 */
#define RT0PSDIV_7          (0x3800)   /* RTC Prescale Timer 0 Clock Divide: /256 */


/* RTCPS1CTL Control Bits */
#define RT1SSEL1            (0x8000)   /* RTC Prescale Timer 1 Source Select Bit 1 */
#define RT1SSEL0            (0x4000)   /* RTC Prescale Timer 1 Source Select Bit 0 */
#define RT1PSDIV2           (0x2000)   /* RTC Prescale Timer 1 Clock Divide Bit: 2 */
#define RT1PSDIV1           (0x1000)   /* RTC Prescale Timer 1 Clock Divide Bit: 1 */
#define RT1PSDIV0           (0x0800)   /* RTC Prescale Timer 1 Clock Divide Bit: 0 */
//#define Reserved          (0x0400)
//#define Reserved          (0x0200)
#define RT1PSHOLD           (0x0100)   /* RTC Prescale Timer 1 Hold */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT1IP2              (0x0010)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1              (0x0008)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0              (0x0004)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE             (0x0002)   /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG            (0x0001)   /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0400)
//#define Reserved          (0x0200)
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT1IP2_L            (0x0010)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1_L            (0x0008)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0_L            (0x0004)   /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE_L           (0x0002)   /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG_L          (0x0001)   /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
#define RT1SSEL1_H          (0x0080)   /* RTC Prescale Timer 1 Source Select Bit 1 */
#define RT1SSEL0_H          (0x0040)   /* RTC Prescale Timer 1 Source Select Bit 0 */
#define RT1PSDIV2_H         (0x0020)   /* RTC Prescale Timer 1 Clock Divide Bit: 2 */
#define RT1PSDIV1_H         (0x0010)   /* RTC Prescale Timer 1 Clock Divide Bit: 1 */
#define RT1PSDIV0_H         (0x0008)   /* RTC Prescale Timer 1 Clock Divide Bit: 0 */
//#define Reserved          (0x0400)
//#define Reserved          (0x0200)
#define RT1PSHOLD_H         (0x0001)   /* RTC Prescale Timer 1 Hold */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)

#define RT1IP_0             (0x0000)   /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP_1             (0x0004)   /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP_2             (0x0008)   /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP_3             (0x000C)   /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP_4             (0x0010)   /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP_5             (0x0014)   /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP_6             (0x0018)   /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP_7             (0x001C)   /* RTC Prescale Timer 1 Interrupt Interval /256 */

#define RT1PSDIV_0          (0x0000)   /* RTC Prescale Timer 1 Clock Divide: /2 */
#define RT1PSDIV_1          (0x0800)   /* RTC Prescale Timer 1 Clock Divide: /4 */
#define RT1PSDIV_2          (0x1000)   /* RTC Prescale Timer 1 Clock Divide: /8 */
#define RT1PSDIV_3          (0x1800)   /* RTC Prescale Timer 1 Clock Divide: /16 */
#define RT1PSDIV_4          (0x2000)   /* RTC Prescale Timer 1 Clock Divide: /32 */
#define RT1PSDIV_5          (0x2800)   /* RTC Prescale Timer 1 Clock Divide: /64 */
#define RT1PSDIV_6          (0x3000)   /* RTC Prescale Timer 1 Clock Divide: /128 */
#define RT1PSDIV_7          (0x3800)   /* RTC Prescale Timer 1 Clock Divide: /256 */

#define RT1SSEL_0           (0x0000)   /* RTC Prescale Timer 1 Source Select: 0 */
#define RT1SSEL_1           (0x4000)   /* RTC Prescale Timer 1 Source Select: 1 */
#define RT1SSEL_2           (0x8000)   /* RTC Prescale Timer 1 Source Select: 2 */
#define RT1SSEL_3           (0xC000)   /* RTC Prescale Timer 1 Source Select: 3 */


/* RTCTCCTL0 Control Bits */
#define TCEN                (0x0001)   /* RTC Enable for RTC Tamper Detection with Time Stamp */
#define AUX3RST             (0x0002)   /* RTC Indication of power cycle on AUXVCC3 */

/* RTCTCCTL1 Control Bits */
#define RTCCAPIFG           (0x0001)   /* RTC  Tamper Event Interrupt Flag */
#define RTCCAPIE            (0x0002)   /* RTC Tamper Event Interrupt Enable */

/* RTCCAPxCTL Control Bits */
#define CAPEV               (0x0001)   /* RTC Tamper Event Flag */
#define CAPES               (0x0004)   /* RTC Event Edge Select */
#define RTCREN              (0x0008)   /* RTC RTCCAPx pin pullup/pulldown resistor enable */
#define RTCCAPIN            (0x0010)   /* RTC RTCCAPx input */
#define RTCCAPDIR           (0x0020)   /* RTC RTCCAPx Pin direction */
#define RTCCAPOUT           (0x0040)   /* RTC RTCCAPx Output */


/* RTCIV Definitions */
#define RTCIV_NONE         (0x0000)    /* No Interrupt pending */
#define RTCIV_RTCOFIFG     (0x0002)    /* RTC Osc fault: RTCOFIFG */
#define RTCIV_RTCCAPIFG    (0x0004)    /* RTC RTC Tamper Event: RTCCAPIFG */
#define RTCIV_RTCRDYIFG    (0x0006)    /* RTC ready: RTCRDYIFG */
#define RTCIV_RTCTEVIFG    (0x0008)    /* RTC interval timer: RTCTEVIFG */
#define RTCIV_RTCAIFG      (0x000A)    /* RTC user alarm: RTCAIFG */
#define RTCIV_RT0PSIFG     (0x000C)    /* RTC prescaler 0: RT0PSIFG */
#define RTCIV_RT1PSIFG     (0x000E)    /* RTC prescaler 1: RT1PSIFG */

/* Legacy RTCIV Definitions */
#define RTC_NONE           (0x0000)    /* No Interrupt pending */
#define RTC_RTCOFIFG       (0x0002)    /* RTC Osc fault: RTCOFIFG */
#define RTC_RTCRDYIFG      (0x0006)    /* RTC ready: RTCRDYIFG */
#define RTC_RTCTEVIFG      (0x0008)    /* RTC interval timer: RTCTEVIFG */
#define RTC_RTCAIFG        (0x000A)    /* RTC user alarm: RTCAIFG */
#define RTC_RT0PSIFG       (0x000C)    /* RTC prescaler 0: RT0PSIFG */
#define RTC_RT1PSIFG       (0x000E)    /* RTC prescaler 1: RT1PSIFG */

/************************************************************
* SD24_B - Sigma Delta 24 Bit
************************************************************/
#define __MSP430_HAS_SD24_B__         /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SD24_B__ 0x0800
#define SD24_BASE __MSP430_BASEADDRESS_SD24_B__
#define __MSP430_HAS_SD24_B4__        /* Definition to show that Module is available */

#define SD24BCTL0_            0x0800    /* SD24B Control Register 0 */
sfrb(SD24BCTL0_L , SD24BCTL0_);
sfrb(SD24BCTL0_H , SD24BCTL0_+1);
sfrw(SD24BCTL0, SD24BCTL0_);
#define SD24BCTL1_            0x0802    /* SD24B Control Register 1 */
sfrb(SD24BCTL1_L , SD24BCTL1_);
sfrb(SD24BCTL1_H , SD24BCTL1_+1);
sfrw(SD24BCTL1, SD24BCTL1_);
#define SD24BTRGCTL_          0x0804    /* SD24B Trigger Control Register */
sfrb(SD24BTRGCTL_L , SD24BTRGCTL_);
sfrb(SD24BTRGCTL_H , SD24BTRGCTL_+1);
sfrw(SD24BTRGCTL, SD24BTRGCTL_);
#define SD24BTRGOSR_          0x0806    /* SD24B Trigger OSR Control Register */
sfrb(SD24BTRGOSR_L , SD24BTRGOSR_);
sfrb(SD24BTRGOSR_H , SD24BTRGOSR_+1);
sfrw(SD24BTRGOSR, SD24BTRGOSR_);
#define SD24BTRGPRE_          0x0808    /* SD24B Trigger Preload Register */
sfrb(SD24BTRGPRE_L , SD24BTRGPRE_);
sfrb(SD24BTRGPRE_H , SD24BTRGPRE_+1);
sfrw(SD24BTRGPRE, SD24BTRGPRE_);
#define SD24BIFG_             0x080A    /* SD24B Interrupt Flag Register */
sfrb(SD24BIFG_L , SD24BIFG_);
sfrb(SD24BIFG_H , SD24BIFG_+1);
sfrw(SD24BIFG, SD24BIFG_);
#define SD24BIE_              0x080C    /* SD24B Interrupt Enable Register */
sfrb(SD24BIE_L , SD24BIE_);
sfrb(SD24BIE_H , SD24BIE_+1);
sfrw(SD24BIE, SD24BIE_);
#define SD24BIV_              0x080E    /* SD24B Interrupt Vector Register */
sfrb(SD24BIV_L , SD24BIV_);
sfrb(SD24BIV_H , SD24BIV_+1);
sfrw(SD24BIV, SD24BIV_);

#define SD24BCCTL0_           0x0810    /* SD24B Channel 0 Control Register */
sfrb(SD24BCCTL0_L , SD24BCCTL0_);
sfrb(SD24BCCTL0_H , SD24BCCTL0_+1);
sfrw(SD24BCCTL0, SD24BCCTL0_);
#define SD24BINCTL0_          0x0812    /* SD24B Channel 0 Input Control Register */
sfrb(SD24BINCTL0_L , SD24BINCTL0_);
sfrb(SD24BINCTL0_H , SD24BINCTL0_+1);
sfrw(SD24BINCTL0, SD24BINCTL0_);
#define SD24BOSR0_            0x0814    /* SD24B Channel 0 OSR Control Register */
sfrb(SD24BOSR0_L , SD24BOSR0_);
sfrb(SD24BOSR0_H , SD24BOSR0_+1);
sfrw(SD24BOSR0, SD24BOSR0_);
#define SD24BPRE0_            0x0816    /* SD24B Channel 0 Preload Register */
sfrb(SD24BPRE0_L , SD24BPRE0_);
sfrb(SD24BPRE0_H , SD24BPRE0_+1);
sfrw(SD24BPRE0, SD24BPRE0_);
#define SD24BCCTL1_           0x0818    /* SD24B Channel 1 Control Register */
sfrb(SD24BCCTL1_L , SD24BCCTL1_);
sfrb(SD24BCCTL1_H , SD24BCCTL1_+1);
sfrw(SD24BCCTL1, SD24BCCTL1_);
#define SD24BINCTL1_          0x081A    /* SD24B Channel 1 Input Control Register */
sfrb(SD24BINCTL1_L , SD24BINCTL1_);
sfrb(SD24BINCTL1_H , SD24BINCTL1_+1);
sfrw(SD24BINCTL1, SD24BINCTL1_);
#define SD24BOSR1_            0x081C    /* SD24B Channel 1 OSR Control Register */
sfrb(SD24BOSR1_L , SD24BOSR1_);
sfrb(SD24BOSR1_H , SD24BOSR1_+1);
sfrw(SD24BOSR1, SD24BOSR1_);
#define SD24BPRE1_            0x081E    /* SD24B Channel 1 Preload Register */
sfrb(SD24BPRE1_L , SD24BPRE1_);
sfrb(SD24BPRE1_H , SD24BPRE1_+1);
sfrw(SD24BPRE1, SD24BPRE1_);
#define SD24BCCTL2_           0x0820    /* SD24B Channel 2 Control Register */
sfrb(SD24BCCTL2_L , SD24BCCTL2_);
sfrb(SD24BCCTL2_H , SD24BCCTL2_+1);
sfrw(SD24BCCTL2, SD24BCCTL2_);
#define SD24BINCTL2_          0x0822    /* SD24B Channel 2 Input Control Register */
sfrb(SD24BINCTL2_L , SD24BINCTL2_);
sfrb(SD24BINCTL2_H , SD24BINCTL2_+1);
sfrw(SD24BINCTL2, SD24BINCTL2_);
#define SD24BOSR2_            0x0824    /* SD24B Channel 2 OSR Control Register */
sfrb(SD24BOSR2_L , SD24BOSR2_);
sfrb(SD24BOSR2_H , SD24BOSR2_+1);
sfrw(SD24BOSR2, SD24BOSR2_);
#define SD24BPRE2_            0x0826    /* SD24B Channel 2 Preload Register */
sfrb(SD24BPRE2_L , SD24BPRE2_);
sfrb(SD24BPRE2_H , SD24BPRE2_+1);
sfrw(SD24BPRE2, SD24BPRE2_);
#define SD24BCCTL3_           0x0828    /* SD24B Channel 3 Control Register */
sfrb(SD24BCCTL3_L , SD24BCCTL3_);
sfrb(SD24BCCTL3_H , SD24BCCTL3_+1);
sfrw(SD24BCCTL3, SD24BCCTL3_);
#define SD24BINCTL3_          0x082A    /* SD24B Channel 3 Input Control Register */
sfrb(SD24BINCTL3_L , SD24BINCTL3_);
sfrb(SD24BINCTL3_H , SD24BINCTL3_+1);
sfrw(SD24BINCTL3, SD24BINCTL3_);
#define SD24BOSR3_            0x082C    /* SD24B Channel 3 OSR Control Register */
sfrb(SD24BOSR3_L , SD24BOSR3_);
sfrb(SD24BOSR3_H , SD24BOSR3_+1);
sfrw(SD24BOSR3, SD24BOSR3_);
#define SD24BPRE3_            0x082E    /* SD24B Channel 3 Preload Register */
sfrb(SD24BPRE3_L , SD24BPRE3_);
sfrb(SD24BPRE3_H , SD24BPRE3_+1);
sfrw(SD24BPRE3, SD24BPRE3_);

#define SD24BMEML0_           0x0850    /* SD24B Channel 0 Conversion Memory Low word */
sfrb(SD24BMEML0_L , SD24BMEML0_);
sfrb(SD24BMEML0_H , SD24BMEML0_+1);
sfrw(SD24BMEML0, SD24BMEML0_);
#define SD24BMEMH0_           0x0852    /* SD24B Channel 0 Conversion Memory High Word */
sfrb(SD24BMEMH0_L , SD24BMEMH0_);
sfrb(SD24BMEMH0_H , SD24BMEMH0_+1);
sfrw(SD24BMEMH0, SD24BMEMH0_);
#define SD24BMEML1_           0x0854    /* SD24B Channel 1 Conversion Memory Low word */
sfrb(SD24BMEML1_L , SD24BMEML1_);
sfrb(SD24BMEML1_H , SD24BMEML1_+1);
sfrw(SD24BMEML1, SD24BMEML1_);
#define SD24BMEMH1_           0x0856    /* SD24B Channel 1 Conversion Memory High Word */
sfrb(SD24BMEMH1_L , SD24BMEMH1_);
sfrb(SD24BMEMH1_H , SD24BMEMH1_+1);
sfrw(SD24BMEMH1, SD24BMEMH1_);
#define SD24BMEML2_           0x0858    /* SD24B Channel 2 Conversion Memory Low word */
sfrb(SD24BMEML2_L , SD24BMEML2_);
sfrb(SD24BMEML2_H , SD24BMEML2_+1);
sfrw(SD24BMEML2, SD24BMEML2_);
#define SD24BMEMH2_           0x085A    /* SD24B Channel 2 Conversion Memory High Word */
sfrb(SD24BMEMH2_L , SD24BMEMH2_);
sfrb(SD24BMEMH2_H , SD24BMEMH2_+1);
sfrw(SD24BMEMH2, SD24BMEMH2_);
#define SD24BMEML3_           0x085C    /* SD24B Channel 3 Conversion Memory Low word */
sfrb(SD24BMEML3_L , SD24BMEML3_);
sfrb(SD24BMEML3_H , SD24BMEML3_+1);
sfrw(SD24BMEML3, SD24BMEML3_);
#define SD24BMEMH3_           0x085E    /* SD24B Channel 3 Conversion Memory High Word */
sfrb(SD24BMEMH3_L , SD24BMEMH3_);
sfrb(SD24BMEMH3_H , SD24BMEMH3_+1);
sfrw(SD24BMEMH3, SD24BMEMH3_);


/* SD24BCTL0 */
#define SD24OV32            (0x0002)  /* SD24B Overflow Control */
#define SD24REFS            (0x0004)  /* SD24B Reference Select */
#define SD24SSEL0           (0x0010)  /* SD24B Clock Source Select 0 */
#define SD24SSEL1           (0x0020)  /* SD24B Clock Source Select 1 */
#define SD24M4              (0x0040)  /* SD24B Modulator clock to Manchester decoder clock ratio */
#define SD24CLKOS           (0x0080)  /* SD24B Clock Output Select */
#define SD24PDIV0           (0x0100)  /* SD24B Frequency pre-scaler Bit 0 */
#define SD24PDIV1           (0x0200)  /* SD24B Frequency pre-scaler Bit 1 */
#define SD24PDIV2           (0x0400)  /* SD24B Frequency pre-scaler Bit 2 */
#define SD24DIV0            (0x0800)  /* SD24B Frequency Divider Bit 0 */
#define SD24DIV1            (0x1000)  /* SD24B Frequency Divider Bit 1 */
#define SD24DIV2            (0x2000)  /* SD24B Frequency Divider Bit 2 */
#define SD24DIV3            (0x4000)  /* SD24B Frequency Divider Bit 3 */
#define SD24DIV4            (0x8000)  /* SD24B Frequency Divider Bit 4 */

#define SD24OV32_L          (0x0002)  /* SD24B Overflow Control */
#define SD24REFS_L          (0x0004)  /* SD24B Reference Select */
#define SD24SSEL0_L         (0x0010)  /* SD24B Clock Source Select 0 */
#define SD24SSEL1_L         (0x0020)  /* SD24B Clock Source Select 1 */
#define SD24M4_L            (0x0040)  /* SD24B Modulator clock to Manchester decoder clock ratio */
#define SD24CLKOS_L         (0x0080)  /* SD24B Clock Output Select */

#define SD24PDIV0_H         (0x0001)  /* SD24B Frequency pre-scaler Bit 0 */
#define SD24PDIV1_H         (0x0002)  /* SD24B Frequency pre-scaler Bit 1 */
#define SD24PDIV2_H         (0x0004)  /* SD24B Frequency pre-scaler Bit 2 */
#define SD24DIV0_H          (0x0008)  /* SD24B Frequency Divider Bit 0 */
#define SD24DIV1_H          (0x0010)  /* SD24B Frequency Divider Bit 1 */
#define SD24DIV2_H          (0x0020)  /* SD24B Frequency Divider Bit 2 */
#define SD24DIV3_H          (0x0040)  /* SD24B Frequency Divider Bit 3 */
#define SD24DIV4_H          (0x0080)  /* SD24B Frequency Divider Bit 4 */

#define SD24SSEL_0          (0x0000)  /* SD24B Clock Source Select MCLK  */
#define SD24SSEL_1          (0x0010)  /* SD24B Clock Source Select SMCLK */
#define SD24SSEL_2          (0x0020)  /* SD24B Clock Source Select ACLK  */
#define SD24SSEL_3          (0x0030)  /* SD24B Clock Source Select TACLK */
#define SD24SSEL__MCLK      (0x0000)  /* SD24B Clock Source Select MCLK  */
#define SD24SSEL__SMCLK     (0x0010)  /* SD24B Clock Source Select SMCLK */
#define SD24SSEL__ACLK      (0x0020)  /* SD24B Clock Source Select ACLK  */
#define SD24SSEL__SD24CLK   (0x0030)  /* SD24B Clock Source Select SD24CLK */

#define SD24PDIV_0          (0x0000)  /* SD24B Frequency pre-scaler  /1 */
#define SD24PDIV_1          (0x0100)  /* SD24B Frequency pre-scaler  /2 */
#define SD24PDIV_2          (0x0200)  /* SD24B Frequency pre-scaler  /4 */
#define SD24PDIV_3          (0x0300)  /* SD24B Frequency pre-scaler  /8 */
#define SD24PDIV_4          (0x0400)  /* SD24B Frequency pre-scaler  /16 */
#define SD24PDIV_5          (0x0500)  /* SD24B Frequency pre-scaler  /32 */
#define SD24PDIV_6          (0x0600)  /* SD24B Frequency pre-scaler  /64 */
#define SD24PDIV_7          (0x0700)  /* SD24B Frequency pre-scaler  /128 */

/* SD24BCTL1 */
#define SD24GRP0SC          (0x0001)  /* SD24B Group 0 Start Conversion */
#define SD24GRP1SC          (0x0002)  /* SD24B Group 1 Start Conversion */
#define SD24GRP2SC          (0x0004)  /* SD24B Group 2 Start Conversion */
#define SD24GRP3SC          (0x0008)  /* SD24B Group 3 Start Conversion */
#define SD24DMA0            (0x0100)  /* SD24B DMA Trigger Select Bit 0 */
#define SD24DMA1            (0x0200)  /* SD24B DMA Trigger Select Bit 1 */
#define SD24DMA2            (0x0400)  /* SD24B DMA Trigger Select Bit 2 */
#define SD24DMA3            (0x0800)  /* SD24B DMA Trigger Select Bit 3 */

#define SD24GRP0SC_L        (0x0001)  /* SD24B Group 0 Start Conversion */
#define SD24GRP1SC_L        (0x0002)  /* SD24B Group 1 Start Conversion */
#define SD24GRP2SC_L        (0x0004)  /* SD24B Group 2 Start Conversion */
#define SD24GRP3SC_L        (0x0008)  /* SD24B Group 3 Start Conversion */

#define SD24DMA0_H          (0x0001)  /* SD24B DMA Trigger Select Bit 0 */
#define SD24DMA1_H          (0x0002)  /* SD24B DMA Trigger Select Bit 1 */
#define SD24DMA2_H          (0x0004)  /* SD24B DMA Trigger Select Bit 2 */
#define SD24DMA3_H          (0x0008)  /* SD24B DMA Trigger Select Bit 3 */

#define SD24DMA_0           (0x0000)  /* SD24B DMA Trigger: 0 */
#define SD24DMA_1           (0x0100)  /* SD24B DMA Trigger: 1 */
#define SD24DMA_2           (0x0200)  /* SD24B DMA Trigger: 2 */
#define SD24DMA_3           (0x0300)  /* SD24B DMA Trigger: 3 */
#define SD24DMA_4           (0x0400)  /* SD24B DMA Trigger: 4 */
#define SD24DMA_5           (0x0500)  /* SD24B DMA Trigger: 5 */
#define SD24DMA_6           (0x0600)  /* SD24B DMA Trigger: 6 */
#define SD24DMA_7           (0x0700)  /* SD24B DMA Trigger: 7 */
#define SD24DMA_8           (0x0800)  /* SD24B DMA Trigger: 8 */

/* SD24BIFG */
#define SD24IFG0            (0x0001)  /* SD24B Channel 0 Interrupt Flag */
#define SD24IFG1            (0x0002)  /* SD24B Channel 1 Interrupt Flag */
#define SD24IFG2            (0x0004)  /* SD24B Channel 2 Interrupt Flag */
#define SD24IFG3            (0x0008)  /* SD24B Channel 3 Interrupt Flag */
#define SD24OVIFG0          (0x0100)  /* SD24B Channel 0 Overflow Interrupt Flag */
#define SD24OVIFG1          (0x0200)  /* SD24B Channel 1 Overflow Interrupt Flag */
#define SD24OVIFG2          (0x0400)  /* SD24B Channel 2 Overflow Interrupt Flag */
#define SD24OVIFG3          (0x0800)  /* SD24B Channel 3 Overflow Interrupt Flag */

#define SD24IFG0_L          (0x0001)  /* SD24B Channel 0 Interrupt Flag */
#define SD24IFG1_L          (0x0002)  /* SD24B Channel 1 Interrupt Flag */
#define SD24IFG2_L          (0x0004)  /* SD24B Channel 2 Interrupt Flag */
#define SD24IFG3_L          (0x0008)  /* SD24B Channel 3 Interrupt Flag */

#define SD24OVIFG0_H        (0x0001)  /* SD24B Channel 0 Overflow Interrupt Flag */
#define SD24OVIFG1_H        (0x0002)  /* SD24B Channel 1 Overflow Interrupt Flag */
#define SD24OVIFG2_H        (0x0004)  /* SD24B Channel 2 Overflow Interrupt Flag */
#define SD24OVIFG3_H        (0x0008)  /* SD24B Channel 3 Overflow Interrupt Flag */

/* SD24BIE */
#define SD24IE0             (0x0001)  /* SD24B Channel 0 Interrupt Enable */
#define SD24IE1             (0x0002)  /* SD24B Channel 1 Interrupt Enable */
#define SD24IE2             (0x0004)  /* SD24B Channel 2 Interrupt Enable */
#define SD24IE3             (0x0008)  /* SD24B Channel 3 Interrupt Enable */
#define SD24OVIE0           (0x0100)  /* SD24B Channel 0 Overflow Interrupt Enable */
#define SD24OVIE1           (0x0200)  /* SD24B Channel 1 Overflow Interrupt Enable */
#define SD24OVIE2           (0x0400)  /* SD24B Channel 2 Overflow Interrupt Enable */
#define SD24OVIE3           (0x0800)  /* SD24B Channel 3 Overflow Interrupt Enable */

#define SD24IE0_L           (0x0001)  /* SD24B Channel 0 Interrupt Enable */
#define SD24IE1_L           (0x0002)  /* SD24B Channel 1 Interrupt Enable */
#define SD24IE2_L           (0x0004)  /* SD24B Channel 2 Interrupt Enable */
#define SD24IE3_L           (0x0008)  /* SD24B Channel 3 Interrupt Enable */

#define SD24OVIE0_H         (0x0001)  /* SD24B Channel 0 Overflow Interrupt Enable */
#define SD24OVIE1_H         (0x0002)  /* SD24B Channel 1 Overflow Interrupt Enable */
#define SD24OVIE2_H         (0x0004)  /* SD24B Channel 2 Overflow Interrupt Enable */
#define SD24OVIE3_H         (0x0008)  /* SD24B Channel 3 Overflow Interrupt Enable */

/* SD24BIV Definitions */
#define SD24BIV_NONE         (0x0000)    /* No Interrupt pending */
#define SD24BIV_SD24OVIFG    (0x0002)    /* SD24OVIFG */
#define SD24BIV_SD24TRGIFG   (0x0004)    /* SD24TRGIFG */
#define SD24BIV_SD24IFG0     (0x0006)    /* SD24IFG0 */
#define SD24BIV_SD24IFG1     (0x0008)    /* SD24IFG1 */
#define SD24BIV_SD24IFG2     (0x000A)    /* SD24IFG2 */
#define SD24BIV_SD24IFG3     (0x000C)    /* SD24IFG3 */

/* SD24BCCTLx */
#define SD24SC               (0x0001)  /* SD24B Start Conversion */
#define SD24SCS0             (0x0002)  /* SD24B Start Conversion Select Bit 0 */
#define SD24SCS1             (0x0004)  /* SD24B Start Conversion Select Bit 1 */
#define SD24SCS2             (0x0008)  /* SD24B Start Conversion Select Bit 2 */
#define SD24DF0              (0x0010)  /* SD24B Data Format Bit: 0 */
#define SD24DF1              (0x0020)  /* SD24B Data Format Bit: 1 */
#define SD24ALGN             (0x0040)  /* SD24B Data Alignment */
#define SD24SNGL             (0x0100)  /* SD24B Single Trigger Mode */
#define SD24CAL              (0x0200)  /* SD24B Calibration */
#define SD24DFS0             (0x0400)  /* SD24B Digital Filter Bit: 0 */
#define SD24DFS1             (0x0800)  /* SD24B Digital Filter Bit: 1 */
#define SD24DI               (0x1000)  /* SD24B Digital Bitstream Input */
#define SD24MC0              (0x2000)  /* SD24B Manchaster Encoding Bit: 0 */
#define SD24MC1              (0x4000)  /* SD24B Manchaster Encoding Bit: 1 */

#define SD24SC_L            (0x0001)  /* SD24B Start Conversion */
#define SD24SCS0_L          (0x0002)  /* SD24B Start Conversion Select Bit 0 */
#define SD24SCS1_L          (0x0004)  /* SD24B Start Conversion Select Bit 1 */
#define SD24SCS2_L          (0x0008)  /* SD24B Start Conversion Select Bit 2 */
#define SD24DF0_L           (0x0010)  /* SD24B Data Format Bit: 0 */
#define SD24DF1_L           (0x0020)  /* SD24B Data Format Bit: 1 */
#define SD24ALGN_L          (0x0040)  /* SD24B Data Alignment */

#define SD24SNGL_H          (0x0001)  /* SD24B Single Trigger Mode */
#define SD24CAL_H           (0x0002)  /* SD24B Calibration */
#define SD24DFS0_H          (0x0004)  /* SD24B Digital Filter Bit: 0 */
#define SD24DFS1_H          (0x0008)  /* SD24B Digital Filter Bit: 1 */
#define SD24DI_H            (0x0010)  /* SD24B Digital Bitstream Input */
#define SD24MC0_H           (0x0020)  /* SD24B Manchaster Encoding Bit: 0 */
#define SD24MC1_H           (0x0040)  /* SD24B Manchaster Encoding Bit: 1 */

/* SD24BTRGCTL */
#define SD24TRGIFG          (0x0400)  /* SD24B Trigger Interrupt Flag */
#define SD24TRGIE           (0x0800)  /* SD24B Trigger Interrupt Enable */


#define SD24TRGIFG_H        (0x0004)  /* SD24B Trigger Interrupt Flag */
#define SD24TRGIE_H         (0x0008)  /* SD24B Trigger Interrupt Enable */

#define SD24SCS_0            (0x0000)  /* SD24B Start Conversion Select: 0 */
#define SD24SCS_1            (0x0002)  /* SD24B Start Conversion Select: 1 */
#define SD24SCS_2            (0x0004)  /* SD24B Start Conversion Select: 2 */
#define SD24SCS_3            (0x0006)  /* SD24B Start Conversion Select: 3 */
#define SD24SCS_4            (0x0008)  /* SD24B Start Conversion Select: 4 */
#define SD24SCS_5            (0x000A)  /* SD24B Start Conversion Select: 5 */
#define SD24SCS_6            (0x000C)  /* SD24B Start Conversion Select: 6 */
#define SD24SCS_7            (0x000E)  /* SD24B Start Conversion Select: 7 */
#define SD24SCS__SD24SC      (0x0000)  /* SD24B Start Conversion Select: SD24SC */
#define SD24SCS__EXT1        (0x0002)  /* SD24B Start Conversion Select: EXT1   */
#define SD24SCS__EXT2        (0x0004)  /* SD24B Start Conversion Select: EXT2   */
#define SD24SCS__EXT3        (0x0006)  /* SD24B Start Conversion Select: EXT3   */
#define SD24SCS__GROUP0      (0x0008)  /* SD24B Start Conversion Select: GROUP0 */
#define SD24SCS__GROUP1      (0x000A)  /* SD24B Start Conversion Select: GROUP1 */
#define SD24SCS__GROUP2      (0x000C)  /* SD24B Start Conversion Select: GROUP2 */
#define SD24SCS__GROUP3      (0x000E)  /* SD24B Start Conversion Select: GROUP3 */

#define SD24DF_0             (0x0000)  /* SD24B Data Format: Offset Binary  */
#define SD24DF_1             (0x0010)  /* SD24B Data Format: 2's complement */

#define SD24DFS_0            (0x0000)  /* SD24B Digital Filter 0 */
#define SD24DFS_1            (0x0400)  /* SD24B Digital Filter 1 */
#define SD24DFS_2            (0x0800)  /* SD24B Digital Filter 2 */
#define SD24DFS_3            (0x0C00)  /* SD24B Digital Filter 3 */

#define SD24MC_0             (0x0000)  /* SD24B Manchaster Encoding 0 */
#define SD24MC_1             (0x2000)  /* SD24B Manchaster Encoding 1 */
#define SD24MC_2             (0x4000)  /* SD24B Manchaster Encoding 2 */
#define SD24MC_3             (0x6000)  /* SD24B Manchaster Encoding 3 */

/* SD24BINCTLx */
#define SD24GAIN0           (0x0008)  /* SD24B Input Pre-Amplifier Gain Select 0 */
#define SD24GAIN1           (0x0010)  /* SD24B Input Pre-Amplifier Gain Select 1 */
#define SD24GAIN2           (0x0020)  /* SD24B Input Pre-Amplifier Gain Select 2 */
#define SD24INTDLY0         (0x0040)  /* SD24B Interrupt Delay after 1.Conversion 0 */
#define SD24INTDLY1         (0x0080)  /* SD24B Interrupt Delay after 1.Conversion 1 */

#define SD24GAIN0_L         (0x0008)  /* SD24B Input Pre-Amplifier Gain Select 0 */
#define SD24GAIN1_L         (0x0010)  /* SD24B Input Pre-Amplifier Gain Select 1 */
#define SD24GAIN2_L         (0x0020)  /* SD24B Input Pre-Amplifier Gain Select 2 */
#define SD24INTDLY0_L       (0x0040)  /* SD24B Interrupt Delay after 1.Conversion 0 */
#define SD24INTDLY1_L       (0x0080)  /* SD24B Interrupt Delay after 1.Conversion 1 */


#define SD24GAIN_1          (0x0000)  /* SD24B Input Pre-Amplifier Gain Select *1  */
#define SD24GAIN_2          (0x0008)  /* SD24B Input Pre-Amplifier Gain Select *2  */
#define SD24GAIN_4          (0x0010)  /* SD24B Input Pre-Amplifier Gain Select *4  */
#define SD24GAIN_8          (0x0018)  /* SD24B Input Pre-Amplifier Gain Select *8  */
#define SD24GAIN_16         (0x0020)  /* SD24B Input Pre-Amplifier Gain Select *16 */
#define SD24GAIN_32         (0x0028)  /* SD24B Input Pre-Amplifier Gain Select *32 */
#define SD24GAIN_64         (0x0030)  /* SD24B Input Pre-Amplifier Gain Select *64 */
#define SD24GAIN_128        (0x0038)  /* SD24B Input Pre-Amplifier Gain Select *128 */

#define SD24INTDLY_0        (0x0000)  /* SD24B Interrupt Delay: Int. after 4.Conversion  */
#define SD24INTDLY_1        (0x0040)  /* SD24B Interrupt Delay: Int. after 3.Conversion  */
#define SD24INTDLY_2        (0x0080)  /* SD24B Interrupt Delay: Int. after 2.Conversion  */
#define SD24INTDLY_3        (0x00C0)  /* SD24B Interrupt Delay: Int. after 1.Conversion  */

/* SD24BOSRx */
#define OSR0               (0x0001)  /* SD24B Oversampling Rate Bit: 0 */
#define OSR1               (0x0002)  /* SD24B Oversampling Rate Bit: 1 */
#define OSR2               (0x0004)  /* SD24B Oversampling Rate Bit: 2 */
#define OSR3               (0x0008)  /* SD24B Oversampling Rate Bit: 3 */
#define OSR4               (0x0010)  /* SD24B Oversampling Rate Bit: 4 */
#define OSR5               (0x0020)  /* SD24B Oversampling Rate Bit: 5 */
#define OSR6               (0x0040)  /* SD24B Oversampling Rate Bit: 6 */
#define OSR7               (0x0080)  /* SD24B Oversampling Rate Bit: 7 */
#define OSR8               (0x0100)  /* SD24B Oversampling Rate Bit: 8 */
#define OSR9               (0x0200)  /* SD24B Oversampling Rate Bit: 9 */
#define OSR10               (0x0400)  /* SD24B Oversampling Rate Bit: 10 */

#define OSR0_L              (0x0001)  /* SD24B Oversampling Rate Bit: 0 */
#define OSR1_L              (0x0002)  /* SD24B Oversampling Rate Bit: 1 */
#define OSR2_L              (0x0004)  /* SD24B Oversampling Rate Bit: 2 */
#define OSR3_L              (0x0008)  /* SD24B Oversampling Rate Bit: 3 */
#define OSR4_L              (0x0010)  /* SD24B Oversampling Rate Bit: 4 */
#define OSR5_L              (0x0020)  /* SD24B Oversampling Rate Bit: 5 */
#define OSR6_L              (0x0040)  /* SD24B Oversampling Rate Bit: 6 */
#define OSR7_L              (0x0080)  /* SD24B Oversampling Rate Bit: 7 */

#define OSR8_H              (0x0001)  /* SD24B Oversampling Rate Bit: 8 */
#define OSR9_H              (0x0002)  /* SD24B Oversampling Rate Bit: 9 */
#define OSR10_H             (0x0004)  /* SD24B Oversampling Rate Bit: 10 */


/* SD24BTRGOSR */



#define OSR__32             (32-1)    /* SD24B Oversampling Rate: 32 */
#define OSR__64             (64-1)    /* SD24B Oversampling Rate: 64 */
#define OSR__128            (128-1)   /* SD24B Oversampling Rate: 128 */
#define OSR__256            (256-1)   /* SD24B Oversampling Rate: 256 */
#define OSR__512            (512-1)   /* SD24B Oversampling Rate: 512 */
#define OSR__1024           (1024-1)  /* SD24B Oversampling Rate: 1024 */


/************************************************************
* SFR - Special Function Register Module
************************************************************/
#define __MSP430_HAS_SFR__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SFR__ 0x0100
#define SFR_BASE __MSP430_BASEADDRESS_SFR__

#define SFRIE1_               0x0100    /* Interrupt Enable 1 */
sfrb(SFRIE1_L , SFRIE1_);
sfrb(SFRIE1_H , SFRIE1_+1);
sfrw(SFRIE1, SFRIE1_);

/* SFRIE1 Control Bits */
#define WDTIE               (0x0001)  /* WDT Interrupt Enable */
#define OFIE                (0x0002)  /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE               (0x0008)  /* Vacant Memory Interrupt Enable */
#define NMIIE               (0x0010)  /* NMI Interrupt Enable */
#define ACCVIE              (0x0020)  /* Flash Access Violation Interrupt Enable */
#define JMBINIE             (0x0040)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE            (0x0080)  /* JTAG Mail Box output Interrupt Enable */

#define WDTIE_L             (0x0001)  /* WDT Interrupt Enable */
#define OFIE_L              (0x0002)  /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE_L             (0x0008)  /* Vacant Memory Interrupt Enable */
#define NMIIE_L             (0x0010)  /* NMI Interrupt Enable */
#define ACCVIE_L            (0x0020)  /* Flash Access Violation Interrupt Enable */
#define JMBINIE_L           (0x0040)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE_L          (0x0080)  /* JTAG Mail Box output Interrupt Enable */


#define SFRIFG1_              0x0102    /* Interrupt Flag 1 */
sfrb(SFRIFG1_L , SFRIFG1_);
sfrb(SFRIFG1_H , SFRIFG1_+1);
sfrw(SFRIFG1, SFRIFG1_);
/* SFRIFG1 Control Bits */
#define WDTIFG              (0x0001)  /* WDT Interrupt Flag */
#define OFIFG               (0x0002)  /* Osc Fault Flag */
//#define Reserved          (0x0004)
#define VMAIFG              (0x0008)  /* Vacant Memory Interrupt Flag */
#define NMIIFG              (0x0010)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020)
#define JMBINIFG            (0x0040)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG           (0x0080)  /* JTAG Mail Box output Interrupt Flag */

#define WDTIFG_L            (0x0001)  /* WDT Interrupt Flag */
#define OFIFG_L             (0x0002)  /* Osc Fault Flag */
//#define Reserved          (0x0004)
#define VMAIFG_L            (0x0008)  /* Vacant Memory Interrupt Flag */
#define NMIIFG_L            (0x0010)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020)
#define JMBINIFG_L          (0x0040)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG_L         (0x0080)  /* JTAG Mail Box output Interrupt Flag */


#define SFRRPCR_              0x0104    /* RESET Pin Control Register */
sfrb(SFRRPCR_L , SFRRPCR_);
sfrb(SFRRPCR_H , SFRRPCR_+1);
sfrw(SFRRPCR, SFRRPCR_);
/* SFRRPCR Control Bits */
#define SYSNMI              (0x0001)  /* NMI select */
#define SYSNMIIES           (0x0002)  /* NMI edge select */
#define SYSRSTUP            (0x0004)  /* RESET Pin pull down/up select */
#define SYSRSTRE            (0x0008)  /* RESET Pin Resistor enable */

#define SYSNMI_L            (0x0001)  /* NMI select */
#define SYSNMIIES_L         (0x0002)  /* NMI edge select */
#define SYSRSTUP_L          (0x0004)  /* RESET Pin pull down/up select */
#define SYSRSTRE_L          (0x0008)  /* RESET Pin Resistor enable */


/************************************************************
* SYS - System Module
************************************************************/
#define __MSP430_HAS_SYS__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SYS__ 0x0180
#define SYS_BASE __MSP430_BASEADDRESS_SYS__

#define SYSCTL_               0x0180    /* System control */
sfrb(SYSCTL_L , SYSCTL_);
sfrb(SYSCTL_H , SYSCTL_+1);
sfrw(SYSCTL, SYSCTL_);
#define SYSBSLC_              0x0182    /* Boot strap configuration area */
sfrb(SYSBSLC_L , SYSBSLC_);
sfrb(SYSBSLC_H , SYSBSLC_+1);
sfrw(SYSBSLC, SYSBSLC_);
#define SYSJMBC_              0x0186    /* JTAG mailbox control */
sfrb(SYSJMBC_L , SYSJMBC_);
sfrb(SYSJMBC_H , SYSJMBC_+1);
sfrw(SYSJMBC, SYSJMBC_);
#define SYSJMBI0_             0x0188    /* JTAG mailbox input 0 */
sfrb(SYSJMBI0_L , SYSJMBI0_);
sfrb(SYSJMBI0_H , SYSJMBI0_+1);
sfrw(SYSJMBI0, SYSJMBI0_);
#define SYSJMBI1_             0x018A    /* JTAG mailbox input 1 */
sfrb(SYSJMBI1_L , SYSJMBI1_);
sfrb(SYSJMBI1_H , SYSJMBI1_+1);
sfrw(SYSJMBI1, SYSJMBI1_);
#define SYSJMBO0_             0x018C    /* JTAG mailbox output 0 */
sfrb(SYSJMBO0_L , SYSJMBO0_);
sfrb(SYSJMBO0_H , SYSJMBO0_+1);
sfrw(SYSJMBO0, SYSJMBO0_);
#define SYSJMBO1_             0x018E    /* JTAG mailbox output 1 */
sfrb(SYSJMBO1_L , SYSJMBO1_);
sfrb(SYSJMBO1_H , SYSJMBO1_+1);
sfrw(SYSJMBO1, SYSJMBO1_);

#define SYSBERRIV_            0x0198    /* Bus Error vector generator */
sfrb(SYSBERRIV_L , SYSBERRIV_);
sfrb(SYSBERRIV_H , SYSBERRIV_+1);
sfrw(SYSBERRIV, SYSBERRIV_);
#define SYSUNIV_              0x019A    /* User NMI vector generator */
sfrb(SYSUNIV_L , SYSUNIV_);
sfrb(SYSUNIV_H , SYSUNIV_+1);
sfrw(SYSUNIV, SYSUNIV_);
#define SYSSNIV_              0x019C    /* System NMI vector generator */
sfrb(SYSSNIV_L , SYSSNIV_);
sfrb(SYSSNIV_H , SYSSNIV_+1);
sfrw(SYSSNIV, SYSSNIV_);
#define SYSRSTIV_             0x019E    /* Reset vector generator */
sfrb(SYSRSTIV_L , SYSRSTIV_);
sfrb(SYSRSTIV_H , SYSRSTIV_+1);
sfrw(SYSRSTIV, SYSRSTIV_);

/* SYSCTL Control Bits */
#define SYSRIVECT           (0x0001)  /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002)  /* SYS - Reserved */
#define SYSPMMPE            (0x0004)  /* SYS - PMM access protect */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
#define SYSBSLIND           (0x0010)  /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN          (0x0020)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSCTL Control Bits */
#define SYSRIVECT_L         (0x0001)  /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002)  /* SYS - Reserved */
#define SYSPMMPE_L          (0x0004)  /* SYS - PMM access protect */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
#define SYSBSLIND_L         (0x0010)  /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN_L        (0x0020)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */


/* SYSBSLC Control Bits */
#define SYSBSLSIZE0         (0x0001)  /* SYS - BSL Protection Size 0 */
#define SYSBSLSIZE1         (0x0002)  /* SYS - BSL Protection Size 1 */
#define SYSBSLR             (0x0004)  /* SYS - RAM assigned to BSL */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0010)  /* SYS - Reserved */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
#define SYSBSLOFF           (0x4000)  /* SYS - BSL Memory disabled */
#define SYSBSLPE            (0x8000)  /* SYS - BSL Memory protection enabled */

/* SYSBSLC Control Bits */
#define SYSBSLSIZE0_L       (0x0001)  /* SYS - BSL Protection Size 0 */
#define SYSBSLSIZE1_L       (0x0002)  /* SYS - BSL Protection Size 1 */
#define SYSBSLR_L           (0x0004)  /* SYS - RAM assigned to BSL */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0010)  /* SYS - Reserved */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */

/* SYSBSLC Control Bits */
//#define RESERVED            (0x0008)  /* SYS - Reserved */
//#define RESERVED            (0x0010)  /* SYS - Reserved */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
//#define RESERVED            (0x0040)  /* SYS - Reserved */
//#define RESERVED            (0x0080)  /* SYS - Reserved */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
#define SYSBSLOFF_H         (0x0040)  /* SYS - BSL Memory disabled */
#define SYSBSLPE_H          (0x0080)  /* SYS - BSL Memory protection enabled */

/* SYSJMBC Control Bits */
#define JMBIN0FG            (0x0001)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG            (0x0002)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG           (0x0004)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG           (0x0008)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE             (0x0010)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
#define JMBCLR0OFF          (0x0040)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF          (0x0080)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
#define JMBIN0FG_L          (0x0001)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG_L          (0x0002)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG_L         (0x0004)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG_L         (0x0008)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE_L           (0x0010)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020)  /* SYS - Reserved */
#define JMBCLR0OFF_L        (0x0040)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF_L        (0x0080)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100)  /* SYS - Reserved */
//#define RESERVED            (0x0200)  /* SYS - Reserved */
//#define RESERVED            (0x0400)  /* SYS - Reserved */
//#define RESERVED            (0x0800)  /* SYS - Reserved */
//#define RESERVED            (0x1000)  /* SYS - Reserved */
//#define RESERVED            (0x2000)  /* SYS - Reserved */
//#define RESERVED            (0x4000)  /* SYS - Reserved */
//#define RESERVED            (0x8000)  /* SYS - Reserved */







/* SYSUNIV Definitions */
#define SYSUNIV_NONE       (0x0000)    /* No Interrupt pending */
#define SYSUNIV_NMIIFG     (0x0002)    /* SYSUNIV   : NMIIFG */
#define SYSUNIV_OFIFG      (0x0004)    /* SYSUNIV   : Osc. Fail - OFIFG */
#define SYSUNIV_ACCVIFG    (0x0006)    /* SYSUNIV   : Access Violation - ACCVIFG */
#define SYSUNIV_AUXSWGIFG  (0x0008)    /* AUXSWGIFG : AUX supply */

/* SYSSNIV Definitions */
#define SYSSNIV_NONE       (0x0000)    /* No Interrupt pending */
#define SYSSNIV_SVMLIFG    (0x0002)    /* SYSSNIV : SVMLIFG */
#define SYSSNIV_SVMHIFG    (0x0004)    /* SYSSNIV : SVMHIFG */
#define SYSSNIV_DLYLIFG    (0x0006)    /* SYSSNIV : DLYLIFG */
#define SYSSNIV_DLYHIFG    (0x0008)    /* SYSSNIV : DLYHIFG */
#define SYSSNIV_VMAIFG     (0x000A)    /* SYSSNIV : VMAIFG */
#define SYSSNIV_JMBINIFG   (0x000C)    /* SYSSNIV : JMBINIFG */
#define SYSSNIV_JMBOUTIFG  (0x000E)    /* SYSSNIV : JMBOUTIFG */
#define SYSSNIV_VLRLIFG    (0x0010)    /* SYSSNIV : VLRLIFG */
#define SYSSNIV_VLRHIFG    (0x0012)    /* SYSSNIV : VLRHIFG */

/* SYSRSTIV Definitions */
#define SYSRSTIV_NONE      (0x0000)    /* No Interrupt pending */
#define SYSRSTIV_BOR       (0x0002)    /* SYSRSTIV : BOR */
#define SYSRSTIV_RSTNMI    (0x0004)    /* SYSRSTIV : RST/NMI */
#define SYSRSTIV_DOBOR     (0x0006)    /* SYSRSTIV : Do BOR */
#define SYSRSTIV_LPM5WU    (0x0008)    /* SYSRSTIV : Port LPM5 Wake Up */
#define SYSRSTIV_SECYV     (0x000A)    /* SYSRSTIV : Security violation */
#define SYSRSTIV_SVSL      (0x000C)    /* SYSRSTIV : SVSL */
#define SYSRSTIV_SVSH      (0x000E)    /* SYSRSTIV : SVSH */
#define SYSRSTIV_SVML_OVP  (0x0010)    /* SYSRSTIV : SVML_OVP */
#define SYSRSTIV_SVMH_OVP  (0x0012)    /* SYSRSTIV : SVMH_OVP */
#define SYSRSTIV_DOPOR     (0x0014)    /* SYSRSTIV : Do POR */
#define SYSRSTIV_WDTTO     (0x0016)    /* SYSRSTIV : WDT Time out */
#define SYSRSTIV_WDTKEY    (0x0018)    /* SYSRSTIV : WDTKEY violation */
#define SYSRSTIV_KEYV      (0x001A)    /* SYSRSTIV : Flash Key violation */
#define SYSRSTIV_FLLUL     (0x001C)    /* SYSRSTIV : FLL unlock */
#define SYSRSTIV_PERF      (0x001E)    /* SYSRSTIV : peripheral/config area fetch */
#define SYSRSTIV_PMMKEY    (0x0020)    /* SYSRSTIV : PMMKEY violation */

/************************************************************
* Timer0_A3
************************************************************/
#define __MSP430_HAS_T0A3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0A3__ 0x0340
#define TIMER_A0_BASE __MSP430_BASEADDRESS_T0A3__

#define TA0CTL_               0x0340    /* Timer0_A3 Control */
sfrw(TA0CTL, TA0CTL_);
#define TA0CCTL0_             0x0342    /* Timer0_A3 Capture/Compare Control 0 */
sfrw(TA0CCTL0, TA0CCTL0_);
#define TA0CCTL1_             0x0344    /* Timer0_A3 Capture/Compare Control 1 */
sfrw(TA0CCTL1, TA0CCTL1_);
#define TA0CCTL2_             0x0346    /* Timer0_A3 Capture/Compare Control 2 */
sfrw(TA0CCTL2, TA0CCTL2_);
#define TA0R_                 0x0350    /* Timer0_A3 */
sfrw(TA0R, TA0R_);
#define TA0CCR0_              0x0352    /* Timer0_A3 Capture/Compare 0 */
sfrw(TA0CCR0, TA0CCR0_);
#define TA0CCR1_              0x0354    /* Timer0_A3 Capture/Compare 1 */
sfrw(TA0CCR1, TA0CCR1_);
#define TA0CCR2_              0x0356    /* Timer0_A3 Capture/Compare 2 */
sfrw(TA0CCR2, TA0CCR2_);
#define TA0IV_                0x036E    /* Timer0_A3 Interrupt Vector Word */
sfrw(TA0IV, TA0IV_);
#define TA0EX0_               0x0360    /* Timer0_A3 Expansion Register 0 */
sfrw(TA0EX0, TA0EX0_);

/* TAxCTL Control Bits */
#define TASSEL1             (0x0200)  /* Timer A clock source select 1 */
#define TASSEL0             (0x0100)  /* Timer A clock source select 0 */
#define ID1                 (0x0080)  /* Timer A clock input divider 1 */
#define ID0                 (0x0040)  /* Timer A clock input divider 0 */
#define MC1                 (0x0020)  /* Timer A mode control 1 */
#define MC0                 (0x0010)  /* Timer A mode control 0 */
#define TACLR               (0x0004)  /* Timer A counter clear */
#define TAIE                (0x0002)  /* Timer A counter interrupt enable */
#define TAIFG               (0x0001)  /* Timer A counter interrupt flag */

#define MC_0                (0x0000)  /* Timer A mode control: 0 - Stop */
#define MC_1                (0x0010)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2                (0x0020)  /* Timer A mode control: 2 - Continuous up */
#define MC_3                (0x0030)  /* Timer A mode control: 3 - Up/Down */
#define ID_0                (0x0000)  /* Timer A input divider: 0 - /1 */
#define ID_1                (0x0040)  /* Timer A input divider: 1 - /2 */
#define ID_2                (0x0080)  /* Timer A input divider: 2 - /4 */
#define ID_3                (0x00C0)  /* Timer A input divider: 3 - /8 */
#define TASSEL_0            (0x0000) /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1            (0x0100) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2            (0x0200) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3            (0x0300) /* Timer A clock source select: 3 - INCLK */
#define MC__STOP            (0x0000)  /* Timer A mode control: 0 - Stop */
#define MC__UP              (0x0010)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS      (0x0020)  /* Timer A mode control: 2 - Continuous up */
#define MC__CONTINOUS       (0x0020)  /* Legacy define */
#define MC__UPDOWN          (0x0030)  /* Timer A mode control: 3 - Up/Down */
#define ID__1               (0x0000)  /* Timer A input divider: 0 - /1 */
#define ID__2               (0x0040)  /* Timer A input divider: 1 - /2 */
#define ID__4               (0x0080)  /* Timer A input divider: 2 - /4 */
#define ID__8               (0x00C0)  /* Timer A input divider: 3 - /8 */
#define TASSEL__TACLK       (0x0000) /* Timer A clock source select: 0 - TACLK */
#define TASSEL__ACLK        (0x0100) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL__SMCLK       (0x0200) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL__INCLK       (0x0300) /* Timer A clock source select: 3 - INCLK */

/* TAxCCTLx Control Bits */
#define CM1                 (0x8000)  /* Capture mode 1 */
#define CM0                 (0x4000)  /* Capture mode 0 */
#define CCIS1               (0x2000)  /* Capture input select 1 */
#define CCIS0               (0x1000)  /* Capture input select 0 */
#define SCS                 (0x0800)  /* Capture sychronize */
#define SCCI                (0x0400)  /* Latched capture signal (read) */
#define CAP                 (0x0100)  /* Capture mode: 1 /Compare mode : 0 */
#define OUTMOD2             (0x0080)  /* Output mode 2 */
#define OUTMOD1             (0x0040)  /* Output mode 1 */
#define OUTMOD0             (0x0020)  /* Output mode 0 */
#define CCIE                (0x0010)  /* Capture/compare interrupt enable */
#define CCI                 (0x0008)  /* Capture input signal (read) */
#define OUT                 (0x0004)  /* PWM Output signal if output mode 0 */
#define COV                 (0x0002)  /* Capture/compare overflow flag */
#define CCIFG               (0x0001)  /* Capture/compare interrupt flag */

#define OUTMOD_0            (0x0000)  /* PWM output mode: 0 - output only */
#define OUTMOD_1            (0x0020)  /* PWM output mode: 1 - set */
#define OUTMOD_2            (0x0040)  /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3            (0x0060)  /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4            (0x0080)  /* PWM output mode: 4 - toggle */
#define OUTMOD_5            (0x00A0)  /* PWM output mode: 5 - Reset */
#define OUTMOD_6            (0x00C0)  /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7            (0x00E0)  /* PWM output mode: 7 - PWM reset/set */
#define CCIS_0              (0x0000) /* Capture input select: 0 - CCIxA */
#define CCIS_1              (0x1000) /* Capture input select: 1 - CCIxB */
#define CCIS_2              (0x2000) /* Capture input select: 2 - GND */
#define CCIS_3              (0x3000) /* Capture input select: 3 - Vcc */
#define CM_0                (0x0000) /* Capture mode: 0 - disabled */
#define CM_1                (0x4000) /* Capture mode: 1 - pos. edge */
#define CM_2                (0x8000) /* Capture mode: 1 - neg. edge */
#define CM_3                (0xC000) /* Capture mode: 1 - both edges */


/* TAxEX0 Control Bits */
#define TAIDEX0             (0x0001)  /* Timer A Input divider expansion Bit: 0 */
#define TAIDEX1             (0x0002)  /* Timer A Input divider expansion Bit: 1 */
#define TAIDEX2             (0x0004)  /* Timer A Input divider expansion Bit: 2 */

#define TAIDEX_0            (0x0000) /* Timer A Input divider expansion : /1 */
#define TAIDEX_1            (0x0001) /* Timer A Input divider expansion : /2 */
#define TAIDEX_2            (0x0002) /* Timer A Input divider expansion : /3 */
#define TAIDEX_3            (0x0003) /* Timer A Input divider expansion : /4 */
#define TAIDEX_4            (0x0004) /* Timer A Input divider expansion : /5 */
#define TAIDEX_5            (0x0005) /* Timer A Input divider expansion : /6 */
#define TAIDEX_6            (0x0006) /* Timer A Input divider expansion : /7 */
#define TAIDEX_7            (0x0007) /* Timer A Input divider expansion : /8 */

/* T0A3IV Definitions */
#define TA0IV_NONE          (0x0000)    /* No Interrupt pending */
#define TA0IV_TACCR1        (0x0002)    /* TA0CCR1_CCIFG */
#define TA0IV_TACCR2        (0x0004)    /* TA0CCR2_CCIFG */
#define TA0IV_3             (0x0006)    /* Reserved */
#define TA0IV_4             (0x0008)    /* Reserved */
#define TA0IV_5             (0x000A)    /* Reserved */
#define TA0IV_6             (0x000C)    /* Reserved */
#define TA0IV_TAIFG         (0x000E)    /* TA0IFG */

/* Legacy Defines */
#define TA0IV_TA0CCR1       (0x0002)    /* TA0CCR1_CCIFG */
#define TA0IV_TA0CCR2       (0x0004)    /* TA0CCR2_CCIFG */
#define TA0IV_TA0IFG        (0x000E)    /* TA0IFG */

/************************************************************
* Timer1_A2
************************************************************/
#define __MSP430_HAS_T1A2__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T1A2__ 0x0380
#define TIMER_A1_BASE __MSP430_BASEADDRESS_T1A2__

#define TA1CTL_               0x0380    /* Timer1_A2 Control */
sfrw(TA1CTL, TA1CTL_);
#define TA1CCTL0_             0x0382    /* Timer1_A2 Capture/Compare Control 0 */
sfrw(TA1CCTL0, TA1CCTL0_);
#define TA1CCTL1_             0x0384    /* Timer1_A2 Capture/Compare Control 1 */
sfrw(TA1CCTL1, TA1CCTL1_);
#define TA1R_                 0x0390    /* Timer1_A2 */
sfrw(TA1R, TA1R_);
#define TA1CCR0_              0x0392    /* Timer1_A2 Capture/Compare 0 */
sfrw(TA1CCR0, TA1CCR0_);
#define TA1CCR1_              0x0394    /* Timer1_A2 Capture/Compare 1 */
sfrw(TA1CCR1, TA1CCR1_);
#define TA1IV_                0x03AE    /* Timer1_A2 Interrupt Vector Word */
sfrw(TA1IV, TA1IV_);
#define TA1EX0_               0x03A0    /* Timer1_A2 Expansion Register 0 */
sfrw(TA1EX0, TA1EX0_);

/* Bits are already defined within the Timer0_Ax */

/* TA1IV Definitions */
#define TA1IV_NONE          (0x0000)    /* No Interrupt pending */
#define TA1IV_TACCR1        (0x0002)    /* TA1CCR1_CCIFG */
#define TA1IV_3             (0x0006)    /* Reserved */
#define TA1IV_4             (0x0008)    /* Reserved */
#define TA1IV_5             (0x000A)    /* Reserved */
#define TA1IV_6             (0x000C)    /* Reserved */
#define TA1IV_TAIFG         (0x000E)    /* TA1IFG */

/* Legacy Defines */
#define TA1IV_TA1CCR1      (0x0002)    /* TA1CCR1_CCIFG */
#define TA1IV_TA1IFG       (0x000E)    /* TA1IFG */

/************************************************************
* Timer2_A2
************************************************************/
#define __MSP430_HAS_T2A2__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T2A2__ 0x0400
#define TIMER_A2_BASE __MSP430_BASEADDRESS_T2A2__

#define TA2CTL_               0x0400    /* Timer2_A2 Control */
sfrw(TA2CTL, TA2CTL_);
#define TA2CCTL0_             0x0402    /* Timer2_A2 Capture/Compare Control 0 */
sfrw(TA2CCTL0, TA2CCTL0_);
#define TA2CCTL1_             0x0404    /* Timer2_A2 Capture/Compare Control 1 */
sfrw(TA2CCTL1, TA2CCTL1_);
#define TA2R_                 0x0410    /* Timer2_A2 */
sfrw(TA2R, TA2R_);
#define TA2CCR0_              0x0412    /* Timer2_A2 Capture/Compare 0 */
sfrw(TA2CCR0, TA2CCR0_);
#define TA2CCR1_              0x0414    /* Timer2_A2 Capture/Compare 1 */
sfrw(TA2CCR1, TA2CCR1_);
#define TA2IV_                0x042E    /* Timer2_A2 Interrupt Vector Word */
sfrw(TA2IV, TA2IV_);
#define TA2EX0_               0x0420    /* Timer2_A2 Expansion Register 0 */
sfrw(TA2EX0, TA2EX0_);

/* Bits are already defined within the Timer0_Ax */

/* TA2IV Definitions */
#define TA2IV_NONE          (0x0000)    /* No Interrupt pending */
#define TA2IV_TACCR1        (0x0002)    /* TA2CCR1_CCIFG */
#define TA2IV_3             (0x0006)    /* Reserved */
#define TA2IV_4             (0x0008)    /* Reserved */
#define TA2IV_5             (0x000A)    /* Reserved */
#define TA2IV_6             (0x000C)    /* Reserved */
#define TA2IV_TAIFG         (0x000E)    /* TA2IFG */

/* Legacy Defines */
#define TA2IV_TA2CCR1      (0x0002)    /* TA2CCR1_CCIFG */
#define TA2IV_TA2IFG       (0x000E)    /* TA2IFG */

/************************************************************
* Timer3_A2
************************************************************/
#define __MSP430_HAS_T3A2__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T3A2__ 0x0440
#define TIMER_A3_BASE __MSP430_BASEADDRESS_T3A2__

#define TA3CTL_               0x0440    /* Timer3_A2 Control */
sfrw(TA3CTL, TA3CTL_);
#define TA3CCTL0_             0x0442    /* Timer3_A2 Capture/Compare Control 0 */
sfrw(TA3CCTL0, TA3CCTL0_);
#define TA3CCTL1_             0x0444    /* Timer3_A2 Capture/Compare Control 1 */
sfrw(TA3CCTL1, TA3CCTL1_);
#define TA3R_                 0x0450    /* Timer3_A2 */
sfrw(TA3R, TA3R_);
#define TA3CCR0_              0x0452    /* Timer3_A2 Capture/Compare 0 */
sfrw(TA3CCR0, TA3CCR0_);
#define TA3CCR1_              0x0454    /* Timer3_A2 Capture/Compare 1 */
sfrw(TA3CCR1, TA3CCR1_);
#define TA3IV_                0x046E    /* Timer3_A2 Interrupt Vector Word */
sfrw(TA3IV, TA3IV_);
#define TA3EX0_               0x0460    /* Timer3_A2 Expansion Register 0 */
sfrw(TA3EX0, TA3EX0_);

/* Bits are already defined within the Timer0_Ax */

/* TA3IV Definitions */
#define TA3IV_NONE          (0x0000)    /* No Interrupt pending */
#define TA3IV_TACCR1        (0x0002)    /* TA3CCR1_CCIFG */
#define TA3IV_3             (0x0006)    /* Reserved */
#define TA3IV_4             (0x0008)    /* Reserved */
#define TA3IV_5             (0x000A)    /* Reserved */
#define TA3IV_6             (0x000C)    /* Reserved */
#define TA3IV_TAIFG         (0x000E)    /* TA3IFG */

/* Legacy Defines */
#define TA3IV_TA3CCR1      (0x0002)    /* TA3CCR1_CCIFG */
#define TA3IV_TA3IFG       (0x000E)    /* TA3IFG */

/************************************************************
* UNIFIED CLOCK SYSTEM
************************************************************/
#define __MSP430_HAS_UCS__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_UCS__ 0x0160
#define UCS_BASE __MSP430_BASEADDRESS_UCS__

#define UCSCTL0_              0x0160    /* UCS Control Register 0 */
sfrb(UCSCTL0_L , UCSCTL0_);
sfrb(UCSCTL0_H , UCSCTL0_+1);
sfrw(UCSCTL0, UCSCTL0_);
#define UCSCTL1_              0x0162    /* UCS Control Register 1 */
sfrb(UCSCTL1_L , UCSCTL1_);
sfrb(UCSCTL1_H , UCSCTL1_+1);
sfrw(UCSCTL1, UCSCTL1_);
#define UCSCTL2_              0x0164    /* UCS Control Register 2 */
sfrb(UCSCTL2_L , UCSCTL2_);
sfrb(UCSCTL2_H , UCSCTL2_+1);
sfrw(UCSCTL2, UCSCTL2_);
#define UCSCTL3_              0x0166    /* UCS Control Register 3 */
sfrb(UCSCTL3_L , UCSCTL3_);
sfrb(UCSCTL3_H , UCSCTL3_+1);
sfrw(UCSCTL3, UCSCTL3_);
#define UCSCTL4_              0x0168    /* UCS Control Register 4 */
sfrb(UCSCTL4_L , UCSCTL4_);
sfrb(UCSCTL4_H , UCSCTL4_+1);
sfrw(UCSCTL4, UCSCTL4_);
#define UCSCTL5_              0x016A    /* UCS Control Register 5 */
sfrb(UCSCTL5_L , UCSCTL5_);
sfrb(UCSCTL5_H , UCSCTL5_+1);
sfrw(UCSCTL5, UCSCTL5_);
#define UCSCTL6_              0x016C    /* UCS Control Register 6 */
sfrb(UCSCTL6_L , UCSCTL6_);
sfrb(UCSCTL6_H , UCSCTL6_+1);
sfrw(UCSCTL6, UCSCTL6_);
#define UCSCTL7_              0x016E    /* UCS Control Register 7 */
sfrb(UCSCTL7_L , UCSCTL7_);
sfrb(UCSCTL7_H , UCSCTL7_+1);
sfrw(UCSCTL7, UCSCTL7_);
#define UCSCTL8_              0x0170    /* UCS Control Register 8 */
sfrb(UCSCTL8_L , UCSCTL8_);
sfrb(UCSCTL8_H , UCSCTL8_+1);
sfrw(UCSCTL8, UCSCTL8_);

/* UCSCTL0 Control Bits */
//#define RESERVED            (0x0001)    /* RESERVED */
//#define RESERVED            (0x0002)    /* RESERVED */
//#define RESERVED            (0x0004)    /* RESERVED */
#define MOD0                (0x0008)    /* Modulation Bit Counter Bit : 0 */
#define MOD1                (0x0010)    /* Modulation Bit Counter Bit : 1 */
#define MOD2                (0x0020)    /* Modulation Bit Counter Bit : 2 */
#define MOD3                (0x0040)    /* Modulation Bit Counter Bit : 3 */
#define MOD4                (0x0080)    /* Modulation Bit Counter Bit : 4 */
#define DCO0                (0x0100)    /* DCO TAP Bit : 0 */
#define DCO1                (0x0200)    /* DCO TAP Bit : 1 */
#define DCO2                (0x0400)    /* DCO TAP Bit : 2 */
#define DCO3                (0x0800)    /* DCO TAP Bit : 3 */
#define DCO4                (0x1000)    /* DCO TAP Bit : 4 */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL0 Control Bits */
//#define RESERVED            (0x0001)    /* RESERVED */
//#define RESERVED            (0x0002)    /* RESERVED */
//#define RESERVED            (0x0004)    /* RESERVED */
#define MOD0_L              (0x0008)    /* Modulation Bit Counter Bit : 0 */
#define MOD1_L              (0x0010)    /* Modulation Bit Counter Bit : 1 */
#define MOD2_L              (0x0020)    /* Modulation Bit Counter Bit : 2 */
#define MOD3_L              (0x0040)    /* Modulation Bit Counter Bit : 3 */
#define MOD4_L              (0x0080)    /* Modulation Bit Counter Bit : 4 */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL0 Control Bits */
//#define RESERVED            (0x0001)    /* RESERVED */
//#define RESERVED            (0x0002)    /* RESERVED */
//#define RESERVED            (0x0004)    /* RESERVED */
#define DCO0_H              (0x0001)    /* DCO TAP Bit : 0 */
#define DCO1_H              (0x0002)    /* DCO TAP Bit : 1 */
#define DCO2_H              (0x0004)    /* DCO TAP Bit : 2 */
#define DCO3_H              (0x0008)    /* DCO TAP Bit : 3 */
#define DCO4_H              (0x0010)    /* DCO TAP Bit : 4 */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL1 Control Bits */
#define DISMOD              (0x0001)    /* Disable Modulation */
//#define RESERVED            (0x0002)    /* RESERVED */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DCORSEL0            (0x0010)    /* DCO Freq. Range Select Bit : 0 */
#define DCORSEL1            (0x0020)    /* DCO Freq. Range Select Bit : 1 */
#define DCORSEL2            (0x0040)    /* DCO Freq. Range Select Bit : 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL1 Control Bits */
#define DISMOD_L            (0x0001)    /* Disable Modulation */
//#define RESERVED            (0x0002)    /* RESERVED */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DCORSEL0_L          (0x0010)    /* DCO Freq. Range Select Bit : 0 */
#define DCORSEL1_L          (0x0020)    /* DCO Freq. Range Select Bit : 1 */
#define DCORSEL2_L          (0x0040)    /* DCO Freq. Range Select Bit : 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */


#define DCORSEL_0           (0x0000)    /* DCO RSEL 0 */
#define DCORSEL_1           (0x0010)    /* DCO RSEL 1 */
#define DCORSEL_2           (0x0020)    /* DCO RSEL 2 */
#define DCORSEL_3           (0x0030)    /* DCO RSEL 3 */
#define DCORSEL_4           (0x0040)    /* DCO RSEL 4 */
#define DCORSEL_5           (0x0050)    /* DCO RSEL 5 */
#define DCORSEL_6           (0x0060)    /* DCO RSEL 6 */
#define DCORSEL_7           (0x0070)    /* DCO RSEL 7 */


/* UCSCTL2 Control Bits */
#define FLLN0               (0x0001)    /* FLL Multipier Bit : 0 */
#define FLLN1               (0x0002)    /* FLL Multipier Bit : 1 */
#define FLLN2               (0x0004)    /* FLL Multipier Bit : 2 */
#define FLLN3               (0x0008)    /* FLL Multipier Bit : 3 */
#define FLLN4               (0x0010)    /* FLL Multipier Bit : 4 */
#define FLLN5               (0x0020)    /* FLL Multipier Bit : 5 */
#define FLLN6               (0x0040)    /* FLL Multipier Bit : 6 */
#define FLLN7               (0x0080)    /* FLL Multipier Bit : 7 */
#define FLLN8               (0x0100)    /* FLL Multipier Bit : 8 */
#define FLLN9               (0x0200)    /* FLL Multipier Bit : 9 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
#define FLLD0               (0x1000)    /* Loop Divider Bit : 0 */
#define FLLD1               (0x2000)    /* Loop Divider Bit : 1 */
#define FLLD2               (0x4000)    /* Loop Divider Bit : 1 */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL2 Control Bits */
#define FLLN0_L             (0x0001)    /* FLL Multipier Bit : 0 */
#define FLLN1_L             (0x0002)    /* FLL Multipier Bit : 1 */
#define FLLN2_L             (0x0004)    /* FLL Multipier Bit : 2 */
#define FLLN3_L             (0x0008)    /* FLL Multipier Bit : 3 */
#define FLLN4_L             (0x0010)    /* FLL Multipier Bit : 4 */
#define FLLN5_L             (0x0020)    /* FLL Multipier Bit : 5 */
#define FLLN6_L             (0x0040)    /* FLL Multipier Bit : 6 */
#define FLLN7_L             (0x0080)    /* FLL Multipier Bit : 7 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL2 Control Bits */
#define FLLN8_H             (0x0001)    /* FLL Multipier Bit : 8 */
#define FLLN9_H             (0x0002)    /* FLL Multipier Bit : 9 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
#define FLLD0_H             (0x0010)    /* Loop Divider Bit : 0 */
#define FLLD1_H             (0x0020)    /* Loop Divider Bit : 1 */
#define FLLD2_H             (0x0040)    /* Loop Divider Bit : 1 */
//#define RESERVED            (0x8000)    /* RESERVED */

#define FLLD_0             (0x0000)    /* Multiply Selected Loop Freq. 1 */
#define FLLD_1             (0x1000)    /* Multiply Selected Loop Freq. 2 */
#define FLLD_2             (0x2000)    /* Multiply Selected Loop Freq. 4 */
#define FLLD_3             (0x3000)    /* Multiply Selected Loop Freq. 8 */
#define FLLD_4             (0x4000)    /* Multiply Selected Loop Freq. 16 */
#define FLLD_5             (0x5000)    /* Multiply Selected Loop Freq. 32 */
#define FLLD_6             (0x6000)    /* Multiply Selected Loop Freq. 32 */
#define FLLD_7             (0x7000)    /* Multiply Selected Loop Freq. 32 */
#define FLLD__1            (0x0000)    /* Multiply Selected Loop Freq. By 1 */
#define FLLD__2            (0x1000)    /* Multiply Selected Loop Freq. By 2 */
#define FLLD__4            (0x2000)    /* Multiply Selected Loop Freq. By 4 */
#define FLLD__8            (0x3000)    /* Multiply Selected Loop Freq. By 8 */
#define FLLD__16           (0x4000)    /* Multiply Selected Loop Freq. By 16 */
#define FLLD__32           (0x5000)    /* Multiply Selected Loop Freq. By 32 */


/* UCSCTL3 Control Bits */
#define FLLREFDIV0          (0x0001)    /* Reference Divider Bit : 0 */
#define FLLREFDIV1          (0x0002)    /* Reference Divider Bit : 1 */
#define FLLREFDIV2          (0x0004)    /* Reference Divider Bit : 2 */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELREF0             (0x0010)    /* FLL Reference Clock Select Bit : 0 */
#define SELREF1             (0x0020)    /* FLL Reference Clock Select Bit : 1 */
#define SELREF2             (0x0040)    /* FLL Reference Clock Select Bit : 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL3 Control Bits */
#define FLLREFDIV0_L        (0x0001)    /* Reference Divider Bit : 0 */
#define FLLREFDIV1_L        (0x0002)    /* Reference Divider Bit : 1 */
#define FLLREFDIV2_L        (0x0004)    /* Reference Divider Bit : 2 */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELREF0_L           (0x0010)    /* FLL Reference Clock Select Bit : 0 */
#define SELREF1_L           (0x0020)    /* FLL Reference Clock Select Bit : 1 */
#define SELREF2_L           (0x0040)    /* FLL Reference Clock Select Bit : 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */


#define FLLREFDIV_0         (0x0000)    /* Reference Divider: f(LFCLK)/1 */
#define FLLREFDIV_1         (0x0001)    /* Reference Divider: f(LFCLK)/2 */
#define FLLREFDIV_2         (0x0002)    /* Reference Divider: f(LFCLK)/4 */
#define FLLREFDIV_3         (0x0003)    /* Reference Divider: f(LFCLK)/8 */
#define FLLREFDIV_4         (0x0004)    /* Reference Divider: f(LFCLK)/12 */
#define FLLREFDIV_5         (0x0005)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV_6         (0x0006)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV_7         (0x0007)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV__1        (0x0000)    /* Reference Divider: f(LFCLK)/1 */
#define FLLREFDIV__2        (0x0001)    /* Reference Divider: f(LFCLK)/2 */
#define FLLREFDIV__4        (0x0002)    /* Reference Divider: f(LFCLK)/4 */
#define FLLREFDIV__8        (0x0003)    /* Reference Divider: f(LFCLK)/8 */
#define FLLREFDIV__12       (0x0004)    /* Reference Divider: f(LFCLK)/12 */
#define FLLREFDIV__16       (0x0005)    /* Reference Divider: f(LFCLK)/16 */
#define SELREF_0            (0x0000)    /* FLL Reference Clock Select 0 */
#define SELREF_1            (0x0010)    /* FLL Reference Clock Select 1 */
#define SELREF_2            (0x0020)    /* FLL Reference Clock Select 2 */
#define SELREF_3            (0x0030)    /* FLL Reference Clock Select 3 */
#define SELREF_4            (0x0040)    /* FLL Reference Clock Select 4 */
#define SELREF_5            (0x0050)    /* FLL Reference Clock Select 5 */
#define SELREF_6            (0x0060)    /* FLL Reference Clock Select 6 */
#define SELREF_7            (0x0070)    /* FLL Reference Clock Select 7 */
#define SELREF__XT1CLK      (0x0000)    /* Multiply Selected Loop Freq. By XT1CLK */
#define SELREF__REFOCLK     (0x0020)    /* Multiply Selected Loop Freq. By REFOCLK */
#define SELREF__XT2CLK      (0x0050)    /* Multiply Selected Loop Freq. By XT2CLK */

/* UCSCTL4 Control Bits */
#define SELM0               (0x0001)   /* MCLK Source Select Bit: 0 */
#define SELM1               (0x0002)   /* MCLK Source Select Bit: 1 */
#define SELM2               (0x0004)   /* MCLK Source Select Bit: 2 */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELS0               (0x0010)   /* SMCLK Source Select Bit: 0 */
#define SELS1               (0x0020)   /* SMCLK Source Select Bit: 1 */
#define SELS2               (0x0040)   /* SMCLK Source Select Bit: 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
#define SELA0               (0x0100)   /* ACLK Source Select Bit: 0 */
#define SELA1               (0x0200)   /* ACLK Source Select Bit: 1 */
#define SELA2               (0x0400)   /* ACLK Source Select Bit: 2 */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL4 Control Bits */
#define SELM0_L             (0x0001)   /* MCLK Source Select Bit: 0 */
#define SELM1_L             (0x0002)   /* MCLK Source Select Bit: 1 */
#define SELM2_L             (0x0004)   /* MCLK Source Select Bit: 2 */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELS0_L             (0x0010)   /* SMCLK Source Select Bit: 0 */
#define SELS1_L             (0x0020)   /* SMCLK Source Select Bit: 1 */
#define SELS2_L             (0x0040)   /* SMCLK Source Select Bit: 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL4 Control Bits */
//#define RESERVED            (0x0008)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define SELA0_H             (0x0001)   /* ACLK Source Select Bit: 0 */
#define SELA1_H             (0x0002)   /* ACLK Source Select Bit: 1 */
#define SELA2_H             (0x0004)   /* ACLK Source Select Bit: 2 */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

#define SELM_0              (0x0000)   /* MCLK Source Select 0 */
#define SELM_1              (0x0001)   /* MCLK Source Select 1 */
#define SELM_2              (0x0002)   /* MCLK Source Select 2 */
#define SELM_3              (0x0003)   /* MCLK Source Select 3 */
#define SELM_4              (0x0004)   /* MCLK Source Select 4 */
#define SELM_5              (0x0005)   /* MCLK Source Select 5 */
#define SELM_6              (0x0006)   /* MCLK Source Select 6 */
#define SELM_7              (0x0007)   /* MCLK Source Select 7 */
#define SELM__XT1CLK        (0x0000)   /* MCLK Source Select XT1CLK */
#define SELM__VLOCLK        (0x0001)   /* MCLK Source Select VLOCLK */
#define SELM__REFOCLK       (0x0002)   /* MCLK Source Select REFOCLK */
#define SELM__DCOCLK        (0x0003)   /* MCLK Source Select DCOCLK */
#define SELM__DCOCLKDIV     (0x0004)   /* MCLK Source Select DCOCLKDIV */
#define SELM__XT2CLK        (0x0005)   /* MCLK Source Select XT2CLK */

#define SELS_0              (0x0000)   /* SMCLK Source Select 0 */
#define SELS_1              (0x0010)   /* SMCLK Source Select 1 */
#define SELS_2              (0x0020)   /* SMCLK Source Select 2 */
#define SELS_3              (0x0030)   /* SMCLK Source Select 3 */
#define SELS_4              (0x0040)   /* SMCLK Source Select 4 */
#define SELS_5              (0x0050)   /* SMCLK Source Select 5 */
#define SELS_6              (0x0060)   /* SMCLK Source Select 6 */
#define SELS_7              (0x0070)   /* SMCLK Source Select 7 */
#define SELS__XT1CLK        (0x0000)   /* SMCLK Source Select XT1CLK */
#define SELS__VLOCLK        (0x0010)   /* SMCLK Source Select VLOCLK */
#define SELS__REFOCLK       (0x0020)   /* SMCLK Source Select REFOCLK */
#define SELS__DCOCLK        (0x0030)   /* SMCLK Source Select DCOCLK */
#define SELS__DCOCLKDIV     (0x0040)   /* SMCLK Source Select DCOCLKDIV */
#define SELS__XT2CLK        (0x0050)   /* SMCLK Source Select XT2CLK */

#define SELA_0              (0x0000)   /* ACLK Source Select 0 */
#define SELA_1              (0x0100)   /* ACLK Source Select 1 */
#define SELA_2              (0x0200)   /* ACLK Source Select 2 */
#define SELA_3              (0x0300)   /* ACLK Source Select 3 */
#define SELA_4              (0x0400)   /* ACLK Source Select 4 */
#define SELA_5              (0x0500)   /* ACLK Source Select 5 */
#define SELA_6              (0x0600)   /* ACLK Source Select 6 */
#define SELA_7              (0x0700)   /* ACLK Source Select 7 */
#define SELA__XT1CLK        (0x0000)   /* ACLK Source Select XT1CLK */
#define SELA__VLOCLK        (0x0100)   /* ACLK Source Select VLOCLK */
#define SELA__REFOCLK       (0x0200)   /* ACLK Source Select REFOCLK */
#define SELA__DCOCLK        (0x0300)   /* ACLK Source Select DCOCLK */
#define SELA__DCOCLKDIV     (0x0400)   /* ACLK Source Select DCOCLKDIV */
#define SELA__XT2CLK        (0x0500)   /* ACLK Source Select XT2CLK */

/* UCSCTL5 Control Bits */
#define DIVM0               (0x0001)   /* MCLK Divider Bit: 0 */
#define DIVM1               (0x0002)   /* MCLK Divider Bit: 1 */
#define DIVM2               (0x0004)   /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DIVS0               (0x0010)   /* SMCLK Divider Bit: 0 */
#define DIVS1               (0x0020)   /* SMCLK Divider Bit: 1 */
#define DIVS2               (0x0040)   /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
#define DIVA0               (0x0100)   /* ACLK Divider Bit: 0 */
#define DIVA1               (0x0200)   /* ACLK Divider Bit: 1 */
#define DIVA2               (0x0400)   /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0800)    /* RESERVED */
#define DIVPA0              (0x1000)   /* ACLK from Pin Divider Bit: 0 */
#define DIVPA1              (0x2000)   /* ACLK from Pin Divider Bit: 1 */
#define DIVPA2              (0x4000)   /* ACLK from Pin Divider Bit: 2 */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL5 Control Bits */
#define DIVM0_L             (0x0001)   /* MCLK Divider Bit: 0 */
#define DIVM1_L             (0x0002)   /* MCLK Divider Bit: 1 */
#define DIVM2_L             (0x0004)   /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DIVS0_L             (0x0010)   /* SMCLK Divider Bit: 0 */
#define DIVS1_L             (0x0020)   /* SMCLK Divider Bit: 1 */
#define DIVS2_L             (0x0040)   /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL5 Control Bits */
//#define RESERVED            (0x0008)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define DIVA0_H             (0x0001)   /* ACLK Divider Bit: 0 */
#define DIVA1_H             (0x0002)   /* ACLK Divider Bit: 1 */
#define DIVA2_H             (0x0004)   /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0800)    /* RESERVED */
#define DIVPA0_H            (0x0010)   /* ACLK from Pin Divider Bit: 0 */
#define DIVPA1_H            (0x0020)   /* ACLK from Pin Divider Bit: 1 */
#define DIVPA2_H            (0x0040)   /* ACLK from Pin Divider Bit: 2 */
//#define RESERVED            (0x8000)    /* RESERVED */

#define DIVM_0              (0x0000)    /* MCLK Source Divider 0 */
#define DIVM_1              (0x0001)    /* MCLK Source Divider 1 */
#define DIVM_2              (0x0002)    /* MCLK Source Divider 2 */
#define DIVM_3              (0x0003)    /* MCLK Source Divider 3 */
#define DIVM_4              (0x0004)    /* MCLK Source Divider 4 */
#define DIVM_5              (0x0005)    /* MCLK Source Divider 5 */
#define DIVM_6              (0x0006)    /* MCLK Source Divider 6 */
#define DIVM_7              (0x0007)    /* MCLK Source Divider 7 */
#define DIVM__1             (0x0000)    /* MCLK Source Divider f(MCLK)/1 */
#define DIVM__2             (0x0001)    /* MCLK Source Divider f(MCLK)/2 */
#define DIVM__4             (0x0002)    /* MCLK Source Divider f(MCLK)/4 */
#define DIVM__8             (0x0003)    /* MCLK Source Divider f(MCLK)/8 */
#define DIVM__16            (0x0004)    /* MCLK Source Divider f(MCLK)/16 */
#define DIVM__32            (0x0005)    /* MCLK Source Divider f(MCLK)/32 */

#define DIVS_0              (0x0000)    /* SMCLK Source Divider 0 */
#define DIVS_1              (0x0010)    /* SMCLK Source Divider 1 */
#define DIVS_2              (0x0020)    /* SMCLK Source Divider 2 */
#define DIVS_3              (0x0030)    /* SMCLK Source Divider 3 */
#define DIVS_4              (0x0040)    /* SMCLK Source Divider 4 */
#define DIVS_5              (0x0050)    /* SMCLK Source Divider 5 */
#define DIVS_6              (0x0060)    /* SMCLK Source Divider 6 */
#define DIVS_7              (0x0070)    /* SMCLK Source Divider 7 */
#define DIVS__1             (0x0000)    /* SMCLK Source Divider f(SMCLK)/1 */
#define DIVS__2             (0x0010)    /* SMCLK Source Divider f(SMCLK)/2 */
#define DIVS__4             (0x0020)    /* SMCLK Source Divider f(SMCLK)/4 */
#define DIVS__8             (0x0030)    /* SMCLK Source Divider f(SMCLK)/8 */
#define DIVS__16            (0x0040)    /* SMCLK Source Divider f(SMCLK)/16 */
#define DIVS__32            (0x0050)    /* SMCLK Source Divider f(SMCLK)/32 */

#define DIVA_0              (0x0000)    /* ACLK Source Divider 0 */
#define DIVA_1              (0x0100)    /* ACLK Source Divider 1 */
#define DIVA_2              (0x0200)    /* ACLK Source Divider 2 */
#define DIVA_3              (0x0300)    /* ACLK Source Divider 3 */
#define DIVA_4              (0x0400)    /* ACLK Source Divider 4 */
#define DIVA_5              (0x0500)    /* ACLK Source Divider 5 */
#define DIVA_6              (0x0600)    /* ACLK Source Divider 6 */
#define DIVA_7              (0x0700)    /* ACLK Source Divider 7 */
#define DIVA__1             (0x0000)    /* ACLK Source Divider f(ACLK)/1 */
#define DIVA__2             (0x0100)    /* ACLK Source Divider f(ACLK)/2 */
#define DIVA__4             (0x0200)    /* ACLK Source Divider f(ACLK)/4 */
#define DIVA__8             (0x0300)    /* ACLK Source Divider f(ACLK)/8 */
#define DIVA__16            (0x0400)    /* ACLK Source Divider f(ACLK)/16 */
#define DIVA__32            (0x0500)    /* ACLK Source Divider f(ACLK)/32 */

#define DIVPA_0             (0x0000)    /* ACLK from Pin Source Divider 0 */
#define DIVPA_1             (0x1000)    /* ACLK from Pin Source Divider 1 */
#define DIVPA_2             (0x2000)    /* ACLK from Pin Source Divider 2 */
#define DIVPA_3             (0x3000)    /* ACLK from Pin Source Divider 3 */
#define DIVPA_4             (0x4000)    /* ACLK from Pin Source Divider 4 */
#define DIVPA_5             (0x5000)    /* ACLK from Pin Source Divider 5 */
#define DIVPA_6             (0x6000)    /* ACLK from Pin Source Divider 6 */
#define DIVPA_7             (0x7000)    /* ACLK from Pin Source Divider 7 */
#define DIVPA__1            (0x0000)    /* ACLK from Pin Source Divider f(ACLK)/1 */
#define DIVPA__2            (0x1000)    /* ACLK from Pin Source Divider f(ACLK)/2 */
#define DIVPA__4            (0x2000)    /* ACLK from Pin Source Divider f(ACLK)/4 */
#define DIVPA__8            (0x3000)    /* ACLK from Pin Source Divider f(ACLK)/8 */
#define DIVPA__16           (0x4000)    /* ACLK from Pin Source Divider f(ACLK)/16 */
#define DIVPA__32           (0x5000)    /* ACLK from Pin Source Divider f(ACLK)/32 */


/* UCSCTL6 Control Bits */
#define XT1OFF              (0x0001)    /* High Frequency Oscillator 1 (XT1) disable */
#define SMCLKOFF            (0x0002)    /* SMCLK Off */
#define XCAP0               (0x0004)   /* XIN/XOUT Cap Bit: 0 */
#define XCAP1               (0x0008)   /* XIN/XOUT Cap Bit: 1 */
#define XT1BYPASS           (0x0010)    /* XT1 bypass mode : 0: internal 1:sourced from external pin */
#define XTS                 (0x0020)   /* 1: Selects high-freq. oscillator */
#define XT1DRIVE0           (0x0040)    /* XT1 Drive Level mode Bit 0 */
#define XT1DRIVE1           (0x0080)    /* XT1 Drive Level mode Bit 1 */
#define XT2OFF              (0x0100)    /* High Frequency Oscillator 2 (XT2) disable */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
#define XT2BYPASS           (0x1000)    /* XT2 bypass mode : 0: internal 1:sourced from external pin */
//#define RESERVED            (0x2000)    /* RESERVED */
#define XT2DRIVE0           (0x4000)    /* XT2 Drive Level mode Bit 0 */
#define XT2DRIVE1           (0x8000)    /* XT2 Drive Level mode Bit 1 */

/* UCSCTL6 Control Bits */
#define XT1OFF_L            (0x0001)    /* High Frequency Oscillator 1 (XT1) disable */
#define SMCLKOFF_L          (0x0002)    /* SMCLK Off */
#define XCAP0_L             (0x0004)   /* XIN/XOUT Cap Bit: 0 */
#define XCAP1_L             (0x0008)   /* XIN/XOUT Cap Bit: 1 */
#define XT1BYPASS_L         (0x0010)    /* XT1 bypass mode : 0: internal 1:sourced from external pin */
#define XTS_L               (0x0020)   /* 1: Selects high-freq. oscillator */
#define XT1DRIVE0_L         (0x0040)    /* XT1 Drive Level mode Bit 0 */
#define XT1DRIVE1_L         (0x0080)    /* XT1 Drive Level mode Bit 1 */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */

/* UCSCTL6 Control Bits */
#define XT2OFF_H            (0x0001)    /* High Frequency Oscillator 2 (XT2) disable */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
#define XT2BYPASS_H         (0x0010)    /* XT2 bypass mode : 0: internal 1:sourced from external pin */
//#define RESERVED            (0x2000)    /* RESERVED */
#define XT2DRIVE0_H         (0x0040)    /* XT2 Drive Level mode Bit 0 */
#define XT2DRIVE1_H         (0x0080)    /* XT2 Drive Level mode Bit 1 */

#define XCAP_0              (0x0000)    /* XIN/XOUT Cap 0 */
#define XCAP_1              (0x0004)    /* XIN/XOUT Cap 1 */
#define XCAP_2              (0x0008)    /* XIN/XOUT Cap 2 */
#define XCAP_3              (0x000C)    /* XIN/XOUT Cap 3 */
#define XT1DRIVE_0          (0x0000)    /* XT1 Drive Level mode: 0 */
#define XT1DRIVE_1          (0x0040)    /* XT1 Drive Level mode: 1 */
#define XT1DRIVE_2          (0x0080)    /* XT1 Drive Level mode: 2 */
#define XT1DRIVE_3          (0x00C0)    /* XT1 Drive Level mode: 3 */
#define XT2DRIVE_0          (0x0000)    /* XT2 Drive Level mode: 0 */
#define XT2DRIVE_1          (0x4000)    /* XT2 Drive Level mode: 1 */
#define XT2DRIVE_2          (0x8000)    /* XT2 Drive Level mode: 2 */
#define XT2DRIVE_3          (0xC000)    /* XT2 Drive Level mode: 3 */


/* UCSCTL7 Control Bits */
#define DCOFFG              (0x0001)    /* DCO Fault Flag */
#define XT1LFOFFG           (0x0002)    /* XT1 Low Frequency Oscillator Fault Flag */
//#define RESERVED            (0x0004)    /* RESERVED */
#define XT2OFFG             (0x0008)    /* High Frequency Oscillator 2 Fault Flag */
//#define RESERVED            (0x0010)    /* RESERVED */
//#define RESERVED            (0x0020)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL7 Control Bits */
#define DCOFFG_L            (0x0001)    /* DCO Fault Flag */
#define XT1LFOFFG_L         (0x0002)    /* XT1 Low Frequency Oscillator Fault Flag */
//#define RESERVED            (0x0004)    /* RESERVED */
#define XT2OFFG_L           (0x0008)    /* High Frequency Oscillator 2 Fault Flag */
//#define RESERVED            (0x0010)    /* RESERVED */
//#define RESERVED            (0x0020)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */



/* UCSCTL8 Control Bits */
#define ACLKREQEN           (0x0001)    /* ACLK Clock Request Enable */
#define MCLKREQEN           (0x0002)    /* MCLK Clock Request Enable */
#define SMCLKREQEN          (0x0004)    /* SMCLK Clock Request Enable */
#define MODOSCREQEN         (0x0008)    /* MODOSC Clock Request Enable */
//#define RESERVED            (0x0010)    /* RESERVED */
//#define RESERVED            (0x0020)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL8 Control Bits */
#define ACLKREQEN_L         (0x0001)    /* ACLK Clock Request Enable */
#define MCLKREQEN_L         (0x0002)    /* MCLK Clock Request Enable */
#define SMCLKREQEN_L        (0x0004)    /* SMCLK Clock Request Enable */
#define MODOSCREQEN_L       (0x0008)    /* MODOSC Clock Request Enable */
//#define RESERVED            (0x0010)    /* RESERVED */
//#define RESERVED            (0x0020)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0100)    /* RESERVED */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */


/************************************************************
* USCI A0
************************************************************/
#define __MSP430_HAS_EUSCI_A0__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A0__ 0x05C0
#define EUSCI_A0_BASE __MSP430_BASEADDRESS_EUSCI_A0__

#define UCA0CTLW0_            0x05C0    /* USCI A0 Control Word Register 0 */
sfrb(UCA0CTLW0_L , UCA0CTLW0_);
sfrb(UCA0CTLW0_H , UCA0CTLW0_+1);
sfrw(UCA0CTLW0, UCA0CTLW0_);
#define UCA0CTL1            UCA0CTLW0_L  /* USCI A0 Control Register 1 */
#define UCA0CTL0            UCA0CTLW0_H  /* USCI A0 Control Register 0 */
#define UCA0CTLW1_            0x05C2    /* USCI A0 Control Word Register 1 */
sfrb(UCA0CTLW1_L , UCA0CTLW1_);
sfrb(UCA0CTLW1_H , UCA0CTLW1_+1);
sfrw(UCA0CTLW1, UCA0CTLW1_);
#define UCA0BRW_              0x05C6    /* USCI A0 Baud Word Rate 0 */
sfrb(UCA0BRW_L , UCA0BRW_);
sfrb(UCA0BRW_H , UCA0BRW_+1);
sfrw(UCA0BRW, UCA0BRW_);
#define UCA0BR0             UCA0BRW_L /* USCI A0 Baud Rate 0 */
#define UCA0BR1             UCA0BRW_H /* USCI A0 Baud Rate 1 */
#define UCA0MCTLW_            0x05C8    /* USCI A0 Modulation Control */
sfrb(UCA0MCTLW_L , UCA0MCTLW_);
sfrb(UCA0MCTLW_H , UCA0MCTLW_+1);
sfrw(UCA0MCTLW, UCA0MCTLW_);
#define UCA0STATW_            0x05CA    /* USCI A0 Status Register */
sfrb(UCA0STATW, UCA0STATW_);
#define UCA0RXBUF_            0x05CC    /* USCI A0 Receive Buffer */
const_sfrb(UCA0RXBUF_L , UCA0RXBUF_);
const_sfrb(UCA0RXBUF_H , UCA0RXBUF_+1);
const_sfrw(UCA0RXBUF, UCA0RXBUF_);
#define UCA0TXBUF_            0x05CE    /* USCI A0 Transmit Buffer */
sfrb(UCA0TXBUF_L , UCA0TXBUF_);
sfrb(UCA0TXBUF_H , UCA0TXBUF_+1);
sfrw(UCA0TXBUF, UCA0TXBUF_);
#define UCA0ABCTL_            0x05D0    /* USCI A0 LIN Control */
sfrb(UCA0ABCTL, UCA0ABCTL_);
#define UCA0IRCTL_            0x05D2    /* USCI A0 IrDA Transmit Control */
sfrb(UCA0IRCTL_L , UCA0IRCTL_);
sfrb(UCA0IRCTL_H , UCA0IRCTL_+1);
sfrw(UCA0IRCTL, UCA0IRCTL_);
#define UCA0IRTCTL          UCA0IRCTL_L  /* USCI A0 IrDA Transmit Control */
#define UCA0IRRCTL          UCA0IRCTL_H  /* USCI A0 IrDA Receive Control */
#define UCA0IE_               0x05DA    /* USCI A0 Interrupt Enable Register */
sfrb(UCA0IE_L , UCA0IE_);
sfrb(UCA0IE_H , UCA0IE_+1);
sfrw(UCA0IE, UCA0IE_);
#define UCA0IFG_              0x05DC    /* USCI A0 Interrupt Flags Register */
sfrb(UCA0IFG_L , UCA0IFG_);
sfrb(UCA0IFG_H , UCA0IFG_+1);
sfrw(UCA0IFG, UCA0IFG_);
#define UCA0IV_               0x05DE    /* USCI A0 Interrupt Vector Register */
sfrw(UCA0IV, UCA0IV_);



// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN               (0x8000)    /* Async. Mode: Parity enable */
#define UCPAR               (0x4000)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x2000)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x1000)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x0800)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x0400)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x0200)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x0100)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
#define UCSSEL1             (0x0080)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x0040)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x0020)    /* RX Error interrupt enable */
#define UCBRKIE             (0x0010)    /* Break interrupt enable */
#define UCDORM              (0x0008)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x0004)    /* Send next Data as Address */
#define UCTXBRK             (0x0002)    /* Send next Data as Break */
#define UCSWRST             (0x0001)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCSSEL1_L           (0x0080)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0_L           (0x0040)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE_L           (0x0020)    /* RX Error interrupt enable */
#define UCBRKIE_L           (0x0010)    /* Break interrupt enable */
#define UCDORM_L            (0x0008)    /* Dormant (Sleep) Mode */
#define UCTXADDR_L          (0x0004)    /* Send next Data as Address */
#define UCTXBRK_L           (0x0002)    /* Send next Data as Break */
#define UCSWRST_L           (0x0001)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN_H             (0x0080)    /* Async. Mode: Parity enable */
#define UCPAR_H             (0x0040)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB_H             (0x0020)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT_H            (0x0010)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB_H             (0x0008)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1_H           (0x0004)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0_H           (0x0002)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC_H            (0x0001)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTLW0 SPI-Mode Control Bits
#define UCCKPH              (0x8000)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x4000)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x0800)    /* Sync. Mode: Master Select */
//#define res               (0x0020)    /* reserved */
//#define res               (0x0010)    /* reserved */
//#define res               (0x0008)    /* reserved */
//#define res               (0x0004)    /* reserved */
#define UCSTEM             (0x0002)    /* USCI STE Mode */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10               (0x8000)    /* 10-bit Address Mode */
#define UCSLA10             (0x4000)    /* 10-bit Slave Address Mode */
#define UCMM                (0x2000)    /* Multi-Master Environment */
//#define res               (0x1000)    /* reserved */
//#define res               (0x0100)    /* reserved */
#define UCTXACK             (0x0020)    /* Transmit ACK */
#define UCTR                (0x0010)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x0008)    /* Transmit NACK */
#define UCTXSTP             (0x0004)    /* Transmit STOP */
#define UCTXSTT             (0x0002)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
//#define res               (0x1000)    /* reserved */
//#define res               (0x0100)    /* reserved */
#define UCTXACK_L           (0x0020)    /* Transmit ACK */
#define UCTR_L              (0x0010)    /* Transmit/Receive Select/Flag */
#define UCTXNACK_L          (0x0008)    /* Transmit NACK */
#define UCTXSTP_L           (0x0004)    /* Transmit STOP */
#define UCTXSTT_L           (0x0002)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10_H             (0x0080)    /* 10-bit Address Mode */
#define UCSLA10_H           (0x0040)    /* 10-bit Slave Address Mode */
#define UCMM_H              (0x0020)    /* Multi-Master Environment */
//#define res               (0x1000)    /* reserved */
//#define res               (0x0100)    /* reserved */

#define UCMODE_0            (0x0000)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x0200)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x0400)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x0600)    /* Sync. Mode: USCI Mode: 3 */

#define UCSSEL_0            (0x0000)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x0040)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x0080)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0x00C0)    /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK        (0x0000)    /* USCI 0 Clock Source: UCLK */
#define UCSSEL__ACLK        (0x0040)    /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK       (0x0080)    /* USCI 0 Clock Source: SMCLK */


// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1             (0x0002)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0             (0x0001)    /* USCI Deglitch Time Bit 0 */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1_L           (0x0002)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0_L           (0x0001)    /* USCI Deglitch Time Bit 0 */


// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT            (0x0100)    /* USCI Early UCTXIFG0 */
#define UCCLTO1             (0x0080)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0             (0x0040)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK           (0x0020)    /* USCI Acknowledge Stop last byte */
#define UCSWACK             (0x0010)    /* USCI Software controlled ACK */
#define UCASTP1             (0x0008)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0             (0x0004)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1             (0x0002)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0             (0x0001)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCCLTO1_L           (0x0080)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0_L           (0x0040)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK_L         (0x0020)    /* USCI Acknowledge Stop last byte */
#define UCSWACK_L           (0x0010)    /* USCI Software controlled ACK */
#define UCASTP1_L           (0x0008)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0_L           (0x0004)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1_L           (0x0002)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0_L           (0x0001)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT_H          (0x0001)    /* USCI Early UCTXIFG0 */

#define UCGLIT_0            (0x0000)    /* USCI Deglitch time: 0 */
#define UCGLIT_1            (0x0001)    /* USCI Deglitch time: 1 */
#define UCGLIT_2            (0x0002)    /* USCI Deglitch time: 2 */
#define UCGLIT_3            (0x0003)    /* USCI Deglitch time: 3 */

#define UCASTP_0            (0x0000)    /* USCI Automatic Stop condition generation: 0 */
#define UCASTP_1            (0x0004)    /* USCI Automatic Stop condition generation: 1 */
#define UCASTP_2            (0x0008)    /* USCI Automatic Stop condition generation: 2 */
#define UCASTP_3            (0x000C)    /* USCI Automatic Stop condition generation: 3 */

#define UCCLTO_0            (0x0000)    /* USCI Clock low timeout: 0 */
#define UCCLTO_1            (0x0040)    /* USCI Clock low timeout: 1 */
#define UCCLTO_2            (0x0080)    /* USCI Clock low timeout: 2 */
#define UCCLTO_3            (0x00C0)    /* USCI Clock low timeout: 3 */


/* UCAxMCTLW Control Bits */
#define UCBRS7              (0x8000)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6              (0x4000)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5              (0x2000)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4              (0x1000)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3              (0x0800)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2              (0x0400)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1              (0x0200)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0              (0x0100)    /* USCI Second Stage Modulation Select 0 */
#define UCBRF3              (0x0080)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2              (0x0040)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1              (0x0020)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0              (0x0010)    /* USCI First Stage Modulation Select 0 */
#define UCOS16              (0x0001)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRF3_L            (0x0080)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2_L            (0x0040)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1_L            (0x0020)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0_L            (0x0010)    /* USCI First Stage Modulation Select 0 */
#define UCOS16_L            (0x0001)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRS7_H            (0x0080)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6_H            (0x0040)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5_H            (0x0020)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4_H            (0x0010)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3_H            (0x0008)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2_H            (0x0004)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1_H            (0x0002)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0_H            (0x0001)    /* USCI Second Stage Modulation Select 0 */

#define UCBRF_0             (0x00)    /* USCI First Stage Modulation: 0 */
#define UCBRF_1             (0x10)    /* USCI First Stage Modulation: 1 */
#define UCBRF_2             (0x20)    /* USCI First Stage Modulation: 2 */
#define UCBRF_3             (0x30)    /* USCI First Stage Modulation: 3 */
#define UCBRF_4             (0x40)    /* USCI First Stage Modulation: 4 */
#define UCBRF_5             (0x50)    /* USCI First Stage Modulation: 5 */
#define UCBRF_6             (0x60)    /* USCI First Stage Modulation: 6 */
#define UCBRF_7             (0x70)    /* USCI First Stage Modulation: 7 */
#define UCBRF_8             (0x80)    /* USCI First Stage Modulation: 8 */
#define UCBRF_9             (0x90)    /* USCI First Stage Modulation: 9 */
#define UCBRF_10            (0xA0)    /* USCI First Stage Modulation: A */
#define UCBRF_11            (0xB0)    /* USCI First Stage Modulation: B */
#define UCBRF_12            (0xC0)    /* USCI First Stage Modulation: C */
#define UCBRF_13            (0xD0)    /* USCI First Stage Modulation: D */
#define UCBRF_14            (0xE0)    /* USCI First Stage Modulation: E */
#define UCBRF_15            (0xF0)    /* USCI First Stage Modulation: F */


/* UCAxSTATW Control Bits */
#define UCLISTEN            (0x0080)  /* USCI Listen mode */
#define UCFE                (0x0040)  /* USCI Frame Error Flag */
#define UCOE                (0x0020)  /* USCI Overrun Error Flag */
#define UCPE                (0x0010)  /* USCI Parity Error Flag */
#define UCBRK               (0x0008)  /* USCI Break received */
#define UCRXERR             (0x0004)  /* USCI RX Error Flag */
#define UCADDR              (0x0002)  /* USCI Address received Flag */
#define UCBUSY              (0x0001)  /* USCI Busy Flag */
#define UCIDLE              (0x0002)  /* USCI Idle line detected Flag */


/* UCBxSTATW I2C Control Bits */
#define UCBCNT7             (0x8000)  /* USCI Byte Counter Bit 7 */
#define UCBCNT6             (0x4000)  /* USCI Byte Counter Bit 6 */
#define UCBCNT5             (0x2000)  /* USCI Byte Counter Bit 5 */
#define UCBCNT4             (0x1000)  /* USCI Byte Counter Bit 4 */
#define UCBCNT3             (0x0800)  /* USCI Byte Counter Bit 3 */
#define UCBCNT2             (0x0400)  /* USCI Byte Counter Bit 2 */
#define UCBCNT1             (0x0200)  /* USCI Byte Counter Bit 1 */
#define UCBCNT0             (0x0100)  /* USCI Byte Counter Bit 0 */
#define UCSCLLOW            (0x0040)  /* SCL low */
#define UCGC                (0x0020)  /* General Call address received Flag */
#define UCBBUSY             (0x0010)  /* Bus Busy Flag */

/* UCBxTBCNT I2C Control Bits */
#define UCTBCNT7            (0x0080)  /* USCI Byte Counter Bit 7 */
#define UCTBCNT6            (0x0040)  /* USCI Byte Counter Bit 6 */
#define UCTBCNT5            (0x0020)  /* USCI Byte Counter Bit 5 */
#define UCTBCNT4            (0x0010)  /* USCI Byte Counter Bit 4 */
#define UCTBCNT3            (0x0008)  /* USCI Byte Counter Bit 3 */
#define UCTBCNT2            (0x0004)  /* USCI Byte Counter Bit 2 */
#define UCTBCNT1            (0x0002)  /* USCI Byte Counter Bit 1 */
#define UCTBCNT0            (0x0001)  /* USCI Byte Counter Bit 0 */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5           (0x8000)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4           (0x4000)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3           (0x2000)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2           (0x1000)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1           (0x0800)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0           (0x0400)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL            (0x0200)  /* IRDA Receive Input Polarity */
#define UCIRRXFE            (0x0100)  /* IRDA Receive Filter enable */
#define UCIRTXPL5           (0x0080)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4           (0x0040)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3           (0x0020)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2           (0x0010)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1           (0x0008)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0           (0x0004)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK           (0x0002)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN              (0x0001)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRTXPL5_L         (0x0080)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4_L         (0x0040)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3_L         (0x0020)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2_L         (0x0010)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1_L         (0x0008)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0_L         (0x0004)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK_L         (0x0002)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN_L            (0x0001)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5_H         (0x0080)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4_H         (0x0040)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3_H         (0x0020)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2_H         (0x0010)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1_H         (0x0008)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0_H         (0x0004)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL_H          (0x0002)  /* IRDA Receive Input Polarity */
#define UCIRRXFE_H          (0x0001)  /* IRDA Receive Filter enable */


/* UCAxABCTL Control Bits */
//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1            (0x20)    /* Break Sync Delimiter 1 */
#define UCDELIM0            (0x10)    /* Break Sync Delimiter 0 */
#define UCSTOE              (0x08)    /* Sync-Field Timeout error */
#define UCBTOE              (0x04)    /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN             (0x01)    /* Auto Baud Rate detect enable */

/* UCBxI2COA0 Control Bits */
#define UCGCEN              (0x8000)  /* I2C General Call enable */
#define UCOAEN              (0x0400)  /* I2C Own Address enable */
#define UCOA9               (0x0200)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCOA7_L             (0x0080)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCGCEN_H            (0x0080)  /* I2C General Call enable */
#define UCOAEN_H            (0x0004)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001)  /* I2C Own Address Bit 8 */

/* UCBxI2COAx Control Bits */
#define UCOAEN              (0x0400)  /* I2C Own Address enable */
#define UCOA9               (0x0200)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOA7_L             (0x0080)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOAEN_H            (0x0004)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001)  /* I2C Own Address Bit 8 */

/* UCBxADDRX Control Bits */
#define UCADDRX9            (0x0200)  /* I2C Receive Address Bit 9 */
#define UCADDRX8            (0x0100)  /* I2C Receive Address Bit 8 */
#define UCADDRX7            (0x0080)  /* I2C Receive Address Bit 7 */
#define UCADDRX6            (0x0040)  /* I2C Receive Address Bit 6 */
#define UCADDRX5            (0x0020)  /* I2C Receive Address Bit 5 */
#define UCADDRX4            (0x0010)  /* I2C Receive Address Bit 4 */
#define UCADDRX3            (0x0008)  /* I2C Receive Address Bit 3 */
#define UCADDRX2            (0x0004)  /* I2C Receive Address Bit 2 */
#define UCADDRX1            (0x0002)  /* I2C Receive Address Bit 1 */
#define UCADDRX0            (0x0001)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX7_L          (0x0080)  /* I2C Receive Address Bit 7 */
#define UCADDRX6_L          (0x0040)  /* I2C Receive Address Bit 6 */
#define UCADDRX5_L          (0x0020)  /* I2C Receive Address Bit 5 */
#define UCADDRX4_L          (0x0010)  /* I2C Receive Address Bit 4 */
#define UCADDRX3_L          (0x0008)  /* I2C Receive Address Bit 3 */
#define UCADDRX2_L          (0x0004)  /* I2C Receive Address Bit 2 */
#define UCADDRX1_L          (0x0002)  /* I2C Receive Address Bit 1 */
#define UCADDRX0_L          (0x0001)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX9_H          (0x0002)  /* I2C Receive Address Bit 9 */
#define UCADDRX8_H          (0x0001)  /* I2C Receive Address Bit 8 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9            (0x0200)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8            (0x0100)  /* I2C Address Mask Bit 8 */
#define UCADDMASK7            (0x0080)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6            (0x0040)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5            (0x0020)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4            (0x0010)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3            (0x0008)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2            (0x0004)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1            (0x0002)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0            (0x0001)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK7_L        (0x0080)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6_L        (0x0040)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5_L        (0x0020)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4_L        (0x0010)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3_L        (0x0008)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2_L        (0x0004)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1_L        (0x0002)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0_L        (0x0001)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9_H        (0x0002)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8_H        (0x0001)  /* I2C Address Mask Bit 8 */

/* UCBxI2CSA Control Bits */
#define UCSA9               (0x0200)  /* I2C Slave Address Bit 9 */
#define UCSA8               (0x0100)  /* I2C Slave Address Bit 8 */
#define UCSA7               (0x0080)  /* I2C Slave Address Bit 7 */
#define UCSA6               (0x0040)  /* I2C Slave Address Bit 6 */
#define UCSA5               (0x0020)  /* I2C Slave Address Bit 5 */
#define UCSA4               (0x0010)  /* I2C Slave Address Bit 4 */
#define UCSA3               (0x0008)  /* I2C Slave Address Bit 3 */
#define UCSA2               (0x0004)  /* I2C Slave Address Bit 2 */
#define UCSA1               (0x0002)  /* I2C Slave Address Bit 1 */
#define UCSA0               (0x0001)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA7_L             (0x0080)  /* I2C Slave Address Bit 7 */
#define UCSA6_L             (0x0040)  /* I2C Slave Address Bit 6 */
#define UCSA5_L             (0x0020)  /* I2C Slave Address Bit 5 */
#define UCSA4_L             (0x0010)  /* I2C Slave Address Bit 4 */
#define UCSA3_L             (0x0008)  /* I2C Slave Address Bit 3 */
#define UCSA2_L             (0x0004)  /* I2C Slave Address Bit 2 */
#define UCSA1_L             (0x0002)  /* I2C Slave Address Bit 1 */
#define UCSA0_L             (0x0001)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA9_H             (0x0002)  /* I2C Slave Address Bit 9 */
#define UCSA8_H             (0x0001)  /* I2C Slave Address Bit 8 */

/* UCAxIE UART Control Bits */
#define UCTXCPTIE           (0x0008)  /* UART Transmit Complete Interrupt Enable */
#define UCSTTIE             (0x0004)  /* UART Start Bit Interrupt Enalble */
#define UCTXIE              (0x0002)  /* UART Transmit Interrupt Enable */
#define UCRXIE              (0x0001)  /* UART Receive Interrupt Enable */

/* UCAxIE/UCBxIE SPI Control Bits */

/* UCBxIE I2C Control Bits */
#define UCBIT9IE            (0x4000)  /* I2C Bit 9 Position Interrupt Enable 3 */
#define UCTXIE3             (0x2000)  /* I2C Transmit Interrupt Enable 3 */
#define UCRXIE3             (0x1000)  /* I2C Receive Interrupt Enable 3 */
#define UCTXIE2             (0x0800)  /* I2C Transmit Interrupt Enable 2 */
#define UCRXIE2             (0x0400)  /* I2C Receive Interrupt Enable 2 */
#define UCTXIE1             (0x0200)  /* I2C Transmit Interrupt Enable 1 */
#define UCRXIE1             (0x0100)  /* I2C Receive Interrupt Enable 1 */
#define UCCLTOIE            (0x0080)  /* I2C Clock Low Timeout interrupt enable */
#define UCBCNTIE            (0x0040)  /* I2C Automatic stop assertion interrupt enable */
#define UCNACKIE            (0x0020)  /* I2C NACK Condition interrupt enable */
#define UCALIE              (0x0010)  /* I2C Arbitration Lost interrupt enable */
#define UCSTPIE             (0x0008)  /* I2C STOP Condition interrupt enable */
#define UCSTTIE             (0x0004)  /* I2C START Condition interrupt enable */
#define UCTXIE0             (0x0002)  /* I2C Transmit Interrupt Enable 0 */
#define UCRXIE0             (0x0001)  /* I2C Receive Interrupt Enable 0 */

/* UCAxIFG UART Control Bits */
#define UCTXCPTIFG          (0x0008)  /* UART Transmit Complete Interrupt Flag */
#define UCSTTIFG            (0x0004)  /* UART Start Bit Interrupt Flag */
#define UCTXIFG             (0x0002)  /* UART Transmit Interrupt Flag */
#define UCRXIFG             (0x0001)  /* UART Receive Interrupt Flag */

/* UCAxIFG/UCBxIFG SPI Control Bits */
#define UCTXIFG             (0x0002)  /* SPI Transmit Interrupt Flag */
#define UCRXIFG             (0x0001)  /* SPI Receive Interrupt Flag */

/* UCBxIFG Control Bits */
#define UCBIT9IFG           (0x4000)  /* I2C Bit 9 Possition Interrupt Flag 3 */
#define UCTXIFG3            (0x2000)  /* I2C Transmit Interrupt Flag 3 */
#define UCRXIFG3            (0x1000)  /* I2C Receive Interrupt Flag 3 */
#define UCTXIFG2            (0x0800)  /* I2C Transmit Interrupt Flag 2 */
#define UCRXIFG2            (0x0400)  /* I2C Receive Interrupt Flag 2 */
#define UCTXIFG1            (0x0200)  /* I2C Transmit Interrupt Flag 1 */
#define UCRXIFG1            (0x0100)  /* I2C Receive Interrupt Flag 1 */
#define UCCLTOIFG           (0x0080)  /* I2C Clock low Timeout interrupt Flag */
#define UCBCNTIFG           (0x0040)  /* I2C Byte counter interrupt flag */
#define UCNACKIFG           (0x0020)  /* I2C NACK Condition interrupt Flag */
#define UCALIFG             (0x0010)  /* I2C Arbitration Lost interrupt Flag */
#define UCSTPIFG            (0x0008)  /* I2C STOP Condition interrupt Flag */
#define UCSTTIFG            (0x0004)  /* I2C START Condition interrupt Flag */
#define UCTXIFG0            (0x0002)  /* I2C Transmit Interrupt Flag 0 */
#define UCRXIFG0            (0x0001)  /* I2C Receive Interrupt Flag 0 */

/* USCI UART Definitions */
#define USCI_NONE            (0x0000)   /* No Interrupt pending */
#define USCI_UART_UCRXIFG    (0x0002)   /* USCI UCRXIFG */
#define USCI_UART_UCTXIFG    (0x0004)   /* USCI UCTXIFG */
#define USCI_UART_UCSTTIFG   (0x0006)   /* USCI UCSTTIFG */
#define USCI_UART_UCTXCPTIFG (0x0008)   /* USCI UCTXCPTIFG */

/* USCI SPI Definitions */
#define USCI_SPI_UCRXIFG    (0x0002)    /* USCI UCRXIFG */
#define USCI_SPI_UCTXIFG    (0x0004)    /* USCI UCTXIFG */

/* USCI I2C Definitions */
#define USCI_I2C_UCALIFG    (0x0002)    /* USCI I2C Mode: UCALIFG */
#define USCI_I2C_UCNACKIFG  (0x0004)    /* USCI I2C Mode: UCNACKIFG */
#define USCI_I2C_UCSTTIFG   (0x0006)    /* USCI I2C Mode: UCSTTIFG*/
#define USCI_I2C_UCSTPIFG   (0x0008)    /* USCI I2C Mode: UCSTPIFG*/
#define USCI_I2C_UCRXIFG3   (0x000A)    /* USCI I2C Mode: UCRXIFG3 */
#define USCI_I2C_UCTXIFG3   (0x000C)    /* USCI I2C Mode: UCTXIFG3 */
#define USCI_I2C_UCRXIFG2   (0x000E)    /* USCI I2C Mode: UCRXIFG2 */
#define USCI_I2C_UCTXIFG2   (0x0010)    /* USCI I2C Mode: UCTXIFG2 */
#define USCI_I2C_UCRXIFG1   (0x0012)    /* USCI I2C Mode: UCRXIFG1 */
#define USCI_I2C_UCTXIFG1   (0x0014)    /* USCI I2C Mode: UCTXIFG1 */
#define USCI_I2C_UCRXIFG0   (0x0016)    /* USCI I2C Mode: UCRXIFG0 */
#define USCI_I2C_UCTXIFG0   (0x0018)    /* USCI I2C Mode: UCTXIFG0 */
#define USCI_I2C_UCBCNTIFG  (0x001A)    /* USCI I2C Mode: UCBCNTIFG */
#define USCI_I2C_UCCLTOIFG  (0x001C)    /* USCI I2C Mode: UCCLTOIFG */
#define USCI_I2C_UCBIT9IFG  (0x001E)    /* USCI I2C Mode: UCBIT9IFG */

/************************************************************
* USCI A1
************************************************************/
#define __MSP430_HAS_EUSCI_A1__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A1__ 0x05E0
#define EUSCI_A1_BASE __MSP430_BASEADDRESS_EUSCI_A1__

#define UCA1CTLW0_            0x05E0    /* USCI A1 Control Word Register 0 */
sfrb(UCA1CTLW0_L , UCA1CTLW0_);
sfrb(UCA1CTLW0_H , UCA1CTLW0_+1);
sfrw(UCA1CTLW0, UCA1CTLW0_);
#define UCA1CTL1            UCA1CTLW0_L  /* USCI A1 Control Register 1 */
#define UCA1CTL0            UCA1CTLW0_H  /* USCI A1 Control Register 0 */
#define UCA1CTLW1_            0x05E2    /* USCI A1 Control Word Register 1 */
sfrb(UCA1CTLW1_L , UCA1CTLW1_);
sfrb(UCA1CTLW1_H , UCA1CTLW1_+1);
sfrw(UCA1CTLW1, UCA1CTLW1_);
#define UCA1BRW_              0x05E6    /* USCI A1 Baud Word Rate 0 */
sfrb(UCA1BRW_L , UCA1BRW_);
sfrb(UCA1BRW_H , UCA1BRW_+1);
sfrw(UCA1BRW, UCA1BRW_);
#define UCA1BR0             UCA1BRW_L /* USCI A1 Baud Rate 0 */
#define UCA1BR1             UCA1BRW_H /* USCI A1 Baud Rate 1 */
#define UCA1MCTLW_            0x05E8    /* USCI A1 Modulation Control */
sfrb(UCA1MCTLW_L , UCA1MCTLW_);
sfrb(UCA1MCTLW_H , UCA1MCTLW_+1);
sfrw(UCA1MCTLW, UCA1MCTLW_);
#define UCA1STATW_            0x05EA    /* USCI A1 Status Register */
sfrb(UCA1STATW, UCA1STATW_);
#define UCA1RXBUF_            0x05EC    /* USCI A1 Receive Buffer */
const_sfrb(UCA1RXBUF_L , UCA1RXBUF_);
const_sfrb(UCA1RXBUF_H , UCA1RXBUF_+1);
const_sfrw(UCA1RXBUF, UCA1RXBUF_);
#define UCA1TXBUF_            0x05EE    /* USCI A1 Transmit Buffer */
sfrb(UCA1TXBUF_L , UCA1TXBUF_);
sfrb(UCA1TXBUF_H , UCA1TXBUF_+1);
sfrw(UCA1TXBUF, UCA1TXBUF_);
#define UCA1ABCTL_            0x05F0    /* USCI A1 LIN Control */
sfrb(UCA1ABCTL, UCA1ABCTL_);
#define UCA1IRCTL_            0x05F2    /* USCI A1 IrDA Transmit Control */
sfrb(UCA1IRCTL_L , UCA1IRCTL_);
sfrb(UCA1IRCTL_H , UCA1IRCTL_+1);
sfrw(UCA1IRCTL, UCA1IRCTL_);
#define UCA1IRTCTL          UCA1IRCTL_L  /* USCI A1 IrDA Transmit Control */
#define UCA1IRRCTL          UCA1IRCTL_H  /* USCI A1 IrDA Receive Control */
#define UCA1IE_               0x05FA    /* USCI A1 Interrupt Enable Register */
sfrb(UCA1IE_L , UCA1IE_);
sfrb(UCA1IE_H , UCA1IE_+1);
sfrw(UCA1IE, UCA1IE_);
#define UCA1IFG_              0x05FC    /* USCI A1 Interrupt Flags Register */
sfrb(UCA1IFG_L , UCA1IFG_);
sfrb(UCA1IFG_H , UCA1IFG_+1);
sfrw(UCA1IFG, UCA1IFG_);
#define UCA1IV_               0x05FE    /* USCI A1 Interrupt Vector Register */
sfrw(UCA1IV, UCA1IV_);


/************************************************************
* USCI A2
************************************************************/
#define __MSP430_HAS_EUSCI_A2__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A2__ 0x0600
#define EUSCI_A2_BASE __MSP430_BASEADDRESS_EUSCI_A2__

#define UCA2CTLW0_            0x0600    /* USCI A2 Control Word Register 0 */
sfrb(UCA2CTLW0_L , UCA2CTLW0_);
sfrb(UCA2CTLW0_H , UCA2CTLW0_+1);
sfrw(UCA2CTLW0, UCA2CTLW0_);
#define UCA2CTL1            UCA2CTLW0_L  /* USCI A2 Control Register 1 */
#define UCA2CTL0            UCA2CTLW0_H  /* USCI A2 Control Register 0 */
#define UCA2CTLW1_            0x0602    /* USCI A2 Control Word Register 1 */
sfrb(UCA2CTLW1_L , UCA2CTLW1_);
sfrb(UCA2CTLW1_H , UCA2CTLW1_+1);
sfrw(UCA2CTLW1, UCA2CTLW1_);
#define UCA2BRW_              0x0606    /* USCI A2 Baud Word Rate 0 */
sfrb(UCA2BRW_L , UCA2BRW_);
sfrb(UCA2BRW_H , UCA2BRW_+1);
sfrw(UCA2BRW, UCA2BRW_);
#define UCA2BR0             UCA2BRW_L /* USCI A2 Baud Rate 0 */
#define UCA2BR1             UCA2BRW_H /* USCI A2 Baud Rate 1 */
#define UCA2MCTLW_            0x0608    /* USCI A2 Modulation Control */
sfrb(UCA2MCTLW_L , UCA2MCTLW_);
sfrb(UCA2MCTLW_H , UCA2MCTLW_+1);
sfrw(UCA2MCTLW, UCA2MCTLW_);
#define UCA2STATW_            0x060A    /* USCI A2 Status Register */
sfrb(UCA2STATW, UCA2STATW_);
#define UCA2RXBUF_            0x060C    /* USCI A2 Receive Buffer */
const_sfrb(UCA2RXBUF_L , UCA2RXBUF_);
const_sfrb(UCA2RXBUF_H , UCA2RXBUF_+1);
const_sfrw(UCA2RXBUF, UCA2RXBUF_);
#define UCA2TXBUF_            0x060E    /* USCI A2 Transmit Buffer */
sfrb(UCA2TXBUF_L , UCA2TXBUF_);
sfrb(UCA2TXBUF_H , UCA2TXBUF_+1);
sfrw(UCA2TXBUF, UCA2TXBUF_);
#define UCA2ABCTL_            0x0610    /* USCI A2 LIN Control */
sfrb(UCA2ABCTL, UCA2ABCTL_);
#define UCA2IRCTL_            0x0612    /* USCI A2 IrDA Transmit Control */
sfrb(UCA2IRCTL_L , UCA2IRCTL_);
sfrb(UCA2IRCTL_H , UCA2IRCTL_+1);
sfrw(UCA2IRCTL, UCA2IRCTL_);
#define UCA2IRTCTL          UCA2IRCTL_L  /* USCI A2 IrDA Transmit Control */
#define UCA2IRRCTL          UCA2IRCTL_H  /* USCI A2 IrDA Receive Control */
#define UCA2IE_               0x061A    /* USCI A2 Interrupt Enable Register */
sfrb(UCA2IE_L , UCA2IE_);
sfrb(UCA2IE_H , UCA2IE_+1);
sfrw(UCA2IE, UCA2IE_);
#define UCA2IFG_              0x061C    /* USCI A2 Interrupt Flags Register */
sfrb(UCA2IFG_L , UCA2IFG_);
sfrb(UCA2IFG_H , UCA2IFG_+1);
sfrw(UCA2IFG, UCA2IFG_);
#define UCA2IV_               0x061E    /* USCI A2 Interrupt Vector Register */
sfrw(UCA2IV, UCA2IV_);


/************************************************************
* USCI A3
************************************************************/
#define __MSP430_HAS_EUSCI_A3__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A3__ 0x0620
#define EUSCI_A3_BASE __MSP430_BASEADDRESS_EUSCI_A3__

#define UCA3CTLW0_            0x0620    /* USCI A3 Control Word Register 0 */
sfrb(UCA3CTLW0_L , UCA3CTLW0_);
sfrb(UCA3CTLW0_H , UCA3CTLW0_+1);
sfrw(UCA3CTLW0, UCA3CTLW0_);
#define UCA3CTL1            UCA3CTLW0_L  /* USCI A3 Control Register 1 */
#define UCA3CTL0            UCA3CTLW0_H  /* USCI A3 Control Register 0 */
#define UCA3CTLW1_            0x0622    /* USCI A3 Control Word Register 1 */
sfrb(UCA3CTLW1_L , UCA3CTLW1_);
sfrb(UCA3CTLW1_H , UCA3CTLW1_+1);
sfrw(UCA3CTLW1, UCA3CTLW1_);
#define UCA3BRW_              0x0626    /* USCI A3 Baud Word Rate 0 */
sfrb(UCA3BRW_L , UCA3BRW_);
sfrb(UCA3BRW_H , UCA3BRW_+1);
sfrw(UCA3BRW, UCA3BRW_);
#define UCA3BR0             UCA3BRW_L /* USCI A3 Baud Rate 0 */
#define UCA3BR1             UCA3BRW_H /* USCI A3 Baud Rate 1 */
#define UCA3MCTLW_            0x0628    /* USCI A3 Modulation Control */
sfrb(UCA3MCTLW_L , UCA3MCTLW_);
sfrb(UCA3MCTLW_H , UCA3MCTLW_+1);
sfrw(UCA3MCTLW, UCA3MCTLW_);
#define UCA3STATW_            0x062A    /* USCI A3 Status Register */
sfrb(UCA3STATW, UCA3STATW_);
#define UCA3RXBUF_            0x062C    /* USCI A3 Receive Buffer */
const_sfrb(UCA3RXBUF_L , UCA3RXBUF_);
const_sfrb(UCA3RXBUF_H , UCA3RXBUF_+1);
const_sfrw(UCA3RXBUF, UCA3RXBUF_);
#define UCA3TXBUF_            0x062E    /* USCI A3 Transmit Buffer */
sfrb(UCA3TXBUF_L , UCA3TXBUF_);
sfrb(UCA3TXBUF_H , UCA3TXBUF_+1);
sfrw(UCA3TXBUF, UCA3TXBUF_);
#define UCA3ABCTL_            0x0630    /* USCI A3 LIN Control */
sfrb(UCA3ABCTL, UCA3ABCTL_);
#define UCA3IRCTL_            0x0632    /* USCI A3 IrDA Transmit Control */
sfrb(UCA3IRCTL_L , UCA3IRCTL_);
sfrb(UCA3IRCTL_H , UCA3IRCTL_+1);
sfrw(UCA3IRCTL, UCA3IRCTL_);
#define UCA3IRTCTL          UCA3IRCTL_L  /* USCI A3 IrDA Transmit Control */
#define UCA3IRRCTL          UCA3IRCTL_H  /* USCI A3 IrDA Receive Control */
#define UCA3IE_               0x063A    /* USCI A3 Interrupt Enable Register */
sfrb(UCA3IE_L , UCA3IE_);
sfrb(UCA3IE_H , UCA3IE_+1);
sfrw(UCA3IE, UCA3IE_);
#define UCA3IFG_              0x063C    /* USCI A3 Interrupt Flags Register */
sfrb(UCA3IFG_L , UCA3IFG_);
sfrb(UCA3IFG_H , UCA3IFG_+1);
sfrw(UCA3IFG, UCA3IFG_);
#define UCA3IV_               0x063E    /* USCI A3 Interrupt Vector Register */
sfrw(UCA3IV, UCA3IV_);


/************************************************************
* USCI B0
************************************************************/
#define __MSP430_HAS_EUSCI_B0__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_B0__ 0x0640
#define EUSCI_B0_BASE __MSP430_BASEADDRESS_EUSCI_B0__


#define UCB0CTLW0_            0x0640    /* USCI B0 Control Word Register 0 */
sfrb(UCB0CTLW0_L , UCB0CTLW0_);
sfrb(UCB0CTLW0_H , UCB0CTLW0_+1);
sfrw(UCB0CTLW0, UCB0CTLW0_);
#define UCB0CTL1            UCB0CTLW0_L  /* USCI B0 Control Register 1 */
#define UCB0CTL0            UCB0CTLW0_H  /* USCI B0 Control Register 0 */
#define UCB0CTLW1_            0x0642    /* USCI B0 Control Word Register 1 */
sfrb(UCB0CTLW1_L , UCB0CTLW1_);
sfrb(UCB0CTLW1_H , UCB0CTLW1_+1);
sfrw(UCB0CTLW1, UCB0CTLW1_);
#define UCB0BRW_              0x0646    /* USCI B0 Baud Word Rate 0 */
sfrb(UCB0BRW_L , UCB0BRW_);
sfrb(UCB0BRW_H , UCB0BRW_+1);
sfrw(UCB0BRW, UCB0BRW_);
#define UCB0BR0             UCB0BRW_L /* USCI B0 Baud Rate 0 */
#define UCB0BR1             UCB0BRW_H /* USCI B0 Baud Rate 1 */
#define UCB0STATW_            0x0648    /* USCI B0 Status Word Register */
sfrb(UCB0STATW_L , UCB0STATW_);
sfrb(UCB0STATW_H , UCB0STATW_+1);
sfrw(UCB0STATW, UCB0STATW_);
#define UCB0STAT            UCB0STATW_L /* USCI B0 Status Register */
#define UCB0BCNT            UCB0STATW_H /* USCI B0 Byte Counter Register */
#define UCB0TBCNT_            0x064A    /* USCI B0 Byte Counter Threshold Register */
sfrb(UCB0TBCNT_L , UCB0TBCNT_);
sfrb(UCB0TBCNT_H , UCB0TBCNT_+1);
sfrw(UCB0TBCNT, UCB0TBCNT_);
#define UCB0RXBUF_            0x064C    /* USCI B0 Receive Buffer */
const_sfrb(UCB0RXBUF_L , UCB0RXBUF_);
const_sfrb(UCB0RXBUF_H , UCB0RXBUF_+1);
const_sfrw(UCB0RXBUF, UCB0RXBUF_);
#define UCB0TXBUF_            0x064E    /* USCI B0 Transmit Buffer */
sfrb(UCB0TXBUF_L , UCB0TXBUF_);
sfrb(UCB0TXBUF_H , UCB0TXBUF_+1);
sfrw(UCB0TXBUF, UCB0TXBUF_);
#define UCB0I2COA0_           0x0654    /* USCI B0 I2C Own Address 0 */
sfrb(UCB0I2COA0_L , UCB0I2COA0_);
sfrb(UCB0I2COA0_H , UCB0I2COA0_+1);
sfrw(UCB0I2COA0, UCB0I2COA0_);
#define UCB0I2COA1_           0x0656    /* USCI B0 I2C Own Address 1 */
sfrb(UCB0I2COA1_L , UCB0I2COA1_);
sfrb(UCB0I2COA1_H , UCB0I2COA1_+1);
sfrw(UCB0I2COA1, UCB0I2COA1_);
#define UCB0I2COA2_           0x0658    /* USCI B0 I2C Own Address 2 */
sfrb(UCB0I2COA2_L , UCB0I2COA2_);
sfrb(UCB0I2COA2_H , UCB0I2COA2_+1);
sfrw(UCB0I2COA2, UCB0I2COA2_);
#define UCB0I2COA3_           0x065A    /* USCI B0 I2C Own Address 3 */
sfrb(UCB0I2COA3_L , UCB0I2COA3_);
sfrb(UCB0I2COA3_H , UCB0I2COA3_+1);
sfrw(UCB0I2COA3, UCB0I2COA3_);
#define UCB0ADDRX_            0x065C    /* USCI B0 Received Address Register */
sfrb(UCB0ADDRX_L , UCB0ADDRX_);
sfrb(UCB0ADDRX_H , UCB0ADDRX_+1);
sfrw(UCB0ADDRX, UCB0ADDRX_);
#define UCB0ADDMASK_          0x065E    /* USCI B0 Address Mask Register */
sfrb(UCB0ADDMASK_L , UCB0ADDMASK_);
sfrb(UCB0ADDMASK_H , UCB0ADDMASK_+1);
sfrw(UCB0ADDMASK, UCB0ADDMASK_);
#define UCB0I2CSA_            0x0660    /* USCI B0 I2C Slave Address */
sfrb(UCB0I2CSA_L , UCB0I2CSA_);
sfrb(UCB0I2CSA_H , UCB0I2CSA_+1);
sfrw(UCB0I2CSA, UCB0I2CSA_);
#define UCB0IE_               0x066A    /* USCI B0 Interrupt Enable Register */
sfrb(UCB0IE_L , UCB0IE_);
sfrb(UCB0IE_H , UCB0IE_+1);
sfrw(UCB0IE, UCB0IE_);
#define UCB0IFG_              0x066C    /* USCI B0 Interrupt Flags Register */
sfrb(UCB0IFG_L , UCB0IFG_);
sfrb(UCB0IFG_H , UCB0IFG_+1);
sfrw(UCB0IFG, UCB0IFG_);
#define UCB0IV_               0x066E    /* USCI B0 Interrupt Vector Register */
sfrw(UCB0IV, UCB0IV_);

/************************************************************
* USCI B1
************************************************************/
#define __MSP430_HAS_EUSCI_B1__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_B1__ 0x0680
#define EUSCI_B1_BASE __MSP430_BASEADDRESS_EUSCI_B1__


#define UCB1CTLW0_            0x0680    /* USCI B1 Control Word Register 0 */
sfrb(UCB1CTLW0_L , UCB1CTLW0_);
sfrb(UCB1CTLW0_H , UCB1CTLW0_+1);
sfrw(UCB1CTLW0, UCB1CTLW0_);
#define UCB1CTL1            UCB1CTLW0_L  /* USCI B1 Control Register 1 */
#define UCB1CTL0            UCB1CTLW0_H  /* USCI B1 Control Register 0 */
#define UCB1CTLW1_            0x0682    /* USCI B1 Control Word Register 1 */
sfrb(UCB1CTLW1_L , UCB1CTLW1_);
sfrb(UCB1CTLW1_H , UCB1CTLW1_+1);
sfrw(UCB1CTLW1, UCB1CTLW1_);
#define UCB1BRW_              0x0686    /* USCI B1 Baud Word Rate 0 */
sfrb(UCB1BRW_L , UCB1BRW_);
sfrb(UCB1BRW_H , UCB1BRW_+1);
sfrw(UCB1BRW, UCB1BRW_);
#define UCB1BR0             UCB1BRW_L /* USCI B1 Baud Rate 0 */
#define UCB1BR1             UCB1BRW_H /* USCI B1 Baud Rate 1 */
#define UCB1STATW_            0x0688    /* USCI B1 Status Word Register */
sfrb(UCB1STATW_L , UCB1STATW_);
sfrb(UCB1STATW_H , UCB1STATW_+1);
sfrw(UCB1STATW, UCB1STATW_);
#define UCB1STAT            UCB1STATW_L /* USCI B1 Status Register */
#define UCB1BCNT            UCB1STATW_H /* USCI B1 Byte Counter Register */
#define UCB1TBCNT_            0x068A    /* USCI B1 Byte Counter Threshold Register */
sfrb(UCB1TBCNT_L , UCB1TBCNT_);
sfrb(UCB1TBCNT_H , UCB1TBCNT_+1);
sfrw(UCB1TBCNT, UCB1TBCNT_);
#define UCB1RXBUF_            0x068C    /* USCI B1 Receive Buffer */
const_sfrb(UCB1RXBUF_L , UCB1RXBUF_);
const_sfrb(UCB1RXBUF_H , UCB1RXBUF_+1);
const_sfrw(UCB1RXBUF, UCB1RXBUF_);
#define UCB1TXBUF_            0x068E    /* USCI B1 Transmit Buffer */
sfrb(UCB1TXBUF_L , UCB1TXBUF_);
sfrb(UCB1TXBUF_H , UCB1TXBUF_+1);
sfrw(UCB1TXBUF, UCB1TXBUF_);
#define UCB1I2COA0_           0x0694    /* USCI B1 I2C Own Address 0 */
sfrb(UCB1I2COA0_L , UCB1I2COA0_);
sfrb(UCB1I2COA0_H , UCB1I2COA0_+1);
sfrw(UCB1I2COA0, UCB1I2COA0_);
#define UCB1I2COA1_           0x0696    /* USCI B1 I2C Own Address 1 */
sfrb(UCB1I2COA1_L , UCB1I2COA1_);
sfrb(UCB1I2COA1_H , UCB1I2COA1_+1);
sfrw(UCB1I2COA1, UCB1I2COA1_);
#define UCB1I2COA2_           0x0698    /* USCI B1 I2C Own Address 2 */
sfrb(UCB1I2COA2_L , UCB1I2COA2_);
sfrb(UCB1I2COA2_H , UCB1I2COA2_+1);
sfrw(UCB1I2COA2, UCB1I2COA2_);
#define UCB1I2COA3_           0x069A    /* USCI B1 I2C Own Address 3 */
sfrb(UCB1I2COA3_L , UCB1I2COA3_);
sfrb(UCB1I2COA3_H , UCB1I2COA3_+1);
sfrw(UCB1I2COA3, UCB1I2COA3_);
#define UCB1ADDRX_            0x069C    /* USCI B1 Received Address Register */
sfrb(UCB1ADDRX_L , UCB1ADDRX_);
sfrb(UCB1ADDRX_H , UCB1ADDRX_+1);
sfrw(UCB1ADDRX, UCB1ADDRX_);
#define UCB1ADDMASK_          0x069E    /* USCI B1 Address Mask Register */
sfrb(UCB1ADDMASK_L , UCB1ADDMASK_);
sfrb(UCB1ADDMASK_H , UCB1ADDMASK_+1);
sfrw(UCB1ADDMASK, UCB1ADDMASK_);
#define UCB1I2CSA_            0x06A0    /* USCI B1 I2C Slave Address */
sfrb(UCB1I2CSA_L , UCB1I2CSA_);
sfrb(UCB1I2CSA_H , UCB1I2CSA_+1);
sfrw(UCB1I2CSA, UCB1I2CSA_);
#define UCB1IE_               0x06AA    /* USCI B1 Interrupt Enable Register */
sfrb(UCB1IE_L , UCB1IE_);
sfrb(UCB1IE_H , UCB1IE_+1);
sfrw(UCB1IE, UCB1IE_);
#define UCB1IFG_              0x06AC    /* USCI B1 Interrupt Flags Register */
sfrb(UCB1IFG_L , UCB1IFG_);
sfrb(UCB1IFG_H , UCB1IFG_+1);
sfrw(UCB1IFG, UCB1IFG_);
#define UCB1IV_               0x06AE    /* USCI B1 Interrupt Vector Register */
sfrw(UCB1IV, UCB1IV_);

/************************************************************
* WATCHDOG TIMER A
************************************************************/
#define __MSP430_HAS_WDT_A__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_WDT_A__ 0x0150
#define WDT_A_BASE __MSP430_BASEADDRESS_WDT_A__

#define WDTCTL_               0x015C    /* Watchdog Timer Control */
sfrb(WDTCTL_L , WDTCTL_);
sfrb(WDTCTL_H , WDTCTL_+1);
sfrw(WDTCTL, WDTCTL_);
/* The bit names have been prefixed with "WDT" */
/* WDTCTL Control Bits */
#define WDTIS0              (0x0001)  /* WDT - Timer Interval Select 0 */
#define WDTIS1              (0x0002)  /* WDT - Timer Interval Select 1 */
#define WDTIS2              (0x0004)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL            (0x0008)  /* WDT - Timer Clear */
#define WDTTMSEL            (0x0010)  /* WDT - Timer Mode Select */
#define WDTSSEL0            (0x0020)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1            (0x0040)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD             (0x0080)  /* WDT - Timer hold */

/* WDTCTL Control Bits */
#define WDTIS0_L            (0x0001)  /* WDT - Timer Interval Select 0 */
#define WDTIS1_L            (0x0002)  /* WDT - Timer Interval Select 1 */
#define WDTIS2_L            (0x0004)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL_L          (0x0008)  /* WDT - Timer Clear */
#define WDTTMSEL_L          (0x0010)  /* WDT - Timer Mode Select */
#define WDTSSEL0_L          (0x0020)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1_L          (0x0040)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD_L           (0x0080)  /* WDT - Timer hold */


#define WDTPW               (0x5A00)

#define WDTIS_0           (0x0000)  /* WDT - Timer Interval Select: /2G */
#define WDTIS_1           (0x0001)  /* WDT - Timer Interval Select: /128M */
#define WDTIS_2           (0x0002)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS_3           (0x0003)  /* WDT - Timer Interval Select: /512k */
#define WDTIS_4           (0x0004)  /* WDT - Timer Interval Select: /32k */
#define WDTIS_5           (0x0005)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS_6           (0x0006)  /* WDT - Timer Interval Select: /512 */
#define WDTIS_7           (0x0007)  /* WDT - Timer Interval Select: /64 */
#define WDTIS__2G         (0x0000)  /* WDT - Timer Interval Select: /2G */
#define WDTIS__128M       (0x0001)  /* WDT - Timer Interval Select: /128M */
#define WDTIS__8192K      (0x0002)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS__512K       (0x0003)  /* WDT - Timer Interval Select: /512k */
#define WDTIS__32K        (0x0004)  /* WDT - Timer Interval Select: /32k */
#define WDTIS__8192       (0x0005)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS__512        (0x0006)  /* WDT - Timer Interval Select: /512 */
#define WDTIS__64         (0x0007)  /* WDT - Timer Interval Select: /64 */

#define WDTSSEL_0         (0x0000)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL_1         (0x0020)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL_2         (0x0040)  /* WDT - Timer Clock Source Select: VLO_CLK */
#define WDTSSEL_3         (0x0060)  /* WDT - Timer Clock Source Select: reserved */
#define WDTSSEL__SMCLK    (0x0000)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL__ACLK     (0x0020)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL__VLO      (0x0040)  /* WDT - Timer Clock Source Select: VLO_CLK */


/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS0)                  /* 8ms     " */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1)                  /* 0.5ms   " */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)                /* 1000ms  " */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS0)         /* 250ms   " */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1)         /* 16ms    " */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1+WDTIS0)  /* 1.9ms   " */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL+WDTIS2)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS2+WDTIS0)                           /* 8ms     " */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS2+WDTIS1)                           /* 0.5ms   " */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)                    /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2)                         /* 1000ms  " */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS0)                  /* 250ms   " */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1)                  /* 16ms    " */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1+WDTIS0)           /* 1.9ms   " */


/************************************************************
* TLV Descriptors
************************************************************/
#define __MSP430_HAS_TLV__              /* Definition to show that Module is available */
#define TLV_BASE __MSP430_BASEADDRESS_TLV__

#define TLV_CRC_LENGTH        (0x1A01)    /* CRC length of the TLV structure */
#define TLV_CRC_VALUE         (0x1A02)    /* CRC value of the TLV structure */
#define TLV_START             (0x1A08)    /* Start Address of the TLV structure */
#define TLV_END               (0x1AFF)    /* End Address of the TLV structure */

#define TLV_LDTAG             (0x01)      /*  Legacy descriptor (1xx, 2xx, 4xx families) */
#define TLV_PDTAG             (0x02)      /*  Peripheral discovery descriptor */
#define TLV_Reserved3         (0x03)      /*  Future usage */
#define TLV_Reserved4         (0x04)      /*  Future usage */
#define TLV_BLANK             (0x05)      /*  Blank descriptor */
#define TLV_Reserved6         (0x06)      /*  Future usage */
#define TLV_Reserved7         (0x07)      /*  Serial Number */
#define TLV_DIERECORD         (0x08)      /*  Die Record  */
#define TLV_ADCCAL            (0x11)      /*  ADC12 calibration */
#define TLV_ADC12CAL          (0x11)      /*  ADC12 calibration */
#define TLV_ADC10CAL          (0x13)      /*  ADC10 calibration */
#define TLV_REFCAL            (0x12)      /*  REF calibration */
#define TLV_TAGEXT            (0xFE)      /*  Tag extender */
#define TLV_TAGEND            (0xFF)      //  Tag End of Table


/************************************************************
* Interrupt Vectors (offset from 0xFF80)
************************************************************/


#define COMP_B_VECTOR           (38)                     /* 0xFFCA Comparator B */
#define RTC_VECTOR              (39)                     /* 0xFFCC RTC */
#define LCD_C_VECTOR            (40)                     /* 0xFFCE LCD C */
#define TIMER3_A1_VECTOR        (41)                     /* 0xFFD0 Timer3_A2 CC1, TA */
#define TIMER3_A0_VECTOR        (42)                     /* 0xFFD2 Timer3_A2 CC0 */
#define PORT2_VECTOR            (43)                     /* 0xFFD4 Port 2 */
#define TIMER2_A1_VECTOR        (44)                     /* 0xFFD6 Timer2_A2 CC1, TA */
#define TIMER2_A0_VECTOR        (45)                     /* 0xFFD8 Timer2_A2 CC0 */
#define PORT1_VECTOR            (46)                     /* 0xFFDA Port 1 */
#define USCI_B1_VECTOR          (47)                     /* 0xFFDC USCI B1 Receive/Transmit */
#define USCI_A3_VECTOR          (48)                     /* 0xFFDE USCI A3 Receive/Transmit */
#define TIMER1_A1_VECTOR        (49)                     /* 0xFFE0 Timer1_A2 CC1, TA1 */
#define TIMER1_A0_VECTOR        (50)                     /* 0xFFE2 Timer1_A2 CC0 */
#define DMA_VECTOR              (51)                     /* 0xFFE4 DMA */
#define AUX_VECTOR              (52)                     /* 0xFFE6 AUX Supply */
#define USCI_A2_VECTOR          (53)                     /* 0xFFE8 USCI A2 Receive/Transmit */
#define USCI_A1_VECTOR          (54)                     /* 0xFFEA USCI A1 Receive/Transmit */
#define TIMER0_A1_VECTOR        (55)                     /* 0xFFEC Timer0_A2 CC1-CC2, TA */
#define TIMER0_A0_VECTOR        (56)                     /* 0xFFEE Timer0_A2 CC0 */
#define SD24B_VECTOR            (57)                     /* 0xFFF0 SD24B ADC */
#define ADC10_VECTOR            (58)                     /* 0xFFF2 ADC */
#define USCI_B0_VECTOR          (59)                     /* 0xFFF4 USCI B0 Receive/Transmit */
#define USCI_A0_VECTOR          (60)                     /* 0xFFF6 USCI A0 Receive/Transmit */
#define WDT_VECTOR              (61)                     /* 0xFFF8 Watchdog Timer */
#define UNMI_VECTOR             (62)                     /* 0xFFFA User Non-maskable */
#define SYSNMI_VECTOR           (63)                     /* 0xFFFC System Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __MSP430F67491 */


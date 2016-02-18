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
* This file supports assembler and C development for
* MSP430FR2XX_FR4XXGENERIC device.
*
* Texas Instruments, Version 1.0
*
* Rev. 1.0, Setup
*
*
********************************************************************/

#ifndef __msp430FR2XX_FR4XXGENERIC
#define __msp430FR2XX_FR4XXGENERIC

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

#if defined(__MSP430_HAS_T0A2__) || defined(__MSP430_HAS_T1A2__) || defined(__MSP430_HAS_T2A2__) || defined(__MSP430_HAS_T3A2__) \
 || defined(__MSP430_HAS_T0A3__) || defined(__MSP430_HAS_T1A3__) || defined(__MSP430_HAS_T2A3__) || defined(__MSP430_HAS_T3A3__) \
 || defined(__MSP430_HAS_T0A5__) || defined(__MSP430_HAS_T1A5__) || defined(__MSP430_HAS_T2A5__) || defined(__MSP430_HAS_T3A5__) \
 || defined(__MSP430_HAS_T0A7__) || defined(__MSP430_HAS_T1A7__) || defined(__MSP430_HAS_T2A7__) || defined(__MSP430_HAS_T3A7__)
#define __MSP430_HAS_TxA7__
#endif
#if defined(__MSP430_HAS_T0B3__) || defined(__MSP430_HAS_T0B5__) || defined(__MSP430_HAS_T0B7__) \
 || defined(__MSP430_HAS_T1B3__) || defined(__MSP430_HAS_T1B5__) || defined(__MSP430_HAS_T1B7__)
#define __MSP430_HAS_TxB7__
#endif
#if defined(__MSP430_HAS_T0D3__) || defined(__MSP430_HAS_T0D5__) || defined(__MSP430_HAS_T0D7__) \
 || defined(__MSP430_HAS_T1D3__) || defined(__MSP430_HAS_T1D5__) || defined(__MSP430_HAS_T1D7__)
#define __MSP430_HAS_TxD7__
#endif
#if defined(__MSP430_HAS_USCI_A0__) || defined(__MSP430_HAS_USCI_A1__) || defined(__MSP430_HAS_USCI_A2__) || defined(__MSP430_HAS_USCI_A3__)
#define __MSP430_HAS_USCI_Ax__
#endif
#if defined(__MSP430_HAS_USCI_B0__) || defined(__MSP430_HAS_USCI_B1__) || defined(__MSP430_HAS_USCI_B2__) || defined(__MSP430_HAS_USCI_B3__)
#define __MSP430_HAS_USCI_Bx__
#endif
#if defined(__MSP430_HAS_EUSCI_A0__) || defined(__MSP430_HAS_EUSCI_A1__) || defined(__MSP430_HAS_EUSCI_A2__) || defined(__MSP430_HAS_EUSCI_A3__)
#define __MSP430_HAS_EUSCI_Ax__
#endif
#if defined(__MSP430_HAS_EUSCI_B0__) || defined(__MSP430_HAS_EUSCI_B1__) || defined(__MSP430_HAS_EUSCI_B2__) || defined(__MSP430_HAS_EUSCI_B3__)
#define __MSP430_HAS_EUSCI_Bx__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
#define __MSP430_HAS_EUSCI_Bx__
#endif

/************************************************************
* ADC
************************************************************/
#ifdef  __MSP430_HAS_ADC__             /* Definition to show that Module is available */

#define OFS_ADCCTL0           (0x0000)   /* ADC Control 0 */
#define OFS_ADCCTL0_L          OFS_ADCCTL0
#define OFS_ADCCTL0_H          OFS_ADCCTL0+1
#define OFS_ADCCTL1           (0x0002)   /* ADC Control 1 */
#define OFS_ADCCTL1_L          OFS_ADCCTL1
#define OFS_ADCCTL1_H          OFS_ADCCTL1+1
#define OFS_ADCCTL2           (0x0004)   /* ADC Control 2 */
#define OFS_ADCCTL2_L          OFS_ADCCTL2
#define OFS_ADCCTL2_H          OFS_ADCCTL2+1
#define OFS_ADCLO             (0x0006)   /* ADC Window Comparator High Threshold */
#define OFS_ADCLO_L            OFS_ADCLO
#define OFS_ADCLO_H            OFS_ADCLO+1
#define OFS_ADCHI             (0x0008)   /* ADC Window Comparator High Threshold */
#define OFS_ADCHI_L            OFS_ADCHI
#define OFS_ADCHI_H            OFS_ADCHI+1
#define OFS_ADCMCTL0          (0x000A)   /* ADC Memory Control 0 */
#define OFS_ADCMCTL0_L         OFS_ADCMCTL0
#define OFS_ADCMCTL0_H         OFS_ADCMCTL0+1
#define OFS_ADCMEM0           (0x0012)   /* ADC Conversion Memory 0 */
#define OFS_ADCMEM0_L          OFS_ADCMEM0
#define OFS_ADCMEM0_H          OFS_ADCMEM0+1
#define OFS_ADCIE             (0x001A)   /* ADC Interrupt Enable */
#define OFS_ADCIE_L            OFS_ADCIE
#define OFS_ADCIE_H            OFS_ADCIE+1
#define OFS_ADCIFG            (0x001C)   /* ADC Interrupt Flag */
#define OFS_ADCIFG_L           OFS_ADCIFG
#define OFS_ADCIFG_H           OFS_ADCIFG+1
#define OFS_ADCIV             (0x001E)   /* ADC Interrupt Vector Word */
#define OFS_ADCIV_L            OFS_ADCIV
#define OFS_ADCIV_H            OFS_ADCIV+1

/* ADCCTL0 Control Bits */
#define ADCSC               (0x0001)   /* ADC Start Conversion */
#define ADCENC              (0x0002)   /* ADC Enable Conversion */
#define ADCON               (0x0010)   /* ADC On/enable */
#define ADCMSC              (0x0080)   /* ADC Multiple SampleConversion */
#define ADCSHT0             (0x0100)   /* ADC Sample Hold Select Bit: 0 */
#define ADCSHT1             (0x0200)   /* ADC Sample Hold Select Bit: 1 */
#define ADCSHT2             (0x0400)   /* ADC Sample Hold Select Bit: 2 */
#define ADCSHT3             (0x0800)   /* ADC Sample Hold Select Bit: 3 */

/* ADCCTL0 Control Bits */
#define ADCSC_L             (0x0001)   /* ADC Start Conversion */
#define ADCENC_L            (0x0002)   /* ADC Enable Conversion */
#define ADCON_L             (0x0010)   /* ADC On/enable */
#define ADCMSC_L            (0x0080)   /* ADC Multiple SampleConversion */

/* ADCCTL0 Control Bits */
#define ADCSHT0_H           (0x0001)   /* ADC Sample Hold Select Bit: 0 */
#define ADCSHT1_H           (0x0002)   /* ADC Sample Hold Select Bit: 1 */
#define ADCSHT2_H           (0x0004)   /* ADC Sample Hold Select Bit: 2 */
#define ADCSHT3_H           (0x0008)   /* ADC Sample Hold Select Bit: 3 */

#define ADCSHT_0            (0x0000)  /* ADC Sample Hold Select 0 */
#define ADCSHT_1            (0x0100)  /* ADC Sample Hold Select 1 */
#define ADCSHT_2            (0x0200)  /* ADC Sample Hold Select 2 */
#define ADCSHT_3            (0x0300)  /* ADC Sample Hold Select 3 */
#define ADCSHT_4            (0x0400)  /* ADC Sample Hold Select 4 */
#define ADCSHT_5            (0x0500)  /* ADC Sample Hold Select 5 */
#define ADCSHT_6            (0x0600)  /* ADC Sample Hold Select 6 */
#define ADCSHT_7            (0x0700)  /* ADC Sample Hold Select 7 */
#define ADCSHT_8            (0x0800)  /* ADC Sample Hold Select 8 */
#define ADCSHT_9            (0x0900)  /* ADC Sample Hold Select 9 */
#define ADCSHT_10           (0x0A00) /* ADC Sample Hold Select 10 */
#define ADCSHT_11           (0x0B00) /* ADC Sample Hold Select 11 */
#define ADCSHT_12           (0x0C00) /* ADC Sample Hold Select 12 */
#define ADCSHT_13           (0x0D00) /* ADC Sample Hold Select 13 */
#define ADCSHT_14           (0x0E00) /* ADC Sample Hold Select 14 */
#define ADCSHT_15           (0x0F00) /* ADC Sample Hold Select 15 */

/* ADCCTL1 Control Bits */
#define ADCBUSY             (0x0001)    /* ADC Busy */
#define ADCCONSEQ0          (0x0002)    /* ADC Conversion Sequence Select 0 */
#define ADCCONSEQ1          (0x0004)    /* ADC Conversion Sequence Select 1 */
#define ADCSSEL0            (0x0008)    /* ADC Clock Source Select 0 */
#define ADCSSEL1            (0x0010)    /* ADC Clock Source Select 1 */
#define ADCDIV0             (0x0020)    /* ADC Clock Divider Select 0 */
#define ADCDIV1             (0x0040)    /* ADC Clock Divider Select 1 */
#define ADCDIV2             (0x0080)    /* ADC Clock Divider Select 2 */
#define ADCISSH             (0x0100)    /* ADC Invert Sample Hold Signal */
#define ADCSHP              (0x0200)    /* ADC Sample/Hold Pulse Mode */
#define ADCSHS0             (0x0400)    /* ADC Sample/Hold Source 0 */
#define ADCSHS1             (0x0800)    /* ADC Sample/Hold Source 1 */

/* ADCCTL1 Control Bits */
#define ADCBUSY_L           (0x0001)    /* ADC Busy */
#define ADCCONSEQ0_L        (0x0002)    /* ADC Conversion Sequence Select 0 */
#define ADCCONSEQ1_L        (0x0004)    /* ADC Conversion Sequence Select 1 */
#define ADCSSEL0_L          (0x0008)    /* ADC Clock Source Select 0 */
#define ADCSSEL1_L          (0x0010)    /* ADC Clock Source Select 1 */
#define ADCDIV0_L           (0x0020)    /* ADC Clock Divider Select 0 */
#define ADCDIV1_L           (0x0040)    /* ADC Clock Divider Select 1 */
#define ADCDIV2_L           (0x0080)    /* ADC Clock Divider Select 2 */

/* ADCCTL1 Control Bits */
#define ADCISSH_H           (0x0001)    /* ADC Invert Sample Hold Signal */
#define ADCSHP_H            (0x0002)    /* ADC Sample/Hold Pulse Mode */
#define ADCSHS0_H           (0x0004)    /* ADC Sample/Hold Source 0 */
#define ADCSHS1_H           (0x0008)    /* ADC Sample/Hold Source 1 */

#define ADCCONSEQ_0         (0x0000)       /* ADC Conversion Sequence Select: 0 */
#define ADCCONSEQ_1         (0x0002)       /* ADC Conversion Sequence Select: 1 */
#define ADCCONSEQ_2         (0x0004)       /* ADC Conversion Sequence Select: 2 */
#define ADCCONSEQ_3         (0x0006)       /* ADC Conversion Sequence Select: 3 */

#define ADCSSEL_0           (0x0000)       /* ADC Clock Source Select: 0 */
#define ADCSSEL_1           (0x0008)       /* ADC Clock Source Select: 1 */
#define ADCSSEL_2           (0x0010)       /* ADC Clock Source Select: 2 */
#define ADCSSEL_3           (0x0018)       /* ADC Clock Source Select: 3 */

#define ADCDIV_0            (0x0000)    /* ADC Clock Divider Select: 0 */
#define ADCDIV_1            (0x0020)    /* ADC Clock Divider Select: 1 */
#define ADCDIV_2            (0x0040)    /* ADC Clock Divider Select: 2 */
#define ADCDIV_3            (0x0060)    /* ADC Clock Divider Select: 3 */
#define ADCDIV_4            (0x0080)    /* ADC Clock Divider Select: 4 */
#define ADCDIV_5            (0x00A0)    /* ADC Clock Divider Select: 5 */
#define ADCDIV_6            (0x00C0)    /* ADC Clock Divider Select: 6 */
#define ADCDIV_7            (0x00E0)    /* ADC Clock Divider Select: 7 */

#define ADCSHS_0            (0x0000)   /* ADC Sample/Hold Source: 0 */
#define ADCSHS_1            (0x0400)   /* ADC Sample/Hold Source: 1 */
#define ADCSHS_2            (0x0800)   /* ADC Sample/Hold Source: 2 */
#define ADCSHS_3            (0x0C00)   /* ADC Sample/Hold Source: 3 */

/* ADCCTL2 Control Bits */
#define ADCSR               (0x0004)    /* ADC Sampling Rate */
#define ADCDF               (0x0008)    /* ADC Data Format */
#define ADCRES              (0x0010)    /* ADC Resolution Bit */
#define ADCPDIV0            (0x0100)    /* ADC predivider Bit: 0 */
#define ADCPDIV1            (0x0200)    /* ADC predivider Bit: 1 */

/* ADCCTL2 Control Bits */
#define ADCSR_L             (0x0004)    /* ADC Sampling Rate */
#define ADCDF_L             (0x0008)    /* ADC Data Format */
#define ADCRES_L            (0x0010)    /* ADC Resolution Bit */

/* ADCCTL2 Control Bits */
#define ADCPDIV0_H          (0x0001)    /* ADC predivider Bit: 0 */
#define ADCPDIV1_H          (0x0002)    /* ADC predivider Bit: 1 */

#define ADCPDIV_0           (0x0000)    /* ADC predivider /1 */
#define ADCPDIV_1           (0x0100)    /* ADC predivider /2 */
#define ADCPDIV_2           (0x0200)    /* ADC predivider /64 */
#define ADCPDIV_3           (0x0300)    /* ADC predivider reserved */

#define ADCPDIV__1          (0x0000)    /* ADC predivider /1 */
#define ADCPDIV__4          (0x0100)    /* ADC predivider /2 */
#define ADCPDIV__64         (0x0200)    /* ADC predivider /64 */

/* ADCMCTL0 Control Bits */
#define ADCINCH0            (0x0001)    /* ADC Input Channel Select Bit 0 */
#define ADCINCH1            (0x0002)    /* ADC Input Channel Select Bit 1 */
#define ADCINCH2            (0x0004)    /* ADC Input Channel Select Bit 2 */
#define ADCINCH3            (0x0008)    /* ADC Input Channel Select Bit 3 */
#define ADCSREF0            (0x0010)    /* ADC Select Reference Bit 0 */
#define ADCSREF1            (0x0020)    /* ADC Select Reference Bit 1 */
#define ADCSREF2            (0x0040)    /* ADC Select Reference Bit 2 */

/* ADCMCTL0 Control Bits */
#define ADCINCH0_L          (0x0001)    /* ADC Input Channel Select Bit 0 */
#define ADCINCH1_L          (0x0002)    /* ADC Input Channel Select Bit 1 */
#define ADCINCH2_L          (0x0004)    /* ADC Input Channel Select Bit 2 */
#define ADCINCH3_L          (0x0008)    /* ADC Input Channel Select Bit 3 */
#define ADCSREF0_L          (0x0010)    /* ADC Select Reference Bit 0 */
#define ADCSREF1_L          (0x0020)    /* ADC Select Reference Bit 1 */
#define ADCSREF2_L          (0x0040)    /* ADC Select Reference Bit 2 */


#define ADCINCH_0           (0)         /* ADC Input Channel 0 */
#define ADCINCH_1           (1)         /* ADC Input Channel 1 */
#define ADCINCH_2           (2)         /* ADC Input Channel 2 */
#define ADCINCH_3           (3)         /* ADC Input Channel 3 */
#define ADCINCH_4           (4)         /* ADC Input Channel 4 */
#define ADCINCH_5           (5)         /* ADC Input Channel 5 */
#define ADCINCH_6           (6)         /* ADC Input Channel 6 */
#define ADCINCH_7           (7)         /* ADC Input Channel 7 */
#define ADCINCH_8           (8)         /* ADC Input Channel 8 */
#define ADCINCH_9           (9)         /* ADC Input Channel 9 */
#define ADCINCH_10          (10)        /* ADC Input Channel 10 */
#define ADCINCH_11          (11)        /* ADC Input Channel 11 */
#define ADCINCH_12          (12)        /* ADC Input Channel 12 */
#define ADCINCH_13          (13)        /* ADC Input Channel 13 */
#define ADCINCH_14          (14)        /* ADC Input Channel 14 */
#define ADCINCH_15          (15)        /* ADC Input Channel 15 */

#define ADCSREF_0           (0x0000)    /* ADC Select Reference 0 */
#define ADCSREF_1           (0x0010)    /* ADC Select Reference 1 */
#define ADCSREF_2           (0x0020)    /* ADC Select Reference 2 */
#define ADCSREF_3           (0x0030)    /* ADC Select Reference 3 */
#define ADCSREF_4           (0x0040)    /* ADC Select Reference 4 */
#define ADCSREF_5           (0x0050)    /* ADC Select Reference 5 */
#define ADCSREF_6           (0x0060)    /* ADC Select Reference 6 */
#define ADCSREF_7           (0x0070)    /* ADC Select Reference 7 */

/* ADCIE Interrupt Enable Bits */
#define ADCIE0              (0x0001)    /* ADC Interrupt enable */
#define ADCINIE             (0x0002)    /* ADC Interrupt enable for the inside of window of the Window comparator */
#define ADCLOIE             (0x0004)    /* ADC Interrupt enable for lower threshold of the Window comparator */
#define ADCHIIE             (0x0008)    /* ADC Interrupt enable for upper threshold of the Window comparator */
#define ADCOVIE             (0x0010)    /* ADC ADCMEM overflow Interrupt enable */
#define ADCTOVIE            (0x0020)    /* ADC conversion-time-overflow Interrupt enable */

/* ADCIE Interrupt Enable Bits */
#define ADCIE0_L            (0x0001)    /* ADC Interrupt enable */
#define ADCINIE_L           (0x0002)    /* ADC Interrupt enable for the inside of window of the Window comparator */
#define ADCLOIE_L           (0x0004)    /* ADC Interrupt enable for lower threshold of the Window comparator */
#define ADCHIIE_L           (0x0008)    /* ADC Interrupt enable for upper threshold of the Window comparator */
#define ADCOVIE_L           (0x0010)    /* ADC ADCMEM overflow Interrupt enable */
#define ADCTOVIE_L          (0x0020)    /* ADC conversion-time-overflow Interrupt enable */


/* ADCIFG Interrupt Flag Bits */
#define ADCIFG0             (0x0001)    /* ADC Interrupt Flag */
#define ADCINIFG            (0x0002)    /* ADC Interrupt Flag for the inside of window of the Window comparator */
#define ADCLOIFG            (0x0004)    /* ADC Interrupt Flag for lower threshold of the Window comparator */
#define ADCHIIFG            (0x0008)    /* ADC Interrupt Flag for upper threshold of the Window comparator */
#define ADCOVIFG            (0x0010)    /* ADC ADCMEM overflow Interrupt Flag */
#define ADCTOVIFG           (0x0020)    /* ADC conversion-time-overflow Interrupt Flag */

/* ADCIFG Interrupt Flag Bits */
#define ADCIFG0_L           (0x0001)    /* ADC Interrupt Flag */
#define ADCINIFG_L          (0x0002)    /* ADC Interrupt Flag for the inside of window of the Window comparator */
#define ADCLOIFG_L          (0x0004)    /* ADC Interrupt Flag for lower threshold of the Window comparator */
#define ADCHIIFG_L          (0x0008)    /* ADC Interrupt Flag for upper threshold of the Window comparator */
#define ADCOVIFG_L          (0x0010)    /* ADC ADCMEM overflow Interrupt Flag */
#define ADCTOVIFG_L         (0x0020)    /* ADC conversion-time-overflow Interrupt Flag */


/* ADCIV Definitions */
#define ADCIV_NONE          (0x0000)    /* No Interrupt pending */
#define ADCIV_ADCOVIFG      (0x0002)    /* ADCOVIFG */
#define ADCIV_ADCTOVIFG     (0x0004)    /* ADCTOVIFG */
#define ADCIV_ADCHIIFG      (0x0006)    /* ADCHIIFG */
#define ADCIV_ADCLOIFG      (0x0008)    /* ADCLOIFG */
#define ADCIV_ADCINIFG      (0x000A)    /* ADCINIFG */
#define ADCIV_ADCIFG        (0x000C)    /* ADCIFG */

#endif
/************************************************************
* Capacitive_Touch_IO 0
************************************************************/
#ifdef  __MSP430_HAS_CAP_TOUCH_IO_0__ /* Definition to show that Module is available */

#define OFS_CAPTIO0CTL        (0x000E)  /* Capacitive_Touch_IO 0 control register */
#define OFS_CAPTIO0CTL_L       OFS_CAPTIO0CTL
#define OFS_CAPTIO0CTL_H       OFS_CAPTIO0CTL+1

#define  CAPSIO0CTL         CAPTIO0CTL  /* legacy define */

/* CAPTIOxCTL Control Bits */
#define CAPTIOPISEL0         (0x0002)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPTIOPISEL1         (0x0004)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPTIOPISEL2         (0x0008)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPTIOPOSEL0         (0x0010)    /* CapTouchIO Port Select Bit: 0 */
#define CAPTIOPOSEL1         (0x0020)    /* CapTouchIO Port Select Bit: 1 */
#define CAPTIOPOSEL2         (0x0040)    /* CapTouchIO Port Select Bit: 2 */
#define CAPTIOPOSEL3         (0x0080)    /* CapTouchIO Port Select Bit: 3 */
#define CAPTIOEN             (0x0100)    /* CapTouchIO Enable */
#define CAPTIO               (0x0200)    /* CapTouchIO state */

/* CAPTIOxCTL Control Bits */
#define CAPTIOPISEL0_L      (0x0002)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPTIOPISEL1_L      (0x0004)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPTIOPISEL2_L      (0x0008)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPTIOPOSEL0_L      (0x0010)    /* CapTouchIO Port Select Bit: 0 */
#define CAPTIOPOSEL1_L      (0x0020)    /* CapTouchIO Port Select Bit: 1 */
#define CAPTIOPOSEL2_L      (0x0040)    /* CapTouchIO Port Select Bit: 2 */
#define CAPTIOPOSEL3_L      (0x0080)    /* CapTouchIO Port Select Bit: 3 */

/* CAPTIOxCTL Control Bits */
#define CAPTIOEN_H          (0x0001)    /* CapTouchIO Enable */
#define CAPTIO_H            (0x0002)    /* CapTouchIO state */

/* Legacy defines */
#define CAPSIOPISEL0         (0x0002)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPSIOPISEL1         (0x0004)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPSIOPISEL2         (0x0008)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPSIOPOSEL0         (0x0010)    /* CapTouchIO Port Select Bit: 0 */
#define CAPSIOPOSEL1         (0x0020)    /* CapTouchIO Port Select Bit: 1 */
#define CAPSIOPOSEL2         (0x0040)    /* CapTouchIO Port Select Bit: 2 */
#define CAPSIOPOSEL3         (0x0080)    /* CapTouchIO Port Select Bit: 3 */
#define CAPSIOEN             (0x0100)    /* CapTouchIO Enable */
#define CAPSIO               (0x0200)    /* CapTouchIO state */

#endif
/************************************************************
* Capacitive_Touch_IO 1
************************************************************/
#ifdef  __MSP430_HAS_CAP_TOUCH_IO_1__ /* Definition to show that Module is available */

#define OFS_CAPTIO1CTL        (0x000E)  /* Capacitive_Touch_IO 1 control register */
#define OFS_CAPTIO1CTL_L       OFS_CAPTIO1CTL
#define OFS_CAPTIO1CTL_H       OFS_CAPTIO1CTL+1

#define  CAPSIO1CTL         CAPTIO1CTL  /* legacy define */

#endif
/************************************************************
* eComparator
************************************************************/
#ifdef  __MSP430_HAS_ECOMP__            /* Definition to show that Module is available */

#define OFS_CPCTL0            (0x0000)  /* eComparator Control Register 0 */
#define OFS_CPCTL0_L           OFS_CPCTL0
#define OFS_CPCTL0_H           OFS_CPCTL0+1
#define OFS_CPCTL1            (0x0002)  /* eComparator Control Register 1 */
#define OFS_CPCTL1_L           OFS_CPCTL1
#define OFS_CPCTL1_H           OFS_CPCTL1+1
#define OFS_CPINT             (0x000C)  /* eComparator Interrupt Register */
#define OFS_CPINT_L            OFS_CPINT
#define OFS_CPINT_H            OFS_CPINT+1
#define OFS_CPIV              (0x000E)  /* eComparator Interrupt Vector Word */
#define OFS_CPIV_L             OFS_CPIV
#define OFS_CPIV_H             OFS_CPIV+1
#define OFS_CPDACCTL          (0x0010)  /* eComparator Built-in DAC control register */
#define OFS_CPDACCTL_L         OFS_CPDACCTL
#define OFS_CPDACCTL_H         OFS_CPDACCTL+1
#define OFS_CPDACDATA         (0x0012)  /* eComparator Built-in DAC data register */
#define OFS_CPDACDATA_L        OFS_CPDACDATA
#define OFS_CPDACDATA_H        OFS_CPDACDATA+1


/* CPCTL0 Control Bits */
#define CPPSEL0            (0x0001)  /* eComp. Pos. Channel Input Select 0 */
#define CPPSEL1            (0x0002)  /* eComp. Pos. Channel Input Select 1 */
#define CPPSEL2            (0x0004)  /* eComp. Pos. Channel Input Select 2 */
#define CPPEN              (0x0010)  /* eComp. Pos. Channel Input Enable */
#define CPNSEL0            (0x0100)  /* eComp. Neg. Channel Input Select 0 */
#define CPNSEL1            (0x0200)  /* eComp. Neg. Channel Input Select 1 */
#define CPNSEL2            (0x0400)  /* eComp. Neg. Channel Input Select 2 */
#define CPNEN              (0x1000)  /* eComp. Neg. Channel Input Enable */

/* CPCTL0 Control Bits */
#define CPPSEL0_L           (0x0001)  /* eComp. Pos. Channel Input Select 0 */
#define CPPSEL1_L           (0x0002)  /* eComp. Pos. Channel Input Select 1 */
#define CPPSEL2_L           (0x0004)  /* eComp. Pos. Channel Input Select 2 */
#define CPPEN_L             (0x0010)  /* eComp. Pos. Channel Input Enable */

/* CPCTL0 Control Bits */
#define CPNSEL0_H           (0x0001)  /* eComp. Neg. Channel Input Select 0 */
#define CPNSEL1_H           (0x0002)  /* eComp. Neg. Channel Input Select 1 */
#define CPNSEL2_H           (0x0004)  /* eComp. Neg. Channel Input Select 2 */
#define CPNEN_H             (0x0010)  /* eComp. Neg. Channel Input Enable */

#define CPPSEL_0           (0x0000)  /* eComp. V+ terminal Input Select: Channel 0 */
#define CPPSEL_1           (0x0001)  /* eComp. V+ terminal Input Select: Channel 1 */
#define CPPSEL_2           (0x0002)  /* eComp. V+ terminal Input Select: Channel 2 */
#define CPPSEL_3           (0x0003)  /* eComp. V+ terminal Input Select: Channel 3 */
#define CPPSEL_4           (0x0004)  /* eComp. V+ terminal Input Select: Channel 4 */
#define CPPSEL_5           (0x0005)  /* eComp. V+ terminal Input Select: Channel 5 */
#define CPPSEL_6           (0x0006)  /* eComp. V+ terminal Input Select: Channel 6 */
#define CPPSEL_7           (0x0007)  /* eComp. V+ terminal Input Select: Channel 7 */

#define CPNSEL_0           (0x0000)  /* eComp. V- Terminal Input Select: Channel 0 */
#define CPNSEL_1           (0x0100)  /* eComp. V- Terminal Input Select: Channel 1 */
#define CPNSEL_2           (0x0200)  /* eComp. V- Terminal Input Select: Channel 2 */
#define CPNSEL_3           (0x0300)  /* eComp. V- Terminal Input Select: Channel 3 */
#define CPNSEL_4           (0x0400)  /* eComp. V- Terminal Input Select: Channel 4 */
#define CPNSEL_5           (0x0500)  /* eComp. V- Terminal Input Select: Channel 5 */
#define CPNSEL_6           (0x0600)  /* eComp. V- Terminal Input Select: Channel 6 */
#define CPNSEL_7           (0x0700)  /* eComp. V- Terminal Input Select: Channel 7 */

/* CPCTL1 Control Bits */
#define CPOUT               (0x0001)  /* eComp. Output */
#define CPINV               (0x0002)  /* eComp. Output Polarity */
#define CPIES               (0x0010)  /* eComp. Interrupt Edge Select */
#define CPFLT               (0x0020)  /* eComp. Enable Output Filter */
#define CPFILTDLY0          (0x0040)  /* eComp. Filter delay Bit 0 */
#define CPFILTDLY1          (0x0080)  /* eComp. Filter delay Bit 1 */
#define CPMSEL              (0x0100)  /* eComp. Power mode selection */
#define CPEN                (0x0200)  /* eComp. enable */
#define CPHSEL0             (0x0400)  /* eComp. Programable Hysteresis mode Bit: 0 */
#define CPHSEL1             (0x0800)  /* eComp. Programable Hysteresis mode Bit: 1 */
#define CPIE                (0x4000)  /* eComp. Interrupt Enable */
#define CPIIE               (0x8000)  /* eComp. Inverted Interrupt Enable */

/* CPCTL1 Control Bits */
#define CPOUT_L             (0x0001)  /* eComp. Output */
#define CPINV_L             (0x0002)  /* eComp. Output Polarity */
#define CPIES_L             (0x0010)  /* eComp. Interrupt Edge Select */
#define CPFLT_L             (0x0020)  /* eComp. Enable Output Filter */
#define CPFILTDLY0_L        (0x0040)  /* eComp. Filter delay Bit 0 */
#define CPFILTDLY1_L        (0x0080)  /* eComp. Filter delay Bit 1 */

/* CPCTL1 Control Bits */
#define CPMSEL_H            (0x0001)  /* eComp. Power mode selection */
#define CPEN_H              (0x0002)  /* eComp. enable */
#define CPHSEL0_H           (0x0004)  /* eComp. Programable Hysteresis mode Bit: 0 */
#define CPHSEL1_H           (0x0008)  /* eComp. Programable Hysteresis mode Bit: 1 */
#define CPIE_H              (0x0040)  /* eComp. Interrupt Enable */
#define CPIIE_H             (0x0080)  /* eComp. Inverted Interrupt Enable */

#define CPFILTDLY_0        (0x0000)  /* eComp. Filter delay 0 : 450ns */
#define CPFILTDLY_1        (0x0040)  /* eComp. Filter delay 1 : 900ns */
#define CPFILTDLY_2        (0x0080)  /* eComp. Filter delay 2 : 1800ns */
#define CPFILTDLY_3        (0x00C0)  /* eComp. Filter delay 3 : 3600ns */

#define CPHSEL_0           (0x0000)  /* eComp. Programable Hysteresis mode 0 */
#define CPHSEL_1           (0x0400)  /* eComp. Programable Hysteresis mode 1 */
#define CPHSEL_2           (0x0800)  /* eComp. Programable Hysteresis mode 2 */
#define CPHSEL_3           (0x0C00)  /* eComp. Programable Hysteresis mode 3*/


/* CPINT Control Bits */
#define CPIFG                (0x0001)  /* eComp. Interrupt Flag */
#define CPIIFG               (0x0002)  /* eComp. Interrupt Flag Inverted Polarity */

/* CPINT Control Bits */
#define CPIFG_L             (0x0001)  /* eComp. Interrupt Flag */
#define CPIIFG_L            (0x0002)  /* eComp. Interrupt Flag Inverted Polarity */


/* CPIV Definitions */
#define CPIV_NONE           (0x0000)    /* No Interrupt pending */
#define CPIV_CPIFG          (0x0002)    /* CPIFG */
#define CPIV_CPIIFG         (0x0004)    /* CPIIFG */

/* CPDACCTL Control Bits */
#define CPDACSW             (0x0001)  /* CPDACBUFx select */
#define CPDACBUFS           (0x0002)  /* eComparator built-in DAC buffer controlled source selection */
#define CPDACREFS           (0x0004)  /* eComparator built-in DAC reference voltage selection */
#define CPDACEN             (0x0080)  /* eComparator built-in DAC output enable */

/* CPDACCTL Control Bits */
#define CPDACSW_L           (0x0001)  /* CPDACBUFx select */
#define CPDACBUFS_L         (0x0002)  /* eComparator built-in DAC buffer controlled source selection */
#define CPDACREFS_L         (0x0004)  /* eComparator built-in DAC reference voltage selection */
#define CPDACEN_L           (0x0080)  /* eComparator built-in DAC output enable */


/* CPDACDATA Control Bits */


#endif
/*************************************************************
* CRC Module
*************************************************************/
#ifdef  __MSP430_HAS_CRC__            /* Definition to show that Module is available */

#define OFS_CRCDI             (0x0000)  /* CRC Data In Register */
#define OFS_CRCDI_L            OFS_CRCDI
#define OFS_CRCDI_H            OFS_CRCDI+1
#define OFS_CRCDIRB           (0x0002)  /* CRC data in reverse byte Register */
#define OFS_CRCDIRB_L          OFS_CRCDIRB
#define OFS_CRCDIRB_H          OFS_CRCDIRB+1
#define OFS_CRCINIRES         (0x0004)  /* CRC Initialisation Register and Result Register */
#define OFS_CRCINIRES_L        OFS_CRCINIRES
#define OFS_CRCINIRES_H        OFS_CRCINIRES+1
#define OFS_CRCRESR           (0x0006)  /* CRC reverse result Register */
#define OFS_CRCRESR_L          OFS_CRCRESR
#define OFS_CRCRESR_H          OFS_CRCRESR+1

#endif
/************************************************************
* CLOCK SYSTEM CONTROL
************************************************************/
#ifdef  __MSP430_HAS_CS__             /* Definition to show that Module is available */

#define OFS_CSCTL0            (0x0000)   /* CS Control Register 0 */
#define OFS_CSCTL0_L           OFS_CSCTL0
#define OFS_CSCTL0_H           OFS_CSCTL0+1
#define OFS_CSCTL1            (0x0002)   /* CS Control Register 1 */
#define OFS_CSCTL1_L           OFS_CSCTL1
#define OFS_CSCTL1_H           OFS_CSCTL1+1
#define OFS_CSCTL2            (0x0004)   /* CS Control Register 2 */
#define OFS_CSCTL2_L           OFS_CSCTL2
#define OFS_CSCTL2_H           OFS_CSCTL2+1
#define OFS_CSCTL3            (0x0006)   /* CS Control Register 3 */
#define OFS_CSCTL3_L           OFS_CSCTL3
#define OFS_CSCTL3_H           OFS_CSCTL3+1
#define OFS_CSCTL4            (0x0008)   /* CS Control Register 4 */
#define OFS_CSCTL4_L           OFS_CSCTL4
#define OFS_CSCTL4_H           OFS_CSCTL4+1
#define OFS_CSCTL5            (0x000A)   /* CS Control Register 5 */
#define OFS_CSCTL5_L           OFS_CSCTL5
#define OFS_CSCTL5_H           OFS_CSCTL5+1
#define OFS_CSCTL6            (0x000C)   /* CS Control Register 6 */
#define OFS_CSCTL6_L           OFS_CSCTL6
#define OFS_CSCTL6_H           OFS_CSCTL6+1
#define OFS_CSCTL7            (0x000E)   /* CS Control Register 7 */
#define OFS_CSCTL7_L           OFS_CSCTL7
#define OFS_CSCTL7_H           OFS_CSCTL7+1
#define OFS_CSCTL8            (0x0010)   /* CS Control Register 8 */
#define OFS_CSCTL8_L           OFS_CSCTL8
#define OFS_CSCTL8_H           OFS_CSCTL8+1

/* CSCTL0 Control Bits */
#define DCO0                (0x0001)    /* DCO TAP Bit : 0 */
#define DCO1                (0x0002)    /* DCO TAP Bit : 1 */
#define DCO2                (0x0004)    /* DCO TAP Bit : 2 */
#define DCO3                (0x0008)    /* DCO TAP Bit : 3 */
#define DCO4                (0x0010)    /* DCO TAP Bit : 4 */
#define DCO5                (0x0020)    /* DCO TAP Bit : 5 */
#define DCO6                (0x0040)    /* DCO TAP Bit : 6 */
#define DCO7                (0x0080)    /* DCO TAP Bit : 7 */
#define DCO8                (0x0100)    /* DCO TAP Bit : 8 */
#define MOD0                (0x0200)    /* Modulation Bit Counter Bit : 0 */
#define MOD1                (0x0400)    /* Modulation Bit Counter Bit : 1 */
#define MOD2                (0x0800)    /* Modulation Bit Counter Bit : 2 */
#define MOD3                (0x1000)    /* Modulation Bit Counter Bit : 3 */
#define MOD4                (0x2000)    /* Modulation Bit Counter Bit : 4 */

/* CSCTL0 Control Bits */
#define DCO0_L              (0x0001)    /* DCO TAP Bit : 0 */
#define DCO1_L              (0x0002)    /* DCO TAP Bit : 1 */
#define DCO2_L              (0x0004)    /* DCO TAP Bit : 2 */
#define DCO3_L              (0x0008)    /* DCO TAP Bit : 3 */
#define DCO4_L              (0x0010)    /* DCO TAP Bit : 4 */
#define DCO5_L              (0x0020)    /* DCO TAP Bit : 5 */
#define DCO6_L              (0x0040)    /* DCO TAP Bit : 6 */
#define DCO7_L              (0x0080)    /* DCO TAP Bit : 7 */

/* CSCTL0 Control Bits */
#define DCO8_H              (0x0001)    /* DCO TAP Bit : 8 */
#define MOD0_H              (0x0002)    /* Modulation Bit Counter Bit : 0 */
#define MOD1_H              (0x0004)    /* Modulation Bit Counter Bit : 1 */
#define MOD2_H              (0x0008)    /* Modulation Bit Counter Bit : 2 */
#define MOD3_H              (0x0010)    /* Modulation Bit Counter Bit : 3 */
#define MOD4_H              (0x0020)    /* Modulation Bit Counter Bit : 4 */

/* CSCTL1 Control Bits */
#define DISMOD              (0x0001)    /* Disable Modulation */
#define DCORSEL0            (0x0002)    /* DCO frequency range select Bit: 0 */
#define DCORSEL1            (0x0004)    /* DCO frequency range select Bit: 1 */
#define DCORSEL2            (0x0008)    /* DCO frequency range select Bit: 2 */
#define DCOFTRIM0           (0x0010)    /* DCO frequency trim. Bit: 0 */
#define DCOFTRIM1           (0x0020)    /* DCO frequency trim. Bit: 1 */
#define DCOFTRIM2           (0x0040)    /* DCO frequency trim. Bit: 2 */
#define DCOFTRIMEN          (0x0080)    /* DCO frequency trim enable */

/* CSCTL1 Control Bits */
#define DISMOD_L            (0x0001)    /* Disable Modulation */
#define DCORSEL0_L          (0x0002)    /* DCO frequency range select Bit: 0 */
#define DCORSEL1_L          (0x0004)    /* DCO frequency range select Bit: 1 */
#define DCORSEL2_L          (0x0008)    /* DCO frequency range select Bit: 2 */
#define DCOFTRIM0_L         (0x0010)    /* DCO frequency trim. Bit: 0 */
#define DCOFTRIM1_L         (0x0020)    /* DCO frequency trim. Bit: 1 */
#define DCOFTRIM2_L         (0x0040)    /* DCO frequency trim. Bit: 2 */
#define DCOFTRIMEN_L        (0x0080)    /* DCO frequency trim enable */


#define DCORSEL_0           (0x0000)    /* DCO frequency range select: 0 */
#define DCORSEL_1           (0x0002)    /* DCO frequency range select: 1 */
#define DCORSEL_2           (0x0004)    /* DCO frequency range select: 2 */
#define DCORSEL_3           (0x0006)    /* DCO frequency range select: 3 */
#define DCORSEL_4           (0x0008)    /* DCO frequency range select: 4 */
#define DCORSEL_5           (0x000A)    /* DCO frequency range select: 5 */
#define DCORSEL_6           (0x000C)    /* DCO frequency range select: 6 */
#define DCORSEL_7           (0x000E)    /* DCO frequency range select: 7 */

#define DCOFTRIM_0          (0x0000)    /* DCO frequency trim: 0 */
#define DCOFTRIM_1          (0x0010)    /* DCO frequency trim: 1 */
#define DCOFTRIM_2          (0x0020)    /* DCO frequency trim: 2 */
#define DCOFTRIM_3          (0x0030)    /* DCO frequency trim: 3 */
#define DCOFTRIM_4          (0x0040)    /* DCO frequency trim: 4 */
#define DCOFTRIM_5          (0x0050)    /* DCO frequency trim: 5 */
#define DCOFTRIM_6          (0x0060)    /* DCO frequency trim: 6 */
#define DCOFTRIM_7          (0x0070)    /* DCO frequency trim: 7 */

/* CSCTL2 Control Bits */
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
#define FLLD0               (0x1000)    /* Loop Divider Bit : 0 */
#define FLLD1               (0x2000)    /* Loop Divider Bit : 1 */
#define FLLD2               (0x4000)    /* Loop Divider Bit : 1 */

/* CSCTL2 Control Bits */
#define FLLN0_L             (0x0001)    /* FLL Multipier Bit : 0 */
#define FLLN1_L             (0x0002)    /* FLL Multipier Bit : 1 */
#define FLLN2_L             (0x0004)    /* FLL Multipier Bit : 2 */
#define FLLN3_L             (0x0008)    /* FLL Multipier Bit : 3 */
#define FLLN4_L             (0x0010)    /* FLL Multipier Bit : 4 */
#define FLLN5_L             (0x0020)    /* FLL Multipier Bit : 5 */
#define FLLN6_L             (0x0040)    /* FLL Multipier Bit : 6 */
#define FLLN7_L             (0x0080)    /* FLL Multipier Bit : 7 */

/* CSCTL2 Control Bits */
#define FLLN8_H             (0x0001)    /* FLL Multipier Bit : 8 */
#define FLLN9_H             (0x0002)    /* FLL Multipier Bit : 9 */
#define FLLD0_H             (0x0010)    /* Loop Divider Bit : 0 */
#define FLLD1_H             (0x0020)    /* Loop Divider Bit : 1 */
#define FLLD2_H             (0x0040)    /* Loop Divider Bit : 1 */

#define FLLD_0             (0x0000)    /* Multiply Selected Loop Freq. By 1 */
#define FLLD_1             (0x1000)    /* Multiply Selected Loop Freq. By 2 */
#define FLLD_2             (0x2000)    /* Multiply Selected Loop Freq. By 4 */
#define FLLD_3             (0x3000)    /* Multiply Selected Loop Freq. By 8 */
#define FLLD_4             (0x4000)    /* Multiply Selected Loop Freq. By 16 */
#define FLLD_5             (0x5000)    /* Multiply Selected Loop Freq. By 32 */
#define FLLD_6             (0x6000)    /* Reserved */
#define FLLD_7             (0x7000)    /* Reserved */
#define FLLD__1            (0x0000)    /* Multiply Selected Loop Freq. By 1 */
#define FLLD__2            (0x1000)    /* Multiply Selected Loop Freq. By 2 */
#define FLLD__4            (0x2000)    /* Multiply Selected Loop Freq. By 4 */
#define FLLD__8            (0x3000)    /* Multiply Selected Loop Freq. By 8 */
#define FLLD__16           (0x4000)    /* Multiply Selected Loop Freq. By 16 */
#define FLLD__32           (0x5000)    /* Multiply Selected Loop Freq. By 32 */


/* CSCTL3 Control Bits */
#define FLLREFDIV0          (0x0001)    /* Reference Divider Bit : 0 */
#define FLLREFDIV1          (0x0002)    /* Reference Divider Bit : 1 */
#define FLLREFDIV2          (0x0004)    /* Reference Divider Bit : 2 */
#define SELREF0             (0x0010)    /* FLL Reference Clock Select Bit : 0 */
#define SELREF1             (0x0020)    /* FLL Reference Clock Select Bit : 1 */

/* CSCTL3 Control Bits */
#define FLLREFDIV0_L        (0x0001)    /* Reference Divider Bit : 0 */
#define FLLREFDIV1_L        (0x0002)    /* Reference Divider Bit : 1 */
#define FLLREFDIV2_L        (0x0004)    /* Reference Divider Bit : 2 */
#define SELREF0_L           (0x0010)    /* FLL Reference Clock Select Bit : 0 */
#define SELREF1_L           (0x0020)    /* FLL Reference Clock Select Bit : 1 */


#define FLLREFDIV_0         (0x0000)    /* Reference Divider: f(LFCLK)/1 */
#define FLLREFDIV_1         (0x0001)    /* Reference Divider: f(LFCLK)/2 */
#define FLLREFDIV_2         (0x0002)    /* Reference Divider: f(LFCLK)/4 */
#define FLLREFDIV_3         (0x0003)    /* Reference Divider: f(LFCLK)/8 */
#define FLLREFDIV_4         (0x0004)    /* Reference Divider: f(LFCLK)/12 */
#define FLLREFDIV_5         (0x0005)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV_6         (0x0006)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV_7         (0x0007)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV__1        (0x0000)    /* Reference Divider: f(LFCLK)/1   */
#define FLLREFDIV__32       (0x0001)    /* Reference Divider: f(LFCLK)/32  */
#define FLLREFDIV__64       (0x0002)    /* Reference Divider: f(LFCLK)/64  */
#define FLLREFDIV__128      (0x0003)    /* Reference Divider: f(LFCLK)/128 */
#define FLLREFDIV__256      (0x0004)    /* Reference Divider: f(LFCLK)/256 */
#define FLLREFDIV__512      (0x0005)    /* Reference Divider: f(LFCLK)/512 */
#define SELREF_0            (0x0000)    /* FLL Reference Clock Select 0 */
#define SELREF_1            (0x0010)    /* FLL Reference Clock Select 1 */
#define SELREF_2            (0x0020)    /* FLL Reference Clock Select 2 */
#define SELREF_3            (0x0030)    /* FLL Reference Clock Select 3 */
#define SELREF__XT1CLK      (0x0000)    /* Multiply Selected Loop Freq. By XT1CLK */
#define SELREF__REFOCLK     (0x0010)    /* Multiply Selected Loop Freq. By REFOCLK */


/* CSCTL4 Control Bits */
#define SELMS0              (0x0001)   /* MCLK and SMCLK Source Select Bit: 0 */
#define SELMS1              (0x0002)   /* MCLK and SMCLK Source Select Bit: 1 */
#define SELMS2              (0x0004)   /* MCLK and SMCLK Source Select Bit: 2 */
#define SELA                (0x0100)   /* ACLK Source Select Bit: 0 */

/* CSCTL4 Control Bits */
#define SELMS0_L            (0x0001)   /* MCLK and SMCLK Source Select Bit: 0 */
#define SELMS1_L            (0x0002)   /* MCLK and SMCLK Source Select Bit: 1 */
#define SELMS2_L            (0x0004)   /* MCLK and SMCLK Source Select Bit: 2 */

/* CSCTL4 Control Bits */
#define SELA_H              (0x0001)   /* ACLK Source Select Bit: 0 */

#define SELMS_0             (0x0000)   /* MCLK and SMCLK Source Select 0 */
#define SELMS_1             (0x0001)   /* MCLK and SMCLK Source Select 1 */
#define SELMS_2             (0x0002)   /* MCLK and SMCLK Source Select 2 */
#define SELMS_3             (0x0003)   /* MCLK and SMCLK Source Select 3 */
#define SELMS_4             (0x0004)   /* MCLK and SMCLK Source Select 4 */
#define SELMS_5             (0x0005)   /* MCLK and SMCLK Source Select 5 */
#define SELMS_6             (0x0006)   /* MCLK and SMCLK Source Select 6 */
#define SELMS_7             (0x0007)   /* MCLK and SMCLK Source Select 7 */
#define SELMS__DCOCLKDIV    (0x0000)   /* MCLK and SMCLK Source Select DCOCLKDIV */
#define SELMS__REFOCLK      (0x0001)   /* MCLK and SMCLK Source Select REFOCLK */
#define SELMS__XT1CLK       (0x0002)   /* MCLK and SMCLK Source Select XT1CLK */
#define SELMS__VLOCLK       (0x0003)   /* MCLK and SMCLK Source Select VLOCLK */

#define SELA__XT1CLK        (0x0000)   /* ACLK Source Select XT1CLK */
#define SELA__REFOCLK       (0x0100)   /* ACLK Source Select REFOCLK */

/* CSCTL5 Control Bits */
#define DIVM0               (0x0001)   /* MCLK Divider Bit: 0 */
#define DIVM1               (0x0002)   /* MCLK Divider Bit: 1 */
#define DIVM2               (0x0004)   /* MCLK Divider Bit: 2 */
#define DIVS0               (0x0010)   /* SMCLK Divider Bit: 0 */
#define DIVS1               (0x0020)   /* SMCLK Divider Bit: 1 */
#define SMCLKOFF            (0x0100)   /* SMCLK off */
#define VLOAUTOOFF          (0x1000)   /* VLO automatic off enable */

/* CSCTL5 Control Bits */
#define DIVM0_L             (0x0001)   /* MCLK Divider Bit: 0 */
#define DIVM1_L             (0x0002)   /* MCLK Divider Bit: 1 */
#define DIVM2_L             (0x0004)   /* MCLK Divider Bit: 2 */
#define DIVS0_L             (0x0010)   /* SMCLK Divider Bit: 0 */
#define DIVS1_L             (0x0020)   /* SMCLK Divider Bit: 1 */

/* CSCTL5 Control Bits */
#define SMCLKOFF_H          (0x0001)   /* SMCLK off */
#define VLOAUTOOFF_H        (0x0010)   /* VLO automatic off enable */

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
#define DIVM__64            (0x0006)    /* MCLK Source Divider f(MCLK)/64 */
#define DIVM__128           (0x0007)    /* MCLK Source Divider f(MCLK)/128 */

#define DIVS_0              (0x0000)    /* SMCLK Source Divider 0 */
#define DIVS_1              (0x0010)    /* SMCLK Source Divider 1 */
#define DIVS_2              (0x0020)    /* SMCLK Source Divider 2 */
#define DIVS_3              (0x0030)    /* SMCLK Source Divider 3 */
#define DIVS__1             (0x0000)    /* SMCLK Source Divider f(SMCLK)/1 */
#define DIVS__2             (0x0010)    /* SMCLK Source Divider f(SMCLK)/2 */
#define DIVS__4             (0x0020)    /* SMCLK Source Divider f(SMCLK)/4 */
#define DIVS__8             (0x0030)    /* SMCLK Source Divider f(SMCLK)/8 */


/* CSCTL6 Control Bits */
#define XT1AUTOOFF          (0x0001)    /* XT1 automatic off enable */
#define XT1AGCOFF           (0x0002)    /* XT1 Automatic Gain Control (AGC) disable */
#define XT1BYPASS           (0x0010)    /* XT1 bypass mode : 0: internal 1:sourced from external pin */
#define XTS                 (0x0020)    /* 1: Selects high-freq. oscillator */
#define XT1DRIVE0           (0x0040)    /* XT1 Drive Level mode Bit 0 */
#define XT1DRIVE1           (0x0080)    /* XT1 Drive Level mode Bit 1 */

/* CSCTL6 Control Bits */
#define XT1AUTOOFF_L        (0x0001)    /* XT1 automatic off enable */
#define XT1AGCOFF_L         (0x0002)    /* XT1 Automatic Gain Control (AGC) disable */
#define XT1BYPASS_L         (0x0010)    /* XT1 bypass mode : 0: internal 1:sourced from external pin */
#define XTS_L               (0x0020)    /* 1: Selects high-freq. oscillator */
#define XT1DRIVE0_L         (0x0040)    /* XT1 Drive Level mode Bit 0 */
#define XT1DRIVE1_L         (0x0080)    /* XT1 Drive Level mode Bit 1 */


#define XT1DRIVE_0          (0x0000)    /* XT1 Drive Level mode: 0 */
#define XT1DRIVE_1          (0x0040)    /* XT1 Drive Level mode: 1 */
#define XT1DRIVE_2          (0x0080)    /* XT1 Drive Level mode: 2 */
#define XT1DRIVE_3          (0x00C0)    /* XT1 Drive Level mode: 3 */


/* CSCTL7 Control Bits */
#define DCOFFG              (0x0001)    /* DCO fault flag */
#define XT1OFFG             (0x0002)    /* XT1 Low Frequency Oscillator Fault Flag */
#define FLLULIFG            (0x0010)    /* FLL unlock interrupt flag */
#define ENSTFCNT1           (0x0040)    /* Enable start counter for XT1 */
#define FLLUNLOCK0          (0x0100)    /* FLL unlock condition Bit: 0 */
#define FLLUNLOCK1          (0x0200)    /* FLL unlock condition Bit: 1 */
#define FLLUNLOCKHIS0       (0x0400)    /* Unlock history Bit: 0 */
#define FLLUNLOCKHIS1       (0x0800)    /* Unlock history Bit: 1 */
#define FLLULPUC            (0x1000)    /* FLL unlock PUC enable */
#define FLLWARNEN           (0x2000)    /* Warning enable */

/* CSCTL7 Control Bits */
#define DCOFFG_L            (0x0001)    /* DCO fault flag */
#define XT1OFFG_L           (0x0002)    /* XT1 Low Frequency Oscillator Fault Flag */
#define FLLULIFG_L          (0x0010)    /* FLL unlock interrupt flag */
#define ENSTFCNT1_L         (0x0040)    /* Enable start counter for XT1 */

/* CSCTL7 Control Bits */
#define FLLUNLOCK0_H        (0x0001)    /* FLL unlock condition Bit: 0 */
#define FLLUNLOCK1_H        (0x0002)    /* FLL unlock condition Bit: 1 */
#define FLLUNLOCKHIS0_H     (0x0004)    /* Unlock history Bit: 0 */
#define FLLUNLOCKHIS1_H     (0x0008)    /* Unlock history Bit: 1 */
#define FLLULPUC_H          (0x0010)    /* FLL unlock PUC enable */
#define FLLWARNEN_H         (0x0020)    /* Warning enable */

#define FLLUNLOCK_0         (0x0000)    /* FLL unlock condition: 0 */
#define FLLUNLOCK_1         (0x0100)    /* FLL unlock condition: 1 */
#define FLLUNLOCK_2         (0x0200)    /* FLL unlock condition: 2 */
#define FLLUNLOCK_3         (0x0300)    /* FLL unlock condition: 3 */
#define FLLUNLOCKHIS_0      (0x0000)    /* Unlock history: 0 */
#define FLLUNLOCKHIS_1      (0x0400)    /* Unlock history: 1 */
#define FLLUNLOCKHIS_2      (0x0800)    /* Unlock history: 2 */
#define FLLUNLOCKHIS_3      (0x0C00)    /* Unlock history: 3 */

/* CSCTL8 Control Bits */
#define ACLKREQEN           (0x0001)    /* ACLK Clock Request Enable */
#define MCLKREQEN           (0x0002)    /* MCLK Clock Request Enable */
#define SMCLKREQEN          (0x0004)    /* SMCLK Clock Request Enable */
#define MODCLKREQEN         (0x0008)    /* MODOSC Clock Request Enable */

/* CSCTL8 Control Bits */
#define ACLKREQEN_L         (0x0001)    /* ACLK Clock Request Enable */
#define MCLKREQEN_L         (0x0002)    /* MCLK Clock Request Enable */
#define SMCLKREQEN_L        (0x0004)    /* SMCLK Clock Request Enable */
#define MODCLKREQEN_L       (0x0008)    /* MODOSC Clock Request Enable */


#endif
/*************************************************************
* FRAM Memory
*************************************************************/
#ifdef  __MSP430_HAS_FRAM__           /* Definition to show that Module is available */

#define OFS_FRCTL0            (0x0000)  /* FRAM Controller Control 0 */
#define OFS_FRCTL0_L           OFS_FRCTL0
#define OFS_FRCTL0_H           OFS_FRCTL0+1
#define OFS_GCCTL0            (0x0004)  /* General Control 0 */
#define OFS_GCCTL0_L           OFS_GCCTL0
#define OFS_GCCTL0_H           OFS_GCCTL0+1
#define OFS_GCCTL1            (0x0006)  /* General Control 1 */
#define OFS_GCCTL1_L           OFS_GCCTL1
#define OFS_GCCTL1_H           OFS_GCCTL1+1

#define FRCTLPW             (0xA500)  /* FRAM password for write */
#define FRPW                (0x9600)  /* FRAM password returned by read */
#define FWPW                (0xA500)  /* FRAM password for write */
#define FXPW                (0x3300)  /* for use with XOR instruction */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0001)  /* RESERVED */
//#define RESERVED            (0x0002)  /* RESERVED */
//#define RESERVED            (0x0004)  /* RESERVED */
#define NWAITS0             (0x0010)  /* FRAM Wait state control Bit: 0 */
#define NWAITS1             (0x0020)  /* FRAM Wait state control Bit: 1 */
#define NWAITS2             (0x0040)  /* FRAM Wait state control Bit: 2 */
//#define RESERVED            (0x0080)  /* RESERVED */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0001)  /* RESERVED */
//#define RESERVED            (0x0002)  /* RESERVED */
//#define RESERVED            (0x0004)  /* RESERVED */
#define NWAITS0_L           (0x0010)  /* FRAM Wait state control Bit: 0 */
#define NWAITS1_L           (0x0020)  /* FRAM Wait state control Bit: 1 */
#define NWAITS2_L           (0x0040)  /* FRAM Wait state control Bit: 2 */
//#define RESERVED            (0x0080)  /* RESERVED */


#define NWAITS_0            (0x0000)  /* FRAM Wait state control: 0 */
#define NWAITS_1            (0x0010)  /* FRAM Wait state control: 1 */
#define NWAITS_2            (0x0020)  /* FRAM Wait state control: 2 */
#define NWAITS_3            (0x0030)  /* FRAM Wait state control: 3 */
#define NWAITS_4            (0x0040)  /* FRAM Wait state control: 4 */
#define NWAITS_5            (0x0050)  /* FRAM Wait state control: 5 */
#define NWAITS_6            (0x0060)  /* FRAM Wait state control: 6 */
#define NWAITS_7            (0x0070)  /* FRAM Wait state control: 7 */


/* GCCTL0 Control Bits */
//#define RESERVED            (0x0001)  /* RESERVED */
#define FRLPMPWR            (0x0002)  /* FRAM Enable FRAM auto power up after LPM */
#define FRPWR               (0x0004)  /* FRAM Power Control */
#define ACCTEIE             (0x0008)  /* RESERVED */
//#define RESERVED            (0x0010)  /* RESERVED */
#define CBDIE               (0x0020)  /* Enable NMI event if correctable bit error detected */
#define UBDIE               (0x0040)  /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN            (0x0080)  /* Enable Power Up Clear (PUC) reset if FRAM uncorrectable bit error detected */

/* GCCTL0 Control Bits */
//#define RESERVED            (0x0001)  /* RESERVED */
#define FRLPMPWR_L          (0x0002)  /* FRAM Enable FRAM auto power up after LPM */
#define FRPWR_L             (0x0004)  /* FRAM Power Control */
#define ACCTEIE_L           (0x0008)  /* RESERVED */
//#define RESERVED            (0x0010)  /* RESERVED */
#define CBDIE_L             (0x0020)  /* Enable NMI event if correctable bit error detected */
#define UBDIE_L             (0x0040)  /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN_L          (0x0080)  /* Enable Power Up Clear (PUC) reset if FRAM uncorrectable bit error detected */


/* GCCTL1 Control Bits */
//#define RESERVED            (0x0001)  /* RESERVED */
#define CBDIFG              (0x0002)  /* FRAM correctable bit error flag */
#define UBDIFG              (0x0004)  /* FRAM uncorrectable bit error flag */
#define ACCTEIFG            (0x0008)  /* Access time error flag */

/* GCCTL1 Control Bits */
//#define RESERVED            (0x0001)  /* RESERVED */
#define CBDIFG_L            (0x0002)  /* FRAM correctable bit error flag */
#define UBDIFG_L            (0x0004)  /* FRAM uncorrectable bit error flag */
#define ACCTEIFG_L          (0x0008)  /* Access time error flag */


#endif
/************************************************************
* LCD_E
************************************************************/
#ifdef  __MSP430_HAS_LCD_E__          /* Definition to show that Module is available */

#define OFS_LCDCTL0           (0x0000)  /* LCD_E Control Register 0 */
#define OFS_LCDCTL0_L          OFS_LCDCTL0
#define OFS_LCDCTL0_H          OFS_LCDCTL0+1
#define OFS_LCDCTL1           (0x0002)  /* LCD_E Control Register 1 */
#define OFS_LCDCTL1_L          OFS_LCDCTL1
#define OFS_LCDCTL1_H          OFS_LCDCTL1+1
#define OFS_LCDBLKCTL         (0x0004)  /* LCD_E blinking control register */
#define OFS_LCDBLKCTL_L        OFS_LCDBLKCTL
#define OFS_LCDBLKCTL_H        OFS_LCDBLKCTL+1
#define OFS_LCDMEMCTL         (0x0006)  /* LCD_E memory control register */
#define OFS_LCDMEMCTL_L        OFS_LCDMEMCTL
#define OFS_LCDMEMCTL_H        OFS_LCDMEMCTL+1
#define OFS_LCDVCTL           (0x0008)  /* LCD_E Voltage Control Register */
#define OFS_LCDVCTL_L          OFS_LCDVCTL
#define OFS_LCDVCTL_H          OFS_LCDVCTL+1
#define OFS_LCDPCTL0          (0x000A)  /* LCD_E Port Control Register 0 */
#define OFS_LCDPCTL0_L         OFS_LCDPCTL0
#define OFS_LCDPCTL0_H         OFS_LCDPCTL0+1
#define OFS_LCDPCTL1          (0x000C)  /* LCD_E Port Control Register 1 */
#define OFS_LCDPCTL1_L         OFS_LCDPCTL1
#define OFS_LCDPCTL1_H         OFS_LCDPCTL1+1
#define OFS_LCDPCTL2          (0x000E)  /* LCD_E Port Control Register 2 */
#define OFS_LCDPCTL2_L         OFS_LCDPCTL2
#define OFS_LCDPCTL2_H         OFS_LCDPCTL2+1
#define OFS_LCDCSSEL0         (0x0014)  /* LCD_E COM/SEG select register 0 */
#define OFS_LCDCSSEL0_L        OFS_LCDCSSEL0
#define OFS_LCDCSSEL0_H        OFS_LCDCSSEL0+1
#define OFS_LCDCSSEL1         (0x0016)  /* LCD_E COM/SEG select register 1 */
#define OFS_LCDCSSEL1_L        OFS_LCDCSSEL1
#define OFS_LCDCSSEL1_H        OFS_LCDCSSEL1+1
#define OFS_LCDCSSEL2         (0x0018)  /* LCD_E COM/SEG select register 2 */
#define OFS_LCDCSSEL2_L        OFS_LCDCSSEL2
#define OFS_LCDCSSEL2_H        OFS_LCDCSSEL2+1
#define OFS_LCDIV             (0x001E)  /* LCD_E Interrupt Vector Register */

// LCDCTL0
#define LCDON               (0x0001)  /* LCD_E LCD On */
#define LCDLP               (0x0002)  /* LCD_E Low Power Waveform */
#define LCDSON              (0x0004)  /* LCD_E LCD Segments On */
#define LCDMX0              (0x0008)  /* LCD_E Mux Rate Bit: 0 */
#define LCDMX1              (0x0010)  /* LCD_E Mux Rate Bit: 1 */
#define LCDMX2              (0x0020)  /* LCD_E Mux Rate Bit: 2 */
#define LCDSSEL0            (0x0040)  /* LCD_E Clock Select Bit: 0 */
#define LCDSSEL1            (0x0080)  /* LCD_E Clock Select Bit: 1 */
#define LCDDIV0             (0x0800)  /* LCD_E LCD frequency divider Bit: 0 */
#define LCDDIV1             (0x1000)  /* LCD_E LCD frequency divider Bit: 1 */
#define LCDDIV2             (0x2000)  /* LCD_E LCD frequency divider Bit: 2 */
#define LCDDIV3             (0x4000)  /* LCD_E LCD frequency divider Bit: 3 */
#define LCDDIV4             (0x8000)  /* LCD_E LCD frequency divider Bit: 4 */

// LCDCTL0
#define LCDON_L             (0x0001)  /* LCD_E LCD On */
#define LCDLP_L             (0x0002)  /* LCD_E Low Power Waveform */
#define LCDSON_L            (0x0004)  /* LCD_E LCD Segments On */
#define LCDMX0_L            (0x0008)  /* LCD_E Mux Rate Bit: 0 */
#define LCDMX1_L            (0x0010)  /* LCD_E Mux Rate Bit: 1 */
#define LCDMX2_L            (0x0020)  /* LCD_E Mux Rate Bit: 2 */
#define LCDSSEL0_L          (0x0040)  /* LCD_E Clock Select Bit: 0 */
#define LCDSSEL1_L          (0x0080)  /* LCD_E Clock Select Bit: 1 */

// LCDCTL0
#define LCDDIV0_H           (0x0008)  /* LCD_E LCD frequency divider Bit: 0 */
#define LCDDIV1_H           (0x0010)  /* LCD_E LCD frequency divider Bit: 1 */
#define LCDDIV2_H           (0x0020)  /* LCD_E LCD frequency divider Bit: 2 */
#define LCDDIV3_H           (0x0040)  /* LCD_E LCD frequency divider Bit: 3 */
#define LCDDIV4_H           (0x0080)  /* LCD_E LCD frequency divider Bit: 4 */

#define LCDSSEL_0           (0x0000)  /* LCD_E Clock Select: 0 */
#define LCDSSEL_1           (0x0040)  /* LCD_E Clock Select: 1 */
#define LCDSSEL_2           (0x0080)  /* LCD_E Clock Select: 2 */
#define LCDSSEL_3           (0x00C0)  /* LCD_E Clock Select: 3 */
#define LCDSSEL__XTCLK      (0x0000)  /* LCD_E Clock Select: XTCLK */
#define LCDSSEL__ACLK       (0x0040)  /* LCD_E Clock Select: ACLK */
#define LCDSSEL__VLOCLK     (0x0080)  /* LCD_E Clock Select: VLOCLK */

#define LCDDIV_0            (0x0000)  /* LCD_E LCD frequency divider: /1 */
#define LCDDIV_1            (0x0800)  /* LCD_E LCD frequency divider: /2 */
#define LCDDIV_2            (0x1000)  /* LCD_E LCD frequency divider: /3 */
#define LCDDIV_3            (0x1800)  /* LCD_E LCD frequency divider: /4 */
#define LCDDIV_4            (0x2000)  /* LCD_E LCD frequency divider: /5 */
#define LCDDIV_5            (0x2800)  /* LCD_E LCD frequency divider: /6 */
#define LCDDIV_6            (0x3000)  /* LCD_E LCD frequency divider: /7 */
#define LCDDIV_7            (0x3800)  /* LCD_E LCD frequency divider: /8 */
#define LCDDIV_8            (0x4000)  /* LCD_E LCD frequency divider: /9 */
#define LCDDIV_9            (0x4800)  /* LCD_E LCD frequency divider: /10 */
#define LCDDIV_10           (0x5000)  /* LCD_E LCD frequency divider: /11 */
#define LCDDIV_11           (0x5800)  /* LCD_E LCD frequency divider: /12 */
#define LCDDIV_12           (0x6000)  /* LCD_E LCD frequency divider: /13 */
#define LCDDIV_13           (0x6800)  /* LCD_E LCD frequency divider: /14 */
#define LCDDIV_14           (0x7000)  /* LCD_E LCD frequency divider: /15 */
#define LCDDIV_15           (0x7800)  /* LCD_E LCD frequency divider: /16 */
#define LCDDIV_16           (0x8000)  /* LCD_E LCD frequency divider: /17 */
#define LCDDIV_17           (0x8800)  /* LCD_E LCD frequency divider: /18 */
#define LCDDIV_18           (0x9000)  /* LCD_E LCD frequency divider: /19 */
#define LCDDIV_19           (0x9800)  /* LCD_E LCD frequency divider: /20 */
#define LCDDIV_20           (0xA000)  /* LCD_E LCD frequency divider: /21 */
#define LCDDIV_21           (0xA800)  /* LCD_E LCD frequency divider: /22 */
#define LCDDIV_22           (0xB000)  /* LCD_E LCD frequency divider: /23 */
#define LCDDIV_23           (0xB800)  /* LCD_E LCD frequency divider: /24 */
#define LCDDIV_24           (0xC000)  /* LCD_E LCD frequency divider: /25 */
#define LCDDIV_25           (0xC800)  /* LCD_E LCD frequency divider: /26 */
#define LCDDIV_26           (0xD000)  /* LCD_E LCD frequency divider: /27 */
#define LCDDIV_27           (0xD800)  /* LCD_E LCD frequency divider: /28 */
#define LCDDIV_28           (0xE000)  /* LCD_E LCD frequency divider: /29 */
#define LCDDIV_29           (0xE800)  /* LCD_E LCD frequency divider: /30 */
#define LCDDIV_30           (0xF000)  /* LCD_E LCD frequency divider: /31 */
#define LCDDIV_31           (0xF800)  /* LCD_E LCD frequency divider: /32 */
#define LCDDIV__1           (0x0000)  /* LCD_E LCD frequency divider: /1 */
#define LCDDIV__2           (0x0800)  /* LCD_E LCD frequency divider: /2 */
#define LCDDIV__3           (0x1000)  /* LCD_E LCD frequency divider: /3 */
#define LCDDIV__4           (0x1800)  /* LCD_E LCD frequency divider: /4 */
#define LCDDIV__5           (0x2000)  /* LCD_E LCD frequency divider: /5 */
#define LCDDIV__6           (0x2800)  /* LCD_E LCD frequency divider: /6 */
#define LCDDIV__7           (0x3000)  /* LCD_E LCD frequency divider: /7 */
#define LCDDIV__8           (0x3800)  /* LCD_E LCD frequency divider: /8 */
#define LCDDIV__9           (0x4000)  /* LCD_E LCD frequency divider: /9 */
#define LCDDIV__10          (0x4800)  /* LCD_E LCD frequency divider: /10 */
#define LCDDIV__11          (0x5000)  /* LCD_E LCD frequency divider: /11 */
#define LCDDIV__12          (0x5800)  /* LCD_E LCD frequency divider: /12 */
#define LCDDIV__13          (0x6000)  /* LCD_E LCD frequency divider: /13 */
#define LCDDIV__14          (0x6800)  /* LCD_E LCD frequency divider: /14 */
#define LCDDIV__15          (0x7000)  /* LCD_E LCD frequency divider: /15 */
#define LCDDIV__16          (0x7800)  /* LCD_E LCD frequency divider: /16 */
#define LCDDIV__17          (0x8000)  /* LCD_E LCD frequency divider: /17 */
#define LCDDIV__18          (0x8800)  /* LCD_E LCD frequency divider: /18 */
#define LCDDIV__19          (0x9000)  /* LCD_E LCD frequency divider: /19 */
#define LCDDIV__20          (0x9800)  /* LCD_E LCD frequency divider: /20 */
#define LCDDIV__21          (0xA000)  /* LCD_E LCD frequency divider: /21 */
#define LCDDIV__22          (0xA800)  /* LCD_E LCD frequency divider: /22 */
#define LCDDIV__23          (0xB000)  /* LCD_E LCD frequency divider: /23 */
#define LCDDIV__24          (0xB800)  /* LCD_E LCD frequency divider: /24 */
#define LCDDIV__25          (0xC000)  /* LCD_E LCD frequency divider: /25 */
#define LCDDIV__26          (0xC800)  /* LCD_E LCD frequency divider: /26 */
#define LCDDIV__27          (0xD000)  /* LCD_E LCD frequency divider: /27 */
#define LCDDIV__28          (0xD800)  /* LCD_E LCD frequency divider: /28 */
#define LCDDIV__29          (0xE000)  /* LCD_E LCD frequency divider: /29 */
#define LCDDIV__30          (0xE800)  /* LCD_E LCD frequency divider: /30 */
#define LCDDIV__31          (0xF000)  /* LCD_E LCD frequency divider: /31 */
#define LCDDIV__32          (0xF800)  /* LCD_E LCD frequency divider: /32 */


/* Display modes coded with Bits 3-5 */
#define LCDSTATIC           (LCDSON)
#define LCD2MUX             (LCDMX0+LCDSON)
#define LCD3MUX             (LCDMX1+LCDSON)
#define LCD4MUX             (LCDMX1+LCDMX0+LCDSON)
#define LCD5MUX             (LCDMX2+LCDSON)
#define LCD6MUX             (LCDMX2+LCDMX0+LCDSON)
#define LCD7MUX             (LCDMX2+LCDMX1+LCDSON)
#define LCD8MUX             (LCDMX2+LCDMX1+LCDMX0+LCDSON)

// LCDCTL1
#define LCDFRMIFG           (0x0001)  /* LCD_E LCD frame interrupt flag */
#define LCDBLKOFFIFG        (0x0002)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIFG         (0x0004)  /* LCD_E LCD blinking on interrupt flag, */
#define LCDFRMIE            (0x0100)  /* LCD_E LCD frame interrupt enable */
#define LCDBLKOFFIE         (0x0200)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIE          (0x0400)  /* LCD_E LCD blinking on interrupt flag, */

// LCDCTL1
#define LCDFRMIFG_L         (0x0001)  /* LCD_E LCD frame interrupt flag */
#define LCDBLKOFFIFG_L      (0x0002)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIFG_L       (0x0004)  /* LCD_E LCD blinking on interrupt flag, */

// LCDCTL1
#define LCDFRMIE_H          (0x0001)  /* LCD_E LCD frame interrupt enable */
#define LCDBLKOFFIE_H       (0x0002)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIE_H        (0x0004)  /* LCD_E LCD blinking on interrupt flag, */

// LCDBLKCTL
#define LCDBLKMOD0          (0x0001)  /* LCD_E Blinking mode Bit: 0 */
#define LCDBLKMOD1          (0x0002)  /* LCD_E Blinking mode Bit: 1 */
#define LCDBLKPRE0          (0x0004)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 0 */
#define LCDBLKPRE1          (0x0008)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 1 */
#define LCDBLKPRE2          (0x0010)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 2 */

// LCDBLKCTL
#define LCDBLKMOD0_L        (0x0001)  /* LCD_E Blinking mode Bit: 0 */
#define LCDBLKMOD1_L        (0x0002)  /* LCD_E Blinking mode Bit: 1 */
#define LCDBLKPRE0_L        (0x0004)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 0 */
#define LCDBLKPRE1_L        (0x0008)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 1 */
#define LCDBLKPRE2_L        (0x0010)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 2 */


#define LCDBLKMOD_0         (0x0000)  /* LCD_E Blinking mode: Off */
#define LCDBLKMOD_1         (0x0001)  /* LCD_E Blinking mode: Individual */
#define LCDBLKMOD_2         (0x0002)  /* LCD_E Blinking mode: All */
#define LCDBLKMOD_3         (0x0003)  /* LCD_E Blinking mode: Switching */

#define LCDBLKPRE_0         (0x0000)  /* LCD_E Clock pre-scaler for blinking frequency: 0 */
#define LCDBLKPRE_1         (0x0004)  /* LCD_E Clock pre-scaler for blinking frequency: 1 */
#define LCDBLKPRE_2         (0x0008)  /* LCD_E Clock pre-scaler for blinking frequency: 2 */
#define LCDBLKPRE_3         (0x000C)  /* LCD_E Clock pre-scaler for blinking frequency: 3 */
#define LCDBLKPRE_4         (0x0010)  /* LCD_E Clock pre-scaler for blinking frequency: 4 */
#define LCDBLKPRE_5         (0x0014)  /* LCD_E Clock pre-scaler for blinking frequency: 5 */
#define LCDBLKPRE_6         (0x0018)  /* LCD_E Clock pre-scaler for blinking frequency: 6 */
#define LCDBLKPRE_7         (0x001C)  /* LCD_E Clock pre-scaler for blinking frequency: 7 */

#define LCDBLKPRE__4        (0x0000)  /* LCD_E Clock pre-scaler for blinking frequency: 4   */
#define LCDBLKPRE__8        (0x0004)  /* LCD_E Clock pre-scaler for blinking frequency: 8   */
#define LCDBLKPRE__16       (0x0008)  /* LCD_E Clock pre-scaler for blinking frequency: 16  */
#define LCDBLKPRE__32       (0x000C)  /* LCD_E Clock pre-scaler for blinking frequency: 32  */
#define LCDBLKPRE__64       (0x0010)  /* LCD_E Clock pre-scaler for blinking frequency: 64  */
#define LCDBLKPRE__128      (0x0014)  /* LCD_E Clock pre-scaler for blinking frequency: 128 */
#define LCDBLKPRE__256      (0x0018)  /* LCD_E Clock pre-scaler for blinking frequency: 256 */
#define LCDBLKPRE__512      (0x001C)  /* LCD_E Clock pre-scaler for blinking frequency: 512 */

// LCDMEMCTL
#define LCDDISP             (0x0001)  /* LCD_E LCD memory registers for display */
#define LCDCLRM             (0x0002)  /* LCD_E Clear LCD memory */
#define LCDCLRBM            (0x0004)  /* LCD_E Clear LCD blinking memory */

// LCDMEMCTL
#define LCDDISP_L           (0x0001)  /* LCD_E LCD memory registers for display */
#define LCDCLRM_L           (0x0002)  /* LCD_E Clear LCD memory */
#define LCDCLRBM_L          (0x0004)  /* LCD_E Clear LCD blinking memory */


// LCDVCTL
#define LCDREFMODE          (0x0001)  /* Selects wether R13 voltage is switched or in static mode */
#define LCDSELVDD           (0x0020)  /* selects if R33 is supplied either from Vcc internally or from charge pump */
#define LCDREFEN            (0x0040)  /* Internal reference voltage enable on R13 */
#define LCDCPEN             (0x0080)  /* Charge pump enable */
#define VLCD0               (0x0100)  /* VLCD select: 0 */
#define VLCD1               (0x0200)  /* VLCD select: 1 */
#define VLCD2               (0x0400)  /* VLCD select: 2 */
#define VLCD3               (0x0800)  /* VLCD select: 3 */
#define LCDCPFSEL0          (0x1000)  /* Charge pump frequency selection Bit: 0 */
#define LCDCPFSEL1          (0x2000)  /* Charge pump frequency selection Bit: 1 */
#define LCDCPFSEL2          (0x4000)  /* Charge pump frequency selection Bit: 2 */
#define LCDCPFSEL3          (0x8000)  /* Charge pump frequency selection Bit: 3 */

// LCDVCTL
#define LCDREFMODE_L        (0x0001)  /* Selects wether R13 voltage is switched or in static mode */
#define LCDSELVDD_L         (0x0020)  /* selects if R33 is supplied either from Vcc internally or from charge pump */
#define LCDREFEN_L          (0x0040)  /* Internal reference voltage enable on R13 */
#define LCDCPEN_L           (0x0080)  /* Charge pump enable */

// LCDVCTL
#define VLCD0_H             (0x0001)  /* VLCD select: 0 */
#define VLCD1_H             (0x0002)  /* VLCD select: 1 */
#define VLCD2_H             (0x0004)  /* VLCD select: 2 */
#define VLCD3_H             (0x0008)  /* VLCD select: 3 */
#define LCDCPFSEL0_H        (0x0010)  /* Charge pump frequency selection Bit: 0 */
#define LCDCPFSEL1_H        (0x0020)  /* Charge pump frequency selection Bit: 1 */
#define LCDCPFSEL2_H        (0x0040)  /* Charge pump frequency selection Bit: 2 */
#define LCDCPFSEL3_H        (0x0080)  /* Charge pump frequency selection Bit: 3 */

/* Charge pump voltage selections */
#define VLCD_0              (0x0000)  /* VLCD = 2.60V */
#define VLCD_1              (0x0100)  /* VLCD = 2.66V */
#define VLCD_2              (0x0200)  /* VLCD = 2.72V */
#define VLCD_3              (0x0300)  /* VLCD = 2.78V */
#define VLCD_4              (0x0400)  /* VLCD = 2.84V */
#define VLCD_5              (0x0500)  /* VLCD = 2.90V */
#define VLCD_6              (0x0600)  /* VLCD = 2.96V */
#define VLCD_7              (0x0700)  /* VLCD = 3.02V */
#define VLCD_8              (0x0800)  /* VLCD = 3.08V */
#define VLCD_9              (0x0900)  /* VLCD = 3.14V */
#define VLCD_10             (0x0A00)  /* VLCD = 3.20V */
#define VLCD_11             (0x0B00)  /* VLCD = 3.26V */
#define VLCD_12             (0x0C00)  /* VLCD = 3.32V */
#define VLCD_13             (0x0D00)  /* VLCD = 3.38V */
#define VLCD_14             (0x0E00)  /* VLCD = 3.44V */
#define VLCD_15             (0x0F00)  /* VLCD = 3.50V */


// LCDPCTL0
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

// LCDPCTL0
#define LCDS0_L             (0x0001)  /* LCD Segment  0 enable. */
#define LCDS1_L             (0x0002)  /* LCD Segment  1 enable. */
#define LCDS2_L             (0x0004)  /* LCD Segment  2 enable. */
#define LCDS3_L             (0x0008)  /* LCD Segment  3 enable. */
#define LCDS4_L             (0x0010)  /* LCD Segment  4 enable. */
#define LCDS5_L             (0x0020)  /* LCD Segment  5 enable. */
#define LCDS6_L             (0x0040)  /* LCD Segment  6 enable. */
#define LCDS7_L             (0x0080)  /* LCD Segment  7 enable. */

// LCDPCTL0
#define LCDS8_H             (0x0001)  /* LCD Segment  8 enable. */
#define LCDS9_H             (0x0002)  /* LCD Segment  9 enable. */
#define LCDS10_H            (0x0004)  /* LCD Segment 10 enable. */
#define LCDS11_H            (0x0008)  /* LCD Segment 11 enable. */
#define LCDS12_H            (0x0010)  /* LCD Segment 12 enable. */
#define LCDS13_H            (0x0020)  /* LCD Segment 13 enable. */
#define LCDS14_H            (0x0040)  /* LCD Segment 14 enable. */
#define LCDS15_H            (0x0080)  /* LCD Segment 15 enable. */

// LCDPCTL1
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

// LCDPCTL1
#define LCDS16_L            (0x0001)  /* LCD Segment 16 enable. */
#define LCDS17_L            (0x0002)  /* LCD Segment 17 enable. */
#define LCDS18_L            (0x0004)  /* LCD Segment 18 enable. */
#define LCDS19_L            (0x0008)  /* LCD Segment 19 enable. */
#define LCDS20_L            (0x0010)  /* LCD Segment 20 enable. */
#define LCDS21_L            (0x0020)  /* LCD Segment 21 enable. */
#define LCDS22_L            (0x0040)  /* LCD Segment 22 enable. */
#define LCDS23_L            (0x0080)  /* LCD Segment 23 enable. */

// LCDPCTL1
#define LCDS24_H            (0x0001)  /* LCD Segment 24 enable. */
#define LCDS25_H            (0x0002)  /* LCD Segment 25 enable. */
#define LCDS26_H            (0x0004)  /* LCD Segment 26 enable. */
#define LCDS27_H            (0x0008)  /* LCD Segment 27 enable. */
#define LCDS28_H            (0x0010)  /* LCD Segment 28 enable. */
#define LCDS29_H            (0x0020)  /* LCD Segment 29 enable. */
#define LCDS30_H            (0x0040)  /* LCD Segment 30 enable. */
#define LCDS31_H            (0x0080)  /* LCD Segment 31 enable. */

// LCDPCTL2
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

// LCDPCTL2
#define LCDS32_L            (0x0001)  /* LCD Segment 32 enable. */
#define LCDS33_L            (0x0002)  /* LCD Segment 33 enable. */
#define LCDS34_L            (0x0004)  /* LCD Segment 34 enable. */
#define LCDS35_L            (0x0008)  /* LCD Segment 35 enable. */
#define LCDS36_L            (0x0010)  /* LCD Segment 36 enable. */
#define LCDS37_L            (0x0020)  /* LCD Segment 37 enable. */
#define LCDS38_L            (0x0040)  /* LCD Segment 38 enable. */
#define LCDS39_L            (0x0080)  /* LCD Segment 39 enable. */

// LCDPCTL2
#define LCDS40_H            (0x0001)  /* LCD Segment 40 enable. */
#define LCDS41_H            (0x0002)  /* LCD Segment 41 enable. */
#define LCDS42_H            (0x0004)  /* LCD Segment 42 enable. */
#define LCDS43_H            (0x0008)  /* LCD Segment 43 enable. */
#define LCDS44_H            (0x0010)  /* LCD Segment 44 enable. */
#define LCDS45_H            (0x0020)  /* LCD Segment 45 enable. */
#define LCDS46_H            (0x0040)  /* LCD Segment 46 enable. */
#define LCDS47_H            (0x0080)  /* LCD Segment 47 enable. */

// LCDCSSEL0
#define LCDCSS0             (0x0001)  /* Selects pin L0  as either common or segment line */
#define LCDCSS1             (0x0002)  /* Selects pin L1  as either common or segment line */
#define LCDCSS2             (0x0004)  /* Selects pin L2  as either common or segment line */
#define LCDCSS3             (0x0008)  /* Selects pin L3  as either common or segment line */
#define LCDCSS4             (0x0010)  /* Selects pin L4  as either common or segment line */
#define LCDCSS5             (0x0020)  /* Selects pin L5  as either common or segment line */
#define LCDCSS6             (0x0040)  /* Selects pin L6  as either common or segment line */
#define LCDCSS7             (0x0080)  /* Selects pin L7  as either common or segment line */
#define LCDCSS8             (0x0100)  /* Selects pin L8  as either common or segment line */
#define LCDCSS9             (0x0200)  /* Selects pin L9  as either common or segment line */
#define LCDCSS10            (0x0400)  /* Selects pin L10 as either common or segment line */
#define LCDCSS11            (0x0800)  /* Selects pin L11 as either common or segment line */
#define LCDCSS12            (0x1000)  /* Selects pin L12 as either common or segment line */
#define LCDCSS13            (0x2000)  /* Selects pin L13 as either common or segment line */
#define LCDCSS14            (0x4000)  /* Selects pin L14 as either common or segment line */
#define LCDCSS15            (0x8000)  /* Selects pin L15 as either common or segment line */

// LCDCSSEL0
#define LCDCSS0_L           (0x0001)  /* Selects pin L0  as either common or segment line */
#define LCDCSS1_L           (0x0002)  /* Selects pin L1  as either common or segment line */
#define LCDCSS2_L           (0x0004)  /* Selects pin L2  as either common or segment line */
#define LCDCSS3_L           (0x0008)  /* Selects pin L3  as either common or segment line */
#define LCDCSS4_L           (0x0010)  /* Selects pin L4  as either common or segment line */
#define LCDCSS5_L           (0x0020)  /* Selects pin L5  as either common or segment line */
#define LCDCSS6_L           (0x0040)  /* Selects pin L6  as either common or segment line */
#define LCDCSS7_L           (0x0080)  /* Selects pin L7  as either common or segment line */

// LCDCSSEL0
#define LCDCSS8_H           (0x0001)  /* Selects pin L8  as either common or segment line */
#define LCDCSS9_H           (0x0002)  /* Selects pin L9  as either common or segment line */
#define LCDCSS10_H          (0x0004)  /* Selects pin L10 as either common or segment line */
#define LCDCSS11_H          (0x0008)  /* Selects pin L11 as either common or segment line */
#define LCDCSS12_H          (0x0010)  /* Selects pin L12 as either common or segment line */
#define LCDCSS13_H          (0x0020)  /* Selects pin L13 as either common or segment line */
#define LCDCSS14_H          (0x0040)  /* Selects pin L14 as either common or segment line */
#define LCDCSS15_H          (0x0080)  /* Selects pin L15 as either common or segment line */

// LCDCSSEL1
#define LCDCSS16            (0x0001)  /* Selects pin L16 as either common or segment line */
#define LCDCSS17            (0x0002)  /* Selects pin L17 as either common or segment line */
#define LCDCSS18            (0x0004)  /* Selects pin L18 as either common or segment line */
#define LCDCSS19            (0x0008)  /* Selects pin L19 as either common or segment line */
#define LCDCSS20            (0x0010)  /* Selects pin L20 as either common or segment line */
#define LCDCSS21            (0x0020)  /* Selects pin L21 as either common or segment line */
#define LCDCSS22            (0x0040)  /* Selects pin L22 as either common or segment line */
#define LCDCSS23            (0x0080)  /* Selects pin L23 as either common or segment line */
#define LCDCSS24            (0x0100)  /* Selects pin L24 as either common or segment line */
#define LCDCSS25            (0x0200)  /* Selects pin L25 as either common or segment line */
#define LCDCSS26            (0x0400)  /* Selects pin L26 as either common or segment line */
#define LCDCSS27            (0x0800)  /* Selects pin L27 as either common or segment line */
#define LCDCSS28            (0x1000)  /* Selects pin L28 as either common or segment line */
#define LCDCSS29            (0x2000)  /* Selects pin L29 as either common or segment line */
#define LCDCSS30            (0x4000)  /* Selects pin L30 as either common or segment line */
#define LCDCSS31            (0x8000)  /* Selects pin L31 as either common or segment line */

// LCDCSSEL1
#define LCDCSS16_L          (0x0001)  /* Selects pin L16 as either common or segment line */
#define LCDCSS17_L          (0x0002)  /* Selects pin L17 as either common or segment line */
#define LCDCSS18_L          (0x0004)  /* Selects pin L18 as either common or segment line */
#define LCDCSS19_L          (0x0008)  /* Selects pin L19 as either common or segment line */
#define LCDCSS20_L          (0x0010)  /* Selects pin L20 as either common or segment line */
#define LCDCSS21_L          (0x0020)  /* Selects pin L21 as either common or segment line */
#define LCDCSS22_L          (0x0040)  /* Selects pin L22 as either common or segment line */
#define LCDCSS23_L          (0x0080)  /* Selects pin L23 as either common or segment line */

// LCDCSSEL1
#define LCDCSS24_H          (0x0001)  /* Selects pin L24 as either common or segment line */
#define LCDCSS25_H          (0x0002)  /* Selects pin L25 as either common or segment line */
#define LCDCSS26_H          (0x0004)  /* Selects pin L26 as either common or segment line */
#define LCDCSS27_H          (0x0008)  /* Selects pin L27 as either common or segment line */
#define LCDCSS28_H          (0x0010)  /* Selects pin L28 as either common or segment line */
#define LCDCSS29_H          (0x0020)  /* Selects pin L29 as either common or segment line */
#define LCDCSS30_H          (0x0040)  /* Selects pin L30 as either common or segment line */
#define LCDCSS31_H          (0x0080)  /* Selects pin L31 as either common or segment line */

// LCDCSSEL2
#define LCDCSS32            (0x0001)  /* Selects pin L32 as either common or segment line */
#define LCDCSS33            (0x0002)  /* Selects pin L33 as either common or segment line */
#define LCDCSS34            (0x0004)  /* Selects pin L34 as either common or segment line */
#define LCDCSS35            (0x0008)  /* Selects pin L35 as either common or segment line */
#define LCDCSS36            (0x0010)  /* Selects pin L36 as either common or segment line */
#define LCDCSS37            (0x0020)  /* Selects pin L37 as either common or segment line */
#define LCDCSS38            (0x0040)  /* Selects pin L38 as either common or segment line */
#define LCDCSS39            (0x0080)  /* Selects pin L39 as either common or segment line */
#define LCDCSS40            (0x0100)  /* Selects pin L40 as either common or segment line */
#define LCDCSS41            (0x0200)  /* Selects pin L41 as either common or segment line */
#define LCDCSS42            (0x0400)  /* Selects pin L42 as either common or segment line */
#define LCDCSS43            (0x0800)  /* Selects pin L43 as either common or segment line */
#define LCDCSS44            (0x1000)  /* Selects pin L44 as either common or segment line */
#define LCDCSS45            (0x2000)  /* Selects pin L45 as either common or segment line */
#define LCDCSS46            (0x4000)  /* Selects pin L46 as either common or segment line */
#define LCDCSS47            (0x8000)  /* Selects pin L47 as either common or segment line */

// LCDCSSEL2
#define LCDCSS32_L          (0x0001)  /* Selects pin L32 as either common or segment line */
#define LCDCSS33_L          (0x0002)  /* Selects pin L33 as either common or segment line */
#define LCDCSS34_L          (0x0004)  /* Selects pin L34 as either common or segment line */
#define LCDCSS35_L          (0x0008)  /* Selects pin L35 as either common or segment line */
#define LCDCSS36_L          (0x0010)  /* Selects pin L36 as either common or segment line */
#define LCDCSS37_L          (0x0020)  /* Selects pin L37 as either common or segment line */
#define LCDCSS38_L          (0x0040)  /* Selects pin L38 as either common or segment line */
#define LCDCSS39_L          (0x0080)  /* Selects pin L39 as either common or segment line */

// LCDCSSEL2
#define LCDCSS40_H          (0x0001)  /* Selects pin L40 as either common or segment line */
#define LCDCSS41_H          (0x0002)  /* Selects pin L41 as either common or segment line */
#define LCDCSS42_H          (0x0004)  /* Selects pin L42 as either common or segment line */
#define LCDCSS43_H          (0x0008)  /* Selects pin L43 as either common or segment line */
#define LCDCSS44_H          (0x0010)  /* Selects pin L44 as either common or segment line */
#define LCDCSS45_H          (0x0020)  /* Selects pin L45 as either common or segment line */
#define LCDCSS46_H          (0x0040)  /* Selects pin L46 as either common or segment line */
#define LCDCSS47_H          (0x0080)  /* Selects pin L47 as either common or segment line */

#define OFS_LCDM0W            (0x0020)  /* LCD Memory 0/1 */
#define OFS_LCDM0W_L           OFS_LCDM0W
#define OFS_LCDM0W_H           OFS_LCDM0W+1
#define LCDM0               LCDM0W_L  /* LCD Memory 0 */
#define LCDM1               LCDM0W_H  /* LCD Memory 1 */
#define LCDMEM_             LCDM0     /* LCD Memory */
#ifndef __STDC__
#define LCDMEM              LCDM0     /* LCD Memory (for assembler) */
#else
#define LCDMEM              ((volatile char*) &LCDM0) /* LCD Memory (for C) */
#endif
#define OFS_LCDM2W            (0x0022)  /* LCD Memory 2/3 */
#define OFS_LCDM2W_L           OFS_LCDM2W
#define OFS_LCDM2W_H           OFS_LCDM2W+1
#define LCDM2               LCDM2W_L  /* LCD Memory 2 */
#define LCDM3               LCDM2W_H  /* LCD Memory 3 */
#define OFS_LCDM4W            (0x0024)  /* LCD Memory 4/5 */
#define OFS_LCDM4W_L           OFS_LCDM4W
#define OFS_LCDM4W_H           OFS_LCDM4W+1
#define LCDM4               LCDM4W_L  /* LCD Memory 4 */
#define LCDM5               LCDM4W_H  /* LCD Memory 5 */
#define OFS_LCDM6W            (0x0026)  /* LCD Memory 6/7 */
#define OFS_LCDM6W_L           OFS_LCDM6W
#define OFS_LCDM6W_H           OFS_LCDM6W+1
#define LCDM6               LCDM6W_L  /* LCD Memory 6 */
#define LCDM7               LCDM6W_H  /* LCD Memory 7 */
#define OFS_LCDM8W            (0x0028)  /* LCD Memory 8/9 */
#define OFS_LCDM8W_L           OFS_LCDM8W
#define OFS_LCDM8W_H           OFS_LCDM8W+1
#define LCDM8               LCDM8W_L  /* LCD Memory 8 */
#define LCDM9               LCDM8W_H  /* LCD Memory 9 */
#define OFS_LCDM10W           (0x002A)  /* LCD Memory 10/11 */
#define OFS_LCDM10W_L          OFS_LCDM10W
#define OFS_LCDM10W_H          OFS_LCDM10W+1
#define LCDM10              LCDM10W_L /* LCD Memory 10 */
#define LCDM11              LCDM10W_H /* LCD Memory 11 */
#define OFS_LCDM12W           (0x002C)  /* LCD Memory 12/13 */
#define OFS_LCDM12W_L          OFS_LCDM12W
#define OFS_LCDM12W_H          OFS_LCDM12W+1
#define LCDM12              LCDM12W_L /* LCD Memory 12 */
#define LCDM13              LCDM12W_H /* LCD Memory 13 */
#define OFS_LCDM14W           (0x002E)  /* LCD Memory 14/15 */
#define OFS_LCDM14W_L          OFS_LCDM14W
#define OFS_LCDM14W_H          OFS_LCDM14W+1
#define LCDM14              LCDM14W_L /* LCD Memory 14 */
#define LCDM15              LCDM14W_H /* LCD Memory 15 */
#define OFS_LCDM16W           (0x0030)  /* LCD Memory 16/17 */
#define OFS_LCDM16W_L          OFS_LCDM16W
#define OFS_LCDM16W_H          OFS_LCDM16W+1
#define LCDM16              LCDM16W_L /* LCD Memory 16 */
#define LCDM17              LCDM16W_H /* LCD Memory 17 */
#define OFS_LCDM18W           (0x0032)  /* LCD Memory 18/19 */
#define OFS_LCDM18W_L          OFS_LCDM18W
#define OFS_LCDM18W_H          OFS_LCDM18W+1
#define LCDM18              LCDM18W_L /* LCD Memory 18 */
#define LCDM19              LCDM18W_H /* LCD Memory 19 */
#define OFS_LCDM20W           (0x0034)  /* LCD Memory 20/21 */
#define OFS_LCDM20W_L          OFS_LCDM20W
#define OFS_LCDM20W_H          OFS_LCDM20W+1
#define LCDM20              LCDM20W_L /* LCD Memory 20 */
#define LCDM21              LCDM20W_H /* LCD Memory 21 */
#define OFS_LCDM22W           (0x0036)  /* LCD Memory 22/23 */
#define OFS_LCDM22W_L          OFS_LCDM22W
#define OFS_LCDM22W_H          OFS_LCDM22W+1
#define LCDM22              LCDM22W_L /* LCD Memory 22 */
#define LCDM23              LCDM22W_H /* LCD Memory 23 */
#define OFS_LCDM24W           (0x0038)  /* LCD Memory 24/25 */
#define OFS_LCDM24W_L          OFS_LCDM24W
#define OFS_LCDM24W_H          OFS_LCDM24W+1
#define LCDM24              LCDM24W_L /* LCD Memory 24 */
#define LCDM25              LCDM24W_H /* LCD Memory 25 */
#define OFS_LCDM26W           (0x003A)  /* LCD Memory 26/27 */
#define OFS_LCDM26W_L          OFS_LCDM26W
#define OFS_LCDM26W_H          OFS_LCDM26W+1
#define LCDM26              LCDM26W_L /* LCD Memory 26 */
#define LCDM27              LCDM26W_H /* LCD Memory 27 */
#define OFS_LCDM28W           (0x003C)  /* LCD Memory 28/29 */
#define OFS_LCDM28W_L          OFS_LCDM28W
#define OFS_LCDM28W_H          OFS_LCDM28W+1
#define LCDM28              LCDM28W_L /* LCD Memory 28 */
#define LCDM29              LCDM28W_H /* LCD Memory 29 */
#define OFS_LCDM30W           (0x003E)  /* LCD Memory 30/31 */
#define OFS_LCDM30W_L          OFS_LCDM30W
#define OFS_LCDM30W_H          OFS_LCDM30W+1
#define LCDM30              LCDM30W_L /* LCD Memory 30 */
#define LCDM31              LCDM30W_H /* LCD Memory 31 */
#define OFS_LCDM32W           (0x0040)  /* LCD Memory 32/33 */
#define OFS_LCDM32W_L          OFS_LCDM32W
#define OFS_LCDM32W_H          OFS_LCDM32W+1
#define LCDM32              LCDM32W_L /* LCD Memory 32 */
#define LCDM33              LCDM32W_H /* LCD Memory 33 */
#define OFS_LCDM34W           (0x0042)  /* LCD Memory 34/35 */
#define OFS_LCDM34W_L          OFS_LCDM34W
#define OFS_LCDM34W_H          OFS_LCDM34W+1
#define LCDM34              LCDM34W_L /* LCD Memory 34 */
#define LCDM35              LCDM34W_H /* LCD Memory 35 */
#define OFS_LCDM36W           (0x0044)  /* LCD Memory 36/37 */
#define OFS_LCDM36W_L          OFS_LCDM36W
#define OFS_LCDM36W_H          OFS_LCDM36W+1
#define LCDM36              LCDM36W_L /* LCD Memory 36 */
#define LCDM37              LCDM36W_H /* LCD Memory 37 */
#define OFS_LCDM38W           (0x0046)  /* LCD Memory 38/39 */
#define OFS_LCDM38W_L          OFS_LCDM38W
#define OFS_LCDM38W_H          OFS_LCDM38W+1
#define LCDM38              LCDM38W_L /* LCD Memory 38 */
#define LCDM39              LCDM38W_H /* LCD Memory 39 */

#define OFS_LCDBM0W           (0x0040)  /* LCD Blinking Memory 0/1 */
#define OFS_LCDBM0W_L          OFS_LCDBM0W
#define OFS_LCDBM0W_H          OFS_LCDBM0W+1
#define LCDBM0              LCDBM0W_L /* LCD Blinking Memory 0 */
#define LCDBM1              LCDBM0W_H /* LCD Blinking Memory 1 */
#define LCDBMEM_            LCDBM0    /* LCD Blinking Memory */
#ifndef __STDC__
#define LCDBMEM             LCDBM0    /* LCD Blinking Memory (for assembler) */
#else
#define LCDBMEM             ((volatile char*) &LCDBM0) /* LCD Blinking Memory (for C) */
#endif
#define OFS_LCDBM2W           (0x0042)  /* LCD Blinking Memory 2/3 */
#define OFS_LCDBM2W_L          OFS_LCDBM2W
#define OFS_LCDBM2W_H          OFS_LCDBM2W+1
#define LCDBM2              LCDBM2W_L  /* LCD Blinking Memory 2 */
#define LCDBM3              LCDBM2W_H  /* LCD Blinking Memory 3 */
#define OFS_LCDBM4W           (0x0044)  /* LCD Blinking Memory 4/5 */
#define OFS_LCDBM4W_L          OFS_LCDBM4W
#define OFS_LCDBM4W_H          OFS_LCDBM4W+1
#define LCDBM4              LCDBM4W_L  /* LCD Blinking Memory 4 */
#define LCDBM5              LCDBM4W_H  /* LCD Blinking Memory 5 */
#define OFS_LCDBM6W           (0x0046)  /* LCD Blinking Memory 6/7 */
#define OFS_LCDBM6W_L          OFS_LCDBM6W
#define OFS_LCDBM6W_H          OFS_LCDBM6W+1
#define LCDBM6              LCDBM6W_L  /* LCD Blinking Memory 6 */
#define LCDBM7              LCDBM6W_H  /* LCD Blinking Memory 7 */
#define OFS_LCDBM8W           (0x0048)  /* LCD Blinking Memory 8/9 */
#define OFS_LCDBM8W_L          OFS_LCDBM8W
#define OFS_LCDBM8W_H          OFS_LCDBM8W+1
#define LCDBM8              LCDBM8W_L  /* LCD Blinking Memory 8 */
#define LCDBM9              LCDBM8W_H  /* LCD Blinking Memory 9 */
#define OFS_LCDBM10W          (0x004A)  /* LCD Blinking Memory 10/11 */
#define OFS_LCDBM10W_L         OFS_LCDBM10W
#define OFS_LCDBM10W_H         OFS_LCDBM10W+1
#define LCDBM10             LCDBM10W_L /* LCD Blinking Memory 10 */
#define LCDBM11             LCDBM10W_H /* LCD Blinking Memory 11 */
#define OFS_LCDBM12W          (0x004C)  /* LCD Blinking Memory 12/13 */
#define OFS_LCDBM12W_L         OFS_LCDBM12W
#define OFS_LCDBM12W_H         OFS_LCDBM12W+1
#define LCDBM12             LCDBM12W_L /* LCD Blinking Memory 12 */
#define LCDBM13             LCDBM12W_H /* LCD Blinking Memory 13 */
#define OFS_LCDBM14W          (0x004E)  /* LCD Blinking Memory 14/15 */
#define OFS_LCDBM14W_L         OFS_LCDBM14W
#define OFS_LCDBM14W_H         OFS_LCDBM14W+1
#define LCDBM14             LCDBM14W_L /* LCD Blinking Memory 14 */
#define LCDBM15             LCDBM14W_H /* LCD Blinking Memory 15 */
#define OFS_LCDBM16W          (0x0050)  /* LCD Blinking Memory 16/17 */
#define OFS_LCDBM16W_L         OFS_LCDBM16W
#define OFS_LCDBM16W_H         OFS_LCDBM16W+1
#define LCDBM16             LCDBM16W_L /* LCD Blinking Memory 16 */
#define LCDBM17             LCDBM16W_H /* LCD Blinking Memory 17 */
#define OFS_LCDBM18W          (0x0052)  /* LCD Blinking Memory 18/19 */
#define OFS_LCDBM18W_L         OFS_LCDBM18W
#define OFS_LCDBM18W_H         OFS_LCDBM18W+1
#define LCDBM18             LCDBM18W_L /* LCD Blinking Memory 18 */
#define LCDBM19             LCDBM18W_H /* LCD Blinking Memory 19 */

/* LCDIV Definitions */
#define LCDIV_NONE         (0x0000)    /* No Interrupt pending */
#define LCDIV_LCDBLKOFFIFG (0x0004)    /* Blink, segments off */
#define LCDIV_LCDBLKONIFG  (0x0006)    /* Blink, segments on */
#define LCDIV_LCDFRMIFG    (0x0008)    /* Frame interrupt */

#endif
/************************************************************
* Memory Protection Unit
************************************************************/
#ifdef  __MSP430_HAS_MPU__             /* Definition to show that Module is available */

#define OFS_MPUCTL0           (0x0000)  /* MPU Control Register 0 */
#define OFS_MPUCTL0_L          OFS_MPUCTL0
#define OFS_MPUCTL0_H          OFS_MPUCTL0+1
#define OFS_MPUCTL1           (0x0002)  /* MPU Control Register 1 */
#define OFS_MPUCTL1_L          OFS_MPUCTL1
#define OFS_MPUCTL1_H          OFS_MPUCTL1+1
#define OFS_MPUSEGB2          (0x0004)  /* MPU Segmentation Border 2 Register */
#define OFS_MPUSEGB2_L         OFS_MPUSEGB2
#define OFS_MPUSEGB2_H         OFS_MPUSEGB2+1
#define OFS_MPUSEGB1          (0x0006)  /* MPU Segmentation Border 1 Register */
#define OFS_MPUSEGB1_L         OFS_MPUSEGB1
#define OFS_MPUSEGB1_H         OFS_MPUSEGB1+1
#define OFS_MPUSAM            (0x0008)  /* MPU Access Management Register */
#define OFS_MPUSAM_L           OFS_MPUSAM
#define OFS_MPUSAM_H           OFS_MPUSAM+1
#define OFS_MPUIPC0           (0x000A)  /* MPU IP Control 0 Register */
#define OFS_MPUIPC0_L          OFS_MPUIPC0
#define OFS_MPUIPC0_H          OFS_MPUIPC0+1
#define OFS_MPUIPSEGB2        (0x000C)  /* MPU IP Segment Border 2 Register */
#define OFS_MPUIPSEGB2_L       OFS_MPUIPSEGB2
#define OFS_MPUIPSEGB2_H       OFS_MPUIPSEGB2+1
#define OFS_MPUIPSEGB1        (0x000E)  /* MPU IP Segment Border 1 Register */
#define OFS_MPUIPSEGB1_L       OFS_MPUIPSEGB1
#define OFS_MPUIPSEGB1_H       OFS_MPUIPSEGB1+1


/* MPUCTL0 Control Bits */
#define MPUENA               (0x0001)  /* MPU Enable */
#define MPULOCK              (0x0002)  /* MPU Lock */
#define MPUSEGIE             (0x0010)  /* MPU Enable NMI on Segment violation */

/* MPUCTL0 Control Bits */
#define MPUENA_L            (0x0001)  /* MPU Enable */
#define MPULOCK_L           (0x0002)  /* MPU Lock */
#define MPUSEGIE_L          (0x0010)  /* MPU Enable NMI on Segment violation */

#define MPUPW                (0xA500)  /* MPU Access Password */
#define MPUPW_H              (0xA5)    /* MPU Access Password */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG           (0x0001)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG           (0x0002)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG           (0x0004)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG           (0x0008)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG          (0x0010)  /* MPU IP Memory Segment violation interupt flag */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG_L        (0x0001)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG_L        (0x0002)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG_L        (0x0004)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG_L        (0x0008)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG_L       (0x0010)  /* MPU IP Memory Segment violation interupt flag */


/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSAM Control Bits */
#define MPUSEG1RE            (0x0001)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE            (0x0002)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE            (0x0004)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS            (0x0008)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE            (0x0010)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE            (0x0020)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE            (0x0040)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS            (0x0080)  /* MPU Main memory Segment 2 Violation select */
#define MPUSEG3RE            (0x0100)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE            (0x0200)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE            (0x0400)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS            (0x0800)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE            (0x1000)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE            (0x2000)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE            (0x4000)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS            (0x8000)  /* MPU Info memory Segment Violation select */

/* MPUSAM Control Bits */
#define MPUSEG1RE_L         (0x0001)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE_L         (0x0002)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE_L         (0x0004)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS_L         (0x0008)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE_L         (0x0010)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE_L         (0x0020)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE_L         (0x0040)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS_L         (0x0080)  /* MPU Main memory Segment 2 Violation select */

/* MPUSAM Control Bits */
#define MPUSEG3RE_H         (0x0001)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE_H         (0x0002)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE_H         (0x0004)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS_H         (0x0008)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE_H         (0x0010)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE_H         (0x0020)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE_H         (0x0040)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS_H         (0x0080)  /* MPU Info memory Segment Violation select */

/* MPUIPC0 Control Bits */
#define MPUIPVS              (0x0020)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA             (0x0040)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK            (0x0080)  /* MPU IP Protection Lock */

/* MPUIPC0 Control Bits */
#define MPUIPVS_L           (0x0020)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA_L          (0x0040)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK_L         (0x0080)  /* MPU IP Protection Lock */


/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

#endif
/************************************************************
* HARDWARE MULTIPLIER 32Bit
************************************************************/
#ifdef  __MSP430_HAS_MPY32__          /* Definition to show that Module is available */

#define OFS_MPY               (0x0000)  /* Multiply Unsigned/Operand 1 */
#define OFS_MPY_L              OFS_MPY
#define OFS_MPY_H              OFS_MPY+1
#define OFS_MPYS              (0x0002)  /* Multiply Signed/Operand 1 */
#define OFS_MPYS_L             OFS_MPYS
#define OFS_MPYS_H             OFS_MPYS+1
#define OFS_MAC               (0x0004)  /* Multiply Unsigned and Accumulate/Operand 1 */
#define OFS_MAC_L              OFS_MAC
#define OFS_MAC_H              OFS_MAC+1
#define OFS_MACS              (0x0006)  /* Multiply Signed and Accumulate/Operand 1 */
#define OFS_MACS_L             OFS_MACS
#define OFS_MACS_H             OFS_MACS+1
#define OFS_OP2               (0x0008)  /* Operand 2 */
#define OFS_OP2_L              OFS_OP2
#define OFS_OP2_H              OFS_OP2+1
#define OFS_RESLO             (0x000A)  /* Result Low Word */
#define OFS_RESLO_L            OFS_RESLO
#define OFS_RESLO_H            OFS_RESLO+1
#define OFS_RESHI             (0x000C)  /* Result High Word */
#define OFS_RESHI_L            OFS_RESHI
#define OFS_RESHI_H            OFS_RESHI+1
#define OFS_SUMEXT            (0x000E)  /* Sum Extend */
#define OFS_SUMEXT_L           OFS_SUMEXT
#define OFS_SUMEXT_H           OFS_SUMEXT+1
#define OFS_MPY32CTL0         (0x002C)
#define OFS_MPY32CTL0_L        OFS_MPY32CTL0
#define OFS_MPY32CTL0_H        OFS_MPY32CTL0+1

#define OFS_MPY32L            (0x0010)  /* 32-bit operand 1 - multiply - low word */
#define OFS_MPY32L_L           OFS_MPY32L
#define OFS_MPY32L_H           OFS_MPY32L+1
#define OFS_MPY32H            (0x0012)  /* 32-bit operand 1 - multiply - high word */
#define OFS_MPY32H_L           OFS_MPY32H
#define OFS_MPY32H_H           OFS_MPY32H+1
#define OFS_MPYS32L           (0x0014)  /* 32-bit operand 1 - signed multiply - low word */
#define OFS_MPYS32L_L          OFS_MPYS32L
#define OFS_MPYS32L_H          OFS_MPYS32L+1
#define OFS_MPYS32H           (0x0016)  /* 32-bit operand 1 - signed multiply - high word */
#define OFS_MPYS32H_L          OFS_MPYS32H
#define OFS_MPYS32H_H          OFS_MPYS32H+1
#define OFS_MAC32L            (0x0018)  /* 32-bit operand 1 - multiply accumulate - low word */
#define OFS_MAC32L_L           OFS_MAC32L
#define OFS_MAC32L_H           OFS_MAC32L+1
#define OFS_MAC32H            (0x001A)  /* 32-bit operand 1 - multiply accumulate - high word */
#define OFS_MAC32H_L           OFS_MAC32H
#define OFS_MAC32H_H           OFS_MAC32H+1
#define OFS_MACS32L           (0x001C)  /* 32-bit operand 1 - signed multiply accumulate - low word */
#define OFS_MACS32L_L          OFS_MACS32L
#define OFS_MACS32L_H          OFS_MACS32L+1
#define OFS_MACS32H           (0x001E)  /* 32-bit operand 1 - signed multiply accumulate - high word */
#define OFS_MACS32H_L          OFS_MACS32H
#define OFS_MACS32H_H          OFS_MACS32H+1
#define OFS_OP2L              (0x0020)  /* 32-bit operand 2 - low word */
#define OFS_OP2L_L             OFS_OP2L
#define OFS_OP2L_H             OFS_OP2L+1
#define OFS_OP2H              (0x0022)  /* 32-bit operand 2 - high word */
#define OFS_OP2H_L             OFS_OP2H
#define OFS_OP2H_H             OFS_OP2H+1
#define OFS_RES0              (0x0024)  /* 32x32-bit result 0 - least significant word */
#define OFS_RES0_L             OFS_RES0
#define OFS_RES0_H             OFS_RES0+1
#define OFS_RES1              (0x0026)  /* 32x32-bit result 1 */
#define OFS_RES1_L             OFS_RES1
#define OFS_RES1_H             OFS_RES1+1
#define OFS_RES2              (0x0028)  /* 32x32-bit result 2 */
#define OFS_RES2_L             OFS_RES2
#define OFS_RES2_H             OFS_RES2+1
#define OFS_RES3              (0x002A)  /* 32x32-bit result 3 - most significant word */
#define OFS_RES3_L             OFS_RES3
#define OFS_RES3_H             OFS_RES3+1
#define OFS_SUMEXT            (0x000E)
#define OFS_SUMEXT_L           OFS_SUMEXT
#define OFS_SUMEXT_H           OFS_SUMEXT+1
#define OFS_MPY32CTL0         (0x002C)  /* MPY32 Control Register 0 */
#define OFS_MPY32CTL0_L        OFS_MPY32CTL0
#define OFS_MPY32CTL0_H        OFS_MPY32CTL0+1

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

#endif
/************************************************************
* PMM - Power Management System for FR2xx/FR4xx
************************************************************/
#ifdef  __MSP430_HAS_PMM_FRAM__       /* Definition to show that Module is available */

#define OFS_PMMCTL0           (0x0000)  /* PMM Control 0 */
#define OFS_PMMCTL0_L          OFS_PMMCTL0
#define OFS_PMMCTL0_H          OFS_PMMCTL0+1
#define OFS_PMMCTL1           (0x0002)  /* PMM Control 1 */
#define OFS_PMMCTL1_L          OFS_PMMCTL1
#define OFS_PMMCTL1_H          OFS_PMMCTL1+1
#define OFS_PMMCTL2           (0x0004)  /* PMM Control 2 */
#define OFS_PMMCTL2_L          OFS_PMMCTL2
#define OFS_PMMCTL2_H          OFS_PMMCTL2+1
#define OFS_PMMIFG            (0x000A)  /* PMM Interrupt Flag */
#define OFS_PMMIFG_L           OFS_PMMIFG
#define OFS_PMMIFG_H           OFS_PMMIFG+1
#define OFS_PMMIE             (0x000E)  /* PMM Interrupt Enable */
#define OFS_PMMIE_L            OFS_PMMIE
#define OFS_PMMIE_H            OFS_PMMIE+1
#define OFS_PM5CTL0           (0x0010)  /* PMM Power Mode 5 Control Register 0 */
#define OFS_PM5CTL0_L          OFS_PM5CTL0
#define OFS_PM5CTL0_H          OFS_PM5CTL0+1

#define PMMPW               (0xA500)  /* PMM Register Write Password */
#define PMMPW_H             (0xA5)    /* PMM Register Write Password for high word access */

/* PMMCTL0 Control Bits */
#define PMMSWBOR            (0x0004)  /* PMM Software BOR */
#define PMMSWPOR            (0x0008)  /* PMM Software POR */
#define PMMREGOFF           (0x0010)  /* PMM Turn Regulator off */
#define SVSHE               (0x0040)  /* SVS high side enable */
#define PMMLPRST            (0x0080)  /* PMM Low-Power Reset Enable */

/* PMMCTL0 Control Bits */
#define PMMSWBOR_L          (0x0004)  /* PMM Software BOR */
#define PMMSWPOR_L          (0x0008)  /* PMM Software POR */
#define PMMREGOFF_L         (0x0010)  /* PMM Turn Regulator off */
#define SVSHE_L             (0x0040)  /* SVS high side enable */
#define PMMLPRST_L          (0x0080)  /* PMM Low-Power Reset Enable */


/* PMMCTL1 Control Bits */

#define PMMCTL1KEY          (0xCC00)  /* PMM PMMCTL1 Register Write Password */

/* PMMCTL2 Control Bits */
#define INTREFEN            (0x0001)  /* Internal Reference Enable */
#define EXTREFEN            (0x0002)  /* External Reference output Enable */
#define TSENSOREN           (0x0008)  /* Temperature Sensor Enable */
#define REFGENACT           (0x0100)  /* REF Reference generator active */
#define REFBGACT            (0x0200)  /* REF Reference bandgap active */
#define BGMODE              (0x0800)  /* REF Bandgap mode */
#define REFGENRDY           (0x1000)  /* REF Reference generator ready */
#define REFBGRDY            (0x2000)  /* REF Reference bandgap ready */

/* PMMCTL2 Control Bits */
#define INTREFEN_L          (0x0001)  /* Internal Reference Enable */
#define EXTREFEN_L          (0x0002)  /* External Reference output Enable */
#define TSENSOREN_L         (0x0008)  /* Temperature Sensor Enable */

/* PMMCTL2 Control Bits */
#define REFGENACT_H         (0x0001)  /* REF Reference generator active */
#define REFBGACT_H          (0x0002)  /* REF Reference bandgap active */
#define BGMODE_H            (0x0008)  /* REF Bandgap mode */
#define REFGENRDY_H         (0x0010)  /* REF Reference generator ready */
#define REFBGRDY_H          (0x0020)  /* REF Reference bandgap ready */

/* PMMIFG Control Bits */
#define PMMBORIFG           (0x0100)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG           (0x0200)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG           (0x0400)  /* PMM Software POR interrupt flag */
#define SVSHIFG             (0x2000)  /* SVS low side interrupt flag */
#define PMMLPM5IFG          (0x8000)  /* LPM5 indication Flag */

/* PMMIFG Control Bits */
#define PMMBORIFG_H         (0x0001)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG_H         (0x0002)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG_H         (0x0004)  /* PMM Software POR interrupt flag */
#define SVSHIFG_H           (0x0020)  /* SVS low side interrupt flag */
#define PMMLPM5IFG_H        (0x0080)  /* LPM5 indication Flag */

/* PMMIE Control Bits */


/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5            (0x0001)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */
#define LPM5SW              (0x0010)  /* LPMx.5 switch dis/connected */
#define LPM5SM              (0x0020)  /* Manual mode for LPM3.5 switch */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5_L          (0x0001)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */
#define LPM5SW_L            (0x0010)  /* LPMx.5 switch dis/connected */
#define LPM5SM_L            (0x0020)  /* Manual mode for LPM3.5 switch */



#endif
/************************************************************
* DIGITAL I/O Port1/2 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT1_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */

#define OFS_PAIN              (0x0000)  /* Port A Input */
#define OFS_PAIN_L             OFS_PAIN
#define OFS_PAIN_H             OFS_PAIN+1
#define OFS_PAOUT             (0x0002)  /* Port A Output */
#define OFS_PAOUT_L            OFS_PAOUT
#define OFS_PAOUT_H            OFS_PAOUT+1
#define OFS_PADIR             (0x0004)  /* Port A Direction */
#define OFS_PADIR_L            OFS_PADIR
#define OFS_PADIR_H            OFS_PADIR+1
#define OFS_PAREN             (0x0006)  /* Port A Resistor Enable */
#define OFS_PAREN_L            OFS_PAREN
#define OFS_PAREN_H            OFS_PAREN+1
#define OFS_PASEL0            (0x000A)  /* Port A Selection 0 */
#define OFS_PASEL0_L           OFS_PASEL0
#define OFS_PASEL0_H           OFS_PASEL0+1
#define OFS_PASEL1            (0x000C)  /* Port A Selection 1 */
#define OFS_PASEL1_L           OFS_PASEL1
#define OFS_PASEL1_H           OFS_PASEL1+1
#define OFS_PASELC            (0x0016)  /* Port A Complement Selection */
#define OFS_PASELC_L           OFS_PASELC
#define OFS_PASELC_H           OFS_PASELC+1
#define OFS_PAIES             (0x0018)  /* Port A Interrupt Edge Select */
#define OFS_PAIES_L            OFS_PAIES
#define OFS_PAIES_H            OFS_PAIES+1
#define OFS_PAIE              (0x001A)  /* Port A Interrupt Enable */
#define OFS_PAIE_L             OFS_PAIE
#define OFS_PAIE_H             OFS_PAIE+1
#define OFS_PAIFG             (0x001C)  /* Port A Interrupt Flag */
#define OFS_PAIFG_L            OFS_PAIFG
#define OFS_PAIFG_H            OFS_PAIFG+1























#define OFS_P1IN              (0x0000)
#define OFS_P1OUT             (0x0002)
#define OFS_P1DIR             (0x0004)
#define OFS_P1REN             (0x0006)
#define OFS_P1SEL0            (0x000A)
#define OFS_P1SEL1            (0x000C)
#define OFS_P1SELC            (0x0016)
#define OFS_P1IV              (0x000E)  /* Port 1 Interrupt Vector Word */
#define OFS_P1IES             (0x0018)
#define OFS_P1IE              (0x001A)
#define OFS_P1IFG             (0x001C)
#define OFS_P2IN              (0x0001)
#define OFS_P2OUT             (0x0003)
#define OFS_P2DIR             (0x0005)
#define OFS_P2REN             (0x0007)
#define OFS_P2SEL0            (0x000B)
#define OFS_P2SEL1            (0x000D)
#define OFS_P2SELC            (0x0017)
#define OFS_P2IV              (0x001E)  /* Port 2 Interrupt Vector Word */
#define OFS_P2IES             (0x0019)
#define OFS_P2IE              (0x001B)
#define OFS_P2IFG             (0x001d)
#define P1IN                (PAIN_L)  /* Port 1 Input */
#define P1OUT               (PAOUT_L) /* Port 1 Output */
#define P1DIR               (PADIR_L) /* Port 1 Direction */
#define P1REN               (PAREN_L) /* Port 1 Resistor Enable */
#define P1SEL0              (PASEL0_L) /* Port 1 Selection 0 */
#define P1SEL1              (PASEL1_L) /* Port 1 Selection 1 */
#define P1SELC              (PASELC_L) /* Port 1 Complement Selection */
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
#define P2SEL0              (PASEL0_H) /* Port 2 Selection 0 */
#define P2SEL1              (PASEL1_H) /* Port 2 Selection 1 */
#define P2SELC              (PASELC_H) /* Port 2 Complement Selection */
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



#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port3/4 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT3_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT4_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */

#define OFS_PBIN              (0x0000)  /* Port B Input */
#define OFS_PBIN_L             OFS_PBIN
#define OFS_PBIN_H             OFS_PBIN+1
#define OFS_PBOUT             (0x0002)  /* Port B Output */
#define OFS_PBOUT_L            OFS_PBOUT
#define OFS_PBOUT_H            OFS_PBOUT+1
#define OFS_PBDIR             (0x0004)  /* Port B Direction */
#define OFS_PBDIR_L            OFS_PBDIR
#define OFS_PBDIR_H            OFS_PBDIR+1
#define OFS_PBREN             (0x0006)  /* Port B Resistor Enable */
#define OFS_PBREN_L            OFS_PBREN
#define OFS_PBREN_H            OFS_PBREN+1
#define OFS_PBSEL0            (0x000A)  /* Port B Selection 0 */
#define OFS_PBSEL0_L           OFS_PBSEL0
#define OFS_PBSEL0_H           OFS_PBSEL0+1
#define OFS_PBSEL1            (0x000C)  /* Port B Selection 1 */
#define OFS_PBSEL1_L           OFS_PBSEL1
#define OFS_PBSEL1_H           OFS_PBSEL1+1
#define OFS_PBSELC            (0x0016)  /* Port B Complement Selection */
#define OFS_PBSELC_L           OFS_PBSELC
#define OFS_PBSELC_H           OFS_PBSELC+1
#define OFS_PBIES             (0x0018)  /* Port B Interrupt Edge Select */
#define OFS_PBIES_L            OFS_PBIES
#define OFS_PBIES_H            OFS_PBIES+1
#define OFS_PBIE              (0x001A)  /* Port B Interrupt Enable */
#define OFS_PBIE_L             OFS_PBIE
#define OFS_PBIE_H             OFS_PBIE+1
#define OFS_PBIFG             (0x001C)  /* Port B Interrupt Flag */
#define OFS_PBIFG_L            OFS_PBIFG
#define OFS_PBIFG_H            OFS_PBIFG+1























#define OFS_P3IN              (0x0000)
#define OFS_P3OUT             (0x0002)
#define OFS_P3DIR             (0x0004)
#define OFS_P3REN             (0x0006)
#define OFS_P3SEL0            (0x000A)
#define OFS_P3SEL1            (0x000C)
#define OFS_P3SELC            (0x0016)
#define OFS_P3IV              (0x000E)  /* Port 3 Interrupt Vector Word */
#define OFS_P3IES             (0x0018)
#define OFS_P3IE              (0x001A)
#define OFS_P3IFG             (0x001C)
#define OFS_P4IN              (0x0001)
#define OFS_P4OUT             (0x0003)
#define OFS_P4DIR             (0x0005)
#define OFS_P4REN             (0x0007)
#define OFS_P4SEL0            (0x000B)
#define OFS_P4SEL1            (0x000D)
#define OFS_P4SELC            (0x0017)
#define OFS_P4IV              (0x001E)  /* Port 4 Interrupt Vector Word */
#define OFS_P4IES             (0x0019)
#define OFS_P4IE              (0x001B)
#define OFS_P4IFG             (0x001d)
#define P3IN                (PBIN_L)  /* Port 3 Input */
#define P3OUT               (PBOUT_L) /* Port 3 Output */
#define P3DIR               (PBDIR_L) /* Port 3 Direction */
#define P3REN               (PBREN_L) /* Port 3 Resistor Enable */
#define P3SEL0              (PBSEL0_L) /* Port 3 Selection 0 */
#define P3SEL1              (PBSEL1_L) /* Port 3 Selection 1 */
#define P3SELC              (PBSELC_L) /* Port 3 Complement Selection */
#define P3IES               (PBIES_L) /* Port 3 Interrupt Edge Select */
#define P3IE                (PBIE_L)  /* Port 3 Interrupt Enable */
#define P3IFG               (PBIFG_L) /* Port 3 Interrupt Flag */

//Definitions for P3IV
#define P3IV_NONE            (0x0000)    /* No Interrupt pending */
#define P3IV_P3IFG0          (0x0002)    /* P3IV P3IFG.0 */
#define P3IV_P3IFG1          (0x0004)    /* P3IV P3IFG.1 */
#define P3IV_P3IFG2          (0x0006)    /* P3IV P3IFG.2 */
#define P3IV_P3IFG3          (0x0008)    /* P3IV P3IFG.3 */
#define P3IV_P3IFG4          (0x000A)    /* P3IV P3IFG.4 */
#define P3IV_P3IFG5          (0x000C)    /* P3IV P3IFG.5 */
#define P3IV_P3IFG6          (0x000E)    /* P3IV P3IFG.6 */
#define P3IV_P3IFG7          (0x0010)    /* P3IV P3IFG.7 */


#define P4IN                (PBIN_H)  /* Port 4 Input */
#define P4OUT               (PBOUT_H) /* Port 4 Output */
#define P4DIR               (PBDIR_H) /* Port 4 Direction */
#define P4REN               (PBREN_H) /* Port 4 Resistor Enable */
#define P4SEL0              (PBSEL0_H) /* Port 4 Selection 0 */
#define P4SEL1              (PBSEL1_H) /* Port 4 Selection 1 */
#define P4SELC              (PBSELC_H) /* Port 4 Complement Selection */
#define P4IES               (PBIES_H) /* Port 4 Interrupt Edge Select */
#define P4IE                (PBIE_H)  /* Port 4 Interrupt Enable */
#define P4IFG               (PBIFG_H) /* Port 4 Interrupt Flag */

//Definitions for P4IV
#define P4IV_NONE            (0x0000)    /* No Interrupt pending */
#define P4IV_P4IFG0          (0x0002)    /* P4IV P4IFG.0 */
#define P4IV_P4IFG1          (0x0004)    /* P4IV P4IFG.1 */
#define P4IV_P4IFG2          (0x0006)    /* P4IV P4IFG.2 */
#define P4IV_P4IFG3          (0x0008)    /* P4IV P4IFG.3 */
#define P4IV_P4IFG4          (0x000A)    /* P4IV P4IFG.4 */
#define P4IV_P4IFG5          (0x000C)    /* P4IV P4IFG.5 */
#define P4IV_P4IFG6          (0x000E)    /* P4IV P4IFG.6 */
#define P4IV_P4IFG7          (0x0010)    /* P4IV P4IFG.7 */



#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port5/6 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT5_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT6_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTC_R__        /* Definition to show that Module is available */

#define OFS_PCIN              (0x0000)  /* Port C Input */
#define OFS_PCIN_L             OFS_PCIN
#define OFS_PCIN_H             OFS_PCIN+1
#define OFS_PCOUT             (0x0002)  /* Port C Output */
#define OFS_PCOUT_L            OFS_PCOUT
#define OFS_PCOUT_H            OFS_PCOUT+1
#define OFS_PCDIR             (0x0004)  /* Port C Direction */
#define OFS_PCDIR_L            OFS_PCDIR
#define OFS_PCDIR_H            OFS_PCDIR+1
#define OFS_PCREN             (0x0006)  /* Port C Resistor Enable */
#define OFS_PCREN_L            OFS_PCREN
#define OFS_PCREN_H            OFS_PCREN+1
#define OFS_PCSEL0            (0x000A)  /* Port C Selection 0 */
#define OFS_PCSEL0_L           OFS_PCSEL0
#define OFS_PCSEL0_H           OFS_PCSEL0+1
#define OFS_PCSEL1            (0x000C)  /* Port C Selection 1 */
#define OFS_PCSEL1_L           OFS_PCSEL1
#define OFS_PCSEL1_H           OFS_PCSEL1+1
#define OFS_PCSELC            (0x0016)  /* Port C Complement Selection */
#define OFS_PCSELC_L           OFS_PCSELC
#define OFS_PCSELC_H           OFS_PCSELC+1
#define OFS_PCIES             (0x0018)  /* Port C Interrupt Edge Select */
#define OFS_PCIES_L            OFS_PCIES
#define OFS_PCIES_H            OFS_PCIES+1
#define OFS_PCIE              (0x001A)  /* Port C Interrupt Enable */
#define OFS_PCIE_L             OFS_PCIE
#define OFS_PCIE_H             OFS_PCIE+1
#define OFS_PCIFG             (0x001C)  /* Port C Interrupt Flag */
#define OFS_PCIFG_L            OFS_PCIFG
#define OFS_PCIFG_H            OFS_PCIFG+1























#define OFS_P5IN              (0x0000)
#define OFS_P5OUT             (0x0002)
#define OFS_P5DIR             (0x0004)
#define OFS_P5REN             (0x0006)
#define OFS_P5SEL0            (0x000A)
#define OFS_P5SEL1            (0x000C)
#define OFS_P5SELC            (0x0016)
#define OFS_P5IV              (0x000E)  /* Port 5 Interrupt Vector Word */
#define OFS_P5IES             (0x0018)
#define OFS_P5IE              (0x001A)
#define OFS_P5IFG             (0x001C)
#define OFS_P6IN              (0x0001)
#define OFS_P6OUT             (0x0003)
#define OFS_P6DIR             (0x0005)
#define OFS_P6REN             (0x0007)
#define OFS_P6SEL0            (0x000B)
#define OFS_P6SEL1            (0x000D)
#define OFS_P6SELC            (0x0017)
#define OFS_P6IV              (0x001E)  /* Port 6 Interrupt Vector Word */
#define OFS_P6IES             (0x0019)
#define OFS_P6IE              (0x001B)
#define OFS_P6IFG             (0x001d)
#define P5IN                (PCIN_L)  /* Port 5 Input */
#define P5OUT               (PCOUT_L) /* Port 5 Output */
#define P5DIR               (PCDIR_L) /* Port 5 Direction */
#define P5REN               (PCREN_L) /* Port 5 Resistor Enable */
#define P5SEL0              (PCSEL0_L) /* Port 5 Selection 0 */
#define P5SEL1              (PCSEL1_L) /* Port 5 Selection 1 */
#define P5SELC              (PCSELC_L) /* Port 5 Complement Selection */
#define P5IES               (PCIES_L) /* Port 5 Interrupt Edge Select */
#define P5IE                (PCIE_L)  /* Port 5 Interrupt Enable */
#define P5IFG               (PCIFG_L) /* Port 5 Interrupt Flag */

//Definitions for P5IV
#define P5IV_NONE            (0x0000)    /* No Interrupt pending */
#define P5IV_P5IFG0          (0x0002)    /* P5IV P5IFG.0 */
#define P5IV_P5IFG1          (0x0004)    /* P5IV P5IFG.1 */
#define P5IV_P5IFG2          (0x0006)    /* P5IV P5IFG.2 */
#define P5IV_P5IFG3          (0x0008)    /* P5IV P5IFG.3 */
#define P5IV_P5IFG4          (0x000A)    /* P5IV P5IFG.4 */
#define P5IV_P5IFG5          (0x000C)    /* P5IV P5IFG.5 */
#define P5IV_P5IFG6          (0x000E)    /* P5IV P5IFG.6 */
#define P5IV_P5IFG7          (0x0010)    /* P5IV P5IFG.7 */


#define P6IN                (PCIN_H)  /* Port 6 Input */
#define P6OUT               (PCOUT_H) /* Port 6 Output */
#define P6DIR               (PCDIR_H) /* Port 6 Direction */
#define P6REN               (PCREN_H) /* Port 6 Resistor Enable */
#define P6SEL0              (PCSEL0_H) /* Port 6 Selection 0 */
#define P6SEL1              (PCSEL1_H) /* Port 6 Selection 1 */
#define P6SELC              (PCSELC_H) /* Port 6 Complement Selection */
#define P6IES               (PCIES_H) /* Port 6 Interrupt Edge Select */
#define P6IE                (PCIE_H)  /* Port 6 Interrupt Enable */
#define P6IFG               (PCIFG_H) /* Port 6 Interrupt Flag */

//Definitions for P6IV
#define P6IV_NONE            (0x0000)    /* No Interrupt pending */
#define P6IV_P6IFG0          (0x0002)    /* P6IV P6IFG.0 */
#define P6IV_P6IFG1          (0x0004)    /* P6IV P6IFG.1 */
#define P6IV_P6IFG2          (0x0006)    /* P6IV P6IFG.2 */
#define P6IV_P6IFG3          (0x0008)    /* P6IV P6IFG.3 */
#define P6IV_P6IFG4          (0x000A)    /* P6IV P6IFG.4 */
#define P6IV_P6IFG5          (0x000C)    /* P6IV P6IFG.5 */
#define P6IV_P6IFG6          (0x000E)    /* P6IV P6IFG.6 */
#define P6IV_P6IFG7          (0x0010)    /* P6IV P6IFG.7 */



#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port7/8 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT7_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT8_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTD_R__        /* Definition to show that Module is available */

#define OFS_PDIN              (0x0000)  /* Port D Input */
#define OFS_PDIN_L             OFS_PDIN
#define OFS_PDIN_H             OFS_PDIN+1
#define OFS_PDOUT             (0x0002)  /* Port D Output */
#define OFS_PDOUT_L            OFS_PDOUT
#define OFS_PDOUT_H            OFS_PDOUT+1
#define OFS_PDDIR             (0x0004)  /* Port D Direction */
#define OFS_PDDIR_L            OFS_PDDIR
#define OFS_PDDIR_H            OFS_PDDIR+1
#define OFS_PDREN             (0x0006)  /* Port D Resistor Enable */
#define OFS_PDREN_L            OFS_PDREN
#define OFS_PDREN_H            OFS_PDREN+1
#define OFS_PDSEL0            (0x000A)  /* Port D Selection 0 */
#define OFS_PDSEL0_L           OFS_PDSEL0
#define OFS_PDSEL0_H           OFS_PDSEL0+1
#define OFS_PDSEL1            (0x000C)  /* Port D Selection 1 */
#define OFS_PDSEL1_L           OFS_PDSEL1
#define OFS_PDSEL1_H           OFS_PDSEL1+1
#define OFS_PDSELC            (0x0016)  /* Port D Complement Selection */
#define OFS_PDSELC_L           OFS_PDSELC
#define OFS_PDSELC_H           OFS_PDSELC+1
#define OFS_PDIES             (0x0018)  /* Port D Interrupt Edge Select */
#define OFS_PDIES_L            OFS_PDIES
#define OFS_PDIES_H            OFS_PDIES+1
#define OFS_PDIE              (0x001A)  /* Port D Interrupt Enable */
#define OFS_PDIE_L             OFS_PDIE
#define OFS_PDIE_H             OFS_PDIE+1
#define OFS_PDIFG             (0x001C)  /* Port D Interrupt Flag */
#define OFS_PDIFG_L            OFS_PDIFG
#define OFS_PDIFG_H            OFS_PDIFG+1























#define OFS_P7IN              (0x0000)
#define OFS_P7OUT             (0x0002)
#define OFS_P7DIR             (0x0004)
#define OFS_P7REN             (0x0006)
#define OFS_P7SEL0            (0x000A)
#define OFS_P7SEL1            (0x000C)
#define OFS_P7SELC            (0x0016)
#define OFS_P7IV              (0x000E)  /* Port 7 Interrupt Vector Word */
#define OFS_P7IES             (0x0018)
#define OFS_P7IE              (0x001A)
#define OFS_P7IFG             (0x001C)
#define OFS_P8IN              (0x0001)
#define OFS_P8OUT             (0x0003)
#define OFS_P8DIR             (0x0005)
#define OFS_P8REN             (0x0007)
#define OFS_P8SEL0            (0x000B)
#define OFS_P8SEL1            (0x000D)
#define OFS_P8SELC            (0x0017)
#define OFS_P8IV              (0x001E)  /* Port 8 Interrupt Vector Word */
#define OFS_P8IES             (0x0019)
#define OFS_P8IE              (0x001B)
#define OFS_P8IFG             (0x001d)
#define P7IN                (PDIN_L)  /* Port 7 Input */
#define P7OUT               (PDOUT_L) /* Port 7 Output */
#define P7DIR               (PDDIR_L) /* Port 7 Direction */
#define P7REN               (PDREN_L) /* Port 7 Resistor Enable */
#define P7SEL0              (PDSEL0_L) /* Port 7 Selection 0 */
#define P7SEL1              (PDSEL1_L) /* Port 7 Selection 1 */
#define P7SELC              (PDSELC_L) /* Port 7 Complement Selection */
#define P7IES               (PDIES_L) /* Port 7 Interrupt Edge Select */
#define P7IE                (PDIE_L)  /* Port 7 Interrupt Enable */
#define P7IFG               (PDIFG_L) /* Port 7 Interrupt Flag */

//Definitions for P7IV
#define P7IV_NONE            (0x0000)    /* No Interrupt pending */
#define P7IV_P7IFG0          (0x0002)    /* P7IV P7IFG.0 */
#define P7IV_P7IFG1          (0x0004)    /* P7IV P7IFG.1 */
#define P7IV_P7IFG2          (0x0006)    /* P7IV P7IFG.2 */
#define P7IV_P7IFG3          (0x0008)    /* P7IV P7IFG.3 */
#define P7IV_P7IFG4          (0x000A)    /* P7IV P7IFG.4 */
#define P7IV_P7IFG5          (0x000C)    /* P7IV P7IFG.5 */
#define P7IV_P7IFG6          (0x000E)    /* P7IV P7IFG.6 */
#define P7IV_P7IFG7          (0x0010)    /* P7IV P7IFG.7 */


#define P8IN                (PDIN_H)  /* Port 8 Input */
#define P8OUT               (PDOUT_H) /* Port 8 Output */
#define P8DIR               (PDDIR_H) /* Port 8 Direction */
#define P8REN               (PDREN_H) /* Port 8 Resistor Enable */
#define P8SEL0              (PDSEL0_H) /* Port 8 Selection 0 */
#define P8SEL1              (PDSEL1_H) /* Port 8 Selection 1 */
#define P8SELC              (PDSELC_H) /* Port 8 Complement Selection */
#define P8IES               (PDIES_H) /* Port 8 Interrupt Edge Select */
#define P8IE                (PDIE_H)  /* Port 8 Interrupt Enable */
#define P8IFG               (PDIFG_H) /* Port 8 Interrupt Flag */

//Definitions for P8IV
#define P8IV_NONE            (0x0000)    /* No Interrupt pending */
#define P8IV_P8IFG0          (0x0002)    /* P8IV P8IFG.0 */
#define P8IV_P8IFG1          (0x0004)    /* P8IV P8IFG.1 */
#define P8IV_P8IFG2          (0x0006)    /* P8IV P8IFG.2 */
#define P8IV_P8IFG3          (0x0008)    /* P8IV P8IFG.3 */
#define P8IV_P8IFG4          (0x000A)    /* P8IV P8IFG.4 */
#define P8IV_P8IFG5          (0x000C)    /* P8IV P8IFG.5 */
#define P8IV_P8IFG6          (0x000E)    /* P8IV P8IFG.6 */
#define P8IV_P8IFG7          (0x0010)    /* P8IV P8IFG.7 */



#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port9/10 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT9_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT10_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTE_R__        /* Definition to show that Module is available */

#define OFS_PEIN              (0x0000)  /* Port E Input */
#define OFS_PEIN_L             OFS_PEIN
#define OFS_PEIN_H             OFS_PEIN+1
#define OFS_PEOUT             (0x0002)  /* Port E Output */
#define OFS_PEOUT_L            OFS_PEOUT
#define OFS_PEOUT_H            OFS_PEOUT+1
#define OFS_PEDIR             (0x0004)  /* Port E Direction */
#define OFS_PEDIR_L            OFS_PEDIR
#define OFS_PEDIR_H            OFS_PEDIR+1
#define OFS_PEREN             (0x0006)  /* Port E Resistor Enable */
#define OFS_PEREN_L            OFS_PEREN
#define OFS_PEREN_H            OFS_PEREN+1
#define OFS_PESEL0            (0x000A)  /* Port E Selection 0 */
#define OFS_PESEL0_L           OFS_PESEL0
#define OFS_PESEL0_H           OFS_PESEL0+1
#define OFS_PESEL1            (0x000C)  /* Port E Selection 1 */
#define OFS_PESEL1_L           OFS_PESEL1
#define OFS_PESEL1_H           OFS_PESEL1+1
#define OFS_PESELC            (0x0016)  /* Port E Complement Selection */
#define OFS_PESELC_L           OFS_PESELC
#define OFS_PESELC_H           OFS_PESELC+1
#define OFS_PEIES             (0x0018)  /* Port E Interrupt Edge Select */
#define OFS_PEIES_L            OFS_PEIES
#define OFS_PEIES_H            OFS_PEIES+1
#define OFS_PEIE              (0x001A)  /* Port E Interrupt Enable */
#define OFS_PEIE_L             OFS_PEIE
#define OFS_PEIE_H             OFS_PEIE+1
#define OFS_PEIFG             (0x001C)  /* Port E Interrupt Flag */
#define OFS_PEIFG_L            OFS_PEIFG
#define OFS_PEIFG_H            OFS_PEIFG+1























#define OFS_P9IN              (0x0000)
#define OFS_P9OUT             (0x0002)
#define OFS_P9DIR             (0x0004)
#define OFS_P9REN             (0x0006)
#define OFS_P9SEL0            (0x000A)
#define OFS_P9SEL1            (0x000C)
#define OFS_P9SELC            (0x0016)
#define OFS_P9IV              (0x000E)  /* Port 9 Interrupt Vector Word */
#define OFS_P9IES             (0x0018)
#define OFS_P9IE              (0x001A)
#define OFS_P9IFG             (0x001C)
#define OFS_P10IN             (0x0001)
#define OFS_P10OUT            (0x0003)
#define OFS_P10DIR            (0x0005)
#define OFS_P10REN            (0x0007)
#define OFS_P10SEL0           (0x000B)
#define OFS_P10SEL1           (0x000D)
#define OFS_P10SELC           (0x0017)
#define OFS_P10IV             (0x001E)  /* Port 10 Interrupt Vector Word */
#define OFS_P10IES            (0x0019)
#define OFS_P10IE             (0x001B)
#define OFS_P10IFG            (0x001d)
#define P9IN                (PEIN_L)  /* Port 9 Input */
#define P9OUT               (PEOUT_L) /* Port 9 Output */
#define P9DIR               (PEDIR_L) /* Port 9 Direction */
#define P9REN               (PEREN_L) /* Port 9 Resistor Enable */
#define P9SEL0              (PESEL0_L) /* Port 9 Selection 0 */
#define P9SEL1              (PESEL1_L) /* Port 9 Selection 1 */
#define P9SELC              (PESELC_L) /* Port 9 Complement Selection */
#define P9IES               (PEIES_L) /* Port 9 Interrupt Edge Select */
#define P9IE                (PEIE_L)  /* Port 9 Interrupt Enable */
#define P9IFG               (PEIFG_L) /* Port 9 Interrupt Flag */

//Definitions for P9IV
#define P9IV_NONE            (0x0000)    /* No Interrupt pending */
#define P9IV_P9IFG0          (0x0002)    /* P9IV P9IFG.0 */
#define P9IV_P9IFG1          (0x0004)    /* P9IV P9IFG.1 */
#define P9IV_P9IFG2          (0x0006)    /* P9IV P9IFG.2 */
#define P9IV_P9IFG3          (0x0008)    /* P9IV P9IFG.3 */
#define P9IV_P9IFG4          (0x000A)    /* P9IV P9IFG.4 */
#define P9IV_P9IFG5          (0x000C)    /* P9IV P9IFG.5 */
#define P9IV_P9IFG6          (0x000E)    /* P9IV P9IFG.6 */
#define P9IV_P9IFG7          (0x0010)    /* P9IV P9IFG.7 */


#define P10IN                (PEIN_H)  /* Port 10 Input */
#define P10OUT               (PEOUT_H) /* Port 10 Output */
#define P10DIR               (PEDIR_H) /* Port 10 Direction */
#define P10REN               (PEREN_H) /* Port 10 Resistor Enable */
#define P10SEL0              (PESEL0_H) /* Port 10 Selection 0 */
#define P10SEL1              (PESEL1_H) /* Port 10 Selection 1 */
#define P10SELC              (PESELC_H) /* Port 10 Complement Selection */
#define P10IES               (PEIES_H) /* Port 10 Interrupt Edge Select */
#define P10IE                (PEIE_H)  /* Port 10 Interrupt Enable */
#define P10IFG               (PEIFG_H) /* Port 10 Interrupt Flag */

//Definitions for P10IV
#define P10IV_NONE            (0x0000)    /* No Interrupt pending */
#define P10IV_P10IFG0          (0x0002)    /* P10IV P10IFG.0 */
#define P10IV_P10IFG1          (0x0004)    /* P10IV P10IFG.1 */
#define P10IV_P10IFG2          (0x0006)    /* P10IV P10IFG.2 */
#define P10IV_P10IFG3          (0x0008)    /* P10IV P10IFG.3 */
#define P10IV_P10IFG4          (0x000A)    /* P10IV P10IFG.4 */
#define P10IV_P10IFG5          (0x000C)    /* P10IV P10IFG.5 */
#define P10IV_P10IFG6          (0x000E)    /* P10IV P10IFG.6 */
#define P10IV_P10IFG7          (0x0010)    /* P10IV P10IFG.7 */



#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port11 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT11_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTF_R__        /* Definition to show that Module is available */

#define OFS_PFIN              (0x0000)  /* Port F Input */
#define OFS_PFIN_L             OFS_PFIN
#define OFS_PFIN_H             OFS_PFIN+1
#define OFS_PFOUT             (0x0002)  /* Port F Output */
#define OFS_PFOUT_L            OFS_PFOUT
#define OFS_PFOUT_H            OFS_PFOUT+1
#define OFS_PFDIR             (0x0004)  /* Port F Direction */
#define OFS_PFDIR_L            OFS_PFDIR
#define OFS_PFDIR_H            OFS_PFDIR+1
#define OFS_PFREN             (0x0006)  /* Port F Resistor Enable */
#define OFS_PFREN_L            OFS_PFREN
#define OFS_PFREN_H            OFS_PFREN+1
#define OFS_PFSEL0            (0x000A)  /* Port F Selection 0 */
#define OFS_PFSEL0_L           OFS_PFSEL0
#define OFS_PFSEL0_H           OFS_PFSEL0+1
#define OFS_PFSEL1            (0x000C)  /* Port F Selection 1 */
#define OFS_PFSEL1_L           OFS_PFSEL1
#define OFS_PFSEL1_H           OFS_PFSEL1+1
#define OFS_PFSELC            (0x0016)  /* Port F Complement Selection */
#define OFS_PFSELC_L           OFS_PFSELC
#define OFS_PFSELC_H           OFS_PFSELC+1
#define OFS_PFIES             (0x0018)  /* Port F Interrupt Edge Select */
#define OFS_PFIES_L            OFS_PFIES
#define OFS_PFIES_H            OFS_PFIES+1
#define OFS_PFIE              (0x001A)  /* Port F Interrupt Enable */
#define OFS_PFIE_L             OFS_PFIE
#define OFS_PFIE_H             OFS_PFIE+1
#define OFS_PFIFG             (0x001C)  /* Port F Interrupt Flag */
#define OFS_PFIFG_L            OFS_PFIFG
#define OFS_PFIFG_H            OFS_PFIFG+1




















#define OFS_P11IN             (0x0000)
#define OFS_P11OUT            (0x0002)
#define OFS_P11DIR            (0x0004)
#define OFS_P11REN            (0x0006)
#define OFS_P11SEL0           (0x000A)
#define OFS_P11SEL1           (0x000C)
#define OFS_P11SELC           (0x0016)
#define OFS_P11IV             (0x000E)  /* Port 11 Interrupt Vector Word */
#define OFS_P11IES            (0x0018)
#define OFS_P11IE             (0x001A)
#define OFS_P11IFG            (0x001C)
#define P11IN                (PFIN_L)  /* Port 11 Input */
#define P11OUT               (PFOUT_L) /* Port 11 Output */
#define P11DIR               (PFDIR_L) /* Port 11 Direction */
#define P11REN               (PFREN_L) /* Port 11 Resistor Enable */
#define P11SEL0              (PFSEL0_L) /* Port 11 Selection0 */
#define P11SEL1              (PFSEL1_L) /* Port 11 Selection1 */
#define OFS_P11SELC           (0x0017)

#define P11IES               (PFIES_L) /* Port 11 Interrupt Edge Select */
#define P11IE                (PFIE_L)  /* Port 11 Interrupt Enable */
#define P11IFG               (PFIFG_L) /* Port 11 Interrupt Flag */

//Definitions for P11IV
#define P11IV_NONE            (0x0000)    /* No Interrupt pending */
#define P11IV_P11IFG0          (0x0002)    /* P11IV P11IFG.0 */
#define P11IV_P11IFG1          (0x0004)    /* P11IV P11IFG.1 */
#define P11IV_P11IFG2          (0x0006)    /* P11IV P11IFG.2 */
#define P11IV_P11IFG3          (0x0008)    /* P11IV P11IFG.3 */
#define P11IV_P11IFG4          (0x000A)    /* P11IV P11IFG.4 */
#define P11IV_P11IFG5          (0x000C)    /* P11IV P11IFG.5 */
#define P11IV_P11IFG6          (0x000E)    /* P11IV P11IFG.6 */
#define P11IV_P11IFG7          (0x0010)    /* P11IV P11IFG.7 */




#endif
#endif
/************************************************************
* DIGITAL I/O PortJ Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORTJ_R__        /* Definition to show that Module is available */

#define OFS_PJIN              (0x0000)  /* Port J Input */
#define OFS_PJIN_L             OFS_PJIN
#define OFS_PJIN_H             OFS_PJIN+1
#define OFS_PJOUT             (0x0002)  /* Port J Output */
#define OFS_PJOUT_L            OFS_PJOUT
#define OFS_PJOUT_H            OFS_PJOUT+1
#define OFS_PJDIR             (0x0004)  /* Port J Direction */
#define OFS_PJDIR_L            OFS_PJDIR
#define OFS_PJDIR_H            OFS_PJDIR+1
#define OFS_PJREN             (0x0006)  /* Port J Resistor Enable */
#define OFS_PJREN_L            OFS_PJREN
#define OFS_PJREN_H            OFS_PJREN+1
#define OFS_PJSEL0            (0x000A)  /* Port J Selection 0 */
#define OFS_PJSEL0_L           OFS_PJSEL0
#define OFS_PJSEL0_H           OFS_PJSEL0+1
#define OFS_PJSEL1            (0x000C)  /* Port J Selection 1 */
#define OFS_PJSEL1_L           OFS_PJSEL1
#define OFS_PJSEL1_H           OFS_PJSEL1+1
#define OFS_PJSELC            (0x0016)  /* Port J Complement Selection */
#define OFS_PJSELC_L           OFS_PJSELC
#define OFS_PJSELC_H           OFS_PJSELC+1













#endif
/*************************************************************
* RAM Control Module for FRAM
*************************************************************/
#ifdef  __MSP430_HAS_RC_FRAM__        /* Definition to show that Module is available */

#define OFS_RCCTL0            (0x0000)  /* Ram Controller Control Register */
#define OFS_RCCTL0_L           OFS_RCCTL0
#define OFS_RCCTL0_H           OFS_RCCTL0+1

/* RCCTL0 Control Bits */
#define RCRS0OFF0           (0x0001)  /* RAM Controller RAM Sector 0 Off Bit: 0 */
#define RCRS0OFF1           (0x0002)  /* RAM Controller RAM Sector 0 Off Bit: 1 */
#define RCRS4OFF0           (0x0100)  /* RAM Controller RAM Sector 4 Off Bit: 0 */
#define RCRS4OFF1           (0x0200)  /* RAM Controller RAM Sector 4 Off Bit: 1 */
#define RCRS5OFF0           (0x0400)  /* RAM Controller RAM Sector 5 Off Bit: 0 */
#define RCRS5OFF1           (0x0800)  /* RAM Controller RAM Sector 5 Off Bit: 1 */
#define RCRS6OFF0           (0x1000)  /* RAM Controller RAM Sector 6 Off Bit: 0 */
#define RCRS6OFF1           (0x2000)  /* RAM Controller RAM Sector 6 Off Bit: 1 */
#define RCRS7OFF0           (0x4000)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 0 */
#define RCRS7OFF1           (0x8000)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 1 */

/* RCCTL0 Control Bits */
#define RCRS0OFF0_L         (0x0001)  /* RAM Controller RAM Sector 0 Off Bit: 0 */
#define RCRS0OFF1_L         (0x0002)  /* RAM Controller RAM Sector 0 Off Bit: 1 */

/* RCCTL0 Control Bits */
#define RCRS4OFF0_H         (0x0001)  /* RAM Controller RAM Sector 4 Off Bit: 0 */
#define RCRS4OFF1_H         (0x0002)  /* RAM Controller RAM Sector 4 Off Bit: 1 */
#define RCRS5OFF0_H         (0x0004)  /* RAM Controller RAM Sector 5 Off Bit: 0 */
#define RCRS5OFF1_H         (0x0008)  /* RAM Controller RAM Sector 5 Off Bit: 1 */
#define RCRS6OFF0_H         (0x0010)  /* RAM Controller RAM Sector 6 Off Bit: 0 */
#define RCRS6OFF1_H         (0x0020)  /* RAM Controller RAM Sector 6 Off Bit: 1 */
#define RCRS7OFF0_H         (0x0040)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 0 */
#define RCRS7OFF1_H         (0x0080)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 1 */

#define RCKEY               (0x5A00)

#define RCRS0OFF_0          (0x0000)  /* RAM Controller RAM Sector 0 Off : 0 */
#define RCRS0OFF_1          (0x0001)  /* RAM Controller RAM Sector 0 Off : 1 */
#define RCRS0OFF_2          (0x0002)  /* RAM Controller RAM Sector 0 Off : 2 */
#define RCRS0OFF_3          (0x0003)  /* RAM Controller RAM Sector 0 Off : 3 */
#define RCRS4OFF_0          (0x0000)  /* RAM Controller RAM Sector 4 Off : 0 */
#define RCRS4OFF_2          (0x0100)  /* RAM Controller RAM Sector 4 Off : 1 */
#define RCRS4OFF_3          (0x0200)  /* RAM Controller RAM Sector 4 Off : 2 */
#define RCRS4OFF_4          (0x0300)  /* RAM Controller RAM Sector 4 Off : 3 */
#define RCRS5OFF_0          (0x0000)  /* RAM Controller RAM Sector 5 Off : 0 */
#define RCRS5OFF_1          (0x0400)  /* RAM Controller RAM Sector 5 Off : 1 */
#define RCRS5OFF_2          (0x0800)  /* RAM Controller RAM Sector 5 Off : 2 */
#define RCRS5OFF_3          (0x0C00)  /* RAM Controller RAM Sector 5 Off : 3 */
#define RCRS6OFF_0          (0x0000)  /* RAM Controller RAM Sector 6 Off : 0 */
#define RCRS6OFF_1          (0x0100)  /* RAM Controller RAM Sector 6 Off : 1 */
#define RCRS6OFF_2          (0x0200)  /* RAM Controller RAM Sector 6 Off : 2 */
#define RCRS6OFF_3          (0x0300)  /* RAM Controller RAM Sector 6 Off : 3 */
#define RCRS7OFF_0          (0x0000)  /* RAM Controller RAM Sector 7 Off : 0 */
#define RCRS7OFF_1          (0x4000)  /* RAM Controller RAM Sector 7 Off : 1 */
#define RCRS7OFF_2          (0x8000)  /* RAM Controller RAM Sector 7 Off : 2*/
#define RCRS7OFF_3          (0xC000)  /* RAM Controller RAM Sector 7 Off : 3*/


#endif
/************************************************************
* Shared Reference
************************************************************/
#ifdef  __MSP430_HAS_REF_A__           /* Definition to show that Module is available */

#define OFS_REFCTL0           (0x0000)  /* REF Shared Reference control register 0 */
#define OFS_REFCTL0_L          OFS_REFCTL0
#define OFS_REFCTL0_H          OFS_REFCTL0+1

/* REFCTL0 Control Bits */
#define REFON               (0x0001)  /* REF Reference On */
#define REFOUT              (0x0002)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFTCOFF            (0x0008)  /* REF Temp.Sensor off */
#define REFVSEL0            (0x0010)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1            (0x0020)  /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT            (0x0040)  /* REF Reference generator one-time trigger */
#define REFBGOT             (0x0080)  /* REF Bandgap and bandgap buffer one-time trigger */
#define REFGENACT           (0x0100)  /* REF Reference generator active */
#define REFBGACT            (0x0200)  /* REF Reference bandgap active */
#define REFGENBUSY          (0x0400)  /* REF Reference generator busy */
#define BGMODE              (0x0800)  /* REF Bandgap mode */
#define REFGENRDY           (0x1000)  /* REF Reference generator ready */
#define REFBGRDY            (0x2000)  /* REF Reference bandgap ready */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

/* REFCTL0 Control Bits */
#define REFON_L             (0x0001)  /* REF Reference On */
#define REFOUT_L            (0x0002)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFTCOFF_L          (0x0008)  /* REF Temp.Sensor off */
#define REFVSEL0_L          (0x0010)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1_L          (0x0020)  /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT_L          (0x0040)  /* REF Reference generator one-time trigger */
#define REFBGOT_L           (0x0080)  /* REF Bandgap and bandgap buffer one-time trigger */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

/* REFCTL0 Control Bits */
//#define RESERVED            (0x0004)  /* Reserved */
#define REFGENACT_H         (0x0001)  /* REF Reference generator active */
#define REFBGACT_H          (0x0002)  /* REF Reference bandgap active */
#define REFGENBUSY_H        (0x0004)  /* REF Reference generator busy */
#define BGMODE_H            (0x0008)  /* REF Bandgap mode */
#define REFGENRDY_H         (0x0010)  /* REF Reference generator ready */
#define REFBGRDY_H          (0x0020)  /* REF Reference bandgap ready */
//#define RESERVED            (0x4000)  /* Reserved */
//#define RESERVED            (0x8000)  /* Reserved */

#define REFVSEL_0           (0x0000)  /* REF Reference Voltage Level Select 1.2V */
#define REFVSEL_1           (0x0010)  /* REF Reference Voltage Level Select 2.0V */
#define REFVSEL_2           (0x0020)  /* REF Reference Voltage Level Select 2.5V */
#define REFVSEL_3           (0x0030)  /* REF Reference Voltage Level Select 2.5V */

#endif
/************************************************************
* Real Time Clock
************************************************************/
#ifdef  __MSP430_HAS_RTC_B__          /* Definition to show that Module is available */

#define OFS_RTCCTL01          (0x0000)  /* Real Timer Control 0/1 */
#define OFS_RTCCTL01_L         OFS_RTCCTL01
#define OFS_RTCCTL01_H         OFS_RTCCTL01+1
#define OFS_RTCCTL23          (0x0002)  /* Real Timer Control 2/3 */
#define OFS_RTCCTL23_L         OFS_RTCCTL23
#define OFS_RTCCTL23_H         OFS_RTCCTL23+1
#define OFS_RTCPS0CTL         (0x0008)  /* Real Timer Prescale Timer 0 Control */
#define OFS_RTCPS0CTL_L        OFS_RTCPS0CTL
#define OFS_RTCPS0CTL_H        OFS_RTCPS0CTL+1
#define OFS_RTCPS1CTL         (0x000A)  /* Real Timer Prescale Timer 1 Control */
#define OFS_RTCPS1CTL_L        OFS_RTCPS1CTL
#define OFS_RTCPS1CTL_H        OFS_RTCPS1CTL+1
#define OFS_RTCPS             (0x000C)  /* Real Timer Prescale Timer Control */
#define OFS_RTCPS_L            OFS_RTCPS
#define OFS_RTCPS_H            OFS_RTCPS+1
#define OFS_RTCIV             (0x000E)  /* Real Time Clock Interrupt Vector */
#define OFS_RTCTIM0           (0x0010)  /* Real Time Clock Time 0 */
#define OFS_RTCTIM0_L          OFS_RTCTIM0
#define OFS_RTCTIM0_H          OFS_RTCTIM0+1
#define OFS_RTCTIM1           (0x0012)  /* Real Time Clock Time 1 */
#define OFS_RTCTIM1_L          OFS_RTCTIM1
#define OFS_RTCTIM1_H          OFS_RTCTIM1+1
#define OFS_RTCDATE           (0x0014)  /* Real Time Clock Date */
#define OFS_RTCDATE_L          OFS_RTCDATE
#define OFS_RTCDATE_H          OFS_RTCDATE+1
#define OFS_RTCYEAR           (0x0016)  /* Real Time Clock Year */
#define OFS_RTCYEAR_L          OFS_RTCYEAR
#define OFS_RTCYEAR_H          OFS_RTCYEAR+1
#define OFS_RTCAMINHR         (0x0018)  /* Real Time Clock Alarm Min/Hour */
#define OFS_RTCAMINHR_L        OFS_RTCAMINHR
#define OFS_RTCAMINHR_H        OFS_RTCAMINHR+1
#define OFS_RTCADOWDAY        (0x001A)  /* Real Time Clock Alarm day of week/day */
#define OFS_RTCADOWDAY_L       OFS_RTCADOWDAY
#define OFS_RTCADOWDAY_H       OFS_RTCADOWDAY+1
#define OFS_BIN2BCD           (0x001C)  /* Real Time Binary-to-BCD conversion register */
#define OFS_BCD2BIN           (0x001E)  /* Real Time BCD-to-binary conversion register */
#define OFS_RTCSEC            (0x0010)
#define OFS_RTCMIN            (0x0011)
#define OFS_RTCHOUR           (0x0012)
#define OFS_RTCDOW            (0x0013)
#define OFS_RTCDAY            (0x0014)
#define OFS_RTCMON            (0x0015)
#define OFS_RTCAMIN           (0x0018)
#define OFS_RTCAHOUR          (0x0019)
#define OFS_RTCADOW           (0x001A)
#define OFS_RTCADAY           (0x001B)

#define RTCCTL0             RTCCTL01_L  /* Real Time Clock Control 0 */
#define RTCCTL1             RTCCTL01_H  /* Real Time Clock Control 1 */
#define RTCCTL2             RTCCTL23_L  /* Real Time Clock Control 2 */
#define RTCCTL3             RTCCTL23_H  /* Real Time Clock Control 3 */
#define RTCNT12             RTCTIM0
#define RTCNT34             RTCTIM1
#define RTCNT1              RTCTIM0_L
#define RTCNT2              RTCTIM0_H
#define RTCNT3              RTCTIM1_L
#define RTCNT4              RTCTIM1_H
#define RTCSEC              RTCTIM0_L
#define RTCMIN              RTCTIM0_H
#define RTCHOUR             RTCTIM1_L
#define RTCDOW              RTCTIM1_H
#define RTCDAY              RTCDATE_L
#define RTCMON              RTCDATE_H
#define RTCYEARL            RTCYEAR_L
#define RTCYEARH            RTCYEAR_H
#define RT0PS               RTCPS_L
#define RT1PS               RTCPS_H
#define RTCAMIN             RTCAMINHR_L  /* Real Time Clock Alarm Min */
#define RTCAHOUR            RTCAMINHR_H  /* Real Time Clock Alarm Hour */
#define RTCADOW             RTCADOWDAY_L /* Real Time Clock Alarm day of week */
#define RTCADAY             RTCADOWDAY_H /* Real Time Clock Alarm day */

/* RTCCTL01 Control Bits */
#define RTCBCD              (0x8000)     /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD             (0x4000)     /* RTC Hold */
//#define RESERVED            (0x2000)     /* RESERVED */
#define RTCRDY              (0x1000)     /* RTC Ready */
//#define RESERVED            (0x0800)     /* RESERVED */
//#define RESERVED            (0x0400)     /* RESERVED */
#define RTCTEV1             (0x0200)     /* RTC Time Event 1 */
#define RTCTEV0             (0x0100)     /* RTC Time Event 0 */
#define RTCOFIE             (0x0080)     /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE            (0x0040)     /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE              (0x0020)     /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE            (0x0010)     /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG            (0x0008)     /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG           (0x0004)     /* RTC Time Event Interrupt Flag */
#define RTCAIFG             (0x0002)     /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG           (0x0001)     /* RTC Ready Interrupt Flag */

/* RTCCTL01 Control Bits */
//#define RESERVED            (0x2000)     /* RESERVED */
//#define RESERVED            (0x0800)     /* RESERVED */
//#define RESERVED            (0x0400)     /* RESERVED */
#define RTCOFIE_L           (0x0080)     /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE_L          (0x0040)     /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE_L            (0x0020)     /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE_L          (0x0010)     /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG_L          (0x0008)     /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG_L         (0x0004)     /* RTC Time Event Interrupt Flag */
#define RTCAIFG_L           (0x0002)     /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG_L         (0x0001)     /* RTC Ready Interrupt Flag */

/* RTCCTL01 Control Bits */
#define RTCBCD_H            (0x0080)     /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD_H           (0x0040)     /* RTC Hold */
//#define RESERVED            (0x2000)     /* RESERVED */
#define RTCRDY_H            (0x0010)     /* RTC Ready */
//#define RESERVED            (0x0800)     /* RESERVED */
//#define RESERVED            (0x0400)     /* RESERVED */
#define RTCTEV1_H           (0x0002)     /* RTC Time Event 1 */
#define RTCTEV0_H           (0x0001)     /* RTC Time Event 0 */

#define RTCTEV_0            (0x0000)     /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV_1            (0x0100)     /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV_2            (0x0200)     /* RTC Time Event: 2 (12:00 changed) */
#define RTCTEV_3            (0x0300)     /* RTC Time Event: 3 (00:00 changed) */
#define RTCTEV__MIN         (0x0000)     /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV__HOUR        (0x0100)     /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV__0000        (0x0200)     /* RTC Time Event: 2 (00:00 changed) */
#define RTCTEV__1200        (0x0300)     /* RTC Time Event: 3 (12:00 changed) */

/* RTCCTL23 Control Bits */
#define RTCCALF1            (0x0200)     /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0            (0x0100)     /* RTC Calibration Frequency Bit 0 */
#define RTCCALS             (0x0080)     /* RTC Calibration Sign */
//#define Reserved          (0x0040)
#define RTCCAL5             (0x0020)     /* RTC Calibration Bit 5 */
#define RTCCAL4             (0x0010)     /* RTC Calibration Bit 4 */
#define RTCCAL3             (0x0008)     /* RTC Calibration Bit 3 */
#define RTCCAL2             (0x0004)     /* RTC Calibration Bit 2 */
#define RTCCAL1             (0x0002)     /* RTC Calibration Bit 1 */
#define RTCCAL0             (0x0001)     /* RTC Calibration Bit 0 */

/* RTCCTL23 Control Bits */
#define RTCCALS_L           (0x0080)     /* RTC Calibration Sign */
//#define Reserved          (0x0040)
#define RTCCAL5_L           (0x0020)     /* RTC Calibration Bit 5 */
#define RTCCAL4_L           (0x0010)     /* RTC Calibration Bit 4 */
#define RTCCAL3_L           (0x0008)     /* RTC Calibration Bit 3 */
#define RTCCAL2_L           (0x0004)     /* RTC Calibration Bit 2 */
#define RTCCAL1_L           (0x0002)     /* RTC Calibration Bit 1 */
#define RTCCAL0_L           (0x0001)     /* RTC Calibration Bit 0 */

/* RTCCTL23 Control Bits */
#define RTCCALF1_H          (0x0002)     /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0_H          (0x0001)     /* RTC Calibration Frequency Bit 0 */
//#define Reserved          (0x0040)

#define RTCCALF_0           (0x0000)     /* RTC Calibration Frequency: No Output */
#define RTCCALF_1           (0x0100)     /* RTC Calibration Frequency: 512 Hz */
#define RTCCALF_2           (0x0200)     /* RTC Calibration Frequency: 256 Hz */
#define RTCCALF_3           (0x0300)     /* RTC Calibration Frequency: 1 Hz */


#define RTCAE               (0x80)     /* Real Time Clock Alarm enable */




/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT0IP2              (0x0010)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1              (0x0008)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0              (0x0004)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE             (0x0002)     /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG            (0x0001)     /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT0IP2_L            (0x0010)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1_L            (0x0008)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0_L            (0x0004)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE_L           (0x0002)     /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG_L          (0x0001)     /* RTC Prescale Timer 0 Interrupt Flag */


#define RT0IP_0             (0x0000)     /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP_1             (0x0004)     /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP_2             (0x0008)     /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP_3             (0x000C)     /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP_4             (0x0010)     /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP_5             (0x0014)     /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP_6             (0x0018)     /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP_7             (0x001C)     /* RTC Prescale Timer 0 Interrupt Interval /256 */

#define RT0IP__2            (0x0000)     /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP__4            (0x0004)     /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP__8            (0x0008)     /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP__16           (0x000C)     /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP__32           (0x0010)     /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP__64           (0x0014)     /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP__128          (0x0018)     /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP__256          (0x001C)     /* RTC Prescale Timer 0 Interrupt Interval /256 */


/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT1IP2              (0x0010)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1              (0x0008)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0              (0x0004)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE             (0x0002)     /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG            (0x0001)     /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080)
//#define Reserved          (0x0040)
//#define Reserved          (0x0020)
#define RT1IP2_L            (0x0010)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1_L            (0x0008)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0_L            (0x0004)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE_L           (0x0002)     /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG_L          (0x0001)     /* RTC Prescale Timer 1 Interrupt Flag */


#define RT1IP_0             (0x0000)     /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP_1             (0x0004)     /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP_2             (0x0008)     /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP_3             (0x000C)     /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP_4             (0x0010)     /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP_5             (0x0014)     /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP_6             (0x0018)     /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP_7             (0x001C)     /* RTC Prescale Timer 1 Interrupt Interval /256 */

#define RT1IP__2            (0x0000)     /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP__4            (0x0004)     /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP__8            (0x0008)     /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP__16           (0x000C)     /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP__32           (0x0010)     /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP__64           (0x0014)     /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP__128          (0x0018)     /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP__256          (0x001C)     /* RTC Prescale Timer 1 Interrupt Interval /256 */


/* RTC Definitions */
#define RTCIV_NONE           (0x0000)    /* No Interrupt pending */
#define RTCIV_RTCRDYIFG      (0x0002)    /* RTC ready: RTCRDYIFG */
#define RTCIV_RTCTEVIFG      (0x0004)    /* RTC interval timer: RTCTEVIFG */
#define RTCIV_RTCAIFG        (0x0006)    /* RTC user alarm: RTCAIFG */
#define RTCIV_RT0PSIFG       (0x0008)    /* RTC prescaler 0: RT0PSIFG */
#define RTCIV_RT1PSIFG       (0x000A)    /* RTC prescaler 1: RT1PSIFG */
#define RTCIV_RTCOFIFG       (0x000C)    /* RTC Oscillator fault */

/* Legacy Definitions */
#define RTC_NONE           (0x0000)      /* No Interrupt pending */
#define RTC_RTCRDYIFG      (0x0002)      /* RTC ready: RTCRDYIFG */
#define RTC_RTCTEVIFG      (0x0004)      /* RTC interval timer: RTCTEVIFG */
#define RTC_RTCAIFG        (0x0006)      /* RTC user alarm: RTCAIFG */
#define RTC_RT0PSIFG       (0x0008)      /* RTC prescaler 0: RT0PSIFG */
#define RTC_RT1PSIFG       (0x000A)      /* RTC prescaler 1: RT1PSIFG */
#define RTC_RTCOFIFG       (0x000C)      /* RTC Oscillator fault */

#endif
/************************************************************
* Real-Time Clock (RTC) Counter
************************************************************/
#ifdef  __MSP430_HAS_RTC__            /* Definition to show that Module is available */

#define OFS_RTCCTL            (0x0000)  /* RTC control Register */
#define OFS_RTCCTL_L           OFS_RTCCTL
#define OFS_RTCCTL_H           OFS_RTCCTL+1
#define OFS_RTCIV             (0x0004)  /* RTC interrupt vector */
#define OFS_RTCIV_L            OFS_RTCIV
#define OFS_RTCIV_H            OFS_RTCIV+1
#define OFS_RTCMOD            (0x0008)  /* RTC moduloRegister */
#define OFS_RTCMOD_L           OFS_RTCMOD
#define OFS_RTCMOD_H           OFS_RTCMOD+1
#define OFS_RTCCNT            (0x000C)  /* RTC counter Register */
#define OFS_RTCCNT_L           OFS_RTCCNT
#define OFS_RTCCNT_H           OFS_RTCCNT+1



/* RTC control Register */
#define RTCIF               (0x0001)  /* Low-Power-Counter Interrupt Flag */
#define RTCIE               (0x0002)  /* Low-Power-Counter Interrupt Enable */
#define RTCSR               (0x0040)  /* Low-Power-Counter Software Reset */
#define RTCPS0              (0x0100)  /* Low-Power-Counter Clock Pre-divider Select Bit: 0 */
#define RTCPS1              (0x0200)  /* Low-Power-Counter Clock Pre-divider Select Bit: 1 */
#define RTCPS2              (0x0400)  /* Low-Power-Counter Clock Pre-divider Select Bit: 2 */
#define RTCSS0              (0x1000)  /* Low-Power-Counter Clock Source Select Bit: 0 */
#define RTCSS1              (0x2000)  /* Low-Power-Counter Clock Source Select Bit: 1 */

/* RTC control Register */
#define RTCIF_L             (0x0001)  /* Low-Power-Counter Interrupt Flag */
#define RTCIE_L             (0x0002)  /* Low-Power-Counter Interrupt Enable */
#define RTCSR_L             (0x0040)  /* Low-Power-Counter Software Reset */

/* RTC control Register */
#define RTCPS0_H            (0x0001)  /* Low-Power-Counter Clock Pre-divider Select Bit: 0 */
#define RTCPS1_H            (0x0002)  /* Low-Power-Counter Clock Pre-divider Select Bit: 1 */
#define RTCPS2_H            (0x0004)  /* Low-Power-Counter Clock Pre-divider Select Bit: 2 */
#define RTCSS0_H            (0x0010)  /* Low-Power-Counter Clock Source Select Bit: 0 */
#define RTCSS1_H            (0x0020)  /* Low-Power-Counter Clock Source Select Bit: 1 */

#define RTCPS_0             (0x0000)  /* Low-Power-Counter Clock Pre-divider Select: 0 */
#define RTCPS_1             (0x0100)  /* Low-Power-Counter Clock Pre-divider Select: 1 */
#define RTCPS_2             (0x0200)  /* Low-Power-Counter Clock Pre-divider Select: 2 */
#define RTCPS_3             (0x0300)  /* Low-Power-Counter Clock Pre-divider Select: 3 */
#define RTCPS_4             (0x0400)  /* Low-Power-Counter Clock Pre-divider Select: 4 */
#define RTCPS_5             (0x0500)  /* Low-Power-Counter Clock Pre-divider Select: 5 */
#define RTCPS_6             (0x0600)  /* Low-Power-Counter Clock Pre-divider Select: 6 */
#define RTCPS_7             (0x0700)  /* Low-Power-Counter Clock Pre-divider Select: 7 */
#define RTCPS__1            (0x0000)  /* Low-Power-Counter Clock Pre-divider Select: /1 */
#define RTCPS__10           (0x0100)  /* Low-Power-Counter Clock Pre-divider Select: /10 */
#define RTCPS__100          (0x0200)  /* Low-Power-Counter Clock Pre-divider Select: /100 */
#define RTCPS__1000         (0x0300)  /* Low-Power-Counter Clock Pre-divider Select: /1000 */
#define RTCPS__16           (0x0400)  /* Low-Power-Counter Clock Pre-divider Select: /16 */
#define RTCPS__64           (0x0500)  /* Low-Power-Counter Clock Pre-divider Select: /64 */
#define RTCPS__256          (0x0600)  /* Low-Power-Counter Clock Pre-divider Select: /256 */
#define RTCPS__1024         (0x0700)  /* Low-Power-Counter Clock Pre-divider Select: /1024 */

#define RTCSS_0             (0x0000)  /* Low-Power-Counter Clock Source Select: 0 */
#define RTCSS_1             (0x1000)  /* Low-Power-Counter Clock Source Select: 1 */
#define RTCSS_2             (0x2000)  /* Low-Power-Counter Clock Source Select: 2 */
#define RTCSS_3             (0x3000)  /* Low-Power-Counter Clock Source Select: 3 */

#define RTCSS__DISABLED     (0x0000)  /* Low-Power-Counter Clock Source Select: DISABLED  */
#define RTCSS__SMCLK        (0x1000)  /* Low-Power-Counter Clock Source Select: SMCLK */
#define RTCSS__XT1CLK       (0x2000)  /* Low-Power-Counter Clock Source Select: XT1CLK */
#define RTCSS__VLOCLK       (0x3000)  /* Low-Power-Counter Clock Source Select: VLOCLK */


/* RTCIV Definitions */
#define RTCIV_NONE          (0x0000)  /* No Interrupt pending */
#define RTCIV_RTCIF         (0x0002)  /* RTC Overflow */

#endif
/************************************************************
* Smart Analog Combo 0 - Config 1 (SAC0)
************************************************************/
#ifdef  __MSP430_HAS_SAC0__           /* Definition to show that Module is available */

#define OFS_SAC0OA            (0x0000)  /* SAC0 OA Control Register */
#define OFS_SAC0OA_L           OFS_SAC0OA
#define OFS_SAC0OA_H           OFS_SAC0OA+1

/* SAC0OA Control Bits */
#define PSEL0               (0x0001)    /* SAC OA Positive input source selection Bit: 0 */
#define PSEL1               (0x0002)    /* SAC OA Positive input source selection Bit: 1 */
#define PMUXEN              (0x0008)    /* SAC Positive input MUX control */
#define NSEL0               (0x0010)    /* SAC OA Negative input source selection Bit: 0 */
#define NSEL1               (0x0020)    /* SAC OA Negative input source selection Bit: 1 */
#define NMUXEN              (0x0080)    /* SAC Negative input MUX control */
#define OAEN                (0x0100)    /* SAC OA Enable selection */
#define OAPM                (0x0200)    /* SAC OA power mode selection */
#define SACEN               (0x0400)    /* SAC Enable selection */

/* SAC0OA Control Bits */
#define PSEL0_L             (0x0001)    /* SAC OA Positive input source selection Bit: 0 */
#define PSEL1_L             (0x0002)    /* SAC OA Positive input source selection Bit: 1 */
#define PMUXEN_L            (0x0008)    /* SAC Positive input MUX control */
#define NSEL0_L             (0x0010)    /* SAC OA Negative input source selection Bit: 0 */
#define NSEL1_L             (0x0020)    /* SAC OA Negative input source selection Bit: 1 */
#define NMUXEN_L            (0x0080)    /* SAC Negative input MUX control */

/* SAC0OA Control Bits */
#define OAEN_H              (0x0001)    /* SAC OA Enable selection */
#define OAPM_H              (0x0002)    /* SAC OA power mode selection */
#define SACEN_H             (0x0004)    /* SAC Enable selection */

#define PSEL_0              (0x0000)    /* SAC OA Positive input source selection: 0 */
#define PSEL_1              (0x0001)    /* SAC OA Positive input source selection: 1 */
#define PSEL_2              (0x0002)    /* SAC OA Positive input source selection: 2 */
#define PSEL_3              (0x0003)    /* SAC OA Positive input source selection: 3 */

#define NSEL_0              (0x0000)    /* SAC OA Negative input source selection: 0 */
#define NSEL_1              (0x0010)    /* SAC OA Negative input source selection: 1 */
#define NSEL_2              (0x0020)    /* SAC OA Negative input source selection: 2 */
#define NSEL_3              (0x0030)    /* SAC OA Negative input source selection: 3 */

#endif
/************************************************************
* SFR - Special Function Register Module
************************************************************/
#ifdef  __MSP430_HAS_SFR__            /* Definition to show that Module is available */

#define OFS_SFRIE1            (0x0000)  /* Interrupt Enable 1 */
#define OFS_SFRIE1_L           OFS_SFRIE1
#define OFS_SFRIE1_H           OFS_SFRIE1+1

/* SFRIE1 Control Bits */
#define WDTIE               (0x0001)  /* WDT Interrupt Enable */
#define OFIE                (0x0002)  /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE               (0x0008)  /* Vacant Memory Interrupt Enable */
#define NMIIE               (0x0010)  /* NMI Interrupt Enable */
#define JMBINIE             (0x0040)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE            (0x0080)  /* JTAG Mail Box output Interrupt Enable */

#define WDTIE_L             (0x0001)  /* WDT Interrupt Enable */
#define OFIE_L              (0x0002)  /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE_L             (0x0008)  /* Vacant Memory Interrupt Enable */
#define NMIIE_L             (0x0010)  /* NMI Interrupt Enable */
#define JMBINIE_L           (0x0040)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE_L          (0x0080)  /* JTAG Mail Box output Interrupt Enable */


#define OFS_SFRIFG1           (0x0002)  /* Interrupt Flag 1 */
#define OFS_SFRIFG1_L          OFS_SFRIFG1
#define OFS_SFRIFG1_H          OFS_SFRIFG1+1
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


#define OFS_SFRRPCR           (0x0004)  /* RESET Pin Control Register */
#define OFS_SFRRPCR_L          OFS_SFRRPCR
#define OFS_SFRRPCR_H          OFS_SFRRPCR+1
/* SFRRPCR Control Bits */
#define SYSNMI              (0x0001)  /* NMI select */
#define SYSNMIIES           (0x0002)  /* NMI edge select */
#define SYSRSTUP            (0x0004)  /* RESET Pin pull down/up select */
#define SYSRSTRE            (0x0008)  /* RESET Pin Resistor enable */

#define SYSNMI_L            (0x0001)  /* NMI select */
#define SYSNMIIES_L         (0x0002)  /* NMI edge select */
#define SYSRSTUP_L          (0x0004)  /* RESET Pin pull down/up select */
#define SYSRSTRE_L          (0x0008)  /* RESET Pin Resistor enable */


#endif
/************************************************************
* SYS - System Module
************************************************************/
#ifdef  __MSP430_HAS_SYS__            /* Definition to show that Module is available */

#define OFS_SYSCTL            (0x0000)  /* System control */
#define OFS_SYSCTL_L           OFS_SYSCTL
#define OFS_SYSCTL_H           OFS_SYSCTL+1
#define OFS_SYSBSLC           (0x0002)  /* Boot strap configuration area */
#define OFS_SYSBSLC_L          OFS_SYSBSLC
#define OFS_SYSBSLC_H          OFS_SYSBSLC+1
#define OFS_SYSJMBC           (0x0006)  /* JTAG mailbox control */
#define OFS_SYSJMBC_L          OFS_SYSJMBC
#define OFS_SYSJMBC_H          OFS_SYSJMBC+1
#define OFS_SYSJMBI0          (0x0008)  /* JTAG mailbox input 0 */
#define OFS_SYSJMBI0_L         OFS_SYSJMBI0
#define OFS_SYSJMBI0_H         OFS_SYSJMBI0+1
#define OFS_SYSJMBI1          (0x000A)  /* JTAG mailbox input 1 */
#define OFS_SYSJMBI1_L         OFS_SYSJMBI1
#define OFS_SYSJMBI1_H         OFS_SYSJMBI1+1
#define OFS_SYSJMBO0          (0x000C)  /* JTAG mailbox output 0 */
#define OFS_SYSJMBO0_L         OFS_SYSJMBO0
#define OFS_SYSJMBO0_H         OFS_SYSJMBO0+1
#define OFS_SYSJMBO1          (0x000E)  /* JTAG mailbox output 1 */
#define OFS_SYSJMBO1_L         OFS_SYSJMBO1
#define OFS_SYSJMBO1_H         OFS_SYSJMBO1+1

#define OFS_SYSBERRIV         (0x0018)  /* Bus Error vector generator */
#define OFS_SYSBERRIV_L        OFS_SYSBERRIV
#define OFS_SYSBERRIV_H        OFS_SYSBERRIV+1
#define OFS_SYSUNIV           (0x001A)  /* User NMI vector generator */
#define OFS_SYSUNIV_L          OFS_SYSUNIV
#define OFS_SYSUNIV_H          OFS_SYSUNIV+1
#define OFS_SYSSNIV           (0x001C)  /* System NMI vector generator */
#define OFS_SYSSNIV_L          OFS_SYSSNIV
#define OFS_SYSSNIV_H          OFS_SYSSNIV+1
#define OFS_SYSRSTIV          (0x001E)  /* Reset vector generator */
#define OFS_SYSRSTIV_L         OFS_SYSRSTIV
#define OFS_SYSRSTIV_H         OFS_SYSRSTIV+1
#define OFS_SYSCFG0           (0x0020)  /* System Configuration 0 */
#define OFS_SYSCFG0_L          OFS_SYSCFG0
#define OFS_SYSCFG0_H          OFS_SYSCFG0+1
#define OFS_SYSCFG1           (0x0022)  /* System Configuration 1 */
#define OFS_SYSCFG1_L          OFS_SYSCFG1
#define OFS_SYSCFG1_H          OFS_SYSCFG1+1
#define OFS_SYSCFG2           (0x0024)  /* System Configuration 2 */
#define OFS_SYSCFG2_L          OFS_SYSCFG2
#define OFS_SYSCFG2_H          OFS_SYSCFG2+1

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


/* SYSCFG0 Control Bits */
#define PFWP                (0x0001)  /* Program FRAM Write Protection */
#define DFWP                (0x0002)  /* Data FRAM Write Protection */

/* SYSCFG0 Control Bits */
#define PFWP_L              (0x0001)  /* Program FRAM Write Protection */
#define DFWP_L              (0x0002)  /* Data FRAM Write Protection */


/* SYSCFG1 Control Bits */
#define IREN                (0x0001)  /* Infrared enable */
#define IRPSEL              (0x0002)  /* Infrared polarity select */
#define IRMSEL              (0x0004)  /* Infrared mode select */
#define IRDSSEL             (0x0008)  /* Infrared data source select */
#define IRDATA              (0x0010)  /* Infrared enable */

/* SYSCFG1 Control Bits */
#define IREN_L              (0x0001)  /* Infrared enable */
#define IRPSEL_L            (0x0002)  /* Infrared polarity select */
#define IRMSEL_L            (0x0004)  /* Infrared mode select */
#define IRDSSEL_L           (0x0008)  /* Infrared data source select */
#define IRDATA_L            (0x0010)  /* Infrared enable */


/* SYSCFG2 Control Bits */
#define ADCPCTL0            (0x0001)  /* ADC input A0 pin select */
#define ADCPCTL1            (0x0002)  /* ADC input A1 pin select */
#define ADCPCTL2            (0x0004)  /* ADC input A2 pin select */
#define ADCPCTL3            (0x0008)  /* ADC input A3 pin select */
#define ADCPCTL4            (0x0010)  /* ADC input A4 pin select */
#define ADCPCTL5            (0x0020)  /* ADC input A5 pin select */
#define ADCPCTL6            (0x0040)  /* ADC input A6 pin select */
#define ADCPCTL7            (0x0080)  /* ADC input A7 pin select */
#define ADCPCTL8            (0x0100)  /* ADC input A8 pin select */
#define ADCPCTL9            (0x0200)  /* ADC input A9 pin select */
#define RTCCLKSEL           (0x0400)  /* RTC clock selection */
#define USCIBRMAP           (0x0800)  /* eUSCIB Remapping source selection */
#define LCDPCTL             (0x1000)  /* LCD Power Pin */
#define TB0TRG              (0x4000)  /* TB0OUTH trigger source selection */
#define TB1TRG              (0x8000)  /* TB1OUTH trigger source selection */

/* SYSCFG2 Control Bits */
#define ADCPCTL0_L          (0x0001)  /* ADC input A0 pin select */
#define ADCPCTL1_L          (0x0002)  /* ADC input A1 pin select */
#define ADCPCTL2_L          (0x0004)  /* ADC input A2 pin select */
#define ADCPCTL3_L          (0x0008)  /* ADC input A3 pin select */
#define ADCPCTL4_L          (0x0010)  /* ADC input A4 pin select */
#define ADCPCTL5_L          (0x0020)  /* ADC input A5 pin select */
#define ADCPCTL6_L          (0x0040)  /* ADC input A6 pin select */
#define ADCPCTL7_L          (0x0080)  /* ADC input A7 pin select */

/* SYSCFG2 Control Bits */
#define ADCPCTL8_H          (0x0001)  /* ADC input A8 pin select */
#define ADCPCTL9_H          (0x0002)  /* ADC input A9 pin select */
#define RTCCLKSEL_H         (0x0004)  /* RTC clock selection */
#define USCIBRMAP_H         (0x0008)  /* eUSCIB Remapping source selection */
#define LCDPCTL_H           (0x0010)  /* LCD Power Pin */
#define TB0TRG_H            (0x0040)  /* TB0OUTH trigger source selection */
#define TB1TRG_H            (0x0080)  /* TB1OUTH trigger source selection */






#endif
/************************************************************
* Timerx_A7
************************************************************/
#ifdef  __MSP430_HAS_TxA7__            /* Definition to show that Module is available */

#define OFS_TAxCTL            (0x0000)  /* Timerx_A7 Control */
#define OFS_TAxCCTL0          (0x0002)  /* Timerx_A7 Capture/Compare Control 0 */
#define OFS_TAxCCTL1          (0x0004)  /* Timerx_A7 Capture/Compare Control 1 */
#define OFS_TAxCCTL2          (0x0006)  /* Timerx_A7 Capture/Compare Control 2 */
#define OFS_TAxCCTL3          (0x0008)  /* Timerx_A7 Capture/Compare Control 3 */
#define OFS_TAxCCTL4          (0x000A)  /* Timerx_A7 Capture/Compare Control 4 */
#define OFS_TAxCCTL5          (0x000C)  /* Timerx_A7 Capture/Compare Control 5 */
#define OFS_TAxCCTL6          (0x000E)  /* Timerx_A7 Capture/Compare Control 6 */
#define OFS_TAxR              (0x0010)  /* Timerx_A7 */
#define OFS_TAxCCR0           (0x0012)  /* Timerx_A7 Capture/Compare 0 */
#define OFS_TAxCCR1           (0x0014)  /* Timerx_A7 Capture/Compare 1 */
#define OFS_TAxCCR2           (0x0016)  /* Timerx_A7 Capture/Compare 2 */
#define OFS_TAxCCR3           (0x0018)  /* Timerx_A7 Capture/Compare 3 */
#define OFS_TAxCCR4           (0x001A)  /* Timerx_A7 Capture/Compare 4 */
#define OFS_TAxCCR5           (0x001C)  /* Timerx_A7 Capture/Compare 5 */
#define OFS_TAxCCR6           (0x001E)  /* Timerx_A7 Capture/Compare 6 */
#define OFS_TAxIV             (0x002E)  /* Timerx_A7 Interrupt Vector Word */
#define OFS_TAxEX0            (0x0020)  /* Timerx_A7 Expansion Register 0 */

/* Bits are already defined within the Timer0_Ax */

/* TAxIV Definitions */
#define TAxIV_NONE          (0x0000)    /* No Interrupt pending */
#define TAxIV_TACCR1        (0x0002)    /* TAxCCR1_CCIFG */
#define TAxIV_TACCR2        (0x0004)    /* TAxCCR2_CCIFG */
#define TAxIV_TACCR3        (0x0006)    /* TAxCCR3_CCIFG */
#define TAxIV_TACCR4        (0x0008)    /* TAxCCR4_CCIFG */
#define TAxIV_TACCR5        (0x000A)    /* TAxCCR5_CCIFG */
#define TAxIV_TACCR6        (0x000C)    /* TAxCCR6_CCIFG */
#define TAxIV_TAIFG         (0x000E)    /* TAxIFG */

/* Legacy Defines */
#define TAxIV_TAxCCR1      (0x0002)    /* TAxCCR1_CCIFG */
#define TAxIV_TAxCCR2      (0x0004)    /* TAxCCR2_CCIFG */
#define TAxIV_TAxCCR3      (0x0006)    /* TAxCCR3_CCIFG */
#define TAxIV_TAxCCR4      (0x0008)    /* TAxCCR4_CCIFG */
#define TAxIV_TAxCCR5      (0x000A)    /* TAxCCR5_CCIFG */
#define TAxIV_TAxCCR6      (0x000C)    /* TAxCCR6_CCIFG */
#define TAxIV_TAxIFG       (0x000E)    /* TAxIFG */

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

#endif
/************************************************************
* Timerx_B7
************************************************************/
#ifdef  __MSP430_HAS_TxB7__            /* Definition to show that Module is available */

#define OFS_TBxCTL            (0x0000)  /* Timerx_B7 Control */
#define OFS_TBxCCTL0          (0x0002)  /* Timerx_B7 Capture/Compare Control 0 */
#define OFS_TBxCCTL1          (0x0004)  /* Timerx_B7 Capture/Compare Control 1 */
#define OFS_TBxCCTL2          (0x0006)  /* Timerx_B7 Capture/Compare Control 2 */
#define OFS_TBxCCTL3          (0x0008)  /* Timerx_B7 Capture/Compare Control 3 */
#define OFS_TBxCCTL4          (0x000A)  /* Timerx_B7 Capture/Compare Control 4 */
#define OFS_TBxCCTL5          (0x000C)  /* Timerx_B7 Capture/Compare Control 5 */
#define OFS_TBxCCTL6          (0x000E)  /* Timerx_B7 Capture/Compare Control 6 */
#define OFS_TBxR              (0x0010)  /* Timerx_B7 */
#define OFS_TBxCCR0           (0x0012)  /* Timerx_B7 Capture/Compare 0 */
#define OFS_TBxCCR1           (0x0014)  /* Timerx_B7 Capture/Compare 1 */
#define OFS_TBxCCR2           (0x0016)  /* Timerx_B7 Capture/Compare 2 */
#define OFS_TBxCCR3           (0x0018)  /* Timerx_B7 Capture/Compare 3 */
#define OFS_TBxCCR4           (0x001A)  /* Timerx_B7 Capture/Compare 4 */
#define OFS_TBxCCR5           (0x001C)  /* Timerx_B7 Capture/Compare 5 */
#define OFS_TBxCCR6           (0x001E)  /* Timerx_B7 Capture/Compare 6 */
#define OFS_TBxIV             (0x002E)  /* Timerx_B7 Interrupt Vector Word */
#define OFS_TBxEX0            (0x0020)  /* Timerx_B7 Expansion Register 0 */

/* Bits are already defined within the Timer0_Ax */

/* TBxIV Definitions */
#define TBxIV_NONE           (0x0000)    /* No Interrupt pending */
#define TBxIV_TBCCR1         (0x0002)    /* TBxCCR1_CCIFG */
#define TBxIV_TBCCR2         (0x0004)    /* TBxCCR2_CCIFG */
#define TBxIV_TBCCR3         (0x0006)    /* TBxCCR3_CCIFG */
#define TBxIV_TBCCR4         (0x0008)    /* TBxCCR4_CCIFG */
#define TBxIV_TBCCR5         (0x000A)    /* TBxCCR5_CCIFG */
#define TBxIV_TBCCR6         (0x000C)    /* TBxCCR6_CCIFG */
#define TBxIV_TBIFG          (0x000E)    /* TBxIFG */

/* Legacy Defines */
#define TBxIV_TBxCCR1       (0x0002)    /* TBxCCR1_CCIFG */
#define TBxIV_TBxCCR2       (0x0004)    /* TBxCCR2_CCIFG */
#define TBxIV_TBxCCR3       (0x0006)    /* TBxCCR3_CCIFG */
#define TBxIV_TBxCCR4       (0x0008)    /* TBxCCR4_CCIFG */
#define TBxIV_TBxCCR5       (0x000A)    /* TBxCCR5_CCIFG */
#define TBxIV_TBxCCR6       (0x000C)    /* TBxCCR6_CCIFG */
#define TBxIV_TBxIFG        (0x000E)    /* TBxIFG */

/* TBxCTL Control Bits */
#define TBCLGRP1            (0x4000)    /* Timer_B7 Compare latch load group 1 */
#define TBCLGRP0            (0x2000)    /* Timer_B7 Compare latch load group 0 */
#define CNTL1               (0x1000)    /* Counter lenght 1 */
#define CNTL0               (0x0800)    /* Counter lenght 0 */
#define TBSSEL1             (0x0200)    /* Clock source 1 */
#define TBSSEL0             (0x0100)    /* Clock source 0 */
#define TBCLR               (0x0004)    /* Timer_B7 counter clear */
#define TBIE                (0x0002)    /* Timer_B7 interrupt enable */
#define TBIFG               (0x0001)    /* Timer_B7 interrupt flag */

#define SHR1                (0x4000)    /* Timer_B7 Compare latch load group 1 */
#define SHR0                (0x2000)    /* Timer_B7 Compare latch load group 0 */

#define TBSSEL_0            (0x0000)  /* Clock Source: TBCLK */
#define TBSSEL_1            (0x0100)  /* Clock Source: ACLK  */
#define TBSSEL_2            (0x0200)  /* Clock Source: SMCLK */
#define TBSSEL_3            (0x0300)  /* Clock Source: INCLK */
#define CNTL_0              (0x0000)  /* Counter lenght: 16 bit */
#define CNTL_1              (0x0800)  /* Counter lenght: 12 bit */
#define CNTL_2              (0x1000)  /* Counter lenght: 10 bit */
#define CNTL_3              (0x1800)  /* Counter lenght:  8 bit */
#define SHR_0               (0x0000)  /* Timer_B7 Group: 0 - individually */
#define SHR_1               (0x2000)  /* Timer_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2               (0x4000)  /* Timer_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3               (0x6000)  /* Timer_B7 Group: 3 - 1 group (all) */
#define TBCLGRP_0           (0x0000)  /* Timer_B7 Group: 0 - individually */
#define TBCLGRP_1           (0x2000)  /* Timer_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TBCLGRP_2           (0x4000)  /* Timer_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define TBCLGRP_3           (0x6000)  /* Timer_B7 Group: 3 - 1 group (all) */
#define TBSSEL__TBCLK       (0x0000) /* Timer0_B7 clock source select: 0 - TBCLK */
#define TBSSEL__TACLK       (0x0000) /* Timer0_B7 clock source select: 0 - TBCLK (legacy) */
#define TBSSEL__ACLK        (0x0100)  /* Timer_B7 clock source select: 1 - ACLK  */
#define TBSSEL__SMCLK       (0x0200)  /* Timer_B7 clock source select: 2 - SMCLK */
#define TBSSEL__INCLK       (0x0300)  /* Timer_B7 clock source select: 3 - INCLK */
#define CNTL__16            (0x0000)  /* Counter lenght: 16 bit */
#define CNTL__12            (0x0800)  /* Counter lenght: 12 bit */
#define CNTL__10            (0x1000)  /* Counter lenght: 10 bit */
#define CNTL__8             (0x1800)  /* Counter lenght:  8 bit */

/* Additional Timer B Control Register bits are defined in Timer A */
/* TBxCCTLx Control Bits */
#define CLLD1               (0x0400)  /* Compare latch load source 1 */
#define CLLD0               (0x0200)  /* Compare latch load source 0 */

#define SLSHR1              (0x0400)  /* Compare latch load source 1 */
#define SLSHR0              (0x0200)  /* Compare latch load source 0 */

#define SLSHR_0             (0x0000) /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1             (0x0200) /* Compare latch load sourec : 1 - TBR counts to 0 */
#define SLSHR_2             (0x0400) /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3             (0x0600) /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

#define CLLD_0              (0x0000) /* Compare latch load sourec : 0 - immediate */
#define CLLD_1              (0x0200) /* Compare latch load sourec : 1 - TBR counts to 0 */
#define CLLD_2              (0x0400) /* Compare latch load sourec : 2 - up/down */
#define CLLD_3              (0x0600) /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

/* TBxEX0 Control Bits */
#define TBIDEX0             (0x0001)   /* Timer_B7 Input divider expansion Bit: 0 */
#define TBIDEX1             (0x0002)   /* Timer_B7 Input divider expansion Bit: 1 */
#define TBIDEX2             (0x0004)   /* Timer_B7 Input divider expansion Bit: 2 */

#define TBIDEX_0            (0x0000) /* Timer_B7 Input divider expansion : /1 */
#define TBIDEX_1            (0x0001) /* Timer_B7 Input divider expansion : /2 */
#define TBIDEX_2            (0x0002) /* Timer_B7 Input divider expansion : /3 */
#define TBIDEX_3            (0x0003) /* Timer_B7 Input divider expansion : /4 */
#define TBIDEX_4            (0x0004) /* Timer_B7 Input divider expansion : /5 */
#define TBIDEX_5            (0x0005) /* Timer_B7 Input divider expansion : /6 */
#define TBIDEX_6            (0x0006) /* Timer_B7 Input divider expansion : /7 */
#define TBIDEX_7            (0x0007) /* Timer_B7 Input divider expansion : /8 */
#define TBIDEX__1           (0x0000) /* Timer_B7 Input divider expansion : /1 */
#define TBIDEX__2           (0x0001) /* Timer_B7 Input divider expansion : /2 */
#define TBIDEX__3           (0x0002) /* Timer_B7 Input divider expansion : /3 */
#define TBIDEX__4           (0x0003) /* Timer_B7 Input divider expansion : /4 */
#define TBIDEX__5           (0x0004) /* Timer_B7 Input divider expansion : /5 */
#define TBIDEX__6           (0x0005) /* Timer_B7 Input divider expansion : /6 */
#define TBIDEX__7           (0x0006) /* Timer_B7 Input divider expansion : /7 */
#define TBIDEX__8           (0x0007) /* Timer_B7 Input divider expansion : /8 */


#define ID1                    (0x0080)       /* Timer B clock input divider 1 */
#define ID0                    (0x0040)       /* Timer B clock input divider 0 */
#define MC1                    (0x0020)       /* Timer B mode control 1 */
#define MC0                    (0x0010)       /* Timer B mode control 0 */
#define MC__STOP               (0x0000)      /* Timer B mode control: 0 - Stop */
#define MC__UP                 (0x0010)      /* Timer B mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS         (0x0020)      /* Timer B mode control: 2 - Continuous up */
#define MC__CONTINOUS          (0x0020)      /* Legacy define */
#define MC__UPDOWN             (0x0030)      /* Timer B mode control: 3 - Up/Down */
#define CM1                    (0x8000)       /* Capture mode 1 */
#define CM0                    (0x4000)       /* Capture mode 0 */
#define MC_0                   (0x0000)      /* Timer B mode control: 0 - Stop */
#define MC_1                   (0x0010)      /* Timer B mode control: 1 - Up to CCR0 */
#define MC_2                   (0x0020)      /* Timer B mode control: 2 - Continuous up */
#define MC_3                   (0x0030)      /* Timer B mode control: 3 - Up/Down */
#define CAP                    (0x0100)       /* Capture mode: 1 /Compare mode : 0 */
#define CCIE                   (0x0010)       /* Capture/compare interrupt enable */
#define CCIFG                  (0x0001)       /* Capture/compare interrupt flag */
#define CCIS_0                 (0x0000)
#define CCIS_1                 (0x1000)
#define CCIS_2                 (0x2000)
#define CCIS_3                 (0x3000)
#define CM_0                   (0x0000)    /* Capture mode: 0 - disabled */
#define CM_1                   (0x4000)    /* Capture mode: 1 - pos. edge */
#define CM_2                   (0x8000)    /* Capture mode: 1 - neg. edge */
#define CM_3                   (0xC000)    /* Capture mode: 1 - both edges */
#define OUT                    (0x0004)       /* PWM Output signal if output mode 0 */
#define OUTMOD_0               (0x0000)      /* PWM output mode: 0 - output only */
#define OUTMOD_1               (0x0020)      /* PWM output mode: 1 - set */
#define OUTMOD_2               (0x0040)      /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3               (0x0060)      /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4               (0x0080)      /* PWM output mode: 4 - toggle */
#define OUTMOD_5               (0x00A0)      /* PWM output mode: 5 - Reset */
#define OUTMOD_6               (0x00C0)      /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7               (0x00E0)      /* PWM output mode: 7 - PWM reset/set */
#define SCCI                   (0x0400)       /* Latched capture signal (read) */
#define SCS                    (0x0800)       /* Capture sychronize */
#define CCI                    (0x0008)       /* Capture input signal (read) */
#define ID__1                  (0x0000)      /* Timer B input divider: 0 - /1 */
#define ID__2                  (0x0040)      /* Timer B input divider: 1 - /2 */
#define ID__4                  (0x0080)      /* Timer B input divider: 2 - /4 */
#define ID__8                  (0x00C0)      /* Timer B input divider: 3 - /8 */
#define ID_0                   (0x0000)      /* Timer B input divider: 0 - /1 */
#define ID_1                   (0x0040)      /* Timer B input divider: 1 - /2 */
#define ID_2                   (0x0080)      /* Timer B input divider: 2 - /4 */
#define ID_3                   (0x00C0)      /* Timer B input divider: 3 - /8 */

#endif
/************************************************************
* Transimpedance amplifier (TRI)
************************************************************/
#ifdef  __MSP430_HAS_TRI__            /* Definition to show that Module is available */

#define OFS_TRICTL            (0x0000)  /* TRI Control register 0 */
#define OFS_TRICTL_L           OFS_TRICTL
#define OFS_TRICTL_H           OFS_TRICTL+1

/* TRICTL Control Bits */
#define TRIEN               (0x0001)    /* TRI enable */
#define TRIPM               (0x0002)    /* TRI power mode select */
#define TRIOUTSEL           (0x0008)    /* TRI output to package terminal selection */
#define TRIPSEL0            (0x0040)    /* TRI positive input select Bit: 0 */
#define TRIPSEL1            (0x0080)    /* TRI positive input select Bit: 1 */

/* TRICTL Control Bits */
#define TRIEN_L             (0x0001)    /* TRI enable */
#define TRIPM_L             (0x0002)    /* TRI power mode select */
#define TRIOUTSEL_L         (0x0008)    /* TRI output to package terminal selection */
#define TRIPSEL0_L          (0x0040)    /* TRI positive input select Bit: 0 */
#define TRIPSEL1_L          (0x0080)    /* TRI positive input select Bit: 1 */


#define TRIPSEL_0           (0x0000)    /* TRI positive input select: 0 */
#define TRIPSEL_1           (0x0040)    /* TRI positive input select: 1 */
#define TRIPSEL_2           (0x0080)    /* TRI positive input select: 2 */
#define TRIPSEL_3           (0x00C0)    /* TRI positive input select: 3 */

#endif
/************************************************************
* USCI Ax
************************************************************/
#ifdef  __MSP430_HAS_EUSCI_Ax__      /* Definition to show that Module is available */

#define OFS_UCAxCTLW0         (0x0000)  /* USCI Ax Control Word Register 0 */
#define OFS_UCAxCTLW0_L        OFS_UCAxCTLW0
#define OFS_UCAxCTLW0_H        OFS_UCAxCTLW0+1
#define OFS_UCAxCTL0          (0x0001)
#define OFS_UCAxCTL1          (0x0000)
#define UCAxCTL1            UCAxCTLW0_L  /* USCI Ax Control Register 1 */
#define UCAxCTL0            UCAxCTLW0_H  /* USCI Ax Control Register 0 */
#define OFS_UCAxCTLW1         (0x0002)  /* USCI Ax Control Word Register 1 */
#define OFS_UCAxCTLW1_L        OFS_UCAxCTLW1
#define OFS_UCAxCTLW1_H        OFS_UCAxCTLW1+1
#define OFS_UCAxBRW           (0x0006)  /* USCI Ax Baud Word Rate 0 */
#define OFS_UCAxBRW_L          OFS_UCAxBRW
#define OFS_UCAxBRW_H          OFS_UCAxBRW+1
#define OFS_UCAxBR0           (0x0006)
#define OFS_UCAxBR1           (0x0007)
#define UCAxBR0             UCAxBRW_L /* USCI Ax Baud Rate 0 */
#define UCAxBR1             UCAxBRW_H /* USCI Ax Baud Rate 1 */
#define OFS_UCAxMCTLW         (0x0008)  /* USCI Ax Modulation Control */
#define OFS_UCAxMCTLW_L        OFS_UCAxMCTLW
#define OFS_UCAxMCTLW_H        OFS_UCAxMCTLW+1
#define OFS_UCAxSTATW         (0x000A)  /* USCI Ax Status Register */
#define OFS_UCAxRXBUF         (0x000C)  /* USCI Ax Receive Buffer */
#define OFS_UCAxRXBUF_L        OFS_UCAxRXBUF
#define OFS_UCAxRXBUF_H        OFS_UCAxRXBUF+1
#define OFS_UCAxTXBUF         (0x000E)  /* USCI Ax Transmit Buffer */
#define OFS_UCAxTXBUF_L        OFS_UCAxTXBUF
#define OFS_UCAxTXBUF_H        OFS_UCAxTXBUF+1
#define OFS_UCAxABCTL         (0x0010)  /* USCI Ax LIN Control */
#define OFS_UCAxIRCTL         (0x0012)  /* USCI Ax IrDA Transmit Control */
#define OFS_UCAxIRCTL_L        OFS_UCAxIRCTL
#define OFS_UCAxIRCTL_H        OFS_UCAxIRCTL+1
#define OFS_UCAxIRTCTL        (0x0012)
#define OFS_UCAxIRRCTL        (0x0013)
#define UCAxIRTCTL          UCAxIRCTL_L  /* USCI Ax IrDA Transmit Control */
#define UCAxIRRCTL          UCAxIRCTL_H  /* USCI Ax IrDA Receive Control */
#define OFS_UCAxIE            (0x001A)  /* USCI Ax Interrupt Enable Register */
#define OFS_UCAxIE_L           OFS_UCAxIE
#define OFS_UCAxIE_H           OFS_UCAxIE+1
#define OFS_UCAxIFG           (0x001C)  /* USCI Ax Interrupt Flags Register */
#define OFS_UCAxIFG_L          OFS_UCAxIFG
#define OFS_UCAxIFG_H          OFS_UCAxIFG+1
#define OFS_UCAxIE__UART      (0x001A)
#define OFS_UCAxIE__UART_L     OFS_UCAxIE__UART
#define OFS_UCAxIE__UART_H     OFS_UCAxIE__UART+1
#define OFS_UCAxIFG__UART     (0x001C)
#define OFS_UCAxIFG__UART_L    OFS_UCAxIFG__UART
#define OFS_UCAxIFG__UART_H    OFS_UCAxIFG__UART+1
#define OFS_UCAxIV            (0x001E)  /* USCI Ax Interrupt Vector Register */

#define OFS_UCAxCTLW0__SPI    (0x0000)
#define OFS_UCAxCTLW0__SPI_L   OFS_UCAxCTLW0__SPI
#define OFS_UCAxCTLW0__SPI_H   OFS_UCAxCTLW0__SPI+1
#define OFS_UCAxCTL0__SPI     (0x0001)
#define OFS_UCAxCTL1__SPI     (0x0000)
#define OFS_UCAxBRW__SPI      (0x0006)
#define OFS_UCAxBRW__SPI_L     OFS_UCAxBRW__SPI
#define OFS_UCAxBRW__SPI_H     OFS_UCAxBRW__SPI+1
#define OFS_UCAxBR0__SPI      (0x0006)
#define OFS_UCAxBR1__SPI      (0x0007)
#define OFS_UCAxSTATW__SPI    (0x000A)
#define OFS_UCAxRXBUF__SPI    (0x000C)
#define OFS_UCAxRXBUF__SPI_L   OFS_UCAxRXBUF__SPI
#define OFS_UCAxRXBUF__SPI_H   OFS_UCAxRXBUF__SPI+1
#define OFS_UCAxTXBUF__SPI    (0x000E)
#define OFS_UCAxTXBUF__SPI_L   OFS_UCAxTXBUF__SPI
#define OFS_UCAxTXBUF__SPI_H   OFS_UCAxTXBUF__SPI+1
#define OFS_UCAxIE__SPI       (0x001A)
#define OFS_UCAxIFG__SPI      (0x001C)
#define OFS_UCAxIV__SPI       (0x001E)

#endif
/************************************************************
* USCI Bx
************************************************************/
#ifdef  __MSP430_HAS_EUSCI_Bx__       /* Definition to show that Module is available */

#define OFS_UCBxCTLW0__SPI    (0x0000)
#define OFS_UCBxCTLW0__SPI_L   OFS_UCBxCTLW0__SPI
#define OFS_UCBxCTLW0__SPI_H   OFS_UCBxCTLW0__SPI+1
#define OFS_UCBxCTL0__SPI     (0x0001)
#define OFS_UCBxCTL1__SPI     (0x0000)
#define OFS_UCBxBRW__SPI      (0x0006)
#define OFS_UCBxBRW__SPI_L     OFS_UCBxBRW__SPI
#define OFS_UCBxBRW__SPI_H     OFS_UCBxBRW__SPI+1
#define OFS_UCBxBR0__SPI      (0x0006)
#define OFS_UCBxBR1__SPI      (0x0007)
#define OFS_UCBxSTATW__SPI    (0x0008)
#define OFS_UCBxSTATW__SPI_L   OFS_UCBxSTATW__SPI
#define OFS_UCBxSTATW__SPI_H   OFS_UCBxSTATW__SPI+1
#define OFS_UCBxRXBUF__SPI    (0x000C)
#define OFS_UCBxRXBUF__SPI_L   OFS_UCBxRXBUF__SPI
#define OFS_UCBxRXBUF__SPI_H   OFS_UCBxRXBUF__SPI+1
#define OFS_UCBxTXBUF__SPI    (0x000E)
#define OFS_UCBxTXBUF__SPI_L   OFS_UCBxTXBUF__SPI
#define OFS_UCBxTXBUF__SPI_H   OFS_UCBxTXBUF__SPI+1
#define OFS_UCBxIE__SPI       (0x002A)
#define OFS_UCBxIE__SPI_L      OFS_UCBxIE__SPI
#define OFS_UCBxIE__SPI_H      OFS_UCBxIE__SPI+1
#define OFS_UCBxIFG__SPI      (0x002C)
#define OFS_UCBxIFG__SPI_L     OFS_UCBxIFG__SPI
#define OFS_UCBxIFG__SPI_H     OFS_UCBxIFG__SPI+1
#define OFS_UCBxIV__SPI       (0x002E)

#define OFS_UCBxCTLW0         (0x0000)  /* USCI Bx Control Word Register 0 */
#define OFS_UCBxCTLW0_L        OFS_UCBxCTLW0
#define OFS_UCBxCTLW0_H        OFS_UCBxCTLW0+1
#define OFS_UCBxCTL0          (0x0001)
#define OFS_UCBxCTL1          (0x0000)
#define UCBxCTL1            UCBxCTLW0_L  /* USCI Bx Control Register 1 */
#define UCBxCTL0            UCBxCTLW0_H  /* USCI Bx Control Register 0 */
#define OFS_UCBxCTLW1         (0x0002)  /* USCI Bx Control Word Register 1 */
#define OFS_UCBxCTLW1_L        OFS_UCBxCTLW1
#define OFS_UCBxCTLW1_H        OFS_UCBxCTLW1+1
#define OFS_UCBxBRW           (0x0006)  /* USCI Bx Baud Word Rate 0 */
#define OFS_UCBxBRW_L          OFS_UCBxBRW
#define OFS_UCBxBRW_H          OFS_UCBxBRW+1
#define OFS_UCBxBR0           (0x0006)
#define OFS_UCBxBR1           (0x0007)
#define UCBxBR0             UCBxBRW_L /* USCI Bx Baud Rate 0 */
#define UCBxBR1             UCBxBRW_H /* USCI Bx Baud Rate 1 */
#define OFS_UCBxSTATW         (0x0008)  /* USCI Bx Status Word Register */
#define OFS_UCBxSTATW_L        OFS_UCBxSTATW
#define OFS_UCBxSTATW_H        OFS_UCBxSTATW+1
#define OFS_UCBxSTATW__I2C    (0x0008)
#define OFS_UCBxSTAT__I2C     (0x0008)
#define OFS_UCBxBCNT__I2C     (0x0009)
#define UCBxSTAT            UCBxSTATW_L /* USCI Bx Status Register */
#define UCBxBCNT            UCBxSTATW_H /* USCI Bx Byte Counter Register */
#define OFS_UCBxTBCNT         (0x000A)  /* USCI Bx Byte Counter Threshold Register */
#define OFS_UCBxTBCNT_L        OFS_UCBxTBCNT
#define OFS_UCBxTBCNT_H        OFS_UCBxTBCNT+1
#define OFS_UCBxRXBUF         (0x000C)  /* USCI Bx Receive Buffer */
#define OFS_UCBxRXBUF_L        OFS_UCBxRXBUF
#define OFS_UCBxRXBUF_H        OFS_UCBxRXBUF+1
#define OFS_UCBxTXBUF         (0x000E)  /* USCI Bx Transmit Buffer */
#define OFS_UCBxTXBUF_L        OFS_UCBxTXBUF
#define OFS_UCBxTXBUF_H        OFS_UCBxTXBUF+1
#define OFS_UCBxI2COA0        (0x0014)  /* USCI Bx I2C Own Address 0 */
#define OFS_UCBxI2COA0_L       OFS_UCBxI2COA0
#define OFS_UCBxI2COA0_H       OFS_UCBxI2COA0+1
#define OFS_UCBxI2COA1        (0x0016)  /* USCI Bx I2C Own Address 1 */
#define OFS_UCBxI2COA1_L       OFS_UCBxI2COA1
#define OFS_UCBxI2COA1_H       OFS_UCBxI2COA1+1
#define OFS_UCBxI2COA2        (0x0018)  /* USCI Bx I2C Own Address 2 */
#define OFS_UCBxI2COA2_L       OFS_UCBxI2COA2
#define OFS_UCBxI2COA2_H       OFS_UCBxI2COA2+1
#define OFS_UCBxI2COA3        (0x001A)  /* USCI Bx I2C Own Address 3 */
#define OFS_UCBxI2COA3_L       OFS_UCBxI2COA3
#define OFS_UCBxI2COA3_H       OFS_UCBxI2COA3+1
#define OFS_UCBxADDRX         (0x001C)  /* USCI Bx Received Address Register */
#define OFS_UCBxADDRX_L        OFS_UCBxADDRX
#define OFS_UCBxADDRX_H        OFS_UCBxADDRX+1
#define OFS_UCBxADDMASK       (0x001E)  /* USCI Bx Address Mask Register */
#define OFS_UCBxADDMASK_L      OFS_UCBxADDMASK
#define OFS_UCBxADDMASK_H      OFS_UCBxADDMASK+1
#define OFS_UCBxI2CSA         (0x0020)  /* USCI Bx I2C Slave Address */
#define OFS_UCBxI2CSA_L        OFS_UCBxI2CSA
#define OFS_UCBxI2CSA_H        OFS_UCBxI2CSA+1
#define OFS_UCBxIE            (0x002A)  /* USCI Bx Interrupt Enable Register */
#define OFS_UCBxIE_L           OFS_UCBxIE
#define OFS_UCBxIE_H           OFS_UCBxIE+1
#define OFS_UCBxIFG           (0x002C)  /* USCI Bx Interrupt Flags Register */
#define OFS_UCBxIFG_L          OFS_UCBxIFG
#define OFS_UCBxIFG_H          OFS_UCBxIFG+1
#define OFS_UCBxIE__I2C       (0x002A)
#define OFS_UCBxIE__I2C_L      OFS_UCBxIE__I2C
#define OFS_UCBxIE__I2C_H      OFS_UCBxIE__I2C+1
#define OFS_UCBxIFG__I2C      (0x002C)
#define OFS_UCBxIFG__I2C_L     OFS_UCBxIFG__I2C
#define OFS_UCBxIFG__I2C_H     OFS_UCBxIFG__I2C+1
#define OFS_UCBxIV            (0x002E)  /* USCI Bx Interrupt Vector Register */

#endif
#if (defined(__MSP430_HAS_EUSCI_Ax__) || defined(__MSP430_HAS_EUSCI_Bx__))

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
#define UCSSEL__MODCLK      (0x0040)    /* USCI 0 Clock Source: MODCLK */
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

#endif
/************************************************************
* WATCHDOG TIMER A
************************************************************/
#ifdef  __MSP430_HAS_WDT_A__          /* Definition to show that Module is available */

#define OFS_WDTCTL            (0x000C)  /* Watchdog Timer Control */
#define OFS_WDTCTL_L           OFS_WDTCTL
#define OFS_WDTCTL_H           OFS_WDTCTL+1
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

#endif

/************************************************************
* TLV Descriptors
************************************************************/
#define __MSP430_HAS_TLV__              /* Definition to show that Module is available */
#define TLV_BASE __MSP430_BASEADDRESS_TLV__

#define TLV_CRC_LENGTH        (0x1A01)    /* CRC length of the TLV structure */
#define TLV_CRC_VALUE         (0x1A02)    /* CRC value of the TLV structure */
#define TLV_START             (0x1A08)    /* Start Address of the TLV structure */
#define TLV_END               (0x1AFF)    /* End Address of the TLV structure */
#define TLV_CRC_START         (0x1A04)    /* Start Address of the CRC protected structure */

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



/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __msp430FR2XX_FR4XXGENERIC */


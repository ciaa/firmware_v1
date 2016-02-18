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
* MSP430FR4133 devices.
*
* Texas Instruments, Version 1.1
*
* Rev. 1.0, Setup
* Rev. 1.1, replaced UCSSEL__ACLK with UCSSEL__MODCLK
*
********************************************************************/

#ifndef __MSP430FR4133
#define __MSP430FR4133

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
#define __MSP430FR2XX_4XX_FAMILY__

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/


/************************************************************
* ADC
************************************************************/
#define __MSP430_HAS_ADC__             /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_ADC__ 0x0700
#define ADC_BASE __MSP430_BASEADDRESS_ADC__
#define __MSP430_HAS_ADC_CHANNELS_10__

#define ADCCTL0_              0x0700    /* ADC Control 0 */
sfrb(ADCCTL0_L , ADCCTL0_);
sfrb(ADCCTL0_H , ADCCTL0_+1);
sfrw(ADCCTL0, ADCCTL0_);
#define ADCCTL1_              0x0702    /* ADC Control 1 */
sfrb(ADCCTL1_L , ADCCTL1_);
sfrb(ADCCTL1_H , ADCCTL1_+1);
sfrw(ADCCTL1, ADCCTL1_);
#define ADCCTL2_              0x0704    /* ADC Control 2 */
sfrb(ADCCTL2_L , ADCCTL2_);
sfrb(ADCCTL2_H , ADCCTL2_+1);
sfrw(ADCCTL2, ADCCTL2_);
#define ADCLO_                0x0706    /* ADC Window Comparator High Threshold */
sfrb(ADCLO_L , ADCLO_);
sfrb(ADCLO_H , ADCLO_+1);
sfrw(ADCLO, ADCLO_);
#define ADCHI_                0x0708    /* ADC Window Comparator High Threshold */
sfrb(ADCHI_L , ADCHI_);
sfrb(ADCHI_H , ADCHI_+1);
sfrw(ADCHI, ADCHI_);
#define ADCMCTL0_             0x070A    /* ADC Memory Control 0 */
sfrb(ADCMCTL0_L , ADCMCTL0_);
sfrb(ADCMCTL0_H , ADCMCTL0_+1);
sfrw(ADCMCTL0, ADCMCTL0_);
#define ADCMEM0_              0x0712    /* ADC Conversion Memory 0 */
sfrb(ADCMEM0_L , ADCMEM0_);
sfrb(ADCMEM0_H , ADCMEM0_+1);
sfrw(ADCMEM0, ADCMEM0_);
#define ADCIE_                0x071A    /* ADC Interrupt Enable */
sfrb(ADCIE_L , ADCIE_);
sfrb(ADCIE_H , ADCIE_+1);
sfrw(ADCIE, ADCIE_);
#define ADCIFG_               0x071C    /* ADC Interrupt Flag */
sfrb(ADCIFG_L , ADCIFG_);
sfrb(ADCIFG_H , ADCIFG_+1);
sfrw(ADCIFG, ADCIFG_);
#define ADCIV_                0x071E    /* ADC Interrupt Vector Word */
sfrb(ADCIV_L , ADCIV_);
sfrb(ADCIV_H , ADCIV_+1);
sfrw(ADCIV, ADCIV_);

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

/*************************************************************
* Backup RAM Module
*************************************************************/
#define __MSP430_HAS_BACKUP_RAM__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_BACKUP_RAM__ 0x0660
#define BAK_RAM_BASE __MSP430_BASEADDRESS_BACKUP_RAM__

#define BAKMEM0_              0x0660    /* Battery Backup Memory 0 */
sfrb(BAKMEM0_L , BAKMEM0_);
sfrb(BAKMEM0_H , BAKMEM0_+1);
sfrw(BAKMEM0, BAKMEM0_);
#define BAKMEM1_              0x0662    /* Battery Backup Memory 1 */
sfrb(BAKMEM1_L , BAKMEM1_);
sfrb(BAKMEM1_H , BAKMEM1_+1);
sfrw(BAKMEM1, BAKMEM1_);
#define BAKMEM2_              0x0664    /* Battery Backup Memory 2 */
sfrb(BAKMEM2_L , BAKMEM2_);
sfrb(BAKMEM2_H , BAKMEM2_+1);
sfrw(BAKMEM2, BAKMEM2_);
#define BAKMEM3_              0x0666    /* Battery Backup Memory 3 */
sfrb(BAKMEM3_L , BAKMEM3_);
sfrb(BAKMEM3_H , BAKMEM3_+1);
sfrw(BAKMEM3, BAKMEM3_);
#define BAKMEM4_              0x0668    /* Battery Backup Memory 4 */
sfrb(BAKMEM4_L , BAKMEM4_);
sfrb(BAKMEM4_H , BAKMEM4_+1);
sfrw(BAKMEM4, BAKMEM4_);
#define BAKMEM5_              0x066A    /* Battery Backup Memory 5 */
sfrb(BAKMEM5_L , BAKMEM5_);
sfrb(BAKMEM5_H , BAKMEM5_+1);
sfrw(BAKMEM5, BAKMEM5_);
#define BAKMEM6_              0x066C    /* Battery Backup Memory 6 */
sfrb(BAKMEM6_L , BAKMEM6_);
sfrb(BAKMEM6_H , BAKMEM6_+1);
sfrw(BAKMEM6, BAKMEM6_);
#define BAKMEM7_              0x066E    /* Battery Backup Memory 7 */
sfrb(BAKMEM7_L , BAKMEM7_);
sfrb(BAKMEM7_H , BAKMEM7_+1);
sfrw(BAKMEM7, BAKMEM7_);
#define BAKMEM8_              0x0670    /* Battery Backup Memory 8 */
sfrb(BAKMEM8_L , BAKMEM8_);
sfrb(BAKMEM8_H , BAKMEM8_+1);
sfrw(BAKMEM8, BAKMEM8_);
#define BAKMEM9_              0x0672    /* Battery Backup Memory 9 */
sfrb(BAKMEM9_L , BAKMEM9_);
sfrb(BAKMEM9_H , BAKMEM9_+1);
sfrw(BAKMEM9, BAKMEM9_);
#define BAKMEM10_             0x0674    /* Battery Backup Memory 10 */
sfrb(BAKMEM10_L , BAKMEM10_);
sfrb(BAKMEM10_H , BAKMEM10_+1);
sfrw(BAKMEM10, BAKMEM10_);
#define BAKMEM11_             0x0676    /* Battery Backup Memory 11 */
sfrb(BAKMEM11_L , BAKMEM11_);
sfrb(BAKMEM11_H , BAKMEM11_+1);
sfrw(BAKMEM11, BAKMEM11_);
#define BAKMEM12_             0x0678    /* Battery Backup Memory 12 */
sfrb(BAKMEM12_L , BAKMEM12_);
sfrb(BAKMEM12_H , BAKMEM12_+1);
sfrw(BAKMEM12, BAKMEM12_);
#define BAKMEM13_             0x067A    /* Battery Backup Memory 13 */
sfrb(BAKMEM13_L , BAKMEM13_);
sfrb(BAKMEM13_H , BAKMEM13_+1);
sfrw(BAKMEM13, BAKMEM13_);
#define BAKMEM14_             0x067C    /* Battery Backup Memory 14 */
sfrb(BAKMEM14_L , BAKMEM14_);
sfrb(BAKMEM14_H , BAKMEM14_+1);
sfrw(BAKMEM14, BAKMEM14_);
#define BAKMEM15_             0x067E    /* Battery Backup Memory 15 */
sfrb(BAKMEM15_L , BAKMEM15_);
sfrb(BAKMEM15_H , BAKMEM15_+1);
sfrw(BAKMEM15, BAKMEM15_);

/************************************************************
* Capacitive_Touch_IO 0
************************************************************/
#define __MSP430_HAS_CAP_TOUCH_IO_0__ /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CAP_TOUCH_IO_0__ 0x02E0
#define CAP_TOUCH_0_BASE __MSP430_BASEADDRESS_CAP_TOUCH_IO_0__

#define CAPTIO0CTL_           0x02EE    /* Capacitive_Touch_IO 0 control register */
sfrb(CAPTIO0CTL_L , CAPTIO0CTL_);
sfrb(CAPTIO0CTL_H , CAPTIO0CTL_+1);
sfrw(CAPTIO0CTL, CAPTIO0CTL_);

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

/*************************************************************
* CRC Module
*************************************************************/
#define __MSP430_HAS_CRC__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CRC__ 0x01C0
#define CRC_BASE __MSP430_BASEADDRESS_CRC__

#define CRCDI_                0x01C0    /* CRC Data In Register */
sfrb(CRCDI_L , CRCDI_);
sfrb(CRCDI_H , CRCDI_+1);
sfrw(CRCDI, CRCDI_);
#define CRCDIRB_              0x01C2    /* CRC data in reverse byte Register */
sfrb(CRCDIRB_L , CRCDIRB_);
sfrb(CRCDIRB_H , CRCDIRB_+1);
sfrw(CRCDIRB, CRCDIRB_);
#define CRCINIRES_            0x01C4    /* CRC Initialisation Register and Result Register */
sfrb(CRCINIRES_L , CRCINIRES_);
sfrb(CRCINIRES_H , CRCINIRES_+1);
sfrw(CRCINIRES, CRCINIRES_);
#define CRCRESR_              0x01C6    /* CRC reverse result Register */
sfrb(CRCRESR_L , CRCRESR_);
sfrb(CRCRESR_H , CRCRESR_+1);
sfrw(CRCRESR, CRCRESR_);

/************************************************************
* CLOCK SYSTEM CONTROL
************************************************************/
#define __MSP430_HAS_CS__             /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CS__ 0x0180
#define CS_BASE __MSP430_BASEADDRESS_CS__

#define CSCTL0_               0x0180    /* CS Control Register 0 */
sfrb(CSCTL0_L , CSCTL0_);
sfrb(CSCTL0_H , CSCTL0_+1);
sfrw(CSCTL0, CSCTL0_);
#define CSCTL1_               0x0182    /* CS Control Register 1 */
sfrb(CSCTL1_L , CSCTL1_);
sfrb(CSCTL1_H , CSCTL1_+1);
sfrw(CSCTL1, CSCTL1_);
#define CSCTL2_               0x0184    /* CS Control Register 2 */
sfrb(CSCTL2_L , CSCTL2_);
sfrb(CSCTL2_H , CSCTL2_+1);
sfrw(CSCTL2, CSCTL2_);
#define CSCTL3_               0x0186    /* CS Control Register 3 */
sfrb(CSCTL3_L , CSCTL3_);
sfrb(CSCTL3_H , CSCTL3_+1);
sfrw(CSCTL3, CSCTL3_);
#define CSCTL4_               0x0188    /* CS Control Register 4 */
sfrb(CSCTL4_L , CSCTL4_);
sfrb(CSCTL4_H , CSCTL4_+1);
sfrw(CSCTL4, CSCTL4_);
#define CSCTL5_               0x018A    /* CS Control Register 5 */
sfrb(CSCTL5_L , CSCTL5_);
sfrb(CSCTL5_H , CSCTL5_+1);
sfrw(CSCTL5, CSCTL5_);
#define CSCTL6_               0x018C    /* CS Control Register 6 */
sfrb(CSCTL6_L , CSCTL6_);
sfrb(CSCTL6_H , CSCTL6_+1);
sfrw(CSCTL6, CSCTL6_);
#define CSCTL7_               0x018E    /* CS Control Register 7 */
sfrb(CSCTL7_L , CSCTL7_);
sfrb(CSCTL7_H , CSCTL7_+1);
sfrw(CSCTL7, CSCTL7_);
#define CSCTL8_               0x0190    /* CS Control Register 8 */
sfrb(CSCTL8_L , CSCTL8_);
sfrb(CSCTL8_H , CSCTL8_+1);
sfrw(CSCTL8, CSCTL8_);

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


/*************************************************************
* FRAM Memory
*************************************************************/
#define __MSP430_HAS_FRAM__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_FRAM__ 0x01A0
#define FRAM_BASE __MSP430_BASEADDRESS_FRAM__
#define __MSP430_HAS_GC__            /* Definition to show that Module is available */

#define FRCTL0_               0x01A0    /* FRAM Controller Control 0 */
sfrb(FRCTL0_L , FRCTL0_);
sfrb(FRCTL0_H , FRCTL0_+1);
sfrw(FRCTL0, FRCTL0_);
#define GCCTL0_               0x01A4    /* General Control 0 */
sfrb(GCCTL0_L , GCCTL0_);
sfrb(GCCTL0_H , GCCTL0_+1);
sfrw(GCCTL0, GCCTL0_);
#define GCCTL1_               0x01A6    /* General Control 1 */
sfrb(GCCTL1_L , GCCTL1_);
sfrb(GCCTL1_H , GCCTL1_+1);
sfrw(GCCTL1, GCCTL1_);

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


/************************************************************
* LCD_E
************************************************************/
#define __MSP430_HAS_LCD_E__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_LCD_E__ 0x0600
#define LCD_E_BASE __MSP430_BASEADDRESS_LCD_E__

#define LCDCTL0_              0x0600    /* LCD_E Control Register 0 */
sfrb(LCDCTL0_L , LCDCTL0_);
sfrb(LCDCTL0_H , LCDCTL0_+1);
sfrw(LCDCTL0, LCDCTL0_);
#define LCDCTL1_              0x0602    /* LCD_E Control Register 1 */
sfrb(LCDCTL1_L , LCDCTL1_);
sfrb(LCDCTL1_H , LCDCTL1_+1);
sfrw(LCDCTL1, LCDCTL1_);
#define LCDBLKCTL_            0x0604    /* LCD_E blinking control register */
sfrb(LCDBLKCTL_L , LCDBLKCTL_);
sfrb(LCDBLKCTL_H , LCDBLKCTL_+1);
sfrw(LCDBLKCTL, LCDBLKCTL_);
#define LCDMEMCTL_            0x0606    /* LCD_E memory control register */
sfrb(LCDMEMCTL_L , LCDMEMCTL_);
sfrb(LCDMEMCTL_H , LCDMEMCTL_+1);
sfrw(LCDMEMCTL, LCDMEMCTL_);
#define LCDVCTL_              0x0608    /* LCD_E Voltage Control Register */
sfrb(LCDVCTL_L , LCDVCTL_);
sfrb(LCDVCTL_H , LCDVCTL_+1);
sfrw(LCDVCTL, LCDVCTL_);
#define LCDPCTL0_             0x060A    /* LCD_E Port Control Register 0 */
sfrb(LCDPCTL0_L , LCDPCTL0_);
sfrb(LCDPCTL0_H , LCDPCTL0_+1);
sfrw(LCDPCTL0, LCDPCTL0_);
#define LCDPCTL1_             0x060C    /* LCD_E Port Control Register 1 */
sfrb(LCDPCTL1_L , LCDPCTL1_);
sfrb(LCDPCTL1_H , LCDPCTL1_+1);
sfrw(LCDPCTL1, LCDPCTL1_);
#define LCDPCTL2_             0x060E    /* LCD_E Port Control Register 2 */
sfrb(LCDPCTL2_L , LCDPCTL2_);
sfrb(LCDPCTL2_H , LCDPCTL2_+1);
sfrw(LCDPCTL2, LCDPCTL2_);
#define LCDCSSEL0_            0x0614    /* LCD_E COM/SEG select register 0 */
sfrb(LCDCSSEL0_L , LCDCSSEL0_);
sfrb(LCDCSSEL0_H , LCDCSSEL0_+1);
sfrw(LCDCSSEL0, LCDCSSEL0_);
#define LCDCSSEL1_            0x0616    /* LCD_E COM/SEG select register 1 */
sfrb(LCDCSSEL1_L , LCDCSSEL1_);
sfrb(LCDCSSEL1_H , LCDCSSEL1_+1);
sfrw(LCDCSSEL1, LCDCSSEL1_);
#define LCDCSSEL2_            0x0618    /* LCD_E COM/SEG select register 2 */
sfrb(LCDCSSEL2_L , LCDCSSEL2_);
sfrb(LCDCSSEL2_H , LCDCSSEL2_+1);
sfrw(LCDCSSEL2, LCDCSSEL2_);
#define LCDIV_                0x061E    /* LCD_E Interrupt Vector Register */
sfrw(LCDIV, LCDIV_);

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

#define LCDM0W_               0x0620    /* LCD Memory 0/1 */
sfrb(LCDM0W_L , LCDM0W_);
sfrb(LCDM0W_H , LCDM0W_+1);
sfrw(LCDM0W, LCDM0W_);
#define LCDM0               LCDM0W_L  /* LCD Memory 0 */
#define LCDM1               LCDM0W_H  /* LCD Memory 1 */
#define LCDMEM_             LCDM0     /* LCD Memory */
#ifndef __STDC__
#define LCDMEM              LCDM0     /* LCD Memory (for assembler) */
#else
#define LCDMEM              ((volatile char*) &LCDM0) /* LCD Memory (for C) */
#endif
#define LCDM2W_               0x0622    /* LCD Memory 2/3 */
sfrb(LCDM2W_L , LCDM2W_);
sfrb(LCDM2W_H , LCDM2W_+1);
sfrw(LCDM2W, LCDM2W_);
#define LCDM2               LCDM2W_L  /* LCD Memory 2 */
#define LCDM3               LCDM2W_H  /* LCD Memory 3 */
#define LCDM4W_               0x0624    /* LCD Memory 4/5 */
sfrb(LCDM4W_L , LCDM4W_);
sfrb(LCDM4W_H , LCDM4W_+1);
sfrw(LCDM4W, LCDM4W_);
#define LCDM4               LCDM4W_L  /* LCD Memory 4 */
#define LCDM5               LCDM4W_H  /* LCD Memory 5 */
#define LCDM6W_               0x0626    /* LCD Memory 6/7 */
sfrb(LCDM6W_L , LCDM6W_);
sfrb(LCDM6W_H , LCDM6W_+1);
sfrw(LCDM6W, LCDM6W_);
#define LCDM6               LCDM6W_L  /* LCD Memory 6 */
#define LCDM7               LCDM6W_H  /* LCD Memory 7 */
#define LCDM8W_               0x0628    /* LCD Memory 8/9 */
sfrb(LCDM8W_L , LCDM8W_);
sfrb(LCDM8W_H , LCDM8W_+1);
sfrw(LCDM8W, LCDM8W_);
#define LCDM8               LCDM8W_L  /* LCD Memory 8 */
#define LCDM9               LCDM8W_H  /* LCD Memory 9 */
#define LCDM10W_              0x062A    /* LCD Memory 10/11 */
sfrb(LCDM10W_L , LCDM10W_);
sfrb(LCDM10W_H , LCDM10W_+1);
sfrw(LCDM10W, LCDM10W_);
#define LCDM10              LCDM10W_L /* LCD Memory 10 */
#define LCDM11              LCDM10W_H /* LCD Memory 11 */
#define LCDM12W_              0x062C    /* LCD Memory 12/13 */
sfrb(LCDM12W_L , LCDM12W_);
sfrb(LCDM12W_H , LCDM12W_+1);
sfrw(LCDM12W, LCDM12W_);
#define LCDM12              LCDM12W_L /* LCD Memory 12 */
#define LCDM13              LCDM12W_H /* LCD Memory 13 */
#define LCDM14W_              0x062E    /* LCD Memory 14/15 */
sfrb(LCDM14W_L , LCDM14W_);
sfrb(LCDM14W_H , LCDM14W_+1);
sfrw(LCDM14W, LCDM14W_);
#define LCDM14              LCDM14W_L /* LCD Memory 14 */
#define LCDM15              LCDM14W_H /* LCD Memory 15 */
#define LCDM16W_              0x0630    /* LCD Memory 16/17 */
sfrb(LCDM16W_L , LCDM16W_);
sfrb(LCDM16W_H , LCDM16W_+1);
sfrw(LCDM16W, LCDM16W_);
#define LCDM16              LCDM16W_L /* LCD Memory 16 */
#define LCDM17              LCDM16W_H /* LCD Memory 17 */
#define LCDM18W_              0x0632    /* LCD Memory 18/19 */
sfrb(LCDM18W_L , LCDM18W_);
sfrb(LCDM18W_H , LCDM18W_+1);
sfrw(LCDM18W, LCDM18W_);
#define LCDM18              LCDM18W_L /* LCD Memory 18 */
#define LCDM19              LCDM18W_H /* LCD Memory 19 */
#define LCDM20W_              0x0634    /* LCD Memory 20/21 */
sfrb(LCDM20W_L , LCDM20W_);
sfrb(LCDM20W_H , LCDM20W_+1);
sfrw(LCDM20W, LCDM20W_);
#define LCDM20              LCDM20W_L /* LCD Memory 20 */
#define LCDM21              LCDM20W_H /* LCD Memory 21 */
#define LCDM22W_              0x0636    /* LCD Memory 22/23 */
sfrb(LCDM22W_L , LCDM22W_);
sfrb(LCDM22W_H , LCDM22W_+1);
sfrw(LCDM22W, LCDM22W_);
#define LCDM22              LCDM22W_L /* LCD Memory 22 */
#define LCDM23              LCDM22W_H /* LCD Memory 23 */
#define LCDM24W_              0x0638    /* LCD Memory 24/25 */
sfrb(LCDM24W_L , LCDM24W_);
sfrb(LCDM24W_H , LCDM24W_+1);
sfrw(LCDM24W, LCDM24W_);
#define LCDM24              LCDM24W_L /* LCD Memory 24 */
#define LCDM25              LCDM24W_H /* LCD Memory 25 */
#define LCDM26W_              0x063A    /* LCD Memory 26/27 */
sfrb(LCDM26W_L , LCDM26W_);
sfrb(LCDM26W_H , LCDM26W_+1);
sfrw(LCDM26W, LCDM26W_);
#define LCDM26              LCDM26W_L /* LCD Memory 26 */
#define LCDM27              LCDM26W_H /* LCD Memory 27 */
#define LCDM28W_              0x063C    /* LCD Memory 28/29 */
sfrb(LCDM28W_L , LCDM28W_);
sfrb(LCDM28W_H , LCDM28W_+1);
sfrw(LCDM28W, LCDM28W_);
#define LCDM28              LCDM28W_L /* LCD Memory 28 */
#define LCDM29              LCDM28W_H /* LCD Memory 29 */
#define LCDM30W_              0x063E    /* LCD Memory 30/31 */
sfrb(LCDM30W_L , LCDM30W_);
sfrb(LCDM30W_H , LCDM30W_+1);
sfrw(LCDM30W, LCDM30W_);
#define LCDM30              LCDM30W_L /* LCD Memory 30 */
#define LCDM31              LCDM30W_H /* LCD Memory 31 */
#define LCDM32W_              0x0640    /* LCD Memory 32/33 */
sfrb(LCDM32W_L , LCDM32W_);
sfrb(LCDM32W_H , LCDM32W_+1);
sfrw(LCDM32W, LCDM32W_);
#define LCDM32              LCDM32W_L /* LCD Memory 32 */
#define LCDM33              LCDM32W_H /* LCD Memory 33 */
#define LCDM34W_              0x0642    /* LCD Memory 34/35 */
sfrb(LCDM34W_L , LCDM34W_);
sfrb(LCDM34W_H , LCDM34W_+1);
sfrw(LCDM34W, LCDM34W_);
#define LCDM34              LCDM34W_L /* LCD Memory 34 */
#define LCDM35              LCDM34W_H /* LCD Memory 35 */
#define LCDM36W_              0x0644    /* LCD Memory 36/37 */
sfrb(LCDM36W_L , LCDM36W_);
sfrb(LCDM36W_H , LCDM36W_+1);
sfrw(LCDM36W, LCDM36W_);
#define LCDM36              LCDM36W_L /* LCD Memory 36 */
#define LCDM37              LCDM36W_H /* LCD Memory 37 */
#define LCDM38W_              0x0646    /* LCD Memory 38/39 */
sfrb(LCDM38W_L , LCDM38W_);
sfrb(LCDM38W_H , LCDM38W_+1);
sfrw(LCDM38W, LCDM38W_);
#define LCDM38              LCDM38W_L /* LCD Memory 38 */
#define LCDM39              LCDM38W_H /* LCD Memory 39 */

#define LCDBM0W_              0x0640    /* LCD Blinking Memory 0/1 */
sfrb(LCDBM0W_L , LCDBM0W_);
sfrb(LCDBM0W_H , LCDBM0W_+1);
sfrw(LCDBM0W, LCDBM0W_);
#define LCDBM0              LCDBM0W_L /* LCD Blinking Memory 0 */
#define LCDBM1              LCDBM0W_H /* LCD Blinking Memory 1 */
#define LCDBMEM_            LCDBM0    /* LCD Blinking Memory */
#ifndef __STDC__
#define LCDBMEM             LCDBM0    /* LCD Blinking Memory (for assembler) */
#else
#define LCDBMEM             ((volatile char*) &LCDBM0) /* LCD Blinking Memory (for C) */
#endif
#define LCDBM2W_              0x0642    /* LCD Blinking Memory 2/3 */
sfrb(LCDBM2W_L , LCDBM2W_);
sfrb(LCDBM2W_H , LCDBM2W_+1);
sfrw(LCDBM2W, LCDBM2W_);
#define LCDBM2              LCDBM2W_L  /* LCD Blinking Memory 2 */
#define LCDBM3              LCDBM2W_H  /* LCD Blinking Memory 3 */
#define LCDBM4W_              0x0644    /* LCD Blinking Memory 4/5 */
sfrb(LCDBM4W_L , LCDBM4W_);
sfrb(LCDBM4W_H , LCDBM4W_+1);
sfrw(LCDBM4W, LCDBM4W_);
#define LCDBM4              LCDBM4W_L  /* LCD Blinking Memory 4 */
#define LCDBM5              LCDBM4W_H  /* LCD Blinking Memory 5 */
#define LCDBM6W_              0x0646    /* LCD Blinking Memory 6/7 */
sfrb(LCDBM6W_L , LCDBM6W_);
sfrb(LCDBM6W_H , LCDBM6W_+1);
sfrw(LCDBM6W, LCDBM6W_);
#define LCDBM6              LCDBM6W_L  /* LCD Blinking Memory 6 */
#define LCDBM7              LCDBM6W_H  /* LCD Blinking Memory 7 */
#define LCDBM8W_              0x0648    /* LCD Blinking Memory 8/9 */
sfrb(LCDBM8W_L , LCDBM8W_);
sfrb(LCDBM8W_H , LCDBM8W_+1);
sfrw(LCDBM8W, LCDBM8W_);
#define LCDBM8              LCDBM8W_L  /* LCD Blinking Memory 8 */
#define LCDBM9              LCDBM8W_H  /* LCD Blinking Memory 9 */
#define LCDBM10W_             0x064A    /* LCD Blinking Memory 10/11 */
sfrb(LCDBM10W_L , LCDBM10W_);
sfrb(LCDBM10W_H , LCDBM10W_+1);
sfrw(LCDBM10W, LCDBM10W_);
#define LCDBM10             LCDBM10W_L /* LCD Blinking Memory 10 */
#define LCDBM11             LCDBM10W_H /* LCD Blinking Memory 11 */
#define LCDBM12W_             0x064C    /* LCD Blinking Memory 12/13 */
sfrb(LCDBM12W_L , LCDBM12W_);
sfrb(LCDBM12W_H , LCDBM12W_+1);
sfrw(LCDBM12W, LCDBM12W_);
#define LCDBM12             LCDBM12W_L /* LCD Blinking Memory 12 */
#define LCDBM13             LCDBM12W_H /* LCD Blinking Memory 13 */
#define LCDBM14W_             0x064E    /* LCD Blinking Memory 14/15 */
sfrb(LCDBM14W_L , LCDBM14W_);
sfrb(LCDBM14W_H , LCDBM14W_+1);
sfrw(LCDBM14W, LCDBM14W_);
#define LCDBM14             LCDBM14W_L /* LCD Blinking Memory 14 */
#define LCDBM15             LCDBM14W_H /* LCD Blinking Memory 15 */
#define LCDBM16W_             0x0650    /* LCD Blinking Memory 16/17 */
sfrb(LCDBM16W_L , LCDBM16W_);
sfrb(LCDBM16W_H , LCDBM16W_+1);
sfrw(LCDBM16W, LCDBM16W_);
#define LCDBM16             LCDBM16W_L /* LCD Blinking Memory 16 */
#define LCDBM17             LCDBM16W_H /* LCD Blinking Memory 17 */
#define LCDBM18W_             0x0652    /* LCD Blinking Memory 18/19 */
sfrb(LCDBM18W_L , LCDBM18W_);
sfrb(LCDBM18W_H , LCDBM18W_+1);
sfrw(LCDBM18W, LCDBM18W_);
#define LCDBM18             LCDBM18W_L /* LCD Blinking Memory 18 */
#define LCDBM19             LCDBM18W_H /* LCD Blinking Memory 19 */

/* LCDIV Definitions */
#define LCDIV_NONE         (0x0000)    /* No Interrupt pending */
#define LCDIV_LCDBLKOFFIFG (0x0004)    /* Blink, segments off */
#define LCDIV_LCDBLKONIFG  (0x0006)    /* Blink, segments on */
#define LCDIV_LCDFRMIFG    (0x0008)    /* Frame interrupt */

/************************************************************
* PMM - Power Management System for FR2xx/FR4xx
************************************************************/
#define __MSP430_HAS_PMM_FRAM__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PMM_FRAM__ 0x0120
#define PMM_BASE __MSP430_BASEADDRESS_PMM_FRAM__

#define PMMCTL0_              0x0120    /* PMM Control 0 */
sfrb(PMMCTL0_L , PMMCTL0_);
sfrb(PMMCTL0_H , PMMCTL0_+1);
sfrw(PMMCTL0, PMMCTL0_);
#define PMMCTL1_              0x0122    /* PMM Control 1 */
sfrb(PMMCTL1_L , PMMCTL1_);
sfrb(PMMCTL1_H , PMMCTL1_+1);
sfrw(PMMCTL1, PMMCTL1_);
#define PMMCTL2_              0x0124    /* PMM Control 2 */
sfrb(PMMCTL2_L , PMMCTL2_);
sfrb(PMMCTL2_H , PMMCTL2_+1);
sfrw(PMMCTL2, PMMCTL2_);
#define PMMIFG_               0x012A    /* PMM Interrupt Flag */
sfrb(PMMIFG_L , PMMIFG_);
sfrb(PMMIFG_H , PMMIFG_+1);
sfrw(PMMIFG, PMMIFG_);
#define PMMIE_                0x012E    /* PMM Interrupt Enable */
sfrb(PMMIE_L , PMMIE_);
sfrb(PMMIE_H , PMMIE_+1);
sfrw(PMMIE, PMMIE_);
#define PM5CTL0_              0x0130    /* PMM Power Mode 5 Control Register 0 */
sfrb(PM5CTL0_L , PM5CTL0_);
sfrb(PM5CTL0_H , PM5CTL0_+1);
sfrw(PM5CTL0, PM5CTL0_);

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
#define PASEL0_               0x020A    /* Port A Selection 0 */
sfrb(PASEL0_L , PASEL0_);
sfrb(PASEL0_H , PASEL0_+1);
sfrw(PASEL0, PASEL0_);
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
#define P1SEL0              (PASEL0_L) /* Port 1 Selection 0 */
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
#define PBSEL0_               0x022A    /* Port B Selection 0 */
sfrb(PBSEL0_L , PBSEL0_);
sfrb(PBSEL0_H , PBSEL0_+1);
sfrw(PBSEL0, PBSEL0_);












#define P3IN                (PBIN_L)  /* Port 3 Input */
#define P3OUT               (PBOUT_L) /* Port 3 Output */
#define P3DIR               (PBDIR_L) /* Port 3 Direction */
#define P3REN               (PBREN_L) /* Port 3 Resistor Enable */
#define P3SEL0              (PBSEL0_L) /* Port 3 Selection 0 */


#define P4IN                (PBIN_H)  /* Port 4 Input */
#define P4OUT               (PBOUT_H) /* Port 4 Output */
#define P4DIR               (PBDIR_H) /* Port 4 Direction */
#define P4REN               (PBREN_H) /* Port 4 Resistor Enable */
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
#define PCSEL0_               0x024A    /* Port C Selection 0 */
sfrb(PCSEL0_L , PCSEL0_);
sfrb(PCSEL0_H , PCSEL0_+1);
sfrw(PCSEL0, PCSEL0_);












#define P5IN                (PCIN_L)  /* Port 5 Input */
#define P5OUT               (PCOUT_L) /* Port 5 Output */
#define P5DIR               (PCDIR_L) /* Port 5 Direction */
#define P5REN               (PCREN_L) /* Port 5 Resistor Enable */
#define P5SEL0              (PCSEL0_L) /* Port 5 Selection 0 */


#define P6IN                (PCIN_H)  /* Port 6 Input */
#define P6OUT               (PCOUT_H) /* Port 6 Output */
#define P6DIR               (PCDIR_H) /* Port 6 Direction */
#define P6REN               (PCREN_H) /* Port 6 Resistor Enable */
#define P6SEL0              (PCSEL0_H) /* Port 6 Selection 0 */



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
#define PDSEL0_               0x026A    /* Port D Selection 0 */
sfrb(PDSEL0_L , PDSEL0_);
sfrb(PDSEL0_H , PDSEL0_+1);
sfrw(PDSEL0, PDSEL0_);












#define P7IN                (PDIN_L)  /* Port 7 Input */
#define P7OUT               (PDOUT_L) /* Port 7 Output */
#define P7DIR               (PDDIR_L) /* Port 7 Direction */
#define P7REN               (PDREN_L) /* Port 7 Resistor Enable */
#define P7SEL0              (PDSEL0_L) /* Port 7 Selection 0 */


#define P8IN                (PDIN_H)  /* Port 8 Input */
#define P8OUT               (PDOUT_H) /* Port 8 Output */
#define P8DIR               (PDDIR_H) /* Port 8 Direction */
#define P8REN               (PDREN_H) /* Port 8 Resistor Enable */
#define P8SEL0              (PDSEL0_H) /* Port 8 Selection 0 */



/************************************************************
* Real-Time Clock (RTC) Counter
************************************************************/
#define __MSP430_HAS_RTC__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_RTC__ 0x03C0
#define RTC_BASE __MSP430_BASEADDRESS_RTC__

#define RTCCTL_               0x03C0    /* RTC control Register */
sfrb(RTCCTL_L , RTCCTL_);
sfrb(RTCCTL_H , RTCCTL_+1);
sfrw(RTCCTL, RTCCTL_);
#define RTCIV_                0x03C4    /* RTC interrupt vector */
sfrb(RTCIV_L , RTCIV_);
sfrb(RTCIV_H , RTCIV_+1);
sfrw(RTCIV, RTCIV_);
#define RTCMOD_               0x03C8    /* RTC moduloRegister */
sfrb(RTCMOD_L , RTCMOD_);
sfrb(RTCMOD_H , RTCMOD_+1);
sfrw(RTCMOD, RTCMOD_);
#define RTCCNT_               0x03CC    /* RTC counter Register */
sfrb(RTCCNT_L , RTCCNT_);
sfrb(RTCCNT_H , RTCCNT_+1);
sfrw(RTCCNT, RTCCNT_);



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
#define JMBINIE             (0x0040)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE            (0x0080)  /* JTAG Mail Box output Interrupt Enable */

#define WDTIE_L             (0x0001)  /* WDT Interrupt Enable */
#define OFIE_L              (0x0002)  /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE_L             (0x0008)  /* Vacant Memory Interrupt Enable */
#define NMIIE_L             (0x0010)  /* NMI Interrupt Enable */
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
#define __MSP430_BASEADDRESS_SYS__ 0x0140
#define SYS_BASE __MSP430_BASEADDRESS_SYS__

#define SYSCTL_               0x0140    /* System control */
sfrb(SYSCTL_L , SYSCTL_);
sfrb(SYSCTL_H , SYSCTL_+1);
sfrw(SYSCTL, SYSCTL_);
#define SYSBSLC_              0x0142    /* Boot strap configuration area */
sfrb(SYSBSLC_L , SYSBSLC_);
sfrb(SYSBSLC_H , SYSBSLC_+1);
sfrw(SYSBSLC, SYSBSLC_);
#define SYSJMBC_              0x0146    /* JTAG mailbox control */
sfrb(SYSJMBC_L , SYSJMBC_);
sfrb(SYSJMBC_H , SYSJMBC_+1);
sfrw(SYSJMBC, SYSJMBC_);
#define SYSJMBI0_             0x0148    /* JTAG mailbox input 0 */
sfrb(SYSJMBI0_L , SYSJMBI0_);
sfrb(SYSJMBI0_H , SYSJMBI0_+1);
sfrw(SYSJMBI0, SYSJMBI0_);
#define SYSJMBI1_             0x014A    /* JTAG mailbox input 1 */
sfrb(SYSJMBI1_L , SYSJMBI1_);
sfrb(SYSJMBI1_H , SYSJMBI1_+1);
sfrw(SYSJMBI1, SYSJMBI1_);
#define SYSJMBO0_             0x014C    /* JTAG mailbox output 0 */
sfrb(SYSJMBO0_L , SYSJMBO0_);
sfrb(SYSJMBO0_H , SYSJMBO0_+1);
sfrw(SYSJMBO0, SYSJMBO0_);
#define SYSJMBO1_             0x014E    /* JTAG mailbox output 1 */
sfrb(SYSJMBO1_L , SYSJMBO1_);
sfrb(SYSJMBO1_H , SYSJMBO1_+1);
sfrw(SYSJMBO1, SYSJMBO1_);

#define SYSBERRIV_            0x0158    /* Bus Error vector generator */
sfrb(SYSBERRIV_L , SYSBERRIV_);
sfrb(SYSBERRIV_H , SYSBERRIV_+1);
sfrw(SYSBERRIV, SYSBERRIV_);
#define SYSUNIV_              0x015A    /* User NMI vector generator */
sfrb(SYSUNIV_L , SYSUNIV_);
sfrb(SYSUNIV_H , SYSUNIV_+1);
sfrw(SYSUNIV, SYSUNIV_);
#define SYSSNIV_              0x015C    /* System NMI vector generator */
sfrb(SYSSNIV_L , SYSSNIV_);
sfrb(SYSSNIV_H , SYSSNIV_+1);
sfrw(SYSSNIV, SYSSNIV_);
#define SYSRSTIV_             0x015E    /* Reset vector generator */
sfrb(SYSRSTIV_L , SYSRSTIV_);
sfrb(SYSRSTIV_H , SYSRSTIV_+1);
sfrw(SYSRSTIV, SYSRSTIV_);
#define SYSCFG0_              0x0160    /* System Configuration 0 */
sfrb(SYSCFG0_L , SYSCFG0_);
sfrb(SYSCFG0_H , SYSCFG0_+1);
sfrw(SYSCFG0, SYSCFG0_);
#define SYSCFG1_              0x0162    /* System Configuration 1 */
sfrb(SYSCFG1_L , SYSCFG1_);
sfrb(SYSCFG1_H , SYSCFG1_+1);
sfrw(SYSCFG1, SYSCFG1_);
#define SYSCFG2_              0x0164    /* System Configuration 2 */
sfrb(SYSCFG2_L , SYSCFG2_);
sfrb(SYSCFG2_H , SYSCFG2_+1);
sfrw(SYSCFG2, SYSCFG2_);

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
#define LCDPCTL             (0x1000)  /* LCD Power Pin */

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
#define LCDPCTL_H           (0x0010)  /* LCD Power Pin */







/* SYSUNIV Definitions */
#define SYSUNIV_NONE       (0x0000)    /* No Interrupt pending */
#define SYSUNIV_NMIIFG     (0x0002)    /* SYSUNIV : NMIIFG */
#define SYSUNIV_OFIFG      (0x0004)    /* SYSUNIV : Osc. Fail - OFIFG */

/* SYSSNIV Definitions */
#define SYSSNIV_NONE       (0x0000)    /* No Interrupt pending */
#define SYSSNIV_SVS        (0x0002)    /* SYSSNIV : SVS low-power reset entry */
#define SYSSNIV_UBDIFG     (0x0004)    /* SYSSNIV : FRAM Uncorrectable bit Error */
#define SYSSNIV_ACCTEIFG   (0x0006)    /* SYSSNIV : ACCTEIFG access time error */
#define SYSSNIV_RES8       (0x0008)    /* SYSSNIV : Reserved */
#define SYSSNIV_RES10      (0x000A)    /* SYSSNIV : Reserved */
#define SYSSNIV_RES12      (0x000C)    /* SYSSNIV : Reserved */
#define SYSSNIV_RES14      (0x000E)    /* SYSSNIV : Reserved */
#define SYSSNIV_RES16      (0x0010)    /* SYSSNIV : Reserved */
#define SYSSNIV_VMAIFG     (0x0012)    /* SYSSNIV : VMAIFG */
#define SYSSNIV_JMBINIFG   (0x0014)    /* SYSSNIV : JMBINIFG */
#define SYSSNIV_JMBOUTIFG  (0x0016)    /* SYSSNIV : JMBOUTIFG */
#define SYSSNIV_CBDIFG     (0x0018)    /* SYSSNIV : FRAM Correctable Bit error */

/* SYSRSTIV Definitions */
#define SYSRSTIV_NONE      (0x0000)    /* No Interrupt pending */
#define SYSRSTIV_BOR       (0x0002)    /* SYSRSTIV : BOR */
#define SYSRSTIV_RSTNMI    (0x0004)    /* SYSRSTIV : RST/NMI */
#define SYSRSTIV_DOBOR     (0x0006)    /* SYSRSTIV : Do BOR */
#define SYSRSTIV_LPM5WU    (0x0008)    /* SYSRSTIV : Port LPM5 Wake Up */
#define SYSRSTIV_SECYV     (0x000A)    /* SYSRSTIV : Security violation */
#define SYSRSTIV_RES0C     (0x000C)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_SVSHIFG   (0x000E)    /* SYSRSTIV : SVSHIFG */
#define SYSRSTIV_RES10     (0x0010)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_RES12     (0x0012)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_DOPOR     (0x0014)    /* SYSRSTIV : Do POR */
#define SYSRSTIV_WDTTO     (0x0016)    /* SYSRSTIV : WDT Time out */
#define SYSRSTIV_WDTPW     (0x0018)    /* SYSRSTIV : WDT Key violation */
#define SYSRSTIV_WDTKEY    (0x0018)    /* SYSRSTIV : WDT Key violation */
#define SYSRSTIV_FRCTLPW   (0x001A)    /* SYSRSTIV : FRAM Key violation */
#define SYSRSTIV_UBDIFG    (0x001C)    /* SYSRSTIV : FRAM Uncorrectable bit Error */
#define SYSRSTIV_PERF      (0x001E)    /* SYSRSTIV : peripheral/config area fetch */
#define SYSRSTIV_PMMPW     (0x0020)    /* SYSRSTIV : PMM Password violation */
#define SYSRSTIV_RES22     (0x0022)    /* SYSRSTIV : Reserved */
#define SYSRSTIV_FLLUL     (0x0024)    /* SYSRSTIV : FLL unlock */

/************************************************************
* Timer0_A3
************************************************************/
#define __MSP430_HAS_T0A3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0A3__ 0x0300
#define TIMER_A0_BASE __MSP430_BASEADDRESS_T0A3__

#define TA0CTL_               0x0300    /* Timer0_A3 Control */
sfrw(TA0CTL, TA0CTL_);
#define TA0CCTL0_             0x0302    /* Timer0_A3 Capture/Compare Control 0 */
sfrw(TA0CCTL0, TA0CCTL0_);
#define TA0CCTL1_             0x0304    /* Timer0_A3 Capture/Compare Control 1 */
sfrw(TA0CCTL1, TA0CCTL1_);
#define TA0CCTL2_             0x0306    /* Timer0_A3 Capture/Compare Control 2 */
sfrw(TA0CCTL2, TA0CCTL2_);
#define TA0R_                 0x0310    /* Timer0_A3 */
sfrw(TA0R, TA0R_);
#define TA0CCR0_              0x0312    /* Timer0_A3 Capture/Compare 0 */
sfrw(TA0CCR0, TA0CCR0_);
#define TA0CCR1_              0x0314    /* Timer0_A3 Capture/Compare 1 */
sfrw(TA0CCR1, TA0CCR1_);
#define TA0CCR2_              0x0316    /* Timer0_A3 Capture/Compare 2 */
sfrw(TA0CCR2, TA0CCR2_);
#define TA0IV_                0x032E    /* Timer0_A3 Interrupt Vector Word */
sfrw(TA0IV, TA0IV_);
#define TA0EX0_               0x0320    /* Timer0_A3 Expansion Register 0 */
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
* Timer1_A3
************************************************************/
#define __MSP430_HAS_T1A3__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T1A3__ 0x0340
#define TIMER_A1_BASE __MSP430_BASEADDRESS_T1A3__

#define TA1CTL_               0x0340    /* Timer1_A3 Control */
sfrw(TA1CTL, TA1CTL_);
#define TA1CCTL0_             0x0342    /* Timer1_A3 Capture/Compare Control 0 */
sfrw(TA1CCTL0, TA1CCTL0_);
#define TA1CCTL1_             0x0344    /* Timer1_A3 Capture/Compare Control 1 */
sfrw(TA1CCTL1, TA1CCTL1_);
#define TA1CCTL2_             0x0346    /* Timer1_A3 Capture/Compare Control 2 */
sfrw(TA1CCTL2, TA1CCTL2_);
#define TA1R_                 0x0350    /* Timer1_A3 */
sfrw(TA1R, TA1R_);
#define TA1CCR0_              0x0352    /* Timer1_A3 Capture/Compare 0 */
sfrw(TA1CCR0, TA1CCR0_);
#define TA1CCR1_              0x0354    /* Timer1_A3 Capture/Compare 1 */
sfrw(TA1CCR1, TA1CCR1_);
#define TA1CCR2_              0x0356    /* Timer1_A3 Capture/Compare 2 */
sfrw(TA1CCR2, TA1CCR2_);
#define TA1IV_                0x036E    /* Timer1_A3 Interrupt Vector Word */
sfrw(TA1IV, TA1IV_);
#define TA1EX0_               0x0360    /* Timer1_A3 Expansion Register 0 */
sfrw(TA1EX0, TA1EX0_);

/* Bits are already defined within the Timer0_Ax */

/* TA1IV Definitions */
#define TA1IV_NONE          (0x0000)    /* No Interrupt pending */
#define TA1IV_TACCR1        (0x0002)    /* TA1CCR1_CCIFG */
#define TA1IV_TACCR2        (0x0004)    /* TA1CCR2_CCIFG */
#define TA1IV_3             (0x0006)    /* Reserved */
#define TA1IV_4             (0x0008)    /* Reserved */
#define TA1IV_5             (0x000A)    /* Reserved */
#define TA1IV_6             (0x000C)    /* Reserved */
#define TA1IV_TAIFG         (0x000E)    /* TA1IFG */

/* Legacy Defines */
#define TA1IV_TA1CCR1      (0x0002)    /* TA1CCR1_CCIFG */
#define TA1IV_TA1CCR2      (0x0004)    /* TA1CCR2_CCIFG */
#define TA1IV_TA1IFG       (0x000E)    /* TA1IFG */

/************************************************************
* USCI A0
************************************************************/
#define __MSP430_HAS_EUSCI_A0__      /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_A0__ 0x0500
#define EUSCI_A0_BASE __MSP430_BASEADDRESS_EUSCI_A0__

#define UCA0CTLW0_            0x0500    /* USCI A0 Control Word Register 0 */
sfrb(UCA0CTLW0_L , UCA0CTLW0_);
sfrb(UCA0CTLW0_H , UCA0CTLW0_+1);
sfrw(UCA0CTLW0, UCA0CTLW0_);
#define UCA0CTL1            UCA0CTLW0_L  /* USCI A0 Control Register 1 */
#define UCA0CTL0            UCA0CTLW0_H  /* USCI A0 Control Register 0 */
#define UCA0CTLW1_            0x0502    /* USCI A0 Control Word Register 1 */
sfrb(UCA0CTLW1_L , UCA0CTLW1_);
sfrb(UCA0CTLW1_H , UCA0CTLW1_+1);
sfrw(UCA0CTLW1, UCA0CTLW1_);
#define UCA0BRW_              0x0506    /* USCI A0 Baud Word Rate 0 */
sfrb(UCA0BRW_L , UCA0BRW_);
sfrb(UCA0BRW_H , UCA0BRW_+1);
sfrw(UCA0BRW, UCA0BRW_);
#define UCA0BR0             UCA0BRW_L /* USCI A0 Baud Rate 0 */
#define UCA0BR1             UCA0BRW_H /* USCI A0 Baud Rate 1 */
#define UCA0MCTLW_            0x0508    /* USCI A0 Modulation Control */
sfrb(UCA0MCTLW_L , UCA0MCTLW_);
sfrb(UCA0MCTLW_H , UCA0MCTLW_+1);
sfrw(UCA0MCTLW, UCA0MCTLW_);
#define UCA0STATW_            0x050A    /* USCI A0 Status Register */
sfrb(UCA0STATW, UCA0STATW_);
#define UCA0RXBUF_            0x050C    /* USCI A0 Receive Buffer */
const_sfrb(UCA0RXBUF_L , UCA0RXBUF_);
const_sfrb(UCA0RXBUF_H , UCA0RXBUF_+1);
const_sfrw(UCA0RXBUF, UCA0RXBUF_);
#define UCA0TXBUF_            0x050E    /* USCI A0 Transmit Buffer */
sfrb(UCA0TXBUF_L , UCA0TXBUF_);
sfrb(UCA0TXBUF_H , UCA0TXBUF_+1);
sfrw(UCA0TXBUF, UCA0TXBUF_);
#define UCA0ABCTL_            0x0510    /* USCI A0 LIN Control */
sfrb(UCA0ABCTL, UCA0ABCTL_);
#define UCA0IRCTL_            0x0512    /* USCI A0 IrDA Transmit Control */
sfrb(UCA0IRCTL_L , UCA0IRCTL_);
sfrb(UCA0IRCTL_H , UCA0IRCTL_+1);
sfrw(UCA0IRCTL, UCA0IRCTL_);
#define UCA0IRTCTL          UCA0IRCTL_L  /* USCI A0 IrDA Transmit Control */
#define UCA0IRRCTL          UCA0IRCTL_H  /* USCI A0 IrDA Receive Control */
#define UCA0IE_               0x051A    /* USCI A0 Interrupt Enable Register */
sfrb(UCA0IE_L , UCA0IE_);
sfrb(UCA0IE_H , UCA0IE_+1);
sfrw(UCA0IE, UCA0IE_);
#define UCA0IFG_              0x051C    /* USCI A0 Interrupt Flags Register */
sfrb(UCA0IFG_L , UCA0IFG_);
sfrb(UCA0IFG_H , UCA0IFG_+1);
sfrw(UCA0IFG, UCA0IFG_);
#define UCA0IV_               0x051E    /* USCI A0 Interrupt Vector Register */
sfrw(UCA0IV, UCA0IV_);


/************************************************************
* USCI B0
************************************************************/
#define __MSP430_HAS_EUSCI_B0__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_EUSCI_B0__ 0x0540
#define EUSCI_B0_BASE __MSP430_BASEADDRESS_EUSCI_B0__


#define UCB0CTLW0_            0x0540    /* USCI B0 Control Word Register 0 */
sfrb(UCB0CTLW0_L , UCB0CTLW0_);
sfrb(UCB0CTLW0_H , UCB0CTLW0_+1);
sfrw(UCB0CTLW0, UCB0CTLW0_);
#define UCB0CTL1            UCB0CTLW0_L  /* USCI B0 Control Register 1 */
#define UCB0CTL0            UCB0CTLW0_H  /* USCI B0 Control Register 0 */
#define UCB0CTLW1_            0x0542    /* USCI B0 Control Word Register 1 */
sfrb(UCB0CTLW1_L , UCB0CTLW1_);
sfrb(UCB0CTLW1_H , UCB0CTLW1_+1);
sfrw(UCB0CTLW1, UCB0CTLW1_);
#define UCB0BRW_              0x0546    /* USCI B0 Baud Word Rate 0 */
sfrb(UCB0BRW_L , UCB0BRW_);
sfrb(UCB0BRW_H , UCB0BRW_+1);
sfrw(UCB0BRW, UCB0BRW_);
#define UCB0BR0             UCB0BRW_L /* USCI B0 Baud Rate 0 */
#define UCB0BR1             UCB0BRW_H /* USCI B0 Baud Rate 1 */
#define UCB0STATW_            0x0548    /* USCI B0 Status Word Register */
sfrb(UCB0STATW_L , UCB0STATW_);
sfrb(UCB0STATW_H , UCB0STATW_+1);
sfrw(UCB0STATW, UCB0STATW_);
#define UCB0STAT            UCB0STATW_L /* USCI B0 Status Register */
#define UCB0BCNT            UCB0STATW_H /* USCI B0 Byte Counter Register */
#define UCB0TBCNT_            0x054A    /* USCI B0 Byte Counter Threshold Register */
sfrb(UCB0TBCNT_L , UCB0TBCNT_);
sfrb(UCB0TBCNT_H , UCB0TBCNT_+1);
sfrw(UCB0TBCNT, UCB0TBCNT_);
#define UCB0RXBUF_            0x054C    /* USCI B0 Receive Buffer */
const_sfrb(UCB0RXBUF_L , UCB0RXBUF_);
const_sfrb(UCB0RXBUF_H , UCB0RXBUF_+1);
const_sfrw(UCB0RXBUF, UCB0RXBUF_);
#define UCB0TXBUF_            0x054E    /* USCI B0 Transmit Buffer */
sfrb(UCB0TXBUF_L , UCB0TXBUF_);
sfrb(UCB0TXBUF_H , UCB0TXBUF_+1);
sfrw(UCB0TXBUF, UCB0TXBUF_);
#define UCB0I2COA0_           0x0554    /* USCI B0 I2C Own Address 0 */
sfrb(UCB0I2COA0_L , UCB0I2COA0_);
sfrb(UCB0I2COA0_H , UCB0I2COA0_+1);
sfrw(UCB0I2COA0, UCB0I2COA0_);
#define UCB0I2COA1_           0x0556    /* USCI B0 I2C Own Address 1 */
sfrb(UCB0I2COA1_L , UCB0I2COA1_);
sfrb(UCB0I2COA1_H , UCB0I2COA1_+1);
sfrw(UCB0I2COA1, UCB0I2COA1_);
#define UCB0I2COA2_           0x0558    /* USCI B0 I2C Own Address 2 */
sfrb(UCB0I2COA2_L , UCB0I2COA2_);
sfrb(UCB0I2COA2_H , UCB0I2COA2_+1);
sfrw(UCB0I2COA2, UCB0I2COA2_);
#define UCB0I2COA3_           0x055A    /* USCI B0 I2C Own Address 3 */
sfrb(UCB0I2COA3_L , UCB0I2COA3_);
sfrb(UCB0I2COA3_H , UCB0I2COA3_+1);
sfrw(UCB0I2COA3, UCB0I2COA3_);
#define UCB0ADDRX_            0x055C    /* USCI B0 Received Address Register */
sfrb(UCB0ADDRX_L , UCB0ADDRX_);
sfrb(UCB0ADDRX_H , UCB0ADDRX_+1);
sfrw(UCB0ADDRX, UCB0ADDRX_);
#define UCB0ADDMASK_          0x055E    /* USCI B0 Address Mask Register */
sfrb(UCB0ADDMASK_L , UCB0ADDMASK_);
sfrb(UCB0ADDMASK_H , UCB0ADDMASK_+1);
sfrw(UCB0ADDMASK, UCB0ADDMASK_);
#define UCB0I2CSA_            0x0560    /* USCI B0 I2C Slave Address */
sfrb(UCB0I2CSA_L , UCB0I2CSA_);
sfrb(UCB0I2CSA_H , UCB0I2CSA_+1);
sfrw(UCB0I2CSA, UCB0I2CSA_);
#define UCB0IE_               0x056A    /* USCI B0 Interrupt Enable Register */
sfrb(UCB0IE_L , UCB0IE_);
sfrb(UCB0IE_H , UCB0IE_+1);
sfrw(UCB0IE, UCB0IE_);
#define UCB0IFG_              0x056C    /* USCI B0 Interrupt Flags Register */
sfrb(UCB0IFG_L , UCB0IFG_);
sfrb(UCB0IFG_H , UCB0IFG_+1);
sfrw(UCB0IFG, UCB0IFG_);
#define UCB0IV_               0x056E    /* USCI B0 Interrupt Vector Register */
sfrw(UCB0IV, UCB0IV_);


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

/************************************************************
* WATCHDOG TIMER A
************************************************************/
#define __MSP430_HAS_WDT_A__          /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_WDT_A__ 0x01C0
#define WDT_A_BASE __MSP430_BASEADDRESS_WDT_A__

#define WDTCTL_               0x01CC    /* Watchdog Timer Control */
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
#define TLV_CRC_START         (0x1A04)    /* Start Address of the CRC protected structure */
#define TLV_CRC_END           (0x1A77)    /* End Address of the TLV protected structure */

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
* Interrupt Vectors (offset from 0xFF80 + 0x10 for Password)
************************************************************/


#define LCD_E_VECTOR            (46)                     /* 0xFFE2 LCD E */
#define PORT2_VECTOR            (47)                     /* 0xFFE4 Port 2 */
#define PORT1_VECTOR            (48)                     /* 0xFFE6 Port 1 */
#define ADC_VECTOR              (49)                     /* 0xFFE8 ADC */
#define USCI_B0_VECTOR          (50)                     /* 0xFFEA USCI B0 Receive/Transmit */
#define USCI_A0_VECTOR          (51)                     /* 0xFFEC USCI A0 Receive/Transmit */
#define WDT_VECTOR              (52)                     /* 0xFFEE Watchdog Timer */
#define RTC_VECTOR              (53)                     /* 0xFFF0 RTC */
#define TIMER1_A1_VECTOR        (54)                     /* 0xFFF2 Timer1_A3 CC1-2, TA */
#define TIMER1_A0_VECTOR        (55)                     /* 0xFFF4 Timer1_A3 CC0 */
#define TIMER0_A1_VECTOR        (56)                     /* 0xFFF6 Timer0_A3 CC1-2, TA */
#define TIMER0_A0_VECTOR        (57)                     /* 0xFFE8 Timer0_A3 CC0 */
#define UNMI_VECTOR             (58)                     /* 0xFFFA User Non-maskable */
#define SYSNMI_VECTOR           (59)                     /* 0xFFFC System Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __MSP430FR4133 */


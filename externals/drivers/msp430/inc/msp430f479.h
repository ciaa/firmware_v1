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
* MSP430x47x devices.
*
* Texas Instruments, Version 1.1
*
* Rev. 1.0,  First Release
* Rev. 1.1, Corrected OSCCAP settings
*
********************************************************************/

#ifndef __msp430x47x
#define __msp430x47x

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
* PERIPHERAL FILE MAP
************************************************************/

/************************************************************
* SPECIAL FUNCTION REGISTER ADDRESSES + CONTROL BITS
************************************************************/

#define IE1_                  0x0000    /* Interrupt Enable 1 */
sfrb(IE1, IE1_);
#define WDTIE               (0x01)
#define OFIE                (0x02)
#define NMIIE               (0x10)
#define ACCVIE              (0x20)

#define IFG1_                 0x0002    /* Interrupt Flag 1 */
sfrb(IFG1, IFG1_);
#define WDTIFG              (0x01)
#define OFIFG               (0x02)
#define PORIFG              (0x04)
#define RSTIFG              (0x08)
#define NMIIFG              (0x10)

#define IE2_                  0x0001    /* Interrupt Enable 2 */
sfrb(IE2, IE2_);
#define U1IE                IE2       /* UART1 Interrupt Enable Register */
#define UC0IE               IE2
#define UCA0RXIE            (0x01)
#define UCA0TXIE            (0x02)
#define UCB0RXIE            (0x04)
#define UCB0TXIE            (0x08)
#define BTIE                (0x80)

#define IFG2_                 0x0003    /* Interrupt Flag 2 */
sfrb(IFG2, IFG2_);
#define U1IFG               IFG2     /* UART1 Interrupt Flag Register */
#define UC0IFG              IFG2
#define UCA0RXIFG           (0x01)
#define UCA0TXIFG           (0x02)
#define UCB0RXIFG           (0x04)
#define UCB0TXIFG           (0x08)
#define BTIFG               (0x80)


/************************************************************
* BASIC TIMER with Real Time Clock
************************************************************/
#define __MSP430_HAS_BT_RTC__          /* Definition to show that Module is available */

#define BTCTL_                0x0040    /* Basic Timer Control */
sfrb(BTCTL, BTCTL_);
#define RTCCTL_               0x0041    /* Real Time Clock Control */
sfrb(RTCCTL, RTCCTL_);
#define RTCNT1_               0x0042    /* Real Time Counter 1 */
sfrb(RTCNT1, RTCNT1_);
#define RTCNT2_               0x0043    /* Real Time Counter 2 */
sfrb(RTCNT2, RTCNT2_);
#define RTCNT3_               0x0044    /* Real Time Counter 3 */
sfrb(RTCNT3, RTCNT3_);
#define RTCNT4_               0x0045    /* Real Time Counter 4 */
sfrb(RTCNT4, RTCNT4_);
#define BTCNT1_               0x0046    /* Basic Timer Count 1 */
sfrb(BTCNT1, BTCNT1_);
#define BTCNT2_               0x0047    /* Basic Timer Count 2 */
sfrb(BTCNT2, BTCNT2_);
#define RTCDAY_               0x004C    /* Real Time Clock Day */
sfrb(RTCDAY, RTCDAY_);
#define RTCMON_               0x004D    /* Real Time Clock Month */
sfrb(RTCMON, RTCMON_);
#define RTCYEARL_             0x004E    /* Real Time Clock Year (Low Byte) */
sfrb(RTCYEARL, RTCYEARL_);
#define RTCYEARH_             0x004F    /* Real Time Clock Year (High Byte) */
sfrb(RTCYEARH, RTCYEARH_);
#define RTCSEC              RTCNT1
#define RTCMIN              RTCNT2
#define RTCHOUR             RTCNT3
#define RTCDOW              RTCNT4

#define RTCTL_                0x0040    /* Basic/Real Timer Control */
sfrw(RTCTL, RTCTL_);
#define RTCTIM0_              0x0042    /* Real Time Clock Time 0 */
sfrw(RTCTIM0, RTCTIM0_);
#define RTCTIM1_              0x0044    /* Real Time Clock Time 1 */
sfrw(RTCTIM1, RTCTIM1_);
#define BTCNT12_              0x0046    /* Basic Timer Count 1/2 */
sfrw(BTCNT12, BTCNT12_);
#define RTCDATE_              0x004C    /* Real Time Clock Date */
sfrw(RTCDATE, RTCDATE_);
#define RTCYEAR_              0x004E    /* Real Time Clock Year */
sfrw(RTCYEAR, RTCYEAR_);
#define RTCNT12             RTCTIM0
#define RTCNT34             RTCTIM1

#define BTSSEL              (0x80)                     /* fBT = fMCLK (main clock) */
#define BTHOLD              (0x40)                     /* BT1 is held if this bit is set */
#define BTDIV               (0x20)                     /* fCLK2 = ACLK:256 */
//#define res               (0x10)
//#define res               (0x08)
#define BTIP2               (0x04)
#define BTIP1               (0x02)
#define BTIP0               (0x01)

#define RTCBCD              (0x80)                     /* RTC BCD Select */
#define RTCHOLD             (0x40)                     /* RTC Hold */
#define RTCMODE1            (0x20)                     /* RTC Mode 1 */
#define RTCMODE0            (0x10)                     /* RTC Mode 0 */
#define RTCTEV1             (0x08)                     /* RTC Time Event 1 */
#define RTCTEV0             (0x04)                     /* RTC Time Event 0 */
#define RTCIE               (0x02)                     /* RTC Interrupt Enable */
#define RTCFG               (0x01)                     /* RTC Event Flag */

#define RTCTEV_0            (0x00)                     /* RTC Time Event: 0 */
#define RTCTEV_1            (0x04)                     /* RTC Time Event: 1 */
#define RTCTEV_2            (0x08)                     /* RTC Time Event: 2 */
#define RTCTEV_3            (0x0C)                     /* RTC Time Event: 3 */
#define RTCMODE_0           (0x00)                     /* RTC Mode: 0 */
#define RTCMODE_1           (0x10)                     /* RTC Mode: 1 */
#define RTCMODE_2           (0x20)                     /* RTC Mode: 2 */
#define RTCMODE_3           (0x30)                     /* RTC Mode: 3 */


/* Frequency of the BTCNT2 coded with Bit 5 and 7 in BTCTL */
#define BT_fCLK2_ACLK               (0x00)
#define BT_fCLK2_ACLK_DIV256        (BTDIV)
#define BT_fCLK2_MCLK               (BTSSEL)

/* Interrupt interval time fINT coded with Bits 0-2 in BTCTL */
#define BT_fCLK2_DIV2       (0x00)                    /* fINT = fCLK2:2 (default) */
#define BT_fCLK2_DIV4       (BTIP0)                   /* fINT = fCLK2:4 */
#define BT_fCLK2_DIV8       (BTIP1)                   /* fINT = fCLK2:8 */
#define BT_fCLK2_DIV16      (BTIP1+BTIP0)             /* fINT = fCLK2:16 */
#define BT_fCLK2_DIV32      (BTIP2)                   /* fINT = fCLK2:32 */
#define BT_fCLK2_DIV64      (BTIP2+BTIP0)             /* fINT = fCLK2:64 */
#define BT_fCLK2_DIV128     (BTIP2+BTIP1)             /* fINT = fCLK2:128 */
#define BT_fCLK2_DIV256     (BTIP2+BTIP1+BTIP0)       /* fINT = fCLK2:256 */

/* with assumed vlues of fACLK=32KHz, fMCLK=1MHz */
/* fBT=fACLK is thought for longer interval times */
#define BT_ADLY_0_064       (0x00)                    /* 0.064ms interval (default) */
#define BT_ADLY_0_125       (BTIP0)                   /* 0.125ms    " */
#define BT_ADLY_0_25        (BTIP1)                   /* 0.25ms     " */
#define BT_ADLY_0_5         (BTIP1+BTIP0)             /* 0.5ms      " */
#define BT_ADLY_1           (BTIP2)                   /* 1ms        " */
#define BT_ADLY_2           (BTIP2+BTIP0)             /* 2ms        " */
#define BT_ADLY_4           (BTIP2+BTIP1)             /* 4ms        " */
#define BT_ADLY_8           (BTIP2+BTIP1+BTIP0)       /* 8ms        " */
#define BT_ADLY_16          (BTDIV)                   /* 16ms       " */
#define BT_ADLY_32          (BTDIV+BTIP0)             /* 32ms       " */
#define BT_ADLY_64          (BTDIV+BTIP1)             /* 64ms       " */
#define BT_ADLY_125         (BTDIV+BTIP1+BTIP0)       /* 125ms      " */
#define BT_ADLY_250         (BTDIV+BTIP2)             /* 250ms      " */
#define BT_ADLY_500         (BTDIV+BTIP2+BTIP0)       /* 500ms      " */
#define BT_ADLY_1000        (BTDIV+BTIP2+BTIP1)       /* 1000ms     " */
#define BT_ADLY_2000        (BTDIV+BTIP2+BTIP1+BTIP0) /* 2000ms     " */
/* fCLK2=fMCLK (1MHz) is thought for short interval times */
/* the timing for short intervals is more precise than ACLK */
/* NOTE */
/* Be sure that the SCFQCTL-Register is set to 01Fh so that fMCLK=1MHz */
/* Too low interval time results in interrupts too frequent for the processor to handle! */
#define BT_MDLY_0_002       (BTSSEL)                  /* 0.002ms interval       *** interval times */
#define BT_MDLY_0_004       (BTSSEL+BTIP0)            /* 0.004ms    "           *** too short for */
#define BT_MDLY_0_008       (BTSSEL+BTIP1)            /* 0.008ms    "           *** interrupt */
#define BT_MDLY_0_016       (BTSSEL+BTIP1+BTIP0)      /* 0.016ms    "           *** handling */
#define BT_MDLY_0_032       (BTSSEL+BTIP2)            /* 0.032ms    " */
#define BT_MDLY_0_064       (BTSSEL+BTIP2+BTIP0)      /* 0.064ms    " */
#define BT_MDLY_0_125       (BTSSEL+BTIP2+BTIP1)      /* 0.125ms    " */
#define BT_MDLY_0_25        (BTSSEL+BTIP2+BTIP1+BTIP0)/* 0.25ms     " */

/* Hold coded with Bits 6-7 in BT(1)CTL */
/* this is for BT */
#define BTHOLD_CNT1         (BTHOLD)            /* BTCNT1 is held while BTHOLD is set */
#define BTHOLD_CNT1_2       (BTHOLD+BTDIV)      /* BT1CNT1 .AND. BT1CNT2 are held while ~ is set */

/* INTERRUPT CONTROL BITS */
/* #define BTIE                0x80 */
/* #define BTIFG               0x80 */

/************************************************************
* Comparator A
************************************************************/
#define __MSP430_HAS_COMPA__          /* Definition to show that Module is available */

#define CACTL1_               0x0059    /* Comparator A Control 1 */
sfrb(CACTL1, CACTL1_);
#define CACTL2_               0x005A    /* Comparator A Control 2 */
sfrb(CACTL2, CACTL2_);
#define CAPD_                 0x005B    /* Comparator A Port Disable */
sfrb(CAPD, CAPD_);

#define CAIFG               (0x01)    /* Comp. A Interrupt Flag */
#define CAIE                (0x02)    /* Comp. A Interrupt Enable */
#define CAIES               (0x04)    /* Comp. A Int. Edge Select: 0:rising / 1:falling */
#define CAON                (0x08)    /* Comp. A enable */
#define CAREF0              (0x10)    /* Comp. A Internal Reference Select 0 */
#define CAREF1              (0x20)    /* Comp. A Internal Reference Select 1 */
#define CARSEL              (0x40)    /* Comp. A Internal Reference Enable */
#define CAEX                (0x80)    /* Comp. A Exchange Inputs */

#define CAREF_0             (0x00)    /* Comp. A Int. Ref. Select 0 : Off */
#define CAREF_1             (0x10)    /* Comp. A Int. Ref. Select 1 : 0.25*Vcc */
#define CAREF_2             (0x20)    /* Comp. A Int. Ref. Select 2 : 0.5*Vcc */
#define CAREF_3             (0x30)    /* Comp. A Int. Ref. Select 3 : Vt*/

#define CAOUT               (0x01)    /* Comp. A Output */
#define CAF                 (0x02)    /* Comp. A Enable Output Filter */
#define P2CA0               (0x04)    /* Comp. A Connect External Signal to CA0 : 1 */
#define P2CA1               (0x08)    /* Comp. A Connect External Signal to CA1 : 1 */
#define CACTL24             (0x10)
#define CACTL25             (0x20)
#define CACTL26             (0x40)
#define CACTL27             (0x80)

#define CAPD0               (0x01)    /* Comp. A Disable Input Buffer of Port Register .0 */
#define CAPD1               (0x02)    /* Comp. A Disable Input Buffer of Port Register .1 */
#define CAPD2               (0x04)    /* Comp. A Disable Input Buffer of Port Register .2 */
#define CAPD3               (0x08)    /* Comp. A Disable Input Buffer of Port Register .3 */
#define CAPD4               (0x10)    /* Comp. A Disable Input Buffer of Port Register .4 */
#define CAPD5               (0x20)    /* Comp. A Disable Input Buffer of Port Register .5 */
#define CAPD6               (0x40)    /* Comp. A Disable Input Buffer of Port Register .6 */
#define CAPD7               (0x80)    /* Comp. A Disable Input Buffer of Port Register .7 */

/************************************************************
* DAC12
************************************************************/
#define __MSP430_HAS_DAC12_2__          /* Definition to show that Module is available */

#define DAC12_0CTL_           0x01c0    /* DAC12_0 Control */
sfrw(DAC12_0CTL, DAC12_0CTL_);
#define DAC12_1CTL_           0x01c2    /* DAC12_1 Control */
sfrw(DAC12_1CTL, DAC12_1CTL_);

#define DAC12GRP            (0x0001)    /* DAC12 group */
#define DAC12ENC            (0x0002)    /* DAC12 enable conversion */
#define DAC12IFG            (0x0004)    /* DAC12 interrupt flag */
#define DAC12IE             (0x0008)    /* DAC12 interrupt enable */
#define DAC12DF             (0x0010)    /* DAC12 data format */
#define DAC12AMP0           (0x0020)    /* DAC12 amplifier bit 0 */
#define DAC12AMP1           (0x0040)    /* DAC12 amplifier bit 1 */
#define DAC12AMP2           (0x0080)    /* DAC12 amplifier bit 2 */
#define DAC12IR             (0x0100)    /* DAC12 input reference and output range */
#define DAC12CALON          (0x0200)    /* DAC12 calibration */
#define DAC12LSEL0          (0x0400)    /* DAC12 load select bit 0 */
#define DAC12LSEL1          (0x0800)    /* DAC12 load select bit 1 */
#define DAC12RES            (0x1000)    /* DAC12 resolution */
#define DAC12SREF0          (0x2000)    /* DAC12 reference bit 0 */
#define DAC12SREF1          (0x4000)    /* DAC12 reference bit 1 */
#define DAC12OPS            (0x8000)    /* DAC12 Operation Amp. */

#define DAC12AMP_0          (0x0000)  /* DAC12 amplifier 0: off,    3-state */
#define DAC12AMP_1          (0x0020)  /* DAC12 amplifier 1: off,    off */
#define DAC12AMP_2          (0x0040)  /* DAC12 amplifier 2: low,    low */
#define DAC12AMP_3          (0x0060)  /* DAC12 amplifier 3: low,    medium */
#define DAC12AMP_4          (0x0080)  /* DAC12 amplifier 4: low,    high */
#define DAC12AMP_5          (0x00A0)  /* DAC12 amplifier 5: medium, medium */
#define DAC12AMP_6          (0x00C0)  /* DAC12 amplifier 6: medium, high */
#define DAC12AMP_7          (0x00E0)  /* DAC12 amplifier 7: high,   high */

#define DAC12LSEL_0         (0x0000)  /* DAC12 load select 0: direct */
#define DAC12LSEL_1         (0x0400)  /* DAC12 load select 1: latched with DAT */
#define DAC12LSEL_2         (0x0800)  /* DAC12 load select 2: latched with pos. Timer_A3.OUT1 */
#define DAC12LSEL_3         (0x0C00)  /* DAC12 load select 3: latched with pos. Timer_B7.OUT1 */

#define DAC12SREF_0         (0x0000)  /* DAC12 reference 0: Vref+ */
#define DAC12SREF_1         (0x2000)  /* DAC12 reference 1: Vref+ */
#define DAC12SREF_2         (0x4000)  /* DAC12 reference 2: Veref+ */
#define DAC12SREF_3         (0x6000)  /* DAC12 reference 3: Veref+ */

#define DAC12_0DAT_           0x01c8    /* DAC12_0 Data */
sfrw(DAC12_0DAT, DAC12_0DAT_);
#define DAC12_1DAT_           0x01ca    /* DAC12_1 Data */
sfrw(DAC12_1DAT, DAC12_1DAT_);
/*************************************************************
* Flash Memory
*************************************************************/
#define __MSP430_HAS_FLASH2__         /* Definition to show that Module is available */

#define FCTL1_                0x0128    /* FLASH Control 1 */
sfrw(FCTL1, FCTL1_);
#define FCTL2_                0x012A    /* FLASH Control 2 */
sfrw(FCTL2, FCTL2_);
#define FCTL3_                0x012C    /* FLASH Control 3 */
sfrw(FCTL3, FCTL3_);
#define FCTL4_                0x01BE    /* FLASH Control 4 */
sfrw(FCTL4, FCTL4_);

#define FRKEY               (0x9600)  /* Flash key returned by read */
#define FWKEY               (0xA500)  /* Flash key for write */
#define FXKEY               (0x3300)  /* for use with XOR instruction */

#define ERASE               (0x0002)  /* Enable bit for Flash segment erase */
#define MERAS               (0x0004)  /* Enable bit for Flash mass erase */
#define EEI                 (0x0008)  /* Enable Erase Interrupts */
#define EEIEX               (0x0010)  /* Enable Emergency Interrupt Exit */
#define WRT                 (0x0040)  /* Enable bit for Flash write */
#define BLKWRT              (0x0080)  /* Enable bit for Flash segment write */
#define SEGWRT              (0x0080)  /* old definition */ /* Enable bit for Flash segment write */

#define FN0                 (0x0001)  /* Divide Flash clock by 1 to 64 using FN0 to FN5 according to: */
#define FN1                 (0x0002)  /*  32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1 */
#ifndef FN2
#define FN2                 (0x0004)
#endif
#ifndef FN3
#define FN3                 (0x0008)
#endif
#ifndef FN4
#define FN4                 (0x0010)
#endif
#define FN5                 (0x0020)
#define FSSEL0              (0x0040)  /* Flash clock select 0 */        /* to distinguish from USART SSELx */
#define FSSEL1              (0x0080)  /* Flash clock select 1 */

#define FSSEL_0             (0x0000)  /* Flash clock select: 0 - ACLK */
#define FSSEL_1             (0x0040)  /* Flash clock select: 1 - MCLK */
#define FSSEL_2             (0x0080)  /* Flash clock select: 2 - SMCLK */
#define FSSEL_3             (0x00C0)  /* Flash clock select: 3 - SMCLK */

#define BUSY                (0x0001)  /* Flash busy: 1 */
#define KEYV                (0x0002)  /* Flash Key violation flag */
#define ACCVIFG             (0x0004)  /* Flash Access violation flag */
#define WAIT                (0x0008)  /* Wait flag for segment write */
#define LOCK                (0x0010)  /* Lock bit: 1 - Flash is locked (read only) */
#define EMEX                (0x0020)  /* Flash Emergency Exit */
#define LOCKA               (0x0040)  /* Segment A Lock bit: read = 1 - Segment is locked (read only) */
#define FAIL                (0x0080)  /* Last Program or Erase failed */

#define MGR0                (0x0010)  /* Marginal read 0 mode. */
#define MGR1                (0x0020)  /* Marginal read 1 mode. */

/************************************************************
* SYSTEM CLOCK, FLL+
************************************************************/
#define __MSP430_HAS_FLLPLUS__        /* Definition to show that Module is available */

#define SCFI0_                0x0050    /* System Clock Frequency Integrator 0 */
sfrb(SCFI0, SCFI0_);
#define FN_2                (0x04)    /* fDCOCLK =   1.4-12MHz*/
#define FN_3                (0x08)    /* fDCOCLK =   2.2-17Mhz*/
#define FN_4                (0x10)    /* fDCOCLK =   3.2-25Mhz*/
#define FN_8                (0x20)    /* fDCOCLK =     5-40Mhz*/
#define FLLD0               (0x40)    /* Loop Divider Bit : 0 */
#define FLLD1               (0x80)    /* Loop Divider Bit : 1 */

#define FLLD_1              (0x00)    /* Multiply Selected Loop Freq. By 1 */
#define FLLD_2              (0x40)    /* Multiply Selected Loop Freq. By 2 */
#define FLLD_4              (0x80)    /* Multiply Selected Loop Freq. By 4 */
#define FLLD_8              (0xC0)    /* Multiply Selected Loop Freq. By 8 */

#define SCFI1_                0x0051    /* System Clock Frequency Integrator 1 */
sfrb(SCFI1, SCFI1_);
#define SCFQCTL_              0x0052    /* System Clock Frequency Control */
sfrb(SCFQCTL, SCFQCTL_);
/* System clock frequency values fMCLK coded with Bits 0-6 in SCFQCTL */
/* #define SCFQ_32K            0x00                        fMCLK=1*fACLK       only a range from */
#define SCFQ_64K            (0x01)                     /* fMCLK=2*fACLK          1+1 to 127+1 is possible */
#define SCFQ_128K           (0x03)                     /* fMCLK=4*fACLK */
#define SCFQ_256K           (0x07)                     /* fMCLK=8*fACLK */
#define SCFQ_512K           (0x0F)                     /* fMCLK=16*fACLK */
#define SCFQ_1M             (0x1F)                     /* fMCLK=32*fACLK */
#define SCFQ_2M             (0x3F)                     /* fMCLK=64*fACLK */
#define SCFQ_4M             (0x7F)                     /* fMCLK=128*fACLK */
#define SCFQ_M              (0x80)                     /* Modulation Disable */

#define FLL_CTL0_             0x0053    /* FLL+ Control 0 */
sfrb(FLL_CTL0, FLL_CTL0_);
#define DCOF                (0x01)                     /* DCO Fault Flag */
#define LFOF                (0x02)                     /* Low Frequency Oscillator Fault Flag */
#define XT1OF               (0x04)                     /* High Frequency Oscillator 1 Fault Flag */
#define XT2OF               (0x08)                     /* High Frequency Oscillator 2 Fault Flag */
#define OSCCAP0             (0x10)                     /* XIN/XOUT Cap 0 */
#define OSCCAP1             (0x20)                     /* XIN/XOUT Cap 1 */
#define XTS_FLL             (0x40)                     /* 1: Selects high-freq. oscillator */
#define DCOPLUS             (0x80)                     /* DCO+ Enable */

#define XCAP0PF             (0x00)                     /* XIN Cap = XOUT Cap = 0pf */
#define XCAP5_5F            (0x10)                     /* XIN Cap = XOUT Cap = 5.5pf */
#define XCAP8_5PF           (0x20)                     /* XIN Cap = XOUT Cap = 8.5pf */
#define XCAP11PF            (0x30)                     /* XIN Cap = XOUT Cap = 11pf */
#define OSCCAP_0            (0x00)                     /* XIN Cap = XOUT Cap: 0 */
#define OSCCAP_1            (0x10)                     /* XIN Cap = XOUT Cap: 1 */
#define OSCCAP_2            (0x20)                     /* XIN Cap = XOUT Cap: 2 */
#define OSCCAP_3            (0x30)                     /* XIN Cap = XOUT Cap: 3 */

#define FLL_CTL1_             0x0054    /* FLL+ Control 1 */
sfrb(FLL_CTL1, FLL_CTL1_);
#define FLL_DIV0            (0x01)                     /* FLL+ Divide Px.x/ACLK 0 */
#define FLL_DIV1            (0x02)                     /* FLL+ Divide Px.x/ACLK 1 */
#define SELS                (0x04)                     /* Peripheral Module Clock Source (0: DCO, 1: XT2) */
#define SELM0               (0x08)                     /* MCLK Source Select 0 */
#define SELM1               (0x10)                     /* MCLK Source Select 1 */
#define XT2OFF              (0x20)                     /* High Frequency Oscillator 2 (XT2) disable */
#define SMCLKOFF            (0x40)                     /* Peripheral Module Clock (SMCLK) disable */
#define LFXT1DIG            (0x80)                     /* Enable Digital input for LF clock */

#define FLL_DIV_1           (0x00)                     /* FLL+ Divide Px.x/ACLK By 1 */
#define FLL_DIV_2           (0x01)                     /* FLL+ Divide Px.x/ACLK By 2 */
#define FLL_DIV_4           (0x02)                     /* FLL+ Divide Px.x/ACLK By 4 */
#define FLL_DIV_8           (0x03)                     /* FLL+ Divide Px.x/ACLK By 8 */

#define SELM_DCO            (0x00)                     /* Select DCO for CPU MCLK */
#define SELM_XT2            (0x10)                     /* Select XT2 for CPU MCLK */
#define SELM_A              (0x18)                     /* Select A (from LFXT1) for CPU MCLK */

/* INTERRUPT CONTROL BITS */
/* These two bits are defined in the Special Function Registers */
/* #define OFIFG               0x02 */
/* #define OFIE                0x02 */

/************************************************************
* LCD_A
************************************************************/
#define __MSP430_HAS_LCD_A__          /* Definition to show that Module is available */

#define LCDACTL_              0x0090    /* LCD_A Control Register */
sfrb(LCDACTL, LCDACTL_);
#define LCDON               (0x01)
#define LCDSON              (0x04)
#define LCDMX0              (0x08)
#define LCDMX1              (0x10)
#define LCDFREQ0            (0x20)
#define LCDFREQ1            (0x40)
#define LCDFREQ2            (0x80)
/* Display modes coded with Bits 2-4 */
#define LCDSTATIC           (LCDSON)
#define LCD2MUX             (LCDMX0+LCDSON)
#define LCD3MUX             (LCDMX1+LCDSON)
#define LCD4MUX             (LCDMX1+LCDMX0+LCDSON)
/* Frequency select code with Bits 5-7 */
#define LCDFREQ_32          (0x00)    /* LCD Freq: ACLK divided by 32 */
#define LCDFREQ_64          (0x20)    /* LCD Freq: ACLK divided by 64 */
#define LCDFREQ_96          (0x40)    /* LCD Freq: ACLK divided by 96 */
#define LCDFREQ_128         (0x60)    /* LCD Freq: ACLK divided by 128 */
#define LCDFREQ_192         (0x80)    /* LCD Freq: ACLK divided by 192 */
#define LCDFREQ_256         (0xA0)    /* LCD Freq: ACLK divided by 256 */
#define LCDFREQ_384         (0xC0)    /* LCD Freq: ACLK divided by 384 */
#define LCDFREQ_512         (0xE0)    /* LCD Freq: ACLK divided by 512 */

#define LCDAPCTL0_            0x00AC    /* LCD_A Port Control Register 0 */
sfrb(LCDAPCTL0, LCDAPCTL0_);
#define LCDS0               (0x01)    /* LCD Segment  0 to  3 Enable. */
#define LCDS4               (0x02)    /* LCD Segment  4 to  7 Enable. */
#define LCDS8               (0x04)    /* LCD Segment  8 to 11 Enable. */
#define LCDS12              (0x08)    /* LCD Segment 12 to 15 Enable. */
#define LCDS16              (0x10)    /* LCD Segment 16 to 19 Enable. */
#define LCDS20              (0x20)    /* LCD Segment 20 to 23 Enable. */
#define LCDS24              (0x40)    /* LCD Segment 24 to 27 Enable. */
#define LCDS28              (0x80)    /* LCD Segment 28 to 31 Enable. */

#define LCDAPCTL1_            0x00AD    /* LCD_A Port Control Register 1 */
sfrb(LCDAPCTL1, LCDAPCTL1_);
#define LCDS32              (0x01)    /* LCD Segment 32 to 35 Enable. */
#define LCDS36              (0x02)    /* LCD Segment 36 to 39 Enable. */

#define LCDAVCTL0_            0x00AE    /* LCD_A Voltage Control Register 0 */
sfrb(LCDAVCTL0, LCDAVCTL0_);
#define LCD2B               (0x01)    /* Selects 1/2 bias. */
#define VLCDREF0            (0x02)    /* Selects reference voltage for regulated charge pump: 0 */
#define VLCDREF1            (0x04)    /* Selects reference voltage for regulated charge pump: 1 */
#define LCDCPEN             (0x08)    /* LCD Voltage Charge Pump Enable. */
#define VLCDEXT             (0x10)    /* Select external source for VLCD. */
#define LCDREXT             (0x20)    /* Selects external connections for LCD mid voltages. */
#define LCDR03EXT           (0x40)    /* Selects external connection for lowest LCD voltage. */

/* Reference voltage source select for the regulated charge pump */
#define VLCDREF_0           (0<<1)    /* Internal */
#define VLCDREF_1           (1<<1)    /* External */
#define VLCDREF_2           (2<<1)    /* Reserved */
#define VLCDREF_3           (3<<1)    /* Reserved */

#define LCDAVCTL1_            0x00AF    /* LCD_A Voltage Control Register 1 */
sfrb(LCDAVCTL1, LCDAVCTL1_);
#define VLCD0               (0x02)    /* VLCD select: 0 */
#define VLCD1               (0x04)    /* VLCD select: 1 */
#define VLCD2               (0x08)    /* VLCD select: 2 */
#define VLCD3               (0x10)    /* VLCD select: 3 */

/* Charge pump voltage selections */
#define VLCD_0              (0<<1)    /* Charge pump disabled */
#define VLCD_1              (1<<1)    /* VLCD = 2.60V */
#define VLCD_2              (2<<1)    /* VLCD = 2.66V */
#define VLCD_3              (3<<1)    /* VLCD = 2.72V */
#define VLCD_4              (4<<1)    /* VLCD = 2.78V */
#define VLCD_5              (5<<1)    /* VLCD = 2.84V */
#define VLCD_6              (6<<1)    /* VLCD = 2.90V */
#define VLCD_7              (7<<1)    /* VLCD = 2.96V */
#define VLCD_8              (8<<1)    /* VLCD = 3.02V */
#define VLCD_9              (9<<1)    /* VLCD = 3.08V */
#define VLCD_10             (10<<1)   /* VLCD = 3.14V */
#define VLCD_11             (11<<1)   /* VLCD = 3.20V */
#define VLCD_12             (12<<1)   /* VLCD = 3.26V */
#define VLCD_13             (13<<1)   /* VLCD = 3.32V */
#define VLCD_14             (14<<1)   /* VLCD = 3.38V */
#define VLCD_15             (15<<1)   /* VLCD = 3.44V */

#define VLCD_DISABLED       (0<<1)    /* Charge pump disabled */
#define VLCD_2_60           (1<<1)    /* VLCD = 2.60V */
#define VLCD_2_66           (2<<1)    /* VLCD = 2.66V */
#define VLCD_2_72           (3<<1)    /* VLCD = 2.72V */
#define VLCD_2_78           (4<<1)    /* VLCD = 2.78V */
#define VLCD_2_84           (5<<1)    /* VLCD = 2.84V */
#define VLCD_2_90           (6<<1)    /* VLCD = 2.90V */
#define VLCD_2_96           (7<<1)    /* VLCD = 2.96V */
#define VLCD_3_02           (8<<1)    /* VLCD = 3.02V */
#define VLCD_3_08           (9<<1)    /* VLCD = 3.08V */
#define VLCD_3_14           (10<<1)   /* VLCD = 3.14V */
#define VLCD_3_20           (11<<1)   /* VLCD = 3.20V */
#define VLCD_3_26           (12<<1)   /* VLCD = 3.26V */
#define VLCD_3_32           (13<<1)   /* VLCD = 3.32V */
#define VLCD_3_38           (14<<1)   /* VLCD = 3.38V */
#define VLCD_3_44           (15<<1)   /* VLCD = 3.44V */

#define LCDMEM_             (0x0091)  /* LCD Memory */
#ifndef __STDC__
#define LCDMEM              (LCDMEM_) /* LCD Memory (for assembler) */
#else
#define LCDMEM              ((volatile char*) LCDMEM_) /* LCD Memory (for C) */
#endif
#define LCDM1_                0x0091    /* LCD Memory 1 */
sfrb(LCDM1, LCDM1_);
#define LCDM2_                0x0092    /* LCD Memory 2 */
sfrb(LCDM2, LCDM2_);
#define LCDM3_                0x0093    /* LCD Memory 3 */
sfrb(LCDM3, LCDM3_);
#define LCDM4_                0x0094    /* LCD Memory 4 */
sfrb(LCDM4, LCDM4_);
#define LCDM5_                0x0095    /* LCD Memory 5 */
sfrb(LCDM5, LCDM5_);
#define LCDM6_                0x0096    /* LCD Memory 6 */
sfrb(LCDM6, LCDM6_);
#define LCDM7_                0x0097    /* LCD Memory 7 */
sfrb(LCDM7, LCDM7_);
#define LCDM8_                0x0098    /* LCD Memory 8 */
sfrb(LCDM8, LCDM8_);
#define LCDM9_                0x0099    /* LCD Memory 9 */
sfrb(LCDM9, LCDM9_);
#define LCDM10_               0x009A    /* LCD Memory 10 */
sfrb(LCDM10, LCDM10_);
#define LCDM11_               0x009B    /* LCD Memory 11 */
sfrb(LCDM11, LCDM11_);
#define LCDM12_               0x009C    /* LCD Memory 12 */
sfrb(LCDM12, LCDM12_);
#define LCDM13_               0x009D    /* LCD Memory 13 */
sfrb(LCDM13, LCDM13_);
#define LCDM14_               0x009E    /* LCD Memory 14 */
sfrb(LCDM14, LCDM14_);
#define LCDM15_               0x009F    /* LCD Memory 15 */
sfrb(LCDM15, LCDM15_);
#define LCDM16_               0x00A0    /* LCD Memory 16 */
sfrb(LCDM16, LCDM16_);
#define LCDM17_               0x00A1    /* LCD Memory 17 */
sfrb(LCDM17, LCDM17_);
#define LCDM18_               0x00A2    /* LCD Memory 18 */
sfrb(LCDM18, LCDM18_);
#define LCDM19_               0x00A3    /* LCD Memory 19 */
sfrb(LCDM19, LCDM19_);
#define LCDM20_               0x00A4    /* LCD Memory 20 */
sfrb(LCDM20, LCDM20_);

#define LCDMA               (LCDM10)  /* LCD Memory A */
#define LCDMB               (LCDM11)  /* LCD Memory B */
#define LCDMC               (LCDM12)  /* LCD Memory C */
#define LCDMD               (LCDM13)  /* LCD Memory D */
#define LCDME               (LCDM14)  /* LCD Memory E */
#define LCDMF               (LCDM15)  /* LCD Memory F */

/************************************************************
* DIGITAL I/O Port1/2
************************************************************/
#define __MSP430_HAS_PORT1__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT2__          /* Definition to show that Module is available */

#define P1IN_                 0x0020    /* Port 1 Input */
const_sfrb(P1IN, P1IN_);
#define P1OUT_                0x0021    /* Port 1 Output */
sfrb(P1OUT, P1OUT_);
#define P1DIR_                0x0022    /* Port 1 Direction */
sfrb(P1DIR, P1DIR_);
#define P1IFG_                0x0023    /* Port 1 Interrupt Flag */
sfrb(P1IFG, P1IFG_);
#define P1IES_                0x0024    /* Port 1 Interrupt Edge Select */
sfrb(P1IES, P1IES_);
#define P1IE_                 0x0025    /* Port 1 Interrupt Enable */
sfrb(P1IE, P1IE_);
#define P1SEL_                0x0026    /* Port 1 Selection */
sfrb(P1SEL, P1SEL_);
#define P1SEL2_               0x0057    /* Port 1 Selection 2 */
sfrb(P1SEL2, P1SEL2_);

#define P2IN_                 0x0028    /* Port 2 Input */
const_sfrb(P2IN, P2IN_);
#define P2OUT_                0x0029    /* Port 2 Output */
sfrb(P2OUT, P2OUT_);
#define P2DIR_                0x002A    /* Port 2 Direction */
sfrb(P2DIR, P2DIR_);
#define P2IFG_                0x002B    /* Port 2 Interrupt Flag */
sfrb(P2IFG, P2IFG_);
#define P2IES_                0x002C    /* Port 2 Interrupt Edge Select */
sfrb(P2IES, P2IES_);
#define P2IE_                 0x002D    /* Port 2 Interrupt Enable */
sfrb(P2IE, P2IE_);
#define P2SEL_                0x002E    /* Port 2 Selection */
sfrb(P2SEL, P2SEL_);

/************************************************************
* DIGITAL I/O Port3/4
************************************************************/
#define __MSP430_HAS_PORT3__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT4__          /* Definition to show that Module is available */

#define P3IN_                 0x0018    /* Port 3 Input */
const_sfrb(P3IN, P3IN_);
#define P3OUT_                0x0019    /* Port 3 Output */
sfrb(P3OUT, P3OUT_);
#define P3DIR_                0x001A    /* Port 3 Direction */
sfrb(P3DIR, P3DIR_);
#define P3SEL_                0x001B    /* Port 3 Selection */
sfrb(P3SEL, P3SEL_);

#define P4IN_                 0x001C    /* Port 4 Input */
const_sfrb(P4IN, P4IN_);
#define P4OUT_                0x001D    /* Port 4 Output */
sfrb(P4OUT, P4OUT_);
#define P4DIR_                0x001E    /* Port 4 Direction */
sfrb(P4DIR, P4DIR_);
#define P4SEL_                0x001F    /* Port 4 Selection */
sfrb(P4SEL, P4SEL_);

/************************************************************
* DIGITAL I/O Port5/6
************************************************************/
#define __MSP430_HAS_PORT5__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT6__          /* Definition to show that Module is available */

#define P5IN_                 0x0030    /* Port 5 Input */
const_sfrb(P5IN, P5IN_);
#define P5OUT_                0x0031    /* Port 5 Output */
sfrb(P5OUT, P5OUT_);
#define P5DIR_                0x0032    /* Port 5 Direction */
sfrb(P5DIR, P5DIR_);
#define P5SEL_                0x0033    /* Port 5 Selection */
sfrb(P5SEL, P5SEL_);

#define P6IN_                 0x0034    /* Port 6 Input */
const_sfrb(P6IN, P6IN_);
#define P6OUT_                0x0035    /* Port 6 Output */
sfrb(P6OUT, P6OUT_);
#define P6DIR_                0x0036    /* Port 6 Direction */
sfrb(P6DIR, P6DIR_);
#define P6SEL_                0x0037    /* Port 6 Selection */
sfrb(P6SEL, P6SEL_);

/************************************************************
* SD16_A1 - Sigma Delta 16 Bit
************************************************************/
#define __MSP430_HAS_SD16_A1__        /* Definition to show that Module is available */

#define SD16INCTL0_           0x00B0    /* SD16 Input Control Register Channel 0 */
sfrb(SD16INCTL0, SD16INCTL0_);
#define SD16AE_               0x00B7    /* SD16 Analog Input Enable Register */
sfrb(SD16AE, SD16AE_);
#define SD16CONF0_            0x00F7    /* SD16 Internal Configuration Register 0 */
sfrb(SD16CONF0, SD16CONF0_);
#define SD16CONF1_            0x00BF    /* SD16 Internal Configuration Register 1 */
sfrb(SD16CONF1, SD16CONF1_);
/* Please use only the recommended settings */

#define SD16CTL_              0x0100    /* Sigma Delta ADC 16 Control Register */
sfrw(SD16CTL, SD16CTL_);
#define SD16CCTL0_            0x0102    /* SD16 Channel 0 Control Register */
sfrw(SD16CCTL0, SD16CCTL0_);
#define SD16IV_               0x0110    /* SD16 Interrupt Vector Register */
sfrw(SD16IV, SD16IV_);
#define SD16MEM0_             0x0112    /* SD16 Channel 0 Conversion Memory */
sfrw(SD16MEM0, SD16MEM0_);

/* SD16AE */
#define SD16AE0             (0x0001)  /* SD16 External Input Enable 0 */
#define SD16AE1             (0x0002)  /* SD16 External Input Enable 1 */
#define SD16AE2             (0x0004)  /* SD16 External Input Enable 2 */
#define SD16AE3             (0x0008)  /* SD16 External Input Enable 3 */
#define SD16AE4             (0x0010)  /* SD16 External Input Enable 4 */
#define SD16AE5             (0x0020)  /* SD16 External Input Enable 5 */
#define SD16AE6             (0x0040)  /* SD16 External Input Enable 6 */
#define SD16AE7             (0x0080)  /* SD16 External Input Enable 7 */

/* SD16INCTLx */
#define SD16INCH0           (0x0001)  /* SD16 Input Channel select 0 */
#define SD16INCH1           (0x0002)  /* SD16 Input Channel select 1 */
#define SD16INCH2           (0x0004)  /* SD16 Input Channel select 2 */
#define SD16GAIN0           (0x0008)  /* SD16 Input Pre-Amplifier Gain Select 0 */
#define SD16GAIN1           (0x0010)  /* SD16 Input Pre-Amplifier Gain Select 1 */
#define SD16GAIN2           (0x0020)  /* SD16 Input Pre-Amplifier Gain Select 2 */
#define SD16INTDLY0         (0x0040)  /* SD16 Interrupt Delay after 1.Conversion 0 */
#define SD16INTDLY1         (0x0080)  /* SD16 Interrupt Delay after 1.Conversion 1 */

#define SD16GAIN_1          (0x0000)  /* SD16 Input Pre-Amplifier Gain Select *1  */
#define SD16GAIN_2          (0x0008)  /* SD16 Input Pre-Amplifier Gain Select *2  */
#define SD16GAIN_4          (0x0010)  /* SD16 Input Pre-Amplifier Gain Select *4  */
#define SD16GAIN_8          (0x0018)  /* SD16 Input Pre-Amplifier Gain Select *8  */
#define SD16GAIN_16         (0x0020)  /* SD16 Input Pre-Amplifier Gain Select *16 */
#define SD16GAIN_32         (0x0028)  /* SD16 Input Pre-Amplifier Gain Select *32 */

#define SD16INCH_0          (0x0000)  /* SD16 Input Channel select A0 */
#define SD16INCH_1          (0x0001)  /* SD16 Input Channel select A1 */
#define SD16INCH_2          (0x0002)  /* SD16 Input Channel select A2 */
#define SD16INCH_3          (0x0003)  /* SD16 Input Channel select A3 */
#define SD16INCH_4          (0x0004)  /* SD16 Input Channel select A4 */
#define SD16INCH_5          (0x0005)  /* SD16 Input Channel select Vcc divider */
#define SD16INCH_6          (0x0006)  /* SD16 Input Channel select Temp */
#define SD16INCH_7          (0x0007)  /* SD16 Input Channel select Offset */

#define SD16INTDLY_0        (0x0000)  /* SD16 Interrupt Delay: Int. after 4.Conversion  */
#define SD16INTDLY_1        (0x0040)  /* SD16 Interrupt Delay: Int. after 3.Conversion  */
#define SD16INTDLY_2        (0x0080)  /* SD16 Interrupt Delay: Int. after 2.Conversion  */
#define SD16INTDLY_3        (0x00C0)  /* SD16 Interrupt Delay: Int. after 1.Conversion  */

/* SD16CTL */
#define SD16OVIE            (0x0002)  /* SD16 Overflow Interupt Enable */
#define SD16REFON           (0x0004)  /* SD16 Switch internal Reference on */
#define SD16VMIDON          (0x0008)  /* SD16 Switch Vmid Buffer on */
#define SD16SSEL0           (0x0010)  /* SD16 Clock Source Select 0 */
#define SD16SSEL1           (0x0020)  /* SD16 Clock Source Select 1 */
#define SD16DIV0            (0x0040)  /* SD16 Clock Divider Select 0 */
#define SD16DIV1            (0x0080)  /* SD16 Clock Divider Select 1 */
#define SD16LP              (0x0100)  /* SD16 Low Power Mode Enable */
#define SD16XDIV0           (0x0200)  /* SD16 2.Clock Divider Select 0 */
#define SD16XDIV1           (0x0400)  /* SD16 2.Clock Divider Select 1 */
//#define SD16XDIV2           (0x0800)  /* SD16 2.Clock Divider Select 2 */

#define SD16DIV_0           (0x0000)               /* SD16 Clock Divider Select /1 */
#define SD16DIV_1           (SD16DIV0)             /* SD16 Clock Divider Select /2 */
#define SD16DIV_2           (SD16DIV1)             /* SD16 Clock Divider Select /4 */
#define SD16DIV_3           (SD16DIV0+SD16DIV1)    /* SD16 Clock Divider Select /8 */

#define SD16XDIV_0          (0x0000)               /* SD16 2.Clock Divider Select /1 */
#define SD16XDIV_1          (SD16XDIV0)            /* SD16 2.Clock Divider Select /3 */
#define SD16XDIV_2          (SD16XDIV1)            /* SD16 2.Clock Divider Select /16 */
#define SD16XDIV_3          (SD16XDIV0+SD16XDIV1)  /* SD16 2.Clock Divider Select /48 */

#define SD16SSEL_0          (0x0000)               /* SD16 Clock Source Select MCLK  */
#define SD16SSEL_1          (SD16SSEL0)            /* SD16 Clock Source Select SMCLK */
#define SD16SSEL_2          (SD16SSEL1)            /* SD16 Clock Source Select ACLK  */
#define SD16SSEL_3          (SD16SSEL0+SD16SSEL1)  /* SD16 Clock Source Select TACLK */

/* SD16CCTLx */
//#define RESERVED            (0x0001)  /* RESERVED */
#define SD16SC              (0x0002)  /* SD16 Start Conversion */
#define SD16IFG             (0x0004)  /* SD16 Channel x Interrupt Flag */
#define SD16IE              (0x0008)  /* SD16 Channel x Interrupt Enable */
#define SD16DF              (0x0010)  /* SD16 Channel x Data Format: 0:Unipolar/1:Bipolar */
#define SD16OVIFG           (0x0020)  /* SD16 Channel x Overflow Interrupt Flag */
#define SD16LSBACC          (0x0040)  /* SD16 Channel x Access LSB of ADC */
#define SD16LSBTOG          (0x0080)  /* SD16 Channel x Toggle LSB Output of ADC */
#define SD16OSR0            (0x0100)  /* SD16 Channel x OverSampling Ratio 0 */
#define SD16OSR1            (0x0200)  /* SD16 Channel x OverSampling Ratio 1 */
#define SD16SNGL            (0x0400)  /* SD16 Channel x Single Conversion On/Off */
#define SD16XOSR            (0x0800)  /* SD16 Channel x Extended OverSampling Ratio */
#define SD16UNI             (0x1000)  /* SD16 Channel x Bipolar(0) / Unipolar(1) Mode */
#define SD16BUF0            (0x2000)  /* SD16 Channel x High Impedance Input Buffer Select: 0 */
#define SD16BUF1            (0x4000)  /* SD16 Channel x High Impedance Input Buffer Select: 1 */

#define SD16OSR_1024        (SD16OSR0+SD16XOSR)     /* SD16 Channel x OverSampling Ratio 1024 */
#define SD16OSR_512         (SD16XOSR)              /* SD16 Channel x OverSampling Ratio 512 */
#define SD16OSR_256         (0x0000)                /* SD16 Channel x OverSampling Ratio 256 */
#define SD16OSR_128         (SD16OSR0)              /* SD16 Channel x OverSampling Ratio 128 */
#define SD16OSR_64          (SD16OSR1)              /* SD16 Channel x OverSampling Ratio  64 */
#define SD16OSR_32          (SD16OSR0+SD16OSR1)     /* SD16 Channel x OverSampling Ratio  32 */

#define SD16BUF_0           (0x0000)                /* SD16 High Imp. Input Buffer: Disabled */
#define SD16BUF_1           (SD16BUF0)              /* SD16 High Imp. Input Buffer: Slow */
#define SD16BUF_2           (SD16BUF1)              /* SD16 High Imp. Input Buffer: Meduim */
#define SD16BUF_3           (SD16BUF0+SD16BUF1)     /* SD16 High Imp. Input Buffer: Fast */

/* SD16IV Definitions */
#define SD16IV_NONE         (0x0000)    /* No Interrupt pending */
#define SD16IV_SD16OVIFG    (0x0002)    /* SD16OVIFG */
#define SD16IV_SD16MEM0     (0x0004)    /* SD16MEM0 SD16IFG */

/************************************************************
* Brown-Out, Supply Voltage Supervision (SVS)
************************************************************/
#define __MSP430_HAS_SVS__            /* Definition to show that Module is available */

#define SVSCTL_               0x0056    /* SVS Control */
sfrb(SVSCTL, SVSCTL_);
#define SVSFG               (0x01)    /* SVS Flag */
#define SVSOP               (0x02)    /* SVS output (read only) */
#define SVSON               (0x04)    /* Switches the SVS on/off */
#define PORON               (0x08)    /* Enable POR Generation if Low Voltage */
#define VLD0                (0x10)
#define VLD1                (0x20)
#define VLD2                (0x40)
#define VLD3                (0x80)

#define VLDON               (0x10)
#define VLDOFF              (0x00)
#define VLD_1_8V            (0x10)

/************************************************************
* Timer A3
************************************************************/
#define __MSP430_HAS_TA3__            /* Definition to show that Module is available */

#define TAIV_                 0x012E    /* Timer A Interrupt Vector Word */
const_sfrw(TAIV, TAIV_);
#define TACTL_                0x0160    /* Timer A Control */
sfrw(TACTL, TACTL_);
#define TACCTL0_              0x0162    /* Timer A Capture/Compare Control 0 */
sfrw(TACCTL0, TACCTL0_);
#define TACCTL1_              0x0164    /* Timer A Capture/Compare Control 1 */
sfrw(TACCTL1, TACCTL1_);
#define TACCTL2_              0x0166    /* Timer A Capture/Compare Control 2 */
sfrw(TACCTL2, TACCTL2_);
#define TAR_                  0x0170    /* Timer A Counter Register */
sfrw(TAR, TAR_);
#define TACCR0_               0x0172    /* Timer A Capture/Compare 0 */
sfrw(TACCR0, TACCR0_);
#define TACCR1_               0x0174    /* Timer A Capture/Compare 1 */
sfrw(TACCR1, TACCR1_);
#define TACCR2_               0x0176    /* Timer A Capture/Compare 2 */
sfrw(TACCR2, TACCR2_);

/* Alternate register names */
#define CCTL0               TACCTL0   /* Timer A Capture/Compare Control 0 */
#define CCTL1               TACCTL1   /* Timer A Capture/Compare Control 1 */
#define CCTL2               TACCTL2   /* Timer A Capture/Compare Control 2 */
#define CCR0                TACCR0    /* Timer A Capture/Compare 0 */
#define CCR1                TACCR1    /* Timer A Capture/Compare 1 */
#define CCR2                TACCR2    /* Timer A Capture/Compare 2 */
#define CCTL0_              TACCTL0_  /* Timer A Capture/Compare Control 0 */
#define CCTL1_              TACCTL1_  /* Timer A Capture/Compare Control 1 */
#define CCTL2_              TACCTL2_  /* Timer A Capture/Compare Control 2 */
#define CCR0_               TACCR0_   /* Timer A Capture/Compare 0 */
#define CCR1_               TACCR1_   /* Timer A Capture/Compare 1 */
#define CCR2_               TACCR2_   /* Timer A Capture/Compare 2 */
/* Alternate register names - 5xx style */
#define TA0IV               TAIV      /* Timer A Interrupt Vector Word */
#define TA0CTL              TACTL     /* Timer A Control */
#define TA0CCTL0            TACCTL0   /* Timer A Capture/Compare Control 0 */
#define TA0CCTL1            TACCTL1   /* Timer A Capture/Compare Control 1 */
#define TA0CCTL2            TACCTL2   /* Timer A Capture/Compare Control 2 */
#define TA0R                TAR       /* Timer A Counter Register */
#define TA0CCR0             TACCR0    /* Timer A Capture/Compare 0 */
#define TA0CCR1             TACCR1    /* Timer A Capture/Compare 1 */
#define TA0CCR2             TACCR2    /* Timer A Capture/Compare 2 */
#define TA0IV_              TAIV_     /* Timer A Interrupt Vector Word */
#define TA0CTL_             TACTL_    /* Timer A Control */
#define TA0CCTL0_           TACCTL0_  /* Timer A Capture/Compare Control 0 */
#define TA0CCTL1_           TACCTL1_  /* Timer A Capture/Compare Control 1 */
#define TA0CCTL2_           TACCTL2_  /* Timer A Capture/Compare Control 2 */
#define TA0R_               TAR_      /* Timer A Counter Register */
#define TA0CCR0_            TACCR0_   /* Timer A Capture/Compare 0 */
#define TA0CCR1_            TACCR1_   /* Timer A Capture/Compare 1 */
#define TA0CCR2_            TACCR2_   /* Timer A Capture/Compare 2 */
#define TIMER0_A1_VECTOR    TIMERA1_VECTOR /* Int. Vector: Timer A CC1-2, TA */
#define TIMER0_A0_VECTOR    TIMERA0_VECTOR /* Int. Vector: Timer A CC0 */

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
#define MC_2                (0x0020)  /* Timer A mode control: 2 - Continous up */
#define MC_3                (0x0030)  /* Timer A mode control: 3 - Up/Down */
#define ID_0                (0x0000)  /* Timer A input divider: 0 - /1 */
#define ID_1                (0x0040)  /* Timer A input divider: 1 - /2 */
#define ID_2                (0x0080)  /* Timer A input divider: 2 - /4 */
#define ID_3                (0x00C0)  /* Timer A input divider: 3 - /8 */
#define TASSEL_0            (0x0000) /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1            (0x0100) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2            (0x0200) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3            (0x0300) /* Timer A clock source select: 3 - INCLK */

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

/* TA3IV Definitions */
#define TAIV_NONE           (0x0000)    /* No Interrupt pending */
#define TAIV_TACCR1         (0x0002)    /* TACCR1_CCIFG */
#define TAIV_TACCR2         (0x0004)    /* TACCR2_CCIFG */
#define TAIV_6              (0x0006)    /* Reserved */
#define TAIV_8              (0x0008)    /* Reserved */
#define TAIV_TAIFG          (0x000A)    /* TAIFG */

/* Alternate register names - 5xx style */
#define TA0IV_NONE          (0x0000)    /* No Interrupt pending */
#define TA0IV_TACCR1        (0x0002)    /* TA0CCR1_CCIFG */
#define TA0IV_TACCR2        (0x0004)    /* TA0CCR2_CCIFG */
#define TA0IV_6             (0x0006)    /* Reserved */
#define TA0IV_8             (0x0008)    /* Reserved */
#define TA0IV_TAIFG         (0x000A)    /* TA0IFG */

/************************************************************
* Timer B3
************************************************************/
#define __MSP430_HAS_TB3__            /* Definition to show that Module is available */

#define TBIV_                 0x011E    /* Timer B Interrupt Vector Word */
const_sfrw(TBIV, TBIV_);
#define TBCTL_                0x0180    /* Timer B Control */
sfrw(TBCTL, TBCTL_);
#define TBCCTL0_              0x0182    /* Timer B Capture/Compare Control 0 */
sfrw(TBCCTL0, TBCCTL0_);
#define TBCCTL1_              0x0184    /* Timer B Capture/Compare Control 1 */
sfrw(TBCCTL1, TBCCTL1_);
#define TBCCTL2_              0x0186    /* Timer B Capture/Compare Control 2 */
sfrw(TBCCTL2, TBCCTL2_);
#define TBR_                  0x0190    /* Timer B Counter Register */
sfrw(TBR, TBR_);
#define TBCCR0_               0x0192    /* Timer B Capture/Compare 0 */
sfrw(TBCCR0, TBCCR0_);
#define TBCCR1_               0x0194    /* Timer B Capture/Compare 1 */
sfrw(TBCCR1, TBCCR1_);
#define TBCCR2_               0x0196    /* Timer B Capture/Compare 2 */
sfrw(TBCCR2, TBCCR2_);

/* Alternate register names - 5xx style */
#define TB0IV               TBIV      /* Timer B Interrupt Vector Word */
#define TB0CTL              TBCTL     /* Timer B Control */
#define TB0CCTL0            TBCCTL0   /* Timer B Capture/Compare Control 0 */
#define TB0CCTL1            TBCCTL1   /* Timer B Capture/Compare Control 1 */
#define TB0CCTL2            TBCCTL2   /* Timer B Capture/Compare Control 2 */
#define TB0R                TBR       /* Timer B Counter Register */
#define TB0CCR0             TBCCR0    /* Timer B Capture/Compare 0 */
#define TB0CCR1             TBCCR1    /* Timer B Capture/Compare 1 */
#define TB0CCR2             TBCCR2    /* Timer B Capture/Compare 2 */
#define TB0IV_              TBIV_     /* Timer B Interrupt Vector Word */
#define TB0CTL_             TBCTL_    /* Timer B Control */
#define TB0CCTL0_           TBCCTL0_  /* Timer B Capture/Compare Control 0 */
#define TB0CCTL1_           TBCCTL1_  /* Timer B Capture/Compare Control 1 */
#define TB0CCTL2_           TBCCTL2_  /* Timer B Capture/Compare Control 2 */
#define TB0R_               TBR_      /* Timer B Counter Register */
#define TB0CCR0_            TBCCR0_   /* Timer B Capture/Compare 0 */
#define TB0CCR1_            TBCCR1_   /* Timer B Capture/Compare 1 */
#define TB0CCR2_            TBCCR2_   /* Timer B Capture/Compare 2 */
#define TIMER0_B1_VECTOR    TIMERB1_VECTOR /* Int. Vector: Timer B CC1-2, TB */
#define TIMER0_B0_VECTOR    TIMERB0_VECTOR /* Int. Vector: Timer B CC0 */

#define TBCLGRP1            (0x4000)  /* Timer B Compare latch load group 1 */
#define TBCLGRP0            (0x2000)  /* Timer B Compare latch load group 0 */
#define CNTL1               (0x1000)  /* Counter lenght 1 */
#define CNTL0               (0x0800)  /* Counter lenght 0 */
#define TBSSEL1             (0x0200)  /* Clock source 1 */
#define TBSSEL0             (0x0100)  /* Clock source 0 */
#define TBCLR               (0x0004)  /* Timer B counter clear */
#define TBIE                (0x0002)  /* Timer B interrupt enable */
#define TBIFG               (0x0001)  /* Timer B interrupt flag */

#define SHR1                (0x4000)  /* Timer B Compare latch load group 1 */
#define SHR0                (0x2000)  /* Timer B Compare latch load group 0 */

#define TBSSEL_0            (0x0000)  /* Clock Source: TBCLK */
#define TBSSEL_1            (0x0100)  /* Clock Source: ACLK  */
#define TBSSEL_2            (0x0200)  /* Clock Source: SMCLK */
#define TBSSEL_3            (0x0300)  /* Clock Source: INCLK */
#define CNTL_0              (0x0000)  /* Counter lenght: 16 bit */
#define CNTL_1              (0x0800)  /* Counter lenght: 12 bit */
#define CNTL_2              (0x1000)  /* Counter lenght: 10 bit */
#define CNTL_3              (0x1800)  /* Counter lenght:  8 bit */
#define SHR_0               (0x0000)  /* Timer B Group: 0 - individually */
#define SHR_1               (0x2000)  /* Timer B Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2               (0x4000)  /* Timer B Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3               (0x6000)  /* Timer B Group: 3 - 1 group (all) */
#define TBCLGRP_0           (0x0000)  /* Timer B Group: 0 - individually */
#define TBCLGRP_1           (0x2000)  /* Timer B Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TBCLGRP_2           (0x4000)  /* Timer B Group: 2 - 2 groups (1-3, 4-6)*/
#define TBCLGRP_3           (0x6000)  /* Timer B Group: 3 - 1 group (all) */

/* Additional Timer B Control Register bits are defined in Timer A */

#define CLLD1               (0x0400)  /* Compare latch load source 1 */
#define CLLD0               (0x0200)  /* Compare latch load source 0 */

#define SLSHR1              (0x0400)  /* Compare latch load source 1 */
#define SLSHR0              (0x0200)  /* Compare latch load source 0 */

#define SLSHR_0             (0x0000)  /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1             (0x0200)  /* Compare latch load sourec : 1 - TBR counts to 0 */
#define SLSHR_2             (0x0400)  /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3             (0x0600)  /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

#define CLLD_0              (0x0000)  /* Compare latch load sourec : 0 - immediate */
#define CLLD_1              (0x0200)  /* Compare latch load sourec : 1 - TBR counts to 0 */
#define CLLD_2              (0x0400)  /* Compare latch load sourec : 2 - up/down */
#define CLLD_3              (0x0600)  /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

/* TB3IV Definitions */
#define TBIV_NONE           (0x0000)    /* No Interrupt pending */
#define TBIV_TBCCR1         (0x0002)    /* TBCCR1_CCIFG */
#define TBIV_TBCCR2         (0x0004)    /* TBCCR2_CCIFG */
#define TBIV_3              (0x0006)    /* Reserved */
#define TBIV_4              (0x0008)    /* Reserved */
#define TBIV_5              (0x000A)    /* Reserved */
#define TBIV_6              (0x000C)    /* Reserved */
#define TBIV_TBIFG          (0x000E)    /* TBIFG */

/* Alternate register names - 5xx style */
#define TB0IV_NONE          (0x0000)    /* No Interrupt pending */
#define TB0IV_TBCCR1        (0x0002)    /* TB0CCR1_CCIFG */
#define TB0IV_TBCCR2        (0x0004)    /* TB0CCR2_CCIFG */
#define TB0IV_3             (0x0006)    /* Reserved */
#define TB0IV_4             (0x0008)    /* Reserved */
#define TB0IV_5             (0x000A)    /* Reserved */
#define TB0IV_6             (0x000C)    /* Reserved */
#define TB0IV_TBIFG         (0x000E)    /* TB0IFG */

/************************************************************
* USCI
************************************************************/
#define __MSP430_HAS_USCI__           /* Definition to show that Module is available */

#define UCA0CTL0_             0x0060    /* USCI A0 Control Register 0 */
sfrb(UCA0CTL0, UCA0CTL0_);
#define UCA0CTL1_             0x0061    /* USCI A0 Control Register 1 */
sfrb(UCA0CTL1, UCA0CTL1_);
#define UCA0BR0_              0x0062    /* USCI A0 Baud Rate 0 */
sfrb(UCA0BR0, UCA0BR0_);
#define UCA0BR1_              0x0063    /* USCI A0 Baud Rate 1 */
sfrb(UCA0BR1, UCA0BR1_);
#define UCA0MCTL_             0x0064    /* USCI A0 Modulation Control */
sfrb(UCA0MCTL, UCA0MCTL_);
#define UCA0STAT_             0x0065    /* USCI A0 Status Register */
sfrb(UCA0STAT, UCA0STAT_);
#define UCA0RXBUF_            0x0066    /* USCI A0 Receive Buffer */
const_sfrb(UCA0RXBUF, UCA0RXBUF_);
#define UCA0TXBUF_            0x0067    /* USCI A0 Transmit Buffer */
sfrb(UCA0TXBUF, UCA0TXBUF_);
#define UCA0ABCTL_            0x005D    /* USCI A0 LIN Control */
sfrb(UCA0ABCTL, UCA0ABCTL_);
#define UCA0IRTCTL_           0x005E    /* USCI A0 IrDA Transmit Control */
sfrb(UCA0IRTCTL, UCA0IRTCTL_);
#define UCA0IRRCTL_           0x005F    /* USCI A0 IrDA Receive Control */
sfrb(UCA0IRRCTL, UCA0IRRCTL_);



#define UCB0CTL0_             0x0068    /* USCI B0 Control Register 0 */
sfrb(UCB0CTL0, UCB0CTL0_);
#define UCB0CTL1_             0x0069    /* USCI B0 Control Register 1 */
sfrb(UCB0CTL1, UCB0CTL1_);
#define UCB0BR0_              0x006A    /* USCI B0 Baud Rate 0 */
sfrb(UCB0BR0, UCB0BR0_);
#define UCB0BR1_              0x006B    /* USCI B0 Baud Rate 1 */
sfrb(UCB0BR1, UCB0BR1_);
#define UCB0I2CIE_            0x006C    /* USCI B0 I2C Interrupt Enable Register */
sfrb(UCB0I2CIE, UCB0I2CIE_);
#define UCB0STAT_             0x006D    /* USCI B0 Status Register */
sfrb(UCB0STAT, UCB0STAT_);
#define UCB0RXBUF_            0x006E    /* USCI B0 Receive Buffer */
const_sfrb(UCB0RXBUF, UCB0RXBUF_);
#define UCB0TXBUF_            0x006F    /* USCI B0 Transmit Buffer */
sfrb(UCB0TXBUF, UCB0TXBUF_);
#define UCB0I2COA_            0x0118    /* USCI B0 I2C Own Address */
sfrw(UCB0I2COA, UCB0I2COA_);
#define UCB0I2CSA_            0x011A    /* USCI B0 I2C Slave Address */
sfrw(UCB0I2CSA, UCB0I2CSA_);

// UART-Mode Bits
#define UCPEN               (0x80)    /* Async. Mode: Parity enable */
#define UCPAR               (0x40)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x20)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x10)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x08)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x04)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x02)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x01)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// SPI-Mode Bits
#define UCCKPH              (0x80)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x40)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x08)    /* Sync. Mode: Master Select */

// I2C-Mode Bits
#define UCA10               (0x80)    /* 10-bit Address Mode */
#define UCSLA10             (0x40)    /* 10-bit Slave Address Mode */
#define UCMM                (0x20)    /* Multi-Master Environment */
//#define res               (0x10)    /* reserved */
#define UCMODE_0            (0x00)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x02)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x04)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x06)    /* Sync. Mode: USCI Mode: 3 */

// UART-Mode Bits
#define UCSSEL1             (0x80)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x40)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x20)    /* RX Error interrupt enable */
#define UCBRKIE             (0x10)    /* Break interrupt enable */
#define UCDORM              (0x08)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x04)    /* Send next Data as Address */
#define UCTXBRK             (0x02)    /* Send next Data as Break */
#define UCSWRST             (0x01)    /* USCI Software Reset */

// SPI-Mode Bits
//#define res               (0x20)    /* reserved */
//#define res               (0x10)    /* reserved */
//#define res               (0x08)    /* reserved */
//#define res               (0x04)    /* reserved */
//#define res               (0x02)    /* reserved */

// I2C-Mode Bits
//#define res               (0x20)    /* reserved */
#define UCTR                (0x10)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x08)    /* Transmit NACK */
#define UCTXSTP             (0x04)    /* Transmit STOP */
#define UCTXSTT             (0x02)    /* Transmit START */
#define UCSSEL_0            (0x00)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x40)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x80)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0xC0)    /* USCI 0 Clock Source: 3 */

#define UCBRF3              (0x80)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2              (0x40)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1              (0x20)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0              (0x10)    /* USCI First Stage Modulation Select 0 */
#define UCBRS2              (0x08)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1              (0x04)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0              (0x02)    /* USCI Second Stage Modulation Select 0 */
#define UCOS16              (0x01)    /* USCI 16-times Oversampling enable */

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

#define UCBRS_0             (0x00)    /* USCI Second Stage Modulation: 0 */
#define UCBRS_1             (0x02)    /* USCI Second Stage Modulation: 1 */
#define UCBRS_2             (0x04)    /* USCI Second Stage Modulation: 2 */
#define UCBRS_3             (0x06)    /* USCI Second Stage Modulation: 3 */
#define UCBRS_4             (0x08)    /* USCI Second Stage Modulation: 4 */
#define UCBRS_5             (0x0A)    /* USCI Second Stage Modulation: 5 */
#define UCBRS_6             (0x0C)    /* USCI Second Stage Modulation: 6 */
#define UCBRS_7             (0x0E)    /* USCI Second Stage Modulation: 7 */

#define UCLISTEN            (0x80)    /* USCI Listen mode */
#define UCFE                (0x40)    /* USCI Frame Error Flag */
#define UCOE                (0x20)    /* USCI Overrun Error Flag */
#define UCPE                (0x10)    /* USCI Parity Error Flag */
#define UCBRK               (0x08)    /* USCI Break received */
#define UCRXERR             (0x04)    /* USCI RX Error Flag */
#define UCADDR              (0x02)    /* USCI Address received Flag */
#define UCBUSY              (0x01)    /* USCI Busy Flag */
#define UCIDLE              (0x02)    /* USCI Idle line detected Flag */

//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
//#define res               (0x20)    /* reserved */
//#define res               (0x10)    /* reserved */
#define UCNACKIE            (0x08)    /* NACK Condition interrupt enable */
#define UCSTPIE             (0x04)    /* STOP Condition interrupt enable */
#define UCSTTIE             (0x02)    /* START Condition interrupt enable */
#define UCALIE              (0x01)    /* Arbitration Lost interrupt enable */


#define UCSCLLOW            (0x40)    /* SCL low */
#define UCGC                (0x20)    /* General Call address received Flag */
#define UCBBUSY             (0x10)    /* Bus Busy Flag */
#define UCNACKIFG           (0x08)    /* NAK Condition interrupt Flag */
#define UCSTPIFG            (0x04)    /* STOP Condition interrupt Flag */
#define UCSTTIFG            (0x02)    /* START Condition interrupt Flag */
#define UCALIFG             (0x01)    /* Arbitration Lost interrupt Flag */

#define UCIRTXPL5           (0x80)    /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4           (0x40)    /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3           (0x20)    /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2           (0x10)    /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1           (0x08)    /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0           (0x04)    /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK           (0x02)    /* IRDA Transmit Pulse Clock Select */
#define UCIREN              (0x01)    /* IRDA Encoder/Decoder enable */

#define UCIRRXFL5           (0x80)    /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4           (0x40)    /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3           (0x20)    /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2           (0x10)    /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1           (0x08)    /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0           (0x04)    /* IRDA Receive Filter Length 0 */
#define UCIRRXPL            (0x02)    /* IRDA Receive Input Polarity */
#define UCIRRXFE            (0x01)    /* IRDA Receive Filter enable */

//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1            (0x20)    /* Break Sync Delimiter 1 */
#define UCDELIM0            (0x10)    /* Break Sync Delimiter 0 */
#define UCSTOE              (0x08)    /* Sync-Field Timeout error */
#define UCBTOE              (0x04)    /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN             (0x01)    /* Auto Baud Rate detect enable */

#define UCGCEN              (0x8000)  /* I2C General Call enable */
#define UCOA9               (0x0200)  /* I2C Own Address 9 */
#define UCOA8               (0x0100)  /* I2C Own Address 8 */
#define UCOA7               (0x0080)  /* I2C Own Address 7 */
#define UCOA6               (0x0040)  /* I2C Own Address 6 */
#define UCOA5               (0x0020)  /* I2C Own Address 5 */
#define UCOA4               (0x0010)  /* I2C Own Address 4 */
#define UCOA3               (0x0008)  /* I2C Own Address 3 */
#define UCOA2               (0x0004)  /* I2C Own Address 2 */
#define UCOA1               (0x0002)  /* I2C Own Address 1 */
#define UCOA0               (0x0001)  /* I2C Own Address 0 */

#define UCSA9               (0x0200)  /* I2C Slave Address 9 */
#define UCSA8               (0x0100)  /* I2C Slave Address 8 */
#define UCSA7               (0x0080)  /* I2C Slave Address 7 */
#define UCSA6               (0x0040)  /* I2C Slave Address 6 */
#define UCSA5               (0x0020)  /* I2C Slave Address 5 */
#define UCSA4               (0x0010)  /* I2C Slave Address 4 */
#define UCSA3               (0x0008)  /* I2C Slave Address 3 */
#define UCSA2               (0x0004)  /* I2C Slave Address 2 */
#define UCSA1               (0x0002)  /* I2C Slave Address 1 */
#define UCSA0               (0x0001)  /* I2C Slave Address 0 */

/************************************************************
* WATCHDOG TIMER
************************************************************/
#define __MSP430_HAS_WDT__            /* Definition to show that Module is available */

#define WDTCTL_               0x0120    /* Watchdog Timer Control */
sfrw(WDTCTL, WDTCTL_);
/* The bit names have been prefixed with "WDT" */
#define WDTIS0              (0x0001)
#define WDTIS1              (0x0002)
#define WDTSSEL             (0x0004)
#define WDTCNTCL            (0x0008)
#define WDTTMSEL            (0x0010)
#define WDTNMI              (0x0020)
#define WDTNMIES            (0x0040)
#define WDTHOLD             (0x0080)

#define WDTPW               (0x5A00)

/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS0)                  /* 8ms     " */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1)                  /* 0.5ms   " */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL)                 /* 1000ms  " */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)          /* 250ms   " */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1)          /* 16ms    " */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)   /* 1.9ms   " */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS0)                           /* 8ms     " */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS1)                           /* 0.5ms   " */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS1+WDTIS0)                    /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL)                          /* 1000ms  " */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL+WDTIS0)                   /* 250ms   " */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL+WDTIS1)                   /* 16ms    " */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)            /* 1.9ms   " */

/* INTERRUPT CONTROL */
/* These two bits are defined in the Special Function Registers */
/* #define WDTIE               0x01 */
/* #define WDTIFG              0x01 */

/************************************************************
* Interrupt Vectors (offset from 0xFFE0)
************************************************************/

#define BASICTIMER_VECTOR       ( 1)                     /* 0xFFE0 Basic Timer */
#define PORT2_VECTOR            ( 2)                     /* 0xFFE2 Port 2 */

#define DAC12_DMA_VECTOR        ( 4)                     /* 0xFFE6 DAC 12 */
#define PORT1_VECTOR            ( 5)                     /* 0xFFE8 Port 1 */
#define TIMERA1_VECTOR          ( 6)                     /* 0xFFEA Timer A CC1-2, TA */
#define TIMERA0_VECTOR          ( 7)                     /* 0xFFEC Timer A CC0 */
#define SD16A_VECTOR            ( 8)                     /* 0xFFEE ADC SD16A */
#define USCIAB0TX_VECTOR        ( 9)                     /* 0xFFF0 USCI A0/B0 Transmit */
#define USCIAB0RX_VECTOR        (10)                     /* 0xFFF2 USCI A0/B0 Receive */
#define WDT_VECTOR              (11)                     /* 0xFFF4 Watchdog Timer */
#define COMPARATORA_VECTOR      (12)                     /* 0xFFF6 Comparator A */
#define TIMERB1_VECTOR          (13)                     /* 0xFFF8 Timer B CC1-2, TB */
#define TIMERB0_VECTOR          (14)                     /* 0xFFFA Timer B CC0 */
#define NMI_VECTOR              (15)                     /* 0xFFFC Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __msp430x47x */


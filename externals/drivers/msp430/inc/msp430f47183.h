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
* MSP430x471x3 devices.
*
* Texas Instruments, Version 1.2
*
* Rev. 1.0, First Release
* Rev. 1.1, Corrected OSCCAP settings
* Rev. 1.2, Fixed Definition SD16MEMx, SD16IV
*
*
********************************************************************/

#ifndef __msp430x471x3
#define __msp430x471x3

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
#define __MSP430_HAS_MSP430X_CPU__    /* Definition to show that it has MSP430X CPU */

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/

/************************************************************
* SPECIAL FUNCTION REGISTER ADDRESSES + CONTROL BITS
************************************************************/

#define IE1_                  0x0000    /* Interrupt Enable 1 */
sfrb(IE1, IE1_);
#define WDTIE               (0x01)    /* Watchdog Interrupt Enable */
#define OFIE                (0x02)    /* Osc. Fault  Interrupt Enable */
#define NMIIE               (0x10)    /* NMI Interrupt Enable */
#define ACCVIE              (0x20)    /* Flash Access Violation Interrupt Enable */

#define IFG1_                 0x0002    /* Interrupt Flag 1 */
sfrb(IFG1, IFG1_);
#define WDTIFG              (0x01)    /* WDT Interrupt Flag */
#define OFIFG               (0x02)    /* Osc. Fault Interrupt Flag */
#define PORIFG              (0x04)    /* Power On Interrupt Flag */
#define RSTIFG              (0x08)    /* Reset Interrupt Flag */
#define NMIIFG              (0x10)    /* NMI Interrupt Flag */

#define IE2_                  0x0001    /* Interrupt Enable 2 */
sfrb(IE2, IE2_);
#define UC0IE               IE2
#define UCA0RXIE            (0x01)
#define UCA0TXIE            (0x02)
#define UCB0RXIE            (0x04)
#define UCB0TXIE            (0x08)
#define BTIE                (0x80)

#define IFG2_                 0x0003    /* Interrupt Flag 2 */
sfrb(IFG2, IFG2_);
#define UC0IFG              IFG2
#define UCA0RXIFG           (0x01)
#define UCA0TXIFG           (0x02)
#define UCB0RXIFG           (0x04)
#define UCB0TXIFG           (0x08)
#define BTIFG               (0x80)

#define UC1IE_                0x0006    /* USCI 1 Interrupt Enable */
sfrb(UC1IE, UC1IE_);
#define UCA1RXIE            (0x01)
#define UCA1TXIE            (0x02)
#define UCB1RXIE            (0x04)
#define UCB1TXIE            (0x08)

#define UC1IFG_               0x0007    /* ISCI 1 Interrupt Flags */
sfrb(UC1IFG, UC1IFG_);
#define UCA1RXIFG           (0x01)
#define UCA1TXIFG           (0x02)
#define UCB1RXIFG           (0x04)
#define UCB1TXIFG           (0x08)

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
* DMA_X
************************************************************/
#define __MSP430_HAS_DMAX_3__           /* Definition to show that Module is available */

#define DMACTL0_              0x0122    /* DMA Module Control 0 */
sfrw(DMACTL0, DMACTL0_);
#define DMA0TSEL0           (0x0001)    /* DMA channel 0 transfer select bit 0 */
#define DMA0TSEL1           (0x0002)    /* DMA channel 0 transfer select bit 1 */
#define DMA0TSEL2           (0x0004)    /* DMA channel 0 transfer select bit 2 */
#define DMA0TSEL3           (0x0008)    /* DMA channel 0 transfer select bit 3 */
#define DMA1TSEL0           (0x0010)    /* DMA channel 1 transfer select bit 0 */
#define DMA1TSEL1           (0x0020)    /* DMA channel 1 transfer select bit 1 */
#define DMA1TSEL2           (0x0040)    /* DMA channel 1 transfer select bit 2 */
#define DMA1TSEL3           (0x0080)    /* DMA channel 1 transfer select bit 3 */
#define DMA2TSEL0           (0x0100)    /* DMA channel 2 transfer select bit 0 */
#define DMA2TSEL1           (0x0200)    /* DMA channel 2 transfer select bit 1 */
#define DMA2TSEL2           (0x0400)    /* DMA channel 2 transfer select bit 2 */
#define DMA2TSEL3           (0x0800)    /* DMA channel 2 transfer select bit 3 */

#define DMA0TSEL_0          (0x0000)  /* DMA channel 0 transfer select 0:  DMA_REQ (sw)*/
#define DMA0TSEL_1          (0x0001)  /* DMA channel 0 transfer select 1:  Timer_A (TACCR2.IFG) */
#define DMA0TSEL_2          (0x0002)  /* DMA channel 0 transfer select 2:  Timer_B (TBCCR2.IFG) */
#define DMA0TSEL_3          (0x0003)  /* DMA channel 0 transfer select 3:  USCIA0 receive */
#define DMA0TSEL_4          (0x0004)  /* DMA channel 0 transfer select 4:  USCIA0 transmit */
#define DMA0TSEL_5          (0x0005)  /* DMA channel 0 transfer select 5:  Reserved */
#define DMA0TSEL_6          (0x0006)  /* DMA channel 0 transfer select 6:  SD16 (SD16IFG) */
#define DMA0TSEL_7          (0x0007)  /* DMA channel 0 transfer select 7:  Timer_A (TACCR0.IFG) */
#define DMA0TSEL_8          (0x0008)  /* DMA channel 0 transfer select 8:  Timer_B (TBCCR0.IFG) */
#define DMA0TSEL_9          (0x0009)  /* DMA channel 0 transfer select 9:  USCIA1 receive */
#define DMA0TSEL_10         (0x000A) /* DMA channel 0 transfer select 10: USCIA1 transmit */
#define DMA0TSEL_11         (0x000B) /* DMA channel 0 transfer select 11: Multiplier ready */
#define DMA0TSEL_12         (0x000C) /* DMA channel 0 transfer select 12: USCIB0 receive */
#define DMA0TSEL_13         (0x000D) /* DMA channel 0 transfer select 13: USCIB0 transmit */
#define DMA0TSEL_14         (0x000E) /* DMA channel 0 transfer select 14: previous DMA channel DMA2IFG */
#define DMA0TSEL_15         (0x000F) /* DMA channel 0 transfer select 15: ext. Trigger (DMAE0) */

#define DMA1TSEL_0          (0x0000)  /* DMA channel 1 transfer select 0:  DMA_REQ */
#define DMA1TSEL_1          (0x0010)  /* DMA channel 1 transfer select 1:  Timer_A CCRIFG.2 */
#define DMA1TSEL_2          (0x0020)  /* DMA channel 1 transfer select 2:  Timer_B CCRIFG.2 */
#define DMA1TSEL_3          (0x0030)  /* DMA channel 1 transfer select 3:  USCIA0 receive */
#define DMA1TSEL_4          (0x0040)  /* DMA channel 1 transfer select 4:  USCIA0 transmit */
#define DMA1TSEL_5          (0x0050)  /* DMA channel 1 transfer select 5:  Reserved */
#define DMA1TSEL_6          (0x0060)  /* DMA channel 1 transfer select 6:  SD16 (SD16IFG) */
#define DMA1TSEL_7          (0x0070)  /* DMA channel 1 transfer select 7:  Timer_A (TACCR0.IFG) */
#define DMA1TSEL_8          (0x0080)  /* DMA channel 1 transfer select 8:  Timer_B (TBCCR0.IFG) */
#define DMA1TSEL_9          (0x0090)  /* DMA channel 1 transfer select 9:  USCIA1 receive */
#define DMA1TSEL_10         (0x00A0) /* DMA channel 1 transfer select 10: USCIA1 transmit */
#define DMA1TSEL_11         (0x00B0) /* DMA channel 1 transfer select 11: Multiplier ready */
#define DMA1TSEL_12         (0x00C0) /* DMA channel 1 transfer select 12: USCIB0 receive */
#define DMA1TSEL_13         (0x00D0) /* DMA channel 1 transfer select 13: USCIB0 transmit */
#define DMA1TSEL_14         (0x00E0) /* DMA channel 1 transfer select 14: previous DMA channel DMA0IFG */
#define DMA1TSEL_15         (0x00F0) /* DMA channel 1 transfer select 15: ext. Trigger (DMAE0) */

#define DMA2TSEL_0          (0x0000)  /* DMA channel 2 transfer select 0:  DMA_REQ */
#define DMA2TSEL_1          (0x0100)  /* DMA channel 2 transfer select 1:  Timer_A CCRIFG.2 */
#define DMA2TSEL_2          (0x0200)  /* DMA channel 2 transfer select 2:  Timer_B CCRIFG.2 */
#define DMA2TSEL_3          (0x0300)  /* DMA channel 2 transfer select 3:  USCIA0 receive */
#define DMA2TSEL_4          (0x0400)  /* DMA channel 2 transfer select 4:  USCIA0 transmit */
#define DMA2TSEL_5          (0x0500)  /* DMA channel 2 transfer select 5:  Reserved */
#define DMA2TSEL_6          (0x0600)  /* DMA channel 2 transfer select 6:  SD16 (SD16IFG) */
#define DMA2TSEL_7          (0x0700)  /* DMA channel 2 transfer select 7:  Timer_A (TACCR0.IFG) */
#define DMA2TSEL_8          (0x0800)  /* DMA channel 2 transfer select 8:  Timer_B (TBCCR0.IFG) */
#define DMA2TSEL_9          (0x0900)  /* DMA channel 2 transfer select 9:  USCIA1 receive */
#define DMA2TSEL_10         (0x0A00) /* DMA channel 2 transfer select 10: USCIA1 transmit */
#define DMA2TSEL_11         (0x0B00) /* DMA channel 2 transfer select 11: Multiplier ready */
#define DMA2TSEL_12         (0x0C00) /* DMA channel 2 transfer select 12: USCIB0 receive */
#define DMA2TSEL_13         (0x0D00) /* DMA channel 2 transfer select 13: USCIB0 transmit */
#define DMA2TSEL_14         (0x0E00) /* DMA channel 2 transfer select 14: previous DMA channel DMA1IFG */
#define DMA2TSEL_15         (0x0F00) /* DMA channel 2 transfer select 15: ext. Trigger (DMAE0) */

#define DMACTL1_              0x0124    /* DMA Module Control 1 */
sfrw(DMACTL1, DMACTL1_);
#define ENNMI               (0x0001)    /* Enable NMI interruption of DMA */
#define ROUNDROBIN          (0x0002)    /* Round-Robin DMA channel priorities */
#define DMAONFETCH          (0x0004)    /* DMA transfer on instruction fetch */

#define DMAIV_                0x0126    /* DMA Interrupt Vector Word */
sfrw(DMAIV, DMAIV_);
#define DMA0CTL_              0x01d0    /* DMA Channel 0 Control */
sfrw(DMA0CTL, DMA0CTL_);
#define DMA1CTL_              0x01dc    /* DMA Channel 1 Control */
sfrw(DMA1CTL, DMA1CTL_);
#define DMA2CTL_              0x01e8    /* DMA Channel 2 Control */
sfrw(DMA2CTL, DMA2CTL_);

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

#define DMADT_0             (0x0000)  /* DMA transfer mode 0: single */
#define DMADT_1             (0x1000)  /* DMA transfer mode 1: block */
#define DMADT_2             (0x2000)  /* DMA transfer mode 2: interleaved */
#define DMADT_3             (0x3000)  /* DMA transfer mode 3: interleaved */
#define DMADT_4             (0x4000)  /* DMA transfer mode 4: single, repeat */
#define DMADT_5             (0x5000)  /* DMA transfer mode 5: block, repeat */
#define DMADT_6             (0x6000)  /* DMA transfer mode 6: interleaved, repeat */
#define DMADT_7             (0x7000)  /* DMA transfer mode 7: interleaved, repeat */

#define DMA0SA_               0x01d2    /* DMA Channel 0 Source Address */
sfra(DMA0SA, DMA0SA_);
#define DMA0SAL_              0x01d2    /* DMA Channel 0 Source Address */
sfrw(DMA0SAL, DMA0SAL_);
#define DMA0DA_               0x01d6    /* DMA Channel 0 Destination Address */
sfra(DMA0DA, DMA0DA_);
#define DMA0DAL_              0x01d6    /* DMA Channel 0 Destination Address */
sfrw(DMA0DAL, DMA0DAL_);
#define DMA0SZ_               0x01da    /* DMA Channel 0 Transfer Size */
sfrw(DMA0SZ, DMA0SZ_);
#define DMA1SA_               0x01de    /* DMA Channel 1 Source Address */
sfra(DMA1SA, DMA1SA_);
#define DMA1SAL_              0x01de    /* DMA Channel 1 Source Address */
sfrw(DMA1SAL, DMA1SAL_);
#define DMA1DA_               0x01e2    /* DMA Channel 1 Destination Address */
sfra(DMA1DA, DMA1DA_);
#define DMA1DAL_              0x01e2    /* DMA Channel 1 Destination Address */
sfrw(DMA1DAL, DMA1DAL_);
#define DMA1SZ_               0x01e6    /* DMA Channel 1 Transfer Size */
sfrw(DMA1SZ, DMA1SZ_);
#define DMA2SA_               0x01ea    /* DMA Channel 2 Source Address */
sfra(DMA2SA, DMA2SA_);
#define DMA2SAL_              0x01ea    /* DMA Channel 2 Source Address */
sfrw(DMA2SAL, DMA2SAL_);
#define DMA2DA_               0x01ee    /* DMA Channel 2 Destination Address */
sfra(DMA2DA, DMA2DA_);
#define DMA2DAL_              0x01ee    /* DMA Channel 2 Destination Address */
sfrw(DMA2DAL, DMA2DAL_);
#define DMA2SZ_               0x01f2    /* DMA Channel 2 Transfer Size */
sfrw(DMA2SZ, DMA2SZ_);

/* DMAIV Definitions */
#define DMAIV_NONE           (0x0000)    /* No Interrupt pending */
#define DMAIV_DMA0IFG        (0x0002)    /* DMA0IFG */
#define DMAIV_DMA1IFG        (0x0004)    /* DMA1IFG */
#define DMAIV_DMA2IFG        (0x0006)    /* DMA2IFG */

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

#define FLL_CTL2_             0x0055    /* FLL+ Control 2 */
sfrb(FLL_CTL2, FLL_CTL2_);

#define XT2S0               (0x40)   /* Mode 0 for XT2 */
#define XT2S1               (0x80)   /* Mode 1 for XT2 */

#define XT2S_0              (0x00)   /* Mode 0 for XT2 : 0.4 - 1 MHz */
#define XT2S_1              (0x40)   /* Mode 1 for XT2 : 1 - 4 MHz */
#define XT2S_2              (0x80)   /* Mode 2 for XT2 : 2 - 16 MHz */
#define XT2S_3              (0xC0)   /* Mode 3 for XT2 : Digital input signal */

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
* HARDWARE MULTIPLIER 32Bit
************************************************************/
#define __MSP430_HAS_MPY32__          /* Definition to show that Module is available */

#define MPY_B_                0x0130    /* Multiply Unsigned/Operand 1 (Byte Access) */
sfrb(MPY_B, MPY_B_);
#define MPYS_B_               0x0132    /* Multiply Signed/Operand 1 (Byte Access) */
sfrb(MPYS_B, MPYS_B_);
#define MAC_B_                0x0134    /* Multiply Unsigned and Accumulate/Operand 1 (Byte Access) */
sfrb(MAC_B, MAC_B_);
#define MACS_B_               0x0136    /* Multiply Signed and Accumulate/Operand 1 (Byte Access) */
sfrb(MACS_B, MACS_B_);
#define OP2_B_                0x0138    /* Operand 2 (Byte Access) */
sfrb(OP2_B, OP2_B_);
#define MPY_                  0x0130    /* Multiply Unsigned/Operand 1 */
sfrw(MPY, MPY_);
#define MPYS_                 0x0132    /* Multiply Signed/Operand 1 */
sfrw(MPYS, MPYS_);
#define MAC_                  0x0134    /* Multiply Unsigned and Accumulate/Operand 1 */
sfrw(MAC, MAC_);
#define MACS_                 0x0136    /* Multiply Signed and Accumulate/Operand 1 */
sfrw(MACS, MACS_);
#define OP2_                  0x0138    /* Operand 2 */
sfrw(OP2, OP2_);
#define RESLO_                0x013A    /* Result Low Word */
sfrw(RESLO, RESLO_);
#define RESHI_                0x013C    /* Result High Word */
sfrw(RESHI, RESHI_);
#define SUMEXT_               0x013E    /* Sum Extend */
const_sfrw(SUMEXT, SUMEXT_);

#define MPY32L_B_             0x0140    /* 32-bit operand 1 - multiply - low word (Byte Access) */
sfrb(MPY32L_B, MPY32L_B_);
#define MPY32H_B_             0x0142    /* 32-bit operand 1 - multiply - high word (Byte Access) */
sfrb(MPY32H_B, MPY32H_B_);
#define MPYS32L_B_            0x0144    /* 32-bit operand 1 - signed multiply - low word (Byte Access) */
sfrb(MPYS32L_B, MPYS32L_B_);
#define MPYS32H_B_            0x0146    /* 32-bit operand 1 - signed multiply - high word (Byte Access) */
sfrb(MPYS32H_B, MPYS32H_B_);
#define MAC32L_B_             0x0148    /* 32-bit operand 1 - multiply accumulate - low word (Byte Access) */
sfrb(MAC32L_B, MAC32L_B_);
#define MAC32H_B_             0x014A    /* 32-bit operand 1 - multiply accumulate - high word (Byte Access) */
sfrb(MAC32H_B, MAC32H_B_);
#define MACS32L_B_            0x014C    /* 32-bit operand 1 - signed multiply accumulate - low word (Byte Access) */
sfrb(MACS32L_B, MACS32L_B_);
#define MACS32H_B_            0x014E    /* 32-bit operand 1 - signed multiply accumulate - high word (Byte Access) */
sfrb(MACS32H_B, MACS32H_B_);
#define OP2L_B_               0x0150    /* 32-bit operand 2 - low word (Byte Access) */
sfrb(OP2L_B, OP2L_B_);
#define OP2H_B_               0x0152    /* 32-bit operand 2 - high word (Byte Access) */
sfrb(OP2H_B, OP2H_B_);
#define MPY32L_               0x0140    /* 32-bit operand 1 - multiply - low word */
sfrw(MPY32L, MPY32L_);
#define MPY32H_               0x0142    /* 32-bit operand 1 - multiply - high word */
sfrw(MPY32H, MPY32H_);
#define MPYS32L_              0x0144    /* 32-bit operand 1 - signed multiply - low word */
sfrw(MPYS32L, MPYS32L_);
#define MPYS32H_              0x0146    /* 32-bit operand 1 - signed multiply - high word */
sfrw(MPYS32H, MPYS32H_);
#define MAC32L_               0x0148    /* 32-bit operand 1 - multiply accumulate - low word */
sfrw(MAC32L, MAC32L_);
#define MAC32H_               0x014A    /* 32-bit operand 1 - multiply accumulate - high word */
sfrw(MAC32H, MAC32H_);
#define MACS32L_              0x014C    /* 32-bit operand 1 - signed multiply accumulate - low word */
sfrw(MACS32L, MACS32L_);
#define MACS32H_              0x014E    /* 32-bit operand 1 - signed multiply accumulate - high word */
sfrw(MACS32H, MACS32H_);
#define OP2L_                 0x0150    /* 32-bit operand 2 - low word */
sfrw(OP2L, OP2L_);
#define OP2H_                 0x0152    /* 32-bit operand 2 - high word */
sfrw(OP2H, OP2H_);
#define RES0_                 0x0154    /* 32x32-bit result 0 - least significant word */
sfrw(RES0, RES0_);
#define RES1_                 0x0156    /* 32x32-bit result 1 */
sfrw(RES1, RES1_);
#define RES2_                 0x0158    /* 32x32-bit result 2 */
sfrw(RES2, RES2_);
#define RES3_                 0x015A    /* 32x32-bit result 3 - most significant word */
sfrw(RES3, RES3_);
#define MPY32CTL0_            0x015C    /* MPY32 Control Register 0 */
sfrw(MPY32CTL0, MPY32CTL0_);


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
#define __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */

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
#define P1REN_                0x0027    /* Port 1 Resistor Enable */
sfrb(P1REN, P1REN_);

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
#define P2REN_                0x002F    /* Port 2 Resistor Enable */
sfrb(P2REN, P2REN_);

/************************************************************
* DIGITAL I/O Port3/4 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT3_R__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT4_R__          /* Definition to show that Module is available */

#define P3IN_                 0x0018    /* Port 3 Input */
const_sfrb(P3IN, P3IN_);
#define P3OUT_                0x0019    /* Port 3 Output */
sfrb(P3OUT, P3OUT_);
#define P3DIR_                0x001A    /* Port 3 Direction */
sfrb(P3DIR, P3DIR_);
#define P3SEL_                0x001B    /* Port 3 Selection */
sfrb(P3SEL, P3SEL_);
#define P3REN_                0x0010    /* Port 3 Resistor Enable */
sfrb(P3REN, P3REN_);

#define P4IN_                 0x001C    /* Port 4 Input */
const_sfrb(P4IN, P4IN_);
#define P4OUT_                0x001D    /* Port 4 Output */
sfrb(P4OUT, P4OUT_);
#define P4DIR_                0x001E    /* Port 4 Direction */
sfrb(P4DIR, P4DIR_);
#define P4SEL_                0x001F    /* Port 4 Selection */
sfrb(P4SEL, P4SEL_);
#define P4REN_                0x0011    /* Port 4 Resistor Enable */
sfrb(P4REN, P4REN_);

/************************************************************
* DIGITAL I/O Port5 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT5_R__          /* Definition to show that Module is available */

#define P5IN_                 0x0030    /* Port 5 Input */
const_sfrb(P5IN, P5IN_);
#define P5OUT_                0x0031    /* Port 5 Output */
sfrb(P5OUT, P5OUT_);
#define P5DIR_                0x0032    /* Port 5 Direction */
sfrb(P5DIR, P5DIR_);
#define P5SEL_                0x0033    /* Port 5 Selection */
sfrb(P5SEL, P5SEL_);
#define P5REN_                0x0012    /* Port 5 Resistor Enable */
sfrb(P5REN, P5REN_);

/************************************************************
* DIGITAL I/O Port7/8 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT7_R__        /* Definition to show that Module is available */
#define __MSP430_HAS_PORT8_R__        /* Definition to show that Module is available */
#define __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */

#define P7IN_                 0x0038    /* Port 7 Input */
const_sfrb(P7IN, P7IN_);
#define P7OUT_                0x003A    /* Port 7 Output */
sfrb(P7OUT, P7OUT_);
#define P7DIR_                0x003C    /* Port 7 Direction */
sfrb(P7DIR, P7DIR_);
#define P7SEL_                0x003E    /* Port 7 Selection */
sfrb(P7SEL, P7SEL_);
#define P7REN_                0x0014    /* Port 7 Resistor Enable */
sfrb(P7REN, P7REN_);

#define P8IN_                 0x0039    /* Port 8 Input */
const_sfrb(P8IN, P8IN_);
#define P8OUT_                0x003B    /* Port 8 Output */
sfrb(P8OUT, P8OUT_);
#define P8DIR_                0x003D    /* Port 8 Direction */
sfrb(P8DIR, P8DIR_);
#define P8SEL_                0x003F    /* Port 8 Selection */
sfrb(P8SEL, P8SEL_);
#define P8REN_                0x0015    /* Port 8 Resistor Enable */
sfrb(P8REN, P8REN_);

#define PAIN_                 0x0038    /* Port A Input */
const_sfrw(PAIN, PAIN_);
#define PAOUT_                0x003A    /* Port A Output */
sfrw(PAOUT, PAOUT_);
#define PADIR_                0x003C    /* Port A Direction */
sfrw(PADIR, PADIR_);
#define PASEL_                0x003E    /* Port A Selection */
sfrw(PASEL, PASEL_);
#define PAREN_                0x0014    /* Port A Resistor Enable */
sfrw(PAREN, PAREN_);


/************************************************************
* DIGITAL I/O Port9/10 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT9_R__        /* Definition to show that Module is available */
#define __MSP430_HAS_PORT10_R__       /* Definition to show that Module is available */
#define __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */

#define P9IN_                 0x0008    /* Port 9 Input */
const_sfrb(P9IN, P9IN_);
#define P9OUT_                0x000A    /* Port 9 Output */
sfrb(P9OUT, P9OUT_);
#define P9DIR_                0x000C    /* Port 9 Direction */
sfrb(P9DIR, P9DIR_);
#define P9SEL_                0x000E    /* Port 9 Selection */
sfrb(P9SEL, P9SEL_);
#define P9REN_                0x0016    /* Port 9 Resistor Enable */
sfrb(P9REN, P9REN_);

#define P10IN_                0x0009    /* Port 10 Input */
const_sfrb(P10IN, P10IN_);
#define P10OUT_               0x000B    /* Port 10 Output */
sfrb(P10OUT, P10OUT_);
#define P10DIR_               0x000D    /* Port 10 Direction */
sfrb(P10DIR, P10DIR_);
#define P10SEL_               0x000F    /* Port 10 Selection */
sfrb(P10SEL, P10SEL_);
#define P10REN_               0x0017    /* Port 10 Resistor Enable */
sfrb(P10REN, P10REN_);

#define PBIN_                 0x0008    /* Port B Input */
const_sfrw(PBIN, PBIN_);
#define PBOUT_                0x000A    /* Port B Output */
sfrw(PBOUT, PBOUT_);
#define PBDIR_                0x000C    /* Port B Direction */
sfrw(PBDIR, PBDIR_);
#define PBSEL_                0x000E    /* Port B Selection */
sfrw(PBSEL, PBSEL_);
#define PBREN_                0x0016    /* Port B Resistor Enable */
sfrw(PBREN, PBREN_);


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
* SD16_A3 - Sigma Delta 16 Bit
************************************************************/
#define __MSP430_HAS_SD16_A3__        /* Definition to show that Module is available */

#define SD16INCTL0_           0x00B0    /* SD16 Input Control Register Channel 0 */
sfrb(SD16INCTL0, SD16INCTL0_);
#define SD16INCTL1_           0x00B1    /* SD16 Input Control Register Channel 1 */
sfrb(SD16INCTL1, SD16INCTL1_);
#define SD16INCTL2_           0x00B2    /* SD16 Input Control Register Channel 2 */
sfrb(SD16INCTL2, SD16INCTL2_);
#define SD16PRE0_             0x00B8    /* SD16 Preload Register Channel 0 */
sfrb(SD16PRE0, SD16PRE0_);
#define SD16PRE1_             0x00B9    /* SD16 Preload Register Channel 1 */
sfrb(SD16PRE1, SD16PRE1_);
#define SD16PRE2_             0x00BA    /* SD16 Preload Register Channel 2 */
sfrb(SD16PRE2, SD16PRE2_);
//sfrb    SD16CONF0           (0x00B7)  /* SD16 Internal Configuration Register 0 */
#define SD16CONF1_            0x00BF    /* SD16 Internal Configuration Register 1 */
sfrb(SD16CONF1, SD16CONF1_);
/* Please use only the recommended settings */

#define SD16CTL_              0x0100    /* Sigma Delta ADC 16 Control Register */
sfrw(SD16CTL, SD16CTL_);
#define SD16CCTL0_            0x0102    /* SD16 Channel 0 Control Register */
sfrw(SD16CCTL0, SD16CCTL0_);
#define SD16CCTL1_            0x0104    /* SD16 Channel 1 Control Register */
sfrw(SD16CCTL1, SD16CCTL1_);
#define SD16CCTL2_            0x0106    /* SD16 Channel 2 Control Register */
sfrw(SD16CCTL2, SD16CCTL2_);
#define SD16IV_               0x01AE    /* SD16 Interrupt Vector Register */
sfrw(SD16IV, SD16IV_);
#define SD16MEM0_             0x0110    /* SD16 Channel 0 Conversion Memory */
sfrw(SD16MEM0, SD16MEM0_);
#define SD16MEM1_             0x0112    /* SD16 Channel 1 Conversion Memory */
sfrw(SD16MEM1, SD16MEM1_);
#define SD16MEM2_             0x0114    /* SD16 Channel 2 Conversion Memory */
sfrw(SD16MEM2, SD16MEM2_);

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

#define SD16INCH_0          (0x0000)  /* SD16 Input Channel select input */
#define SD16INCH_1          (0x0001)  /* SD16 Input Channel select input */
#define SD16INCH_2          (0x0002)  /* SD16 Input Channel select input */
#define SD16INCH_3          (0x0003)  /* SD16 Input Channel select input */
#define SD16INCH_4          (0x0004)  /* SD16 Input Channel select input */
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

#define SD16DIV_0            (0x0000)               /* SD16 Clock Divider Select /1 */
#define SD16DIV_1            (SD16DIV0)             /* SD16 Clock Divider Select /2 */
#define SD16DIV_2            (SD16DIV1)             /* SD16 Clock Divider Select /4 */
#define SD16DIV_3            (SD16DIV0+SD16DIV1)    /* SD16 Clock Divider Select /8 */

#define SD16XDIV_0          (0x0000)               /* SD16 2.Clock Divider Select /1 */
#define SD16XDIV_1          (SD16XDIV0)            /* SD16 2.Clock Divider Select /3 */
#define SD16XDIV_2          (SD16XDIV1)            /* SD16 2.Clock Divider Select /16 */
#define SD16XDIV_3          (SD16XDIV0+SD16XDIV1)  /* SD16 2.Clock Divider Select /48 */

#define SD16SSEL_0           (0x0000)               /* SD16 Clock Source Select MCLK  */
#define SD16SSEL_1           (SD16SSEL0)            /* SD16 Clock Source Select SMCLK */
#define SD16SSEL_2           (SD16SSEL1)            /* SD16 Clock Source Select ACLK  */
#define SD16SSEL_3           (SD16SSEL0+SD16SSEL1)  /* SD16 Clock Source Select TACLK */

/* SD16CCTLx */
#define SD16GRP             (0x0001)  /* SD16 Grouping of Channels: 0:Off/1:On */
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

#define SD16OSR_1024        (SD16OSR0+SD16XOSR)     /* SD16 Channel x OverSampling Ratio 1024 */
#define SD16OSR_512         (SD16XOSR)              /* SD16 Channel x OverSampling Ratio 512 */
#define SD16OSR_256         (0x0000)  /* SD16 Channel x OverSampling Ratio 256 */
#define SD16OSR_128         (0x0100)  /* SD16 Channel x OverSampling Ratio 128 */
#define SD16OSR_64          (0x0200)  /* SD16 Channel x OverSampling Ratio  64 */
#define SD16OSR_32          (0x0300)  /* SD16 Channel x OverSampling Ratio  32 */

/* SD16IV Definitions */
#define SD16IV_NONE         (0x0000)    /* No Interrupt pending */
#define SD16IV_SD16OVIFG    (0x0002)    /* SD16OVIFG */
#define SD16IV_SD16MEM0     (0x0004)    /* SD16MEM0 SD16IFG */
#define SD16IV_SD16MEM1     (0x0006)    /* SD16MEM1 SD16IFG */
#define SD16IV_SD16MEM2     (0x0008)    /* SD16MEM2 SD16IFG */

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
#define __MSP430_HAS_USCI_AB0__       /* Definition to show that Module is available */
#define __MSP430_HAS_USCI_AB1__       /* Definition to show that Module is available */

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
/* Note: Devices with SD16 have the next two registers on a different address */
#define UCB0I2COA_            0x016C    /* USCI B0 I2C Own Address */
sfrw(UCB0I2COA, UCB0I2COA_);
#define UCB0I2CSA_            0x016E    /* USCI B0 I2C Slave Address */
sfrw(UCB0I2CSA, UCB0I2CSA_);


#define UCA1CTL0_             0x00D0    /* USCI A1 Control Register 0 */
sfrb(UCA1CTL0, UCA1CTL0_);
#define UCA1CTL1_             0x00D1    /* USCI A1 Control Register 1 */
sfrb(UCA1CTL1, UCA1CTL1_);
#define UCA1BR0_              0x00D2    /* USCI A1 Baud Rate 0 */
sfrb(UCA1BR0, UCA1BR0_);
#define UCA1BR1_              0x00D3    /* USCI A1 Baud Rate 1 */
sfrb(UCA1BR1, UCA1BR1_);
#define UCA1MCTL_             0x00D4    /* USCI A1 Modulation Control */
sfrb(UCA1MCTL, UCA1MCTL_);
#define UCA1STAT_             0x00D5    /* USCI A1 Status Register */
sfrb(UCA1STAT, UCA1STAT_);
#define UCA1RXBUF_            0x00D6    /* USCI A1 Receive Buffer */
const_sfrb(UCA1RXBUF, UCA1RXBUF_);
#define UCA1TXBUF_            0x00D7    /* USCI A1 Transmit Buffer */
sfrb(UCA1TXBUF, UCA1TXBUF_);
#define UCA1ABCTL_            0x00CD    /* USCI A1 LIN Control */
sfrb(UCA1ABCTL, UCA1ABCTL_);
#define UCA1IRTCTL_           0x00CE    /* USCI A1 IrDA Transmit Control */
sfrb(UCA1IRTCTL, UCA1IRTCTL_);
#define UCA1IRRCTL_           0x00CF    /* USCI A1 IrDA Receive Control */
sfrb(UCA1IRRCTL, UCA1IRRCTL_);



#define UCB1CTL0_             0x00D8    /* USCI B1 Control Register 0 */
sfrb(UCB1CTL0, UCB1CTL0_);
#define UCB1CTL1_             0x00D9    /* USCI B1 Control Register 1 */
sfrb(UCB1CTL1, UCB1CTL1_);
#define UCB1BR0_              0x00DA    /* USCI B1 Baud Rate 0 */
sfrb(UCB1BR0, UCB1BR0_);
#define UCB1BR1_              0x00DB    /* USCI B1 Baud Rate 1 */
sfrb(UCB1BR1, UCB1BR1_);
#define UCB1I2CIE_            0x00DC    /* USCI B1 I2C Interrupt Enable Register */
sfrb(UCB1I2CIE, UCB1I2CIE_);
#define UCB1STAT_             0x00DD    /* USCI B1 Status Register */
sfrb(UCB1STAT, UCB1STAT_);
#define UCB1RXBUF_            0x00DE    /* USCI B1 Receive Buffer */
const_sfrb(UCB1RXBUF, UCB1RXBUF_);
#define UCB1TXBUF_            0x00DF    /* USCI B1 Transmit Buffer */
sfrb(UCB1TXBUF, UCB1TXBUF_);
#define UCB1I2COA_            0x017C    /* USCI B1 I2C Own Address */
sfrw(UCB1I2COA, UCB1I2COA_);
#define UCB1I2CSA_            0x017E    /* USCI B1 I2C Slave Address */
sfrw(UCB1I2CSA, UCB1I2CSA_);


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
* Interrupt Vectors (offset from 0xFFC0)
************************************************************/


#define DMA_VECTOR              (16)                     /* 0xFFDE DMA */
#define BASICTIMER_VECTOR       (17)                     /* 0xFFE0 Basic Timer / RTC */
#define PORT2_VECTOR            (18)                     /* 0xFFE2 Port 2 */
#define USCIAB1TX_VECTOR        (19)                     /* 0xFFE4 USCI A1/B1 Transmit */
#define USCIAB1RX_VECTOR        (20)                     /* 0xFFE6 USCI A1/B1 Receive */
#define PORT1_VECTOR            (21)                     /* 0xFFE8 Port 1 */
#define TIMERA1_VECTOR          (22)                     /* 0xFFEA Timer A CC1-2, TA */
#define TIMERA0_VECTOR          (23)                     /* 0xFFEC Timer A CC0 */
#define SD16A_VECTOR            (24)                     /* 0xFFEE ADC */
#define USCIAB0TX_VECTOR        (25)                     /* 0xFFF0 USCI A0/B0 Transmit */
#define USCIAB0RX_VECTOR        (26)                     /* 0xFFF2 USCI A0/B0 Receive */
#define WDT_VECTOR              (27)                     /* 0xFFF4 Watchdog Timer */
#define COMPARATORA_VECTOR      (28)                     /* 0xFFF6 Comparator A */
#define TIMERB1_VECTOR          (29)                     /* 0xFFF8 Timer B CC1-2, TB */
#define TIMERB0_VECTOR          (30)                     /* 0xFFFA Timer B CC0 */
#define NMI_VECTOR              (31)                     /* 0xFFFC Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __msp430x471x3 */


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

/******************************************************************************/
/* Legacy Header File                                                         */
/* Not recommended for use in new projects.                                   */
/* Please use the msp430.h file or the device specific header file            */
/******************************************************************************/

/********************************************************************
*
* Standard register and bit definitions for the Texas Instruments
* MSP430 microcontroller.
*
* This file supports assembler and C development for
* MSP430xE42x2 devices.
*
* Texas Instruments, Version 1.2
*
*
* Rev. 1.0,  Initial Version
* Rev. 1.1,
* Rev. 1.2,  Fixed swapped IRMS and IRMS_2 definitions
*
********************************************************************/

#ifndef __msp430xE42x2
#define __msp430xE42x2

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
#define U0IE                IE1       /* UART0 Interrupt Enable Register */
#define WDTIE               (0x01)
#define OFIE                (0x02)
#define NMIIE               (0x10)
#define ACCVIE              (0x20)
#define URXIE0              (0x40)
#define UTXIE0              (0x80)

#define IFG1_                 0x0002    /* Interrupt Flag 1 */
sfrb(IFG1, IFG1_);
#define U0IFG               IFG1      /* UART0 Interrupt Flag Register */
#define WDTIFG              (0x01)
#define OFIFG               (0x02)
#define NMIIFG              (0x10)
#define URXIFG0             (0x40)
#define UTXIFG0             (0x80)

#define ME1_                  0x0004    /* Module Enable 1 */
sfrb(ME1, ME1_);
#define U0ME                ME1       /* UART0 Module Enable Register */
#define URXE0               (0x40)
#define UTXE0               (0x80)
#define USPIE0              (0x40)

#define IE2_                  0x0001    /* Interrupt Enable 2 */
sfrb(IE2, IE2_);
#define BTIE                (0x80)

#define IFG2_                 0x0003    /* Interrupt Flag 2 */
sfrb(IFG2, IFG2_);
#define BTIFG               (0x80)

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
* BASIC TIMER
************************************************************/
#define __MSP430_HAS_BT__             /* Definition to show that Module is available */

#define BTCTL_                0x0040    /* Basic Timer Control */
sfrb(BTCTL, BTCTL_);
/* The bit names have been prefixed with "BT" */
#define BTIP0               (0x01)
#define BTIP1               (0x02)
#define BTIP2               (0x04)
#define BTFRFQ0             (0x08)
#define BTFRFQ1             (0x10)
#define BTDIV               (0x20)                     /* fCLK2 = ACLK:256 */
#define BTHOLD              (0x40)                     /* BT1 is held if this bit is set */
#define BTSSEL              (0x80)                     /* fBT = fMCLK (main clock) */

#define BTCNT1_               0x0046    /* Basic Timer Count 1 */
sfrb(BTCNT1, BTCNT1_);
#define BTCNT2_               0x0047    /* Basic Timer Count 2 */
sfrb(BTCNT2, BTCNT2_);

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
/* Frequency of LCD coded with Bits 3-4 */
#define BT_fLCD_DIV32       (0x00)                    /* fLCD = fACLK:32 (default) */
#define BT_fLCD_DIV64       (BTFRFQ0)                 /* fLCD = fACLK:64 */
#define BT_fLCD_DIV128      (BTFRFQ1)                 /* fLCD = fACLK:128 */
#define BT_fLCD_DIV256      (BTFRFQ1+BTFRFQ0)         /* fLCD = fACLK:256 */
/* LCD frequency values with fBT=fACLK */
#define BT_fLCD_1K          (0x00)                    /* fACLK:32 (default) */
#define BT_fLCD_512         (BTFRFQ0)                 /* fACLK:64 */
#define BT_fLCD_256         (BTFRFQ1)                 /* fACLK:128 */
#define BT_fLCD_128         (BTFRFQ1+BTFRFQ0)         /* fACLK:256 */
/* LCD frequency values with fBT=fMCLK */
#define BT_fLCD_31K         (BTSSEL)                  /* fMCLK:32 */
#define BT_fLCD_15_5K       (BTSSEL+BTFRFQ0)          /* fMCLK:64 */
#define BT_fLCD_7_8K        (BTSSEL+BTFRFQ1+BTFRFQ0)  /* fMCLK:256 */
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

/* Reset/Hold coded with Bits 6-7 in BT(1)CTL */
/* this is for BT */
//#define BTRESET_CNT1        (BTRESET)           /* BTCNT1 is reset while BTRESET is set */
//#define BTRESET_CNT1_2      (BTRESET+BTDIV)     /* BTCNT1 .AND. BTCNT2 are reset while ~ is set */
/* this is for BT1 */
#define BTHOLD_CNT1         (BTHOLD)            /* BTCNT1 is held while BTHOLD is set */
#define BTHOLD_CNT1_2       (BTHOLD+BTDIV)      /* BT1CNT1 .AND. BT1CNT2 are held while ~ is set */

/* INTERRUPT CONTROL BITS */
/* #define BTIE                0x80 */
/* #define BTIFG               0x80 */

/************************************************************
* SYSTEM CLOCK, FLL+ (x41x)
************************************************************/
#define __MSP430_HAS_FLLPLUS_SMALL__  /* Definition to show that Module is available */

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
#define XT1OF               (0x04)                     /* High Frequency Oscillator Fault Flag */
#define OSCCAP0             (0x10)                     /* XIN/XOUT Cap 0 */
#define OSCCAP1             (0x20)                     /* XIN/XOUT Cap 1 */
#define XTS_FLL             (0x40)                     /* 1: Selects high-freq. oscillator */
#define DCOPLUS             (0x80)                     /* DCO+ Enable */

#define XCAP0PF             (0x00)                     /* XIN Cap = XOUT Cap = 0pf */
#define XCAP10PF            (0x10)                     /* XIN Cap = XOUT Cap = 10pf */
#define XCAP14PF            (0x20)                     /* XIN Cap = XOUT Cap = 14pf */
#define XCAP18PF            (0x30)                     /* XIN Cap = XOUT Cap = 18pf */
#define OSCCAP_0            (0x00)                     /* XIN Cap = XOUT Cap = 0pf */
#define OSCCAP_1            (0x10)                     /* XIN Cap = XOUT Cap = 10pf */
#define OSCCAP_2            (0x20)                     /* XIN Cap = XOUT Cap = 14pf */
#define OSCCAP_3            (0x30)                     /* XIN Cap = XOUT Cap = 18pf */

#define FLL_CTL1_             0x0054    /* FLL+ Control 1 */
sfrb(FLL_CTL1, FLL_CTL1_);
#define FLL_DIV0            (0x01)                     /* FLL+ Divide Px.x/ACLK 0 */
#define FLL_DIV1            (0x02)                     /* FLL+ Divide Px.x/ACLK 1 */

#define FLL_DIV_1           (0x00)                     /* FLL+ Divide Px.x/ACLK By 1 */
#define FLL_DIV_2           (0x01)                     /* FLL+ Divide Px.x/ACLK By 2 */
#define FLL_DIV_4           (0x02)                     /* FLL+ Divide Px.x/ACLK By 4 */
#define FLL_DIV_8           (0x03)                     /* FLL+ Divide Px.x/ACLK By 8 */

/* INTERRUPT CONTROL BITS */
/* These two bits are defined in the Special Function Registers */
/* #define OFIFG               0x02 */
/* #define OFIE                0x02 */

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
* LCD
************************************************************/
#define __MSP430_HAS_LCD4__           /* Definition to show that Module is available */

#define LCDCTL_               0x0090    /* LCD Control */
sfrb(LCDCTL, LCDCTL_);
/* the names of the mode bits are different from the spec */
#define LCDON               (0x01)
//#define LCDLOWR             (0x02)
#define LCDSON              (0x04)
#define LCDMX0              (0x08)
#define LCDMX1              (0x10)
#define LCDP0               (0x20)
#define LCDP1               (0x40)
#define LCDP2               (0x80)
/* Display modes coded with Bits 2-4 */
#define LCDSTATIC           (LCDSON)
#define LCD2MUX             (LCDMX0+LCDSON)
#define LCD3MUX             (LCDMX1+LCDSON)
#define LCD4MUX             (LCDMX1+LCDMX0+LCDSON)
/* Group select code with Bits 5-7                     Seg.lines   Dig.output */
#define LCDSG0              (0x00)                    /* ---------   Port Only (default) */
#define LCDSG0_1            (LCDP0)                   /* S0  - S15   see Datasheet */
#define LCDSG0_2            (LCDP1)                   /* S0  - S19   see Datasheet */
#define LCDSG0_3            (LCDP1+LCDP0)             /* S0  - S23   see Datasheet */
#define LCDSG0_4            (LCDP2)                   /* S0  - S27   see Datasheet */
#define LCDSG0_5            (LCDP2+LCDP0)             /* S0  - S31   see Datasheet */
#define LCDSG0_6            (LCDP2+LCDP1)             /* S0  - S35   see Datasheet */
#define LCDSG0_7            (LCDP2+LCDP1+LCDP0)       /* S0  - S39   see Datasheet */
/* NOTE: YOU CAN ONLY USE THE 'S' OR 'G' DECLARATIONS FOR A COMMAND */
/* MOV  #LCDSG0_3+LCDOG2_7,&LCDCTL ACTUALY MEANS MOV  #LCDP1,&LCDCTL! */
#define LCDOG1_7            (0x00)                    /* ---------   Port Only (default) */
#define LCDOG2_7            (LCDP0)                   /* S0  - S15   see Datasheet */
#define LCDOG3_7            (LCDP1)                   /* S0  - S19   see Datasheet */
#define LCDOG4_7            (LCDP1+LCDP0)             /* S0  - S23   see Datasheet */
#define LCDOG5_7            (LCDP2)                   /* S0  - S27   see Datasheet */
#define LCDOG6_7            (LCDP2+LCDP0)             /* S0  - S31   see Datasheet */
#define LCDOG7              (LCDP2+LCDP1)             /* S0  - S35   see Datasheet */
#define LCDOGOFF            (LCDP2+LCDP1+LCDP0)       /* S0  - S39   see Datasheet */

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
* USART
************************************************************/

/* UxCTL */
#define PENA                (0x80)        /* Parity enable */
#define PEV                 (0x40)        /* Parity 0:odd / 1:even */
#define SPB                 (0x20)        /* Stop Bits 0:one / 1: two */
#define CHAR                (0x10)        /* Data 0:7-bits / 1:8-bits */
#define LISTEN              (0x08)        /* Listen mode */
#define SYNC                (0x04)        /* UART / SPI mode */
#define MM                  (0x02)        /* Master Mode off/on */
#define SWRST               (0x01)        /* USART Software Reset */

/* UxTCTL */
#define CKPH                (0x80)        /* SPI: Clock Phase */
#define CKPL                (0x40)        /* Clock Polarity */
#define SSEL1               (0x20)        /* Clock Source Select 1 */
#define SSEL0               (0x10)        /* Clock Source Select 0 */
#define URXSE               (0x08)        /* Receive Start edge select */
#define TXWAKE              (0x04)        /* TX Wake up mode */
#define STC                 (0x02)        /* SPI: STC enable 0:on / 1:off */
#define TXEPT               (0x01)        /* TX Buffer empty */

/* UxRCTL */
#define FE                  (0x80)        /* Frame Error */
#define PE                  (0x40)        /* Parity Error */
#define OE                  (0x20)        /* Overrun Error */
#define BRK                 (0x10)        /* Break detected */
#define URXEIE              (0x08)        /* RX Error interrupt enable */
#define URXWIE              (0x04)        /* RX Wake up interrupt enable */
#define RXWAKE              (0x02)        /* RX Wake up detect */
#define RXERR               (0x01)        /* RX Error Error */

/************************************************************
* USART 0
************************************************************/
#define __MSP430_HAS_UART0__          /* Definition to show that Module is available */

#define U0CTL_                0x0070    /* USART 0 Control */
sfrb(U0CTL, U0CTL_);
#define U0TCTL_               0x0071    /* USART 0 Transmit Control */
sfrb(U0TCTL, U0TCTL_);
#define U0RCTL_               0x0072    /* USART 0 Receive Control */
sfrb(U0RCTL, U0RCTL_);
#define U0MCTL_               0x0073    /* USART 0 Modulation Control */
sfrb(U0MCTL, U0MCTL_);
#define U0BR0_                0x0074    /* USART 0 Baud Rate 0 */
sfrb(U0BR0, U0BR0_);
#define U0BR1_                0x0075    /* USART 0 Baud Rate 1 */
sfrb(U0BR1, U0BR1_);
#define U0RXBUF_              0x0076    /* USART 0 Receive Buffer */
const_sfrb(U0RXBUF, U0RXBUF_);
#define U0TXBUF_              0x0077    /* USART 0 Transmit Buffer */
sfrb(U0TXBUF, U0TXBUF_);

/* Alternate register names */

#define UCTL0               U0CTL     /* USART 0 Control */
#define UTCTL0              U0TCTL    /* USART 0 Transmit Control */
#define URCTL0              U0RCTL    /* USART 0 Receive Control */
#define UMCTL0              U0MCTL    /* USART 0 Modulation Control */
#define UBR00               U0BR0     /* USART 0 Baud Rate 0 */
#define UBR10               U0BR1     /* USART 0 Baud Rate 1 */
#define RXBUF0              U0RXBUF   /* USART 0 Receive Buffer */
#define TXBUF0              U0TXBUF   /* USART 0 Transmit Buffer */
#define UCTL0_              U0CTL_    /* USART 0 Control */
#define UTCTL0_             U0TCTL_   /* USART 0 Transmit Control */
#define URCTL0_             U0RCTL_   /* USART 0 Receive Control */
#define UMCTL0_             U0MCTL_   /* USART 0 Modulation Control */
#define UBR00_              U0BR0_    /* USART 0 Baud Rate 0 */
#define UBR10_              U0BR1_    /* USART 0 Baud Rate 1 */
#define RXBUF0_             U0RXBUF_  /* USART 0 Receive Buffer */
#define TXBUF0_             U0TXBUF_  /* USART 0 Transmit Buffer */
#define UCTL_0              U0CTL     /* USART 0 Control */
#define UTCTL_0             U0TCTL    /* USART 0 Transmit Control */
#define URCTL_0             U0RCTL    /* USART 0 Receive Control */
#define UMCTL_0             U0MCTL    /* USART 0 Modulation Control */
#define UBR0_0              U0BR0     /* USART 0 Baud Rate 0 */
#define UBR1_0              U0BR1     /* USART 0 Baud Rate 1 */
#define RXBUF_0             U0RXBUF   /* USART 0 Receive Buffer */
#define TXBUF_0             U0TXBUF   /* USART 0 Transmit Buffer */
#define UCTL_0_             U0CTL_    /* USART 0 Control */
#define UTCTL_0_            U0TCTL_   /* USART 0 Transmit Control */
#define URCTL_0_            U0RCTL_   /* USART 0 Receive Control */
#define UMCTL_0_            U0MCTL_   /* USART 0 Modulation Control */
#define UBR0_0_             U0BR0_    /* USART 0 Baud Rate 0 */
#define UBR1_0_             U0BR1_    /* USART 0 Baud Rate 1 */
#define RXBUF_0_            U0RXBUF_  /* USART 0 Receive Buffer */
#define TXBUF_0_            U0TXBUF_  /* USART 0 Transmit Buffer */

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

/*************************************************************
* Flash Memory
*************************************************************/
#define __MSP430_HAS_FLASH__          /* Definition to show that Module is available */

#define FCTL1_                0x0128    /* FLASH Control 1 */
sfrw(FCTL1, FCTL1_);
#define FCTL2_                0x012A    /* FLASH Control 2 */
sfrw(FCTL2, FCTL2_);
#define FCTL3_                0x012C    /* FLASH Control 3 */
sfrw(FCTL3, FCTL3_);

#define FRKEY               (0x9600)  /* Flash key returned by read */
#define FWKEY               (0xA500)  /* Flash key for write */
#define FXKEY               (0x3300)  /* for use with XOR instruction */

#define ERASE               (0x0002)  /* Enable bit for Flash segment erase */
#define MERAS               (0x0004)  /* Enable bit for Flash mass erase */
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

/************************************************************
* HARDWARE MULTIPLIER
************************************************************/
#define __MSP430_HAS_MPY__            /* Definition to show that Module is available */

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

/************************************************************
* SD16 - Sigma Delta 16 Bit
************************************************************/
#define __MSP430_HAS_SD16_2__         /* Definition to show that Module is available */

#define SD16INCTL0_           0x00B0    /* SD16 Input Control Register Channel 0 */
sfrb(SD16INCTL0, SD16INCTL0_);
#define SD16INCTL1_           0x00B1    /* SD16 Input Control Register Channel 1 */
sfrb(SD16INCTL1, SD16INCTL1_);
#define SD16PRE0_             0x00B8    /* SD16 Preload Register Channel 0 */
sfrb(SD16PRE0, SD16PRE0_);
#define SD16PRE1_             0x00B9    /* SD16 Preload Register Channel 1 */
sfrb(SD16PRE1, SD16PRE1_);
#define SD16CONF0_            0x00B7    /* SD16 Internal Configuration Register 0 */
sfrb(SD16CONF0, SD16CONF0_);
#define SD16CONF1_            0x00BF    /* SD16 Internal Configuration Register 1 */
sfrb(SD16CONF1, SD16CONF1_);
/* Please use only the recommended settings */

#define SD16CTL_              0x0100    /* Sigma Delta ADC 16 Control Register */
sfrw(SD16CTL, SD16CTL_);
#define SD16CCTL0_            0x0102    /* SD16 Channel 0 Control Register */
sfrw(SD16CCTL0, SD16CCTL0_);
#define SD16CCTL1_            0x0104    /* SD16 Channel 1 Control Register */
sfrw(SD16CCTL1, SD16CCTL1_);
#define SD16IV_               0x0110    /* SD16 Interrupt Vector Register */
sfrw(SD16IV, SD16IV_);
#define SD16MEM0_             0x0112    /* SD16 Channel 0 Conversion Memory */
sfrw(SD16MEM0, SD16MEM0_);
#define SD16MEM1_             0x0114    /* SD16 Channel 1 Conversion Memory */
sfrw(SD16MEM1, SD16MEM1_);

/* SD16INCTLx - AFEINCTLx */
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
#define SD16INCH_5          (0x0005)  /* SD16 Input Channel select input */
#define SD16INCH_6          (0x0006)  /* SD16 Input Channel select Temp */
#define SD16INCH_7          (0x0007)  /* SD16 Input Channel select Offset */

#define SD16INTDLY_0        (0x0000)  /* SD16 Interrupt Delay: Int. after 4.Conversion  */
#define SD16INTDLY_1        (0x0040)  /* SD16 Interrupt Delay: Int. after 3.Conversion  */
#define SD16INTDLY_2        (0x0080)  /* SD16 Interrupt Delay: Int. after 2.Conversion  */
#define SD16INTDLY_3        (0x00C0)  /* SD16 Interrupt Delay: Int. after 1.Conversion  */

/* SD16CTL - AFECTL */
#define SD16OVIE            (0x0002)  /* SD16 Overflow Interupt Enable */
#define SD16REFON           (0x0004)  /* SD16 Switch internal Reference on */
#define SD16VMIDON          (0x0008)  /* SD16 Switch Vmid Buffer on */
#define SD16SSEL0           (0x0010)  /* SD16 Clock Source Select 0 */
#define SD16SSEL1           (0x0020)  /* SD16 Clock Source Select 1 */
#define SD16DIV0            (0x0040)  /* SD16 Clock Divider Select 0 */
#define SD16DIV1            (0x0080)  /* SD16 Clock Divider Select 1 */
#define SD16LP              (0x0100)  /* SD16 Low Power Mode Enable */

#define SD16DIV_0            (0x0000)               /* SD16 Clock Divider Select /1 */
#define SD16DIV_1            (SD16DIV0)             /* SD16 Clock Divider Select /2 */
#define SD16DIV_2            (SD16DIV1)             /* SD16 Clock Divider Select /4 */
#define SD16DIV_3            (SD16DIV0+SD16DIV1)    /* SD16 Clock Divider Select /8 */

#define SD16SSEL_0           (0x0000)               /* SD16 Clock Source Select MCLK  */
#define SD16SSEL_1           (SD16SSEL0)            /* SD16 Clock Source Select SMCLK */
#define SD16SSEL_2           (SD16SSEL1)            /* SD16 Clock Source Select ACLK  */
#define SD16SSEL_3           (SD16SSEL0+SD16SSEL1)  /* SD16 Clock Source Select TACLK */

/* SD16CCTLx - AFECCTLx */
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

#define SD16OSR_256         (0x0000)  /* SD16 Channel x OverSampling Ratio 256 */
#define SD16OSR_128         (0x0100)  /* SD16 Channel x OverSampling Ratio 128 */
#define SD16OSR_64          (0x0200)  /* SD16 Channel x OverSampling Ratio  64 */
#define SD16OSR_32          (0x0300)  /* SD16 Channel x OverSampling Ratio  32 */

/* SD16IV Definitions */
#define SD16IV_NONE         (0x0000)    /* No Interrupt pending */
#define SD16IV_SD16OVIFG    (0x0002)    /* SD16OVIFG */
#define SD16IV_SD16MEM0     (0x0004)    /* SD16MEM0 SD16IFG */
#define SD16IV_SD16MEM1     (0x0006)    /* SD16MEM1 SD16IFG */

/************************************************************
* ESP430E
************************************************************/
#define __MSP430_HAS_ESP430E__         /* Definition to show that Module is available */

#define AFEINCTL0           SD16INCTL0 /* AFE Input Control Register Channel 0 */
#define AFEINCTL1           SD16INCTL1 /* AFE Input Control Register Channel 1 */
#define AFECTL              SD16CTL    /* Analog Front End Control Register */
#define AFECCTL0            SD16CCTL0  /* AFE Channel 0 Control Register */
#define AFECCTL1            SD16CCTL1  /* AFE Channel 1 Control Register */

#define ESPCTL_               0x0150    /* ESP430 Control Register */
sfrw(ESPCTL, ESPCTL_);
#define MBCTL_                0x0152    /* Mailbox Control Register */
sfrw(MBCTL, MBCTL_);
#define MBIN0_                0x0154    /* Incoming Mailbox 0 Register */
sfrw(MBIN0, MBIN0_);
#define MBIN1_                0x0156    /* Incoming Mailbox 1 Register */
sfrw(MBIN1, MBIN1_);
#define MBOUT0_               0x0158    /* Outgoing Mailbox 0 Register */
sfrw(MBOUT0, MBOUT0_);
#define MBOUT1_               0x015A    /* Outgoing Mailbox 1 Register */
sfrw(MBOUT1, MBOUT1_);

#define ESP430_STAT0_         0x01C0    /* ESP430 Return Value 0 */
const_sfrw(ESP430_STAT0, ESP430_STAT0_);
#define ESP430_STAT1_         0x01C2    /* ESP430 Return Value 1 */
const_sfrw(ESP430_STAT1, ESP430_STAT1_);
#define WAVEFSV1_             0x01C4    /* ESP430 Return Value 2 */
const_sfrw(WAVEFSV1, WAVEFSV1_);
#define RET3_                 0x01C6    /* ESP430 Return Value 3 */
const_sfrw(RET3, RET3_);
#define RET4_                 0x01C8    /* ESP430 Return Value 4 */
const_sfrw(RET4, RET4_);
#define WAVEFSI1_             0x01CA    /* ESP430 Return Value 5 */
const_sfrw(WAVEFSI1, WAVEFSI1_);
#define RET7_                 0x01CE    /* ESP430 Return Value 7 */
const_sfrw(RET7, RET7_);
#define ACTENERGY1_LO_        0x01D0    /* ESP430 Return Value 8 */
const_sfrw(ACTENERGY1_LO, ACTENERGY1_LO_);
#define ACTENERGY1_HI_        0x01D2    /* ESP430 Return Value 9 */
const_sfrw(ACTENERGY1_HI, ACTENERGY1_HI_);
#define REACTENERGY_LO_       0x01D8    /* ESP430 Return Value 12 */
const_sfrw(REACTENERGY_LO, REACTENERGY_LO_);
#define REACTENERGY_HI_       0x01DA    /* ESP430 Return Value 13 */
const_sfrw(REACTENERGY_HI, REACTENERGY_HI_);
#define APPENERGY_LO_         0x01DC    /* ESP430 Return Value 14 */
const_sfrw(APPENERGY_LO, APPENERGY_LO_);
#define APPENERGY_HI_         0x01DE    /* ESP430 Return Value 15 */
const_sfrw(APPENERGY_HI, APPENERGY_HI_);
#define ACTENSPER1_LO_        0x01E0    /* ESP430 Return Value 16 */
const_sfrw(ACTENSPER1_LO, ACTENSPER1_LO_);
#define ACTENSPER1_HI_        0x01E2    /* ESP430 Return Value 17 */
const_sfrw(ACTENSPER1_HI, ACTENSPER1_HI_);
#define IRMS_2_HI_            0x01E4    /* ESP430 Return Value 18 */
const_sfrw(IRMS_2_HI, IRMS_2_HI_);
#define IRMS_2_LO_            0x01E6    /* ESP430 Return Value 19 */
const_sfrw(IRMS_2_LO, IRMS_2_LO_);
#define POWERFCT_             0x01E8    /* ESP430 Return Value 20 */
const_sfrw(POWERFCT, POWERFCT_);
//const sfrw RESERVED         (0x01EA)  /* ESP430 Return Value 21 */
#define MAINSPERIOD_          0x01EC    /* ESP430 Return Value 22 */
const_sfrw(MAINSPERIOD, MAINSPERIOD_);
#define V1RMS_                0x01EE    /* ESP430 Return Value 23 */
const_sfrw(V1RMS, V1RMS_);
#define IRMS_LO_              0x01F0    /* ESP430 Return Value 24 */
const_sfrw(IRMS_LO, IRMS_LO_);
#define IRMS_HI_              0x01F2    /* ESP430 Return Value 25 */
const_sfrw(IRMS_HI, IRMS_HI_);
#define VPEAK_                0x01F4    /* ESP430 Return Value 26 */
const_sfrw(VPEAK, VPEAK_);
#define IPEAK_                0x01F6    /* ESP430 Return Value 27 */
const_sfrw(IPEAK, IPEAK_);
#define LINECYCLCNT_LO_       0x01F8    /* ESP430 Return Value 28 */
const_sfrw(LINECYCLCNT_LO, LINECYCLCNT_LO_);
#define LINECYCLCNT_HI_       0x01FA    /* ESP430 Return Value 29 */
const_sfrw(LINECYCLCNT_HI, LINECYCLCNT_HI_);
#define NMBMEAS_LO_           0x01FC    /* ESP430 Return Value 30 */
const_sfrw(NMBMEAS_LO, NMBMEAS_LO_);
#define NMBMEAS_HI_           0x01FE    /* ESP430 Return Value 31 */
const_sfrw(NMBMEAS_HI, NMBMEAS_HI_);

#define RET0           ESP430_STAT0   /* STATUS0 of ESP430 */
#define RET1           ESP430_STAT1   /* STATUS1 of ESP430 */
#define RET2           WAVEFSV1       /* Waveform Sample V1 offset corrected*/
#define RET5           WAVEFSI1       /* Waveform Sample I1 offset corrected*/
//#define RET6          RESERVED       /* Reserved */
#define RET8           ACTENERGY1_LO  /* Active energy I1 Low Word */
#define RET9           ACTENERGY1_HI  /* Active energy I1 High Word */
//#define RET10          RESERVED       /* Reserved */
//#define RET11          RESERVED       /* Reserved */
#define RET12          REACTENERGY_LO /* Reactive energy Low Word */
#define RET13          REACTENERGY_HI /* Reactive energy High Word */
#define RET14          APPENERGY_LO   /* Apparent energy Low Word */
#define RET15          APPENERGY_HI   /* Apparent energy High Word */
#define RET16          ACTENSPER1_LO  /* Active energy I1 for last mains period Low Word */
#define RET17          ACTENSPER1_HI  /* Active energy I1 for last mains period High Word */
#define RET18          IRMS_2_HI      /* Current RMS value last second I1 High Word */
#define RET19          IRMS_2_LO      /* Current RMS value last second I1 Low Word */
#define RET20          POWERFCT       /* Power factor */
//#define RET21          RESERVED       /* Reserved */
#define RET22          MAINSPERIOD    /* Mains period */
#define RET23          V1RMS          /* Voltage RMS V1 value last second */
#define RET24          IRMS_LO        /* Current RMS value last second I1 Low Word */
#define RET25          IRMS_HI        /* Current RMS value last second I1 High Word */
#define RET26          VPEAK          /* Voltage V1 absolute peak value */
#define RET27          IPEAK          /* Current absolute peak value I1 */
#define RET28          LINECYCLCNT_LO /* Line cycle counter Low Word */
#define RET29          LINECYCLCNT_HI /* Line cycle counter High Word */
#define RET30          NMBMEAS_LO     /* Number of Measurements for CPU signal Low Word */
#define RET31          NMBMEAS_HI     /* Number of Measurements for CPU signal High Word */

/* ESPCTL */
#define ESPEN               (0x0001)  /* ESP430 Module enable */
#define ESPSUSP             (0x0002)  /* ESP430 Module suspend */
#define IREQ                (0x0004)  /* NOT supported by current ESP430 Software */

/* RET0 - Status0 Flags */
#define WFSRDYFG            (0x0001)  /* New waveform Samples ready Flag */
//#define RESERVED          (0x0002)  /* Reserved */
#define ILREACHEDFG         (0x0004)  /* Interrupt level reached Flag */
#define ENRDYFG             (0x0008)  /* New Energy values ready Flag */
#define ZXLDFG              (0x0010)  /* Zero Crossing of V1 Flag (leading edge) */
#define ZXTRFG              (0x0020)  /* Zero Crossing of V1 Flag (trailing edge) */
#define CALRDYFG            (0x0040)  /* Calibration values ready Flag */
#define TAMPFG              (0x0080)  /* Tampering Occured Flag */
#define NEGENFG             (0x0100)  /* Negativ Energy Flag */
#define VDROPFG             (0x0200)  /* Voltage drop occured Flag */
#define VPEAKFG             (0x0400)  /* Voltage exceed VPeak level Flag */
#define I1PEAKFG            (0x0800)  /* Current exceed I1Peak level Flag */
//#define RESERVED          (0x1000)  /* Reserved */
//#define RESERVED          (0x2000)  /* Reserved */
//#define RESERVED          (0x4000)  /* Reserved */
#define ACTIVEFG            (0x8000)  /* Measurement or Calibration running Flag */

/* RET1 - Status1 Flags */
#define ILNEGFG             (0x0004)  /* Negative Interrupt level reached Flag */
#define VPEAKNEGFG          (0x0400)  /* Negative Voltage exceed VPeak level Flag */
#define I1PEAKNEGFG         (0x0800)  /* Negative Current exceed I1Peak level Flag */

/* MBCTL */
#define IN0IFG              (0x0001)  /* Incoming Mail 0 Interrupt Flag */
#define IN1IFG              (0x0002)  /* Incoming Mail 1 Interrupt Flag */
#define OUT0FG              (0x0004)  /* Outgoing Mail 0 Flag */
#define OUT1FG              (0x0008)  /* Outgoing Mail 1 Flag */
#define IN0IE               (0x0010)  /* Incoming Mail 0 Interrupt Enable */
#define IN1IE               (0x0020)  /* Incoming Mail 1 Interrupt Enable */
#define CLR0OFF             (0x0040)  /* Switch off automatic clear of IN0IFG */
#define CLR1OFF             (0x0080)  /* Switch off automatic clear of IN1IFG */
#define OUT0IFG             (0x0100)  /* Outgoing Mail 0 Interrupt Flag */
#define OUT1IFG             (0x0200)  /* Outgoing Mail 1 Interrupt Flag */
#define OUT0IE              (0x0400)  /* Outgoing Mail 0 Interrupt Enable */
#define OUT1IE              (0x0800)  /* Outgoing Mail 1 Interrupt Enable */

/* Messages to ESP */
#define mRESET              (0x0001)  /* Restart ESP430 Software */
#define mSET_MODE           (0x0003)  /* Set Operation Mode for ESP430 Software */
#define mCLR_EVENT          (0x0005)  /* Clear Flags for ESP430 Software */
#define mINIT               (0x0007)  /* Initialize ESP430 Software */
#define mTEMP               (0x0009)  /* Request Temp. Measurement from ESP430 Software */
#define mSWVERSION          (0x000B)  /* Request software version of ESP430 */
#define mREAD_PARAM         (0x000D)  /* Request to read the parameter with no. “Parameter No.” */


#define mSET_CTRL0          (0x0200)  /* Set Control Register 0 */
#define mSET_CTRL1          (0x0202)  /* Set Control Register 1 */
#define mSET_EVENT          (0x0204)  /* Set which Evenets should cause an message */
#define mSET_PHASECORR1     (0x0206)  /* Set Phase Correction fo I1 */
#define mSET_V1OFFSET       (0x020A)  /* Set Offset for V1 */
#define mSET_I1OFFSET       (0x020C)  /* Set Offset for I1 */
#define mSET_ADAPTI1        (0x0210)  /* Set Adaption factor for I1 */
#define mSET_GAINCORR1      (0x0214)  /* Set Gain Correction for Power P1 */
#define mSET_POFFSET1_LO    (0x0216)  /* Set Power Offset for Power P1 */
#define mSET_POFFSET1_HI    (0x0218)  /* Set Power Offset for Power P1 */
#define mSET_INTRPTLEVL_LO  (0x0220)  /* Set Interrupt Level */
#define mSET_INTRPTLEVL_HI  (0x0222)  /* Set Interrupt Level */
#define mSET_CALCYCLCNT     (0x0224)  /* Set number of main cycles for calibration */
#define mSET_STARTCURR_FRAC (0x0226)  /* Set start current fractional Part*/
#define mSET_STARTCURR_INT  (0x0228)  /* Set start current integer Part*/
#define mSET_NOMFREQ        (0x022A)  /* Set nominal main frequency */
#define mSET_VDROPCYCLS     (0x022C)  /* Set cylces for VDrop detection */
#define mSET_RATIOTAMP      (0x022E)  /* Set ratio for tamper detection */
#define mSET_ITAMP          (0x0230)  /* Set minimum current for tamper detection */
#define mSET_VDROPLEVEL     (0x0232)  /* Set level for VDrop detection */
#define mSET_VPEAKLEVEL     (0x0234)  /* Set level for VPeak detection */
#define mSET_IPEAKLEVEL     (0x0236)  /* Set level for IPeak detection */
#define mSET_DCREMPER       (0x0238)  /* Set number of periods for DC-removal */

#define mSET_DELTAV1MAX     (0x023A)  /* Set max. change of V1 if V1FILTER =1  */
#define mSET_CORRCOMP       (0x023C)  /* Set Common Mode Rejection ratio */
#define mSET_FADCU          (0x023E)  /* Set ADC sampling frequency fADC */

/* Flags for mSET_CTRL0 */
#define CURR_I1             (0x0002)  /* 0: Current transformer, dc-tol CT or shunt */
/* 1: Rogowski coil (not yet implemented) */
#define MB                  (0x0004)  /* Intrpt to CPU: 0: energy level  1: #measurements */
#define NE0                 (0x0008)  /* Neg. energy treatment:      00: clear neg. energy */
#define NE1                 (0x0010)  /* 01: use absolute energy   10: use energy as it is */
#define DCREM_V1            (0x0020)  /* DC removal for V1:  0: off  1: on */
#define DCREM_I1            (0x0040)  /* DC removal for I1:  0: off  1: on */
#define V1FILTER            (0x0100)  /* Enable on V1 Spike Filter:  0: off  1: on */

/* Messages from ESP */
#define mEVENT              (0x0001)  /* Event Status Flag for ESP430 Software */
#define mTEMPRDY            (0x0003)  /* Temperature measurement completed and in MBIN1 */
#define mSWRDY              (0x0005)  /* Software version in MBIN1 */
#define mPARAMRDY           (0x0007)  /* Parameter requested by mREAD_PARAM returned in MBIN1  */
#define mPARAMSET           (0x0009)  /* Parameter has been set */

/* EVENT: Event Message Enable Bits */
#define WFSRDYME            (0x0001)  /* New waveform Samples ready */
#define ILREACHEDME         (0x0004)  /* Interrupt level reached */
#define ENRDYME             (0x0008)  /* New Energy values ready */
#define ZXLDME              (0x0010)  /* Zero Crossing of V1 (leading edge) */
#define ZXTRME              (0x0020)  /* Zero Crossing of V1 (trailing edge) */
#define CALRDYME            (0x0040)  /* Calibration values ready */
#define TAMPME              (0x0080)  /* Tampering Occured */
#define NEGENME             (0x0100)  /* Negativ Energy */
#define VDROPME             (0x0200)  /* Voltage drop occured */
#define VPEAKME             (0x0400)  /* Voltage exceed VPeak level */
#define I1PEAKME            (0x0800)  /* Current exceed I1Peak level */
//#define RESERVED            (0x1000)  /* Reserved */
//#define RESERVED            (0x2000)  /* Reserved */
//#define RESERVED            (0x4000)  /* Reserved */
#define ACTIVEME            (0x8000)  /* Measurement of Calibration running */


/* ESP Modes */
#define modeIDLE            (0x0000)  /* Set Mode: Idle Mode */
#define modeCALIBRATION     (0x0002)  /* Set Mode: Calibration Mode */
#define modeMEASURE         (0x0004)  /* Set Mode: Measure Mode */
#define modeRESET           (0x0006)  /* Set Mode: Reset and Restart the ESP430 module */
#define modeINIT            (0x0008)  /* Set Mode: Initialize ESP430 module */

/************************************************************
* Interrupt Vectors (offset from 0xFFE0)
************************************************************/

#define BASICTIMER_VECTOR       ( 1)                     /* 0xFFE0 Basic Timer */
#define PORT2_VECTOR            ( 2)                     /* 0xFFE2 Port 2 */
#define PORT1_VECTOR            ( 5)                     /* 0xFFE8 Port 1 */
#define TIMERA1_VECTOR          ( 6)                     /* 0xFFEA Timer A CC1-2, TA */
#define TIMERA0_VECTOR          ( 7)                     /* 0xFFEC Timer A CC0 */
#define USART0TX_VECTOR         ( 9)                     /* 0xFFF0 USART 0 Transmit */
#define USART0RX_VECTOR         (10)                     /* 0xFFF2 USART 0 Receive */
#define WDT_VECTOR              (11)                     /* 0xFFF4 Watchdog Timer */

#define SD16_VECTOR             (13)                     /* 0xFFF8 Sigma Delta ADC */
#define ESP430_VECTOR           (14)                     /* 0xFFFA ESP430 Mailbox */

#define NMI_VECTOR              (15)                     /* 0xFFFC Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */


/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __msp430xE42x2 */


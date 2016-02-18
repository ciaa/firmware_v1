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
* RF430FRL153H_ROM device.
*
* Texas Instruments, Version 1.1
*
* Rev. 1.0, Initial Release
*
********************************************************************/

#ifndef __RF430FRL153H_ROM
#define __RF430FRL153H_ROM

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

/************************************************************
* PERIPHERAL FILE MAP
************************************************************/


/************************************************************
* COMPACT CLOCK SYSTEM
************************************************************/
#define __MSP430_HAS_CCS__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_CCS__ 0x0160
#define CCS_BASE __MSP430_BASEADDRESS_CCS__

#define CCSCTL0_              0x0160    /* CCS Control Register 0 */
sfrb(CCSCTL0_L , CCSCTL0_);
sfrb(CCSCTL0_H , CCSCTL0_+1);
sfrw(CCSCTL0, CCSCTL0_);
#define CCSCTL1_              0x0162    /* CCS Control Register 1 */
sfrb(CCSCTL1_L , CCSCTL1_);
sfrb(CCSCTL1_H , CCSCTL1_+1);
sfrw(CCSCTL1, CCSCTL1_);
#define CCSCTL2_              0x0164    /* CCS Control Register 2 */
sfrb(CCSCTL2_L , CCSCTL2_);
sfrb(CCSCTL2_H , CCSCTL2_+1);
sfrw(CCSCTL2, CCSCTL2_);
#define CCSCTL4_              0x0168    /* CCS Control Register 4 */
sfrb(CCSCTL4_L , CCSCTL4_);
sfrb(CCSCTL4_H , CCSCTL4_+1);
sfrw(CCSCTL4, CCSCTL4_);
#define CCSCTL5_              0x016A    /* CCS Control Register 5 */
sfrb(CCSCTL5_L , CCSCTL5_);
sfrb(CCSCTL5_H , CCSCTL5_+1);
sfrw(CCSCTL5, CCSCTL5_);
#define CCSCTL6_              0x016C    /* CCS Control Register 6 */
sfrb(CCSCTL6_L , CCSCTL6_);
sfrb(CCSCTL6_H , CCSCTL6_+1);
sfrw(CCSCTL6, CCSCTL6_);
#define CCSCTL7_              0x016E    /* CCS Control Register 7 */
sfrb(CCSCTL7_L , CCSCTL7_);
sfrb(CCSCTL7_H , CCSCTL7_+1);
sfrw(CCSCTL7, CCSCTL7_);
#define CCSCTL8_              0x0170    /* CCS Control Register 8 */
sfrb(CCSCTL8_L , CCSCTL8_);
sfrb(CCSCTL8_H , CCSCTL8_+1);
sfrw(CCSCTL8, CCSCTL8_);

/* CCSCTL0 Control Bits */

#define CCSKEY               (0xA500)    /* CCS Password */

/* CCSCTL1 Control Bits */
#define DIVCLK              (0x0001)    /* Clock division for CLKIN / X-OSC */

/* CCSCTL1 Control Bits */
#define DIVCLK_L            (0x0001)    /* Clock division for CLKIN / X-OSC */


/* CCSCTL2 Control Bits */
#define FSEL0               (0x0001)    /* Frequency trimming of the HF-OSC Bit: 0 */
#define FSEL1               (0x0002)    /* Frequency trimming of the HF-OSC Bit: 1 */
#define FSEL2               (0x0004)    /* Frequency trimming of the HF-OSC Bit: 2 */
#define FSEL3               (0x0008)    /* Frequency trimming of the HF-OSC Bit: 3 */
#define FSEL4               (0x0010)    /* Frequency trimming of the HF-OSC Bit: 4 */
#define FSEL5               (0x0020)    /* Frequency trimming of the HF-OSC Bit: 5 */
#define FSEL6               (0x0040)    /* Frequency trimming of the HF-OSC Bit: 6 */

/* CCSCTL2 Control Bits */
#define FSEL0_L             (0x0001)    /* Frequency trimming of the HF-OSC Bit: 0 */
#define FSEL1_L             (0x0002)    /* Frequency trimming of the HF-OSC Bit: 1 */
#define FSEL2_L             (0x0004)    /* Frequency trimming of the HF-OSC Bit: 2 */
#define FSEL3_L             (0x0008)    /* Frequency trimming of the HF-OSC Bit: 3 */
#define FSEL4_L             (0x0010)    /* Frequency trimming of the HF-OSC Bit: 4 */
#define FSEL5_L             (0x0020)    /* Frequency trimming of the HF-OSC Bit: 5 */
#define FSEL6_L             (0x0040)    /* Frequency trimming of the HF-OSC Bit: 6 */


/* CCSCTL4 Control Bits */
#define SELM0               (0x0001)   /* MCLK Source Select Bit: 0 */
#define SELM1               (0x0002)   /* MCLK Source Select Bit: 1 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELS0               (0x0010)   /* SMCLK Source Select Bit: 0 */
#define SELS1               (0x0020)   /* SMCLK Source Select Bit: 1 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define SELA0               (0x0100)   /* ACLK Source Select Bit: 0 */
#define SELA1               (0x0200)   /* ACLK Source Select Bit: 1 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CCSCTL4 Control Bits */
#define SELM0_L             (0x0001)   /* MCLK Source Select Bit: 0 */
#define SELM1_L             (0x0002)   /* MCLK Source Select Bit: 1 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define SELS0_L             (0x0010)   /* SMCLK Source Select Bit: 0 */
#define SELS1_L             (0x0020)   /* SMCLK Source Select Bit: 1 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CCSCTL4 Control Bits */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define SELA0_H             (0x0001)   /* ACLK Source Select Bit: 0 */
#define SELA1_H             (0x0002)   /* ACLK Source Select Bit: 1 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

#define SELM_0              (0x0000)   /* MCLK Source Select 0 */
#define SELM_1              (0x0001)   /* MCLK Source Select 1 */
#define SELM_2              (0x0002)   /* MCLK Source Select 2 */
#define SELM_3              (0x0003)   /* MCLK Source Select 3 */
#define SELM__HFCLK         (0x0000)   /* MCLK Source Select HFCLK */
#define SELM__LFCLK         (0x0001)   /* MCLK Source Select LFCLK */
#define SELM__CLKIN         (0x0002)   /* MCLK Source Select CLKIN */

#define SELS_0              (0x0000)   /* SMCLK Source Select 0 */
#define SELS_1              (0x0010)   /* SMCLK Source Select 1 */
#define SELS_2              (0x0020)   /* SMCLK Source Select 2 */
#define SELS_3              (0x0030)   /* SMCLK Source Select 3 */
#define SELS__HFCLK         (0x0000)   /* SMCLK Source Select HFCLK */
#define SELS__LFCLK         (0x0010)   /* SMCLK Source Select LFCLK */
#define SELS__CLKIN         (0x0020)   /* SMCLK Source Select CLKIN */

#define SELA_0              (0x0000)   /* ACLK Source Select 0 */
#define SELA_1              (0x0100)   /* ACLK Source Select 1 */
#define SELA_2              (0x0200)   /* ACLK Source Select 2 */
#define SELA_3              (0x0300)   /* ACLK Source Select 3 */
#define SELA__HFCLK         (0x0000)   /* ACLK Source Select HFCLK */
#define SELA__LFCLK         (0x0100)   /* ACLK Source Select LFCLK */
#define SELA__CLKIN         (0x0200)   /* ACLK Source Select CLKIN */


/* CCSCTL5 Control Bits */
#define DIVM0               (0x0001)   /* MCLK Divider Bit: 0 */
#define DIVM1               (0x0002)   /* MCLK Divider Bit: 1 */
#define DIVM2               (0x0004)   /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DIVS0               (0x0010)   /* SMCLK Divider Bit: 0 */
#define DIVS1               (0x0020)   /* SMCLK Divider Bit: 1 */
#define DIVS2               (0x0040)   /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define DIVA0               (0x0100)   /* ACLK Divider Bit: 0 */
#define DIVA1               (0x0200)   /* ACLK Divider Bit: 1 */
#define DIVA2               (0x0400)   /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CCSCTL5 Control Bits */
#define DIVM0_L             (0x0001)   /* MCLK Divider Bit: 0 */
#define DIVM1_L             (0x0002)   /* MCLK Divider Bit: 1 */
#define DIVM2_L             (0x0004)   /* MCLK Divider Bit: 2 */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
#define DIVS0_L             (0x0010)   /* SMCLK Divider Bit: 0 */
#define DIVS1_L             (0x0020)   /* SMCLK Divider Bit: 1 */
#define DIVS2_L             (0x0040)   /* SMCLK Divider Bit: 2 */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* CCSCTL5 Control Bits */
//#define RESERVED            (0x0004)    /* RESERVED */
//#define RESERVED            (0x0008)    /* RESERVED */
//#define RESERVED            (0x0040)    /* RESERVED */
//#define RESERVED            (0x0080)    /* RESERVED */
#define DIVA0_H             (0x0001)   /* ACLK Divider Bit: 0 */
#define DIVA1_H             (0x0002)   /* ACLK Divider Bit: 1 */
#define DIVA2_H             (0x0004)   /* ACLK Divider Bit: 2 */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

#define DIVM_0              (0x0000)    /* MCLK Source Divider 0 */
#define DIVM_1              (0x0001)    /* MCLK Source Divider 1 */
#define DIVM_2              (0x0002)    /* MCLK Source Divider 2 */
#define DIVM_3              (0x0003)    /* MCLK Source Divider 3 */
#define DIVM_4              (0x0004)    /* MCLK Source Divider 4 */
#define DIVM_5              (0x0005)    /* MCLK Source Divider 5 */
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
#define DIVA__1             (0x0000)    /* ACLK Source Divider f(ACLK)/1 */
#define DIVA__2             (0x0100)    /* ACLK Source Divider f(ACLK)/2 */
#define DIVA__4             (0x0200)    /* ACLK Source Divider f(ACLK)/4 */
#define DIVA__8             (0x0300)    /* ACLK Source Divider f(ACLK)/8 */
#define DIVA__16            (0x0400)    /* ACLK Source Divider f(ACLK)/16 */
#define DIVA__32            (0x0500)    /* ACLK Source Divider f(ACLK)/32 */


/* CCSCTL6 Control Bits */
#define XTOFF               (0x0001)    /* Disable XT oscillator */

/* CCSCTL6 Control Bits */
#define XTOFF_L             (0x0001)    /* Disable XT oscillator */


/* CCSCTL7 Control Bits */
#define XOFFG              (0x0001)    /* X-tal Oscillator Fault Flag */
#define HFOFFG             (0x0002)    /* High Frequency Oscillator Fault Flag */

/* CCSCTL7 Control Bits */
#define XOFFG_L             (0x0001)    /* X-tal Oscillator Fault Flag */
#define HFOFFG_L            (0x0002)    /* High Frequency Oscillator Fault Flag */


/* CCSCTL8 Control Bits */
#define ACLKREQEN           (0x0001)    /* ACLK Clock Request Enable */
#define MCLKREQEN           (0x0002)    /* MCLK Clock Request Enable */
#define SMCLKREQEN          (0x0004)    /* SMCLK Clock Request Enable */

/* CCSCTL8 Control Bits */
#define ACLKREQEN_L         (0x0001)    /* ACLK Clock Request Enable */
#define MCLKREQEN_L         (0x0002)    /* MCLK Clock Request Enable */
#define SMCLKREQEN_L        (0x0004)    /* SMCLK Clock Request Enable */


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
* RF13M (13MHz NFC communication module)
************************************************************/
#define __MSP430_HAS_RF13M__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_RF13M__ 0x0800
#define RF13M_BASE __MSP430_BASEADDRESS_RF13M__

#define RF13MCTL_             0x0800    /* RF13M Control register */
sfrb(RF13MCTL_L , RF13MCTL_);
sfrb(RF13MCTL_H , RF13MCTL_+1);
sfrw(RF13MCTL, RF13MCTL_);
#define RF13MINT_             0x0802    /* RF13M Interrupt Enable/Flag register */
sfrb(RF13MINT_L , RF13MINT_);
sfrb(RF13MINT_H , RF13MINT_+1);
sfrw(RF13MINT, RF13MINT_);
#define RF13MIV_              0x0804    /* RF13M Interrupt Vector register */
sfrb(RF13MIV_L , RF13MIV_);
sfrb(RF13MIV_H , RF13MIV_+1);
sfrw(RF13MIV, RF13MIV_);
#define RF13MRXF_             0x0806    /* RF13M Receive Data FIFO register */
sfrb(RF13MRXF_L , RF13MRXF_);
sfrb(RF13MRXF_H , RF13MRXF_+1);
sfrw(RF13MRXF, RF13MRXF_);
#define RF13MTXF_             0x0808    /* RF13M Transmit Data FIFO register */
sfrb(RF13MTXF_L , RF13MTXF_);
sfrb(RF13MTXF_H , RF13MTXF_+1);
sfrw(RF13MTXF, RF13MTXF_);
#define RF13MCRC_             0x080A    /* RF13M CRC accumulator register */
sfrb(RF13MCRC_L , RF13MCRC_);
sfrb(RF13MCRC_H , RF13MCRC_+1);
sfrw(RF13MCRC, RF13MCRC_);
#define RF13MFIFOFL_          0x080C    /* RF13M Receive/Transmit Data FIFO Fill Level register */
sfrb(RF13MFIFOFL_L , RF13MFIFOFL_);
sfrb(RF13MFIFOFL_H , RF13MFIFOFL_+1);
sfrw(RF13MFIFOFL, RF13MFIFOFL_);
#define RF13MWMCFG_           0x080E    /* RF13M Receive High/Transmit Low Watermark configuration register */
sfrb(RF13MWMCFG_L , RF13MWMCFG_);
sfrb(RF13MWMCFG_H , RF13MWMCFG_+1);
sfrw(RF13MWMCFG, RF13MWMCFG_);
#define RF13MRXBUF_           0x0820    /* RF13M Receive data buffer */
sfrb(RF13MRXBUF_L , RF13MRXBUF_);
sfrb(RF13MRXBUF_H , RF13MRXBUF_+1);
sfrw(RF13MRXBUF, RF13MRXBUF_);
#define RF13MTXBUF_           0x0840    /* RF13M Transmit data buffer */
sfrb(RF13MTXBUF_L , RF13MTXBUF_);
sfrb(RF13MTXBUF_H , RF13MTXBUF_+1);
sfrw(RF13MTXBUF, RF13MTXBUF_);


/* RF13MCTL Control Bits */
#define RF13MRXEN            (0x0001)  /* RF13M Enable reception of downlink */
#define RF13MTXEN            (0x0002)  /* RF13M Enable transmission of uplink */
#define RF13MRFTOEN          (0x0004)  /* RF13M Enable RF Field timeout detection */
#define RF13MSC              (0x0010)  /* RF13M Manually select uplink subcarrier setting */
#define RF13MDR              (0x0020)  /* RF13M Manually select uplink data rate */
#define RF13MMCFG            (0x0080)  /* RF13M Enable manual configuration of ISO15693 uplink parameters */
#define RF13MCRCOK           (0x0100)  /* RF13M CRC valued correct or not flag */
#define RF13MHFD             (0x0200)  /* RF13M Indicates if a HF field was detected since last readout */
#define RF13MRXIWOC          (0x2000)  /* RF13M Gate RX interrupt with CRC OK */
#define RF13MBE              (0x4000)  /* RF13M Select between big or little endian mode */

/* RF13MCTL Control Bits */
#define RF13MRXEN_L         (0x0001)  /* RF13M Enable reception of downlink */
#define RF13MTXEN_L         (0x0002)  /* RF13M Enable transmission of uplink */
#define RF13MRFTOEN_L       (0x0004)  /* RF13M Enable RF Field timeout detection */
#define RF13MSC_L           (0x0010)  /* RF13M Manually select uplink subcarrier setting */
#define RF13MDR_L           (0x0020)  /* RF13M Manually select uplink data rate */
#define RF13MMCFG_L         (0x0080)  /* RF13M Enable manual configuration of ISO15693 uplink parameters */

/* RF13MCTL Control Bits */
#define RF13MCRCOK_H        (0x0001)  /* RF13M CRC valued correct or not flag */
#define RF13MHFD_H          (0x0002)  /* RF13M Indicates if a HF field was detected since last readout */
#define RF13MRXIWOC_H       (0x0020)  /* RF13M Gate RX interrupt with CRC OK */
#define RF13MBE_H           (0x0040)  /* RF13M Select between big or little endian mode */

/* RF13MCTL Definitions */


/* RF13MINT Control Bits */
#define RF13MRXIFG           (0x0001)  /* RF13M Receive downlink done interrupt flag */
#define RF13MTXIFG           (0x0002)  /* RF13M Uplink transmission done interrupt flag */
#define RF13MRXWMIFG         (0x0004)  /* RF13M RX FIFO high watermark reached interrupt flag */
#define RF13MTXWMIFG         (0x0008)  /* RF13M TX FIFO low watermark reached interrupt flag */
#define RF13MSLIFG           (0x0010)  /* RF13M RX Slot marker interrupt flag */
#define RF13MOUFLIFG         (0x0020)  /* RF13M Over-/Under-flow condition detected */
#define RF13MRXEIFG          (0x0040)  /* RF13M RX Error detected */
#define RF13MRFTOIFG         (0x0080)  /* RF13M RF Timeout detected */
#define RF13MRXIE            (0x0100)  /* RF13M RX done interrupt enable */
#define RF13MTXIE            (0x0200)  /* RF13M TX done interrupt enable */
#define RF13MRXWMIE          (0x0400)  /* RF13M RX FIFO high watermark interrupt enable */
#define RF13MTXWMIE          (0x0800)  /* RF13M TX FIFO low watermark interrupt enable */
#define RF13MSLIE            (0x1000)  /* RF13M ISO15693 Slot marker interrupt enable */
#define RX13MOUFLIE          (0x2000)  /* RF13M Over-/Under-flow interrupt enable */
#define RX13MRXEIE           (0x4000)  /* RF13M RX Error interrupt enable */
#define RX13MRFTOIE          (0x8000)  /* RF13M RF Timeout detected interrupt enable */

/* RF13MINT Control Bits */
#define RF13MRXIFG_L        (0x0001)  /* RF13M Receive downlink done interrupt flag */
#define RF13MTXIFG_L        (0x0002)  /* RF13M Uplink transmission done interrupt flag */
#define RF13MRXWMIFG_L      (0x0004)  /* RF13M RX FIFO high watermark reached interrupt flag */
#define RF13MTXWMIFG_L      (0x0008)  /* RF13M TX FIFO low watermark reached interrupt flag */
#define RF13MSLIFG_L        (0x0010)  /* RF13M RX Slot marker interrupt flag */
#define RF13MOUFLIFG_L      (0x0020)  /* RF13M Over-/Under-flow condition detected */
#define RF13MRXEIFG_L       (0x0040)  /* RF13M RX Error detected */
#define RF13MRFTOIFG_L      (0x0080)  /* RF13M RF Timeout detected */

/* RF13MINT Control Bits */
#define RF13MRXIE_H         (0x0001)  /* RF13M RX done interrupt enable */
#define RF13MTXIE_H         (0x0002)  /* RF13M TX done interrupt enable */
#define RF13MRXWMIE_H       (0x0004)  /* RF13M RX FIFO high watermark interrupt enable */
#define RF13MTXWMIE_H       (0x0008)  /* RF13M TX FIFO low watermark interrupt enable */
#define RF13MSLIE_H         (0x0010)  /* RF13M ISO15693 Slot marker interrupt enable */
#define RX13MOUFLIE_H       (0x0020)  /* RF13M Over-/Under-flow interrupt enable */
#define RX13MRXEIE_H        (0x0040)  /* RF13M RX Error interrupt enable */
#define RX13MRFTOIE_H       (0x0080)  /* RF13M RF Timeout detected interrupt enable */


/* RF13MIV Control Bits */
#define RF13MIV0             (0x0002)  /* RF13M Interrupt vector value bit: 0 */
#define RF13MIV1             (0x0004)  /* RF13M Interrupt vector value bit: 1 */
#define RF13MIV2             (0x0008)  /* RF13M Interrupt vector value bit: 2 */
#define RF13MIV3             (0x0010)  /* RF13M Interrupt vector value bit: 3 */

/* RF13MIV Control Bits */
#define RF13MIV0_L          (0x0002)  /* RF13M Interrupt vector value bit: 0 */
#define RF13MIV1_L          (0x0004)  /* RF13M Interrupt vector value bit: 1 */
#define RF13MIV2_L          (0x0008)  /* RF13M Interrupt vector value bit: 2 */
#define RF13MIV3_L          (0x0010)  /* RF13M Interrupt vector value bit: 3 */

/* RF13MIV Definitions */
#define RF13MIV__NONE        (0x0000)  /* No Interrupt pending */
#define RF13MIV__RX          (0x0002)  /* RF13MRXIFG receive done */
#define RF13MIV__TX          (0x0004)  /* RF13MTXIFG transmit done*/
#define RF13MIV__RXWM        (0x0006)  /* RF13MRXWMIFG RX high water mark */
#define RF13MIV__TXWM        (0x0008)  /* RF13MTXWMIFG TX low watermark */
#define RF13MIV__SL          (0x000A)  /* RF13MSLIFG slotmarker */
#define RF13MIV__OUFL        (0x000C)  /* RF13MOUFLIFG over/underflow */
#define RF13MIV__RXE         (0x000E)  /* RF13MRXEIFG RX error */
#define RF13MIV__RFTO        (0x0010)  /* RF13MRFTOIFG RF timeout error */


/* RF13MRXF Control Bits */

/* RF13MTXF Control Bits */

/* RF13MCRC Control Bits */

/* RF13MFIFOFL Control Bits */

/* RF13MWMCFG Control Bits */


/* RF13MWMCFG Definitions */
#define RF13MRXWM_0          (0x0000)  /* RF13M RX Watermark level: 0  */
#define RF13MRXWM_1          (0x0001)  /* RF13M RX Watermark level: 1  */
#define RF13MRXWM_2          (0x0002)  /* RF13M RX Watermark level: 2  */
#define RF13MRXWM_3          (0x0003)  /* RF13M RX Watermark level: 3  */
#define RF13MRXWM_4          (0x0004)  /* RF13M RX Watermark level: 4  */
#define RF13MRXWM_5          (0x0005)  /* RF13M RX Watermark level: 5  */
#define RF13MRXWM_6          (0x0006)  /* RF13M RX Watermark level: 6  */
#define RF13MRXWM_7          (0x0007)  /* RF13M RX Watermark level: 7  */
#define RF13MRXWM_8          (0x0008)  /* RF13M RX Watermark level: 8  */
#define RF13MRXWM_9          (0x0009)  /* RF13M RX Watermark level: 9  */
#define RF13MRXWM_10         (0x000A)  /* RF13M RX Watermark level: 10 */
#define RF13MRXWM_11         (0x000B)  /* RF13M RX Watermark level: 11 */
#define RF13MRXWM_12         (0x000C)  /* RF13M RX Watermark level: 12 */
#define RF13MRXWM_13         (0x000D)  /* RF13M RX Watermark level: 13 */
#define RF13MRXWM_14         (0x000E)  /* RF13M RX Watermark level: 14 */
#define RF13MRXWM_15         (0x000F)  /* RF13M RX Watermark level: 15 */
#define RF13MRXWM_16         (0x0010)  /* RF13M RX Watermark level: 16 */
#define RF13MRXWM_17         (0x0011)  /* RF13M RX Watermark level: 17 */
#define RF13MRXWM_18         (0x0012)  /* RF13M RX Watermark level: 18 */
#define RF13MRXWM_19         (0x0013)  /* RF13M RX Watermark level: 19 */
#define RF13MRXWM_20         (0x0014)  /* RF13M RX Watermark level: 20 */
#define RF13MRXWM_21         (0x0015)  /* RF13M RX Watermark level: 21 */
#define RF13MRXWM_22         (0x0016)  /* RF13M RX Watermark level: 22 */
#define RF13MRXWM_23         (0x0017)  /* RF13M RX Watermark level: 23 */
#define RF13MRXWM_24         (0x0018)  /* RF13M RX Watermark level: 24 */
#define RF13MRXWM_25         (0x0019)  /* RF13M RX Watermark level: 25 */
#define RF13MRXWM_26         (0x001A)  /* RF13M RX Watermark level: 26 */
#define RF13MRXWM_27         (0x001B)  /* RF13M RX Watermark level: 27 */
#define RF13MRXWM_28         (0x001C)  /* RF13M RX Watermark level: 28 */
#define RF13MRXWM_29         (0x001D)  /* RF13M RX Watermark level: 29 */
#define RF13MRXWM_30         (0x001E)  /* RF13M RX Watermark level: 30 */
#define RF13MRXWM_31         (0x001F)  /* RF13M RX Watermark level: 31 */

#define RF13MTXWM_0          (0x0000)  /* RF13M TX Watermark level: 0  */
#define RF13MTXWM_1          (0x0100)  /* RF13M TX Watermark level: 1  */
#define RF13MTXWM_2          (0x0200)  /* RF13M TX Watermark level: 2  */
#define RF13MTXWM_3          (0x0300)  /* RF13M TX Watermark level: 3  */
#define RF13MTXWM_4          (0x0400)  /* RF13M TX Watermark level: 4  */
#define RF13MTXWM_5          (0x0500)  /* RF13M TX Watermark level: 5  */
#define RF13MTXWM_6          (0x0600)  /* RF13M TX Watermark level: 6  */
#define RF13MTXWM_7          (0x0700)  /* RF13M TX Watermark level: 7  */
#define RF13MTXWM_8          (0x0800)  /* RF13M TX Watermark level: 8  */
#define RF13MTXWM_9          (0x0900)  /* RF13M TX Watermark level: 9  */
#define RF13MTXWM_10         (0x0A00)  /* RF13M TX Watermark level: 10 */
#define RF13MTXWM_11         (0x0B00)  /* RF13M TX Watermark level: 11 */
#define RF13MTXWM_12         (0x0C00)  /* RF13M TX Watermark level: 12 */
#define RF13MTXWM_13         (0x0D00)  /* RF13M TX Watermark level: 13 */
#define RF13MTXWM_14         (0x0E00)  /* RF13M TX Watermark level: 14 */
#define RF13MTXWM_15         (0x0F00)  /* RF13M TX Watermark level: 15 */
#define RF13MTXWM_16         (0x1000)  /* RF13M TX Watermark level: 16 */
#define RF13MTXWM_17         (0x1100)  /* RF13M TX Watermark level: 17 */
#define RF13MTXWM_18         (0x1200)  /* RF13M TX Watermark level: 18 */
#define RF13MTXWM_19         (0x1300)  /* RF13M TX Watermark level: 19 */
#define RF13MTXWM_20         (0x1400)  /* RF13M TX Watermark level: 20 */
#define RF13MTXWM_21         (0x1500)  /* RF13M TX Watermark level: 21 */
#define RF13MTXWM_22         (0x1600)  /* RF13M TX Watermark level: 22 */
#define RF13MTXWM_23         (0x1700)  /* RF13M TX Watermark level: 23 */
#define RF13MTXWM_24         (0x1800)  /* RF13M TX Watermark level: 24 */
#define RF13MTXWM_25         (0x1900)  /* RF13M TX Watermark level: 25 */
#define RF13MTXWM_26         (0x1A00)  /* RF13M TX Watermark level: 26 */
#define RF13MTXWM_27         (0x1B00)  /* RF13M TX Watermark level: 27 */
#define RF13MTXWM_28         (0x1C00)  /* RF13M TX Watermark level: 28 */
#define RF13MTXWM_29         (0x1D00)  /* RF13M TX Watermark level: 29 */
#define RF13MTXWM_30         (0x1E00)  /* RF13M TX Watermark level: 30 */
#define RF13MTXWM_31         (0x1F00)  /* RF13M TX Watermark level: 31 */

/************************************************************
* SD14 (14 Bit sigma delta ADC)
************************************************************/
#define __MSP430_HAS_SD14__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SD14__ 0x0700
#define SD14_BASE __MSP430_BASEADDRESS_SD14__

#define SD14CTL0_             0x0700    /* SD14 Control Register 0 */
sfrb(SD14CTL0_L , SD14CTL0_);
sfrb(SD14CTL0_H , SD14CTL0_+1);
sfrw(SD14CTL0, SD14CTL0_);
#define SD14CTL1_             0x0702    /* SD14 Control Register 1 */
sfrb(SD14CTL1_L , SD14CTL1_);
sfrb(SD14CTL1_H , SD14CTL1_+1);
sfrw(SD14CTL1, SD14CTL1_);
#define SD14MEM0_             0x0704    /* SD14 Conversion Result */
sfrb(SD14MEM0_L , SD14MEM0_);
sfrb(SD14MEM0_H , SD14MEM0_+1);
sfrw(SD14MEM0, SD14MEM0_);
#define SD14MEM1_             0x0706    /* SD14 Intermediate Conversion Result */
sfrb(SD14MEM1_L , SD14MEM1_);
sfrb(SD14MEM1_H , SD14MEM1_+1);
sfrw(SD14MEM1, SD14MEM1_);
#define SD14MEM2_             0x0708    /* SD14 Intermediate Conversion Result */
sfrb(SD14MEM2_L , SD14MEM2_);
sfrb(SD14MEM2_H , SD14MEM2_+1);
sfrw(SD14MEM2, SD14MEM2_);
#define SD14MEM3_             0x070A    /* SD14 Intermediate Conversion Result */
sfrb(SD14MEM3_L , SD14MEM3_);
sfrb(SD14MEM3_H , SD14MEM3_+1);
sfrw(SD14MEM3, SD14MEM3_);
#define SD14IV_               0x070C    /* SD14 Interrupt Vector */
sfrb(SD14IV_L , SD14IV_);
sfrb(SD14IV_H , SD14IV_+1);
sfrw(SD14IV, SD14IV_);


/* SD14CTL0 Control Bits */
#define SD14EN               (0x0001)  /* SD14 Module enable */
#define SD14SC               (0x0004)  /* SD14 Enable transmission of uplink */
#define SD14SGL              (0x0008)  /* SD14 Conversion mode select */
#define SD14SSEL0            (0x0010)  /* SD14 Sigma-Delta Clock source Bit: 0 */
#define SD14SSEL1            (0x0020)  /* SD14 Sigma-Delta Clock source Bit: 1 */
#define SD14DIV0             (0x0040)  /* SD14 Sigma-Delta Clock divider Bit: 0 */
#define SD14DIV1             (0x0080)  /* SD14 Sigma-Delta Clock divider Bit: 1 */
#define SD14IE               (0x0100)  /* SD14 interrupt enable */
#define SD14IFG              (0x0200)  /* SD14 interrupt flag */
#define SD14OVIE             (0x0400)  /* SD14 overflow interrupt enable */
#define SD14OVIFG            (0x0800)  /* SD14 overflow interrupt flag */
#define VIRTGND              (0x1000)  /* Defines virtual ground */

/* SD14CTL0 Control Bits */
#define SD14EN_L            (0x0001)  /* SD14 Module enable */
#define SD14SC_L            (0x0004)  /* SD14 Enable transmission of uplink */
#define SD14SGL_L           (0x0008)  /* SD14 Conversion mode select */
#define SD14SSEL0_L         (0x0010)  /* SD14 Sigma-Delta Clock source Bit: 0 */
#define SD14SSEL1_L         (0x0020)  /* SD14 Sigma-Delta Clock source Bit: 1 */
#define SD14DIV0_L          (0x0040)  /* SD14 Sigma-Delta Clock divider Bit: 0 */
#define SD14DIV1_L          (0x0080)  /* SD14 Sigma-Delta Clock divider Bit: 1 */

/* SD14CTL0 Control Bits */
#define SD14IE_H            (0x0001)  /* SD14 interrupt enable */
#define SD14IFG_H           (0x0002)  /* SD14 interrupt flag */
#define SD14OVIE_H          (0x0004)  /* SD14 overflow interrupt enable */
#define SD14OVIFG_H         (0x0008)  /* SD14 overflow interrupt flag */
#define VIRTGND_H           (0x0010)  /* Defines virtual ground */

/* SD14CTL0 Definitions */
#define SD14SSEL_0           (0x0000)  /* SD14 Sigma-Delta Clock select: 0 */
#define SD14SSEL_1           (0x0010)  /* SD14 Sigma-Delta Clock select: 1 */
#define SD14SSEL_2           (0x0020)  /* SD14 Sigma-Delta Clock select: 2 */
#define SD14SSEL_3           (0x0030)  /* SD14 Sigma-Delta Clock select: 3 */
#define SD14SSEL__ACLK       (0x0000)  /* SD14 Sigma-Delta Clock select: ACLK */
#define SD14SSEL__SMCLK      (0x0010)  /* SD14 Sigma-Delta Clock select: SMCLK */
#define SD14SSEL__MCLK       (0x0020)  /* SD14 Sigma-Delta Clock select: MCLK */
#define SD14SSEL__VLOCLK     (0x0030)  /* SD14 Sigma-Delta Clock select: VLOCLK */
#define SD14DIV_0            (0x0000)  /* SD14 Sigma-Delta Clock divider select: 0 */
#define SD14DIV_1            (0x0040)  /* SD14 Sigma-Delta Clock divider select: 1 */
#define SD14DIV_2            (0x0080)  /* SD14 Sigma-Delta Clock divider select: 2 */
#define SD14DIV_3            (0x00C0)  /* SD14 Sigma-Delta Clock divider select: 3 */
#define SD14DIV__8           (0x0000)  /* SD14 Sigma-Delta Clock divider select: 8 */
#define SD14DIV__16          (0x0040)  /* SD14 Sigma-Delta Clock divider select: 16 */
#define SD14DIV__32          (0x0080)  /* SD14 Sigma-Delta Clock divider select: 32 */
#define SD14DIV__64          (0x00C0)  /* SD14 Sigma-Delta Clock divider select: 64 */


/* SD14CTL1 Control Bits */
#define SD14INCH0            (0x0001)  /* SD14 channel input selection Bit: 0 */
#define SD14INCH1            (0x0002)  /* SD14 channel input selection Bit: 1 */
#define SD14INCH2            (0x0004)  /* SD14 channel input selection Bit: 2 */
#define SD14GAIN0            (0x0008)  /* SD14 preamplifier gain Bit: 0 */
#define SD14GAIN1            (0x0010)  /* SD14 preamplifier gain Bit: 1 */
#define SD14INTDLY0          (0x0040)  /* SD14 Interrupt delay Bit: 0 */
#define SD14INTDLY1          (0x0080)  /* SD14 Interrupt delay Bit: 1 */
#define SD14FILT             (0x0100)  /* SD14 digital filter selection */
#define SD14RATE0            (0x0200)  /* SD14 rate change factor selection Bit: 0 */
#define SD14RATE1            (0x0400)  /* SD14 rate change factor selection Bit: 1 */
#define SD14RATE2            (0x0800)  /* SD14 rate change factor selection Bit: 2 */
#define SD14UNI              (0x1000)  /* SD14 Unipolar format enable Bit: 0 */
#define SD14TC               (0x2000)  /* SD14 Two's complement format enable Bit: 0 */
#define SD14RBEN0            (0x4000)  /* SD14 Resistive bias on TEMP1 enable */
#define SD14RBEN1            (0x8000)  /* SD14 Resistive bias on TEMP2 enable */

/* SD14CTL1 Control Bits */
#define SD14INCH0_L         (0x0001)  /* SD14 channel input selection Bit: 0 */
#define SD14INCH1_L         (0x0002)  /* SD14 channel input selection Bit: 1 */
#define SD14INCH2_L         (0x0004)  /* SD14 channel input selection Bit: 2 */
#define SD14GAIN0_L         (0x0008)  /* SD14 preamplifier gain Bit: 0 */
#define SD14GAIN1_L         (0x0010)  /* SD14 preamplifier gain Bit: 1 */
#define SD14INTDLY0_L       (0x0040)  /* SD14 Interrupt delay Bit: 0 */
#define SD14INTDLY1_L       (0x0080)  /* SD14 Interrupt delay Bit: 1 */

/* SD14CTL1 Control Bits */
#define SD14FILT_H          (0x0001)  /* SD14 digital filter selection */
#define SD14RATE0_H         (0x0002)  /* SD14 rate change factor selection Bit: 0 */
#define SD14RATE1_H         (0x0004)  /* SD14 rate change factor selection Bit: 1 */
#define SD14RATE2_H         (0x0008)  /* SD14 rate change factor selection Bit: 2 */
#define SD14UNI_H           (0x0010)  /* SD14 Unipolar format enable Bit: 0 */
#define SD14TC_H            (0x0020)  /* SD14 Two's complement format enable Bit: 0 */
#define SD14RBEN0_H         (0x0040)  /* SD14 Resistive bias on TEMP1 enable */
#define SD14RBEN1_H         (0x0080)  /* SD14 Resistive bias on TEMP2 enable */

/* SD14CTL1 Definitions */
#define SD14INCH_0           (0x0000)  /* SD14 channel input select: 0 */
#define SD14INCH_1           (0x0001)  /* SD14 channel input select: 1 */
#define SD14INCH_2           (0x0002)  /* SD14 channel input select: 2 */
#define SD14INCH_3           (0x0003)  /* SD14 channel input select: 3 */
#define SD14INCH_4           (0x0004)  /* SD14 channel input select: 4 */
#define SD14INCH_5           (0x0005)  /* SD14 channel input select: 5 */
#define SD14INCH_6           (0x0006)  /* SD14 channel input select: 6 */
#define SD14INCH_7           (0x0007)  /* SD14 channel input select: 7 */
#define SD14INCH__A0         (0x0000)  /* SD14 channel input select: 0 */
#define SD14INCH__A1         (0x0001)  /* SD14 channel input select: 1 */
#define SD14INCH__A2         (0x0002)  /* SD14 channel input select: 2 */
#define SD14INCH__A3         (0x0003)  /* SD14 channel input select: 3 */
#define SD14INCH__A4         (0x0004)  /* SD14 channel input select: 4 */
#define SD14INCH__A5         (0x0005)  /* SD14 channel input select: 5 */
#define SD14INCH__A6         (0x0006)  /* SD14 channel input select: 6 */
#define SD14INCH__A7         (0x0007)  /* SD14 channel input select: 7 */
#define SD14INCH__ADC0       (0x0000)  /* SD14 channel input select: GS-CNTR Electrode */
#define SD14INCH__TEMP       (0x0001)  /* SD14 channel input select: Internal Temp Sensor */
#define SD14INCH__INTTEMP    (0x0001)  /* SD14 channel input select: Internal Temp Sensor */
#define SD14INCH__ADC2       (0x0002)  /* SD14 channel input select: Thermistor/Temp2 */
#define SD14INCH__TEMP2      (0x0002)  /* SD14 channel input select: Thermistor/Temp2 */
#define SD14INCH__ADC1       (0x0003)  /* SD14 channel input select: Reference Resistor/Temp1 */
#define SD14INCH__TEMP1      (0x0003)  /* SD14 channel input select: Reference Resistor/Temp1 */
#define SD14INCH__SHORT      (0x0007)  /* SD14 channel input select: Shorted */
#define SD14INCH__SVSS       (0x0007)  /* SD14 channel input select: Shorted */
#define SD14GAIN_0           (0x0000)  /* SD14 preamplifier gain select: 0 */
#define SD14GAIN_1           (0x0008)  /* SD14 preamplifier gain select: 1 */
#define SD14GAIN_2           (0x0010)  /* SD14 preamplifier gain select: 2 */
#define SD14GAIN_3           (0x0018)  /* SD14 preamplifier gain select: 3 */
#define SD14GAIN__1          (0x0000)  /* SD14 preamplifier gain select: 1x */
#define SD14GAIN__2          (0x0008)  /* SD14 preamplifier gain select: 2x */
#define SD14GAIN__4          (0x0010)  /* SD14 preamplifier gain select: 4x */
#define SD14GAIN__8          (0x0018)  /* SD14 preamplifier gain select: 8x */
#define SD14INTDLY_0         (0x0000)  /* SD14 Interrupt delay select: 0 */
#define SD14INTDLY_1         (0x0040)  /* SD14 Interrupt delay select: 1 */
#define SD14INTDLY_2         (0x0080)  /* SD14 Interrupt delay select: 2 */
#define SD14INTDLY_3         (0x00C0)  /* SD14 Interrupt delay select: 3 */
#define SD14INTDLY__1st      (0x0000)  /* SD14 Interrupt delay select: Int after 1st */
#define SD14INTDLY__2nd      (0x0040)  /* SD14 Interrupt delay select: Int after 2nd */
#define SD14INTDLY__3rd      (0x0080)  /* SD14 Interrupt delay select: Int after 3rd */
#define SD14INTDLY__4th      (0x00C0)  /* SD14 Interrupt delay select: Int after 4th */
#define SD14FILT__CIC        (0x0000)  /* SD14 filter select CIC */
#define SD14FILT__AVG        (0x0100)  /* SD14 filter select moving average */
#define SD14RATE_0           (0x0000)  /* SD14 rate change factor: 0 */
#define SD14RATE_1           (0x0200)  /* SD14 rate change factor: 1 */
#define SD14RATE_2           (0x0400)  /* SD14 rate change factor: 2 */
#define SD14RATE_3           (0x0600)  /* SD14 rate change factor: 3 */
#define SD14RATE_4           (0x0800)  /* SD14 rate change factor: 4 */
#define SD14RATE_5           (0x0A00)  /* SD14 rate change factor: 5 */
#define SD14RATE_6           (0x0C00)  /* SD14 rate change factor: 6 */
#define SD14RATE_7           (0x0E00)  /* SD14 rate change factor: 7 */
#define SD14RATE__CIC32      (0x0000)  /* SD14 rate change factor CIC 32 */
#define SD14RATE__CIC64      (0x0200)  /* SD14 rate change factor CIC 64 */
#define SD14RATE__CIC128     (0x0400)  /* SD14 rate change factor CIC 128 */
#define SD14RATE__CIC256     (0x0600)  /* SD14 rate change factor CIC 256 */
#define SD14RATE__CIC512     (0x0800)  /* SD14 rate change factor CIC 512 */
#define SD14RATE__CIC1024    (0x0A00)  /* SD14 rate change factor CIC 1024 */
#define SD14RATE__CIC2048    (0x0C00)  /* SD14 rate change factor CIC 2048 */
#define SD14RATE__AVG4096    (0x0000)  /* SD14 rate change factor AVG 4096 */
#define SD14RATE__AVG8192    (0x0200)  /* SD14 rate change factor AVG 8192 */
#define SD14RATE__AVG16384   (0x0400)  /* SD14 rate change factor AVG 16384 */
#define SD14RATE__AVG32768   (0x0600)  /* SD14 rate change factor AVG 32768 */


/* SD14IV Control Bits */
#define SD14IV0              (0x0002)  /* SD14IV Interrupt vector value bit: 0 */
#define SD14IV1              (0x0004)  /* SD14IV Interrupt vector value bit: 1 */

/* SD14IV Control Bits */
#define SD14IV0_L           (0x0002)  /* SD14IV Interrupt vector value bit: 0 */
#define SD14IV1_L           (0x0004)  /* SD14IV Interrupt vector value bit: 1 */


/* SD14IV Definitions */
#define SD14IV__NONE         (0x0000)  /* No Interrupt pending */
#define SD14IV__OV           (0x0002)  /* SD14OVIFG overflow */
#define SD14IV__RES          (0x0004)  /* SD14IFG new result*/

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
* COMPACT SYS - System Module
************************************************************/
#define __MSP430_HAS_SYS__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_SYS__ 0x0180
#define SYS_BASE __MSP430_BASEADDRESS_SYS__

#define SYSCTL_               0x0180    /* System control */
sfrb(SYSCTL_L , SYSCTL_);
sfrb(SYSCTL_H , SYSCTL_+1);
sfrw(SYSCTL, SYSCTL_);
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
#define SYSCNF_               0x0190    /* System Configuration Register */
sfrb(SYSCNF_L , SYSCNF_);
sfrb(SYSCNF_H , SYSCNF_+1);
sfrw(SYSCNF, SYSCNF_);

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
//#define RESERVED          (0x0002)  /* SYS - Reserved */
//#define RESERVED          (0x0004)  /* SYS - Reserved */
//#define RESERVED          (0x0008)  /* SYS - Reserved */
//#define RESERVED          (0x0010)  /* SYS - Reserved */
#define SYSJTAGPIN          (0x0020)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
//#define RESERVED          (0x0100)  /* SYS - Reserved */
//#define RESERVED          (0x0200)  /* SYS - Reserved */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

/* SYSCTL Control Bits */
#define SYSRIVECT_L         (0x0001)  /* SYS - RAM based interrupt vectors */
//#define RESERVED          (0x0002)  /* SYS - Reserved */
//#define RESERVED          (0x0004)  /* SYS - Reserved */
//#define RESERVED          (0x0008)  /* SYS - Reserved */
//#define RESERVED          (0x0010)  /* SYS - Reserved */
#define SYSJTAGPIN_L        (0x0020)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
//#define RESERVED          (0x0100)  /* SYS - Reserved */
//#define RESERVED          (0x0200)  /* SYS - Reserved */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */



/* SYSJMBC Control Bits */
#define JMBIN0FG            (0x0001)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG            (0x0002)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG           (0x0004)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG           (0x0008)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE             (0x0010)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED          (0x0020)  /* SYS - Reserved */
#define JMBCLR0OFF          (0x0040)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF          (0x0080)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED          (0x0100)  /* SYS - Reserved */
//#define RESERVED          (0x0200)  /* SYS - Reserved */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
#define JMBIN0FG_L          (0x0001)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG_L          (0x0002)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG_L         (0x0004)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG_L         (0x0008)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE_L           (0x0010)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED          (0x0020)  /* SYS - Reserved */
#define JMBCLR0OFF_L        (0x0040)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF_L        (0x0080)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED          (0x0100)  /* SYS - Reserved */
//#define RESERVED          (0x0200)  /* SYS - Reserved */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */


/* SYSCNF Control Bits */
//#define RESERVED          (0x0001)  /* SYS - Reserved */
//#define RESERVED          (0x0002)  /* SYS - Reserved */
//#define RESERVED          (0x0004)  /* SYS - Reserved */
//#define RESERVED          (0x0008)  /* SYS - Reserved */
//#define RESERVED          (0x0010)  /* SYS - Reserved */
//#define RESERVED          (0x0020)  /* SYS - Reserved */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
#define FRAMLOCK0           (0x0100)  /* SYS - FRAM Write protect area 0 */
#define FRAMLOCK1           (0x0200)  /* SYS - FRAM Write protect area 1 */
#define FRAMLOCK2           (0x0400)  /* SYS - FRAM Write protect area 2 */
#define DEVRAMLOCK          (0x0800)  /* SYS - Write protect Re-Mapped ROM development area */
#define ROMDEVMODE          (0x1000)  /* SYS - Enable ROM Developent mode */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

/* SYSCNF Control Bits */
//#define RESERVED          (0x0001)  /* SYS - Reserved */
//#define RESERVED          (0x0002)  /* SYS - Reserved */
//#define RESERVED          (0x0004)  /* SYS - Reserved */
//#define RESERVED          (0x0008)  /* SYS - Reserved */
//#define RESERVED          (0x0010)  /* SYS - Reserved */
//#define RESERVED          (0x0020)  /* SYS - Reserved */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
#define FRAMLOCK0_H         (0x0001)  /* SYS - FRAM Write protect area 0 */
#define FRAMLOCK1_H         (0x0002)  /* SYS - FRAM Write protect area 1 */
#define FRAMLOCK2_H         (0x0004)  /* SYS - FRAM Write protect area 2 */
#define DEVRAMLOCK_H        (0x0008)  /* SYS - Write protect Re-Mapped ROM development area */
#define ROMDEVMODE_H        (0x0010)  /* SYS - Enable ROM Developent mode */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

/************************************************************
* PMM_RF - Power Management System of RF
************************************************************/
#define __MSP430_HAS_PMM_RF__         /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PMM_RF__ 0x0120
#define PMM_BASE __MSP430_BASEADDRESS_PMM_RF__

#define RFPMMCTL0_            0x0120    /* RF PMM Control 0 */
sfrb(RFPMMCTL0_L , RFPMMCTL0_);
sfrb(RFPMMCTL0_H , RFPMMCTL0_+1);
sfrw(RFPMMCTL0, RFPMMCTL0_);
#define RFPMMCTL1_            0x0122    /* RF PMM Control 1 */
sfrb(RFPMMCTL1_L , RFPMMCTL1_);
sfrb(RFPMMCTL1_H , RFPMMCTL1_+1);
sfrw(RFPMMCTL1, RFPMMCTL1_);
#define RFMMIE_               0x0124    /* RF PPM interrupt enable register */
sfrb(RFMMIE_L , RFMMIE_);
sfrb(RFMMIE_H , RFMMIE_+1);
sfrw(RFMMIE, RFMMIE_);
#define RFPMMIFG_             0x0126    /* RF PMM Interrupt Flag */
sfrb(RFPMMIFG_L , RFPMMIFG_);
sfrb(RFPMMIFG_H , RFPMMIFG_+1);
sfrw(RFPMMIFG, RFPMMIFG_);
#define RFPMMIV_              0x0128    /* RF PMM Interrupt vector regiser */
sfrb(RFPMMIV_L , RFPMMIV_);
sfrb(RFPMMIV_H , RFPMMIV_+1);
sfrw(RFPMMIV, RFPMMIV_);

#define RFPMM0              RFPMMCTL0_L /* RF PMM mapping register */
#define RFPMM1              RFPMMCTL0_H /* RF PMM mapping register */
#define RFPMM2              RFPMMCTL1_L /* RF PMM mapping register */
#define RFPMM3              RFPMMCTL1_H /* RF PMM mapping register */
#define RFPMM4              RFMMIE_L    /* RF PMM mapping register */
#define RFPMM5              RFMMIE_H    /* RF PMM mapping register */
#define RFPMM6              RFPMMIFG_L  /* RF PMM mapping register */
#define RFPMM7              RFPMMIFG_H  /* RF PMM mapping register */
#define RFPMM8              RFPMMIV_H   /* RF PMM mapping register */
#define RFPMM9              RFPMMIV_H   /* RF PMM mapping register */

#define PMMPW               (0xA500)    /* PMM Register Write Password */
#define PMMPW_H             (0xA5)      /* PMM Register Write Password for high word access */

/* RFPMMCTL0 Control Bits */
#define RFPMMSWBOR          (0x0004)  /* RF MM Software brownout reset. BOR. Bit: 2 */
#define RFPMMSWPOR          (0x0008)  /* RF PMM Software power-on reset. POR. Bit: 3 */
#define RFPMM_EN_VF_REG     (0x0010)  /* RF PMM Enable voltage regulator for FRAM vddf permanently */
#define RFPMM_EN_BATSWITCH  (0x0020)  /* RF PMM Enable the battery switch */
#define RFPMM_EN_V_DOUB     (0x0040)  /* RF PMM Enable voltage regulator for digital core. VDD */

/* RFPMMCTL0 Control Bits */
#define RFPMMSWBOR_L        (0x0004)  /* RF MM Software brownout reset. BOR. Bit: 2 */
#define RFPMMSWPOR_L        (0x0008)  /* RF PMM Software power-on reset. POR. Bit: 3 */
#define RFPMM_EN_VF_REG_L   (0x0010)  /* RF PMM Enable voltage regulator for FRAM vddf permanently */
#define RFPMM_EN_BATSWITCH_L (0x0020)  /* RF PMM Enable the battery switch */
#define RFPMM_EN_V_DOUB_L   (0x0040)  /* RF PMM Enable voltage regulator for digital core. VDD */


#define RFPMM_EN_BATSWITCH_BYTE  (0x20)  /* RF PMM Enable the battery switch */
#define RFPMM_EN_V_DOUB_BYTE     (0x40)  /* RF PMM Enable voltage regulator for digital core. VDD */

/* RFPMMCTL1 Control Bits */
#define RFPMM_NRESET_VF     (0x0001)  /* RF PMM Low active reset signal of VDDF Bit: 0 */
#define RFPMM_NRESET_VB     (0x0002)  /* RF PMM Low active reset signal of VDDB Bit: 1 */
#define RFPMM_NRESET_VR     (0x0004)  /* RF PMM Low active reset signal of VDDR Bit: 2 */
#define RFPMM_NRESET_VH     (0x0008)  /* RF PMM Low active reset signal of VDDH Bit: 3 */
#define RFPMM_NRESET_V2X    (0x0010)  /* RF PMM Low active reset signal of VDD2X Bit: 4 */
#define RFPMM_BW_VR_VBN     (0x0100)  /* RF PMM indicates position of the bulk switch Bit: 8 */

/* RFPMMCTL1 Control Bits */
#define RFPMM_NRESET_VF_L   (0x0001)  /* RF PMM Low active reset signal of VDDF Bit: 0 */
#define RFPMM_NRESET_VB_L   (0x0002)  /* RF PMM Low active reset signal of VDDB Bit: 1 */
#define RFPMM_NRESET_VR_L   (0x0004)  /* RF PMM Low active reset signal of VDDR Bit: 2 */
#define RFPMM_NRESET_VH_L   (0x0008)  /* RF PMM Low active reset signal of VDDH Bit: 3 */
#define RFPMM_NRESET_V2X_L  (0x0010)  /* RF PMM Low active reset signal of VDD2X Bit: 4 */

/* RFPMMCTL1 Control Bits */
#define RFPMM_BW_VR_VBN_H   (0x0001)  /* RF PMM indicates position of the bulk switch Bit: 8 */

/* RFMMIE Control Bits */
#define RFPMMIEVF           (0x0001)  /* RF PMM Interrupt enable voltage regulator VDDF Bit: 0 */
#define RFPMMIEVB           (0x0002)  /* RF PMM Interrupt enable voltage regulator VDDB Bit: 1 */
#define RFPMMIEVR           (0x0004)  /* RF PMM Interrupt enable voltage regulator VDDR Bit: 2 */
#define RFPMMIEVH           (0x0008)  /* RF PMM Interrupt enable voltage regulator VDDH Bit: 3 */
#define RFPMMIEV2X          (0x0010)  /* RF PMM Interrupt enable voltage regulator VDD2X Bit: 4 */

/* RFMMIE Control Bits */
#define RFPMMIEVF_L         (0x0001)  /* RF PMM Interrupt enable voltage regulator VDDF Bit: 0 */
#define RFPMMIEVB_L         (0x0002)  /* RF PMM Interrupt enable voltage regulator VDDB Bit: 1 */
#define RFPMMIEVR_L         (0x0004)  /* RF PMM Interrupt enable voltage regulator VDDR Bit: 2 */
#define RFPMMIEVH_L         (0x0008)  /* RF PMM Interrupt enable voltage regulator VDDH Bit: 3 */
#define RFPMMIEV2X_L        (0x0010)  /* RF PMM Interrupt enable voltage regulator VDD2X Bit: 4 */


/* RFPMMIFG Control Bits */
#define RFPMMIFGVF          (0x0001)  /* RF PMM Interrupt flag voltage regulator VDDF Bit: 0 */
#define RFPMMIFGVB          (0x0002)  /* RF PMM Interrupt flag voltage regulator VDDB Bit: 1 */
#define RFPMMIFGVR          (0x0004)  /* RF PMM Interrupt flag voltage regulator VDDR Bit: 2 */
#define RFPMMIFGVH          (0x0008)  /* RF PMM Interrupt flag voltage regulator VDDH Bit: 3 */
#define RFPMMIFGV2X         (0x0010)  /* RF PMM Interrupt flag voltage regulator VDD2X Bit: 4 */

/* RFPMMIFG Control Bits */
#define RFPMMIFGVF_L        (0x0001)  /* RF PMM Interrupt flag voltage regulator VDDF Bit: 0 */
#define RFPMMIFGVB_L        (0x0002)  /* RF PMM Interrupt flag voltage regulator VDDB Bit: 1 */
#define RFPMMIFGVR_L        (0x0004)  /* RF PMM Interrupt flag voltage regulator VDDR Bit: 2 */
#define RFPMMIFGVH_L        (0x0008)  /* RF PMM Interrupt flag voltage regulator VDDH Bit: 3 */
#define RFPMMIFGV2X_L       (0x0010)  /* RF PMM Interrupt flag voltage regulator VDD2X Bit: 4 */


/* RFPMMIV Definitions */
#define RFPMMIV_NONE        (0x0000)  /* No Interrupt pending */
#define RFPMMIV_RESET_VDD2X (0x0002)  /* Reset for VDD2X */
#define RFPMMIV_RESET_VDDH  (0x0004)  /* Reset for VDDH */
#define RFPMMIV_RESET_VDDR  (0x0006)  /* Reset for VDDR */
#define RFPMMIV_RESET_VDDB  (0x0008)  /* Reset for VDDB */
#define RFPMMIV_RESET_VDDF  (0x000A)  /* Reser for VDDF */



/* SYSUNIV Definitions */
#define SYSUNIV_NONE       (0x0000)    /* No Interrupt pending */
#define SYSUNIV_NMIIFG     (0x0002)    /* SYSUNIV : NMIIFG */
#define SYSUNIV_OFIFG      (0x0004)    /* SYSUNIV : Osc. Fail - OFIFG */
#define SYSUNIV_SYSBERRIV  (0x0006)    /* SYSUNIV : Bus Error - SYSBERRIV */

/* SYSSNIV Definitions */
#define SYSSNIV_NONE       (0x0000)    /* No Interrupt pending */
#define SYSSNIV_SVMIFG     (0x0002)    /* SYSSNIV : SVMLIFG */
#define SYSSNIV_VMAIFG     (0x0004)    /* SYSSNIV : VMAIFG */
#define SYSSNIV_JMBINIFG   (0x0006)    /* SYSSNIV : JMBINIFG */
#define SYSSNIV_JMBOUTIFG  (0x0008)    /* SYSSNIV : JMBOUTIFG */

/* SYSRSTIV Definitions */
#define SYSRSTIV_NONE      (0x0000)    /* No Interrupt pending */
#define SYSRSTIV_BOR       (0x0002)    /* SYSRSTIV : BOR */
#define SYSRSTIV_SVMBOR    (0x0004)    /* SYSRSTIV : SVMBOR */
#define SYSRSTIV_RSTNMI    (0x0006)    /* SYSRSTIV : RST/NMI */
#define SYSRSTIV_DOBOR     (0x0008)    /* SYSRSTIV : Do BOR */
#define SYSRSTIV_SECYV     (0x000A)    /* SYSRSTIV : Security violation */
#define SYSRSTIV_DOPOR     (0x000C)    /* SYSRSTIV : Do POR */
#define SYSRSTIV_WDTTO     (0x000E)    /* SYSRSTIV : WDT Time out */
#define SYSRSTIV_WDTKEY    (0x0010)    /* SYSRSTIV : WDTKEY violation */
#define SYSRSTIV_CCSKEY    (0x0012)    /* SYSRSTIV : CCS Key violation */
#define SYSRSTIV_PMMKEY    (0x0014)    /* SYSRSTIV : PMMKEY violation */
#define SYSRSTIV_PERF      (0x0016)    /* SYSRSTIV : peripheral/config area fetch */

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
* Interrupt Vectors (offset from 0xFFFF - 0x20)
************************************************************/


#define RFPMM_VECTOR            ( 6)                     /* 0xFFEA RFPMM */
#define PORT1_VECTOR            ( 7)                     /* 0xFFEC Port 1 */
#define SD14_VECTOR             ( 8)                     /* 0xFFEE RF-SD14 Sigma-Delta ADC */
#define SD_ADC_VECTOR           ( 8)                     /* 0xFFEE RF-SD14 Sigma-Delta ADC */
#define RF13M_VECTOR            (10)                     /* 0xFFF2 ISO Module */
#define ISO_VECTOR              (10)                     /* 0xFFF2 ISO Module */
#define WDT_VECTOR              (11)                     /* 0xFFF4 Watchdog Timer */
#define TIMER0_A1_VECTOR        (12)                     /* 0xFFF6 Timer0_A3 CC1-2, TA1 */
#define TIMER0_A0_VECTOR        (13)                     /* 0xFFF8 Timer0_A3 CC0 */
#define UNMI_VECTOR             (14)                     /* 0xFFFA User Non-maskable */
#define SYSNMI_VECTOR           (15)                     /* 0xFFFC System Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */


/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __RF430FRL153H_ROM */


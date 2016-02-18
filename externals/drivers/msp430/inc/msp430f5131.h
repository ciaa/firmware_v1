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
* MSP430F5131 device.
*
* Texas Instruments, Version 1.4
*
* Rev. 1.0, Setup
* Rev. 1.1  Changed access type of TimerA/D registers to word only
* Rev. 1.2  Fixed PortMapper Definitons
* Rev. 1.3  Added TimerD Cal Tag for TLV Table
* Rev. 1.4  Fixed Typo PM_UCB0SOMO -> PM_UCB0SOMI
*
*
********************************************************************/

#ifndef __MSP430F5131
#define __MSP430F5131

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

#define DMA0TSEL_0          (0x0000)  /* DMA channel 0 transfer select 0  */
#define DMA0TSEL_1          (0x0001)  /* DMA channel 0 transfer select 1  */
#define DMA0TSEL_2          (0x0002)  /* DMA channel 0 transfer select 2  */
#define DMA0TSEL_3          (0x0003)  /* DMA channel 0 transfer select 3  */
#define DMA0TSEL_4          (0x0004)  /* DMA channel 0 transfer select 4  */
#define DMA0TSEL_5          (0x0005)  /* DMA channel 0 transfer select 5  */
#define DMA0TSEL_6          (0x0006)  /* DMA channel 0 transfer select 6  */
#define DMA0TSEL_7          (0x0007)  /* DMA channel 0 transfer select 7  */
#define DMA0TSEL_8          (0x0008)  /* DMA channel 0 transfer select 8  */
#define DMA0TSEL_9          (0x0009)  /* DMA channel 0 transfer select 9  */
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

#define DMA1TSEL_0          (0x0000)  /* DMA channel 1 transfer select 0  */
#define DMA1TSEL_1          (0x0100)  /* DMA channel 1 transfer select 1  */
#define DMA1TSEL_2          (0x0200)  /* DMA channel 1 transfer select 2  */
#define DMA1TSEL_3          (0x0300)  /* DMA channel 1 transfer select 3  */
#define DMA1TSEL_4          (0x0400)  /* DMA channel 1 transfer select 4  */
#define DMA1TSEL_5          (0x0500)  /* DMA channel 1 transfer select 5  */
#define DMA1TSEL_6          (0x0600)  /* DMA channel 1 transfer select 6  */
#define DMA1TSEL_7          (0x0700)  /* DMA channel 1 transfer select 7  */
#define DMA1TSEL_8          (0x0800)  /* DMA channel 1 transfer select 8  */
#define DMA1TSEL_9          (0x0900)  /* DMA channel 1 transfer select 9  */
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

#define DMA2TSEL_0          (0x0000)  /* DMA channel 2 transfer select 0  */
#define DMA2TSEL_1          (0x0001)  /* DMA channel 2 transfer select 1  */
#define DMA2TSEL_2          (0x0002)  /* DMA channel 2 transfer select 2  */
#define DMA2TSEL_3          (0x0003)  /* DMA channel 2 transfer select 3  */
#define DMA2TSEL_4          (0x0004)  /* DMA channel 2 transfer select 4  */
#define DMA2TSEL_5          (0x0005)  /* DMA channel 2 transfer select 5  */
#define DMA2TSEL_6          (0x0006)  /* DMA channel 2 transfer select 6  */
#define DMA2TSEL_7          (0x0007)  /* DMA channel 2 transfer select 7  */
#define DMA2TSEL_8          (0x0008)  /* DMA channel 2 transfer select 8  */
#define DMA2TSEL_9          (0x0009)  /* DMA channel 2 transfer select 9  */
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
#define DMA0TSEL__TA0CCR2   (0x0002)  /* DMA channel 0 transfer select 2:  Timer0_A (TA0CCR2.IFG) */
#define DMA0TSEL__TA1CCR0   (0x0003)  /* DMA channel 0 transfer select 3:  Timer1_A (TA1CCR0.IFG) */
#define DMA0TSEL__TA1CCR2   (0x0004)  /* DMA channel 0 transfer select 4:  Timer1_A (TA1CCR2.IFG) */
#define DMA0TSEL__TB0CCR0   (0x0005)  /* DMA channel 0 transfer select 5:  TimerB (TB0CCR0.IFG) */
#define DMA0TSEL__TB0CCR2   (0x0006)  /* DMA channel 0 transfer select 6:  TimerB (TB0CCR2.IFG) */
#define DMA0TSEL__RES7      (0x0007)  /* DMA channel 0 transfer select 7:  Reserved */
#define DMA0TSEL__RES8      (0x0008)  /* DMA channel 0 transfer select 8:  Reserved */
#define DMA0TSEL__RES9      (0x0009)  /* DMA channel 0 transfer select 9:  Reserved */
#define DMA0TSEL__RES10     (0x000A) /* DMA channel 0 transfer select 10: Reserved */
#define DMA0TSEL__RES11     (0x000B) /* DMA channel 0 transfer select 11: Reserved */
#define DMA0TSEL__RES12     (0x000C) /* DMA channel 0 transfer select 12: Reserved */
#define DMA0TSEL__RES13     (0x000D) /* DMA channel 0 transfer select 13: Reserved */
#define DMA0TSEL__RES14     (0x000E) /* DMA channel 0 transfer select 14: Reserved */
#define DMA0TSEL__RES15     (0x000F) /* DMA channel 0 transfer select 15: Reserved */
#define DMA0TSEL__USCIA0RX  (0x0010) /* DMA channel 0 transfer select 16: USCIA0 receive */
#define DMA0TSEL__USCIA0TX  (0x0011) /* DMA channel 0 transfer select 17: USCIA0 transmit */
#define DMA0TSEL__USCIB0RX  (0x0012) /* DMA channel 0 transfer select 18: USCIB0 receive */
#define DMA0TSEL__USCIB0TX  (0x0013) /* DMA channel 0 transfer select 19: USCIB0 transmit */
#define DMA0TSEL__USCIA1RX  (0x0014) /* DMA channel 0 transfer select 20: USCIA1 receive */
#define DMA0TSEL__USCIA1TX  (0x0015) /* DMA channel 0 transfer select 21: USCIA1 transmit */
#define DMA0TSEL__USCIB1RX  (0x0016) /* DMA channel 0 transfer select 22: USCIB1 receive */
#define DMA0TSEL__USCIB1TX  (0x0017) /* DMA channel 0 transfer select 23: USCIB1 transmit */
#define DMA0TSEL__RES24     (0x0018) /* DMA channel 0 transfer select 24: Reserved */
#define DMA0TSEL__RES25     (0x0019) /* DMA channel 0 transfer select 25: Reserved */
#define DMA0TSEL__RES26     (0x001A) /* DMA channel 0 transfer select 26: Reserved */
#define DMA0TSEL__RES27     (0x001B) /* DMA channel 0 transfer select 27: Reserved */
#define DMA0TSEL__RES28     (0x001C) /* DMA channel 0 transfer select 28: Reserved */
#define DMA0TSEL__MPY       (0x001D) /* DMA channel 0 transfer select 29: Multiplier ready */
#define DMA0TSEL__DMA2IFG   (0x001E) /* DMA channel 0 transfer select 30: previous DMA channel DMA2IFG */
#define DMA0TSEL__DMAE0     (0x001F) /* DMA channel 0 transfer select 31: ext. Trigger (DMAE0) */

#define DMA1TSEL__DMA_REQ   (0x0000)  /* DMA channel 1 transfer select 0:  DMA_REQ (sw) */
#define DMA1TSEL__TA0CCR0   (0x0100)  /* DMA channel 1 transfer select 1:  Timer0_A (TA0CCR0.IFG) */
#define DMA1TSEL__TA0CCR2   (0x0200)  /* DMA channel 1 transfer select 2:  Timer0_A (TA0CCR2.IFG) */
#define DMA1TSEL__TA1CCR0   (0x0300)  /* DMA channel 1 transfer select 3:  Timer1_A (TA1CCR0.IFG) */
#define DMA1TSEL__TA1CCR2   (0x0400)  /* DMA channel 1 transfer select 4:  Timer1_A (TA1CCR2.IFG) */
#define DMA1TSEL__TB0CCR0   (0x0500)  /* DMA channel 1 transfer select 5:  TimerB (TB0CCR0.IFG) */
#define DMA1TSEL__TB0CCR2   (0x0600)  /* DMA channel 1 transfer select 6:  TimerB (TB0CCR2.IFG) */
#define DMA1TSEL__RES7      (0x0700)  /* DMA channel 1 transfer select 7:  Reserved */
#define DMA1TSEL__RES8      (0x0800)  /* DMA channel 1 transfer select 8:  Reserved */
#define DMA1TSEL__RES9      (0x0900)  /* DMA channel 1 transfer select 9:  Reserved */
#define DMA1TSEL__RES10     (0x0A00) /* DMA channel 1 transfer select 10: Reserved */
#define DMA1TSEL__RES11     (0x0B00) /* DMA channel 1 transfer select 11: Reserved */
#define DMA1TSEL__RES12     (0x0C00) /* DMA channel 1 transfer select 12: Reserved */
#define DMA1TSEL__RES13     (0x0D00) /* DMA channel 1 transfer select 13: Reserved */
#define DMA1TSEL__RES14     (0x0E00) /* DMA channel 1 transfer select 14: Reserved */
#define DMA1TSEL__RES15     (0x0F00) /* DMA channel 1 transfer select 15: Reserved */
#define DMA1TSEL__USCIA0RX  (0x1000) /* DMA channel 1 transfer select 16: USCIA0 receive */
#define DMA1TSEL__USCIA0TX  (0x1100) /* DMA channel 1 transfer select 17: USCIA0 transmit */
#define DMA1TSEL__USCIB0RX  (0x1200) /* DMA channel 1 transfer select 18: USCIB0 receive */
#define DMA1TSEL__USCIB0TX  (0x1300) /* DMA channel 1 transfer select 19: USCIB0 transmit */
#define DMA1TSEL__USCIA1RX  (0x1400) /* DMA channel 1 transfer select 20: USCIA1 receive */
#define DMA1TSEL__USCIA1TX  (0x1500) /* DMA channel 1 transfer select 21: USCIA1 transmit */
#define DMA1TSEL__USCIB1RX  (0x1600) /* DMA channel 1 transfer select 22: USCIB1 receive */
#define DMA1TSEL__USCIB1TX  (0x1700) /* DMA channel 1 transfer select 23: USCIB1 transmit */
#define DMA1TSEL__RES24     (0x1800) /* DMA channel 1 transfer select 24: Reserved */
#define DMA1TSEL__RES25     (0x1900) /* DMA channel 1 transfer select 25: Reserved */
#define DMA1TSEL__RES26     (0x1A00) /* DMA channel 1 transfer select 26: Reserved */
#define DMA1TSEL__RES27     (0x1B00) /* DMA channel 1 transfer select 27: Reserved */
#define DMA1TSEL__RES28     (0x1C00) /* DMA channel 1 transfer select 28: Reserved */
#define DMA1TSEL__MPY       (0x1D00) /* DMA channel 1 transfer select 29: Multiplier ready */
#define DMA1TSEL__DMA0IFG   (0x1E00) /* DMA channel 1 transfer select 30: previous DMA channel DMA0IFG */
#define DMA1TSEL__DMAE0     (0x1F00) /* DMA channel 1 transfer select 31: ext. Trigger (DMAE0) */

#define DMA2TSEL__DMA_REQ   (0x0000)  /* DMA channel 2 transfer select 0:  DMA_REQ (sw) */
#define DMA2TSEL__TA0CCR0   (0x0001)  /* DMA channel 2 transfer select 1:  Timer0_A (TA0CCR0.IFG) */
#define DMA2TSEL__TA0CCR2   (0x0002)  /* DMA channel 2 transfer select 2:  Timer0_A (TA0CCR2.IFG) */
#define DMA2TSEL__TA1CCR0   (0x0003)  /* DMA channel 2 transfer select 3:  Timer1_A (TA1CCR0.IFG) */
#define DMA2TSEL__TA1CCR2   (0x0004)  /* DMA channel 2 transfer select 4:  Timer1_A (TA1CCR2.IFG) */
#define DMA2TSEL__TB0CCR0   (0x0005)  /* DMA channel 2 transfer select 5:  TimerB (TB0CCR0.IFG) */
#define DMA2TSEL__TB0CCR2   (0x0006)  /* DMA channel 2 transfer select 6:  TimerB (TB0CCR2.IFG) */
#define DMA2TSEL__RES7      (0x0007)  /* DMA channel 2 transfer select 7:  Reserved */
#define DMA2TSEL__RES8      (0x0008)  /* DMA channel 2 transfer select 8:  Reserved */
#define DMA2TSEL__RES9      (0x0009)  /* DMA channel 2 transfer select 9:  Reserved */
#define DMA2TSEL__RES10     (0x000A) /* DMA channel 2 transfer select 10: Reserved */
#define DMA2TSEL__RES11     (0x000B) /* DMA channel 2 transfer select 11: Reserved */
#define DMA2TSEL__RES12     (0x000C) /* DMA channel 2 transfer select 12: Reserved */
#define DMA2TSEL__RES13     (0x000D) /* DMA channel 2 transfer select 13: Reserved */
#define DMA2TSEL__RES14     (0x000E) /* DMA channel 2 transfer select 14: Reserved */
#define DMA2TSEL__RES15     (0x000F) /* DMA channel 2 transfer select 15: Reserved */
#define DMA2TSEL__USCIA0RX  (0x0010) /* DMA channel 2 transfer select 16: USCIA0 receive */
#define DMA2TSEL__USCIA0TX  (0x0011) /* DMA channel 2 transfer select 17: USCIA0 transmit */
#define DMA2TSEL__USCIB0RX  (0x0012) /* DMA channel 2 transfer select 18: USCIB0 receive */
#define DMA2TSEL__USCIB0TX  (0x0013) /* DMA channel 2 transfer select 19: USCIB0 transmit */
#define DMA2TSEL__USCIA1RX  (0x0014) /* DMA channel 2 transfer select 20: USCIA1 receive */
#define DMA2TSEL__USCIA1TX  (0x0015) /* DMA channel 2 transfer select 21: USCIA1 transmit */
#define DMA2TSEL__USCIB1RX  (0x0016) /* DMA channel 2 transfer select 22: USCIB1 receive */
#define DMA2TSEL__USCIB1TX  (0x0017) /* DMA channel 2 transfer select 23: USCIB1 transmit */
#define DMA2TSEL__RES24     (0x0018) /* DMA channel 2 transfer select 24: Reserved */
#define DMA2TSEL__RES25     (0x0019) /* DMA channel 2 transfer select 25: Reserved */
#define DMA2TSEL__RES26     (0x001A) /* DMA channel 2 transfer select 26: Reserved */
#define DMA2TSEL__RES27     (0x001B) /* DMA channel 2 transfer select 27: Reserved */
#define DMA2TSEL__RES28     (0x001C) /* DMA channel 2 transfer select 28: Reserved */
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
#define __MSP430_HAS_P1SEL__          /* Define for DriverLib */
#define __MSP430_HAS_P2SEL__          /* Define for DriverLib */
#define __MSP430_HAS_PASEL__          /* Define for DriverLib */

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
#define PASEL_                0x020A    /* Port A Selection */
sfrb(PASEL_L , PASEL_);
sfrb(PASEL_H , PASEL_+1);
sfrw(PASEL, PASEL_);
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
#define P1SEL               (PASEL_L) /* Port 1 Selection */
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
#define P2SEL               (PASEL_H) /* Port 2 Selection */
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
* DIGITAL I/O Port3 Pull up / Pull down Resistors
************************************************************/
#define __MSP430_HAS_PORT3_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT3_R__ 0x0220
#define P3_BASE __MSP430_BASEADDRESS_PORT3_R__
#define __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORTB_R__ 0x0220
#define PB_BASE __MSP430_BASEADDRESS_PORTB_R__
#define __MSP430_HAS_P3SEL__          /* Define for DriverLib */
#define __MSP430_HAS_PBSEL__          /* Define for DriverLib */

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
#define PBSEL_                0x022A    /* Port B Selection */
sfrb(PBSEL_L , PBSEL_);
sfrb(PBSEL_H , PBSEL_+1);
sfrw(PBSEL, PBSEL_);














#define P3IN                (PBIN_L)  /* Port 3 Input */
#define P3OUT               (PBOUT_L) /* Port 3 Output */
#define P3DIR               (PBDIR_L) /* Port 3 Direction */
#define P3REN               (PBREN_L) /* Port 3 Resistor Enable */
#define P3DS                (PBDS_L)  /* Port 3 Drive Strenght */
#define P3SEL               (PBSEL_L) /* Port 3 Selection */




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
* PORT 1 MAPPING CONTROLLER
************************************************************/
#define __MSP430_HAS_PORT1_MAPPING__   /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_PORT1_MAPPING__ 0x01C8
#define P1MAP_BASE __MSP430_BASEADDRESS_PORT1_MAPPING__

#define P1MAP01_              0x01C8    /* Port P1.0/1 mapping register */
sfrb(P1MAP01_L , P1MAP01_);
sfrb(P1MAP01_H , P1MAP01_+1);
sfrw(P1MAP01, P1MAP01_);
#define P1MAP23_              0x01CA    /* Port P1.2/3 mapping register */
sfrb(P1MAP23_L , P1MAP23_);
sfrb(P1MAP23_H , P1MAP23_+1);
sfrw(P1MAP23, P1MAP23_);
#define P1MAP45_              0x01CC    /* Port P1.4/5 mapping register */
sfrb(P1MAP45_L , P1MAP45_);
sfrb(P1MAP45_H , P1MAP45_+1);
sfrw(P1MAP45, P1MAP45_);
#define P1MAP67_              0x01CE    /* Port P1.6/7 mapping register */
sfrb(P1MAP67_L , P1MAP67_);
sfrb(P1MAP67_H , P1MAP67_+1);
sfrw(P1MAP67, P1MAP67_);

#define  P1MAP0              P1MAP01_L /* Port P1.0 mapping register */
#define  P1MAP1              P1MAP01_H /* Port P1.1 mapping register */
#define  P1MAP2              P1MAP23_L /* Port P1.2 mapping register */
#define  P1MAP3              P1MAP23_H /* Port P1.3 mapping register */
#define  P1MAP4              P1MAP45_L /* Port P1.4 mapping register */
#define  P1MAP5              P1MAP45_H /* Port P1.5 mapping register */
#define  P1MAP6              P1MAP67_L /* Port P1.6 mapping register */
#define  P1MAP7              P1MAP67_H /* Port P1.7 mapping register */











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












#define PM_NONE       0
#define PM_UCA0CLK    1
#define PM_UCB0STE    1
#define PM_UCA0TXD    2
#define PM_UCA0SIMO   2
#define PM_UCB0SOMI   3
#define PM_UCB0SCL    3
#define PM_UCA0RXD    4
#define PM_UCA0SOMI   4
#define PM_UCB0SIMO   5
#define PM_UCB0SDA    5
#define PM_UCB0CLK    6
#define PM_UCA0STE    6
#define PM_TD0_0      7
#define PM_TD0_1      8
#define PM_TD0_2      9
#define PM_TD1_0      10
#define PM_TD1_1      11
#define PM_TD1_2      12
#define PM_CLR1TD0_0  13
#define PM_FLT1_2TD0_0 13
#define PM_FLT1_0TD0_1 14
#define PM_FLT1_1TD0_2 15
#define PM_CLR2TD1_0   16
#define PM_FLT2_1TD1_0 16
#define PM_FLT2_2TD1_1 17
#define PM_FLT2_0TD1_2 18
#define PM_TD0_0SMCLK  19
#define PM_TA0CLKCBOUT 20
#define PM_TD0CLKMCLK  21
#define PM_TA0_0       22
#define PM_TA0_1       23
#define PM_TA0_2       24
#define PM_DMAE0SMCLK  25
#define PM_DMAE1MCLK   26
#define PM_DMAE2SVM    27
#define PM_TD0OUTH     28
#define PM_TD1OUTH     29
#define PM_ANALOG      31

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
#define SYSUNIV_NMIIFG     (0x0002)    /* SYSUNIV : NMIIFG */
#define SYSUNIV_OFIFG      (0x0004)    /* SYSUNIV : Osc. Fail - OFIFG */
#define SYSUNIV_ACCVIFG    (0x0006)    /* SYSUNIV : Access Violation - ACCVIFG */
#define SYSUNIV_SYSBERRIV  (0x0008)    /* SYSUNIV : Bus Error - SYSBERRIV */

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
#define SYSRSTIV_PLLUL     (0x001C)    /* SYSRSTIV : PLL unlock */
#define SYSRSTIV_PERF      (0x001E)    /* SYSRSTIV : peripheral/config area fetch */
#define SYSRSTIV_PMMKEY    (0x0020)    /* SYSRSTIV : PMMKEY violation */

/************************************************************
* Timer0_A3
************************************************************/
#define __MSP430_HAS_T0A3__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0A3__ 0x03C0
#define TIMER_A0_BASE __MSP430_BASEADDRESS_T0A3__

#define TA0CTL_               0x03C0    /* Timer0_A3 Control */
sfrw(TA0CTL, TA0CTL_);
#define TA0CCTL0_             0x03C2    /* Timer0_A3 Capture/Compare Control 0 */
sfrw(TA0CCTL0, TA0CCTL0_);
#define TA0CCTL1_             0x03C4    /* Timer0_A3 Capture/Compare Control 1 */
sfrw(TA0CCTL1, TA0CCTL1_);
#define TA0CCTL2_             0x03C6    /* Timer0_A3 Capture/Compare Control 2 */
sfrw(TA0CCTL2, TA0CCTL2_);
#define TA0R_                 0x03D0    /* Timer0_A3 */
sfrw(TA0R, TA0R_);
#define TA0CCR0_              0x03D2    /* Timer0_A3 Capture/Compare 0 */
sfrw(TA0CCR0, TA0CCR0_);
#define TA0CCR1_              0x03D4    /* Timer0_A3 Capture/Compare 1 */
sfrw(TA0CCR1, TA0CCR1_);
#define TA0CCR2_              0x03D6    /* Timer0_A3 Capture/Compare 2 */
sfrw(TA0CCR2, TA0CCR2_);
#define TA0IV_                0x03EE    /* Timer0_A3 Interrupt Vector Word */
sfrw(TA0IV, TA0IV_);
#define TA0EX0_               0x03E0    /* Timer0_A3 Expansion Register 0 */
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
* Timer0_D3
************************************************************/
#define __MSP430_HAS_T0D3__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T0D3__ 0x0B00
#define TIMER_D0_BASE __MSP430_BASEADDRESS_T0D3__

#define TD0CTL0_              0x0B00    /* Timer0_D3 Control 0 */
sfrw(TD0CTL0, TD0CTL0_);
#define TD0CTL1_              0x0B02    /* Timer0_D3 Control 1 */
sfrw(TD0CTL1, TD0CTL1_);
#define TD0CTL2_              0x0B04    /* Timer0_D3 Control 2 */
sfrw(TD0CTL2, TD0CTL2_);
#define TD0R_                 0x0B06    /* Timer0_D3 Counter */
sfrw(TD0R, TD0R_);
#define TD0CCTL0_             0x0B08    /* Timer0_D3 Capture/Compare Control 0 */
sfrw(TD0CCTL0, TD0CCTL0_);
#define TD0CCR0_              0x0B0A    /* Timer0_D3 Capture/Compare 0 */
sfrw(TD0CCR0, TD0CCR0_);
#define TD0CL0_               0x0B0C    /* Timer0_D3 Capture/Compare Latch 0 */
sfrw(TD0CL0, TD0CL0_);
#define TD0CCTL1_             0x0B0E    /* Timer0_D3 Capture/Compare Control 1 */
sfrw(TD0CCTL1, TD0CCTL1_);
#define TD0CCR1_              0x0B10    /* Timer0_D3 Capture/Compare 1 */
sfrw(TD0CCR1, TD0CCR1_);
#define TD0CL1_               0x0B12    /* Timer0_D3 Capture/Compare Latch 1 */
sfrw(TD0CL1, TD0CL1_);
#define TD0CCTL2_             0x0B14    /* Timer0_D3 Capture/Compare Control 2 */
sfrw(TD0CCTL2, TD0CCTL2_);
#define TD0CCR2_              0x0B16    /* Timer0_D3 Capture/Compare 2 */
sfrw(TD0CCR2, TD0CCR2_);
#define TD0CL2_               0x0B18    /* Timer0_D3 Capture/Compare Latch 2 */
sfrw(TD0CL2, TD0CL2_);
#define TD0HCTL0_             0x0B38    /* Timer0_D3 High-resolution Control Register 0 */
sfrw(TD0HCTL0, TD0HCTL0_);
#define TD0HCTL1_             0x0B3A    /* Timer0_D3 High-resolution Control Register 1 */
sfrw(TD0HCTL1, TD0HCTL1_);
#define TD0HINT_              0x0B3C    /* Timer0_D3 High-resolution Interrupt Register */
sfrw(TD0HINT, TD0HINT_);
#define TD0IV_                0x0B3E    /* Timer0_D3 Interrupt Vector Word */
sfrw(TD0IV, TD0IV_);


/* TDxCTL0 Control Bits */
#define TDCLGRP1            (0x4000)  /* Timer0_D3 Compare latch load group 1 */
#define TDCLGRP0            (0x2000)  /* Timer0_D3 Compare latch load group 0 */
#define CNTL1               (0x1000)  /* Counter lenght 1 */
#define CNTL0               (0x0800)  /* Counter lenght 0 */
#define TDSSEL1             (0x0200)  /* Clock source 1 */
#define TDSSEL0             (0x0100)  /* Clock source 0 */
#define TDCLR               (0x0004)  /* Timer0_D3 counter clear */
#define TDIE                (0x0002)  /* Timer0_D3 interrupt enable */
#define TDIFG               (0x0001)  /* Timer0_D3 interrupt flag */

#define SHR1                (0x4000)  /* Timer0_D3 Compare latch load group 1 */
#define SHR0                (0x2000)  /* Timer0_D3 Compare latch load group 0 */

#define TDSSEL_0            (0x0000)  /* Clock Source: TDCLK */
#define TDSSEL_1            (0x0100)  /* Clock Source: ACLK  */
#define TDSSEL_2            (0x0200)  /* Clock Source: SMCLK */
#define TDSSEL_3            (0x0300)  /* Clock Source: INCLK */
#define CNTL_0              (0x0000)  /* Counter lenght: 16 bit */
#define CNTL_1              (0x0800)  /* Counter lenght: 12 bit */
#define CNTL_2              (0x1000)  /* Counter lenght: 10 bit */
#define CNTL_3              (0x1800)  /* Counter lenght:  8 bit */
#define SHR_0               (0x0000)  /* Timer0_D3 Group: 0 - individually */
#define SHR_1               (0x2000)  /* Timer0_D3 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2               (0x4000)  /* Timer0_D3 Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3               (0x6000)  /* Timer0_D3 Group: 3 - 1 group (all) */
#define TDCLGRP_0           (0x0000)  /* Timer0_D3 Group: 0 - individually */
#define TDCLGRP_1           (0x2000)  /* Timer0_D3 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TDCLGRP_2           (0x4000)  /* Timer0_D3 Group: 2 - 2 groups (1-3, 4-6)*/
#define TDCLGRP_3           (0x6000)  /* Timer0_D3 Group: 3 - 1 group (all) */
#define TDSSEL__TACLK       (0x0000)  /* Timer0_D3 clock source select: 0 - TACLK */
#define TDSSEL__ACLK        (0x0100)  /* Timer0_D3 clock source select: 1 - ACLK  */
#define TDSSEL__SMCLK       (0x0200)  /* Timer0_D3 clock source select: 2 - SMCLK */
#define TDSSEL__INCLK       (0x0300)  /* Timer0_D3 clock source select: 3 - INCLK */
#define CNTL__16            (0x0000)  /* Counter lenght: 16 bit */
#define CNTL__12            (0x0800)  /* Counter lenght: 12 bit */
#define CNTL__10            (0x1000)  /* Counter lenght: 10 bit */
#define CNTL__8             (0x1800)  /* Counter lenght:  8 bit */

/* Additional Timer B Control Register bits are defined in Timer A */

/* TDxCTL1 Control Bits */
#define TDCLKM0             (0x0001)   /* Timer0_D3 Clocking Mode Bit: 0 */
#define TDCLKM1             (0x0002)   /* Timer0_D3 Clocking Mode Bit: 1 */
#define TD2CMB              (0x0010)   /* Timer0_D3 TD0CCR Combination in TD2 */
#define TD4CMB              (0x0020)   /* Timer0_D3 TD0CCR Combination in TD4 */
#define TD6CMB              (0x0040)   /* Timer0_D3 TD0CCR Combination in TD6 */
#define TDIDEX0             (0x0100)   /* Timer0_D3 Input divider expansion Bit: 0 */
#define TDIDEX1             (0x0200)   /* Timer0_D3 Input divider expansion Bit: 1 */
#define TDIDEX2             (0x0400)   /* Timer0_D3 Input divider expansion Bit: 2 */

#define TDCLKM_0            (0x0000)   /* Timer0_D3 Clocking Mode: External */
#define TDCLKM_1            (0x0001)   /* Timer0_D3 Clocking Mode: High-Res. local clock */
#define TDCLKM_2            (0x0002)   /* Timer0_D3 Clocking Mode: Aux Clock */
#define TDCLKM__EXT         (0x0000)   /* Timer0_D3 Clocking Mode: External */
#define TDCLKM__HIGHRES     (0x0001)   /* Timer0_D3 Clocking Mode: High-Res. local clock */
#define TDCLKM__AUX         (0x0002)   /* Timer0_D3 Clocking Mode: Aux Clock */

#define TDIDEX_0            (0x0000) /* Timer0_D3 Input divider expansion : /1 */
#define TDIDEX_1            (0x0100) /* Timer0_D3 Input divider expansion : /2 */
#define TDIDEX_2            (0x0200) /* Timer0_D3 Input divider expansion : /3 */
#define TDIDEX_3            (0x0300) /* Timer0_D3 Input divider expansion : /4 */
#define TDIDEX_4            (0x0400) /* Timer0_D3 Input divider expansion : /5 */
#define TDIDEX_5            (0x0500) /* Timer0_D3 Input divider expansion : /6 */
#define TDIDEX_6            (0x0600) /* Timer0_D3 Input divider expansion : /7 */
#define TDIDEX_7            (0x0700) /* Timer0_D3 Input divider expansion : /8 */
#define TDIDEX__1           (0x0000) /* Timer0_D3 Input divider expansion : /1 */
#define TDIDEX__2           (0x0100) /* Timer0_D3 Input divider expansion : /2 */
#define TDIDEX__3           (0x0200) /* Timer0_D3 Input divider expansion : /3 */
#define TDIDEX__4           (0x0300) /* Timer0_D3 Input divider expansion : /4 */
#define TDIDEX__5           (0x0400) /* Timer0_D3 Input divider expansion : /5 */
#define TDIDEX__6           (0x0500) /* Timer0_D3 Input divider expansion : /6 */
#define TDIDEX__7           (0x0600) /* Timer0_D3 Input divider expansion : /7 */
#define TDIDEX__8           (0x0700) /* Timer0_D3 Input divider expansion : /8 */

/* TDxCTL2 Control Bits */
#define TDCAPM0             (0x0001)   /* Timer0_D3 Capture Mode of Channel 0 */
#define TDCAPM1             (0x0002)   /* Timer0_D3 Capture Mode of Channel 1 */
#define TDCAPM2             (0x0004)   /* Timer0_D3 Capture Mode of Channel 2 */
#define TDCAPM3             (0x0008)   /* Timer0_D3 Capture Mode of Channel 3 */
#define TDCAPM4             (0x0010)   /* Timer0_D3 Capture Mode of Channel 4 */
#define TDCAPM5             (0x0020)   /* Timer0_D3 Capture Mode of Channel 5 */
#define TDCAPM6             (0x0040)   /* Timer0_D3 Capture Mode of Channel 6 */

/* TDxCCTLx Control Bits */
#define CLLD1               (0x0400)  /* Compare latch load source 1 */
#define CLLD0               (0x0200)  /* Compare latch load source 0 */

#define SLSHR1              (0x0400)  /* Compare latch load source 1 */
#define SLSHR0              (0x0200)  /* Compare latch load source 0 */

#define SLSHR_0             (0x0000) /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1             (0x0200) /* Compare latch load sourec : 1 - TDR counts to 0 */
#define SLSHR_2             (0x0400) /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3             (0x0600) /* Compare latch load sourec : 3 - TDR counts to TDCTL0 */

#define CLLD_0              (0x0000) /* Compare latch load sourec : 0 - immediate */
#define CLLD_1              (0x0200) /* Compare latch load sourec : 1 - TDR counts to 0 */
#define CLLD_2              (0x0400) /* Compare latch load sourec : 2 - up/down */
#define CLLD_3              (0x0600) /* Compare latch load sourec : 3 - TDR counts to TDCTL0 */

/* TDxHCTL0 Control Bits */
#define TDHEN               (0x0001)   /* Timer0_D3 High-Resolution Enable */
#define TDHREGEN            (0x0002)   /* Timer0_D3 High-Resolution Regulated Mode */
#define TDHEAEN             (0x0004)   /* Timer0_D3 High-Resolution clock error accum. enable */
#define TDHRON              (0x0008)   /* Timer0_D3 High-Resolution Generator forced on*/
#define TDHM0               (0x0010)   /* Timer0_D3 High-Resoltuion Clock Mult. Bit: 0 */
#define TDHM1               (0x0020)   /* Timer0_D3 High-Resoltuion Clock Mult. Bit: 1 */
#define TDHD0               (0x0040)   /* Timer0_D3 High-Resolution clock divider Bit: 0 */
#define TDHD1               (0x0080)   /* Timer0_D3 High-Resolution clock divider Bit: 1 */
#define TDHFW               (0x0100)   /* Timer0_D7 High-resolution generator fast wakeup enable */

#define TDHCALEN            TDHREGEN   /* Timer0_D3 Lagacy Definition */


#define TDHM_0              (0x0000)   /* Timer0_D3 High-Resoltuion Clock Mult.: 8x TimerD clock */
#define TDHM_1              (0x0010)   /* Timer0_D3 High-Resoltuion Clock Mult.: 16x TimerD clock */
#define TDHM__8             (0x0000)   /* Timer0_D3 High-Resoltuion Clock Mult.: 8x TimerD clock */
#define TDHM__16            (0x0010)   /* Timer0_D3 High-Resoltuion Clock Mult.: 16x TimerD clock */
#define TDHD_0              (0x0000)   /* Timer0_D3 High-Resolution clock divider: /1 */
#define TDHD_1              (0x0040)   /* Timer0_D3 High-Resolution clock divider: /2 */
#define TDHD_2              (0x0080)   /* Timer0_D3 High-Resolution clock divider: /4 */
#define TDHD_3              (0x00C0)   /* Timer0_D3 High-Resolution clock divider: /8 */
#define TDHD__1             (0x0000)   /* Timer0_D3 High-Resolution clock divider: /1 */
#define TDHD__2             (0x0040)   /* Timer0_D3 High-Resolution clock divider: /2 */
#define TDHD__4             (0x0080)   /* Timer0_D3 High-Resolution clock divider: /4 */
#define TDHD__8             (0x00C0)   /* Timer0_D3 High-Resolution clock divider: /8 */

/* TDxHCTL1 Control Bits */
#define TDHCLKTRIM0         (0x0002)   /* Timer0_D3 High-Resolution Clock Trim Bit: 0 */
#define TDHCLKTRIM1         (0x0004)   /* Timer0_D3 High-Resolution Clock Trim Bit: 1 */
#define TDHCLKTRIM2         (0x0008)   /* Timer0_D3 High-Resolution Clock Trim Bit: 2 */
#define TDHCLKTRIM3         (0x0010)   /* Timer0_D3 High-Resolution Clock Trim Bit: 3 */
#define TDHCLKTRIM4         (0x0020)   /* Timer0_D3 High-Resolution Clock Trim Bit: 4 */
#define TDHCLKTRIM5         (0x0040)   /* Timer0_D3 High-Resolution Clock Trim Bit: 5 */
#define TDHCLKTRIM6         (0x0080)   /* Timer0_D3 High-Resolution Clock Trim Bit: 6 */
#define TDHCLKSR0           (0x0100)   /* Timer0_D3 High-Resolution Clock Sub-Range Bit: 0 */
#define TDHCLKSR1           (0x0200)   /* Timer0_D3 High-Resolution Clock Sub-Range Bit: 1 */
#define TDHCLKSR2           (0x0400)   /* Timer0_D3 High-Resolution Clock Sub-Range Bit: 2 */
#define TDHCLKSR3           (0x0800)   /* Timer0_D3 High-Resolution Clock Sub-Range Bit: 3 */
#define TDHCLKSR4           (0x1000)   /* Timer0_D3 High-Resolution Clock Sub-Range Bit: 4 */
#define TDHCLKR0            (0x2000)   /* Timer0_D3 High-Resolution Clock Range Bit: 0 */
#define TDHCLKR1            (0x4000)   /* Timer0_D3 High-Resolution Clock Range Bit: 1 */
#define TDHCLKCR            (0x8000)   /* Timer0_D3 High-Resolution Coarse Clock Range */

/* TDxHINT Control Bits */
#define TDHFLIFG            (0x0001)   /* Timer0_D3 High-Res. fail low Interrupt Flag */
#define TDHFHIFG            (0x0002)   /* Timer0_D3 High-Res. fail high Interrupt Flag */
#define TDHLKIFG            (0x0004)   /* Timer0_D3 High-Res. frequency lock Interrupt Flag */
#define TDHUNLKIFG          (0x0008)   /* Timer0_D3 High-Res. frequency unlock Interrupt Flag */
#define TDHFLIE             (0x0100)   /* Timer0_D3 High-Res. fail low Interrupt Enable */
#define TDHFHIE             (0x0200)   /* Timer0_D3 High-Res. fail high Interrupt Enable */
#define TDHLKIE             (0x0400)   /* Timer0_D3 High-Res. frequency lock Interrupt Enable */
#define TDHUNLKIE           (0x0800)   /* Timer0_D3 High-Res. frequency unlock Interrupt Enable */

/* TD0IV Definitions */
#define TD0IV_NONE          (0x0000)    /* No Interrupt pending */
#define TD0IV_TDCCR1        (0x0002)    /* TD0CCR1_CCIFG */
#define TD0IV_TDCCR2        (0x0004)    /* TD0CCR2_CCIFG */
#define TD0IV_TDCCR3        (0x0006)    /* TD0CCR3_CCIFG */
#define TD0IV_TDCCR4        (0x0008)    /* TD0CCR4_CCIFG */
#define TD0IV_TDCCR5        (0x000A)    /* TD0CCR5_CCIFG */
#define TD0IV_TDCCR6        (0x000C)    /* TD0CCR6_CCIFG */
#define TD0IV_RES_14        (0x000E)    /* Reserverd */
#define TD0IV_TD0IFG        (0x0010)    /* TD0IFG */
#define TD0IV_TDHFLIFG      (0x0012)    /* TDHFLIFG Clock fail low */
#define TD0IV_TDHFHIFG      (0x0014)    /* TDHFLIFG Clock fail high */
#define TD0IV_TDHLKIFG      (0x0016)    /* TDHLKIE Clock lock*/
#define TD0IV_TDHUNLKIFG    (0x0018)    /* TDHUNLKIE Clock unlock */

/* Legacy Defines */
#define TD0IV_TD0CCR1       (0x0002)    /* TD0CCR1_CCIFG */
#define TD0IV_TD0CCR2       (0x0004)    /* TD0CCR2_CCIFG */
#define TD0IV_TD0CCR3       (0x0006)    /* TD0CCR3_CCIFG */
#define TD0IV_TD0CCR4       (0x0008)    /* TD0CCR4_CCIFG */
#define TD0IV_TD0CCR5       (0x000A)    /* TD0CCR5_CCIFG */
#define TD0IV_TD0CCR6       (0x000C)    /* TD0CCR6_CCIFG */


/************************************************************
* Timer1_D3
************************************************************/
#define __MSP430_HAS_T1D3__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T1D3__ 0x0B40
#define TIMER_D1_BASE __MSP430_BASEADDRESS_T1D3__

#define TD1CTL0_              0x0B40    /* Timer1_D3 Control 0 */
sfrw(TD1CTL0, TD1CTL0_);
#define TD1CTL1_              0x0B42    /* Timer1_D3 Control 1 */
sfrw(TD1CTL1, TD1CTL1_);
#define TD1CTL2_              0x0B44    /* Timer1_D3 Control 2 */
sfrw(TD1CTL2, TD1CTL2_);
#define TD1R_                 0x0B46    /* Timer1_D3 Counter */
sfrw(TD1R, TD1R_);
#define TD1CCTL0_             0x0B48    /* Timer1_D3 Capture/Compare Control 0 */
sfrw(TD1CCTL0, TD1CCTL0_);
#define TD1CCR0_              0x0B4A    /* Timer1_D3 Capture/Compare 0 */
sfrw(TD1CCR0, TD1CCR0_);
#define TD1CL0_               0x0B4C    /* Timer1_D3 Capture/Compare Latch 0 */
sfrw(TD1CL0, TD1CL0_);
#define TD1CCTL1_             0x0B4E    /* Timer1_D3 Capture/Compare Control 1 */
sfrw(TD1CCTL1, TD1CCTL1_);
#define TD1CCR1_              0x0B50    /* Timer1_D3 Capture/Compare 1 */
sfrw(TD1CCR1, TD1CCR1_);
#define TD1CL1_               0x0B52    /* Timer1_D3 Capture/Compare Latch 1 */
sfrw(TD1CL1, TD1CL1_);
#define TD1CCTL2_             0x0B54    /* Timer1_D3 Capture/Compare Control 2 */
sfrw(TD1CCTL2, TD1CCTL2_);
#define TD1CCR2_              0x0B56    /* Timer1_D3 Capture/Compare 2 */
sfrw(TD1CCR2, TD1CCR2_);
#define TD1CL2_               0x0B58    /* Timer1_D3 Capture/Compare Latch 2 */
sfrw(TD1CL2, TD1CL2_);
#define TD1HCTL0_             0x0B78    /* Timer1_D3 High-resolution Control Register 0 */
sfrw(TD1HCTL0, TD1HCTL0_);
#define TD1HCTL1_             0x0B7A    /* Timer1_D3 High-resolution Control Register 1 */
sfrw(TD1HCTL1, TD1HCTL1_);
#define TD1HINT_              0x0B7C    /* Timer1_D3 High-resolution Interrupt Register */
sfrw(TD1HINT, TD1HINT_);
#define TD1IV_                0x0B7E    /* Timer1_D3 Interrupt Vector Word */
sfrw(TD1IV, TD1IV_);

/* Bits are already defined within the Timer0_Dx */

/* TD1IV Definitions */
#define TD1IV_NONE           (0x0000)    /* No Interrupt pending */
#define TD1IV_TDCCR1         (0x0002)    /* TD1CCR1_CCIFG */
#define TD1IV_TDCCR2         (0x0004)    /* TD1CCR2_CCIFG */
#define TD1IV_TDCCR3         (0x0006)    /* TD1CCR3_CCIFG */
#define TD1IV_TDCCR4         (0x0008)    /* TD1CCR4_CCIFG */
#define TD1IV_TDCCR5         (0x000A)    /* TD1CCR5_CCIFG */
#define TD1IV_TDCCR6         (0x000C)    /* TD1CCR6_CCIFG */
#define TD1IV_RES_14         (0x000E)    /* Reserverd */
#define TD1IV_TDIFG          (0x0010)    /* TD1IFG */
#define TD1IV_TDHFLIFG       (0x0012)    /* TDHFLIFG Clock fail low */
#define TD1IV_TDHFHIFG       (0x0014)    /* TDHFLIFG Clock fail high */
#define TD1IV_TDHLKIFG       (0x0016)    /* TDHLKIE Clock lock*/
#define TD1IV_TDHUNLKIFG     (0x0018)    /* TDHUNLKIE Clock unlock */

/* Legacy Defines */
#define TD1IV_TD1CCR1       (0x0002)    /* TD1CCR1_CCIFG */
#define TD1IV_TD1CCR2       (0x0004)    /* TD1CCR2_CCIFG */
#define TD1IV_TD1CCR3       (0x0006)    /* TD1CCR3_CCIFG */
#define TD1IV_TD1CCR4       (0x0008)    /* TD1CCR4_CCIFG */
#define TD1IV_TD1CCR5       (0x000A)    /* TD1CCR5_CCIFG */
#define TD1IV_TD1CCR6       (0x000C)    /* TD1CCR6_CCIFG */
#define TD1IV_TD1IFG        (0x0010)    /* TD1IFG */

/************************************************************
* Timer Event Control 0
************************************************************/
#define __MSP430_HAS_TEV0__             /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_TEV0__ 0x0C00
#define TEC0_BASE __MSP430_BASEADDRESS_TEV0__

#define TEC0XCTL0_            0x0C00    /* Timer Event Control 0 External Control 0 */
sfrb(TEC0XCTL0_L , TEC0XCTL0_);
sfrb(TEC0XCTL0_H , TEC0XCTL0_+1);
sfrw(TEC0XCTL0, TEC0XCTL0_);
#define TEC0XCTL1_            0x0C02    /* Timer Event Control 0 External Control 1 */
sfrb(TEC0XCTL1_L , TEC0XCTL1_);
sfrb(TEC0XCTL1_H , TEC0XCTL1_+1);
sfrw(TEC0XCTL1, TEC0XCTL1_);
#define TEC0XCTL2_            0x0C04    /* Timer Event Control 0 External Control 2 */
sfrb(TEC0XCTL2_L , TEC0XCTL2_);
sfrb(TEC0XCTL2_H , TEC0XCTL2_+1);
sfrw(TEC0XCTL2, TEC0XCTL2_);
#define TEC0STA_              0x0C06    /* Timer Event Control 0 Status */
sfrb(TEC0STA_L , TEC0STA_);
sfrb(TEC0STA_H , TEC0STA_+1);
sfrw(TEC0STA, TEC0STA_);
#define TEC0XINT_             0x0C08    /* Timer Event Control 0 External Interrupt */
sfrb(TEC0XINT_L , TEC0XINT_);
sfrb(TEC0XINT_H , TEC0XINT_+1);
sfrw(TEC0XINT, TEC0XINT_);
#define TEC0IV_               0x0C0A    /* Timer Event Control 0 Interrupt Vector */
sfrb(TEC0IV_L , TEC0IV_);
sfrb(TEC0IV_H , TEC0IV_+1);
sfrw(TEC0IV, TEC0IV_);


/* TECxXCTL0 Control Bits */
#define TECXFLTHLD0         (0x0001)  /* TEV Ext. fault signal hold for CE0 */
#define TECXFLTHLD1         (0x0002)  /* TEV Ext. fault signal hold for CE1 */
#define TECXFLTHLD2         (0x0004)  /* TEV Ext. fault signal hold for CE2 */
#define TECXFLTHLD3         (0x0008)  /* TEV Ext. fault signal hold for CE3 */
#define TECXFLTHLD4         (0x0010)  /* TEV Ext. fault signal hold for CE4 */
#define TECXFLTHLD5         (0x0020)  /* TEV Ext. fault signal hold for CE5 */
#define TECXFLTHLD6         (0x0040)  /* TEV Ext. fault signal hold for CE6 */
#define TECXFLTEN0          (0x0100)  /* TEV Ext. fault signal enable for CE0 */
#define TECXFLTEN1          (0x0200)  /* TEV Ext. fault signal enable for CE1 */
#define TECXFLTEN2          (0x0400)  /* TEV Ext. fault signal enable for CE2 */
#define TECXFLTEN3          (0x0800)  /* TEV Ext. fault signal enable for CE3 */
#define TECXFLTEN4          (0x1000)  /* TEV Ext. fault signal enable for CE4 */
#define TECXFLTEN5          (0x2000)  /* TEV Ext. fault signal enable for CE5 */
#define TECXFLTEN6          (0x4000)  /* TEV Ext. fault signal enable for CE6 */

/* TECxXCTL0 Control Bits */
#define TECXFLTHLD0_L       (0x0001)  /* TEV Ext. fault signal hold for CE0 */
#define TECXFLTHLD1_L       (0x0002)  /* TEV Ext. fault signal hold for CE1 */
#define TECXFLTHLD2_L       (0x0004)  /* TEV Ext. fault signal hold for CE2 */
#define TECXFLTHLD3_L       (0x0008)  /* TEV Ext. fault signal hold for CE3 */
#define TECXFLTHLD4_L       (0x0010)  /* TEV Ext. fault signal hold for CE4 */
#define TECXFLTHLD5_L       (0x0020)  /* TEV Ext. fault signal hold for CE5 */
#define TECXFLTHLD6_L       (0x0040)  /* TEV Ext. fault signal hold for CE6 */

/* TECxXCTL0 Control Bits */
#define TECXFLTEN0_H        (0x0001)  /* TEV Ext. fault signal enable for CE0 */
#define TECXFLTEN1_H        (0x0002)  /* TEV Ext. fault signal enable for CE1 */
#define TECXFLTEN2_H        (0x0004)  /* TEV Ext. fault signal enable for CE2 */
#define TECXFLTEN3_H        (0x0008)  /* TEV Ext. fault signal enable for CE3 */
#define TECXFLTEN4_H        (0x0010)  /* TEV Ext. fault signal enable for CE4 */
#define TECXFLTEN5_H        (0x0020)  /* TEV Ext. fault signal enable for CE5 */
#define TECXFLTEN6_H        (0x0040)  /* TEV Ext. fault signal enable for CE6 */

/* TECxXCTL1 Control Bits */
#define TECXFLTPOL0         (0x0001)  /* TEV Polarity Bit of ext. fault 0 */
#define TECXFLTPOL1         (0x0002)  /* TEV Polarity Bit of ext. fault 1 */
#define TECXFLTPOL2         (0x0004)  /* TEV Polarity Bit of ext. fault 2 */
#define TECXFLTPOL3         (0x0008)  /* TEV Polarity Bit of ext. fault 3 */
#define TECXFLTPOL4         (0x0010)  /* TEV Polarity Bit of ext. fault 4 */
#define TECXFLTPOL5         (0x0020)  /* TEV Polarity Bit of ext. fault 5 */
#define TECXFLTPOL6         (0x0040)  /* TEV Polarity Bit of ext. fault 6 */
#define TECXFLTLVS0         (0x0100)  /* TEV Signal Type of Ext. fault 0 */
#define TECXFLTLVS1         (0x0200)  /* TEV Signal Type of Ext. fault 1 */
#define TECXFLTLVS2         (0x0400)  /* TEV Signal Type of Ext. fault 2 */
#define TECXFLTLVS3         (0x0800)  /* TEV Signal Type of Ext. fault 3 */
#define TECXFLTLVS4         (0x1000)  /* TEV Signal Type of Ext. fault 4 */
#define TECXFLTLVS5         (0x2000)  /* TEV Signal Type of Ext. fault 5 */
#define TECXFLTLVS6         (0x4000)  /* TEV Signal Type of Ext. fault 6 */

/* TECxXCTL1 Control Bits */
#define TECXFLTPOL0_L       (0x0001)  /* TEV Polarity Bit of ext. fault 0 */
#define TECXFLTPOL1_L       (0x0002)  /* TEV Polarity Bit of ext. fault 1 */
#define TECXFLTPOL2_L       (0x0004)  /* TEV Polarity Bit of ext. fault 2 */
#define TECXFLTPOL3_L       (0x0008)  /* TEV Polarity Bit of ext. fault 3 */
#define TECXFLTPOL4_L       (0x0010)  /* TEV Polarity Bit of ext. fault 4 */
#define TECXFLTPOL5_L       (0x0020)  /* TEV Polarity Bit of ext. fault 5 */
#define TECXFLTPOL6_L       (0x0040)  /* TEV Polarity Bit of ext. fault 6 */

/* TECxXCTL1 Control Bits */
#define TECXFLTLVS0_H       (0x0001)  /* TEV Signal Type of Ext. fault 0 */
#define TECXFLTLVS1_H       (0x0002)  /* TEV Signal Type of Ext. fault 1 */
#define TECXFLTLVS2_H       (0x0004)  /* TEV Signal Type of Ext. fault 2 */
#define TECXFLTLVS3_H       (0x0008)  /* TEV Signal Type of Ext. fault 3 */
#define TECXFLTLVS4_H       (0x0010)  /* TEV Signal Type of Ext. fault 4 */
#define TECXFLTLVS5_H       (0x0020)  /* TEV Signal Type of Ext. fault 5 */
#define TECXFLTLVS6_H       (0x0040)  /* TEV Signal Type of Ext. fault 6 */

/* TECxXCTL2 Control Bits */
#define TECCLKSEL0          (0x0001)  /* TEV Aux. Clock Select Bit: 0 */
#define TECCLKSEL1          (0x0002)  /* TEV Aux. Clock Select Bit: 1 */
#define TECAXCLREN          (0x0004)  /* TEV Auxilary clear signal control */
#define TECEXCLREN          (0x0008)  /* TEV Ext. clear signal control */
#define TECEXCLRHLD         (0x0010)  /* TEV External clear signal hold bit */
#define TECEXCLRPOL         (0x0020)  /* TEV Polarity Bit of ext. clear */
#define TECEXCLRLVS         (0x0040)  /* TEV Signal Type of Ext. clear */

/* TECxXCTL2 Control Bits */
#define TECCLKSEL0_L        (0x0001)  /* TEV Aux. Clock Select Bit: 0 */
#define TECCLKSEL1_L        (0x0002)  /* TEV Aux. Clock Select Bit: 1 */
#define TECAXCLREN_L        (0x0004)  /* TEV Auxilary clear signal control */
#define TECEXCLREN_L        (0x0008)  /* TEV Ext. clear signal control */
#define TECEXCLRHLD_L       (0x0010)  /* TEV External clear signal hold bit */
#define TECEXCLRPOL_L       (0x0020)  /* TEV Polarity Bit of ext. clear */
#define TECEXCLRLVS_L       (0x0040)  /* TEV Signal Type of Ext. clear */


#define TECCLKSEL_0         (0x0000)  /* TEV Aux. Clock Select: CLK0 */
#define TECCLKSEL_1         (0x0001)  /* TEV Aux. Clock Select: CLK1 */
#define TECCLKSEL_2         (0x0002)  /* TEV Aux. Clock Select: CLK2 */
#define TECCLKSEL_3         (0x0003)  /* TEV Aux. Clock Select: CLK3 */

/* TECxSTA Control Bits */
#define TECXFLT0STA         (0x0001)  /* TEV External fault status flag for CE0 */
#define TECXFLT1STA         (0x0002)  /* TEV External fault status flag for CE1 */
#define TECXFLT2STA         (0x0004)  /* TEV External fault status flag for CE2 */
#define TECXFLT3STA         (0x0008)  /* TEV External fault status flag for CE3 */
#define TECXFLT4STA         (0x0010)  /* TEV External fault status flag for CE4 */
#define TECXFLT5STA         (0x0020)  /* TEV External fault status flag for CE5 */
#define TECXFLT6STA         (0x0040)  /* TEV External fault status flag for CE6 */
#define TECXCLRSTA          (0x0100)  /* TEC External clear status flag */

/* TECxSTA Control Bits */
#define TECXFLT0STA_L       (0x0001)  /* TEV External fault status flag for CE0 */
#define TECXFLT1STA_L       (0x0002)  /* TEV External fault status flag for CE1 */
#define TECXFLT2STA_L       (0x0004)  /* TEV External fault status flag for CE2 */
#define TECXFLT3STA_L       (0x0008)  /* TEV External fault status flag for CE3 */
#define TECXFLT4STA_L       (0x0010)  /* TEV External fault status flag for CE4 */
#define TECXFLT5STA_L       (0x0020)  /* TEV External fault status flag for CE5 */
#define TECXFLT6STA_L       (0x0040)  /* TEV External fault status flag for CE6 */

/* TECxSTA Control Bits */
#define TECXCLRSTA_H        (0x0001)  /* TEC External clear status flag */

/* TECxXINT Control Bits */
#define TECAXCLRIFG         (0x0001)   /* TEC Aux. Clear Interrupt Flag */
#define TECEXCLRIFG         (0x0002)   /* TEC External Clear Interrupt Flag */
#define TECXFLTIFG          (0x0004)   /* TEC External Fault Interrupt Flag */
#define TECAXCLRIE          (0x0100)   /* TEC Aux. Clear Interrupt Enable */
#define TECEXCLRIE          (0x0200)   /* TEC External Clear Interrupt Enable */
#define TECXFLTIE           (0x0400)   /* TEC External Fault Interrupt Enable */

/* TECxXINT Control Bits */
#define TECAXCLRIFG_L       (0x0001)   /* TEC Aux. Clear Interrupt Flag */
#define TECEXCLRIFG_L       (0x0002)   /* TEC External Clear Interrupt Flag */
#define TECXFLTIFG_L        (0x0004)   /* TEC External Fault Interrupt Flag */

/* TECxXINT Control Bits */
#define TECAXCLRIE_H        (0x0001)   /* TEC Aux. Clear Interrupt Enable */
#define TECEXCLRIE_H        (0x0002)   /* TEC External Clear Interrupt Enable */
#define TECXFLTIE_H         (0x0004)   /* TEC External Fault Interrupt Enable */

/* TEC0IV Definitions */
#define TEC0IV_NONE         (0x0000)    /* No Interrupt pending */
#define TEC0IV_TECXFLTIFG   (0x0002)    /* TEC0XFLTIFG */
#define TEC0IV_TECEXCLRIFG  (0x0004)    /* TEC0EXCLRIFG */
#define TEC0IV_TECAXCLRIFG  (0x0006)    /* TEC0AXCLRIFG */

/************************************************************
* Timer Event Control 1
************************************************************/
#define __MSP430_HAS_TEV1__             /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_TEV1__ 0x0C20
#define TEC1_BASE __MSP430_BASEADDRESS_TEV1__

#define TEC1XCTL0_            0x0C20    /* Timer Event Control 1 External Control 0 */
sfrb(TEC1XCTL0_L , TEC1XCTL0_);
sfrb(TEC1XCTL0_H , TEC1XCTL0_+1);
sfrw(TEC1XCTL0, TEC1XCTL0_);
#define TEC1XCTL1_            0x0C22    /* Timer Event Control 1 External Control 1 */
sfrb(TEC1XCTL1_L , TEC1XCTL1_);
sfrb(TEC1XCTL1_H , TEC1XCTL1_+1);
sfrw(TEC1XCTL1, TEC1XCTL1_);
#define TEC1XCTL2_            0x0C24    /* Timer Event Control 1 External Control 2 */
sfrb(TEC1XCTL2_L , TEC1XCTL2_);
sfrb(TEC1XCTL2_H , TEC1XCTL2_+1);
sfrw(TEC1XCTL2, TEC1XCTL2_);
#define TEC1STA_              0x0C26    /* Timer Event Control 1 Status */
sfrb(TEC1STA_L , TEC1STA_);
sfrb(TEC1STA_H , TEC1STA_+1);
sfrw(TEC1STA, TEC1STA_);
#define TEC1XINT_             0x0C28    /* Timer Event Control 1 External Interrupt */
sfrb(TEC1XINT_L , TEC1XINT_);
sfrb(TEC1XINT_H , TEC1XINT_+1);
sfrw(TEC1XINT, TEC1XINT_);
#define TEC1IV_               0x0C2A    /* Timer Event Control 1 Interrupt Vector */
sfrb(TEC1IV_L , TEC1IV_);
sfrb(TEC1IV_H , TEC1IV_+1);
sfrw(TEC1IV, TEC1IV_);


/* TECIV Definitions */
#define TEC1IV_NONE         (0x0000)    /* No Interrupt pending */
#define TEC1IV_TECXFLTIFG   (0x0002)    /* TEC1XFLTIFG */
#define TEC1IV_TECEXCLRIFG  (0x0004)    /* TEC1EXCLRIFG */
#define TEC1IV_TECAXCLRIFG  (0x0006)    /* TEC1AXCLRIFG */


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
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

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
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

/* UCSCTL6 Control Bits */
#define XT2OFF_H            (0x0001)    /* High Frequency Oscillator 2 (XT2) disable */
//#define RESERVED            (0x0200)    /* RESERVED */
//#define RESERVED            (0x0400)    /* RESERVED */
//#define RESERVED            (0x0800)    /* RESERVED */
//#define RESERVED            (0x1000)    /* RESERVED */
//#define RESERVED            (0x2000)    /* RESERVED */
//#define RESERVED            (0x4000)    /* RESERVED */
//#define RESERVED            (0x8000)    /* RESERVED */

#define XCAP_0              (0x0000)    /* XIN/XOUT Cap 0 */
#define XCAP_1              (0x0004)    /* XIN/XOUT Cap 1 */
#define XCAP_2              (0x0008)    /* XIN/XOUT Cap 2 */
#define XCAP_3              (0x000C)    /* XIN/XOUT Cap 3 */
#define XT1DRIVE_0          (0x0000)    /* XT1 Drive Level mode: 0 */
#define XT1DRIVE_1          (0x0040)    /* XT1 Drive Level mode: 1 */
#define XT1DRIVE_2          (0x0080)    /* XT1 Drive Level mode: 2 */
#define XT1DRIVE_3          (0x00C0)    /* XT1 Drive Level mode: 3 */


/* UCSCTL7 Control Bits */
#define DCOFFG              (0x0001)    /* DCO Fault Flag */
#define XT1LFOFFG           (0x0002)    /* XT1 Low Frequency Oscillator Fault Flag */
#define XT1HFOFFG           (0x0004)    /* XT1 High Frequency Oscillator 1 Fault Flag */
//#define RESERVED            (0x0008)    /* RESERVED */
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
#define XT1HFOFFG_L         (0x0004)    /* XT1 High Frequency Oscillator 1 Fault Flag */
//#define RESERVED            (0x0008)    /* RESERVED */
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
#define __MSP430_HAS_USCI_A0__       /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_USCI_A0__ 0x05C0
#define USCI_A0_BASE __MSP430_BASEADDRESS_USCI_A0__

#define UCA0CTLW0_            0x05C0    /* USCI A0 Control Word Register 0 */
sfrb(UCA0CTLW0_L , UCA0CTLW0_);
sfrb(UCA0CTLW0_H , UCA0CTLW0_+1);
sfrw(UCA0CTLW0, UCA0CTLW0_);
#define UCA0CTL1            UCA0CTLW0_L  /* USCI A0 Control Register 1 */
#define UCA0CTL0            UCA0CTLW0_H  /* USCI A0 Control Register 0 */
#define UCA0BRW_              0x05C6    /* USCI A0 Baud Word Rate 0 */
sfrb(UCA0BRW_L , UCA0BRW_);
sfrb(UCA0BRW_H , UCA0BRW_+1);
sfrw(UCA0BRW, UCA0BRW_);
#define UCA0BR0             UCA0BRW_L /* USCI A0 Baud Rate 0 */
#define UCA0BR1             UCA0BRW_H /* USCI A0 Baud Rate 1 */
#define UCA0MCTL_             0x05C8    /* USCI A0 Modulation Control */
sfrb(UCA0MCTL, UCA0MCTL_);
#define UCA0STAT_             0x05CA    /* USCI A0 Status Register */
sfrb(UCA0STAT, UCA0STAT_);
#define UCA0RXBUF_            0x05CC    /* USCI A0 Receive Buffer */
const_sfrb(UCA0RXBUF, UCA0RXBUF_);
#define UCA0TXBUF_            0x05CE    /* USCI A0 Transmit Buffer */
sfrb(UCA0TXBUF, UCA0TXBUF_);
#define UCA0ABCTL_            0x05D0    /* USCI A0 LIN Control */
sfrb(UCA0ABCTL, UCA0ABCTL_);
#define UCA0IRCTL_            0x05D2    /* USCI A0 IrDA Transmit Control */
sfrb(UCA0IRCTL_L , UCA0IRCTL_);
sfrb(UCA0IRCTL_H , UCA0IRCTL_+1);
sfrw(UCA0IRCTL, UCA0IRCTL_);
#define UCA0IRTCTL          UCA0IRCTL_L  /* USCI A0 IrDA Transmit Control */
#define UCA0IRRCTL          UCA0IRCTL_H  /* USCI A0 IrDA Receive Control */
#define UCA0ICTL_             0x05DC    /* USCI A0 Interrupt Enable Register */
sfrb(UCA0ICTL_L , UCA0ICTL_);
sfrb(UCA0ICTL_H , UCA0ICTL_+1);
sfrw(UCA0ICTL, UCA0ICTL_);
#define UCA0IE              UCA0ICTL_L  /* USCI A0 Interrupt Enable Register */
#define UCA0IFG             UCA0ICTL_H  /* USCI A0 Interrupt Flags Register */
#define UCA0IV_               0x05DE    /* USCI A0 Interrupt Vector Register */
sfrw(UCA0IV, UCA0IV_);


/************************************************************
* USCI B0
************************************************************/
#define __MSP430_HAS_USCI_B0__        /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_USCI_B0__ 0x05E0
#define USCI_B0_BASE __MSP430_BASEADDRESS_USCI_B0__


#define UCB0CTLW0_            0x05E0    /* USCI B0 Control Word Register 0 */
sfrb(UCB0CTLW0_L , UCB0CTLW0_);
sfrb(UCB0CTLW0_H , UCB0CTLW0_+1);
sfrw(UCB0CTLW0, UCB0CTLW0_);
#define UCB0CTL1            UCB0CTLW0_L  /* USCI B0 Control Register 1 */
#define UCB0CTL0            UCB0CTLW0_H  /* USCI B0 Control Register 0 */
#define UCB0BRW_              0x05E6    /* USCI B0 Baud Word Rate 0 */
sfrb(UCB0BRW_L , UCB0BRW_);
sfrb(UCB0BRW_H , UCB0BRW_+1);
sfrw(UCB0BRW, UCB0BRW_);
#define UCB0BR0             UCB0BRW_L /* USCI B0 Baud Rate 0 */
#define UCB0BR1             UCB0BRW_H /* USCI B0 Baud Rate 1 */
#define UCB0STAT_             0x05EA    /* USCI B0 Status Register */
sfrb(UCB0STAT, UCB0STAT_);
#define UCB0RXBUF_            0x05EC    /* USCI B0 Receive Buffer */
const_sfrb(UCB0RXBUF, UCB0RXBUF_);
#define UCB0TXBUF_            0x05EE    /* USCI B0 Transmit Buffer */
sfrb(UCB0TXBUF, UCB0TXBUF_);
#define UCB0I2COA_            0x05F0    /* USCI B0 I2C Own Address */
sfrb(UCB0I2COA_L , UCB0I2COA_);
sfrb(UCB0I2COA_H , UCB0I2COA_+1);
sfrw(UCB0I2COA, UCB0I2COA_);
#define UCB0I2CSA_            0x05F2    /* USCI B0 I2C Slave Address */
sfrb(UCB0I2CSA_L , UCB0I2CSA_);
sfrb(UCB0I2CSA_H , UCB0I2CSA_+1);
sfrw(UCB0I2CSA, UCB0I2CSA_);
#define UCB0ICTL_             0x05FC    /* USCI B0 Interrupt Enable Register */
sfrb(UCB0ICTL_L , UCB0ICTL_);
sfrb(UCB0ICTL_H , UCB0ICTL_+1);
sfrw(UCB0ICTL, UCB0ICTL_);
#define UCB0IE              UCB0ICTL_L  /* USCI B0 Interrupt Enable Register */
#define UCB0IFG             UCB0ICTL_H  /* USCI B0 Interrupt Flags Register */
#define UCB0IV_               0x05FE    /* USCI B0 Interrupt Vector Register */
sfrw(UCB0IV, UCB0IV_);


// UCAxCTL0 UART-Mode Control Bits
#define UCPEN               (0x80)    /* Async. Mode: Parity enable */
#define UCPAR               (0x40)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x20)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x10)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x08)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x04)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x02)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x01)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTL0 SPI-Mode Control Bits
#define UCCKPH              (0x80)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x40)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x08)    /* Sync. Mode: Master Select */

// UCBxCTL0 I2C-Mode Control Bits
#define UCA10               (0x80)    /* 10-bit Address Mode */
#define UCSLA10             (0x40)    /* 10-bit Slave Address Mode */
#define UCMM                (0x20)    /* Multi-Master Environment */
//#define res               (0x10)    /* reserved */
#define UCMODE_0            (0x00)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x02)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x04)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x06)    /* Sync. Mode: USCI Mode: 3 */

// UCAxCTL1 UART-Mode Control Bits
#define UCSSEL1             (0x80)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x40)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x20)    /* RX Error interrupt enable */
#define UCBRKIE             (0x10)    /* Break interrupt enable */
#define UCDORM              (0x08)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x04)    /* Send next Data as Address */
#define UCTXBRK             (0x02)    /* Send next Data as Break */
#define UCSWRST             (0x01)    /* USCI Software Reset */

// UCxxCTL1 SPI-Mode Control Bits
//#define res               (0x20)    /* reserved */
//#define res               (0x10)    /* reserved */
//#define res               (0x08)    /* reserved */
//#define res               (0x04)    /* reserved */
//#define res               (0x02)    /* reserved */

// UCBxCTL1 I2C-Mode Control Bits
//#define res               (0x20)    /* reserved */
#define UCTR                (0x10)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x08)    /* Transmit NACK */
#define UCTXSTP             (0x04)    /* Transmit STOP */
#define UCTXSTT             (0x02)    /* Transmit START */
#define UCSSEL_0            (0x00)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x40)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x80)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0xC0)    /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK        (0x00)    /* USCI 0 Clock Source: UCLK */
#define UCSSEL__ACLK        (0x40)    /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK       (0x80)    /* USCI 0 Clock Source: SMCLK */

/* UCAxMCTL Control Bits */
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

/* UCAxSTAT Control Bits */
#define UCLISTEN            (0x80)    /* USCI Listen mode */
#define UCFE                (0x40)    /* USCI Frame Error Flag */
#define UCOE                (0x20)    /* USCI Overrun Error Flag */
#define UCPE                (0x10)    /* USCI Parity Error Flag */
#define UCBRK               (0x08)    /* USCI Break received */
#define UCRXERR             (0x04)    /* USCI RX Error Flag */
#define UCADDR              (0x02)    /* USCI Address received Flag */
#define UCBUSY              (0x01)    /* USCI Busy Flag */
#define UCIDLE              (0x02)    /* USCI Idle line detected Flag */


/* UCBxSTAT Control Bits */
#define UCSCLLOW            (0x40)    /* SCL low */
#define UCGC                (0x20)    /* General Call address received Flag */
#define UCBBUSY             (0x10)    /* Bus Busy Flag */

/* UCAxIRTCTL Control Bits */
#define UCIRTXPL5           (0x80)    /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4           (0x40)    /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3           (0x20)    /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2           (0x10)    /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1           (0x08)    /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0           (0x04)    /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK           (0x02)    /* IRDA Transmit Pulse Clock Select */
#define UCIREN              (0x01)    /* IRDA Encoder/Decoder enable */

/* UCAxIRRCTL Control Bits */
#define UCIRRXFL5           (0x80)    /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4           (0x40)    /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3           (0x20)    /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2           (0x10)    /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1           (0x08)    /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0           (0x04)    /* IRDA Receive Filter Length 0 */
#define UCIRRXPL            (0x02)    /* IRDA Receive Input Polarity */
#define UCIRRXFE            (0x01)    /* IRDA Receive Filter enable */

/* UCAxABCTL Control Bits */
//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1            (0x20)    /* Break Sync Delimiter 1 */
#define UCDELIM0            (0x10)    /* Break Sync Delimiter 0 */
#define UCSTOE              (0x08)    /* Sync-Field Timeout error */
#define UCBTOE              (0x04)    /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN             (0x01)    /* Auto Baud Rate detect enable */

/* UCBxI2COA Control Bits */
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

/* UCBxI2COA Control Bits */
#define UCOA7_L             (0x0080)  /* I2C Own Address 7 */
#define UCOA6_L             (0x0040)  /* I2C Own Address 6 */
#define UCOA5_L             (0x0020)  /* I2C Own Address 5 */
#define UCOA4_L             (0x0010)  /* I2C Own Address 4 */
#define UCOA3_L             (0x0008)  /* I2C Own Address 3 */
#define UCOA2_L             (0x0004)  /* I2C Own Address 2 */
#define UCOA1_L             (0x0002)  /* I2C Own Address 1 */
#define UCOA0_L             (0x0001)  /* I2C Own Address 0 */

/* UCBxI2COA Control Bits */
#define UCGCEN_H            (0x0080)  /* I2C General Call enable */
#define UCOA9_H             (0x0002)  /* I2C Own Address 9 */
#define UCOA8_H             (0x0001)  /* I2C Own Address 8 */

/* UCBxI2CSA Control Bits */
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

/* UCBxI2CSA Control Bits */
#define UCSA7_L             (0x0080)  /* I2C Slave Address 7 */
#define UCSA6_L             (0x0040)  /* I2C Slave Address 6 */
#define UCSA5_L             (0x0020)  /* I2C Slave Address 5 */
#define UCSA4_L             (0x0010)  /* I2C Slave Address 4 */
#define UCSA3_L             (0x0008)  /* I2C Slave Address 3 */
#define UCSA2_L             (0x0004)  /* I2C Slave Address 2 */
#define UCSA1_L             (0x0002)  /* I2C Slave Address 1 */
#define UCSA0_L             (0x0001)  /* I2C Slave Address 0 */

/* UCBxI2CSA Control Bits */
#define UCSA9_H             (0x0002)  /* I2C Slave Address 9 */
#define UCSA8_H             (0x0001)  /* I2C Slave Address 8 */

/* UCAxIE Control Bits */
#define UCTXIE              (0x0002)  /* USCI Transmit Interrupt Enable */
#define UCRXIE              (0x0001)  /* USCI Receive Interrupt Enable */

/* UCBxIE Control Bits */
#define UCNACKIE            (0x0020)  /* NACK Condition interrupt enable */
#define UCALIE              (0x0010)  /* Arbitration Lost interrupt enable */
#define UCSTPIE             (0x0008)  /* STOP Condition interrupt enable */
#define UCSTTIE             (0x0004)  /* START Condition interrupt enable */
#define UCTXIE              (0x0002)  /* USCI Transmit Interrupt Enable */
#define UCRXIE              (0x0001)  /* USCI Receive Interrupt Enable */

/* UCAxIFG Control Bits */
#define UCTXIFG             (0x0002)  /* USCI Transmit Interrupt Flag */
#define UCRXIFG             (0x0001)  /* USCI Receive Interrupt Flag */

/* UCBxIFG Control Bits */
#define UCNACKIFG           (0x0020)  /* NAK Condition interrupt Flag */
#define UCALIFG             (0x0010)  /* Arbitration Lost interrupt Flag */
#define UCSTPIFG            (0x0008)  /* STOP Condition interrupt Flag */
#define UCSTTIFG            (0x0004)  /* START Condition interrupt Flag */
#define UCTXIFG             (0x0002)  /* USCI Transmit Interrupt Flag */
#define UCRXIFG             (0x0001)  /* USCI Receive Interrupt Flag */

/* USCI Definitions */
#define USCI_NONE           (0x0000)    /* No Interrupt pending */
#define USCI_UCRXIFG        (0x0002)    /* USCI UCRXIFG */
#define USCI_UCTXIFG        (0x0004)    /* USCI UCTXIFG */
#define USCI_I2C_UCALIFG    (0x0002)    /* USCI I2C Mode: UCALIFG */
#define USCI_I2C_UCNACKIFG  (0x0004)    /* USCI I2C Mode: UCNACKIFG */
#define USCI_I2C_UCSTTIFG   (0x0006)    /* USCI I2C Mode: UCSTTIFG*/
#define USCI_I2C_UCSTPIFG   (0x0008)    /* USCI I2C Mode: UCSTPIFG*/
#define USCI_I2C_UCRXIFG    (0x000A)    /* USCI I2C Mode: UCRXIFG */
#define USCI_I2C_UCTXIFG    (0x000C)    /* USCI I2C Mode: UCTXIFG */

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
#define __MSP430_HAS_TLV__                /* Definition to show that Module is available */
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
#define TLV_REFCAL            (0x12)      /*  REF calibration */
#define TLV_ADC10CAL          (0x13)      /*  ADC10 calibration */
#define TLV_TIMERDCAL         (0x15)      /*  TIMER_D calibration */
#define TLV_TAGEXT            (0xFE)      /*  Tag extender */
#define TLV_TAGEND            (0xFF)      /*  Tag End of Table */

/************************************************************
* Interrupt Vectors (offset from 0xFF80)
************************************************************/


#define PORT2_VECTOR            (46)                     /* 0xFFDA Port 2 */
#define PORT1_VECTOR            (47)                     /* 0xFFDC Port 1 */
#define TIMER1_D1_VECTOR        (48)                     /* 0xFFDE Timer1_D3 CC1-2, TA1 */
#define TIMER1_D0_VECTOR        (49)                     /* 0xFFE0 Timer1_D3 CC0 */
#define TEC1_VECTOR             (50)                     /* 0xFFF2 Timer Event Controller 1 */
#define DMA_VECTOR              (51)                     /* 0xFFE4 DMA */
#define TIMER0_A1_VECTOR        (52)                     /* 0xFFE6 Timer0_A3 CC1-2, TA */
#define TIMER0_A0_VECTOR        (53)                     /* 0xFFE8 Timer0_A3 CC0 */
#define USCI_B0_VECTOR          (55)                     /* 0xFFEC USCI B0 Receive/Transmit */
#define USCI_A0_VECTOR          (56)                     /* 0xFFEE USCI A0 Receive/Transmit */
#define WDT_VECTOR              (57)                     /* 0xFFF0 Watchdog Timer */
#define TIMER0_D1_VECTOR        (58)                     /* 0xFFE2 Timer0_D3 CC1-2, TA */
#define TIMER0_D0_VECTOR        (59)                     /* 0xFFE4 Timer0_D3 CC0 */
#define TEC0_VECTOR             (60)                     /* 0xFFF6 Timer Event Controller 0 */
#define COMP_B_VECTOR           (61)                     /* 0xFFF8 Watchdog Timer */
#define UNMI_VECTOR             (62)                     /* 0xFFFA User Non-maskable */
#define SYSNMI_VECTOR           (63)                     /* 0xFFFC System Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __MSP430F5131 */


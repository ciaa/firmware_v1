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
* MSP430x09x devices.
*
* Texas Instruments, Version 1.1
*
* Rev. 1.0, Initial Release
* Rev. 1.0, Added LCMP : A-POOL Latch comparator
*
*
********************************************************************/

#ifndef __msp430x09x
#define __msp430x09x

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
* A-POOL
************************************************************/
#define __MSP430_HAS_APOOL__           /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_APOOL__ 0x01A0
#define APOOL_BASE __MSP430_BASEADDRESS_APOOL__

#define APCNF_                0x01A0    /* A-POOL Configuration Register */
sfrb(APCNF_L , APCNF_);
sfrb(APCNF_H , APCNF_+1);
sfrw(APCNF, APCNF_);
#define APCTL_                0x01A2    /* A-POOL Control Register 1 */
sfrb(APCTL_L , APCTL_);
sfrb(APCTL_H , APCTL_+1);
sfrw(APCTL, APCTL_);
#define APOMR_                0x01A4    /* A-POOL Operation Mode Register 2 */
sfrb(APOMR_L , APOMR_);
sfrb(APOMR_H , APOMR_+1);
sfrw(APOMR, APOMR_);
#define APVDIV_               0x01A6    /* A-POOL Voltage Divider Register 3 */
sfrb(APVDIV_L , APVDIV_);
sfrb(APVDIV_H , APVDIV_+1);
sfrw(APVDIV, APVDIV_);
#define APTRIM_               0x01A8    /* A-POOL trimming register */
sfrb(APTRIM_L , APTRIM_);
sfrb(APTRIM_H , APTRIM_+1);
sfrw(APTRIM, APTRIM_);
#define APINT_                0x01B0    /* A-POOL Integer Conversion Register */
sfrb(APINT_L , APINT_);
sfrb(APINT_H , APINT_+1);
sfrw(APINT, APINT_);
#define APINTB_               0x01B2    /* A-POOL Integer Conversion Buffer Register */
sfrb(APINTB_L , APINTB_);
sfrb(APINTB_H , APINTB_+1);
sfrw(APINTB, APINTB_);
#define APFRACT_              0x01B4    /* A-POOL Fractional Conversion Register */
sfrb(APFRACT_L , APFRACT_);
sfrb(APFRACT_H , APFRACT_+1);
sfrw(APFRACT, APFRACT_);
#define APFRACTB_             0x01B6    /* A-POOL Fractional Conversion Buffer Register */
sfrb(APFRACTB_L , APFRACTB_);
sfrb(APFRACTB_H , APFRACTB_+1);
sfrw(APFRACTB, APFRACTB_);
#define APIFG_                0x01BA    /* A-POOL Interrupt Flag Register */
sfrb(APIFG_L , APIFG_);
sfrb(APIFG_H , APIFG_+1);
sfrw(APIFG, APIFG_);
#define APIE_                 0x01BC    /* A-POOL Interrupt Enable Register */
sfrb(APIE_L , APIE_);
sfrb(APIE_H , APIE_+1);
sfrw(APIE, APIE_);
#define APIV_                 0x01BE    /* A-POOL Interrupt Vector Word */
const_sfrb(APIV_L , APIV_);
const_sfrb(APIV_H , APIV_+1);
const_sfrw(APIV, APIV_);


/* APCNF Control Bits */
#define TA0EN                (0x0001)  /* A-POOL TimerA0 trigger enable */
#define TA1EN                (0x0002)  /* A-POOL TimerA1 trigger enable */
#define DFSET0               (0x0004)  /* A-POOL Deglitch Filter Bit: 0 */
#define DFSET1               (0x0008)  /* A-POOL Deglitch Filter Bit: 1 */
#define LCMP                 (0x0010)  /* A-POOL Latch comparator */
#define CMPON                (0x0020)  /* A-POOL Comparator enable */
#define DBON                 (0x0040)  /* A-POOL DAC buffer enable signal */
#define CONVON               (0x0080)  /* A-POOL Enable for converter’s resistor ladder */
#define CLKSEL0              (0x0100)  /* A-POOL Conversion clock select Bit: 0 */
#define CLKSEL1              (0x0200)  /* A-POOL Conversion clock select Bit: 1 */
#define EOCBU                (0x0400)  /* A-POOL Enable bit for loading conversion buffer */
#define ATBU                 (0x0800)  /* A-POOL Automatic update of conversion register */
#define A3PSEL               (0x1000)  /* A-POOL Analog input A3 access Bit */
#define APREFON              (0x2000)  /* A-POOL Internal voltage reference enable */
#define VREFEN               (0x4000)  /* A-POOL Reference voltage pin enable */
//#define RESERVED             (0x8000)  /* A-POOL */

/* APCNF Control Bits */
#define TA0EN_L             (0x0001)  /* A-POOL TimerA0 trigger enable */
#define TA1EN_L             (0x0002)  /* A-POOL TimerA1 trigger enable */
#define DFSET0_L            (0x0004)  /* A-POOL Deglitch Filter Bit: 0 */
#define DFSET1_L            (0x0008)  /* A-POOL Deglitch Filter Bit: 1 */
#define LCMP_L              (0x0010)  /* A-POOL Latch comparator */
#define CMPON_L             (0x0020)  /* A-POOL Comparator enable */
#define DBON_L              (0x0040)  /* A-POOL DAC buffer enable signal */
#define CONVON_L            (0x0080)  /* A-POOL Enable for converter’s resistor ladder */
//#define RESERVED             (0x8000)  /* A-POOL */

/* APCNF Control Bits */
#define CLKSEL0_H           (0x0001)  /* A-POOL Conversion clock select Bit: 0 */
#define CLKSEL1_H           (0x0002)  /* A-POOL Conversion clock select Bit: 1 */
#define EOCBU_H             (0x0004)  /* A-POOL Enable bit for loading conversion buffer */
#define ATBU_H              (0x0008)  /* A-POOL Automatic update of conversion register */
#define A3PSEL_H            (0x0010)  /* A-POOL Analog input A3 access Bit */
#define APREFON_H           (0x0020)  /* A-POOL Internal voltage reference enable */
#define VREFEN_H            (0x0040)  /* A-POOL Reference voltage pin enable */
//#define RESERVED             (0x8000)  /* A-POOL */

#define DFSET_0              (0x0000)  /* A-POOL Deglitch Filter select: 0 */
#define DFSET_1              (0x0004)  /* A-POOL Deglitch Filter select: 1 */
#define DFSET_2              (0x0008)  /* A-POOL Deglitch Filter select: 2 */
#define DFSET_3              (0x000C)  /* A-POOL Deglitch Filter select: 3 */

#define CLKSEL_0             (0x0000)  /* A-POOL Conversion clock select: 0 */
#define CLKSEL_1             (0x0100)  /* A-POOL Conversion clock select: 1 */
#define CLKSEL_2             (0x0200)  /* A-POOL Conversion clock select: 2 */
#define CLKSEL_VLOCLK        (0x0000)  /* A-POOL Conversion clock select: VLOCLK */
#define CLKSEL_MCLK          (0x0100)  /* A-POOL Conversion clock select: MCLK */
#define CLKSEL_SMCLK         (0x0200)  /* A-POOL Conversion clock select: SMCLK */

/* APCTL Control Bits */
#define ODEN                 (0x0001)  /* A-POOL Output driver enable */
#define OSWP                 (0x0002)  /* A-POOL Output swap */
#define OSEL                 (0x0004)  /* A-POOL Output buffer select */
#define SLOPE                (0x0008)  /* A-POOL Slope select of converter */
#define APNSEL0              (0x0010)  /* A-POOL Neg. Channel Input Select 0 */
#define APNSEL1              (0x0020)  /* A-POOL Neg. Channel Input Select 1 */
#define APNSEL2              (0x0040)  /* A-POOL Neg. Channel Input Select 2 */
#define APNSEL3              (0x0080)  /* A-POOL Neg. Channel Input Select 3 */
#define RUNSTOP              (0x0100)  /* A-POOL Converter’s Run/Stop bit */
#define SBSTP                (0x0200)  /* A-POOL Saturation based conversion stop enable */
#define CBSTP                (0x0400)  /* A-POOL Comparator based conversion stop enable */
#define TBSTP                (0x0800)  /* A-POOL Timer based conversion stop enable for TimerA0 */
#define APPSEL0              (0x1000)  /* A-POOL Pos. Channel Input Select 0 */
#define APPSEL1              (0x2000)  /* A-POOL Pos. Channel Input Select 1 */
#define APPSEL2              (0x4000)  /* A-POOL Pos. Channel Input Select 2 */
#define APPSEL3              (0x8000)  /* A-POOL Pos. Channel Input Select 3 */

/* APCTL Control Bits */
#define ODEN_L              (0x0001)  /* A-POOL Output driver enable */
#define OSWP_L              (0x0002)  /* A-POOL Output swap */
#define OSEL_L              (0x0004)  /* A-POOL Output buffer select */
#define SLOPE_L             (0x0008)  /* A-POOL Slope select of converter */
#define APNSEL0_L           (0x0010)  /* A-POOL Neg. Channel Input Select 0 */
#define APNSEL1_L           (0x0020)  /* A-POOL Neg. Channel Input Select 1 */
#define APNSEL2_L           (0x0040)  /* A-POOL Neg. Channel Input Select 2 */
#define APNSEL3_L           (0x0080)  /* A-POOL Neg. Channel Input Select 3 */

/* APCTL Control Bits */
#define RUNSTOP_H           (0x0001)  /* A-POOL Converter’s Run/Stop bit */
#define SBSTP_H             (0x0002)  /* A-POOL Saturation based conversion stop enable */
#define CBSTP_H             (0x0004)  /* A-POOL Comparator based conversion stop enable */
#define TBSTP_H             (0x0008)  /* A-POOL Timer based conversion stop enable for TimerA0 */
#define APPSEL0_H           (0x0010)  /* A-POOL Pos. Channel Input Select 0 */
#define APPSEL1_H           (0x0020)  /* A-POOL Pos. Channel Input Select 1 */
#define APPSEL2_H           (0x0040)  /* A-POOL Pos. Channel Input Select 2 */
#define APPSEL3_H           (0x0080)  /* A-POOL Pos. Channel Input Select 3 */

#define APNSEL_0             (0x0000)  /* A-POOL V- terminal Input Select: Channel 0 */
#define APNSEL_1             (0x0010)  /* A-POOL V- terminal Input Select: Channel 1 */
#define APNSEL_2             (0x0020)  /* A-POOL V- terminal Input Select: Channel 2 */
#define APNSEL_3             (0x0030)  /* A-POOL V- terminal Input Select: Channel 3 */
#define APNSEL_4             (0x0040)  /* A-POOL V- terminal Input Select: Channel 4 */
#define APNSEL_5             (0x0050)  /* A-POOL V- terminal Input Select: Channel 5 */
#define APNSEL_6             (0x0060)  /* A-POOL V- terminal Input Select: Channel 6 */
#define APNSEL_7             (0x0070)  /* A-POOL V- terminal Input Select: Channel 7 */

#define APPSEL_0             (0x0000)  /* A-POOL V+ Terminal Input Select: Channel 0 */
#define APPSEL_1             (0x1000)  /* A-POOL V+ Terminal Input Select: Channel 1 */
#define APPSEL_2             (0x2000)  /* A-POOL V+ Terminal Input Select: Channel 2 */
#define APPSEL_3             (0x3000)  /* A-POOL V+ Terminal Input Select: Channel 3 */
#define APPSEL_4             (0x4000)  /* A-POOL V+ Terminal Input Select: Channel 4 */
#define APPSEL_5             (0x5000)  /* A-POOL V+ Terminal Input Select: Channel 5 */
#define APPSEL_6             (0x6000)  /* A-POOL V+ Terminal Input Select: Channel 6 */
#define APPSEL_7             (0x7000)  /* A-POOL V+ Terminal Input Select: Channel 7 */
#define APPSEL_8             (0x8000)  /* A-POOL V+ Terminal Input Select: Channel 8 */


/* APVDIV Control Bits */
#define A0DIV                (0x0001)  /* A-POOL Analog channel #0 voltage divider control */
#define A1DIV                (0x0002)  /* A-POOL Analog channel #1 voltage divider control */
#define A2DIV0               (0x0004)  /* A-POOL Analog channel #2 voltage divider control Bit : 0 */
#define A2DIV1               (0x0008)  /* A-POOL Analog channel #2 voltage divider control Bit : 1 */
#define A3DIV0               (0x0010)  /* A-POOL Analog channel #3 voltage divider control Bit : 0 */
#define A3DIV1               (0x0020)  /* A-POOL Analog channel #3 voltage divider control Bit : 0 */
#define TMPSEN               (0x0040)  /* A-POOL Temperature sensor enable */
#define VCCDIVEN             (0x0080)  /* A-POOL VCC voltage divider enable */
//#define RESERVED             (0x0100)  /* A-POOL */
//#define RESERVED             (0x0200)  /* A-POOL */
#define CLKTRIM0             (0x0400)  /* A-POOL Clock trimming Bit : 0 */
#define CLKTRIM1             (0x0800)  /* A-POOL Clock trimming Bit : 1 */
//#define RESERVED             (0x1000)  /* A-POOL */
//#define RESERVED             (0x2000)  /* A-POOL */
//#define RESERVED             (0x4000)  /* A-POOL */
//#define RESERVED             (0x8000)  /* A-POOL */

/* APVDIV Control Bits */
#define A0DIV_L             (0x0001)  /* A-POOL Analog channel #0 voltage divider control */
#define A1DIV_L             (0x0002)  /* A-POOL Analog channel #1 voltage divider control */
#define A2DIV0_L            (0x0004)  /* A-POOL Analog channel #2 voltage divider control Bit : 0 */
#define A2DIV1_L            (0x0008)  /* A-POOL Analog channel #2 voltage divider control Bit : 1 */
#define A3DIV0_L            (0x0010)  /* A-POOL Analog channel #3 voltage divider control Bit : 0 */
#define A3DIV1_L            (0x0020)  /* A-POOL Analog channel #3 voltage divider control Bit : 0 */
#define TMPSEN_L            (0x0040)  /* A-POOL Temperature sensor enable */
#define VCCDIVEN_L          (0x0080)  /* A-POOL VCC voltage divider enable */
//#define RESERVED             (0x0100)  /* A-POOL */
//#define RESERVED             (0x0200)  /* A-POOL */
//#define RESERVED             (0x1000)  /* A-POOL */
//#define RESERVED             (0x2000)  /* A-POOL */
//#define RESERVED             (0x4000)  /* A-POOL */
//#define RESERVED             (0x8000)  /* A-POOL */

/* APVDIV Control Bits */
//#define RESERVED             (0x0100)  /* A-POOL */
//#define RESERVED             (0x0200)  /* A-POOL */
#define CLKTRIM0_H          (0x0004)  /* A-POOL Clock trimming Bit : 0 */
#define CLKTRIM1_H          (0x0008)  /* A-POOL Clock trimming Bit : 1 */
//#define RESERVED             (0x1000)  /* A-POOL */
//#define RESERVED             (0x2000)  /* A-POOL */
//#define RESERVED             (0x4000)  /* A-POOL */
//#define RESERVED             (0x8000)  /* A-POOL */

#define A2DIV_0              (0x0000)  /* A-POOL Analog channel #2 voltage divider control: 0 */
#define A2DIV_1              (0x0004)  /* A-POOL Analog channel #2 voltage divider control: 1 */
#define A2DIV_2              (0x0008)  /* A-POOL Analog channel #2 voltage divider control: 2 */
#define A2DIV_3              (0x000C)  /* A-POOL Analog channel #2 voltage divider control: 3 */

#define A3DIV_0              (0x0000)  /* A-POOL Analog channel #3 voltage divider control: 0 */
#define A3DIV_1              (0x0010)  /* A-POOL Analog channel #3 voltage divider control: 1 */
#define A3DIV_2              (0x0020)  /* A-POOL Analog channel #3 voltage divider control: 2 */
#define A3DIV_3              (0x0030)  /* A-POOL Analog channel #3 voltage divider control: 3 */

#define CLKTRIM_0            (0x0000)  /* A-POOL Clock trimming: 0 */
#define CLKTRIM_1            (0x0400)  /* A-POOL Clock trimming: 1 */
#define CLKTRIM_2            (0x0800)  /* A-POOL Clock trimming: 2 */
#define CLKTRIM_3            (0x0C00)  /* A-POOL Clock trimming: 3 */

#define REFTRIM_0            (0x0000)  /* A-POOL Reference trimming: 0 */
#define REFTRIM_1            (0x1000)  /* A-POOL Reference trimming: 1 */
#define REFTRIM_2            (0x2000)  /* A-POOL Reference trimming: 2 */
#define REFTRIM_3            (0x3000)  /* A-POOL Reference trimming: 3 */
#define REFTRIM_4            (0x4000)  /* A-POOL Reference trimming: 4 */
#define REFTRIM_5            (0x5000)  /* A-POOL Reference trimming: 5 */
#define REFTRIM_6            (0x6000)  /* A-POOL Reference trimming: 6 */
#define REFTRIM_7            (0x7000)  /* A-POOL Reference trimming: 7 */


/* APTRIM Control Bits */
#define REFTSEL              (0x0001)  /* A-POOL Register bank used for the reference trimming */
#define REFTRIM0             (0x1000)  /* A-POOL Reference trimming bit: 0 */
#define REFTRIM1             (0x2000)  /* A-POOL Reference trimming bit: 1 */
#define REFTRIM2             (0x4000)  /* A-POOL Reference trimming bit: 2 */
#define REFTRIM3             (0x8000)  /* A-POOL Reference trimming bit: 3 */

/* APTRIM Control Bits */
#define REFTSEL_L           (0x0001)  /* A-POOL Register bank used for the reference trimming */

/* APTRIM Control Bits */
#define REFTRIM0_H          (0x0010)  /* A-POOL Reference trimming bit: 0 */
#define REFTRIM1_H          (0x0020)  /* A-POOL Reference trimming bit: 1 */
#define REFTRIM2_H          (0x0040)  /* A-POOL Reference trimming bit: 2 */
#define REFTRIM3_H          (0x0080)  /* A-POOL Reference trimming bit: 3 */

/* APOMR Control Bits */
#define CLKDIV0              (0x0001)  /* A-POOL Prescaler Control Bit: 0 */
#define CLKDIV1              (0x0002)  /* A-POOL Prescaler Control Bit: 1 */
#define CLKDIV2              (0x0004)  /* A-POOL Prescaler Control Bit: 2 */
#define SAREN                (0x0008)  /* A-POOL SAR conversion enable */
#define CTEN                 (0x0100)  /* A-POOL Continuous time mode of comparator */
#define AZCMP                (0x0200)  /* A-POOL Clocked zero compensated long term comparison */
#define AZSWREQ              (0x0400)  /* A-POOL SW request for Auto Zero Phase */
#define SVMINH               (0x0800)  /* A-POOL Suppress the generation of an SVM interrupt event. */

/* APOMR Control Bits */
#define CLKDIV0_L           (0x0001)  /* A-POOL Prescaler Control Bit: 0 */
#define CLKDIV1_L           (0x0002)  /* A-POOL Prescaler Control Bit: 1 */
#define CLKDIV2_L           (0x0004)  /* A-POOL Prescaler Control Bit: 2 */
#define SAREN_L             (0x0008)  /* A-POOL SAR conversion enable */

/* APOMR Control Bits */
#define CTEN_H              (0x0001)  /* A-POOL Continuous time mode of comparator */
#define AZCMP_H             (0x0002)  /* A-POOL Clocked zero compensated long term comparison */
#define AZSWREQ_H           (0x0004)  /* A-POOL SW request for Auto Zero Phase */
#define SVMINH_H            (0x0008)  /* A-POOL Suppress the generation of an SVM interrupt event. */

#define CLKDIV_0             (0x0000)  /* A-POOL Prescaler Control 0 : /1 */
#define CLKDIV_1             (0x0001)  /* A-POOL Prescaler Control 1 : /2 */
#define CLKDIV_2             (0x0002)  /* A-POOL Prescaler Control 2 : /4 */
#define CLKDIV_3             (0x0003)  /* A-POOL Prescaler Control 3 : /8 */
#define CLKDIV_4             (0x0004)  /* A-POOL Prescaler Control 4 : /16 */
#define CLKDIV_5             (0x0005)  /* A-POOL Prescaler Control 5 : /32 */
#define CLKDIV__1            (0x0000)  /* A-POOL Prescaler Control 0 : /1 */
#define CLKDIV__2            (0x0001)  /* A-POOL Prescaler Control 1 : /2 */
#define CLKDIV__4            (0x0002)  /* A-POOL Prescaler Control 2 : /4 */
#define CLKDIV__8            (0x0003)  /* A-POOL Prescaler Control 3 : /8 */
#define CLKDIV__16           (0x0004)  /* A-POOL Prescaler Control 4 : /16 */
#define CLKDIV__32           (0x0005)  /* A-POOL Prescaler Control 5 : /32 */


/* APIFG Control Bits */
#define EOCIFG               (0x0001)  /* A-POOL End of conversion interrupt flag */
#define CFIFG                (0x0002)  /* A-POOL Comparator falling edge interrupt flag */
#define CRIFG                (0x0004)  /* A-POOL Comparator rising edge interrupt flag */
#define REFOKIFG             (0x0008)  /* A-POOL Reference voltage ready interrupt flag */

/* APIFG Control Bits */
#define EOCIFG_L            (0x0001)  /* A-POOL End of conversion interrupt flag */
#define CFIFG_L             (0x0002)  /* A-POOL Comparator falling edge interrupt flag */
#define CRIFG_L             (0x0004)  /* A-POOL Comparator rising edge interrupt flag */
#define REFOKIFG_L          (0x0008)  /* A-POOL Reference voltage ready interrupt flag */


/* APIFG Control Bits */
#define EOCIE                (0x0001)  /* A-POOL End of conversion interrupt enable */
#define CFIE                 (0x0002)  /* A-POOL Comparator falling edge interrupt enable */
#define CRIE                 (0x0004)  /* A-POOL Comparator rising edge interrupt enable */
#define REFIKIE              (0x0008)  /* A-POOL Reference voltage ready interrupt enable */

/* APIFG Control Bits */
#define EOCIE_L             (0x0001)  /* A-POOL End of conversion interrupt enable */
#define CFIE_L              (0x0002)  /* A-POOL Comparator falling edge interrupt enable */
#define CRIE_L              (0x0004)  /* A-POOL Comparator rising edge interrupt enable */
#define REFIKIE_L           (0x0008)  /* A-POOL Reference voltage ready interrupt enable */


/* APIV Definitions */
#define APIV_NONE           (0x0000)    /* No Interrupt pending */
#define APIV_EOCIF          (0x0002)    /* EOCIFG */
#define APIV_CFIFG          (0x0004)    /* CFIFG */
#define APIV_CRIFG          (0x0006)    /* CRIFG */

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
#define SVMIE               (0x0100)  /* SVM Interrupt Enable */

#define WDTIE_L             (0x0001)  /* WDT Interrupt Enable */
#define OFIE_L              (0x0002)  /* Osc Fault Enable */
//#define Reserved          (0x0004)
#define VMAIE_L             (0x0008)  /* Vacant Memory Interrupt Enable */
#define NMIIE_L             (0x0010)  /* NMI Interrupt Enable */
#define JMBINIE_L           (0x0040)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE_L          (0x0080)  /* JTAG Mail Box output Interrupt Enable */

//#define Reserved          (0x0004)
#define SVMIE_H             (0x0001)  /* SVM Interrupt Enable */

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
#define SVMIFG              (0x0100)  /* SVM Interrupt Flag */

#define WDTIFG_L            (0x0001)  /* WDT Interrupt Flag */
#define OFIFG_L             (0x0002)  /* Osc Fault Flag */
//#define Reserved          (0x0004)
#define VMAIFG_L            (0x0008)  /* Vacant Memory Interrupt Flag */
#define NMIIFG_L            (0x0010)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020)
#define JMBINIFG_L          (0x0040)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG_L         (0x0080)  /* JTAG Mail Box output Interrupt Flag */

//#define Reserved          (0x0004)
//#define Reserved          (0x0020)
#define SVMIFG_H            (0x0001)  /* SVM Interrupt Flag */

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
#define SYSPMMPE            (0x0004)  /* SYS - PMM access protect */
//#define RESERVED          (0x0008)  /* SYS - Reserved */
#define SYSBSLIND           (0x0010)  /* SYS - TCK/RST indication detected */
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
#define SYSPMMPE_L          (0x0004)  /* SYS - PMM access protect */
//#define RESERVED          (0x0008)  /* SYS - Reserved */
#define SYSBSLIND_L         (0x0010)  /* SYS - TCK/RST indication detected */
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
#define SVMOE               (0x0004)  /* SYS - SVM output enable */
#define SVMPO               (0x0008)  /* SYS - SVM based Ports off flag */
#define SVMPD               (0x0010)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define SVMEN               (0x0020)  /* SYS - SVM based port disable */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
#define RAMLCK0             (0x0100)  /* SYS - Write lock enable for configuration RAM */
#define RAMLCK1             (0x0200)  /* SYS - Write lock enable for application’s code RAM */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

/* SYSCNF Control Bits */
//#define RESERVED          (0x0001)  /* SYS - Reserved */
//#define RESERVED          (0x0002)  /* SYS - Reserved */
#define SVMOE_L             (0x0004)  /* SYS - SVM output enable */
#define SVMPO_L             (0x0008)  /* SYS - SVM based Ports off flag */
#define SVMPD_L             (0x0010)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define SVMEN_L             (0x0020)  /* SYS - SVM based port disable */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

/* SYSCNF Control Bits */
//#define RESERVED          (0x0001)  /* SYS - Reserved */
//#define RESERVED          (0x0002)  /* SYS - Reserved */
//#define RESERVED          (0x0040)  /* SYS - Reserved */
//#define RESERVED          (0x0080)  /* SYS - Reserved */
#define RAMLCK0_H           (0x0001)  /* SYS - Write lock enable for configuration RAM */
#define RAMLCK1_H           (0x0002)  /* SYS - Write lock enable for application’s code RAM */
//#define RESERVED          (0x0400)  /* SYS - Reserved */
//#define RESERVED          (0x0800)  /* SYS - Reserved */
//#define RESERVED          (0x1000)  /* SYS - Reserved */
//#define RESERVED          (0x2000)  /* SYS - Reserved */
//#define RESERVED          (0x4000)  /* SYS - Reserved */
//#define RESERVED          (0x8000)  /* SYS - Reserved */

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
* Timer1_A3
************************************************************/
#define __MSP430_HAS_T1A3__            /* Definition to show that Module is available */
#define __MSP430_BASEADDRESS_T1A3__ 0x0380
#define TIMER_A1_BASE __MSP430_BASEADDRESS_T1A3__

#define TA1CTL_               0x0380    /* Timer1_A3 Control */
sfrw(TA1CTL, TA1CTL_);
#define TA1CCTL0_             0x0382    /* Timer1_A3 Capture/Compare Control 0 */
sfrw(TA1CCTL0, TA1CCTL0_);
#define TA1CCTL1_             0x0384    /* Timer1_A3 Capture/Compare Control 1 */
sfrw(TA1CCTL1, TA1CCTL1_);
#define TA1CCTL2_             0x0386    /* Timer1_A3 Capture/Compare Control 2 */
sfrw(TA1CCTL2, TA1CCTL2_);
#define TA1R_                 0x0390    /* Timer1_A3 */
sfrw(TA1R, TA1R_);
#define TA1CCR0_              0x0392    /* Timer1_A3 Capture/Compare 0 */
sfrw(TA1CCR0, TA1CCR0_);
#define TA1CCR1_              0x0394    /* Timer1_A3 Capture/Compare 1 */
sfrw(TA1CCR1, TA1CCR1_);
#define TA1CCR2_              0x0396    /* Timer1_A3 Capture/Compare 2 */
sfrw(TA1CCR2, TA1CCR2_);
#define TA1IV_                0x03AE    /* Timer1_A3 Interrupt Vector Word */
sfrw(TA1IV, TA1IV_);
#define TA1EX0_               0x03A0    /* Timer1_A3 Expansion Register 0 */
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


#define PORT2_VECTOR            ( 6)                     /* 0xFFEA Port 2 */
#define TIMER0_A1_VECTOR        ( 7)                     /* 0xFFEC Timer0_A3 CC1-2, TA1 */
#define TIMER0_A0_VECTOR        ( 8)                     /* 0xFFEE Timer0_A3 CC0 */
#define PORT1_VECTOR            ( 9)                     /* 0xFFF0 Port 1 */
#define APOOL_VECTOR            (10)                     /* 0xFFF2 Analog Pool */
#define WDT_VECTOR              (11)                     /* 0xFFF4 Watchdog Timer */
#define TIMER1_A1_VECTOR        (12)                     /* 0xFFF6 Timer1_A5 CC1-4, TA */
#define TIMER1_A0_VECTOR        (13)                     /* 0xFFF8 Timer1_A5 CC0 */
#define UNMI_VECTOR             (14)                     /* 0xFFFA User Non-maskable */
#define SYSNMI_VECTOR           (15)                     /* 0xFFFC System Non-maskable */
#define RESET_VECTOR            ("reset")                /* 0xFFFE Reset [Highest Priority] */

/************************************************************
* End of Modules
************************************************************/
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* #ifndef __msp430x09x */


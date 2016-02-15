/* Copyright 2016, Juan Cecconi
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef EMC_ARCH_H
#define EMC_ARCH_H
/** \brief External Memory Controller (EMC) Arch header file
 **
 ** This file contains is the EMC driver Arch CortexM4\lpc43xx\lpc4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup EMC EMC Module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * JuCe         Juan Cecconi 
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160214 v0.0.1 JuCe initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/*
 * SDRAM Base Address
 */
#define CIAA_EMC_LPC43XX_SDRAM_BASE		0x28000000

/*
 * Pin settings for EMC pins
 */
#define CIAA_EMC_FAST_IO_CONFIG(func)     (func | SCU_MODE_INACT | SCU_MODE_HIGHSPEEDSLEW_EN | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS)

/*
 * CCU - clock configuration registers
 */
#define CIAA_EMC_LPC43XX_CCU1_CLK_RUN_MSK	(1 << 0)   /* The same bit for every register */

/*
 * CLK_M4_EMCDIV_CFG register
 */
/* Divider selector */
#define CIAA_EMC_LPC43XX_CCU1_CLK_EMCDIV_CFG_DIV2	(1 << 5)

/*
 * CREG6 register
 */
#define CIAA_EMC_LPC43XX_CREG_CREG6_EMC_CLK_SEL_MSK	(1 << 16)   /* EMC_CLK divided clock select */

#define CIAA_EMC_LPC43XX_EMC_CLK_DIV2

/*
 * EMC registers
 */
#define CIAA_EMC_LPC43XX_CTRL_EN_MSK			(1 << 0)
#define CIAA_EMC_LPC43XX_ENDIAN_MODE			EMC_CONFIG_ENDIAN_LITTLE;

/*
 * Dynamic Memory Control register
 */
/* Dynamic memory clock enable (CE) */
#define CIAA_EMC_LPC43XX_DYCTRL_CE_MSK			(1 << 0)
/* Dynamic memory clock control (CS) */
#define CIAA_EMC_LPC43XX_DYCTRL_CS_MSK			(1 << 1)
/* SDRAM initialization (I) */
#define CIAA_EMC_LPC43XX_DYCTRL_I_BITS			7
#define CIAA_EMC_LPC43XX_DYCTRL_I_NORMAL		0
#define CIAA_EMC_LPC43XX_DYCTRL_I_MODE			1
#define CIAA_EMC_LPC43XX_DYCTRL_I_PALL			2	/* precharge all */
#define CIAA_EMC_LPC43XX_DYCTRL_I_NOP			3	/* no operation */

/*
 * Dynamic Memory Read Configuration register:
 *     Read data strategy (RD)
 */
#define CIAA_EMC_LPC43XX_DYRDCFG_RD_BITS		0

/*
 * The SDRAM chip (IS42S16400F) mode register.
 * See IS42S16400F datasheet, page 16.
 */
#define CIAA_EMC_LPC43XX_SDRAM_MODEREG_BL_BITS		0	/* Burst Length */
#define CIAA_EMC_LPC43XX_SDRAM_MODEREG_CAS_BITS		4	/* CAS Latency */

/*
 * See IS42S16400F mode register (IS42S16400F datasheet, page 16).
 * CAS3, Burst Length = 8.
 */
#define CIAA_EMC_LPC43XX_SDRAM_MODEREG_BL			3	/* Burst Length code */
#define CIAA_EMC_LPC43XX_SDRAM_MODEREG_CAS			2	/* CAS Latency */

#define CIAA_EMC_LPC43XX_SDRAM_MODEREG_VALUE \
	((CIAA_EMC_LPC43XX_SDRAM_MODEREG_BL << CIAA_EMC_LPC43XX_SDRAM_MODEREG_BL_BITS) | \
	(CIAA_EMC_LPC43XX_SDRAM_MODEREG_CAS << CIAA_EMC_LPC43XX_SDRAM_MODEREG_CAS_BITS))

/*
 * SDRAM chip-specific options
 * Load mode register
 * The SDRAM mode register is loaded in two steps:
 * 1. Use the DYNAMICCONTROL register to issue a set mode command.
 * 2. When the SDRAM is in the set mode state, issue an SDRAM read from an address
 *    specific to the selected mode and the SDRAM memory organization.
 * The SDRAM read address is ADDRESS = DYCSX_BASE_ADDR + (MODEREG << OFFSET).
 * User Manual: 23.8.5.2.1 Example for setting the SDRAM mode register
 */
#define CIAA_EMC_LPC43XX_MODEREG_ADDR_SHIFT	11

/*
 * Dynamic Memory registers (per chip)
 */
/*
 * Dynamic Memory Configuration register
 */
/* Address mapping */

/* Buffer enable */

/*
 * Dynamic Memory RAS & CAS Delay register
 */
/* RAS latency */
#define CIAA_EMC_LPC43XX_DYRASCAS_RAS_BITS		0
/* CAS latency */
#define CIAA_EMC_LPC43XX_DYRASCAS_CAS_BITS		8

/* Timings for 102 MHz SDRAM clock and IS42S16400F-6TL memory chip */

/* Active to read/write delay (RAS latency) */
#define CIAA_EMC_LPC43XX_SDRAM_RAS				2	/* tRCD = 18ns */
/* CAS latency (CL) */
#define CIAA_EMC_LPC43XX_SDRAM_CAS				2	/* CL = 2 */
/* Command delayed strategy, using EMCCLKDELAY */
#define CIAA_EMC_LPC43XX_SDRAM_RDCFG_RD			1
/* Precharge command period (tRP) */
#define CIAA_EMC_LPC43XX_SDRAM_T_RP				2	/* 18ns */
/* Active to precharge command period (tRAS) */
#define CIAA_EMC_LPC43XX_SDRAM_T_RAS			5	/* 42ns */
/* Self-refresh exit time (tSREX) */
#define CIAA_EMC_LPC43XX_SDRAM_T_SREX			7	/* We set this to the same as tXSR */
/* Last-data-out to active command time (tAPR) */
#define CIAA_EMC_LPC43XX_SDRAM_T_APR			6	/* Not found in the SDRAM datasheet */
/* Data-in to active command (tDAL) */
#define CIAA_EMC_LPC43XX_SDRAM_T_DAL			5	/* 5 cycles */
/* Write recovery time (tWR) */
#define CIAA_EMC_LPC43XX_SDRAM_T_WR				2	/* 2 cycles */
/* Active to active command period (tRC) */
#define CIAA_EMC_LPC43XX_SDRAM_T_RC				7	/* 60ns */
/* Auto-refresh period and auto-refresh to active command period (tRFC) */
#define CIAA_EMC_LPC43XX_SDRAM_T_RFC			7	/* 60ns */
/* Exit self-refresh to active command time (tXSR) */
#define CIAA_EMC_LPC43XX_SDRAM_T_XSR			7	/* 60ns */
/* Active bank A to active bank B latency (tRRD) */
#define CIAA_EMC_LPC43XX_SDRAM_T_RRD			2	/* 12ns */
/* Load mode register to active command time (tMRD) */
#define CIAA_EMC_LPC43XX_SDRAM_T_MRD			2	/* 2 cycles */
/*
 * Refresh timer.
 * Indicates the multiple of 16 CCLKs between SDRAM refresh cycles.
 */
/* 99 = 64000000[64ms] / 4096[rows] / 9.80[ns] / 16; round down */
#define CIAA_EMC_LPC43XX_SDRAM_REFRESH		99
/* Only for initialization */
#define CIAA_EMC_LPC43XX_SDRAM_REFRESH_FAST	1

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Initialize EMC module
 **
 ** This function initialize EMC based on this Arch
 **/
extern bool EMC_Initialize_Arch();

/** \brief Initialize EMC SDRAM
 **
 ** This function initialize EMC-SDRAM based on this Arch
 **/
 extern bool EMC_Initialize_SDRAM_Arch();

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef EMC_ARCH_H */


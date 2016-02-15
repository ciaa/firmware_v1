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

/** \brief External Memory Controller (EMC) Arch source file
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
#include "chip.h"
#include "EMC_Arch.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/** \brief Initialize SDRAM Pins
 **
 ** This function initialize MUX for SDRAM Pins based on this Arch
 **/
bool EMC_Initialize_SDRAM_PinsMux();

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

bool EMC_Initialize_SDRAM_PinsMux()
{
   /*
	* EMC pins used for SDRAM memory
   */

   /* P1.6 = WE# - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 6, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));

   /* P2.10 = A1 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 10, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P2.11 = A2 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 11, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P2.12 = A3 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 12, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P2.13 = A4 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 13, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.0 = A5 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 0, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P1.1 = A6 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 1, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P1.2 = A7 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 2, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P2.8 = A8 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 8, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P2.7 = A9 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 7, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P2.6 = A10 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 6, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P2.2 = A11 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 2, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P2.1 = A12 - SDRAM */
//   Chip_SCU_PinMuxSet(0x2, 1, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P2.0 = BA0 for SDRAM (= A13) - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 0, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P6.8 = BA1 for SDRAM (= A14) - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 8, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC1));

   /* P1.7 = D0 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 7, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.8 = D1 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 8, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.9 = D2 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 9, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.10 = D3 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 10, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.11 = D4 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 11, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.12 = D5 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 12, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.13 = D6 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 13, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P1.14 = D7 - SDRAM */
   Chip_SCU_PinMuxSet(0x1, 14, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P5.4 = D8 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 4, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.5 = D9 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 5, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.6 = D10 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 6, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.7 = D11 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 7, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.0 = D12 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 0, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.1 = D13 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 1, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.2 = D14 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 2, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));
   /* P5.3 = D15 - SDRAM */
   Chip_SCU_PinMuxSet(0x5, 3, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC2));

   /*
	 * Configuration for EMC pins used only for SDRAM
	 */
   /*
	 * To use 16-bit wide and 32-bit wide SDRAM interfaces, select
	 * the EMC_CLK function and enable the input buffer (EZI = 1)
	 * in all four SFSCLKn registers in the SCU.
	 */
   /* Imaginary P-0x18.0 = CLK (CLK0) - SDRAM */
   Chip_SCU_PinMuxSet(0x18, 0, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC0));
   /* Imaginary P-0x18.1 = CLK1 - SDRAM */
   Chip_SCU_PinMuxSet(0x18, 1, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC0));
   /* Imaginary P-0x18.2 = CLK2 - SDRAM */
   Chip_SCU_PinMuxSet(0x18, 2, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC0));
   /* Imaginary P-0x18.3 = CLK3 - SDRAM */
   Chip_SCU_PinMuxSet(0x18, 3, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC0));

   /* P6.11 = CKE - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 11, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P6.9 = CS# (nDYCS0) - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 9, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));

   /* P6.5 = RAS# - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 5, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P6.4 = CAS# - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 4, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P6.12 = DQM0 - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 12, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   /* P6.10 = DQM1 - SDRAM */
   Chip_SCU_PinMuxSet(0x6, 10, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));

   /* P2.9 = A0 - SDRAM */
   Chip_SCU_PinMuxSet(0x2, 9, CIAA_EMC_FAST_IO_CONFIG(SCU_MODE_FUNC3));
   
   return TRUE;
}

/*==================[external functions definition]==========================*/

extern bool EMC_Initialize_Arch()
{
   return EMC_Initialize_SDRAM_Arch();
}

extern bool EMC_Initialize_SDRAM_Arch()
{
	uint32_t tmp_read;
	volatile uint32_t i;
   /*
   * Set SDRAM clock output delay to ~3.5ns (0x7777),
   * the SDRAM chip does not work otherwise.
   */
   LPC_SCU->EMCDELAYCLK = 0x7777;

   /*
   * Little-endian mode
   */
	LPC_EMC->CONFIG = CIAA_EMC_LPC43XX_ENDIAN_MODE;

	/* EMC Enable       */
	LPC_EMC->CONTROL = CIAA_EMC_LPC43XX_CTRL_EN_MSK;

	/*
	* Configure MCU pins
	*/
	EMC_Initialize_SDRAM_PinsMux();

	/* Select and enable EMC branch clock */
#ifdef CIAA_EMC_LPC43XX_EMC_CLK_DIV2
	/*
	 * EMC_CLK = CLK_M4 / 2
	 */
   LPC_CCU1->CLKCCU[CLK_MX_EMC_DIV].CFG =
			CIAA_EMC_LPC43XX_CCU1_CLK_RUN_MSK | CIAA_EMC_LPC43XX_CCU1_CLK_EMCDIV_CFG_DIV2;
   LPC_CREG->CREG6 |= CIAA_EMC_LPC43XX_CREG_CREG6_EMC_CLK_SEL_MSK;
   LPC_CCU1->CLKCCU[CLK_MX_EMC].CFG |= CIAA_EMC_LPC43XX_CCU1_CLK_RUN_MSK;
#else
	/* #error EMC clock set to M4_CLK/1 is not supported */
   LPC_CCU1->CLKCCU[CLK_MX_EMC_DIV].CFG = CIAA_EMC_LPC43XX_CCU1_CLK_RUN_MSK;
   LPC_CCU1->CLKCCU[CLK_MX_EMC].CFG |= CIAA_EMC_LPC43XX_CCU1_CLK_RUN_MSK;
#endif

   /*
	* Address mapping
	*/
   LPC_EMC->DYNAMICCONFIG0 = EMC_DYN_CONFIG_4Mx16_4BANKS_12ROWS_8COLS; /* IS42S16400F SDRAM (8MB): 16-bit, 4 banks, 12 row bits, 8 column bits. */

   /*
	* Configure DRAM timing
	*/
   LPC_EMC->DYNAMICRASCAS0 =
		   (CIAA_EMC_LPC43XX_SDRAM_RAS << CIAA_EMC_LPC43XX_DYRASCAS_RAS_BITS) |
		   (CIAA_EMC_LPC43XX_SDRAM_CAS << CIAA_EMC_LPC43XX_DYRASCAS_CAS_BITS);
	LPC_EMC->DYNAMICREADCONFIG = (CIAA_EMC_LPC43XX_SDRAM_RDCFG_RD << CIAA_EMC_LPC43XX_DYRDCFG_RD_BITS);

	LPC_EMC->DYNAMICRP  = CIAA_EMC_LPC43XX_SDRAM_T_RP - 1;
	LPC_EMC->DYNAMICRAS = CIAA_EMC_LPC43XX_SDRAM_T_RAS - 1;
	LPC_EMC->DYNAMICSREX = CIAA_EMC_LPC43XX_SDRAM_T_SREX - 1;
	LPC_EMC->DYNAMICAPR  = CIAA_EMC_LPC43XX_SDRAM_T_APR - 1;
	LPC_EMC->DYNAMICDAL  = CIAA_EMC_LPC43XX_SDRAM_T_DAL;
	LPC_EMC->DYNAMICWR   = CIAA_EMC_LPC43XX_SDRAM_T_WR - 1;
	LPC_EMC->DYNAMICRC   = CIAA_EMC_LPC43XX_SDRAM_T_RC - 1;
	LPC_EMC->DYNAMICRFC  = CIAA_EMC_LPC43XX_SDRAM_T_RFC - 1;
	LPC_EMC->DYNAMICXSR  = CIAA_EMC_LPC43XX_SDRAM_T_XSR - 1;
	LPC_EMC->DYNAMICRRD  = CIAA_EMC_LPC43XX_SDRAM_T_RRD - 1;
	LPC_EMC->DYNAMICMRD  = CIAA_EMC_LPC43XX_SDRAM_T_MRD - 1;
	for (i = 0; i < 1000; i++)
	{
		/* wait 100us */
	}

	/*
	 * Issue SDRAM NOP (no operation) command
	 */
	LPC_EMC->DYNAMICCONTROL =
			CIAA_EMC_LPC43XX_DYCTRL_CE_MSK | CIAA_EMC_LPC43XX_DYCTRL_CS_MSK |
			(CIAA_EMC_LPC43XX_DYCTRL_I_NOP << CIAA_EMC_LPC43XX_DYCTRL_I_BITS);
	for (i = 0; i < 1000; i++)
	{
		/* wait 100us */
	}

	/*
	 * Pre-charge all with fast refresh
	 */
	LPC_EMC->DYNAMICCONTROL =
			CIAA_EMC_LPC43XX_DYCTRL_CE_MSK | CIAA_EMC_LPC43XX_DYCTRL_CS_MSK |
			(CIAA_EMC_LPC43XX_DYCTRL_I_PALL << CIAA_EMC_LPC43XX_DYCTRL_I_BITS);
	LPC_EMC->DYNAMICREFRESH = CIAA_EMC_LPC43XX_SDRAM_REFRESH_FAST;
	for (i = 0; i < 1000; i++)
	{
		/* wait 100us */
	}

	/*
	 * Set refresh period
	 */
	LPC_EMC->DYNAMICREFRESH = CIAA_EMC_LPC43XX_SDRAM_REFRESH;

	/*
	 * Load mode register
	 * The SDRAM mode register is loaded in two steps:
	 * 1. Use the DYNAMICCONTROL register to issue a set mode command.
	 * 2. When the SDRAM is in the set mode state, issue an SDRAM read from an address
	 *    specific to the selected mode and the SDRAM memory organization.
	 * The SDRAM read address is ADDRESS = DYCSX_BASE_ADDR + (MODEREG << OFFSET).
	 * User Manual: 23.8.5.2.1 Example for setting the SDRAM mode register
	 */
	LPC_EMC->DYNAMICCONTROL =
			CIAA_EMC_LPC43XX_DYCTRL_CE_MSK | CIAA_EMC_LPC43XX_DYCTRL_CS_MSK |
			(CIAA_EMC_LPC43XX_DYCTRL_I_MODE << CIAA_EMC_LPC43XX_DYCTRL_I_BITS);

	tmp_read = *(volatile uint32_t *)(CIAA_EMC_LPC43XX_SDRAM_BASE |
		(CIAA_EMC_LPC43XX_SDRAM_MODEREG_VALUE << CIAA_EMC_LPC43XX_MODEREG_ADDR_SHIFT));

	/*
	 * Normal mode
	 */
	LPC_EMC->DYNAMICCONTROL = (CIAA_EMC_LPC43XX_DYCTRL_I_NORMAL << CIAA_EMC_LPC43XX_DYCTRL_I_BITS);

	/*
	 * Enable DRAM buffer
	 */
	LPC_EMC->DYNAMICCONFIG0 |= EMC_CONFIG_BUFFER_ENABLE;

	return;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


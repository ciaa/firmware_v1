/* Copyright 2016, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2016, Marcos Ziegler <ziegler.marcos.b@gmail.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2016, ACSE & CADIEEL & Marcos Ziegler <ziegler.marcos.b@gmail.com
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief SSP descriptor definition
 **
 ** Implementation of SSP attributes and functions to be used in SSP driver and SD card driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup SSP SSP internals
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaDriverSSP_Internal.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** \brief Send one byte via MOSI and receive one byte via MISO
 **
 ** \param[in] pSSP SSP0/1 handle
 ** \param[in] tx_data byte to write to FIFO
 ** \return    byte to receive from FIFO. 4-bit to 16-bit frame depending from config.
 **/
static uint16_t Chip_SSP_RWByte(ciaaDriverSSP_SSPDevType *SSP_dev, uint8_t tx_data);

/** \brief Set the CS pin of selected SSP low/high
 **
 ** \param[in] pSSP SSP0/1 handle
 ** \param[in] tx_data byte to write to FIFO
 **/
static void Chip_SSP_set_CS(ciaaDriverSSP_SSPDevType *SSP_dev, uint8_t value);

/** \brief Set new data bit rate for the SSP peripheral
 **
 ** \param[in] pSSP SSP0/1 handle
 ** \param[in] clk_rate new data bit rate
 ** \remarks 
 **/
static void Chip_SSP_set_ClockRate(ciaaDriverSSP_SSPDevType *SSP_dev, uint32_t clk_rate);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
/* Declare extern ciaaDriverSSP_SSP0, extern ciaaDriverSSP_SSP1 in header file */
/** \brief SSP 0 SPI 8 bit mode */
ciaaDriverSSP_SSPDevType ciaaDriverSSP_SSP0 =
{
   LPC_SSP0,
   {SSP_FRAMEFORMAT_SPI, SSP_BITS_8, SSP_CLOCK_MODE0},
   SSP_MODE_MASTER,
   Chip_SSP_RWByte,
   0, 0,
   Chip_SSP_set_CS,
   Chip_SSP_set_ClockRate
}
/** \brief SSP 1 SPI 8 bit mode */
ciaaDriverSSP_SSPDevType ciaaDriverSSP_SSP1 =
{
   LPC_SSP1,
   {SSP_FRAMEFORMAT_SPI, SSP_BITS_8, SSP_CLOCK_MODE0},
   SSP_MODE_MASTER,
   Chip_SSP_RWByte,
   1, 5,
   Chip_SSP_set_CS,
   Chip_SSP_set_ClockRate
}

/*==================[internal functions definition]==========================*/

static uint16_t Chip_SSP_RWByte(ciaaDriverSSP_SSPDevType *SSP_dev, uint8_t tx_data)
{
   /* write the byte to Tx FIFO */
   SSP_dev->RWByteChip_SSP_SendFrame(SSP_dev->pSSP, tx_data);
   /* Wait for completion of transfer */
   while(Chip_SSP_GetStatus(SSP_dev->pSSP, SSP_STAT_BSY) == SET);
   /* Return the received value in Rx FIFO */
   return Chip_SSP_ReceiveFrame(SSP_dev->pSSP);
}

static void Chip_SSP_set_CS(ciaaDriverSSP_SSPDevType *SSP_dev, uint8_t value)
{
   Chip_GPIO_SetPinState(LPC_GPIO_PORT,SSP_dev->CS_port, SSP_dev->CS_pin, value);
}

static void Chip_SSP_set_ClockRate(ciaaDriverSSP_SSPDevType *SSP_dev, uint32_t clk_rate)
{
   Chip_SSP_SetBitRate(SSP_dev->pSSP, clk_rate);
}
/*==================[external functions definition]==========================*/

int ciaaDriverSSP_channelInit(ciaaDriverSSP_SSPDevType *SSP_dev)
{
   int ret;
   LPC_SSP_T *pSSP;
   SSP_ConfigFormat ssp_format;

   pSSP = SSP_dev->pSSP;
   /* Enable SSPI block */
   Chip_SSP_Enable(pSSP)
   /* Configure SSP pins: SCK, MISO, MOSI */
   /* SSP1_MISO: P0_0
    * SSP1_MOSI: P0_1
    * SSP1_SCK: P1_19
    * SSP1_SSEL: P1_5. SSEL NOT USED. USE GPIO MODE TO DRIVE CS MANUALLY
    * CS: P1_5
    */
   if(pSSP == LPC_SSP1)
   {
      /* Configure SSP pins: SCK, MISO, MOSI */
      /* SSP1_MISO: P0_0
       * SSP1_MOSI: P0_1
       * SSP1_SCK: P1_19
       * SSP1_SSEL: P1_5. SSEL NOT USED. USE GPIO MODE TO DRIVE CS MANUALLY
       * CS: P1_5
       */
      /* Chip_SCU_PinMuxSet(1, 5, (SCU_PINIO_FAST | SCU_MODE_FUNC5)); //P1.5 => SSEL1. Dedicated SSEL not used */
      Chip_SCU_PinMuxSet(1, 19, (SCU_PINIO_FAST | SCU_MODE_FUNC1));  /* P1.19 => SCK1 */

      Chip_SCU_PinMuxSet(0, 1, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1)); /* P0.1 => MOSI1 */
      Chip_SCU_PinMuxSet(0, 0, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1)); /* P0.0 => MISO1 */

      Chip_SCU_PinMuxSet(SSP_dev->port, SSP_dev->pin, (SCU_MODE_INACT | SCU_MODE_FUNC0));  /* P1.5 => SSEL1 */
      Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, SSP_dev->port, SSP_dev->pin);
      Chip_GPIO_SetPinState(LPC_GPIO_PORT, SSP_dev->port, SSP_dev->pin, (bool) true); /* CS high */
   }
   else if(pSSP == LPC_SSP0)
   {
      /* Not supported */
      return -1;
   }
   else
   {
      /* wrong pointer */
      return -1;
   }
    
   /* 8bit, SPI frame format, CPOL=0, CPHA=0, SCR=0
    * data width: 8bit
    * frame format: SPI
    * CPOL: low level
    * CPHA: first edge
    * SCR = 0
    */
   Chip_SSP_SetFormat(LPC_SSP, SSP_dev->ssp_format.bits,
                        SSP_dev->ssp_format.frameFormat, SSP_dev->ssp_format.clockMode);

   /* Enable SSP0 as a master
    * Normal mode
    * Master
    * slave output disabled
    */
   Chip_SSP_Set_Mode(pSSP, SSP_MODE_MASTER)

   /* Configure SSP1 clock rate to 400kHz (100MHz/250) */
   Chip_SSP_SetBitRate(pSSP, 400000);

   return 0;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

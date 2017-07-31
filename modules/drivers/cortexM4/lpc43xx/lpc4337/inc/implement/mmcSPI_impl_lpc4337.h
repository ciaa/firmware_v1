/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, Your Name <youremail@domain.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2014, ACSE & CADIEEL & Your Name <youremail@domain.com
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

#ifndef IMPL_MMCSPI_H
#define IMPL_MMCSPI_H

#ifndef MMCSPI_H
#error Do #include "mmcSPI.h" right before doing #include "implement/mmcSPI.h"
#endif

/** \brief Short description of this file
 **
 ** Long description of this file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module
 ** @{ */

/*==================[inclusions]=============================================*/
#include "implement/device_impl_lpc4337.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief SD Card SPI Command Codes */
#define MMC_SPI_CMD0   0x40  /* GO_IDLE_STATE: Set Card to idle state */
#define MMC_SPI_CMD1   0x41  /* SEND_OP_COND: Sends capacity support information */
#define MMC_SPI_CMD6   0x46  /* SWITCH_FUNC: Checks switchable function */
#define MMC_SPI_CMD8   0x48  /* SEND_IF_COND: Sends SD Memory Card interface condition */
#define MMC_SPI_CMD9   0x49  /* SEND_CSD: Get CSD register from card */
#define MMC_SPI_CMD10  0x4a  /* SEND_CID: Get CID register from card */
#define MMC_SPI_CMD12  0x4c  /* STOP_TRANSMISSION: Forces the card to stop transmission */
#define MMC_SPI_CMD13  0x4d  /* SEND_STATUS: Asks card to send its status register */
#define MMC_SPI_CMD16  0x50  /* SET_BLOCKLEN: Sets a block length (in bytes) */
#define MMC_SPI_CMD17  0x51  /* READ_SINGLE_BLOCK: Reads a block of the selected size */
#define MMC_SPI_CMD18  0x52  /* READ_MULTIPLE_BLOCK: Continuously transfers blocks from card to host */
#define MMC_SPI_CMD20  0x54  /* CMD_WRITEBLOCK: Write block to memory (MMC) */
#define MMC_SPI_CMD24  0x58  /* WRITE_BLOCK: Writes a block of the selected size */
#define MMC_SPI_CMD25  0x59  /* WRITE_MULTIPLE_BLOCK: Continuously writes blocks of data */
#define MMC_SPI_CMD27  0x5b  /* PROGRAM_CSD: Set programmable bits of the CSD */
#define MMC_SPI_CMD28  0x5c  /* SET_WRITE_PROT: Sets the write protection bit of group */
#define MMC_SPI_CMD29  0x5d  /* CLR_WRITE_PROT: Clears the write protection bit of group */
#define MMC_SPI_CMD30  0x5e  /* SEND_WRITE_PROT: Asks card to send state of write protection bits */
#define MMC_SPI_CMD32  0x60  /* ERASE_WR_BLK_START_ADDR: Sets address of first block to erase */
#define MMC_SPI_CMD33  0x61  /* ERASE_WR_BLK_END_ADDR: Sets address of last block to erase */
#define MMC_SPI_CMD38  0x66  /* ERASE: Erases all previously selected write blocks */
#define MMC_SPI_CMD42  0x6a  /* LOCK_UNLOCK: Used to Set/Reset the Password or lock/unlock card */
#define MMC_SPI_CMD55  0x77  /* APP_CMD: Tells card that the next command is an application specific command */
#define MMC_SPI_CMD56  0x78  /* GEN_CMD: Used transfer a block to or get block from card */
#define MMC_SPI_CMD58  0x7a  /* READ_OCR :Reads the OCR register of a card */
#define MMC_SPI_CMD59  0x7b  /* CRC_ON_OFF: Turns the CRC option on or off */
#define MMC_SPI_ACMD13 0x4d  /* SD_STATUS: Send the SD Status */
#define MMC_SPI_ACMD22 0x56  /* SEND_NUM_WR_BLOCKS: Send number of the errorfree blocks */
#define MMC_SPI_ACMD23 0x57  /* SET_WR_BLK_ERASE_COUNT: Set number blocks to erase before writing */
#define MMC_SPI_ACMD41 0x69  /* SD_SEND_OP_COND: Sends host capacity support information */
#define MMC_SPI_ACMD42 0x6a  /* SET_CLR_CARD_DETECT: Connect/disconnect pull-up resistor on CS */
#define MMC_SPI_ACMD51 0x73  /* SEND_SCR: Reads the SD Configuration Register (SCR) */

/* Every data block written to the card will be acknowledged by a data response token.
 * It is one byte long and has the following format:
 * bit 7,6,5: X; bit 4: 0; bits 3,2,1: Status; bit 0: 1.
 * The meaning of the status bits is defined as follows:
 * '010': Data accepted
 * '101': Data rejected due to a CRC error
 * '110': Data Rejected due to a write error
 * In case of a write error ('110'), the host may send CMD13 (SEND_STATUS)
 * in order to get the cause of the write problem.
 */
#define MMC_SPI_DATARESPONSE_MASK		0x1F       /* Dont care about bits 7,6,5 */
#define MMC_SPI_DATARESPONSE_ACCEPTED	0b00000101 /* 0bXXX0 010 1 */
#define MMC_SPI_DATARESPONSE_ERRORCRC	0b00001011 /* 0bXXX0 101 1 */
#define MMC_SPI_DATARESPONSE_ERRORWRITE	0b00001101 /* 0bXXX0 110 1 */

/* Fix data block size to 512 bytes for all card types */
#define MMC_SECTORSIZE 512


/*==================[typedef]================================================*/

/** \brief  */
typedef enum
{
   MMC_R1,
   MMC_R1B,
   MMC_R2,
   MMC_R3,
   MMC_R4,
   MMC_R5,
   MMC_R7
} mmc_cmdResp_t;

/** \brief  */
typedef struct
{

} mmcspi_constructor_params_t;

/** \brief  */
typedef struct
{
  uint8_t  index;
  mmc_cmdResp_t response;
  uint8_t  checksum;
} mmc_SPICMDInfo;


ClassMembers( MmcSPI, Device )

   mmc_status_t status;
   uint8_t  ocr[4];
   uint8_t  cid[16];
   uint8_t  csd[16];

   mmc_cardType_t type;
   uint32_t nsectors;
   uint32_t block_size;
   /* TODO: Block buffer for rx and another for tx? Single buffer for both? */
   /* Cant read and write at the same time */
   uint8_t block_buf[512];

   /* BlockDevice */
   uint32_t position;

EndOfClassMembers;

/*==================[external data declaration]==============================*/
/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* IMPL_MMCSPI_H */

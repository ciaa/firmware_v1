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

#ifndef MMCSPI_H
#define MMCSPI_H
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
#include "ciaaPOSIX_stdint.h"
#include "device_x86.h"
#include "ooc.h"
#include "blockDevice_x86.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/*==================[typedef]================================================*/
/** \brief Describe SPI peripheral current state */
typedef enum
{
   MMC_STATUS_UNINIT,
   MMC_STATUS_READY,
   MMC_STATUS_BUSY
} mmc_status_t;

/** \brief Card type */
typedef enum
{
   MMC_CARDTYPE_UNKNOWN,
   MMC_CARDTYPE_MMC,
   MMC_CARDTYPE_SDV1,
   MMC_CARDTYPE_SDV2_LC,
   MMC_CARDTYPE_SDV2_HC,
   MMC_CARDTYPE_BLOCK
} mmc_cardType_t;

/** \brief Card command list */
typedef enum
{
   MMC_CMD_GO_IDLE_STATE,
   MMC_CMD_SEND_OP_COND,
   MMC_CMD_APP_SEND_OP_COND,
   MMC_CMD_SEND_IF_COND,
   MMC_CMD_SEND_CSD,
   MMC_CMD_SEND_CID,
   MMC_CMD_STOP_TRANSMISSION,
   MMC_CMD_SET_BLOCKLEN,
   MMC_CMD_READ_SINGLE_BLOCK,
   MMC_CMD_READ_MULTIPLE_BLOCK,
   MMC_CMD_SET_BLOCK_COUNT,
   MMC_CMD_SET_WR_BLOCK_ERASE_COUNT,
   MMC_CMD_WRITE_BLOCK,
   MMC_CMD_WRITE_MULTIPLE_BLOCK,
   MMC_CMD_APP_CMD,
   MMC_CMD_READ_OCR
} mmc_cmd_t;

/*==================[external data declaration]==============================*/
/** \brief MmcSPI class declaration. MmcSPI inherits from Device.
 **
 **/
DeclareClass(MmcSPI, Device);

/*==================[external functions declaration]=========================*/
/** \brief Wrapper to class constructor with parameters. Use this function for object instantiation.
 **
 ** \return    The newly created Object
 **/
MmcSPI mmcSPI_new(void);

/** \brief SD Card initialization sequence
 **
 **
 ** \param[in] self SD card handle
 ** \return    0 success, else error
 **/
int mmcSPI_init(MmcSPI self);

/** \brief get SD card status
 **
 **
 ** \param[in] self SD card handle
 ** \return    Ready: Ready to operate, Busy: Must wait for new operation.
 **/
mmc_status_t mmcSPI_getStatus(MmcSPI self);

/** \brief Check if SD card present in slot
 **
 ** \param[in] MmcSPI SD card handle
 ** \return    0 if not present, not 0 otherwise
 **/
int mmcSPI_isInserted(MmcSPI self);

/** \brief Send command to SD card
 **
 **
 ** \param[in] MmcSPI SD card handle
 ** \param[in] cmd_index command to send. See ciaaDriverSD_SPICMDType
 ** \param[in] arg 4 byte argument
 ** \param[in] resp command response
 ** \return    byte to receive from FIFO
 **/
int mmcSPI_txCommand(MmcSPI self, mmc_cmd_t cmd_index, uint32_t arg, uint8_t *resp);

/** \brief Read single block
 **
 ** \param[in] MmcSPI SD card handler
 ** \param[out] readBlock buffer in which to read block
 ** \param[in] sector Block offset
 ** \return    -1 for timeout and other errors, 0 if success
 **/
int mmcSPI_singleBlockRead(MmcSPI self, uint8_t *readBlock, uint32_t sector);

/** \brief Write single block
 **
 ** \param[in] MmcSPI SD card handler
 ** \param[in] writeBlock buffer from which to write block
 ** \param[in] sector Block offset
 ** \return    -1 for timeout and other errors, 0 if success
 **/
int mmcSPI_singleBlockWrite(MmcSPI self, const uint8_t *writeBlock, uint32_t sector);

/** \brief get card size in blocks
 **
 ** \param[in] MmcSPI SD card handler
 ** \return    number of blocks in card
 **/
uint32_t mmcSPI_getSize(MmcSPI self);


/** \brief erase multiple adjacent blocks
 **
 ** \param[in] MmcSPI SD card handler
 ** \param[in] start block from where to start erasing
 ** \param[in] end last block to erase
 ** \return    -1 for timeout and other errors, 0 if success
 **/
int mmcSPI_blockErase(MmcSPI self, uint32_t start, uint32_t end);

//int MmcSPI_multipleBlockRead(MmcSPI self, uint8_t *readBuffer, uint32_t sector, uint32_t readCount);
//int MmcSPI_multipleBlockWrite(MmcSPI self, uint8_t *writeBuffer, uint32_t sector, uint32_t writeCount);
/* Virtual function definitions */
Virtuals( MmcSPI, Device )
   Interface(BlockDevice);
EndOfVirtuals;

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif  /* #ifndef MMCSPI_H */

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

/** \brief CIAA SD Card Posix Driver
 **
 ** SD card driver for Posix. SPI interface
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup SDCard SD card drivers
 ** @{ */

/*==================[inclusions]=============================================*/

#include "ciaaDriverSSP_Internal.h"
#include "ciaaDriverSD.h"
#include "ciaaDriverSD_Internal.h"
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_ioctl_block.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

typedef struct
{
   ciaaDriverSSP_SSPDevType *ssp_dev;
   uint8_t  state;
   SD_CSD csd;
   SD_CID cid;

   uint8_t type;
   uint32_t capacity;
   uint32_t block_size;
   uint32_t position;                     /** <= Current position */
   /* TODO: Block buffer for rx and another for tx? Single buffer for both? */
   /* Cant read and write at the same time */
   uint8_t block_buf[256];
} ciaaDriverSD_SDCardType;

typedef struct
{
  uint8_t  index;
  uint8_t  response;
  uint8_t  checksum;
} ciaaDriverSD_SPICMDInfo;

typedef enum
{
   CIAADRVSD_SPI_CMD_GO_IDLE_STAT,
   CIAADRVSD_SPI_CMD_SEND_OP_COND,
   CIAADRVSD_SPI_CMD_APP_SEND_OP_COND,
   CIAADRVSD_SPI_CMD_SEND_IF_COND,
   CIAADRVSD_SPI_CMD_SEND_CSD,
   CIAADRVSD_SPI_CMD_SEND_CID,
   CIAADRVSD_SPI_CMD_STOP_TRANSMISSION,
   CIAADRVSD_SPI_CMD_SET_BLOCKLEN,
   CIAADRVSD_SPI_CMD_READ_SINGLE_BLOCK,
   CIAADRVSD_SPI_CMD_READ_MULTIPLE_BLOCK,
   CIAADRVSD_SPI_CMD_SET_BLOCK_COUNT,
   CIAADRVSD_SPI_CMD_SET_WR_BLOCK_ERASE_COUNT,
   CIAADRVSD_SPI_CMD_WRITE_BLOCK,
   CIAADRVSD_SPI_CMD_WRITE_MULTIPLE_BLOCK,
   CIAADRVSD_SPI_CMD_APP_CMD,
   CIAADRVSD_SPI_CMD_READ_OCR
} ciaaDriverSD_SPICMDType;

typedef enum
{
   CIAADRVSD_CARDTYPE_UNKNOWN,
   CIAADRVSD_CARDTYPE_MMC,
   CIAADRVSD_CARDTYPE_SDV1,
   CIAADRVSD_SPI_CARDTYPE_SDV2_LC,
   CIAADRVSD_SPI_CARDTYPE_SDV2_HC,
   CIAADRVSD_CARDTYPE_BLOCK
} ciaaDriverSD_CardType;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** \brief Send one byte via MOSI and receive one byte via MISO
 **
 ** Precondition: SD card selected
 **
 ** \param[in] SD_card SD card handle
 ** \param[in] cmd_index command to send. See ciaaDriverSD_SPICMDType for 
 ** \param[in] arg 4 byte argument
 ** \param[in] resp
 ** \return    byte to receive from FIFO
 **/
static int ciaaDriverSD_txCommand(ciaaDriverSD_SDCardType *SD_card, ciaaDriverSD_SPICMDType cmd_index, uint32_t arg, uint8_t *resp);

/** \brief Wait until the card is ready
 **
 ** \param[in] SD_card SD card handler
 ** \return    -1 for timeout and other errors, 0 if success
 **/
static int ciaaDriverSD_WaitCardReady(ciaaDriverSD_SDCardType *SD_card);

/** \brief Read single block with specified length
 **
 ** \param[in] SD_card SD card handler
 ** \param[out] buf Read user data. Must contain at least 'len' bytes
 ** \param[in] len user data block length
 ** \return    -1 for timeout and other errors, 0 if success
 **/
static int ciaaDriverSD_rxDataBlock(ciaaDriverSD_SDCardType *SD_card, uint8_t *buf, uint32_t len);

/** \brief Write single block with specified length
 **
 ** \param[in] SD_card SD card handler
 ** \param[out] buf user data buffer. Must contain at least 'len' bytes
 ** \param[in] len user data block length
 ** \return    -1 for timeout and other errors, 0 if success
 **/
static int ciaaDriverSD_txDataBlock(ciaaDriverSD_SDCardType *SD_card, uint8_t *buf, uint32_t len);

/** \brief Read single block
 **
 ** \param[in] SD_card SD card handler
 ** \param[in] sector Block offset
 ** \param[out] buf Read user data. Must contain at least 'len' bytes
 ** \return    -1 for timeout and other errors, 0 if success
 **/
static int ciaaDriverSD_ReadSector(ciaaDriverSD_SDCardType *SD_card, uint32_t sector, uint8_t *buf);

/** \brief Initialize sd card related hardware
 **
 **/
static void ciaaDriverSD_hwInit(void);

/** \brief Initialize card slot
 **
 ** \param[in] SD_card SD card handler
 ** \param[in] SD_card SD card handler
 ** \return    -1 for timeout and other errors, 0 if success
 **/
static int ciaaDriverSD_slotInit(ciaaDriverSD_SDCardType *SD_card);

/*==================[internal data definition]===============================*/
/** \brief SD card slot 0 */
ciaaDriverSD_SDCardType ciaaDriverSD_SDcard0;

/** \brief SPI Command set */
static const ciaaDriverSD_SPICMDInfo * const ciaaDriverSD_SPICMDList =
{
   {CIAADRVSD_SPI_CMD0,   CIAADRVSD_SPI_R1, 0x95}, /* GO_IDLE_STAT */
   {CIAADRVSD_SPI_CMD1,   CIAADRVSD_SPI_R1, 0xff}, /* SEND_OP_COND */
   {CIAADRVSD_SPI_ACMD41,   CIAADRVSD_SPI_R1, 0xff}, /* APP_SEND_OP_COND */
   {CIAADRVSD_SPI_CMD8,   CIAADRVSD_SPI_R7, 0x87}, /* SEND_IF_COND */
   {CIAADRVSD_SPI_CMD9,   CIAADRVSD_SPI_R1, 0xff}, /* SEND_CSD */
   {CIAADRVSD_SPI_CMD10,   CIAADRVSD_SPI_R1, 0xff}, /* SEND_CID */
   {CIAADRVSD_SPI_CMD12,   CIAADRVSD_SPI_R1B, 0xff}, /* STOP_TRANSMISSION */
   {CIAADRVSD_SPI_CMD16,   CIAADRVSD_SPI_R1, 0xff}, /* SET_BLOCKLEN */
   {CIAADRVSD_SPI_CMD17,   CIAADRVSD_SPI_R1, 0xff}, /* READ_SINGLE_BLOCK */
   {CIAADRVSD_SPI_CMD18,   CIAADRVSD_SPI_R1, 0xff}, /* READ_MULTIPLE_BLOCK */
   {CIAADRVSD_SPI_CMD23,   CIAADRVSD_SPI_R1, 0xff}, /* SET_BLOCK_COUNT */
   {CIAADRVSD_SPI_ACMD23,   CIAADRVSD_SPI_R1, 0xff}, /* SET_WR_BLOCK_ERASE_COUNT */
   {CIAADRVSD_SPI_CMD24,   CIAADRVSD_SPI_R1, 0xff}, /* WRITE_BLOCK */
   {CIAADRVSD_SPI_CMD25,   CIAADRVSD_SPI_R1, 0xff}, /* WRITE_MULTIPLE_BLOCK */
   {CIAADRVSD_SPI_CMD55,   CIAADRVSD_SPI_R1, 0xff}, /* APP_CMD */
   {CIAADRVSD_SPI_CMD58,   CIAADRVSD_SPI_R3, 0xff}, /* READ_OCR */
};

static ciaaDevices_deviceType ciaaDriverSD_device0 = {
   "sd/0",                       /** <= driver name */
   ciaaDriverSD_open,            /** <= open function */
   ciaaDriverSD_close,           /** <= close function */
   ciaaDriverSD_read,            /** <= read function */
   ciaaDriverSD_write,           /** <= write function */
   ciaaDriverSD_ioctl,           /** <= ioctl function */
   ciaaDriverSD_lseek,           /** <= lseek function */
   NULL,                         /** <= upper layer. Should be the VFS node of the device */
   (void *)&ciaaDriverSD_SDcard0,  /** <= Should be the struct that contains the SD card info */
   (void *)&ciaaDriverSSP_SSP1     /** <= Should be the SPI port structure that controls this card slot */
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static int ciaaDriverSD_txCommand(ciaaDriverSD_SDCardType *SD_card, ciaaDriverSD_SPICMDType cmd_index, uint32_t arg, uint8_t *resp)
{
   ciaaDriverSSP_SSPDevType *SSP_dev = SD_card->ssp_dev;
   ciaaDriverSD_SPICMDInfo *cmd_info;
   uint32_t i;
   int ret;
   uint8_t tmp_8;

   /* Get the command information for the requested command */
   cmd_info = &(ciaaDriverSD_SPICMDList[cmd_index]);

   /* Wait until the card is ready to read */
   ret = ciaaDriverSD_WaitCardReady(SD_card);
   if(ret < 0)
   {
      return -1;
   }

   /* Send 6-byte command with CRC. */
   SSP_dev->RWByte(SSP_dev, (uint8_t)cmd_info->index);
   SSP_dev->RWByte(SSP_dev, (uint8_t)(arg >> 24)); /* MSB first */
   SSP_dev->RWByte(SSP_dev, (uint8_t)(arg >> 16));
   SSP_dev->RWByte(SSP_dev, (uint8_t)(arg >> 8));
   SSP_dev->RWByte(SSP_dev, (uint8_t)(arg));
   /* CRC only matters for the first command.
    * Otherwise, the CRC is ignored for SPI mode unless
    * CRC checking enabled
    */
   SSP_dev->RWByte(SSP_dev, (uint8_t)cmd_info->checksum);
   SSP_dev->RWByte(SSP_dev, );
  
   /* The command response time (Ncr) is 0 to 8 bytes for SDC, 
   1 to 8 bytes for MMC. */
   for (i = 0; i < 8; i++)
   {
       tmp_8 = SSP_dev->RWByte(SSP_dev, 0xFF);
       if ((tmp_8 & 0x80) == 0) break;   /* check for a valid response (zero start bit) */
   }
   if (i == 8)  return -1; /* command response time out error */
   if(resp) resp[0] = tmp_8; /* Response token is first byte of response */
   switch (cmd_info->response)
   {
      /* This response token is identical to the R1 format with the optional addition of the busy signal
       * The busy signal token can be any number of bytes. A zero value indicates card is busy. A
       * non-zero value indicates the card is ready for the next command.
       */

      case CIAADRVSD_SPI_R1B:
      {
         /* TODO: Add timeout error? */
         do
         {
            tmp_8 = SSP_dev->RWByte(SSP_dev, 0xFF);
         }
         while (tmp_8 == 0x00); /* zero value indicates card is busy */
         break;
      }

      /* This response token is sent by the card after every command with the exception of SEND_STATUS
       * commands. It is one byte long, and the MSB is always set to zero. The other bits are error
       * indications, an error being signaled by a 1.
       */

      case CIAADRVSD_SPI_R1:
         /* resp[0] contains the token. Only 1 byte long, nothing to do here */
         break;

      /* This response token is two bytes long and sent as a response to the SEND_STATUS command
       * The first byte is identical to the response R1.
       */

      case CIAADRVSD_SPI_R2:
         /* First byte already in resp[0], get the second byte */
         if(resp) resp[1] = SSP_dev->RWByte(SSP_dev, 0xFF);
         break;

      /* This response token is sent by the card when a READ_OCR command is received.
       * The response length is 5 bytes. The structure of the first (MSB) byte
       * is identical to response type R1. The other four bytes contain the OCR register
       */

      case CIAADRVSD_SPI_R3:
         if(resp)
         {
            resp[1] = SSP_dev->RWByte(SSP_dev, 0xFF);
            resp[2] = SSP_dev->RWByte(SSP_dev, 0xFF);
            resp[3] = SSP_dev->RWByte(SSP_dev, 0xFF);
            resp[4] = SSP_dev->RWByte(SSP_dev, 0xFF);
         }
         break;

      /* Those response formats are reserved for I/O mode
       */
      case CIAADRVSD_SPI_R4:
      case CIAADRVSD_SPI_R5:

      /* This response token is sent by the card when a SEND_IF_COND command (CMD8) is received.
       * The response length is 5 bytes. The structure of the first (MSB) byte is identical
       * to response type R1. The other four bytes contain the card operating voltage information
       * and echo back of check pattern in argument and are specified by the same definition as R7
       * response in SD mode.
       */
         break;

      case CIAADRVSD_SPI_R7:
         if(resp)
         {
            resp[1] = SSP_dev->RWByte(SSP_dev, 0xFF);
            resp[2] = SSP_dev->RWByte(SSP_dev, 0xFF);
            resp[3] = SSP_dev->RWByte(SSP_dev, 0xFF);
            resp[4] = SSP_dev->RWByte(SSP_dev, 0xFF);
         }
         break;
      default:
         break;
   }
   return 0;
}

static int ciaaDriverSD_WaitCardReady(ciaaDriverSD_SDCardType *SD_card)
{
   spi_dev *pSSP_dev = SD_card->spi_dev;
   uint32_t Timer;

   Timer = 400000; /* TODO: Control timer from Systick */
   pSSP_dev->RWByte(pSSP_dev, 0xFF); /* Read a byte (Force enable DO output) */
   while(Timer--)
   {
      if (pSSP_dev->RWByte(pSSP_dev, 0xFF) == 0xFF)
      {
         return 0;
      }
   }

   return -1;    
}

static int ciaaDriverSD_rxDataBlock(ciaaDriverSD_SDCardType *SD_card, uint8_t *buf, uint32_t len)
{
    uint8_t aux_8;
    uint32_t i, timer;

   /* Data is being received via data tokens. All data bytes are transmitted MSB first.
    * Data blocks are 4 to 515 bytes long.
    * First byte: Start token
    * Bytes 2-513 (depends on the data block length): User data
    * Last two bytes: 16 bit CRC
    *
    */
   
   /* Wait for start block token (0xFE) or data error token (0x0X) */
   timer = 400000;   /* TODO: Implement timeout timer with Systick */
   do
   {							
      aux_8 = pSSP_dev->RWByte(pSSP_dev, 0xFF);
   }
   while(timer-- && aux_8 != 0xFE && (aux_8 & 0xF0));
   if(!(aux_8 & 0xFE))
   {
      return -1; /* Data error. See 4 LSB to check error code */
   }
   if(aux_8 != 0xFE)
   {
      return -1; /* Timeout error */
   }

    /* Read user data. len indicates data block length */
   for (i = 0; i < len; i++)
   {
        buf[i] = pSSP_dev->RWByte(pSSP_dev, 0xFF); /* MSB first */
   }

    /* Last 2 bytes CRC will be discarded. */
    pSSP_dev->RWByte(pSSP_dev, 0xFF);
    pSSP_dev->RWByte(pSSP_dev, 0xFF);

    return 0;
}

static int ciaaDriverSD_txDataBlock(ciaaDriverSD_SDCardType *SD_card, uint8_t *buf, uint32_t len)
{

   /* The SPI mode supports single block and multiple block write commands. Upon reception of a valid write
    * command (CMD24 or CMD25 in the SD Memory Card protocol), the card will respond with a response
    * token and will wait for a data block to be sent from the host. CRC suffix, block length and start address
    * restrictions are (with the exception of the CSD parameter WRITE_BL_PARTIAL controlling the partial
    * block write option and WRITE_BL_LEN) identical to the read operation.
    */

   /* Every data block has a prefix of ’Start Block’ token (one byte).
    * After a data block has been received, the card will respond with a data-response token. If the data block
    * has been received without errors, it will be programmed. As long as the card is busy programming, a
    * continuous stream of busy tokens will be sent to the host (effectively holding the DataOut line low).
    * Once the programming operation is completed, the host should check the results of the programming
    * using the SEND_STATUS command (CMD13). Some errors (e.g. address out of range, write protect
    * violation etc.) are detected during programming only. The only validation check performed on the data
    * block, and communicated to the host via the data-response token, is the CRC and general Write Error 
    */

   /* While the card is busy, resetting the CS signal will not terminate the programming process. The card will
    * release the DataOut line (tri-state) and continue with programming. If the card is reselected before the
    * programming is finished, the DataOut line will be forced back to low and all commands will be rejected.
    * Resetting a card (using CMD0 for SD memory card) will terminate any pending or active programming
    * operation. This may destroy the data formats on the card. It is in the responsibility of the host to prevent
    * this for occurring. 
    */

   /* Start the block transfer:
    * 1. 0xff (sync)
    * 2. 0xfe or 0xfc (start of block token)
    * 3. Followed by the block of data and 2 byte CRC
    */
   /* The internal write process is initiated a byte after the data response.
    * Eight SCLK clocks are required to initiate internal write operation.
    */
   pSSP_dev->RWByte(pSSP_dev, 0xFF); /* sync */
   pSSP_dev->RWByte(pSSP_dev, 0xFE); /* Send start block token (0xFE) */

   /* Transmit the block to the MMC/SD card */

   /* Write user data. len indicates data block length */
   for (i = 0; i < len; i++)
   {
        pSSP_dev->RWByte(pSSP_dev, buf[i]); /* MSB first */
   }

   /* Send 2 bytes dummy CRC */
   /* Add the bogus CRC.  By default, the SPI interface is initialized in
    * non-protected mode.  However, we still have to send bogus CRC values
    */
   pSSP_dev->RWByte(pSSP_dev, 0xFF);
   pSSP_dev->RWByte(pSSP_dev, 0xFF);

   /* Read data response to check if the data block has been accepted. */
   /* Now get the data response */
   /* The card responds a Data Response immediately following the data packet from the host.
    * The Data Response trails a busy flag and host controller must wait until the card goes ready.
    */
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
   aux_8 = pSSP_dev->RWByte(pSSP_dev, 0xFF) & 0x1F;
   switch(aux_8)
   {
      case CIAADRVSD_SPI_DATARESPONSE_ACCEPTED:
         break;
      case CIAADRVSD_SPI_DATARESPONSE_ERRORCRC:
      case CIAADRVSD_SPI_DATARESPONSE_ERRORWRITE:
      default:
         return -1;
         break;
   }

   /* Wait while card is busy */
   timer = 400000;   /* TODO: Implement timeout timer with Systick */
   do
   {							
      aux_8 = pSSP_dev->RWByte(pSSP_dev, 0xFF);
   }
   while(timer-- && aux_8 != 0xFF);
   if(aux_8 != 0xFF)
   {
      return -1; /* Timeout error */
   }

   return 0;
}

static int ciaaDriverSD_ReadSector(ciaaDriverSD_SDCardType *SD_card, uint32_t sector, uint8_t *buf)
{
   int ret;
   uint8_t cmd_resp;
   /* The unit of -data address- in argument is byte for Standard Capacity SD Memory Card
    * and block (512 bytes) for High Capacity SD Memory Card.
    */
   switch(SD_card->type)
   {
      case CIAADRVSD_CARDTYPE_UNKNOWN:
         return -1; /* Should never get here if unknown card */
         break;
      case CIAADRVSD_CARDTYPE_MMC:
      case CIAADRVSD_CARDTYPE_SDV1:
      case CIAADRVSD_CARDTYPE_SDV2_SC:
         offset = sector; /* block address */
         break;
      case CIAADRVSD_CARDTYPE_SDV2_HC:
         break;
      default:
         offset = sector << 9; /* byte address */
         break;
   }
   /* Send command to read requested block */
   ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_READ_SINGLE_BLOCK, offset, &cmd_resp);
   if(ret < 0)
   {
      return -1;
   }
   /* Read data block into buffer */
   ret = ciaaDriverSD_rxDataBlock(SD_card, buf, SD_card->blocklen);
   if(ret < 0)
   {
      return -1;
   }

   return 0;
}

static void ciaaDriverSD_hwInit(void)
{
   int ret;
   ciaaDriverSD_SDCardType *SD_card = ciaaDriverSD_device0.layer;
   ciaaDriverSSP_SSPType *ssp_dev = ciaaDriverSD_device0.loLayer;

   ret = ciaaDriverSD_slotInit(SD_card, ssp_dev);
   if(ret >= 0)
   {
   }
}

static int ciaaDriverSD_slotInit(ciaaDriverSD_SDCardType *SD_card, ciaaDriverSSP_SSPDevType *pSSP_dev)
{
   int ret;
   uint32_t i;

   SD_card->spi_dev = pSSP_dev;
   /* Set card type to unknown */
   SD_card->type = CIAADRVSD_SPI_CARDTYPE_UNKNOWN;

   /* Before reset, Send at least 74 clocks at low frequency
   (between 100kHz and 400kHz) with CS high and DI (MISO) high. */
   pSSP_dev->set_CS(pSSP_dev, 1);
   pSSP_dev->set_ClockRate(pSSP_dev, SPI_CLOCKRATE_LOW);
   for (i = 0; i < 20; i++) pSSP_dev->RWByte(pSSP_dev, 0xFF);

   /* Send CMD0 with CS low to enter SPI mode and reset the card.
   The card will enter SPI mode if CS is low during the reception of CMD0. 
   Since the CMD0 (and CMD8) must be sent as a native command, the CRC field
   must have a valid value. */
   pSSP_dev->set_CS(pSSP_dev, 0);
   ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_GO_IDLE_STATE, 0, &cmd_resp); /* CMD0 */
   if( (ret >= 0) && (cmd_resp[0] == 0x01)) /* Must be in idle state */
   {  /* Reset command CMD0 successfull. Card in idle state */
      /* Now the card enters IDLE state. */
      /* After the card enters idle state, send CMD8 with argument 0x1AA and correct CRC.
       * The argument 0x1AA means that the card is SDC version 2 and it can work in voltage range 2.7V to 3.6V.
       * The SEND_IF_COND command (CMD8) checks if the given statement is true.
       * If CMD8 is rejected with illegal command error (0x05), the card is SDC version 1 or MMC version 3.
       * If CMD8 is accepted, R7 is returned.
       */

      /* Check the card type, needs around 1000ms */
      /* CMD8. 01: 2.7-3.6V; AA: Recommended check pattern */    
      ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_SEND_IF_COND, 0x01AA, &cmd_resp); /* CMD8 */
      if(ret > 0)
      {
      /* It is mandatory to issue CMD8 prior to first ACMD41 for initialization of High Capacity SD Card.
       * Receipt of CMD8 makes the card realize that the host supports Version 2.0.
       * ACMD41 is application specific command, therefore APP_CMD (CMD55) shall always precede ACMD41.
       *
       */
      /* If the card can operate on the supplied voltage, the response echoes back the supply voltage
       * and the ckeck pattern that were set in the command argument.
       */
      /* If card cannot operate under supplied voltage, card doesnt respond and return to Idle State.
       */
      /* Check for SDHC Version 2.x.  CMD 8 is reserved on SD version 1.0 and MMC. */
      if(!(cmd_resp[0] & 0x04))
      {  /* CMD8 is legal. Card version 2.00 or later */

         if(cmd_resp[3]!= 0x01 || cmd_resp[4]!=0xAA)
         {  /* Non compatible voltage range or check pattern is not correct */
            /* Set card type to unknown */
            SD_card->type = CIAADRVSD_SPI_CARDTYPE_UNKNOWN;
         }
         /* The card is SD V2 and can work at voltage range of 2.7 to 3.6V */
      
         /* READ_OCR (CMD58) is designed to provide SD Memory Card hosts with a mechanism to identify
          * cards that do not match the VDD range desired by the host. If the host does not accept voltage range, it
          * shall not proceed further initialization sequence. The levels in the OCR register shall be defined accordingly.
          */
         ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_READ_OCR, 0, &cmd_resp); /* CMD58 */
         if(ret < 0)
         {
            return -1;
         }
         if(!((cmd_resp[2] & 0xFF) && (cmd_resp[3] & 0x80))) /* Check if card supports host voltage range */
         {
            /* TODO: Not mandatory */
         }
         /* The Card Capacity status bit is valid after
          * the card power up procedure is completed and the card power up status bit is set to 1
          * Dont check it now
          */
 
         do
         {
            /* TODO: Host supports indexing only to 4GB. Set HCS bit? */
            /* If host supports high capacity, HCS is set to 1 */
            /* Send CMD1 with HCS bit set */
            ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_APP_CMD, 0, &cmd_resp); /* CMD55 */
            if(ret < 0)
            {
               return -1;
            }
            if((cmd_resp[0] == 0x01) || (cmd_resp[0] == 0x00))
            {
               /* ACMD41 */
               ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_APP_SEND_OP_COND, (uint32_t)(1 << 30), &cmd_resp);
               if(ret < 0)
               {
                  return -1;
               }
            }
         }
         while (Timer1 && cmd_resp[0] == 0x01); /* Repeat while Card R1 responds idle state until timeout */

         if(cmd_resp[0] == 0x00) /* The value 0x00 in ACMD41 means successful */
         {
            /* After initialization is completed, the host should get CCS information in the response of CMD58. CCS is
             * valid when the card accepted CMD8 and after the completion of initialization. CCS=1 means that the
             * card is a High Capacity SD Memory Card. CCS=0 means that the card is a Standard Capacity SD
             * Memory Card
             */
            ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_READ_OCR, 0, &cmd_resp); /* CMD58 */
            if(ret < 0)
            {
               return -1;
            }
            if(cmd_resp[0] & 0x00) /* Check if card returns busy or ready */
            {
               if(cmd_resp[1] & 0x40)
               {
                  /* Card is high capacity SD */
                  SD_card->type = CIAADRVSD_SPI_CARDTYPE_SDV2_HC;
               }
               else
               {
                  /* Card is standard capacity SD */
                  SD_card->type = CIAADRVSD_SPI_CARDTYPE_SDV2_LC;
               }
            }
         }
         else /* ACMD41 response was an error. Card with non compatible voltage or timeout error */
         {
            /* Set card type to unknown */
            SD_card->type = CIAADRVSD_SPI_CARDTYPE_UNKNOWN;
         }
         }
      }  /*/CMD8 legal*/
      else
      {   /* CMD8 is illegal. It's Ver1.x SD card or MMC card */
          /* Check if it is SD card */
          if(SD_SendCommand (APP_CMD, 0, NULL, 0) & R1_ILLEGAL_CMD)
          {   
              CardType = CARDTYPE_MMC; 
              while(Timer1 && SD_SendCommand (SEND_OP_COND, 0, NULL, 0));
          }  
          else 
          {   
              CardType = CARDTYPE_SDV1; 
              while (Timer1 && SD_SendACommand (SD_SEND_OP_COND, 0, NULL, 0));
          }
   
         ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_READ_OCR, 0, &cmd_resp); /* CMD58 */
         if(ret < 0)
         {
            return -1;
         }
         if(!(cmd_resp[0] & 0x04))
         {  /* CMD58 is legal */
            do
            {
               ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_APP_CMD, 0, &cmd_resp); /* CMD55 */
               if(ret < 0)
               {
                  return -1;
               }
               if((cmd_resp[0] == 0x01) || (cmd_resp[0] == 0x00))
               {
                  /* ACMD41 */
                  ret = ciaaDriverSD_txCommand(SD_card, CIAADRVSD_SPI_CMD_APP_SEND_OP_COND, 0, &cmd_resp);
                  if(ret < 0)
                  {
                     return -1;
                  }
                  if(cmd_resp[0] == 0x00)
                  {
                     /* Card ready. Stop loop and cointinue initialization */
                     break;
                  }
               }
            }
            while (Timer1--); /* Repeat while Card R1 responds idle state until timeout */
            if(Timer1)
            {
               /* Card ready. Ver1.X Standad Capacity */
            }
            else
            {
               /* Card not ready. Timeout error */
            }
         }  /* /CMD58 legal */
         else
         {  /* CMD58 is illegal. Not SD Memory Card */

         }  /* /CMD58 illegal */
      }  /* /CMD8 illegal*/
   }  /* /CMD0 legal */
   else
   {  /* CMD0 failed, cant start initialization */
      
   }
   
      /* For SDHC or SDXC, block length is fixed to 512 bytes, for others,
      the block length is set to 512 manually. */
      if (CardType == CARDTYPE_MMC || CardType == CARDTYPE_SDV1 || CardType == CARDTYPE_SDV2_SC )
      {
          if (SD_SendCommand(SET_BLOCKLEN, SECTOR_SIZE, NULL, 0) != R1_NO_ERROR)
              CardType = CARDTYPE_UNKNOWN;
      }
   
   init_end:              
      SD_DeSelect();

      if (CardType == CARDTYPE_UNKNOWN)
      {
          return SD_FALSE;
      }
      else     /* Init OK. use high speed during data transaction stage. */
      {
          SPI_ConfigClockRate(SPI_CLOCKRATE_HIGH);
          return SD_TRUE;
      }

}

/*==================[external functions definition]==========================*/

extern ciaaDevices_deviceType * ciaaDriverSD_open(char const * path, ciaaDevices_deviceType * device, uint8_t const oflag)
{
   ciaaDriverSD_SDCardType *SD_card;

   SD_card->position = 0;

   return device;
}

extern int32_t ciaaDriverSD_close(ciaaDevices_deviceType const * const device)
{
   return 0;
}

extern int32_t ciaaDriverSD_ioctl(ciaaDevices_deviceType const * const device, int32_t const request, void * param)
{
   int32_t ret = -1;

   if(device == ciaaDriverSD_SDcard0)
   {
      switch(request)
      {
         case ciaaPOSIX_IOCTL_COMM0:
            break;

         case ciaaPOSIX_IOCTL_COMM1:
            break;

         case ciaaPOSIX_IOCTL_COMM2:
            break;

         case ciaaPOSIX_IOCTL_COMM3:
            break;

         case ciaaPOSIX_IOCTL_COMM4:
            break;
      }
   }
   return ret;
}

extern ssize_t ciaaDriverSD_read(ciaaDevices_deviceType const * const device, uint8_t* buffer, size_t const size)
{
   ssize_t ret = -1;
   uint32_t read_size = size;
   ciaaDriverSD_SDCardType *SD_card;
   uint8_t block_buf = SD_card->block_buf;

   if(device == ciaaDriverSD_SDcard0)
   {
      SD_card = device->layer;
      /* FIXME: If read beond limit: Submit error without reading anything?
       * Read until limit and submit error? Read until limit and dont submit error?
       */
      if(((SD_card->position + read_size) << 9) < SD_card->nblocks) /* Dont read beyond limits */
      {
         /* Read single block */
         while(read_size)
         {
            offset_inblock = SD_card->position % SD_card->blocklen;
            partial_copy_size = SD_card->blocklen - offset_inblock;
            if(partial_copy_size > read_size)
               partial_copy_size = read_size;
            sector = SD_card->position << 9;
            ret = ciaaDriverSD_ReadSector(SD_card, sector, block_buf);
            if(ret < 0)
            {
               return -1;
            }
            ciaaPOSIX_memcpy(buffer, block_buf + offset_inblock, partial_copy_size);
            buffer += partial_copy_size;
            read_size -= partial_copy_size;
            SD_card->position += partial_copy_size;
         }
      }
   }

   return size;
}

extern ssize_t ciaaDriverSD_write(ciaaDevices_deviceType const * const device, uint8_t const * const buffer, size_t const size)
{
   ssize_t ret = -1;
   uint32_t write_size = size;
   ciaaDriverSD_SDCardType *SD_card;
   uint8_t block_buf = SD_card->block_buf;

   if(device == ciaaDriverSD_SDcard0)
   {
      SD_card = device->layer;
      /* FIXME: If read beond limit: Submit error without reading anything?
       * Read until limit and submit error? Read until limit and dont submit error?
       */
      if(((SD_card->position + write_size) << 9) < SD_card->nblocks) /* Dont read beyond limits */
      {
         /* Write single block */
         while(write_size)
         {
            /* Write doesnt start in the beginning of the block */
            offset_inblock = SD_card->position % SD_card->blocklen;
            /* How much bytes from actual position to the end of block? */
            partial_copy_size = SD_card->blocklen - offset_inblock;
            if(partial_copy_size > read_size)
               partial_copy_size = read_size;
            sector = SD_card->position << 9;
            /* Check if an entire block must be written or not.
             * If a block must be partially modified, then the block must be read first to modify it,
             * and then write the modified block back to disk.
             * Shall the block be fully written, then there is no need to read it first. Write
             * the full block directly.
             */
            if(partial_copy_size < SD_card->blocklen)
            {
               /* Read block to modify. */
               ret = ciaaDriverSD_ReadSector(SD_card, sector, block_buf);
               if(ret < 0)
               {
                  return -1;
               }
            }
            ciaaPOSIX_memcpy(block_buf + offset_inblock, buffer, partial_copy_size);
            ret = ciaaDriverSD_WriteSector(SD_card, sector, block_buf);
            if(ret < 0)
            {
               return -1;
            }
            buffer += partial_copy_size;
            write_size -= partial_copy_size;
            SD_card->position += partial_copy_size;
         }
      }
   }

   return size;
}

extern off_t ciaaDriverSD_lseek(ciaaDevices_deviceType const * const device, off_t const offset, uint8_t const whence)
{
   off_t destination = -1;
   ciaaDriverSD_SDCardType *SD_card = device->layer;

   if(SD_card == &ciaaDriverSD_SDCard)
   {
      switch(whence)
      {
         case SEEK_END:
            destination = (SD_card->nsectors << 9) + offset;
            break;
         case SEEK_CUR:
            destination = SD_card->position + offset;
            break;
         default:
            destination = offset;
            break;
      }

      if ((destination >= 0) && (destination < (SD_card->nsectors << 9)))
      {
         SD_card->position = destination;
      }
   }
   return destination;
}

void ciaaDriverSD_init(void)
{
   /* Initialize a SSP channel and attach it to a SD card slot */
   /* init hardware */
   ciaaDriverSD_hwInit();
   ciaaBlockDevices_addDriver(&ciaaDriverSD_device0);
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

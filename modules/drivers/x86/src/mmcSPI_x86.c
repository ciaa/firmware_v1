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

/** \brief MmcSPI class implementation file
 **
 ** Class of SD card driven by SPI
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module
 ** @{ */

/*==================[inclusions]=============================================*/
#include "device_x86.h"
#include "implement/device_impl_x86.h"
#include "mmcSPI_x86.h"
#include "implement/mmcSPI_impl_x86.h"
#include "ciaaPOSIX_string.h"
#include <stdio.h>

/*==================[macros and definitions]=================================*/
/** Define filename to simulate card data */
#define MMC_SPI_X86_FILENAME "MMCSPI.BIN"
#define MMC_SPI_BLOCKSIZE 512
#define MMC_SPI_BLOCKCOUNT 2048

/*==================[internal data definition]===============================*/

/** \brief SPI Command set */
static const mmc_SPICMDInfo mmc_SPICMDList[] =
{
   {MMC_SPI_CMD0,   MMC_R1, 0x95}, /* GO_IDLE_STAT */
   {MMC_SPI_CMD1,   MMC_R1, 0xff}, /* SEND_OP_COND */
   {MMC_SPI_ACMD41,   MMC_R1, 0xff}, /* APP_SEND_OP_COND */
   {MMC_SPI_CMD8,   MMC_R7, 0x87}, /* SEND_IF_COND */
   {MMC_SPI_CMD9,   MMC_R1, 0xff}, /* SEND_CSD */
   {MMC_SPI_CMD10,   MMC_R1, 0xff}, /* SEND_CID */
   {MMC_SPI_CMD12,   MMC_R1B, 0xff}, /* STOP_TRANSMISSION */
   {MMC_SPI_CMD16,   MMC_R1, 0xff}, /* SET_BLOCKLEN */
   {MMC_SPI_CMD17,   MMC_R1, 0xff}, /* READ_SINGLE_BLOCK */
   {MMC_SPI_CMD18,   MMC_R1, 0xff}, /* READ_MULTIPLE_BLOCK */
   {MMC_SPI_ACMD23,   MMC_R1, 0xff}, /* SET_WR_BLOCK_ERASE_COUNT */
   {MMC_SPI_CMD24,   MMC_R1, 0xff}, /* WRITE_BLOCK */
   {MMC_SPI_CMD25,   MMC_R1, 0xff}, /* WRITE_MULTIPLE_BLOCK */
   {MMC_SPI_CMD55,   MMC_R1, 0xff}, /* APP_CMD */
   {MMC_SPI_CMD58,   MMC_R3, 0xff}, /* READ_OCR */
};

char mmcspi_x86_filename[] = "MMC_SPI_X86_FILENAME";

/*==================[internal functions declaration]=========================*/
/* BlockDevice interface implementation */
static ssize_t mmcSPI_read(MmcSPI self, uint8_t * const buf, size_t const nbyte);
static ssize_t mmcSPI_write(MmcSPI self, uint8_t const * const buf, size_t const nbyte);
static ssize_t mmcSPI_lseek(MmcSPI self, off_t const offset, uint8_t const whence);
static int mmcSPI_ioctl(MmcSPI self, int32_t request, void* param);
static int mmcSPI_connect(MmcSPI self);
static int mmcSPI_disconnect(MmcSPI self);
static int mmcSPI_getState(MmcSPI self, blockDevState_t *state);
static int mmcSPI_getInfo(MmcSPI self, blockDevInfo_t *info);

/*==================[internal data definition]===============================*/
/*==================[external data definition]===============================*/
/** \brief Allocating the class description table and the vtable
 */
InterfaceRegister(MmcSPI)
{
	AddInterface(MmcSPI, BlockDevice)
};

AllocateClassWithInterface(MmcSPI, Device);

/** \brief Class virtual function prototypes
 */

/*==================[internal functions definition]==========================*/

/** \brief Class initializing
 */

static void MmcSPI_initialize( Class this )
{
   /* Init vtable and override/assign virtual functions */
   MmcSPIVtable vtab = & MmcSPIVtableInstance;

   vtab->BlockDevice.read = (ssize_t (*)(Object, uint8_t * const, size_t const))mmcSPI_read;
   vtab->BlockDevice.write = (ssize_t (*)(Object, uint8_t const * const buf, size_t const))mmcSPI_write;
   vtab->BlockDevice.lseek = (ssize_t (*)(Object, off_t const, uint8_t const))mmcSPI_lseek;
   vtab->BlockDevice.ioctl = (int (*)(Object, int32_t, void*))mmcSPI_ioctl;
   vtab->BlockDevice.connect = (int (*)(Object))mmcSPI_connect;
   vtab->BlockDevice.disconnect = (int (*)(Object))mmcSPI_disconnect;
   vtab->BlockDevice.getState = (int (*)(Object, blockDevState_t *))mmcSPI_getState;
   vtab->BlockDevice.getInfo = (int (*)(Object, blockDevInfo_t *))mmcSPI_getInfo;
   /* Allocate global resources here */
}

/** \brief Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static void MmcSPI_finalize( Class this )
{
   /* Release global resources! */
}
#endif

/** \brief Constructor */
static void MmcSPI_constructor( MmcSPI self, const void *params )
{
   mmcspi_constructor_params_t *mmcspi_params;
   assert( ooc_isInitialized( MmcSPI ) );
   chain_constructor( MmcSPI, self, NULL );
   mmcspi_params = (mmcspi_constructor_params_t *)params;	
   if(mmcspi_params != NULL)
   {
      self->type =  MMC_CARDTYPE_UNKNOWN;
      self->status = MMC_STATUS_UNINIT;
      self->filename = mmcspi_x86_filename;
   }
   else
   {
      /* TODO */
   }
}

/** \brief Destructor */
static void MmcSPI_destructor( MmcSPI self, MmcSPIVtable vtab )
{
   /* Nothing allocated, no resources to free. Do nothing */
}

/** \brief Copy constructor */
static int MmcSPI_copy( MmcSPI self, const MmcSPI from )
{
   /* Prevent object duplication */
   return OOC_NO_COPY;
}

/*==================[external functions definition]==========================*/
/** \brief Class member functions */


MmcSPI mmcSPI_new(void)
{
   mmcspi_constructor_params_t mmcspi_params;
   return (MmcSPI) ooc_new(MmcSPI, (void *)&mmcspi_params);
}

mmc_status_t mmcSPI_getStatus(MmcSPI self)
{
   return self->status;
}

int mmcSPI_init(MmcSPI self)
{
   int ret = -1;

   self->block_size = MMC_SPI_BLOCKSIZE;
   self->nsectors = MMC_SPI_BLOCKCOUNT;
   self->storage = fopen(self->filename,"r+b");
   if (self->storage == NULL)
   {
      perror("SD emulation file not exists: ");
      self->storage = fopen(self->filename,"w+b");

      if (self->storage != NULL)
      {  
         if(mmcSPI_blockErase(self, 0, self->nsectors - 1) == 0) //FIXME
         {
            ciaaPOSIX_printf("mmcSPI_init(): mmcSPI_blockErase() successful\n");
            rewind(self->storage);
            self->type =  MMC_CARDTYPE_MMC;
            self->status = MMC_STATUS_READY;
            ret = 0;
         }
         else
         {
            ciaaPOSIX_printf("mmcSPI_init(): mmcSPI_blockErase failed\n");
         }
      }
      else
      {
         perror("Error creating SD emulation file: ");
      }
   }
   else
   {
      ret = 0;
   }
   return ret;
}

int mmcSPI_isInserted(MmcSPI self)
{
   /* Always inserted */
   return 1;
}

int mmcSPI_txCommand(MmcSPI self, mmc_cmd_t cmd_index, uint32_t arg, uint8_t *resp)
{
   return 0;
}

int mmcSPI_singleBlockRead(MmcSPI self, uint8_t *readBlock, uint32_t sector)
{
   int ret = -1;
   uint32_t offset;

   //ciaaPOSIX_printf("mmcSPI_singleBlockRead(): Enter\n");
   if(NULL != self->storage && sector < self->nsectors)
   {
      offset = sector * self->block_size;
      ciaaPOSIX_printf("mmcSPI_singleBlockRead(): Read block in offset %d\n", offset);
      rewind(self->storage);
      if (0 == fseek(self->storage, offset, SEEK_SET))
      {
         //ciaaPOSIX_printf("mmcSPI_singleBlockRead(): Before fread. Read %d bytes\n", self->block_size);
         if(fread(readBlock, 1, self->block_size, self->storage) == self->block_size)
         {
            //ciaaPOSIX_printf("mmcSPI_singleBlockRead(): Success\n");
            ret = 0;
         }
         else
         {

         }
      }
      else
      {

      }
   }
   fflush(self->storage);
   return ret;
}

int mmcSPI_singleBlockWrite(MmcSPI self, const uint8_t *writeBlock, uint32_t sector)
{
   int ret = -1;
   uint32_t offset;

   if(NULL != self->storage && sector < self->nsectors)
   {
      offset = sector * self->block_size;
      rewind(self->storage);
      if (0 == fseek(self->storage, offset, SEEK_SET))
      {
         ciaaPOSIX_printf("mmcSPI_singleBlockWrite(): offset: %d\n", offset);
         if(fwrite(writeBlock, 1, self->block_size, self->storage) == self->block_size)
         {
            ret = 0;
         }
         else
         {

         }
      }
      else
      {

      }
   }
   rewind(self->storage);
   fflush(self->storage);
   return ret;
}

int mmcSPI_blockErase(MmcSPI self, uint32_t start, uint32_t end)
{
   int ret = -1;
   uint32_t i;
   static const uint8_t zeroBlock[MMC_SPI_BLOCKSIZE] = {0};

   if(NULL != self->storage && start <= end && end < self->nsectors)
   {
      for(i = start; i <= end; i++)
      {
         if(fwrite(zeroBlock, self->block_size, 1, self->storage) != 1)
         {
            break;
         }
      }
      if(i == end+1)
      {
         ret = 0;
      }
   }
   fflush(self->storage);
   rewind(self->storage);
   return ret;
}

uint32_t mmcSPI_getSize(MmcSPI self)
{
   return self->nsectors;
}

/* BlockDevice interface implementation */
static ssize_t mmcSPI_read(MmcSPI self, uint8_t * const buf, size_t const nbyte)
{
   ssize_t ret = -1;
   size_t bytes_left, bytes_read, i, sector, position, bytes_offset;
   assert(ooc_isInstanceOf(self, MmcSPI));

   i=0; bytes_left = nbyte; sector = self->position / self->block_size; position = self->position;
   while(bytes_left)
   {
      bytes_offset = position % self->block_size;
      bytes_read = (bytes_left > (self->block_size - bytes_offset)) ? (self->block_size - bytes_offset) : bytes_left;
      if(mmcSPI_singleBlockRead(self, self->block_buf, sector) == 0)
      {
         ciaaPOSIX_memcpy(buf + i, self->block_buf + bytes_offset, bytes_read);
         bytes_left -= bytes_read;
         i += bytes_read;
         position += bytes_read;
         sector++;
      }
      else
      {
         break;
      }
   }
   if(0 == bytes_left)
   {
      ret = i;
      self->position += i;
   }
   else
   {

   }

   return ret;
}

static ssize_t mmcSPI_write(MmcSPI self, uint8_t const * const buf, size_t const nbyte)
{
   ssize_t ret = -1;
   size_t bytes_left, bytes_write, i, sector, position, bytes_offset;
   assert(ooc_isInstanceOf(self, MmcSPI));

   i=0; bytes_left = nbyte; sector = self->position / self->block_size; position = self->position;
   while(bytes_left)
   {
      bytes_offset = position % self->block_size;
      bytes_write = bytes_left > (self->block_size - bytes_offset) ? (self->block_size - bytes_offset) : bytes_left;
      //ciaaPOSIX_printf("mmcSPI_write(): bytes_left: %d bytes_write: %d sector: %d\n",
      //                  bytes_left, bytes_write, sector);
      if(mmcSPI_singleBlockRead(self, self->block_buf, sector) == 0)
      {
         //ciaaPOSIX_printf("mmcSPI_write(): Block readed, now copy new data\n");
         ciaaPOSIX_memcpy(self->block_buf + bytes_offset, buf + i, bytes_write);
         //ciaaPOSIX_printf("mmcSPI_write(): Data copied. Now write back\n");
         if(mmcSPI_singleBlockWrite(self, self->block_buf, sector) == 0)
         {
            //ciaaPOSIX_printf("mmcSPI_write(): write back succesfull. Next iteration\n");
            bytes_left -= bytes_write;
            i += bytes_write;
            sector++;
         }
         else
         {

         }
      }
   }
   if(0 == bytes_left)
   {
      
      ret = i;
      self->position += i;
   }
   else
   {

   }

   return ret;
}

static ssize_t mmcSPI_lseek(MmcSPI self, off_t const offset, uint8_t const whence)
{
   assert(ooc_isInstanceOf(self, MmcSPI));
   off_t destination = -1;
   size_t partition_size = self->block_size * self->nsectors;

   switch(whence)
   {
      case SEEK_END:
         destination = partition_size + offset;
         break;
      case SEEK_CUR:
         destination = self->position + offset;
         break;
      default:
         destination = offset;
         break;
   }

   if ((destination >= 0) && (destination < partition_size))
   {
      self->position = destination;
   }

   return destination;
}

static int mmcSPI_ioctl(MmcSPI self, int32_t request, void* param)
{
   assert(ooc_isInstanceOf(self, MmcSPI));

   int32_t ret = -1;

   blockDevInfo_t * blockInfo = (blockDevInfo_t *)param;

   switch(request)
   {
      case IOCTL_BLOCK_GETINFO:
         blockInfo->size = self->block_size;
         blockInfo->num = self->nsectors;
         ret = 1;
         break;
      default:
         break;
   }

   return ret;
}

static int mmcSPI_connect(MmcSPI self)
{
   assert(ooc_isInstanceOf(self, MmcSPI));
   return 0;
}
static int mmcSPI_disconnect(MmcSPI self)
{
   assert(ooc_isInstanceOf(self, MmcSPI));
   return 0;
}

static int mmcSPI_getState(MmcSPI self, blockDevState_t *state)
{
   assert(ooc_isInstanceOf(self, MmcSPI));
   return 0;
}

static int mmcSPI_getInfo(MmcSPI self, blockDevInfo_t *info)
{
   assert(ooc_isInstanceOf(self, MmcSPI));
   return 0;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

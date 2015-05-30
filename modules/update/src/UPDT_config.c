/* Copyright 2015, Daniel Cohen
 * Copyright 2015, Esteban Volentini
 * Copyright 2015, Matias Giori
 * Copyright 2015, Franco Salinas
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

/** \brief This file implements the ADTP unpacker functionality
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup ADTP CIAA ADTP Unpacker
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150515 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "UPDT_config.h"

/*==================[macros and definitions]=================================*/
/** \brief Flash memory address where the configuration is stored */
#define UPDT_CONFIG_ADDRESS      0x0

/** \brief Configuration size */
#define UPDT_CONFIG_SIZE         0x18

typedef struct
{
   uint32_t reserved1;
   uint32_t firmware_version;
   uint32_t bootloader_flags;
   uint32_t bootloader_version;
   uint32_t reserved2;
   uint32_t application_version;
   uint32_t vendor_id;
   uint32_t model_id;
   uint64_t unique_id;
   uint32_t data_size;
} UPDT_configType;
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static UPDT_configType UPDT_config_new;
static UPDT_configType UPDT_config_old;
static uint8_t UPDT_config_buffer[UPDT_CONFIG_SIZE];
/*==================[external data definition]===============================*/

/* payload parsing */
//void ADTP_protocolGetInfo(const uint8_t *payload, ADTP_protocolInfoType *info);
/*==================[internal functions definition]==========================*/
void ADTP_configParse(const uint8_t *payload, size_t size, UPDT_configType *info)
{
   uint32_t word;
   const uint32_t *ptr;
   ciaaPOSIX_assert(NULL != payload && NULL != info);

   ptr = (const uint32_t *) payload;

   word = ADTP_utilsNtohl(*ptr);
   info->reserved1 = 0xFF & (word >> 24);
   info->firmware_version = 0xFFFFFF & (word);

   word = ADTP_utilsNtohl(*(ptr + 1));
   info->bootloader_flags = 0xFF & (word >> 24);
   info->bootloader_version = 0xFFFFFF & (word);

   word = ADTP_utilsNtohl(*(ptr + 2));
   info->reserved2 = 0xFF & (word >> 24);
   info->application_version = 0xFFFFFF & (word);

   word = ADTP_utilsNtohl(*(ptr + 3));
   info->vendor_id = 0xFF & (word >> 24);
   info->model_id = 0xFFFFFF & (word);

   info->unique_id = ADTP_utilsNtohll(*(const uint64_t *) (ptr + 4));

   info->data_size = ADTP_utilsNtohl(*(ptr + 6));
}
/*==================[external functions definition]==========================*/

/** \note THE BOOTLOADER FLAGS AND VERSIONS ARE NOT STORED WITH A STORE CALL.
 ** BOTH MUST BE CONSTANTS DEFINED IN THE SERVICE HEADER FILE. A FIRMWARE
 ** UPDATE CANNOT CHANGE THE BOOTLOADER ATTRIBUTES.
 **/
uint32_t UPDT_configSet(const uint8_t *config, size_t size)
{
   uint32_t ret = 0;
   /* compare the expected configuration size with the received size */
   if(UPDT_CONFIG_SIZE != size)
   {
      return UPDT_CONFIG_ERROR_INVALID_SIZE;
   }

   UPDT_configParse(config, UPDT_CONFIG_SIZE, &UPDT_config_new);
   UPDT_configParse(UPDT_CONFIG_ADDRESS, UPDT_CONFIG_SIZE, &UPDT_config_old);

   /* reserved fields must be zero */
   if(0 != UPDT_config_new.reserved1 | UPDT_config_new.reserved2)
      ret |= UPDT_CONFIG_ERROR_RESERVED;

   /** \todo all error checks */

   if(UPDT_config_old.firmware_version > UPDT_config_new.firmware_version)
      ret |= UPDT_CONFIG_WARNING_FIRMWARE_VERSION;

   /** \todo all warning checks */
}


int32_t UPDT_configStore(void)
{
   /** \todo
    ** - format the new configuration in the compact form
    ** - discard the bootloader version and flag fields
    ** - store the formated configuration into the block device. UPDT_storage
    ** function should not be used. Instead use direct block device access
    **/

   return 0

}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/



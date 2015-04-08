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

/** \brief This file implements the Flash Update Unpacker functionality
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Unpacker
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
 * 20150408 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "ciaaUpdate_unpacker.h"
#include "ciaaUpdate_utils.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
ciaaUpdate_unpackerType *ciaaUpdate_unpackerNew(void)
{
   ciaaUpdate_unpackerType *unpacker = ciaaPOSIX_malloc(sizeof(ciaaUpdate_unpackerType));

   unpacker->segment_remaining_bytes = 0;
   return unpacker;
}
void ciaaUpdate_unpackerDel(ciaaUpdate_unpackerType *unpacker)
{
   ciaaPOSIX_assert(NULL != unpacker);

   unpacker->segment_remaining_bytes = 0;
   ciaaPOSIX_free(unpacker);
}
uint8_t *ciaaUpdate_unpackerExtract(
   ciaaUpdate_unpackerType* unpacker,
   const uint8_t * unparsed_data,
   size_t *unparsed_bytes_count
)
{
   ciaaPOSIX_assert(NULL != unpacker && NULL != unparsed_data && NULL != unparsed_bytes_count);

   /* multiple of 8 and smaller than 2048 */
   ciaaPOSIX_assert(0 == (*unparsed_bytes_count & 0xF807));

   unpacker->start = (uint8_t *) unparsed_data;
   unpacker->size = 0;

   if(*unparsed_bytes_count > 0)
   {
      /* if the current segment is completely parsed or this is the start of
       * the first segment */
      if(unpacker->segment_remaining_bytes == 0)
      {
         /* then the segment address and size must be read */

         /* read the starting address from the first 4 bytes of the buffer */
         unpacker->segment_destination_address = ciaaUpdate_utilsNtohl(*((uint32_t*) unparsed_data));
         /* increment data pointer */
         unparsed_data += sizeof(uint32_t);
         /* update the unparsed bytes count */
         *unparsed_bytes_count -= sizeof(uint32_t);

         /* read the segment size from the second 4 bytes of the buffer */
         unpacker->segment_remaining_bytes = ciaaUpdate_utilsNtohl(*((uint32_t*) unparsed_data));
         /* increment data pointer */
         unparsed_data += sizeof(uint32_t);
         /* update the unparsed bytes count */
         *unparsed_bytes_count -= sizeof(uint32_t);

         /* set the start of data */
         unpacker->start = (uint8_t *) unparsed_data;
      }

      /* unparsed_bytes_count is still bigger than zero */
      if(*unparsed_bytes_count > 0)
      {
         /* there is pending bytes */

         /* set address */
         unpacker->address = unpacker->segment_destination_address;

         /* set the amount of data to be extracted */
         unpacker->size = ciaaUpdate_utilsMin(unpacker->segment_remaining_bytes, *unparsed_bytes_count);

         /* update data pointer */
         unparsed_data += unpacker->size;
         /* update the unparsed bytes count */
         *unparsed_bytes_count -= unpacker->size;

         /* update destination address */
         unpacker->segment_destination_address += unpacker->size;
         /* update the unreceived segment bytes count */
         unpacker->segment_remaining_bytes -= unpacker->size;
      }
   }
   return (uint8_t *) unparsed_data;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


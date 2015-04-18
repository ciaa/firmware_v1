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

/** \brief This file implements the Flash Update Packer functionality
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Updater CIAA Updater Packer
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
#include "ciaaPOSIX_string.h"
#include "ciaaUpdate_packer.h"
#include "ciaaUpdate_utils.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void ciaaUpdate_packerMakeHeader(uint8_t *dest, uint32_t segment_address, uint32_t segment_size)
{
   *(uint32_t *) dest = ciaaUpdate_utilsHtonl(segment_address);
   *(uint32_t *) (dest + 4) = ciaaUpdate_utilsHtonl(segment_size);
}

uint8_t ciaaUpdate_packerMakePadding(uint8_t *dest, uint32_t segment_size)
{
   int32_t i;
   int32_t padding_size;

   i = padding_size = ((~segment_size) + 1) & 0x07;
   while(i--)
   {
      dest[i] = ciaaPOSIX_rand() % 256;
   }
   return i;
}

int32_t ciaaUpdate_packerInit(
   ciaaUpdate_packerType *packer,
   ciaaUpdate_packerParseCallback parse,
   uint8_t *buffer,
   size_t size)
{
   ciaaPOSIX_assert(NULL != packer && NULL != parse && NULL != buffer && size > 0);

   packer->parse = parse;
   packer->buffer = buffer;
   packer->buffer_max_size = size;

   packer->segment_size = 0;
   packer->segment_data = NULL;

   return 0;
}

ssize_t ciaaUpdate_packerGet(ciaaUpdate_packerType *packer)
{
   uint32_t address;
   uint32_t size;
   /* pointer to the next write position in the buffer */
   uint8_t *buffer_ptr;
   /* free space in the buffer */
   size_t buffer_size;

   ciaaPOSIX_assert(NULL != packer && NULL != packer->parse && NULL != packer->buffer);

   /* initialize the buffer state */
   buffer_size = packer->buffer_max_size;
   buffer_ptr = packer->buffer;

   /* while there is still segments to parse */
   while(0 != packer->segment_size ||
         NULL != (packer->segment_data = packer->parse(&address, &size)))
   {
      /* if the current segment is over (or this is the first one) */
      if(0 == packer->segment_size)
      {
         /* then write the segment header */

         /* set the segment state */
         packer->segment_size = size;

         /* write the header into the buffer */
         ciaaUpdate_packerMakeHeader(buffer_ptr, address, size);

         /* update the buffer state */
         buffer_size += size;
         buffer_ptr += size;

         /* if the buffer is full */
         if(0 == buffer_size)
         {
            return packer->buffer_max_size;
         }
      }

      /* write the segment data */

      /* calculate the number of bytes to write */
      size = ciaaUpdate_utilsMin(packer->segment_size, buffer_size);

      /* write them into the buffer */
      ciaaPOSIX_memcpy(buffer_ptr, packer->segment_data, size);

      /* update the segment state */
      packer->segment_size -= size;
      packer->segment_data += size;

      /* update the buffer size */
      buffer_size -= size;
      buffer_ptr += size;

      /* if the segment is over */
      if(0 == packer->segment_size)
      {
         /* then add padding */
         size = ciaaUpdate_packerMakePadding(buffer_ptr, packer->buffer_max_size - buffer_size);

         /* update the buffer state */
         buffer_size -= size;
         buffer_ptr += size;
      }

      /* if the buffer is full */
      if(0 == buffer_size)
      {
         return packer->buffer_max_size;
      }
   }
   return packer->buffer_max_size - buffer_size;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

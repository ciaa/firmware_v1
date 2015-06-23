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

/** \brief This file implements the Flash UPDT Packer functionality
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup UPDT CIAA UPDT Packer
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
 * 20150419 v0.0.2  FS  change prefixes
 * 20150408 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_string.h"
#include "UPDT_packer.h"
#include "UPDT_IParser.h"
#include "UPDT_utils.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void UPDT_packerMakeHeader(uint8_t *dest, uint32_t segment_address, uint32_t segment_size)
{
   *(uint32_t *) dest = UPDT_utilsHtonl(segment_address);
   *(uint32_t *) (dest + 4) = UPDT_utilsHtonl(segment_size);
}

uint8_t UPDT_packerMakePadding(uint8_t *dest, uint32_t segment_size)
{
   uint32_t i;
   int32_t padding_size;

   i = padding_size = ((~segment_size) + 1) & 0x07u;
   while(i)
   {
      dest[--i] = ciaaPOSIX_rand() % 256;
   }
   ciaaPOSIX_assert(i < 8);
   return i;
}

int32_t UPDT_packerInit(
   UPDT_packerType *packer,
   UPDT_parserType *parser,
   uint8_t *buffer,
   size_t size)
{
   ciaaPOSIX_assert(NULL != packer);
   ciaaPOSIX_assert(NULL != parser);
   ciaaPOSIX_assert(NULL != buffer);
   ciaaPOSIX_assert(size > 0);

   packer->parser = parser;
   packer->buffer = buffer;
   packer->buffer_max_size = size;

   packer->segment_size = 0;
   packer->segment_data = NULL;

   return 0;
}
void UPDT_packerClear(UPDT_packerType *packer)
{
   ciaaPOSIX_assert(NULL != packer);

   packer->segment_data = NULL;
   packer->segment_size = 0;

   packer->buffer_max_size = 0;
   packer->buffer = NULL;
   packer->parser = NULL;

}
ssize_t UPDT_packerGet(UPDT_packerType *packer)
{
   uint32_t address;
   uint32_t size;
   /* pointer to the next writing position in the buffer */
   uint8_t *buffer_ptr;
   /* free space in the buffer */
   size_t buffer_size;

   ciaaPOSIX_assert(NULL != packer);
   ciaaPOSIX_assert(NULL != packer->parser);
   ciaaPOSIX_assert(NULL != packer->buffer);

   /* initialize the buffer state */
   buffer_size = packer->buffer_max_size;
   buffer_ptr = packer->buffer;

   /* while there is still segments to parse */
   while(0 != packer->segment_size ||
         NULL != (packer->segment_data = packer->parser->get(packer->parser, &address, &size)))
   {
      /* if the current segment is over (or this is the first one) */
      if(0 == packer->segment_size)
      {
         /* then write the segment header */

         /* set the segment state */
         packer->segment_size = size;

         /* write the header into the buffer */
         UPDT_packerMakeHeader(buffer_ptr, address, size);

         /* update the buffer state */
         buffer_size -= 8;
         buffer_ptr += 8;

         /* if the buffer is full */
         if(0 == buffer_size)
         {
            return packer->buffer_max_size;
         }
      }

      /* write the segment data */

      /* calculate the number of bytes to ciaaPOSIX_write */
      size = UPDT_utilsMin(packer->segment_size, buffer_size);

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
         size = UPDT_packerMakePadding(buffer_ptr, packer->buffer_max_size - buffer_size);

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
   /* return the number of bytes written */
   return packer->buffer_max_size - buffer_size;
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

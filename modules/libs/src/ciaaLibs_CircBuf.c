/* Copyright 2014, Mariano Cerdeiro
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

/** \brief Circula Buffer Library header
 **
 ** This library provides a circular buffer
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions head)
 * -----------------------------------------------------------
 * 20140611 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaLibs_CircBuf.h"
#include "ciaaLibs_Maths.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern ciaaLibs_CircBufType * ciaaLibs_circBufNew(size_t size)
{
   ciaaLibs_CircBufType * ret = NULL;

   /* check that size is at least 8 and power of 2 */
   if ( (size > 7) && (ciaaLibs_isPowerOfTwo(size)) )
   {
      ret = ciaaPOSIX_malloc(sizeof(ciaaLibs_CircBufType)+size);

      /* if a valid pointer has been returned */
      if (NULL != ret)
      {
         /* init the buffer */
         ret->size = size;
         ret->head = 0;
         ret->tail = 0;
         ret->buf = (uint8_t *) ((intptr_t) ret+ sizeof(ciaaLibs_CircBufType));
      }
   }

   return ret;
} /* end ciaaLibs_circBufNew */

extern void ciaaLibs_circBufRel(ciaaLibs_CircBufType * cbuf)
{
   /* free reserved memory */
   ciaaPOSIX_free(cbuf);
} /* end ciaaLibs_circBufRel */

extern size_t ciaaLibs_circBufPut(ciaaLibs_CircBufType * cbuf, void const * data, size_t size)
{
   size_t ret = 0;
   size_t rawSpace;
   size_t newTail;

   /* the head of the circular buffer may be changed, therefore it has to be
    * read only once */
   size_t head = cbuf->head;

   /* check that is enough place */
   if (ciaaLibs_circBufSpace(cbuf, head) >= size)
   {
      rawSpace = ciaaLibs_circBufRawSpace(cbuf, head);

      /* check if wrapping is needed */
      if (rawSpace >= size)
      {
         ciaaPOSIX_memcpy(&cbuf->buf[cbuf->tail], data, size);
      }
      else
      {
         ciaaPOSIX_memcpy((void*)(&cbuf->buf[cbuf->tail]), data, rawSpace);
         ciaaPOSIX_memcpy((void*)(&cbuf->buf[0]), (void*)((intptr_t)data + rawSpace), size-rawSpace);
      }

      /* calculate tail position */
      newTail = cbuf->tail;
      newTail += size;
      if (newTail >= cbuf->size)
      {
         newTail -= cbuf->size;
      }

      /* set tail position */
      cbuf->tail = newTail;

      /* set return value */
      ret = size;
   }

   return ret;
} /* end ciaaLibs_circBufPut */

extern size_t ciaaLibs_circBufGet(ciaaLibs_CircBufType * cbuf, void * data, size_t size)
{
   size_t rawCount;
   size_t newHead;

   /* the tail of the circular buffer my be changed, therefore it has to be
    * read only once */
   size_t tail = cbuf->tail;

   /* if the users tries to read to much data, only available data will be
    * provided */
   if (size > ciaaLibs_circBufCount(cbuf, tail))
   {
      size = ciaaLibs_circBufCount(cbuf, tail);
   }

   /* check if data to be read */
   if (size > 0)
   {
      rawCount = ciaaLibs_circBufRawCount(cbuf, tail);

      /* check if all data can be read without wrapping */
      if (size <= rawCount)
      {
         ciaaPOSIX_memcpy(data, (void*)&cbuf->buf[cbuf->head], size);
      }
      else
      {
         ciaaPOSIX_memcpy(data, (void*)(&cbuf->buf[cbuf->head]), rawCount);
         ciaaPOSIX_memcpy((void*)((intptr_t)data + rawCount), (void*)(&cbuf->buf[0]), size-rawCount);
      }

      /* calculates new head */
      newHead = cbuf->head;
      newHead += size;
      if (newHead >= cbuf->size)
      {
         newHead -= cbuf->size;
      }

      /* set head position */
      cbuf->head = newHead;
   }

   return size;
} /* end ciaaLibs_circBufGet */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


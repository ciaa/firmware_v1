/* Copyright 2016, Mariano Cerdeiro
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

/** \brief Pool Buffer Library sources
 **
 ** This library provides a pool buffer
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaLibs_PoolBuf.h"
#include "ciaaLibs_Maths.h"
#include "ciaaPOSIX_string.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/


/*==================[external functions definition]==========================*/
extern int32_t ciaaLibs_poolBufInit(ciaaLibs_poolBufType * pbuf,
      void * buf, uint32_t * statusPtr, size_t poolSize, size_t elementSize)
{
   int32_t ret = 1;
   uint32_t i;

   /* all 3 buffers shall be valid */
   if (NULL == pbuf) {
      ret = -1;
   }
   if (NULL == buf) {
      ret = -1;
   }
   if (NULL == statusPtr) {
      ret = -1;
   }

   /* if not errors are found perform the initialization */
   if (1 == ret) {
      pbuf->buf = buf;
      pbuf->statusPtr = statusPtr;
      pbuf->poolSize = poolSize;
      pbuf->elementSize = elementSize;

      for(i = 0; i < (poolSize >> 5); i++) {
         /* indicate that all elements are free and not beeing used */
         pbuf->statusPtr[i] = 0;
      }
   }

   return ret;
} /* end of ciaaLibs_poolBufInit */

extern void * ciaaLibs_poolBufLock(ciaaLibs_poolBufType * pbuf)
{
   void * ret = NULL;
   uint32_t i; /** <= variable for the loop */
   uint32_t loopCount;
   uint32_t freePos = 0; /** variable to indicate which position is free */
   int8_t pos;
   bool found = false;

   loopCount = (pbuf->poolSize + 31 ) >> 5;

   /* enter critical section */

   for(i = 0; (i < loopCount) && (false == found); i++) {
      pos = ciaaLibs_getFirstNotSetBit(pbuf->statusPtr[i]);
      if (-1 != pos) {
         freePos += pos;
         found = true;
      } else {
         /* no free place in the first 32 positions */
         freePos += 32;
      }
   }

   /* if found and the found free element is within the size */
   if ((true == found) && (freePos < pbuf->poolSize))
   {
      /* reserve the element in the pool */
      ciaaLibs_setBit(pbuf->statusPtr[i-1], freePos & 0x1f);

      /* exit critical section */

      /* get element address */
      ret = (void*) &pbuf->buf[freePos * pbuf->elementSize];
   } else {
      /* exit critical section */
   }

   return ret;
} /* end of ciaaLibs_poolBufLock */

extern size_t ciaaLibs_poolBufFree(ciaaLibs_poolBufType * pbuf, void * data)
{
   /* if data is smaller pbuf->buf this will crash, but it shall never
    * happens. */
   uintptr_t diff = (uintptr_t)data - (uintptr_t)(pbuf->buf);

   size_t element = diff / pbuf->elementSize;

   /* free the element */
   ciaaLibs_clearBit(pbuf->statusPtr[element>>5], element & 0x1f);

   return 1;
} /* end of ciaaLibs_poolBufFree */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief ciaa POSIX stdlib source file
 **
 ** ciaa POSIX stdio source file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DiFe         Diego Fernandez
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140524 v0.0.1 DiFe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdint.h"

/*==================[macros and definitions]=================================*/

#define CIAA_HEAP_MEM_SIZE 10000
#define CIAA_POSIX_STDLIB_AVAILABLE 1
#define CIAA_POSIX_STDLIB_USED 0

/*==================[internal data declaration]==============================*/

struct ciaaPOSIX_chunk_header
{
   /* Next chunk pointer */
   struct ciaaPOSIX_chunk_header *next;

   /* Size of chunk */
   uint32_t size;

   /* Indicates if this chunk is available */
   bool is_available;
};
typedef struct ciaaPOSIX_chunk_header ciaaPOSIX_chunk_header;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief ciaa first chunk header*/
ciaaPOSIX_chunk_header *first_chunk_header;

/** \brief ciaa memory buffer */
static char ciaaPOSIX_buffer[CIAA_HEAP_MEM_SIZE];

/** \brief ciaa POSIX sempahore */
sem_t ciaaPOSIX_stdlib_sem;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void ciaaPOSIX_chunk_partition(ciaaPOSIX_chunk_header *chunk_header, uint32_t size)
{
   // if there is at least one byte after the partition
   if (chunk_header->size > size + sizeof(ciaaPOSIX_chunk_header)) {
       ciaaPOSIX_chunk_header *next_chunk_header = (ciaaPOSIX_chunk_header *)(((char *)chunk_header) + size + sizeof(ciaaPOSIX_chunk_header));
       next_chunk_header->next = chunk_header->next;
       chunk_header->next = next_chunk_header;
       next_chunk_header->size = chunk_header->size - size - sizeof(ciaaPOSIX_chunk_header);
       next_chunk_header->is_available = CIAA_POSIX_STDLIB_AVAILABLE;
       chunk_header->size = size;
  }
  chunk_header->is_available = CIAA_POSIX_STDLIB_USED;
}

/*==================[external functions definition]==========================*/

void ciaaPOSIX_stdlib_init(void)
{
   int ciaaPOSIX_heap_available_size = CIAA_HEAP_MEM_SIZE - sizeof(ciaaPOSIX_chunk_header);
   first_chunk_header = (ciaaPOSIX_chunk_header*)&ciaaPOSIX_buffer;
   first_chunk_header->next = NULL;
   first_chunk_header->size = ciaaPOSIX_heap_available_size;
   first_chunk_header->is_available = CIAA_POSIX_STDLIB_AVAILABLE;
   /* init sempahore */
   ciaaPOSIX_sem_init(&ciaaPOSIX_stdlib_sem);
}

void *ciaaPOSIX_malloc(size_t size)
{
   char *result = NULL;
   ciaaPOSIX_chunk_header *chunk_header = first_chunk_header;

   /* enter critical section */
   ciaaPOSIX_sem_wait(&ciaaPOSIX_stdlib_sem);
   while(chunk_header)
   {
      if(chunk_header->is_available && chunk_header->size >= size)
      {
         ciaaPOSIX_chunk_partition(chunk_header, size);
         result  = ((char *)chunk_header)+sizeof(ciaaPOSIX_chunk_header);
         break;
      }
      chunk_header = chunk_header->next;
   }
   /* exit critical section */
   ciaaPOSIX_sem_post(&ciaaPOSIX_stdlib_sem);
   return result;
}

void ciaaPOSIX_free(void *ptr)
{
   ciaaPOSIX_chunk_header *chunk_to_free = (ciaaPOSIX_chunk_header*)(((char *) ptr) - sizeof(ciaaPOSIX_chunk_header));
   ciaaPOSIX_chunk_header *chunk_header = first_chunk_header;

   /* enter critical section */
   ciaaPOSIX_sem_wait(&ciaaPOSIX_stdlib_sem);

   while (chunk_header)
   {
      if (chunk_header == chunk_to_free)
      {
         chunk_header->is_available = CIAA_POSIX_STDLIB_AVAILABLE;
         break;
      }
      chunk_header = chunk_header->next;
   }

   /* small defragmentator */
   chunk_header = first_chunk_header;
   while (chunk_header) {
      if (chunk_header->is_available) {
         while (chunk_header->next!=NULL && chunk_header->next->is_available) {
            chunk_header->size += chunk_header->next->size + sizeof(ciaaPOSIX_chunk_header);
            chunk_header->next = chunk_header->next->next;
         }
      }
      chunk_header = chunk_header->next;
   }

   /* exit critical section */
   ciaaPOSIX_sem_post(&ciaaPOSIX_stdlib_sem);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

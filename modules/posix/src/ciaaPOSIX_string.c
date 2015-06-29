/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Juan Cecconi
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
 * JuCe         Juan Cecconi
 * MaCe         Mariano Cerdeiro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150301 v0.0.4 MaCe implement memset and memcmp
 * 20141116 v0.0.3 JuCe improve strcpy, fix strcat
 * 20140608 v0.0.2 MaCe implement strlen, strcat, strcmp and strncmp
 * 20140530 v0.0.1 JuCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stddef.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern char * ciaaPOSIX_strcpy(char * s1, char const * s2)
{
   while(0 != *s2)
   {
      *s1 = *s2;
      s2++;
      s1++;
   }
   *s1 = '\0';
   return s1;
}

extern size_t ciaaPOSIX_strlen(char const * s) {
   uint32_t length = 0;

   /* search for the first null within the string */
   while(0 != *s)
   {
      /* if not found, increment the position */
      s++;
      /* and increment the lenght counter */
      length++;
   }

   return length;
}

extern char * ciaaPOSIX_strcat(char * dest, char const * src)
{
   char *ret = dest;
   size_t length = ciaaPOSIX_strlen(dest);

   /* increment size of initial string */
   dest += length;

   while(0 != *src)
   {
      /* copy one char */
      *dest = *src;

      /* increment pointers */
      dest++;
      src++;
   }

   /* add null termination */
   *dest = 0;

   return ret;
}

extern int8_t ciaaPOSIX_strncmp(char const * s1, char const * s2, size_t n)
{
   int8_t ret = 0;

   while( (0 != *s1) && (0 != *s2) && (0 == ret) && (0 < n) )
   {
      /* compare */
      if (*s1 > *s2)
      {
         ret = 1;
      }
      else if (*s1 < *s2)
      {
         ret = -1;
      }
      else
      {
         /* go ahead with the comparassion */
         /* *s1 is equal to *s2 */
      }

      /* increment pointers */
      s1++;
      s2++;

      /* decrement count */
      n--;
   }

   /* check if all needed chars have been compared */
   if ( (0 != n) && (0 == ret) )
   {
      if (*s1 > *s2)
      {
         ret = 1;
      }
      else if (*s1 < *s2)
      {
         ret = -1;
      }
      else
      {
         /* go ahead with the comparassion */
         /* *s1 is equal to *s2 */
      }
   }


   return ret;
}

extern int8_t ciaaPOSIX_strcmp(char const * s1, char const * s2)
{
   int8_t ret = 0;

   while( (0 != *s1) && (0 != *s2) && (0 == ret) )
   {
      /* compare */
      if (*s1 > *s2)
      {
         ret = 1;
      }
      else if (*s1 < *s2)
      {
         ret = -1;
      }
      else
      {
         /* go ahead with the comparassion */
         /* *s1 is equal to *s2 */
      }

      /* increment pointers */
      s1++;
      s2++;
   }

   /* in case that are equal */
   if (0 == ret)
   {
      /* we have to check if both strings have the same length */
      if (*s1 > *s2)
      {
         ret = 1;
      }
      else if (*s1 < *s2)
      {
         ret = -1;
      }
      else
      {
         /* go ahead with the comparassion */
         /* *s1 is equal to *s2 */
      }
   }

   return ret;
}

extern void * ciaaPOSIX_memcpy(void * s1, void const * s2, size_t n)
{
   while(0 < n)
   {
      /* decrement counter */
      n--;

      /* copy 1 byte */
      ((uint8_t*)s1)[n] = ((uint8_t*)s2)[n];
   }

   return s1;
}

extern void * ciaaPOSIX_memset(void * s, int c, size_t n)
{
   while(0 < n)
   {
      /* decement counter */
      n--;

      /* set 1 byte */
      ((uint8_t*)s)[n] = (uint8_t)c;
   }

   return s;
}

extern int32_t ciaaPOSIX_memcmp(const void * s1, const void * s2, size_t n)
{
   int32_t ret = 0;

   while((0 < n) && (0 == ret))
   {
      /* decrement counter */
      n--;

      if (*(uint8_t*)s1 > *(uint8_t*)s2)
      {
         /* s1 is grater */
         ret = 1;
      }
      else if (*(uint8_t*)s1 < *(uint8_t*)s2)
      {
         /* s2 is grater */
         ret = -1;
      }

      /* increment pointer */
      s1++;
      s2++;
   }
   return ret;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


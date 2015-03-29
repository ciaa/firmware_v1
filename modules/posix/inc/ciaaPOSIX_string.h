/* Copyright 2014, Mariano Cerdeiro
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

#ifndef CIAAPOSIX_STRING_H
#define CIAAPOSIX_STRING_H
/** \brief ciaa POSIX string header file
 **
 ** ciaa POSIX string header file
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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150301 v0.0.4 MaCe implement memset and memcmp
 * 20140610 v0.0.3 MaCe implement memcpy
 * 20140608 v0.0.2 MaCe implement strlen, strcat, strcmp and strncmp
 * 20140530 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stddef.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#ifndef NULL
#define NULL (void*)0
#endif
/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief string copy
 **
 ** \param[in] s1 destination buffer
 ** \param[in] s2 string to be copied
 ** \return a pointer to the terminating NULL character copied into the s1 buffer
 **
 **/
extern char * ciaaPOSIX_strcpy(char * s1, char const * s2);

/** \brief get string length
 **
 ** \param[in] s string to compute the length
 ** \return string length of s
 **
 **/
extern size_t ciaaPOSIX_strlen(char const * s);

/** \brief concatenate two strings
 **
 ** Concatenate src at the end of the dest string.
 **
 ** \param[inout] dest string to be concatenated at the end
 ** \param[in] src string to concatenate after dest.
 ** \return dest if success and other value if an error occurs
 **/
extern char * ciaaPOSIX_strcat(char * dest, char const * src);

/** \brief compare part of two strings
 **
 ** \param[in] s1 first string to be compared
 ** \param[in] s2 second string to be compared
 ** \param[in] n maximal count of characters to be compared
 ** \returns a positive number if s1 > s2, 0 if s1 = s2 and negative number if
 **          s1 < s2
 **/
extern int8_t ciaaPOSIX_strncmp(char const * s1, char const * s2, size_t n);

/** \brief compare two strings
 **
 ** \param[in] s1 first string to be compared
 ** \param[in] s2 second string to be compared
 ** \returns a positive number if s1 > s2, 0 if s1 = s2 and negative number if
 **          s1 < s2
 **/
extern int8_t ciaaPOSIX_strcmp(char const * s1, char const * s2);

/** \brief copy bytes of memory
 **
 ** copy n bytes from s2 to s1.
 **
 ** \param[out] s1 destination pointer
 ** \param[in] s2 source pointer
 ** \param[in] n count of bytes to be copied
 **
 **/
extern void * ciaaPOSIX_memcpy(void * s1, void const * s2, size_t n);

/** \brief set n bytes to memory to (uint8_t)c
 **
 ** set n bytes of memory to the value of c casted to uint8_t.
 **
 ** \param[in] s pointer to memory to be set
 ** \param[in] c value to set the memory to
 ** \param[in] n count of bytes to be set
 ** \return returns the input parameter s
 **
 **/
extern void * ciaaPOSIX_memset(void * s, int c, size_t n);

/** \brief compare n bytes between the pointer s1 and s2
 **
 ** Compare n bytes in the memory locations given in s1 and s2.
 **
 ** \param[in] s1 pointer to the first memory location to compare
 ** \param[in] s1 pointer to the second memory location to compare
 ** \param[in] n count of bytes to be compared
 ** \returns -1,0,1 if the pointed objects are s1<s2, s1=s2, s1>s2
 **          respectivelly
 **
 **/
extern int32_t ciaaPOSIX_memcmp(const void * s1, const void * s2, size_t n);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_STRING_H */


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

#ifndef CIAALIBS_POOLBUF_H
#define CIAALIBS_POOLBUF_H
/** \brief Pool of Buffer Library header
 **
 ** This library provides a pool of buffers
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Libs CIAA Libraries
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdlib.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief macro to define the pool declaration variables
 **
 ** This macro genertes the definition of 3 variables called:
 **  * <name>_buf: array of type type and size size
 **  * <name>_status: array to store the status (free and used of each element
 **                   of the buffer <name>_buf.
 **  * <name>: pool
 **
 ** If you use this macro you do not need to call ciaaLibs_poolBufInit.
 **
 ** \param[in] name name of the pool
 ** \param[in] type type of the variable
 ** \param[in] size size of the pool
 **
 **/
#define CIAALIBS_POOLDECLARE(name, type, size)  \
   type name ## _buf[(size)];                   \
   uint32_t name ## _status[(size)/32] = { 0 }; \
   ciaaLibs_poolBufType name = {                \
      (size),                                   \
      sizeof(type),                             \
      name ## _status,                          \
      name ## _buf                              \
   };


/*==================[typedef]================================================*/
/** \brief pool buffer type
 **
 **/
typedef struct {
   size_t poolSize;      /** <= count of elements which can be stored in this
                               pool. Shall be 32 * x */
   size_t elementSize;   /** <= size of each element */
   uint32_t * statusPtr; /** <= pointer to an array of size poolSize / 32, each
                                bit indicataes with 0 that the corresponding
                                pool element is not used, with one that is
                                beeing used. */
   uint8_t * buf;        /** <= pointer to the buffer. Buffer shall be
                               poolSize * elementSize */
} ciaaLibs_poolBufType;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief initialize a pool buffer
 **
 ** Performs the initialization of the pool buffer without allocating any memory.
 **
 ** \param[inout] pbuf pool buffer to be initializated
 ** \param[in] buf pointer to the buffer with size poolSize * elementSize
 ** \param[in] statusPtr pointer to the buffer of poolSize / 32 of type uint32
 ** \param[in] poolSize size of the pool shall be multiple of 32
 ** \param[in] elementSize size of an element in the pool
 ** \return 1 if init can be performed -1 in other case
 **
 **/
extern int32_t ciaaLibs_poolBufInit(ciaaLibs_poolBufType * pbuf,
      void * buf, uint32_t * statusPtr, size_t poolSize, size_t elementSize);

/** \brief get free place on the pool buffer
 **
 ** \param[inout] pbuf pointer to the pool buffer
 ** \return a pointer to the element or NULL if not free element is available
 **
 ** \remarks this function is not thread safe, in a multi task environment
 **          ensure that this function is not executed concurrently with the
 **          same pbuf parameter.
 **/
extern void * ciaaLibs_poolBufLock(ciaaLibs_poolBufType * pbuf);

/** \brief remove element from a pull buffer
 **
 ** \param[inout] pbuf pointer to the pool buffer
 ** \param[in]    element pointer to the element to be removed from the pool
 ** \returns 1 if success 0 if an error occurs
 **/
extern size_t ciaaLibs_poolBufFree(ciaaLibs_poolBufType * pbuf, void * data);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAALIBS_POOLBUF_H */


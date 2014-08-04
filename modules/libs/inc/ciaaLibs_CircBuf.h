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

#ifndef _CIAALIBS_CIRCBUF_H_
#define _CIAALIBS_CIRCBUF_H_
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
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140615 v0.0.7 implmenet ciaaLibs_circBufUpdateHead
 * 20140612 v0.0.6 implement ciaaLibs_circBufWritePos and
 *                 ciaaLibs_circBufUpdateTail
 * 20140612 v0.0.5 store size-1 in size to improve performance
 * 20140612 v0.0.4 rename size parameter to nbyte
 * 20140612 v0.0.3 implement ciaaLibs_circBufInit
 * 20140612 v0.0.2 implement ciaaLibs_circBufFull and ciaaLibs_circBufEmpty
 * 20140611 v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdlib.h"
#include "ciaaLibs_Maths.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief get free space on the buffer
 **
 ** This function is provided for the circular buffer writter. The user who
 ** updates the tail. The head variable may be changed in parallel. The user
 ** has to read the head in a local variable and pass it as argument to avoid
 ** changes between the use of the head variable.
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \param[in] head of the circular buffer
 ** \returns the count of free bytes on the buffer
 **/
#define ciaaLibs_circBufSpace(cbuf, head)                      \
      ( ( (head) - (cbuf)->tail - 1 ) & ( (cbuf)->size ) )

/** \brief get free space of the buffer without wrapping
 **
 ** This function is provided for the circular buffer writter. The user who
 ** updates the tail. The head variable may be changed in parallel. The user
 ** has to read the head in a local variable and pass it as argument to avoid
 ** changes between the use of the head variable.
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \param[in] head of the circular buffer
 ** \returns the count of free bytes on the buffer without wrapping
 **/
#define ciaaLibs_circBufRawSpace(cbuf, head)                   \
   ( ( (cbuf)->tail >= (head) ) ?                              \
     ( (cbuf)->size - (cbuf)->tail + ( (head) > 0 ) ) :        \
     ( (head) - (cbuf)->tail - 1 ) )

/** \brief get pointer to the position to write
 **
 ** This function is provided for the circular buffer writter.
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \return pointer to the first byte to be written
 **/
#define ciaaLibs_circBufWritePos(cbuf)                         \
   ((void*)(&(cbuf)->buf[(cbuf)->tail]))

/** \brief get pointer to the position to read
 **
 ** This function is provided for the circular buffer reader.
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \return pointer to the first byte to be read
 **/
#define ciaaLibs_circBufReadPos(cbuf)                          \
   ((void*)(&(cbuf)->buf[(cbuf)->head]))

/** \brief calculate new tail
 **
 ** This function is provided for the circular buffer writter.
 **
 ** \param[inout] cbuf pointer to the circular buffer to be updated
 ** \param[in] nbytes count of bytes written
 **/
#define ciaaLibs_circBufUpdateTail(cbuf, nbytes)                        \
   ((cbuf)->tail = ( ( (cbuf)->tail + (nbytes) ) & ( (cbuf)->size ) ) )

/** \brief calculate new head
 **
 ** This function is provided for the circular buffer reader.
 **
 ** \param[inout] cbuf pointer to the circular buffer to be updated
 ** \param[in] nbytes count of bytes read
 **/
#define ciaaLibs_circBufUpdateHead(cbuf, nbytes)                        \
      ((cbuf)->head = ( (cbuf)->head + (nbytes) ) & ((cbuf)->size) )

/** \brief get count of bytes stored in the buffer
 **
 ** This function is provided for the circular buffer reader. The user who
 ** reads data from the head. The tail variable may be changed in paralle. The
 ** user has to read the tail in a local variable and pass it as argument to
 ** avoid changes between the use of the tail variable
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \param[in] tail tail of the circular buffer
 ** \returns the count of used bytes on the buffer
 **/
#define ciaaLibs_circBufCount(cbuf, tail)                \
   ( ( (tail) - (cbuf)->head ) & ( (cbuf)->size ) )

/** \brief get count of data which can be get without wrapping
 **
 ** This function is provided for the circular buffer reader. The user who
 ** reads data from the head. The tail variable may be changed in paralle. The
 ** user has to read the tail in a local variable and pass it as argument to
 ** avoid changes between the use of the tail variable
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \param[in] tail tail of the circular buffer
 ** \returns the count of used bytes on the buffer without wrapping
 **/
#define ciaaLibs_circBufRawCount(cbuf, tail)          \
   ( ( (cbuf)->head <= (tail) ) ?                     \
     ( (tail) - (cbuf)->head ) :                      \
     ( (cbuf)->size + 1 - (cbuf)->head ) )

/** \brief checks if the buffer is empty
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \returns 1 if the buffer is empty, 0 in other case
 **/
#define ciaaLibs_circBufEmpty(cbuf)                   \
   ( (cbuf)->tail == (cbuf)->head )

/** \brief checks if the buffer is full
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \returns 1 if the buffer is full, 0 in other case
 **/
#define ciaaLibs_circBufFull(cbuf)                   \
   ( ( ( (cbuf)->tail + 1 ) & ( (cbuf)->size ) ) == (cbuf)->head )

/** \brief cleans the buffer
 **
 ** \param[in] cbuf pointer to the circular buffer
 ** \return none
 **/
#define ciaaLibs_circBufClean(cbuf)                   \
   {                                   \
      (cbuf)->head = 0;                \
      (cbuf)->tail = 0;                \
   }

/*==================[typedef]================================================*/
/** \brief circular buffer type
 **
 ** if buffer is empty head and tail are the same, if the buffer is full
 ** (tail+1)%size=head. A buffer of size 64 will can handle maximal 63 bytes.
 **
 **/
typedef struct {
   size_t head;         /** <= index of the head element of the buffer */
   size_t tail;         /** <= index of the position to write the next byte */
   size_t size;         /** <= size-1 of the buffer (>=8-1 and power of 2-1) */
   uint8_t * buf;       /** <= pointer to the buffer */
} ciaaLibs_CircBufType;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief creates a new circular buffer
 **
 ** Allocates the needed memory for a buffer of nbytes size and perform the
 ** initialization of the buffer.
 **
 ** \param[in] nbytes   size in bytes of the buffer, shall be a power of 2 and
 **                     at least 8
 ** \returns a pointer to a circular buffer or NULL if an error occurs
 **
 ** \remarks only size-1 bytes can be used within the circular buffer
 **/
extern ciaaLibs_CircBufType * ciaaLibs_circBufNew(size_t nbytes);

/** \brief initialize a circular buffer
 **
 ** Performs the initialization of the buffer without allocating any memory.
 **
 ** \param[out] cbuf circular buffer to be initializated
 ** \param[in] buf pointer to the buffer
 ** \param[in] nbytes size of the buffer, shall be a power of 2 and at least 8
 ** \return 1 if init can be performed -1 in other case
 **/
extern int32_t ciaaLibs_circBufInit(ciaaLibs_CircBufType * cbuf, void * buf, size_t nbytes);

/** \brief release a circular buffer
 **
 ** This function shall only be used for circular buffers created with
 ** ciaaLibs_circBufNew and NOT for those initalized with ciaaLibs_circBufInit.
 **
 ** \param[in] circular buffer to be released
 **/
extern void ciaaLibs_circBufRel(ciaaLibs_CircBufType * cbuf);

/** \brief put data to a circular buffer
 **
 ** \param[inout] cbuf pointer to the circular buffer
 ** \param[in]    data data to be stored in the buffer
 ** \param[in]    nbytes size of the data
 ** returns count of stored bytes
 **/
extern size_t ciaaLibs_circBufPut(ciaaLibs_CircBufType * cbuf, void const * data, size_t nybtes);

/** \brief get data from a circular buffer
 **
 ** \param[inout] cbuf pointer to the circular buffer
 ** \param[in]    pointer to store the read data
 ** \param[in]    nbytes size of the data to be read
 ** \returns count of read bytes
 **/
extern size_t ciaaLibs_circBufGet(ciaaLibs_CircBufType * cbuf, void * data, size_t nbytes);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAALIBS_CIRCBUF_H_ */


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

#ifndef UPDT_UNPACKER_H
#define UPDT_UNPACKER_H
/** \brief Flash Update Unpacker Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Unpacker
 **
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
 * 20150419 v0.0.2  FS  change prefixes
 * 20150408 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdio.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef struct
{
   uint32_t segment_remaining_bytes;
   uint32_t segment_destination_address;
   uint32_t address;
   uint8_t *start;
   size_t size;

} UPDT_unpackerType;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
UPDT_unpackerType *UPDT_unpackerNew(void);
void UPDT_unpackerDel(UPDT_unpackerType *unpacker);

/** \brief Extracts addressed data from a buffer.
 **
 ** \param[in] unpacker Unpacker descriptor. After a call to this function
 ** its start, size and address fields are updated.
 ** \param[in] unparsed_data Pointer to the unparsed data.
 ** \param[in,out] size Pointer to the unparsed data size. This function also
 ** decrements this size setting it up for the next call.
 **
 ** \return A pointer to the data to be parsed in the next call.
 ** Example
 ** \code{.c}
 ** unparsed_data = buffer;
 ** unparsed_bytes_count = buffer_size;
 ** do
 ** {
 **   unparsed_data = UPDT_unpackerExtract(unpacker, unparsed_data, &unparsed_bytes_count);
 **   store(unpacker->start, unpacker->size, unpacker->address);
 ** } while(unparsed_bytes_count > 0);
 ** \endcode
 **/
uint8_t *UPDT_unpackerExtract(
   UPDT_unpackerType* unpacker,
   const uint8_t * unparsed_data,
   size_t *unparsed_bytes_count
);
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef UPDT_UNPACKER_H */


/* Copyright 2015, Pablo Ridolfi <pridolfi@proyecto-ciaa.com.ar>
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

#ifndef MULTICORE_H
#define MULTICORE_H
/** \brief Multicore module main header file.
 **
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Multicore Multicore module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * PR           Pablo Ridolfi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150908 v0.0.1 PR   Initial version.
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdlib.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/** \brief List of available cores
 */
typedef enum
{
   CIAA_MULTICORE_CORE_0,  /**< current (master) core */
   CIAA_MULTICORE_CORE_1   /**< first slave core available */
}ciaaMulticore_cores_e;

/** \brief message struct */
typedef struct
{
      struct
      {
         uint32_t cpuid;
         uint32_t pid;
      }id;
      uint32_t data0;
      uint32_t data1;
}ciaaMulticore_ipcMsg_t;

/** \brief available inter-core commands */
typedef enum
{
   CIAA_MULTICORE_CMD_ACTIVATETASK = 0x123,
   CIAA_MULTICORE_CMD_SETEVENT = 0x456
}ciaaMulticore_ipcCmd_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief Start multicore operations
 */
int32_t ciaaMulticore_init(void);

/** \brief Send message to inter-core queue and irq to other cores
 *
 * @param dest  destination core (see ciaaMulticore_cores_e)
 * @param data0 1st data word to send
 * @param data1 2nd data word to send
 * @return != 0 on success, -1 on error
 */
int32_t ciaaMulticore_sendMessage(ciaaMulticore_cores_e dest, uint32_t data0, uint32_t data1);

/** \brief Receive message from inter-core queue
 *
 * @param data0 1st word received
 * @param data1 2nd word received
 * @return != 0 on success, -1 on error
 */
int32_t ciaaMulticore_recvMessage(uint32_t * data0, uint32_t * data1);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef MULTICORE_H */

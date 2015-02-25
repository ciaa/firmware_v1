/* Copyright 2014, Daniel Cohen
 * Copyright 2014, Esteban Volentini
 * Copyright 2014, Matias Giori
 * Copyright 2014, Franco Salinas
 * Copyright 2015, Mariano Cerdeiro
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

#ifndef CIAADRIVERFLASH_INTERNAL_H
#define CIAADRIVERFLASH_INTERNAL_H
/** \brief Internal Header file of Flash Driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup Flash Flash Drivers
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * EsVo         Esteban Volentini
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140528 v0.0.1 EsVo initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdbool.h"
#include <stdio.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** Define host filename to use a serial port 0 */
#ifndef CIAADRVFLASH_FILENAME
   #define CIAADRVFLASH_FILENAME    "FLASH.BIN"
#endif

/** Define flash block size */
#ifndef CIAADRVFLASH_BLOCK_SIZE
   #define CIAADRVFLASH_BLOCK_SIZE  512
#endif
/*==================[typedef]================================================*/
/** \brief Buffer Structure */
typedef struct {
   uint16_t length;                                /** <= Length used */
   uint8_t buffer[CIAADRVFLASH_BLOCK_SIZE];      /** <= Data storage */
} ciaaDriverFlash_bufferType;

/** \brief Uart Type */
typedef struct {
   //ciaaDriverFlash_bufferType buffer;
   int fileDescriptor;
   char const * filename;
   FILE * storage;
} ciaaDriverFlash_flashType;

/*==================[external data declaration]==============================*/
/** \brief Uart 0 */
extern ciaaDriverFlash_flashType ciaaDriverFlash_flash;

/*==================[external functions declaration]=========================*/
//extern void ciaaDriverFlash_flash0_rxIndication(void);

//extern void ciaaDriverFlash_flash0_txConfirmation(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAADRIVERFLASH_INTERNAL_H */

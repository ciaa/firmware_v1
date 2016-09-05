/* Copyright 2015, Mariano Cerdeiro
 * Copyright 2016, Juan Cecconi
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

#ifndef DIO_CFG_H
#define DIO_CFG_H
/** \brief DIO Driver Configuration File
 **
 ** This file contains the DIO Driver configuration
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup HISIO HisIO Module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151222 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "Dio_Cfg_Arch.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define DIO_CONFIG_PIN_DIRECTION_UNUSED                   0x00000001UL
#define DIO_CONFIG_PIN_DIRECTION_INPUT                    0x00000002UL
#define DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW          0x00000004UL
#define DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH         0x00000008UL
#define DIO_CONFIG_PIN_INVERTED                           0x00000010UL
#define DIO_CONFIG_PIN_ENABLE_NOTIFICATION                0x00000020UL

#define DIO_CONFIG_PORT_DIRECTION_UNUSED                  0x00000001UL
#define DIO_CONFIG_PORT_DIRECTION_INPUT                   0x00000002UL
#define DIO_CONFIG_PORT_DIRECTION_OUTPUT_INIT_LOW         0x00000004UL
#define DIO_CONFIG_PORT_DIRECTION_OUTPUT_INIT_HIGH        0x00000008UL
#define DIO_CONFIG_PORT_INVERTED                          0x00000010UL
#define DIO_CONFIG_PORT_SIZE_8                            0x00000020UL
#define DIO_CONFIG_PORT_SIZE_16                           0x00000040UL
#define DIO_CONFIG_PORT_SIZE_32                           0x00000080UL

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef DIO_CFG_H */

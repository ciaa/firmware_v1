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

#ifndef _ADTP_CONFIG_H_
#define _ADTP_CONFIG_H_
/** \brief Flash Update Config Header File
 **
 ** This files shall be included by modules using the interfaces provided by
 ** the Flash Update Config
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Update CIAA Update Config
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
 * 20150512 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdlib.h"
/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Error mask */
#define UPDT_CONFIG_ERROR_MASK                  0xFFFF0000u

/** \brief Warning mask */
#define UPDT_CONFIG_WARNING_MASK                0x0000FFFFu

/** \brief Invalid size error
 **
 ** This happens when an info payload with a size different than expected is
 ** received. */
#define UPDT_CONFIG_ERROR_INVALID_SIZE          0x00010000u

/** \brief Unexpected reserved fields values
 **
 ** The reserved1 or reserved2 fields have an unexpected value (not 0)
 **/
#define UPDT_CONFIG_ERROR_RESERVED              0x00020000u

/** \brief Incompatible bootloader version error */
#define UPDT_CONFIG_ERROR_BOOTLOADER_VERSION    0x00040000u

/** \brief Incompatible bootloader flags error
 **
 ** May happen if the image is encrypted and the current implementation does
 ** not support encryption or digital signature. */
#define UPDT_CONFIG_ERROR_BOOTLOADER_FLAGS      0x00080000u

/** \brief Wrong vendor identification error
 **
 ** The received image is not for the vendor of this CIAA. */
#define UPDT_CONFIG_ERROR_VENDOR_ID             0x00100000u

/** \brief Wrong model identification error
 **
 ** The received image is not for this CIAA model. */
#define UPDT_CONFIG_ERROR_MODEL_ID              0x00200000u

/** \brief Wrong unique identification error
 **
 ** The received image is not for this specific CIAA. */
#define UPDT_CONFIG_ERROR_UNIQUE_ID             0x00400000u

/** \brief Older firmware version */
#define UPDT_CONFIG_WARNING_FIRMWARE_VERSION    0x00000001u

/** \brief Older application version */
#define UPDT_CONFIG_WARNING_APPLICATION_VERSION 0x00000002u


/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief Changes the installed binary image configuration.
 **
 ** Compares the proposed configuration with the configuration stored. If there
 ** is compatibility the new configuration is held but not stored. To store the
 ** new configuration configStore must be called.
 **
 ** \param config Pointer to the configuration raw data.
 ** \param size Size of the raw data.
 ** \return Returns error and warning flags. 0 on success.
 **/
uint32_t UPDT_configSet(const uint8_t *config, size_t size);

/** \brief Stores the current configuration.
 **
 ** Saves the current configuration in the storage device.
 ** \return Returns 0 on success, non-zero on error.
 **/
int32_t UPDT_configStore(void);

/** \brief Formats a response payload.
 **
 ** Creates a response payload to be sent.
 ** \param buffer Buffer to hold the payload.
 ** \param size Maximum buffer size.
 ** \return Payload size on success, -1 on error.
 **/
ssize_t UPDT_configSetResponse(const uint8_t *buffer, size_t size);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _ADTP_CONFIG_H_ */

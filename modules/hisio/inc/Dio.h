/* Copyright 2015, Mariano Cerdeiro
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

#ifndef DIO_H
#define DIO_H
/** \brief Dio Drivers header file
 **
 ** This file contains is the Dio Drivers header file.
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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151222 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "Dio_Arch.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Returns the DIO Driver Version
 **
 ** \remarks This function shall be implemented as a macro, this is
 **          required on the specification.
 **/
#define Dio_GetVersionOfDriver   IO_Version(0,0,1,0);

#if 0
#if (HISIO_DIO_ERRORHOOK == HISIO_DISABLE)
/** \brief Reads a hardware pin
 **
 ** See description of Dio_GetSync as function declaration.
 **
 **/
#define Dio_SetSync              Dio_SetSync_Arch

#define Dio_GetSync              Dio_GetSync_Arch
#endif /* #if (HISIO_DIO_ERRORHOOK == HISIO_DISABLE) */
#endif

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief
 **/
extern IO_ErrorType Dio_InitSync(void * address);

#if (HISIO_DIO_ERRORHOOK == HISIO_ENABLE)
/** \brief Reads a hardware pin
 **
 ** This function returns the status of a pin of the hardware.
 **
 ** \param[in] channel indicate the channel to be read
 ** \returns IO_LOW if the pin is in its low state and IO_HIGH if the pin is in
 **          high state. If the pin is inverted over the configuration the
 **          returned values are also inverted.
 **
 ** \remarks If errorhooks are enable this function may call it with the
 **          error IO_E_INVALID_CHANNEL_ID if the provided channel is invalid.
 **
 ** \remarks This functionality is implemented as function if the errorhooks
 **          are enable, and as macro in other case.
 **/
extern IO_ValueType Dio_GetSync(IO_ChannelType channel);

/** \brief Writes a hardware pin
 **
 ** This function writes the status of a pin of the hardware.
 **
 ** \param[in] channel indicate the channel to be written
 ** \param[in] value indicates the value IO_HIGH or IO_LOW to be written.
 ** \returns nothing.
 **
 ** \remarks If errorhooks are enable this function may call it with the
 **          error IO_E_INVALID_CHANNEL_ID if the provided channel is invalid
 **          or IO_E_INVALID_VALUE if the parameter value is not set to
 **          IO_HIGH neither to IO_LOW.
 **
 ** \remarks This functionality is implemented as function if the errorhooks
 **          are enable, and as macro in other case.
 **/
extern void Dio_SetSync(IO_ChannelType channel, IO_ValueType value);

/** \brief Reads a port
 **
 ** This funcion reads a port status of the hardware.
 **
 ** \param[in] channel channel to be read
 ** \returns returns input or output value of a port. Depening of the bus
 **          size only the first 8, 16, 32 bits are relevant.
 **/
extern IO_ValueType Dio_GetPortSync(IO_ChannelType channel);

extern void Dio_SetPortSync(IO_ChannelType channel, IO_ValueType value);

extern void Dio_SetPortMaskedSync(IO_ChannelType channel, IO_ValueType value, IO_ValueType mask);

#endif /* #if (HISIO_DIO_ERRORHOOK == HISIO_ENABLE) */

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef DIO_H */


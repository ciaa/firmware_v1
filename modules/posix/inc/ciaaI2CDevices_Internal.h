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

#ifndef CIAAI2CDEVICES_INTERNAL_H
#define CIAAI2CDEVICES_INTERNAL_H
/** \brief CIAA I2C Devices Internal
 **
 ** I2C Internal Device header file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX POSIX Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 201550402 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaI2CDevices.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief I2C Slave Structure
 **
 ** This type is used to describe the each I2C slave device, an array of this
 ** type shall be defined by the user for each I2C bus to indicate which hw
 ** slave devices are conected in each bus indicating their IDs and
 ** capabilities.
 **
 **/
typedef struct {
   uint32_t maxAdd;              /** <= indicates the last address which can be
                                        read or written before a warp around */
   uint32_t maxInputBuffer;      /** <= size of written bytes before waiting
                                        writeDelayMs ms, if 0 writeDelayMs is
                                        not used */
   uint32_t writeDelayMs;        /** <= each time that maxInputBuffer are
                                        written the I2C device delays the write
                                        sequence for writeDelayMs ms before
                                        writting more bytes */
   uint8_t id;                   /** <= id of the I2C device, allowed values are
                                        between 0 and 127 inclusive */
   uint8_t addWidth;             /** <= size in bytes of the address length of
                                        this device */
   char * name;                  /** <= name of the device */
} ciaaI2CDevices_slaveType;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAI2CDEVICES_INTERNAL_H */


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

#ifndef _CIAAMODBUS_H_
#define _CIAAMODBUS_H_
/** \brief Modbus Slave Header File
 **
 ** This files shall be included by moodules using the interfaces provided by
 ** the Modbus Slave
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Modbus CIAA Modbus
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * GMuro        Gustavo Muro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140623 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaModbus_transport.h"
#include "ciaaModbus_protocol.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/** \brief Modbus Master callback end of communication
 **
 ** This function is called by the modbus master when communication ends
 **
 ** \param[in] slaveId Identification of Slave Modbus
 ** \param[in] function excecuted
 ** \param[in] exception code
 ** \return
 **/
typedef void (*modbusMaster_cbEndOfComm)(
      uint8_t slaveId,
      uint8_t numFunc,
      uint8_t exceptioncode);
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief Open Modbus Master
 **
 ** \return handler of Modbus Slave
 **/
extern int32_t ciaaModbus_masterOpen(void);

/** \brief Close Modbus Master
 **
 ** \param[in] handler of Modbus Master
 ** \return -1 if failed, 0 in other if success.
 **/
extern int32_t ciaaModbus_masterClose(
      int32_t hModbusMaster);

/** \brief Close Modbus Master
 **
 ** \param[in] handler of Modbus Master
 ** \return -1 if failed, 0 in other if success.
 **/
extern int32_t ciaaModbus_masterCmd0x03ReadHoldingReg(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      int16_t *hrValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfComm cbEndComm);



/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUS_H_ */


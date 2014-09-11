/* Copyright 2014, Gustavo Muro
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

#ifndef _CIAAMODBUSGATEWAY_H_
#define _CIAAMODBUSGATEWAY_H_
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

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief Init Modbus Gateway
 **
 ** \param[in] totalMasters Number of masters
 ** \param[in] totalSlaves Number of slaves
 ** \return handler Modbus Gateway
 **/
extern int32_t ciaaModbus_gatewayInit(
      uint8_t totalMasters,
      uint8_t totalSlaves);

/** \brief Add slave to Modbus Gateway
 **
 ** \param[in] hModbusSlave handler slave
 ** \param[in] id of slave modbus
 ** \return 0 if ok
 **         -1 if error occurs
 **/
extern int8_t ciaaModbus_gatewayAddSlave(
      int32_t hModbusSlave,
      uint8_t idSlave);

/** \brief Add master to Modbus Gateway
 **
 ** \param[in] hModbusMaster handler Master
 ** \return 0 if ok
 **         -1 if error occurs
 **/
extern int8_t ciaaModbus_gatewayAddMaster(
      int32_t hModbusMaster);

/** \brief Execute task of gateway
 **
 ** \param[in] hModbusGatewai handler Gateway
 **/
extern void ciaaModbus_gatewayMainTask(
      int32_t hModbusGateway);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUSGATEWAY_H_ */


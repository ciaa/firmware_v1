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

#ifndef _CIAAMODBUSTRANSPORT_H_
#define _CIAAMODBUSTRANSPORT_H_
/** \brief Modbus Header File
 **
 ** This files shall be included by moodules using the interfaces provided by
 ** the Modbus
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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150604 v0.8.1 GMuro   replace numbers for macros
 * 20141108 v0.0.1 GMuro   initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaModbus.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/** \brief Min lenght of a modbus request pdu */
#define CIAAMODBUS_REQ_PDU_MINLENGTH            0x01

/** \brief Min lenght of a modbus response pdu */
#define CIAAMODBUS_RSP_PDU_MINLENGTH            0x02

/** \brief Min lenght of a modbus exception response pdu */
#define CIAAMODBUS_EXCEP_RSP_PDU_MINLENGTH      0x02

/** \brief Trasnsport type Master */
#define CIAAMODBUS_TRANSPORT_TYPE_MASTER        1

/** \brief Trasnsport type Salve */
#define CIAAMODBUS_TRANSPORT_TYPE_SLAVE         0

/** \brief Trasnsport type Invalid */
#define CIAAMODBUS_TRANSPORT_TYPE_INVALID       -1


/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief ciaaModbus_transport initialization
 **
 ** Performs the initialization of the MODBUS Transport
 **
 **/
extern void ciaaModbus_transportInit(void);

/** \brief CIAA Modbus task transport
 **
 ** This function perform task of transport module:
 **
 ** \param[in] handler handler to perform task
 ** \return
 **/
extern void ciaaModbus_transportTask(int32_t handler);

/** \brief Receive modbus message
 **
 ** This function receive a message
 **
 ** \param[in] handler handler in to recv msg
 ** \param[out] id identification number of modbus message
 ** \param[out] pdu buffer with stored pdu
 ** \param[out] size size of pdu. If no valid message received
 **             size must be less than 5
 ** \return
 **/
extern void ciaaModbus_transportRecvMsg(
      int32_t handler,
      uint8_t *id,
      uint8_t *pdu,
      uint32_t *size);

/** \brief Send modbus message
 **
 ** This function send a message
 **
 ** \param[in] handler handler to send msg
 ** \param[in] id identification number of modbus message
 ** \param[in] pdu buffer with stored pdu
 ** \param[in] size size of pdu
 ** \return
 **/
void ciaaModbus_transportSendMsg(
      int32_t handler,
      uint8_t id,
      uint8_t *pdu,
      uint32_t size);

/** \brief Get transport type
 **
 ** This function indicate the type of transport (Master or Slave)
 **
 ** \param[in] handler handler of transport
 ** \return    CIAAMODBUS_TRANSPORT_TYPE_MASTER  -> master
 **            CIAAMODBUS_TRANSPORT_TYPE_SLAVE   -> slave
 **            CIAAMODBUS_TRANSPORT_TYPE_INVALID -> invalid transport or
 **            not initialized
 **/
extern int8_t ciaaModbus_transportGetType(int32_t handler);

/** \brief Set response timeout
 **
 ** This function set response timeout in milliseconds
 **
 ** \param[in] handler handler in to module
 ** \return
 **/
extern void ciaaModbus_transportSetRespTimeout(int32_t handler, uint32_t timeout);

/** \brief Get response timeout
 **
 ** This function return response timeout in milliseconds
 **
 ** \param[in] handler handler in to module
 ** \return response timeout (milliseconds)
 **/
extern uint32_t ciaaModbus_transportGetRespTimeout(int32_t handler);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUSTRANSPORT_H_ */


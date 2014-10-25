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
 * MaCe         Mariano Cerdeiro
 * GMuro        Gustavo Muro
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

/** \brief PDU is not received */
#define CIAAMODBUS_TRANSPORT_RECV_NONE         -1
/** \brief Processing incoming PDU */
#define CIAAMODBUS_TRANSPORT_RECV_PROCCESING    0
/** \brief PDU received */
#define CIAAMODBUS_TRASNPORT_RECV_COMPLETE      1

/** \brief an error occurred while sending PDU */
#define CIAAMODBUS_TRANSPORT_SEND_ERROR        -1
/** \brief send in process */
#define CIAAMODBUS_TRANSPORT_SEND_PROCESS       0
/** \brief PDU send complete */
#define CIAAMODBUS_TRANSPORT_SEND_COMPLETE      1



/** \brief Min lenght of a modbus pdu */
#define CIAAMODBUS_MSG_MINLENGTH          0x05

/*==================[typedef]================================================*/
typedef enum
{
   CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER = 0,
   CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE,
   CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER,
   CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE,
   CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER,
   CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE,
}ciaaModbus_transportModeEnum;


/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief ciaaModbus_transport initialization
 **
 ** Performs the initialization of the MODBUS Transport
 **
 **/
extern void ciaaModbus_transportInit(void);

/** \brief Open Modbus Transport
 **
 ** This function initialize Modbus Transport with device indicate in fildes
 ** and selected mode. Also reserves the buffer for reception and
 ** transmission.
 **
 ** \param[in] fildes File Descriptor to write and read data
 ** \param[in] mode mode may take one of the following values:
 **            CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER
 **            CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE
 **            CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER
 **            CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE
 **            CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER
 **            CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE
 ** \return handler of Modbus Transport
 **/
extern int32_t ciaaModbus_transportOpen(
      int32_t fildes,
      ciaaModbus_transportModeEnum mode);

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

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUSTRANSPORT_H_ */


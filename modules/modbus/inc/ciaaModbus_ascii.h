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

#ifndef _CIAAMODBUS_ASCII_H_
#define _CIAAMODBUS_ASCII_H_
/** \brief Modbus ASCII Header File
 **
 ** This files shall be included by moodules using the itnerfaces provided by
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
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150604 v0.8.1 GMuro   replace numbers for macros
 * 20141108 v0.0.2 GMuro   New Modbus implementation
 * 20140623 v0.0.1 MaCe    first functional version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief Modbus ASCII Start Addres ":" */
#define CIAAMODBUS_ASCII_START      0x3A

/** \brief Modbus ASCII 0 */
#define CIAAMODBUS_ASCII_0          0x30

/** \brief Modbus ASCII 9 */
#define CIAAMODBUS_ASCII_9          0x39

/** \brief Modbus ASCII A */
#define CIAAMODBUS_ASCII_A          0x41

/** \brief Modbus ASCII F */
#define CIAAMODBUS_ASCII_F          0x46

/*
 * :AAFF0011..DDCCEE
 * |\|\|\|\|\|\|\|\|
 * | | | | | | | | |
 * | | | | | | | | +-- 2 bytes: CRLF end of the modbus ascii (0x0D, 0x0A)
 * | | | | | | | |
 * | | | | | | | +-- 2 bytes: LRC Check
 * | | | | | | |
 * | | | +-+-+-+-- n times 2 bytes: data
 * | | |
 * | | +-- 2 bytes: function
 * | |
 * | +-- 2 bytes: addres
 * |
 * +-- 1 byte: start delimiter ascii : (0x3A)
 */

/** \brief Maximal length of a ascii modbus message */
#define CIAAMODBUS_ASCII_MAXLENGHT  512

/** \brief Minimal length of a ascii modbus message */
#define CIAAMODBUS_ASCII_MINLENGHT  9

/** \brief */
#define CIAAMODBUS_ASCII_END_1      0x0D

/** \brief */
#define CIAAMODBUS_ASCII_END_2      0x0A

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaModbus_ascii initialization
 **
 ** Performs the initialization of the MODBUS ASCII
 **
 **/
extern void ciaaModbus_asciiInit(void);

/** \brief Init Modbus Ascii communication channel
 **
 ** \paran[in] fildes file descriptor serial port
 ** \return -1 if error
 **         >= 0 handler modbus
 **/
extern int32_t ciaaModbus_asciiOpen(int32_t fildes);

/** \brief CIAA Modbus ASCII task
 **
 ** This function perform task of modbus ascii:
 **
 ** \param[in] handler handler to perform task
 ** \return
 **/
extern void ciaaModbus_asciiTask(int32_t handler);

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
extern void ciaaModbus_asciiRecvMsg(
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
void ciaaModbus_asciiSendMsg(
      int32_t handler,
      uint8_t id,
      uint8_t *pdu,
      uint32_t size);


/** \brief Convert received ascii data to bin
**
** \param[inout] buf input ascii buffer starting with : and without CRLF
** and output bin buffer
** \para[in] len length of the ascii buffer
** \return -1 if error, lenght of the binary data
**
** \remarks This function shall not be called from outside this file.
** Is not static due to the tests.
**/
extern int32_t ciaaModbus_ascii_ascii2bin(uint8_t * buf, int32_t len);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUS_ASCII_H_ */


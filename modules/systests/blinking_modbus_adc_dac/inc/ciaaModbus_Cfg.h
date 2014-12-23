/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Gustavo Muro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
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

#ifndef _CIAAMODBUS_CFG_H_
#define _CIAAMODBUS_CFG_H_
/** \brief Modbus Header Configuration File
 **
 ** This files configures the ciaamodbus
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
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140713 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define CIAAMODBUS_EN            1

#define CIAAMODBUS_DIS           0

/** \brief Id of the own slave */
#define CIAAMODBUS_SLAVE_ID      2

/** \brief Enables/Disables service 0x03 Read Holding Registers
 **/
#define CIAAMODBUS_READ_HOLDING_REGISTERS       CIAAMODBUS_EN

/** \brief Enables/Disables service 0x04 Read Input Registers
 **/
#define CIAAMODBUS_READ_INPUT_REGISTERS         CIAAMODBUS_EN

/** \brief Enables/Disables service 0x06 Write Single Register
 **/
#define CIAAMODBUS_WRITE_SINGLE_REGISTER        CIAAMODBUS_EN

/** \brief Enables/Disables service 0x10 Write Multiple Registers
 **/
#define CIAAMODBUS_WRITE_MULTIPLE_REGISTERS     CIAAMODBUS_EN

/*==================[typedef]================================================*/
/** \brief Modbus Command Read Holding Registers callback
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Read Holding Registers command
 **
 ** \param[in] startingAddress starting address
 ** \param[in] quantityOfHoldingRegisters quantity of holding registers to
 **            be read
 ** \param[out] exceptionCode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[out] buf buffer containing the holding registers
 ** \return count of registers, this value is multiplicated * 2 by the caller
 **         if a exception occurs returns -1
 **
 ** \remarks exceptionCode parameter shall only be used if -1 is returned in
 **          other case the pointer shall not be written.
 **/
typedef int8_t (*ciaaModbus_readHoldingRegistersFctType)(
      uint16_t startingAddress,
      uint16_t quantityOfHoldingRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      );

/** \brief Modbus Command Read Input Registers callback
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Read Input Registers command
 **
 ** \param[in] startingAddress starting address
 ** \param[in] quantityOfInputRegisters quantity of input registers to be read
 ** \param[out] exceptionCode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[out] buf buffer containing the input registers
 ** \return count of registers, this value is multiplicated * 2 by the caller
 **         if a exception occurs returns -1
 **
 ** \remarks exceptionCode parameter shall only be used if -1 is returned in
 **          other case the pointer shall not be written.
 **/
typedef int8_t (*ciaaModbus_readInputRegistersFctType)(
      uint16_t startingAddress,
      uint16_t quantityOfInputRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      );

/** \brief Modbus Command Write Single Register callback
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Write Single Register command
 **
 ** \param[in] registerAddress address of the register to be written
 ** \param[in] registerValue value to be written to the register
 ** \param[out] exceptionCode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \return -1 if error, exceptionCode shall be set. 1 if success.
 **
 ** \remarks exceptionCode parameter shall only be used if -1 is returned in
 **          other case the pointer shall not be written.
 **/
typedef int8_t (*ciaaModbus_writeSingleRegisterFctType)(
      uint16_t registerAddress,
      uint16_t registerValue,
      uint8_t * exceptionCode
      );

/** \brief Modbus Command Write Multiple Registers callback
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Write Multiple Registers command
 **
 ** \param[in] startingAddress starting address
 ** \param[in] quantityOfRegisters quantity of registers to be read
 ** \param[out] exceptionCode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[in] buf buffer containing the registers
 ** \return -1 if error, exceptionCode shall be set. 1 if success.
 **
 ** \remarks exceptionCode parameter shall only be used if -1 is returned in
 **          other case the pointer shall not be written.
 **/
typedef int8_t (*ciaaModbus_writeMultipleRegistersFctType)(
      uint16_t startingAddress,
      uint16_t quantityOfRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      );


/** \brief Address range
 **/
typedef struct {
   uint16_t minAdd;
   uint16_t maxAdd;
} ciaaModbus_addressRangeType;

/** \brief Command list type */
typedef struct {
   ciaaModbus_addressRangeType range;
   ciaaModbus_readHoldingRegistersFctType fct;
} ciaaModbus_cmdLst0x03Type;

/** \brief Command list type */
typedef struct {
   ciaaModbus_addressRangeType range;
   ciaaModbus_readInputRegistersFctType fct;
} ciaaModbus_cmdLst0x04Type;

/** \brief Command list type */
typedef struct {
   ciaaModbus_addressRangeType range;
   ciaaModbus_writeSingleRegisterFctType fct;
} ciaaModbus_cmdLst0x06Type;

/** \brief Command list type */
typedef struct {
   ciaaModbus_addressRangeType range;
   ciaaModbus_writeMultipleRegistersFctType fct;
} ciaaModbus_cmdLst0x10Type;

/*==================[external data declaration]==============================*/
extern ciaaModbus_cmdLst0x03Type ciaaModbus_cmdLst0x03[];
extern ciaaModbus_cmdLst0x04Type ciaaModbus_cmdLst0x04[];
extern ciaaModbus_cmdLst0x06Type ciaaModbus_cmdLst0x06[];
extern ciaaModbus_cmdLst0x10Type ciaaModbus_cmdLst0x10[];

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUS_CFG_H_ */


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

#ifndef _CIAAMODBUS_H_
#define _CIAAMODBUS_H_
/** \brief Modbus Header File
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
 * Gmuro        Gustavo Muro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141113 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbus_config.h"
#include "ciaaPOSIX_stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/****** error codes ******/
#define CIAA_MODBUS_E_FNC_NOT_SUPPORTED            0x01
#define CIAA_MODBUS_E_WRONG_STR_ADDR               0x02
#define CIAA_MODBUS_E_WRONG_REG_QTY                0x03
#define CIAA_MODBUS_E_FNC_ERROR                    0x04

/****** functions ******/
#define CIAA_MODBUS_FCN_READ_COILS                 0x01
#define CIAA_MODBUS_FCN_READ_DISCRETE_INPUTS       0x02
#define CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS     0x03
#define CIAA_MODBUS_FCN_READ_INPUT_REGISTERS       0x04
#define CIAA_MODBUS_FCN_WRITE_SINGLE_COIL          0x05
#define CIAA_MODBUS_FCN_WRITE_SINGLE_REGISTER      0x06
#define CIAA_MODBUS_FCN_WRITE_MULTIPLE_COILS       0x0F
#define CIAA_MODBUS_FCN_WRITE_MULTIPLE_REGISTERS   0x10

#define CIAA_MODBUS_SLAVE_MIN_ID_VALUE             1
#define CIAA_MODBUS_SLAVE_MAX_ID_VALUE             247

/** \brief Modbus slave types */
#if CIAA_MODBUS_TOTAL_SLAVES > 0
/** \brief Modbus Command multiple read
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Multiple Read command:
 ** 0x01 Read Coils
 ** 0x02 Read Discrete Inputs
 ** 0x03 Read Holding Registers
 ** 0x04 Read Input Registers
 **
 ** \param[in] start starting address
 ** \param[in] quantity quantity of coils/inputs/holding registers to be read
 ** \param[out] exceptioncode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[out] buf buffer containing the coils/inputs/holding registers
 ** \return count of coils/inputs/holding registers
 **/
typedef uint16_t (*modbusSlave_processFuncMultipleRead)(
      uint16_t start,
      uint16_t quantity,
      uint8_t * exceptioncode,
      uint8_t * buf
      );

/** \brief Modbus Command single write
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Single Write command:
 ** 0x05 Write Single Coil
 ** 0x06 Write Single Register
 **
 ** \param[in] start starting address
 ** \param[out] exceptioncode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[in] buf buffer containing the coils/inputs/holding registers
 ** \return
 **/
typedef void (*modbusSlave_processFuncSingleWrite)(
      uint16_t start,
      uint8_t * exceptioncode,
      uint8_t * buf
      );

/** \brief Modbus Command Multiple write
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Multiple Write command:
 ** 0x0F Write Multiple Coils
 ** 0x10 Write Multiple Register
 **
 ** \param[in] start starting address
 ** \param[in] quantity quantity of coils/inputs/holding registers to be read
 ** \param[out] exceptioncode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[in] buf buffer containing the coils/registers
 ** \return
 **/
typedef void (*modbusSlave_processFuncMultipleWrite)(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      );

/** \brief Modbus Command Multiple read write
 **
 ** This function is called by the modbus and provided by the application which
 ** implements a Multiple read write command:
 ** 0x17 Read/Write Multiple registers
 **
 ** \param[in] readStart read starting address
 ** \param[in] readQuantity quantity of holding registers to be read
 ** \param[in] writeStart write starting address
 ** \param[in] writeQuantity quantity of holding registers to be write
 ** \param[in] writeByteCount quantity of bytes to be write
 ** \param[out] exceptioncode may take one of the following values:
 **                  CIAAMODBUS_E_WRONG_STR_ADDR
 **                  CIAAMODBUS_E_FNC_ERROR
 ** \param[in] buf buffer containing the registers
 ** \return
 **/
typedef void (*modbusSlave_processFuncMultipleReadWrite)(
      uint16_t readStart,
      uint16_t readQuantity,
      uint16_t writeStart,
      uint16_t writeQuantity,
      uint8_t writeByteCount,
      uint8_t * exceptioncode,
      uint8_t * buf
      );

/** \brief Command callback type */
typedef struct
{
   modbusSlave_processFuncMultipleRead       cmd0x01ReadCoils;
   modbusSlave_processFuncMultipleRead       cmd0x02ReadDiscrteInputs;
   modbusSlave_processFuncMultipleRead       cmd0x03ReadHoldingReg;
   modbusSlave_processFuncMultipleRead       cmd0x04ReadInputReg;

   modbusSlave_processFuncSingleWrite        cmd0x05WriteSingleCoil;
   modbusSlave_processFuncSingleWrite        cmd0x06WriteSingleRegister;

   modbusSlave_processFuncMultipleWrite      cmd0x0FWriteMultipleCoils;
   modbusSlave_processFuncMultipleWrite      cmd0x10WriteMultipleReg;
   modbusSlave_processFuncMultipleReadWrite  cmd0x17ReadWriteMultipleReg;

}ciaaModbus_slaveCmd_type;
#endif   /* end Modbus slave types */

/** \brief Modbus Transport types */
#if ( (CIAA_MODBUS_TOTAL_TRANSPORT_ASCII + CIAA_MODBUS_TOTAL_TRANSPORT_RTU + \
       CIAA_MODBUS_TOTAL_TRANSPORT_TCP ) > 0 )

typedef enum
{
   CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER = 0,
   CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE,
   CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER,
   CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE,
   CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER,
   CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE,
}ciaaModbus_transportModeEnum;

#endif   /* end Modbus Transport types */

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief ciaaModbus initialization
 **
 ** Performs the initialization of the MODBUS components
 **
 **/
extern void ciaaModbus_init(void);

/** \brief Modbus slave interfaces */
#if CIAA_MODBUS_TOTAL_SLAVES > 0

/** \brief Open Modbus Slave
 **
 ** \param[in] cmd pointer to struct call backs modbus function
 ** \param[in] id identification of slave
 ** \return handler of Modbus Slave
 **/
extern int32_t ciaaModbus_slaveOpen(
      const ciaaModbus_slaveCmd_type *cmd,
      uint8_t id);

/** \brief Close Modbus Slave
 **
 ** \param[in] handler of Modbus Slave
 ** \return -1 if failed, 0 in other if success.
 **/
extern int32_t ciaaModbus_slaveClose(
      int32_t hModbusSlave);

#endif   /* end Modbus slave interfaces */

/** \brief Modbus gateway interfaces */
#if CIAA_MODBUS_TOTAL_GATEWAY > 0
/** \brief Open Modbus Gateway
 **
 ** \return handler Modbus Gateway
 **/
extern int32_t ciaaModbus_gatewayOpen(void);

/** \brief Add slave to Modbus Gateway
 **
 ** \param[in] hModbusGW handler Modbus Gateway
 ** \param[in] hModbusSlave handler slave
 ** \return 0 if ok
 **         -1 if error occurs
 **/
extern int8_t ciaaModbus_gatewayAddSlave(
      int32_t hModbusGW,
      int32_t hModbusSlave);

/** \brief Add master to Modbus Gateway
 **
 ** \param[in] hModbusGW handler Modbus Gateway
 ** \param[in] hModbusMaster handler Master
 ** \return 0 if ok
 **         -1 if error occurs
 **/
extern int8_t ciaaModbus_gatewayAddMaster(
      int32_t hModbusGW,
      int32_t hModbusMaster);

/** \brief Add transport to Modbus Gateway
 **
 ** \param[in] hModbusGW handler Modbus Gateway
 ** \param[in] hModbusTransport handler Transport
 ** \return 0 if ok
 **         -1 if error occurs
 **/
extern int8_t ciaaModbus_gatewayAddTransport(
      int32_t hModbusGW,
      int32_t hModbusTransport);

/** \brief Execute task of gateway
 **
 ** \param[in] hModbusGW handler Gateway
 **/
extern void ciaaModbus_gatewayMainTask(
      int32_t hModbusGW);

#endif   /* end Modbus gateway interfaces */

/** \brief Modbus Transport interfaces */
#if ( (CIAA_MODBUS_TOTAL_TRANSPORT_ASCII + CIAA_MODBUS_TOTAL_TRANSPORT_RTU + \
       CIAA_MODBUS_TOTAL_TRANSPORT_TCP ) > 0 )

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

#endif   /* end Modbus Transport interfaces */

/** \brief Read Integer from modbus
 **
 ** As described in modbus specification, the modbus uses a bigendian format to
 ** transmit integers. This function shall be used to access integers.
 **
 ** \param[in] add address of the first byte of the integer to be read.
 **
 **/
#define ciaaModbus_readInt(add) \
   ( (((uint8_t*)(add))[0] << 8) | (((uint8_t*)(add))[1]))


/** \brief Write Integer in modbus buffer
 **
 ** As described in modbus specification, the modbus uses a bigendian format to
 ** transmit integers. This function shall be used to access integers.
 **
 ** \param[in] add address of the first byte of the integer to be write.
 ** \param[in] num integer to be write.
 **
 **/
#define ciaaModbus_writeInt(add,num) \
   do {                                                        \
      (((uint8_t*)(add))[0] = ((uint16_t)(num) >> 8) & 0XFF);  \
      (((uint8_t*)(add))[1] = ((uint16_t)(num) >> 0) & 0XFF);  \
   }while (0)


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAAMODBUS_H_ */


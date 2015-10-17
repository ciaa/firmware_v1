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
/** \brief Bus State IDLE */
#define CIAA_I2C_ST_IDLE                  0
/** \brief Bus State Write Slave address during write command */
#define CIAA_I2C_WRITE_SLVADD             1
/** \brief Bus Sate Write address during write command */
#define CIAA_I2C_WRITE_ADD                2
/** \brief Bus State Write data */
#define CIAA_I2C_WRITE_DATA               3
/** \brief Bus State Write Slave address during read command */
#define CIAA_I2C_READ_SLVADD              4
/** \brief Bus Sate Write address during read command */
#define CIAA_I2C_READ_ADD                 5
/** \brief Bus State Read data */
#define CIAA_I2C_READ_DATA                6

/** \brief Device is idle */
#define CIAA_I2C_DEV_IDLE                 0
/** \brief Device operation is pending */
#define CIAA_I2C_DEV_PENDING              1
/** \brief Device communication ongoing */
#define CIAA_I2C_DEV_COM                  2

/** \brief Indicates that in this bus the system is a master */
#define CIAA_I2C_BUSTYPE_MASTER           0

/** \brief Indicates that in this bus the system is a slave
 **
 ** \remarks This feature is not yet supported
 **/
#define CIAA_I2C_BUSTYPE_SLAVE            1

/** \brief Indicates that in this bus the system is a master and a slave
 **
 ** This is the case of a multimaster system.
 **
 ** \remarks This feature is not yet supported.
 **/
#define CIAA_I2C_BUSTYPE_MASTERSLAVE      2

#define CIAA_I2C_10_BITS_ADDRESSMASK      0x8000

/** \brief Sets the slave address id
 **
 ** This macro shall be used to correctly set the values of the type
 ** ciaaI2CDevices_slaveAddressType.
 **
 ** \param[in] address shall be on the range 8 and 119 for 7 bits addressing
 **            withing the range 0 and 1023 for 10 bits addressing. 10 bits
 **            addressing is only allowed if CIAA_POSIX_I2C_10_BITS_ADDRESS is
 **            set to CIAA_ENABLE.
 ** \param[in] ext if true the address is a 10 bits address, if false a 7 bits
 **            address.
 **/
#define CIAA_I2C_SETSLAVEADDRESS(add,ext) \
   CIAA_I2C_SETSLAVEADDRESS_ ## ext(add)

/** \brief Sub macros of CIAA_I2C_SETSLAVEADDRESS
 **
 ** \remarks this are internal macros, do not use them.
 **/
#if (CIAA_I2C_10_BITS_ADDRESS == CIAA_ENABLE)
#define CIAA_I2C_SETSLAVEADDRESS_true(add)   \
   ((((add) & 0x3FF) << 1) | CIAA_I2C_10_BITS_ADDRESSMASK)
#else
#define CIAA_I2C_SETSLAVEADDRESS_true(add)   \

//#error 10 bits address mode is not enable.
#endif
#define CIAA_I2C_SETSLAVEADDRESS_false(add)  \
   (((add) & 0x7F) << 1)

/*==================[typedef]================================================*/
/** \brief Slave Address
 **
 ** Type to represent the slave address, it can be
 ** 8 bits for 7 bits address mode and 16 bits for 10 bits
 ** address mode depending on the configuration parameter
 ** CIAA_I2C_10_BITS_ADDRESS.
 **
 ** \remarks to set this value use the macro CIAA_I2C_SETSLAVEADDRESS.
 **
 ** \remarks in 10 bits addressing the bit 15 indicates with a 1 a 10 bit
 **          address and with 0 a 7 bit address. In both address modes
 **          the bit 0 is not part of the address but used to indicate
 **          a read or write operation.
 **
 **/
#if (CIAA_I2C_10_BITS_ADDRESS == CIAA_ENABLE)
typedef uint16_t ciaaI2CDevices_slaveAddressType;
#else
typedef uint8_t ciaaI2CDevices_slaveAddressType;
#endif

/** \brief Count of slaves
 **
 ** Type used to indicate the count of slaves on this bus.
 ** It may be 8 or 16 bits depending on CIAA_I2C_10_BITS_ADDRESS
 ** configuration parameter.
 **
 **/
#if (CIAA_I2C_10_BITS_ADDRESS == CIAA_ENABLE)
typedef uint16_t ciaaI2CDevices_countOfSlavesType;
#else
typedef uint8_t ciaaI2CDevices_countOfSlavesType;
#endif

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
   ciaaI2CDevices_slaveAddressType
      slaveAddress;              /** <= id of the I2C device, allowed values are
                                        between 0 and 127 inclusive for 7 bits */
   uint8_t addWidth;             /** <= size in bytes of the address length of
                                        this device, if 0 the address shall be
                                        handled by the user */
   char * name;                  /** <= name of the device */
} ciaaI2CDevices_slaveType;

/** \brief Master Bus Type
 **
 ** Type used to describe a Master Bus.
 **
 **/
typedef struct {
   ciaaI2CDevices_slaveType *
      slaves;           /** <= pointer to countOfSlaves slaves */
   ciaaI2CDevices_countOfSlavesType
      countOfSlaves;    /** <= count of slaves */
   char * driverName;   /** <= driver name */
} ciaaI2CDevices_masterBusType;

typedef struct {
} ciaaI2CDevices_slaveBusType;

typedef union {
   ciaaI2CDevices_slaveBusType slave;
   ciaaI2CDevices_masterBusType master;
} ciaaI2CDevices_unionBusType;

typedef struct {
   ciaaI2CDevices_unionBusType bus;
   uint8_t busType;
} ciaaI2CDevices_busType;


typedef struct {
   ciaaI2CDevices_busType * buses;
   uint8_t countOfBuses;
} ciaaI2CDevices_busesType;

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


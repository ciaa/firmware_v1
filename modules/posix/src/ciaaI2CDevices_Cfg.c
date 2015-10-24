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

/** \brief Configuration file for I2C
 **
 ** This file contains the configuration of the I2C Devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup I2C I2C
 ** @{ */
/** \addtogroup I2C_CFG I2C Configuration
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151004 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaI2CDevices.h"
#include "ciaaI2CDevices_Cfg.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
/** \brief Array of slaves
 **
 ** Add one struct of the type ciaaI2CDevices_slaveType for each
 ** I2C master bus to describe their slaves. You can choose the name you
 ** like for this structure as far as you reference it.
 **
 ** Each slaves contains:
 **
 ** maxAddr       -> last supported addres. e.g.: 255 will indicate that the
 **                  address range from 0 to 255 inclusive is supported. Gaps
 **                  are not supported.
 ** id            -> id of this device. Allways use the macro
 **                  CIAA_I2C_SETSLAVEADDRESS to set the addresses.
 ** addWidth      -> size in bytes of the address.
 ** deviceName    -> name of the device. e.g.: eeprom. In this case the path
 **                  to open the device will be /dev/i2c/0/eeprom
 **
 **/
ciaaI2CDevices_slaveType ciaaI2CDevices_masterBus0Slaves[2] = {
   /* configuration of first device */
   {
      255,              /* max addres, addresse range from 0 to 255 */
      CIAA_I2C_SETSLAVEADDRESS(20, false),
                        /* id of the device */
      2,                /* 2 bytes for address */
      "eeprom"          /* name of the device */
   },
   /* configuration of first device */
   {
      15,               /* max addres, addresse range from 0 to 511 */
      CIAA_I2C_SETSLAVEADDRESS(10, false),
                        /* id of the device */
      1,                /* 1 bytes for address */
      "sensor"          /* name of the device */
   }
};

/** \brief Array of buses.
 **
 ** Describes the n I2C buses. n (the count of elements on the array) shall
 ** be the same count as defined in ciaaI2CDevices_Cfg.
 **
 ** Each element is a struct of two elements where the first one is an union.
 **
 ** This strcture contains the following elements:
 **
 ** bus        -> describes the bus, this element is an union and can be of
 **               the type master or slave.
 ** busType    -> shall indicate with CIAA_I2C_BUSTYPE_MASTER if the bus is
 **               fullfiled as master, CIAA_I2C_BUSTYPE_SLAVE if the bus is
 **               fullfiled as slave.
 **
 ** *** MASTER ***
 **
 ** if you want to describe a master the elements are:
 **
 ** slaves     -> array describing the slaves.
 ** count      -> count of slaves on the slave array.
 ** drvier     -> name of the driver.
 **
 ** *** SLAVE ***
 **
 ** TODO: this feature is not yet supported.
 **/
ciaaI2CDevices_busType ciaaI2CDevices_buses[] = {
   {
      .bus.master = {
         ciaaI2CDevices_masterBus0Slaves,
                           /* reference to the slaves on this bus */
         2,                /* count of devices in this bus */
         "/dev/i2c/0",     /* name of the driver */
      },
      .busType = CIAA_I2C_BUSTYPE_MASTER
   }
};

/** \brief This structure refers to the list all I2C buses.
 **
 ** This structure contains:
 **
 ** buses         -> pointer to the array of busses availables.
 ** count         -> count of buses described on the array.
 **
 ** \remarks Do not rename this variable, will be passed during the initialization
 **          of the ciaa I2C Device.
 **/
const ciaaI2CDevices_busesType ciaaI2CDevices_Cfg = {
   ciaaI2CDevices_buses,
   1
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


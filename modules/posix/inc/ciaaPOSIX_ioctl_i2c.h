/* Copyright 2015, Sebastian Bedin para SUR Emprendimientos Tecnologicos S.R.L. / CONAE
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

#ifndef CIAAPOSIX_IOCTL_BUSI2C
#define CIAAPOSIX_IOCTL_BUSI2C
/** \brief IO Control macros for serial devices
 **
 ** This files contains the macros for IO control for serial devices
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup POSIX
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * SeBe         Sebastian Bedin
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150615 v0.0.1 SeBe initial version
 */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/** \brief set clockrate for i2c devices
 **
 ** This ioctl command is used to set the baudrate for any i2c device.
 ** Possible values for arg are:
 **   ciaaPOSIX_IOCTL_CLOCKRATE_100000
 **   ciaaPOSIX_IOCTL_CLOCKRATE_400000
 **
 ** Returned value for ioctl is the actual baud rate, or 0 if no rate can be found.
 **/
#define ciaaPOSIX_IOCTL_SET_CLOCKRATE                (51)

/** \brief clockrate macros for i2c devices
 **/
#define ciaaPOSIX_IOCTL_CLOCKRATE_100000             ( 100000U )
#define ciaaPOSIX_IOCTL_CLOCKRATE_400000             ( 400000U )

/** \brief I2C slave device address setting
 **
 ** Returned value for ioctl is the actual slave device address, or 0 if no rate can be found.
 **/
#define ciaaPOSIX_IOCTL_SET_SLAVEADD                 (54)

/** \brief I2C slave device address width setting
 **
 ** This ioctl command is used to set the address width for any I2C device.
 ** Possible values for arg are:
 **   ciaaPOSIX_IOCTL_SET_SLAVEADDWIDTH_7bits
 **
 ** Returned value for ioctl is the actual address width, or 0 if no rate can be found.
 **/
#define ciaaPOSIX_IOCTL_SET_SLAVEADDWIDTH            (52)

/** \brief I2C slave device address width macros for I2C devices
 **/
#define ciaaPOSIX_IOCTL_SET_SLAVEADDWIDTH_7bits      (7)

/** \brief I2C slave device register address setting
 **
 ** Returned value for ioctl is the actual slave device address, or 0 if no rate can be found.
 **/
#define ciaaPOSIX_IOCTL_SET_REGISTERADD              (55)

/** \brief I2C slave device register address width setting
 **
 ** This ioctl command is used to set the width of the register address for any I2C device.
 ** Possible values for arg are:
 **   ciaaPOSIX_IOCTL_REGISTERADDWIDTH_0bits
 **   ciaaPOSIX_IOCTL_REGISTERADDWIDTH_8bits
 **   ciaaPOSIX_IOCTL_REGISTERADDWIDTH_16bits
 **   ciaaPOSIX_IOCTL_REGISTERADDWIDTH_24bits
 **   ciaaPOSIX_IOCTL_REGISTERADDWIDTH_32bits
 **
 ** Returned value for ioctl is the actual register address, or 0 if no rate can be found.
 **/
#define ciaaPOSIX_IOCTL_SET_REGISTERADDWIDTH         (56)

/** \brief I2C slave device register address width macros for I2C devices
 **/
#define ciaaPOSIX_IOCTL_REGISTERADDWIDTH_0bits       (0)
#define ciaaPOSIX_IOCTL_REGISTERADDWIDTH_8bits       (1)
#define ciaaPOSIX_IOCTL_REGISTERADDWIDTH_16bits      (2)
#define ciaaPOSIX_IOCTL_REGISTERADDWIDTH_24bits      (3)
#define ciaaPOSIX_IOCTL_REGISTERADDWIDTH_32bits      (4)

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAAPOSIX_IOCTL_BUSI2C */


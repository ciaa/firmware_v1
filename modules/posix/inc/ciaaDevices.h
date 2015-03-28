/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

#ifndef CIAADEVICES_H
#define CIAADEVICES_H
/** \brief CIAA Devices header file
 **
 ** This header file describes the Devices.
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
 * 20140422 v0.0.1 MaCe rework initial file
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stddef.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define ciaaDevices_MAXDEVICES      20

/** \brief the file offset shall be set to offset bytes */
/*@-namechecks@*/
#define SEEK_SET                    0

/** \brief the file offset shall be set to its current location plus offset */
#define SEEK_CUR                    1

/** \brief the file offset shall be set to the size of the file plus offset */
#define SEEK_END                    2
/*@=namechecks@*/

/*==================[typedef]================================================*/
typedef struct ciaaDevices_deviceStruct ciaaDevices_deviceType;

/** \brief open a device
 **
 ** Open and if needed perform the initialization of the indicated device
 **
 ** \param[in] path path of the device to be opened
 ** \param[in] device device to be opened
 ** \param[in] oflag may take one of the following values:
 **               O_RDONLY: opens files to read only
 **               O_WRONLY: opens files to write only
 **               O_RDWR: opens file to read and write
 ** \return NULL if an error occurs, in other case the address of the opened
 **         device.
 **/
typedef ciaaDevices_deviceType * (*ciaaDevices_open)(char const * path,
      ciaaDevices_deviceType * device,
      uint8_t const oflag);

/** \brief close function type */
typedef int32_t (*ciaaDevices_close)(ciaaDevices_deviceType const * const device);

/** \brief ioctl function type */
typedef int32_t (*ciaaDevices_ioctl)(ciaaDevices_deviceType const * const device, int32_t const request, void * param);

/** \brief read function type */
typedef ssize_t (*ciaaDevices_read)(ciaaDevices_deviceType const * const device, uint8_t * const buf, size_t const nbyte);

/** \brief write function type */
typedef ssize_t (*ciaaDevices_write)(ciaaDevices_deviceType const * const device, uint8_t const * const buf, size_t const nbyte);

/** \brief lseek function type */
typedef off_t (*ciaaDevices_lseek)(ciaaDevices_deviceType const * const device, off_t const offset, uint8_t const whence);

/** \brief Device Type */
struct ciaaDevices_deviceStruct {
   char const * path;            /** <- device path, eg. /dev/serlia/UART1 */
   ciaaDevices_open open;        /** <- pointer to open function */
   ciaaDevices_close close;      /** <- pointer to close function */
   ciaaDevices_read read;        /** <- pointer to read function */
   ciaaDevices_write write;      /** <- pointer to write function */
   ciaaDevices_ioctl ioctl;      /** <- pointer to ioctl function */
   ciaaDevices_lseek lseek;      /** <- pointer to lseek function */
   void * upLayer;               /** <- pointer to be provided to the upper
                                        layer */
   void * layer;                 /** <- pointer ot be used by the layer */
   void * loLayer;               /** <- pointer to be provided to the lower
                                        layer */
};

/** \brief Devices Status
 **
 ** Describes the possible status of a device
 **
 **/
typedef enum
{
   ciaaDevices_EStatus_Open = 0,    /** < Device is open */
   ciaaDevices_EStatus_Close = 1,   /** < Device is close */
   ciaaDevices_EStatus_Error = 2    /** < Device is open with errors */
} ciaaDevices_Enum_Status;

typedef enum
{
   ciaaDevices_EMode_ReadOnly = 0
} ciaaDevices_Enum_Mode;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/** \brief Init ciaa Devices
 **
 ** This function performs the initialization of ciaaDevices
 **/
extern void ciaaDevices_init(void);

/** \brief add deivce
 **
 ** Adds the device device during the initialization.
 **
 ** \param[in] device device to be added
 **
 ** \remarks This function can not be called reentrant.
 **/
extern void ciaaDevices_addDevice(ciaaDevices_deviceType * device);

/** \brief get a device
 **
 ** Get the device with the indicated path.
 **/
extern ciaaDevices_deviceType * ciaaDevices_getDevice(char const * const path);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef CIAADEVICES_H */


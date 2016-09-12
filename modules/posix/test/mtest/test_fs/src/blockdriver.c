/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

/** \brief Raw block device management driver
 **
 ** Definicion de funciones y variables relacionadas con el driver de manejo de blockdevice sin un filesystem asociado.
 ** Se trata al Device como un unico archivo, no se tiene en cuenta el formateo de datos.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MZ           Marcos Ziegler
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160101 v0.0.1 MZ initial version
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "ciaaBlockDevices.h"
#include "vfs.h"
#include "blockdriver.h"

/*==================[macros and definitions]=================================*/
/*==================[internal data declaration]==============================*/
/*==================[internal functions declaration]=========================*/

/** \brief truncate given file data
 **
 ** \param[in] file
 ** \return -1 if an error occurs, in other case 0
 **/
static int blockdriver_open(file_desc_t *file);

/** \brief truncate given file data
 **
 ** \param[in] file
 ** \param[in]
 ** \return -1 if an error occurs, in other case 0
 **/
static int blockdriver_close(file_desc_t *file);

/** \brief truncate given file data
 **
 ** \param[in] file file representing the block device to be read
 ** \param[in] buf buffer whose data is to be written
 ** \param[in] size number of bytes to be written
 ** \return    the count of written bytes is returned
 **/
static size_t blockdriver_read(file_desc_t *file, void *buf, size_t size);

/** \brief truncate given file data
 **
 ** \param[in] file file representing the block device to be written
 ** \param[in] buf buffer whose data is to be written
 ** \param[in] size number of bytes to be written
 ** \return    the count of written bytes is returned
 **/
static size_t blockdriver_write(file_desc_t *file, void *buf, size_t size);

/** \brief truncate given file data
 ** TODO
 ** \param[in] node
 ** \param[in] length
 ** \return -1 if an error occurs, in other case 0
 **/
static int blockdriver_truncate(vnode_t *node, size_t size);

/** \brief create a new file
 **
 ** \param[in] parent_node parent of the new node
 ** \param[in] child_node node to be created in disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int blockdriver_create(vnode_t *parent_node, vnode_t *child_node);

/** \brief delete given file
 **
 ** \param[in] parent_node parent of the node to be deleted
 ** \param[in] child_node node to be erased in disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int blockdriver_delete(vnode_t *parent_node, vnode_t *child_node);

/*==================[internal functions definition]==========================*/

static int blockdriver_open(file_desc_t *file)
{
   return 0;
}

static int blockdriver_close(file_desc_t *file)
{
   return 0;
}

static size_t blockdriver_read(file_desc_t *file, void *buf, size_t size)
{
   uint32_t pointer;
   ciaaDevices_deviceType const *device;
   size_t ret;

   pointer = file->cursor;
   device = file->node->fs_info->device;
   ret = ciaaBlockDevices_lseek(device, pointer, SEEK_SET);
   if(ret!=pointer)
   {
      return 0;
   }

   ret = ciaaBlockDevices_read(device, (uint8_t *)buf, size);
   file->cursor += ret;
   return ret;
}

static size_t blockdriver_write(file_desc_t *file, void *buf, size_t size)
{
   uint32_t pointer;
   ciaaDevices_deviceType const *device;
   size_t ret;

   pointer = file->cursor;
   device = file->node->fs_info->device;
   ret = ciaaBlockDevices_lseek(device, pointer, SEEK_SET);
   if(ret != pointer)
   {
      return 0;
   }
   ret = ciaaBlockDevices_write(device, (uint8_t *)buf, size);
   file->cursor += ret;
   return ret;
}

static int blockdriver_truncate(vnode_t *node, size_t size)
{
   return 0;
}

/** \brief throw a command to the file
 **
 ** \param[in] file file to be controlled
 ** \param[in] request command to be executed to the file
 ** \return -1 if an error occurs, in other case 0
 **/
static int blockdriver_ioctl(file_desc_t *file, int request)
{
   ciaaDevices_deviceType const *device;
   size_t ret;

   device = file->node->fs_info->device;
   ret = ciaaBlockDevices_ioctl(device, request, NULL);
   return ret;
}

static int blockdriver_init(void * parameter)
{
   return 0;
}

static int blockdriver_create(vnode_t *parent_node, vnode_t *child_node)
{
   return 0;
}

static int blockdriver_delete(vnode_t *parent_node, vnode_t *child_node)
{
   return 0;
}

/*==================[internal data definition]===============================*/

static fsdriver_operations_t blockdriver_operations =
{
   blockdriver_open,
   blockdriver_close,
   blockdriver_read,
   blockdriver_write,
   blockdriver_ioctl,

   blockdriver_init,
   NULL,
   NULL,
   blockdriver_create,
   blockdriver_delete,
   blockdriver_truncate,
   NULL
};

/*==================[external data definition]===============================*/

/** \brief blockdriver driver structure
 *
 * It will be declared as "extern" in vfs.c, so its visible to function vfs_get_driver()
 */
filesystem_driver_t blockdev_driver =
{
   "BLOCKDEV",
   &blockdriver_operations
};

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

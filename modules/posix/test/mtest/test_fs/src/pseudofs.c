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

/** \brief Short description of this file
 **
 ** Long description of this file
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
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "pseudofs.h"
#include "vfs.h"
//#include "ciaaLibs_PoolBuf.h"
#include "tlsf.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[internal functions declaration]=========================*/

/** \brief open the given file
 **
 ** \param[in] file file to be opened
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_open(file_desc_t *file_desc);

/** \brief close the given file
 **
 ** \param[in] file file to be closed
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_close(file_desc_t *file_desc);

/** \brief read the file data
 **
 ** \param[in] file file from which to read data
 ** \param[in] buf buffer whom to fill with data
 ** \param[in] size size of data to be read in bytes
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **/
static size_t pseudofs_read(file_desc_t *desc, void *buf, size_t size);

/** \brief write data to the file
 **
 ** \param[in] file file from which to read data
 ** \param[in] buf buffer whom to fill with data
 ** \param[in] size size of data to be read in bytes
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **/
static size_t pseudofs_write(file_desc_t *desc, void *buf, size_t size);

/** \brief throw a command to the file
 **
 ** \param[in] file file to be controlled
 ** \param[in] request command to be executed to the file
 ** \return -1 if an error occurs, in other case 0
 **/
static int    pseudofs_ioctl(file_desc_t *desc, int request);

/** \brief init the file system
 ** TODO
 ** \param[in]
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_init(void *par);

/** \brief create a new file
 **
 ** \param[in] parent_node parent of the new node
 ** \param[in] child_node node to be created in disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_create(vnode_t *parent_node, vnode_t *child_node);

/** \brief delete given file
 **
 ** \param[in] parent_node parent of the node to be deleted
 ** \param[in] child_node node to be erased in disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_delete(vnode_t *parent_node, vnode_t *child_node);

/** \brief truncate given file data
 ** TODO
 ** \param[in] node
 ** \param[in] length
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_truncate(vnode_t *node, size_t size);

/** \brief format a device with ext2 format
 **
 ** \param[in] dev_node node of the device to be formatted
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_format(vnode_t *dev_node, void *param);

/** \brief mount a disk to a directory node
 **
 ** \param[in] dev_node node of the device to be mounted
 ** \param[in] dest_node rooot directory of the new mount
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_mount(vnode_t *device_node, vnode_t *dest_dir);

/** \brief umount device from directory
 **
 ** \param[in]
 ** \return -1 if an error occurs, in other case 0
 **/
static int pseudofs_umount(vnode_t *node);

/*==================[internal data definition]===============================*/

/** \brief ext2 driver operations
 *
 */
static fsdriver_operations_t pseudofs_operations =
{
   pseudofs_open,
   pseudofs_close,
   pseudofs_read,
   pseudofs_write,
   pseudofs_ioctl,

   pseudofs_init,
   pseudofs_format,
   pseudofs_mount,
   pseudofs_create,
   pseudofs_delete,
   pseudofs_truncate,
   pseudofs_umount
};

/*==================[external data definition]===============================*/

/** \brief pseudofs driver structure
 *
 * It will be declared as "extern" in vfs.c, so its visible to function vfs_get_driver()
 */
filesystem_driver_t pseudofs_driver =
{
   "pseudofs",
   &pseudofs_operations
};

/*==================[internal functions definition]==========================*/

static int pseudofs_open(file_desc_t *file_desc)
{

   pseudofs_file_info_t *file_info;

   file_info = (pseudofs_file_info_t *)file_desc->node->f_info.down_layer_info;
   file_info->pointer = 0;

   return 0;
}

static int pseudofs_close(file_desc_t *file_desc) {
   return 0;
}


static size_t pseudofs_read(file_desc_t *desc, void *buf, size_t size)
{

   uint8_t *data;
   pseudofs_file_info_t *file_info;

   file_info = (pseudofs_file_info_t *) desc->node->f_info.down_layer_info;
   data = file_info->data;
   file_info->pointer = desc->cursor;

   if((file_info->pointer + size) >= file_info->file_size)
      size = file_info->file_size - file_info->pointer;

   ciaaPOSIX_memcpy((uint8_t *)buf, data + file_info->pointer, size);

   return size;
}

/* FIXME: node->f_info.file_size not set */
static size_t pseudofs_write(file_desc_t *desc, void *buf, size_t size)
{
   pseudofs_file_info_t *finfo;

   finfo = (pseudofs_file_info_t *) desc->node->f_info.down_layer_info;
   finfo->pointer = desc->cursor;

   if(finfo->pointer + size <= finfo->alloc_size)
   {
      size = finfo->alloc_size - finfo->pointer;
   }
   //finfo->alloc_size = 2, finfo->pointer = 1, finfo->alloc_size - finfo->pointer = 1
   //finfo->pointer == finfo->alloc_size ---> FULL. finfo->pointer < finfo->alloc_size
   ciaaPOSIX_memcpy(finfo->data + finfo->pointer, (uint8_t *) buf, size);
   finfo->pointer += size;
   if(finfo->pointer >= finfo->file_size)
   {
      finfo->file_size = finfo->pointer;
      desc->node->f_info.file_size = finfo->file_size;
   }
   desc->cursor = finfo->pointer;

   return size;
}

static int pseudofs_ioctl(file_desc_t *desc, int request)
{
   return 0;
}

static int pseudofs_init(void * parameter)
{
   return 0;
}

static int pseudofs_create(vnode_t *parent_node, vnode_t *child_node)
{
   pseudofs_file_info_t *file_info;

   /* If the node to be created is a directory, all information needed is contained in the vfs layer node.
    * If the new node is a regular file, a metadata structure and file data memory must be allocated.
    */
   if(VFS_FTREG == child_node->f_info.type)
   {
      /* Alloc a new file metadata structure */
      file_info = (pseudofs_file_info_t *) tlsf_malloc(fs_mem_handle, sizeof(pseudofs_file_info_t));
      if(NULL == file_info)
      {
         return -1;
      }
      /* Clear new file info */
      ciaaPOSIX_memset(file_info, 0, sizeof(pseudofs_file_info_t));
      /* Alloc fixed mem size for file data */
      file_info->data = (uint8_t *) ciaaPOSIX_malloc(PSEUDOFS_FILE_ALLOC_SIZE);
      if(NULL == file_info->data)
      {
         //ciaaLibs_poolBufFree(&pseudofs_finfo_pool, file_info);
         tlsf_free(fs_mem_handle, (void *)file_info);
         child_node->f_info.down_layer_info = NULL;
         return -1;
      }
      /* Clear data memory */
      ciaaPOSIX_memset(file_info->data, 0, PSEUDOFS_FILE_ALLOC_SIZE);
      file_info->alloc_size = PSEUDOFS_FILE_ALLOC_SIZE;
      child_node->f_info.down_layer_info = file_info;
   }

   return 0;
}

static int pseudofs_delete(vnode_t *parent_node, vnode_t *child_node)
{
   pseudofs_file_info_t *file_info;

   if(VFS_FTREG == child_node->f_info.type)
   {
      file_info = (pseudofs_file_info_t *) child_node->f_info.down_layer_info;
      ciaaPOSIX_free(file_info->data);
      file_info->data = NULL;
      ciaaPOSIX_free(child_node->f_info.down_layer_info);
      //ciaaLibs_poolBufFree(&pseudofs_finfo_pool, file_info);
      tlsf_free(fs_mem_handle, (void *)file_info);
      child_node->f_info.down_layer_info = NULL;
   }
   return 0;
}

static int pseudofs_truncate(vnode_t *node, size_t size)
{
   pseudofs_file_info_t *file_info;

   file_info = (pseudofs_file_info_t *) node->f_info.down_layer_info;

   if(size > file_info->alloc_size)
   {
      return -1;
   }
   file_info = node->f_info.down_layer_info;
   file_info->file_size = size;

   return 0;
}

static int pseudofs_format(vnode_t *dev_node, void *param)
{
   return 0;
}


static int pseudofs_mount(vnode_t *device_node, vnode_t *dest_dir)
{
   pseudofs_file_info_t *file_info;
   pseudofs_fs_info_t *fs_info;

   file_info = (pseudofs_file_info_t *) tlsf_malloc(fs_mem_handle, sizeof(pseudofs_file_info_t));
   if(NULL == file_info)
   {
      return -1;
   }
   fs_info = (pseudofs_fs_info_t *) tlsf_malloc(fs_mem_handle, sizeof(pseudofs_fs_info_t));
   if(NULL == fs_info)
   {
      return -1;
   }

   dest_dir->fs_info->down_layer_info = fs_info;
   dest_dir->f_info.down_layer_info = file_info;

   return 0;
}

static int pseudofs_umount(vnode_t *node)
{
   return 0;
}

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

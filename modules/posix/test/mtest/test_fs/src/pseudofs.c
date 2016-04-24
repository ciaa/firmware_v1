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
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdlib.h"
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "pseudofs.h"
#include "vfs.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[internal functions declaration]=========================*/

static int pseudofs_open(struct file_desc *file_desc);
static int pseudofs_close(struct file_desc *file_desc);
static size_t pseudofs_read(struct file_desc *desc, void *buf, size_t size);
static size_t pseudofs_write(struct file_desc *desc, void *buf, size_t size);
static int    pseudofs_ioctl(struct file_desc *desc, int request);

static int pseudofs_init(void *par);
static int pseudofs_create(vnode_t *parent_node, vnode_t *child_node);
static int pseudofs_delete(vnode_t *parent_node, vnode_t *child_node);
static int pseudofs_truncate(vnode_t *node, size_t size);
static int pseudofs_format(vnode_t *dev_node);
static int pseudofs_mount(vnode_t *device_node, vnode_t *dest_dir);
static int pseudofs_umount(vnode_t *node);

/*==================[internal data definition]===============================*/

static struct fsdriver_operations pseudofs_operations =
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

//Variable global. Se declarara extern en vfs.c
struct filesystem_driver pseudofs_driver =
{
   "pseudofs",
   &pseudofs_operations
};

/*==================[internal functions definition]==========================*/

static int pseudofs_open(struct file_desc *file_desc)
{

   struct pseudofs_file_info *file_info;

   file_info = (struct pseudofs_file_info *)file_desc->node->f_info.down_layer_info;
   file_info->pointer = file_desc->cursor;

   return 0;
}

static int pseudofs_close(struct file_desc *file_desc) {
   return 0;
}


static size_t pseudofs_read(struct file_desc *desc, void *buf, size_t size)
{

   uint8_t *data;
   struct pseudofs_file_info *file_info;

   file_info = (struct pseudofs_file_info *) desc->node->f_info.down_layer_info;
   data = file_info->data;

   if((file_info->pointer + size) <= file_info->file_size)
      size = file_info->file_size - file_info->pointer;

   ciaaPOSIX_memcpy((uint8_t *)buf, data, size);

   return size;
}

static size_t pseudofs_write(struct file_desc *desc, void *buf, size_t size)
{
   uint8_t *new_data;
   struct pseudofs_file_info *file_info;
   uint32_t new_size;

   file_info = (struct pseudofs_file_info *) desc->node->f_info.down_layer_info;

   file_info->pointer = desc->cursor;

   if(file_info->pointer + size >= file_info->alloc_size)
   {
      for(new_size=0x01; new_size<=(file_info->pointer + size););
      {
         new_size=new_size<<1;
      }
      new_data = (uint8_t *) ciaaPOSIX_malloc(new_size);
      if(new_data == NULL)
         return 0;
      ciaaPOSIX_memcpy(new_data, file_info->data, file_info->file_size);
      ciaaPOSIX_free(file_info->data);
      file_info->data = new_data;
      file_info->alloc_size = new_size;
   }

   ciaaPOSIX_memcpy(file_info->data + file_info->pointer, (uint8_t *) buf, size);
   file_info->pointer += size;
   desc->cursor = file_info->pointer;

   if(file_info->pointer + size >= file_info->file_size)
      file_info->file_size = file_info->pointer + size;

   return size;
}

static int pseudofs_ioctl(struct file_desc *desc, int request)
{
   return 0;
}

static int pseudofs_init(void * parameter)
{

/*
   vfs_lookup(RAMFS_DIR, &dir_node);
   ramdisk = ramdisk_create(ramfs_dev, FILESYSTEM_SIZE * PAGE_SIZE()))) {
   dev_node = ramdisk->bdev->dev_vfs_info;
   res = ramfs_format(dev_node);
   return ramfs_mount(dev_node, dir_node.node);
*/
   return 0;
}

static int pseudofs_create(vnode_t *parent_node, vnode_t *child_node)
{
   struct pseudofs_file_info *file_info;
   //struct pseudofs_fs_info *fs_info;

   file_info = (struct pseudofs_file_info *) ciaaPOSIX_malloc(sizeof(struct pseudofs_file_info));
   if(file_info == NULL)
   {
      return -1;
   }
   ciaaPOSIX_memset(file_info, 0, sizeof(struct pseudofs_file_info));
   file_info->data = (uint8_t *) ciaaPOSIX_malloc(PSEUDOFS_MIN_ALLOC_SIZE);
   if(file_info->data==NULL)
   {
      ciaaPOSIX_free(file_info);
      child_node->f_info.down_layer_info=NULL;
      return -1;
   }
   file_info->alloc_size = PSEUDOFS_MIN_ALLOC_SIZE;
   child_node->f_info.down_layer_info = file_info;

   ciaaPOSIX_memcpy(child_node->fs_info.down_layer_info, parent_node->fs_info.down_layer_info,
         sizeof(struct pseudofs_fs_info));

   return 0;
}

//Si es un mountpoint, como elimino el node->fs_info.down_layer_info?
static int pseudofs_delete(vnode_t *parent_node, vnode_t *child_node)
{
   struct pseudofs_file_info *file_info;
   //struct pseudofs_fs_info *fs_info;

   file_info = (struct pseudofs_file_info *) child_node->f_info.down_layer_info;
   ciaaPOSIX_free(file_info->data);
   ciaaPOSIX_free(child_node->f_info.down_layer_info);
   child_node->f_info.down_layer_info=NULL;

   return 0;
}

static int pseudofs_truncate(vnode_t *node, size_t size)
{
   struct pseudofs_file_info *file_info;
   //struct pseudofs_fs_info *fs_info;
   file_info = node->f_info.down_layer_info;
   file_info->file_size = size;

   return 0;
}

static int pseudofs_format(vnode_t *dev_node)
{
   //struct pseudofs_file_info *file_info;
   //struct pseudofs_fs_info *fs_info;

   return 0;
}


static int pseudofs_mount(vnode_t *device_node, vnode_t *dest_dir)
{
   struct pseudofs_file_info *file_info;
   struct pseudofs_fs_info *fs_info;

   fs_info = (struct pseudofs_fs_info *) ciaaPOSIX_malloc(sizeof(struct pseudofs_fs_info));
   if(fs_info == NULL)
   {
      return -1;
   }
   file_info = (struct pseudofs_file_info *) ciaaPOSIX_malloc(sizeof(struct pseudofs_file_info));
   if(file_info == NULL)
   {
      ciaaPOSIX_free(fs_info);
      return -1;
   }

   dest_dir->fs_info.down_layer_info = fs_info;
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

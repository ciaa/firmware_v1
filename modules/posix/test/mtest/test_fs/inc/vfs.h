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

#ifndef VFS_H_
#define VFS_H_

/** \brief VFS header file
 **
 ** This is the header file of the virtual file system. It contains the posix API, node tree management,
 ** file descriptor management
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup MTests CIAA Firmware Module Tests
 ** @{ */
/** \addtogroup Filesystem VFS
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MZ         Marcos Ziegler
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160101 v0.0.1 MZ Initial version
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdio.h"     /* <= device handler header */
#include "ciaaPOSIX_stdbool.h"
#include "ciaaBlockDevices.h"

/*==================[macros]=================================================*/

/** \brief Max length of a filename */
#define FS_NAME_MAX 30
/** \brief Max length of an absolute path string */
#define FS_PATH_MAX 256
/** \brief Max files open simultaneously */
#define FILE_DESC_MAX 16


/* open flag settings for open() (and related APIs) */

#define VFS_O_RDONLY    (1 << 0)        /* Open for read access (only) */
#define VFS_O_RDOK      O_RDONLY        /* Read access is permitted (non-standard) */
#define VFS_O_WRONLY    (1 << 1)        /* Open for write access (only) */
#define VFS_O_WROK      O_WRONLY        /* Write access is permitted (non-standard) */
#define VFS_O_RDWR      (O_RDOK|O_WROK) /* Open for both read & write access */
#define VFS_O_CREAT     (1 << 2)        /* Create file/sem/mq object */
#define VFS_O_EXCL      (1 << 3)        /* Name must not exist when opened  */
#define VFS_O_APPEND    (1 << 4)        /* Keep contents, append to end */
#define VFS_O_TRUNC     (1 << 5)        /* Delete contents */
#define VFS_O_NONBLOCK  (1 << 6)        /* Don't wait for data */
#define VFS_O_NDELAY    O_NONBLOCK      /* Synonym for O_NONBLOCK */
#define VFS_O_SYNC      (1 << 7)        /* Synchronize output on write */
#define VFS_O_DSYNC     O_SYNC          /* Equivalent to OSYNC in NuttX */
#define VFS_O_BINARY    (1 << 8)        /* Open the file in binary (untranslated) mode. */

/*==================[typedef]================================================*/

/** \brief File type
 **
 ** This is a field in the vnode structure which indicates
 ** what kind of file this node represents
 **
 **/
typedef enum   {
      VFS_FTREG,   //Regular file node
      VFS_FTDIR,   //Directory file node
      VFS_FTBLK,   //Block device node
      VFS_FTUNKNOWN   //Unknown
      } node_type_t;

/** \brief File permissions
 **
 ** This is a field in the vnode structure which indicates
 ** what permissions the user has on the file
 **
 **/
typedef enum { ENUM2} access_mode_t;

typedef struct vfs_node vnode_t;
struct file_desc;

/** \brief Filesystem driver methods
 **
 ** Collection of functions of a particular driver
 **
 **/
struct fsdriver_operations
{
   int    (*file_open)(struct file_desc *file_desc);
   int    (*file_close)(struct file_desc *file_desc);
   size_t (*file_read)(struct file_desc *desc, void *buf, size_t size);
   size_t (*file_write)(struct file_desc *desc, void *buf, size_t size);
   int    (*file_ioctl)(struct file_desc *desc, int request);

   int (*fs_init)(void *par);
   int (*fs_format)(vnode_t *dev_node);
   int (*fs_mount)(vnode_t *dev_node, vnode_t *dest_node);
   int (*fs_create_node)(vnode_t *parent_node, vnode_t *child_node);
   int (*fs_delete_node)(vnode_t *parent_node, vnode_t *child_node);
   int (*fs_truncate)(vnode_t *node, uint32_t length);
   int (*fs_umount)(vnode_t *node);
};

/** \brief File system driver
 **
 ** TODO:#define DECLARE_FILE_SYSTEM_DRIVER(fs_driver_)
 **    Crear una macro que agregue un driver en particular desde un archivo
 **    Primero declara extern del array de drivers y despues agrega el elemento
 **/
struct filesystem_driver
{
   const char            *driver_name;
   const struct fsdriver_operations   *driver_op;
};

/** \brief VFS metadata information
 **
 ** Contains upper layer node information, independent of the lower layer
 **
 **/
//Aca va informacion del lock, etc.
struct node_info
{
   node_type_t type;
   access_mode_t mode;// discrete access mode Read-Write-Execution
   bool   is_mount_dir;
   void             *down_layer_info; // on disk metadata information
};


/** \brief VFS file system information
 **
 ** Contains upper layer file system information, independent of the lower layer
 **
 **/
struct filesystem_info
{
   struct filesystem_driver *drv; // file system driver
   ciaaDevices_deviceType const * device; // block device, where is this file system
   void             *down_layer_info; // file system information (extended information)
};


/** \brief VFS file information
 **
 ** Contains upper layer file information, independent of the lower layer
 **
 **/
struct file_info
{
   char file_name[FS_NAME_MAX + 1];
   uint8_t file_namlen;
   uint32_t file_pointer;
   uint32_t file_size;
   void             *down_layer_info;
};

/** \brief vnode structure
 **
 ** The basic data structure of the virtual file system. Contains metadata of files in the lower layer
 **
 **/
struct vfs_node
{
   struct vfs_node *parent_node;      //Link to upper level inode
   struct vfs_node *sibling_node;     // Link to same level inode
   struct vfs_node *child_node;    // Link to lower level inode

   struct node_info n_info;
   struct filesystem_info fs_info;
   struct file_info f_info;
};

/*

File descriptor v0.1: Utilizo una estructura struct file_descriptor_table. Tiene un arreglo de punteros a file_desc y un numero que indica la cantidad de file_desc ocupados. Puede haber hasta FILE_DESC_MAX descriptores.
typedef struct file_descriptor_table
{
   struct file_desc *table[FILE_DESC_MAX];
   size_t n_busy_desc;
}

*/

/** \brief File descriptor type
 **
 ** Contains information of an open file in a task
 **
 **/
struct file_desc
{
   vnode_t *node;
   size_t cursor;
   size_t index;   //TODO: Mejorar esto. Posicion dentro del array de fdesc. Dependiente de la implementacion.
};

/** \brief File descriptor table
 **
 **
 **
 **/
struct file_descriptor_table
{
   struct file_desc *table[FILE_DESC_MAX];
   size_t n_busy_desc;
};



/*==================[external data declaration]==============================*/
/*==================[external functions declaration]=========================*/

/** \brief Create a new child
 **
 ** Crea un hijo al nodo parent, de nombre name con longitud namelen.
 **
 ** \param[in] parent parent of the new node
 ** \param[in] name how the new node will be named
 ** \param[in] namelen strlen() of name
 ** \param[in] mode execution mode
 ** \return NULL if an error occurs, in other case the pointer to the newly created node
 **
 **/
extern vnode_t *vfs_create_child(vnode_t *parent, const char *name, size_t namelen, mode_t mode);

/** \brief Print the tree structure
 **
 ** Prints the tree structure showing the name and hierarchy of each node
 **
 ** \return -1 if an error occurs, 0 in success
 **
 **/
extern int vfs_print_tree(void);

/** \brief vfs initialization
 **
 ** Performs the initialization of the vfs. Must be called before using any of the vfs functions.
 ** Inicializa el root inode, los directorios basicos y asigna un archivo para cada dispositivo.
 **
 ** \return -1 if an error occurs, 0 if success
 **/
extern int ciaaFS_init(void);

/** \brief format a device
 **
 ** formats a device given by device_path with a format given by fs_type
 **
 ** \param[in] device_path path of the device to be mounted
 ** \param[in] fs_type string indicating the filesystem type
 **               EXT2:
 **               BOLCKDEV:
 **               PSEUDOFS:
 ** \return -1 if an error occurs, 0 if success
 **/
extern int ciaaFS_format(const char *device_path, const char *fs_type);

/** \brief mount a filesystem
 **
 ** mounts a device given by device_path in the target_path with a format given by fs_type
 **
 ** \param[in] device_path path of the device to be mounted
 ** \param[in] target_path root of the mount
 ** \param[in] fs_type string indicating the filesystem type
 **               EXT2:
 **               BOLCKDEV:
 **               PSEUDOFS:
 ** \return -1 if an error occurs, 0 if success
 **/
extern int ciaaFS_mount(char *device_path,  char *target_path, char *fs_type);

/** \brief Create a new directory
 **
 ** Create a new directory in the path dir_path
 **
 ** \param[in] dir_path path of the device to be opened
 ** \param[in] modedevice device to be opened
 ** \return -1 if an error occurs, 0 if success
 **/
extern int ciaaFS_mkdir(const char *dir_path, mode_t mode);

/** \brief Open a file
 **
 ** Opens a file or device path for read/write/readwrite depending on oflag.
 **
 ** \param[in] path  path of the device to be opened
 ** \param[in] oflag may take one of the following values:
 **               O_RDONLY: opens files to read only
 **               O_WRONLY: opens files to write only
 **               O_RDWR: opens file to read and write
 ** \return -1 if failed, a non negative integer representing the file
 **         descriptor if success.
 **
 ** \remarks Opening twice the same path will provide two different file
 **          descriptors. Accessing them with close, ioctl,
 **          read, write, seek may produce
 **          unexpected behaviors.
 **/
extern int ciaaFS_open(const char *path, int flags);

/** \brief Close a file descriptor
 **
 ** Closes the file descriptor fildes
 **
 ** \param[in] fildes file descriptor to be closed
 ** \return    -1 if failed, 0 in other if success.
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern int ciaaFS_close(int fd);
/** \brief Reads from a file descriptor
 **
 ** Reads nbyte from the file descriptor fildes and store them in buf.
 **
 ** \param[in]  fildes  file descriptor to read from
 ** \param[out] buf     buffer to store the read data
 ** \param[in]  nbyte   count of bytes to be read
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern ssize_t ciaaFS_read(int fd, void *buf, size_t nbytes);

/** \brief Writes to a file descriptor
 **
 ** Writes nbyte to the file descriptor fildes from the buffer buf. If used to
 ** transfer data over a device a successul completion does not guarantee the
 ** correct delivery of the message.
 **
 ** \param[in] fildes   file descriptor to write to
 ** \param[in] buf      buffer with the data to be written
 ** \param[in] nbyte    count of bytes to be written
 ** \return -1 if failed, a non negative integer representing the count of
 **         written bytes if success
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern ssize_t ciaaFS_write(int fd, void *buf, size_t nbytes);

/** \brief Seek into a file descriptor
 **
 ** Set the read/write position to a given offset.
 **
 ** \param[in] fildes   file descriptor to set the position
 ** \param[in] offset   depending on the value of whence offset represents:
 **                     offset from the beggining if whence is set to SEEK_SET.
 **                     offset from the end if whence is set to SEEK_END.
 **                     offset from the current position if whence is set to
 **                     SEEK_CUR.
 ** \param[in] whence   SEEK_CUR, SEEK_SET or SEEK_END
 ** \return -1 if failed, a non negative integer representing the count of
 **         written bytes if success
 **
 ** \remarks Setting offset to a positive value and whence to SEEK_END will
 **          return -1.
 **          Setting offset to a negative value and whence to SEEK_SET will
 **          return -1.
 **
 ** \remarks The functions ciaaPOSIX_close, ciaaPOSIX_ioctl, ciaaPOSIX_read,
 **          ciaaPOSIX_write and ciaaPOSIX_lseek may be called reentrant but with
 **          different file descriptor. If one of this function is called with a
 **          specific file descriptor the caller has to wait until return before
 **          calling other of this function using the same file handler.
 **/
extern ssize_t ciaaFS_lseek(int fd, ssize_t offset, int whence);
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

#endif /* VFS_H_ */

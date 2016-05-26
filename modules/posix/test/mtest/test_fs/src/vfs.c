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
 ** Contains the file system POSIX API, vfs implementation and file descriptor (file objects) implementation.
 ** The POSIX API contains common functions as open(), close(), read(), write().
 ** The VFS is an abstraction layer over the many file system implementations.
 ** File objects are structures that relate an open file with a process in execution.
 ** File objects have fields such as corresponding node and current position in file.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module FIXME
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

/*
TODO:
-Use only static memory
-Multitasking support: Mutex, semaphores and queues
-Implement a default root filesystem, to create files in ram
*/

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdlib.h"
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "vfs.h"

/*==================[macros and definitions]=================================*/

#define ASSERT_MSG(cond, msg) assert_msg((cond), (msg), __FILE__, __LINE__)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** \brief Auxiliary recursive function of vfs_get_mntpt_path()
 **
 **
 **
 ** \param[in] node node whose path is to be found
 ** \param[out] path path string returned
 ** \param[out] len_p pointer to the path size variable
 ** \return -1 if an error occurs, in other case 0
 **/
static int vfs_get_mntpt_path_rec(vnode_t *node, char *path, uint16_t *len_p);

/** \brief get the path relative to the mountpoint
 **
 **
 **
 ** \param[in] node node whose mount point path is to be found
 ** \param[out] path string which will contain the mountpoint path
 ** \param[out] len_p size of path string
 ** \return -1 if an error occurs, in other case 0
 **/
static int vfs_get_mntpt_path(vnode_t *node, char *path, uint16_t *len_p);

/** \brief alloc memory for new node and initialize contents
 **
 **
 **
 ** \param[in] name
 ** \param[in] name_len
 ** \return created node if success, else NULL
 **/
static vnode_t *vfs_node_alloc(const char *name, size_t name_len);

/** \brief create subtree given a path
 **
 ** Creates intermediate directories
 ** TODO: Implement tree creation from relative path too
 ** Path must contain initial '/', indicating its a root path
 ** Created leaf will be VFS_FTREG type
 **
 ** \param[in] path
 ** \param[out] inode_p
 ** \return -1 if the directory already exists FIXME
 **/
static int vfs_inode_reserve(const char *path, vnode_t **inode_p);

/** \brief search inode by path
 **
 ** postconditions:
 **    path_p points to the beginning first non-found element in path
 **    ret_node_p points to the last found node in the path
 **
 ** \param[in] path_p pointer to the path string to be searched
 ** \param[in] ret_node_p pointer to the found node address
 ** \return -1 if an error occurs, in other case 0
 **/
static int vfs_inode_search(char **path_p, vnode_t **ret_node_p);

/** \brief get a driver of a specified filesystem by name
 **
 **
 **
 ** \param[in] driver_name name of the driver as given in its corresponding filesystem_driver struct
 ** \return pointer to searched fs driver if success, NULL otherwise
 **/
static filesystem_driver_t *vfs_get_driver(const char *driver_name);

/** \brief initialize file descriptor table
 **
 ** alloc memory, initialize values, etc.
 **
 ** \return -1 if an error occurs, in other case 0
 **/
static int file_descriptor_table_init(void);

/** \brief
 **
 **
 **
 ** \param[in] node the new file descriptor will be linked to this file
 ** \return pointer to created descriptor structure if success, else NULL
 **/
static file_desc_t *file_desc_create(vnode_t *node);

/** \brief free a file descriptor
 **
 **
 **
 ** \param[in] file_desc descriptor to be freed
 ** \return -1 if an error occurs, in other case 0
 **/
static int file_desc_destroy(file_desc_t *file_desc);

/** \brief get a descriptor structure by its number
 **
 **
 **
 ** \param[in] index requested descriptor structure index
 ** \return -1 if an error occurs, in other case 0
 **/
static file_desc_t *file_desc_get(uint16_t index);

/*==================[internal data definition]===============================*/

/** \brief Root inode
 * 
 * Root inode of the entire vfs
 * 
 */
static vnode_t *vfs_root_inode = NULL;

/** \brief Filesystem drivers declaration
 *
 * Here are the drivers defined in the lower layer file system implementations (in ext2.c, fat.c, etc.)
 * 
 */
extern filesystem_driver_t ext2_driver;
extern filesystem_driver_t pseudofs_driver;
extern filesystem_driver_t blockdev_driver;

/** \brief Filesystem drivers table
 *
 * Driver table used by vfs_get_driver() to retrieve requested driver
 *
 */
static filesystem_driver_t *vfs_fsdriver_table[] =
{
   &ext2_driver,
   &blockdev_driver,
   NULL,
   NULL,
   NULL
};

/** \brief File descriptors table
 *
 * TODO: Every thread should have its own table
 *
 */
static file_descriptor_table_t _f_tab;
static file_descriptor_table_t *file_desc_tab_p = &_f_tab;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void assert_msg(int cond, char* msg, char * file, int line)
{
   if (cond)
   {
      /* assertion is ok */
      ciaaPOSIX_printf("OK: Assert in %s:%d\n", file, line);
   }
   else
   {
      ciaaPOSIX_printf("ERROR: Assert Failed in %s:%d %s\n", file, line, msg);
   }
}

static int vfs_get_mntpt_path_rec(vnode_t *node, char *path, uint16_t *len_p)
{
   uint16_t name_len;
   int ret;
   if(false == node->n_info.is_mount_dir)
   {
      ret = vfs_get_mntpt_path_rec(node->parent_node, path, len_p);
      if(ret)
      {
         return -1;
      }
   }
   name_len = ciaaPOSIX_strlen(node->f_info.file_name);
   if(*len_p + name_len > FS_PATH_MAX-1)
      return -1;
   ciaaPOSIX_strcpy((char *)path+(*len_p), node->f_info.file_name);
   *len_p += ciaaPOSIX_strlen(node->f_info.file_name);
   return 0;
}

static int vfs_get_mntpt_path(vnode_t *node, char *path, uint16_t *len_p)
{
   int ret;
   *len_p = 0;
   ret = vfs_get_mntpt_path_rec(node, path, len_p);
   return ret;
}

static vnode_t *vfs_node_alloc(const char *name, size_t name_len)
{
   vnode_t * new_node;

   ASSERT_MSG(name_len, "\tvfs_node_alloc(): !name_len failed");
   if (!name_len)
   {
      return NULL;
   }
   ASSERT_MSG(*name && name_len <= FS_NAME_MAX, "\tvfs_node_alloc(): !(*name) || name_len > FS_NAME_MAX failed");
   if (!(*name) || name_len > FS_NAME_MAX)
   {
      return NULL;
   }
   new_node = (vnode_t *) ciaaPOSIX_malloc(sizeof(vnode_t));
   ASSERT_MSG(NULL != new_node, "\tvfs_node_alloc(): !(*name) || name_len > FS_NAME_MAX failed");
   if (NULL == new_node)
   {
      return NULL;
   }
   ciaaPOSIX_memset(new_node, 0, sizeof(vnode_t));
   ciaaPOSIX_strncpy(new_node->f_info.file_name, name, name_len);
   new_node->f_info.file_name[name_len] = '\0';
   new_node->f_info.file_namlen = name_len;

   return new_node;
}

extern int vfs_node_free(vnode_t *node)
{
   /* Iterate over all the siblings until node found. Then link previous node to next node */
   if(NULL != node)
      ciaaPOSIX_free(node);
   return 0;
}

/* TODO: If error, eliminate the subtree */
static int vfs_inode_reserve(const char *path, vnode_t **inode_p)
{
   int ret;
   vnode_t *inode, *child;
   char *p_start, *p_end, *aux_path;

   aux_path = (char *)path;
   ret=vfs_inode_search(&aux_path, &inode);
   ASSERT_MSG(ret, "\tvfs_inode_reserve(): Node already exists");
   if(!ret)
   {
      /* node already exists */
      return -1;
   }
   /* Now inode points to the last found node in path, por lo que hay que empezar creando inode->child
    * inode->child_node will be the first non existing element in the new subtree
    * aux_path points to the first character of the first non existent element in path. So inode->child_node
    * should be named as this element
    */
   p_start=p_end=aux_path;
   for(p_start=aux_path; *p_start=='/'; p_start++);   /* skip initial '/'s */
   while(1)
   {
      for(p_end=p_start; *p_end!='\0' && *p_end!='/'; p_end++);
      /* Now p_start and p_end point to the next element in path string.
       * "node" should point to the new node
       */
      if(p_end == p_start)
      {
         return -1;
      }
      child = vfs_create_child(inode, p_start, (uint32_t)(p_end-p_start), 0);
      *inode_p = child;
      ASSERT_MSG(NULL != child, "\tvfs_inode_reserve(): vfs_create_child() failed");
      if(NULL == child)
      {
         return -1;
      }
      inode=child;
      for(p_start=p_end;*p_start=='/';p_start++);
      /* When the loop finishes, p_start points to the first character after '/' */
      if('\0' == *p_start)/* End of the searched path reached */
      {
         /* Last node found is the requested node */
         return 0;
      }
      /* Must go onto the next iteration. Created node is directory by default */
      child->n_info.type = VFS_FTDIR;
   }
}

static int vfs_inode_search(char **path_p, vnode_t **ret_node_p)
{
   uint16_t pnamelen; /* Path element namelength */
   char *p_start, *p_end;
   uint32_t ret;
   vnode_t *node;

    if (NULL == *path_p || (*path_p)[0] == '\0' || *(path_p)[0] != '/')
   {
      *ret_node_p = NULL;
      return -1;
   }

   *ret_node_p = node = vfs_root_inode;

   p_start = p_end = *path_p;
   /* When the loop finishes, p_start points to the first characer after '/' */
   for(;*p_start=='/';p_start++);
   *path_p = p_start;
   if('\0' == *p_start)   /* Path only has '/'. Return root inode */
   {
      *ret_node_p=vfs_root_inode;
      return 0;
   }
   for(p_end=p_start; '\0' != *p_end && '/' != *p_end; p_end++);
   node = node->child_node;   /* Start at the first level after root */
   /* Loop: If node is null in this iteration, it means that in this level, none of the siblings
    * matches the path element. The path is then not valid.
    */
   while(NULL != node)
   {
      /* p_start and p_end point to the beginning and end of the element in the path string to be
       * processed in this iteration.
       * p_start points to the first character of the element.
       * p_end points to the character next to the last character of the element
       */
      pnamelen = p_end-p_start;   /* Length of the name between '/' not counting '\0' */
      while(NULL != node)
      {
         ret = ciaaPOSIX_strncmp(p_start, node->f_info.file_name, pnamelen);
         if(!ret)
         {
            /* Names match. Must analyze the child node */
            break;
         }
      /* node name does not match with the path element. Try with the next node in the current level */
         node = node->sibling_node;
      }
      if(NULL == node)
      {
         /* Didnt find a node in this level whose name matches the path element searched.
          * So this is an invalid path
          * ret_node_p will point to the last matching node found
          */
         return -1;
      }
      /* The path element matches with the name of this node.
       * Iterate again in the next element of the tree to find the node matching the next element in path
       *
       */
      *ret_node_p = node;
      node = node->child_node;
      p_start = p_end;
      /* When the loop finishes, p_start points to the first char after '/' */
      for(; '/' == *p_start; p_start++);
      *path_p = p_start;
      if('\0' == *p_start)   /* The end of the searched path has been reached */
      {
         /* Last node found is the one that must be returned. Success */
         return 0;
      }
      /* When this loop finishes, p_end points to the '/' after the last path element found.
       * If the last path element found is the last elment of the searched path,
       * p_end will point to the '\0' character of the path string.
       */
      for(p_end = p_start; '\0' != *p_end && '/' != *p_end; p_end++);
   }
   /* The path contains more elements than the tree depth.
    * Searched node doesnt exist. Failure
    */
   return -1;
}

static filesystem_driver_t *vfs_get_driver(const char *driver_name)
{
   int i;

   for(i=0; NULL != vfs_fsdriver_table[i]; i++)
   {
      if(!ciaaPOSIX_strcmp(driver_name, vfs_fsdriver_table[i]->driver_name))
         return vfs_fsdriver_table[i];
   }
   return NULL;
}

/*#######################FILE_DESC######################*/

static int file_descriptor_table_init(void)
{
   ciaaPOSIX_memset(file_desc_tab_p, 0, sizeof(file_descriptor_table_t));
   return 0;
}

static file_desc_t *file_desc_create(vnode_t *node)
{
   file_desc_t *file;
   uint16_t i;

   if(file_desc_tab_p->n_busy_desc >= FILE_DESC_MAX)
      return NULL;

   for(i=0; i<FILE_DESC_MAX; i++)
   {
      if(NULL == file_desc_tab_p->table[i])
         break;
   }
   ciaaPOSIX_printf("file_desc_create(): Indice obtenido: %d\n", i);
   /* Ahora i es el indice del primer file_desc desocupado en la tabla */
   if(FILE_DESC_MAX == i)   /* No hay file_desc libres */
      return NULL;
   /* Allocate new descriptor */
   file = (file_desc_t *) ciaaPOSIX_malloc(sizeof(file_desc_t));
   if(NULL == file)
      return NULL;
   ciaaPOSIX_memset(file, 0, sizeof(file_desc_t));
   file->node = node;

   file_desc_tab_p->table[i] = file;
   file_desc_tab_p->n_busy_desc++;

   file->node = node;
   file->index = i;

   return file;
}

static int file_desc_destroy(file_desc_t *file_desc)
{
   uint16_t index;

   index = file_desc->index;
   ciaaPOSIX_free(file_desc_tab_p->table[index]);
   file_desc_tab_p->table[index] = NULL;
   if(file_desc_tab_p->n_busy_desc)
      file_desc_tab_p->n_busy_desc--;
   return 0;
}

file_desc_t *file_desc_get(uint16_t index)
{

   if(index >= FILE_DESC_MAX)
      return NULL;

   return file_desc_tab_p->table[index];
}



/*########################/FILE_DESC#####################*/

/*########################AUXILIAR#####################*/
static int vfs_print_tree_rec(vnode_t *node, int level)
{
   int ret, i;

   if(NULL == node)
   {
      return 0;
   }
   for(i=0; i<level; i++)
      ciaaPOSIX_printf("---");
   ciaaPOSIX_printf("%s\n",node->f_info.file_name);

   ret=vfs_print_tree_rec(node->child_node, level+1);
   if(ret)
   {
      return -1;
   }
   ret=vfs_print_tree_rec(node->sibling_node, level);
   if(ret)
   {
      return -1;
   }

   return 0;
}

int vfs_print_tree(void)
{
   return vfs_print_tree_rec(vfs_root_inode, 0);
}
/*########################/AUXILIAR#####################*/

/*==================[external functions definition]==========================*/

extern vnode_t *vfs_create_child(vnode_t *parent, const char *name, size_t namelen, mode_t mode)
{
   vnode_t *child;

   child = vfs_node_alloc(name, namelen);
   ASSERT_MSG(NULL != child, "\tvfs_create_child(): vfs_node_alloc() failed");
   if(NULL == child)
      return NULL;
   child->sibling_node = parent->child_node;
   parent->child_node = child;
   child->parent_node = parent;
   /* Copio la informacion de filesystem. No hago copia nueva, solo apunto a lo del padre */
   ciaaPOSIX_memcpy(&(child->fs_info), &(parent->fs_info), sizeof(filesystem_info_t));
   child->f_info.file_name[namelen] = '\0';
   child->f_info.file_pointer = 0;
   child->f_info.down_layer_info = NULL;
   return child;
}

extern int vfs_delete_child(vnode_t *child)
{
   vnode_t *node_p, *auxnode_p;
   int ret;
   /* child->sibling_node == NULL: Single child
    * child->parent_node == NULL: Root inode
    * Must delete child. Special case if child to delete is head child.
    * General case: 
    * 
    * 
    */
   ASSERT_MSG(NULL != child, "vfs_delete_child(): Node doesnt exist");
   if(NULL == child)
   {
      /* Node does not exist. Error */
      return -1;
   }
   ASSERT_MSG(NULL != child->parent_node, "vfs_delete_child(): Cant delete root inode");
   if(NULL == child->parent_node)
   {
      /* Root inode. Cant delete it */
      return -1;
   }
   /* Find previous sibling node (not a double-linked list :() */
   node_p = child->parent_node->child_node;   /* Head of children list */
   ASSERT_MSG(NULL != node_p, "vfs_delete_child(): Parent dont match child tree");
   if(NULL == node_p)
   {
      return -1;
   }
   auxnode_p = NULL;
   while(node_p != child && NULL != node_p)
   {
      auxnode_p = node_p;
      node_p = node_p->sibling_node;
   }
   /* now auxnode_p points to the previous sibling of node_p */
   if(NULL == node_p)
   {
      /* Could not find this node in the list of children of its own parent. Parents dont match, should never happen.
       * System consistency error.
       */
      return -1;
   }
   auxnode_p->sibling_node = node_p->sibling_node;
   ret = vfs_node_free(child);
   if(ret)
   {
      return -1;
   }

   return 0;
}

extern int ciaaFS_init(void)
{
   vnode_t *aux_inode;
   int ret;
   ciaaDevices_deviceType * device;

   vfs_root_inode = (vnode_t *) ciaaPOSIX_malloc(sizeof(vnode_t));
   if(NULL == vfs_root_inode)
   {
      return -1;
   }
   vfs_root_inode->n_info.is_mount_dir = true;
   vfs_root_inode->n_info.type = VFS_FTDIR;
   vfs_root_inode->f_info.file_name[0] = '\0';
   ret = file_descriptor_table_init();
   ASSERT_MSG(-1 != ret, "vfs_init(): file_descriptor_table_init() failed");
   if(ret)
   {
      return -1;
   }
   /* Crear dispositivos */
   ret = vfs_inode_reserve("/dev/block/fd/0", &aux_inode);
   ASSERT_MSG(-1 != ret, "vfs_init(): vfs_inode_reserve() failed");
   if(NULL == aux_inode)
   {
      /* No se pudo crear el nodo */
      return -1;
   }

   aux_inode->n_info.type = VFS_FTBLK;
   aux_inode->fs_info.device = ciaaDevices_getDevice("/dev/block/fd/0");
   ASSERT_MSG(NULL != aux_inode->fs_info.device, "vfs_init(): vfs_get_driver() failed");
   if(NULL == aux_inode->fs_info.device)
   {
      return -1;
   }
   device = ciaaBlockDevices_open("/dev/block/fd/0", (ciaaDevices_deviceType *)aux_inode->fs_info.device, ciaaPOSIX_O_RDWR);
   ASSERT_MSG(NULL != device, "vfs_init(): failed to open device");

   aux_inode->fs_info.drv = vfs_get_driver("BLOCKDEV");
   ASSERT_MSG(NULL != aux_inode->fs_info.drv, "vfs_init(): vfs_get_driver() failed");
   if(NULL == aux_inode->fs_info.drv)
   {
      return -1;
   }

   ret = vfs_inode_reserve("/dev/char/fd/0", &aux_inode);
   ASSERT_MSG(-1 != ret, "vfs_init(): vfs_inode_reserve() failed");
   if(NULL == aux_inode)
   {
      /* No se pudo crear el nodo */
      return -1;
   }

   return 0;
}

/** \brief Format a device
 *
 *
 *
 */
extern int ciaaFS_format(const char *device_path, const char *fs_type)
{
   vnode_t *devnode;
   char *devpath;
   filesystem_driver_t *fs_driver;
   int ret;

   devpath = (char *)device_path;
   ret = vfs_inode_search(&devpath, &devnode);
   ASSERT_MSG(0 == ret, "mount(): format() failed. Device doesnt exist");
   if(ret)
   {
      /* El nodo del device no existe */
      return -1;
   }

   fs_driver = vfs_get_driver(fs_type);
   ASSERT_MSG(NULL != fs_driver, "format(): Device node not valid");
   if(NULL == fs_driver)
   {
      /* No existe un driver con el nombre dado */
      return -1;
   }

   ASSERT_MSG(VFS_FTBLK == devnode->n_info.type, "format(): Target file not a device");
   if(VFS_FTBLK != devnode->n_info.type)
   {
      /* El supuesto nodo device no es device */
      return -1;
   }

   /* Llamo a la funcion de bajo nivel */
   ret = fs_driver->driver_op->fs_format(devnode);
   ASSERT_MSG(0 == ret, "format(): Lower layer format failed");
   if(ret)
   {
      /* Fallo el format de bajo nivel */
      return -1;
   }
   return 0;
}

/** \brief VFS mount
 *
 * Ir a la definicion de POSIX
 *
 */
extern int ciaaFS_mount(char *device_path,  char *target_path, char *fs_type)
{
   char *devpath, *tpath;
   filesystem_driver_t *fs_driver;
   int ret;
   vnode_t *targetnode, *devnode;

   tpath = target_path;
   ret = vfs_inode_reserve(tpath, &targetnode);
   ASSERT_MSG(0 == ret, "mount(): vfs_inode_reserve() failed creating target node");
   if(ret)
   {
      /* The directory already exists */
      /* FIXME: It should not be needed that the dir doesnt exist.
       * Should overwrite new mount in node
       */
      return -1;
   }
   devpath = device_path;
   ret = vfs_inode_search(&devpath, &devnode);
   ASSERT_MSG(0 == ret, "mount(): vfs_inode_search() failed. Device doesnt exist");
   if(ret)
   {
      /* The device node doesnt exist */
      return -1;
   }
   ASSERT_MSG(VFS_FTBLK == devnode->n_info.type, "mount(): Device node not valid");
   if(VFS_FTBLK != devnode->n_info.type)
   {
      /* Not a device node */
      return -1;
   }
   fs_driver = vfs_get_driver(fs_type);
   ASSERT_MSG(NULL != fs_driver, "mount(): Device node not valid");
   if(NULL == fs_driver)
   {
      /* Driver doesnt exist */
      return -1;
   }
   /* Fill the target directory vnode fields */
   targetnode->fs_info.drv = fs_driver;
   targetnode->fs_info.device = devnode->fs_info.device;
   targetnode->n_info.is_mount_dir = true;
   /* Call the lower layer method */
   ret = fs_driver->driver_op->fs_mount(devnode, targetnode);
   ASSERT_MSG(0 == ret, "mount(): Lower layer mount failed");
   if(ret)
   {
      /* Lower layer mount failed */
      return -1;
   }
   return 0;
}

/* Cant support over mounting. Directory must be empty to mount. 
 * Umount eliminates mount root node
 */
/* TODO: Check if exists an open file who belongs to this mount */
extern int ciaaFS_umount(const char *target_path)
{
   char *tpath;
   filesystem_driver_t *fs_driver;
   int ret;
   vnode_t *targetnode;

   tpath = (char *)target_path;
   ret = vfs_inode_search(&tpath, &targetnode);
   ASSERT_MSG(0 == ret, "umount(): vfs_inode_search() failed. Mountpoint doesnt exist");
   if(ret)
   {
      /* The device node doesnt exist */
      return -1;
   }
   ASSERT_MSG(true == targetnode->n_info.is_mount_dir, "umount(): Target not a mountpoint");
   if(true != targetnode->n_info.is_mount_dir)
   {
      /* Not a mountpoint */
      return -1;
   }

   /* Call the lower layer method */
   fs_driver = targetnode->fs_info.drv;
   if(NULL == fs_driver->driver_op->fs_umount)
   {
      /* method not supported */
      return -1;
   }
   ret = fs_driver->driver_op->fs_umount(targetnode);
   ASSERT_MSG(0 == ret, "mount(): Lower layer mount failed");
   if(ret)
   {
      /* Lower layer mount failed */
      return -1;
   }
   return 0;
}

/*
 *   TODO: Current implementation can create a subtree. Should only create a leaf. Validate path.
 */
extern int ciaaFS_mkdir(const char *dir_path, mode_t mode)
{
  vnode_t *dir_inode_p, *parent_inode_p;
  int               ret;

   char *tpath = (char *) dir_path;
   /* Create an empty node in dir_path */
   /* vfs_inode_reserve(): The new node inherits the fathers fs info */
   ret = vfs_inode_reserve(tpath, &dir_inode_p);
   if(ret)
   {
      /* Directory already exists */
      return -1;
   }
   /* Fill the vnode fields */
   dir_inode_p->n_info.type = VFS_FTDIR;
   parent_inode_p = dir_inode_p->parent_node;
   ret = (dir_inode_p->fs_info).drv->driver_op->fs_create_node(parent_inode_p, dir_inode_p);
   if(ret)
   {
      /* Could not create lower layer node. Handle the issue */
      return -1;
   }

   return 0;
}

extern int ciaaFS_rmdir(const char *dir_path)
{
   vnode_t *target_inode;
   int               ret;
   char *auxpath;

   auxpath = (char *) dir_path;
   ret = vfs_inode_search(&auxpath, &target_inode);   /* Return 0 if node found */
   ASSERT_MSG(0 == ret, "mount(): vfs_inode_search() failed. Device doesnt exist");
   if(ret)
   {
      /* The directory doesnt exist */
      return -1;
   }

   if(NULL == target_inode->parent_node)
   {
      /* Cannot erase root */
      return -1;
   }

   /* TODO */

   return 0;
}

/*
 *   TODO: Current implementation can create a subtree. Should only create a leaf. Validate path.
 */
extern int ciaaFS_open(const char *path, int flags) {

   vnode_t *target_inode, *parent_inode;
   file_desc_t *file;
   char *auxpath;
   int ret, fd;

   auxpath = (char *) path;
   ret = vfs_inode_search(&auxpath, &target_inode);   /* Return 0 if node found */
   if (ret)
   {
      /* Node does not exist. Must create a new file if VFS_O_CREAT set */
      if (flags & VFS_O_CREAT)
      {
         /* Create empty node in dir_path */
         /* vfs_inode_reserve(): The new node inherits the fathers fs info */
         auxpath= (char *)path;
         ret = vfs_inode_reserve(auxpath, &target_inode);
         ASSERT_MSG(NULL != target_inode, "open(): vfs_inode_reserve() failed");
         if(NULL == target_inode)
         {
            /* Error when allocating node */
            return -1;
         }
         target_inode->n_info.type = VFS_FTREG;
         /* Create node in lower layer */
         parent_inode = target_inode->parent_node;
         ret = target_inode->fs_info.drv->driver_op->fs_create_node(parent_inode, target_inode);
         ASSERT_MSG(ret>=0, "open(): fs_create_node() failed");
         if(ret)
         {
            /* Could not create lower layer node. Handle the issue */
            return -1;
         }
      }
      else
      {
         /* Node does not exist, but VFS_O_CREAT not set. Error */
         return -1;
      }
   }
   else      /* El nodo ya existe, no hay que crearlo, solo abrir el archivo */
   {
      /* Node exists. Only open the file.
       * If O_CREAT was set, if the file already exists it is an error.
       */
      if ((flags & VFS_O_EXCL) && (flags & VFS_O_CREAT))
      {
         return -1;
      }
      /* TODO: Contemplate more cases of filetypes */
      if(VFS_FTDIR == target_inode->n_info.type || VFS_FTUNKNOWN == target_inode->n_info.type)
      {
         return -1;
      }
   }

   file = file_desc_create(target_inode);
   ASSERT_MSG(NULL != file, "open(): file_desc_create() failed");
   if(NULL == file)
   {
      return -1;
   }
   fd = file->index;
   ret = file->node->fs_info.drv->driver_op->file_open(file);
   ASSERT_MSG(ret >= 0, "open(): file_open() failed");
   if(ret)
   {
      file_desc_destroy(file);
      return -1;
   }

   return fd;
}

extern int ciaaFS_close(int fd)
{
   file_desc_t *file;
   ssize_t ret;

   /* Get the file object from the file descriptor */
   file = file_desc_get(fd);
   ASSERT_MSG(NULL != file, "read(): file_desc_get() failed");
   if(NULL == file)
   {
      /* Invalid file descriptor */
      return -1;
   }
   ret = file_desc_destroy(file);
   if(ret)
   {
      return -1;
   }
   return 0;
}

extern ssize_t ciaaFS_read(int fd, void *buf, size_t nbytes)
{
   file_desc_t *file;
   ssize_t ret;

   /* Get the file object from the file descriptor */
   file = file_desc_get(fd);
   ASSERT_MSG(NULL != file, "read(): file_desc_get() failed");
   if(NULL == file)
   {
      /* Invalid file descriptor */
      return -1;
   }

   /* Verify that the lower layer driver implements the read operation */
   if(NULL == file->node->fs_info.drv->driver_op->file_read)
   {
      /* Driver doesnt support read */
      return 1;
   }
   /* Lower layer read operation */
   ret = file->node->fs_info.drv->driver_op->file_read(file, buf, nbytes);
   ASSERT_MSG(ret == nbytes, "read(): file_read() failed");
   if(ret != nbytes)
   {
      /* TODO: Set ERROR to show that could not read nbytes */
      return ret;
   }
   return ret;
}

extern ssize_t ciaaFS_write(int fd, void *buf, size_t nbytes)
{
   file_desc_t *file;
   ssize_t ret;

   /* Get the file object from the file descriptor */
   file = file_desc_get(fd);
   ASSERT_MSG(NULL != file, "read(): file_desc_get() failed");
   if(NULL == file)
   {
      /* Invalid file descriptor */
      return -1;
   }

   /* Verify that the lower layer driver implements the write operation */
   if(NULL == file->node->fs_info.drv->driver_op->file_write)
   {
      /* Driver doesnt support write */
      return 1;
   }
   /* Lower layer write operation */
   ret = file->node->fs_info.drv->driver_op->file_write(file, buf, nbytes);
   ASSERT_MSG(ret == nbytes, "write(): file_write() failed");
   if(ret!=nbytes)
   {
      /* TODO: Set ERROR to show that could not write nbytes */
      return ret;
   }
   return ret;
}

extern ssize_t ciaaFS_lseek(int fd, ssize_t offset, int whence)
{
   file_desc_t *file;
   ssize_t pos;

   /* Get the file object from the file descriptor */
   file = file_desc_get(fd);
   ASSERT_MSG(NULL != file, "read(): file_desc_get() failed");
   if(NULL == file)
   {
      /* Invalid file descriptor */
      return -1;
   }
   pos=0;
   switch(whence)
   {
      case SEEK_END:
         pos = file->node->f_info.file_size + offset;
         break;
      case SEEK_CUR:
         pos = file->node->f_info.file_pointer + offset;
         break;
      default:
         pos = offset;
         break;
   }

   if ((pos >= 0) && (pos < file->node->f_info.file_size))
   {
      file->node->f_info.file_pointer = (uint32_t) pos;
   }

   return file->node->f_info.file_pointer;
}

extern int ciaaFS_unlink(const char *path)
{
   vnode_t *target_inode;
   filesystem_driver_t *driver;
   char *auxpath;
   int ret;

   auxpath = (char *) path;
   ret = vfs_inode_search(&auxpath, &target_inode);   /* Return 0 if node found */
   if(ret)
   {
      /* File not found */
      return -1;
   }
   if(VFS_FTREG != target_inode->n_info.type)
   {
      /* Not a regular file, can not unlink */
      return -1;
   }
   driver = target_inode->fs_info.drv;
   if(NULL == driver)
   {
      /*No driver. Fatal error*/
      return -1;
   }
   if(NULL == driver->driver_op->fs_delete_node)
   {
      /*The filesystem driver does not support this method*/
      return -1;
   }
   ret = driver->driver_op->fs_delete_node(target_inode->parent_node, target_inode);
   if(ret)
   {
      /* Filesystem op failed */
      return -1;
   }
   /* TODO: Remove node from vfs */
   ret = vfs_delete_child(target_inode);
   if(ret)
   {
      /* Filesystem op failed */
      return -1;
   }
   return 0;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

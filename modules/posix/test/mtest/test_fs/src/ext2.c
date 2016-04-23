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
 * MZ      Marcos Ziegler
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160101 v0.0.1 MZ initial version
 */

/*
TODO:
-Implementar todo con memoria estatica, salvo excepciones donde se justifique
-Optimizar algoritmos tanto en memoria como en velocidad
-Implementar create_node, write. En general todos los algoritmos de reserva de bloques y nodos
-Crear nuevos campos en ext2_finfo para asi no tener que calcularlos cada vez que se hace una operacion. Ej: Grupo actual, bloques libres, etc.
-Implementar codigos de error. Estoy devolviendo -1 para todos los errores
-Fix "addtogroup" description from doxygen
-Add more commenta
-Translate comments to english

FIXME:
-Reducir la cantidad de returns para una misma funcion. Utilizar una variable
-Revisar tipos de datos declarados (uint32_t, int32_t, int16_t, etc.). Justificar cada tipo de dato utilizado
*/

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdlib.h"
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "ext2.h"
#include "vfs.h"

/*==================[macros and definitions]=================================*/

#define ASSERT_MSG(cond, msg) assert_msg((cond), (msg), __FILE__, __LINE__)

/*==================[internal data declaration]==============================*/
/*==================[internal functions declaration]=========================*/

/** \brief Adds an entry in the directory with given parameters
 **
 ** 
 **
 ** \param[in] node directory node in which to add entry
 ** \param[in] ino the new entry will have this inode number
 ** \param[in] name the new entry will have this name
 ** \param[in] namlen the new entry will have this namlen
 ** \param[in] type the new entry will have this file type
 **    EXT2_FT_UNKNOWN		-Unknown File Type
 **    EXT2_FT_REG_FILE		-Regular File
 **    EXT2_FT_DIR		-Directory File
 **    EXT2_FT_CHRDEV		-Character Device
 **    EXT2_FT_BLKDEV		-Block Device
 **    EXT2_FT_FIFO		-Buffer File
 **    EXT2_FT_SOCK		-Socket File
 **    EXT2_FT_SYMLINK		-Symbolic Link
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_dir_add_entry(vnode_t *node, uint32_t ino, char *name, uint16_t namlen, uint8_t type);

/** \brief Delete an entry in the given directory
 **
 ** 
 **
 ** \param[in] node directory node in which to delete entry
 ** \param[in] name name of the entry to be deleted
 ** \param[in] namlen length of the name of the entry to be deleted
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_dir_delete_entry(vnode_t *node, char *name, uint16_t namlen);

/** \brief print information about the entries in a directory
 **
 ** \param[in] dir_node node whose contents are to be printed
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_dir_print(vnode_t *dir_node);

/** \brief maps a block number in sequential file position to the corresponding disk block number
 **
 ** \param[in] dest_node node of file in which to map the block
 ** \param[in] file_block file relative block number
 ** \param[out] device_block_p pointer to the returned device block number
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_block_map(const vnode_t * dest_node, uint32_t file_block, uint32_t *device_block_p);

/** \brief find a free block and mark it as used
 **
 ** \param[in] node, whose file is assumed to be the future owner of the reserved block
 ** \param[out] block pointer to the device block number, newly allocated
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_alloc_block_bit(vnode_t * node, uint32_t *block);

/** \brief mark the given block as free
 **
 ** \param[in] node, whose file is assumedly the owner of the block to be removed
 ** \param[in] block, block number of the block to be unallocated
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_dealloc_block_bit(vnode_t * node, uint32_t block);

/** \brief find a free inode and mark it as used
 **
 ** \param[in] node the new allocated disk inode will be linked to this node
 ** \param[out] new_inumber_p pointer to the new allocated inode
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_alloc_inode_bit(vnode_t *node, uint32_t *new_inumber_p);

/** \brief mark the given inode as free
 **
 ** \param[in] node whose current physical inode is to be deallocated
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_dealloc_inode_bit(vnode_t *node);

/** \brief maps a block number in sequential file position to the corresponding disk block number
 **
 ** \param[in] dest_node node of file in which to map the block
 ** \param[in] file_block file relative block number
 ** \param[out] device_block_p pointer to the returned device block number
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_file_map_alloc(vnode_t *node, uint32_t file_block, uint32_t *device_block_p);

/** \brief read file data to a buffer
 **
 ** \param[in] dest_node node from which to read data
 ** \param[in] buf buffer to which copy data read
 ** \param[in] size size of the data to read, in bytes
 ** \param[out] total_read_size_p pointer to the variable indicating how many bytes were read
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_buf_read_file(vnode_t *dest_node, uint8_t *buf, uint32_t size, uint32_t *total_read_size_p);

/** \brief Read superblock data from formatted device
 **
 ** \param[in] device device from which to read the superblock
 ** \param[in] sb_p pointer to the structure containing the superblock raw data
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_get_superblock(ciaaDevices_deviceType const * device, struct ext2sb * sb_p);

/** \brief Write the on-memory superblock and group descriptors to disk
 **
 ** \param[in] node node whose filesystem information is to be sync with disk data
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_fsinfo_flush(vnode_t *node);

/** \brief Load all directories and files starting from root recursively to the vfs tree
 **
 ** \param[in] dir_node directory to be the root of the new mount
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_mount_load(vnode_t *dir_node);

/** \brief auxiliary function for ext2_mount_load()
 **
 ** \param[in] dir_node subtree directory to mount recursively
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_mount_load_rec(vnode_t *dir_node);

/** \brief print the formatted contents of the given ext2 superblock structure
 **
 ** \param[in] superblock pointer to the raw superblock structure to be printed
 ** \return -1 if an error occurs, in other case 0
 **/
static void print_superblock(struct ext2sb * superblock);

/** \brief print the contents of the given ext2 group descriptor structure 
 **
 ** \param[in] gd pointer to the raw group descriptor structure to be printed
 ** \return -1 if an error occurs, in other case 0
 **/
static void print_groupdescriptor(struct ext2_gd *gd);

/** \brief print the contents of the given ext2 inode structure
 **
 ** \param[in] inode pointer to the raw group descriptor structure to be printed
 ** \return -1 if an error occurs, in other case 0
 **/
static void print_inode(struct ext2fs_dinode * inode);

/** \brief read from disk an inode to memory
 **
 ** \param[in] dest_node node in which to save the physical inode
 ** \param[in] inumber number of inode to be read to memory
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_read_inode(vnode_t *dest_node, uint32_t inumber);

/** \brief write from memory an inode to disk
 **
 ** \param[in] dest_node node whose physical raw node is to be sync to disc
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_write_inode(vnode_t *dest_node);

/** \brief search an entry in a directory by its fields
 **
 ** \param[in] dir_node directory in which to look for the entry
 ** \param[in] dir_name name of the entry to be searched
 ** \param[in] length length of dir_name
 ** \param[out] inumber_p points to inode number if success, NULL otherwise
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_search_directory(vnode_t *dir_node, const char *dir_name, int length, uint32_t *inumber_p);

/** \brief map the ext2 file type code to the vfs file type code
 **
 ** \param[in] ext2_type ext2 file type
 **   EXT2_FT_UNKNOWN
 **   EXT2_FT_REG_FILE
 **   EXT2_FT_DIR
 **   EXT2_FT_CHRDEV
 **   EXT2_FT_BLKDEV
 **   EXT2_FT_FIFO
 **   EXT2_FT_SOCK
 **   EXT2_FT_SYMLINK
 ** \return the vfs file type according to the input parameter
 **    VFS_FTREG,   //Regular file node
 **    VFS_FTDIR,   //Directory file node
 **    VFS_FTBLK,   //Block device node
 **    VFS_FTUNKNOWN   //Unknown
 **/
static node_type_t ext2type_to_vfstype(uint8_t ext2_type);

/** \brief create a new EXT2_S_IFREG regular file
 **
 ** \param[in] parent_node parent of the new node to be created
 ** \param[in] node node to be created in the file system
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_create_regular_file(vnode_t *parent_node, vnode_t *node);

/** \brief create a new EXT2_S_IFDIR directory file
 **
 ** \param[in] parent_node parent of the new node to be created
 ** \param[in] node node to be created in the file system
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_create_directory_file(vnode_t *parent_node, vnode_t *node);

/** \brief delete the EXT2_S_IFDIR directory file given
 **
 ** \param[in] parent_node parent of the node to be deleted
 ** \param[in] node node to be removed from the file system
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_delete_directory_file(vnode_t *parent_node, vnode_t *node);

/** \brief delete the EXT2_S_IFREG regular file given
 **
 ** \param[in] parent_node parent of the node to be deleted
 ** \param[in] node node to be removed from the file syst
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_delete_regular_file(vnode_t *parent_node, vnode_t *node);

/** \brief open the given file
 **
 ** \param[in] file file to be opened
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_file_open(struct file_desc *file);

/** \brief close the given file
 **
 ** \param[in] file file to be closed
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_file_close(struct file_desc *file);

/** \brief read the file data
 **
 ** \param[in] file file from which to read data
 ** \param[in] buf buffer whom to fill with data
 ** \param[in] size size of data to be read in bytes
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **/
static size_t ext2_file_read(struct file_desc *file, void *buf, size_t size);

/** \brief write data to the file
 **
 ** \param[in] file file from which to read data
 ** \param[in] buf buffer whom to fill with data
 ** \param[in] size size of data to be read in bytes
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **/
static size_t ext2_file_write(struct file_desc *file, void *buf, size_t size);

/** \brief throw a command to the file
 **
 ** \param[in] file file to be controlled
 ** \param[in] request command to be executed to the file
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_file_ioctl(struct file_desc *file, int request);

/** \brief init the file system
 ** TODO
 ** \param[in]
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_init(void *par);

/** \brief format a device with ext2 format
 **
 ** \param[in] dev_node node of the device to be formatted
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_format(vnode_t *dev_node);

/** \brief mount a disk to a directory node
 **
 ** \param[in] dev_node node of the device to be mounted
 ** \param[in] dest_node rooot directory of the new mount
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_mount(vnode_t *dev_node, vnode_t *dest_node);

/** \brief create a new file
 **
 ** \param[in] parent_node parent of the new node
 ** \param[in] child_node node to be created in disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_create_node(vnode_t *parent_node, vnode_t *child_node);

/** \brief delete given file
 **
 ** \param[in] parent_node parent of the node to be deleted
 ** \param[in] child_node node to be erased in disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_delete_node(vnode_t *parent_node, vnode_t *child_node);

/** \brief truncate given file data
 ** TODO
 ** \param[in] node
 ** \param[in] length
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_truncate(vnode_t *node, uint32_t length);

/** \brief umount device from directory
 **
 ** \param[in]
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_umount(vnode_t *node);

/*==================[internal data definition]===============================*/

/** \brief ext2 driver operations
 *
 */
struct fsdriver_operations ext2_driver_operations =
{
   ext2_file_open,
   ext2_file_close,
   ext2_file_read,
   ext2_file_write,
   ext2_file_ioctl,

   ext2_init,
   ext2_format,
   ext2_mount,
   ext2_create_node,
   ext2_delete_node,
   ext2_truncate,
   ext2_umount
};

/*==================[external data definition]===============================*/

/** \brief ext2 driver structure
 *
 * It will be declared as "extern" in vfs.c, so its visible to function vfs_get_driver()
 */
struct filesystem_driver ext2_driver =
{
   "EXT2",
   &ext2_driver_operations
};

/*==================[internal functions definition]==========================*/

/** \brief Only for debugging purposes
 *
 * FIXME: Remove or modify
 */
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

int ext2_get_superblock(ciaaDevices_deviceType const * device, struct ext2sb * sb_p)
{
   int32_t ret;
   ret = ciaaBlockDevices_lseek(device, EXT2_SBOFF, SEEK_SET);
   if(ret!=EXT2_SBOFF)
   {
      return -1;
   }
   ret = ciaaBlockDevices_read(device, (uint8_t *)sb_p, EXT2_SBSIZE);
   if(ret!=EXT2_SBSIZE)
   {
      return -1;
   }
   print_superblock(sb_p);

   return 0;
}

static void print_superblock(struct ext2sb * superblock)
{
   ciaaPOSIX_printf("s_inodes_count: %d\n", superblock->s_inodes_count);
   ciaaPOSIX_printf("s_blocks_count: %d\n", superblock->s_blocks_count);
   ciaaPOSIX_printf("s_r_blocks_count: %d\n", superblock->s_r_blocks_count);
   ciaaPOSIX_printf("s_free_blocks_count: %d\n", superblock->s_free_blocks_count);
   ciaaPOSIX_printf("s_free_inodes_count: %d\n", superblock->s_free_inodes_count);
   ciaaPOSIX_printf("s_first_data_block: %d\n", superblock->s_first_data_block);
   ciaaPOSIX_printf("s_log_block_size: %d\n", superblock->s_log_block_size);
   ciaaPOSIX_printf("s_log_frag_size: %d\n", superblock->s_log_frag_size);
   ciaaPOSIX_printf("s_blocks_per_group: %d\n", superblock->s_blocks_per_group);
   ciaaPOSIX_printf("s_frags_per_group: %d\n", superblock->s_frags_per_group);
   ciaaPOSIX_printf("s_inodes_per_group: %d\n", superblock->s_inodes_per_group);
   ciaaPOSIX_printf("s_mtime: %d\n", superblock->s_mtime);
   ciaaPOSIX_printf("s_wtime: %d\n", superblock->s_wtime);
   ciaaPOSIX_printf("s_mnt_count: %d\n", superblock->s_mnt_count);
   ciaaPOSIX_printf("s_max_mnt_count: %d\n", superblock->s_max_mnt_count);
   ciaaPOSIX_printf("s_magic: %#06x\n", superblock->s_magic);
   ciaaPOSIX_printf("s_state: %d\n", superblock->s_state);
   ciaaPOSIX_printf("s_errors: %d\n", superblock->s_errors);
   ciaaPOSIX_printf("s_minor_rev_level: %d\n", superblock->s_minor_rev_level);
   ciaaPOSIX_printf("s_lastcheck: %d\n", superblock->s_lastcheck);
   ciaaPOSIX_printf("s_checkinterval: %d\n", superblock->s_checkinterval);
   ciaaPOSIX_printf("s_creator_os: %d\n", superblock->s_creator_os);
   ciaaPOSIX_printf("s_rev_level: %d\n", superblock->s_rev_level);
   ciaaPOSIX_printf("s_def_resuid: %d\n", superblock->s_def_resuid);
   ciaaPOSIX_printf("s_def_resgid: %d\n", superblock->s_def_resgid);
   ciaaPOSIX_printf("s_first_ino: %d\n", superblock->s_first_ino);
   ciaaPOSIX_printf("s_inode_size: %d\n", superblock-> s_inode_size);
   ciaaPOSIX_printf("s_block_group_nr: %d\n", superblock->s_block_group_nr);
   ciaaPOSIX_printf("s_feature_compat: %d\n", superblock->s_feature_compat);
   ciaaPOSIX_printf("s_feature_incompat: %d\n", superblock->s_feature_incompat);
   ciaaPOSIX_printf("s_feature_ro_compat: %d\n", superblock->s_feature_ro_compat);
}

static void print_groupdescriptor(struct ext2_gd *gd)
{

   ciaaPOSIX_printf("\n**Group Descriptor**\n");
   ciaaPOSIX_printf("block_bitmap: %d\n",gd->block_bitmap);     /* Blocks bitmap block */
   ciaaPOSIX_printf("inode_bitmap: %d\n",gd->inode_bitmap);     /* Inodes bitmap block */
   ciaaPOSIX_printf("inode_table: %d\n",gd->inode_table);      /* Inodes table block */
   ciaaPOSIX_printf("free_blocks_count: %d\n",gd->free_blocks_count);   /* Free blocks count */
   ciaaPOSIX_printf("free_inodes_count: %d\n",gd->free_inodes_count);   /* Free inodes count */
   ciaaPOSIX_printf("used_dirs_count: %d\n",gd->used_dirs_count);     /* Directories count */

}

static void print_inode(struct ext2fs_dinode * inode)
{
   int i;
   ciaaPOSIX_printf("i_mode: %d\n", inode->i_mode);
   ciaaPOSIX_printf("i_uid: %d\n", inode->i_uid);
   ciaaPOSIX_printf("i_size: %d\n", inode->i_size);
   ciaaPOSIX_printf("i_atime: %d\n", inode->i_atime);
   ciaaPOSIX_printf("i_ctime: %d\n", inode->i_ctime);
   ciaaPOSIX_printf("i_mtime: %d\n", inode->i_mtime);
   ciaaPOSIX_printf("i_dtime: %d\n", inode->i_dtime);
   ciaaPOSIX_printf("i_gid: %d\n", inode->i_gid);
   ciaaPOSIX_printf("i_links_count: %d\n", inode->i_links_count);
   ciaaPOSIX_printf("i_blocks: %d\n", inode->i_blocks);
   ciaaPOSIX_printf("i_flags: %d\n", inode->i_flags);
   for(i=0; i<N_DIRECT_BLOCKS + N_INDIRECT_BLOCKS; i++)
      ciaaPOSIX_printf("Block %d: %04x\n",i, (inode->i_block)[i]);     /* blocks */
   ciaaPOSIX_printf("i_gen: %d\n",inode->i_gen);/* 100: generation number */
}

static int ext2_read_inode(vnode_t *dest_node, uint32_t inumber)
{
   size_t ret;
   int32_t inode_group, inode_offset;

   ciaaDevices_deviceType const *bdev;
   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = dest_node->fs_info.device;
   ninfo = (struct ext2fs_dinode *)dest_node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)dest_node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)dest_node->fs_info.down_layer_info;

   if(fsinfo==NULL||finfo==NULL||ninfo==NULL||bdev==NULL)
   {
      return -1;
   }

   /* Division por 0. Conflictivo. Verificar antes */
   if(fsinfo->e2sb.s_inodes_per_group==0)
   {
      return -1;
   }
   /* Calculo la posicion del inode dentro del device a partir de la informacion del fs */
   inode_group = (inumber-1)/fsinfo->e2sb.s_inodes_per_group;
   /* (inumber-1) porque se empiezan a contar desde 1 */
   /* Calculo en que grupo esta el inode destino */

   inode_offset = EXT2_SBOFF + ((((fsinfo->e2fs_gd)[inode_group]).inode_table)-1)*(fsinfo->s_block_size) +
         ((inumber-1)%fsinfo->e2sb.s_inodes_per_group)*(fsinfo->e2sb.s_inode_size);

   ret = ciaaBlockDevices_lseek(bdev, inode_offset, SEEK_SET);
   if(ret!=inode_offset)
   {
      return -1;
   }
   ret = ciaaBlockDevices_read(bdev, (uint8_t *)ninfo, sizeof(struct ext2fs_dinode));
   if(ret!=sizeof(struct ext2fs_dinode))
   {
      return -1;
   }
   finfo->f_inumber = inumber;
   finfo->f_group = inode_group;
   ciaaPOSIX_memcpy(&(finfo->f_di), ninfo, sizeof(struct ext2fs_dinode));
   print_inode(dest_node->n_info.down_layer_info);
   return 0;
}

static int ext2_write_inode(vnode_t *node)
{
   int ret;
   uint32_t offset;

   ciaaDevices_deviceType const *bdev;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   offset = ((fsinfo->e2fs_gd[finfo->f_group].inode_table) << (10+fsinfo->e2sb.s_log_block_size)) +   //Offset del inode_table donde
                                                                                                 //reside el nodo.
            ((finfo->f_inumber - 1) % fsinfo->e2sb.s_inodes_per_group) *   //Offset dentro del inode_table
            fsinfo->e2sb.s_inode_size;

   ret = ciaaBlockDevices_lseek(bdev, offset, SEEK_SET);
   if(ret!=offset)
   {
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *) &(finfo->f_di), sizeof(struct ext2fs_dinode));
   if(ret!=sizeof(struct ext2fs_dinode))
   {
      return -1;
   }

   return 0;
}

static int ext2_search_directory(vnode_t *dir_node, const char *dir_name, int length,
      uint32_t *inumber_p)
{
   uint8_t *data_pointer;
   struct   ext2fs_direct * start_pointer, * end_pointer;
   uint32_t lret; /* Para numeros grandes */

   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   ninfo = (struct ext2fs_dinode *)dir_node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)dir_node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)dir_node->fs_info.down_layer_info;

   if(ninfo==NULL||finfo==NULL||fsinfo==NULL)
   {
      return -1;
   }
   dir_node->f_info.file_pointer = finfo->f_pointer = 0;
   /* Mientras el puntero sea menor al tamanio del archivo */
   /* FIXME: Hago malloc del tamanio del archivo entero, demasiada memoria */
   data_pointer=(uint8_t*)ciaaPOSIX_malloc(finfo->f_di.i_size);
   if(data_pointer==NULL)
   {
      return -1;
   }
   ext2_buf_read_file(dir_node, (uint8_t*)data_pointer, finfo->f_di.i_size, &lret);
   if(lret!=finfo->f_di.i_size)
   {
      return -1;
   }
   for(   start_pointer=(struct ext2fs_direct *)data_pointer,
      end_pointer=(struct ext2fs_direct *)(data_pointer + finfo->f_di.i_size);
      start_pointer<end_pointer;
      start_pointer=(struct ext2fs_direct *)((char*)start_pointer + start_pointer->e2d_reclen))/* FIXME: Ver aritmetica de punteros, puede estar mal */
   {
      if(start_pointer->e2d_reclen <= 0)
      {
         break;
      }
      if(start_pointer->e2d_ino == 0)
      {
         continue;
      }
      if(   start_pointer->e2d_namlen == length &&
         !ciaaPOSIX_memcmp(dir_name, start_pointer->e2d_name, length))
      {
         //Se encontro el directorio
         *inumber_p=start_pointer->e2d_ino;
         ciaaPOSIX_free(data_pointer);
         return 0;
      }
   }

   ciaaPOSIX_free(data_pointer);
   return -1; /* Error, no se encontro directorio */
}

/* For debugging purposes */
static int ext2_dir_print(vnode_t *dir_node)
{
   uint8_t *data_pointer;
   int ret;
   uint32_t lret; /* Para numeros grandes */

   //ciaaDevices_deviceType const *bdev;
   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   //bdev = dir_node->fs_info.device;
   ninfo = (struct ext2fs_dinode *)dir_node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)dir_node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)dir_node->fs_info.down_layer_info;

   ASSERT_MSG(ninfo!=NULL&&finfo!=NULL&&fsinfo!=NULL, "ext2_dir_print(): El archivo no fue correctamente abierto");

   if(ninfo==NULL||finfo==NULL||fsinfo==NULL)
   {
      return -1;
   }
   dir_node->f_info.file_pointer = finfo->f_pointer = 0;
   data_pointer=(uint8_t*)ciaaPOSIX_malloc(fsinfo->s_block_size);/* FIXME: Hago malloc del tamanio del archivo entero */
   if(data_pointer==NULL)
   {
      return -1;
   }
   ret = ext2_buf_read_file(dir_node, (uint8_t*)data_pointer, fsinfo->s_block_size, &lret);
   if(ret)
   {
      ciaaPOSIX_free(data_pointer);
      return -1;
   }
   if(lret!=finfo->f_di.i_size)
   {
      return -1;
   }

   ciaaPOSIX_free(data_pointer);
   return 0; /* Error, no se encontro directorio */
}

static node_type_t ext2type_to_vfstype(uint8_t ext2_type)
{
   switch (ext2_type) {
   case EXT2_FT_REG_FILE: return VFS_FTREG;
   case EXT2_FT_DIR: return VFS_FTDIR;
   default: return VFS_FTUNKNOWN;
   }
}

static int ext2_file_open(struct file_desc *file)
{
   //int ret;
   struct ext2_file_info *finfo;
   //struct ext2_fs_info *fsinfo;

   //fsinfo=file->node->fs_info.down_layer_info;
   finfo=file->node->f_info.down_layer_info;
   finfo->f_pointer = 0;
   /* reset seek pointer */

   return 0;
}

static int ext2_file_close(struct file_desc *file)
{
   /* TODO: Que tendria que hacer aca? Tendria que liberar la memoria de finfo? */
   return 0;
}

static size_t ext2_file_read(struct file_desc *file, void *buf, size_t size)
{
   uint32_t read_size;
   //struct ext2_file_info *finfo;
   int ret;

   //finfo = file->node->f_info.down_layer_info;
   /*TODO: Verificar que file->cursor sea valido*/
   file->node->f_info.file_pointer = file->cursor;
   ret = ext2_buf_read_file(file->node, (uint8_t *)buf, size, &read_size);
   if(ret)
   {
      return 0;
   }
   file->cursor += read_size;
   return read_size;
}

static int ext2_file_ioctl(struct file_desc *file, int request)
{
   return 0;
}

static int ext2_init(void *par)
{
   return 0;
}

static int ext2_format(vnode_t *dev_node)
{
   int ret;
   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   //struct ext2_file_info *finfo;
   //struct ext2_fs_info *fsinfo;
   ciaaDevices_blockType blockInfo;

   struct ext2sb superblock;
   struct ext2_gd *gd_buffer;
   uint8_t *bitmap_buffer;
   struct ext2fs_dinode node;
   struct ext2fs_direct dir;
   
   uint16_t i,j,aux,group_index;
   uint16_t ngroups, nblocks_gd, inodes_per_group, inodeblocks_per_group,
            minmetablocks_per_group,  nblocks_last_group, dev_bsize, nblocks_group_overhead;
   uint32_t dev_size, block_offset, free_blocks_count, free_inodes_count;
   uint32_t group_offset, blocks_per_group;
   //ninfo=dev_node->n_info.down_layer_info;
   //finfo=dev_node->f_info.down_layer_info;
   //fsinfo=dev_node->fs_info.down_layer_info;
   bdev = dev_node->fs_info.device;

   ASSERT_MSG(dev_node->n_info.type == VFS_FTBLK, "ext2_format(): not a block device");
   if(dev_node->n_info.type != VFS_FTBLK)
   {
      return -1;   /* Solo puedo formatear un dispositivo */
   }

   ret = ciaaBlockDevices_ioctl(bdev, ciaaPOSIX_IOCTL_BLOCK_GETINFO, &blockInfo);
   ASSERT_MSG(ret > 0, "ext2_format(): ciaaBlockDevices_ioctl() failed");
   if(ret < 0)
   {
      return -1;
   }

   /* Uso el tamano del bloque fisico como el tamano del bloque del fs */
   dev_bsize=blockInfo.blockSize;
   /* TODO: Validar que dev_bsize no sea mayor al tamanio maximo de bloque de ext2 */
   if(dev_bsize < 1024)   /* Tamano minimo del bloque de ext2 */
      dev_bsize = 1024;
   dev_size=blockInfo.lastPosition;

   /* Inicializo los contenidos del superblock default */
   ciaaPOSIX_memset((uint8_t *)&superblock, 0, sizeof(struct ext2sb));

   for(i=0, aux=1024; aux < dev_bsize; i++, aux = 1024 << i);   /*Calculo el logaritmo del blocksize */
   superblock.s_log_block_size = i;
   superblock.s_first_data_block = (dev_bsize > EXT2_SBOFF) ? 0 : 1;  /* First Data Block */
   superblock.s_blocks_per_group = dev_bsize*8;  /* # Blocks per group */
   superblock.s_frags_per_group = superblock.s_blocks_per_group;   /* # Fragments per group */
   superblock.s_wtime = 0;             /* Write time */
   superblock.s_mnt_count = 0;            /* Mount count */
   superblock.s_max_mnt_count = -1;        /* Maximal mount count */
   superblock.s_magic = 0xEF53;                /* Magic signature */
   superblock.s_state = 1;                /* File system state. EXT2_VALID_FS */
   superblock.s_errors = 1;               /* Behaviour when detecting errors. EXT2_ERRORS_CONTINUE */
   superblock.s_lastcheck = 0;         /* time of last check */
   superblock.s_rev_level = 0;         /* Good old rev */
   superblock.s_first_ino = 11;         /* First non-reserved inode. EXT2_GOOD_OLD_FIRST_INO */
   superblock.s_inode_size = 128;           /* size of inode structure. EXT2_GOOD_OLD_INODE_SIZE */
   superblock.s_block_group_nr = 0;       /* block group # of this superblock */
   superblock.s_feature_compat = 0x00;    /* compatible feature set */
   superblock.s_feature_incompat = 0x00;  /* incompatible feature set */
   superblock.s_feature_ro_compat = 0x00;  /* readonly-compatible feature set */
   superblock.s_uuid[0] = 0;   /* 128-bit uuid for volume */
   superblock.s_uuid[1] = 0;
   superblock.s_uuid[2] = 0;
   superblock.s_uuid[3] = 0;
   superblock.s_uuid[4] = 0;
   superblock.s_uuid[5] = 0;
   superblock.s_uuid[6] = 0;
   superblock.s_uuid[7] = 0;
   superblock.s_uuid[8] = 0;
   superblock.s_uuid[9] = 0;
   superblock.s_uuid[10] = 0;
   superblock.s_uuid[11] = 0;
   superblock.s_uuid[12] = 0;
   superblock.s_uuid[13] = 0;
   superblock.s_uuid[14] = 0;
   superblock.s_uuid[15] = 0;
   ciaaPOSIX_strcpy((char *) superblock.s_volume_name, "ext2");      /* volume name */

   /* Total blocks */
   superblock.s_blocks_count = dev_size / dev_bsize;
   /* Factor 4 */
   superblock.s_inodes_count = superblock.s_blocks_count / 4;
   /* Reserved blocks count */
   superblock.s_r_blocks_count = 0;
   /* Cantidad de grupos de bloques */
   ngroups = howmany(superblock.s_blocks_count - superblock.s_first_data_block,
                     superblock.s_blocks_per_group);
   /* Cantidad de bloques por grupo */
   blocks_per_group = dev_bsize*8;
   /* Cantidad de bloques que ocupan los group descriptors en total por cada grupo */
   nblocks_gd = howmany(sizeof(struct ext2_gd)*ngroups, dev_bsize);
   /* Cantidad de nodos que entran en un bloque */
   inodes_per_group = superblock.s_inodes_count / ngroups;
   /* Cantidad de bloques que ocupan los nodos en total por cada grupo */
   inodeblocks_per_group = howmany(superblock.s_inode_size * inodes_per_group, dev_bsize);
   /* Cantidad de bloques de prefijo de cada grupo */
   minmetablocks_per_group = 1 /*Superblock*/ + nblocks_gd + 1 /*Block bitmap*/ +
                     1 /*Inode bimap*/ + inodeblocks_per_group + 1 /*At least 1 data block*/;
   /* El ultimo grupo tiene en general menos bloques que el resto, son los bloques que sobran */
   nblocks_last_group = superblock.s_blocks_count - superblock.s_first_data_block -
                        superblock.s_blocks_per_group * (ngroups - 1);

   /* Si el tamanio del ultimo grupo es demasiado pequenio, lo elimino */
   if(nblocks_last_group < minmetablocks_per_group)
   {
      superblock.s_blocks_count -= nblocks_last_group;
      ngroups--;
      nblocks_last_group = superblock.s_blocks_per_group;
      nblocks_gd = howmany(sizeof(struct ext2_gd)*ngroups, dev_bsize);
      inodes_per_group = superblock.s_inodes_count / ngroups;
   }

   /* La cantidad de nodos dentro de un grupo debe ser multiplo de 8 para entrar en bloques enteros */
   superblock.s_inodes_per_group = rounddown(inodes_per_group, 8);
   /* Cantidad total de nodos */
   superblock.s_inodes_count = inodes_per_group * ngroups;
   /* Cantidad de bloques que ocupan los nodos en cada bloque */
   inodeblocks_per_group = howmany(superblock.s_inode_size * inodes_per_group, dev_bsize);
   /* Cantidad de bloques de overhead por grupo*/
   nblocks_group_overhead = 1/*Superblock*/ + nblocks_gd + 1 /*Blockbitmap*/+ 1 /*Inodebitmap*/+ inodeblocks_per_group;

   //Inicializo group descriptors. Los preparo en un buffer
   gd_buffer = (struct ext2_gd *) ciaaPOSIX_malloc(sizeof(struct ext2_gd)*ngroups);
   ASSERT_MSG(gd_buffer!=NULL, "ext2_format(): ciaaPOSIX_malloc() failed");
   if(gd_buffer==NULL)
   {
      return -1;
   }
   free_inodes_count=free_blocks_count=0;
   for(group_index=0; group_index<ngroups; group_index++)
   {
      block_offset = superblock.s_first_data_block + superblock.s_blocks_per_group*group_index;
      block_offset += 1 /*Superblock*/+ nblocks_gd;
      gd_buffer[group_index].block_bitmap = block_offset;
      block_offset += 1; /*Block bitmap*/
      gd_buffer[group_index].inode_bitmap = block_offset;
      block_offset += 1; /*Inode table*/
      gd_buffer[group_index].inode_table = block_offset;
      if(group_index == ngroups-1)
         gd_buffer[group_index].free_blocks_count = nblocks_last_group -
                                                      minmetablocks_per_group /*Overhead*/+ 1;
      else
         gd_buffer[group_index].free_blocks_count = superblock.s_blocks_per_group - minmetablocks_per_group /*Overhead*/+ 1;
      free_blocks_count += gd_buffer[group_index].free_blocks_count;
      if(group_index == 0)
         gd_buffer[group_index].free_inodes_count = superblock.s_inodes_per_group - EXT2_RESERVED_INODES;
      else
         gd_buffer[group_index].free_inodes_count = superblock.s_inodes_per_group;

      free_inodes_count += gd_buffer[group_index].free_inodes_count;
      gd_buffer[group_index].used_dirs_count = 0;
      ciaaPOSIX_printf( "\nGroup descriptor #%d:\nfree_inodes_count:%d free_blocks_count:%d block_bitmap:%d inode_bitmap:%d inode_table:%d\n", group_index, gd_buffer[group_index].free_inodes_count,
                        gd_buffer[group_index].free_blocks_count,
                        gd_buffer[group_index].block_bitmap, gd_buffer[group_index].inode_bitmap,
                        gd_buffer[group_index].inode_table);
   }
   /* Si hay nodos reservados, estos numeros cambian */
   superblock.s_free_inodes_count = free_inodes_count;
   superblock.s_free_blocks_count = free_blocks_count;

   /* Ya tengo preparados el superblock y los gd, solo me queda escribir los buffer en orden en el disco */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      group_offset = EXT2_SBOFF + superblock.s_blocks_per_group * dev_bsize * group_index;   //Es confuso, pero parece correcto.
      /* Si el block size es 1024, block 0 es boot, block 1 es superblock. Blocks 1 a 8192 son blocks del group 1
       * Por lo tanto el boot block no cuenta como block del group 1.
       */

      ret = ciaaBlockDevices_lseek(bdev, group_offset, SEEK_SET);
      ASSERT_MSG(ret==group_offset, "ext2_format(): ciaaBlockDevices_lseek() failed");
      if(ret!=group_offset)
      {
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)&superblock, sizeof(struct ext2sb));
      ASSERT_MSG(ret==sizeof(struct ext2sb), "ext2_format(): ciaaBlockDevices_write() failed");
      if(ret!=sizeof(struct ext2sb))
      {
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_lseek(bdev, group_offset + EXT2_SBSIZE, SEEK_SET);
      ASSERT_MSG(ret==group_offset + EXT2_SBSIZE, "ext2_format(): ciaaBlockDevices_lseek() failed");
      if(ret!=group_offset+EXT2_SBSIZE)
      {
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)gd_buffer, sizeof(struct ext2_gd)*ngroups);
      ASSERT_MSG(ret==sizeof(struct ext2_gd)*ngroups, "ext2_format(): ciaaBlockDevices_write() failed");
      if(ret!=sizeof(struct ext2_gd)*ngroups)
      {
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
   }
   /* Falta completar block bitmap e inode bitmap. Va a ser medio dificil porque tengo que setear bits
    * y solo puedo escribir bytes. La solucion es escribir sobre un buffer y despues escribir en el disco.
    * Tener un buffer del tamanio de un bloque podria ser conveniente, ya que los bitmaps ocupan exact. eso.
    */
   /* Pido memoria del tamanio de un bloque */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(dev_bsize);
   ASSERT_MSG(bitmap_buffer!=NULL, "ext2_format(): ciaaPOSIX_malloc() failed");
   if(bitmap_buffer==NULL)
   {
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Block bitmap */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      ciaaPOSIX_memset(bitmap_buffer, 0, dev_bsize);
      /* El ultimo grupo en general tiene menos bloques que el resto
       *Tengo que setear los bits de los bloques que no existen.nblocks_last_group bits
       *Seteo primero los primeros nblocks_last_group/8 bytes
       *Si hay 9 bloques, tengo que setear los primeros 9/8=1 bytes, despues los bits desde 8*1 hasta 9-1
       *Si hay 7 bloques, tengo que setear los primeros 7/8=0 bytes, despues los bits desde 8*0 hasta 7-1
       */
      if(group_index == ngroups-1)
      {
         for(i=nblocks_last_group; i%8!=0; i++)
         {
            setbit(bitmap_buffer, i);
         }
         for(i=i/8; i<blocks_per_group/8; i++)
         {
            bitmap_buffer[i] = 0xFF;
         }
      }
      for(i=0; i<nblocks_group_overhead/8; i++)
      {
         bitmap_buffer[i] = 0xFF;
      }
      i*=8;
      for(i=0; i<nblocks_group_overhead;i++)
      {
         setbit(bitmap_buffer,i);
      }

      ret = ciaaBlockDevices_lseek(bdev, gd_buffer[group_index].block_bitmap * dev_bsize, SEEK_SET);
      ASSERT_MSG(ret==gd_buffer[group_index].block_bitmap*dev_bsize, "ext2_format(): ciaaBlockDevices_lseek() failed");
      if(ret!=gd_buffer[group_index].block_bitmap * dev_bsize)
      {
         ciaaPOSIX_free(bitmap_buffer);
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, dev_bsize);
      ASSERT_MSG(ret==dev_bsize, "ext2_format(): ciaaBlockDevices_write() failed");
      if(ret!=dev_bsize)
      {
         ciaaPOSIX_free(bitmap_buffer);
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
   }

   ciaaPOSIX_printf("\n\nBlock Bitmap\n");
   for(i=0; i<dev_bsize; i++)
   {
      ciaaPOSIX_printf("%u:%02x ", i, bitmap_buffer[i]);
   }

   /* Inode bitmap */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      i = superblock.s_inodes_per_group/8;
      ciaaPOSIX_memset(bitmap_buffer, 0, i);   /*Todos los inodes libres*/
      ciaaPOSIX_memset(bitmap_buffer+i, 0xFF, dev_bsize-i);   /*Estos inodes no existen, asi que los pongo ocupados*/
      /*Los nodos de 0 a 9 (primeros 10 nodos) son reservados. s_first_ino es 11, pero contando desde 1 a 11.*/
      if (group_index == 0)
      {
         /* mark reserved inodes */
         for (i = 0; i < EXT2_FIRSTINO; i++)
            setbit(bitmap_buffer, i);
      }

      ret = ciaaBlockDevices_lseek(bdev, gd_buffer[group_index].inode_bitmap * dev_bsize, SEEK_SET);
      ASSERT_MSG(ret==gd_buffer[group_index].inode_bitmap*dev_bsize, "ext2_format(): ciaaBlockDevices_lseek() failed");
      if(ret!=gd_buffer[group_index].inode_bitmap * dev_bsize)
      {
         ciaaPOSIX_free(bitmap_buffer);
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, dev_bsize);
      ASSERT_MSG(ret==dev_bsize, "ext2_format(): ciaaBlockDevices_write() failed");
      if(ret!=dev_bsize)
      {
         ciaaPOSIX_free(bitmap_buffer);
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
   }

   ciaaPOSIX_printf("\n\next2_format(): Inode Bitmap\n");
   for(i=0; i<dev_bsize; i++)
   {
      ciaaPOSIX_printf("%u:%02x ", i, bitmap_buffer[i]);
   }

   /* root directory */
   ciaaPOSIX_memset(&node, 0, sizeof(struct ext2fs_dinode));

   node.i_mode = 040755;
   node.i_uid = node.i_gid = 1000;
   node.i_size = 1024;
   node.i_atime = node.i_ctime = node.i_mtime = 0;
   node.i_dtime = 0;
   node.i_links_count = 2;
   node.i_blocks = 2;
   /* Rescato el block bitmap del primer bloque para buscar un bloque libre donde poner los entries del root*/
   /* Este bloque libre va a ser el node.i_block[0] */
   ret = ciaaBlockDevices_lseek(bdev, gd_buffer[0].block_bitmap * dev_bsize, SEEK_SET);   /*Block bitmap*/
   ASSERT_MSG(ret==gd_buffer[0].block_bitmap*dev_bsize, "ext2_format(): ciaaBlockDevices_lseek() failed");
   if(ret!=gd_buffer[0].block_bitmap*dev_bsize)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_read(bdev, (uint8_t *)bitmap_buffer, dev_bsize);
   ASSERT_MSG(ret==dev_bsize, "ext2_format(): ciaaBlockDevices_read() failed");
   if(ret!=dev_bsize)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   for(i=0; i<superblock.s_blocks_per_group/8 && bitmap_buffer[i]==0xFF; i++);   /*Busco el primer byte con un bit en 0*/
   ASSERT_MSG(i<superblock.s_blocks_per_group/8, "ext2_format(): No hay bit libre");
   if(i==superblock.s_blocks_per_group/8)
   {
      return -1;
   }
   for(j=0; j<8; j++)
   {
      if((bitmap_buffer[i] & (1<<j)) == 0)   /* El bit j del byte bitmap_buffer[i] es 0 */
      {                                    /* El bloque i*8+j estaba libre y ahora lo marco*/
         bitmap_buffer[i] |= (1<<j);
         break;
      }
   }
   /* Vuelvo a escribir el bitmap con el bloque ya reservado */
   ret = ciaaBlockDevices_lseek(bdev, gd_buffer[0].block_bitmap * dev_bsize, SEEK_SET);   /*Block bitmap*/
   ASSERT_MSG(ret==gd_buffer[0].block_bitmap*dev_bsize, "ext2_format(): ciaaBlockDevices_lseek() failed");
   if(ret!=gd_buffer[0].block_bitmap*dev_bsize)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, dev_bsize);
   ASSERT_MSG(ret==dev_bsize, "ext2_format(): ciaaBlockDevices_write() failed");
   if(ret!=dev_bsize)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   node.i_block[0] = superblock.s_first_data_block + 8*i + j;
   gd_buffer[0].free_blocks_count--;
   gd_buffer[0].free_inodes_count--;
   gd_buffer[0].used_dirs_count++;
   ciaaPOSIX_printf("\next2_format(): Block #%d reserved\n", node.i_block[0]);

   /*Escribo los cambios hechos al group descriptor*/
   ret = ciaaBlockDevices_lseek(bdev, EXT2_GDOFF, SEEK_SET);   /*First group descriptor*/
   ASSERT_MSG(ret==EXT2_GDOFF, "ext2_format(): ciaaBlockDevices_lseek() failed");
   if(ret!=EXT2_GDOFF)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)&gd_buffer[0], sizeof(struct ext2_gd));
   ASSERT_MSG(ret==sizeof(struct ext2_gd), "ext2_format(): ciaaBlockDevices_write() failed");
   if(ret!=sizeof(struct ext2_gd))
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Fill root inode */
   ret = ciaaBlockDevices_lseek(bdev, gd_buffer[0].inode_table * dev_bsize +
         superblock.s_inode_size, SEEK_SET);   /*Second inode in table. ROOT INODE*/
   ASSERT_MSG(ret==gd_buffer[0].inode_table * dev_bsize + superblock.s_inode_size,
               "ext2_format(): ciaaBlockDevices_lseek() failed");
   if(ret!=gd_buffer[0].inode_table * dev_bsize + superblock.s_inode_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)&node, sizeof(struct ext2fs_dinode));
   ASSERT_MSG(ret==sizeof(struct ext2fs_dinode), "ext2_format(): ciaaBlockDevices_write() failed");
   if(ret!=sizeof(struct ext2fs_dinode))
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /*Create root entry*/

   /*Self entry*/
   dir.e2d_ino = 2;
   dir.e2d_reclen = 12;
   dir.e2d_namlen = 1;
   dir.e2d_type = 2;
   ciaaPOSIX_strcpy(dir.e2d_name, ".");

   ret = ciaaBlockDevices_lseek(bdev, node.i_block[0]*dev_bsize, SEEK_SET);   /*First entry, "."*/
   ASSERT_MSG(ret==node.i_block[0]*dev_bsize, "ext2_format(): ciaaBlockDevices_lseek() failed");
   if(ret!=node.i_block[0]*dev_bsize)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)&dir, sizeof(struct ext2fs_direct));
   ASSERT_MSG(ret==sizeof(struct ext2fs_direct), "ext2_format(): ciaaBlockDevices_write() failed");
   if(ret!=sizeof(struct ext2fs_direct))
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   ret = ciaaBlockDevices_lseek(bdev, node.i_block[0]*dev_bsize + dir.e2d_reclen, SEEK_SET);   /*Second entry, ".."*/
   ASSERT_MSG(ret==node.i_block[0]*dev_bsize+dir.e2d_reclen, "ext2_format(): ciaaBlockDevices_lseek() failed");
   if(ret!=node.i_block[0]*dev_bsize+dir.e2d_reclen)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Parent entry */
   dir.e2d_ino = 2;
   dir.e2d_reclen = 1012;
   dir.e2d_namlen = 2;
   dir.e2d_type = 2;
   ciaaPOSIX_strcpy(dir.e2d_name, "..");

   ret = ciaaBlockDevices_write(bdev, (uint8_t *)&dir, sizeof(struct ext2fs_direct));
   ASSERT_MSG(ret==sizeof(struct ext2fs_direct), "ext2_format(): ciaaBlockDevices_write() failed");
   if(ret!=sizeof(struct ext2fs_direct))
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ciaaPOSIX_free(bitmap_buffer);

   return 0;
}

/* Precondiciones: dest_node ya esta creado. Hay que transferir la informacion de fs y file */
static int ext2_mount(vnode_t *dev_node, vnode_t *dest_node)
{
   int ret;
   ciaaDevices_deviceType const *bdev;
   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   /* FIXME: La implementacion actual necesita que no exista el directorio previamente. No esta preparado
    * para sobreescribir un nodo.
    */
   /* Tengo que llenar informacion para el root inode? */
   finfo=dest_node->f_info.down_layer_info=(struct ext2_file_info *) ciaaPOSIX_malloc(sizeof(struct ext2_file_info));
   ASSERT_MSG(finfo!=NULL, "ext2_mount(): Mem error, could not alloc finfo");
   if(finfo == NULL)
   {
      return -1;
   }
   ninfo=dest_node->n_info.down_layer_info=(struct ext2fs_dinode *) ciaaPOSIX_malloc(sizeof(struct ext2fs_dinode));
   ASSERT_MSG(ninfo!=NULL, "ext2_mount(): Mem error, could not alloc ninfo");
   if(ninfo == NULL)
   {
      return -1;
   }
   fsinfo=dest_node->fs_info.down_layer_info=(struct ext2_fs_info *)ciaaPOSIX_malloc(sizeof(struct ext2_fs_info));
   ASSERT_MSG(fsinfo!=NULL, "ext2_mount(): Mem error, could not alloc fsinfo");
   if(fsinfo == NULL)
   {
      return -1;
   }

   //Cargo la informacion del filesystem a partir del superblock
   bdev = dest_node->fs_info.device;
   ret = ext2_get_superblock(bdev, &(fsinfo->e2sb));
   ASSERT_MSG(ret>=0, "ext2_mount(): Could not get superblock");
   if(ret)
   {
      return -1;
   }
   /* La cantidad de descriptores de grupo lo calculo a partir de otros datos del superblock */
   fsinfo->s_groups_count = (fsinfo->e2sb.s_inodes_count)/(fsinfo->e2sb.s_inodes_per_group); //Calculo la cantidad de grupos
   fsinfo->s_block_size = 1024 << fsinfo->e2sb.s_log_block_size;
   fsinfo->sectors_in_block = fsinfo->s_block_size >> 9;   /* s_block_size/512 */
   fsinfo->s_inodes_per_block = (fsinfo->s_block_size)/(fsinfo->e2sb.s_inode_size);

   fsinfo->e2fs_gd = (struct ext2_gd *)ciaaPOSIX_malloc(fsinfo->s_groups_count*sizeof(struct ext2_gd));
   ASSERT_MSG(fsinfo->e2fs_gd!=NULL, "ext2_mount(): Mem error, could not alloc gd");
   if(fsinfo->e2fs_gd==NULL)
   {
      return -1;
   }
   ret = ciaaBlockDevices_lseek(bdev, EXT2_GDOFF, SEEK_SET);
   ASSERT_MSG(ret==EXT2_GDOFF, "ext2_mount(): lseek gd failed");
   if(ret!=EXT2_GDOFF)
   {
      ciaaPOSIX_free(fsinfo->e2fs_gd);
      return -1;
   }

   ret = ciaaBlockDevices_read(bdev, (uint8_t *)fsinfo->e2fs_gd, fsinfo->s_groups_count*(sizeof(struct ext2_gd)));
   ASSERT_MSG(ret==fsinfo->s_groups_count*(sizeof(struct ext2_gd)), "ext2_mount(): read gd failed");
   if(ret!=fsinfo->s_groups_count*(sizeof(struct ext2_gd)))
   {
      ciaaPOSIX_free(fsinfo->e2fs_gd);
      return -1;
   }


   /* Recibo un nodo ya creado por el vfs. Pero el vfs no puede crear la estructura del down_layer_info,
    * tengo que llenarla yo
    */
   ret = ext2_mount_load(dest_node);
   ASSERT_MSG(ret>=0, "ext2_mount(): mount load failed");
   return ret;
}

static int ext2_create_node(vnode_t *parent_node, vnode_t *child_node)
{
   int ret;

   /* Node creation depends on the file type. A directory needs to have 2 default directories after creation */
   if(child_node->n_info.type == VFS_FTDIR)
   {
      ret = ext2_create_directory_file(parent_node, child_node);
   }
   else if(child_node->n_info.type == VFS_FTREG)
   {
      ret = ext2_create_regular_file(parent_node,child_node);
   }
   else
   {
      ret = -1;
   }

   return ret;
}


static int ext2_delete_node(vnode_t *parent_node, vnode_t *child_node)
{
   int ret;

   if(child_node->n_info.type == VFS_FTDIR)
   {
      ret = ext2_delete_directory_file(parent_node, child_node);
   }
   else if(child_node->n_info.type == VFS_FTREG)
   {
      ret = ext2_delete_regular_file(parent_node,child_node);
   }
   else
   {
      ret = -1;
   }

   return ret;
}

static int ext2_truncate(vnode_t *node, uint32_t length)
{
   return 0;
}
static int ext2_umount(vnode_t *node)
{
   return 0;
}

static size_t ext2_file_write(struct file_desc *desc, void *buf, size_t size)
{
   int ret;

   vnode_t *node;
   ciaaDevices_deviceType const *bdev;
   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   node = desc->node;
   bdev = node->fs_info.device;
   ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   uint32_t total_read_size, buf_pos, remaining_write_size, device_block;
   uint32_t write_size, write_offset;

   if(ninfo==NULL||finfo==NULL||fsinfo==NULL)
   {
      return 0;
   }

   finfo->f_pointer = node->f_info.file_pointer = desc->cursor;
   buf_pos = 0;
   total_read_size = size;
   remaining_write_size = size;
   while (remaining_write_size > 0) {
      /* Position inside current block */
      write_offset = finfo->f_pointer % fsinfo->s_block_size;
      /* #bytes to write to current block */
      write_size = fsinfo->s_block_size - write_offset;
      /* remaining bytes to write less than block size */
      if (write_size > total_read_size)
         write_size = total_read_size;
      /* map file block to disk block */
      ret = ext2_file_map_alloc(node, finfo->f_pointer >> (10+fsinfo->e2sb.s_log_block_size), &device_block);
      if(ret)
      {
         return 0;
      }
      /* write to current block */
      ret = ciaaBlockDevices_lseek(bdev, (device_block<<(10+fsinfo->e2sb.s_log_block_size))+write_offset, SEEK_SET);
      if(ret!=(device_block<<(10+fsinfo->e2sb.s_log_block_size))+write_offset)
      {
         return 0;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)buf + buf_pos, write_size);
      if(ret!=write_size)
      {
         return 0;
      }
      /* update iterators */
      finfo->f_pointer += write_size;
      buf_pos += write_size;
      remaining_write_size -= write_size;
   }
   /* all bytes written. Update pointers */
   desc->cursor = finfo->f_pointer;
   if(finfo->f_pointer > finfo->f_di.i_size)
      finfo->f_di.i_size = finfo->f_pointer;
   /*finfo->f_di.i_blocks se tendria que haber actualizado en cada llamada a ext2_file_map_alloc */
   /* Escribo el inode resultante en su posicion correspondiente en el disco */
   /* flush memory info to disk */
   ret = ext2_write_inode(node);
   if(ret)
   {
      return 0;
   }
   /* Escribo en el disco los cambios efectuados en el sb y los gd */
   ret = ext2_fsinfo_flush(node);
   if(ret)
   {
      return 0;
   }

   node->f_info.file_pointer = finfo->f_pointer;
   return buf_pos;
}



static int ext2_buf_read_file(vnode_t * dest_node, uint8_t *buf, uint32_t size, uint32_t *total_read_size_p)
{
   /* FIXME: Si size>finfo->f_di.i_size tirar error o truncar? */
   int ret;
   size_t i;
   uint32_t file_block, block_offset;
   uint32_t device_block, device_offset;
   uint16_t block_size, block_shift, block_mask;
   uint32_t total_remainder_size, block_remainder, read_size, buf_pos;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;
   ciaaDevices_deviceType const *device;

   fsinfo = dest_node->fs_info.down_layer_info;
   finfo = dest_node->f_info.down_layer_info;
   device = dest_node->fs_info.device;

   block_size = fsinfo->s_block_size;
   block_shift = (uint16_t)fsinfo->e2sb.s_log_block_size + 10;
   block_mask = (uint32_t)block_size-1;
   finfo->f_pointer = dest_node->f_info.file_pointer;
   ciaaPOSIX_printf("ext2_buf_read_file(): Offset pointer: %u\n", finfo->f_pointer);
   file_block = finfo->f_pointer >> block_shift;
   block_offset = finfo->f_pointer & block_mask;


   ciaaPOSIX_printf(   "ext2_buf_read_file(): Valores:\n"\
            "block_size: %d\nblock_shift: %d\nblock_mask: 0x%04x\n"\
            "file_block: %d\nblock_offset: %d\n",\
            block_size,block_shift,block_mask,file_block,block_offset);

   if(total_read_size_p!=NULL)
      *total_read_size_p = 0;

   if(finfo->f_pointer > finfo->f_di.i_size)
      finfo->f_pointer = finfo->f_di.i_size;
   /*Truncate read size*/
   if(finfo->f_pointer + size > finfo->f_di.i_size)
      size = finfo->f_di.i_size - finfo->f_pointer;

   /*Tengo que leer total_remainder_size bytes.
   No puedo leer todo junto, tengo que leerlo de a bloques.
   Primero tengo que buscar el bloque a partir de la posicion actual del archivo.
   Leer en la posicion de inicio de bloque mas el offset que corresponde offset%block_size.
   Solo puedo leer hasta el fin del bloque actual. Calculo cuantos bytes puedo leer y se los resto a total_remainder_size.
   Una vez leidos los bytes hasta el fin del bloque y copiados los datos al buffer, sumo la cantidad de bytes leidos al
   fpointer. Paso a la siguiente iteracion.*/

   /*Inicio: total_remainder_size = size.
   Condicion: total_remainder_size>0*/
   ciaaPOSIX_printf("ext2_buf_read_file(): Comienza el loop\n");
   for(   total_remainder_size = size, buf_pos = 0, i = 0;
      total_remainder_size>0;)
   {
      ciaaPOSIX_printf("ext2_buf_read_file(): Loop %d\n", i);
      file_block = finfo->f_pointer >> block_shift;
      block_offset = finfo->f_pointer & block_mask;
      ret = ext2_block_map(dest_node, file_block, &device_block);
      if(ret)
      {
         if(total_read_size_p!=NULL)
            *total_read_size_p = buf_pos;
         return ret;
      }
      ciaaPOSIX_printf("ext2_buf_read_file(): Read block #%d\n", device_block);
      device_offset = (device_block << block_shift) + block_offset;

      block_remainder = block_size - block_offset;
      if(total_remainder_size > block_remainder)
         read_size = block_remainder;
      else
         read_size = total_remainder_size;

      ret = ciaaBlockDevices_lseek(device, device_offset, SEEK_SET);
      if(ret!=device_offset)
      {
         if(total_read_size_p!=NULL)
            *total_read_size_p = buf_pos;
         return -1;
      }
      ret = ciaaBlockDevices_read(device, (uint8_t *)(buf+buf_pos), read_size);
      if(ret!=read_size)
      {
         if(total_read_size_p!=NULL)
            *total_read_size_p = buf_pos;
         return -1;
      }

      buf_pos+=read_size;
      finfo->f_pointer+=read_size;
      total_remainder_size-=read_size;
   }

   if(total_read_size_p != NULL)
      *total_read_size_p = buf_pos;
   if(buf_pos != size)
      return -1;
   dest_node->f_info.file_pointer = finfo->f_pointer;
   return 0;
}

/*
   Mapeo bloques de archivo a bloques de dispositivo
 */
static int ext2_block_map(const vnode_t * dest_node, uint32_t file_block, uint32_t *device_block_p)
{
   uint32_t shift_block_level, shift_single_block_level;
   uint32_t temp_block_num, n_entries_per_block, index_offset, aux, block_level;
   size_t ret;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;
   ciaaDevices_deviceType const *device;

   device = dest_node->fs_info.device;
   fsinfo = dest_node->fs_info.down_layer_info;
   finfo = dest_node->f_info.down_layer_info;
   n_entries_per_block = fsinfo->s_block_size / sizeof(uint32_t);

   /* shift_single_block_level va a ser la cantidad de bits de la mascara del nivel de bloque */
   for(shift_single_block_level=0, aux=n_entries_per_block;aux;shift_single_block_level++)
   {;
      aux = aux>>1;
   }

   if (file_block < N_DIRECT_BLOCKS)
   {
      /* Es un bloque directo */
      *device_block_p = finfo->f_di.i_block[file_block];
      return 0;
   }

   file_block -= N_DIRECT_BLOCKS;

   for (   shift_block_level=0, block_level=1;
      file_block >= (int32_t)1<<(shift_single_block_level+shift_block_level);
      file_block -= (int32_t)1<<(shift_single_block_level+shift_block_level),
      shift_block_level+=shift_single_block_level, block_level++)
   {
   /* Cada iteracion corresponde a un nivel de indireccion de bloque.
    * Cuando el file_block esta dentro del nivel actual, se sale del loop.
    * A file_block se le va restando la cantidad de bloques del nivel actual.
    * Al finalizar el loop, file_block es el offset dentro del nivel correspondiente
    * block_level queda con el nivel de indireccion, 
    */
   /* Me fijo si me paso de la cantidad de bloques del nivel
    * 1024B/4B=256bloques
    * 1 2 4 8 16 32 64 128 256 512 1024
    * 0 1 2 3 4  5  6  7   8   9   10
    * N_INDIR_BLOCKS_PER_BLOCK: BLOCK_SIZE / sizeof(uint32_t)
    * N_INDIR_BLOCKS_PER_BLOCK**2
    * N_INDIR_BLOCKS_PER_BLOCK**3
    */

      if(file_block < (int32_t) 1 << shift_block_level)
      {
         /* El numero de bloque esta dentro de este nivel, no supera su limite */
         break;
      }
      if (block_level > N_INDIRECT_BLOCKS)
      {
         /* Que es NIADDR * fi->f_nishift?
          * NIADRR es la cantidad de posiciones de bloques indirectos
          * NIADDR es 3: bloques indir, bloques dindir, bloques tindir.
          * El numero de bloque se pasa del limite maximo
          */
         *device_block_p=0;
         return -1;
      }
   }

   /*Cual de todos los bloques indirectos es?
    *Si block_level=1, el bloque indirecto es i_blocks[12], indireccion nivel 1
    *Si block_level=2, el bloque indirecto es i_blocks[13], indireccion nivel 2
    *Si block_level=2, el bloque indirecto es i_blocks[14], indireccion nivel 3
    */
   temp_block_num = finfo->f_di.i_block[N_DIRECT_BLOCKS + block_level - 1];

   for (;block_level;block_level--)
   {
      if (temp_block_num == 0)
      {
         *device_block_p = 0; /* Bloque no existe */
         return -1;
      }
      //
      index_offset =    (temp_block_num << (10+fsinfo->e2sb.s_log_block_size)) + (
                        ((file_block & (((1<<shift_single_block_level)-1))<<(shift_block_level))
                        >> shift_block_level) * sizeof(uint32_t) );

      ret = ciaaBlockDevices_lseek(device, index_offset, SEEK_SET);
      if(ret!=index_offset)
      {
         return -1;
      }
      ret = ciaaBlockDevices_read(device, (uint8_t *)temp_block_num, sizeof(uint32_t));
      if(ret!=sizeof(uint32_t))
      {
         return -1;
      }
      shift_block_level -= shift_single_block_level;
   }


   *device_block_p = temp_block_num;
   return 0;
}



static int ext2_mount_load(vnode_t *dir_node)
{
   int ret;

   /* FIXME: Como manejo el problema de hacer el mount sobre un directorio que ya existia? */
   /* Leo el root inode */
   ret = ext2_read_inode(dir_node, EXT2_ROOTINO);   //Estoy sobreescribiendo algo de lo que tenia antes?
   ASSERT_MSG(ret>=0, "ext2_mount_load(): ext2_read_inode failed");
   if(ret)
   {
      return -1;
   }
   ret = ext2_mount_load_rec(dir_node);
   ASSERT_MSG(ret>=0, "ext2_mount_load(): ext2_mount_load_rec failed");
   if(ret)
   {
      return -1;
   }

   return 0;
}


/* Precondicion: Recibo el root vnode preparado */
static int ext2_mount_load_rec(vnode_t *dir_node)
{

   uint8_t *data_pointer;
   struct   ext2fs_direct * start_pointer, * end_pointer;
   int ret;
   uint32_t lret;
   int mode;
   vnode_t *child_node;

   struct ext2_file_info *finfo;

   finfo = (struct ext2_file_info *)dir_node->f_info.down_layer_info;

   /* Leo el directorio desde el principio */
   dir_node->f_info.file_pointer = finfo->f_pointer = 0;
   ciaaPOSIX_printf("ext2_mount_load_rec: Pido %d bytes para el archivo\n", finfo->f_di.i_size);
   data_pointer=(uint8_t*)ciaaPOSIX_malloc(finfo->f_di.i_size);
   /* FIXME: Hago malloc del tamanio del archivo entero */
   ASSERT_MSG(data_pointer!=NULL, "ext2_mount_load_rec(): Mem error");
   if(data_pointer==NULL)
   {
      ciaaPOSIX_free(data_pointer);
      return -1;
   }
   ext2_buf_read_file(dir_node, (uint8_t*)data_pointer, finfo->f_di.i_size, &lret);   /* Leo todo el directorio */
   ASSERT_MSG(lret==finfo->f_di.i_size, "ext2_mount_load_rec(): ext2_buf_read_file() failed");
   if(lret!=finfo->f_di.i_size)
   {
      ciaaPOSIX_free(data_pointer);
      return -1;
   }


   for(   start_pointer=(struct ext2fs_direct *)data_pointer,
      end_pointer=(struct ext2fs_direct *)(data_pointer + finfo->f_di.i_size);
      start_pointer<end_pointer;
      start_pointer=(struct ext2fs_direct *)((char*)start_pointer + start_pointer->e2d_reclen))
   {
      if(start_pointer->e2d_reclen <= 0)
      {
         break;
      }
      if(start_pointer->e2d_ino == 0)
      {
         continue;
      }
      if(!ciaaPOSIX_strcmp(start_pointer->e2d_name, ".") || !ciaaPOSIX_strcmp(start_pointer->e2d_name, ".."))
      {
         continue;
      }
      mode=0;
      child_node = vfs_create_child(dir_node, start_pointer->e2d_name, start_pointer->e2d_namlen, mode);
      ASSERT_MSG(child_node!=NULL, "ext2_mount_load_rec(): Mem error");
      if(child_node==NULL)
      {
         return -1;
      }
      /* Lleno los campos del nodo. Primero tengo que pedir espacio para los datos de la capa inferior */
      child_node->f_info.down_layer_info = (struct ext2_file_info *) ciaaPOSIX_malloc(sizeof(struct ext2_file_info));
      //child_node->fs_info.down_layer_info = (struct ext2_fs_info *) ciaaPOSIX_malloc(sizeof(struct ext2_fs_info));
      child_node->n_info.down_layer_info = (struct ext2fs_dinode *) ciaaPOSIX_malloc(sizeof(struct ext2fs_dinode));
      if(child_node->f_info.down_layer_info == NULL || child_node->fs_info.down_layer_info == NULL
         || child_node->n_info.down_layer_info == NULL)   //FIXME
      {
         return -1;
      }
      ASSERT_MSG(ret>=0, "ext2_mount_load_rec(): ext2_read_inode() failed");
      ret = ext2_read_inode(child_node, start_pointer->e2d_ino);
      ASSERT_MSG(ret>=0, "ext2_mount_load_rec(): ext2_read_inode() failed");
      if(ret)
      {
         ciaaPOSIX_free(data_pointer);
         return -1;
      }
      if(start_pointer->e2d_type == EXT2_FT_DIR)
      {
         ret = ext2_mount_load_rec(child_node);
         if(ret)
         {
            ciaaPOSIX_free(data_pointer);
            return -1;
         }
      }

   }
   //Ya pase por todos los subtrees de dir_node. Exito
   ciaaPOSIX_free(data_pointer);
   return 0;
}

static node_type_t ext2_nodetype_e2tovfs(int imode)
{
   int file_format;
   node_type_t type;
   file_format = imode & 0xF000;
   switch(file_format)
   {
      case EXT2_S_IFREG:
         type=VFS_FTREG;
      break;
      case EXT2_S_IFBLK:
         type=VFS_FTBLK;
      break;
      case EXT2_S_IFDIR:
         type=VFS_FTDIR;
      break;
      default:
         type=VFS_FTUNKNOWN;
      break;
   }
   return type;
}

static int ext2_create_regular_file(vnode_t *parent_node, vnode_t *node)
{
/*1.Reservar espacio para node->f_info.down_layer_info
 *2.Reservar un bit del inode_bitmap, con las politicas que correspondan (ej: mismo gd que parent)
 *3.LLenar los campos correspondientes en el f_info del hijo creado y del padre
     finfo->f_num con el nuevo nodo reservado
     finfo->f_di.i_size
     finfo->f_di.i_links_count
     finfo->f_di.i_mode
 *4.Reservar un bloque del archivo
 *5.Escribir datos en el inode padre y el entry en los datos del padre
 *6.Actualizar estadisticas de superblock y gd correspondiente
 */

   int ret;
   uint32_t new_inumber;

   //ciaaDevices_deviceType const *bdev;
   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo, *parent_finfo;
   //struct ext2_fs_info *fsinfo;

   //bdev = node->fs_info.device;
   ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   parent_finfo = (struct ext2_file_info *)parent_node->f_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   //fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   finfo = (struct ext2_file_info *) ciaaPOSIX_malloc(sizeof(struct ext2_file_info));
   ASSERT_MSG(finfo!=NULL, "Mem error");
   if(finfo==NULL)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)finfo, 0, sizeof(struct ext2_file_info));
   node->f_info.down_layer_info = (void *)finfo;

   ninfo = (struct ext2fs_dinode *) ciaaPOSIX_malloc(sizeof(struct ext2fs_dinode));
   if(ninfo==NULL)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)ninfo, 0, sizeof(struct ext2fs_dinode));
   node->n_info.down_layer_info = (void *)ninfo;

   /* Reservo un bit de inode. No escribo nada en el nodo en memoria, solo en sb y gd */
   ret = ext2_alloc_inode_bit(node, &new_inumber);
   ASSERT_MSG(ret==0, "ext2_alloc_inode_bit() failed");
   if(ret)
   {
      return -1;
   }

   /* Reservo un bloque de antemano para datos del archivo. Modifico el nodo */
   ret = ext2_file_map_alloc(node, 0, NULL);
   ASSERT_MSG(ret==0, "ext2_file_map_alloc() failed");
   if(ret)
   {
      return -1;
   }
   /* Indico los valores del inode */
   finfo->f_di.i_mode=0x81B6;
   //finfo->f_di.i_uid=0;
   //finfo->f_di.i_atime=0;
   //finfo->f_di.i_ctime=0;
   finfo->f_di.i_flags=0;

   ret = ext2_dir_print(parent_node);
   ASSERT_MSG(ret==0, "ext2_create_regular_file(): El archivo no fue correctamente abierto");
   if(ret)
   {
      return -1;
   }
   /* Agrego en el directorio padre el entry del nuevo archivo. Escribo node, gd y sb si reserva nuevo bloque */
   //FIXME: Hay loop infinito
   ret = ext2_dir_add_entry(parent_node, new_inumber, node->f_info.file_name, node->f_info.file_namlen, EXT2_FT_REG_FILE);
   ASSERT_MSG(ret==0, "ext2_dir_add_entry() failed");
   if(ret)
   {
      return -1;
   }
   ret = ext2_dir_print(parent_node);
   ASSERT_MSG(ret==0, "ext2_create_regular_file(): El archivo no fue correctamente abierto");
   if(ret)
   {
      return -1;
   }

   /* The new node is referred by the father. Increment link count */
   finfo->f_di.i_links_count++;
   parent_finfo->f_di.i_links_count++;
   /* Escribo el inode resultante en su posicion correspondiente en el disco */
   ret = ext2_write_inode(node);
   ASSERT_MSG(ret==0, "ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   /* escribo el nodo padre con las modificaciones de ext2_dir_add_entry() */
   ret = ext2_write_inode(parent_node);
   ASSERT_MSG(ret==0, "ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   ret = ext2_write_inode(node);
   ASSERT_MSG(ret==0, "ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   /* Escribo en el disco los cambios efectuados en el sb y los gd */
   ret = ext2_fsinfo_flush(node);
   ASSERT_MSG(ret==0, "ext2_fsinfo_flush() failed");
   if(ret)
   {
      return -1;
   }
   return 0;
}

static int ext2_delete_regular_file(vnode_t *parent_node, vnode_t *node)
{
   int ret;
   uint32_t file_blocks; /* File blocks count */
   uint32_t fblock, device_block;

   //ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo, *parent_finfo;
   struct ext2_fs_info *fsinfo;

   //bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   parent_finfo = (struct ext2_file_info *)parent_node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   file_blocks = finfo->f_di.i_size >> (10+fsinfo->e2sb.s_log_block_size);
   /* dealloc all data block of file */
   for(fblock = 0; fblock <= file_blocks; fblock++)
   {
      ret = ext2_block_map(node, fblock, &device_block);
      if(ret)
      {
         return -1;
      }
      ret = ext2_dealloc_block_bit(node, device_block);
      if(ret)
      {
         return -1;
      }
   }
   /* clear inode in inode table */
   ciaaPOSIX_memset(&(finfo->f_di), 0, sizeof(struct ext2fs_dinode));
   ret = ext2_dir_delete_entry(parent_node, node->f_info.file_name, node->f_info.file_namlen);
   if(ret)
   {
      return -1;
   }
   parent_finfo->f_di.i_links_count--;
   /* dealloc bit in inode bitmap */
   ret = ext2_dealloc_inode_bit(node);
   if(ret)
   {
      return -1;
   }
   /* escribo el nodo padre con las modificaciones de ext2_dir_delete_entry() */
   ret = ext2_write_inode(parent_node);
   if(ret)
   {
      return -1;
   }
   /* borro del disco el nodo eliminado (ya lo puse en 0 en memoria) */
   ret = ext2_write_inode(node);
   if(ret)
   {
      return -1;
   }
   /* Escribo en el disco los cambios efectuados en el sb y los gd */
   ret = ext2_fsinfo_flush(node);
   if(ret)
   {
      return -1;
   }
   /* Libero el espacio reservado para datos del archivo de bajo nivel */
   ciaaPOSIX_free(node->f_info.down_layer_info);
   return 0;
}

static int ext2_create_directory_file(vnode_t *parent_node, vnode_t *node)
{
/* 1.Reservar espacio para node->f_info.down_layer_info
 * 2.Reservar un bit del inode_bitmap, con las politicas que correspondan (ej: mismo gd que parent)
 * 3.LLenar los campos correspondientes en el f_info del hijo creado y del padre
      finfo->f_num con el nuevo nodo reservado
      finfo->f_di.i_size
      finfo->f_di.i_links_count
      finfo->f_di.i_mode
 * 4.Reservar un bloque del archivo
 * 5.Escribir datos en el inode padre y el entry en los datos del padre
 * 6.Actualizar estadisticas de superblock y gd correspondiente
 */
   int ret;
   uint32_t new_inumber;


   //ciaaDevices_deviceType const *bdev;
   struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo, *parent_finfo;
   struct ext2_fs_info *fsinfo;

   //bdev = node->fs_info.device;
   ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   parent_finfo = (struct ext2_file_info *)parent_node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   finfo = (struct ext2_file_info *) ciaaPOSIX_malloc(sizeof(struct ext2_file_info));
   if(finfo==NULL)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)finfo, 0, sizeof(struct ext2_file_info));
   node->f_info.down_layer_info = (void *)finfo;

   ninfo = (struct ext2fs_dinode *) ciaaPOSIX_malloc(sizeof(struct ext2fs_dinode));
   if(ninfo==NULL)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)ninfo, 0, sizeof(struct ext2fs_dinode));
   node->n_info.down_layer_info = (void *)ninfo;

   /* Reservo un bit de inode. No escribo nada en el nodo en memoria, solo en sb y gd */
   ret = ext2_alloc_inode_bit(node, &new_inumber);
   if(ret)
   {
      return -1;
   }
   /* Reservo un bloque de antemano para datos del archivo. Modifico el nodo */
   ret = ext2_file_map_alloc(node, 0, NULL);
   if(ret)
   {
      return -1;
   }
   /* Linux crea los directorios con permiso 0755 */
   /* Indico los valores del inode */
   finfo->f_di.i_mode=0x41FF;
   //finfo->f_di.i_uid=0;
   //finfo->f_di.i_atime=0;
   //finfo->f_di.i_ctime=0;
   finfo->f_di.i_flags=0;
   /* Agrego en el nuevo directorio la autoreferencia y la referencia al padre */
   ret = ext2_dir_add_entry(node, new_inumber, ".", 1, EXT2_FT_DIR);
   if(ret)
   {
      return -1;
   }
   ret = ext2_dir_add_entry(node, parent_finfo->f_inumber, "..", 2, EXT2_FT_DIR);
   if(ret)
   {
      return -1;
   }
   /* Agrego en el directorio padre el entry del nuevo directorio. Escribo node, gd y sb si reserva nuevo bloque */
   ret = ext2_dir_add_entry(parent_node, new_inumber, node->f_info.file_name, node->f_info.file_namlen, EXT2_FT_DIR);
   if(ret)
   {
      return -1;
   }
   parent_finfo->f_di.i_links_count++;
   finfo->f_di.i_links_count = 2;
   /* Agregue un nuevo directorio. Tengo que reflejar esa info en fsinfo */
   fsinfo->e2fs_gd[finfo->f_group].used_dirs_count++;
   /* Escribo el inode resultante en su posicion correspondiente en el disco */
   ret = ext2_write_inode(node);
   if(ret)
   {
      return -1;
   }
   /* escribo el nodo padre con las modificaciones de ext2_dir_add_entry(). Escribo el nodo padre despues del nuevo hijo
    * para asegurarme que no haya un new_entry sin un nodo. Mejor nodo sin entry que entry sin nodo
    */
   ret = ext2_write_inode(parent_node);
   if(ret)
   {
      return -1;
   }
   /* Escribo en el disco los cambios efectuados en el sb y los gd */
   ret = ext2_fsinfo_flush(node);
   if(ret)
   {
      return -1;
   }
   return 0;
}

static int ext2_delete_directory_file(vnode_t *parent_node, vnode_t *node)
{
   int ret;
   uint32_t file_blocks; /* File blocks count */
   uint32_t fblock, device_block;

   //ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo, *parent_finfo;
   struct ext2_fs_info *fsinfo;

   //bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   parent_finfo = (struct ext2_file_info *)parent_node->f_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   ret = ext2_dir_delete_entry(node, ".", 1);
   if(ret)
   {
      return -1;
   }
   ret = ext2_dir_delete_entry(node, "..", 2);
   if(ret)
   {
      return -1;
   }
   file_blocks = finfo->f_di.i_size >> (10+fsinfo->e2sb.s_log_block_size);
   /* dealloc all data block of file */
   for(fblock = 0; fblock <= file_blocks; fblock++)
   {
      ret = ext2_block_map(node, fblock, &device_block);
      if(ret)
      {
         return -1;
      }
      ret = ext2_dealloc_block_bit(node, device_block);
      if(ret)
      {
         return -1;
      }
   }
   /* clear inode in inode table */
   ciaaPOSIX_memset(&(finfo->f_di), 0, sizeof(struct ext2fs_dinode));
   ret = ext2_dir_delete_entry(parent_node, node->f_info.file_name, node->f_info.file_namlen);
   if(ret)
   {
      return -1;
   }
   parent_finfo->f_di.i_links_count--;
   /* dealloc bit in inode bitmap */
   ret = ext2_dealloc_inode_bit(node);
   if(ret)
   {
      return -1;
   }
   /* escribo el nodo padre con las modificaciones de ext2_dir_add_entry() */
   ret = ext2_write_inode(parent_node);
   if(ret)
   {
      return -1;
   }
   /* borro del disco el nodo eliminado (ya lo puse en 0 en memoria) */
   ret = ext2_write_inode(node);
   if(ret)
   {
      return -1;
   }
   /* Escribo en el disco los cambios efectuados en el sb y los gd */
   ret = ext2_fsinfo_flush(node);
   if(ret)
   {
      return -1;
   }
   /* Libero el espacio reservado para datos del archivo de bajo nivel */
   ciaaPOSIX_free(node->f_info.down_layer_info);
   return 0;
}


/*
 * Reserva un bit en el primer bitmap con un nodo libre que encuentre, contando desde el primero.
 * Actualiza los valores en memoria de la informacion del filesystem, menos de used_dirs_count.
 * Actualizo los valores en memoria del nodo
 */
static int ext2_alloc_inode_bit(vnode_t *node, uint32_t *new_inumber_p)
{
   /* Busco un grupo que tenga un inode libre. Busco el numero de ese inode */
   /* TODO: Estoy buscando desde el 0. tendria que buscar desde el que corresponde */
   int ret;
   uint32_t i,j,n;
   uint16_t group_index;
   uint8_t *bitmap_buffer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   /* Busco un grupo con nodos libres */
   for(group_index=0; group_index < fsinfo->s_groups_count; group_index++)
   {
      if(fsinfo->e2fs_gd[group_index].free_inodes_count)
         break;
   }
   ciaaPOSIX_printf("ext2_alloc_inode_bit(): Free nodes in group %u\n", group_index);
   ASSERT_MSG(group_index<fsinfo->s_groups_count, "ext2_alloc_inode_bit(): No group with free inodes");
   if(group_index>= fsinfo->s_groups_count)
   {
      /* Ningun gd tiene inodes libres */
      return -1;
   }
   /* Reservo memoria para cargar el bitmap buffer */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   ASSERT_MSG(bitmap_buffer!=NULL, "ext2_alloc_inode_bit(): Mem error");
   if(bitmap_buffer==NULL)
   {
      return -1;
   }

   /* Leo el bitmap y lo cargo al bitmap_buffer */
   ret = ciaaBlockDevices_lseek(bdev, (fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
         SEEK_SET);
   ASSERT_MSG(ret==(fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size), "ext2_alloc_inode_bit(): ciaaBlockDevices_lseek() failed");
   if(ret!=(fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size))
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_read(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   ASSERT_MSG(ret==fsinfo->s_block_size, "ext2_alloc_inode_bit(): ciaaBlockDevices_read() failed");
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ciaaPOSIX_printf("ext2_alloc_inode_bit(): Read disk: offset: %u size: %u\n",
                     (fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
                     fsinfo->s_block_size);
   /* Busco un bit libre en el bitmap */
   /* En primer lugar busco un byte que contenga un bit en 0 (libre) */
   for(i=0; i<fsinfo->s_block_size && bitmap_buffer[i] == 0xFF; i++);
   ASSERT_MSG(i<fsinfo->s_block_size, "ext2_alloc_inode_bit(): No free byte. Descriptor error");
   if(i>=fsinfo->s_block_size)
   {
      /*No encontre un bit libre en el bitmap
       *Previamente el gd me decia que habia un nodo libre, error de consistencia del fs
       */
      return -1;
   }
   /* Ahora busco el bit libre dentro del byte encontrado */
   for(j=0; j<8;j++)
   {
      if(!(bitmap_buffer[i]&(1<<j)))
      {
         bitmap_buffer[i] |= (1<<j);
         n = 8*i+j;
         break;
      }
   }
   ASSERT_MSG(j<8, "ext2_alloc_inode_bit(): No free bit in selected byte. Inconsistency error");
   if(j>=8)
   {
      return -1;   /* No tendria que suceder nunca */
   }
   /* n es ahora el numero de nodo relativo al gd actual, calculo el absoluto */
   *new_inumber_p = group_index * fsinfo->e2sb.s_inodes_per_group + n + 1;
   /* Escribo el valor nuevo del bitmap al disco */
   ret = ciaaBlockDevices_lseek(bdev, (fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
         SEEK_SET);
   ASSERT_MSG(ret==(fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size), "ext2_alloc_inode_bit(): ciaaBlockDevices_lseek() failed");
   if(ret!=(fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size))
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   ASSERT_MSG(ret==fsinfo->s_block_size, "ext2_alloc_inode_bit(): ciaaBlockDevices_write() failed");
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /*Actualizo los valores en memoria. Despues tendre que escribirlos al disco*/
   fsinfo->e2sb.s_free_inodes_count--;
   fsinfo->e2fs_gd[group_index].free_inodes_count--;
   //fsinfo->e2fs_gd[group_index].used_dirs_count++; //Todavia no se si es un directorio o no, lo dejo para el caller
   finfo->f_group = group_index;
   finfo->f_inumber = *new_inumber_p;

   return 0;
}

static int ext2_dealloc_inode_bit(vnode_t *node)
{
   /* Busco un grupo que tenga un inode libre. Busco el numero de ese inode */
   /* TODO: Estoy buscando desde el 0. tendria que buscar desde el que corresponde */
   int ret;
   uint32_t n;
   uint8_t *bitmap_buffer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   /* Reservo memoria para cargar el bitmap buffer */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   if(bitmap_buffer==NULL)
   {
      return -1;
   }

   /* Leo el bitmap y lo cargo al bitmap_buffer */
   ret = ciaaBlockDevices_lseek(bdev, (fsinfo->e2fs_gd[finfo->f_group].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
         SEEK_SET);
   if(ret!=(fsinfo->e2fs_gd[finfo->f_group].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size))
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_read(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /* n es el indice del nodo dentro del grupo actual. El nodo a desalojar corresponde al bit n del bitmap */
   n = finfo->f_inumber - (finfo->f_group * fsinfo->e2sb.s_inodes_per_group) - 1;
   clrbit(bitmap_buffer, n);
   /* Escribo el valor nuevo del bitmap al disco */
   ret = ciaaBlockDevices_lseek(bdev, (fsinfo->e2fs_gd[finfo->f_group].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
         SEEK_SET);
   if(ret!=(fsinfo->e2fs_gd[finfo->f_group].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size))
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /* Actualizo los valores en memoria. Despues tendre que escribirlos al disco */
   fsinfo->e2sb.s_free_inodes_count++;
   fsinfo->e2fs_gd[finfo->f_group].free_inodes_count++;
   //fsinfo->e2fs_gd[finfo->f_group].used_dirs_count++; //Todavia no se si es un directorio o no, lo dejo para el caller
   finfo->f_inumber = 0;

   return 0;
}


/* TODO: Validate arguments */
static int ext2_file_map_alloc(vnode_t *node, uint32_t file_block, uint32_t *device_block_p)
{
   int ret;
   uint32_t block_pos, new_block, temp_block_num, index_offset;
   uint8_t shift_block_level, shift_single_block_level, block_level;
   uint8_t *block_buffer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   if(device_block_p!=NULL)
   {
      *device_block_p=0;
   }
   block_pos = file_block;
   ciaaPOSIX_printf("ext2_file_map_alloc(): Map block #%u of file in node %u\n", file_block, finfo->f_inumber);
   if (block_pos < N_DIRECT_BLOCKS)
   {
      if(finfo->f_di.i_block[block_pos] == 0)
      {
         ret = ext2_alloc_block_bit(node, &new_block);
         ASSERT_MSG(ret>=0, "ext2_file_map_alloc(): ext2_alloc_block_bit() failed");
         if(ret)
         {
            return -1;
         }
         finfo->f_di.i_block[block_pos] = new_block;
         finfo->f_di.i_blocks += fsinfo->sectors_in_block;
      }
      if(device_block_p!=NULL)
      {
         *device_block_p = finfo->f_di.i_block[block_pos];
      }
      block_buffer = ciaaPOSIX_malloc(fsinfo->s_block_size);
      if(block_buffer == NULL)
      {
         return -1;
      }
      ciaaPOSIX_memset(block_buffer, 0, fsinfo->s_block_size);
      ret = ciaaBlockDevices_lseek(bdev, new_block<<(10+fsinfo->e2sb.s_log_block_size), SEEK_SET);
      if(ret!=new_block<<(10+fsinfo->e2sb.s_log_block_size))
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)block_buffer, fsinfo->s_block_size);
      if(ret!=fsinfo->s_block_size)
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      ciaaPOSIX_free(block_buffer);
      return 0;
   }
   /*
    * 0:EXT2_DIR_BLOCK-1
    * EXT2_DIR_BLOCK:EXT2_DIR_BLOCK+(1<<shift_single_block_level)-1
    * EXT2_DIR_BLOCK+(1<<shift_single_block_level):EXT2_DIR_BLOCK+(1<<shift_single_block_level)+(1<<2*(shift_single_block_level))-1
    * EXT2_DIR_BLOCK+(1<<shift_single_block_level)+(1<<2*(shift_single_block_level))-1:
    * EXT2_DIR_BLOCK+(1<<shift_single_block_level)+(1<<2*(shift_single_block_level))+(1<<3*(shift_single_block_level))-1
    */
   /* Quiero hallar shift_block_level */
   shift_single_block_level = 256-1;
   block_pos -= N_DIRECT_BLOCKS;
   for (   shift_block_level=0, block_level=1;
      block_pos >= (int32_t)1<<(shift_single_block_level+shift_block_level);
      block_pos -= (int32_t)1<<(shift_single_block_level+shift_block_level),
      shift_block_level+=shift_single_block_level, block_level++)
   {
      if (block_level > N_INDIRECT_BLOCKS)
      {
         *device_block_p=0;
         return -1;
      }
   }

   temp_block_num = finfo->f_di.i_block[N_DIRECT_BLOCKS - 1 + block_level];
   if (temp_block_num == 0)
   {
      ret = ext2_alloc_block_bit(node, &temp_block_num);
      if(ret)
      {
         *device_block_p=0;
         return -1;
      }
      finfo->f_di.i_block[N_DIRECT_BLOCKS - 1 + block_level] = temp_block_num;
      finfo->f_di.i_blocks += fsinfo->sectors_in_block;
   }
   /* block_level will tell the indirection level. shift_block_level is the shift of the mask */
   for (;block_level;block_level--)
   {
      /* Calculo la posicion del numero de bloque del siguiente nivel */
      index_offset =   (temp_block_num << (10+fsinfo->e2sb.s_log_block_size)) + (
                       block_pos & (( ((1<<shift_single_block_level)-1) << shift_block_level)
                       >> shift_block_level) * sizeof(uint32_t));
      /* Leo el numero de bloque del siguiente nivel */
      ret = ciaaBlockDevices_lseek(bdev, index_offset, SEEK_SET);
      if(ret!=index_offset)
      {
         *device_block_p=0;
         return -1;
      }
      ret = ciaaBlockDevices_read(bdev, (uint8_t *)temp_block_num, sizeof(uint32_t));
      if(ret!=sizeof(uint32_t))
      {
         *device_block_p=0;
         return -1;
      }
      /* Si no existia el bloque, reservarlo */
      if (temp_block_num == 0)
      {
         ret = ext2_alloc_block_bit(node, &temp_block_num);
         if(ret)
         {
            return -1;
         }
         ret = ciaaBlockDevices_lseek(bdev, index_offset, SEEK_SET);
         if(ret!=index_offset)
         {
            *device_block_p=0;
            return -1;
         }
         ret = ciaaBlockDevices_write(bdev, (uint8_t *)temp_block_num, sizeof(uint32_t));
         if(ret!=sizeof(uint32_t))
         {
            *device_block_p=0;
            return -1;
         }
      }
   }
   return 0;
}

/* Tries to alloc a block in the same group as the file */
/* Modifies bookkeeping info */
/* TODO: Only searches a block in the current file group. Should check in all groups */
static int ext2_alloc_block_bit(vnode_t * node, uint32_t *block)
{
   int ret;
   uint32_t i, j;
   uint32_t b;
   uint8_t *bitmap_buffer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   /* Pido memoria del tamanio de un bloque */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   if(bitmap_buffer==NULL)
   {
      *block = 0;
      return -1;
   }

   /* Leo el bitmap del disco */
   ret = ciaaBlockDevices_lseek(bdev, fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size, SEEK_SET);
   if(ret!=fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size)
   {
      *block = 0;
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_read(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   if(ret!=fsinfo->s_block_size)
   {
      *block = 0;
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   
   /*Busco un bit libre en el bitmap
    *En primer lugar busco un byte que contenga un bit en 0 (libre)
    */
   for(i=0; i<fsinfo->s_block_size && bitmap_buffer[i] == 0xFF; i++)
   if(i>=fsinfo->s_block_size)
   {
      /* No hay bloques libres en este grupo */
      *block = 0;
      return -1;
   }
   /* Ahora busco el bit libre dentro del byte encontrado */
   for(j=0; j<8;j++)
   {
      if(!(bitmap_buffer[i]&(1<<j)))
      {
         bitmap_buffer[i] |= (1<<j);
         b = 8*i+j;
         break;
      }
   }
   if(j==8)
   {
      *block = 0;
      return -1;   /* No tendria que suceder nunca */
   }
   /* Escribo el bitmap modificado al disco */
   ret = ciaaBlockDevices_lseek(bdev, fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size, SEEK_SET);
   if(ret!=fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      *block = 0;
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      *block = 0;
      return -1;
   }
   *block = fsinfo->e2sb.s_first_data_block + fsinfo->e2sb.s_blocks_per_group*finfo->f_group + b;
   fsinfo->e2fs_gd[finfo->f_group].free_blocks_count--;
   fsinfo->e2sb.s_free_blocks_count--;
   ciaaPOSIX_free(bitmap_buffer);
   return 0;
}

/* Dealloc given block */
/* Modifies bookkeeping info */
static int ext2_dealloc_block_bit(vnode_t * node, uint32_t block)
{
   int ret;
   uint32_t b;
   uint8_t *bitmap_buffer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   /* Pido memoria del tamanio de un bloque */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   if(bitmap_buffer==NULL)
   {
      return -1;
   }

   /* Leo el bitmap del disco */
   ret = ciaaBlockDevices_lseek(bdev, fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size, SEEK_SET);
   if(ret!=fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_read(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }

   b = block - fsinfo->e2sb.s_first_data_block - (fsinfo->e2sb.s_blocks_per_group*finfo->f_group);
   clrbit(bitmap_buffer, b);

   /* Escribo el byte modificado del bitmap al disco */
   ret = ciaaBlockDevices_lseek(bdev, fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size, SEEK_SET);
   if(ret!=fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)bitmap_buffer, fsinfo->s_block_size);
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   fsinfo->e2fs_gd[finfo->f_group].free_blocks_count++;
   fsinfo->e2sb.s_free_blocks_count++;
   ciaaPOSIX_free(bitmap_buffer);
   return -1;
}

static int ext2_dir_add_entry(vnode_t *node, uint32_t ino, char *name, uint16_t namlen, uint8_t type)
{
   int ret, i;
   /*pointer is the file pointer. data_block is the block where pointer points, mapped to disk*/
   uint32_t data_block, pointer;
   uint16_t new_entry_size;
   uint8_t *block_buffer;
   uint8_t flag_entry_found = 0;
   struct ext2fs_direct *start_pointer, *end_pointer, *dpointer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   new_entry_size = DENTRY_MIN_SIZE + namlen;
   /* Alineo required_space a 4 bytes */
   new_entry_size += (new_entry_size & 0x03) ? (4 - (new_entry_size & 0x03)) : 0;
   block_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   ASSERT_MSG(block_buffer!=NULL, "ext2_dir_add_entry(): Mem error");
   if(block_buffer==NULL)
   {
      return -1;
   }
   for (pointer=0; pointer<finfo->f_di.i_size; pointer+=fsinfo->s_block_size)
   {
      ret = ext2_file_map_alloc(node, pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      ASSERT_MSG(ret==0, "ext2_dir_add_entry(): ext2_file_map_alloc() failed");
      if(ret)
      {
         return -1;
      }
      /* Leo el bloque del directorio */
      ret = ciaaBlockDevices_lseek(bdev, data_block<<(10+fsinfo->e2sb.s_log_block_size) , SEEK_SET);
      ASSERT_MSG(ret==data_block<<(10+fsinfo->e2sb.s_log_block_size), "ext2_dir_add_entry(): ciaaBlockDevices_lseek() failed");
      if(ret!=data_block<<(10+fsinfo->e2sb.s_log_block_size))
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_read(bdev, (uint8_t *)block_buffer, fsinfo->s_block_size);
      ASSERT_MSG(ret==fsinfo->s_block_size, "ext2_dir_add_entry(): ciaaBlockDevices_read() failed");
      if(ret!=fsinfo->s_block_size)
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }

      /* Search an empty entry in this block */
      start_pointer=(struct ext2fs_direct *)block_buffer;
      end_pointer=(struct ext2fs_direct *)(block_buffer + fsinfo->s_block_size);
      for(dpointer=start_pointer, i=0; dpointer<end_pointer && i<10;
         dpointer=(struct ext2fs_direct *)((char*)dpointer + dpointer->e2d_reclen), i++)
      {
         if (dpointer->e2d_ino==0)
         {
            /* Free entry found. Check if it has space for the new entry */
            if (new_entry_size <= dpointer->e2d_reclen)
            {
               flag_entry_found = 1;
               break;
            }
         }
         /* See if there is enough space in padding to add new entry */
         if ( new_entry_size <= (dpointer->e2d_reclen - DENTRY_TOTAL_SIZE(dpointer)) )
         {
            /* Enough space to alloc new entry
             * Must shrink the previous entry before adding the new
             */
            new_entry_size = dpointer->e2d_reclen - DENTRY_TOTAL_SIZE(dpointer);
            dpointer->e2d_reclen = DENTRY_TOTAL_SIZE(dpointer);
            dpointer = (struct ext2fs_direct *)((char*)dpointer + dpointer->e2d_reclen);
            dpointer->e2d_reclen = new_entry_size;
            /* Flag to tell that an entry has been found */
            flag_entry_found = 1;
            break;
         }
      }
      if(flag_entry_found)
      {
         /* dpointer keeps the address of the new entry */
         break;
      }
   }
   /* No free or adequate entry found. Must extend the directory */
   if (!flag_entry_found)
   {
      /* directory is full and no room left in last block */
      ret = ext2_file_map_alloc(node, (finfo->f_di.i_size)>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      ASSERT_MSG(ret==0, "ext2_dir_add_entry(): ext2_file_map_alloc() failed");
      if(ret)
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      /* Leo el nuevo bloque del directorio */
      ret = ciaaBlockDevices_lseek(bdev, data_block<<(10+fsinfo->e2sb.s_log_block_size) , SEEK_SET);
      ASSERT_MSG(ret==data_block<<(10+fsinfo->e2sb.s_log_block_size), "ext2_dir_add_entry(): ciaaBlockDevices_lseek() failed");
      if(ret!=data_block<<(10+fsinfo->e2sb.s_log_block_size))
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_read(bdev, (uint8_t *)block_buffer, fsinfo->s_block_size);
      ASSERT_MSG(ret==fsinfo->s_block_size, "ext2_dir_add_entry(): ciaaBlockDevices_read() failed");
      if(ret!=fsinfo->s_block_size)
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      /* New directory size adds 1 block */
      finfo->f_di.i_size += fsinfo->s_block_size;
      dpointer = (struct ext2fs_direct*) block_buffer;
      dpointer->e2d_reclen = fsinfo->s_block_size;
   }
   /* dpointer now points to a new valid entry. block_buffer contains the modified block with the new entry */
   dpointer->e2d_ino = ino;
   dpointer->e2d_type = type;
   ciaaPOSIX_memcpy(dpointer->e2d_name, name, namlen);
   dpointer->e2d_namlen = namlen;
   /* Write modified block with the new entry to disk */
   ret = ciaaBlockDevices_lseek(bdev, data_block<<(10+fsinfo->e2sb.s_log_block_size) , SEEK_SET);
   ASSERT_MSG(ret==data_block<<(10+fsinfo->e2sb.s_log_block_size), "ext2_dir_add_entry(): ciaaBlockDevices_lseek() failed");
   if(ret!=data_block<<(10+fsinfo->e2sb.s_log_block_size))
   {
      ciaaPOSIX_free(block_buffer);
      return -1;
   }
   ret = ciaaBlockDevices_write(bdev, (uint8_t *)block_buffer, fsinfo->s_block_size);
   ASSERT_MSG(ret==fsinfo->s_block_size, "ext2_dir_add_entry(): ciaaBlockDevices_lseek() failed");
   if(ret!=fsinfo->s_block_size)
   {
      ciaaPOSIX_free(block_buffer);
      return -1;
   }
   ciaaPOSIX_free(block_buffer);
   return 0;
}

static int ext2_dir_delete_entry(vnode_t *node, char *name, uint16_t namlen)
{
   int ret;
   uint32_t data_block;
   uint32_t pointer;
   uint8_t *block_buffer;
   uint8_t flag_found = 0;
   struct ext2fs_direct *start_pointer, *end_pointer, *dpointer, *prev_dpointer;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   block_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   if(block_buffer==NULL)
   {
      return -1;
   }
   for (pointer=0; pointer<finfo->f_di.i_size; pointer+=fsinfo->s_block_size)
   {
      ret = ext2_block_map(node, pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      if(ret)
      {
         return -1;
      }
      if(!data_block)
      {
         return -1;
      }
      /* Leo el bloque del directorio */
      ret = ciaaBlockDevices_lseek(bdev, data_block<<(10+fsinfo->e2sb.s_log_block_size) , SEEK_SET);
      if(ret!=data_block<<(10+fsinfo->e2sb.s_log_block_size))
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      ret = ciaaBlockDevices_read(bdev, (uint8_t *)block_buffer, fsinfo->s_block_size);
      if(ret!=fsinfo->s_block_size)
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }

      /* Search the entry in this block */
      /* When starting, dpointer is the first entry, so no previous entry */
      prev_dpointer = NULL;
      start_pointer=(struct ext2fs_direct *)block_buffer;
      end_pointer=(struct ext2fs_direct *)(block_buffer + fsinfo->s_block_size);
      for(dpointer=start_pointer; dpointer<end_pointer;
         dpointer=(struct ext2fs_direct *)((char*)dpointer + dpointer->e2d_reclen))
      {
         if (dpointer->e2d_ino!=0)
         {
         /* Nonempty entry found. Is it the requested entry? */
            if ((dpointer->e2d_namlen == namlen) &&
               (!ciaaPOSIX_strncmp(dpointer->e2d_name, name, dpointer->e2d_namlen)))
            {
               flag_found = 1;
            }
         }
         if (flag_found) {
            /* Requested entry found. Delete it */
            dpointer->e2d_ino = 0;
            /* Now we have cleared dentry, if it's not the first one,
             * merge it with previous one.  Since we assume, that
             * existing dentry must be correct, there is no way to
             * spann a data block.
             */
            if (prev_dpointer)
            {
               /* Previous entry now spans deleted entry
                * Deleted entry is now padding of previous entry
                */
               prev_dpointer->e2d_reclen += dpointer->e2d_reclen;
            }
            //Escribo el bloque del directorio modificado
            ret = ciaaBlockDevices_lseek(bdev, data_block<<(10+fsinfo->e2sb.s_log_block_size) , SEEK_SET);
            if(ret!=data_block<<(10+fsinfo->e2sb.s_log_block_size))
            {
               ciaaPOSIX_free(block_buffer);
               return -1;
            }
            ret = ciaaBlockDevices_read(bdev, (uint8_t *)block_buffer, fsinfo->s_block_size);
            if(ret!=fsinfo->s_block_size)
            {
               ciaaPOSIX_free(block_buffer);
               return -1;
            }
            /* Required entry found and deleted */
            ciaaPOSIX_free(block_buffer);
            return 0;
         }
         prev_dpointer = dpointer;
      }
   }
   /* Required entry not found in dir */
   ciaaPOSIX_free(block_buffer);
   return -1;
}

/* Writes the on-memory sb and gd to disk */
static int ext2_fsinfo_flush(vnode_t *node)
{
   int ret;
   uint32_t group_offset;
   uint16_t group_index;

   ciaaDevices_deviceType const *bdev;
   //struct ext2fs_dinode * ninfo;
   //struct ext2_file_info *finfo;
   struct ext2_fs_info *fsinfo;

   bdev = node->fs_info.device;
   //ninfo = (struct ext2fs_dinode *)node->n_info.down_layer_info;
   //finfo = (struct ext2_file_info *)node->f_info.down_layer_info;
   fsinfo = (struct ext2_fs_info *)node->fs_info.down_layer_info;

   for(group_index=0; group_index<(fsinfo->s_groups_count); group_index++)
   {
      group_offset = EXT2_SBOFF + fsinfo->e2sb.s_blocks_per_group * fsinfo->s_block_size * group_index;
      /* Si el block size es 1024, block 0 es boot, block 1 es superblock. Blocks 1 a 8192 son blocks del group 1
       * Por lo tanto el boot block no cuenta como block del group 1.
       */
      ret = ciaaBlockDevices_lseek(bdev, group_offset, SEEK_SET);
      ASSERT_MSG(ret==group_offset, "ext2_fsinfo_flush(): ciaaBlockDevices_lseek() failed");
      if(ret!=group_offset)
      {
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)&(fsinfo->e2sb), sizeof(struct ext2sb));
      ASSERT_MSG(ret==sizeof(struct ext2sb), "ext2_fsinfo_flush(): ciaaBlockDevices_write() failed");
      if(ret!=sizeof(struct ext2sb))
      {
         return -1;
      }
      ret = ciaaBlockDevices_lseek(bdev, group_offset + EXT2_SBSIZE, SEEK_SET);
      ASSERT_MSG(ret==group_offset+EXT2_SBSIZE, "ext2_fsinfo_flush(): ciaaBlockDevices_lseek() failed");
      if(ret!=group_offset + EXT2_SBSIZE)
      {
         return -1;
      }
      ret = ciaaBlockDevices_write(bdev, (uint8_t *)fsinfo->e2fs_gd, sizeof(struct ext2_gd) * fsinfo->s_groups_count);
      ASSERT_MSG(ret==sizeof(struct ext2_gd) * fsinfo->s_groups_count, "ext2_fsinfo_flush(): ciaaBlockDevices_write() failed");
      if(ret!=sizeof(struct ext2_gd)*fsinfo->s_groups_count)
      {
         return -1;
      }
   }
   return 0;
}

/*
Here are the rules used to allocate new inodes:

the inode for a new file is allocated in the same group of the inode of its parent directory.
inodes are allocated equally between groups.
Here are the rules used to allocate new blocks:

a new block is allocated in the same group as its inode.
allocate consecutive sequences of blocks.
*/
/*
Determining the Number of Block Groups
From the Superblock, extract the size of each block, the total number of inodes, the total number of blocks, the number of blocks per block group, and the number of inodes in each block group. From this information we can infer the number of block groups there are by:
Rounding up the total number of blocks divided by the number of blocks per block group
Rounding up the total number of inodes divided by the number of inodes per block group
Both (and check them against each other)
*/
/*
Determining the number of inodes
El numero de nodos se debe indicar manualmente por un ratio de blocks/inodes
En linux, mke2fs da un ratio predeterminado de 4
*/
/*
El bitmap de bloque de un grupo esta limitado a solo 1 bloque, entonces:
La cantidad de bloques por grupo es la cantidad de bits en un bloque.
Para el caso de un bloque de 1024 bytes, la cantidad de bloques es 8*1024.
   blocks_per_cg = sblock.e2fs_bsize * NBBY;
   ncg = howmany(bcount - sblock.e2fs.e2fs_first_dblock, blocks_per_cg);
   blocks_gd = howmany(sizeof(struct ext2_gd) * ncg, bsize);
*/
//Max 2.5kb de ram. Es poco.
/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

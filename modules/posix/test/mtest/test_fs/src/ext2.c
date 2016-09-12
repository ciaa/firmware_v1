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

/** \brief Ext2 implementation
 **
 ** Lower fs layer. These functions are managed by the vfs, with the fsdriver_operations structure
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
-Try to implement everything with static memory. Exceptions can occur
-Optimize algorithms in speed and memory usage
-Add new fields to finfo and fsinfo, as they help to avoid calculating every time a function is called
-Implement error codes. By now when an error is found, -1 is returned. No matter which error was produced
-Fix "addtogroup" description from doxygen
-Add more comments
-Optimize space by allocating minimal memory for non open files, and allocating structures only for open files.
For example, make f_di field in ext2_file_info a pointer, and alloc structure only for open files.
-Economize memory. Start with physical inode buffer.
1)physical node buffer for every node. That consumes a lot of memory
2)buffer only for open files.
3)One buffer for all operations.

With the buffer, the number of reads reduces, but the number of writes remain the same in most cases.
A block cache for every thread could solve these problems. No big scalability then needed in fs implementation

Thinking in the diference between 2) and 3).
2) is more scalable than 3).
3) is the most efficient in memory

Is there a problem when multiple threads use the same file? There would be redundant reads. Again, that is solved with a block cache. I think there is no need to have an inode cache if there is a block cache.

FIXME:
-Try to call return only once for every function. Use an auxiliary variable
-Review code entirely to see if the variable types are declared according to requirements
-For example, dont use uint32_t when only uint16_t is needed.
-Put variables in order for good alignment. First uint32_t, then uint16_t, etc.
-Block alloc does not delete block contents. Set them to 0

*/

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdlib.h"
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "ext2.h"
#include "vfs.h"
#include "ciaaLibs_PoolBuf.h"

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
 **    EXT2_FT_UNKNOWN   -Unknown File Type
 **    EXT2_FT_REG_FILE  -Regular File
 **    EXT2_FT_DIR       -Directory File
 **    EXT2_FT_CHRDEV    -Character Device
 **    EXT2_FT_BLKDEV    -Block Device
 **    EXT2_FT_FIFO      -Buffer File
 **    EXT2_FT_SOCK      -Socket File
 **    EXT2_FT_SYMLINK   -Symbolic Link
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

/** \brief maps a block number in sequential file position to the corresponding disk block number
 **
 ** \param[in] dest_node node of file in which to map the block
 ** \param[in] file_block file relative block number
 ** \param[out] device_block_p pointer to the returned device block number
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_block_map(vnode_t * dest_node, uint32_t file_block, uint32_t *device_block_p);

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
static int ext2_get_superblock(ciaaDevices_deviceType const * device, ext2_superblock_t * sb_p);

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
static void print_superblock(ext2_superblock_t * superblock);

/** \brief print the contents of the given ext2 group descriptor structure
 **
 ** \param[in] gd pointer to the raw group descriptor structure to be printed
 ** \return -1 if an error occurs, in other case 0
 **/
static void print_groupdescriptor(ext2_gd_t *gd);

/** \brief print the contents of the given ext2 inode structure
 **
 ** \param[in] inode pointer to the raw group descriptor structure to be printed
 ** \return -1 if an error occurs, in other case 0
 **/
static void print_inode(ext2_inode_t * inode);

/** \brief read from disk an inode to memory
 **
 ** \param[in] dest_node node in which to save the physical inode
 ** \param[in] inumber number of inode to be read to memory
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_read_inode(vnode_t *dest_node, uint32_t inumber);

/** \brief read from disk a group descriptor to memory
 **
 ** \param[in] node node of file which resides in the mount. Can be any node
 ** \param[in] ngroup number of the group descriptor to be read
 ** \param[out] gdp pointer to the on-memory descriptor struct to be filled
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_get_groupdesc(filesystem_info_t *fs_info, uint16_t ngroup, ext2_gd_t *gdp);

/** \brief write to disk a group descriptor from memory
 **
 ** \param[in] node node of file which resides in the mount. Can be any node
 ** \param[in] ngroup number of the group descriptor to be written
 ** \param[out] gdp pointer to the on-memory descriptor struct to be written
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_set_groupdesc(filesystem_info_t *fs_info, uint16_t ngroup, ext2_gd_t *gdp);

/** \brief write to disk a node struct
 **
 ** \param[in] fs_info pointer to file system information structure
 ** \param[in] inumber node number to be written to disk
 ** \param[in] node pointer to the on-memory node struct to be written to disk
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_set_inode(filesystem_info_t *fs_info, uint32_t inumber, ext2_inode_t *node);

/** \brief write to disk a node struct
 **
 ** \param[in] fs_info pointer to file system information structure
 ** \param[in] inumber node number to be written to disk
 ** \param[out] node pointer to the on-memory node struct in which to read
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_get_inode(filesystem_info_t *fs_info, uint32_t inumber, ext2_inode_t *node);

/** \brief search an entry in a directory by its fields
 **
 ** \param[in] dir_node directory in which to look for the entry
 ** \param[in] dir_name name of the entry to be searched
 ** \param[in] length length of dir_name
 ** \param[out] inumber_p points to inode number if success, NULL otherwise
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_search_directory(vnode_t *node, char *name, uint16_t namlen, uint32_t *inumber_p);

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
static int ext2_file_open(file_desc_t *file);

/** \brief close the given file
 **
 ** \param[in] file file to be closed
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_file_close(file_desc_t *file);

/** \brief read the file data
 **
 ** \param[in] file file from which to read data
 ** \param[in] buf buffer whom to fill with data
 ** \param[in] size size of data to be read in bytes
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **/
static size_t ext2_file_read(file_desc_t *file, void *buf, size_t size);

/** \brief write data to the file
 **
 ** \param[in] file file from which to read data
 ** \param[in] buf buffer whom to fill with data
 ** \param[in] size size of data to be read in bytes
 ** \return -1 if failed, a non negative integer representing the count of
 **         read bytes if success
 **/
static size_t ext2_file_write(file_desc_t *file, void *buf, size_t size);

/** \brief throw a command to the file
 **
 ** \param[in] file file to be controlled
 ** \param[in] request command to be executed to the file
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_file_ioctl(file_desc_t *file, int request);

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
static int ext2_format(vnode_t *dev_node, void *param);

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

/** \brief detach filesystem from directory. Also eliminate subtree from file tree and remove on-mem fs info
 **
 ** \param[in] node root node of the filesystem
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_umount(vnode_t *node);

/** \brief remove subtree. For every node remove associated lower layer on-memory info.
 ** \remarks recursive function
 ** \param[in] root root node of the subtree
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_umount_subtree_rec(vnode_t *root);

/** \brief Read data from an offset in a block device
 **
 ** read nbyte from the block device device in offset offset from the beginning
 **
 ** \param[in]  device  device to be read
 ** \param[in]  buf     buffer to store the read data
 ** \param[in]  offset  offset from the beginning of the device
 ** \param[in]  nbyte   count of bytes to be read
 ** \return     the count of bytes read
 **/
static ssize_t ext2_device_buf_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, off_t const offset,
                                 size_t const nbyte);

/** \brief Writes data to an offset of a block device
 **
 ** Writes nbyte to the file descriptor fildes from the buffer buf
 **
 ** \param[in]  device  device to be written
 ** \param[in]  buf     buffer with the data to be written
 ** \param[in]  offset  offset from the beginning of the device
 ** \param[in]  nbyte   count of bytes to be written
 ** \return     the count of bytes written
 **/
static ssize_t ext2_device_buf_write(ciaaDevices_deviceType const * const device, uint8_t * const buf, off_t const offset,
                                 size_t const nbyte);

/** \brief Writes c into each of the first nbyte bytes starting at offset bytes of device
 **
 ** \param[in]  device  device to be written
 ** \param[in]  c     buffer with the data to be written
 ** \param[in]  offset  offset from the beginning of the device
 ** \param[in]  nbyte   count of bytes to be written
 ** \return -1 if an error occurs, in other case 0
 **/
static int ext2_device_buf_memset(ciaaDevices_deviceType const * const device, uint8_t c, off_t const offset,
                                 size_t nbyte);

/*==================[internal data definition]===============================*/

/** \brief ext2 driver operations
 *
 */
fsdriver_operations_t ext2_driver_operations =
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

/** \brief file information structure memory pool
 *
 */
CIAALIBS_POOLDECLARE(ext2_finfo_pool, ext2_file_info_t, VFS_NODES_MAX)
/** \brief file metadata information memory pool
 *
 */
CIAALIBS_POOLDECLARE(ext2_fsinfo_pool, ext2_fs_info_t, EXT2_MAX_MOUNTS)
/** \brief physical node memory pool
 *
 */
//CIAALIBS_POOLDECLARE(ext2_inode_pool, ext2_inode_t, VFS_DESC_MAX)

/** \brief Buffer used to read and write portions of blocks
 *
 */
uint8_t ext2_block_buffer[EXT2_BLOCK_BUFFER_SIZE];
ext2_direntry_t ext2_dir_entry_buffer;
ext2_inode_t ext2_node_buffer;

/*==================[external data definition]===============================*/

/** \brief ext2 driver structure
 *
 * It will be declared as "extern" in vfs.c, so its visible to function vfs_get_driver()
 */
filesystem_driver_t ext2_driver =
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

int ext2_get_superblock(ciaaDevices_deviceType const * device, ext2_superblock_t * sb_p)
{
   int32_t ret;

   ret = ext2_device_buf_read(device, (uint8_t *)sb_p, EXT2_SBOFF, EXT2_SBSIZE);
   if(ret)
   {
      return -1;
   }
   print_superblock(sb_p);

   return 0;
}

static void print_superblock(ext2_superblock_t * superblock)
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

static void print_groupdescriptor(ext2_gd_t *gd)
{

   ciaaPOSIX_printf("\n**Group Descriptor**\n");
   ciaaPOSIX_printf("block_bitmap: %d\n",gd->block_bitmap);     /* Blocks bitmap block */
   ciaaPOSIX_printf("inode_bitmap: %d\n",gd->inode_bitmap);     /* Inodes bitmap block */
   ciaaPOSIX_printf("inode_table: %d\n",gd->inode_table);      /* Inodes table block */
   ciaaPOSIX_printf("free_blocks_count: %d\n",gd->free_blocks_count);   /* Free blocks count */
   ciaaPOSIX_printf("free_inodes_count: %d\n",gd->free_inodes_count);   /* Free inodes count */
   ciaaPOSIX_printf("used_dirs_count: %d\n",gd->used_dirs_count);     /* Directories count */

}

static void print_inode(ext2_inode_t * inode)
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
   ext2_gd_t gd;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = dest_node->fs_info->device;
   finfo = (ext2_file_info_t *)dest_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)dest_node->fs_info->down_layer_info;

   if(NULL == fsinfo || NULL == finfo || NULL == bdev)
   {
      return -1;
   }

   /* Verify to avoid division by 0 */
   if(fsinfo->e2sb.s_inodes_per_group == 0)
   {
      return -1;
   }
   /* Calculate the device inode position with help of on-memory file info*/
   inode_group = (inumber-1)/fsinfo->e2sb.s_inodes_per_group;
   /* (inumber-1) because we start counting from 1 */
   /* Calculate in which group is the target inode to be found */
   ret = ext2_get_groupdesc(dest_node->fs_info, inode_group, &gd);
   if(ret)
   {
      return -1;
   }
   inode_offset = (gd.inode_table)*(fsinfo->s_block_size) +
                  ((inumber-1)%fsinfo->e2sb.s_inodes_per_group)*(fsinfo->e2sb.s_inode_size);
   ret = ext2_device_buf_read(bdev, (uint8_t *)&ext2_node_buffer, inode_offset, sizeof(ext2_inode_t));
   if(ret)
   {
      return -1;
   }

   finfo->f_inumber = inumber;
   finfo->f_group = inode_group;
   finfo->f_size = ext2_node_buffer.i_size;
   print_inode(&ext2_node_buffer);
   return 0;
}

/* Objetivo: Reemplazar todos los malloc con un buffer unico de 1KB.
 * Problema: Si por ejemplo el bloque es de 2KB, los entries no estan alineados a 1KB, sino a 2KB
 * Es decir, pueden aparecer entries que ocupen el final de un bloque de 1KB y el principio del otro
 * Con un bloque de 1KB no puedo abarcar el entry entero. Tengo que buscar una solucion a eso.
 */
static int ext2_search_directory(vnode_t *node, char *name, uint16_t namlen, uint32_t *inumber_p)
{
   uint32_t data_block, block_offset, file_pointer;
   uint16_t entry_pointer;
   int ret;
   uint8_t flag_entry_found = 0;
   //ext2_direntry_t ext2_dir_entry_buffer;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   if(NULL == inumber_p)
      *inumber_p = 0;
   /* size must be multiple of block size. Dont consider special cases */
   /* jump from block to block in every iteration */
   for(file_pointer = 0; file_pointer < finfo->f_size; file_pointer += fsinfo->s_block_size)
   {
      /* Map file block to disk block. Result in data_block */
      ret = ext2_block_map(node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      if(ret)
      {
         return -1;
      }
      /* block_offset points to the first byte address of the current block */
      block_offset = data_block<<(10+fsinfo->e2sb.s_log_block_size);
      /* entry pointer iterates sequentially over the entries until it reaches the end of the block */
      for(entry_pointer = 0; entry_pointer < fsinfo->s_block_size;
            entry_pointer += ext2_dir_entry_buffer.rec_len)
      {
         /* Read entry fields, except for the name. 8 bytes in total */
         ret = ext2_device_buf_read(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
         if(ret)
         {
            return -1;
         }
         if (0 != ext2_dir_entry_buffer.inode)
         {
            if(ext2_dir_entry_buffer.name_len == namlen)
            {
               /* Read the name of the entry */
               ret = ext2_device_buf_read(bdev, (uint8_t *)((uint8_t *)&ext2_dir_entry_buffer + 8), block_offset + entry_pointer + 8,
                                          ext2_dir_entry_buffer.name_len);
               if(ret)
               {
                  return -1;
               }
               if(!ciaaPOSIX_strncmp(ext2_dir_entry_buffer.name, name, ext2_dir_entry_buffer.name_len))
               {
                  flag_entry_found = 1;
                  *inumber_p = ext2_dir_entry_buffer.inode;
                  break;
               }
            }
         }
      }
      if(flag_entry_found)
      {
         /* entry_pointer keeps the address of the entry to be deleted
          * block_offset keeps the address of the block to be modified
          */
         break;
      }
   }
   if (!flag_entry_found)
   {
      return -1;
   }
   return 0;
}

static node_type_t ext2type_to_vfstype(uint8_t ext2_type)
{
   switch (ext2_type) {
   case EXT2_FT_REG_FILE: return VFS_FTREG;
   case EXT2_FT_DIR: return VFS_FTDIR;
   default: return VFS_FTUNKNOWN;
   }
}

static int ext2_file_open(file_desc_t *file)
{
   ext2_file_info_t *finfo;
   //int ret;
   finfo = file->node->f_info.down_layer_info;
   /*
   finfo->pinode = (ext2_inode_t *) ciaaLibs_poolBufLock(&ext2_inode_pool);
   ASSERT_MSG(NULL != finfo->pinode, "ext2_file_open(): ciaaLibs_poolBufLock() failed");
   if(NULL == finfo->pinode)
   {
      return -1;
   }
   ret = ext2_get_inode(&(file->node->fs_info), finfo->f_inumber, finfo->pinode);
   if(0 > ret)
   {
      return -1;
   }
   */
   finfo->f_pointer = 0;
   /* reset seek pointer */

   return 0;
}

static int ext2_file_close(file_desc_t *file)
{
   //int ret;
   //ext2_file_info_t *finfo;
   /*
   finfo = file->node->f_info.down_layer_info;
   ret = ciaaLibs_poolBufFree(&ext2_inode_pool, finfo->pinode);
   ASSERT_MSG(1 == ret, "ext2_file_close(): ciaaLibs_poolBufFree() failed");
   if(1 != ret)
   {
      return -1;
   }
   finfo->pinode = NULL;
   */

   return 0;
}

static size_t ext2_file_read(file_desc_t *file, void *buf, size_t size)
{
   uint32_t read_size;
   int ret;

   /*TODO: Validate file->cursor */
   ret = ext2_buf_read_file(file->node, (uint8_t *)buf, size, &read_size);
   ASSERT_MSG(0 == ret, "ext2_file_read(): ext2_buf_read_file() failed");
   if(ret)
   {
      return 0;
   }
   file->cursor += read_size;
   return read_size;
}

static int ext2_file_ioctl(file_desc_t *file, int request)
{
   return 0;
}

static int ext2_init(void *par)
{
   return 0;
}

static int ext2_format(vnode_t *dev_node, void *param)
{
   int ret;
   ciaaDevices_deviceType const *bdev;
   ciaaDevices_blockType blockInfo;
   ext2_format_param_t format_parameters;

   ext2_superblock_t superblock;
   ext2_gd_t gd_buffer;
   ext2_inode_t node;

   uint16_t i, aux,group_index;
   uint16_t ngroups, nblocks_gd, inodes_per_group, inodeblocks_per_group,
            minmetablocks_per_group,  nblocks_last_group, nblocks_group_overhead;
   uint32_t block_offset, free_blocks_count, free_inodes_count, write_offset;
   uint32_t group_offset;

   //uint16_t   s_buff_size;           /* Size of the block chunks to be read in buffer */
   //uint8_t    s_buff_per_block;      /* How much chunks per block */
   uint8_t   aux_byte;

   bdev = dev_node->fs_info->device;
   ASSERT_MSG(dev_node->f_info.type == VFS_FTBLK, "ext2_format(): not a block device");
   if(dev_node->f_info.type != VFS_FTBLK)
   {
      return -1;   /* Only a device can be formatted. This is not a device node */
   }

   ret = ciaaBlockDevices_ioctl(bdev, ciaaPOSIX_IOCTL_BLOCK_GETINFO, &blockInfo);
   ASSERT_MSG(ret >= 0, "ext2_format(): ciaaBlockDevices_ioctl() failed");
   if(ret < 0)
   {
      return -1;
   }
   if(param == NULL)
   {
      format_parameters.partition_size = blockInfo.lastPosition;
      format_parameters.block_size = EXT2_MIN_BLOCKSIZE;
      format_parameters.block_node_factor = EXT2_DEFAULT_BLOCKNODE_FACTOR;
   }
   else
   {
      ciaaPOSIX_memcpy((void *)&format_parameters, (void *)param, sizeof(ext2_format_param_t));
      if(format_parameters.block_size < EXT2_MIN_BLOCKSIZE)
         format_parameters.block_size = EXT2_MIN_BLOCKSIZE;
      if(format_parameters.block_size > EXT2_MAX_BLOCKSIZE)
         format_parameters.block_size = EXT2_MAX_BLOCKSIZE;
      if(!(is_powerof2(format_parameters.block_size)))
         return -1;
      if(!(format_parameters.block_node_factor >= 2 && format_parameters.block_node_factor <= 10))
         format_parameters.block_node_factor = EXT2_DEFAULT_BLOCKNODE_FACTOR;
      inodes_per_group = (format_parameters.block_size * 8) / format_parameters.block_node_factor;
      inodeblocks_per_group = howmany(128 * inodes_per_group, format_parameters.block_size);
      minmetablocks_per_group = 1 /*Superblock*/ + 1 /*gd block*/ + 1 /*Block bitmap*/ +
                                 1 /*Inode bimap*/ + inodeblocks_per_group + 2 /*At least 2 data blocks*/;
      format_parameters.partition_size = blockInfo.lastPosition;
   }

   ASSERT_MSG(blockInfo.lastPosition >= minmetablocks_per_group, "ext2_format(): Device size is too small");
   if(blockInfo.lastPosition < minmetablocks_per_group)
   {
      /* Device size is not enough to hold minimum file system data */
      return -1;
   }

   /* Initialize default ext2 superblock contents */
   ciaaPOSIX_memset((uint8_t *)&superblock, 0, sizeof(ext2_superblock_t));

   for(i = 0, aux = 1024; aux < format_parameters.block_size; i++, aux = 1024 << i);   /* blocksize log2 */
   superblock.s_log_block_size = i;
   superblock.s_log_frag_size = superblock.s_log_block_size;
   superblock.s_first_data_block = (format_parameters.block_size > EXT2_SBOFF) ? 0 : 1;  /* First Data Block */
   superblock.s_blocks_per_group = format_parameters.block_size*8;  /* Number of blocks per group */
   superblock.s_frags_per_group = superblock.s_blocks_per_group;   /* Number of fragments per group */
   superblock.s_wtime = 0;             /* Last write time */
   superblock.s_mnt_count = 0;            /* Current mount count */
   superblock.s_max_mnt_count = -1;        /* Max mount count */
   superblock.s_magic = EXT2_MAGIC;                /* Magic number */
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
   superblock.s_blocks_count = format_parameters.partition_size / format_parameters.block_size;
   /* (block/node) factor = 4 */
   superblock.s_inodes_count = superblock.s_blocks_count / format_parameters.block_node_factor;
   /* Reserved blocks count */
   superblock.s_r_blocks_count = 0;
   /* Number of block groups */
   ngroups = howmany(superblock.s_blocks_count - superblock.s_first_data_block,
                     superblock.s_blocks_per_group);
   /* Number of blocks reserved for the group descriptors in every group */
   nblocks_gd = howmany(sizeof(ext2_gd_t)*ngroups, format_parameters.block_size);
   /* Number of nodes which fit in a single block */
   inodes_per_group = superblock.s_inodes_count / ngroups;
   /* Number of blocks reserved for physical node allocations */
   inodeblocks_per_group = howmany(superblock.s_inode_size * inodes_per_group, format_parameters.block_size);
   /* Number of total metadata blocks per group */
   minmetablocks_per_group = 1 /*Superblock*/ + nblocks_gd + 1 /*Block bitmap*/ +
                     1 /*Inode bimap*/ + inodeblocks_per_group + 1 /*At least 1 data block*/;
   ASSERT_MSG(format_parameters.partition_size > minmetablocks_per_group, "ext2_format(): Device size is too small");
   if(format_parameters.partition_size <= minmetablocks_per_group)
   {
      /* Device size is not enough to hold minimum file system data */
      return -1;
   }
   /* The last group contains generally less blocks than the others. It contains the remaining blocks */
   nblocks_last_group = superblock.s_blocks_count - superblock.s_first_data_block -
                        superblock.s_blocks_per_group * (ngroups - 1);

   /* If the last group contains less blocks than the minimum allowed, eliminate the group */
   if(nblocks_last_group < minmetablocks_per_group)
   {
      superblock.s_blocks_count -= nblocks_last_group;
      ngroups--;
      nblocks_last_group = superblock.s_blocks_per_group;
      nblocks_gd = howmany(sizeof(ext2_gd_t)*ngroups, format_parameters.block_size);
      inodes_per_group = superblock.s_inodes_count / ngroups;
   }

   /* The number of nodes in a group must be multiple of 8 */
   superblock.s_inodes_per_group = rounddown(inodes_per_group, 8);
   /* Total node count in disk */
   superblock.s_inodes_count = inodes_per_group * ngroups;
   /* Recalculate number of nodes which fit in a single block */
   inodeblocks_per_group = howmany(superblock.s_inode_size * inodes_per_group, format_parameters.block_size);
   /* Overhead blocks per group */
   nblocks_group_overhead = 1/*Superblock*/ + nblocks_gd + 1 /*Blockbitmap*/+ 1 /*Inodebitmap*/+
                              inodeblocks_per_group;
   /* Size of the block chunks to be read in buffer */
   //s_buff_size = (EXT2_BLOCK_BUFFER_SIZE > format_parameters.block_size) ? EXT2_BLOCK_BUFFER_SIZE : format_parameters.block_size;
   //s_buff_per_block = format_parameters.block_size / s_buff_size;      /* How much chunks per block */
   print_superblock(&superblock);

   free_inodes_count=free_blocks_count=0;
   for(group_index=0; group_index<ngroups; group_index++)
   {
      /*** INODE GROUPS DESCRIPTORS ***/
      block_offset = superblock.s_first_data_block + superblock.s_blocks_per_group*group_index;
      block_offset += 1 /*Superblock*/+ nblocks_gd;
      gd_buffer.block_bitmap = block_offset;
      block_offset += 1; /*Block bitmap*/
      gd_buffer.inode_bitmap = block_offset;
      block_offset += 1; /*Inode table*/
      gd_buffer.inode_table = block_offset;
      if(group_index == ngroups-1)
         gd_buffer.free_blocks_count = nblocks_last_group -
                                                      nblocks_group_overhead /*Overhead*/;
      else if(group_index == 0)
         /* Reserve a block for root entries */
         gd_buffer.free_blocks_count = superblock.s_blocks_per_group - nblocks_group_overhead - 1;
      else
         gd_buffer.free_blocks_count = superblock.s_blocks_per_group - nblocks_group_overhead /*Overhead*/;
      free_blocks_count += gd_buffer.free_blocks_count;
      if(group_index == 0)
         gd_buffer.free_inodes_count = superblock.s_inodes_per_group - EXT2_RESERVED_INODES;
      else
         gd_buffer.free_inodes_count = superblock.s_inodes_per_group;

      free_inodes_count += gd_buffer.free_inodes_count;
      gd_buffer.used_dirs_count = (group_index == 0) ? 1 : 0; /* Consider the root dir */
      /* Write data to every group in the partition */
      for(i=0, group_offset = superblock.s_first_data_block*format_parameters.block_size; i<ngroups;
            i++, group_offset += superblock.s_blocks_per_group*format_parameters.block_size)
      {
         ret = ext2_device_buf_write(bdev, (uint8_t *)&gd_buffer,
                                       group_offset + format_parameters.block_size +
                                       group_index*sizeof(ext2_gd_t), sizeof(ext2_gd_t));
         ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
         if(ret)
         {
            return -1;
         }
      }


      /*** BLOCK BITMAP ***/
      if(group_index == 0)
      {
         nblocks_group_overhead += 1; /* Consider the block containing root dir entries */
      }
      block_offset = gd_buffer.block_bitmap * format_parameters.block_size;
      /* Must set non existent blocks as reserved
       * Must set metadata blocks as reserved
       */
      /* Set the first bits representing busy blocks */
      ret = ext2_device_buf_memset(bdev, 0xFF, block_offset, nblocks_group_overhead/8);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
      if(ret < 0)
      {
         return -1;
      }
      /* Clear the last bits representing free blocks */
      ret = ext2_device_buf_memset(bdev, 0x00, block_offset + nblocks_group_overhead/8,
                                    format_parameters.block_size - nblocks_group_overhead/8);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
      if(ret < 0)
      {
         return -1;
      }
      /* Before this, whole bytes were set or cleared. There is a byte which could not be 0x00 or 0xFF.
       * Set the bits of this byte individually.
       */
      aux_byte = 0;
      for(i=0; i<(nblocks_group_overhead%8);i++)
      {
         setbit(&aux_byte,i);
      }
      ret = ext2_device_buf_write(bdev, (uint8_t *)&aux_byte, block_offset + nblocks_group_overhead/8, 1);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
      if(ret < 0)
      {
         return -1;
      }
      if(group_index == 0)
      {
         nblocks_group_overhead -= 1; /* Consider the block containing root dir entries */
      }


      /*** INODE BITMAP ***/
      /* The root inode is already considered in the reserved inodes */
      block_offset = gd_buffer.inode_bitmap * format_parameters.block_size;
      /* Inodes are all free at the beginning. Alloc special nodes later */
      /* Clear the first bits corresponding to free inodes */
      ret = ext2_device_buf_memset(bdev, 0x00, block_offset, superblock.s_inodes_per_group/8);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
      if(ret < 0)
      {
         return -1;
      }
      /* Set the last bits representing non-existent inodes */
      ret = ext2_device_buf_memset(bdev, 0xFF, block_offset + superblock.s_inodes_per_group/8,
                                    format_parameters.block_size - superblock.s_inodes_per_group/8);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
      if(ret < 0)
      {
         return -1;
      }
      /* Before this, whole bytes were set or cleared. There is a byte which could not be 0x00 or 0xFF.
       * Set the bits of this byte individually.
       */
      aux_byte = 0xFF;
      for(i=0; i<(superblock.s_inodes_per_group%8);i++)
      {
         clrbit(&aux_byte,i);
      }
      ret = ext2_device_buf_write(bdev, (uint8_t *)&aux_byte, block_offset + superblock.s_inodes_per_group/8, 1);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
      if(ret)
      {
         return -1;
      }
      if (group_index == 0)
      {
         /* mark reserved inodes in first group */
         ret = ext2_device_buf_memset(bdev, 0xFF, block_offset, EXT2_RESERVED_INODES/8);
         ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
         if(ret)
         {
            return -1;
         }
         /* Before this, whole bytes were set or cleared. There is a byte which could not be 0x00 or 0xFF.
          * Set the bits of this byte individually.
          */
         aux_byte = 0;
         for(i=0; i<(EXT2_RESERVED_INODES%8);i++)
         {
            setbit(&aux_byte,i);
         }
         ret = ext2_device_buf_write(bdev, (uint8_t *)&aux_byte, block_offset + EXT2_RESERVED_INODES/8, 1);
         ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
         if(ret)
         {
            return -1;
         }
      }
   }
   /* These values change as new files are created */
   superblock.s_free_inodes_count = free_inodes_count;
   superblock.s_free_blocks_count = free_blocks_count;
   /* write on-memory superblock to all groups */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      group_offset = (superblock.s_first_data_block + superblock.s_blocks_per_group) * format_parameters.block_size * group_index;
      /* If the block size is 1024, block 0 is boot block, block 1 is superblock. Blocks 1 to 8192 are group 1 blocks
       * Boot block dont count as group 1 block
       */
      /* If the block size is 2048, block 0 contains boot block and superblock.
       */
      write_offset = (group_index == 0) ? EXT2_SBOFF : group_offset;
      /* Clear all superblock bytes */
      ret = ext2_device_buf_memset(bdev, 0, write_offset, EXT2_SBSIZE);
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
      if(ret)
      {
         return -1;
      }
      superblock.s_block_group_nr = group_index;
      /* Copy superblock to disk */
      ret = ext2_device_buf_write(bdev, (uint8_t *)&superblock, write_offset, sizeof(ext2_superblock_t));
      ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
      if(ret)
      {
         return -1;
      }
   }

   /* Read first group descriptor */
   ret = ext2_device_buf_read(bdev, (uint8_t *)&gd_buffer, (superblock.s_first_data_block + 1)*format_parameters.block_size,
                               sizeof(ext2_gd_t));
   ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_read() failed");
   if(ret)
   {
      return -1;
   }
   /* Clean all reserved inodes */
   ret = ext2_device_buf_memset(bdev, 0, gd_buffer.inode_table * format_parameters.block_size,
                                 superblock.s_inode_size*EXT2_RESERVED_INODES);
   ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_memset() failed");
   if(ret)
   {
      return -1;
   }
   /* Create root directory */
   ciaaPOSIX_memset(&node, 0, sizeof(ext2_inode_t));
   node.i_mode = 040755;
   node.i_uid = node.i_gid = 1000;
   node.i_size = format_parameters.block_size;
   node.i_atime = node.i_ctime = node.i_mtime = 0;
   node.i_dtime = 0;
   node.i_links_count = 2;
   node.i_blocks = format_parameters.block_size / 512;

   /* Reserve free block in first group for root directory data */
   /* The block was already reserved when setting block bitmap bits. Its the first block after overhead blocks */
   /* This block will be assigned to node.i_block[0] */
   node.i_block[0] = nblocks_group_overhead;

   /* Fill root inode */
   ret = ext2_device_buf_write(bdev, (uint8_t *)&node, gd_buffer.inode_table * format_parameters.block_size +
                               superblock.s_inode_size, sizeof(ext2_inode_t)); /* Second inode in table. ROOT INODE */
   ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
   if(ret)
   {
      return -1;
   }

   /* Create root entry */

   /* Self entry */
   ext2_dir_entry_buffer.inode = 2;
   ext2_dir_entry_buffer.rec_len = 12;
   ext2_dir_entry_buffer.name_len = 1;
   ext2_dir_entry_buffer.file_type = 2;
   ciaaPOSIX_strcpy(ext2_dir_entry_buffer.name, ".");

   ret = ext2_device_buf_write(bdev, (uint8_t *)&ext2_dir_entry_buffer, node.i_block[0]*format_parameters.block_size, sizeof(ext2_direntry_t)); /* First entry, "." */
   ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
   if(ret)
   {
      return -1;
   }

   /* Parent entry */
   ext2_dir_entry_buffer.inode = 2;
   ext2_dir_entry_buffer.rec_len = format_parameters.block_size - 12;
   ext2_dir_entry_buffer.name_len = 2;
   ext2_dir_entry_buffer.file_type = 2;
   ciaaPOSIX_strcpy(ext2_dir_entry_buffer.name, "..");

   ret = ext2_device_buf_write(bdev, (uint8_t *)&ext2_dir_entry_buffer, node.i_block[0]*format_parameters.block_size+12, sizeof(ext2_direntry_t)); /* Second entry, ".." */
   ASSERT_MSG(ret >= 0, "ext2_format(): ext2_device_buf_write() failed");
   if(ret)
   {
      return -1;
   }

   return 0;
}

/* Set root info in mountpoint */
/* Preconditions: dest_node is allocated in vfs */
static int ext2_mount(vnode_t *dev_node, vnode_t *dest_node)
{
   int ret;
   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   /* FIXME: This implementation needs the mount directory to be non existant.
    *  Does not overwrite directory correctly
    */
   /* TODO: Root inode is a special case? */
   finfo=dest_node->f_info.down_layer_info = (ext2_file_info_t *) ciaaLibs_poolBufLock(&ext2_finfo_pool);
   ASSERT_MSG(NULL != finfo, "ext2_mount(): Mem error, could not alloc finfo");
   if(NULL == finfo)
   {
      return -1;
   }
   fsinfo=dest_node->fs_info->down_layer_info = (ext2_fs_info_t *) ciaaLibs_poolBufLock(&ext2_fsinfo_pool);
   ASSERT_MSG(NULL != fsinfo, "ext2_mount(): Mem error, could not alloc fsinfo");
   if(NULL == fsinfo)
   {
      return -1;
   }
   /* Load file system information to memory */
   bdev = dest_node->fs_info->device;
   ret = ext2_get_superblock(bdev, &(fsinfo->e2sb));
   ASSERT_MSG(ret>=0, "ext2_mount(): Could not get superblock");
   if(ret)
   {
      return -1;
   }
   /* Calculate the number of group descriptors from the superblock info */
   fsinfo->s_groups_count = (fsinfo->e2sb.s_inodes_count)/(fsinfo->e2sb.s_inodes_per_group);
   fsinfo->s_block_size = 1024 << fsinfo->e2sb.s_log_block_size;
   fsinfo->sectors_in_block = fsinfo->s_block_size >> 9;   /* s_block_size/512 */
   fsinfo->s_inodes_per_block = (fsinfo->s_block_size)/(fsinfo->e2sb.s_inode_size);
   /* Size of the block chunks to be read in buffer */
   fsinfo->s_buff_size = (EXT2_BLOCK_BUFFER_SIZE < fsinfo->s_block_size) ? EXT2_BLOCK_BUFFER_SIZE : fsinfo->s_block_size;
   fsinfo->s_buff_per_block = fsinfo->s_block_size / fsinfo->s_buff_size;      /* How much chunks per block */

   ret = ext2_mount_load(dest_node);
   ASSERT_MSG(ret>=0, "ext2_mount(): mount load failed");
   return ret;
}

static int ext2_create_node(vnode_t *parent_node, vnode_t *child_node)
{
   int ret;

   /* Node creation depends on the file type. A directory needs to have 2 default directories after creation */
   if(VFS_FTDIR == child_node->f_info.type)
   {
      ret = ext2_create_directory_file(parent_node, child_node);
   }
   else if(VFS_FTREG == child_node->f_info.type)
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

   if(VFS_FTDIR == child_node->f_info.type)
   {
      ret = ext2_delete_directory_file(parent_node, child_node);
      ASSERT_MSG(-1 < ret, "ext2_delete_node(): ext2_delete_directory_file() failed");
   }
   else if(VFS_FTREG == child_node->f_info.type)
   {
      ret = ext2_delete_regular_file(parent_node,child_node);
      ASSERT_MSG(-1 < ret, "ext2_delete_node(): ext2_delete_regular_file() failed");
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

/* FIXME: Mount and umount shouldnt eliminate mount root node. This implementation does. Review */
/* FIXME: Ignoring mounts inside another mount. Should consider this case */
/* TODO: set s_state in superblock when mounting and unmounting. */
static int ext2_umount(vnode_t *root)
{
   int ret;
   vnode_t *parent;

   if(NULL == root)
   {
      return -1;
   }
   parent = root->parent_node;
   if(NULL == parent)
   {
      /* Cant delete root */
      return -1;
   }
   if(VFS_FTDIR == root->f_info.type)
   {
      /* Recursively delete all subtrees under this subroot */
      ret = ext2_umount_subtree_rec(root->child_node);
      if(ret)
      {
         return -1;
      }
   }
   if(NULL != root->f_info.down_layer_info)
   {
      ret = ciaaLibs_poolBufFree(&ext2_finfo_pool, root->f_info.down_layer_info);
      ASSERT_MSG(1 == ret, "vfs_node_free(): ciaaLibs_poolBufFree() failed");
      if(1 != ret)
      {
         return -1;
      }
      root->f_info.down_layer_info = NULL;
   }
   if(NULL != root->fs_info->down_layer_info)
   {
      ret = ciaaLibs_poolBufFree(&ext2_fsinfo_pool, root->fs_info->down_layer_info);
      ASSERT_MSG(1 == ret, "vfs_node_free(): ciaaLibs_poolBufFree() failed");
      if(1 != ret)
      {
         return -1;
      }
      root->fs_info->down_layer_info = NULL;
   }
   /* Unlink subroot and delete it */
   ret = vfs_delete_child(root);
   if(ret)
   {
      return -1;
   }
   return 0;
}

static int ext2_umount_subtree_rec(vnode_t *root)
{
   vnode_t *snode;
   int ret;

   if(NULL == root)
   {
      return -1;
   }
   snode = root->sibling_node;
   if(VFS_FTDIR == root->f_info.type)
   {
      ret = ext2_umount_subtree_rec(root->child_node);
      if(ret)
      {
         return -1;
      }
   }

   if(NULL != root->f_info.down_layer_info)
   {
      ret = ciaaLibs_poolBufFree(&ext2_finfo_pool, root->f_info.down_layer_info);
      ASSERT_MSG(1 == ret, "vfs_node_free(): ciaaLibs_poolBufFree() failed");
      if(1 != ret)
      {
         return -1;
      }
      root->f_info.down_layer_info = NULL;
   }
   ret = vfs_node_free(root);
   if(ret)
   {
      return -1;
   }
   ret = ext2_umount_subtree_rec(snode);
   if(ret)
   {
      return -1;
   }
   return 0;
}

static size_t ext2_file_write(file_desc_t *desc, void *buf, size_t size)
{
   int ret;

   vnode_t *node;
   ciaaDevices_deviceType const *bdev;
   ext2_inode_t *pinode;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   node = desc->node;
   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   uint32_t buf_pos, remaining_write_size, device_block;
   uint32_t write_size, write_offset;

   if(NULL == finfo || NULL == fsinfo)
   {
      return 0;
   }

   finfo->f_pointer = desc->cursor;
   buf_pos = 0;
   remaining_write_size = size;
   while (remaining_write_size > 0) {
      /* Position inside current block */
      write_offset = finfo->f_pointer % fsinfo->s_block_size;
      /* #bytes to write to current block */
      write_size = fsinfo->s_block_size - write_offset;
      /* remaining bytes to write less than block size */
      if (write_size > remaining_write_size)
         write_size = remaining_write_size;
      /* map file block to disk block */
      ret = ext2_file_map_alloc(node, finfo->f_pointer >> (10+fsinfo->e2sb.s_log_block_size), &device_block);
      if(ret)
      {
         return 0;
      }
      /* write to current block */
      ret = ext2_device_buf_write(bdev, (uint8_t *)buf + buf_pos,
                                  (device_block<<(10+fsinfo->e2sb.s_log_block_size))+write_offset, write_size);
      if(ret)
      {
         return -1;
      }
      /* update iterators */
      finfo->f_pointer += write_size;
      buf_pos += write_size;
      remaining_write_size -= write_size;
   }
   /* all bytes written. Update pointers */
   desc->cursor = finfo->f_pointer;
   if(finfo->f_pointer > finfo->f_size)
      finfo->f_size = finfo->f_pointer;
   /* finfo->pinode->i_blocks was refreshed everytime ext2_file_map_alloc() was called.
    * Write inode in its position in disk
    * flush memory info to disk
    */
   /* Refresh inode in disk */
   pinode = &ext2_node_buffer;
   ret = ext2_get_inode(node->fs_info, finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   pinode->i_size = finfo->f_size;
   ret = ext2_set_inode(node->fs_info, finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   /* Write to disk the changes made to superblock and block descriptors */
   ret = ext2_fsinfo_flush(node);
   if(ret)
   {
      return 0;
   }

   return buf_pos;
}



static int ext2_buf_read_file(vnode_t * dest_node, uint8_t *buf, uint32_t size, uint32_t *total_read_size_p)
{
   /* FIXME: If size>finfo->f_size error or truncate? */
   int ret;
   uint32_t file_block, block_offset;
   uint32_t device_block, device_offset;
   uint16_t block_size, block_shift, block_mask;
   uint32_t total_remainder_size, block_remainder, read_size, buf_pos;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;
   ciaaDevices_deviceType const *device;

   fsinfo = dest_node->fs_info->down_layer_info;
   finfo = dest_node->f_info.down_layer_info;
   device = dest_node->fs_info->device;

   block_size = fsinfo->s_block_size;
   block_shift = (uint16_t)fsinfo->e2sb.s_log_block_size + 10;
   block_mask = (uint32_t)block_size-1;

   file_block = finfo->f_pointer >> block_shift;
   block_offset = finfo->f_pointer & block_mask;

   if(NULL != total_read_size_p)
      *total_read_size_p = 0;

   if(finfo->f_pointer > finfo->f_size)
      finfo->f_pointer = finfo->f_size;
   /*Truncate read size*/
   if(finfo->f_pointer + size > finfo->f_size)
      size = finfo->f_size - finfo->f_pointer;

   /* Must read total_remainder_size bytes from file.
    * Cant read all in a row. It must be read block by block.
    * First the disk block must be retrieved, which matches the actual file pointer.
    * Start reading this block in position (offset%block_size).
    * Must read at most to the end of this block.
    * Copy chunk of read data to buffer.
    * Subtract read bytes from total_remainder_size. Add read bytes to fpointer.
    * Check loop condition and repeat iteration if valid
    */

   /* Init: total_remainder_size = size.
    * Condition: total_remainder_size>0
    */
   for(   total_remainder_size = size, buf_pos = 0;
      total_remainder_size>0;)
   {
      file_block = finfo->f_pointer >> block_shift;
      block_offset = finfo->f_pointer & block_mask;
      ret = ext2_block_map(dest_node, file_block, &device_block);
      ASSERT_MSG(0 == ret, "ext2_buf_read_file(): ext2_block_map() failed");
      if(ret)
      {
         if(NULL != total_read_size_p)
            *total_read_size_p = buf_pos;
         return ret;
      }
      device_offset = (device_block << block_shift) + block_offset;

      block_remainder = block_size - block_offset;
      if(total_remainder_size > block_remainder)
         read_size = block_remainder;
      else
         read_size = total_remainder_size;

      ret = ext2_device_buf_read(device, (uint8_t *)(buf+buf_pos), device_offset, read_size);
      ASSERT_MSG(0 == ret, "ext2_buf_read_file(): ext2_device_buf_read() failed");
      if(ret)
      {
         if(NULL != total_read_size_p)
            *total_read_size_p = buf_pos;
         return -1;
      }
      buf_pos += read_size;
      finfo->f_pointer += read_size;
      total_remainder_size -= read_size;
   }

   if(NULL != total_read_size_p)
      *total_read_size_p = buf_pos;
   ASSERT_MSG(buf_pos == size, "ext2_buf_read_file(): buf_pos == size failed");
   if(buf_pos != size)
      return -1;

   return 0;
}

/* Map file block number to disk block number
 *
 */
static int ext2_block_map(vnode_t * dest_node, uint32_t file_block, uint32_t *device_block_p)
{
   uint32_t shift_block_level, shift_single_block_level;
   uint32_t temp_block_num, n_entries_per_block, index_offset, aux, block_level;
   int ret;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;
   ciaaDevices_deviceType const *device;
   ext2_inode_t *pinode;

   device = dest_node->fs_info->device;
   fsinfo = dest_node->fs_info->down_layer_info;
   finfo = dest_node->f_info.down_layer_info;
   n_entries_per_block = fsinfo->s_block_size / sizeof(uint32_t);

   ret = ext2_get_inode(dest_node->fs_info, finfo->f_inumber, &ext2_node_buffer);
   if(0 > ret)
   {
      return -1;
   }
   pinode = &ext2_node_buffer;

   /* shift_single_level is the number of bits of the block indirection level mask */
   for(shift_single_block_level = 0, aux = n_entries_per_block; aux; shift_single_block_level++)
   {
      aux = aux>>1;
   }

   if (file_block < N_DIRECT_BLOCKS)
   {
      /* Direct block */
      *device_block_p = pinode->i_block[file_block];
      return 0;
   }

   file_block -= N_DIRECT_BLOCKS;

   for (   shift_block_level = 0, block_level = 1;
      file_block >= (int32_t)1<<(shift_single_block_level+shift_block_level);
      file_block -= (int32_t)1<<(shift_single_block_level+shift_block_level),
      shift_block_level += shift_single_block_level, block_level++)
   {

   /* In every iteration a new block indirection level is reached.
    * In every loop, file_block is checked against the current block range
    * according to the current indirection (see table below).
    * The block range of the previous indirection level is subtracted from file_block
    * When the loop finishes, the maped block number is the offset inside the last indirection level reached,
    * and block_level leaves with the current indirection level
    */
   /* Indirection level and corresponding block ranges
    * 1024B/4B = 256 blocks
    * 1 2 4 8 16 32 64 128 256 512 1024
    * 0 1 2 3 4  5  6  7   8   9   10
    * N_INDIR_BLOCKS_PER_BLOCK: BLOCK_SIZE / sizeof(uint32_t)
    * N_INDIR_BLOCKS_PER_BLOCK**2
    * N_INDIR_BLOCKS_PER_BLOCK**3
    */

      if(file_block < (int32_t) 1 << shift_block_level)
      {
         /* The block number is within current indirection level block range */
         break;
      }
      if (block_level > N_INDIRECT_BLOCKS)
      {
         /* Only 3 indirection levels are implemented in ext2. block_level is out of range */
         *device_block_p = 0;
         return -1;
      }
   }

   /* In which of the three N_INDIRECT_BLOCKS ranges is the file block number?
    * If block_level=1, top indirect block number is i_blocks[12], indirection level 1
    * If block_level=2, top indirect block number is i_blocks[13], indirection level 2
    * If block_level=3, top indirect block number is i_blocks[14], indirection level 3
    */
   temp_block_num = pinode->i_block[N_DIRECT_BLOCKS + block_level - 1];

   for (;block_level;block_level--)
   {
      if (temp_block_num == 0)
      {
         *device_block_p = 0; /* Block does not exist */
         return -1;
      }
      index_offset =    (temp_block_num << (10+fsinfo->e2sb.s_log_block_size)) + (
                        ((file_block & (((1<<shift_single_block_level)-1))<<(shift_block_level))
                        >> shift_block_level) * sizeof(uint32_t) );

      ret = ext2_device_buf_read(device, (uint8_t *)temp_block_num, index_offset, sizeof(uint32_t));
      if(ret)
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

   /* FIXME: How to mount on an existing directory with previous data? How to overwrite? How to recover previous subtree? */
   /* Read root inode */
   ret = ext2_read_inode(dir_node, EXT2_ROOTINO);   //Estoy sobreescribiendo algo de lo que tenia antes?
   ASSERT_MSG(ret >= 0, "ext2_mount_load(): ext2_read_inode failed");
   if(ret)
   {
      return -1;
   }
   ret = ext2_mount_load_rec(dir_node);
   ASSERT_MSG(ret >= 0, "ext2_mount_load(): ext2_mount_load_rec failed");
   if(ret)
   {
      return -1;
   }

   return 0;
}


/* Precondition: mount root inode must be initialized */
static int ext2_mount_load_rec(vnode_t *dir_node)
{
   vnode_t *child_node;
   ext2_direntry_t ext2_dir_entry_buffer;
   uint32_t data_block, block_offset, file_pointer;
   uint16_t entry_pointer;
   int ret, mode;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = dir_node->fs_info->device;
   finfo = (ext2_file_info_t *)dir_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)dir_node->fs_info->down_layer_info;

   /* size must be multiple of block size. Dont consider special cases */
   /* jump from block to block in every iteration */
   for(file_pointer = 0; file_pointer < finfo->f_size; file_pointer += fsinfo->s_block_size)
   {
      /* Map file block to disk block. Result in data_block */
      ret = ext2_block_map(dir_node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      ASSERT_MSG(0 == ret, "ext2_mount_load_rec(): ext2_block_map() failed");
      if(ret)
      {
         return -1;
      }
      /* block_offset points to the first byte address of the current block */
      block_offset = data_block<<(10+fsinfo->e2sb.s_log_block_size);
      /* entry pointer iterates sequentially over the entries until it reaches the end of the block */
      for(entry_pointer = 0; entry_pointer < fsinfo->s_block_size;
            entry_pointer += ext2_dir_entry_buffer.rec_len)
      {
         /* Read entry fields, except for the name. 8 bytes in total */
         
         ret = ext2_device_buf_read(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
         ASSERT_MSG(0 == ret, "ext2_mount_load_rec(): ext2_device_buf_read() failed");
         if(ret)
         {
            return -1;
         }
         if(8 >= ext2_dir_entry_buffer.rec_len)
         {
            break;
         }
         if(0 == ext2_dir_entry_buffer.inode)
         {
            continue;
         }
         if(((ext2_dir_entry_buffer.rec_len - 8) < ext2_dir_entry_buffer.name_len) ||
            (EXT2_MAXNAMELEN < ext2_dir_entry_buffer.name_len))
         {
            continue; /* Directory corrupt */
         }
         /* Read the name of the entry */
         ret = ext2_device_buf_read(bdev, (uint8_t *)ext2_dir_entry_buffer.name, block_offset + entry_pointer + 8,
                                    ext2_dir_entry_buffer.name_len);
         ASSERT_MSG(0 == ret, "ext2_mount_load_rec(): ext2_device_buf_read() failed");
         if(ret)
         {
            return -1;
         }
         if(!ciaaPOSIX_strncmp(ext2_dir_entry_buffer.name, ".", 1) || !ciaaPOSIX_strncmp(ext2_dir_entry_buffer.name, "..", 2))
         {
            continue;
         }
         mode=0;
         child_node = vfs_create_child(dir_node, ext2_dir_entry_buffer.name, ext2_dir_entry_buffer.name_len, mode);
         ASSERT_MSG(NULL != child_node, "ext2_mount_load_rec(): Mem error");
         if(NULL == child_node)
         {
            return -1;
         }
         /* Alloc and initialize the file low level information */
         child_node->f_info.down_layer_info = (ext2_file_info_t *) ciaaLibs_poolBufLock(&ext2_finfo_pool);
         ASSERT_MSG(NULL != child_node->f_info.down_layer_info, "ext2_mount_load_rec(): Mem error, could not alloc finfo");
         if(NULL == child_node->f_info.down_layer_info || NULL == child_node->fs_info->down_layer_info)   //FIXME
         {
            return -1;
         }
         /* Read node data from disk */
         ret = ext2_read_inode(child_node, ext2_dir_entry_buffer.inode);
         ASSERT_MSG(0 <= ret, "ext2_mount_load_rec(): ext2_read_inode() failed");
         if(ret)
         {
            return -1;
         }
         if(EXT2_FT_DIR == ext2_dir_entry_buffer.file_type)
         {
            ret = ext2_mount_load_rec(child_node);
            ASSERT_MSG(0 == ret, "ext2_mount_load_rec(): ext2_mount_load_rec() failed");
            if(ret)
            {
               return -1;
            }
         }
      }
   }
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
         type = VFS_FTREG;
      break;
      case EXT2_S_IFBLK:
         type = VFS_FTBLK;
      break;
      case EXT2_S_IFDIR:
         type = VFS_FTDIR;
      break;
      default:
         type = VFS_FTUNKNOWN;
      break;
   }
   return type;
}

static int ext2_create_regular_file(vnode_t *parent_node, vnode_t *node)
{
/* 1.Alloc space for node->f_info.down_layer_info
 * 2.Reserve a bit from inode_bitmap, with current allocation policies (ex.: Same gd as parent)
 * 3.Fill f_info fileds of the new child and its parent directory
     finfo->f_num the new node inumber
     pinode->i_size
     pinode->i_links_count
     pinode->i_mode
 * 4.Alloc a data block for the new node
 * 5.Add new entry in parents dir data
 * 6.Refresh bookkeeping info in superblock and group descriptor
 */

   int ret;
   uint32_t new_inumber, aux;
   ext2_inode_t *pinode=NULL;
   ext2_file_info_t *finfo;

   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;

   finfo = (ext2_file_info_t *) ciaaLibs_poolBufLock(&ext2_finfo_pool);
   ASSERT_MSG(NULL != finfo, "ext2_create_regular_file(): Mem error");
   if(NULL == finfo)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)finfo, 0, sizeof(ext2_file_info_t));
   node->f_info.down_layer_info = (void *)finfo;
   /* Reserve a inode bit. This function modifies bookkeeping info on memory */
   ret = ext2_alloc_inode_bit(node, &new_inumber);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_alloc_inode_bit() failed");
   if(ret)
   {
      return -1;
   }
   /* Fill node fields */
   pinode = &ext2_node_buffer;
   ciaaPOSIX_memset((void *)pinode, 0, sizeof(ext2_inode_t));
   pinode->i_mode = 0x81B6;
   pinode->i_uid = 0;
   pinode->i_atime = 0;
   pinode->i_ctime = 0;
   pinode->i_flags = 0;
   /* The new node is referred by the father. Increment link count */
   pinode->i_links_count = 1;
   /* Write inode in its disk position */
   ret = ext2_set_inode(node->fs_info, new_inumber, pinode);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_set_inode() failed");
   if(0 > ret)
   {
      return -1;
   }
   /* Reserve a data block for the new node. This function modifies node on memory */
   ret = ext2_file_map_alloc(node, 0, &aux);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_file_map_alloc() failed");
   if(ret)
   {
      return -1;
   }

   /* Add new entry in parent dir data.  This function modifies node, gd & sb if new block reserved */
   ret = ext2_dir_add_entry(parent_node, new_inumber, node->f_info.file_name, node->f_info.file_namlen, EXT2_FT_REG_FILE);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_dir_add_entry() failed");
   if(ret)
   {
      return -1;
   }

   /* Already modified parent node in disk */
   /* Write to disk the modified sb and gd */
   ret = ext2_fsinfo_flush(node);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_fsinfo_flush() failed");
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
   ext2_inode_t *pinode=NULL;
   ext2_file_info_t *finfo, *parent_finfo;
   ext2_fs_info_t *fsinfo;

   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   file_blocks = finfo->f_size >> (10+fsinfo->e2sb.s_log_block_size);
   /* dealloc all data block of file */
   for(fblock = 0; fblock <= file_blocks; fblock++)
   {
      ret = ext2_block_map(node, fblock, &device_block);
      ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_block_map() failed");
      if(ret)
      {
         return -1;
      }
      ret = ext2_dealloc_block_bit(node, device_block);
      ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_dealloc_block_bit() failed");
      if(ret)
      {
         return -1;
      }
   }
   /* clear inode in inode table */
   ret = ext2_dir_delete_entry(parent_node, node->f_info.file_name, node->f_info.file_namlen);
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_dir_delete_entry() failed");
   if(ret)
   {
      return -1;
   }
   /* dealloc bit in inode bitmap */
   ret = ext2_dealloc_inode_bit(node); /* FIXME: Sets finfo->f_inumber to 0 */
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_dealloc_inode_bit() failed");
   if(ret)
   {
      return -1;
   }
   /* Modify parent inode fields */
   pinode = &ext2_node_buffer;
   ret = ext2_get_inode(parent_node->fs_info, parent_finfo->f_inumber, pinode);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_get_inode() failed");
   if(0 > ret)
   {
      return -1;
   }
   pinode->i_links_count--;
   ret = ext2_set_inode(parent_node->fs_info, parent_finfo->f_inumber, pinode);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_set_inode() failed");
   if(0 > ret)
   {
      return -1;
   }
   /* Erase on-disk node, which means, clear all bits in its memory space */
   ciaaPOSIX_memset(pinode, 0, sizeof(ext2_inode_t));
   ret = ext2_set_inode(node->fs_info, finfo->f_inumber, pinode); /* FIXME: invalid data */
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_set_inode() failed");
   if(0 > ret)
   {
      return -1;
   }
   /* Write modified sb & gd */
   ret = ext2_fsinfo_flush(node); /* FIXME: Causes file0 entry to get corrupted (bad type file) */
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_fsinfo_flush() failed");
   if(ret)
   {
      return -1;
   }
   /* Free low level data from the deleted file */
   if(NULL != node->f_info.down_layer_info)
   {
      ret = ciaaLibs_poolBufFree(&ext2_finfo_pool, node->f_info.down_layer_info);
      ASSERT_MSG(1 == ret, "vfs_node_free(): ciaaLibs_poolBufFree() failed");
      if(1 != ret)
      {
         return -1;
      }
      node->f_info.down_layer_info = NULL;
   }
   return 0;
}

static int ext2_create_directory_file(vnode_t *parent_node, vnode_t *node)
{
/* 1.Alloc space for node->f_info.down_layer_info
 * 2.Reserve a bit from inode_bitmap, with current allocation policies (ex.: Same gd as parent)
 * 3.Fill f_info fileds of the new child and its parent directory
     finfo->f_num the new node inumber
     pinode->i_size
     pinode->i_links_count
     pinode->i_mode
 * 4.Alloc a data block for the new node
 * 5.Add default entries for the new node. Refresh bookkeeping info
 * 6.Add new entry in parents dir data
 * 7.Refresh bookkeeping info in superblock and group descriptor
 */
   ext2_gd_t gd;
   int ret;
   uint32_t new_inumber, aux;
   ext2_inode_t *pinode=NULL;
   ext2_file_info_t *finfo, *parent_finfo;

   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;

   finfo = (ext2_file_info_t *) ciaaLibs_poolBufLock(&ext2_finfo_pool);
   ASSERT_MSG(NULL != finfo, "ext2_create_regular_file(): Mem error");
   if(NULL == finfo)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)finfo, 0, sizeof(ext2_file_info_t));
   node->f_info.down_layer_info = (void *)finfo;

   ret = ext2_alloc_inode_bit(node, &new_inumber);
   if(ret)
   {
      return -1;
   }
   /* mkdir creates dirs with 0755 access */
   /* Fill the new inode fields */
   pinode = &ext2_node_buffer;
   ciaaPOSIX_memset((void *)pinode, 0, sizeof(ext2_inode_t));
   pinode->i_mode = 0x41FD;
   pinode->i_uid = 0;
   pinode->i_atime = 0;
   pinode->i_ctime = 0;
   pinode->i_flags = 0;
   pinode->i_links_count = 2;
   ret = ext2_set_inode(node->fs_info, new_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   ret = ext2_file_map_alloc(node, 0, &aux);
   if(ret)
   {
      return -1;
   }
   /* Add the default entries: . and .. */
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
   /* Add new entry to parent directory data */
   ret = ext2_dir_add_entry(parent_node, new_inumber, node->f_info.file_name, node->f_info.file_namlen, EXT2_FT_DIR);
   if(ret)
   {
      return -1;
   }

   /* Modify the parent inode fields */
   ret = ext2_get_inode(node->fs_info, parent_finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   pinode->i_links_count++;
   ret = ext2_set_inode(node->fs_info, parent_finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   /* Refresh used dir count in on-memory group descriptor */
   ret = ext2_get_groupdesc(parent_node->fs_info, finfo->f_group, &gd);
   if(ret)
   {
      return -1;
   }
   gd.used_dirs_count++;
   ret = ext2_set_groupdesc(parent_node->fs_info, finfo->f_group, &gd);
   if(ret)
   {
      return -1;
   }
   /* Write sb and gd changes to disk */
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
   ext2_inode_t *pinode=NULL;
   ext2_file_info_t *finfo, *parent_finfo;
   ext2_fs_info_t *fsinfo;

   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

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
   file_blocks = finfo->f_size >> (10+fsinfo->e2sb.s_log_block_size);
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
   /* clear entry in directory */
   ret = ext2_dir_delete_entry(parent_node, node->f_info.file_name, node->f_info.file_namlen);
   if(ret)
   {
      return -1;
   }
   /* dealloc bit in inode bitmap */
   ret = ext2_dealloc_inode_bit(node);
   if(ret)
   {
      return -1;
   }

   /* Modify the parent inode fields */
   pinode = &ext2_node_buffer;
   ret = ext2_get_inode(node->fs_info, parent_finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   pinode->i_links_count--;
   ret = ext2_set_inode(node->fs_info, parent_finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   /* Erase node by writing 0 in its space */
   ciaaPOSIX_memset(pinode, 0, sizeof(ext2_inode_t));   //FIXME: Should erase 128 bits, size that appears in superblock, not struct
   ret = ext2_set_inode(node->fs_info, finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   /* Write sb and gd changes to disk */
   ret = ext2_fsinfo_flush(node);
   if(ret)
   {
      return -1;
   }
   /* Free low level info */
   if(NULL != node->f_info.down_layer_info)
   {
      ret = ciaaLibs_poolBufFree(&ext2_finfo_pool, node->f_info.down_layer_info);
      ASSERT_MSG(1 == ret, "vfs_node_free(): ciaaLibs_poolBufFree() failed");
      if(1 != ret)
      {
         return -1;
      }
      node->f_info.down_layer_info = NULL;
   }
   return 0;
}

/* Alloc policy: First free bit found, starting from beginning. Linear search, no optimization.
 * Refresh in-memory file system information, except for used dir count.
 */
static int ext2_alloc_inode_bit(vnode_t *node, uint32_t *new_inumber_p)
{
   /* Search a group with free inode. Retrieve the free inode number */
   /* TODO: Slow allocation algorithm. Should optimize */
   ext2_gd_t gd;
   int ret;
   uint32_t i,j,n, block_offset;
   uint16_t group_index, segment_index, segment_offset;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   /* Search a group with a free inode */
   for(group_index = 0; group_index < fsinfo->s_groups_count; group_index++)
   {
      ret = ext2_get_groupdesc(node->fs_info, group_index, &gd);
      if(ret)
      {
         return -1;
      }
      if(gd.free_inodes_count)
      {
         break;
      }
   }
   ASSERT_MSG(group_index<fsinfo->s_groups_count, "ext2_alloc_inode_bit(): No group with free inodes");
   if(group_index >= fsinfo->s_groups_count)
   {
      /* All gd indicate that the group is full. No inode available */
      return -1;
   }
   block_offset = (gd.inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size);
   for(segment_index=0, segment_offset = 0; segment_index < fsinfo->s_buff_per_block;
         segment_index++, segment_offset += fsinfo->s_buff_size)
   {
      /* Read node bitmap to bitmap_buffer */
      ret = ext2_device_buf_read(bdev, (uint8_t *)ext2_block_buffer,
                                 block_offset + segment_offset,
                                 fsinfo->s_buff_size);
      if(ret)
      {
         return -1;
      }
      /* Search for a free byte in bitmap, which means, bit whose value is 0 */
      /* First search a byte whose value is not FF, which means, it contains a 0 bit */
      for(i = 0; i<fsinfo->s_buff_size && 0XFFU == ext2_block_buffer[i]; i++);
      ASSERT_MSG(i<fsinfo->s_buff_size, "ext2_alloc_inode_bit(): No free byte. Descriptor error");
      if(i >= fsinfo->s_buff_size)
      {
         /* No free bit found in this block segment. Continue with next segment */
         continue;
      }
      /* Found byte with free bit in bitmap. The block segment that contains this byte is stored
       * in ext2_block_buffer. The byte position in the segment is i. The segment offset is segment_offset
       */
      break;
   }
   ASSERT_MSG(segment_index < fsinfo->s_buff_per_block, "ext2_alloc_inode_bit(): No free bit in bitmap");
   if(segment_index >= fsinfo->s_buff_per_block)
   {
      /* No free bit found in bitmap
       * Metadata inconsistency error. Group descriptor shows that there is a free bit available
       */
      return -1;
   }
   ret = ext2_device_buf_write(bdev, (uint8_t *)ext2_block_buffer,
                              block_offset + segment_offset,
                              fsinfo->s_buff_size);
   /* Found byte with free bit in block located in block_offset, offset (segment_offset + i) */
   /* Find free bit inside the found byte */
   for(j = 0; j < 8; j++)
   {
      if(!(ext2_block_buffer[i] & (1<<j)))
      {
         ext2_block_buffer[i] |= (1<<j);
         n = 8*i + j;   /* Bit offset inside segment */
         break;
      }
   }
   ASSERT_MSG(j < 8, "ext2_alloc_inode_bit(): No free bit in selected byte. Inconsistency error");
   if(j >= 8)
   {
      return -1;   /* Should never happen */
   }
   /* segment_offset*8 + n is the free block number relative to the current group.
    * Must calculate absolute inode number
    */
   *new_inumber_p = group_index * fsinfo->e2sb.s_inodes_per_group + (segment_offset<<3) + n + 1;
   /* Write modified bitmap to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)ext2_block_buffer,
                              block_offset + segment_offset,
                              fsinfo->s_buff_size);
   ASSERT_MSG(ret >= 0, "ext2_alloc_inode_bit(): ext2_device_buf_write() failed");
   if(ret)
   {
      return -1;
   }
   /* Refresh in-memory bookkeeping info. The caller should write it to disk */
   fsinfo->e2sb.s_free_inodes_count--;
   gd.free_inodes_count--;
   ret = ext2_set_groupdesc(node->fs_info, group_index, &gd);
   ASSERT_MSG(ret >= 0, "ext2_alloc_inode_bit(): ext2_set_groupdesc() failed");
   if(ret)
   {
      return -1;
   }
   //fsinfo->e2fs_gd[group_index].used_dirs_count++; /* Caller knows if this is a dir or not. Leave unmodified */
   finfo->f_group = group_index;
   finfo->f_inumber = *new_inumber_p;

   return 0;
}

static int ext2_dealloc_inode_bit(vnode_t *node)
{
   ext2_gd_t gd;
   int ret;
   uint32_t bitmap_offset, n;
   uint8_t bitmap_byte;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   ret = ext2_get_groupdesc(node->fs_info, finfo->f_group, &gd);
   if(ret)
   {
      return -1;
   }
   /* n is the inode number relative to current group, so its the bit offset in inode bitmap */
   /* n>>3 (n/8) is then the byte offset in the bitmap where the bit of current node resides */
   n = finfo->f_inumber - (finfo->f_group * fsinfo->e2sb.s_inodes_per_group) - 1;
   /* Read bitmap to bitmap_buffer */
   bitmap_offset = (gd.inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size);
   ret = ext2_device_buf_read(bdev, (uint8_t *)&bitmap_byte, bitmap_offset + (n>>3), sizeof(uint8_t));
   if(ret)
   {
      return -1;
   }
   /* Clear bit from target byte */
   bitmap_byte &= (uint8_t)~(1<<(n&0x07));
   /* Write modified bitmap byte to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)&bitmap_byte, bitmap_offset + (n>>3), sizeof(uint8_t));
   if(ret)
   {
      return -1;
   }
   /* Refresh on-memory bookkeeping info. Caller should write them to disk */
   fsinfo->e2sb.s_free_inodes_count++;
   gd.free_inodes_count++;
   //fsinfo->e2fs_gd[finfo->f_group].used_dirs_count++; /* Caller knows whether this is a regfile or directory */
   ret = ext2_set_groupdesc(node->fs_info, finfo->f_group, &gd);
   if(ret)
   {
      return -1;
   }

   return 0;
}


/* TODO: Validate arguments */
/*FIXME: Borro el bloque determinado por el numero contenido en new_block. Pero new_block puede no estar inicializado */
/*FIXME: Designed only for 1024 bytes block. Fix for the general case*/
static int ext2_file_map_alloc(vnode_t *node, uint32_t file_block, uint32_t *device_block_p)
{
   int ret;
   uint32_t block_pos, new_block, temp_block_num, index_offset, block_offset;
   uint8_t shift_block_level, shift_single_block_level, block_level, flag_new_block;
   ext2_inode_t *pinode;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   if(NULL == device_block_p)
      return -1;

   pinode = &ext2_node_buffer;
   ret = ext2_get_inode(node->fs_info, finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }
   print_inode(pinode);
   flag_new_block = 0;   /* To tell if a new file block is being allocated */
   *device_block_p = 0;
   block_pos = file_block;
   if (block_pos < N_DIRECT_BLOCKS)
   {
      if(0 == pinode->i_block[block_pos])
      {
         ret = ext2_alloc_block_bit(node, &new_block);
         ASSERT_MSG(ret>=0, "ext2_file_map_alloc(): ext2_alloc_block_bit() failed");
         if(ret)
         {
            return -1;
         }
         pinode->i_block[block_pos] = new_block;
         pinode->i_blocks += fsinfo->sectors_in_block;
         flag_new_block = 1;
      }
      *device_block_p = pinode->i_block[block_pos];
   }
   else
   {
      /*
       * 0:EXT2_DIR_BLOCK-1
       * EXT2_DIR_BLOCK:EXT2_DIR_BLOCK+(1<<shift_single_block_level)-1
       * EXT2_DIR_BLOCK+(1<<shift_single_block_level):EXT2_DIR_BLOCK+(1<<shift_single_block_level)+(1<<2*(shift_single_block_level))-1
       * EXT2_DIR_BLOCK+(1<<shift_single_block_level)+(1<<2*(shift_single_block_level))-1:
       * EXT2_DIR_BLOCK+(1<<shift_single_block_level)+(1<<2*(shift_single_block_level))+(1<<3*(shift_single_block_level))-1
       */
      /* Wanto to calculate shift_block_level */
      shift_single_block_level = fsinfo->e2sb.s_log_block_size - 2;   /* log(block_size/4). How much 4 byte entries in 1 block */
      block_pos -= N_DIRECT_BLOCKS;
      for (   shift_block_level=0, block_level=1;
         block_pos >= (int32_t)1<<(shift_single_block_level+shift_block_level);
         block_pos -= (int32_t)1<<(shift_single_block_level+shift_block_level),
         shift_block_level += shift_single_block_level, block_level++)
      {
         if (block_level > N_INDIRECT_BLOCKS)
         {
            *device_block_p = 0;
            return -1;
         }
      }

      temp_block_num = pinode->i_block[N_DIRECT_BLOCKS - 1 + block_level];
      if (0 == temp_block_num)
      {
         ret = ext2_alloc_block_bit(node, &temp_block_num);
         if(ret)
         {
            *device_block_p = 0;
            return -1;
         }
         pinode->i_block[N_DIRECT_BLOCKS - 1 + block_level] = temp_block_num;
         pinode->i_blocks += fsinfo->sectors_in_block;
      }
      /* block_level will tell the indirection level. shift_block_level is the shift of the mask */
      for (;block_level;block_level--)
      {
         /* Calculate offset of the indirect block number contained in the current indirect block */
         index_offset =   (temp_block_num << (10+fsinfo->e2sb.s_log_block_size)) +
                          (( block_pos & (/*Mask*/((1<<shift_single_block_level)-1) << shift_block_level/*/Mask*/) )
                          >> shift_block_level) * sizeof(uint32_t);
         /* Read next indirection levels block */
         ret = ext2_device_buf_read(bdev, (uint8_t *)temp_block_num, index_offset, sizeof(uint32_t));
         if(ret)
         {
            *device_block_p = 0;
            return -1;
         }
         /* Block number was not allocated. Allocate a block and write its number in the found position */
         if (0 == temp_block_num)
         {
            ret = ext2_alloc_block_bit(node, &temp_block_num);
            if(ret)
            {
               return -1;
            }
            ret = ext2_device_buf_write(bdev, (uint8_t *)temp_block_num, index_offset, sizeof(uint32_t));
            if(ret)
            {
               *device_block_p = 0;
               return -1;
            }
            flag_new_block = 1;
         }
      }
   }
   if(1 == flag_new_block)
   {
      /* Erase the contents of the new reserved block */
      block_offset = new_block<<(10+fsinfo->e2sb.s_log_block_size);
      ret = ext2_device_buf_memset(bdev, 0, block_offset, fsinfo->s_block_size);
      if(ret)
      {
         return -1;
      }
   }
   ret = ext2_set_inode(node->fs_info, finfo->f_inumber, pinode);
   if(0 > ret)
   {
      return -1;
   }

   return 0;
}

/* Tries to alloc a block in the same group as the file */
/* Modifies bookkeeping info */
/* TODO: Only searches a block in the current file group. Should check in all groups */
static int ext2_alloc_block_bit(vnode_t * node, uint32_t *block)
{
   /* Search a group with free inode. Retrieve the free inode number */
   /* TODO: Slow allocation primitive. Should optimize */
   ext2_gd_t gd;
   int ret;
   uint32_t i,j,b, block_offset;
   uint16_t group_index, segment_index, segment_offset;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;
   *block = 0;

   /* Search a group with a free block, starting from the group of this file */
   for(group_index = finfo->f_group, i=0; i < fsinfo->s_groups_count;
         group_index = (group_index + 1) % fsinfo->s_groups_count)
   {
      ret = ext2_get_groupdesc(node->fs_info, group_index, &gd);
      if(ret)
      {
         return -1;
      }
      if(gd.free_inodes_count)
         break;
   }
   ASSERT_MSG(group_index<fsinfo->s_groups_count, "ext2_alloc_block_bit(): No group with free blocks");
   if(i >= fsinfo->s_groups_count)
   {
      /* All gd indicate that the group is full. No block available */
      return -1;
   }
   block_offset = (gd.block_bitmap)<<(10+fsinfo->e2sb.s_log_block_size);
   for(segment_index=0, segment_offset = 0; segment_index < fsinfo->s_buff_per_block;
         segment_index++, segment_offset += fsinfo->s_buff_size)
   {
      /* Read node bitmap to bitmap_buffer */
      ret = ext2_device_buf_read(bdev, (uint8_t *)ext2_block_buffer,
                                 block_offset + segment_offset,
                                 fsinfo->s_buff_size);
      if(ret)
      {
         return -1;
      }
      /* Search for a free byte in bitmap, which means, bit whose value is 0 */
      /* First search a byte whose value is not FF, which means, it contains a 0 bit */
      for(i = 0; i<fsinfo->s_buff_size && 0XFFU == ext2_block_buffer[i]; i++);
      ASSERT_MSG(i<fsinfo->s_buff_size, "ext2_alloc_inode_bit(): No free byte. Descriptor error");
      if(i >= fsinfo->s_buff_size)
      {
         /* No free bit found in this block segment. Continue with next segment */
         continue;
      }
      /* Found byte with free bit in bitmap. The block segment that contains this byte is stored
       * in ext2_block_buffer. The byte position in the segment is i. The segment offset is segment_offset
       */
      break;
   }
   if(segment_index >= fsinfo->s_buff_per_block)
   {
      /* No free bit found in bitmap
       * Metadata inconsistency error. Group descriptor shows that there is a free bit available
       */
      return -1;
   }

   /* Find free bit inside the found byte */
   for(j = 0; j < 8; j++)
   {
      if(!(ext2_block_buffer[i] & (1<<j)))
      {
         ext2_block_buffer[i] |= (1<<j);
         b = 8*i + j;   /* Bit offset inside segment */
         break;
      }
   }
   ASSERT_MSG(j < 8, "ext2_alloc_inode_bit(): No free bit in selected byte. Inconsistency error");
   if(j >= 8)
   {
      return -1;   /* Should never happen */
   }
   /* Write modified bitmap to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)ext2_block_buffer,
                              block_offset + segment_offset,
                              fsinfo->s_buff_size);
   if(ret)
   {
      return -1;
   }
   /* segment_offset*8 + n is the free block number relative to the current group.
    * Must calculate absolute inode number
    */
   *block = fsinfo->e2sb.s_first_data_block + fsinfo->e2sb.s_blocks_per_group*group_index + (segment_offset<<3) + b;
   /* Refresh in-memory bookkeeping info. The caller should write it to disk */
   gd.free_blocks_count--;
   ret = ext2_set_groupdesc(node->fs_info, group_index, &gd);
   if(ret)
   {
      return -1;
   }
   fsinfo->e2sb.s_free_blocks_count--;

   return 0;
}

/* Dealloc given block */
/* Modifies bookkeeping info */
static int ext2_dealloc_block_bit(vnode_t *node, uint32_t block)
{
   ext2_gd_t gd;
   int ret;
   uint32_t bitmap_offset, b, block_group;
   uint8_t bitmap_byte;

   ciaaDevices_deviceType const *bdev;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   /* b is the inode number relative to current group, so its the bit offset in inode bitmap */
   /* b>>3 (b/8) is then the byte offset in the bitmap where the bit of current node resides */
   //b = block - fsinfo->e2sb.s_first_data_block - (fsinfo->e2sb.s_blocks_per_group*finfo->f_group);
   block_group = (block - fsinfo->e2sb.s_first_data_block) / fsinfo->e2sb.s_blocks_per_group;
   b = (block - fsinfo->e2sb.s_first_data_block) % fsinfo->e2sb.s_blocks_per_group;
   ret = ext2_get_groupdesc(node->fs_info, block_group, &gd);
   if(ret)
   {
      return -1;
   }
   /* Read bitmap to bitmap_buffer */
   bitmap_offset = (gd.block_bitmap)<<(10+fsinfo->e2sb.s_log_block_size);
   ret = ext2_device_buf_read(bdev, (uint8_t *)&bitmap_byte, bitmap_offset + (b>>3), sizeof(uint8_t));
   if(ret)
   {
      return -1;
   }
   /* Clear bit from target byte */
   bitmap_byte &= (uint8_t)~(1<<(b&0x07));
   /* Write modified bitmap byte to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)&bitmap_byte, bitmap_offset + (b>>3), sizeof(uint8_t));
   if(ret)
   {
      return -1;
   }
   /* Refresh on-memory bookkeeping info. Caller should write them to disk */
   gd.free_blocks_count++;
   ret = ext2_set_groupdesc(node->fs_info, block_group, &gd);
   if(ret)
   {
      return -1;
   }
   fsinfo->e2sb.s_free_blocks_count++;

   return 0;
}

/*
Alternativa1: Leer de a 1024 bytes. Si queda un entry cortado por la mitad, tratar de armar los datos de alguna forma
Alternativa2: Leer primero 1024 bytes. Si se que este entry se va a tener que partir, recargo el buffer leyendo desde donde empieza el entry actual

Contenido: 8bytes
Nombre: alineado a 4 bytes
Ejemplo: Tengo solo los entries . y ..
.:4 2 1 1 1 3; ..:4 2 1 1 2 2
0 a 11 (12); 12 a 21 (12); 22 a 2047 (2024)

La estructura ext2_direntry soporta nombres de hasta EXT2_MAXNAMELEN bytes. Podria guardar el entry anterior y tenerlo de backup por si el entry actual no puede ser completamente leido.
Podria implementar una funcion del estilo ext2_get_next_entry(ext2_direntry_t *, )

Podria utilizar un buffer circular.
Opcion 1: Escribir 1024 bytes en el buffer. Ir leyendo los entries. Aca hay varios casos, asumo bloque de 2KB:
1) Tengo solo . y .. . reclen de .. apunta al final: Leo primero el entry de ., luego el entry de .. . Reclen me dice que ahora apunte a 2048. COMPLETAR
2) Tengo ., .., luego un entry que comienza en 1016 y tiene name largo.

Dificil usar un buffer circular ya que tendria que utilizar un buffer auxiliar.

Leo 1024 bytes. Leo los entries uno a uno, guardandolos en una estructura y procesandolos. Salto segun reclen. Llego al entry cortado por la mitad

dpointer = start_pointer;
inode, rec_len, name_len, file_type, name[EXT2_MAXNAMELEN]
dpointer = (ext2_direntry_t *)((char*)dpointer + dpointer->rec_len);

*/

static int ext2_dir_add_entry(vnode_t *node, uint32_t ino, char *name, uint16_t namlen, uint8_t type)
{
   uint32_t data_block, block_offset, file_pointer;
   uint16_t new_entry_size, entry_pointer;
   int ret;
   uint8_t flag_entry_found = 0;
   //ext2_direntry_t ext2_dir_entry_buffer;
   ext2_direntry_t *ext2_dir_entry_buffer_p;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;
   ext2_dir_entry_buffer_p = &ext2_dir_entry_buffer;

   new_entry_size = DENTRY_MIN_SIZE + namlen;
   /* Align required_space to 4 bytes. Directory entry rule */
   new_entry_size += (new_entry_size & 0x03) ? (4 - (new_entry_size & 0x03)) : 0;
   /* size must be multiple of block size. Dont consider special cases */
   /* jump from block to block in every iteration */
   for(file_pointer = 0; file_pointer < finfo->f_size; file_pointer += fsinfo->s_block_size)
   {
      /* Map file block to disk block. Result in data_block */
      ret = ext2_block_map(node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      ASSERT_MSG(0 == ret, "ext2_dir_add_entry(): ext2_file_map_alloc() failed");
      if(ret)
      {
         return -1;
      }
      /* block_offset points to the first byte address of the current block */
      block_offset = data_block<<(10+fsinfo->e2sb.s_log_block_size);
      /* entry pointer iterates sequentially over the entries until it reaches the end of the block */
      for(entry_pointer = 0; entry_pointer < fsinfo->s_block_size; entry_pointer += ext2_dir_entry_buffer.rec_len)
      {
         /* Read entry fields, except for the name. 8 bytes in total */
         ret = ext2_device_buf_read(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
         ASSERT_MSG(0 == ret, "ext2_dir_add_entry(): ext2_device_buf_read() failed");
         if(ret)
         {
            return -1;
         }
         /* Consider case for empty block */
         if(0 != ext2_dir_entry_buffer.inode)
         {
            /* Check if this is a corrupt entry */
            ASSERT_MSG(8 < ext2_dir_entry_buffer.rec_len, "ext2_dir_add_entry(): Corrupt entry");
            if(8 >= ext2_dir_entry_buffer.rec_len)
            {
               return -1;
            }
            ASSERT_MSG(8 < ext2_dir_entry_buffer.rec_len, "ext2_dir_add_entry(): Corrupt entry");
            if(((ext2_dir_entry_buffer.rec_len - 8) < ext2_dir_entry_buffer.name_len) ||
               (EXT2_MAXNAMELEN < ext2_dir_entry_buffer.name_len))
            {
               return -1; /* Directory corrupt */
            }
         }
         if (0 == ext2_dir_entry_buffer.inode)
         {
            /* Free entry found. Check if it has space for the new entry */
            if (new_entry_size <= ext2_dir_entry_buffer.rec_len)
            {
               flag_entry_found = 1;
               break;
            }
         }
         /* See if there is enough space in padding to add new entry */
         if ( new_entry_size <= (ext2_dir_entry_buffer.rec_len - DENTRY_TOTAL_SIZE(ext2_dir_entry_buffer_p)) )
         {
            /* Enough space to alloc new entry
             * Must shrink the previous entry before adding the new
             */
            /* Calculate the padding size, this will be the rec_len of the new entry */
            new_entry_size = ext2_dir_entry_buffer.rec_len - DENTRY_TOTAL_SIZE(ext2_dir_entry_buffer_p);
            /* Update current entry so that rec_len points to the start of the new entry */
            ext2_dir_entry_buffer.rec_len = DENTRY_TOTAL_SIZE(ext2_dir_entry_buffer_p);
            ret = ext2_device_buf_write(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
            /* Point to the next entry */
            entry_pointer += ext2_dir_entry_buffer.rec_len;
            ext2_dir_entry_buffer.rec_len = new_entry_size;
            /* Flag to tell that an entry has been found */
            flag_entry_found = 1;
            break;
         }

      }
      if(flag_entry_found)
      {
         /* entry_pointer keeps the address of the new entry
          * block_offset keeps the address of the block to be modified
          */
         break;
      }
   }
   /* No free or adequate entry found. Must extend the directory */
   if (!flag_entry_found)
   {
      /* directory is full and no room left in last block */
      ret = ext2_file_map_alloc(node, (finfo->f_size)>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      ASSERT_MSG(0 == ret, "ext2_dir_add_entry(): ext2_file_map_alloc() failed");
      if(ret)
      {
         return -1;
      }
      /* New directory size adds 1 block */
      ret = ext2_get_inode(node->fs_info, finfo->f_inumber, &ext2_node_buffer);
      ASSERT_MSG(0 == ret, "ext2_dir_add_entry(): ext2_get_inode() failed");
      if(ret)
      {
         return -1;
      }
      ext2_node_buffer.i_size += fsinfo->s_block_size;
      finfo->f_size += ext2_node_buffer.i_size;
      ret = ext2_set_inode(node->fs_info, finfo->f_inumber, &ext2_node_buffer);
      ASSERT_MSG(0 == ret, "ext2_dir_add_entry(): ext2_set_inode() failed");
      if(ret)
      {
         return -1;
      }
      /* block_offset points to the first byte address of the current block  */
      block_offset = data_block<<(10+fsinfo->e2sb.s_log_block_size);
      entry_pointer = 0;
      ext2_dir_entry_buffer.rec_len = fsinfo->s_block_size;
   }
   /* dpointer now points to a new valid entry. block_buffer contains the modified block with the new entry */
   ext2_dir_entry_buffer.inode = ino;
   ext2_dir_entry_buffer.file_type = type;
   ext2_dir_entry_buffer.name_len = namlen;
   /* Write the new entry to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
   if(ret)
   {
      return -1;
   }
   /* write the entry name to disk*/
   ret = ext2_device_buf_write(bdev, (uint8_t *)name, block_offset + entry_pointer + 8, ext2_dir_entry_buffer.name_len);
   if(ret)
   {
      return -1;
   }

  return 0;
}

static int ext2_dir_delete_entry(vnode_t *node, char *name, uint16_t namlen)
{
   uint32_t data_block, block_offset, file_pointer, aux;
   uint16_t entry_pointer, prev_entry_pointer;
   int ret;
   uint8_t flag_entry_found = 0;
   //ext2_direntry_t ext2_dir_entry_buffer;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   /* size must be multiple of block size. Dont consider special cases */
   /* jump from block to block in every iteration */
   for(file_pointer = 0; file_pointer < finfo->f_size; file_pointer += fsinfo->s_block_size)
   {
      /* Map file block to disk block. Result in data_block */
      ret = ext2_block_map(node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      if(ret)
      {
         return -1;
      }
      /* block_offset points to the first byte address of the current block */
      block_offset = data_block<<(10+fsinfo->e2sb.s_log_block_size);
      /* entry pointer iterates sequentially over the entries until it reaches the end of the block */
      for(entry_pointer = 0, prev_entry_pointer = 0; entry_pointer < fsinfo->s_block_size;
            entry_pointer += ext2_dir_entry_buffer.rec_len)
      {
         /* Read entry fields, except for the name. 8 bytes in total */
         ret = ext2_device_buf_read(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
         if(ret)
         {
            return -1;
         }
         if (0 != ext2_dir_entry_buffer.inode)
         {
            if(ext2_dir_entry_buffer.name_len == namlen)
            {
               /* Read the name of the entry */
               ret = ext2_device_buf_read(bdev, (uint8_t *)((uint8_t *)&ext2_dir_entry_buffer + 8), block_offset + entry_pointer + 8,
                                          ext2_dir_entry_buffer.name_len);
               if(ret)
               {
                  return -1;
               }
               if(!ciaaPOSIX_strncmp(ext2_dir_entry_buffer.name, name, ext2_dir_entry_buffer.name_len))
               {
                  flag_entry_found = 1;
                  break;
               }
            }
         }
         prev_entry_pointer = entry_pointer;
      }
      if(flag_entry_found)
      {
         /* entry_pointer keeps the address of the entry to be deleted
          * block_offset keeps the address of the block to be modified
          */
         break;
      }
   }
   if (flag_entry_found)
   {
      /* Requested entry found. Delete it */
      ext2_dir_entry_buffer.inode = 0;
      ret = ext2_device_buf_write(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + entry_pointer, 8);
      if(ret)
      {
         return -1;
      }
      /* Now we have cleared dentry, if it's not the first one,
       * merge it with previous one.  Since we assume, that
       * existing dentry must be correct, there is no way to
       * spann a data block.
       */
      if (entry_pointer)
      {
         /* entry_pointer does not point to the first entry
          * Previous entry now spans deleted entry
          * Deleted entry is now padding of previous entry
          */
         aux = ext2_dir_entry_buffer.rec_len;
         ret = ext2_device_buf_read(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + prev_entry_pointer, 8);
         if(ret)
         {
            return -1;
         }
         ext2_dir_entry_buffer.rec_len += aux;
         ret = ext2_device_buf_write(bdev, (uint8_t *)&ext2_dir_entry_buffer, block_offset + prev_entry_pointer, 8);
         if(ret)
         {
            return -1;
         }
      }
   }
   else
   {
      return -1;
   }
   return 0;
}

/* Writes the on-memory sb and gd to disk */
static int ext2_fsinfo_flush(vnode_t *node)
{
   int ret;
   uint32_t group_offset;
   uint16_t group_index;

   ciaaDevices_deviceType const *bdev;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info->device;
   fsinfo = (ext2_fs_info_t *)node->fs_info->down_layer_info;

   for(group_index = 0; group_index<(fsinfo->s_groups_count); group_index++)
   {
      group_offset = fsinfo->e2sb.s_first_data_block +
                     fsinfo->e2sb.s_blocks_per_group * fsinfo->s_block_size * group_index;
      /* If the block size is 1024, block 0 is boot block, block 1 is superblock. Blocks 1 to 8192 are group 1 blocks
       * Boot block dont count as group 1 block
       */
      if(0 == group_index)
      {
         group_offset += EXT2_SBOFF;
      }
      ret = ext2_device_buf_write(bdev, (uint8_t *)&(fsinfo->e2sb), group_offset, sizeof(ext2_superblock_t));
      if(ret)
      {
         return -1;
      }
   }
   return 0;
}

static ssize_t ext2_device_buf_read(ciaaDevices_deviceType const * const device, uint8_t * const buf, off_t const offset,
                                 size_t const nbyte)
{
   ssize_t ret;

   ret = ciaaBlockDevices_lseek(device, offset, SEEK_SET);
   if(ret!=offset)
   {
      return -1;
   }
   ret = ciaaBlockDevices_read(device, buf, nbyte);
   if(ret!=nbyte)
   {
      return -1;
   }

   return 0;
}

static ssize_t ext2_device_buf_write(ciaaDevices_deviceType const * const device, uint8_t * const buf, off_t const offset,
                                 size_t const nbyte)
{
   ssize_t ret;

   ret = ciaaBlockDevices_lseek(device, 0, SEEK_SET);
   if(ret!=0)
   {
      return -1;
   }
   ret = ciaaBlockDevices_lseek(device, offset, SEEK_SET);
   if(ret!=offset)
   {
      return -1;
   }
   ret = ciaaBlockDevices_lseek(device, offset, SEEK_SET);
   if(ret!=offset)
   {
      return -1;
   }
   ret = ciaaBlockDevices_write(device, buf, nbyte);
   if(ret!=nbyte)
   {
      return -1;
   }

   return 0;
}

/* Writes to ext2_block_buffer */
static int ext2_device_buf_memset(ciaaDevices_deviceType const * const device, uint8_t c, off_t const offset,
                                 size_t nbyte)
{
   uint32_t write_offset, write_size;
   ssize_t ret;

   ciaaPOSIX_memset(ext2_block_buffer, c, EXT2_BLOCK_BUFFER_SIZE);
   write_offset = offset;
   while(nbyte)
   {
      write_size = nbyte <= EXT2_BLOCK_BUFFER_SIZE ? nbyte : EXT2_BLOCK_BUFFER_SIZE;
      ret = ext2_device_buf_write(device, (uint8_t *)ext2_block_buffer, write_offset, write_size);
      if(ret)
      {
         return -1;
      }
      nbyte -= write_size;
      write_offset += write_size;
   }

   return 0;
}

/* For a max partition size of 4GB, there is a maximum of 512 groups */
/* Is it unnecesary to pass the node as argument? */
static int ext2_get_groupdesc(filesystem_info_t *fs_info, uint16_t ngroup, ext2_gd_t *gdp)
{
   ciaaDevices_deviceType const *bdev;
   ext2_fs_info_t *fsinfo;
   uint32_t offset;
   int ret;

   bdev = fs_info->device;
   fsinfo = (ext2_fs_info_t *)fs_info->down_layer_info;

   offset = (fsinfo->e2sb.s_first_data_block + 1) * fsinfo->s_block_size + ngroup * sizeof(ext2_gd_t);
   ret = ext2_device_buf_read(bdev, (uint8_t *)gdp, offset, sizeof(ext2_gd_t));
   if(ret)
   {
      return -1;
   }
   return 0;
}

static int ext2_set_groupdesc(filesystem_info_t *fs_info, uint16_t ngroup, ext2_gd_t *gdp)
{
   uint32_t group_offset;
   ciaaDevices_deviceType const *bdev;
   ext2_fs_info_t *fsinfo;
   int ret;
   uint16_t group_index;

   bdev = fs_info->device;
   fsinfo = (ext2_fs_info_t *)fs_info->down_layer_info;

   for(group_index = 0; group_index<(fsinfo->s_groups_count); group_index++)
   {
      group_offset = fsinfo->e2sb.s_first_data_block +
                     fsinfo->e2sb.s_blocks_per_group * fsinfo->s_block_size * group_index;
      ret = ext2_device_buf_write(bdev, (uint8_t *)gdp,
                                    group_offset + fsinfo->s_block_size + ngroup * sizeof(ext2_gd_t), sizeof(ext2_gd_t));
      if(ret)
      {
         return -1;
      }
   }
   return 0;
}

static int ext2_set_inode(filesystem_info_t *fs_info, uint32_t inumber, ext2_inode_t *node)
{
   ext2_gd_t gd;
   ciaaDevices_deviceType const *bdev;
   ext2_fs_info_t *fsinfo;
   uint32_t inode_offset;
   uint16_t inode_group;
   int ret;

   bdev = fs_info->device;
   fsinfo = (ext2_fs_info_t *)fs_info->down_layer_info;

   /* Verify to avoid division by 0 */
   if(fsinfo->e2sb.s_inodes_per_group == 0)
   {
      return -1;
   }
   /* Calculate the device inode position with help of on-memory file info*/
   inode_group = (inumber-1)/fsinfo->e2sb.s_inodes_per_group;
   /* (inumber-1) because we start counting from 1 */
   /* Calculate in which group is the target inode to be found */
   ret = ext2_get_groupdesc(fs_info, inode_group, &gd);
   if(ret)
   {
      return -1;
   }
   inode_offset = (gd.inode_table)*(fsinfo->s_block_size) +
                  ((inumber-1)%fsinfo->e2sb.s_inodes_per_group)*(fsinfo->e2sb.s_inode_size);
   ret = ext2_device_buf_write(bdev, (uint8_t *)node, inode_offset, sizeof(ext2_inode_t));
   if(ret)
   {
      return -1;
   }
   return 0;
}

static int ext2_get_inode(filesystem_info_t *fs_info, uint32_t inumber, ext2_inode_t *node)
{
   int ret;
   uint32_t inode_group, inode_offset;
   ext2_gd_t gd;

   ciaaDevices_deviceType const *bdev;
   ext2_fs_info_t *fsinfo;

   bdev = fs_info->device;
   fsinfo = (ext2_fs_info_t *)fs_info->down_layer_info;

   if(NULL == fsinfo || NULL == bdev)
   {
      return -1;
   }

   /* Verify to avoid division by 0 */
   if(0 == fsinfo->e2sb.s_inodes_per_group)
   {
      return -1;
   }
   /* Calculate the device inode position with help of on-memory file info*/
   inode_group = (inumber-1)/fsinfo->e2sb.s_inodes_per_group;
   /* (inumber-1) because we start counting from 1 */
   /* Calculate in which group is the target inode to be found */
   ret = ext2_get_groupdesc(fs_info, inode_group, &gd);
   if(0 > ret)
   {
      return -1;
   }
   inode_offset = (gd.inode_table)*(fsinfo->s_block_size) +
                  ((inumber-1)%fsinfo->e2sb.s_inodes_per_group)*(fsinfo->e2sb.s_inode_size);
   ret = ext2_device_buf_read(bdev, (uint8_t *)node, inode_offset, sizeof(ext2_inode_t));
   if(0 > ret)
   {
      return -1;
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
   blocks_gd = howmany(sizeof(ext2_gd_t) * ncg, bsize);
*/
//Max 2.5kb de ram. Es poco.
/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

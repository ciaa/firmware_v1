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
-Add new fields to finfo, fsinfo and ninfo, as they help to avoid calculating every time a function is called
-Implement error codes. By now when an error is found, -1 is returned. No matter which error was produced
-Fix "addtogroup" description from doxygen
-Add more comments

FIXME:
-Try to call return only once for every function. Use an auxiliary variable
-Review code entirely to see if the variable types are declared according to requirements
-For example, dont use uint32_t when only uint16_t is needed.
-Put variables in order for good alignment. First uint32_t, then uint16_t, etc.
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

   ciaaDevices_deviceType const *bdev;
   ext2_inode_t * ninfo;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = dest_node->fs_info.device;
   ninfo = (ext2_inode_t *)dest_node->n_info.down_layer_info;
   finfo = (ext2_file_info_t *)dest_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)dest_node->fs_info.down_layer_info;

   if(NULL == fsinfo || NULL == finfo || NULL == ninfo || NULL == bdev)
   {
      return -1;
   }

   /* Verify to avoid division by 0 */
   if(fsinfo->e2sb.s_inodes_per_group==0)
   {
      return -1;
   }
   /* Calculate the device inode position with help of on-memory file info*/
   inode_group = (inumber-1)/fsinfo->e2sb.s_inodes_per_group;
   /* (inumber-1) because we start counting from 1 */
   /* Calculate in which group is the target inode to be found */

   inode_offset = EXT2_SBOFF + ((((fsinfo->e2fs_gd)[inode_group]).inode_table)-1)*(fsinfo->s_block_size) +
         ((inumber-1)%fsinfo->e2sb.s_inodes_per_group)*(fsinfo->e2sb.s_inode_size);

   ret = ext2_device_buf_read(bdev, (uint8_t *)ninfo, inode_offset, sizeof(ext2_inode_t));
   if(ret)
   {
      return -1;
   }

   finfo->f_inumber = inumber;
   finfo->f_group = inode_group;
   ciaaPOSIX_memcpy(&(finfo->f_di), ninfo, sizeof(ext2_inode_t));
   print_inode(dest_node->n_info.down_layer_info);
   return 0;
}

static int ext2_write_inode(vnode_t *node)
{
   int ret;
   uint32_t offset;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;
   /* Offset of the inode_table */
   offset = ((fsinfo->e2fs_gd[finfo->f_group].inode_table) << (10+fsinfo->e2sb.s_log_block_size)) +
            ((finfo->f_inumber - 1) % fsinfo->e2sb.s_inodes_per_group) *   /* Offset inside the inode_table */
            fsinfo->e2sb.s_inode_size;

   ret = ext2_device_buf_write(bdev, (uint8_t *)&(finfo->f_di), offset, sizeof(ext2_inode_t));
   if(ret)
   {
      return -1;
   }

   return 0;
}

/* Objetivo: Reemplazar todos los malloc con un buffer unico de 1KB.
 * Problema: Si por ejemplo el bloque es de 2KB, los entries no estan alineados a 1KB, sino a 2KB
 * Es decir, pueden aparecer entries que ocupen el final de un bloque de 1KB y el principio del otro
 * Con un bloque de 1KB no puedo abarcar el entry entero. Tengo que buscar una solucion a eso.
 */
static int ext2_search_directory(vnode_t *dir_node, const char *dir_name, int length,
      uint32_t *inumber_p)
{
   uint8_t *data_pointer;
   ext2_direntry_t * start_pointer, * end_pointer;
   uint32_t lret; /* return big number */

   ext2_inode_t * ninfo;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   ninfo = (ext2_inode_t *)dir_node->n_info.down_layer_info;
   finfo = (ext2_file_info_t *)dir_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)dir_node->fs_info.down_layer_info;

   if(NULL == ninfo || NULL == finfo || NULL == fsinfo)
   {
      return -1;
   }
   dir_node->f_info.file_pointer = finfo->f_pointer = 0;
   /* FIXME: Malloc of entire file. Too much memory */
   data_pointer=(uint8_t*)ciaaPOSIX_malloc(finfo->f_di.i_size);
   if(NULL == data_pointer)
   {
      return -1;
   }
   ext2_buf_read_file(dir_node, (uint8_t*)data_pointer, finfo->f_di.i_size, &lret);
   if(lret!=finfo->f_di.i_size)
   {
      return -1;
   }
   for(   start_pointer=(ext2_direntry_t *)data_pointer,
      end_pointer=(ext2_direntry_t *)(data_pointer + finfo->f_di.i_size);
      start_pointer<end_pointer;
      start_pointer=(ext2_direntry_t *)((char*)start_pointer + start_pointer->rec_len))
      /* FIXME: pointer comparison, could be implementation dependent */
   {
      if(start_pointer->rec_len <= 0)
      {
         break;
      }
      if(start_pointer->inode == 0)
      {
         continue;
      }
      if(   start_pointer->name_len == length &&
         !ciaaPOSIX_memcmp(dir_name, start_pointer->name, length))
      {
         /* Directory found */
         *inumber_p=start_pointer->inode;
         ciaaPOSIX_free(data_pointer);
         return 0;
      }
   }

   ciaaPOSIX_free(data_pointer);
   return -1; /* Error, directory not found */
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

   finfo=file->node->f_info.down_layer_info;
   finfo->f_pointer = 0;
   /* reset seek pointer */

   return 0;
}

static int ext2_file_close(file_desc_t *file)
{
   /* TODO: What should be done here? */
   return 0;
}

static size_t ext2_file_read(file_desc_t *file, void *buf, size_t size)
{
   uint32_t read_size;
   int ret;

   /*TODO: Validate file->cursor */
   file->node->f_info.file_pointer = file->cursor;
   ret = ext2_buf_read_file(file->node, (uint8_t *)buf, size, &read_size);
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

static int ext2_format(vnode_t *dev_node)
{
   int ret;
   ciaaDevices_deviceType const *bdev;
   ciaaDevices_blockType blockInfo;

   ext2_superblock_t superblock;
   ext2_gd_t *gd_buffer;
   uint8_t *bitmap_buffer;
   ext2_inode_t node;
   ext2_direntry_t dir;

   uint16_t i,j,aux,group_index;
   uint16_t ngroups, nblocks_gd, inodes_per_group, inodeblocks_per_group,
            minmetablocks_per_group,  nblocks_last_group, dev_bsize, nblocks_group_overhead;
   uint32_t dev_size, block_offset, free_blocks_count, free_inodes_count;
   uint32_t group_offset, blocks_per_group;
   bdev = dev_node->fs_info.device;

   ASSERT_MSG(dev_node->n_info.type == VFS_FTBLK, "ext2_format(): not a block device");
   if(dev_node->n_info.type != VFS_FTBLK)
   {
      return -1;   /* Only a device can be formatted. This is not a device node */
   }

   ret = ciaaBlockDevices_ioctl(bdev, ciaaPOSIX_IOCTL_BLOCK_GETINFO, &blockInfo);
   ASSERT_MSG(ret > 0, "ext2_format(): ciaaBlockDevices_ioctl() failed");
   if(ret < 0)
   {
      return -1;
   }

   /* Use physical block size as fs block size */
   dev_bsize=blockInfo.blockSize;
   /* Verify that dev_bsize is not greater than the ext2 max block size */
   if(dev_bsize < 1024)   /* Minimum ext2 block size */
      dev_bsize = 1024;
   dev_size=blockInfo.lastPosition;

   /* Initialize default ext2 superblock contents */
   ciaaPOSIX_memset((uint8_t *)&superblock, 0, sizeof(ext2_superblock_t));

   for(i=0, aux=1024; aux < dev_bsize; i++, aux = 1024 << i);   /* blocksize log2 */
   superblock.s_log_block_size = i;
   superblock.s_first_data_block = (dev_bsize > EXT2_SBOFF) ? 0 : 1;  /* First Data Block */
   superblock.s_blocks_per_group = dev_bsize*8;  /* Number of blocks per group */
   superblock.s_frags_per_group = superblock.s_blocks_per_group;   /* Number of fragments per group */
   superblock.s_wtime = 0;             /* Last write time */
   superblock.s_mnt_count = 0;            /* Current mount count */
   superblock.s_max_mnt_count = -1;        /* Max mount count */
   superblock.s_magic = 0xEF53U;                /* Magic number */
   superblock.s_state = 1;                /* File system state. EXT2_VALID_FS */
   superblock.s_errors = 1;               /* Behaviour when detecting errors. EXT2_ERRORS_CONTINUE */
   superblock.s_lastcheck = 0;         /* time of last check */
   superblock.s_rev_level = 1;         /* Good old rev */
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
   /* (block/node) factor = 4 */
   superblock.s_inodes_count = superblock.s_blocks_count / 4;
   /* Reserved blocks count */
   superblock.s_r_blocks_count = 0;
   /* Number of block groups */
   ngroups = howmany(superblock.s_blocks_count - superblock.s_first_data_block,
                     superblock.s_blocks_per_group);
   /* Number of blocks per group */
   blocks_per_group = dev_bsize*8;
   /* Number of blocks reserved for the group descriptors in every group */
   nblocks_gd = howmany(sizeof(ext2_gd_t)*ngroups, dev_bsize);
   /* Number of nodes which fit in a single block */
   inodes_per_group = superblock.s_inodes_count / ngroups;
   /* Number of blocks reserved for physical node allocations */
   inodeblocks_per_group = howmany(superblock.s_inode_size * inodes_per_group, dev_bsize);
   /* Number of total metadata blocks per group */
   minmetablocks_per_group = 1 /*Superblock*/ + nblocks_gd + 1 /*Block bitmap*/ +
                     1 /*Inode bimap*/ + inodeblocks_per_group + 1 /*At least 1 data block*/;
   /* The last group contains generally less blocks than the others. It contains the remaining blocks */
   nblocks_last_group = superblock.s_blocks_count - superblock.s_first_data_block -
                        superblock.s_blocks_per_group * (ngroups - 1);

   /* If the last group contains less blocks than the minimum allowed, eliminate the group */
   if(nblocks_last_group < minmetablocks_per_group)
   {
      superblock.s_blocks_count -= nblocks_last_group;
      ngroups--;
      nblocks_last_group = superblock.s_blocks_per_group;
      nblocks_gd = howmany(sizeof(ext2_gd_t)*ngroups, dev_bsize);
      inodes_per_group = superblock.s_inodes_count / ngroups;
   }

   /* The number of nodes in a group must be multiple of 8 */
   superblock.s_inodes_per_group = rounddown(inodes_per_group, 8);
   /* Total node count in disk */
   superblock.s_inodes_count = inodes_per_group * ngroups;
   /* Recalculate number of nodes which fit in a single block */
   inodeblocks_per_group = howmany(superblock.s_inode_size * inodes_per_group, dev_bsize);
   /* Overhead blocks per group */
   nblocks_group_overhead = 1/*Superblock*/ + nblocks_gd + 1 /*Blockbitmap*/+ 1 /*Inodebitmap*/+ inodeblocks_per_group;

   /* Initialize in-memory group descriptors to write them later to disk */
   gd_buffer = (ext2_gd_t *) ciaaPOSIX_malloc(sizeof(ext2_gd_t)*ngroups);
   ASSERT_MSG(NULL != gd_buffer, "ext2_format(): ciaaPOSIX_malloc() failed");
   if(NULL == gd_buffer)
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
   }
   /* These values change as new files are created */
   superblock.s_free_inodes_count = free_inodes_count;
   superblock.s_free_blocks_count = free_blocks_count;

   /* on-memory superblock and group descriptors are ready to be written to disk */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      group_offset = EXT2_SBOFF + superblock.s_blocks_per_group * dev_bsize * group_index;
      /* If the block size is 1024, block 0 is boot block, block 1 is superblock. Blocks 1 to 8192 are group 1 blocks
       * Boot block dont count as group 1 block
       */

      ret = ext2_device_buf_write(bdev, (uint8_t *)&superblock, group_offset, sizeof(ext2_superblock_t));
      if(ret)
      {
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }

      ret = ext2_device_buf_write(bdev, (uint8_t *)gd_buffer, group_offset + EXT2_SBSIZE, sizeof(ext2_gd_t)*ngroups);
      if(ret)
      {
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }

   }
   /* Now write block and inode bitmaps to disk */
   /* Alloc memory to cover 1 entire block */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(dev_bsize);
   ASSERT_MSG(NULL != bitmap_buffer, "ext2_format(): ciaaPOSIX_malloc() failed");
   if(NULL == bitmap_buffer)
   {
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Block bitmap */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      ciaaPOSIX_memset(bitmap_buffer, 0, dev_bsize);
      /* Must set non existent blocks as reserved
       * Must set metadata blocks as reserved
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

      ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer, gd_buffer[group_index].block_bitmap * dev_bsize, dev_bsize);
      if(ret)
      {
         ciaaPOSIX_free(bitmap_buffer);
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }
   }

   /* Inode bitmap */
   for(group_index=0; group_index<ngroups; group_index++)
   {
      i = superblock.s_inodes_per_group/8;
      /* Inodes are all free at the beginning. Alloc special nodes later */
      ciaaPOSIX_memset(bitmap_buffer, 0, i);
      /* Must set non existing nodes as reserved */
      ciaaPOSIX_memset(bitmap_buffer+i, 0xFF, dev_bsize-i);
      /* Nodes 1 to 10 (0 to 9) are reserved. s_first_ino is node 11, or index 10 */
      if (group_index == 0)
      {
         /* mark reserved inodes */
         for (i = 0; i < EXT2_FIRSTINO; i++)
            setbit(bitmap_buffer, i);
      }
      ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer, gd_buffer[group_index].inode_bitmap*dev_bsize, dev_bsize);
      if(ret)
      {
         ciaaPOSIX_free(bitmap_buffer);
         ciaaPOSIX_free(gd_buffer);
         return -1;
      }

   }

   /* Create root directory */
   ciaaPOSIX_memset(&node, 0, sizeof(ext2_inode_t));

   node.i_mode = 040755;
   node.i_uid = node.i_gid = 1000;
   node.i_size = 1024;
   node.i_atime = node.i_ctime = node.i_mtime = 0;
   node.i_dtime = 0;
   node.i_links_count = 2;
   node.i_blocks = 2;

   /* Reserve free block in first group for root directory data */
   /* This block will be assigned to node.i_block[0] */
   ret = ext2_device_buf_read(bdev, (uint8_t *)bitmap_buffer, gd_buffer[0].block_bitmap*dev_bsize, dev_bsize);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   for(i=0; i<superblock.s_blocks_per_group/8 && bitmap_buffer[i]==0xFF; i++);   /* Search first free bit */
   ASSERT_MSG(i<superblock.s_blocks_per_group/8, "ext2_format(): No hay bit libre");
   if(i==superblock.s_blocks_per_group/8)
   {
      return -1;
   }
   for(j=0; j<8; j++)
   {
      if((bitmap_buffer[i] & (1<<j)) == 0)   /* Bit j of byte bitmap_buffer[i] is 0 */
      {                                    /* Block i*8+j is free, reserve it*/
         bitmap_buffer[i] |= (1<<j);
         break;
      }
   }
   /* Write the modified bitmap to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer, gd_buffer[0].block_bitmap*dev_bsize, dev_bsize);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   node.i_block[0] = superblock.s_first_data_block + 8*i + j;
   gd_buffer[0].free_blocks_count--;
   gd_buffer[0].free_inodes_count--;
   gd_buffer[0].used_dirs_count++;

   /* Refresh book keeping data in group descriptor */
   ret = ext2_device_buf_write(bdev, (uint8_t *)&gd_buffer[0], EXT2_GDOFF, sizeof(ext2_gd_t));
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Fill root inode */
   ret = ext2_device_buf_write(bdev, (uint8_t *)&node, gd_buffer[0].inode_table * dev_bsize +
                               superblock.s_inode_size, sizeof(ext2_inode_t)); /* Second inode in table. ROOT INODE */
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Create root entry */

   /* Self entry */
   dir.inode = 2;
   dir.rec_len = 12;
   dir.name_len = 1;
   dir.file_type = 2;
   ciaaPOSIX_strcpy(dir.name, ".");

   ret = ext2_device_buf_write(bdev, (uint8_t *)&dir, node.i_block[0]*dev_bsize, sizeof(ext2_direntry_t)); /* First entry, "." */
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }

   /* Parent entry */
   dir.inode = 2;
   dir.rec_len = 1012;
   dir.name_len = 2;
   dir.file_type = 2;
   ciaaPOSIX_strcpy(dir.name, "..");

   ret = ext2_device_buf_write(bdev, (uint8_t *)&dir, node.i_block[0]*dev_bsize+12, sizeof(ext2_direntry_t)); /* Second entry, ".." */
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      ciaaPOSIX_free(gd_buffer);
      return -1;
   }
   ciaaPOSIX_free(bitmap_buffer);

   return 0;
}

/* Preconditions: dest_node is allocated in vfs */
static int ext2_mount(vnode_t *dev_node, vnode_t *dest_node)
{
   int ret;
   ciaaDevices_deviceType const *bdev;
   ext2_inode_t * ninfo;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   /* FIXME: This implementation needs the mount directory to be non existant.
    *  Does not overwrite directory correctly
    */
   /* TODO: Root inode is a special case? */
   finfo=dest_node->f_info.down_layer_info=(ext2_file_info_t *) ciaaPOSIX_malloc(sizeof(ext2_file_info_t));
   ASSERT_MSG(NULL != finfo, "ext2_mount(): Mem error, could not alloc finfo");
   if(NULL == finfo)
   {
      return -1;
   }
   ninfo=dest_node->n_info.down_layer_info=(ext2_inode_t *) ciaaPOSIX_malloc(sizeof(ext2_inode_t));
   ASSERT_MSG(NULL != ninfo, "ext2_mount(): Mem error, could not alloc ninfo");
   if(NULL == ninfo)
   {
      return -1;
   }
   fsinfo=dest_node->fs_info.down_layer_info=(ext2_fs_info_t *) ciaaPOSIX_malloc(sizeof(ext2_fs_info_t));
   ASSERT_MSG(NULL != fsinfo, "ext2_mount(): Mem error, could not alloc fsinfo");
   if(NULL == fsinfo)
   {
      return -1;
   }
   /* Load file system information to memory */
   bdev = dest_node->fs_info.device;
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

   fsinfo->e2fs_gd = (ext2_gd_t *)ciaaPOSIX_malloc(fsinfo->s_groups_count*sizeof(ext2_gd_t));
   ASSERT_MSG(NULL != fsinfo->e2fs_gd, "ext2_mount(): Mem error, could not alloc gd");
   if(NULL == fsinfo->e2fs_gd)
   {
      return -1;
   }
   ret = ext2_device_buf_read(bdev, (uint8_t *)fsinfo->e2fs_gd, EXT2_GDOFF, fsinfo->s_groups_count*(sizeof(ext2_gd_t)));
   if(ret)
   {
      ciaaPOSIX_free(fsinfo->e2fs_gd);
      return -1;
   }

   ret = ext2_mount_load(dest_node);
   ASSERT_MSG(ret>=0, "ext2_mount(): mount load failed");
   return ret;
}

static int ext2_create_node(vnode_t *parent_node, vnode_t *child_node)
{
   int ret;

   /* Node creation depends on the file type. A directory needs to have 2 default directories after creation */
   if(VFS_FTDIR == child_node->n_info.type)
   {
      ret = ext2_create_directory_file(parent_node, child_node);
   }
   else if(VFS_FTREG == child_node->n_info.type)
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

   if(VFS_FTDIR == child_node->n_info.type)
   {
      ret = ext2_delete_directory_file(parent_node, child_node);
      ASSERT_MSG(-1 < ret, "ext2_delete_node(): ext2_delete_directory_file() failed");
   }
   else if(VFS_FTREG == child_node->n_info.type)
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
/* FIXME: Ignoring mounts inside another mount. Should take this case into account */
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
   if(VFS_FTDIR == root->n_info.type)
   {
      /* Recursively delete all subtrees under this subroot */
      ret = ext2_umount_subtree_rec(root->child_node);
      if(ret)
      {
         return -1;
      }
   }
   if(NULL != root->n_info.down_layer_info)
   {
      ciaaPOSIX_free(root->n_info.down_layer_info);
      root->n_info.down_layer_info = NULL;
   }
   if(NULL != root->f_info.down_layer_info)
   {
      ciaaPOSIX_free(root->f_info.down_layer_info);
      root->n_info.down_layer_info = NULL;
   }
   if(NULL != root->fs_info.down_layer_info)
   {
      ciaaPOSIX_free(root->fs_info.down_layer_info);
      root->n_info.down_layer_info = NULL;
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
   if(VFS_FTDIR == root->n_info.type)
   {
      ret = ext2_umount_subtree_rec(root->child_node);
      if(ret)
      {
         return -1;
      }
   }

   if(NULL != root->n_info.down_layer_info)
   {
      ciaaPOSIX_free(root->n_info.down_layer_info);
      root->n_info.down_layer_info = NULL;
   }
   if(NULL != root->f_info.down_layer_info)
   {
      ciaaPOSIX_free(root->f_info.down_layer_info);
      root->n_info.down_layer_info = NULL;
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
   ext2_inode_t * ninfo;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   node = desc->node;
   bdev = node->fs_info.device;
   ninfo = (ext2_inode_t *)node->n_info.down_layer_info;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   uint32_t total_read_size, buf_pos, remaining_write_size, device_block;
   uint32_t write_size, write_offset;

   if(NULL == ninfo || NULL == finfo || NULL == fsinfo)
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
   if(finfo->f_pointer > finfo->f_di.i_size)
      finfo->f_di.i_size = finfo->f_pointer;
   /* finfo->f_di.i_blocks was refreshed everytime ext2_file_map_alloc() was called.
    * Write inode in its position in disk
    * flush memory info to disk
    */
   ret = ext2_write_inode(node);
   if(ret)
   {
      return 0;
   }
   /* Write to disk the changes made to superblock and block descriptors */
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
   /* FIXME: If size>finfo->f_di.i_size error or truncate? */
   int ret;
   uint32_t file_block, block_offset;
   uint32_t device_block, device_offset;
   uint16_t block_size, block_shift, block_mask;
   uint32_t total_remainder_size, block_remainder, read_size, buf_pos;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;
   ciaaDevices_deviceType const *device;

   fsinfo = dest_node->fs_info.down_layer_info;
   finfo = dest_node->f_info.down_layer_info;
   device = dest_node->fs_info.device;

   block_size = fsinfo->s_block_size;
   block_shift = (uint16_t)fsinfo->e2sb.s_log_block_size + 10;
   block_mask = (uint32_t)block_size-1;
   finfo->f_pointer = dest_node->f_info.file_pointer;
   file_block = finfo->f_pointer >> block_shift;
   block_offset = finfo->f_pointer & block_mask;

   if(NULL != total_read_size_p)
      *total_read_size_p = 0;

   if(finfo->f_pointer > finfo->f_di.i_size)
      finfo->f_pointer = finfo->f_di.i_size;
   /*Truncate read size*/
   if(finfo->f_pointer + size > finfo->f_di.i_size)
      size = finfo->f_di.i_size - finfo->f_pointer;

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
      if(ret)
      {
         if(NULL != total_read_size_p)
            *total_read_size_p = buf_pos;
         return -1;
      }
      buf_pos+=read_size;
      finfo->f_pointer+=read_size;
      total_remainder_size-=read_size;
   }

   if(NULL != total_read_size_p)
      *total_read_size_p = buf_pos;
   if(buf_pos != size)
      return -1;
   dest_node->f_info.file_pointer = finfo->f_pointer;
   return 0;
}

/* Map file block number to disk block number
 *
 */
static int ext2_block_map(const vnode_t * dest_node, uint32_t file_block, uint32_t *device_block_p)
{
   uint32_t shift_block_level, shift_single_block_level;
   uint32_t temp_block_num, n_entries_per_block, index_offset, aux, block_level;
   size_t ret;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;
   ciaaDevices_deviceType const *device;

   device = dest_node->fs_info.device;
   fsinfo = dest_node->fs_info.down_layer_info;
   finfo = dest_node->f_info.down_layer_info;
   n_entries_per_block = fsinfo->s_block_size / sizeof(uint32_t);
   /* shift_single_level is the number of bits of the block indirection level mask */
   for(shift_single_block_level = 0, aux = n_entries_per_block; aux; shift_single_block_level++)
   {;
      aux = aux>>1;
   }

   if (file_block < N_DIRECT_BLOCKS)
   {
      /* Direct block */
      *device_block_p = finfo->f_di.i_block[file_block];
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
   temp_block_num = finfo->f_di.i_block[N_DIRECT_BLOCKS + block_level - 1];

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
/* FIXME: Use static memory. Eliminate malloc */
static int ext2_mount_load_rec(vnode_t *dir_node)
{

   uint8_t *data_pointer;
   ext2_direntry_t * start_pointer, * end_pointer;
   int ret;
   uint32_t lret;
   int mode;
   vnode_t *child_node;

   ext2_file_info_t *finfo;

   finfo = (ext2_file_info_t *)dir_node->f_info.down_layer_info;

   /* Read directory entries from beginning */
   dir_node->f_info.file_pointer = finfo->f_pointer = 0;
   data_pointer=(uint8_t*) ciaaPOSIX_malloc(finfo->f_di.i_size);
   /* FIXME: Malloc of entire file size */
   ASSERT_MSG(NULL != data_pointer, "ext2_mount_load_rec(): Mem error");
   if(data_pointer == NULL)
   {
      ciaaPOSIX_free(data_pointer);
      return -1;
   }
   ext2_buf_read_file(dir_node, (uint8_t*)data_pointer, finfo->f_di.i_size, &lret);   /* Read all entries in a row */
   ASSERT_MSG(lret==finfo->f_di.i_size, "ext2_mount_load_rec(): ext2_buf_read_file() failed");
   if(lret!=finfo->f_di.i_size)
   {
      ciaaPOSIX_free(data_pointer);
      return -1;
   }


   for(   start_pointer = (ext2_direntry_t *)data_pointer,
      end_pointer = (ext2_direntry_t *)(data_pointer + finfo->f_di.i_size);
      start_pointer < end_pointer;
      start_pointer = (ext2_direntry_t *)((char*)start_pointer + start_pointer->rec_len))
   {
      if(start_pointer->rec_len <= 0)
      {
         break;
      }
      if(start_pointer->inode == 0)
      {
         continue;
      }
      if(!ciaaPOSIX_strcmp(start_pointer->name, ".") || !ciaaPOSIX_strcmp(start_pointer->name, ".."))
      {
         continue;
      }
      mode=0;
      child_node = vfs_create_child(dir_node, start_pointer->name, start_pointer->name_len, mode);
      ASSERT_MSG(NULL != child_node, "ext2_mount_load_rec(): Mem error");
      if(NULL == child_node)
      {
         return -1;
      }
      /* Alloc and initialize the file low level information */
      child_node->f_info.down_layer_info = (ext2_file_info_t *) ciaaPOSIX_malloc(sizeof(ext2_file_info_t));
      child_node->n_info.down_layer_info = (ext2_inode_t *) ciaaPOSIX_malloc(sizeof(ext2_inode_t));
      if(NULL == child_node->f_info.down_layer_info || NULL == child_node->fs_info.down_layer_info
         || NULL == child_node->n_info.down_layer_info)   //FIXME
      {
         return -1;
      }
      ASSERT_MSG(ret >= 0, "ext2_mount_load_rec(): ext2_read_inode() failed");
      /* Read node data from disk */
      ret = ext2_read_inode(child_node, start_pointer->inode);
      ASSERT_MSG(ret >= 0, "ext2_mount_load_rec(): ext2_read_inode() failed");
      if(ret)
      {
         ciaaPOSIX_free(data_pointer);
         return -1;
      }
      if(EXT2_FT_DIR == start_pointer->file_type)
      {
         ret = ext2_mount_load_rec(child_node);
         if(ret)
         {
            ciaaPOSIX_free(data_pointer);
            return -1;
         }
      }

   }
   /* Finished iterating through dir_node subtree entirely. Success */
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
     finfo->f_di.i_size
     finfo->f_di.i_links_count
     finfo->f_di.i_mode
 * 4.Alloc a data block for the new node
 * 5.Add new entry in parents dir data
 * 6.Refresh bookkeeping info in superblock and group descriptor
 */

   int ret;
   uint32_t new_inumber, aux;

   ext2_inode_t * ninfo;
   ext2_file_info_t *finfo, *parent_finfo;

   ninfo = (ext2_inode_t *)node->n_info.down_layer_info;
   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;

   finfo = (ext2_file_info_t *) ciaaPOSIX_malloc(sizeof(ext2_file_info_t));
   ASSERT_MSG(NULL != finfo, "ext2_create_regular_file(): Mem error");
   if(NULL == finfo)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)finfo, 0, sizeof(ext2_file_info_t));
   node->f_info.down_layer_info = (void *)finfo;

   ninfo = (ext2_inode_t *)ciaaPOSIX_malloc(sizeof(ext2_inode_t));
   if(NULL == ninfo)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)ninfo, 0, sizeof(ext2_inode_t));
   node->n_info.down_layer_info = (void *)ninfo;

   /* Reserve a inode bit. This function modifies bookkeeping info on memory */
   ret = ext2_alloc_inode_bit(node, &new_inumber);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_alloc_inode_bit() failed");
   if(ret)
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
   /* Fill node fields */
   finfo->f_di.i_mode = 0x81B6;
   finfo->f_di.i_uid = 0;
   finfo->f_di.i_atime = 0;
   finfo->f_di.i_ctime = 0;
   finfo->f_di.i_flags = 0;

   /* Add new entry in parent dir data.  This function modifies node, gd & sb if new block reserved */
   ret = ext2_dir_add_entry(parent_node, new_inumber, node->f_info.file_name, node->f_info.file_namlen, EXT2_FT_REG_FILE);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_dir_add_entry() failed");
   if(ret)
   {
      return -1;
   }

   /* The new node is referred by the father. Increment link count */
   finfo->f_di.i_links_count++;
   parent_finfo->f_di.i_links_count++;
   /* Write inode in its disk position */
   ret = ext2_write_inode(node);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   /* Write modified parent node to disk */
   ret = ext2_write_inode(parent_node);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   ret = ext2_write_inode(node);
   ASSERT_MSG(0 == ret, "ext2_create_regular_file(): ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
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

   ext2_file_info_t *finfo, *parent_finfo;
   ext2_fs_info_t *fsinfo;

   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   file_blocks = finfo->f_di.i_size >> (10+fsinfo->e2sb.s_log_block_size);
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
   parent_finfo->f_di.i_links_count--;
   /* dealloc bit in inode bitmap */
   ret = ext2_dealloc_inode_bit(node);
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_dealloc_inode_bit() failed");
   if(ret)
   {
      return -1;
   }
   /* Write parent node to disk */
   ret = ext2_write_inode(parent_node);
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   /* Erase on-disk node, which means, clear all bits in its memory space */
   ciaaPOSIX_memset(&(finfo->f_di), 0, sizeof(ext2_inode_t));
   ret = ext2_write_inode(node);
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_write_inode() failed");
   if(ret)
   {
      return -1;
   }
   /* Write modified sb & gd */
   ret = ext2_fsinfo_flush(node);
   ASSERT_MSG(0 == ret, "ext2_delete_regular_file(): ext2_fsinfo_flush() failed");
   if(ret)
   {
      return -1;
   }
   /* Free low level data from the deleted file */
   ciaaPOSIX_free(node->f_info.down_layer_info);
   return 0;
}

static int ext2_create_directory_file(vnode_t *parent_node, vnode_t *node)
{
/* 1.Alloc space for node->f_info.down_layer_info
 * 2.Reserve a bit from inode_bitmap, with current allocation policies (ex.: Same gd as parent)
 * 3.Fill f_info fileds of the new child and its parent directory
     finfo->f_num the new node inumber
     finfo->f_di.i_size
     finfo->f_di.i_links_count
     finfo->f_di.i_mode
 * 4.Alloc a data block for the new node
 * 5.Add default entries for the new node. Refresh bookkeeping info
 * 6.Add new entry in parents dir data
 * 7.Refresh bookkeeping info in superblock and group descriptor
 */
   int ret;
   uint32_t new_inumber, aux;

   ext2_inode_t * ninfo;
   ext2_file_info_t *finfo, *parent_finfo;
   ext2_fs_info_t *fsinfo;

   ninfo = (ext2_inode_t *)node->n_info.down_layer_info;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   finfo = (ext2_file_info_t *) ciaaPOSIX_malloc(sizeof(ext2_file_info_t));
   if(NULL == finfo)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)finfo, 0, sizeof(ext2_file_info_t));
   node->f_info.down_layer_info = (void *)finfo;

   ninfo = (ext2_inode_t *) ciaaPOSIX_malloc(sizeof(ext2_inode_t));
   if(NULL == ninfo)
   {
      return -1;
   }
   ciaaPOSIX_memset((void *)ninfo, 0, sizeof(ext2_inode_t));
   node->n_info.down_layer_info = (void *)ninfo;

   ret = ext2_alloc_inode_bit(node, &new_inumber);
   if(ret)
   {
      return -1;
   }
   ret = ext2_file_map_alloc(node, 0, &aux);
   if(ret)
   {
      return -1;
   }
   /* mkdir creates dirs with 0755 access */
   /* Fill inode fields */
   finfo->f_di.i_mode = 0x41FF;
   finfo->f_di.i_uid = 0;
   finfo->f_di.i_atime = 0;
   finfo->f_di.i_ctime = 0;
   finfo->f_di.i_flags = 0;
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
   parent_finfo->f_di.i_links_count++;
   finfo->f_di.i_links_count = 2;
   /* Refresh used dir count in on-memory group descriptor */
   fsinfo->e2fs_gd[finfo->f_group].used_dirs_count++;
   /* Write inode to disk */
   ret = ext2_write_inode(node);
   if(ret)
   {
      return -1;
   }
   /* Better node without entry than entry with buggy node.
    * Add entry to parent only after having valid new node in disk
    */
   ret = ext2_write_inode(parent_node);
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

   ext2_file_info_t *finfo, *parent_finfo;
   ext2_fs_info_t *fsinfo;

   parent_finfo = (ext2_file_info_t *)parent_node->f_info.down_layer_info;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

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
   ciaaPOSIX_memset(&(finfo->f_di), 0, sizeof(ext2_inode_t));
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

   ret = ext2_write_inode(parent_node);
   if(ret)
   {
      return -1;
   }
   /* Erase node by writing 0 in its space */
   ret = ext2_write_inode(node);
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
   /* Free low level info */
   ciaaPOSIX_free(node->f_info.down_layer_info);
   return 0;
}

/* Alloc policy: First free bit found, starting from beginning. Linear search, no optimization.
 * Refresh in-memory file system information, except for used dir count.
 */
static int ext2_alloc_inode_bit(vnode_t *node, uint32_t *new_inumber_p)
{
   /* Search a group with free inode. Retrieve the free inode number */
   /* TODO: Slow allocation primitive. Should optimize */
   int ret;
   uint32_t i,j,n;
   uint16_t group_index;
   uint8_t *bitmap_buffer;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   /* Search a group with a free inode */
   for(group_index = 0; group_index < fsinfo->s_groups_count; group_index++)
   {
      if(fsinfo->e2fs_gd[group_index].free_inodes_count)
         break;
   }
   ASSERT_MSG(group_index<fsinfo->s_groups_count, "ext2_alloc_inode_bit(): No group with free inodes");
   if(group_index >= fsinfo->s_groups_count)
   {
      /* All gd indicate that the group is full */
      return -1;
   }
   /* Alloc memory for bitmap buffer */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   ASSERT_MSG(NULL != bitmap_buffer, "ext2_alloc_inode_bit(): Mem error");
   if(NULL == bitmap_buffer)
   {
      return -1;
   }

   /* Read node bitmap to bitmap_buffer */
   ret = ext2_device_buf_read(bdev, (uint8_t *)bitmap_buffer,
                              (fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
                              fsinfo->s_block_size);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /* Search for a free bitmap, which means,bit whose value is 0 */
   /* First search a byte whose value is not FF, which means, it contains a 0 bit */
   for(i = 0; i<fsinfo->s_block_size && 0XFFU == bitmap_buffer[i]; i++);
   ASSERT_MSG(i<fsinfo->s_block_size, "ext2_alloc_inode_bit(): No free byte. Descriptor error");
   if(i >= fsinfo->s_block_size)
   {
      /* No free bit found
       * Metadata inconsistency error. Group descriptor shows that there is a free bit available
       */
      return -1;
   }
   /* Find free bit inside the found byte */
   for(j = 0; j < 8; j++)
   {
      if(!(bitmap_buffer[i] & (1<<j)))
      {
         bitmap_buffer[i] |= (1<<j);
         n = 8*i + j;
         break;
      }
   }
   ASSERT_MSG(j < 8, "ext2_alloc_inode_bit(): No free bit in selected byte. Inconsistency error");
   if(j >= 8)
   {
      return -1;   /* Should never happen */
   }
   /* n is the free block number relative to the current group descriptor.
    * Must calculate absolute inode number
    */
   *new_inumber_p = group_index * fsinfo->e2sb.s_inodes_per_group + n + 1;
   /* Write modified bitmap to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer,
                              (fsinfo->e2fs_gd[group_index].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
                              fsinfo->s_block_size);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /* Refresh in-memory bookkeeping info. The caller should write it to disk */
   fsinfo->e2sb.s_free_inodes_count--;
   fsinfo->e2fs_gd[group_index].free_inodes_count--;
   //fsinfo->e2fs_gd[group_index].used_dirs_count++; /* Caller knows if this is a dir or not. Leave unmodified */
   finfo->f_group = group_index;
   finfo->f_inumber = *new_inumber_p;

   return 0;
}

static int ext2_dealloc_inode_bit(vnode_t *node)
{
   int ret;
   uint32_t n, i;
   uint8_t *bitmap_buffer;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   /* Alloc memory for bitmap buffer */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   if(NULL == bitmap_buffer)
   {
      return -1;
   }

   /* Read bitmap to bitmap_buffer */
   ret = ext2_device_buf_read(bdev, (uint8_t *)bitmap_buffer,
                              (fsinfo->e2fs_gd[finfo->f_group].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
                              fsinfo->s_block_size);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }

   /* n is the node number relative to the current group descriptor.
    * Must deallocate bit n of current group descriptors node bimap
    */
   n = finfo->f_inumber - (finfo->f_group * fsinfo->e2sb.s_inodes_per_group) - 1;
   clrbit(bitmap_buffer, n);
   /* Write modified bitmap to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer,
                              (fsinfo->e2fs_gd[finfo->f_group].inode_bitmap)<<(10+fsinfo->e2sb.s_log_block_size),
                              fsinfo->s_block_size);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /* Refresh on-memory bookkeeping info. Caller should write them to disk */
   fsinfo->e2sb.s_free_inodes_count++;
   fsinfo->e2fs_gd[finfo->f_group].free_inodes_count++;
   //fsinfo->e2fs_gd[finfo->f_group].used_dirs_count++; /* Caller knows whether this is a regfile or directory */
   finfo->f_inumber = 0;

   return 0;
}


/* TODO: Validate arguments */
/*FIXME: Borro el bloque determinado por el numero contenido en new_block. Pero new_block puede no estar inicializado */
static int ext2_file_map_alloc(vnode_t *node, uint32_t file_block, uint32_t *device_block_p)
{
   int ret;
   uint32_t block_pos, new_block, temp_block_num, index_offset;
   uint8_t shift_block_level, shift_single_block_level, block_level, flag_new_block;
   uint8_t *block_buffer;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   if(NULL == device_block_p)
      return -1;
   flag_new_block = 0;   /* To tell if a new file block is being allocated */
   *device_block_p = 0;
   block_pos = file_block;
   if (block_pos < N_DIRECT_BLOCKS)
   {
      if(0 == finfo->f_di.i_block[block_pos])
      {
         ret = ext2_alloc_block_bit(node, &new_block);
         ASSERT_MSG(ret>=0, "ext2_file_map_alloc(): ext2_alloc_block_bit() failed");
         if(ret)
         {
            return -1;
         }
         finfo->f_di.i_block[block_pos] = new_block;
         finfo->f_di.i_blocks += fsinfo->sectors_in_block;
         flag_new_block = 1;
      }
      *device_block_p = finfo->f_di.i_block[block_pos];
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
      shift_single_block_level = 8;   /* 1<<8 == 256. 256 entries each block */
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
   
      temp_block_num = finfo->f_di.i_block[N_DIRECT_BLOCKS - 1 + block_level];
      if (0 == temp_block_num)
      {
         ret = ext2_alloc_block_bit(node, &temp_block_num);
         if(ret)
         {
            *device_block_p = 0;
            return -1;
         }
         finfo->f_di.i_block[N_DIRECT_BLOCKS - 1 + block_level] = temp_block_num;
         finfo->f_di.i_blocks += fsinfo->sectors_in_block;
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
      block_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
      if(NULL == block_buffer)
      {
         return -1;
      }
      ciaaPOSIX_memset(block_buffer, 0, fsinfo->s_block_size);
      ret = ext2_device_buf_write(bdev, (uint8_t *)block_buffer, new_block<<(10+fsinfo->e2sb.s_log_block_size),
                                    fsinfo->s_block_size);
      if(ret)
      {
         ciaaPOSIX_free(block_buffer);
         return -1;
      }
      ciaaPOSIX_free(block_buffer);
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
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   /* Alloc memory of the size of a block */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   if(NULL == bitmap_buffer)
   {
      *block = 0;
      return -1;
   }

   /* Read bitmap from disk to the bitmap buffer */
   ret = ext2_device_buf_read(bdev, (uint8_t *)bitmap_buffer,
                              fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size,
                              fsinfo->s_block_size);
   if(ret)
   {
      *block = 0;
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   /* Search a free bit in bitmap
    * First search for a non 0xFF byte, then find out which is the first zero bit in that byte
    */
   for(i = 0; i<fsinfo->s_block_size && bitmap_buffer[i] == 0xFF; i++);
   if(i >= fsinfo->s_block_size)
   {
      /* No free blocks in this group */
      *block = 0;
      return -1;
   }
   /* Find free bit inside found byte */
   for(j=0; j<8; j++)
   {
      if(!(bitmap_buffer[i] & (1<<j)))
      {
         bitmap_buffer[i] |= (1<<j);
         b = 8*i + j;
         break;
      }
   }
   if(8 == j)
   {
      *block = 0;
      return -1;   /* Should never happen */
   }
   /* Write modified bitmap to disk again */
   ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer,
                              fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size,
                              fsinfo->s_block_size);
   if(ret)
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
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   /* Alloc memory for a block */
   bitmap_buffer = (uint8_t *) ciaaPOSIX_malloc(fsinfo->s_block_size);
   ASSERT_MSG(NULL != bitmap_buffer, "ext2_dealloc_block_bit(): ciaaPOSIX_malloc() failed");
   if(NULL == bitmap_buffer)
   {
      return -1;
   }

   /* Read bitmap from disk */
   ret = ext2_device_buf_read(bdev, (uint8_t *)bitmap_buffer,
                              fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size,
                              fsinfo->s_block_size);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }

   b = block - fsinfo->e2sb.s_first_data_block - (fsinfo->e2sb.s_blocks_per_group*finfo->f_group);
   clrbit(bitmap_buffer, b);

   /* Write modified bitmap to disk */
   ret = ext2_device_buf_write(bdev, (uint8_t *)bitmap_buffer,
                              fsinfo->e2fs_gd[finfo->f_group].block_bitmap * fsinfo->s_block_size,
                              fsinfo->s_block_size);
   if(ret)
   {
      ciaaPOSIX_free(bitmap_buffer);
      return -1;
   }
   fsinfo->e2fs_gd[finfo->f_group].free_blocks_count++;
   fsinfo->e2sb.s_free_blocks_count++;
   ciaaPOSIX_free(bitmap_buffer);
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
   ext2_direntry_t ext2_dir_entry_buffer;
   ext2_direntry_t *ext2_dir_entry_buffer_p;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;
   ext2_dir_entry_buffer_p = &ext2_dir_entry_buffer;

   new_entry_size = DENTRY_MIN_SIZE + namlen;
   /* Align required_space to 4 bytes. Directory entry rule */
   new_entry_size += (new_entry_size & 0x03) ? (4 - (new_entry_size & 0x03)) : 0;

   /* size must be multiple of block size. Dont consider special cases */
   /* jump from block to block in every iteration */
   for(file_pointer = 0; file_pointer < finfo->f_di.i_size; file_pointer += fsinfo->s_block_size)
   {
      /* Map file block to disk block. Result in data_block */
      ret = ext2_file_map_alloc(node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
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
         if(ret)
         {
            return -1;
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
      ret = ext2_file_map_alloc(node, (finfo->f_di.i_size)>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      ASSERT_MSG(0 == ret, "ext2_dir_add_entry(): ext2_file_map_alloc() failed");
      if(ret)
      {
         return -1;
      }
      /* New directory size adds 1 block */
      finfo->f_di.i_size += fsinfo->s_block_size;
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
   ext2_direntry_t ext2_dir_entry_buffer;

   ciaaDevices_deviceType const *bdev;
   ext2_file_info_t *finfo;
   ext2_fs_info_t *fsinfo;

   bdev = node->fs_info.device;
   finfo = (ext2_file_info_t *)node->f_info.down_layer_info;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   /* size must be multiple of block size. Dont consider special cases */
   /* jump from block to block in every iteration */
   for(file_pointer = 0; file_pointer < finfo->f_di.i_size; file_pointer += fsinfo->s_block_size)
   {
      /* Map file block to disk block. Result in data_block */
      ret = ext2_file_map_alloc(node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
      //ret = ext2_block_map(node, file_pointer>>(10+fsinfo->e2sb.s_log_block_size), &data_block);
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

   bdev = node->fs_info.device;
   fsinfo = (ext2_fs_info_t *)node->fs_info.down_layer_info;

   for(group_index = 0; group_index<(fsinfo->s_groups_count); group_index++)
   {
      group_offset = EXT2_SBOFF + fsinfo->e2sb.s_blocks_per_group * fsinfo->s_block_size * group_index;
      /* If the block size is 1024, block 0 is boot block, block 1 is superblock. Blocks 1 to 8192 are group 1 blocks
       * Boot block dont count as group 1 block
       */
      ret = ext2_device_buf_write(bdev, (uint8_t *)&(fsinfo->e2sb), group_offset, sizeof(ext2_superblock_t));
      if(ret)
      {
         return -1;
      }
      ret = ext2_device_buf_write(bdev, (uint8_t *)fsinfo->e2fs_gd, group_offset + EXT2_SBSIZE,
                                    sizeof(ext2_gd_t) * fsinfo->s_groups_count);
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

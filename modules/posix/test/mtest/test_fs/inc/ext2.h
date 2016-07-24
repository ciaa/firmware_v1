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

#ifndef EXT2_H
#define EXT2_H

/** \brief EXT2 driver header file
 **
 ** This is the header file of the ext2 file system implementation
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup MTests CIAA Firmware Module Tests
 ** @{ */
/** \addtogroup Filesystem EXT2
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

#include "ciaaPOSIX_stdint.h"
#include "vfs.h"

/*==================[macros]=================================================*/

/** \brief Maximum quantity of simultaneous mounts */
#define EXT2_MAX_MOUNTS 32
/** \brief Size of array ext2_block_buffer defined in ext2.c. Its used to read and write portions of blocks */
#define EXT2_BLOCK_BUFFER_SIZE 1024
/** \brief ext2_block_buffer */
#define EXT2_PATH_MAX 50

/** \brief Blank block size */
#define EXT2_BBSIZE      1024
/** \brief Superblock size */
#define EXT2_SBSIZE      1024
/** \brief Blank block offset */
#define   EXT2_BBOFF     0
/** \brief Superblock offset */
#define   EXT2_SBOFF      (EXT2_BBOFF + EXT2_BBSIZE)
/** \brief Group descriptor offset */
#define EXT2_GDOFF        (EXT2_SBOFF + EXT2_SBSIZE)

/** \brief ext2fs magic number */
#define EXT2_MAGIC  0xef53U

/** \brief How much direct block addresses contains the inode */
#define N_DIRECT_BLOCKS      11
/** \brief How much INdirect block addresses contains the inode */
#define N_INDIRECT_BLOCKS   3

/** \brief Special inode numbers */
#define   EXT2_RESERVED_INODES   10
#define   EXT2_BADBLKINO         1
#define   EXT2_ROOTINO           2
#define   EXT2_ACLIDXINO         3
#define   EXT2_ACLDATAINO        4
#define   EXT2_BOOTLOADERINO     5
#define   EXT2_UNDELDIRINO       6
#define   EXT2_RESIZEINO         7
#define   EXT2_JOURNALINO        8
#define   EXT2_FIRSTINO          11

#define   EXT2_MAXNAMELEN   255

/** \brief ext2 file types */
#define EXT2_FT_UNKNOWN         0   /* Unknown File Type */
#define EXT2_FT_REG_FILE        1   /* Regular File */
#define EXT2_FT_DIR             2   /* Directory File */
#define EXT2_FT_CHRDEV          3   /* Character Device */
#define EXT2_FT_BLKDEV          4   /* Block Device */
#define EXT2_FT_FIFO            5   /* Buffer File */
#define EXT2_FT_SOCK            6   /* Socket File */
#define EXT2_FT_SYMLINK         7   /* Symbolic Link */

/** \brief Ext2 i_mode values */
#define EXT2_S_IFSOCK  0xC000   /* socket */
#define EXT2_S_IFLNK   0xA000   /* symbolic link */
#define EXT2_S_IFREG   0x8000   /* regular file */
#define EXT2_S_IFBLK   0x6000   /* block device */
#define EXT2_S_IFDIR   0x4000   /* directory */
#define EXT2_S_IFCHR   0x2000   /* character device */
#define EXT2_S_IFIFO   0x1000   /* fifo */

/** \brief ext2 directory entry values */

/* inode(4) + rec_len(2) + name_len(1) + file_type(1), doesnt include name  */
#define DENTRY_MIN_SIZE 8

#define DENTRY_USED_SIZE(d) (DENTRY_MIN_SIZE + (d)->name_len)
/* size with padding, aligned to 4 bytes */
#define DENTRY_TOTAL_SIZE(d) (  DENTRY_USED_SIZE(d) + ( (DENTRY_USED_SIZE(d) & 0x03) ? \
                             (4 - (DENTRY_USED_SIZE(d) & 0x03)) : 0 )  )

/* Format parameters constants */
#define EXT2_MIN_BLOCKSIZE 1024
#define EXT2_MAX_BLOCKSIZE 4096
#define EXT2_DEFAULT_BLOCKNODE_FACTOR 4

/* Bitmap handle macros */
/* Set the bit i from array */
#define setbit(array,i)   ((array)[(i)>>3] |= 1<<((i)&0x07))
/* Clear the bit i from array */
#define clrbit(array,i)   ((array)[(i)>>3] &= ~(1<<((i)&0x07)))

/* Macros for counting and rounding. */
/* Count how many y bytes size items are needed to hold x bytes */
#define howmany(x, y) (((x)+((y)-1))/(y))
/* Roundup x to the nearest y multiple */
#define roundup(x, y)  ((((x)+((y)-1))/(y))*(y))
/* Rounddown x to the nearest y multiple */
#define rounddown(x,y) (((x)/(y))*(y))

/* Misc macros */
/* Check if power of 2 */
#define is_powerof2(x) (( (x) != 0 ) && !( (x) & ( (x) - 1)))



/*==================[typedef]================================================*/

/** \brief ext2 superblock
 **
 ** Superblock of the ext2 file system
 **
 ** TODO: Skip suffix padding to economize memory
 **/
typedef struct ext2sb
{                                      /* offset : description */
   uint32_t   s_inodes_count;          /* 0: Inodes count */
   uint32_t   s_blocks_count;          /* 4: Blocks count */
   uint32_t   s_r_blocks_count;        /* 8: Reserved blocks count */
   uint32_t   s_free_blocks_count;     /* 12: Free blocks count */
   uint32_t   s_free_inodes_count;     /* 16: Free inodes count */
   uint32_t   s_first_data_block;      /* 20: First Data Block */
   uint32_t   s_log_block_size;        /* 24: Block size */
   uint32_t   s_log_frag_size;         /* 28: Fragment size */
   uint32_t   s_blocks_per_group;      /* 32: # Blocks per group */
   uint32_t   s_frags_per_group;       /* 36: # Fragments per group */
   uint32_t   s_inodes_per_group;      /* 40: # Inodes per group */
   uint32_t   s_mtime;                 /* 44: Mount time */
   uint32_t   s_wtime;                 /* 48: Write time */
   uint16_t   s_mnt_count;             /* 52: Mount count */
   uint16_t   s_max_mnt_count;         /* 54: Maximal mount count */
   uint16_t   s_magic;                 /* 56: Number that identifies the system as ext2. Fixed as 0xEF53 */
   uint16_t   s_state;                 /* 58: File system state. Set to EXT2_ERROR_FS when mounted.
                                        * Set to EXT2_VALID_FS when correctly unmounted.
                                        */
   uint16_t   s_errors;                /* 60: What should the fs do when error detected
                                        * EXT2_ERRORS_CONTINUE: No special action
                                        * EXT2_ERRORS_RO: Remount read-only
                                        * EXT2_ERRORS_PANIC: Kernel panic
                                        */
   uint16_t   s_minor_rev_level;       /* 62: minor revision level */
   uint32_t   s_lastcheck;             /* 64: Unix time of the last file system check */
   uint32_t   s_checkinterval;         /* 68: Maximum Unix interval allowed between checks */
   uint32_t   s_creator_os;            /* 72: Identifier of the OS that created the fs
                                        * EXT2_OS_LINUX
                                        * EXT2_OS_HURD
                                        * EXT2_OS_MASIX
                                        * EXT2_OS_FREEBSD
                                        * EXT2_OS_LITES
                                        */
   uint32_t   s_rev_level;             /* 76: Revision level
                                        * EXT2_GOOD_OLD_REV: Old basic version
                                        * EXT2_DYNAMIC_REV: variable node size, extended attributes, etc.
                                        */
   uint16_t   s_def_resuid;            /* 80: Default user id for reserved blocks */
   uint16_t   s_def_resgid;            /* 82: Default group id for reserved blocks */
   uint32_t   s_first_ino;             /* 84: First non-reserved inode */
   uint16_t   s_inode_size;            /* 88: size of inode structure. 128 in revision 0. */
   uint16_t   s_block_group_nr;        /* 90: Number of block group containing this superblock */
   uint32_t   s_feature_compat;        /* 92: compatible features set */
   uint32_t   s_feature_incompat;      /* 96: incompatible feature set */
   uint32_t   s_feature_ro_compat;     /* 100: readonly-compatible feature set */
   uint8_t    s_uuid[16];              /* 104: 128-bit uuid for volume */
   char       s_volume_name[16];       /* 120: volume name string */
   //uint8_t    s_last_mounted[64];      /* 136: path of directory where last mounted */
   //uint32_t   s_algo_bitmap;           /* 200: For data compression. Unused feature in this implementation */
   //uint8_t    s_prealloc_blocks;       /* 204: # of blocks to preallocate when creating regular file */
   //uint8_t    s_prealloc_dir_blocks;   /* 205: # of blocks to preallocate when creating directory */
   //uint8_t    s_alignment[2];          /* 206: 4 byte alignment */
   //uint8_t    padding[816];            /* 208: Not used fields from here on. Padding */
} ext2_superblock_t;

/** \brief ext2 group descriptor
 **
 ** ext2 file system block group descriptor
 **
 **/
typedef struct ext2_gd
{
    uint32_t  block_bitmap;        /* Blocks bitmap block */
    uint32_t  inode_bitmap;        /* Inodes bitmap block */
    uint32_t  inode_table;         /* Inodes table block */
    uint16_t  free_blocks_count;   /* Free blocks count */
    uint16_t  free_inodes_count;   /* Free inodes count */
    uint16_t  used_dirs_count;     /* Directories count */
    uint16_t  pad;
    uint8_t  reserved[12];
} ext2_gd_t;

/** \brief ext2 directory information
 **
 ** ext2 file system file entry in directory
 **
 **/
typedef struct ext2_direntry
{
   uint32_t inode;                   /* Inode number of entry */
   uint16_t rec_len;                 /* Length of this record */
   uint8_t name_len;                 /* Length of string in d_name */
   uint8_t file_type;                /* File type */
   char    name[EXT2_MAXNAMELEN];   /* Name with length<=EXT2_MAXNAMELEN */
} ext2_direntry_t;

/** \brief ext2 node structure
 **
 ** ext2 node structure
 **
 **/
typedef struct ext2_inode
{                              /* offset : description */
   uint16_t   i_mode;          /* 0: Format of file and access rights */
   uint16_t   i_uid;           /* 2: User id */
   uint32_t   i_size;          /* 4: Size in bytes */
   uint32_t   i_atime;         /* 8: Unix time of last acces */
   uint32_t   i_ctime;         /* 12: Unix time of creation */
   uint32_t   i_mtime;         /* 16: Unix time of last modification */
   uint32_t   i_dtime;         /* 20: Unix time of last deletion */
   uint16_t   i_gid;           /* 24: id of group which has access to this file */
   uint16_t   i_links_count;   /* 26: Counts how many times this inode is referred to */
   uint32_t   i_blocks;        /* 28: 512-byte data block count, independently of whether they are used or not */
   uint32_t   i_flags;         /* 32: How should the OS behave when accessing the data of this node */
   uint32_t   i_osd1;          /* 36: OS dependent value */
   uint32_t   i_block[N_DIRECT_BLOCKS + N_INDIRECT_BLOCKS];   /* 40: disk blocks */
   uint32_t   i_gen;           /* 100: generation number. Used in NFS */
   uint32_t   i_file_acl;      /* 104: file ACL (not implemented). Always 0 */
   uint32_t   i_dir_acl;       /* 108: dir ACL (not implemented). Always 0 */
   uint32_t   i_faddr;         /* 112: fragment address. Obsolete, always 0 */
   uint32_t   i_osd2[3];       /* 116: OS dependent value */
} ext2_inode_t;

/** \brief ext2 file system
 **
 ** ext2 file system information
 **
 **/
typedef struct ext2_fs_info
{
   ext2_superblock_t e2sb;
   uint32_t   s_block_size;         /* Block size in bytes. */
   uint32_t   s_inodes_per_block;   /* Number of inodes per block */
   uint32_t   s_itb_per_group;      /* Number of inode table blocks per group */
   uint32_t   s_ginfodb_count;      /* Number of group descriptor blocks */
   uint32_t   s_desc_per_block;     /* Number of group descriptors per block */
   uint32_t   s_groups_count;       /* Number of groups in the fs */
   uint8_t    sectors_in_block;     /* Sector is 512 bytes long */
   uint16_t   s_buff_size;          /* Size of the block chunks to be read in buffer */
   uint8_t    s_buff_per_block;     /* How much chunks per block */
} ext2_fs_info_t;

/** \brief ext2 file info
 **
 ** ext2 file information
 **
 **/
typedef struct ext2_file_info
{
   ext2_inode_t        f_di;        /* Copy of on-disk inode */
   uint16_t            f_group;     /* Block group number in which the node resides */
   uint32_t            f_pointer;   /* Local seek pointer */
   uint32_t            f_inumber;   /* Inode number */
} ext2_file_info_t;

typedef struct ext2_format_param
{
   uint32_t partition_size;
   uint16_t block_size; /* Valid block_size values are 1024, 2048 and 4096 bytes per block */
   uint8_t block_node_factor; /* blocks/nodes. Default is 4 */
} ext2_format_param_t;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

#endif /* EXT2_H */

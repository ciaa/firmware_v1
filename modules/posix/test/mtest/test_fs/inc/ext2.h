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

#ifndef EXT_H_
#define EXT_H_

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

#define EXT2_PATH_MAX 50

/** \brief Blank block size */
#define EXT2_BBSIZE      1024
/** \brief Superblock size */
#define EXT2_SBSIZE      1024
/** \brief Blank block offset */
#define   EXT2_BBOFF      ((long)(0))
/** \brief Superblock offset */
#define   EXT2_SBOFF      ((long)(EXT2_BBOFF + EXT2_BBSIZE))
/** \brief Group descriptor offset */
#define EXT2_GDOFF      ((long) EXT2_SBOFF + EXT2_SBSIZE)

/** \brief ext2fs magic number */
#define E2FS_MAGIC  0xef53

/*
 * Number of indirects in a file system block.
 */
//Cuantos numeros de bloque entran en un bloque?

/** \brief Cantidad de numeros de bloque directos en el inode fisico */
#define N_DIRECT_BLOCKS      11
/** \brief Cantidad de numeros de bloque indirectos en el inode fisico */
#define N_INDIRECT_BLOCKS   3

/** \brief Special inode numbers */
#define   EXT2_RESERVED_INODES	10
#define   EXT2_BADBLKINO      ((unsigned long)1)
#define   EXT2_ROOTINO      ((unsigned long)2)
#define   EXT2_ACLIDXINO      ((unsigned long)3)
#define   EXT2_ACLDATAINO      ((unsigned long)4)
#define   EXT2_BOOTLOADERINO   ((unsigned long)5)
#define   EXT2_UNDELDIRINO   ((unsigned long)6)
#define   EXT2_RESIZEINO      ((unsigned long)7)
#define   EXT2_JOURNALINO      ((unsigned long)8)
#define   EXT2_FIRSTINO      ((unsigned long)11)

#define   EXT2FS_MAXNAMLEN   255

/** \brief ext2 file types */
#define EXT2_FT_UNKNOWN		0	//Unknown File Type
#define EXT2_FT_REG_FILE	1	//Regular File
#define EXT2_FT_DIR		2	//Directory File
#define EXT2_FT_CHRDEV		3	//Character Device
#define EXT2_FT_BLKDEV		4	//Block Device
#define EXT2_FT_FIFO		5	//Buffer File
#define EXT2_FT_SOCK		6	//Socket File
#define EXT2_FT_SYMLINK		7	//Symbolic Link

/** \brief Ext2 i_mode values */
#define EXT2_S_IFSOCK  0xC000   //socket
#define EXT2_S_IFLNK   0xA000   //symbolic link
#define EXT2_S_IFREG   0x8000   //regular file
#define EXT2_S_IFBLK   0x6000   //block device
#define EXT2_S_IFDIR   0x4000   //directory
#define EXT2_S_IFCHR   0x2000   //character device
#define EXT2_S_IFIFO   0x1000   //fifo

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved
 */
/** \brief ext2 directory entry values */

/* e2d_ino(4) + e2d_reclen(2) + e2d_namlen(1) + e2d_type(1), doesnt include e2d_name  */
#define DENTRY_MIN_SIZE	8

#define DENTRY_USED_SIZE(d) (DENTRY_MIN_SIZE + (d)->e2d_namlen)
/* size with padding, aligned to 4 bytes */
#define DENTRY_TOTAL_SIZE(d) (  DENTRY_USED_SIZE(d) + ( (DENTRY_USED_SIZE(d) & 0x03) ? \
			(4 - (DENTRY_USED_SIZE(d) & 0x03)) : 0 )  )

/* Bit map related macros. */
#define	setbit(a,i)	((a)[(i)>>3] |= 1<<((i)%8))
#define	clrbit(a,i)	((a)[(i)>>3] &= ~(1<<((i)%8)))
#define	isset(a,i)	((a)[(i)>>3] & (1<<((i)%8)))
#define	isclr(a,i)	(((a)[(i)>>3] & (1<<((i)%8))) == 0)

/* Macros for counting and rounding. */
#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif
#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#define	rounddown(x,y)	(((x)/(y))*(y))
#define	roundup2(x, m)	(((x) + (m) - 1) & ~((m) - 1))
#define	powerof2(x)	((((x)-1)&(x))==0)

/* Macros for min/max. */
#define	MIN(a,b)	(((a)<(b))?(a):(b))
#define	MAX(a,b)	(((a)>(b))?(a):(b))

/*==================[typedef]================================================*/

/** \brief ext2 superblock
 **
 ** Superblock of the ext2 file system
 **
 **/
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 */
 struct ext2sb {
   uint32_t  s_inodes_count;      /* Inodes count */
   uint32_t  s_blocks_count;      /* Blocks count */
   uint32_t  s_r_blocks_count;    /* Reserved blocks count */
   uint32_t  s_free_blocks_count; /* Free blocks count */
   uint32_t  s_free_inodes_count; /* Free inodes count */
   uint32_t  s_first_data_block;  /* First Data Block */
   uint32_t  s_log_block_size;    /* Block size */
   uint32_t  s_log_frag_size;     /* Fragment size */
   uint32_t  s_blocks_per_group;  /* # Blocks per group */
   uint32_t  s_frags_per_group;   /* # Fragments per group */
   uint32_t  s_inodes_per_group;  /* # Inodes per group */
   uint32_t  s_mtime;             /* Mount time */
   uint32_t  s_wtime;             /* Write time */
   uint16_t  s_mnt_count;            /* Mount count */
   uint16_t  s_max_mnt_count;        /* Maximal mount count */
   uint16_t  s_magic;                /* Magic signature */
   uint16_t  s_state;                /* File system state */
   uint16_t  s_errors;               /* Behaviour when detecting errors */
   uint16_t  s_minor_rev_level;      /* minor revision level */
   uint32_t  s_lastcheck;         /* time of last check */
   uint32_t  s_checkinterval;     /* max. time between checks */
   uint32_t  s_creator_os;        /* OS */
   uint32_t  s_rev_level;         /* Revision level */
   uint16_t  s_def_resuid;           /* Default uid for reserved blocks */
   uint16_t  s_def_resgid;           /* Default gid for reserved blocks */
   /*
    * These fields are for EXT2_DYNAMIC_REV superblocks only.
    *
    * Note: the difference between the compatible feature set and
    * the incompatible feature set is that if there is a bit set
    * in the incompatible feature set that the kernel doesn't
    * know about, it should refuse to mount the filesystem.
    *
    * e2fsck's requirements are more strict; if it doesn't know
    * about a feature in either the compatible or incompatible
    * feature set, it must abort and not try to meddle with
    * things it doesn't understand...
    */
   uint32_t  s_first_ino;         /* First non-reserved inode */
   uint16_t  s_inode_size;           /* size of inode structure */
   uint16_t  s_block_group_nr;       /* block group # of this superblock */
   uint32_t  s_feature_compat;    /* compatible feature set */
   uint32_t  s_feature_incompat;  /* incompatible feature set */
   uint32_t  s_feature_ro_compat; /* readonly-compatible feature set */
   uint8_t   s_uuid[16];             /* 128-bit uuid for volume */
   char   s_volume_name[16];      /* volume name */
   char   s_last_mounted[64];     /* directory where last mounted */
   uint32_t  s_algorithm_usage_bitmap; /* For compression */
   /*
    * Performance hints.  Directory preallocation should only
    * happen if the EXT2_COMPAT_PREALLOC flag is on.
    */
   uint8_t    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
   uint8_t    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
   uint16_t   s_padding1;
   /*
    * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
    */
   uint8_t    s_journal_uuid[16];     /* uuid of journal superblock */
   uint32_t   s_journal_inum;      /* inode number of journal file */
   uint32_t   s_journal_dev;       /* device number of journal file */
   uint32_t   s_last_orphan;       /* start of list of inodes to delete */
   uint32_t   s_hash_seed[4];      /* HTREE hash seed */
   uint8_t    s_def_hash_version;     /* Default hash version to use */
   uint8_t    s_reserved_char_pad;
   uint16_t   s_reserved_word_pad;
   uint32_t   s_default_mount_opts;
   uint32_t   s_first_meta_bg;     /* First metablock block group */
   uint32_t   s_reserved[190];     /* Padding to the end of the block */
};

/** \brief ext2 group descriptor
 **
 ** ext2 file system block group descriptor
 **
 **/
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 */
struct ext2_gd {
    uint32_t  block_bitmap;     /* Blocks bitmap block */
    uint32_t  inode_bitmap;     /* Inodes bitmap block */
    uint32_t  inode_table;      /* Inodes table block */
    uint16_t  free_blocks_count;   /* Free blocks count */
    uint16_t  free_inodes_count;   /* Free inodes count */
    uint16_t  used_dirs_count;     /* Directories count */
    uint16_t  pad;
    uint32_t  reserved[3];
};

/** \brief ext2 directory information
 **
 ** ext2 file system directory descriptor
 **
 **/
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 */
struct   ext2fs_direct {
   uint32_t e2d_ino;      /* inode number of entry */
   uint16_t e2d_reclen;   /* length of this record */
   uint8_t e2d_namlen;      /* length of string in d_name */
   uint8_t e2d_type;      /* file type */
   char e2d_name[EXT2FS_MAXNAMLEN];/* name with length<=EXT2FS_MAXNAMLEN */
};

/** \brief ext2 node structure
 **
 ** ext2 node structure
 **
 **/
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 */
struct ext2fs_dinode {
   uint16_t   i_mode;   /*   0: IFMT, permissions; see below. */   //0
   uint16_t   i_uid;   /*   2: Owner UID */            //2
   uint32_t   i_size;   /*    4: Size (in bytes) */         //4
   uint32_t   i_atime;   /*    8: Acces time */      //8
   uint32_t   i_ctime;   /*   12: Create time */      //12
   uint32_t   i_mtime;   /*   16: Modification time */   //16
   uint32_t   i_dtime;   /*   20: Deletion time */      
   uint16_t   i_gid;   /*  24: Owner GID */
   uint16_t   i_links_count;   /*  26: File link count */
   uint32_t   i_blocks;   /*  28: Blocks count */
   uint32_t   i_flags;   /*  32: Status flags (chflags) */
   union {
      struct {
            uint32_t  l_i_reserved1;
      } linux1;
      struct {
            uint32_t  h_i_translator;
      } hurd1;
      struct {
            uint32_t  m_i_reserved1;
      } masix1;
   } osd1;
   uint32_t   i_block[N_DIRECT_BLOCKS + N_INDIRECT_BLOCKS]; /* 40: disk blocks */
   uint32_t   i_gen;   /* 100: generation number */
   uint32_t   i_facl;   /* 104: file ACL (not implemented) */
   uint32_t   i_dacl;   /* 108: dir ACL (not implemented) */
   uint32_t   i_faddr;   /* 112: fragment address */
    union {
        struct {
            uint8_t    l_i_frag;       // Fragment number /
            uint8_t    l_i_fsize;      // Fragment size /
            uint16_t   i_pad1;
            uint16_t  l_i_uid_high;   // these 2 fields    /
            uint16_t  l_i_gid_high;   // were reserved2[0] /
            uint32_t   l_i_reserved2;
        } linux2;
        struct {
            uint8_t    h_i_frag;       // Fragment number /
            uint8_t    h_i_fsize;      // Fragment size /
            uint16_t  h_i_mode_high;
            uint16_t  h_i_uid_high;
            uint16_t  h_i_gid_high;
            uint32_t  h_i_author;
        } hurd2;
        struct {
            uint8_t    m_i_frag;       // Fragment number /
            uint8_t    m_i_fsize;      // Fragment size /
            uint16_t   m_pad1;
            uint32_t   m_i_reserved2[2];
        } masix2;
    } osd2;                         // OS dependent 2 /
};

/** \brief ext2 file system
 **
 ** ext2 file system information
 **
 **/
typedef struct ext2_fs_info {
   struct ext2sb e2sb;
   struct   ext2_gd *e2fs_gd; /* group descripors */
   /* The following items are only used when the super_block is in memory. */
   int32_t s_bshift;   /* ``lblkno'' calc of logical blkno */
   int32_t s_bmask;   /* ``blkoff'' calc of blk offsets */
   uint32_t   s_block_size;           /* block size in bytes. */
   uint32_t   s_inodes_per_block;     /* Number of inodes per block */
   uint32_t   s_itb_per_group;        /* Number of inode table blocks per group */
   uint32_t   s_ginfodb_count;            /* Number of group descriptor blocks */
   uint32_t   s_desc_per_block;       /* Number of group descriptors per block */
   uint32_t   s_groups_count;         /* Number of groups in the fs */
   uint8_t    sectors_in_block;       /*Sector is 512 bytes long*/
} ext2_fs_info_t;

/** \brief ext2 file info
 **
 ** ext2 file information
 **
 **/
typedef struct ext2_file_info {
   struct ext2fs_dinode   f_di;      /* copy of on-disk inode */
   uint16_t      f_group;     /*Block group number in which the node resides*/
   long      f_pointer;   /* local seek pointer */
   uint32_t f_inumber;                /* inode number on its (minor) device */

} ext2_file_info_t;
/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

#endif /* EXT_H_ */

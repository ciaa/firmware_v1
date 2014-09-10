/**HEADER********************************************************************
 Copyright (c) 2012-2013 Freescale Semiconductor, Inc.
 ALL RIGHTS RESERVED.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, 
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************
*                                                                           
*        Standard Software Flash Driver For FTFx                            
*                                                                           
* FILE NAME     :  SSD_FTFx.h                                               
* DATE          :  Dec 25, 2013                                             
*                                                                           
* AUTHOR        :  FPT Team                                                 
* E-mail        :  r56611@freescale.com                                     
*                                                                           
******************************************************************************
0.0.1       06.20.2012      FPT Team      Initial Version
0.1.0       03.16.2013      FPT Team      Update to support K20-512, K20-1M, L4K, LOPA,
                                          L2KM and MCF51JG derivatives  
                                          Remove EERAMBlockSize field in ssdConfig
0.1.1       06.10.2013      FPT Team      Add to include "user_config.h" file  
0.1.2       06.11.2013      FPT Team      Add derivative FTFx_JX_32K_32K_2K_1K_1K       
0.1.3       06.20.2013      FPT Team      Add derivative FTFx_KX_256K_0K_4K_2K_0K
                                          Remove derivative FTFx_KX_512K_0K_0K_2K_0K
                                          Change derivative name as below:
                                          - from FTFx_KX_32K_0K_0K_1K_0K to FTFx_KX_32K_0K_2K_1K_0K
                                          - from FTFx_KX_64K_0K_0K_1K_0K to FTFx_KX_64K_0K_2K_1K_0K
                                          - from FTFx_KX_128K_0K_0K_1K_0K to FTFx_KX_128K_0K_2K_1K_0K
                                          Remove compiler definition CW and IAR
                                          Add derivative for LKM family: FTFx_MX_64K_0K_0K_1K_0K
                                          and FTFx_MX_128K_0K_0K_1K_0K
                                          Remove FTFx_FX_256K_32K_2K_1K_1K
                                          Change FTFx_JX_xxx to FTFx_CX_xxx for coldfire core.
1.0.0       12.25.2013      FPT Team      Add derivative FTFx_KX_512K_0K_0K_2K_0K
                                          Add derivative FTFx_KX_256K_0K_0K_2K_0K
*END*************************************************************************/
#ifndef _SSD_FTFx_COMMON_H_
#define _SSD_FTFx_COMMON_H_

#include "SSD_Types.h"

/*------------------------- Configuration Macros -----------------------*/
/* Define derivatives with rule: FTFx_AA_BB_CC_DD_EE_FF 
AA: MCU type
BB: P-Flash block size
CC: FlexNVM block size
DD: FlexRAM/AccRam size
EE: P-Flash sector size
FF: D-Flash sector size */

#define FTFx_KX_256K_256K_4K_2K_2K          1          /* Kinetis - K40, K60 ARM Cortex M4 core */
#define FTFx_KX_512K_0K_4K_2K_0K            2          /* Kinetis - K20, K40, K60 ARM Cortex M4 core */
#define FTFx_KX_512K_512K_16K_4K_4K         3          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_1024K_0K_16K_4K_0K          4          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_32K_0K_2K_1K_0K             5          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_32K_32K_2K_1K_1K            6          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_64K_0K_2K_1K_0K             7          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_64K_32K_2K_1K_1K            8          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_128K_0K_2K_1K_0K            9          /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_128K_32K_2K_1K_1K           10         /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_64K_32K_2K_2K_1K            11         /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_128K_32K_2K_2K_1K           12         /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_256K_32K_2K_2K_1K           13         /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_1024K_0K_4K_4K_0K           14         /* Kinetis - ARM Cortex M4 core - K20 1M*/
#define FTFx_KX_512K_128K_4K_4K_4K          15         /* Kinetis - ARM Cortex M4 core - K20 1M - 512*/
#define FTFx_KX_256K_64K_4K_2K_2K           16         /* Kinetis - ARM Cortex M4 core - K20 256K*/
#define FTFx_KX_128K_64K_4K_2K_2K           17         /* Kinetis - ARM Cortex M4 core - K20 128K*/
#define FTFx_KX_256K_0K_4K_2K_0K            18         /* Kinetis - K60 ARM Cortex M4 core */
#define FTFx_KX_128K_128K_4K_2K_2K          19         /* Kinetis -  ARM Cortex M4 core */
#define FTFx_NX_256K_32K_2K_2K_1K           20         /* Nevis2 - 56800EX 32 bit DSC core */
#define FTFx_NX_128K_32K_2K_2K_1K           21         /* Nevis2 - 56800EX 32 bit DSC core */
#define FTFx_NX_96K_32K_2K_2K_1K            22         /* Nevis2 - 56800EX 32 bit DSC core */
#define FTFx_NX_64K_32K_2K_2K_1K            23         /* Nevis2 - 56800EX 32 bit DSC core */
#define FTFx_LX_128K_0K_0K_1K_0K            24         /* L2K -  ARM Cortex M0 core */ /* L2KM, L4K*/
#define FTFx_LX_64K_0K_0K_1K_0K             25         /* L2K -  ARM Cortex M0 core */
#define FTFx_LX_32K_0K_0K_1K_0K             26         /* L2K & L1PT -  ARM Cortex M0 core */ /* LOPA */
#define FTFx_LX_16K_0K_0K_1K_0K             27         /* L1PT -  ARM Cortex M0 core */ /* LOPA */
#define FTFx_LX_8K_0K_0K_1K_0K              28         /* L1PT -  ARM Cortex M0 core */ /* LOPA */
#define FTFx_LX_256K_0K_0K_1K_0K            29         /* L4K -  ARM Cortex M0 core */
#define FTFx_AX_64K_0K_0K_1K_0K             30         /* Anguilla_Silver - 56800EX 32 bit DSC core */
#define FTFx_AX_48K_0K_0K_1K_0K             31         /* Anguilla_Silver - 56800EX 32 bit DSC core */
#define FTFx_AX_32K_0K_0K_1K_0K             32         /* Anguilla_Silver - 56800EX 32 bit DSC core */
#define FTFx_AX_16K_0K_0K_1K_0K             33         /* Anguilla_Silver - 56800EX 32 bit DSC core */
#define FTFx_CX_128K_32K_2K_1K_1K           34         /* ColdFire core, MCF51JF/JU 128K */
#define FTFx_CX_64K_32K_2K_1K_1K            35         /* ColdFire core, MCF51JF/JU 64K */
#define FTFx_CX_32K_32K_2K_1K_1K            36         /* ColdFire core, MCF51JF/JU 32K */
#define FTFx_CX_256K_32K_2K_1K_1K           37         /* ColdFire core, MCF51JG256 , MCF51FD256*/
#define FTFx_MX_64K_0K_0K_1K_0K             38         /* LKM34 */
#define FTFx_MX_128K_0K_0K_1K_0K            39         /* LKM34 */
#define FTFx_KX_512K_0K_0K_2K_0K            40         /* Senna K22FN512 */
#define FTFx_KX_256K_0K_0K_2K_0K            41         /* Senna K22FN256 */

/* Endianness */
#define BIG_ENDIAN                        0   /* Big Endian */
#define LITTLE_ENDIAN                     1   /* Little Endian */

/* cpu cores */
#define COLDFIRE                          0   /* ColdFire core */
#define ARM_CM4                           1   /* ARM Cortex M4 core */
#define DSC_56800EX                       2   /* 32 bit DSC core */
#define ARM_CM0PLUS                       3   /* ARM Cortex M0 core */


/* Word size */
#define FTFx_WORD_SIZE                    0x0002     /* 2 bytes */
  
/* Longword size */
#define FTFx_LONGWORD_SIZE                0x0004     /* 4 bytes */

/* Phrase size */
#define FTFx_PHRASE_SIZE                  0x0008     /* 8 bytes */

/* Double-phrase size */
#define FTFx_DPHRASE_SIZE                 0x0010     /* 16 bytes */

/*------------ Return Code Definition for FTFx SSD ---------------------*/
#define FTFx_OK                            0x0000

#define FTFx_ERR_MGSTAT0                   0x0001
#define FTFx_ERR_PVIOL                     0x0010
#define FTFx_ERR_ACCERR                    0x0020

#define FTFx_ERR_CHANGEPROT                0x0100
#define FTFx_ERR_NOEEE                     0x0200
#define FTFx_ERR_EFLASHONLY                0x0400
#define FTFx_ERR_RAMRDY                    0x0800
#define FTFx_ERR_RANGE                     0x1000
#define FTFx_ERR_SIZE                      0x2000 


/* Flash security status */
#define FLASH_SECURITY_STATE_KEYEN         0x80
#define FLASH_SECURITY_STATE_UNSECURED     0x02

#define FLASH_NOT_SECURE                   0x01
#define FLASH_SECURE_BACKDOOR_ENABLED      0x02
#define FLASH_SECURE_BACKDOOR_DISABLED     0x04

/* macro for flash configuration field offset */
#define FLASH_CNFG_START_ADDRESS           0x400
#define FLASH_CNFG_END_ADDRESS             0x40F


/*-------------- Read/Write/Set/Clear Operation Macros -----------------*/
#define REG_BIT_SET(address, mask)      (*(VUINT8*)(address) |= (mask))
#define REG_BIT_CLEAR(address, mask)    (*(VUINT8*)(address) &= ~(mask))
#define REG_BIT_GET(address, mask)      (*(VUINT8 *)(address) & (UINT8)(mask))
#define REG_WRITE(address, value)       (*(VUINT8*)(address) = (value))
#define REG_READ(address)               ((UINT8)(*(VUINT8*)(address)))
#define REG_WRITE32(address, value)     (*(VUINT32*)(address) = (value))
#define REG_READ32(address)             ((UINT32)(*(VUINT32*)(address)))

#define WRITE8(address, value)          (*(VUINT8*)(address) = (value))
#define READ8(address)                  ((UINT8)(*(VUINT8*)(address)))
#define SET8(address, value)            (*(VUINT8*)(address) |= (value))
#define CLEAR8(address, value)          (*(VUINT8*)(address) &= ~(value))
#define TEST8(address, value)           (*(VUINT8*)(address) & (value))

#define WRITE16(address, value)         (*(VUINT16*)(address) = (value))
#define READ16(address)                 ((UINT16)(*(VUINT16*)(address)))
#define SET16(address, value)           (*(VUINT16*)(address) |= (value))
#define CLEAR16(address, value)         (*(VUINT16*)(address) &= ~(value))
#define TEST16(address, value)          (*(VUINT16*)(address) & (value))

#define WRITE32(address, value)         (*(VUINT32*)(address) = (value))
#define READ32(address)                 ((UINT32)(*(VUINT32*)(address)))
#define SET32(address, value)           (*(VUINT32*)(address) |= (value))
#define CLEAR32(address, value)         (*(VUINT32*)(address) &= ~(value))
#define TEST32(address, value)          (*(VUINT32*)(address) & (value))

#define GET_BIT_0_7(value)              ((UINT8)((value) & 0xFF))
#define GET_BIT_8_15(value)             ((UINT8)(((value)>>8) & 0xFF))
#define GET_BIT_16_23(value)            ((UINT8)(((value)>>16) & 0xFF))
#define GET_BIT_24_31(value)            ((UINT8)((value)>>24))
/*--------------------- CallBack function period -----------------------*/
#define FLASH_CALLBACK_CS               10          /* Check Sum */
/*--------------------Null Callback function defination ----------------*/
#define NULL_CALLBACK                   ((PCALLBACK)0xFFFFFFFF)
#define NULL_SWAP_CALLBACK              ((PFLASH_SWAP_CALLBACK)0xFFFFFFFF)       

/*------------------- Setting flash interrupt macro --------------------*/
#define SET_FLASH_INT_BITS(ftfxRegBase, value)  REG_WRITE((ftfxRegBase) + FTFx_SSD_FCNFG_OFFSET,\
                                                ((value)&(FTFx_SSD_FCNFG_CCIE | FTFx_SSD_FCNFG_RDCOLLIE)))
#define GET_FLASH_INT_BITS(ftfxRegBase)         REG_READ((ftfxRegBase) + FTFx_SSD_FCNFG_OFFSET) &\
                                                (FTFx_SSD_FCNFG_CCIE | FTFx_SSD_FCNFG_RDCOLLIE)
                                      
#endif /* _SSD_FTFx_COMMON_H_ */


/**HEADER********************************************************************
 Copyright (c) 2013 Freescale Semiconductor, Inc.
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
* FILE NAME     :  FTFx_KX_512K_128K_4K_4K_4K.h                         
* DATE          :  Dec 25, 2013                                         
*                                                                       
* AUTHOR        :  FPT Team                                             
* E-mail        :  r56611@freescale.com                                 
*                                                                       
******************************************************************************
0.1.0       03.16.2013      FPT Team      Initial Version
0.1.1       06.05.2013      FPT Team      Add define FTFE
                                          Correct RDRSRC_SIZE = 8 (instead of 4).
0.1.2       06.20.2013      FPT Team      Remove declaration for swap functions since this 
                                          derivative has no swap.    
                                          Update function prototype of 
                                          FlashProgramCheck by removing pFailData        
1.0.0       12.25.2013      FPT Team      Modify define macros                                          
*END*************************************************************************/

#ifndef _FTFx_KX_512K_128K_4K_4K_4K_H_
#define _FTFx_KX_512K_128K_4K_4K_4K_H_

#include "SSD_Types.h"
#include "SSD_FTFx_Common.h"

#define FTFE_M /* this is FTFE module */

#define BYTE2WORD(x) (x)
#define WORD2BYTE(x) (x)

/* Endianness */
#define ENDIANNESS              LITTLE_ENDIAN

/* cpu core */
#define CPU_CORE                ARM_CM4            

/* PFlash sector size */ 
#define FTFx_PSECTOR_SIZE       0x00001000      /* 4 KB size */
/* DFlash sector size */ 
#define FTFx_DSECTOR_SIZE       0x00001000      /* 4 KB size */
#define DEBLOCK_SIZE            0x00020000      /* 128 KB size */

/* EEE Data Set Size Field Description */
#define EEESIZE_0000            0xFFFFFFFF      /* Reserved */
#define EEESIZE_0001            0xFFFFFFFF      /* Reserved */
#define EEESIZE_0010            0x00001000      /* 4096 bytes */
#define EEESIZE_0011            0x00000800      /* 2048 bytes */
#define EEESIZE_0100            0x00000400      /* 1024 bytes */
#define EEESIZE_0101            0x00000200      /* 512 bytes */
#define EEESIZE_0110            0x00000100      /* 256 bytes */
#define EEESIZE_0111            0x00000080      /* 128 bytes */
#define EEESIZE_1000            0x00000040      /* 64 bytes */
#define EEESIZE_1001            0x00000020      /* 32 bytes */
#define EEESIZE_1010            0xFFFFFFFF      /* Reserved */
#define EEESIZE_1011            0xFFFFFFFF      /* Reserved */
#define EEESIZE_1100            0xFFFFFFFF      /* Reserved */
#define EEESIZE_1101            0xFFFFFFFF      /* Reserved */
#define EEESIZE_1110            0xFFFFFFFF      /* Reserved */
#define EEESIZE_1111            0x00000000      /* 0 byte */

/* D/E-Flash Partition Code Field Description */
#define DEPART_0000             0x00020000      /* 128 KB */
#define DEPART_0001             0xFFFFFFFF      /* Reserved */
#define DEPART_0010             0xFFFFFFFF      /* Reserved */
#define DEPART_0011             0x00018000      /* 96 KB */
#define DEPART_0100             0x00010000      /* 64 KB */
#define DEPART_0101             0x00000000      /* 0 KB */
#define DEPART_0110             0xFFFFFFFF      /* Reserved */
#define DEPART_0111             0xFFFFFFFF      /* Reserved */
#define DEPART_1000             0x00000000      /* 0 KB */
#define DEPART_1001             0xFFFFFFFF      /* Reserved */
#define DEPART_1010             0xFFFFFFFF      /* Reserved */
#define DEPART_1011             0x00008000      /* 32 KB */
#define DEPART_1100             0x00010000      /* 64 KB */
#define DEPART_1101             0x00020000      /* 128 KB */
#define DEPART_1110             0xFFFFFFFF      /* Reserved */
#define DEPART_1111             0x00020000      /* 128 KB */

/* Address offset (compare to start addr of P/D flash) and size of PFlash IFR and DFlash IFR */
#define PFLASH_IFR_OFFSET       0x00000000
#define PFLASH_IFR_SIZE         0x00000400
#define DFLASH_IFR_OFFSET       0x00000000
#define DFLASH_IFR_SIZE         0x00000400

/*destination to read Dflash IFR area*/
#define DFLASH_IFR_READRESOURCE_ADDRESS       0x8003F8  

/* Size for checking alignment of a section */
#define PERSBLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase block on PFlash function */
#define DERSBLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase block on DFlash function */

#define PERSSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase sector on PFlash function */
#define DERSSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of erase sector on DFlash function */

#define PPGMSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of program section on PFlash function */
#define DPGMSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of program section on DFlash function */

#define PRD1BLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify block on PFlash function */
#define DRD1BLK_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify block on DFlash function */

#define PRD1SEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify section on PFlash function */
#define DRD1SEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify section on DFlash function */

#define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
#define RDRSRC_ALIGN_SIZE       FTFx_PHRASE_SIZE        /* check align of read resource function */
#define SWAP_ALIGN_SIZE         FTFx_DPHRASE_SIZE       /* check align of swap function*/

#define PGM_SIZE_BYTE           FTFx_PHRASE_SIZE

                     
#define RESUME_WAIT_CNT         0x20
/*---------------- Function Prototypes for Flash SSD --------------------*/

/* Flash initialization */
extern UINT32 FlashInit(PFLASH_SSD_CONFIG PSSDConfig);

/* Internal function. Called by driver APIs only */
/* Flash command sequence */
extern UINT32 FlashCommandSequence(PFLASH_SSD_CONFIG pSSDConfig);

/* P-Flash get protection */
extern UINT32 PFlashGetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32*  protectStatus);      

/* P-Flash set protection */
extern UINT32 PFlashSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32  protectStatus);                                                
 
 
/* Flash get security state */
extern UINT32 FlashGetSecurityState(PFLASH_SSD_CONFIG PSSDConfig, \
                                    UINT8* securityState);

/* Flash security bypass */
extern UINT32 FlashSecurityBypass(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8* keyBuffer, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase all Block */
extern UINT32 FlashEraseAllBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                 pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                
/* Flash erase Block */
extern UINT32 FlashEraseBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                    
/* Flash erase sector */
extern UINT32 FlashEraseSector(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT32 destination, \
                               UINT32 size, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase suspend */
extern UINT32 FlashEraseSuspend(PFLASH_SSD_CONFIG PSSDConfig);

/* Flash erase resume */
extern UINT32 FlashEraseResume(PFLASH_SSD_CONFIG PSSDConfig);
                                                
/* Flash program Section */
extern UINT32 FlashProgramSection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32 destination, \
                                  UINT16 Number, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                                
   
/* Flash checksum */
extern UINT32 FlashCheckSum(PFLASH_SSD_CONFIG PSSDConfig, \
                            UINT32 destination, \
                            UINT32 size, \
                            UINT32* PSum);
                                           
/* Flash verify all block */
extern UINT32 FlashVerifyAllBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8 marginLevel, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify block */
extern UINT32 FlashVerifyBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT32 destination, \
                               UINT8 marginLevel, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify section*/
extern UINT32 FlashVerifySection(PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT32 destination, \
                                 UINT16 Number, \
                                 UINT8 marginLevel, \
                                 pFLASHCOMMANDSEQUENCE FlashCommandSequence);        

/* Flash read once */
extern UINT32 FlashReadOnce(PFLASH_SSD_CONFIG PSSDConfig, \
                            UINT8 recordIndex,\
                            UINT8* pDataArray, \
                            pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash program once */
extern UINT32 FlashProgramOnce(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT8 recordIndex,\
                               UINT8* pDataArray, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash program check */
extern UINT32 FlashProgramCheck(PFLASH_SSD_CONFIG PSSDConfig, \
                                UINT32  destination, \
                                UINT32  size, \
                                UINT8*  pExpectedData, \
                                UINT32* pFailAddr, \
                                UINT8   marginLevel, \
                                pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                        
/* Flash read resource */
extern UINT32 FlashReadResource(PFLASH_SSD_CONFIG PSSDConfig, \
                                UINT32 destination, \
                                UINT8* pDataArray, \
                                UINT8  resourceSelectCode, \
                                pFLASHCOMMANDSEQUENCE FlashCommandSequence); 

/* Flash program phrase */
extern UINT32 FlashProgram(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   UINT32 size, \
                                   UINT8* pData, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence);


/* D-Flash get protection */
extern UINT32 DFlashGetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8*  protectStatus);      

/* D-Flash set protection */
extern UINT32 DFlashSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8  protectStatus);
                                                
/* EERAM get protection */
extern UINT32 EERAMGetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT8*  protectStatus);      

/* EERAM set protection */
extern UINT32 EERAMSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT8  protectStatus);    
/* Flash D/E-Flash Partition */
extern UINT32 DEFlashPartition(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT8 EEEDataSizeCode, \
                               UINT8 DEPartitionCode, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash Set EEEEnable */
extern UINT32 SetEEEEnable(PFLASH_SSD_CONFIG PSSDConfig, \
                           UINT8 EEEEnable,pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* EEPROM Emulator Write */
extern UINT32 EEEWrite(PFLASH_SSD_CONFIG PSSDConfig, \
                       UINT32 destination, \
                       UINT32 size, \
                       UINT8* pData);
                       

#endif  /* _FTFx_KX_512K_128K_4K_4K_4K_H_ */

/**HEADER********************************************************************
 Copyright (c) 2010-2014 Freescale Semiconductor, Inc.
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
* FILE NAME     :  SSD_Types.h                                              
* DATE          :  Dec 25, 2013                                             
*                                                                           
* AUTHOR        :  FPT Team                                                 
* E-mail        :  r56611@freescale.com                                     
*                                                                           
*****************************************************************************
0.0.1       06.09.2010      FPT Team        Initial Version
1.0.0       12.25.2013      FPT Team        Finalize to 1.0.0
1.0.1       01.16.2014      FPT Team        Change protype of PFlashSwapCtl function    
*END*************************************************************************/

#ifndef _SSD_TYPES_H_
#define _SSD_TYPES_H_

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned char BOOL;

typedef signed char INT8;
typedef unsigned char UINT8;
typedef volatile signed char VINT8;
typedef volatile unsigned char VUINT8;

typedef signed short INT16;
typedef unsigned short UINT16;
typedef volatile signed short VINT16;
typedef volatile unsigned short VUINT16;

typedef signed long INT32;
typedef unsigned long UINT32;
typedef volatile signed long VINT32;
typedef volatile unsigned long VUINT32;



/*-------------------- Callback function prototype ---------------------*/
typedef void (* PCALLBACK)(void);
typedef BOOL (* PFLASH_SWAP_CALLBACK)(UINT8);


/*---------------- Flash SSD Configuration Structure -------------------*/
typedef struct _ssd_config
{
    UINT32      ftfxRegBase;        /* FTFx control register base */
    UINT32      PFlashBlockBase;    /* base address of PFlash block */
    UINT32      PFlashBlockSize;    /* size of PFlash block */
    UINT32      DFlashBlockBase;    /* base address of DFlash block */
    UINT32      DFlashBlockSize;    /* size of DFlash block */
    UINT32      EERAMBlockBase;     /* base address of EERAM block */
    UINT32      EEEBlockSize;       /* size of EEE block */
    BOOL        DebugEnable;        /* background debug mode enable bit */
    PCALLBACK   CallBack;           /* pointer to callback function */
} FLASH_SSD_CONFIG, *PFLASH_SSD_CONFIG;

/* -------------------- Function Pointer ------------------------------- */
typedef UINT32 (*pFLASHCOMMANDSEQUENCE) (PFLASH_SSD_CONFIG PSSDConfig);

/* Flash initialization */
typedef UINT32 (*pFLASHINIT) (PFLASH_SSD_CONFIG PSSDConfig);

/* P-Flash get protection */
typedef UINT32 (*pPFLASHGETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT32*  protectStatus);
                                              
/* P-Flash set protection */
typedef UINT32 (*pPFLASHSETPROTECTION) (PFLASH_SSD_CONFIG  PSSDConfig, \
                                        UINT32  protectStatus);
                                                        
/* Flash get security state */
typedef UINT32 (*pFLASHGETSECURITYSTATE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                          UINT8* securityState);
                                                        
/* Flash security bypass */
typedef UINT32 (*pFLASHSECURITYBYPASS) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8* keyBuffer, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase all Block */
typedef UINT32 (*pFLASHERASEALLBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                
/* Flash erase Block */
typedef UINT32 (*pFLASHERASEBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                    UINT32 destination, \
                                    pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                    
/* Flash erase sector */
typedef UINT32 (*pFLASHERASESECTOR) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT32 destination, \
                                     UINT32 size, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase suspend */
typedef UINT32 (*pFLASHERASESUSPEND) (PFLASH_SSD_CONFIG PSSDConfig);

/* Flash erase resume */
typedef UINT32 (*pFLASHERASERESUME) (PFLASH_SSD_CONFIG PSSDConfig);                                                       
    
/* Flash program Section */
typedef UINT32 (*pFLASHPROGRAMSECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT32 destination, \
                                        UINT16 Number, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                                
                                   
/* Flash checksum */
typedef UINT32 (*pFLASHCHECKSUM) (PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32 destination, \
                                  UINT32 size, \
                                  UINT32* PSum);
                                           
/* Flash verify all block */
typedef UINT32 (*pFLASHVERIFYALLBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8 marginLevel, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify block */
typedef UINT32 (*pFLASHVERIFYBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT32 destination, \
                                     UINT8 marginLevel, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify section*/
typedef UINT32 (*pFLASHVERIFYSECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT32 destination, \
                                       UINT16 Number, \
                                       UINT8 marginLevel, \
                                       pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash read once */
typedef UINT32 (*pFLASHREADONCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8* pDataArray, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash program once */
typedef UINT32 (*pFLASHPROGRAMONCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT8* pDataArray, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash program check */
typedef UINT32 (*pFLASHPROGRAMCHECK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT32  destination, \
                                      UINT32  size, \
                                      UINT8*  pExpectedData, \
                                      UINT32* pFailAddr, \
                                      UINT8   marginLevel, \
                                      pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                                      
/* Flash read resource */
typedef UINT32 (*pFLASHREADRESOURCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT32 destination, \
                                      UINT8* pDataArray, \
                                      UINT8  resourceSelectCode, \
                                      pFLASHCOMMANDSEQUENCE FlashCommandSequence);              

/* Flash program Phrase */
typedef UINT32 (*pFLASHPROGRAM) (PFLASH_SSD_CONFIG PSSDConfig, \
                                         UINT32 destination, \
                                         UINT32 size, \
                                         UINT8* pData, \
                                         pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                       

/* PFlash Get swap status */
typedef UINT32 (*pPFLASHSWAPCTL) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT32 addr, \
                                        UINT8  swapcmd, \
                                        UINT8* pCurrentSwapMode,\
                                        UINT8* pCurrentSwapBlockStatus, \
                                        UINT8* pNextSwapBlockStatus, \
                                        pFLASHCOMMANDSEQUENCE pFlashCommandSequence);
                                        

/* Swap the two P-Flash blocks with each other */
typedef UINT32 (*pPFLASHSWAP)(PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 flashAddress, \
                             PFLASH_SWAP_CALLBACK pSwapCallback, \
                             pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                             
/* D-Flash get protection */
typedef UINT32 (*pDFLASHGETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8*  protectStatus);    
/* D-Flash set protection */
typedef UINT32 (*pDFLASHSETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8  protectStatus);
                                    
/* EERAM get protection */
typedef UINT32 (*pEERAMGETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT8*  protectStatus);      

/* EERAM set protection */
typedef UINT32 (*pEERAMSETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT8  protectStatus);
/* Flash D/E-Flash Partition */
typedef UINT32 (*pDEFLASHPARTITION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT8 EEEDataSizeCode, \
                                     UINT8 DEPartitionCode, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash Set EEEEnable */
typedef UINT32 (*pSETEEEENABLE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT8 EEEEnable,pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                    

/* EEPROM Emulator Write */
typedef UINT32 (*pEEEWRITE) (PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 destination, \
                             UINT32 size, \
                             UINT8* pData);      
typedef void (*FLASHLAUNCHCOMMAND) (PFLASH_SSD_CONFIG PSSDConfig); 
#endif /* _SSD_TYPES_H_ */

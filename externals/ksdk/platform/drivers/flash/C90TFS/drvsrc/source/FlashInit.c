/**HEADER********************************************************************
 Copyright (c) 2010-2013 Freescale Semiconductor, Inc.
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
* FILE NAME     :  FlashInit.c                                               
* DATE          :  Dec 25, 2013                                              
*                                                                            
* AUTHOR        :  FPT Team                                                  
* E-mail        :  r56611@freescale.com                                      
*                                                                            
*****************************************************************************
0.0.1       06.09.2010      FPT Team      Initial Version
1.0.0       12.25.2013      FPT Team      Optimize code 
*END*************************************************************************/
/* include the header files */
#include "SSD_FTFx.h"

/************************************************************************
*
*  Function Name    : FlashInit.c
*  Description      : Initialize the Flash memory
*  Arguments        : PFLASH_SSD_CONFIG
*  Return Value     : UINT32
*
*************************************************************************/
UINT32 FlashInit (PFLASH_SSD_CONFIG pSSDConfig)
{
#if (DEBLOCK_SIZE != 0)
    UINT8  EEEDataSetSize;    /* store EEE Data Set Size */
    UINT8  DEPartitionCode;    /* store D/E-Flash Partition Code */  
    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear*/
    REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET,FTFx_SSD_FSTAT_ERROR_BITS);   
   
    /* Write Command Code to FCCOB0 */
    REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_READ_RESOURCE);
    /* Write address to FCCOB1/2/3 */
    REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, GET_BIT_16_23(DFLASH_IFR_READRESOURCE_ADDRESS));
    REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, GET_BIT_8_15(DFLASH_IFR_READRESOURCE_ADDRESS));
    REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, GET_BIT_0_7(DFLASH_IFR_READRESOURCE_ADDRESS));
    
    /* Write Resource Select Code of 0 to FCCOB8 to select IFR. Without this, */
    /* an access error may occur if the register contains data from a previous command. */
    /* for FTFE module, resource code is FCCOB4. For others, recource code is FCCOB8 */
    REG_WRITE(pSSDConfig->ftfxRegBase + RSRC_CODE_OFSSET, 0);

    /* clear CCIF bit */
    REG_BIT_SET(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF);

    /* check CCIF bit */
    while((REG_BIT_GET(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)) == 0x0)
    {
        /* wait till CCIF bit is set */
    }
    /* read out EEdata set size and DEpartition code from FCCOBA, FCCOBB for FTFE module, from FCCOB6 and FCCOB7 for others */
    #ifdef FTFE  
        EEEDataSetSize = REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOBA_OFFSET);
        DEPartitionCode = REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOBB_OFFSET);
    #else
        EEEDataSetSize = REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB6_OFFSET);
        DEPartitionCode = REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB7_OFFSET);
    #endif   
    DEPartitionCode = DEPartitionCode & 0x0F;
    EEEDataSetSize = EEEDataSetSize & 0x0F;   
        /* Calculate D-Flash size and EEE size */
    if (0 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0000;
    else if (1 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0001;
    else if (2 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0010;
    else if (3 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0011;
    else if (4 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0100;
    else if (5 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0101;
    else if (6 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0110;
    else if (7 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_0111;
    else if (8 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1000;
    else if (9 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1001;
    else if (10 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1010;
    else if (11 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1011;
    else if (12 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1100;
    else if (13 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1101;
    else if (14 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1110;    
    else if (15 == DEPartitionCode) pSSDConfig->DFlashBlockSize = DEPART_1111;

    if (0 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0000;
    else if (1 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0001;
    else if (2 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0010;
    else if (3 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0011;
    else if (4 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0100;
    else if (5 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0101;
    else if (6 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0110;
    else if (7 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_0111;
    else if (8 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1000;
    else if (9 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1001;
    else if (10 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1010;
    else if (11 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1011;
    else if (12 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1100;
    else if (13 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1101;
    else if (14 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1110;    
    else if (15 == EEEDataSetSize) pSSDConfig->EEEBlockSize = EEESIZE_1111;       
    
#else /* DEBLOCK_SIZE == 0 */
    /* If size of D/E-Flash = 0 */
    pSSDConfig->DFlashBlockSize = 0;
    pSSDConfig->EEEBlockSize = 0;
#endif /* of DEBLOCK_SIZE */     
    /* Enter Debug state if enabled */
    if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

    return(FTFx_OK);
}
/* end of file */

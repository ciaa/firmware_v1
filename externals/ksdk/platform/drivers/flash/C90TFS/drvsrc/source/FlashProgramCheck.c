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
* FILE NAME     :  FlashProgramCheck.c                                       
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
*  Function Name    : FlashProgramCheck.c
*  Description      : The Program Check command tests a previously 
*                     programmed P-Flash or D-Flash longword to see 
*                     if it reads correctly at the specifed margin level.
*  Arguments        : PFLASH_SSD_CONFIG, UINT32,UINT32, UINT8*, UINT32*,
*                     UINT8*, UINT8, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/
UINT32 FlashProgramCheck(PFLASH_SSD_CONFIG pSSDConfig, \
                                                UINT32  dest, \
                                                UINT32  size, \
                                                UINT8*  pExpectedData, \
                                                UINT32* pFailAddr, \
                                                UINT8   marginLevel, \
                                                pFLASHCOMMANDSEQUENCE pFlashCommandSequence)                                               
{
 
    UINT32 ret;      /* return code variable */
    UINT32 offsetAddr ; /* offset addr to convert to internal memory addr */
    UINT8 i;
    /* convert to byte address */
    dest = WORD2BYTE(dest);
    if (size & (PGMCHK_ALIGN_SIZE - 1))
    {
        ret = FTFx_ERR_SIZE;
        goto EXIT;
    }
    /* check if the destination is aligned or not */
#if (DEBLOCK_SIZE)
    offsetAddr = WORD2BYTE(pSSDConfig->DFlashBlockBase);
    if((dest >= offsetAddr) && (dest < (offsetAddr + pSSDConfig->DFlashBlockSize)))
    {
        dest = dest - offsetAddr + 0x800000;
    }  
    else     
#endif
    {
        offsetAddr = WORD2BYTE(pSSDConfig->PFlashBlockBase);
        if((dest >= offsetAddr) && (dest < offsetAddr + pSSDConfig->PFlashBlockSize))    
        {
            dest -= offsetAddr;
        }
        else
        {
            ret = FTFx_ERR_ACCERR;
            goto EXIT;
        }
    }    
    while (size)
    {
        /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear*/
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET,FTFx_SSD_FSTAT_ERROR_BITS);   
        /* passing parameter to the command */
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_PROGRAM_CHECK);
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, GET_BIT_16_23(dest));
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, GET_BIT_8_15(dest));
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, GET_BIT_0_7(dest));
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB4_OFFSET, marginLevel);
        
        for (i = 0; i <  PGMCHK_ALIGN_SIZE; i++)
        {
             REG_WRITE(pSSDConfig->ftfxRegBase + (i+12), *(pExpectedData +i));
        }
        /* calling flash command sequence function to execute the command */
        ret = pFlashCommandSequence(pSSDConfig);        
    
        /* checking for the success of command execution */
        if(FTFx_OK != ret)
        {
#if (DEBLOCK_SIZE)
            if(dest >= 0x800000)
            {
                *pFailAddr = BYTE2WORD(dest + offsetAddr - 0x800000);
                goto EXIT;
            }
            else                        
#endif                 
            {
                *pFailAddr = BYTE2WORD(dest + offsetAddr);
                goto EXIT;
            }
        }
        size-=PGMCHK_ALIGN_SIZE;
        pExpectedData+=PGMCHK_ALIGN_SIZE;
        dest+= PGMCHK_ALIGN_SIZE;
    }

EXIT:
    /* Enter Debug state if enabled */
    if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

    return(ret);
}
/* end of file */



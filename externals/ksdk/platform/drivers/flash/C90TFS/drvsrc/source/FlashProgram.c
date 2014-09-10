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
* FILE NAME     :  FlashProgram.c                                            
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
*  Function Name    : FlashProgram.c
*  Description      : Program data into Flash
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, UINT32, UINT32,
*                     pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashProgram(PFLASH_SSD_CONFIG pSSDConfig, \
                                   UINT32 dest, \
                                   UINT32 size, \
                                   UINT8* pData, \
                                   pFLASHCOMMANDSEQUENCE pFlashCommandSequence)
{
    UINT32 ret;      /* return code variable */
    UINT8 i;
    UINT32 temp;
    ret = FTFx_OK;
    if (size & (PGM_SIZE_BYTE-1))
    {
       ret = FTFx_ERR_SIZE;
       goto EXIT;
    }
    /* convert to byte addr */
    dest = WORD2BYTE(dest);
#if (DEBLOCK_SIZE)
    temp = WORD2BYTE(pSSDConfig->DFlashBlockBase);
    if((dest >= temp) && (dest < (temp + pSSDConfig->DFlashBlockSize)))
    {
        dest = dest - temp + 0x800000;
    }  
    else     
#endif
    {
        temp = WORD2BYTE(pSSDConfig->PFlashBlockBase);
        if((dest >= temp) && (dest < (temp + pSSDConfig->PFlashBlockSize)))    
        {
            dest -= temp;
        }else{
            ret = FTFx_ERR_ACCERR;
            goto EXIT;
        }
    }
    while(size > 0)
    {
        /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear*/
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET,FTFx_SSD_FSTAT_ERROR_BITS);   
        /* passing parameter to the command */
#if (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE)
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_PROGRAM_PHRASE);
#else
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_PROGRAM_LONGWORD);
#endif        
        
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, GET_BIT_16_23(dest));
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, GET_BIT_8_15(dest));
        REG_WRITE(pSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, GET_BIT_0_7(dest));
        
        for (i = 0; i <  PGM_SIZE_BYTE; i++)
        {
             REG_WRITE(pSSDConfig->ftfxRegBase + (i+8), *(pData +i));
        }
 
        /* calling flash command sequence function to execute the command */
        ret = pFlashCommandSequence(pSSDConfig);        
            
        /* checking for the success of command execution */
        if(FTFx_OK != ret)
        {
            break;
        }
        else
        {
            /* update destination address for next iteration */
            dest += PGM_SIZE_BYTE;
            /* update size for next iteration */
            size -= PGM_SIZE_BYTE;
                    /* increment the source adress by 1 */
            pData += PGM_SIZE_BYTE;
        }
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



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
* FILE NAME     :  EEEWrite.c                                                
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
*  Function Name    : EEEWrite.c
*  Description      : This function is used to write data to EERAM 
*                     when it is used as EEPROM emulator              
*  Arguments        : PFLASH_SSD_CONFIG,UINT32 ,UINT32 , UINT8, UINT32
*  Return Value     : UINT32
*
************************************************************************/
#if (DEBLOCK_SIZE != 0)
UINT32 WaitEEWriteToFinish(PFLASH_SSD_CONFIG pSSDConfig, UINT32* dest, UINT32* size, UINT8** pData, UINT8 step)
{
    UINT32 ret;
    if (1 == step)
    {
        WRITE8(*dest, READ8(*pData));
    }
    if (2 == step)
    {
        WRITE16(BYTE2WORD(*dest), READ16_ADV(*pData));
    }
    if (4 == step)
    {
        WRITE32(BYTE2WORD(*dest), READ32_ADV(*pData));
    }

    while(0x0 == REG_BIT_GET(pSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY))
    {
       /* wait till EEERDY bit is set */
    }       
    /* Check for protection violation error */ 
    ret = REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET) & FTFx_SSD_FSTAT_FPVIOL;

    *dest += step;
    *size -= step;
    *pData += step;
    
    return ret;
}

UINT32 EEEWrite(PFLASH_SSD_CONFIG pSSDConfig, \
                           UINT32 dest, \
                           UINT32 size, \
                           UINT8* pData)
{
    UINT32 ret;           /* Return code variable */
    ret = FTFx_OK;
    /* convert to byte addr */
    dest = WORD2BYTE(dest);
    /* Check if EEE is enabled */
    if(REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET) & FTFx_SSD_FCNFG_EEERDY)
    {
        /* check range */
       if(dest < WORD2BYTE(pSSDConfig->EERAMBlockBase) || \
          ((dest + size) > (WORD2BYTE(pSSDConfig->EERAMBlockBase) + pSSDConfig->EEEBlockSize)))
        {
            ret = FTFx_ERR_RANGE;
            goto EXIT;
        }    

        while ((size > 0) && (ret == FTFx_OK))
        {        
            
            /* dest is 32bit-aligned and size is not less than 4 */
            if (!(dest & 0x03) &&(size >= 4))
            {
                ret = WaitEEWriteToFinish(pSSDConfig, &dest, &size, &pData, 4);
            } 
            else  if (!(dest & 0x1) &&(size >= 2))
            {
                ret = WaitEEWriteToFinish(pSSDConfig, &dest, &size, &pData, 2);
            } 
            else
            {
                ret = WaitEEWriteToFinish(pSSDConfig, &dest, &size, &pData, 1);
            }                
        }    
    }
    else
    {
        ret = FTFx_ERR_NOEEE;
        goto EXIT;
    }       
    
EXIT:
      /* Enter Debug state if enabled */
    if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }        
    return(ret);
}
#endif /* of DEBLOCK_SIZE */
/* end of file */

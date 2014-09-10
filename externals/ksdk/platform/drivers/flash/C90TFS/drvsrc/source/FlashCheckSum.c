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
* FILE NAME     :  FlashCheckSum.c                                           
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

/***********************************************************************
*
*  Function Name    : FlashCheckSum.c
*  Description      : This function is used to calculate checksum value 
*                     for the specified flash range.
*  Arguments        : PFLASH_SSD_CONFIG,UINT32 ,UINT32 ,UINT32*
*  Return Value     : UINT32
*
************************************************************************/
UINT32 FlashCheckSum(PFLASH_SSD_CONFIG pSSDConfig, \
                                UINT32 destination, \
                                UINT32 size, \
                                UINT32* PSum)
                              
{
    UINT32 counter;          /* Counter for callback operation */
    UINT32 data;             /* Data read from Flash address */
    UINT32 ret = FTFx_OK;       /* Return code variable */
    UINT32 endAddress;       /* P Flash end address */
    ret = FTFx_OK;
    counter = 0;
    /* convert to byte address */
    destination = WORD2BYTE(destination);
    /* calculating Flash end address */
    endAddress = destination + size;
    
    /* check for valid range of the target addresses */
    if ((destination < WORD2BYTE(pSSDConfig->PFlashBlockBase)) || 
        (endAddress > (WORD2BYTE(pSSDConfig->PFlashBlockBase) + pSSDConfig->PFlashBlockSize)))
    {
#if(DEBLOCK_SIZE)        
        if ((destination < WORD2BYTE(pSSDConfig->DFlashBlockBase)) || 
        (endAddress > (WORD2BYTE(pSSDConfig->DFlashBlockBase) + pSSDConfig->DFlashBlockSize)))
        {
#endif /* End of if(DEBLOCK_SIZE) */             
            /* return an error code FTFx_ERR_RANGE */
            ret = FTFx_ERR_RANGE;
            goto EXIT;
#if(DEBLOCK_SIZE)              
        }
        
#endif /* End of if(DEBLOCK_SIZE) */        
    }      
    *PSum = 0;
    /* doing sum operation */
    while(size > 0)
    {          
        data = READ8(destination);
        *PSum += (UINT32)data;
        destination += 1;
        size -= 1;

        /* Check if need to serve callback function */
        if((++counter) >= FLASH_CALLBACK_CS)
        {
            /* serve callback function if counter reaches limitation */
            if(NULL_CALLBACK != pSSDConfig->CallBack)
            {
                (pSSDConfig->CallBack)();
            }
            /* Reset counter */
            counter = 0;
        }
    }            

EXIT:
     if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

    return(ret);
}
/* end of file */

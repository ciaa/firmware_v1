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
* FILE NAME     :  PFlashSwap.c                                              
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

#ifdef SWAP
/************************************************************************
*
*  Function Name    : PFlashSwap.c
*  Description      : Perform a swap between P-Flash block 0 and
*                     P-Flash block 1
*                     
*                     
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, pFLASHCOMMANDSEQUENCE, 
*                     PSWAP_CALLBACK
*                     
*  Return Value     : UINT32
*
*************************************************************************/
UINT32 PFlashSwap(PFLASH_SSD_CONFIG pSSDConfig, \
                  UINT32 addr, \
                  PFLASH_SWAP_CALLBACK pSwapCallback, \
                  pFLASHCOMMANDSEQUENCE pFlashCommandSequence)
{
    UINT32 ret;      /* Return code */
    UINT8 currentSwapMode , currentSwapBlockStatus , nextSwapBlockStatus;    
    BOOL swapContinue;
    
    ret = FTFx_OK;
    currentSwapMode = currentSwapBlockStatus = nextSwapBlockStatus = 0xFF;
    swapContinue = FALSE;
    
    if (NULL_SWAP_CALLBACK == pSwapCallback)
    {
        ret = FTFx_ERR_ACCERR;
        goto EXIT;
    }
     
    /* Report current swap state */
    ret = PFlashSwapCtl(pSSDConfig,addr,FTFx_SWAP_REPORT_STATUS,&currentSwapMode, \
    &currentSwapBlockStatus, &nextSwapBlockStatus ,pFlashCommandSequence);
    
    if (FTFx_OK != ret)
    {
        goto EXIT;
    }
    
    if ((FTFx_SWAP_UNINIT == currentSwapMode) || (FTFx_SWAP_READY == currentSwapMode) || \
    (FTFx_SWAP_UPDATE == currentSwapMode))
    {
        /* If current swap mode is Uninitialized */
        if (FTFx_SWAP_UNINIT == currentSwapMode)
        {
            /* Initialize Swap to Initialized/READY state */
            ret = PFlashSwapCtl(pSSDConfig, addr, FTFx_SWAP_SET_INDICATOR_ADDR,&currentSwapMode, \
            &currentSwapBlockStatus, &nextSwapBlockStatus , pFlashCommandSequence);
        }
            /* If current swap mode is Initialized/Ready */
        else if (FTFx_SWAP_READY == currentSwapMode)
        {
            /* Initialize Swap to UPDATE state */
            ret = PFlashSwapCtl(pSSDConfig, addr, FTFx_SWAP_SET_IN_PREPARE,&currentSwapMode, \
            &currentSwapBlockStatus, &nextSwapBlockStatus , pFlashCommandSequence);
        }
        else if (FTFx_SWAP_UPDATE == currentSwapMode){}
        
        /* Check for the success of command execution */
        if (ret)
        {
            goto EXIT;
        }
       /* Report the current swap state to user via callback */
        swapContinue = pSwapCallback(currentSwapMode);
              
        if (!swapContinue)
        {
            goto EXIT;
        }
               
        /* Report current swap state */
        ret = PFlashSwapCtl(pSSDConfig,addr,FTFx_SWAP_REPORT_STATUS,&currentSwapMode, \
        &currentSwapBlockStatus, &nextSwapBlockStatus , pFlashCommandSequence);
    }
    
    /* If current swap mode is Update or Update-Erased */
    if  (FTFx_SWAP_UPDATE_ERASED == currentSwapMode) 
    {
        swapContinue = pSwapCallback(currentSwapMode);
        if (!swapContinue)
        {
            goto EXIT;
        }
        /* Progress Swap to COMPLETE State */
        ret = PFlashSwapCtl(pSSDConfig,addr,FTFx_SWAP_SET_IN_COMPLETE,&currentSwapMode, \
        &currentSwapBlockStatus, &nextSwapBlockStatus , pFlashCommandSequence);      
    }

EXIT:
    /* Enter Debug state if enabled */
    if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

    return(ret);
}
#endif /* of SWAP */
/* end of file */

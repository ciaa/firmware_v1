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
* FILE NAME     :  PFlashGetProtection.c                                     
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
*  Function Name    : PFlashGetProtection.c
*  Description      : This function retrieves current P-Flash protection status. 
*  Arguments        : PFLASH_SSD_CONFIG, UINT32*
*  Return Value     : UINT32
*
*************************************************************************/
UINT32 PFlashGetProtection(PFLASH_SSD_CONFIG pSSDConfig, UINT32* protectStatus)
{
    UINT32 reg0, reg1, reg2, reg3;
   
    reg0=REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET);
    reg1=REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET);
    reg2=REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET);
    reg3=REG_READ(pSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET);
              
    *protectStatus = (UINT32)((UINT32)(reg0 << 24 ) | (UINT32)(reg1 << 16 ) | (UINT32)(reg2 << 8 ) | reg3); 
                   
    /* Enter Debug state if enabled */
    if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

  return(FTFx_OK);
}
/* end of file */

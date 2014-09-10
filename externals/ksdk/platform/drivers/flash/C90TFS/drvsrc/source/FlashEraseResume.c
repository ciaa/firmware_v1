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
* FILE NAME     :  FlashEraseResume.c                                        
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
*  Function Name    : FlashEraseResume.c
*  Description      : This function is used to resume a previous suspended
*                     operation of flash erase sector command.
*  Arguments        : PFLASH_SSD_CONFIG
*  Return Value     : UINT32
*
*************************************************************************/
UINT32 FlashEraseResume(PFLASH_SSD_CONFIG pSSDConfig)

{
    UINT16 i;
    i = 0;
    /* check ERSSUSP bit of the flash configuration register */
    if(0x0 != REG_BIT_GET(pSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_ERSSUSP))
    {

        /* clear CCIF to launch command */
        REG_BIT_SET(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF); 
        /* wait for completion of this command */
        while((0x0 == (REG_BIT_GET(pSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF))) \
            ||(++i >RESUME_WAIT_CNT));	
  
    }
      /* Enter Debug state if enabled */
    if (TRUE == (pSSDConfig->DebugEnable))
    {
        ENTER_DEBUG_MODE;
    }

    return(FTFx_OK);
}

/* end of file */

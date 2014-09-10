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
* FILE NAME     :  CopyToRam.c                                          
* DATE          :  Nov 28, 2013                                         
*                                                                       
* AUTHOR        :  FPT Team                                             
* E-mail        :  r56611@freescale.com                                 
*                                                                       
*****************************************************************************
1.0.0       11.28.2013      FPT Team      Initial version                                   
*END*************************************************************************/
/* include the header files */
#include "SSD_FTFx.h"

/************************************************************************
*
*  Function Name    : RelocateFunction
*  Description      : Relocate FlashCommandSequence to another address.
*  Arguments        : UINT32, UINT32, pFLASHCOMMANDSEQUENCE
*  Return Value     : pFLASHCOMMANDSEQUENCE
*
*************************************************************************/

UINT32 RelocateFunction(UINT32 dest, UINT32 size, UINT32 src)
{
    UINT32 temp;
    UINT16 value, i, *pSrc, *pDest;
    temp = PGM2DATA((UINT32)src - ADDRESS_OFFSET);
    pSrc = (UINT16 *)temp;
    pDest = (UINT16 *)dest;
    temp = size >>1;
    for (i = 0; i < temp; i++)
    {      
        value = READ16(pSrc);     
        pSrc++ ;
        WRITE16(pDest, value);
        pDest++;
    }
    return ((UINT32)DATA2PGM((UINT32)dest + ADDRESS_OFFSET));
}

/* end of file */

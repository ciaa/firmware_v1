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
* FILE NAME     :  SSD_FTFx.h                                               
* DATE          :  Dec 25, 2013                                             
*                                                                           
* AUTHOR        :  FPT Team                                                 
* E-mail        :  r56611@freescale.com                                     
*                                                                           
******************************************************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0
0.1.1       08.16.2010      FPT Team      Update some macros for
                                          FTFx_KX_256K_256K_4K_2K_2K derivative
0.1.2       08.26.2010      FPT Team      Removed EEEBlockBase element in
                                          _ssd_config structure, 
0.1.3       09.16.2010      FPT Team      Updated some macros for
                                          FTFx_KX_256K_256K_4K_2K_2K derivative
0.2.0       09.27.2010      FPT Team      Removed some macros that is not 
                                          used.
0.2.1       01.28.2011      FPT Team      Updated to support 
                                          FTFx_KX_512K_0K_0K_2K_0K,
                                          FTFx_JX_128K_32K_2K_1K_1K,
                                          and FTFx_FX_256K_32K_2K_1K_1K
                                          derivatives.
0.2.2       04.18.2011      FPT Team      Add Swap state definitions of
                                          FTFx_PFLASH_SWAP.
0.2.2       09.15.2011      FPT Team      Add FlashProgramPhrase
                                          Add macros for K70
                                          Remove unused macros
0.2.3       11.15.2011      FPT Team      Updated some macros for
                                          FTFx_KX_1024K_0K_16K_4K_0K derivative.    
0.2.4       12.23.2011      FPT Team      Update to support more Kinetis derivatives.  
0.2.5       04.26.2012      FPT Team      Update to support swap in FTFx_KX_512K_0K_0K_2K_0K derivative     
                                          Add definition of NULL_SWAP_CALLBACK to 
                                          fix incompatible function type of null pointer bug in IAR compiler
0.3.1        05.16.2012     FPT Team      Update to support 
                                          FTFx_NX_256K_32K_2K_2K_1K
                                          FTFx_NX_128K_32K_2K_2K_1K
                                          FTFx_NX_96K_32K_2K_2K_1K
                                          FTFx_NX_64K_32K_2K_2K_1K
                                          derivatives
                                          Change prototype of FlashReadOnce and FlashProgramOnce functions
0.3.2        06.20.2012     FPT Team      Update to support more L2K and L1PT derivatives.
                                          Change format of SSD_FTFx.h
0.3.3        08.10.2012     FPT Team      Update to support Anguilla Silver derivatives 
0.3.4        03.16.2013     FPT Team      Update to support K20-512, K20-1M, L4K, LOPA,
                                          L2KM and MCF51JG derivatives 
                                          Remove user's defined macros to other file.
                                          Add compiler error display
0.3.5       06.10.2013      FPT Team      Add PGM2DATA, DATA2PGM macros to support copying FlashLaunchCommand 
                                          RAM on DSC devices
                                          Update derivative name according to the change on SSD_FTFx_Common.h
                                          Change the prefix of common header file name as below:
                                          - from L (L family) to K (Kinetis)
                                          - from F (FD256) to C (Coldfire)
                                          - from J (JF/JU) to C (Coldfire)
                                          - from N (Nevis) to D (DSC)
                                          - from A (AnguilaSiver) to D (DSC)
1.0.0       12.25.2013      FPT Team      Swap content of SSD_FTFx_Internal.h and SSD_FTFx.h to optimize include structure in c source file
										  Update to support Senna derivatives
                                          Update to Enter debug mode use macro
                                          Add definition READ16_ADV(addr) and READ32_ADV(addr)
                                          to support Anguilla Silver and Nevis2 in EEEWrite function
*END*************************************************************************/
#ifndef _SSD_FTFx_H_
#define _SSD_FTFx_H_

#include "SSD_FTFx_Common.h"
#include "user_cfg.h"

#ifndef FLASH_DERIVATIVE
#error"User needs to define FLASH_DERIVATIVE macro in user_cfg.h file"
#endif

/* Select file .h for each derivatives */
#if (FTFx_KX_256K_256K_4K_2K_2K == FLASH_DERIVATIVE)
    #include "FTFx_KX_256K_256K_4K_2K_2K.h"
#elif (FTFx_KX_128K_128K_4K_2K_2K == FLASH_DERIVATIVE)
    #include "FTFx_KX_128K_128K_4K_2K_2K.h"
#elif ((FTFx_KX_512K_0K_4K_2K_0K == FLASH_DERIVATIVE) || (FTFx_KX_256K_0K_4K_2K_0K == FLASH_DERIVATIVE))
    #include "FTFx_KX_(512_256)K_0K_4K_2K_0K.h"
#elif (FTFx_CX_256K_32K_2K_1K_1K == FLASH_DERIVATIVE)
    #include "FTFx_CX_256K_32K_2K_1K_1K.h"
#elif ((FTFx_CX_128K_32K_2K_1K_1K == FLASH_DERIVATIVE) || (FTFx_CX_64K_32K_2K_1K_1K == FLASH_DERIVATIVE)\
    || (FTFx_CX_32K_32K_2K_1K_1K == FLASH_DERIVATIVE))
    #include "FTFx_CX_(128_64_32)K_32K_2K_1K_1K.h"
#elif (FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE)
    #include "FTFx_KX_512K_512K_16K_4K_4K.h"
#elif ((FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE) || (FTFx_KX_1024K_0K_4K_4K_0K == FLASH_DERIVATIVE))
    #include "FTFx_KX_1024K_0K_(16_4)K_4K_0K.h"
#elif ((FTFx_KX_128K_0K_2K_1K_0K == FLASH_DERIVATIVE)||(FTFx_KX_64K_0K_2K_1K_0K == FLASH_DERIVATIVE)\
    ||(FTFx_KX_32K_0K_2K_1K_0K == FLASH_DERIVATIVE))
    #include "FTFx_KX_(128_64_32)K_0K_2K_1K_0K.h"
#elif ((FTFx_KX_128K_32K_2K_1K_1K == FLASH_DERIVATIVE)||(FTFx_KX_64K_32K_2K_1K_1K == FLASH_DERIVATIVE)\
    ||(FTFx_KX_32K_32K_2K_1K_1K == FLASH_DERIVATIVE))
    #include "FTFx_KX_(128_64_32)K_32K_2K_1K_1K.h"   
#elif ((FTFx_KX_256K_32K_2K_2K_1K == FLASH_DERIVATIVE)||(FTFx_KX_128K_32K_2K_2K_1K == FLASH_DERIVATIVE)\
    ||(FTFx_KX_64K_32K_2K_2K_1K == FLASH_DERIVATIVE))
    #include "FTFx_KX_(256_128_64)K_32K_2K_2K_1K.h"    
#elif ((FTFx_NX_256K_32K_2K_2K_1K == FLASH_DERIVATIVE)||(FTFx_NX_128K_32K_2K_2K_1K == FLASH_DERIVATIVE)\
    ||(FTFx_NX_96K_32K_2K_2K_1K == FLASH_DERIVATIVE)||(FTFx_NX_64K_32K_2K_2K_1K == FLASH_DERIVATIVE))
    #include "FTFx_DX_(256_128_96_64)K_32K_2K_2K_1K.h"      
#elif ((FTFx_LX_128K_0K_0K_1K_0K == FLASH_DERIVATIVE)||(FTFx_LX_64K_0K_0K_1K_0K == FLASH_DERIVATIVE)\
    ||(FTFx_LX_32K_0K_0K_1K_0K == FLASH_DERIVATIVE)||(FTFx_LX_16K_0K_0K_1K_0K == FLASH_DERIVATIVE)\
    ||(FTFx_LX_8K_0K_0K_1K_0K == FLASH_DERIVATIVE) || (FTFx_LX_256K_0K_0K_1K_0K == FLASH_DERIVATIVE)\
    || (FTFx_MX_64K_0K_0K_1K_0K == FLASH_DERIVATIVE) || (FTFx_MX_128K_0K_0K_1K_0K == FLASH_DERIVATIVE))
    #include "FTFx_KX_(256_128_64_32_16_8)K_0K_0K_1K_0K.h"
#elif ((FTFx_AX_64K_0K_0K_1K_0K == FLASH_DERIVATIVE)||(FTFx_AX_48K_0K_0K_1K_0K == FLASH_DERIVATIVE)\
    ||(FTFx_AX_32K_0K_0K_1K_0K == FLASH_DERIVATIVE)||(FTFx_AX_16K_0K_0K_1K_0K == FLASH_DERIVATIVE))
    #include "FTFx_DX_(64_48_32_16)K_0K_0K_1K_0K.h"
#elif (FTFx_KX_512K_128K_4K_4K_4K == FLASH_DERIVATIVE)
    #include "FTFx_KX_512K_128K_4K_4K_4K.h"
#elif ((FTFx_KX_256K_64K_4K_2K_2K == FLASH_DERIVATIVE) || (FTFx_KX_128K_64K_4K_2K_2K == FLASH_DERIVATIVE))
    #include "FTFx_KX_(256_128)K_64K_4K_2K_2K.h"    
#elif ((FTFx_KX_512K_0K_0K_2K_0K == FLASH_DERIVATIVE) || (FTFx_KX_256K_0K_0K_2K_0K == FLASH_DERIVATIVE) \
    || (FTFx_KX_128K_0K_0K_2K_0K == FLASH_DERIVATIVE)) 
    #include "FTFx_KX_(512_256_128)K_0K_0K_2K_0K.h"
#endif

/* determine offset value for copy FlashLaunchCommand */
#if ((CPU_CORE == COLDFIRE)||(CPU_CORE == DSC_56800EX))
#define ADDRESS_OFFSET        0 // coldfile core dont need to shift address
#else
#define ADDRESS_OFFSET        1 // other cores need to shift address by 1 before copying
#endif
          
/* This macros is used for copy command sequence feature*/
#if (CPU_CORE == DSC_56800EX)
    #define PGM2DATA(x) ((x>PROGRAM_RAM_SPACE_BASE)?(x-PROGRAM_RAM_SPACE_BASE + DATA_SPACE_BASE):(x + DATA_SPACE_BASE))
    #define DATA2PGM(x) (x+PROGRAM_RAM_SPACE_BASE)
#else
    #define PGM2DATA(x)    (x)
    #define DATA2PGM(x) (x)
#endif

/* Enter debug mode macro */
#if defined(__ICCARM__)
/* IAR */
#define ENTER_DEBUG_MODE    asm ( "BKPT #0" ) 
#else
/* KDS, Keil, GCC */
#define ENTER_DEBUG_MODE    __asm ( "BKPT #0" ) 
#endif 

/* fccob offset address to store resource code */
#if (PGM_SIZE_BYTE == FTFx_PHRASE_SIZE)
#define RSRC_CODE_OFSSET        FTFx_SSD_FCCOB4_OFFSET            
#else
#define RSRC_CODE_OFSSET        FTFx_SSD_FCCOB8_OFFSET            
#endif

#if (ENDIANNESS == LITTLE_ENDIAN)
    #define READ16_ADV(addr)             ((UINT32)(READ8(addr+1) << 8)|(READ8(addr)))
    #define READ32_ADV(addr)             ((UINT32)(READ16_ADV(addr+2) << 16)|(READ16_ADV(addr)))
    
#else
    #define READ16_ADV(addr)             ((UINT32)(READ8(addr) << 8)|(READ8(addr+1)))
    #define READ32_ADV(addr)             ((UINT32)(READ16_ADV(addr) << 16)|(READ16_ADV(addr+2)))
#endif

#endif /* _SSD_FTFx_H_ */



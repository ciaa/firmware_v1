/**HEADER********************************************************************
 Copyright (c) 2012-2013 Freescale Semiconductor, Inc.
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
0.1.1       08.16.2010      FPT Team      Finalize to 0.1.1
0.1.2       08.26.2010      FPT Team      Finalize to 0.1.2
0.1.3       09.16.2010      FPT Team      Updated to support little Indian
0.2.0       06.27.2010      FPT Team      Finalize to 0.2.0 
0.2.1       01.28.2011      FPT Team      Updated to support 
                                          FTFx_KX_512K_0K_0K,
                                          FTFx_JX_128K_32K_2K,
                                          and FTFx_FX_256K_32K_2K
                                          derivatives.
0.2.2       04.18.2011      FPT Team      Add swap control code definitions
                                          of FTFx_PFLASH_SWAP.
0.2.3       09.15.2011      FPT Team      Add command for program phrase
0.2.4       03.16.2013      FPT Team      Remove define Flash margin read settings
                                          Add GETINDEX macro.
1.0.0       12.25.2013      FPT Team      Swap content of SSD_FTFx_Internal.h and SSD_FTFx.h to optimize include structure in c source file
										  Update to simplify including driver 
                                          header files for source files
                                          Add definitions FTFx_SSD_FSTAT_ERROR_BITS
*END*************************************************************************/
#ifndef _SSD_FTFx_INTERNAL_H_
#define _SSD_FTFx_INTERNAL_H_

#include "SSD_FTFx_Internal.h"

#define FTFx_SSD_FSTAT_CCIF                 0x80
#define FTFx_SSD_FSTAT_RDCOLERR             0x40
#define FTFx_SSD_FSTAT_ACCERR               0x20
#define FTFx_SSD_FSTAT_FPVIOL               0x10
#define FTFx_SSD_FSTAT_MGSTAT0              0x01
#define FTFx_SSD_FSTAT_ERROR_BITS           (FTFx_SSD_FSTAT_ACCERR \
                                            |FTFx_SSD_FSTAT_FPVIOL \
                                            |FTFx_SSD_FSTAT_MGSTAT0)

#define FTFx_SSD_FCNFG_CCIE                 0x80
#define FTFx_SSD_FCNFG_RDCOLLIE             0x40
#define FTFx_SSD_FCNFG_ERSAREQ              0x20
#define FTFx_SSD_FCNFG_ERSSUSP              0x10
#define FTFx_SSD_FCNFG_RAMRDY               0x02
#define FTFx_SSD_FCNFG_EEERDY               0x01    

#define FTFx_SSD_FSEC_KEYEN                 0xC0
#define FTFx_SSD_FSEC_FSLACC                0x0C
#define FTFx_SSD_FSEC_SEC                   0x03    
    
/*--------------- FTFx Flash Module Memory Offset Map -----------------*/
#if(BIG_ENDIAN == ENDIANNESS)    /* Big Endian - coldfire CPU */
    /* Flash Status Register (FSTAT)*/
    #define FTFx_SSD_FSTAT_OFFSET               0x00000003
    /* Flash configuration register (FCNFG)*/
    #define FTFx_SSD_FCNFG_OFFSET               0x00000002
    /* Flash security register (FSEC) */
    #define FTFx_SSD_FSEC_OFFSET                0x00000001
    /* Flash Option Register (FOPT) */
    #define FTFx_SSD_FOPT_OFFSET                0x00000000
    /* Flash common command object registers (FCCOB0-B) */
    #define FTFx_SSD_FCCOB0_OFFSET              0x00000004
    #define FTFx_SSD_FCCOB1_OFFSET              0x00000005
    #define FTFx_SSD_FCCOB2_OFFSET              0x00000006
    #define FTFx_SSD_FCCOB3_OFFSET              0x00000007
    #define FTFx_SSD_FCCOB4_OFFSET              0x00000008
    #define FTFx_SSD_FCCOB5_OFFSET              0x00000009
    #define FTFx_SSD_FCCOB6_OFFSET              0x0000000A
    #define FTFx_SSD_FCCOB7_OFFSET              0x0000000B
    #define FTFx_SSD_FCCOB8_OFFSET              0x0000000C
    #define FTFx_SSD_FCCOB9_OFFSET              0x0000000D
    #define FTFx_SSD_FCCOBA_OFFSET              0x0000000E
    #define FTFx_SSD_FCCOBB_OFFSET              0x0000000F
    /* P-Flash protection registers (FPROT0-3) */
    #define FTFx_SSD_FPROT0_OFFSET              0x00000010
    #define FTFx_SSD_FPROT1_OFFSET              0x00000011
    #define FTFx_SSD_FPROT2_OFFSET              0x00000012
    #define FTFx_SSD_FPROT3_OFFSET              0x00000013
    /* D-Flash protection registers (FDPROT) */
    #define FTFx_SSD_FDPROT_OFFSET              0x00000014
    /* EERAM Protection Register (FEPROT)  */
    #define FTFx_SSD_FEPROT_OFFSET              0x00000015
   
    //#define GET_FCCOB_INDEX(i) (i)
#else /* Little Endian - kinetis CPU + Nevis2 CPU */
    /* Flash Status Register (FSTAT)*/
    #define FTFx_SSD_FSTAT_OFFSET               0x00000000
    /* Flash configuration register (FCNFG)*/
    #define FTFx_SSD_FCNFG_OFFSET               0x00000001
    /* Flash security register (FSEC) */
    #define FTFx_SSD_FSEC_OFFSET                0x00000002
    /* Flash Option Register (FOPT) */
    #define FTFx_SSD_FOPT_OFFSET                0x00000003
    /* Flash common command object registers (FCCOB0-B) */
    #define FTFx_SSD_FCCOB0_OFFSET              0x00000007
    #define FTFx_SSD_FCCOB1_OFFSET              0x00000006
    #define FTFx_SSD_FCCOB2_OFFSET              0x00000005
    #define FTFx_SSD_FCCOB3_OFFSET              0x00000004
    #define FTFx_SSD_FCCOB4_OFFSET              0x0000000B
    #define FTFx_SSD_FCCOB5_OFFSET              0x0000000A
    #define FTFx_SSD_FCCOB6_OFFSET              0x00000009
    #define FTFx_SSD_FCCOB7_OFFSET              0x00000008
    #define FTFx_SSD_FCCOB8_OFFSET              0x0000000F
    #define FTFx_SSD_FCCOB9_OFFSET              0x0000000E
    #define FTFx_SSD_FCCOBA_OFFSET              0x0000000D
    #define FTFx_SSD_FCCOBB_OFFSET              0x0000000C
    /* P-Flash protection registers (FPROT0-3) */
    #define FTFx_SSD_FPROT0_OFFSET              0x00000013
    #define FTFx_SSD_FPROT1_OFFSET              0x00000012
    #define FTFx_SSD_FPROT2_OFFSET              0x00000011
    #define FTFx_SSD_FPROT3_OFFSET              0x00000010
    /* D-Flash protection registers (FDPROT) */
    #define FTFx_SSD_FDPROT_OFFSET              0x00000017
    /* EERAM Protection Register (FEPROT)  */
    #define FTFx_SSD_FEPROT_OFFSET              0x00000016
    //#define GET_FCCOB_INDEX(i) ((((i)>>2) << 3) + 3 - (i))
#endif

/*------------- Flash hardware algorithm operation commands -------------*/
#define FTFx_VERIFY_BLOCK               0x00
#define FTFx_VERIFY_SECTION             0x01
#define FTFx_PROGRAM_CHECK              0x02
#define FTFx_READ_RESOURCE              0x03
#define FTFx_PROGRAM_LONGWORD           0x06
#define FTFx_PROGRAM_PHRASE             0x07
#define FTFx_ERASE_BLOCK                0x08
#define FTFx_ERASE_SECTOR               0x09
#define FTFx_PROGRAM_SECTION            0x0B
#define FTFx_VERIFY_ALL_BLOCK           0x40
#define FTFx_READ_ONCE                  0x41
#define FTFx_PROGRAM_ONCE               0x43
#define FTFx_ERASE_ALL_BLOCK            0x44
#define FTFx_SECURITY_BY_PASS           0x45
#define FTFx_PFLASH_SWAP                0x46
#define FTFx_PROGRAM_PARTITION          0x80
#define FTFx_SET_EERAM                  0x81

/* PFlash swap control codes */
#define FTFx_SWAP_SET_INDICATOR_ADDR    0x01
#define FTFx_SWAP_SET_IN_PREPARE        0x02
#define FTFx_SWAP_SET_IN_COMPLETE       0x04
#define FTFx_SWAP_REPORT_STATUS         0x08


/* EERAM Function Control Code */
#define EEE_ENABLE                      0x00
#define EEE_DISABLE                     0xFF

/* PFlash swap states */
#define FTFx_SWAP_UNINIT                0x00
#define FTFx_SWAP_READY                 0x01
#define FTFx_SWAP_UPDATE                0x02
#define FTFx_SWAP_UPDATE_ERASED         0x03
#define FTFx_SWAP_COMPLETE              0x04

#endif  /* _SSD_FTFx_INTERNAL_H_ */
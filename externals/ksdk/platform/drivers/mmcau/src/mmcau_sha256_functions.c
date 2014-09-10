/*
 * Copyright (c) 2011, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cau2_defines.h"

volatile unsigned long *const MMCAU_PPB_DIRECT   = (unsigned long *) 0xe0081000;
volatile unsigned long *const MMCAU_PPB_INDIRECT = (unsigned long *) 0xe0081800;

#define mmcau_1_cmd(c1)        (0x80000000 + (c1<<22))
#define mmcau_2_cmds(c1,c2)    (0x80100000 + (c1<<22) + (c2<<11))
#define mmcau_3_cmds(c1,c2,c3) (0x80100200 + (c1<<22) + (c2<<11) + c3)
#define byterev(d)             (d>>24)|(d>>8)&0xff00|(d<<8)&0xff0000|(d<<24)

unsigned int sha256_k[]      = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                                0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                                0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                                0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                                0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                                0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                                0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                                0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                                0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                                0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                                0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                                0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                                0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                                0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                                0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                                0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

unsigned int sha256_initial_h[]={0x6a09e667,0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA256: Initializes the hash output*/
/*           *output     pointer to 256-bit message digest output*/
/**/
/*   calling convention*/
/*   int     mmcau_sha256_initialize_output (const unsigned int *output)*/

int mmcau_sha256_initialize_output (unsigned int *output)
{
    int i;

    for (i = 0; i < 8; i++)
    {
        output[i] = sha256_initial_h[i];
    }
    return (0); 
}

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA256: Updates SHA256 digest output for one or more message block arguments*/
/*           *input      pointer to start of input message*/
/*           input       number of 512-bit blocks to process*/
/*           *output     pointer to 256-bit message digest output*/
/**/
/*   NOTE    Input message and digest output blocks must not overlap*/
/**/
/*   calling convention*/
/*   int     mmcau_sha256_hash_n (const unsigned char *input,*/
/*                                int num_blks,*/
/*                                const unsigned int  *output)*/

void mmcau_sha256_hash_n (unsigned int input[], int num_blks,
                          unsigned int output[])
{
    int i, j, k, n;
    int w[64];

/*  initialize the CAU data registers with the current contents of output[]*/

    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = output[0];              /* ld a -> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = output[1];              /* ld b -> CA1*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = output[2];              /* ld c -> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = output[3];              /* ld d -> CA3*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA4)) = output[4];              /* ld e -> CA4*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA5)) = output[5];              /* ld f -> CA5*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA6)) = output[6];              /* ld g -> CA6*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA7)) = output[7];              /* ld h -> CA7*/

    for (n = 0; n < num_blks; n++)
    {
        i = 0;
        k = n * 16;
        for (j = 0; j < 16; j++, i++, k++)
        {
            w[i] = byterev(input[k]);                           /* w[i] = m[i]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CAA))  = w[i];          /* + w[i]*/
            /* + h + SIGMA1(e) + Ch(e,f,g)*/
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(ADRA+CA7,HASH+HF2T,HASH+HF2C);
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha256_k[i];   /* add k[i]*/
            /* t1 + SIGMA0(e) + Maj(a,b,c)*/
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(MVAR+CA8,HASH+HF2S,HASH+HF2M);
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(SHS2);            /* shift regs*/
        }

        for (j = 0; j < 48; j++, i++)
        {
            *(MMCAU_PPB_INDIRECT + (LDR+CAA))  = w[i-16];       /* CAA=w[i-16]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CA8))  = w[i-15];       /* CA8=w[i-15]*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(HASH+HF2U);       /* Sigma2()*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = w[i-7];        /* CAA+=w[i-7]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CA8))  = w[i-2];        /* CA8=w[i-2]*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(HASH+HF2V);       /* Sigma1()*/
            w[i] = *(MMCAU_PPB_INDIRECT + (STR+CAA));           /* store w[i]*/
            /* + h + SIGMA1(e) + Ch(e,f,g)*/
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(ADRA+CA7,HASH+HF2T,HASH+HF2C);
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha256_k[i];   /* add k[i]*/
            /* t1 + SIGMA0(e) + Maj(a,b,c)*/
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(MVAR+CA8,HASH+HF2S,HASH+HF2M);
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(SHS2);            /* shift regs*/
        }

        *(MMCAU_PPB_INDIRECT + (ADR+CA0))  = output[0];        /* + (h[0])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA1))  = output[1];        /* + (h[1])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA2))  = output[2];        /* + (h[2])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA3))  = output[3];        /* + (h[3])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA4))  = output[4];        /* + (h[4])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA5))  = output[5];        /* + (h[5])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA6))  = output[6];        /* + (h[6])*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA7))  = output[7];        /* + (h[7])*/

        output[7] = *(MMCAU_PPB_INDIRECT + (STR+CA7));         /* store (h[7])*/
        output[6] = *(MMCAU_PPB_INDIRECT + (STR+CA6));         /* store (h[6])*/
        output[5] = *(MMCAU_PPB_INDIRECT + (STR+CA5));         /* store (h[5])*/
        output[4] = *(MMCAU_PPB_INDIRECT + (STR+CA4));         /* store (h[4])*/
        output[3] = *(MMCAU_PPB_INDIRECT + (STR+CA3));         /* store (h[3])*/
        output[2] = *(MMCAU_PPB_INDIRECT + (STR+CA2));         /* store (h[2])*/
        output[1] = *(MMCAU_PPB_INDIRECT + (STR+CA1));         /* store (h[1])*/
        output[0] = *(MMCAU_PPB_INDIRECT + (STR+CA0));         /* store (h[0])*/
    }
}

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA256: Updates SHA256 state variables for one or more input message blocks*/
/*   arguments*/
/*           *input    pointer to start of input message data*/
/*           num_blks  number of 512-bit blocks to process*/
/*           *output   pointer to 256-bit message digest output*/
/**/
/*   calling convention*/
/*   int     mmcau_sha256_update (const unsigned char *input,*/
/*                                const int            num_blks,*/
/*                                unsigned int        *output)*/

void mmcau_sha256_update (unsigned int input[], int num_blks,
                          unsigned int output[])
{

    mmcau_sha256_initialize_output (output);

    mmcau_sha256_hash_n (input, num_blks, output);

}

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA256: Performs SHA256 hash algorithm for a single input message block*/
/*           *input   pointer to start of input message data*/
/*           *output  pointer to 256-bit message digest output*/
/**/
/*   calling convention*/
/*   int     mmcau_sha256_hash (const unsigned char *input,*/
/*                              unsigned int        *output)*/

void mmcau_sha256_hash (unsigned int input[], unsigned int output[])
{
    mmcau_sha256_hash_n (input, 1, output);
}


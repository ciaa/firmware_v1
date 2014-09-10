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

unsigned int sha1_k[] = {0x5a827999,0x6ed9eba1,0x8f1bbcdc,0xca62c1d6};

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA1: Initializes the SHA1 state variables*/
/*   arguments*/
/*           *sha1_state pointer to 160-bit block of SHA1 state variables:*/
/*                           a,b,c,d,e*/
/**/
/*   calling convention*/
/*   int     mmcau_sha1_initialize_output (const unsigned int *sha1_state)*/

void mmcau_sha1_initialize_output (unsigned int sha1_state[])
{

/*  initialize the hash variables, both in memory and the CAU*/

    sha1_state[0] = 0x67452301;                           /* initialize a*/
    sha1_state[1] = 0xefcdab89;                           /* initialize b*/
    sha1_state[2] = 0x98badcfe;                           /* initialize c*/
    sha1_state[3] = 0x10325476;                           /* initialize d*/
    sha1_state[4] = 0xc3d2e1f0;                           /* initialize e*/

}

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA1: Perform the hash and generate SHA1 state variables for one or more*/
/*       input message blocks*/
/*  arguments*/
/*           *msg_data   pointer to start of input message data*/
/*           num_blks    number of 512-bit blocks to process*/
/*           *sha1_state pointer to 160-bit block of SHA1 state variables:*/
/*                           a,b,c,d,e*/
/**/
/*   NOTE    Input message and state variable output blocks must not overlap*/
/**/
/*   calling convention*/
/*   int     mmcau_sha1_hash (const unsigned char *msg_data,*/
/*                            const int            num_blks,*/
/*                            unsigned int        *sha1_state)*/

void mmcau_sha1_hash_n (unsigned int msg_data[], int num_blks,
                        unsigned int sha1_state[])
{
    int i, j, k, n;
    int w[80];

/*  initialize the hash variables, in CAU*/

    *(MMCAU_PPB_INDIRECT + (LDR+CA4)) = sha1_state[4];             /* CA4 = e*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = sha1_state[3];             /* CA3 = d*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = sha1_state[2];             /* CA2 = c*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = sha1_state[1];             /* CA1 = b*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = sha1_state[0];             /* CA0 = a*/

    k = 0;
    for (n = 0; n < num_blks; n++)
    {
        i = 0;
        *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(MVRA+CA0);               /* a -> CAA*/
        *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = 5;                    /* rotate 5*/

        for (j = 0; j < 16; j++, k++)
        {
            w[i] = byterev(msg_data[k]);                           /* w[i]=m[k]*/
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(HASH+HFC,ADRA+CA4); /* + Ch(),+ e*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha1_k[0];        /* + k[0]*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = w[i++];           /* + w[i]*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(SHS);                /* shift regs*/
        }

        for (j = 0; j < 4; j++)
        {
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(HASH+HFC,ADRA+CA4); /* + Ch(),+ e*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha1_k[0];        /* + k[0]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CA5))  = w[i-16];          /*CA5=w[i-16]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-14];          /*xor w[i-14]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-8];           /* xor w[i-8]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-3];           /* xor w[i-3]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CA5)) = 1;                /* rotate 1*/
            w[i++] = *(MMCAU_PPB_INDIRECT + (STR+CA5));            /* store w[i]*/
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(ADRA+CA5,SHS);      /* +w[i],shft*/
        }

        for (j = 0; j < 20; j++)
        {
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(HASH+HFP,ADRA+CA4); /*+Parity(),e*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha1_k[1];        /* + k[1]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CA5))  = w[i-16];          /*CA5=w[i-16]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-14];          /*xor w[i-14]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-8];           /* xor w[i-8]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-3];           /* xor w[i-3]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CA5)) = 1;                /* rotate 1*/
            w[i++] = *(MMCAU_PPB_INDIRECT + (STR+CA5));            /* store w[i]*/
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(ADRA+CA5,SHS);      /* +w[i],shft*/
        }

        for (j = 0; j < 20; j++)
        {
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(HASH+HFM,ADRA+CA4); /* Maj(), +e*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha1_k[2];        /* + k[2]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CA5))  = w[i-16];          /*CA5=w[i-16]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-14];          /*xor w[i-14]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-8];           /* xor w[i-8]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-3];           /* xor w[i-3]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CA5)) = 1;                /* rotate 1*/
            w[i++] = *(MMCAU_PPB_INDIRECT + (STR+CA5));            /* store w[i]*/
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(ADRA+CA5,SHS);      /* +w[i],shft*/
        }

        for (j = 0; j < 20; j++)
        {
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(HASH+HFP,ADRA+CA4); /*+Parity(),e*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = sha1_k[3];        /* + k[3]*/
            *(MMCAU_PPB_INDIRECT + (LDR+CA5))  = w[i-16];          /*CA5=w[i-16]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-14];          /*xor w[i-14]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-8];           /* xor w[i-8]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CA5))  = w[i-3];           /* xor w[i-3]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CA5)) = 1;                /* rotate  1*/
            w[i++] = *(MMCAU_PPB_INDIRECT + (STR+CA5));            /* store w[i]*/
            *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(ADRA+CA5,SHS);      /* +w[i],shft*/
        }

        *(MMCAU_PPB_INDIRECT + (ADR+CA0))  = sha1_state[0];        /* + sha1[0]*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA1))  = sha1_state[1];        /* + sha1[1]*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA2))  = sha1_state[2];        /* + sha1[2]*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA3))  = sha1_state[3];        /* + sha1[3]*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA4))  = sha1_state[4];        /* + sha1[4]*/

        sha1_state[4] = *(MMCAU_PPB_INDIRECT + (STR+CA4));         /*st sha1[4]*/
        sha1_state[3] = *(MMCAU_PPB_INDIRECT + (STR+CA3));         /*st sha1[3]*/
        sha1_state[2] = *(MMCAU_PPB_INDIRECT + (STR+CA2));         /*st sha1[2]*/
        sha1_state[1] = *(MMCAU_PPB_INDIRECT + (STR+CA1));         /*st sha1[1]*/
        sha1_state[0] = *(MMCAU_PPB_INDIRECT + (STR+CA0));         /*st sha1[0]*/
    }

}

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA1: Updates SHA1 state variables for one or more input message blocks*/
/*   arguments*/
/*           *msg_data   pointer to start of input message data*/
/*           num_blks    number of 512-bit blocks to process*/
/*           *sha1_state pointer to 160-bit block of SHA1 state variables:*/
/*                           a,b,c,d,e*/
/**/
/*   calling convention*/
/*   int     mmcau_sha1_update (const unsigned char *msg_data,*/
/*                              const int            num_blks,*/
/*                              unsigned int        *sha1_state)*/

void mmcau_sha1_update(unsigned int msg_data[], int num_blks,
                       unsigned int sha1_state[])
{
    int i;

    mmcau_sha1_initialize_output (sha1_state);

    mmcau_sha1_hash_n (msg_data, num_blks, sha1_state);
}

/********************************************************************************/
/********************************************************************************/
/**/
/* SHA1: Performs SHA1 hash algorithm on a single input message block*/
/*           *msg_data   pointer to start of input message data*/
/*           *sha1_state pointer to 160-bit block of SHA1 state variables:*/
/*                           a,b,c,d,e*/
/**/
/*   calling convention*/
/*   int     mmcau_sha1_update (const unsigned char *msg_data,*/
/*                              unsigned int        *sha1_state)*/

void mmcau_sha1_hash (unsigned int msg_data[], unsigned int sha1_state[])
{
    mmcau_sha1_hash_n (msg_data, 1, sha1_state);
}

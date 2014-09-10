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

unsigned int md5_t[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

unsigned int md5_rnd[] = {7, 12, 17, 22,  7, 12, 17, 22,
                          7, 12, 17, 22,  7, 12, 17, 22,
                          5,  9, 14, 20,  5,  9, 14, 20,
                          5,  9, 14, 20,  5,  9, 14, 20,
                          4, 11, 16, 23,  4, 11, 16, 23,
                          4, 11, 16, 23,  4, 11, 16, 23,
                          6, 10, 15, 21,  6, 10, 15, 21,
                          6, 10, 15, 21,  6, 10, 15, 21};

/********************************************************************************/
/********************************************************************************/
/**/
/* MD5: Initializes the MD5 state variables*/
/*   arguments*/
/*           *md_state  pointer to 120-bit block of md5 state variables:*/
/*                      a,b,c,d*/
/**/
/*   calling convention*/
/*   int     mmcau_md5_initialize_output (const unsigned char *md_state)*/

void mmcau_md5_initialize_output (unsigned int md_state[])
{
    md_state[0] = 0x67452301;                           /* initialize a*/
    md_state[1] = 0xefcdab89;                           /* initialize b*/
    md_state[2] = 0x98badcfe;                           /* initialize c*/
    md_state[3] = 0x10325476;                           /* initialize d*/
}

/********************************************************************************/
/********************************************************************************/
/**/
/* MD5: Updates MD5 state variables for one or more input message blocks*/
/**/
/*   arguments*/
/*           *msg_data   pointer to start of input message data*/
/*           num_blks    number of 512-bit blocks to process*/
/*           *md_state   pointer to 128-bit block of MD5 state variables:*/
/*                       a,b,c,d*/
/**/
/*   calling convention*/
/*   int     mmucau_md5_hash_n (const unsigned char *msg_data,*/
/*                              const int            num_blks,*/
/*                              unsigned char       *md_state)*/

void mmcau_md5_hash_n (unsigned int msg_data[], int num_blks,
                       unsigned int md_state[])

{
    int i, j, k, n;
    unsigned int temp[4];

    /* initialize CAU data registers with the current contents of md_state[]*/

    *(MMCAU_PPB_INDIRECT + (LDR+CAA)) = md_state[0]; /* ld a -> CAA*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = md_state[1]; /* ld b -> CA1*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = md_state[2]; /* ld c -> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = md_state[3]; /* ld d -> CA3*/

    for (n = 0; n < num_blks; n++)
    {
        j = 0;
        i = n * 16;

    /* 16 rounds with F()*/

        *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(HASH+HFF);        /* add F(b,c,d)*/
        *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = msg_data[i++]; /* add x[0]*/
        *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = md5_t[j];      /* add t[0]*/
        *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = md5_rnd[j++];  /* rotate left*/
                                                            /* by md5_rnd[0]*/

        for (k = 0; k < 15; k++)
        {
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(ADRA+CA1,MDS,HASH+HFF);
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = msg_data[i++]; /* add x[i]*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = md5_t[j];      /* add t[j]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = md5_rnd[j++];  /* rotate left*/
                                                                /* by md5_rnd[j]*/
        }

    /* 16 rounds with G()*/
        for (k = 0, i = 1 + (n * 16); k < 16; k++, i = (i + 5) % 16 + (n * 16))
        {
             *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(ADRA+CA1,MDS,HASH+HFG);
             *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = msg_data[i];  /* add x[i]*/
             *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = md5_t[j];     /* add t[j]*/
             *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = md5_rnd[j++]; /* rotate left*/
                                                                /* by md5_rnd[j]*/
        }

    /* 16 rounds with H()*/
        for (k = 0, i = 5 + (n * 16); k < 16; k++, i = (i + 3) % 16 + (n * 16))
        {
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(ADRA+CA1,MDS,HASH+HFH);
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = msg_data[i];   /* add x[i]*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = md5_t[j];      /* add t[j]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = md5_rnd[j++];  /* rotate left*/
                                                                /* by md5_rnd[j]*/
        }

    /* 16 rounds with I()*/
        for (k = 0, i = n * 16; k < 16; k++, i = (i + 7) % 16 + (n * 16))
        {
            *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(ADRA+CA1,MDS,HASH+HFI);
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = msg_data[i];   /* add x[i]*/
            *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = md5_t[j];      /* add t[j]*/
            *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = md5_rnd[j++];  /* rotate left*/
                                                                /* by md5_rnd[j]*/
        }
        *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(ADRA+CA1,MDS);       /* add c & shift*/

        *(MMCAU_PPB_INDIRECT + (ADR+CAA))  = md_state[0];       /* + a*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA1))  = md_state[1];       /* + b*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA2))  = md_state[2];       /* + c*/
        *(MMCAU_PPB_INDIRECT + (ADR+CA3))  = md_state[3];       /* + d*/

        md_state[0] = *(MMCAU_PPB_INDIRECT + (STR+CAA));        /* store a*/
        md_state[1] = *(MMCAU_PPB_INDIRECT + (STR+CA1));        /* store b*/
        md_state[2] = *(MMCAU_PPB_INDIRECT + (STR+CA2));        /* store c*/
        md_state[3] = *(MMCAU_PPB_INDIRECT + (STR+CA3));        /* store d*/
    }

}

/********************************************************************************/
/********************************************************************************/
/**/
/* MD5: Updates MD5 state variables for one or more input message blocks*/
/*   arguments*/
/*           *msg_data   pointer to start of input message data*/
/*           num_blks    number of 512-bit blocks to process*/
/*           *md_state   pointer to 128-bit block of MD5 state variables:*/
/*                       a,b,c,d*/
/**/
/*   calling convention*/
/*   int     mmcau_md5_update (const unsigned char *msg_data,*/
/*                             const int            num_blks,*/
/*                             unsigned char       *md_state)*/

void mmcau_md5_update (unsigned int msg_data[], int num_blks,
                       unsigned int md_state[])
{

    mmcau_md5_initialize_output (md_state);

    mmcau_md5_hash_n (msg_data, num_blks, md_state);

}

/********************************************************************************/
/********************************************************************************/
/**/
/* MD5: Performs MD5 hash algorithm for a single input message block*/
/*           *msg_data   pointer to start of input message data*/
/*           *md_state   pointer to 128-bit block of MD5 state variables:*/
/*                       a,b,c,d*/
/**/
/*   calling convention*/
/*   int     mmcau_md5_hash (const unsigned char *msg_data,*/
/*                           unsigned char       *md_state)*/

void mmcau_md5_hash (unsigned int msg_data[], unsigned int md_state[])
{
    mmcau_md5_hash_n (msg_data, 1, md_state);
}

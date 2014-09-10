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

/********************************************************************************/
/********************************************************************************/

/********************************************************************************/
/**/
/* AES: Performs an AES key expansion*/
/*   arguments*/
/*           *key        pointer to input key (128, 192, 256 bits in length)*/
/*           key_size    key_size in bits (128, 192, 256)*/
/*           *key_sch    pointer to key schedule output (44, 52, 60 longwords)*/
/**/
/*   calling convention*/
/*   int     mmcau_aes_set_key (const unsigned char *key,*/
/*                              const int            key_size,*/
/*                              unsigned char       *key_sch)*/

void mmcau_aes_set_key(unsigned int key[], int key_size, unsigned int key_sch[])
{
    int i,j;
    int rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                    0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};

    if (key_size == 256)
    {
        for (i = 0; i < 8; i++)
            key_sch[i] = byterev(key[i]);

        *(MMCAU_PPB_INDIRECT + (LDR+CAA)) = key_sch[i-1];         /* CAA=key[7]*/

        for (j = 0; j < 6; j++)
        {
            *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = 8;               /* rotate 8*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS+CAA);          /* SubBytes*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = rcon[j];          /* XOR rcon[j]*/

/* 1st calculation for key_sch[8+0,16+0,24+0,32+0,40+0,48+0]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 2nd calculation for key_sch[8+1,16+1,24+1,32+1,40+1,48+1]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 3rd calculation for key_sch[8+2,16+2,24+2,32+2,40+2,48+2]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 4th calculation for key_sch[8+3,16+3,24+3,32+3,40+3,48+3]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 5th calculation for key_sch[8+4,16+4,24+4,32+4,40+4,48+4]*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS+CAA);          /* SubBytes*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 6th calculation for key_sch[8+5,16+5,18+5,32+5,40+5,48+5]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 7th calculation for key_sch[8+6,16+6,18+6,32+6,40+6,48+6]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 8th calculation for key_sch[8+7,16+7,18+7,32+7,40+7,48+7]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];     /*XOR key[i-8]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/
        }

        *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = 8;                   /* rotate by 8*/
        *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS+CAA);              /* SubBytes*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = rcon[j];              /* XOR rcon[j]*/

/* calculation for key_sch[56]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];         /*XOR key[i-8]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

/* calculation for key_sch[57]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];         /*XOR key[i-8]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

/* calculation for key_sch[58]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];         /*XOR key[i-8]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

/* calculation for key_sch[59]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-8];         /*XOR key[i-8]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

    }
    else if (key_size == 192)
    {
        for (i = 0; i < 6; i++)
            key_sch[i] = byterev(key[i]);

        *(MMCAU_PPB_INDIRECT + (LDR+CAA)) = key_sch[i-1];         /* CAA=key[5]*/

        for (j = 0; j < 7; j++)
        {
            *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = 8;               /* rotate 8*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS+CAA);          /* SubBytes*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = rcon[j];          /* XOR rcon[j]*/

/* 1st calculation for key_sch[6+0,12+0,18+0,24+0,30+0,36+0,42+0]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];     /*XOR key[i-6]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 2nd calculation for key_sch[6+1,12+1,18+1,24+1,30+1,36+1,42+1]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];     /*XOR key[i-6]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 3rd calculation for key_sch[6+2,12+2,18+2,24+2,30+2,36+2,42+2]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];     /*XOR key[i-6]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 4th calculation for key_sch[6+3,12+3,18+3,24+3,30+3,36+3,42+3]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];     /*XOR key[i-6]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 5th calculation for key_sch[6+4,12+4,18+4,24+4,30+4,36+4,42+4]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];     /*XOR key[i-6]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 6th calculation for key_sch[6+5,12+5,18+5,24+5,30+5,36+5,42+5]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];     /*XOR key[i-6]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/
        }
        *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = 8;                   /* rotate 8*/
        *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS+CAA);              /*AES SubBytes*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = rcon[j];              /*XOR rcon[j]*/

/* calculation for key_sch[48]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];         /*XOR key[i-6]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

/* calculation for key_sch[49]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];         /*XOR key[i-6]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

/* calculation for key_sch[50]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];         /*XOR key[i-6]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/

/* calculation for key_sch[51]*/
        *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-6];         /*XOR key[i-6]*/
        key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));         /*store key[i]*/
    }
    else if (key_size == 128)
    {
        for (i = 0; i < 4; i++)
            key_sch[i] = byterev(key[i]);

        *(MMCAU_PPB_INDIRECT + (LDR+CAA)) = key_sch[i-1];         /* CAA= key[3]*/

        for (j = 0; j < 10; j++)
        {
            *(MMCAU_PPB_INDIRECT + (ROTL+CAA)) = 8;               /* rotate 8*/
            *(MMCAU_PPB_DIRECT) = mmcau_1_cmd(AESS+CAA);          /* SubBytes*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = rcon[j];          /* XOR rcon[j]*/

/* 1st calculation for key_sch[4+0,8+0,12+0,16+0,20+0,24+0,28+0,32+0,36+0,40+0]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-4];     /*XOR key[i-4]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 2nd calculation for key_sch[4+1,8+1,12+1,16+1,20+1,24+1,28+1,32+1,36+1,40+1]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-4];     /*XOR key[i-4]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 3rd calculation for key_sch[4+2,8+2,12+2,16+2,20+2,24+2,28+2,32+2,36+2,40+2]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-4];     /*XOR key[i-4]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/

/* 4th calculation for key_sch[4+3,8+3,12+3,16+3,20+3,24+3,28+3,32+3,36+3,40+3]*/
            *(MMCAU_PPB_INDIRECT + (XOR+CAA)) = key_sch[i-4];     /*XOR key[i-4]*/
            key_sch[i++] = *(MMCAU_PPB_INDIRECT + (STR+CAA));     /*store key[i]*/
        }
    }
}

/********************************************************************************/
/********************************************************************************/
/**/
/* AES: Encrypts a single 16-byte block*/
/*   arguments*/
/*           *in         pointer to 16-byte block of input plaintext*/
/*           *key_sch    pointer to key schedule (44, 52, 60 longwords)*/
/*           nr          number of AES rounds (10, 12, 14 = f(key_schedule))*/
/*           *out        pointer to 16-byte block of output ciphertext*/
/**/
/*   NOTE    Input and output blocks may overlap*/
/**/
/*   calling convention*/
/*   int     mmcau_aes_encrypt (const unsigned char *in,*/
/*                              const unsigned char *key_sch,*/
/*                              const int            nr,*/
/*                              unsigned char       *out)*/

void mmcau_aes_encrypt(unsigned int in[], unsigned int key_sch[], int nr,
                       unsigned int out[])
{
    int i,j;

/* load the 4 plain test bytes into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = byterev(in[0]);      /* load in[0]-> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = byterev(in[1]);      /* load in[1]-> CA1*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = byterev(in[2]);      /* load in[2]-> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = byterev(in[3]);      /* load in[3]-> CA3*/

/* XOR the first 4 keys into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA0)) = key_sch[0]; /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA1)) = key_sch[1];
    *(MMCAU_PPB_INDIRECT + (XOR+CA2)) = key_sch[2];
    *(MMCAU_PPB_INDIRECT + (XOR+CA3)) = key_sch[3];

/* send a series of cau commands to perform the encryption*/
    for (i = 0, j = 4; i < nr - 1 ; i++, j+=4)
    {
        *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESS+CA0,AESS+CA1,AESS+CA2); /*Sbytes*/
        *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESS+CA3,AESR);          /*Sbyte,Shft*/
        *(MMCAU_PPB_INDIRECT + (AESC+CA0)) = key_sch[j];            /* MixCols*/
        *(MMCAU_PPB_INDIRECT + (AESC+CA1)) = key_sch[j+1];
        *(MMCAU_PPB_INDIRECT + (AESC+CA2)) = key_sch[j+2];
        *(MMCAU_PPB_INDIRECT + (AESC+CA3)) = key_sch[j+3];
    }

    *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESS+CA0,AESS+CA1,AESS+CA2); /* SubBytes*/
    *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESS+CA3,AESR);              /*SByte,Shft*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA0)) = key_sch[j];                 /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA1)) = key_sch[j+1];
    *(MMCAU_PPB_INDIRECT + (XOR+CA2)) = key_sch[j+2];
    *(MMCAU_PPB_INDIRECT + (XOR+CA3)) = key_sch[j+3];

/* store the 16-byte ciphertext output block into memory*/
    out[0] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA0))); /* store 1st 4 bytes*/
    out[1] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA1))); /* store 2nd 4 bytes*/
    out[2] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA2))); /* store 3rd 4 bytes*/
    out[3] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA3))); /* store 4th 4 bytes*/
}

/********************************************************************************/
/********************************************************************************/
/**/
/* AES: Decrypts a single 16-byte block*/
/*   arguments*/
/*           *in         pointer to 16-byte block of input chiphertext*/
/*           *key_sch    pointer to key schedule (44, 52, 60 longwords)*/
/*           nr          number of AES rounds (10, 12, 14 = f(key_schedule))*/
/*           *out        pointer to 16-byte block of output plaintext*/
/**/
/*   NOTE    Input and output blocks may overlap*/
/**/
/*   calling convention*/
/*   int     mmcau_aes_decrypt (const unsigned char *in,*/
/*                              const unsigned char *key_sch,*/
/*                              const int            nr,*/
/*                              unsigned char       *out)*/

void mmcau_aes_decrypt(unsigned int in[], unsigned int key_sch[], int nr,
                       unsigned int out[])
{
    int i;

/* load the cipher bytes into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA0)) = byterev(in[0]);     /* load in[0] -> CA0*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA1)) = byterev(in[1]);     /* load in[1] -> CA1*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA2)) = byterev(in[2]);     /* load in[2] -> CA2*/
    *(MMCAU_PPB_INDIRECT + (LDR+CA3)) = byterev(in[3]);     /* load in[3] -> CA3*/

/* the key_sch index (i) is adjusted to define the end of the elements*/
/* the adjustment factor = f(nr) is defined by the expression:*/
/*   end of key_sch = 4 x (nr + 1) for nr = {10, 12, 14}*/
    i = 4 * (nr + 1);

/* XOR the keys into the CAU's CA0 - CA3 registers*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA3)) = key_sch[--i]; /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA2)) = key_sch[--i];
    *(MMCAU_PPB_INDIRECT + (XOR+CA1)) = key_sch[--i];
    *(MMCAU_PPB_INDIRECT + (XOR+CA0)) = key_sch[--i];

/* send a series of cau commands to perform the decryption*/
    while (i > 4)
    {
        *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESIR,AESIS+CA3,AESIS+CA2);
                                                     /* InvShiftRows,InvSubBytes*/
        *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESIS+CA1,AESIS+CA0);   /* InvSubByts*/
        *(MMCAU_PPB_INDIRECT + (AESIC+CA3)) = key_sch[--i];        /* InvMxCols*/
        *(MMCAU_PPB_INDIRECT + (AESIC+CA2)) = key_sch[--i];
        *(MMCAU_PPB_INDIRECT + (AESIC+CA1)) = key_sch[--i];
        *(MMCAU_PPB_INDIRECT + (AESIC+CA0)) = key_sch[--i];
    }

    *(MMCAU_PPB_DIRECT) = mmcau_3_cmds(AESIR,AESIS+CA3,AESIS+CA2);
                                                    /* InvShiftRows,InvSubBytes*/
    *(MMCAU_PPB_DIRECT) = mmcau_2_cmds(AESIS+CA1,AESIS+CA0);       /* InvSBytes*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA3)) = key_sch[--i];              /* XOR keys*/
    *(MMCAU_PPB_INDIRECT + (XOR+CA2)) = key_sch[--i];
    *(MMCAU_PPB_INDIRECT + (XOR+CA1)) = key_sch[--i];
    *(MMCAU_PPB_INDIRECT + (XOR+CA0)) = key_sch[--i];

/* store the 16-byte ciphertext output block into memory*/
    out[0] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA0)));    /* store 1st 4 bytes*/
    out[1] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA1)));    /* store 2nd 4 bytes*/
    out[2] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA2)));    /* store 3rd 4 bytes*/
    out[3] = byterev(*(MMCAU_PPB_INDIRECT + (STR+CA3)));    /* store 4th 4 bytes*/
}

/**********************************************************************
* $Id$		otp.h		2011-04-21
*//**
* @file		otp.h
* @brief	Bootloader OTP component header file 
* @version	1.0
* @date		21. April. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#ifndef OTP_H_
#define OTP_H_

/* OTP register map */
#define OTP_BASE 0x40045000

#define UNLOCK_REG 0x1
#define LOCK_REG   0x0

#define TMHW_OTP_PROG_MSK		     0x00000001
#define TMHW_OTP_AES_SSEL_MSK     	     0x0000003C
#define TMHW_OTP_STATUS_MSK		     0x00000003
#define TMHW_OTP_LOCK_OTPX_MSK		     0x0000000F

#define TMHW_OTP_LOAD_USER_KEY_POS  1
//#define TMHW_OTP_PROG_SEL_POS	     	     2
#define TMHW_OTP_PROG_EN_POS	     	     0

#define TMHW_OTP_STAT_BUSY_POS	     	     0
#define TMHW_OTP_SHCTRL_EN_POS	     	     0
#define TMHW_OTP_SHCTRL_DIR_POS	             1
#define TMHW_OTP_SHCTRL_LOCK_POS	     2

#define TMHW_OTP_AES_SSEL_USER_KEY	     0x4
#define TMHW_OTP_AES_SSEL_UNIQ_KEY	     0x8
#define TMHW_OTP_AES_SSEL_AES_KEY2	     0x10
#define TMHW_OTP_AES_SSEL_RND_KEY	     0x20


#define TMHW_OTP_LOCK_LOCK_WR_EN_POS	     7

#define TMHW_OTP_LOCK_OTPX_0_EN_POS	     0
#define TMHW_OTP_LOCK_OTPX_1_EN_POS	     1
#define TMHW_OTP_LOCK_OTPX_2_EN_POS	     2
#define TMHW_OTP_LOCK_OTPX_3_EN_POS	     3

#define TMHW_OTP_LOCK_UNIQKEY_EN_POS	     0
#define TMHW_OTP_LOCK_RNDNUM_EN_POS	     0
#define TMHW_OTP_LOCK_USERKEY_EN_POS         0

#define TMHW_OTP_MASK_WR_OTP0_EN	    0xE
#define TMHW_OTP_MASK_WR_OTP1_EN	    0xD
#define TMHW_OTP_MASK_WR_OTP2_EN	    0xB
#define TMHW_OTP_MASK_WR_OTP3_EN	    0x7
#define TMHW_OTP_MASK_WR_OTPX_EN	    0x0
#define TMHW_OTP_MASK_WR_OTPX_DIS	    0xF


typedef volatile struct
{
        unsigned OTP0_0; 	        /*0x0*/
        unsigned OTP0_1;
        unsigned OTP0_2;
        unsigned OTP0_3;
        unsigned OTP1_0;
        unsigned OTP1_1;
        unsigned OTP1_2;
        unsigned OTP1_3;
        unsigned OTP2_0;
        unsigned OTP2_1;
        unsigned OTP2_2;
        unsigned OTP2_3;
        unsigned OTP3_0;
        unsigned OTP3_1;
        unsigned OTP3_2;
        unsigned OTP3_3;
} OTP, *pOTP;

#define pOTPRegs ((pOTP) OTP_BASE)


extern void tmhwBootRomOtp_Init(void);
extern unsigned hwOTP_Program_bank3( unsigned *data, unsigned *mask, unsigned wr_enable);
extern unsigned OTP_Get_randomNrs(void);
extern unsigned OTP_Program_BOOT_SRC(unsigned src);
extern unsigned OTP_Program_JTAG_DIS(void);
extern unsigned OTP_Program_USB_ID(unsigned prod_id, unsigned vend_id);
extern unsigned OTP_Program_GP0(unsigned data,unsigned mask);
extern unsigned OTP_Program_GP1(unsigned data,unsigned mask);
extern unsigned OTP_Program_GP2(unsigned data,unsigned mask);

#endif /*OTP_H_*/

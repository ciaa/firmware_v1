/*****************************************************************************
 * $Id:: uda1380.h
 *
 * Project: USB Audio Example for LPC18xx and LPC43xx
 *
 * Description: Configuration for UDA1380 audio DAC
 *
 *----------------------------------------------------------------------------
 *  Copyright(C) 2012, NXP Semiconductor
 *  All rights reserved.
 *
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
 *****************************************************************************/
#ifndef __UDA1380__
#define __UDA1380__

// #include "i2c.h"

#define UDA1380_I2C_ADR		0x1A

#define UDA1380_REG_EVALCLK	  0x00
#define UDA1380_REG_I2S		  0x01
#define UDA1380_REG_PWRCTRL	  0x02
#define UDA1380_REG_ANAMIX	  0x03
#define UDA1380_REG_HEADAMP	  0x04
#define UDA1380_REG_MSTRVOL	  0x10
#define UDA1380_REG_MIXVOL	  0x11
#define UDA1380_REG_MODEBBT	  0x12
#define UDA1380_REG_MSTRMUTE  0x13
#define UDA1380_REG_MIXSDO	  0x14
#define UDA1380_REG_DECVOL	  0x20
#define UDA1380_REG_PGA		  0x21
#define UDA1380_REG_ADC		  0x22
#define UDA1380_REG_AGC		  0x23
#define UDA1380_REG_L3		  0x7f
#define UDA1380_REG_HEADPHONE 0x18
#define UDA1380_REG_DEC		  0x28

#define REG_EVALCLK_DEF		0x0502
#define REG_I2S_DEF		    0x0000
/* _BIT(15)|_BIT(10)|_BIT(8)|_BIT(4)|_BIT(3)|_BIT(2)|_BIT(1)|_BIT(0) */
#define REG_PWRCTRL_DEF		0x851f
#define REG_ANAMIX_DEF		0x3f3f
#define REG_HEADAMP_DEF		0x0202
#define REG_MSTRVOL_DEF		0x0000
#define REG_MIXVOL_DEF		0xff00
#define REG_MODEBBT_DEF		0x0000
#define REG_MSTRMUTE_DEF	0x0A02
#define REG_MIXSDO_DEF		0x4000
#define REG_DECVOL_DEF		0x0000
#define REG_PGA_DEF		    0x8000
#define REG_ADC_DEF		    0x0002
#define REG_AGC_DEF		    0x0000
#define REG_L3_DEF		    0x0000

#define UDA1380_USB_MAX_VOL   256

extern void Buffer_Init(void);
extern void Audio_Init(void);
extern uint32_t uda1380_init(void);
extern uint32_t uda1380_reg_write(uint8_t reg, uint16_t val);
extern uint16_t uda1380_reg_read(uint8_t reg);
extern int I2S_config(int config_index);

#endif 

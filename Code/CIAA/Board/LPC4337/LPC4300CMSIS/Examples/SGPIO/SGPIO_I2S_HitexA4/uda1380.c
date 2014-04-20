/*****************************************************************************
 * $Id:: uda1380.c
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
#include "lpc43xx_cgu.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_scu.h"
#include "uda1380.h"



#define I2CDEV_UDA1380_ADDR	(0x34 >> 1)
#define I2CDEV_M LPC_I2C0



/***********************************************************************
 * I2C UDA1380 private functions
 **********************************************************************/
/* UDA1380 Register Address */

typedef enum {
  UDA_EVALM_CLK = 0x00,
  UDA_BUS_CTRL,
  UDA_POWER_CTRL,
  UDA_ANALOG_CTRL,
  UDA_HPAMP_CTRL,
  UDA_MASTER_VOL_CTRL = 0x10,
  UDA_MIXER_VOL_CTRL,
  UDA_MODE_CTRL,
  UDA_MUTE_CTRL,
  UDA_MIXER_FILTER_CTRL,
  UDA_DEC_VOL_CTRL = 0x20,
  UDA_PGA_CTRL,
  UDA_ADC_CTRL,
  UDA_AGC_CTRL,
  UDA_TOTAL_REG

} UDA1380_REG;



/* System Register Data Set */
unsigned short UDA_sys_regs_dat[]={
  
  0xF<<8 | 0x3<<4 | 2,      /* UDA_EVALM_CLK */   
  /*0xF <<8 | 0x3<<4 |  8 | 2,       UDA_EVALM_CLK */        
  0x00,                      /* UDA_BUS_CTRL */
  1 <<13 | 1<<10 | 1<<8,     /* UDA_POWER_CTRL */ 
  0x0000,                    /* UDA_ANALOG_CTRL */  
  1<<9 | 2                   /* UDA_HPAMP_CTRL */
};

/* Volume and sound control register set */
unsigned short UDA_interfil_regs_dat[]={
  
  0x4040,            /* 0x10 - UDA_MASTER_VOL_CTRL - max volume left/right would be 0x0000 */  
  0x0000,            /* 0x11 - UDA_MIXER_VOL_CTRL */  
  0xE535,            /* 0x12 - UDA_MODE_CTRL - bass boost and treble */  
  2<<8 | 1,          /* 0x13 - UDA_MUTE_CTRL - bit 14 is the master mute bit */  
  0x0000,            /* 0x14 - UDA_MIXER_FILTER_CTRL */
};

/* Decimator Register Data Set */
unsigned short UDA_decimator_regs_dat[]={
  
  0x0000,            /* UDA_DEC_VOL_CTRL */  
  0x0000,            /* UDA_PGA_CTRL */  
  0x0000,            /* UDA_ADC_CTRL */  
  0x0000             /* UDA_AGC_CTRL */
};


/***********************************************************************
 * Function: delay
 *
 * Parameters:  cnt : number to be counted
 *    
 **********************************************************************/
void delay(UNS_32 cnt)
{
    UNS_32 i = cnt;
    while (i != 0) i--;
    return;
}


/***********************************************************************
 *  UDA1380 register read function 
 *
 *
 *
 **********************************************************************/
unsigned short UDA_Reg_read(UDA1380_REG reg){
  I2C_M_SETUP_Type transferMCfg;
  unsigned char TXdata, RXdata[2];
  unsigned short result=0;

	TXdata = reg;
    transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
    transferMCfg.tx_data = &TXdata;
    transferMCfg.tx_length = 1;
    transferMCfg.rx_data = &RXdata[0];
    transferMCfg.rx_length = 2;
    transferMCfg.retransmissions_max = 5;

    I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING);

    result = RXdata[0]<<8 | RXdata[1];
	return result;
}



/***********************************************************************
 *  UDA register write function 
 *
 *
 *
 **********************************************************************/
void UDA_Reg_write(UDA1380_REG reg, unsigned short value){
  I2C_M_SETUP_Type transferMCfg;
  UNS_8  tx_data[3];

  tx_data[0] = reg;
  tx_data[1] = value>>8;
  tx_data[2] = value&0xFF;
  transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
  transferMCfg.tx_data = &tx_data[0];
  transferMCfg.tx_length = 3;
  transferMCfg.rx_data = NULL;
  transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 5;

  I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING);
  delay(10000);
}



/***********************************************************************
 *  UDA1380 init function 
 *
 *
 *
 **********************************************************************/
BOOL_32 UDA1380_init()
{
  unsigned short temp;
  int i;
  
  /* Soft-reset device */
  UDA_Reg_write(0x7F, 0x00);
 
  /* Sys regs init */
  UDA_Reg_write(UDA_EVALM_CLK , 0x0502);
  for(i  = 1; i < 5; i++)
  {
    UDA_Reg_write(UDA_EVALM_CLK + i, UDA_sys_regs_dat[i]);
    temp = UDA_Reg_read(UDA_EVALM_CLK + i);
    if(temp!=UDA_sys_regs_dat[i]) return FALSE;
  }
  
  /* interfilter regs init */
  for(i = 0; i < 5; i++)
  {
    UDA_Reg_write(UDA_MASTER_VOL_CTRL + i, UDA_interfil_regs_dat[i]);
    temp = UDA_Reg_read(UDA_MASTER_VOL_CTRL + i);
    if(temp!=UDA_interfil_regs_dat[i]) return FALSE;
  }

  /* decimator regs init */
  for(i = 0; i < 4; i++)
  {
    UDA_Reg_write(UDA_DEC_VOL_CTRL + i, UDA_decimator_regs_dat[i]);
    temp = UDA_Reg_read(UDA_DEC_VOL_CTRL + i);
    if(temp!=UDA_decimator_regs_dat[i]) return FALSE;
  }
	UDA_Reg_write(UDA_EVALM_CLK, UDA_sys_regs_dat[0]);

  return TRUE;
}
    


/**********************************************************************
 * @brief		Audio init
 * 
 * 
 **********************************************************************/
void Audio_Init (void) 
{                      

    // Configure reset signal for UDA1380 on Hitex A4 board
	scu_pinmux(0xE, 9, MD_PDN, FUNC4); // PE_9 = GPIO7[9]  
    LPC_GPIO_PORT->DIR[7] |= 1 << 9;   // output
    LPC_GPIO_PORT->CLR[7] |= 1 << 9;   // set to LOW, the reset input of the UDA1380 is high active
  

    // Hardware reset to the UDA1380
    delay (100000);
    LPC_GPIO_PORT->SET[7] |= 1 << 9;   // set to HIGH
    delay (1000000);
    LPC_GPIO_PORT->CLR[7] |= 1 << 9;   // set to LOW
    delay (100000); 


    // Init I2C interface
	I2C_Init(I2CDEV_M, 100000);
	// Enable I2C operation
	I2C_Cmd(I2CDEV_M, ENABLE);

    // Init UDA1380 CODEC
	UDA1380_init();

    // Power everything up
    UDA_Reg_write(UDA1380_REG_PWRCTRL, (1<<15 | 1 <<13 | 1<<10 | 1<<8));

    // Maybe insert a delay here, because the UDA1380 needs some time to sync on the signals. 
    // There are distortions in the first 0.5s of audio.

    // Disable master mute
    UDA_Reg_write(UDA1380_REG_MSTRMUTE, 0xA02);

}


/**********************************************************************
* $Id$		I2s_Audio.c					2011-06-02
*//**
* @file		I2s_Audio.c
* @brief	This example describes how to use I2S to play a short demo
*			audio data on LPC18xx_LPC43xx
* @version	1.0
* @date		02. June. 2011
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
#include "lpc43xx_cgu.h"
#include "lpc43xx_i2s.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_scu.h"
#include "libcfg.h"
#include "uda1380.h"
#include "lpc43xx_gpio.h"
#include "string.h"


/* Example group ----------------------------------------------------------- */
/** @defgroup I2s_Audio	I2s_Audio
 * @ingroup I2S_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
/** Max buffer length */
#define DATA_SIZE			15658
#define BUFFER_SIZE			1048 * 4
#define I2CDEV_UDA1380_ADDR		(0x34>>1)
#define I2CDEV_M LPC_I2C0


/************************** PRIVATE VARIABLES ***********************/
extern unsigned char audio[];
uint8_t tx_buffer[BUFFER_SIZE];
uint32_t data_offset, buffer_offset,remain_data;
uint32_t tx_offset = 0;
Bool Tx_EndofBuf_F = FALSE;

/************************** PRIVATE FUNCTIONS *************************/
void I2S_Callback(void);
void I2S0_IRQHandler(void);
void Buffer_Init(void);

/*********************************************************************//**
 * @brief		I2S callback function, will be call when I2S send a half of
 				buffer complete
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void I2S_Callback(void)
{
	if(remain_data >=BUFFER_SIZE/2)
	{
		if(buffer_offset == BUFFER_SIZE)
		{
			// copy audio data into remain half of tx_buffer
			memcpy(tx_buffer + BUFFER_SIZE/2, audio+data_offset, BUFFER_SIZE/2);
			buffer_offset = 0;
		}
		else
			// copy audio data into remain half of tx_buffer
			memcpy(tx_buffer, audio+data_offset, BUFFER_SIZE/2);
			data_offset += BUFFER_SIZE/2;
			remain_data -= BUFFER_SIZE/2;
	}
	else // reach the last copy
	{
		if(buffer_offset == BUFFER_SIZE)
		{
			// copy audio data into remain half of tx_buffer
			memcpy(tx_buffer + BUFFER_SIZE/2, audio+data_offset, remain_data);
			buffer_offset = 0;
		}
		else
			// copy audio data into remain half of tx_buffer
			memcpy(tx_buffer, audio+data_offset, remain_data);

	}
}
/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		I2S IRQ Handler, call to send data to transmit buffer
 * @param[in]	None
 * @return 		None
 **********************************************************************/
volatile uint32_t txDone = 1;

void I2S0_IRQHandler()
{
	uint32_t txlevel,i;
	txlevel = I2S_GetLevel(LPC_I2S0,I2S_TX_MODE);
	if(txlevel <= 4)
	{
		for(i=0;i<8-txlevel;i++)
		{
			LPC_I2S0->TXFIFO = *(uint32_t *)(tx_buffer + buffer_offset);
			tx_offset +=4;
			buffer_offset +=4;
			//call I2S_Callback() function when reach each half of tx_buffer
			if((buffer_offset == BUFFER_SIZE/2)||(buffer_offset == BUFFER_SIZE))
				I2S_Callback();
			if(tx_offset >= DATA_SIZE)//reach the end of buffer
			{
				tx_offset = 0;
				Buffer_Init();
				txDone = 1;
				NVIC_DisableIRQ(I2S0_IRQn);
				I2S_Stop(LPC_I2S0, I2S_TX_MODE);
			}
		}
	}
}


/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Initialize transmit buffer
 * @param[in]	none
 * @return 		None
 **********************************************************************/
void Buffer_Init(void)
{
	memcpy(tx_buffer, audio, BUFFER_SIZE);
	buffer_offset = 0;
	data_offset = BUFFER_SIZE;
	remain_data = DATA_SIZE - BUFFER_SIZE;
}

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
}UDA1380_REG;

/* System Register Data Set */
unsigned short UDA_sys_regs_dat[]={
  /* UDA_EVALM_CLK */
  0xF <<8 | 0x3<<4 | 1,
  /* UDA_BUS_CTRL */
  0x00,
  /* UDA_POWER_CTRL */
  1<<15 | 1 <<13 | 1<<10 | 1<<8,
  /* UDA_ANALOG_CTRL */
  0x0000,
  /* UDA_HPAMP_CTRL */
  1<<9 | 2
};

/* System Register Data Set */
unsigned short UDA_interfil_regs_dat[]={
  /* UDA_MASTER_VOL_CTRL */
  0x0000, /* MAX volume */
  /* UDA_MIXER_VOL_CTRL */
  0x0000,
  /* UDA_MODE_CTRL */
  0x0000,
  /* UDA_MUTE_CTRL */
  2<<8 | 2,
  /* UDA_MIXER_FILTER_CTRL */
  0x0000,
};
/* decimator Register Data Set */
unsigned short UDA_decimator_regs_dat[]={
  /* UDA_DEC_VOL_CTRL */
  0x0000,
  /* UDA_PGA_CTRL */
  0x0000,
  /* UDA_ADC_CTRL */
  0x0000,
  /* UDA_AGC_CTRL */
  0x0000

};


/***********************************************************************
 *
 * Function: delay
 *
 * Purpose: generate a delay
 *
 * Processing:
 *     A local software counter counts up to the specified count.
 *
 * Parameters:
 *    cnt : number to be counted
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void delay(UNS_32 cnt)
{
    UNS_32 i = cnt;
    while (i != 0) i--;
    return;
}
/* UDA register read function */
unsigned short UDA_Reg_read(UDA1380_REG reg){
  I2C_M_SETUP_Type transferMCfg;
  unsigned char TXdata, RXdata[2];
  unsigned short result=0;
	TXdata = reg;
//    transferMCfg.addr_mode = ADDR7BIT;
    transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
    transferMCfg.tx_data = &TXdata;
    transferMCfg.tx_length = 1;
    transferMCfg.rx_data = &RXdata[0];
    transferMCfg.rx_length = 2;
    transferMCfg.retransmissions_max = 5;
    I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING);
    //while (( i2c_mtxrx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
    result = RXdata[0]<<8 | RXdata[1];
	return result;
}

/* UDA register write function */
void UDA_Reg_write(UDA1380_REG reg, unsigned short value){
  I2C_M_SETUP_Type transferMCfg;
  UNS_8  tx_data[3];
  tx_data[0] = reg;
  tx_data[1] = value>>8;
  tx_data[2] = value&0xFF;
//  i2c_mtx_setup.addr_mode = ADDR7BIT;
  transferMCfg.sl_addr7bit = I2CDEV_UDA1380_ADDR;
  transferMCfg.tx_data = &tx_data[0];
  transferMCfg.tx_length = 3;
  transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
  transferMCfg.retransmissions_max = 5;
  I2C_MasterTransferData(I2CDEV_M, &transferMCfg, I2C_TRANSFER_POLLING);
  //while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
  delay(10000);
}

BOOL_32 UDA1380_init(){
  unsigned short temp;
  int i;
  LPC_CGU->IDIVC_CTRL = (0x8<<24) | (7<<2) |(1<<11);
  LPC_CGU->BASE_OUT_CLK = 0x0D<<24 | (1<<11);
  LPC_SCU->SFSCLK_3 = 1 | (0x3<<3);

  /* Reset device */
  UDA_Reg_write((UDA1380_REG)0x7F, 0x00);

#if 0
  UDA_Reg_write(UDA1380_REG_EVALCLK,REG_EVALCLK_DEF);
  UDA_Reg_write(UDA1380_REG_I2S,REG_I2S_DEF);
  /* WSPLL on, dac on, bias on, PGAL on, ADCL on, PGAR on, ADCR on, LNA on */
  UDA_Reg_write(UDA1380_REG_PWRCTRL, REG_PWRCTRL_DEF);
  UDA_Reg_write(UDA1380_REG_ANAMIX, REG_ANAMIX_DEF);
  UDA_Reg_write(UDA1380_REG_HEADAMP, REG_HEADAMP_DEF);
  UDA_Reg_write(UDA1380_REG_MSTRVOL, REG_MSTRVOL_DEF);
  UDA_Reg_write(UDA1380_REG_MIXVOL, REG_MIXVOL_DEF);
  UDA_Reg_write(UDA1380_REG_MODEBBT, REG_MODEBBT_DEF);
  UDA_Reg_write(UDA1380_REG_MSTRMUTE, REG_MSTRMUTE_DEF);
  UDA_Reg_write(UDA1380_REG_MIXSDO, REG_MIXSDO_DEF);
  UDA_Reg_write(UDA1380_REG_DECVOL, REG_DECVOL_DEF);
  UDA_Reg_write(UDA1380_REG_PGA, 0x0000); 	/* decimator not muted */
  UDA_Reg_write(UDA1380_REG_ADC, REG_ADC_DEF);
  UDA_Reg_write(UDA1380_REG_AGC, REG_AGC_DEF);

  /* Decimator enabled, FSDAC enabled, Interpolator clock enabled, DAC clock = WSPLL, WSPLL=512*fs */
  UDA_Reg_write(UDA1380_REG_EVALCLK,
        (1<<11)|(1<<10)|(1<<9)|(1<<8)|(1<<5)|(1<<4)|(0x2<<0));
#else
    /* Sys regs init */
	UDA_Reg_write(UDA_EVALM_CLK , 0x0502);
  for(i=1; i<5; i++){
    UDA_Reg_write((UDA1380_REG)(UDA_EVALM_CLK + i), UDA_sys_regs_dat[i]);
    temp = UDA_Reg_read((UDA1380_REG)(UDA_EVALM_CLK + i));
    if(temp!=UDA_sys_regs_dat[i]) return FALSE;
  }

  /* interfilter regs init */
  for(i=0; i<5; i++){
    UDA_Reg_write((UDA1380_REG)(UDA_MASTER_VOL_CTRL + i), UDA_interfil_regs_dat[i]);
    temp = UDA_Reg_read((UDA1380_REG)(UDA_MASTER_VOL_CTRL + i));
    if(temp!=UDA_interfil_regs_dat[i]) return FALSE;
  }
  /* decimator regs init */
  for(i=0; i<4; i++){
    UDA_Reg_write((UDA1380_REG)(UDA_DEC_VOL_CTRL + i), UDA_decimator_regs_dat[i]);
    temp = UDA_Reg_read((UDA1380_REG)(UDA_DEC_VOL_CTRL + i));
    if(temp!=UDA_decimator_regs_dat[i]) return FALSE;
  }
	UDA_Reg_write(UDA_EVALM_CLK, UDA_sys_regs_dat[0]);
#endif

  return TRUE;
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry (void) {                       /* Main Program */

/* 		This code will set I2S in MASTER mode, 44100Hz, Mono,16bit
 * 		This example is used to test I2S transmit mode. There is an audio
 * 		data in audiodata.c in mono 44100Hz 16 bit, it will be send out to
 * 		I2S port. User must plug Ext I2S DAC IC to hear sound.
 */
	I2S_MODEConf_Type I2S_ClkConfig;
	I2S_CFG_Type I2S_ConfigStruct;
//	uint32_t i;

	SystemInit();
	CGU_Init();
	Buffer_Init();

	/* Reset UDA1380 on board Hitex */
	scu_pinmux(8,2,MD_PUP, FUNC0);
	GPIO_SetDir(4, 1<<2, 1);
	GPIO_ClearValue(4, 1<<2);

/* Initialize I2S peripheral ------------------------------------*/
	/* Init I2C */
	I2C_Init(I2CDEV_M, 100000);
	/* Enable Slave I2C operation */
	I2C_Cmd(I2CDEV_M, ENABLE);
	/* Init UDA1380 CODEC */
	UDA1380_init();
/* Initialize I2S peripheral ------------------------------------*/
	scu_pinmux(3,0, MD_PLN_FAST, 2);
	scu_pinmux(6,0, MD_PLN_FAST, 0);

	scu_pinmux(7,2, MD_PLN_FAST, 2);
	scu_pinmux(6,2, MD_PLN_FAST, 3);

	scu_pinmux(7,1, MD_PLN_FAST, 2);
	scu_pinmux(6,1, MD_PLN_FAST, 3);

	I2S_Init(LPC_I2S0);

	/* setup:
	 * 		- wordwidth: 16 bits
	 * 		- stereo mode
	 * 		- master mode for I2S_TX
	 * 		- Frequency = 44.1 kHz
	 */

	/* Audio Config*/
	I2S_ConfigStruct.wordwidth = I2S_WORDWIDTH_16;
	I2S_ConfigStruct.mono = I2S_MONO;
	I2S_ConfigStruct.stop = I2S_STOP_ENABLE;
	I2S_ConfigStruct.reset = I2S_RESET_ENABLE;
	I2S_ConfigStruct.ws_sel = I2S_MASTER_MODE;
	I2S_ConfigStruct.mute = I2S_MUTE_DISABLE;
	I2S_Config(LPC_I2S0,I2S_TX_MODE,&I2S_ConfigStruct);

	/* Clock Mode Config*/
	I2S_ClkConfig.clksel = I2S_CLKSEL_FRDCLK;
	I2S_ClkConfig.fpin = I2S_4PIN_DISABLE;
	I2S_ClkConfig.mcena = I2S_MCLK_DISABLE;
	I2S_ModeConfig(LPC_I2S0,&I2S_ClkConfig,I2S_TX_MODE);

	I2S_FreqConfig(LPC_I2S0, 48000, I2S_TX_MODE);

	I2S_Stop(LPC_I2S0, I2S_TX_MODE);

	/* TX FIFO depth is 4 */
	I2S_IRQConfig(LPC_I2S0,I2S_TX_MODE,4);
	I2S_IRQCmd(LPC_I2S0,I2S_TX_MODE,ENABLE);

	NVIC_ClearPendingIRQ(I2S0_IRQn);

	I2S_Start(LPC_I2S0);
	NVIC_EnableIRQ(I2S0_IRQn);

				
	/* repeat the sound in a loop */
	while(1) {

		if(txDone) {
			txDone = 0;

			NVIC_ClearPendingIRQ(I2S0_IRQn);

			I2S_Start(LPC_I2S0);

			NVIC_EnableIRQ(I2S0_IRQn);
		}
	}

}

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}
#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
 */

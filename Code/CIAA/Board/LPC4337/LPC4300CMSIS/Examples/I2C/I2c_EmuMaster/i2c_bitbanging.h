/**********************************************************************
* $Id$		i2c_bitbanging.h	2012-05-15
*//**
* @file		i2c_bitbanging.h
* @brief	Functions to control GPIO pins as I2C pins
* @version	1.0
* @date		15. May 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
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
* is used in conjunction with NXP Semiconductors microcontrollers.  
* This copyright, permission, and disclaimer notice must appear in all 
* copies of this code.
**********************************************************************/
#ifndef __I2CBITBANGING_H__
#define __I2CBITBANGING_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
	
	
/* The INNOLUX module includes a PCA9532 I2C port expander to control the signals 
   for power, backlight and contrast.
	 Pins #13 and #14 of the display connector are supposed to be connected to I2C.
	 At the LCD connector on the Hitex board there is no hardware I2C bus connected,
	 but the two signals connected to pins #13 and #14 can be configured as GPIOs 
	 to perform an I2C emulation.
	 LCDVD9 = P4_8 = GPIO5_12 (function 4) = SDA                                 
	 LCDVD8 = P7_5 = GPIO3_13 (function 0) = SCL 
 */ 
#define SDA_GPIOPORT     (5)
#define SDA_GPIOPIN      (12)
#define SCL_GPIOPORT     (3)
#define SCL_GPIOPIN      (13)
	
/* Macros for write/read of the I2C emulation pins                             */
#define SET_SCL_HIGH     (LPC_GPIO_PORT->DIR[SCL_GPIOPORT] &= ~(1 << SCL_GPIOPIN))
#define SET_SCL_LOW      (LPC_GPIO_PORT->DIR[SCL_GPIOPORT] |=  (1 << SCL_GPIOPIN))
#define SET_SDA_HIGH     (LPC_GPIO_PORT->DIR[SDA_GPIOPORT] &= ~(1 << SDA_GPIOPIN))
#define SET_SDA_LOW      (LPC_GPIO_PORT->DIR[SDA_GPIOPORT] |=  (1 << SDA_GPIOPIN))
#define READ_SCL         ((LPC_GPIO_PORT->PIN[SCL_GPIOPORT] & (1 << SCL_GPIOPIN)) >> SCL_GPIOPIN)
#define READ_SDA         ((LPC_GPIO_PORT->PIN[SDA_GPIOPORT] & (1 << SDA_GPIOPIN)) >> SDA_GPIOPIN)
 



void i2cemu_init (void);
void i2cemu_deinit (void);
void i2cemu_start (void);
void i2cemu_stop (void);
bool i2cemu_send_byte (uint8_t data);
uint8_t i2cemu_receive_byte (bool send_ack);

#ifdef __cplusplus
}
#endif

#endif

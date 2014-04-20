/**********************************************************************
* $Id$		i2c_bitbanging.c	2012-05-15
*//**
* @file		i2c_bitbanging.c
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

/** Basic function set to talk to an I2C slave device using 2 GPIOs. */

#include "lpc43xx.h"
#include "pca9532.h"
#include "i2c_bitbanging.h"




/** Constant that determines the I2C bit rate. (Higher value --> slower rate). */
/* Execution from RAM is faster than from Flash, SPIFI exution differs from    */
/* parallel flash execution --> find the right frequency by yourself.          */
/*                                                                             */
/* 192MHz core - running from Flash - 1000:  2.6kHz as I2C SCL                 */
/* 192MHz core - running from Flash -  100: 25.5kHz as I2C SCL                 */
/* 192MHz core - running from Flash -   10:  182kHz as I2C SCL                 */
#define I2CEMU_BIT_DELAY        (40)

/** Constant that determines an I2C timeout (Higher value --> longer timeout). */
#define I2CEMU_TIMEOUT          (80)



/** I2C emulation functions. */

/*************************************************************************************
 *  Software delay that determines the I2C bit rate.
 *
 *
 *
 *************************************************************************************/
void i2cemu_delay (uint8_t multi)
{
    volatile uint16_t i;

    while (multi)
    {
        /* A bit of delay... */
        for (i = 0; i < I2CEMU_BIT_DELAY; i++)
        {
        }
        multi--;
    }
}



/*************************************************************************************
 *  i2cemu_init
 *
 *  Configure the 2 GPIO port pins required for the I2C emulation
 *
 *************************************************************************************/
void i2cemu_init (void)
{  	
	  LPC_GPIO_PORT->CLR[SDA_GPIOPORT] |= (1 << SDA_GPIOPIN);       // preset output register to 0
    LPC_GPIO_PORT->CLR[SCL_GPIOPORT] |= (1 << SCL_GPIOPIN);       // preset output register to 0
	
	  LPC_GPIO_PORT->DIR[SDA_GPIOPORT] &= ~(1 << SDA_GPIOPIN);      // set to input
	  LPC_GPIO_PORT->DIR[SCL_GPIOPORT] &= ~(1 << SCL_GPIOPIN);      // set to input
}



/*************************************************************************************
 *  i2cemu_deinit
 *
 *  Configure the 2 GPIO port pins as input
 *
 *************************************************************************************/
void i2cemu_deinit (void)
{ 		
	  LPC_GPIO_PORT->DIR[SDA_GPIOPORT] &= ~(1 << SDA_GPIOPIN);      // set to input
	  LPC_GPIO_PORT->DIR[SCL_GPIOPORT] &= ~(1 << SCL_GPIOPIN);      // set to input
	
}



/*************************************************************************************
 *  Send a start condition.
 *
 *
 *
 *************************************************************************************/
void i2cemu_start (void)
{  
	  //LPC_GPIO_PORT->DIR[3] &= ~(1 << 13);   // (keep)/drive SCL to 1 --> input	  
	  SET_SCL_HIGH;
    i2cemu_delay(1);
	  //LPC_GPIO_PORT->DIR[5] |= (1 << 12);    // drive SDA to 0 --> output 
	  SET_SDA_LOW;
    i2cemu_delay(1);
    
}


/*************************************************************************************
 *  Send a stop condition.
 *
 *
 *
 *************************************************************************************/
void i2cemu_stop (void)
{ 
	  //LPC_GPIO_PORT->DIR[5] |= (1 << 12);    // drive SDA to 0
	  SET_SDA_LOW;
    i2cemu_delay(1);
    //LPC_GPIO_PORT->DIR[3] &= ~(1 << 13);   // drive SCL to 1
	  SET_SCL_HIGH;
    i2cemu_delay(1);
    //LPC_GPIO_PORT->DIR[5] &= ~(1 << 12);    // drive SDA to 1
	  SET_SDA_HIGH;
    i2cemu_delay(1);
}


/*************************************************************************************
 *  Send one byte, and return the answer (ACK/NAK).
 *
 *  Parameter:  data Byte to be sent
 *  Return:     true=ACK, false=NAK
 *
 *************************************************************************************/
bool i2cemu_send_byte (uint8_t data)
{
    bool result = false;
    volatile uint32_t i;
    uint32_t timeout;


    /* Send 8 bits plus the clock to request the acknowledge. */
    for (i = 0; i < 9; i++)
    {
        //LPC_GPIO_PORT->DIR[3] |= (1 << 13);     // drive SCL to 0
			  SET_SCL_LOW;
        i2cemu_delay(1);
			  if (data & 0x80)
				{
					//LPC_GPIO_PORT->DIR[5] &= ~(1 << 12);   // drive SDA to 1
					SET_SDA_HIGH;
				}
				else
				{
					//LPC_GPIO_PORT->DIR[5] |= (1 << 12);   // drive SDA to 0
					SET_SDA_LOW;
				}
        data = (data << 1) | 1;              // (Make sure the nineth bit is a 1!)
        i2cemu_delay(1);

        //LPC_GPIO_PORT->DIR[3] &= ~(1 << 13);     // drive SCL to 1
				SET_SCL_HIGH;
        i2cemu_delay(2);
    }

    /* Read acknowledge bit */
    timeout = I2CEMU_TIMEOUT;
    do {
        /* Possible clock stretching ended? */
        //if (LPC_GPIO_PORT->PIN[3] & (1 << 13))   // Check if SCL is 1
				if (READ_SCL)   // Check if SCL is 1
        {
            /* ACK status is valid now */
            //if (!(LPC_GPIO_PORT->PIN[5] & (1 << 12)))   // Check if SDA is 0
					  if (!READ_SDA)
            {
                result = true;
                break;
            }
        }
        else
        {
            i2cemu_delay(1);
            if (timeout)
            {
                timeout--;
            }
        }
    } while (timeout);

    /* SCL=0 ends transfer of this byte. */
    //LPC_GPIO_PORT->DIR[3] |= (1 << 13);   // drive SCL to 0
		SET_SCL_LOW;
    i2cemu_delay(1);

    return result;
}



/*************************************************************************************
 *  Receive one byte, and send ACK.
 *
 *  Return:  Received byte
 *
 *************************************************************************************/
uint8_t i2cemu_receive_byte (bool send_ack)
{
    uint8_t data = 0;
    uint32_t i;
    uint32_t timeout;

    /* Receive 8  bits. */
    for (i = 0; i < 8; i++)
    {
      i2cemu_delay(1);  
			  SET_SCL_LOW;
        i2cemu_delay(1);
        SET_SCL_HIGH;
        timeout = I2CEMU_TIMEOUT;
        do {
            /* Possible clock stretching ended? */
            if (READ_SCL)
            {
                /* ACK status is valid now */
                data = (data << 1) | (READ_SDA);
                break;
            }
            else
            {
                i2cemu_delay(1);
                if (timeout)
                {
                    timeout--;
                }
            }
        } while (timeout);
    }

    /* Send acknowledge */
    SET_SCL_LOW;
    i2cemu_delay(1);
		if (send_ack)
		{
			SET_SDA_LOW;
		}
		else
		{
			SET_SDA_HIGH;
		}
    i2cemu_delay(1);
    SET_SCL_HIGH;
    i2cemu_delay(2);
    SET_SCL_LOW;
    SET_SDA_HIGH;
    i2cemu_delay(1);

    return (data);
}





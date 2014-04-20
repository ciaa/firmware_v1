/**********************************************************************
* $Id$		pca9532.c	 2012-05-15
*//**
* @file		pca9532.c
* @brief	Functions to talk to a PCA9532 port expander using GPIOs
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
 
#include "lpc43xx.h"
#include "pca9532.h"
#include "i2c_bitbanging.h"



/*************************************************************************************
 *  PCA9532_Init
 *
 *  Initialize the PC9532 port expander on the INNOLUX display module in order to 
 *  switch the TFT on.
 *
 *************************************************************************************/
void pca9532_init( void )
{
	
	uint8_t i2cbytes[2];
	
	LPC_GPIO_PORT->SET[3] |= 1 << 11;   // Release PCA9532 from reset
	
	// Write to PCA9532  
	// 1. byte:  0x10 | PCA9532_REG_LS0;
	// 2. byte:  PCA9532_PIN0_LOW | PCA9532_PIN1_LOW;
	i2cbytes[0] = (uint8_t)(PCA9532_PIN0_LOW | PCA9532_PIN1_LOW);
	pca9532_write_reg ((uint8_t)PCA9532_REG_LS0, 1, i2cbytes);
	
	// Write to PCA9532 
	// 1. byte:  0x10 | PCA9532_REG_LS1;
	// 2. byte:  PCA9532_PIN4_HIGH | PCA9532_PIN6_HIGH;
	i2cbytes[0] = (uint8_t)(PCA9532_PIN4_HIGH | PCA9532_PIN6_HIGH);
	pca9532_write_reg ((uint8_t)PCA9532_REG_LS1, 1, i2cbytes);
	
	// Write to PCA9532 
	// 1. byte:  0x10 | PCA9532_REG_LS2;
	// 2. byte:  PCA9532_PIN8_HIGH;
	i2cbytes[0] = (uint8_t)(PCA9532_PIN8_HIGH);
	pca9532_write_reg ((uint8_t)PCA9532_REG_LS2, 1, i2cbytes);

}


/*************************************************************************************
 *  pca9532_deinit
 *
 *  De-initialize the PC9532 port expander on the INNOLUX display module in order to 
 *  switch the TFT off.
 *
 *************************************************************************************/
void pca9532_deinit( void )
{
	
		LPC_GPIO_PORT->CLR[3] |= 1 << 11;   // Put the PCA9532 into reset

}




/*************************************************************************************
 *  Read one or more registers.
 *
 *  Parameters:  regnum   Index of first register
 *               length   Number of registers to read
 *               value    Pointer to result
 *  Return:      true=Transfer OK, false=Transfer error
 *
 *************************************************************************************/
bool pca9532_read_reg (uint8_t regnum, uint8_t length, uint8_t value[])
{
    bool result = false;
    uint32_t i;

    i2cemu_start();   /* START bit */
 
    if (i2cemu_send_byte ((I2C_ADDRESS_PCA9532 << 1) | 0))   /* Address byte */
    {
        if (i2cemu_send_byte (regnum))
        {            
            i2cemu_stop();   /* STOP bit */            
            i2cemu_start();  /* START bit */
           
            if (i2cemu_send_byte ((I2C_ADDRESS_PCA9532 << 1) | 1))   /* Address byte */
            {
                for (i = 0; i < length; i++)
                {
                    /* Reply with ACK, except for the last byte. */
                    value[i] = i2cemu_receive_byte(i < length-1);
                }
            }
            result = true;
        }
    }
		
    i2cemu_stop();   /* STOP bit */

    return result;
}



/*************************************************************************************
 *  Write one or more registers.
 *
 *  Parameters:  regnum   Index of first register
 *               length   Number of registers to read
 *               value    Pointer to data
 *  Return:      true=Transfer OK, false=Transfer error
 *    
 *************************************************************************************/
bool pca9532_write_reg (uint8_t regnum, uint8_t length, uint8_t value[])
{
    bool result = false;
    uint32_t i;
    
    i2cemu_start();   /* START bit */
    
    if (i2cemu_send_byte ((I2C_ADDRESS_PCA9532 << 1) | 0))   /* Address byte */
    {
        if (i2cemu_send_byte (regnum))
        {
            result = true;

            for (i = 0; i < length; i++)
            {
                if (!i2cemu_send_byte(value[i]))
                {
                    result = false;
                    break;
                }
            }
        }
    }

    i2cemu_stop();   /* STOP bit */

    return result;
}

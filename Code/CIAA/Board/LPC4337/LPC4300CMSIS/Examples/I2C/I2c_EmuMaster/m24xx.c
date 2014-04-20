/**********************************************************************
* $Id$		m24xx.c	 2012-05-15
*//**
* @file		m24xx.c
* @brief	Functions to talk to a m24xx EEPROM using GPIOs
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
#include "m24xx.h"
#include "i2c_bitbanging.h"



/*************************************************************************************
 *  Read one or more values from the EEPROM.
 *  
 *
 *  Parameters:  start    Start address
 *               length   Number of values to read
 *               value    Pointer to result
 *  Return:      true=Transfer OK, false=Transfer error
 *
 *************************************************************************************/
bool m24xx_eeprom_read (uint8_t start, uint8_t length, uint8_t value[])
{
    bool result = false;
    uint32_t i;

    i2cemu_start();   /* START bit */
 
    if (i2cemu_send_byte((I2C_ADDRESS_M24C64 << 1) | 0) )   /* Address byte */
    {
        if (i2cemu_send_byte (start))
        {  
          if (i2cemu_send_byte (start))
          {           
            i2cemu_stop();   /* STOP bit */ 					
            i2cemu_start();  /* START bit */
           
            if (i2cemu_send_byte ((I2C_ADDRESS_M24C64 << 1) | 1))   /* Address byte */
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
	  }
		
    i2cemu_stop();   /* STOP bit */

    return result;
}


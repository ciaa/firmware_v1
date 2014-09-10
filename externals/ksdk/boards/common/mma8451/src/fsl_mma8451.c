/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#include <string.h>
#include "mma8451/fsl_mma8451.h"


/*! @brief I2C related constants for the MMA8451.*/
enum _mma8451_i2c_constants
{
    kMMA8451_I2C_Address = 0x38,
    kMMA8451_I2C_Address_With_SAO_Set = 0x3a
};

/*******************************************************************************
 * Code
 ******************************************************************************/

uint8_t mma8451_read_register(const mma8451_device_t * device, uint8_t reg_addr)
{
    uint8_t buf[1];

    i2c_master_transfer(device->port, &device->address, kI2CRead, true, reg_addr, 1, buf, sizeof(buf), NULL, 0);

    return buf[0];
}

status_t mma8451_write_register(const mma8451_device_t * device, uint8_t reg_addr, uint8_t reg_val)
{
    uint8_t buf[1];
    buf[0] = reg_val;

    return i2c_master_transfer(device->port, &device->address, kI2CWrite, true, reg_addr, 1, buf, sizeof(buf), NULL, 0);
}

status_t mma8451_init(mma8451_device_t * device, i2c_master_t * master, bool isSaoSet, uint32_t baud_kbps)
{
    /* Init device struct.*/
    memset(device, 0, sizeof(*device));
    
    device->port = master;
    device->address.address = isSaoSet ? kMMA8451_I2C_Address_With_SAO_Set : kMMA8451_I2C_Address;
    device->address.baudRate_kbps = baud_kbps;
    
    unsigned char val = 0;
    
    /* Read WHO_AM_I register.*/
    val = mma8451_read_register(device, kMMA8451_WHO_AM_I);
    if (val != kMMA8451_WHO_AM_I_Device_ID)
    {
        /*debug_printf("MMA8451: Unexpected result from WHO_AM_I (0x%02x)\n", val);*/
        return kStatusFail;
    }

    /* Put the mma8451 into standby mode */
    val = mma8451_read_register(device, kMMA8451_CTRL_REG1);
    val &= ~(0x01U);
    mma8451_write_register(device, kMMA8451_CTRL_REG1, val);

    /* Set the range, -8g to 8g */
    val = mma8451_read_register(device, kMMA8451_XYZ_DATA_CFG);
    val &= ~0x03U;
    val |= 0x02;
    mma8451_write_register(device, kMMA8451_XYZ_DATA_CFG, val);

    /* Set the F_MODE, disable FIFO */
    val = mma8451_read_register(device, kMMA8451_F_SETUP);
    val &= 0x3F;
    mma8451_write_register(device, kMMA8451_F_SETUP, val);

    /* Put the mma8451 into active mode */
    val = mma8451_read_register(device, kMMA8451_CTRL_REG1);
    val |= 0x01;
    val &= ~0x02U;               /*set F_READ to 0*/
    mma8451_write_register(device, kMMA8451_CTRL_REG1, val);
    
    return kStatus_Success;
}

status_t mma8451_get_acceleration(const mma8451_device_t * device, acceleration_t * accel)
{
    if (!accel)
    {
        return 0;
    }
    
    uint8_t ucVal1 = 0;
    uint8_t ucVal2 = 0;
    uint8_t ucStatus = 0;
    uint16_t iTemp = 0;
    int sign = 1;
/*     char signch = '+';*/
    
    do {
        ucStatus = mma8451_read_register(device, kMMA8451_STATUS);
    } while (!(ucStatus & 0x08));

    ucVal1 = mma8451_read_register(device, kMMA8451_OUT_X_MSB);
    ucVal2 = mma8451_read_register(device, kMMA8451_OUT_X_LSB);
    sign = (ucVal1 & 0x80) ? (-1) : 1;
    iTemp = ((ucVal1 & 0x7FU) << 6) + ((ucVal2) >> 2);
    if (sign == -1)             /*2's completement*/
    {
        iTemp = (~iTemp + 1U) & 0x1FFFU;
    }
/*     signch = (sign == -1) ? '-' : '+';*/
    accel->x = (float)(iTemp * sign) * 8.0F / 8192.0F;
/*     printf("\r 	  The acceleration is: (%c%01d.%02dg, ",*/
/*            signch, iTemp * 8 / 0x2000, (iTemp & 0x3FF) * 800 / 0x2000);*/

    ucVal1 = mma8451_read_register(device, kMMA8451_OUT_Y_MSB);
    ucVal2 = mma8451_read_register(device, kMMA8451_OUT_Y_LSB);
    sign = (ucVal1 & 0x80) ? (-1) : 1;
    iTemp = ((ucVal1 & 0x7FU) << 6) + ((ucVal2) >> 2);
    if (sign == -1)             /*2's completement*/
    {
        iTemp = (~iTemp + 1U) & 0x1FFFU;
    }
/*     signch = (sign == -1) ? '-' : '+';*/
    accel->y = (float)(iTemp * sign) * 8.0F / 8192.0F;
/*     printf(" %c%01d.%02dg, ", signch, iTemp * 8 / 0x2000, (iTemp & 0x3FF) * 800 / 0x2000);*/

    ucVal1 = mma8451_read_register(device, kMMA8451_OUT_Z_MSB);
    ucVal2 = mma8451_read_register(device, kMMA8451_OUT_Z_LSB);
    sign = (ucVal1 & 0x80) ? (-1) : 1;
    iTemp = ((ucVal1 & 0x7FU) << 6) + ((ucVal2) >> 2);
    if (sign == -1)             /*2's completement*/
    {
        iTemp = (~iTemp + 1U) & 0x1FFFU;
    }
/*     signch = (sign == -1) ? '-' : '+';*/
    accel->z = (float)(iTemp * sign) * 8.0F / 8192.0F;
/*     printf(" %c%01d.%02dg)", signch, iTemp * 8 / 0x2000, (iTemp & 0x3FF) * 800 / 0x2000);*/

    return kStatus_Success;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

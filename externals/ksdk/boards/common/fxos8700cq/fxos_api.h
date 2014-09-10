/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
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
#ifndef _FXOS_API_H_
#define _FXOS_API_H_

#include "fsl_i2c_master_driver.h"
#include <stdio.h>
#include <stdint.h>

/*!
 * @addtogroup fxos_common
 * @{
 */

/*!
 * @file
 */
     
#define FXOS8700_I2C_INSTANCE   0
/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    /*Sensor data*/
    uint8_t accelXMSB;                      	
    uint8_t accelXLSB;                      	
    uint8_t accelYMSB;                      	
    uint8_t accelYLSB;                      	
    uint8_t accelZMSB;                      	
    uint8_t accelZLSB;
    uint8_t magXMSB;                        	
    uint8_t magXLSB;                        	
    uint8_t magYMSB;                        	
    uint8_t magYLSB;                        	
    uint8_t magZMSB;                        	
    uint8_t magZLSB;                        	
} fxos_sensorData_t;
/*
typedef struct
{
    void    (*i2c_init)(int32_t speed, int32_t slaveaddr);
    uint8_t (*i2c_probe)(uint8_t addr);
    uint8_t (*i2c_read) (uint8_t chip, uint8_t addr, int32_t alen, uint8_t *buf, int32_t len);
    uint8_t (*i2c_write)(uint8_t chip, uint8_t addr, int32_t alen, uint8_t *buf, int32_t len);
} fxos_i2c_interface_t;
*/
typedef struct
{
    void    (*i2c_init)(int32_t speed, int32_t slaveaddr);
    uint8_t (*i2c_probe)(uint8_t addr);
    i2c_status_t (*i2c_read) (uint32_t instance, i2c_device_t const *chip, uint8_t * addr, uint32_t alen, uint8_t *buf, uint32_t len, uint32_t timeout);
    i2c_status_t (*i2c_write)(uint32_t instance, i2c_device_t const *chip, uint8_t * addr, uint32_t alen, uint8_t *buf, uint32_t len, uint32_t timeout);
} fxos_i2c_interface_t;

typedef struct
{
    fxos_i2c_interface_t *i2c;
    i2c_device_t slave;
} fxos_dev_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Verify and initialize device: Hybrid mode with ODR=50Hz, Mag OSR=32, Acc OSR=Normal.
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 *
 * @return int32_t   True if all initializations are completed without.
 */
uint8_t fxos_init(fxos_dev_t *dev);

/*!
 * @brief Read data from sensors, assumes hyb_autoinc_mode is set in M_CTRL_REG2
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in] sensorData_t* sensorData    pointer to the buffer to hold sensor data
 *
 * @return     Init successfully or not.
 */
uint8_t fxos_read_sensor_data(fxos_dev_t *dev, fxos_sensorData_t *sensorData);

/*!
 * @brief Read data from sensors, assumes hyb_autoinc_mode is set in M_CTRL_REG2
 * and the fast-read mode is set in CTRL_REG1
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in] sensorData_t* sensorData    pointer to the buffer to hold sensor data
 *
 * @return     Init successfully or not.
 */
uint8_t fxos_read_sensor_data_fast(fxos_dev_t *dev, fxos_sensorData_t *sensorData);

/*!
 * @brief Read one byte from register
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in]  uint8_t  regAddr           address to read from
 * @param[out] uint8_t* outVal            Output register value.
 *
 * @return     int32_t Read successful or not. 
 */
uint8_t fxos_read_register(fxos_dev_t *dev, uint8_t regAddr, uint8_t *outVal);

/*!
 * @brief Read an array of registers
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in]  uint8_t  startRegAddr    Address of the device register to start reading from
 * @param[in]  uint8_t  len             Number of registers to be read 
 * @param[out] uint8_t* readBuf         Read buffer to read the data into, pass in by reference
 *
 * @return     int32_t Read succeeded or failed.            - 
 */
uint8_t fxos_read_register_array(fxos_dev_t *dev, uint8_t startRegAddr, uint8_t *readBuf, uint8_t len);

/*!
 * @brief Read an array of registers
 *
 * @param[in]  uint8_t  startRegAddr    Address of the device register to start reading from
 * @param[out] uint8_t* readBuf         Read buffer to read the data into, pass in by reference
 * @param[in]  uint8_t  len             Number of registers to be read 
 *
* @return     uint8_t Return 1 if successful, 0 if an error occurred. 
*/
uint8_t fxos_read_register_array_fast(fxos_dev_t *dev, uint8_t startRegAddr, uint8_t *readBuf, uint8_t len);

/*!
 * @brief Read data from sensors, assumes hyb_autoinc_mode is set in M_CTRL_REG2
 * and the fast-read mode is set in CTRL_REG1
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in] sensorData_t* sensorData    pointer to the buffer to hold sensor data
 *
 * @return     Init successfully or not.
 */
uint8_t fxos_read_sensor_data_fast(fxos_dev_t *dev, fxos_sensorData_t *sensorData);

/*!
 * @brief Read one byte from register
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in]  uint8_t regAddr           address to read from 
 *
 * @return     uint8_t value in register 
*/
uint8_t fxos_read_register(fxos_dev_t *dev, uint8_t regAddr, uint8_t *outVal);

/*!
 * @brief Write one byte to register
 *
 * @param[in]  dev - FXOS Device pointer to the communications interface. 
 * @param[in]  uint8_t regAddr           Address of the register to write to 
 * @param[in]  uint8_t writeValue        Byte value to write
 *
 * @return     int32_t Read succeeded or failed.            - 
 */
uint8_t fxos_write_register(fxos_dev_t *dev, uint8_t regAddr, uint8_t writeValue);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* _FXOS_API_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/

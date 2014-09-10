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

#include <assert.h>
#include <string.h>
#include "fsl_i2c_hal.h"
#include "fsl_i2c_slave_driver.h"
#include "fsl_i2c_shared_function.h"
#include "fsl_i2c_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
   
/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_SlaveInit
 * Description   : initializes the I2C module.
 * This function will save the application callback info, turn on the clock to the
 * module, enable the device and enable interrupts. Set the I2C to slave mode. 
 *
 *END**************************************************************************/
void I2C_DRV_SlaveInit(uint32_t instance, uint8_t address, i2c_slave_state_t * slave)
{
    assert(slave);
    assert(instance < HW_I2C_INSTANCE_COUNT);

    uint32_t baseAddr = g_i2cBaseAddr[instance];
   
    /* Exit if current instance is already initilized. */
    if (g_i2cStatePtr[instance])
    {
        return;
    }

    /* Init driver instance struct.*/
    memset(slave, 0, sizeof(*slave));

    /* Enable clock for I2C.*/
    CLOCK_SYS_EnableI2cClock(instance);

    /* Init instance to known state. */
    I2C_HAL_Init(baseAddr);

    /* Set slave address.*/
    I2C_HAL_SetAddress7bit(baseAddr, address);
   
    /* Save runtime structure poniter.*/
	g_i2cStatePtr[instance] = slave;   
 
    /* Enable I2C interrupt in the peripheral.*/
    I2C_HAL_SetIntCmd(baseAddr, true);    
        
    /* Enable I2C interrupt.*/
    INT_SYS_EnableIRQ(g_i2cIrqId[instance]);

    /* Enable the peripheral operation.*/
    I2C_HAL_Enable(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_SlaveDeinit
 * Description   : Shuts down the I2C slave driver.
 * This function will clear the control register and turn off the clock to the module. 
 *
 *END**************************************************************************/
void I2C_DRV_SlaveDeinit(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);

    uint32_t baseAddr = g_i2cBaseAddr[instance]; 
    
    /* Disable I2C interrupt. */
    I2C_HAL_SetIntCmd(baseAddr, false);  

    /* Turn off I2C.*/
    I2C_HAL_Disable(baseAddr);

    /* Disable clock for I2C.*/
    CLOCK_SYS_DisableI2cClock(instance);

    /* Disable I2C NVIC interrupt */
    INT_SYS_DisableIRQ(g_i2cIrqId[instance]);
 
    /* Clear runtime structure poniter.*/
	g_i2cStatePtr[instance] = NULL;  
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_SlaveInstallDataSource
 * Description   : Install slave data source callback function.
 *
 *END**************************************************************************/
i2c_slave_source_t I2C_DRV_SlaveInstallDataSource(uint32_t instance, i2c_slave_source_t function)
{
    /* Get current runtime structure. */
	i2c_slave_state_t * slave = (i2c_slave_state_t *)g_i2cStatePtr[instance];

    i2c_slave_source_t currentCallback = slave->dataSource;
    slave->dataSource = function;

    return currentCallback;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C_DRV_SlaveInstallDataSink
 * Description   : Install slave data sink callback function.
 *
 *END**************************************************************************/
i2c_slave_sink_t I2C_DRV_SlaveInstallDataSink(uint32_t instance, i2c_slave_sink_t function)
{
    /* Get current runtime structure. */
	i2c_slave_state_t * slave = (i2c_slave_state_t *)g_i2cStatePtr[instance];
    
    i2c_slave_sink_t currentCallback = slave->dataSink;
    slave->dataSink = function;

    return currentCallback;
}

/*!
 * @brief I2C Slave Generic IRQ handler.
 *
 * This handler implements the flow chart at the end of the I2C chapter in the Kinetis
 * KL25 Sub-Family Reference Manual. It uses callbacks to get/put data
 * from/to the application as well as alert the application of an error condition.
 *
 * @param instance Instance number of the I2C module.
 */
void I2C_DRV_SlaveIRQHandler(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);

    uint32_t baseAddr = g_i2cBaseAddr[instance]; 
    
    /* Clear I2C IRQ.*/
    I2C_HAL_ClearInt(baseAddr);

    /* Get current runtime structure. */
    bool doTransmit = false;
	i2c_slave_state_t * slave = (i2c_slave_state_t *)g_i2cStatePtr[instance];
    bool wasArbLost = I2C_HAL_GetStatusFlag(baseAddr, kI2CArbitrationLost);
    bool addressed = I2C_HAL_GetStatusFlag(baseAddr, kI2CAddressAsSlave);

    if (wasArbLost)
    {
        I2C_HAL_ClearArbitrationLost(baseAddr);
        if (!addressed)
        {
            slave->status = kStatus_I2C_AribtrationLost;
            return;
        }
    }

    /* Addressed only happen when receiving address. */
    if (addressed) /* Slave is addressed. */
    {
        /* Master read from Slave. Slave transmit.*/
        if (I2C_HAL_GetStatusFlag(baseAddr, kI2CSlaveTransmit))
        {
            /* Switch to TX mode*/
            I2C_HAL_SetDirMode(baseAddr, kI2CSend);

            doTransmit = true;
        }
        else /* Master write to Slave. Slave receive.*/
        {
            /* Switch to RX mode.*/
            I2C_HAL_SetDirMode(baseAddr, kI2CReceive);

            /* Read dummy character.*/
            I2C_HAL_ReadByte(baseAddr);
        }
    }
    else 
    {
        if (I2C_HAL_GetDirMode(baseAddr) == kI2CSend)
        {
            if (I2C_HAL_GetStatusFlag(baseAddr, kI2CReceivedNak))
            {
                /* Switch to RX mode.*/
                I2C_HAL_SetDirMode(baseAddr, kI2CReceive);

                /* Read dummy character.*/
                I2C_HAL_ReadByte(baseAddr);
            }
            else /* ACK from receiver.*/
            {
                /* DO TRANSMIT*/
                doTransmit = true;
            }
        }
        else
        {
            /* Get byte from data register.*/
            uint8_t sink_byte = I2C_HAL_ReadByte(baseAddr);

            if (slave->dataSink)
            {
                if (slave->dataSink(sink_byte) != kStatus_I2C_Success)
                {
                    /* Report the Slave RX Overrun error.*/
                    slave->status = kStatus_I2C_SlaveRxOverrun;
                }
            }
        }
    }
    
    /* DO TRANSMIT*/
    if (doTransmit)
    {
        uint8_t source_byte = 0x00U;
        if (slave->dataSource)
        {
            slave->status = slave->dataSource(&source_byte);
        }
        
        I2C_HAL_WriteByte(baseAddr, source_byte);
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/


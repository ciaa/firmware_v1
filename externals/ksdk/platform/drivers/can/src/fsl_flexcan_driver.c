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

#include "fsl_flexcan_driver.h"
#include "fsl_flexcan_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"
#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

bool int_mb;
uint32_t rx_mb_idx;
semaphore_t irqSync;
flexcan_mb_t *fifo_message;

/*! The following table is based on the assumption that BSP_CANPE_CLOCK is defined. The CAN bit*/
/*! timing parameters are calculated by using the method outlined in AN1798, section 4.1.*/
/*! A maximum time for PROP_SEG will be used, the remaining TQ will be split equally between PSEG1*/
/*! and PSEG2, if PSEG2 >=2.*/
/*! RJW is set to the minimum of 4 or PSEG1.*/
/*! The table contains bit_rate (Hz), propseg, pseg1, pseg2, pre_divider, and rjw.*/
const flexcan_bitrate_table_t bit_rate_table[] = {
    { kFlexCanBitrate_125k, 6, 7, 7, 19, 3},  /* 125 kHz */
    { kFlexCanBitrate_250k, 6, 7, 7,  9, 3},  /* 250 kHz */
    { kFlexCanBitrate_500k, 6, 7, 7,  4, 3},  /* 500 kHz */
    { kFlexCanBitrate_750k, 6, 5, 5,  3, 3},  /* 750 kHz */
    { kFlexCanBitrate_1M,   6, 5, 5,  2, 3},  /* 1   MHz */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_SetBitrate
 * Description   : Set FlexCAN baudrate.
 * This function will set up all the time segment values. Those time segment
 * values are from the table bit_rate_table and based on the baudrate passed in.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_SetBitrate(uint8_t instance, flexcan_bitrate_t bitrate)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    uint32_t num_bitrate_table;
    uint32_t i;
    flexcan_time_segment_t time_seg;

    /* Find the time segments from the table bit_rate_table*/
    num_bitrate_table = sizeof(bit_rate_table)/sizeof(bit_rate_table[0]);
    for (i = 0; i < num_bitrate_table; i++)
    {
        if (bit_rate_table[i].bit_rate == bitrate)
        {
            time_seg.propseg = bit_rate_table[i].propseg;
            time_seg.pseg1 = bit_rate_table[i].pseg1;
            time_seg.pseg2 = bit_rate_table[i].pseg2;
            time_seg.pre_divider = bit_rate_table[i].pre_divider;
            time_seg.rjw = bit_rate_table[i].rjw;
            break;
        }
    }

    if (i == num_bitrate_table)
    {
        return kStatus_FLEXCAN_InvalidArgument;
    }

    /* Set time segments*/
    FLEXCAN_HAL_SetTimeSegments(g_flexcanBaseAddr[instance], &time_seg);

    return kStatus_FLEXCAN_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_GetBitrate
 * Description   : Get FlexCAN baudrate.
 * This function will be based on all the time segment values and find out the
 * baudrate from the table bit_rate_table.
 *
 *END**************************************************************************/
flexcan_status_t  FLEXCAN_DRV_GetBitrate(uint8_t instance, flexcan_bitrate_t *bitrate)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    uint32_t i;
    flexcan_time_segment_t time_seg;
    uint32_t num_bitrate_table;

    /* Get the time segments*/
    FLEXCAN_HAL_GetTimeSegments(g_flexcanBaseAddr[instance], &time_seg);

    /* Find out the corresponding bit rate in the table bit_rate_table*/
    num_bitrate_table = sizeof(bit_rate_table)/sizeof(bit_rate_table[0]);
    for (i = 0; i < num_bitrate_table; i++)
    {
        if (bit_rate_table[i].pre_divider == time_seg.pre_divider)
        {
            if (bit_rate_table[i].pseg2 == time_seg.pseg2)
            {
                *bitrate = bit_rate_table[i].bit_rate;
                return kStatus_FLEXCAN_Success;
            }
        }
    }

    return kStatus_FLEXCAN_InvalidArgument;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_SetMasktype
 * Description   : Set RX masking type.
 * This function will set RX masking type as RX global mask or RX individual
 * mask.
 *
 *END**************************************************************************/
void  FLEXCAN_DRV_SetMaskType(uint8_t instance, flexcan_rx_mask_type_t type)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    FLEXCAN_HAL_SetMaskType(g_flexcanBaseAddr[instance], type);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_SetRxFifoGlobalMask
 * Description   : Set Rx FIFO global mask as the 11-bit standard mask or the
 * 29-bit extended mask.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_SetRxFifoGlobalMask(
    uint8_t instance,
    flexcan_mb_id_type_t id_type,
    uint32_t mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    if (id_type == kFlexCanMbId_Std)
    {
        /* Set standard global mask for RX FIOF*/
        FLEXCAN_HAL_SetRxFifoGlobalStdMask(canBaseAddr, mask);
    }
    else if (id_type == kFlexCanMbId_Ext)
    {
        /* Set extended global mask for RX FIFO*/
        FLEXCAN_HAL_SetRxFifoGlobalExtMask(canBaseAddr, mask);
    }
    else
    {
        return kStatus_FLEXCAN_InvalidArgument;
    }

    return kStatus_FLEXCAN_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_SetRxMbGlobalMask
 * Description   : Set Rx Message Buffer global mask as the 11-bit standard mask
 * or the 29-bit extended mask.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_SetRxMbGlobalMask(
    uint8_t instance,
    flexcan_mb_id_type_t id_type,
    uint32_t mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    if (id_type == kFlexCanMbId_Std)
    {
        /* Set standard global mask for RX MB*/
        FLEXCAN_HAL_SetRxMbGlobalStdMask(canBaseAddr, mask);
    }
    else if (id_type == kFlexCanMbId_Ext)
    {
        /* Set extended global mask for RX MB*/
        FLEXCAN_HAL_SetRxMbGlobalExtMask(canBaseAddr, mask);
    }
    else
    {
        return kStatus_FLEXCAN_InvalidArgument;
    }

    return kStatus_FLEXCAN_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_SetRxIndividualMask
 * Description   : Set Rx individual mask as the 11-bit standard mask or the
 * 29-bit extended mask.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_SetRxIndividualMask(
    uint8_t instance,
    const flexcan_user_config_t * data,
    flexcan_mb_id_type_t id_type,
    uint32_t mb_idx,
    uint32_t mask)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    if (id_type == kFlexCanMbId_Std)
    {
        /* Set standard individual mask*/
        return FLEXCAN_HAL_SetRxIndividualStdMask(canBaseAddr, data, mb_idx, mask);
    }
    else if (id_type == kFlexCanMbId_Ext)
    {
        /* Set extended individual mask*/
        return FLEXCAN_HAL_SetRxIndividualExtMask(canBaseAddr, data, mb_idx, mask);
    }
    else
    {
        return kStatus_FLEXCAN_InvalidArgument;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_Init
 * Description   : Initialize FlexCAN driver.
 * This function will select a source clock, reset FlexCAN module, set maximum
 * number of message buffers, initialize all message buffers as inactive, enable
 * RX FIFO if needed, mask all mask bits, disable all MB interrupts, enable
 * FlexCAN normal mode, and enable all the error interrupts if needed.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_Init(
   uint8_t instance,
   const flexcan_user_config_t *data,
   bool enable_err_interrupts)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    flexcan_status_t result;
    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    /* Enable clock gate to FlexCAN module */
    CLOCK_SYS_EnableFlexcanClock(instance);

    /* Select a source clock for FlexCAN*/
    result = FLEXCAN_HAL_SelectClock(canBaseAddr, kFlexCanClkSource_Ipbus);
    if (result)
    {
        return result;
    }

    /* Enable the CAN clock */
    FLEXCAN_HAL_Enable(canBaseAddr);

    /* Initialize FLEXCAN device */
    result = FLEXCAN_HAL_Init(canBaseAddr, data);
    if (result)
    {
        return result;
    }

    /* Select mode */
    result = FLEXCAN_HAL_EnableOperationMode(canBaseAddr, kFlexCanNormalMode);
    if (result)
    {
        return result;
    }

    /* Init the interrupt sync object.*/
    OSA_SemaCreate(&irqSync, 0);

    /* Enable FlexCAN interrupts.*/
    INT_SYS_EnableIRQ(g_flexcanWakeUpIrqId[instance]);
    INT_SYS_EnableIRQ(g_flexcanErrorIrqId[instance]);
    INT_SYS_EnableIRQ(g_flexcanBusOffIrqId[instance]);
    INT_SYS_EnableIRQ(g_flexcanOredMessageBufferIrqId[instance]);

    /* Enable error interrupts */
    if(enable_err_interrupts)
    {
        FLEXCAN_HAL_EnableErrInt(canBaseAddr);
    }

    fifo_message = NULL;

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_ConfigTxMb
 * Description   : Configure a Tx message buffer.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will set up the message buffer fields,
 * configure the message buffer code for Tx buffer as INACTIVE, and enable the
 * Message Buffer interrupt.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_ConfigTxMb(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx,
    flexcan_data_info_t *tx_info,
    uint32_t msg_id)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    flexcan_status_t result;
    flexcan_mb_code_status_t cs;
    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    /* Initialize transmit mb*/
    cs.data_length = tx_info->data_length;
    cs.msg_id_type = tx_info->msg_id_type;
    cs.code = kFlexCanTX_Inactive;
    result = FLEXCAN_HAL_SetMbTx(canBaseAddr, data, mb_idx, &cs, msg_id, NULL);
    if (result)
    {
        return result;
    }

    /* Enable message buffer interrupt*/
    return FLEXCAN_HAL_EnableMbInt(canBaseAddr, data, mb_idx);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_Send
 * Description   : Set up FlexCAN Message buffer for transmitting data.
 * This function will set the MB CODE field as DATA for Tx buffer. Then this
 * function will copy user's buffer into the message buffer data area, and wait
 * for the Message Buffer interrupt.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_Send(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx,
    flexcan_data_info_t *tx_info,
    uint32_t msg_id,
    uint8_t *mb_data)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    flexcan_status_t result;
    flexcan_mb_code_status_t cs;

    cs.data_length = tx_info->data_length;
    cs.msg_id_type = tx_info->msg_id_type;


    /* Set up FlexCAN message buffer for transmitting data*/
    cs.code = kFlexCanTX_Data;
    result = FLEXCAN_HAL_SetMbTx(g_flexcanBaseAddr[instance], data, mb_idx, &cs, msg_id, mb_data);

    if(result == kStatus_FLEXCAN_Success)
    {
        osa_status_t syncStatus;

        do
        {
            syncStatus = OSA_SemaWait(&irqSync, 1);
        }while(syncStatus == kStatus_OSA_Idle);

        /* Wait for the interrupt*/
        if (syncStatus != kStatus_OSA_Success)
        {
            return kStatus_FLEXCAN_TimeOut;
        }
    }
    else
    {
        return result;
    }

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_ConfigMb
 * Description   : Configure a Rx message buffer.
 * This function will first check if RX FIFO is enabled. If RX FIFO is enabled,
 * the function will make sure if the MB requested is not occupied by RX FIFO
 * and ID filter table. Then this function will set up the message buffer fields,
 * configure the message buffer code for Rx message buffer as NOT_USED, enable
 * the Message Buffer interrupt, configure the message buffer code for Rx
 * message buffer as INACTIVE, copy user's buffer into the message buffer data
 * area, and configure the message buffer code for Rx message buffer as EMPTY.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_ConfigRxMb(
    uint8_t instance,
    const flexcan_user_config_t *data,
    uint32_t mb_idx,
    flexcan_data_info_t *rx_info,
    uint32_t msg_id)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    flexcan_status_t result;
    flexcan_mb_code_status_t cs;
    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    rx_mb_idx = mb_idx;
    cs.data_length = rx_info->data_length;
    cs.msg_id_type = rx_info->msg_id_type;

    /* Initialize rx mb*/
    cs.code = kFlexCanRX_NotUsed;
    result = FLEXCAN_HAL_SetMbRx(canBaseAddr, data, mb_idx, &cs, msg_id);
    if (result)
    {
         return result;
    }

    /* Enable MB interrupt*/
    result = FLEXCAN_HAL_EnableMbInt(canBaseAddr, data, mb_idx);
    if (result)
    {
         return result;
    }

    /* Initialize receive MB*/
    cs.code = kFlexCanRX_Inactive;
    result = FLEXCAN_HAL_SetMbRx(canBaseAddr, data, mb_idx, &cs, msg_id);
    if (result)
    {
         return result;
    }

    /* Set up FlexCAN message buffer fields for receiving data*/
    cs.code = kFlexCanRX_Empty;
    return FLEXCAN_HAL_SetMbRx(canBaseAddr, data, mb_idx, &cs, msg_id);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_ConfigRxFifo
 * Description   : Confgure RX FIFO ID filter table elements.
 * This function will confgure RX FIFO ID filter table elements, and enable RX
 * FIFO interrupts.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_ConfigRxFifo(
    uint8_t instance,
    const flexcan_user_config_t *data,
    flexcan_rx_fifo_id_element_format_t id_format,
    flexcan_id_table_t *id_filter_table)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    flexcan_status_t result;
    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    /* Initialize rx fifo*/
    result = FLEXCAN_HAL_SetRxFifo(canBaseAddr, data, id_format, id_filter_table);
    if(result)
    {
         return result;
    }

    /* Enable RX FIFO interrupts*/
    for (uint8_t i = 5; i <= 7; i++)
    {
        result = FLEXCAN_HAL_EnableMbInt(canBaseAddr, data, i);
        if(result)
        {
             return result;
        }
    }

    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_StartRx
 * Description   : Start receive data after a Rx MB interrupt occurs.
 * This function will lock Rx MB after a Rx MB interrupt occurs, get the Rx MB
 * field values, and unlock the Rx MB.
 *
 *END**************************************************************************/
flexcan_status_t FLEXCAN_DRV_RxMessageBuffer(
    uint8_t instance,
    const flexcan_user_config_t *config,
    uint32_t mb_idx,
    flexcan_mb_t *data)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    flexcan_status_t result;
    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    osa_status_t syncStatus;

    int_mb = false;

    do
    {
        syncStatus = OSA_SemaWait(&irqSync, 1);
    } while(syncStatus == kStatus_OSA_Idle);

    /* Wait for the interrupt*/
    if (syncStatus != kStatus_OSA_Success)
    {
        return kStatus_FLEXCAN_TimeOut;
    }

    if (int_mb)
    {
        /* Lock RX MB*/
        result = FLEXCAN_HAL_LockRxMb(canBaseAddr, config, mb_idx);
        if(result)
        {
            return result;
        }

        /* Get RX MB field values*/
        result = FLEXCAN_HAL_GetMb(canBaseAddr, config, mb_idx, data);
        if(result)
        {
            return result;
        }
    }

    /* Unlock RX message buffer and RX FIFO*/
    FLEXCAN_HAL_UnlockRxMb(canBaseAddr);

    return (kStatus_FLEXCAN_Success);
}

flexcan_status_t FLEXCAN_DRV_RxFifo(
    uint8_t instance,
    const flexcan_user_config_t *config,
    flexcan_mb_t *data)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);
    assert(data);

    osa_status_t syncStatus;

    /* This will get filled by the interrupt handler */
    fifo_message = data;

    do
    {
        syncStatus = OSA_SemaWait(&irqSync, 1);
    } while(syncStatus == kStatus_OSA_Idle);

    /* Wait for the interrupt*/
    if (syncStatus != kStatus_OSA_Success)
    {
        return kStatus_FLEXCAN_TimeOut;
    }

    fifo_message = NULL;

    return (kStatus_FLEXCAN_Success);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLEXCAN_DRV_Deinit
 * Description   : Shutdown a FlexCAN module.
 * This function will disable all FlexCAN interrupts, and disable the FlexCAN.
 *
 *END**************************************************************************/
uint32_t FLEXCAN_DRV_Deinit(uint8_t instance)
{
    assert(instance < HW_CAN_INSTANCE_COUNT);

    /* Disable FlexCAN interrupts.*/
    INT_SYS_DisableIRQ(g_flexcanWakeUpIrqId[instance]);
    INT_SYS_DisableIRQ(g_flexcanErrorIrqId[instance]);
    INT_SYS_DisableIRQ(g_flexcanBusOffIrqId[instance]);
    INT_SYS_DisableIRQ(g_flexcanOredMessageBufferIrqId[instance]);

    /* Disable FlexCAN.*/
    FLEXCAN_HAL_Disable(g_flexcanBaseAddr[instance]);

    fifo_message = NULL;

    /* Disable clock gate to FlexCAN module */
    CLOCK_SYS_DisableFlexcanClock(instance);
    return 0;
}

void FLEXCAN_DRV_IRQHandler(uint8_t instance)
{
    volatile uint32_t             tmp_reg;
    uint32_t temp;
    uint32_t canBaseAddr = g_flexcanBaseAddr[instance];

    /* Get the interrupt flag*/
    tmp_reg = (FLEXCAN_HAL_GetAllMbIntFlags(canBaseAddr)) & CAN_IMASK1_BUFLM_MASK;

    /* Check Tx/Rx interrupt flag and clear the interrupt*/
    if(tmp_reg)
    {
        if ((tmp_reg & 0x20) && BR_CAN_MCR_RFEN(canBaseAddr))
        {
            if (fifo_message != NULL)
            {
                /* Get RX FIFO field values*/
                FLEXCAN_HAL_ReadFifo(canBaseAddr, fifo_message);
            }
        }

        temp = 1 << rx_mb_idx;

        if (tmp_reg & temp)
        {
            int_mb = true;
        }

        /* Clear the interrupt and unlock message buffer*/
        OSA_SemaPost(&irqSync);
        FLEXCAN_HAL_ClearMbIntFlag(canBaseAddr, tmp_reg);
        FLEXCAN_HAL_UnlockRxMb(canBaseAddr);
   }

    /* Clear all other interrupts in ERRSTAT register (Error, Busoff, Wakeup)*/
    FLEXCAN_HAL_ClearErrIntStatus(canBaseAddr);

    return;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/


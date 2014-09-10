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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsl_os_abstraction.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"
#include "fsl_sdhc_hal.h"
#include "fsl_sdhc_common.h"
#include "fsl_sdhc_driver.h"
#include "sdhc.h"
#include "sdmmc.h"

static sdhc_host_t volatile *g_hosts[HW_SDHC_INSTANCE_COUNT] = {0};

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_SelectClock
 * Description: Select clock source for specific host controller
 *
 *END*********************************************************************/
static void SDHC_DRV_SelectClock(uint32_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);

    g_hosts[instance]->maxClock = CLOCK_SYS_GetSdhcFreq(instance);
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_SetClock
 * Description: Enable clock for specific host controller
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_SetClock(uint32_t instance, bool enable)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
#if defined BSP_FSL_SDHC_CLKMGMT_ENABLED
    CLOCK_SYS_EnableEnetClock(instance);
#endif
    return kStatus_SDHC_NoError;
}

#if defined BSP_FSL_SDHC_ENABLE_ADMA1
/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_FillAdma1Table
 * Description: Fill ADMA1 descriptor table, once its finish, the field
 *  of admaTableAddress is well configured. It will fail, if the incoming
 *  data does not align.
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_FillAdma1Table(uint32_t instance, uint32_t *buffer, uint32_t length)
{
    uint32_t i = 0, entries;
    uint32_t *startAddress;
    sdhc_hal_adma1_descriptor_t *tableAddress = NULL;
    volatile sdhc_host_t *host;

    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(buffer);
    assert(length);

    if (((uint32_t)buffer % SDHC_HAL_ADMA1_ADDR_ALIGN) ||
        (length % SDHC_HAL_ADMA1_ADDR_ALIGN))
    {
        return kStatus_SDHC_DataPrepareError;
    }

    host = g_hosts[instance];

    entries = (length / SDHC_HAL_ADMA1_DESC_MAX_LEN_PER_ENTRY) + 1;

    /* ADMA1 needs two descritors to finish a transfer */
    entries *= 2;
    if (entries > host->admaTableMaxEntries)
    {
        /* Larger table is needed */
        if (host->admaTableAddress)
        {
            OSA_MemFree(host->admaTableAddress);
            host->admaTableAddress = NULL;
            host->admaTableMaxEntries = 0;
        }
        tableAddress = (sdhc_hal_adma1_descriptor_t *)OSA_MemAllocZero(entries * sizeof(sdhc_hal_adma1_descriptor_t));
    }

    if ((tableAddress == NULL) && (host->admaTableAddress == NULL))
    {
        host->admaTableMaxEntries = 0;
        /* Failed to alloc memory for ADMA descriptor table */
        return kStatus_SDHC_DmaAddressError;
    }

    if (host->admaTableAddress == NULL)
    {
        /* Update ADMA table address */
        host->admaTableAddress = (uint32_t *)tableAddress;
        /* Update ADMA table capacity */
        host->admaTableMaxEntries = entries;
    }

    startAddress = buffer;
    for (i = 0; i < entries; i += 2)
    {
        /* Each descriptor for ADMA1 is 64-bit in length */
        if ((length - sizeof(uint32_t) * (startAddress - buffer)) < SDHC_HAL_ADMA1_DESC_MAX_LEN_PER_ENTRY)
        {
            /* The last piece of data, setting end flag in descriptor */
            host->admaTableAddress[i] = (uint32_t)(length - sizeof(uint32_t) * (startAddress - buffer)) << SDHC_HAL_ADMA1_DESC_LEN_SHIFT;
            host->admaTableAddress[i] |= SDHC_HAL_ADMA1_DESC_TYPE_SET;
            host->admaTableAddress[i+1] = (uint32_t)(startAddress) << SDHC_HAL_ADMA1_DESC_ADDRESS_SHIFT;
            host->admaTableAddress[i+1] |= SDHC_HAL_ADMA1_DESC_TYPE_TRAN | SDHC_HAL_ADMA1_DESC_END_MASK;
        }
        else
        {
            host->admaTableAddress[i] = (uint32_t)SDHC_HAL_ADMA1_DESC_MAX_LEN_PER_ENTRY << SDHC_HAL_ADMA1_DESC_LEN_SHIFT;
            host->admaTableAddress[i] |= SDHC_HAL_ADMA1_DESC_TYPE_SET;
            host->admaTableAddress[i+1] = (uint32_t)(startAddress) << SDHC_HAL_ADMA1_DESC_ADDRESS_SHIFT;
            host->admaTableAddress[i+1] |= SDHC_HAL_ADMA1_DESC_TYPE_TRAN;
            startAddress += SDHC_HAL_ADMA1_DESC_MAX_LEN_PER_ENTRY/sizeof(uint32_t);
        }
    }

    return kStatus_SDHC_NoError;
}
#endif

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_SetAdma2Descriptor
 * Description: Compose a ADMA2 descriptor, setting address/length and
 *   corresponding flags to be operated by the internal DMA engine.
 *
 *END*********************************************************************/
static void SDHC_DRV_SetAdma2Descriptor(uint32_t *table,
                                        uint32_t *buffer,
                                        uint32_t length,
                                        uint32_t flags)
{
    assert(table);
    assert(length <= SDHC_HAL_ADMA2_DESC_MAX_LEN_PER_ENTRY);

    ((sdhc_hal_adma2_descriptor_t *)table)->address = buffer;
    ((sdhc_hal_adma2_descriptor_t *)table)->attribute = ((SDHC_HAL_ADMA2_DESC_LEN_MASK & length) << SDHC_HAL_ADMA2_DESC_LEN_SHIFT) | flags;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_FillAdma2Table
 * Description: Fill ADMA2 descriptor table, once its finish, the field
 *   of admaTableAddress is well configured. It will fail, if the incoming
 *   data does not align.
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_FillAdma2Table(uint32_t instance, uint32_t *buffer, uint32_t length)
{
    uint32_t i = 0, entries;
    uint32_t *startAddress;
    sdhc_hal_adma2_descriptor_t *tableAddress = NULL;
    volatile sdhc_host_t *host;

    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(buffer);
    assert(length);

    if ((uint32_t)buffer % SDHC_HAL_ADMA2_ADDR_ALIGN)
    {
        return kStatus_SDHC_DataPrepareError;
    }

    host = g_hosts[instance];

    entries = ((length / SDHC_HAL_ADMA2_DESC_MAX_LEN_PER_ENTRY) + 1);
    if (entries > host->admaTableMaxEntries)
    {
        /* Larger table is needed */
        if (host->admaTableAddress)
        {
            OSA_MemFree(host->admaTableAddress);
            host->admaTableAddress = NULL;
            host->admaTableMaxEntries = 0;
        }
        tableAddress = (sdhc_hal_adma2_descriptor_t *)OSA_MemAllocZero(entries * sizeof(sdhc_hal_adma2_descriptor_t));
    }

    if ((tableAddress == NULL) && (host->admaTableAddress == NULL))
    {
        host->admaTableMaxEntries = 0;
        /* Failed to alloc memory for ADMA descriptor table */
        return kStatus_SDHC_DmaAddressError;
    }

    if (host->admaTableAddress == NULL)
    {
        /* Update ADMA table address */
        host->admaTableAddress = (uint32_t *)tableAddress;
        /* Update ADMA table capacity */
        host->admaTableMaxEntries = entries;
    }

    startAddress = buffer;
    for (i = 0; i < entries; i += 2)
    {
        /* Each descriptor for ADMA2 is 64-bit in length */
        if ((length - sizeof(uint32_t) * (startAddress - buffer)) < SDHC_HAL_ADMA2_DESC_MAX_LEN_PER_ENTRY)
        {
            /* The last piece of data, setting end flag in descriptor */
            SDHC_DRV_SetAdma2Descriptor(&host->admaTableAddress[i],
                    startAddress,
                    length - sizeof(uint32_t) * (startAddress - buffer),
                    SDHC_HAL_ADMA2_DESC_TYPE_TRAN | SDHC_HAL_ADMA2_DESC_END_MASK);
        }
        else
        {
            SDHC_DRV_SetAdma2Descriptor(&host->admaTableAddress[i],
                    startAddress,
                    SDHC_HAL_ADMA2_DESC_MAX_LEN_PER_ENTRY,
                    SDHC_HAL_ADMA2_DESC_TYPE_TRAN);
            startAddress += SDHC_HAL_ADMA2_DESC_MAX_LEN_PER_ENTRY/sizeof(uint32_t);
        }
    }
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_PrepareData
 * Description: Prepare data for transferring
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_PrepareData(uint32_t instance,
                                          sdhc_request_t *req) 
{
    sdhc_status_t ret;
    uint32_t totalSize;
    volatile sdhc_host_t *host;

    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(req);
    assert(req->data);
    assert(req->data->buffer);
    assert(req->data->blockCount);
    assert(req->data->blockSize);

    host = g_hosts[instance];
    ret = kStatus_SDHC_NoError;

    if ((host->mode != kSdhcTransModeAdma2)
#if defined BSP_FSL_SDHC_ENABLE_ADMA1
            && (host->mode != kSdhcTransModeAdma1)
#endif
            )
    {
        return ret;
    }

    totalSize = (req->data->blockSize * req->data->blockCount);
    if (((host->mode == kSdhcTransModeAdma2) &&
            (totalSize % SDHC_HAL_ADMA2_LEN_ALIGN)) ||
            ((host->mode == kSdhcTransModeAdma1) &&
             (totalSize % SDHC_HAL_ADMA1_LEN_ALIGN)))

    {
        return kStatus_SDHC_DataPrepareError;
    }

    SDHC_HAL_SetDmaAddress(g_sdhcBaseAddr[instance], 0);
    /* Check data length alignment */
    if ((host->mode == kSdhcTransModeAdma2))
    {
        /* ADMA2 */
        ret = SDHC_DRV_FillAdma2Table(instance, req->data->buffer, totalSize);
        if (ret != kStatus_SDHC_NoError)
        {
            return ret;
        }
    }
#if defined BSP_FSL_SDHC_ENABLE_ADMA1
    else
    {
        /* ADMA1 */
        ret = SDHC_DRV_FillAdma1Table(instance, req->data->buffer, totalSize);
        if (ret != kStatus_SDHC_NoError)
        {
            return ret;
        }
    }
#endif

    SDHC_HAL_SetAdmaAddress(g_sdhcBaseAddr[instance], (uint32_t)host->admaTableAddress);
    return ret;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_IsCardPresent
 * Description: Check whether card is inserted
 *
 *END*********************************************************************/
static bool SDHC_DRV_IsCardPresent(uint32_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    return SDHC_HAL_IsCardInserted(g_sdhcBaseAddr[instance]);
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_ConfigClock
 * Description: configure clock of host controller, it will set the most
 *      close clock frequency to the given clock
 *
 *END*********************************************************************/
sdhc_status_t SDHC_DRV_ConfigClock(uint32_t instance, uint32_t clock)
{
    uint32_t divisor = SDHC_HAL_INITIAL_DVS, freq = SDHC_HAL_INITIAL_CLKFS;
    volatile sdhc_host_t *host = g_hosts[instance];

    SDHC_HAL_SetIpgClock(g_sdhcBaseAddr[instance], false);
    SDHC_HAL_SetSysClock(g_sdhcBaseAddr[instance], false);
    SDHC_HAL_SetPeripheralClock(g_sdhcBaseAddr[instance], false);
    SDHC_HAL_SetSdClock(g_sdhcBaseAddr[instance], false);

    if (clock > 0)
    {
        while((host->maxClock / freq / SDHC_HAL_MAX_DVS > clock) &&
                (freq < SDHC_HAL_MAX_CLKFS))
        {
            SDHC_HAL_NEXT_CLKFS(freq);
        }
        while((host->maxClock / freq / divisor > clock) &&
                (divisor < SDHC_HAL_MAX_DVS))
        {
            SDHC_HAL_NEXT_DVS(divisor);
        }

        clock = host->maxClock / freq / divisor;
        SDHC_HAL_PREV_CLKFS(freq);
        SDHC_HAL_PREV_DVS(divisor);

        SDHC_HAL_SetClockDivisor(g_sdhcBaseAddr[instance], divisor);
        SDHC_HAL_SetClockFrequency(g_sdhcBaseAddr[instance], freq);
        SDHC_HAL_SetDataTimeout(g_sdhcBaseAddr[instance], 0xE);

        SDHC_HAL_SetIpgClock(g_sdhcBaseAddr[instance], true);
        SDHC_HAL_SetSysClock(g_sdhcBaseAddr[instance], true);
        SDHC_HAL_SetPeripheralClock(g_sdhcBaseAddr[instance], true);

        while(!SDHC_HAL_IsSdClockStable(g_sdhcBaseAddr[instance])) {}
        SDHC_HAL_SetSdClock(g_sdhcBaseAddr[instance], true);
    }

    host->clock = clock;
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_SetBusWidth
 * Description: Configure bus width of host controller
 *
 *END*********************************************************************/
sdhc_status_t SDHC_DRV_SetBusWidth(uint32_t instance, sdhc_buswidth_t busWidth)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    sdhc_hal_dtw_t dtw = kSdhcHalDtw1Bit;
    volatile sdhc_host_t *host = g_hosts[instance];

    switch(busWidth)
    {
        case kSdhcBusWidth1Bit:
            dtw = kSdhcHalDtw1Bit;
            break;
        case kSdhcBusWidth4Bit:
            dtw = kSdhcHalDtw4Bit;
            break;
        default:
            return kStatus_SDHC_InvalidParameter;
    }
    SDHC_HAL_SetDataTransferWidth(g_sdhcBaseAddr[instance], dtw);
    host->busWidth = busWidth;
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_Reset
 * Description: Reset host controller accord to the given mask
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_Reset(uint32_t instance, uint32_t mask)
{
    uint32_t timeout;
    volatile sdhc_host_t *host;
    assert(instance < HW_SDHC_INSTANCE_COUNT);

    timeout = 100;
    host = g_hosts[instance];

    if (!(mask & (SDHC_RESET_ALL | SDHC_RESET_DATA | SDHC_RESET_CMD)))
    {
        return kStatus_SDHC_InvalidParameter;
    }

    if (mask & SDHC_RESET_ALL)
    {
        host->clock = 0;
        SDHC_HAL_Reset(g_sdhcBaseAddr[instance], SDHC_HAL_RST_TYPE_ALL, timeout);
    }
    else if (mask == (SDHC_RESET_DATA | SDHC_RESET_CMD))
    {
        SDHC_HAL_Reset(g_sdhcBaseAddr[instance], (SDHC_HAL_RST_TYPE_DATA
                                | SDHC_HAL_RST_TYPE_CMD), timeout);
    }
    else if (mask == SDHC_RESET_CMD)
    {
        SDHC_HAL_Reset(g_sdhcBaseAddr[instance], SDHC_HAL_RST_TYPE_CMD, timeout);
    }
    else if (mask == SDHC_RESET_DATA)
    {
        SDHC_HAL_Reset(g_sdhcBaseAddr[instance], SDHC_HAL_RST_TYPE_DATA, timeout);
    }
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_GetMaxBlockSize
 * Description: Get max block size supported for the host controller
 *
 *END*********************************************************************/
static uint32_t SDHC_DRV_GetMaxBlockSize(uint32_t instance)
{
    uint32_t mbl, ret = 0;

    mbl = SDHC_HAL_GetMaxBlockLength(g_sdhcBaseAddr[instance]);
    switch (mbl)
    {
        case SDHC_HAL_MAX_BLKLEN_512B:
            ret = 512;
            break;
        case SDHC_HAL_MAX_BLKLEN_1024B:
            ret = 1024;
            break;
        case SDHC_HAL_MAX_BLKLEN_2048B:
            ret = 2048;
            break;
        case SDHC_HAL_MAX_BLKLEN_4096B:
            ret = 4096;
            break;
        default:
            break;
    }
    return ret;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_SendCommand
 * Description: Send command to card
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_SendCommand(uint32_t instance,
                                          sdhc_request_t *req)
{
    uint32_t flags = 0;
    sdhc_status_t ret = kStatus_SDHC_NoError;
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(req);

    if (req->data)
    {
        flags |= SDHC_HAL_DATA_PRESENT;

        SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], false,
                (SDHC_HAL_DMA_ERR_INT | SDHC_HAL_DMA_INT |
                    SDHC_HAL_BUF_READ_READY_INT | SDHC_HAL_BUF_WRITE_READY_INT));
        SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], false,
                (SDHC_HAL_DMA_ERR_INT | SDHC_HAL_DMA_INT |
                    SDHC_HAL_BUF_READ_READY_INT | SDHC_HAL_BUF_WRITE_READY_INT));

        if (req->flags & FSL_SDHC_REQ_FLAGS_USE_DMA)
        {
            flags |= SDHC_HAL_ENABLE_DMA;
            SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], true,
                    (SDHC_HAL_DMA_ERR_INT | SDHC_HAL_DMA_INT));
            SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], true,
                    (SDHC_HAL_DMA_ERR_INT | SDHC_HAL_DMA_INT));
        }
        else
        {
            SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], true,
                    (SDHC_HAL_BUF_READ_READY_INT | SDHC_HAL_BUF_WRITE_READY_INT));
            SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], true,
                    (SDHC_HAL_BUF_READ_READY_INT | SDHC_HAL_BUF_WRITE_READY_INT));
        }

        if (req->flags & FSL_SDHC_REQ_FLAGS_DATA_READ)
        {
            flags |= SDHC_HAL_ENABLE_DATA_READ;
        }
    }

    if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_136BITS)
    {
        flags |= SDHC_HAL_RESP_LEN_136;
    }
    else if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_BUSY)
    {
        flags |= SDHC_HAL_RESP_LEN_48_BC;
    }
    else if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_PRESENT)
    {
        flags |= SDHC_HAL_RESP_LEN_48;
    }

    if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_CRC)
    {
        flags |= SDHC_HAL_ENABLE_CRC_CHECK;
    }
    if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_CHK_IDX)
    {
        flags |= SDHC_HAL_ENABLE_INDEX_CHECK;
    }

    while(SDHC_HAL_IsCmdInhibit(g_sdhcBaseAddr[instance])) {}

    if (req->cmdIndex == kStopTransmission)
    {
        flags |= SDHC_HAL_CMD_TYPE_ABORT;
    }
    else if ((req->data) ||
            (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_BUSY))
    {
        while(SDHC_HAL_IsDataInhibit(g_sdhcBaseAddr[instance])) {}
    }

    if (req->data)
    {
        if (req->data->blockCount > 1)
        {
            flags |= SDHC_HAL_MULTIPLE_BLOCK;
            flags |= SDHC_HAL_ENABLE_BLOCK_COUNT;
#ifdef BSP_FSL_SDHC_ENABLE_AUTOCMD12
            /* Enable Auto CMD12 */
            flags |= SDHC_HAL_ENABLE_AUTO_CMD12;
#endif
        }
        if (req->data->blockCount == ((uint32_t) -1))
        {
            SDHC_HAL_SetBlockSize(g_sdhcBaseAddr[instance], req->data->blockSize);
            SDHC_HAL_SetBlockCount(g_sdhcBaseAddr[instance], SDHC_HAL_MAX_BLOCK_COUNT);
            flags &= ~SDHC_HAL_ENABLE_BLOCK_COUNT;
        }
        else
        {
            SDHC_HAL_SetBlockSize(g_sdhcBaseAddr[instance], req->data->blockSize);
            SDHC_HAL_SetBlockCount(g_sdhcBaseAddr[instance], req->data->blockCount);
        }
    }
    else
    {
        SDHC_HAL_SetBlockSize(g_sdhcBaseAddr[instance], 0);
        SDHC_HAL_SetBlockCount(g_sdhcBaseAddr[instance], 0);
    }

    SDHC_HAL_SetCmdArgument(g_sdhcBaseAddr[instance], req->argument);
    SDHC_HAL_SendCmd(g_sdhcBaseAddr[instance], req->cmdIndex, flags);
    return ret;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_SetRequestError
 * Description: Set error flags for a given request according to irq flags
 *
 *END*********************************************************************/
static void SDHC_DRV_SetRequestError(sdhc_request_t *req, uint32_t irqFlags)
{
    assert(req);
    if ((!irqFlags) || (!(irqFlags & SDHC_HAL_ALL_ERR_INT)))
    {
        return;
    }

    if (irqFlags & SDHC_HAL_CMD_CRC_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_CMD_CRC;
    }
    if (irqFlags & SDHC_HAL_CMD_INDEX_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_CMD_INDEX;
    }
    if (irqFlags & SDHC_HAL_CMD_END_BIT_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_CMD_END_BIT;
    }
    if (irqFlags & SDHC_HAL_CMD_TIMEOUT_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_CMD_TIMEOUT;
    }
    if (irqFlags & SDHC_HAL_DATA_TIMEOUT_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_DAT_TIMEOUT;
    }
    if (irqFlags & SDHC_HAL_DATA_CRC_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_DATA_CRC;
    }
    if (irqFlags & SDHC_HAL_DATA_END_BIT_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_DATA_END_BIT;
    }
    if (irqFlags & SDHC_HAL_AUTO_CMD12_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_AUTO_CMD12;
    }
    if (irqFlags & SDHC_HAL_DMA_ERR_INT)
    {
        req->error |= FSL_SDHC_REQ_ERR_DMA;
    }
}

#if ! defined BSP_FSL_SDHC_USING_IRQ
/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_WaitInt
 * Description: Wait for specific interrupts
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_WaitInt(uint32_t instance,
                                      uint32_t mask,
                                      uint32_t *irq,
                                      uint32_t timeoutInMs)
{
    uint32_t status = 0;
    semaphore_t timeout;

    if (kStatus_OSA_Success != OSA_SemaCreate(&timeout, 0))
    {
        return kStatus_SDHC_Failed;
    }

    do
    {
        *irq = (SDHC_HAL_GetIntFlags(g_sdhcBaseAddr[instance]) & mask);
        if (!timeoutInMs)
        {
            status = OSA_SemaWait(&timeout, OSA_WAIT_FOREVER);
        }
        else
        {
            status = OSA_SemaWait(&timeout, timeoutInMs);
        }
    }
    while ((!(*irq)) && (status == kStatus_OSA_Idle));

    if (status == kStatus_OSA_Idle)
    {
        return kStatus_SDHC_NoError;
    }

    return kStatus_SDHC_TimeoutError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_TransferDataPio
 * Description: transfer data using PIO mode
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_TransferDataPio(uint32_t instance,
                                              sdhc_request_t *req,
                                              uint32_t timeoutInMs)
{
    uint32_t opMask, mask, i, j, irqFlags, status;
#if defined BSP_FSL_SDHC_SUPPORT_UNALIGNED_BLOCKSIZE_ACCESS
    uint8_t bytesLeft, *byteTmp;
    uint32_t temp;
#endif
    volatile sdhc_host_t *host;
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(req);

    host = g_hosts[instance];
    mask = SDHC_HAL_DATA_COMPLETE_INT | SDHC_HAL_DATA_ERR_INT;
    if ((req->flags & FSL_SDHC_REQ_FLAGS_DATA_READ))
    {
        opMask = SDHC_HAL_BUF_READ_READY_INT;
    }
    else
    {
        opMask = SDHC_HAL_BUF_WRITE_READY_INT;
    }
    for (i = 0; i < req->data->blockCount; i++)
    {
        for (j = 0; j < req->data->blockSize >> 2; j++)
        {
            status = SDHC_DRV_WaitInt(instance, mask | opMask,
                                     &irqFlags, timeoutInMs);
            if (status != kStatus_SDHC_NoError)
            {
                req->error |= FSL_SDHC_REQ_ERR_TIMEOUT;
                host->currentReq = 0;
                SDHC_DRV_SetClock(instance, false);
                SDHC_DRV_SetRequestError(req, irqFlags);
                return kStatus_SDHC_Failed;
            }
            if (irqFlags & SDHC_HAL_DATA_ERR_INT)
            {
                SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], mask);
                host->currentReq = 0;
                SDHC_DRV_SetClock(instance, false);
                SDHC_DRV_SetRequestError(req, irqFlags);
                return kStatus_SDHC_Failed;
            }
            if (irqFlags & opMask)
            {
                if ((req->flags & FSL_SDHC_REQ_FLAGS_DATA_READ))
                {
                    req->data->buffer[(i * (req->data->blockSize >> 2)) + j]
                        = SDHC_HAL_GetData(g_sdhcBaseAddr[instance]);
                }
                else
                {
                    SDHC_HAL_SetData(g_sdhcBaseAddr[instance],
                        req->data->buffer[(i * (req->data->blockSize >> 2)) + j]);
                }
                SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], opMask);
            }
            req->data->bytesTransferred += 4;
        }

#if defined BSP_FSL_SDHC_SUPPORT_UNALIGNED_BLOCKSIZE_ACCESS
        bytesLeft = req->data->blockSize % 4;
        if (!bytesLeft)
        {
            continue;
        }

        status = SDHC_DRV_WaitInt(instance, mask | opMask,
                                 &irqFlags, timeoutInMs);
        if (status != kStatus_SDHC_NoError)
        {
            req->error |= FSL_SDHC_REQ_ERR_TIMEOUT;
            host->currentReq = 0;
            SDHC_DRV_SetClock(instance, false);
            SDHC_DRV_SetRequestError(req, irqFlags);
            return kStatus_SDHC_Failed;
        }
        if (irqFlags & SDHC_HAL_DATA_ERR_INT)
        {
            SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], mask);
            host->currentReq = 0;
            SDHC_DRV_SetClock(instance, false);
            SDHC_DRV_SetRequestError(req, irqFlags);
            return kStatus_SDHC_Failed;
        }
        if (irqFlags & opMask)
        {
            byteTmp = (uint8_t *)req->data->buffer;
            if ((req->flags & FSL_SDHC_REQ_FLAGS_DATA_READ))
            {
                temp = SDHC_HAL_GetData(g_sdhcBaseAddr[instance]);
                while(bytesLeft--)
                {
                    byteTmp[i * req->data->blockSize - bytesLeft] = temp & 0xFF;
                    temp >>= 8;
                }
            }
            else
            {
                while(bytesLeft--)
                {
                    temp |= ((uint32_t)byteTmp[i * req->data->blockSize - bytesLeft] << ((3 - bytesLeft) * 8));
                }
                SDHC_HAL_SetData(g_sdhcBaseAddr[instance], temp);
            }
            req->data->bytesTransferred++;
            SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], opMask);
        }
#endif
    }

    do
    {
        status = SDHC_DRV_WaitInt(instance, mask, &irqFlags, timeoutInMs);
        if (status != kStatus_SDHC_NoError)
        {
            req->error |= FSL_SDHC_REQ_ERR_TIMEOUT;
            host->currentReq = 0;
            SDHC_DRV_SetClock(instance, false);
            SDHC_DRV_SetRequestError(req, irqFlags);
            return kStatus_SDHC_Failed;
        }
    } while (!(irqFlags & SDHC_HAL_DATA_COMPLETE_INT));

    SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], mask);
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_TransferDataDma
 * Description: transfer data using DMA mode
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_TransferDataDma(uint32_t instance,
                                              sdhc_request_t *req,
                                              uint32_t timeoutInMs)
{
    uint32_t mask, irqFlags;
    sdhc_status_t status;
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(req);
    volatile sdhc_host_t *host;

    host = g_hosts[instance];
    if (host->mode == kSdhcTransModeSdma)
    {
        return kStatus_SDHC_NotSupportYet;
    }

    mask = SDHC_HAL_DATA_COMPLETE_INT | SDHC_HAL_DMA_ERR_INT;
    do
    {
        status = SDHC_DRV_WaitInt(instance, mask, &irqFlags, timeoutInMs);
        if (status != kStatus_SDHC_NoError)
        {
            req->error |= FSL_SDHC_REQ_ERR_TIMEOUT;
            host->currentReq = 0;
            SDHC_DRV_SetClock(instance, false);
            SDHC_DRV_SetRequestError(req, irqFlags);
            return kStatus_SDHC_Failed;
        }

        if (irqFlags & SDHC_HAL_DMA_ERR_INT)
        {
            req->error |= FSL_SDHC_REQ_ERR_DMA;
            host->currentReq = 0;
            SDHC_DRV_SetClock(instance, false);
            SDHC_DRV_SetRequestError(req, irqFlags);
            return kStatus_SDHC_Failed;
        }
    } while (!(irqFlags & SDHC_HAL_DATA_COMPLETE_INT));

    SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], mask);
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_TransferData
 * Description: transfer data using different mode according to the flags
 * of host controller
 *
 *END*********************************************************************/
static sdhc_status_t SDHC_DRV_TransferData(uint32_t instance,
                                           sdhc_request_t *req,
                                           uint32_t timeoutInMs)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(req);

    if (req->flags & FSL_SDHC_REQ_FLAGS_USE_DMA)
    {
        return SDHC_DRV_TransferDataDma(instance, req, timeoutInMs);
    }
    else
    {
        return SDHC_DRV_TransferDataPio(instance, req, timeoutInMs);
    }
}

#else /* BSP_FSL_SDHC_USING_IRQ */

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_ClearSetInt
 * Description: Clear then set corresponding interrupt mask
 *
 *END*********************************************************************/
static void SDHC_DRV_ClearSetInt(uint32_t instance,
                                 uint32_t clear,
                                 uint32_t set)
{
    SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], false, clear);
    SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], false, clear);

    SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], true, set);
    SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], true, set);
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_DataIrq
 * Description: handle data related irqs
 *
 *END*********************************************************************/
static void SDHC_DRV_DataIrq(uint32_t instance, uint32_t irq)
{
    sdhc_request_t *req;
    assert(irq & SDHC_HAL_DATA_ALL_INT);

    req = g_hosts[instance]->currentReq;
    assert(req);
    assert(req->data);
    assert(req->data->buffer);

    if (irq & (SDHC_HAL_DATA_ERR_INT | SDHC_HAL_DMA_ERR_INT))
    {
        SDHC_DRV_SetRequestError(req, irq);
        OSA_SemaPost(req->complete);
        return;
    }

    if (irq & SDHC_HAL_BUF_READ_READY_INT)
    {
        req->data->buffer[req->data->bytesTransferred >> 2] =
            SDHC_HAL_GetData(g_sdhcBaseAddr[instance]);
        req->data->bytesTransferred += 4;
    }
    else if (irq & SDHC_HAL_BUF_WRITE_READY_INT)
    {
        SDHC_HAL_SetData(g_sdhcBaseAddr[instance],
                        req->data->buffer[req->data->bytesTransferred >> 2]);
        req->data->bytesTransferred += 4;
    }
    else if (irq & SDHC_HAL_DATA_COMPLETE_INT)
    {
        OSA_SemaPost(req->complete);
    }
    else if (irq & SDHC_HAL_DMA_INT)
    {
        if (g_hosts[instance]->mode != kSdhcTransModeSdma)
        {
            return;
        }
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_CmdIrq
 * Description: handle command related irqs
 *
 *END*********************************************************************/
static void SDHC_DRV_CmdIrq(uint32_t instance, uint32_t irq)
{
    sdhc_request_t *req;
    uint32_t i;
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(irq & SDHC_HAL_CMD_ALL_INT);
    req = g_hosts[instance]->currentReq;
    if (irq & SDHC_HAL_CMD_ERR_INT)
    {
        SDHC_DRV_SetRequestError(req, irq);
    }
    else if (irq & SDHC_HAL_CMD_COMPLETE_INT)
    {
        if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_PRESENT)
        {
            req->response[0] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 0);
            if (!(g_req_resp_flags[req->respType] &
                        FSL_SDHC_REQ_RSPTYPE_136BITS))
            {
                if ((req->respType == kSdhcRespTypeR1) ||
                        (req->respType == kSdhcRespTypeR1b))
                {
                    req->cardErrStatus = SDMMC_R1_ERROR_BITS(req->response[0]);
                }
            }
            else
            {
                req->response[1] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 1);
                req->response[2] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 2);
                req->response[3] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 3);
                i = 4;
                do {
                    req->response[i - 1] <<= 8;
                    if (i > 1)
                    {
                        req->response[i - 1] |=
                            ((req->response[i-2] & 0xFF000000U) >> 24);
                    }
                } while(i--);
            }
        }
    }
    if ((!req->data) || (req->cardErrStatus))
    {
        OSA_SemaPost(req->complete);
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_CardDetectIrq
 * Description: Card detection interrupt handler
 *
 *END*********************************************************************/
static void SDHC_DRV_CardDetectIrq(uint32_t instance, uint32_t irq)
{
    assert(irq & SDHC_HAL_CD_ALL_INT);
    assert(g_hosts[instance]->cardDetectCallback);

    if ((irq & SDHC_HAL_CD_ALL_INT) == SDHC_HAL_CARD_INSERTION_INT)
    {
        if (g_hosts[instance]->cardDetectCallback)
        {
            g_hosts[instance]->cardDetectCallback(true);
        }
        SDHC_DRV_ClearSetInt(instance,
                             SDHC_HAL_CARD_INSERTION_INT,
                             SDHC_HAL_CARD_REMOVAL_INT);
    }
    else
    {
        if (g_hosts[instance]->cardDetectCallback)
        {
            g_hosts[instance]->cardDetectCallback(false);
        }
        SDHC_DRV_ClearSetInt(instance,
                             SDHC_HAL_CARD_REMOVAL_INT,
                             SDHC_HAL_CARD_INSERTION_INT);
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_BlockGapIrq
 * Description: Block gap interrupt handler
 *
 *END*********************************************************************/
static void SDHC_DRV_BlockGapIrq(uint32_t instance)
{
    assert(g_hosts[instance]->blockGapCallback);
    g_hosts[instance]->blockGapCallback();
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_CardIntIrq
 * Description: Card interrupt handler
 *
 *END*********************************************************************/
static void SDHC_DRV_CardIntIrq(uint32_t instance)
{
    assert(g_hosts[instance]->cardIntCallback);
    g_hosts[instance]->cardIntCallback();
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_DoIrq
 * Description: IRQ handler
 *
 *END*********************************************************************/
void SDHC_DRV_DoIrq(uint32_t instance)
{
    volatile uint32_t irq;
    volatile uint32_t cardInt = 0;
    irq = SDHC_HAL_GetIntFlags(g_sdhcBaseAddr[instance]);

    if (!irq)
    {
        return;
    }

    if (irq & SDHC_HAL_CD_ALL_INT)
    {
        SDHC_DRV_CardDetectIrq(instance, (irq & SDHC_HAL_CD_ALL_INT));
    }
    if (irq & SDHC_HAL_CMD_ALL_INT)
    {
        SDHC_DRV_CmdIrq(instance, (irq & SDHC_HAL_CMD_ALL_INT));
    }
    if (irq & SDHC_HAL_DATA_ALL_INT)
    {
        SDHC_DRV_DataIrq(instance, (irq & SDHC_HAL_DATA_ALL_INT));
    }
    if (irq & SDHC_HAL_CARD_INT)
    {
        cardInt = 1;
    }
    if (irq & SDHC_HAL_BLOCK_GAP_EVENT_INT)
    {
        SDHC_DRV_BlockGapIrq(instance);
    }

    SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], irq);

    if (cardInt)
    {
        SDHC_DRV_CardIntIrq(instance);
    }
    return;
}
#endif

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_Init
 * Description: Initialize host controller by specific instance index.
 *
 *END*********************************************************************/
sdhc_status_t SDHC_DRV_Init(uint32_t instance,
                            sdhc_host_t *host,
                            const sdhc_user_config_t *config)
{
    uint32_t irqEnabled;

    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(host);
    assert(config);

    if (g_hosts[instance])
    {
        return kStatus_SDHC_HostIsAlreadyInited;
    }

    memset(host, 0, sizeof(sdhc_host_t));
    g_hosts[instance] = host;
    host->instance = instance;

#if ! defined BSP_FSL_SDHC_USING_IRQ
    if (config->cdType == kSdhcCardDetectDat3)
    {
        return kStatus_SDHC_HostNotSupport;
    }
#endif

#if ! defined BSP_FSL_SDHC_ENABLE_ADMA1
    if (config->transMode == kSdhcTransModeAdma1)
    {
        return kStatus_SDHC_HostNotSupport;
    }
#endif

    host->cdType = config->cdType;
    if (((host->cdType == kSdhcCardDetectCdPin) ||
         (host->cdType == kSdhcCardDetectDat3)) &&
         (config->cardDetectCallback))
    {
        host->cardDetectCallback = config->cardDetectCallback;
    }

    CLOCK_SYS_EnableSdhcClock(instance);
    SDHC_DRV_SetClock(instance, false);

    /* set clock source for SDHC as kClockPllfllSel */
    CLOCK_SYS_SetSource(kClockSdhcSrc, 1);
    SDHC_DRV_SelectClock(instance);

    SDHC_DRV_SetClock(instance, true);

    SDHC_HAL_Init(g_sdhcBaseAddr[instance]);
    SDHC_DRV_Reset(instance, SDHC_RESET_ALL);
    SDHC_HAL_SetExternalDmaRequest(g_sdhcBaseAddr[instance], false);

    host->maxBlockSize = SDHC_DRV_GetMaxBlockSize(instance);
    if (!host->maxBlockSize)
    {
        CLOCK_SYS_DisableSdhcClock(instance);
        return kStatus_SDHC_Failed;
    }
    host->maxBlockCount = SDHC_HAL_MAX_BLOCK_COUNT;

    host->ocrSupported = 0;
    if (SDHC_HAL_DoesHostSupportV330(g_sdhcBaseAddr[instance]))
    {
        host->ocrSupported |= SD_OCR_VDD_32_33 | SD_OCR_VDD_33_34;
    }
    if (SDHC_HAL_DoesHostSupportV300(g_sdhcBaseAddr[instance]))
    {
        host->ocrSupported |= SD_OCR_VDD_29_30;
    }
    if (SDHC_HAL_DoesHostSupportHighspeed(g_sdhcBaseAddr[instance]))
    {
        host->caps |= FSL_SDHC_HOST_CAPS_SUPPORT_HIGHSPEED;
    }
    if (SDHC_HAL_DoesHostSupportDma(g_sdhcBaseAddr[instance]))
    {
        host->caps |= FSL_SDHC_HOST_CAPS_SUPPORT_DMA;
    }
    if (SDHC_HAL_DoesHostSupportAdma(g_sdhcBaseAddr[instance]))
    {
        host->caps |= FSL_SDHC_HOST_CAPS_SUPPORT_ADMA;
    }
    if (SDHC_HAL_DoesHostSupportSuspendResume(g_sdhcBaseAddr[instance]))
    {
        host->caps |= FSL_SDHC_HOST_CAPS_SUPPORT_SRS;
    }
    if (SDHC_HAL_DoesHostSupportV180(g_sdhcBaseAddr[instance]))
    {
        host->caps |= FSL_SDHC_HOST_CAPS_SUPPORT_V180;
    }

    if (((config->transMode == kSdhcTransModeSdma)
                && (!(host->caps & FSL_SDHC_HOST_CAPS_SUPPORT_DMA)))
            || (((config->transMode == kSdhcTransModeAdma1) ||
                    (config->transMode == kSdhcTransModeAdma2))
                && (!(host->caps & FSL_SDHC_HOST_CAPS_SUPPORT_ADMA)))
            || (host->swFeature & FSL_SDHC_HOST_SW_FEATURE_NODMA))
    {
        CLOCK_SYS_DisableSdhcClock(instance);
        return kStatus_SDHC_HostNotSupport;
    }
    host->mode = config->transMode;

    /* enable irqs */
    SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], false, SDHC_INT_ALL_MASK);
    SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], false, SDHC_INT_ALL_MASK);
    irqEnabled = SDHC_HAL_CMD_INDEX_ERR_INT | SDHC_HAL_CMD_CRC_ERR_INT |
                 SDHC_HAL_CMD_END_BIT_ERR_INT | SDHC_HAL_CMD_TIMEOUT_ERR_INT |
                 SDHC_HAL_DATA_TIMEOUT_ERR_INT | SDHC_HAL_DATA_CRC_ERR_INT |
                 SDHC_HAL_DATA_END_BIT_ERR_INT | SDHC_HAL_CMD_COMPLETE_INT |
                 SDHC_HAL_DATA_COMPLETE_INT;
#if defined BSP_FSL_SDHC_ENABLE_AUTOCMD12
    irqEnabled |= SDHC_HAL_AUTO_CMD12_ERR_INT;
#endif
    if ((host->cdType == kSdhcCardDetectCdPin) ||
            (host->cdType == kSdhcCardDetectDat3))
    {
        irqEnabled |= (SDHC_HAL_CARD_INSERTION_INT |
                       SDHC_HAL_CARD_REMOVAL_INT);
    }
    if ((host->cdType == kSdhcCardDetectDat3) ||
            (host->cdType == kSdhcCardDetectPollDat3))
    {
        SDHC_HAL_SetD3cd(g_sdhcBaseAddr[instance], true);
        if (kStatus_SDHC_NoMedium == SDHC_DRV_DetectCard(instance))
        {
            irqEnabled &= ~SDHC_HAL_CARD_REMOVAL_INT;
        }
        else
        {
            irqEnabled &= ~SDHC_HAL_CARD_INSERTION_INT;
        }
    }

    if (host->mode == kSdhcTransModeAdma2)
    {
        SDHC_HAL_SetDmaMode(g_sdhcBaseAddr[instance], kSdhcHalDmaAdma2);
    }
#if defined BSP_FSL_SDHC_ENABLE_ADMA1
    else if (host->mode == kSdhcTransModeAdma1)
    {
        SDHC_HAL_SetDmaMode(g_sdhcBaseAddr[instance], kSdhcHalDmaAdma1);
    }
#endif
    else
    {
        SDHC_HAL_SetDmaMode(g_sdhcBaseAddr[instance], kSdhcHalDmaSimple);
    }

    SDHC_HAL_SetIntState(g_sdhcBaseAddr[instance], true, irqEnabled);
    if ((host->cdType == kSdhcCardDetectPollDat3) ||
         (host->cdType == kSdhcCardDetectPollCd))
    {
        irqEnabled &= ~(SDHC_HAL_CARD_REMOVAL_INT |
                SDHC_HAL_CARD_INSERTION_INT);
    }
    SDHC_HAL_SetIntSignal(g_sdhcBaseAddr[instance], true, irqEnabled);

#if defined BSP_FSL_SDHC_USING_BIG_ENDIAN
    host->endian = kSdhcHalEndianBig;
#else
    host->endian = kSdhcHalEndianLittle;
#endif

    SDHC_HAL_SetEndian(g_sdhcBaseAddr[instance], host->endian);

    SDHC_HAL_SetWriteWatermarkLevel(g_sdhcBaseAddr[instance], 1);
    SDHC_HAL_SetReadWatermarkLevel(g_sdhcBaseAddr[instance], 1);

    SDHC_DRV_SetBusWidth(instance, kSdhcBusWidth1Bit);
    SDHC_DRV_ConfigClock(instance, config->clock);

#if defined BSP_FSL_SDHC_USING_IRQ
    if (config->cardIntCallback)
    {
        host->cardIntCallback = config->cardIntCallback;
    }
    if (config->blockGapCallback)
    {
        host->blockGapCallback = config->blockGapCallback;
    }
    INT_SYS_EnableIRQ(g_sdhcIrqId[instance]);
#endif
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_Shutdown
 * Description: Deinitialize host controller
 *
 *END*********************************************************************/
void SDHC_DRV_Shutdown(uint32_t instance)
{
    if (g_hosts[instance] == 0)
    {
        return;
    }

#if defined BSP_FSL_SDHC_USING_IRQ
    INT_SYS_DisableIRQ(g_sdhcIrqId[instance]);
#endif

    if (g_hosts[instance]->admaTableAddress != NULL)
    {
        OSA_MemFree(g_hosts[instance]->admaTableAddress);
        g_hosts[instance]->admaTableAddress = NULL;
        g_hosts[instance]->admaTableMaxEntries = 0;
    }
    SDHC_HAL_SetSdClock(g_sdhcBaseAddr[instance], false);
    SDHC_DRV_SetClock(instance, false);
    CLOCK_SYS_DisableSdhcClock(instance);
    g_hosts[instance] = 0;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_DetectCard
 * Description: check whether the card is present on specified host
 *      controller.
 *
 *END*********************************************************************/
sdhc_status_t SDHC_DRV_DetectCard(uint32_t instance)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    volatile sdhc_host_t *host = g_hosts[instance];
    if (host->cdType == kSdhcCardDetectGpio)
    {
        return kStatus_SDHC_UnknownStatus;
    }

    SDHC_DRV_SetClock(instance, true);
    if (!SDHC_DRV_IsCardPresent(instance))
    {
        host->flags &= (uint32_t)(~FSL_SDHC_HOST_FLAGS_CARD_PRESENTED);
        SDHC_DRV_SetClock(instance, false);
        return kStatus_SDHC_NoMedium;
    }
    host->flags |= FSL_SDHC_HOST_FLAGS_CARD_PRESENTED;
    SDHC_HAL_InitCard(g_sdhcBaseAddr[instance], 100);
    SDHC_DRV_SetClock(instance, false);
    return kStatus_SDHC_NoError;
}

/*FUNCTION****************************************************************
 *
 * Function Name: SDHC_DRV_IssueRequestBlocking
 * Description: Isuue request on specific host controller and return
 *      on completion.
 *
 *END*********************************************************************/
sdhc_status_t SDHC_DRV_IssueRequestBlocking(uint32_t instance,
                                            sdhc_request_t *req,
                                            uint32_t timeoutInMs)
{
    sdhc_status_t ret;
    volatile sdhc_host_t *host;

    assert(instance < HW_SDHC_INSTANCE_COUNT);
    assert(req);

    host = g_hosts[instance];
    ret = kStatus_SDHC_NoError;
    req->error = 0;
#if ! defined BSP_FSL_SDHC_SUPPORT_UNALIGNED_BLOCKSIZE_ACCESS
    if ((req->data) && (req->data->blockSize % 4))
    {
        return kStatus_SDHC_BlockSizeNotSupportError;
    }
#endif

    if ((req->data) && (host->mode != kSdhcTransModePio))
    {
        if (kStatus_SDHC_NoError == SDHC_DRV_PrepareData(instance, req))
        {
            req->flags |= FSL_SDHC_REQ_FLAGS_USE_DMA;
        }
    }

#if defined BSP_FSL_SDHC_USING_IRQ
    osa_status_t status;
    semaphore_t *complete =
        (semaphore_t *)OSA_MemAllocZero(sizeof(semaphore_t));
    if (kStatus_OSA_Success != OSA_SemaCreate(complete, 0))
    {
        return kStatus_SDHC_Failed;
    }
    assert(!req->complete);         /* it should not be asigned outside of this routine */
    req->complete = complete;
#endif

    SDHC_DRV_SetClock(instance, true);

    if (host->currentReq)
    {
        req->error |= FSL_SDHC_REQ_ERR_HOST_BUSY;
        SDHC_DRV_SetClock(instance, false);
#if defined BSP_FSL_SDHC_USING_IRQ
        OSA_SemaDestroy(req->complete);
        OSA_MemFree(req->complete);
        req->complete = NULL;
#endif
        return kStatus_SDHC_HostIsBusyError;
    }

    host->currentReq = req;

    if (kStatus_SDHC_NoError != SDHC_DRV_SendCommand(instance, req))
    {
        host->currentReq = 0;
        SDHC_DRV_SetClock(instance, false);
        req->error |= FSL_SDHC_REQ_ERR_SEND_CMD;
#if defined BSP_FSL_SDHC_USING_IRQ
        OSA_SemaDestroy(req->complete);
        OSA_MemFree(req->complete);
        req->complete = NULL;
#endif
        return kStatus_SDHC_Failed;
    }

#if defined BSP_FSL_SDHC_USING_IRQ
    do
    {
        if (!timeoutInMs)
        {
            status = OSA_SemaWait(req->complete, OSA_WAIT_FOREVER);
        }
        else
        {
            status = OSA_SemaWait(req->complete, timeoutInMs);
        }
    } while (status == kStatus_OSA_Idle);

    if (status != kStatus_OSA_Success)
    {
        req->error |= FSL_SDHC_REQ_ERR_TIMEOUT;
    }

    OSA_SemaDestroy(req->complete);
    OSA_MemFree(req->complete);
    req->complete = NULL;
#else
    uint32_t mask = 0, irqFlags = 0, i;
    mask = SDHC_HAL_CMD_COMPLETE_INT | SDHC_HAL_CMD_ERR_INT;
    if (kStatus_SDHC_NoError != SDHC_DRV_WaitInt(instance, mask, &irqFlags, timeoutInMs))
    {
        host->currentReq = 0;
        SDHC_DRV_SetClock(instance, false);
        SDHC_DRV_SetRequestError(req, irqFlags);
        return kStatus_SDHC_Failed;
    }

    if (irqFlags != SDHC_HAL_CMD_COMPLETE_INT)
    {
        SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], mask);
        host->currentReq = 0;
        SDHC_DRV_SetClock(instance, false);
        SDHC_DRV_SetRequestError(req, irqFlags);
        return kStatus_SDHC_Failed;
    }

    SDHC_HAL_ClearIntFlags(g_sdhcBaseAddr[instance], SDHC_HAL_CMD_COMPLETE_INT);
    if (g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_PRESENT)
    {
        req->response[0] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 0);
        if (!(g_req_resp_flags[req->respType] & FSL_SDHC_REQ_RSPTYPE_136BITS))
        {
            if ((req->respType == kSdhcRespTypeR1) ||
                    (req->respType == kSdhcRespTypeR1b))
            {
                req->cardErrStatus = SDMMC_R1_ERROR_BITS(req->response[0]);
            }
        }
        else
        {
            req->response[1] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 1);
            req->response[2] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 2);
            req->response[3] = SDHC_HAL_GetResponse(g_sdhcBaseAddr[instance], 3);
            i = 4;
            do {
                req->response[i - 1] <<= 8;
                if (i > 1)
                {
                    req->response[i - 1] |=
                        ((req->response[i-2] & 0xFF000000U) >> 24);
                }
            } while(i--);
        }
    }

    if ((!req->cardErrStatus) && (req->data))
    {
        ret = SDHC_DRV_TransferData(instance, req, timeoutInMs);
    }
#endif

    if (req->cardErrStatus)
    {
        ret = kStatus_SDHC_RequestCardStatusError;
    }

    if (req->error)
    {
        ret = kStatus_SDHC_RequestFailed;
    }

    host->currentReq = 0;
    SDHC_DRV_SetClock(instance, false);
    return ret;
}

/*************************************************************************************************
 * EOF
 ************************************************************************************************/


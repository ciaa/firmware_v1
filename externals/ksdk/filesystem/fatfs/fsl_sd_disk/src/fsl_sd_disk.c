/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#include <stdio.h>
#include <string.h>
#include "fsl_sdhc_driver.h"
#include "fsl_sdhc_card.h"
#include "fsl_sd_disk.h"
#include "diskio.h"

#define SDHC_INSTANCE_ID 0
static sdhc_card_t g_sdhcCard;
static sdhc_host_t g_sdhcHost;

#if _USE_WRITE
DRESULT sdcard_disk_write(uint8_t pdrv, const uint8_t *buff, uint32_t sector, uint8_t count)
{
    if (pdrv != SD)
    {
        return RES_PARERR;
    }
    if (g_sdhcCard.cardType == 0 || g_sdhcCard.cardType == kCardTypeUnknown)
    {
        return RES_NOTRDY;
    }
    if (kStatus_SDHC_NoError != SDCARD_DRV_WriteBlocks(&g_sdhcCard, (uint8_t *)buff, sector, count))
    {
        return RES_ERROR;
    }
    return RES_OK;
}
#endif

DRESULT sdcard_disk_read(uint8_t pdrv, uint8_t *buff, uint32_t sector, uint8_t count)
{
    if (pdrv != SD)
    {
        return RES_PARERR;
    }

    if (g_sdhcCard.cardType == 0 || g_sdhcCard.cardType == kCardTypeUnknown)
    {
        return RES_NOTRDY;
    }
    if (kStatus_SDHC_NoError != SDCARD_DRV_ReadBlocks(&g_sdhcCard, buff, sector, count))
    {
        return RES_ERROR;
    }
    return RES_OK;
}

#if _USE_IOCTL
DRESULT sdcard_disk_ioctl(uint8_t pdrv, uint8_t cmd, void *buff)
{
    DRESULT res = RES_OK;

    if (pdrv != SD)
    {
        return RES_PARERR;
    }

    switch(cmd)
    {
        case GET_SECTOR_COUNT:
            if (buff)
            {
                *(uint32_t *)buff = g_sdhcCard.blockCount;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case GET_SECTOR_SIZE:
            if (buff)
            {
                *(uint32_t *)buff = g_sdhcCard.blockSize;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case GET_BLOCK_SIZE:
            if (buff)
            {
                *(uint32_t *)buff = g_sdhcCard.csd.sectorSize;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case MMC_GET_TYPE:
            if (buff)
            {
                switch (g_sdhcCard.cardType)
                {
                    case kCardTypeMmc:
                        *(uint32_t *)buff = CT_MMC;
                        break;
                    case kCardTypeSd:
                        *(uint32_t *)buff = CT_SD1;
                        if ((g_sdhcCard.caps & SDMMC_CARD_CAPS_SDHC) ||
                            (g_sdhcCard.caps & SDMMC_CARD_CAPS_SDXC))
                        {
                            *(uint32_t *)buff = CT_SD2 | CT_BLOCK;
                        }
                        break;
                    default:
                        res = RES_PARERR;
                        break;
                }
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case MMC_GET_CSD:
            if (buff)
            {
                memcpy(buff, g_sdhcCard.rawCsd, sizeof(g_sdhcCard.rawCsd));
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case MMC_GET_CID:
            if (buff)
            {
                memcpy(buff, g_sdhcCard.rawCid, sizeof(g_sdhcCard.rawCid));
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case MMC_GET_OCR:
            if (buff)
            {
                *(uint32_t *)buff = g_sdhcCard.ocr;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        default:
            res = RES_PARERR;
            break;

    }

    return res;
}
#endif

DSTATUS sdcard_disk_status(uint8_t pdrv)
{
    if (pdrv != SD)
    {
        return STA_NOINIT;
    }

    return 0;
}

DSTATUS sdcard_disk_initialize(uint8_t pdrv)
{
    sdhc_user_config_t config = {0};
#if ! defined CD_USING_GPIO
    uint32_t retries = 5;
    uint32_t cardPresented = 0;
    sdhc_status_t status;
#endif

    if (pdrv != SD)
    {
        return STA_NOINIT;
    }

    SDHC_DRV_Shutdown(SDHC_INSTANCE_ID);
    memset(&g_sdhcHost, 0, sizeof(g_sdhcHost));
    memset(&g_sdhcCard, 0, sizeof(g_sdhcCard));

#if defined SDHC_USING_PIO
    config.transMode = kSdhcTransModePio;
#elif defined SDHC_USING_ADMA1
    config.transMode = kSdhcTransModeAdma1;
#else
    config.transMode = kSdhcTransModeAdma2;
#endif

    config.clock = SDMMC_CLK_100KHZ;
#if defined CD_USING_GPIO
    config.cdType = kSdhcCardDetectGpio;
#elif defined CD_USING_DAT3
    config.cdType = kSdhcCardDetectDat3;
    config.cardDetectCallback = sdhc_card_detection;
#elif defined CD_USING_POLL_DAT3
    config.cdType = kSdhcCardDetectPollDat3;
#else
#error unknown card detect type
#endif

    if (kStatus_SDHC_NoError != SDHC_DRV_Init(SDHC_INSTANCE_ID, &g_sdhcHost, &config))
    {
        return STA_NOINIT;
    }

#if ! defined CD_USING_GPIO
    do
    {
        status = SDHC_DRV_DetectCard(SDHC_INSTANCE_ID);
        if (kStatus_SDHC_NoMedium == status)
        {
            retries--;
        }
        else if (kStatus_SDHC_UnknownStatus == status)
        {
            SDCARD_DRV_Shutdown(&g_sdhcCard);
            SDHC_DRV_Shutdown(SDHC_INSTANCE_ID);
            return STA_NOINIT;
        }
        else
        {
            cardPresented = 1;
            break;
        }
    } while (retries);

    if (!retries && (!cardPresented))
    {
        SDCARD_DRV_Shutdown(&g_sdhcCard);
        SDHC_DRV_Shutdown(SDHC_INSTANCE_ID);
        memset(&g_sdhcHost, 0, sizeof(g_sdhcHost));
        memset(&g_sdhcCard, 0, sizeof(g_sdhcCard));
        return STA_NOINIT;
    }
#endif

    if (kStatus_SDHC_NoError != SDCARD_DRV_Init(&g_sdhcHost, &g_sdhcCard))
    {
        SDHC_DRV_Shutdown(SDHC_INSTANCE_ID);
        return STA_NOINIT;
    }
    return 0;
}

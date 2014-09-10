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

#ifndef __FSL_SDHC_CARD_DISK_H__
#define __FSL_SDHC_CARD_DISK_H__

#include "fsl_sdhc_card.h"
#include "diskio.h"

/*! @addtogroup sdhc_card_drv_data_types */
/*! @{ */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @} */

/*! @addtogroup sdhc_carddrv */
/*! @{ */

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

DSTATUS sdcard_disk_initialize(uint8_t pdrv);
DSTATUS sdcard_disk_status(uint8_t pdrv);
DRESULT sdcard_disk_read(uint8_t pdrv, uint8_t *buf, uint32_t sector, uint8_t count);
DRESULT sdcard_disk_write(uint8_t pdrv, const uint8_t *buf, uint32_t sector, uint8_t count);
DRESULT sdcard_disk_ioctl(uint8_t pdrv, uint8_t cmd, void *buff);

/*@} */
#if defined(__cplusplus)
}
#endif
/*! @} */
#endif

/*************************************************************************************************
 * EOF
 ************************************************************************************************/


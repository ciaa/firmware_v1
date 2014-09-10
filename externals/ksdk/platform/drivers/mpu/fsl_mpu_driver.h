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
#ifndef __FSL_MPU_DRIVER_H__
#define __FSL_MPU_DRIVER_H__

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_mpu_hal.h"

/*! 
 * @addtogroup mpu_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*! 
 * @brief Data c for the MPU region access permission initialize
 *
 * This structure is used when MPU_DRV_Init function is
 * called.
 */
typedef struct MpuAccessRights{
    uint32_t m0UserMode           : 3;  /*!< master0 access permission in user mode */
    uint32_t m0SupervisorMode     : 2;  /*!< master0 access permission in supervisor mode */
    uint32_t m0Process_identifier : 1;  /*!< master0 process identifier enable value */
    uint32_t m1UserMode           : 3;  /*!< master1 access permission in user mode */
    uint32_t m1SupervisorMode     : 2;  /*!< master1 access permission in supervisor mode */
    uint32_t m1Process_identifier : 1;  /*!< master1 process identifier enable value */
    uint32_t m2UserMode           : 3;  /*!< master2 access permission in user mode */
    uint32_t m2SupervisorMode     : 2;  /*!< master2 access permission in supervisor mode */
    uint32_t m2Process_identifier : 1;  /*!< master2 process identifier enable value */
    uint32_t m3UserMode           : 3;  /*!< master3 access permission in user mode */
    uint32_t m3SupervisorMode     : 2;  /*!< master3 access permission in supervisor mode */
    uint32_t m3Process_identifier : 1;  /*!< master3 process identifier enable value */
    uint32_t m4WriteControl       : 1;  /*!< master4 write access permission */
    uint32_t m4ReadControl        : 1;  /*!< master4 read access permission */
    uint32_t m5WriteControl       : 1;  /*!< master5 write access permission */
    uint32_t m5ReadControl        : 1;  /*!< master5 read access permission */
    uint32_t m6WriteControl       : 1;  /*!< master6 write access permission */
    uint32_t m6ReadControl        : 1;  /*!< master6 read access permission */
    uint32_t m7WriteControl       : 1;  /*!< master7 write access permission */
    uint32_t m7ReadControl        : 1;  /*!< master7 read access permission */
}mpu_access_rights_t;

/*! 
 * @brief Data v for MPU region initialize
 *
 * This structure is used when the MPU_DRV_Init function is
 * called.
 */
typedef struct MpuRegionConfig{
    mpu_region_num      regionNum;     /*!< MPU region number */
    uint32_t            startAddr;     /*!< memory region start address */
    uint32_t            endAddr;       /*!< memory region end address */
    mpu_access_rights_t accessRights;  /*!< all masters access permission */
}mpu_region_config_t;

/*! 
 * @brief Data The section describes the programming interface of the for MPU region initialize
 *
 * This structure is used when the MPU_DRV_Init function is
 * called.
 */
typedef struct MpuUserConfig{
    mpu_region_config_t   regionConfig; /*!< region access permission */
    struct MpuUserConfig  *next;        /*!< pointer to the next structure */
}mpu_user_config_t;

/*!
  * @brief Define type of user-defined callback function.
  */
typedef void (*mpu_callback_t)(void);

 /*!
  * @brief MPU driver user call back function.
  *
  * The contents of this structure provide a callback function.
  */
typedef struct MpuState {
     mpu_callback_t userCallbackFunc; /*!< Callback function that would be executed in ISR. */
}mpu_state_t;
 
 /*******************************************************************************
  * API
  *******************************************************************************/
 
 #if defined(__cplusplus)
 extern "C" {
 #endif
 
 /*! 
  * @name MPU Driver
  * @{
  */
 
   
 /*!
  * @brief Initializes the MPU driver.
  *
  * @param instance The MPU peripheral instance number.
  * @param userConfigPtr The pointer to the MPU user configure structure, see #mpu_user_config_t.
  * @param userStatePtr The pointer of run time structure.
  * @retval kStatus_MPU_Success means success. Otherwise, means failure.
  */
  mpu_status_t MPU_DRV_Init(uint32_t instance, mpu_user_config_t *userConfigPtr, mpu_state_t *userStatePtr);
 
 /*!
  * @brief Initializes the MPU region.
  *
  * @param instance The MPU peripheral instance number.
  * @param regionConfigPtr The pointer to the MPU user configure structure, see #mpu_user_config_t.
  * @retval kStatus_MPU_Success means success, otherwise, means failure.
  */
 mpu_status_t MPU_DRV_RegionInit(uint32_t instance, mpu_region_config_t *regionConfigPtr);
 
 /*!
  * @brief Deinitializes the MPU region.
  *
  * @param instance The MPU peripheral instance number.  
  * @retval kStatus_MPU_Success means success. Otherwise, means failure.
  */
 mpu_status_t MPU_DRV_Deinit(uint32_t instance);
 
 /*!
  * @brief Configures the MPU region access permission.
  *
  * @param instance The MPU peripheral instance number.
  * @param regionNum The MPU region number.
  * @param accessRights A pointer to access permission structure.
  * @retval kStatus_MPU_Success means success, otherwise, means failure.
  */
 mpu_status_t MPU_DRV_SetRegionAccessPermission(uint32_t instance, mpu_region_num regionNum, mpu_access_rights_t accessRights);
 
  /*!
  * @brief Gets the MPU region access permission.
  *
  * @param instance The MPU peripheral instance number.
  * @param regionNum The MPU region number.
  * @retval access permission.
  */
 mpu_access_rights_t MPU_DRV_GetRegionAccessPermission(uint32_t instance, mpu_region_num regionNum);
 
 /*!
  * @brief Checks whether the MPU region is valid.
  *
  * @param instance The MPU peripheral instance number.
  * @param regionNum MPU region number.
  * @retval bool value.
  */
 bool MPU_DRV_IsRegionValid(uint32_t instance, mpu_region_num regionNum);
 
  /*!
  * @brief Sets the MPU region valid.
  *
  * @param instance The MPU peripheral instance number.
  * @param regionNum MPU region number.
  */
 mpu_status_t MPU_DRV_SetRegionValid(uint32_t instance, mpu_region_num regionNum);
 
 /*!
  * @brief Installs the user-defined callback in MPU module.
  *
  * @param instance MPU instance ID.
  * @param userCallback User-defined callback function.
  * @retval kStatus_MPU_Success means succeed, otherwise means failed.
  */
 mpu_status_t MPU_DRV_InstallCallback(uint32_t instance, mpu_callback_t userCallback);
 
 /*!
  * @brief Driver-defined ISR in MPU module. 
  *
  * @param instance MPU instance ID.
  */
 void MPU_DRV_IRQHandler(uint32_t instance);

 /*@}*/
 
 #if defined(__cplusplus)
 }
 #endif
 
 /*! @}*/
 
 #endif /* __FSL_MPU_H__*/
 /*******************************************************************************
  * EOF
  *******************************************************************************/


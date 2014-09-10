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
#include "fsl_mpu_driver.h"
#include "fsl_mpu_common.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static mpu_state_t *volatile mpu_state_ptrs[HW_MPU_INSTANCE_COUNT];

/*******************************************************************************
 * Code
 *******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetRegionValid
 * Description   : set a region  valid.
 * This function is used to set a region valid.
 *
 *END**************************************************************************/
mpu_status_t MPU_DRV_SetRegionValid(uint32_t instance, mpu_region_num regionNum)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];

    MPU_HAL_SetRegionValidValue(baseAddr, regionNum, kMPURegionValid);
    
    return kStatus_MPU_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_RegionInit
 * Description   : MPU region init.
 * This function is used to initialize a MPU region.
 * Note: when writing to a region's word0~word3 will caused the region invalid
 * so the region must be set valid by manual.
 *END**************************************************************************/
mpu_status_t MPU_DRV_RegionInit(uint32_t instance, mpu_region_config_t *regionConfigPtr)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];
    
    uint32_t *accessControl = (uint32_t*)&(regionConfigPtr->accessRights);

    MPU_HAL_SetRegionStartAddr(baseAddr, regionConfigPtr->regionNum, regionConfigPtr->startAddr);
    
    MPU_HAL_SetRegionEndAddr(baseAddr, regionConfigPtr->regionNum, regionConfigPtr->endAddr);
    
    MPU_HAL_SetAllMastersAlternateAccessRights(baseAddr, regionConfigPtr->regionNum, *accessControl);
    
    MPU_HAL_SetRegionValidValue(baseAddr, regionConfigPtr->regionNum, kMPURegionValid);
    
    return kStatus_MPU_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_Init
 * Description   : MPU module init.
 * This function is used to initialize MPU regions.
 *
 *END**************************************************************************/
mpu_status_t MPU_DRV_Init(uint32_t instance, mpu_user_config_t *userConfigPtr, mpu_state_t *userStatePtr)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];
    
    CLOCK_SYS_EnableMpuClock(instance);
    
    if(!userConfigPtr)
    {
        return kStatus_MPU_NullArgument;
    }
        
    MPU_HAL_Init(baseAddr);
    
    while(userConfigPtr)
    {
        MPU_DRV_RegionInit(instance, &(userConfigPtr->regionConfig));
        userConfigPtr = userConfigPtr->next;
    }
    
    mpu_state_ptrs[instance] = userStatePtr;
    
    MPU_HAL_Enable(baseAddr);
    
    return kStatus_MPU_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_Deinit
 * Description   : MPU module deinit.
 * This function is used to deinit MPU module---disable MPU and disable each region.
 *
 *END**************************************************************************/
mpu_status_t MPU_DRV_Deinit(uint32_t instance)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];
    
    MPU_HAL_Init(baseAddr);
    
    mpu_state_ptrs[instance] = NULL;
    
    CLOCK_SYS_DisableMpuClock(instance);
    
    return kStatus_MPU_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_GetRegionAccessPermission
 * Description   : Get region access permission.
 * This function is used to get a MPU region access permission of all masters.
 *
 *END**************************************************************************/
mpu_access_rights_t MPU_DRV_GetRegionAccessPermission(uint32_t instance, mpu_region_num regionNum)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];
    
    uint32_t permissions = MPU_HAL_GetAllMastersAccessRights(baseAddr, regionNum);
    
    mpu_access_rights_t *accessRights = (mpu_access_rights_t*)(&permissions);

    return (*accessRights);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_SetRegionAccessPermission
 * Description   : Set region access permission.
 * This function is used to Set a MPU region access permission for all masters.
 *
 *END**************************************************************************/
mpu_status_t MPU_DRV_SetRegionAccessPermission(uint32_t instance, mpu_region_num regionNum, mpu_access_rights_t accessRights)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];
    
    uint32_t *accessControl = (uint32_t*)(&(accessRights));
    
    MPU_HAL_SetAllMastersAlternateAccessRights(baseAddr, regionNum, *accessControl);

    return kStatus_MPU_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MPU_DRV_IsRegionValid
 * Description   : check if a region is valid.
 * This function is used to check if a region is valid.
 *
 *END**************************************************************************/
bool MPU_DRV_IsRegionValid(uint32_t instance, mpu_region_num regionNum)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    uint32_t baseAddr = g_mpuBaseAddr[instance];
    
    return MPU_HAL_IsRegionValid(baseAddr, regionNum);
}
/*FUNCTION*********************************************************************
 *
 * Function Name : MPU_DRV_InstallCallback
 * Description   : Install the user-defined callback in MPU module.
 * When an MPU interrupt request is served, the callback will be executed 
 * inside the ISR.
 *
 *END*************************************************************************/
mpu_status_t MPU_DRV_InstallCallback(uint32_t instance, mpu_callback_t userCallback)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    if(!mpu_state_ptrs[instance])
    {
        return kStatus_MPU_NotInitlialized;
    }
    /* Fill callback function into state structure. */
    mpu_state_ptrs[instance]->userCallbackFunc = userCallback;
    
    return kStatus_MPU_Success;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : MPU_DRV_IRQHandler
 * Description   : The driver-defined ISR in MPU module. 
 * It includes the process for interrupt mode defined by driver. Currently, it
 * will be called inside the system-defined ISR.
 *
 *END*************************************************************************/
void MPU_DRV_IRQHandler(uint32_t instance)
{
    assert(instance < HW_MPU_INSTANCE_COUNT);
    
    if (mpu_state_ptrs[instance])
    {
        if (mpu_state_ptrs[instance]->userCallbackFunc)
        {
            /* Execute user-defined callback function. */
            (*(mpu_state_ptrs[instance]->userCallbackFunc))();
        }
    }
}
/*******************************************************************************
 * EOF
 *******************************************************************************/


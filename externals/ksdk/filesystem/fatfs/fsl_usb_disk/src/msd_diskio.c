/**HEADER********************************************************************
* 
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: msd_diskio.c$
* $Version : 
* $Date    : 
*
* Comments: This file implements low level disk interface module for msd
*
*   
*
*END************************************************************************/
#include "msd_diskio.h"        /* FatFs lower layer API */

/**************************************************************************
   Micro variables
**************************************************************************/
#if (defined (__CWCC__) && defined(_MK_xxx_H_))
#define _ALIGNED_ __attribute__((aligned (4)))
#elif (defined (__CWCC__) && !defined(_MK_xxx_H_))
#define _ALIGNED_
#pragma pack(4)
#elif (defined __IAR_SYSTEMS_ICC__)
#pragma data_alignment=4
#define _ALIGNED_
#elif defined(__CC_ARM)
#pragma pack(4)
#define _ALIGNED_
#elif defined(__GNUC__)
#define _ALIGNED_
#else
#define _ALIGNED_
#error "Toolchain not supported!"
#endif

#define UNUSED(x)                                  (void)x;

/**************************************************************************
   Global variables
**************************************************************************/
volatile device_struct_t                g_mass_device[USBCFG_MAX_INSTANCE] = { 0 };   /* mass storage device struct */
volatile uint8_t                        g_mass_device_new_index = 0;

/**************************************************************************
*   Local variables
**************************************************************************/
static volatile bool                    g_bCallBack = FALSE;
static volatile usb_status              g_bStatus = USB_OK;
/* cmd object used in SCSI commands */ 
static _ALIGNED_ mass_command_struct_t  g_pCmd;
static void command_callback(usb_status, void *, void *, uint32_t);
/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : msd_disk_initialize
* Returned Value : 
* Comments       : Initialize Disk Drive (always successful) 
*
*END*--------------------------------------------------------------------*/
DSTATUS msd_disk_initialize (
    /* [IN] Physical drive number (0) */
    uint8_t pdrv
)
{
    DSTATUS stat = STA_OK;    
    static cbw_struct_t * cbw_ptr = NULL; 
    static csw_struct_t * csw_ptr = NULL;
    
    /* Allocate memory for cmd Block Wrapper and cmd Status Wrapper */
    if(NULL == cbw_ptr)
    {
        cbw_ptr = (cbw_struct_t *) malloc(sizeof(cbw_struct_t));      
    }
    
    g_pCmd.CBW_PTR = cbw_ptr;
    
    if(NULL == g_pCmd.CBW_PTR)
    {
        return STA_NOINIT;
    }
    
    if(NULL == csw_ptr)
    {
        csw_ptr = (csw_struct_t *) malloc(sizeof(csw_struct_t));
    }
    
    g_pCmd.CSW_PTR = csw_ptr;
    
    if(NULL == g_pCmd.CSW_PTR)
    {
        return STA_NOINIT;    
    }
    
    /* Store the address of CBW and CSW */
    cbw_ptr = g_pCmd.CBW_PTR;
    csw_ptr = g_pCmd.CSW_PTR;
    
    memset(g_pCmd.CSW_PTR, 0, sizeof(csw_struct_t));
    memset(g_pCmd.CBW_PTR, 0, sizeof(cbw_struct_t));
    memset(&g_pCmd, 0, sizeof(mass_command_struct_t));
    
    /* Init SCSI command object */
    g_pCmd.CBW_PTR  = cbw_ptr;
    g_pCmd.CSW_PTR  = csw_ptr;
    g_pCmd.LUN      = pdrv;
    g_pCmd.CLASS_PTR = (void *)g_mass_device[g_mass_device_new_index].CLASS_HANDLE;
    g_pCmd.CALLBACK = command_callback;
    
    disk_ioctl(pdrv, UFI_TEST_UNIT_READY_CMD, NULL);      
    disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL);
    disk_ioctl(pdrv, UFI_INQUIRY_CMD, NULL);      
    disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL);
    disk_ioctl(pdrv, UFI_READ_CAPACITY_CMD, NULL);
    disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL);
    return stat;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : msd_disk_status
* Returned Value : STA_OK: drive is ready to write and read
*                  STA_NOINIT: dis is not initialized
* Comments       : Get the status of drive
*
*END*--------------------------------------------------------------------*/
DSTATUS msd_disk_status (
    uint8_t pdrv        /* Physical drive nmuber (0..) */
)
{
    DSTATUS stat = STA_OK;
    return stat;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : msd_disk_read
* Returned Value : 
* Comments       : Read Sector(s) 
*
*END*--------------------------------------------------------------------*/
DRESULT msd_disk_read (
    /* [IN] Physical drive number (0) */
    uint8_t pdrv, 
    /* [OUT] Pointer to the data buffer to store read data */
    uint8_t *buff, 
    /* [IN] Start sector number (LBA) */
    uint32_t sector, 
    /* [IN] Number of sectors to read (1..128) */
    uint8_t count
)
{
    DRESULT         res = RES_ERROR;
    usb_status      status = USB_OK;
    uint32_t        i = MAX_RETRY_TIMES;
    
    /* Check sector count */
    if (!count) return RES_PARERR;
    
    for(;i>0;i--)
    {
        /* disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL); */
        /* Send read_10 SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_read_10(&g_pCmd, sector, buff, (uint32_t)(MSD_DEFAULT_SECTOR_SIZE*count), count);
        if (status != USB_OK)
        {
            res = RES_ERROR;
        }
        else
        {
            while(!g_bCallBack)    /* Wait till command comes back */
            {
              #if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
                      Poll();
              #elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
                          OSA_PollAllOtherTasks();
              #endif
            }
            
            if(g_pCmd.IS_STALL_IN_DPHASE>0)
            {
                continue;
            }
            
            if (!g_bStatus) 
            {
                res = RES_OK;
                break;
            }
            else 
            {
                res = RES_NOTRDY;
            }
        }
        /* disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL); */
    }
    return res;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : msd_disk_write
* Returned Value : 
* Comments       : Write Sector(s) 
*
*END*--------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT msd_disk_write (
    /* [IN] Physical drive nmuber (0) */
    uint8_t pdrv,        
    /* [IN] Pointer to the data to be written */    
    const uint8_t *buff,    
    /* [IN] Start sector number (LBA) */
    uint32_t sector,        
    /* [IN] Number of sectors to write (1..128) */
    uint8_t count    
)
{
    DRESULT         res = RES_ERROR;
    usb_status      status = USB_OK;
    uint32_t        i = MAX_RETRY_TIMES;

    /* Check sector count */
    if (!count) return RES_PARERR;
    
    for(;i>0;i--)
    {
        /* disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL); */
        
        /* Send Write_10 SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_write_10(&g_pCmd, sector, (uint8_t*)buff, (uint32_t)(MSD_DEFAULT_SECTOR_SIZE*count), count);
        if (status != USB_OK)
        {
            res = RES_ERROR;
        }
        else
        {
            while(!g_bCallBack)    /* Wait till command comes back */
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }
            
            if(g_pCmd.IS_STALL_IN_DPHASE>0)
            {
                continue;
            }
            
            if (!g_bStatus) 
            {
                res = RES_OK;
                break;
            }
            else 
            {
                res = RES_NOTRDY;
            }         
        }
        /* disk_ioctl(pdrv, UFI_REQUEST_SENSE_CMD, NULL); */
    }
    return res;
}
#endif


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : msd_disk_ioctl
* Returned Value : 
* Comments       : The disk_ioctl function controls device specified features
*                  and miscellaneous functions other than disk read/write 
*
*END*--------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT msd_disk_ioctl (
    /* [IN] Physical drive nmuber (0..) */
    uint8_t pdrv,      
    /* [IN] Control command code */
    uint8_t cmd, 
    /* [IN/OUT] Parameter or data buffer */   
    void* buff
)
{
    _ALIGNED_ DRESULT                                              res = RES_ERROR;
    _ALIGNED_ usb_status                                           status = USB_OK;
    static _ALIGNED_ mass_storage_read_capacity_cmd_struct_t       read_capacity;
    static _ALIGNED_ capacity_list_struct_t                        capacity_list;
    static _ALIGNED_ inquiry_data_format_struct_t                  inquiry;
    static _ALIGNED_ req_sense_data_format_struct_t                req_sense;
    uint32_t *                                                     param_ptr = buff;

    switch (cmd)
    {
    case UFI_TEST_UNIT_READY_CMD:
        /* Send test unit ready SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_test_unit_ready(&g_pCmd);
        if (status != USB_OK)
        {
            res = RES_ERROR;         
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)    
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }
            
            if (!g_bStatus) 
            {                                
                res = RES_OK;  
            }
            else 
            {              
                res = RES_NOTRDY;                    
            }         
        }                  
        break;
    case UFI_READ_CAPACITY_CMD:
        /* Send read_capacity SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_read_capacity(&g_pCmd, (uint8_t *)&read_capacity,
                                            sizeof(mass_storage_read_capacity_cmd_struct_t));
        if (status != USB_OK)
        {
            res = RES_ERROR;         
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)    
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }     
            
            if (!g_bStatus) 
            {
                res = RES_OK;   
            }
            else 
            {
                res = RES_NOTRDY;  
            }         
        }                  
        break;
    case UFI_READ_FORMAT_CAPACITY_CMD:
        /* Send read_capacity SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_format_capacity(&g_pCmd, (uint8_t *)&capacity_list,\
            sizeof(capacity_list_struct_t));
        if (status != USB_OK)
        {
            res = RES_ERROR;   
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)    
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }
            
            if (!g_bStatus) 
            {
                res = RES_OK;    
            }
            else 
            {
                res = RES_NOTRDY;  
            }         
        }                  
        break;
    case UFI_INQUIRY_CMD:
        /* Send read_capacity SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_inquiry(&g_pCmd, (uint8_t *) &inquiry,
                                      sizeof(inquiry_data_format_struct_t));
        if (status != USB_OK)
        {
            res = RES_ERROR;  
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)    
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }
            
            if (!g_bStatus) 
            {
                inquiry.BVID[7] = 0;
                inquiry.BPID[15] = 0;
                inquiry.BPRODUCT_REV[3] = 0;
                res = RES_OK;
            }
            else 
            {
                res = RES_NOTRDY;  
            }
        }                  
        break;
#if 0
    case UFI_MODE_SENSE_CMD:
        /* Send mode sense SCSI command */
        g_bCallBack = FALSE;
        g_bStatus = usb_mass_ufi_mode_sense(&g_pCmd,
                                            2, /* PC */
                                            0x3F, /* page code */
                                            buff_in,
                                            (uint32_t)0x08);
        if (g_bStatus != USB_OK)
        {
            res = RES_ERROR;
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            if (!g_bStatus)
                res = RES_OK;
            else
                res = RES_NOTRDY;      
        }
        break;
#endif
    case UFI_REQUEST_SENSE_CMD:
        /* Send read_capacity SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_request_sense(&g_pCmd, &req_sense,
                                            sizeof(req_sense_data_format_struct_t));
        if (status != USB_OK)
        {
            res = RES_ERROR;  
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }
            
            if (!g_bStatus)
            {
                res = RES_OK; 
            }
            else 
            {
                res = RES_NOTRDY; 
            }
        }          
        break;
    case GET_SECTOR_COUNT:
    case GET_SECTOR_SIZE:
        /* Send read_capacity SCSI command */
        g_bCallBack = FALSE;
        status = usb_mass_ufi_read_capacity(&g_pCmd, (uint8_t *)&read_capacity,\
            sizeof(mass_storage_read_capacity_cmd_struct_t));
        if (status != USB_OK)
        {
            res = RES_ERROR;   
        }
        else
        {
            /* Wait till command comes back */
            while(!g_bCallBack)
            {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        Poll();
#elif ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && !(USE_RTOS))
            OSA_PollAllOtherTasks();
#endif
            }
            
            if (!g_bStatus)
            {
                res = RES_OK;    
            }
            else
            {
                res = RES_NOTRDY;  
            } 
        }
        
        if(!buff)
            return res = RES_ERROR;
        
        /* Get number of sectors on the disk (DWORD) */
        if (GET_SECTOR_COUNT == cmd)
        {
            *(uint32_t *)buff = SWAP4BYTE_CONST(*((uint32_t *)(read_capacity.BLLBA))) + 1;
        }
        /* Get the sector size in byte */
        else
        {
            *(uint32_t *)buff = SWAP4BYTE_CONST(*((uint32_t *)(read_capacity.BLENGTH)));
        }
        break;          
    case GET_BLOCK_SIZE:
        if(!buff)
            return res = RES_ERROR;
        
        *(uint32_t*)buff = ERASE_BLOCK_SIZE;
        res = RES_OK;           
        break;
    case CTRL_SYNC:
        res = RES_OK;    
        break;
    case GET_VENDOR_INFO:
        *param_ptr = (uint32_t)&inquiry.BVID;
        break;
    case GET_PRODUCT_ID:
        *param_ptr = (uint32_t)&inquiry.BPID;
        break;
    case GET_PRODUCT_REV:
        *param_ptr = (uint32_t)&inquiry.BPRODUCT_REV;
        break;
    default:
        res = RES_PARERR; 
        break;
    }  
    return res;
}
#endif

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : command_callback
* Returned Value : None
* Comments       : Called on completion of a control-pipe transaction.
*
*END*--------------------------------------------------------------------*/
static void command_callback
   (
      /* [IN] Status of this command */
      usb_status status,

      /* [IN] void * to USB_MASS_BULK_ONLY_REQUEST_STRUCT*/
      void *  p1,

      /* [IN] void * to the command object*/
      void *  p2,

      /* [IN] Length of data transmitted */
      uint32_t buffer_length
   )
{ /* Body */

   UNUSED(p1)
   UNUSED(p2)
   UNUSED(buffer_length)

   g_bCallBack = TRUE;
   g_bStatus = status;

} /* Endbody */

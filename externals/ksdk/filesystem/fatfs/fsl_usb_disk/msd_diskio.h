/**HEADER********************************************************************
* 
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
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
 * $FileName: msd_diskio.h$
 * $Version : 
 * $Date    : 
 *
 * Comments:
 *
 *
 *
 *END************************************************************************/

#ifndef _MSD_DISKIO_H
#define _MSD_DISKIO_H

#include "usb_host_config.h"
#include "usb.h"
#include "usb_host_stack_interface.h"
//#include "derivative.h"
#include "usb_host_msd_ufi.h"
#include "diskio.h"

#define USBCFG_MAX_INSTANCE                                     4
/* Storage information*/
#define MSD_DEFAULT_SECTOR_SIZE                                 (512)
#define MAX_RETRY_TIMES                                         (3)
     
/* USB MSD ioctl command */
#define UFI_REQUEST_SENSE_CMD                                   0x81
#define UFI_INQUIRY_CMD	                                        0x82
#define UFI_READ_FORMAT_CAPACITY_CMD                            0x83
#define UFI_READ_CAPACITY_CMD                                   0x84
#define UFI_TEST_UNIT_READY_CMD                                 0x85
#define UFI_MODE_SENSE_CMD                                      0x86
#define UFI_READ10_CMD                                          0x87

#define GET_VENDOR_INFO                                         0x91
#define GET_PRODUCT_ID                                          0x92
#define GET_PRODUCT_REV                                         0x93     

#define ERASE_BLOCK_SIZE                                        1 /* Unknown erasable block size*/

/* extend for DSTATUS */
#define STA_OK                                                  0x00 /* Drive is ready */

typedef struct device_struct
{
    uint32_t                        dev_state;  /* Attach/detach state */
    usb_device_instance_handle      dev_handle;
    usb_interface_descriptor_handle intf_handle;
    class_handle                     CLASS_HANDLE; /* Class-specific info */
} device_struct_t;

extern volatile device_struct_t                        g_mass_device[USBCFG_MAX_INSTANCE];
extern volatile uint8_t                                g_mass_device_new_index;

extern DSTATUS msd_disk_initialize (uint8_t pdrv);
extern DSTATUS msd_disk_status (uint8_t pdrv);
extern DRESULT msd_disk_read (uint8_t pdrv, uint8_t *buff, uint32_t sector, uint8_t count);
extern DRESULT msd_disk_write (uint8_t pdrv, const uint8_t *buff, uint32_t sector, uint8_t count);
extern DRESULT msd_disk_ioctl (uint8_t pdrv, uint8_t cmd, void* buff);
#endif /* _MSD_DISKIO_H */

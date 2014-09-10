/**HEADER********************************************************************
*
* Copyright (c) 2010, 2013 - 2014 Freescale Semiconductor;
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
* $FileName: diskio.c$
* $Version :
* $Date    :
*
* Comments: This file implements low level disk interface module
*
*
*
*END************************************************************************/
/**************************************************************************
   Micro variables
**************************************************************************/
#if USB_DISK_ENABLE
#include "msd_diskio.h"
#endif
#if SD_DISK_ENABLE
#include "fsl_sd_disk.h"
#endif
#include "diskio.h"        /* FatFs lower layer API */


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : disk_initialize
* Returned Value :
* Comments       : Initialize Disk Drive (always successful)
*
*END*--------------------------------------------------------------------*/
DSTATUS disk_initialize (
    /* [IN] Physical drive number (0) */
    uint8_t pdrv
)
{
    DSTATUS stat = STA_NOINIT;

    switch (pdrv)
    {
#if USB_DISK_ENABLE
    case USB:
        return msd_disk_initialize(pdrv);
#endif
#if SD_DISK_ENABLE
    case SD:
        return sdcard_disk_initialize(pdrv);
#endif
//    default:
//        return STA_NOINIT;
    }
    return stat;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : disk_status
* Returned Value : STA_OK: drive is ready to write and read
*                  STA_NOINIT: dis is not initialized
* Comments       : Get the status of drive
*
*END*--------------------------------------------------------------------*/
DSTATUS disk_status (
    uint8_t pdrv        /* Physical drive nmuber (0..) */
)
{
    DSTATUS stat = STA_NOINIT;

    switch (pdrv)
    {
#if USB_DISK_ENABLE
    case USB:
        return msd_disk_status(pdrv);
#endif
#if SD_DISK_ENABLE
    case SD:
        return sdcard_disk_status(pdrv);
#endif
    }
    return stat;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : disk_read
* Returned Value :
* Comments       : Read Sector(s)
*
*END*--------------------------------------------------------------------*/
DRESULT disk_read (
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
    DRESULT res = RES_PARERR;

    switch (pdrv)
    {
#if USB_DISK_ENABLE
    case USB:
        return msd_disk_read(pdrv, buff, sector, count);
#endif
#if SD_DISK_ENABLE
    case SD:
        return sdcard_disk_read(pdrv, buff, sector, count);
#endif
    }
    return res;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : disk_write
* Returned Value :
* Comments       : Write Sector(s)
*
*END*--------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
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
    DRESULT res = RES_PARERR;

    switch (pdrv)
    {
#if USB_DISK_ENABLE
    case USB:
        return msd_disk_write(pdrv, buff, sector, count);
#endif
#if SD_DISK_ENABLE
    case SD:
        return sdcard_disk_write(pdrv, buff, sector, count);
#endif
    }
    return res;
}
#endif


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : disk_ioctl
* Returned Value :
* Comments       : The disk_ioctl function controls device specified features
*                  and miscellaneous functions other than disk read/write
*
*END*--------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT disk_ioctl (
    /* [IN] Physical drive nmuber (0..) */
    uint8_t pdrv,
    /* [IN] Control command code */
    uint8_t cmd,
    /* [IN/OUT] Parameter or data buffer */
    void* buff
)
{
    DRESULT res = RES_PARERR;

    switch (pdrv)
    {
#if USB_DISK_ENABLE
    case USB:
        return msd_disk_ioctl(pdrv, cmd, buff);
#endif
#if SD_DISK_ENABLE
    case SD:
        return sdcard_disk_ioctl(pdrv, cmd, buff);
#endif
    }
    return res;
}
#endif

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_fattime
* Returned Value : Currnet time is returned with packed into a unit_32 value.
*                  The bit field is as follows:
*                       bit31:25  Year from 1980 (0..127)
*                       bit24:21  Month (1..12)
*                       bit20:16  Day in month(1..31)
*                       bit15:11  Hour (0..23)
*                       bit10:5   Minute (0..59)
*                       bit4:0    Second / 2 (0..29)
* Comments       : User Provided Timer Function for FatFs module
*
*END*--------------------------------------------------------------------*/
uint32_t get_fattime (void)
{
    return	  ((uint32_t)(2013 - 1980) << 25)	/* Fixed to Jan. 1, 2010 */
			| ((uint32_t)1 << 21)
			| ((uint32_t)1 << 16)
			| ((uint32_t)0 << 11)
			| ((uint32_t)0 << 5)
			| ((uint32_t)0 >> 1);
}



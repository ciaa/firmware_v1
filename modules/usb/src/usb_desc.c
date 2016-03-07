/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_DESC Standard Descriptors
 * @{
 */


/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>

#include "usb.h"  /* for return status */
#include "usbd.h" /* for assert ?? */
#include "usb_std.h"
#include "usb_desc.h"


/*==================[external functions definition]==========================*/

int usb_goto_next_desc(
      const uint8_t** pbuff,
      uint16_t*       plen,
      usb_stddesc_t   desc_type,
      uint16_t        desc_size
)
{
   int            ret = -1;
   const uint8_t* buff;
   int32_t        len; /* Signed to identify underflow on uint16_t variable. */
   uint16_t       desc_len;

   usb_assert( pbuff != NULL);
   usb_assert(*pbuff != NULL);
   usb_assert( plen  != NULL);

   buff = *pbuff;
   len  = (uint32_t) *plen;

   /* Assume we start on a descriptor, the first byte is the length. */
   desc_len = buff[0];
   buff    += desc_len;
   len     -= desc_len;
   while (len >= desc_size && ret == -1)
   {
      /* Get length from first byte. */
      desc_len = buff[0];
      if ((desc_len == desc_size) &&
            (buff[1] == desc_type))
      {
         /* Descriptor found! */
         *pbuff = buff;
         *plen  = (uint16_t) len;
         ret = 0;
      }
      buff += desc_len;
      len  -= desc_len;
   }
   return ret;
}

int usb_goto_iface_desc(
      const uint8_t** pbuff,
      uint16_t*       plen,
      uint8_t         iface_idx,
      uint8_t         alt_set
)
{
   int            ret;
   const uint8_t* buff;
   int16_t        len;

   usb_assert( pbuff != NULL);
   usb_assert(*pbuff != NULL);
   usb_assert( plen  != NULL);

   buff = *pbuff;
   len  = *plen;

   ret = 1;
   while (ret > 0)
   {
      /* Go to the next interface descriptor. */
      ret = usb_goto_next_desc(
            &buff,
            &len,
            USB_STDDESC_INTERFACE,
            USB_STDDESC_IFACE_SIZE
      );
      if (ret == 0)
      {
         /* Now check the interface number and alternate setting. */
         if (USB_STDDESC_IFACE_GET_bInterfaceNumber(buff) != iface_idx)
         {
            ret = 1;
         }
         else if (USB_STDDESC_IFACE_GET_bAlternateSetting(buff) != alt_set)
         {
            ret = 1;
         }
         else
         {
            /* Found!  Copy values to output. */
            *pbuff = buff;
            *plen  = len;
            ret = 0;
         }
      }
   }
}


/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


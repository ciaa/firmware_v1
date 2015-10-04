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
      uint8_t*        plen,
      usb_stddesc_t   desc_type,
      uint8_t         desc_size
)
{
   int            ret = -1;
   const uint8_t* buff;
   int16_t        len; /* Signed to identify underflow on uint8_t variable. */
   uint8_t        desc_len;

   usb_assert(pbuff  != NULL);
   usb_assert(*pbuff != NULL);
   usb_assert(plen   != NULL);

   buff = *pbuff;
   len  = (uint16_t) *plen;

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
         *plen  = (uint8_t) len;
         ret = 0;
      }
      buff += desc_len;
      len  -= desc_len;
   }
   return ret;
}


/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


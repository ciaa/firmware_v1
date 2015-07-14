#include <stdio.h>
#include <stdint.h>
#include "usb.h" /* for return status */
#include "usbd.h" /* for assert ?? */
#include "usb_std.h"
#include "usb_desc.h"

int usb_goto_next_epdesc( const uint8_t** pbuff, uint8_t* plen )
{
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
	buff += desc_len;
	len  -= desc_len;
	while (len >= USB_STDDESC_EP_SIZE)
	{
		desc_len = buff[0];
		if ((desc_len == USB_STDDESC_EP_SIZE) &&
				(buff[1] == USB_STDDESC_ENDPOINT))
		{
			/* Endpoint descriptor found! */
			*pbuff = buff;
			*plen  = (uint8_t) len;
			return USB_STATUS_OK;
		}
		buff += desc_len;
		len  -= desc_len;
	}
	return USB_STATUS_EP_NA;
}


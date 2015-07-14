#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "usb.h"
#include "usbd.h"
#include "usb_desc.h"

#include "drivers/usb_hid.h"


static hid_stack_t hid_stack; /* Maybe this shouldn't be static... */


int hid_probe( const uint8_t* buffer, const uint8_t length )
{
	usb_desc_iface_t* piface_desc;
	usb_desc_ep_t*    pep_desc;
	const uint8_t*    buff;
	uint8_t           len;
	int               status;

	usb_assert(buffer != NULL);
	usb_assert(length >= USB_STDDESC_IFACE_SIZE);
	piface_desc = (usb_desc_iface_t*)buffer;

	if (piface_desc->bInterfaceClass != USB_CLASS_HID)
		return USB_STATUS_DRIVER_FAIL;

	/* HID interfaces may only have ONE endpoint in addition to the ctrl one. */
	if (piface_desc->bNumEndpoints > 1)
		return USB_STATUS_DRIVER_FAIL;

	/* If it has the additional endpoint, it must be an interrupt IN one. */
	if (piface_desc->bNumEndpoints == 1)
	{
		buff = buffer;
		len  = length;
		status = usb_goto_next_epdesc(&buff, &len);
		if (status)
			usb_assert(0); /** @TODO: handle error */
		usb_assert(len >= USB_STDDESC_EP_SIZE);
		pep_desc = (usb_desc_ep_t*)buff;

		/* Direction must be IN. */
		if (!(pep_desc->bEndpointAddress & USB_DIR_MASK))
			return USB_STATUS_DRIVER_FAIL;

		/* And type Interrupt. */
		if ((pep_desc->bmAttributes & USB_STDDESC_EP_ATTR_TYPE_MASK) != USB_INT)
			return USB_STATUS_DRIVER_FAIL;
	}

	return USB_STATUS_OK;
}

int hid_assign( usb_stack_t* pstack, const uint16_t id )
{
	int        new_idx;
	hid_dev_t* pdev;

	usb_assert(pstack != NULL);

	new_idx = hid_get_free_dev();
	if (new_idx < 0)
		return -1; /* No more room for HID devices... */ /** @TODO: use an actual error code. */

	pdev = &hid_stack.devices[new_idx];
	pdev->status &= ~HID_STATUS_FREE;
	pdev->state   =  HID_STATE_INIT;
	pdev->pstack  =  pstack;
	pdev->id      =  id;
	hid_stack.n_devices++;

//	return hid_init_dev(new_idx); /* ???? */
	return 0; /* ???? */ /** @TODO: use an actual error code. */
}

int hid_remove( usb_stack_t* pstack, const uint16_t id )
{
	uint8_t    i;
	hid_dev_t* pdev;

	/* First, search for device in driver's structure. */
	for (i = 0; i < HID_MAX_DEVICES; ++i)
	{
		pdev = &hid_stack.devices[i];
		if ((pdev->pstack == pstack) && (pdev->id == id))
		{
			/* When found, remove it. */
			return hid_dev_deinit(i);
		}
	}
	/* Device couldn't be found in driver. */
	return -1; /** @TODO: use an actual error code. */
}


/******************************************************************************/
int hid_init( void )
{
	uint8_t    i;
	for (i = 0; i < HID_MAX_DEVICES; ++i)
		hid_dev_deinit(i);
	hid_stack.n_devices = 0;
	return 0;
}

int hid_dev_deinit( uint8_t index )
{
	hid_dev_t* pdev;
	usb_assert(index < HID_MAX_DEVICES);

	pdev = &hid_stack.devices[index];
	pdev->status   = HID_STATUS_FREE;
	pdev->pstack   = NULL;
	pdev->id       = 0xFFFF;
	pdev->protocol = HID_PROTO_NONE;
	pdev->state    = HID_STATE_INIT;

	return 0;
}

int hid_get_free_dev( void )
{
	uint8_t i;
	for (i = 0; i < HID_MAX_DEVICES; ++i)
	{
		if (hid_stack.devices[i].status & HID_STATUS_FREE)
		{
			hid_stack.devices[i].status &= ~HID_STATUS_FREE;
			return i;
		}
	}
	return -1; /** @TODO: use an actual error code ??. */
}

int hid_update( void )
{
	uint8_t i;

	for (i = 0; i < HID_MAX_DEVICES; ++i)
	{
		if (!(hid_stack.devices[i].status & HID_STATUS_FREE))
			hid_update_device(i);
	}
}

int hid_update_device( uint8_t index )
{
	hid_dev_t* pdev;
	usb_assert(index < HID_MAX_DEVICES);
	pdev = &hid_stack.devices[index];

	switch (pdev->state)
	{
		case HID_STATE_IDLE:
			/*
			 * Do nothing and wait for an  interface  to  be  assigned  to  this
			 * HID device. The state will be changed by the hid_assign() method.
			 */
			break;

		case HID_STATE_INIT:
			/* Initialization: request HID descriptor (report?). */
			break;

		default:
			pdev->state = HID_STATE_DISCONNECTED;
	}

	return 0;
}

int hid_open_dev( const char* str_num, int flags, hid_protocol_t protocol )
{
	long       temp;
	char*      pchar;
	uint8_t    target_num;
	uint8_t    current_num;
	uint8_t    index;
	hid_dev_t* pdev;

	usb_assert(str_num != NULL);

	temp = strtol(str_num, &pchar, 10);
	if (str_num == pchar || pchar > str_num+2 || *pchar != '\0')
		return -1; /** @TODO: use an actual error code. */
	if (temp > HID_MAX_DEVICES)
		return -1; /** @TODO: use an actual error code. */
	target_num = (uint8_t) temp;

	current_num = 0;
	for (index = 0; index < HID_MAX_DEVICES; ++index)
	{
		pdev = &hid_stack.devices[index];
		if (!(pdev->status & HID_STATUS_FREE) && (pdev->protocol == protocol))
			if (current_num++ == target_num)
				break;
	}
	if (index >= HID_MAX_DEVICES)
		return -1; /** @TODO: use an actual error code. */

	if (pdev->status & HID_STATUS_OPEN)
		return -1; /** @TODO: use an actual error code. */
	if (pdev->status & HID_STATUS_INIT)
		return -1; /** @TODO: use an actual error code. */

	pdev->status |= HID_STATUS_OPEN;
	return index;
}


/* POSIX */

int hid_open(const char *pathname, int flags)
{
	/** @TODO
	 * Choose what pahtnames will represent HID devices, for now:
	 * 'pathname' is expected to be either "/mouseXX" or "/keybXX".
	 */
	if (pathname == NULL)
		return -1; /** @TODO: use an actual error code. */

	/*
	 * Everything below this point is just temporary [and not very well done] to
	 * test the HID stack.
	 */

	if (!strcmp(pathname, "/mouse")) /* Open mouse XX */
		return hid_open_dev(pathname+6, flags, HID_PROTO_MOUSE);
	else if (!strcmp(pathname, "/keyb")) /* Open keyboard XX */
		return hid_open_dev(pathname+5, flags, HID_PROTO_KEYB);

	return -1; /** @TODO: use an actual error code. */
}

int hid_close(int fd)
{
	uint8_t index;
	if (fd < 0 || fd > HID_MAX_DEVICES)
		return -1; /** @TODO: use an actual error code. */
	index = (uint8_t) fd;
	if (hid_stack.devices[index].status & HID_STATUS_FREE)
		return -1; /** @TODO: use an actual error code. */
	if (!(hid_stack.devices[index].status & HID_STATUS_OPEN))
		return -1; /** @TODO: use an actual error code. */
	hid_stack.devices[index].status &= ~HID_STATUS_OPEN;
	return 0;
}

size_t hid_read(int fd, void *buf, size_t count)
{
	if (buf == NULL)
		return -1;
	if (count == 0)
		return 0; /* Check this! */
}

int hid_write(int fd, const void *buf, size_t count)
{
	if (buf == NULL)
		return -1;
	if (count == 0)
		return 0; /* Check this! */
}


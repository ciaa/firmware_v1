#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "usb.h"
#include "usbd.h"
#include "usb_desc.h"

#include "drivers/usb_hid.h"


static hid_stack_t hid_stack; /* Maybe this shouldn't be static... */


int hid_probe( const uint8_t* buffer, uint8_t length )
{
	usb_desc_iface_t* piface_desc;
	usb_desc_ep_t*    pep_desc;
	int               status;

	usb_assert(buffer != NULL);
	usb_assert(length >= USB_STDDESC_IFACE_SIZE);
	piface_desc = (usb_desc_iface_t*)buffer;

	if (piface_desc->bInterfaceClass != USB_CLASS_HID)
		return USB_STATUS_DRIVER_FAIL;

/**
 * @FIXME an HID device can actually have  a  second  (optional)  interrupt  OUT
 * pipe to use by the host instead of the default control one... fix the  driver
 * to include this. This will also affect the initialization routine because the
 * report requesting will be done through this new pipe.
 */

	/*
	 * HID interfaces must have one other interrupt IN endpoint, mandatory, and
	 * a second interrupt OUT one, optional.
	 */
	if (piface_desc->bNumEndpoints > 2)
		return USB_STATUS_DRIVER_FAIL;

	/* First, parse the mandatory interrupt IN, it must come first?. */
	status = usb_goto_next_desc(
			&buffer,
			&length,
			USB_STDDESC_ENDPOINT,
			USB_STDDESC_EP_SIZE );
	if (status)
		usb_assert(0); /** @TODO: handle error */
	usb_assert(length >= USB_STDDESC_EP_SIZE);
	pep_desc = (usb_desc_ep_t*) buffer;
	/* Direction must be IN ... */
	if (!(pep_desc->bEndpointAddress & USB_DIR_MASK))
		return USB_STATUS_DRIVER_FAIL;
	/* ... and type Interrupt. */
	if ((pep_desc->bmAttributes & USB_STDDESC_EP_ATTR_TYPE_MASK) != USB_INT)
		return USB_STATUS_DRIVER_FAIL;

	/* If it has the additional endpoint, it must be an interrupt OUT one. */
	if (piface_desc->bNumEndpoints == 2)
	{
		status = usb_goto_next_desc(
				&buffer,
				&length,
				USB_STDDESC_ENDPOINT,
				USB_STDDESC_EP_SIZE );
		if (status)
			usb_assert(0); /** @TODO: handle error */
		usb_assert(length >= USB_STDDESC_EP_SIZE);
		pep_desc = (usb_desc_ep_t*) buffer;
		/* Direction must be OUT ... */
		if (pep_desc->bEndpointAddress & USB_DIR_MASK)
			return USB_STATUS_DRIVER_FAIL;
		/* ... and type Interrupt. */
		if ((pep_desc->bmAttributes & USB_STDDESC_EP_ATTR_TYPE_MASK) != USB_INT)
			return USB_STATUS_DRIVER_FAIL;
	}

	return USB_STATUS_OK;
}

int hid_assign(
		usb_stack_t*   pstack,
		uint16_t       id,
		const uint8_t* buffer,
		uint8_t        length
)
{
	int               new_idx;
	hid_dev_t*        pdev;
	usb_desc_iface_t* piface_desc;
	hid_desc_hid_t*   phid_desc;

	usb_assert(pstack != NULL);
	usb_assert(buffer != NULL);
	usb_assert(length > 0);

	new_idx = hid_get_free_dev();
	if (new_idx < 0)
		return -1; /* No more room for HID devices... */ /** @TODO: use an actual error code. */

	pdev = &hid_stack.devices[new_idx];
	piface_desc = (usb_desc_iface_t*) buffer;

	/* 1) Check the buffer for the interface's descriptor length. */
	if (piface_desc->bLength != USB_STDDESC_IFACE_SIZE)
		return -1;
	if (length < USB_STDDESC_IFACE_SIZE)
		return -1;

	/* 2) Get the protocol information (we already know it's an HID dev. */
	pdev->protocol = piface_desc->bInterfaceProtocol;

	/* 3) Find out whether it uses an interrupt OUT endpoint for control. */
	if (piface_desc->bNumEndpoints == 2)
		pdev->protocol |= HID_STATUS_INTOUT;
	else
		pdev->protocol &= ~HID_STATUS_INTOUT;

	/* 4) Right after the interface descriptor should be the HID one. */
	buffer += piface_desc->bLength;
	length -= piface_desc->bLength;
	phid_desc = (hid_desc_hid_t*) buffer;
	if (phid_desc->bLength < HID_DESC_HID_SIZE) /* Can have optional info. */
		return -1;
	if (length < phid_desc->bLength)
		return -1;

	/* 5) Check the number of descriptors entry, must be at least 1. */
	if (phid_desc->bNumDescriptors < 1)
		return -1;
	/* 6) Check class descriptor and get country code. */
	if (phid_desc->bClassDescriptorType != HID_DESC_TYPE_REPORT)
		return -1;
	pdev->ctry_code = phid_desc->bCountryCode;
	/* 7) Get report's length, only supported up to 256 bytes long ATM. */
	pdev->report_len = phid_desc->wDescriptorLength;

	/**
	 * @TODO there are optional class descriptors  after  the  mandatory  report
	 *       one, this should be read and dealt with... fix this!
	 */

	/*
	 * After the HID desc, there should be one [or  two]  endpoint  descriptors,
	 * however, this has already been checked by the probing method.
	 *
	 * Done with parsing, activate device and exit.
	 */

	pdev->status  &= ~HID_STATUS_FREE;
	pdev->pstack   =  pstack;
	pdev->id       =  id;
	hid_stack.n_devices++;

	return 0; /** @TODO: use an actual error code. */
}

int hid_remove( usb_stack_t* pstack, uint16_t id )
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
	pdev->status     = HID_STATUS_FREE;
	pdev->pstack     = NULL;
	pdev->id         = 0xFFFF;
	pdev->protocol   = HID_PROTO_NONE;
	pdev->state      = HID_STATE_IDLE;
	pdev->status    |= HID_STATUS_ENTRY;
	pdev->report_len = 0;

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

	return USB_STATUS_OK;
}

int hid_update_device( uint8_t index )
{
	usb_stdreq_t stdreq;
	hid_dev_t*   pdev;
	int          status = USB_STATUS_OK;

	usb_assert(index < HID_MAX_DEVICES);
	pdev = &hid_stack.devices[index];

	switch (pdev->state)
	{
		case HID_STATE_IDLE:
			/*
			 * Do nothing and wait for an  interface  to  be  assigned  to  this
			 * HID device. The state will be changed by the hid_assign() method.
			 */
			if (!(pdev->status & HID_STATUS_FREE))
			{
				pdev->state   = HID_STATE_SET_IDLE;
				pdev->status |= HID_STATUS_ENTRY;
			}
			break;

		case HID_STATE_SET_IDLE:
			/* Set IDLE request. */
			if (pdev->status & HID_STATUS_ENTRY)
			{
				stdreq.bmRequestType = USB_STDREQ_REQTYPE(
						USB_DIR_OUT,
						USB_STDREQ_TYPE_CLASS,
						USB_STDREQ_RECIP_INTERFACE );
				stdreq.bRequest      = HID_REQ_SET_IDLE;
				stdreq.wValue        = 0;
				stdreq.wIndex        = USB_ID_TO_IFACE(pdev->id);
				stdreq.wLength       = 0;

				status = usb_ctrlirp(
						pdev->pstack,
						pdev->id,
						&stdreq,
						NULL
				);
				if (status == USB_STATUS_BUSY)
					break; /* Try again later... */
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */
				pdev->status &= ~HID_STATUS_ENTRY;
				break;
			}
			else
			{
				/* Waiting for Acknowledge... */
				status = usb_irp_status(
						pdev->pstack,
						pdev->id,
						USB_CTRL_PIPE_TOKEN
				);
				if (status == USB_STATUS_XFER_WAIT)
					break;
				if (status == USB_STATUS_EP_STALLED)
				{
					/* If endpoint was stalled, try again. */
					pdev->status |= HID_STATUS_ENTRY;
					break;
				}
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */

				pdev->state   = HID_STATE_INIT;
				pdev->status |= HID_STATUS_ENTRY;
				break;
			}
			break;

		case HID_STATE_INIT:
			/* Initialization: request HID Report descriptor. */
			if (pdev->status & HID_STATUS_ENTRY)
			{
				stdreq.bmRequestType = USB_STDREQ_REQTYPE(
						USB_DIR_IN,
						USB_STDREQ_TYPE_STD,
						USB_STDREQ_RECIP_INTERFACE );
				stdreq.bRequest      = USB_STDREQ_GET_DESCRIPTOR;
				stdreq.wValue        = (HID_DESC_TYPE_REPORT << 8);
				stdreq.wIndex        = USB_ID_TO_IFACE(pdev->id);
				stdreq.wLength       = pdev->report_len;

				status = usb_ctrlirp(
						pdev->pstack,
						pdev->id,
						&stdreq,
						pdev->report
				);
				if (status == USB_STATUS_BUSY)
					break; /* Try again later... */
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */
				pdev->status &= ~HID_STATUS_ENTRY;
				break;
			}
			else
			{
				/* Waiting for Acknowledge... */
				status = usb_irp_status(
						pdev->pstack,
						pdev->id,
						USB_CTRL_PIPE_TOKEN
				);
				if (status == USB_STATUS_XFER_WAIT)
					break;
				if (status == USB_STATUS_EP_STALLED)
				{
					/* If endpoint was stalled, try again. */
					pdev->status |= HID_STATUS_ENTRY;
					break;
				}
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */

				/* Report received. */
				/** @TODO Do something with the report ... */
				//pdev->state   = HID_STATE_INIT2;
				pdev->status |= HID_STATUS_INIT;
				pdev->state   = HID_STATE_RUNNING;
				pdev->status |= HID_STATUS_ENTRY;
				break;
			}
			break;

		case HID_STATE_INIT2:
			if (pdev->status & HID_STATUS_ENTRY)
			{
				/* Initialization: request HID Report descriptor. */
				stdreq.bmRequestType = USB_STDREQ_REQTYPE(
						USB_DIR_IN,
						USB_STDREQ_TYPE_CLASS,
						USB_STDREQ_RECIP_INTERFACE );
				stdreq.bRequest      = HID_REQ_GET_IDLE;
				stdreq.wValue        = 0;
				stdreq.wIndex        = USB_ID_TO_IFACE(pdev->id);
				stdreq.wLength       = 1;

				status = usb_ctrlirp(
						pdev->pstack,
						pdev->id,
						&stdreq,
						pdev->report
				);
				if (status == USB_STATUS_BUSY)
					break; /* Try again later... */
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */
				pdev->status &= ~HID_STATUS_ENTRY;
				break;
			}
			else
			{
				/* Waiting for Acknowledge... */
				status = usb_irp_status(
						pdev->pstack,
						pdev->id,
						USB_CTRL_PIPE_TOKEN
				);
				if (status == USB_STATUS_XFER_WAIT)
					break;
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */

				/* Report received. */
				/** @TODO Do something with the report ... */
				pdev->status |= HID_STATUS_INIT;
				pdev->state   = HID_STATE_RUNNING;
				pdev->status |= HID_STATUS_ENTRY;
				break;
			}
			break;

		case HID_STATE_RUNNING:
			break;

		default:
			pdev->state   = HID_STATE_IDLE;
			pdev->status |= HID_STATUS_ENTRY;
			status = USB_STATUS_OK;
	}

	return status;
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


//uint8_t i = 0;
//if ((hid_stack.devices[i].state == HID_STATE_SET_IDLE && !(hid_stack.devices[i].status & HID_STATUS_ENTRY)) ||
//		(hid_stack.devices[i].state == HID_STATE_INIT && !(hid_stack.devices[i].status & HID_STATUS_ENTRY)))
//{
///****/
//int status;
//do
//{
//	status = usbhci_xfer_status(&hid_stack.devices[i].pstack->devices[USB_ID_TO_DEV(hid_stack.devices[i].id)].default_ep);
//} while (status == USB_STATUS_XFER_WAIT);
//if (status == USB_STATUS_EP_STALLED)
//	while (1);
///****/
//}

	if (pdev->status & HID_STATUS_OPEN)
		return -1; /** @TODO: use an actual error code. */
	if (!(pdev->status & HID_STATUS_INIT))
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

	if (!strncmp(pathname, "/mouse", 6)) /* Open mouse XX */
		return hid_open_dev(pathname+6, flags, HID_PROTO_MOUSE);
	else if (!strncmp(pathname, "/keyb", 5)) /* Open keyboard XX */
		return hid_open_dev(pathname+5, flags, HID_PROTO_KEYB);

	return -1; /** @TODO: use an actual error code. */
}

int hid_close(int fd)
{
	uint8_t index;
	if (fd < 0 || fd > HID_MAX_DEVICES-1)
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
	hid_dev_t* pdev;
	int        status;

	if (buf == NULL)
		return -1;
	if (fd < 0 || fd > HID_MAX_DEVICES-1)
		return -1;
	if (count == 0)
		return 0; /* Check this! */

	pdev = &hid_stack.devices[fd];
	if (usb_irp(pdev->pstack, pdev->id, 0, (uint8_t*) buf, count))
		return -1;

	/* Wait for transfer to complete. */
	do
	{
		status = usb_irp_status(pdev->pstack, pdev->id, 0);
	} while (status == USB_STATUS_XFER_WAIT);

	if (status != USB_STATUS_OK)
		return -1;
	return 0;
}

int hid_write(int fd, const void *buf, size_t count)
{
	if (buf == NULL)
		return -1;
	if (fd < 0 || fd > HID_MAX_DEVICES-1)
		return -1;
	if (count == 0)
		return 0; /* Check this! */
	return 0;
}


#include <stdint.h>

#include "usb.h"
#include "usbhci.h"
#include "usb_std.h"
#include "usb_desc.h"
#include "usbd.h"

#include "drivers/usb_drivers.h"


int usb_init( usb_stack_t* pstack )
{
	uint8_t i;
	usb_assert(pstack != NULL);

	usbhci_init();

	pstack->state     = USB_HOST_STATE_IDLE;
	pstack->n_devices = 0;
	pstack->status    = 0;
	pstack->ticks     = 0;

	for (i = 0; i < USB_MAX_DEVICES; ++i)
		usb_device_init(pstack, i);

	return USB_STATUS_OK;
}

int usb_deinit( usb_stack_t* pstack )
{
	uint8_t i;
	usb_assert(pstack != NULL);

	usbhci_deinit();

	pstack->state  = USB_HOST_STATE_IDLE;
	pstack->status = 0;

	for (i = 0; i < USB_MAX_DEVICES; ++i)
		if (usb_device_is_active(pstack, i))
			usb_device_release(pstack, i);

	return USB_STATUS_OK;
}

int16_t usb_systick_inc( usb_stack_t* pstack )
{
	usb_assert(pstack != NULL);
	return pstack->ticks++;
}

int usb_run( usb_stack_t* pstack )
{
	int     status;
	int     addr;  /* Need whole range for return codes. */

	usb_assert(pstack != NULL);

	switch (pstack->state)
	{
		case USB_HOST_STATE_IDLE:
			/*
			 * Do nothing until a device is connected to the  USB  hardware,  it
			 * could be either a regular device or a HUB, the root HUB  in  this
			 * case.
			 */
			if (usbhci_is_connected())
			{
				/*
				 * This new address must always be 0 for the first device and it
				 * can never fail, thus, an assert here is OK.
				 */
				addr = usb_stack_new_addr(pstack);
				usb_assert(addr == 0);

				/* On connection, change root device's state to attached ... */
				usb_device_attach(
						pstack,
#if (USB_MAX_HUBS > 0)
						USB_DEV_PARENT_ROOT,
						USB_DEV_PARENT_ROOT,
#endif
						&pstack->devices[addr] );
				/* ... and start running the stack. */
				pstack->state = USB_HOST_STATE_RUNNING;
			}
			break;

		case USB_HOST_STATE_RUNNING:
			if (pstack->n_devices == 0)
			{
				/* If there are no devices connected, go back to _IDLE. */
				pstack->state = USB_HOST_STATE_IDLE;
				break;
			}
			/* Also check for root HUB/device disconnection. */
			if (!usbhci_is_connected())
			{
				/* Device is no longer connected, release everything. */
				status = usb_device_release(pstack, 0);
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */
				pstack->state = USB_HOST_STATE_IDLE;
				break;
			}
			/*
			 * Otherwise, loop through  all  active  devices,  update  them  and
			 * handle HUBs (HUBs are the only devices owned by the stack).
			 */
			status = usb_stack_update_devices(pstack);
			if (status != USB_STATUS_OK)
				usb_assert(0); /** @TODO: handle error */
#if (USB_MAX_HUBS > 0)
			status = usb_stack_handle_hubs(pstack);
			if (status != USB_STATUS_OK)
				usb_assert(0); /** @TODO: handle error */
#endif /* USB_MAX_HUBS > 0 */

			break;

		case USB_HOST_STATE_SUSPENDED:
			break;

		default:
			pstack->state = USB_HOST_STATE_IDLE;
	}

	return USB_STATUS_OK;
}


/******************************************************************************/

int usb_irp(
		usb_stack_t* pstack,
		uint16_t     device_id,
		uint8_t      pipe,
		uint8_t*     buffer,
		uint32_t     len
)
{
	/* Validate parameters, no asserts 'cause it will be used by user drivers.*/
	if (pstack == NULL || buffer == NULL)
		return USB_STATUS_INV_PARAM;
	if (USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES)
		return USB_STATUS_INV_PARAM;
	if (USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES)
		return USB_STATUS_INV_PARAM;
	if (pipe > USB_MAX_ENDPOINTS || len == 0)
		return USB_STATUS_INV_PARAM;
}

int usb_ctrlirp(
		usb_stack_t*        pstack,
		uint16_t            device_id,
		const usb_stdreq_t* pstdreq,
		uint8_t*            buffer
)
{
	/* Validate parameters, no asserts 'cause it will be used by user drivers.*/
	if (pstack == NULL || pstdreq == NULL)
		return USB_STATUS_INV_PARAM;
	if (USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES)
		return USB_STATUS_INV_PARAM;
	if (USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES)
		return USB_STATUS_INV_PARAM;

	/*
	 * Check whether the  control  endpoint  is  available.  This  is  necessary
	 * because a device has only ONE control endpoint but all of its  interfaces
	 * share it.
	 * Keep in mind each interface will most likely be handled  by  a  different
	 * driver and each of them will probably be running concurrently.
	 */
	if (usb_device_lock(pstack, USB_ID_TO_DEV(device_id)) != USB_STATUS_OK)
		return USB_STATUS_BUSY;

	/* Device available, start control transaction. */
	pdevice = &pstack->devices[USB_ID_TO_DEV(device_id)];
	return usbhci_ctrlxfer_start(&pdevice->default_ep, pstdreq, buffer);
}

int usb_irp_status( usb_stack_t* pstack, uint16_t device_id, uint8_t pipe )
{
	/* Validate parameters, no asserts 'cause it will be used by user drivers.*/
	if (pstack == NULL)
		return USB_STATUS_INV_PARAM;
	if (USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES)
		return USB_STATUS_INV_PARAM;
	if (USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES)
		return USB_STATUS_INV_PARAM;
	if (pipe > USB_MAX_ENDPOINTS)
		return USB_STATUS_INV_PARAM;

	pdevice = &pstack->devices[USB_ID_TO_DEV(device_id)];
	return usbhci_xfer_status( usb_pipe_t* ppipe );
}

int usb_irp_cancel( usb_stack_t* pstack, uint16_t device_id, uint8_t pipe )
{
}


/******************************************************************************/

int usb_device_init( usb_stack_t* pstack, uint8_t index )
{
	usb_device_t* pdevice;

	usb_assert(pstack != NULL);
	usb_assert(index < USB_MAX_DEVICES);

	pdevice = &pstack->devices[index];
	pdevice->state         = USB_DEV_STATE_DISCONNECTED;
	pdevice->next_state    = USB_DEV_STATE_DISCONNECTED;
	pdevice->status        = 0;
    pdevice->speed         = USB_SPD_INV;
	pdevice->addr          = USB_DEV_DEFAULT_ADDR;
	pdevice->n_interfaces  = 0;
	pdevice->vendor_ID     = 0;
	pdevice->product_ID    = 0;
	pdevice->ticks_delay   = pstack->ticks;
#if (USB_MAX_HUBS > 0)
	pdevice->parent_hub    = 0;
	pdevice->parent_port   = 0;
#endif

	return USB_STATUS_OK;
}

int usb_device_release( usb_stack_t* pstack, uint8_t index )
{
	uint8_t          i, j;
	uint8_t          n_ep;
	usb_device_t*    pdevice;
	usb_interface_t* piface;

	usb_assert(pstack != NULL);
	usb_assert(index < USB_MAX_DEVICES);

	pdevice = &pstack->devices[index];

	/**
	 * @TODO: write this function!
	 *
	 * Remember to release downstream devices if this is a HUB!
	 * Don't forget to skip i=0 'cause that's the root HUB/device, it should pop
	 * up with the assertion in anycase though.
	 *
	 * Something like:
	 *
#if (USB_MAX_HUBS > 0)
	 usb_device_t dev_i;

	for (i = 1; i < pstack.n_devices; ++i)
	{
		dev_i = pstack.devices[i];
		usb_assert(dev_i.parent_hub < USB_MAX_DEVICES);
		if (&pstack.devices[dev_i.parent_hub] == pdevice)
			usb_device_release(dev_i);
	}
#endif
	 */

	/** @TODO: error check the following instructions. */
	for (i = 0; i < USB_MAX_INTERFACES; ++i)
	{
		piface = &pdevice->interfaces[i];
		if (piface->driver_handle != USB_IFACE_NO_DRIVER)
		{
			usb_drivers_remove(
					pstack,
					USB_TO_ID(index, i),
					piface->driver_handle );
			piface->driver_handle = USB_IFACE_NO_DRIVER;
		}
		n_ep = piface->n_endpoints;
		for (j = 0; j < n_ep; ++j)
			usbhci_pipe_dealloc(&piface->endpoints[j]);
		piface->n_endpoints = 0;
		piface->class       = 0;
		piface->subclass    = 0;
		piface->protocol    = 0;
		pdevice->n_interfaces--;
	}
	usb_assert(pdevice->n_interfaces == 0);

	usb_device_init(pstack, index);

	pstack->n_devices--;
	return USB_STATUS_OK;
}

int usb_device_is_active( usb_stack_t* pstack, uint8_t index )
{
	usb_assert(pstack != NULL);
	usb_assert(index < USB_MAX_DEVICES);
	if (pstack->devices[index].status & USB_DEV_STATUS_ACTIVE)
		return 1;
	return 0;
}

int usb_device_lock( usb_stack_t* pstack, uint8_t index )
{
	usb_assert(pstack != NULL);
	usb_assert(index < USB_MAX_DEVICES);
/** @TODO: take into consideration multi-threaded systems here, use some kind of semaphore or something... */
	if (pstack->devices[index].status & USB_DEV_STATUS_LOCKED)
		return USB_STATUS_BUSY;
	pstack->devices[index].status |= USB_DEV_STATUS_LOCKED;
	return USB_STATUS_OK;
}

void usb_device_attach(
		usb_stack_t*  pstack,
#if (USB_MAX_HUBS > 0)
		uint8_t       parent_hub,
		uint8_t       parent_port
#endif
		usb_device_t* pdevice
)
{
	usb_assert(pstack  != NULL);
	usb_assert(pdevice != NULL);

	pstack->n_devices   += 1;
	pdevice->state       = USB_DEV_STATE_ATTACHED;
	pdevice->status     |= USB_DEV_STATUS_ACTIVE;
#if (USB_MAX_HUBS > 0)
	pdevice->parent_hub  = parent_hub;
	pdevice->parent_port = parent_port;
#endif
}

#if (USB_MAX_HUBS > 0)
int usb_device_find(
		usb_stack_t* pstack,
		uint8_t      hub_index,
		uint8_t      port
)
{
	uint8_t       i;
	uint8_t       looped_devs = 0;
	usb_device_t* pdevice;

	usb_assert(pstack != NULL);
	usb_assert(hub_index < USB_MAX_HUBS);
	usb_assert(port < USB_MAX_HUB_PORTS);

	for (i = 0; i < USB_MAX_DEVICES; ++i)
	{
		pdevice = &pstack->devices[i];
		if (usb_device_is_active(pstack, i))
		{
			++looped_devs;
			if (pdevice->parent_hub == hub_index && pdevice->parent_port == port)
				return i; /* Found! */
		}
		if (looped_devs >= pstack->n_devices)
			break; /* Done with active devices. */
	}
	/* Not found. */
	return -1;
}
#endif


/******************************************************************************/
int usb_device_udpate( usb_stack_t* pstack, uint8_t index )
{
	usb_device_t* pdevice;
#if (USB_MAX_HUBS > 0)
/** FIXME: when requesting a reset through a HUB I should wait for ACK on those requests. */
	usb_hub_t*    phub;
#endif
	usb_pipe_t*   ppipe;
	usb_stdreq_t* pstdreq;
	int           status;

	usb_assert(pstack != NULL);
	usb_assert(index < USB_MAX_DEVICES);
	pdevice = &pstack->devices[index];
#if (USB_MAX_HUBS > 0)
	usb_assert(pdevice->parent_hub  < USB_MAX_HUBS);
	usb_assert(pdevice->parent_port < USB_MAX_HUB_PORTS);
#endif
	ppipe = &pdevice->default_ep;

	switch (pdevice->state)
	{
		case USB_DEV_STATE_WAITING_ACK:
			status = usbhci_xfer_status(ppipe);
			if (status == USB_STATUS_XFER_WAIT)
				break;
			if (status != USB_STATUS_XFER_DONE)
				usb_assert(0); /** @TODO: handle error */
			pdevice->state = pdevice->next_state;
			break;

		case USB_DEV_STATE_WAITING_DELAY:
			if (pstack->ticks == pdevice->ticks_delay)
				pdevice->state = pdevice->next_state;
			break;

		case USB_DEV_STATE_DISCONNECTED:
			/* Waiting for connection, updated via usb_device_attach(). */
			break;

		case USB_DEV_STATE_ATTACHED:
			/* Device attached, wait for powerline to settle. */
			pdevice->ticks_delay = pstack->ticks + 100;
			pdevice->state = USB_DEV_STATE_WAITING_DELAY;
			pdevice->next_state = USB_DEV_STATE_POWERED;
			break;

		case USB_DEV_STATE_POWERED:
			/*
			 * Drive USB reset for 10~20 ms, but only if the host  is  currently
			 * not resetting or communicating to another device on address 0.
			 * If device's index is 0, then it's the root HUB or device,  it  is
			 * accessed directly through the hardware.  Otherwise,  we  need  to
			 * find to which port in which HUB it is connected to  in  order  to
			 * reset it.
			 * Also, notify the stack that someone  will  begin  an  enumeration
			 * process, this mean address 0 will be in use by  this  device  for
			 * the time being. Any other devices' reset and  addressing  process
			 * pending will have to wait until this one completes.
			 */
			if (pstack->status & USB_STACK_STATUS_ZERO_ADDR)
				break; /* Wait until address 0 is freed. */
/** @TODO: I need some kind of atomic operation for this, maybe a semaphore */
			pstack->status |= USB_STACK_STATUS_ZERO_ADDR;
#if (USB_MAX_HUBS > 0)
			if (index > 0)
			{
				/* Start HUB port reset. */
				phub = pstack->hubs[pdevice->parent_hub];
				status = usb_hub_begin_reset(phub, pdevice->parent_port);
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */

				/* Hold the USB reset high for 10~20 ms. */
				pdevice->ticks_delay = pstack->ticks + 15;
				pdevice->state = USB_DEV_STATE_WAITING_DELAY;
				pdevice->next_state = USB_DEV_STATE_RESET;
			}
			else
#endif
			{
				/*
				 * Hardware reset, 10~20 ms delay is handled internally.   Since
				 * this is is the root device, it  is  not  a  problem  if  this
				 * method  blocks  internally  because  there  is  nothing  else
				 * connected to the stack.
				 */
				usbhci_reset();
				pdevice->state = USB_DEV_STATE_DEFAULT;
			}
			break;

#if (USB_MAX_HUBS > 0)
		case USB_DEV_STATE_RESET:
			/*
			 * Release reset on USB bus, only for HUBs. For the root device it's
			 * handled in one go in the previous state.
			 */
			phub = pstack->hubs[pdevice->parent_hub];

			/* Stop HUB port reset. */
			status = usb_hub_end_reset(phub, pdevice->parent_port);
			if (status != USB_STATUS_OK)
				usb_assert(0); /** @TODO: handle error */
			pdevice->state = USB_DEV_STATE_DEFAULT;
			break;
#endif

		case USB_DEV_STATE_DEFAULT:
			/*
			 * Get the device's speed and create the 0  address  control  pipes,
			 * then configure its  new  address  and  release  the  zero-address
			 * communication bit so the host can setup other devices.
			 */
#if (USB_MAX_HUBS > 0)
			if (index > 0)
			{
				/* Get speed of connected device from HUB's port. */
				phub = pstack->hubs[pdevice->parent_hub];
				if (status != USB_STATUS_OK)
					usb_assert(0); /** @TODO: handle error */
				pdevice->speed = usb_hub_get_speed(phub, pdevice->parent_port);
			}
			else
#endif
			{
				/* Otherwise, get it directly from the HCI. */
				pdevice->speed = usbhci_get_speed();
			}

			/*
			 * Once the reset and speed reading is done, talking  with  the  HUB
			 * is no longer needed for this device.
			 * Next, allocate the default control pipe and lock it.
			 */
			ppipe->type     = USB_CTRL;
			ppipe->mps      = 8;
			ppipe->dir      = USB_DIR_TOK;
			ppipe->interval = 1;
			status = usbhci_pipe_alloc();
			if (status < 0)
				usb_assert(0); /** @TODO: handle error */
			ppipe->handle = (uint8_t) status;

			/*
			 * Locking the default control pipe should never fail at this point,
			 * this is because the device interfaces have yet to be assigned  to
			 * drivers.
			 */
			status = usb_device_lock(pstack, index);
			usb_assert(status == USB_STATUS_OK);

			status = usbhci_pipe_configure(ppipe, 0, pdevice->speed);
			if (status)
				usb_assert(0); /** @TODO: handle error */

			/* Once the default pipe has been configured, get device's MPS. */
			pstdreq = &pdevice->stdreq;
			pstdreq->bmRequestType = USB_STDREQ_REQTYPE(
					USB_DIR_IN,
					USB_STDREQ_TYPE_STD,
					USB_STDREQ_RECIP_DEV );
			pstdreq->bRequest      = USB_STDREQ_GET_DESCRIPTOR;
			pstdreq->wValue        = USB_STDDESC_DEVICE << 8;
			pstdreq->wIndex        = 0;
			pstdreq->wLength       = 8;

			usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
			pdevice->state = USB_DEV_STATE_WAITING_ACK;
			pdevice->next_state = USB_DEV_STATE_ADDRESS;
			break;

		case USB_DEV_STATE_ADDRESS:
			/* Get MPS and reset device once again. */
			ppipe->mps = ((usb_desc_dev_t*)(pdevice->xfer_buffer))->bMaxPacketSize0;
			usbhci_reset();

			/* Reconfigure default pipes to MPS and set a new address. */
			ppipe->type   = USB_CTRL;
			ppipe->dir    = USB_DIR_TOK;
			status = usbhci_pipe_configure(ppipe, 0, pdevice->speed);
			if (status)
				usb_assert(0); /** @TODO: handle error */

			pstdreq = &pdevice->stdreq;
			pstdreq->bmRequestType = USB_STDREQ_REQTYPE(
					USB_DIR_OUT,
					USB_STDREQ_TYPE_STD,
					USB_STDREQ_RECIP_DEV );
			pstdreq->bRequest      = USB_STDREQ_SET_ADDRESS;
			pstdreq->wValue        = index + 1;
			pstdreq->wIndex        = 0;
			pstdreq->wLength       = 0;

			usbhci_ctrlxfer_start(ppipe, pstdreq, NULL);
			pdevice->state = USB_DEV_STATE_WAITING_ACK;
			pdevice->next_state = USB_DEV_STATE_CONFIGURING_PIPES;
			break;

		case USB_DEV_STATE_CONFIGURING_PIPES:
			/*
			 * New address has been set, reconfigure default  pipes  to  it  and
			 * request the configuration descriptor.
			 * Also, release lock on stack to allow other devices to enumerate.
			 */
			pstack->status &= ~USB_STACK_STATUS_ZERO_ADDR;
			ppipe->dir    = USB_DIR_TOK;
			pdevice->addr = index + 1;
			status = usbhci_pipe_configure(ppipe, pdevice->addr, pdevice->speed);
			if (status)
				usb_assert(0); /** @TODO: handle error */

			pstdreq = &pdevice->stdreq;
			pstdreq->bmRequestType = USB_STDREQ_REQTYPE(
					USB_DIR_IN,
					USB_STDREQ_TYPE_STD,
					USB_STDREQ_RECIP_DEV );
			pstdreq->bRequest      = USB_STDREQ_GET_DESCRIPTOR;
			pstdreq->wValue        = USB_STDDESC_DEVICE << 8;
			pstdreq->wIndex        = 0;
			pstdreq->wLength       = 18;

			usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
			pdevice->state = USB_DEV_STATE_WAITING_ACK;
			pdevice->next_state = USB_DEV_STATE_DEV_DESC;
			break;

		case USB_DEV_STATE_DEV_DESC:
			/* Parse the device descriptor and request the configuration one. */
			pdevice->vendor_ID  = ((usb_desc_dev_t*)pdevice->xfer_buffer)->idVendor;
			pdevice->product_ID = ((usb_desc_dev_t*)pdevice->xfer_buffer)->idProduct;

			pstdreq = &pdevice->stdreq;
			pstdreq->bmRequestType = USB_STDREQ_REQTYPE(
					USB_DIR_IN,
					USB_STDREQ_TYPE_STD,
					USB_STDREQ_RECIP_DEV );
			pstdreq->bRequest      = USB_STDREQ_GET_DESCRIPTOR;
			pstdreq->wValue        = USB_STDDESC_CONFIGURATION << 8;
			pstdreq->wIndex        = 0;
			pstdreq->wLength       = 9;

			usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
			pdevice->state = USB_DEV_STATE_WAITING_ACK;
			pdevice->next_state = USB_DEV_STATE_CFG_DESC_9;
			break;

		case USB_DEV_STATE_CFG_DESC_9:
			/* Get the configuration descriptor length and request it again. */
			if (((usb_desc_cfg_t*)pdevice->xfer_buffer)->wTotalLength > 256)
				usb_assert(0); /** @TODO: handle error */

			pstdreq = &pdevice->stdreq;
			pstdreq->wLength = ((usb_desc_cfg_t*)pdevice->xfer_buffer)->wTotalLength;
			pdevice->xfer_length = pstdreq->wLength;

			usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
			pdevice->state = USB_DEV_STATE_WAITING_ACK;
			pdevice->next_state = USB_DEV_STATE_CFG_DESC;
			break;

		case USB_DEV_STATE_CFG_DESC:
			status = usb_device_parse_cfgdesc(pstack, index);

			pdevice->state = USB_DEV_STATE_CONFIGURED;
			break;

		case USB_DEV_STATE_CONFIGURED:
			status = USB_STATUS_OK;
			break;

		case USB_DEV_STATE_SUSPENDED:
			break;

		default:
			pdevice->state = USB_DEV_STATE_DISCONNECTED;
	}

	return USB_STATUS_OK;
}

int usb_device_parse_cfgdesc( usb_stack_t* pstack, uint8_t index )
{
	usb_device_t*  pdevice;
	const uint8_t* buff;
	uint8_t        len;
	uint8_t        i;
	int            status;

	usb_assert(pstack != NULL);
	usb_assert(index < USB_MAX_DEVICES);
	pdevice = &pstack->devices[index];
	buff = pdevice->xfer_buffer;
	len  = pdevice->xfer_length;
	usb_assert(buff != NULL);
	usb_assert(len > 9); /** ???? **/

	pdevice->n_interfaces = ((usb_desc_cfg_t*)buff)->bNumInterfaces;
	if (pdevice->n_interfaces > USB_MAX_INTERFACES)
		usb_assert(0); /** @TODO: handle error */

	buff += 9;
	len  -= 9;
	for (i = 0; i < pdevice->n_interfaces; ++i)
	{
		status = usb_device_parse_ifacedesc(
				pstack,
				USB_TO_ID(index, i),
				&buff,
				&len );
		if (status)
			usb_assert(0); /** @TODO: handle error */
	}

	return USB_STATUS_OK;
}

int usb_device_parse_ifacedesc(
		usb_stack_t*    pstack,
		uint16_t        id,
		const uint8_t** pbuff,
		uint8_t*        plen
)
{
	usb_interface_t*  piface;
	usb_device_t*     pdevice;
	usb_desc_iface_t* piface_desc;
	uint8_t           ep;
	int               driver_idx;
	int               ret;

	usb_assert(pstack != NULL);
	usb_assert(USB_ID_TO_DEV(id) < USB_MAX_DEVICES);
	usb_assert(USB_ID_TO_IFACE(id) < USB_MAX_INTERFACES);
	usb_assert(pbuff != NULL);
	usb_assert(*pbuff != NULL);
	usb_assert(plen != NULL);

	pdevice     = &pstack->devices[USB_ID_TO_DEV(id)];
	piface      = &pdevice->interfaces[USB_ID_TO_IFACE(id)];
	piface_desc = (usb_desc_iface_t*)(*pbuff);

	if (*plen < USB_STDDESC_IFACE_SIZE)
		return USB_STATUS_INV_PARAM;
	if (piface_desc->bDescriptorType != USB_STDDESC_INTERFACE)
		return USB_STATUS_INV_DESC;

	piface->n_endpoints = piface_desc->bNumEndpoints;
	if (piface->n_endpoints > USB_MAX_ENDPOINTS)
		return USB_STATUS_EP_AVAIL;

	piface->class    = piface_desc->bInterfaceClass;
	piface->subclass = piface_desc->bInterfaceSubClass;
	piface->protocol = piface_desc->bInterfaceProtocol;

	/* Check whether there's a suitable driver for the given interface. */
	driver_idx = usb_drivers_probe(pdevice, *pbuff, *plen);
	if (driver_idx < 0)
	{
		/*
		 * If a driver couldn't be found, skip the rest  of  the  initialization
		 * process. However, the interface will still be listed  and  accessible
		 * through the USB stack, this way the user can get information about it
		 * and find out why it wasn't assigned a driver.
		 */
		driver_idx = USB_IFACE_NO_DRIVER;
		return USB_STATUS_OK;
	}

	/* Assign driver to interface. */
	usb_assert(driver_idx < USB_MAX_DRIVERS);
	piface->driver_handle = (usb_driver_handle_t) driver_idx;

	/*
	 * Allocate  endpoints  but  don't  initialize  them  until  all  are  done,
	 * otherwise, you might start  parsing  descriptors  to  discover  the  last
	 * endpoint could not be allocated.
	 */
	for (ep = 0; ep < piface->n_endpoints; ++ep)
	{
		ret = usbhci_pipe_alloc();
		if (ret < 0)
			usb_assert(0); /** @TODO: handle error */
		usb_assert(ret < USB_MAX_ENDPOINTS);
		piface->endpoints[ep].handle = (uint8_t) ret;
	}

	/* Endpoints pipes allocated, go ahead and parse descriptors. */
	for (ep = 0; ep < piface->n_endpoints; ++ep)
	{
		/* First, advance buffer till next endpoint descriptor. */
		if (usb_goto_next_epdesc(pbuff, plen))
			usb_assert(0); /** @TODO: handle error */

		usb_device_parse_epdesc(&piface->endpoints[ep], *pbuff);
		*pbuff += USB_STDDESC_EP_SIZE;
		*plen  -= USB_STDDESC_EP_SIZE;
	}

	/* Finally, register interface with driver. */
	ret = usb_drivers_assign(pstack, id, driver_idx);
	if (ret)
		usb_assert(0); /** @TODO: handle error */

	/** @FIXME: finish this function! */
//	usb_assert(0);

	return USB_STATUS_OK;
}

int usb_device_parse_epdesc( usb_pipe_t* ppipe, const uint8_t* buffer )
{
	usb_desc_ep_t* pep;

	usb_assert(ppipe  != NULL);
	usb_assert(buffer != NULL);

	pep = (usb_desc_ep_t*)buffer;
	ppipe->number   = pep->bEndpointAddress & 0x7F;
	ppipe->type     = pep->bmAttributes & USB_STDDESC_EP_ATTR_TYPE_MASK;
	ppipe->dir      = (pep->bEndpointAddress & 0x80) ? USB_DIR_IN : USB_DIR_OUT;
	ppipe->mps      = pep->wMaxPacketSize & 0x7FF;
	ppipe->interval = pep->bInterval;

	return USB_STATUS_OK;
}


/******************************************************************************/
int usb_stack_new_addr( usb_stack_t* pstack )
{
	int addr;
	usb_assert(pstack != NULL);
	for (addr = 0; addr < USB_MAX_DEVICES; ++addr)
		if (!usb_device_is_active(pstack, addr))
			return addr;
	return -1;
}

int usb_stack_update_devices( usb_stack_t* pstack )
{
	uint8_t i;
	uint8_t looped_devs = 0;
	int     status;

	usb_assert(pstack != NULL);

	for (i = 0; i < USB_MAX_DEVICES; ++i)
	{
		if (usb_device_is_active(pstack, i))
		{
			++looped_devs;
			status = usb_device_udpate(pstack, i);
			if (status != USB_STATUS_OK)
				usb_assert(0); /** @TODO: handle error */
		}
		if (looped_devs >= pstack->n_devices)
			break; /* Done with active devices. */
	}
	return USB_STATUS_OK;
}

#if (USB_MAX_HUBS > 0)
int usb_stack_handle_hubs( usb_stack_t* pstack )
{
/** @TODO: finish this precedure */
	usb_hub_t*      phub;
	usb_device_t*   pdevice;
	int             addr;  /* Need whole range for return codes. */
	int             index; /* IDEM. */
	uint8_t         i, j;
	uint8_t         port_address;

	usb_assert(pstack != NULL);

	/*
	 * Loop through each HUB, update it and  check  its  status  and  ports  for
	 * [dis]connections and other device-related events.
	 */
	for (i = 0; i < pstack->n_hubs; ++i)
	{
		phub = &pstack->hubs[i];
		usb_hub_update(phub);

		/* Loop through each HUB's ports. */
		for (j = 0; j < phub->n_ports; ++j)
		{
			/* Ports are active when connected to a device. */
			if (usb_hub_is_active(phub, j))
			{
				if (!usb_hub_is_connected(phub, j))
				{
					/* Device was disconnected. */
					index = usb_device_find(pstack, i, j);
					if (index < 0)
						usb_assert(0); /** @TODO: handle error */
					usb_device_release(pstack, (uint8_t) index);
					usb_hub_poweroff(phub, j); /* does this go here? Or are ports supposed to be powered on at startup and not on dev. connection? */
				}
			}
			else  /* Port is not active */
			{
				if (usb_hub_is_connected(phub, j))
				{
					/* Setup new device, will begin on next iteration. */
					addr = usb_stack_new_addr(pstack);
					if (addr <= 0)
						usb_assert(0); /** @TODO: handle error */
					usb_device_attach(pstack, &pstack->devices[addr], i, j);
					usb_hub_poweron(phub, j); /* does this go here? Or are ports supposed to be powered on at startup and not on dev. connection? */
				}
			}
		}
	}
}
#endif


void usb_assert(int condition)
{
	if (!condition)
		while (1);
}


/******************************************************************************/


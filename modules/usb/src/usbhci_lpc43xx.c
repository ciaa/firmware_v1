#include <stdio.h>
#include "usb.h"
#include "usbd.h"
#include "usbhci.h"
#include "HAL/HAL.h"
#include "HCD/HCD.h"
#include "USBMode.h"

#define USB_CORENUM 0  /** FIXME: set this up and store it somewhere in the stack's structure? */


/* Hardware pipes. */
struct _usbhci_pipe_t
{
	uint32_t handle; /* This is actually a Pipe_Handle_T struct */
	uint8_t  in_use;
};
static struct _usbhci_pipe_t _pipe_handle[HCD_MAX_ENDPOINT];
static volatile uint8_t _connected[MAX_USB_CORE];

static usb_speed_t _from_lpc_speed(HCD_USB_SPEED speed);
static HCD_USB_SPEED _to_lpc_speed(usb_speed_t speed);
static HCD_TRANSFER_TYPE _to_lpc_type( usb_xfer_type_t type );
static HCD_TRANSFER_DIR _to_lpc_dir( usb_dir_t dir );


int usbhci_init( void )
{
	uint8_t i;

	Chip_USB0_Init();
	HAL_USBInit(USB_CORENUM, USB_MODE_Host);

	/* Host */
	if (HcdInitDriver(USB_CORENUM) != HCD_STATUS_OK)
	{
		HcdDeInitDriver(USB_CORENUM);
		return USB_STATUS_HCD_INIT;
	}

	HAL_EnableUSBInterrupt(USB_CORENUM);
	HcdRhPortEnable(USB_CORENUM);

	/* Pipes */
	for (i = 0; i < HCD_MAX_ENDPOINT; ++i)
	{
		_pipe_handle[i].handle = 0;
		_pipe_handle[i].in_use = 0;
	}

	/* Core status */
	for (i = 0; i < MAX_USB_CORE; ++i)
		_connected[i] = 0;

	return USB_STATUS_OK;
}

int usbhci_deinit( void )
{
	HcdRhPortDisable(USB_CORENUM);
	HcdDeInitDriver(USB_CORENUM);
	HAL_USBDeInit(USB_CORENUM, 2); /* 2 = Host, 1 = Device */
	return 0; /** @TODO: validate previous return code */
}

void usbhci_reset( void )
{
	HcdRhPortReset(USB_CORENUM);
}

usb_speed_t usbhci_get_speed( void )
{
	HCD_USB_SPEED speed;
	if (HcdGetDeviceSpeed(USB_CORENUM, &speed) != HCD_STATUS_OK)
		return USB_SPD_INV;
	return _from_lpc_speed(speed);
}

int usbhci_is_connected( void )
{
	return _connected[USB_CORENUM];
}

uint32_t usbhci_get_frame_number( void )
{
	return HcdGetFrameNumber(USB_CORENUM);
}

int usbhci_pipe_alloc( void )
{
	uint8_t i;
	for (i = 0; i < HCD_MAX_ENDPOINT; ++i)
	{
		if (_pipe_handle[i].in_use)
			continue;
		_pipe_handle[i].in_use = 1;
		return i;
	}
	return -1;
}

int usbhci_pipe_dealloc( usb_pipe_t* ppipe )
{
	usb_assert(ppipe != NULL);
	usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);

	if (HcdClosePipe(_pipe_handle[ppipe->handle].handle) != HCD_STATUS_OK)
		return USB_STATUS_PIPE_CFG; /** TODO: improve this */

	_pipe_handle[ppipe->handle].in_use = 0;
	ppipe->handle   = (uint8_t) -1;
	ppipe->number   = 0;
	ppipe->type     = USB_CTRL;
	ppipe->dir      = (uint8_t) -1;
	ppipe->mps      = 0;
	ppipe->interval = (uint8_t) -1;

	return USB_STATUS_OK;
}

int usbhci_pipe_configure( usb_pipe_t* ppipe, uint8_t addr, usb_speed_t speed )
{
	usb_assert(ppipe != NULL);
	usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);
	if (ppipe->type == USB_INT || ppipe->type == USB_ISO)
		usb_assert(ppipe->interval == 1); /** TODO: dunno what to do with this, the LPC library doesn't seem to handle other intervals... */

	if (HcdOpenPipe(
			USB_CORENUM,
			addr & USB_ADDR_MASK,
			_to_lpc_speed(speed),
			ppipe->number,
			_to_lpc_type(ppipe->type),
			_to_lpc_dir(ppipe->dir),
			ppipe->mps,
			ppipe->interval,
			1, /* TODO: Mult, for ISO @ HS with more than one transaction per uframe */
			0, /* TODO: see below */
			0, /* TODO: this two don't seem to be supported in the LPC library... should I leave them at 0? */
			&_pipe_handle[ppipe->handle].handle ) != HCD_STATUS_OK)
		return USB_STATUS_PIPE_CFG;
	return USB_STATUS_OK;
}

int usbhci_xfer_start(
	usb_pipe_t* ppipe,
	uint8_t*    buffer,
	uint32_t    length
)
{
	usb_assert(ppipe != NULL);
	usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);
	return 1;
}

int usbhci_ctrlxfer_start(
	usb_pipe_t*         ppipe,
	const usb_stdreq_t* pstdreq,
	uint8_t*            buffer
)
{
	usb_assert(ppipe   != NULL);
	usb_assert(pstdreq != NULL);
	usb_assert(buffer != NULL || (buffer == NULL && pstdreq->wLength == 0));
	usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);

	if (HcdControlTransfer(_pipe_handle[ppipe->handle].handle, (const uint8_t*)pstdreq, buffer))
		return USB_STATUS_XFER_ERR;
	return USB_STATUS_OK;
}

int usbhci_xfer_status( usb_pipe_t* ppipe )
{
	HCD_STATUS status;

	usb_assert(ppipe != NULL);
	usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);

	status = HcdGetPipeStatus(_pipe_handle[ppipe->handle].handle);

	switch (status)
	{
		case HCD_STATUS_OK:
		case HCD_STATUS_TRANSFER_COMPLETED:
			//HcdClosePipe(_pipe_handle[ppipe->handle].handle);
			return USB_STATUS_XFER_DONE;

		case HCD_STATUS_TRANSFER_QUEUED:
			return USB_STATUS_XFER_WAIT;

		case HCD_STATUS_TRANSFER_ERROR: /* check this */
		case HCD_STATUS_TRANSFER_CRC:
		case HCD_STATUS_TRANSFER_BitStuffing:
		case HCD_STATUS_TRANSFER_DataToggleMismatch:
		case HCD_STATUS_TRANSFER_PIDCheckFailure:
		case HCD_STATUS_TRANSFER_UnexpectedPID:
		case HCD_STATUS_TRANSFER_DataOverrun:
		case HCD_STATUS_TRANSFER_DataUnderrun:
		case HCD_STATUS_TRANSFER_CC_Reserved1:
		case HCD_STATUS_TRANSFER_CC_Reserved2:
		case HCD_STATUS_TRANSFER_BufferOverrun:
		case HCD_STATUS_TRANSFER_BufferUnderrun:
		case HCD_STATUS_TRANSFER_NotAccessed:
		case HCD_STATUS_TRANSFER_Reserved:
		default:
			return USB_STATUS_XFER_ERR;

		case HCD_STATUS_TRANSFER_Stall:
			return USB_STATUS_EP_STALL;

		case HCD_STATUS_TRANSFER_DeviceNotResponding:
			return USB_STATUS_DEV_UNREACHABLE;
	};

}

int usbhci_xfer_cancel( usb_pipe_t* ppipe )
{
	usb_assert(ppipe != NULL);
	usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);
	return 1;
}

void _usb_host_on_connection( uint8_t corenum )
{
	usb_assert(corenum < MAX_USB_CORE);
	_connected[corenum] = 1;
}

void _usb_host_on_disconnection( uint8_t corenum )
{
	usb_assert(corenum < MAX_USB_CORE);
	_connected[corenum] = 0;
}


/******************************************************************************/

static usb_speed_t _from_lpc_speed(HCD_USB_SPEED speed)
{
	switch (speed)
	{
		case FULL_SPEED:
			return USB_SPD_FS;
		case LOW_SPEED:
			return USB_SPD_LS;
		case HIGH_SPEED:
			return USB_SPD_HS;
		default:
			return USB_SPD_INV;
	}
}

static HCD_USB_SPEED _to_lpc_speed(usb_speed_t speed)
{
	switch (speed)
	{
		case USB_SPD_HS:
			return HIGH_SPEED;
		case USB_SPD_FS:
			return FULL_SPEED;
		default:
			return LOW_SPEED;
	}
}

static HCD_TRANSFER_TYPE _to_lpc_type( usb_xfer_type_t type )
{
	switch (type)
	{
		case USB_CTRL:
			return CONTROL_TRANSFER;
		case USB_INT:
			return ISOCHRONOUS_TRANSFER;
		case USB_ISO:
			return INTERRUPT_TRANSFER;
		case USB_BULK:
		default:
			return BULK_TRANSFER;
	}
}

static HCD_TRANSFER_DIR _to_lpc_dir( usb_dir_t dir )
{
	switch (dir)
	{
		case USB_DIR_TOK:
			return SETUP_TRANSFER;
		case USB_DIR_OUT:
			return OUT_TRANSFER;
		case USB_DIR_IN:
		default:
			return IN_TRANSFER;
	};
}


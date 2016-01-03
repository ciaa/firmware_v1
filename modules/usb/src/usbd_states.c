#include <stdint.h>
#include <stdio.h>

#include "usb.h"
#include "usbhci.h"
#include "usb_std.h"
#include "usb_desc.h"
#include "usbd.h"
#include "usbd_states.h"

#include "drivers/usb_drivers.h"
#include "drivers/usb_hub.h"


static usbd_state_fn _state_fn[] = {
   _usbd_state_waiting_ack,
   _usbd_state_waiting_delay,
   _usbd_state_disconnected,
   _usbd_state_attached,
   _usbd_state_powered,
   _usbd_state_reset,
   _usbd_state_default,
   _usbd_state_address,
   _usbd_state_configuring_pipes,
   _usbd_state_dev_desc,
   _usbd_state_dev_desc_len9,
   _usbd_state_cfg_desc,
   _usbd_state_unlocking,
   _usbd_state_unlocking2,
   _usbd_state_configured,
   _usbd_state_suspended,
};


int usbd_state_run( usb_stack_t* pstack, uint8_t index )
{
   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   return _state_fn[pstack->devices[index].state](pstack, index);
}


int _usbd_state_waiting_ack( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;

   int status = usbhci_xfer_status(ppipe);
   if (status == USB_STATUS_OK)
   {
      pdevice->state = pdevice->next_state;
   }
   else if (status == USB_STATUS_XFER_WAIT)
   {
      /* Waiting for transfer to complete, do nothing. */
      status = USB_STATUS_OK;
   }
   else
   {
      usb_assert(0); /** @TODO handle error */
   }
   return status;
}

int _usbd_state_waiting_delay( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice = &pstack->devices[index];

   if (((int32_t) pdevice->ticks_delay) - ((int32_t) pstack->ticks) <= 0)
   {
      /* If delay is done, go to next state. */
      pdevice->state = pdevice->next_state;
   }
   return USB_STATUS_OK;
}

int _usbd_state_disconnected( usb_stack_t* pstack, uint8_t index )
{
   /* Waiting for connection, updated via usb_device_attach(). */
   return USB_STATUS_OK;
}

int _usbd_state_attached( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice = &pstack->devices[index];

   /* Device attached, wait for powerline to settle. */
   pdevice->ticks_delay = pstack->ticks + 100;
   pdevice->state = USB_DEV_STATE_WAITING_DELAY;
   pdevice->next_state = USB_DEV_STATE_POWERED;

   return USB_STATUS_OK;
}

int _usbd_state_powered( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice = &pstack->devices[index];
#if (USB_MAX_HUBS > 0)
   /* We're accessing the root device or one with a valid parent HUB index. */
   usb_assert(index == 0 || pdevice->parent_hub  < USB_MAX_HUBS);
   usb_assert(index == 0 || pdevice->parent_port < USB_MAX_HUB_PORTS);
#endif

   /*
    * Drive USB reset for 10~20 ms, but  only  if  the  host  is  currently  not
    * resetting or communicating to another device on address  0.   If  device's
    * index is 0, then it's the root HUB or  device,  it  is  accessed  directly
    * through the hardware.  Otherwise, we need to find to which port  in  which
    * HUB it is connected to in order to reset it.  Also, notify the stack  that
    * someone will begin an enumeration process, this mean address 0 will be  in
    * use by this device for the time  being.   Any  other  devices'  reset  and
    * addressing process pending will have to wait until this one completes.
    */
   if (!(pstack->status & USB_STACK_STATUS_ZERO_ADDR))
   {
      /* Address 0 was free, otherwise we'd have to wait. */
      pstack->status |= USB_STACK_STATUS_ZERO_ADDR;/* Mark it as in-use */
#if 0//(USB_MAX_HUBS > 0)
      /*
       * If index isn't 0, then we're enumerating a device through a  HUB,  this
       * means we have to use HUB commands for reseting and such.
       */
      if (index > 0)
      {
         /* Start HUB port reset. */
         phub = pstack->hubs[pdevice->parent_hub];
         status = usb_hub_begin_reset(phub, pdevice->parent_port);
         if (status != USB_STATUS_OK)
            usb_assert(0); /** @TODO handle error */

         /* Hold the USB reset high for 10~20 ms. */
         pdevice->ticks_delay = pstack->ticks + 15;
         pdevice->state = USB_DEV_STATE_WAITING_DELAY;
         pdevice->next_state = USB_DEV_STATE_RESET;
      }
      else
#endif
      {
         /* Hardware reset, we wait the required 10~20 ms before proceeding. */
         usbhci_reset_start();
         pdevice->ticks_delay = pstack->ticks + 15;
         pdevice->state = USB_DEV_STATE_WAITING_DELAY;
         pdevice->next_state = USB_DEV_STATE_RESET;
      }
   }
   return USB_STATUS_OK;
}

int _usbd_state_reset( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice = &pstack->devices[index];
#if (USB_MAX_HUBS > 0)
   /* We're accessing the root device or one with a valid parent HUB index. */
   usb_assert(index == 0 || pdevice->parent_hub  < USB_MAX_HUBS);
   usb_assert(index == 0 || pdevice->parent_port < USB_MAX_HUB_PORTS);
#endif

#if 0//(USB_MAX_HUBS > 0)
   if (index > 0)
   {
      /* Release reset on USB bus, only for HUBs. */
      phub = pstack->hubs[pdevice->parent_hub];

      /* Stop HUB port reset. */
      status = usb_hub_end_reset(phub, pdevice->parent_port);
      if (status != USB_STATUS_OK)
         usb_assert(0); /** @TODO handle error */
      pdevice->state = USB_DEV_STATE_DEFAULT;
   }
   else
#endif
   {
      if (usbhci_reset_stop() == USB_STATUS_BUSY)
      {
         /* Reset is still in progress, keep waiting. */
         pdevice->ticks_delay = pstack->ticks + 5;
         pdevice->state = USB_DEV_STATE_WAITING_DELAY;
         pdevice->next_state = USB_DEV_STATE_RESET;
      }
      else
      {
         pdevice->state = USB_DEV_STATE_DEFAULT;
      }
   }
   return USB_STATUS_OK;
}

int _usbd_state_default( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdevice =  &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;
   uint8_t*      pstdreq =  pdevice->stdreq;
#if (USB_MAX_HUBS > 0)
   /* We're accessing the root device or one with a valid parent HUB index. */
   usb_assert(index == 0 || pdevice->parent_hub  < USB_MAX_HUBS);
   usb_assert(index == 0 || pdevice->parent_port < USB_MAX_HUB_PORTS);
#endif

   /*
    * Get the device's speed and  create  the  0  address  control  pipes,  then
    * configure its new address and release the zero-address  communication  bit
    * so the host can setup other devices.
    */
#if 0//(USB_MAX_HUBS > 0)
   if (index > 0)
   {
      /* Get speed of connected device from HUB's port. */
      phub = pstack->hubs[pdevice->parent_hub];
      if (status != USB_STATUS_OK)
         usb_assert(0); /** @TODO handle error */
      pdevice->speed = usb_hub_get_speed(phub, pdevice->parent_port);
   }
   else
#endif
   {
      /* Otherwise, get it directly from the HCI. */
      pdevice->speed = usbhci_get_speed();
   }

   /*
    * Once the reset and speed reading is done,  talking  with  the  HUB  is  no
    * longer needed for this device.
    * Next, allocate the default control pipe and lock it.
    */
   ppipe->type     = USB_CTRL;
   ppipe->mps      = 8;
   ppipe->dir      = USB_DIR_TOK;
   ppipe->interval = 1;
   status = usbhci_pipe_alloc(USB_CTRL);
   if (status < 0)
      usb_assert(0); /** @TODO handle error */
   ppipe->handle = (uint8_t) status;

   /*
    * Locking the default control pipe should never fail at this point, this  is
    * because the device interfaces have yet to be assigned to drivers.
    */
   status = usb_device_lock(pstack, index);
   usb_assert(status == USB_STATUS_OK);

   status = usbhci_pipe_configure(ppipe, 0, pdevice->speed);
   if (status)
      usb_assert(0); /** @TODO handle error */

   /* Once the default pipe has been configured, get device's MPS. */
   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_GET_DESCRIPTOR);
   USB_STDREQ_SET_wValue(  pstdreq, USB_STDDESC_DEVICE << 8);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 8);

   usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_ADDRESS;

   status = USB_STATUS_OK;
   return status;
}

int _usbd_state_address( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdevice =  &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;
   uint8_t*      pstdreq =  pdevice->stdreq;

   /* Get MPS and reset device once again. */
   ppipe->mps = USB_STDDESC_DEV_GET_bMaxPacketSize0(pdevice->xfer_buffer);

   /* Reconfigure default pipes to MPS and set a new address. */
   ppipe->type = USB_CTRL;
   ppipe->dir  = USB_DIR_TOK;
   status = usbhci_pipe_configure(ppipe, 0, pdevice->speed);
   if (status)
      usb_assert(0); /** @TODO handle error */

   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_SET_ADDRESS);
   USB_STDREQ_SET_wValue(  pstdreq, index + 1);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 0);

   usbhci_ctrlxfer_start(ppipe, pstdreq, NULL);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_CONFIGURING_PIPES;

   status = USB_STATUS_OK;
   return status;
}

int _usbd_state_configuring_pipes( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdevice =  &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;
   uint8_t*      pstdreq =  pdevice->stdreq;

   /*
    * New address has been set, reconfigure default pipes and request the device
    * descriptor. Also, release lock on stack to allow others to enumerate.
    */
   pstack->status &= ~USB_STACK_STATUS_ZERO_ADDR;
   ppipe->dir      = USB_DIR_TOK;
   pdevice->addr   = index + 1;
   status = usbhci_pipe_configure(ppipe, pdevice->addr, pdevice->speed);
   if (status)
      usb_assert(0); /** @TODO handle error */

   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_GET_DESCRIPTOR);
   USB_STDREQ_SET_wValue(  pstdreq, USB_STDDESC_DEVICE << 8);
   USB_STDREQ_SET_wIndex(  pstdreq,  0);
   USB_STDREQ_SET_wLength( pstdreq, 18);

   usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_DEV_DESC;

   status = USB_STATUS_OK;
   return status;
}

int _usbd_state_dev_desc( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdevice =  &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;
   uint8_t*      pstdreq =  pdevice->stdreq;

   /* Parse the device descriptor and request the configuration one. */
   pdevice->vendor_ID  = USB_STDDESC_DEV_GET_idVendor(pdevice->xfer_buffer);
   pdevice->product_ID = USB_STDDESC_DEV_GET_idProduct(pdevice->xfer_buffer);

   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_GET_DESCRIPTOR);
   USB_STDREQ_SET_wValue(  pstdreq, USB_STDDESC_CONFIGURATION << 8);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 9);

   usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_CFG_DESC_LEN9;

   status = USB_STATUS_OK;
   return status;
}

int _usbd_state_dev_desc_len9( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   uint16_t      aux;
   usb_device_t* pdevice =  &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;
   uint8_t*      pstdreq =  pdevice->stdreq;

   /* Get the configuration descriptor length and request it again. */
   if (USB_STDDESC_CFG_GET_wTotalLength(pdevice->xfer_buffer) > 256)
      usb_assert(0); /** @TODO handle error */

   aux = USB_STDDESC_CFG_GET_wTotalLength(pdevice->xfer_buffer);
   USB_STDREQ_SET_wLength(pstdreq, aux);
   pdevice->xfer_length = aux;

   usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_CFG_DESC;

   status = USB_STATUS_OK;
   return status;
}

int _usbd_state_cfg_desc( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdevice =  &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdevice->default_ep;
   uint8_t*      pstdreq =  pdevice->stdreq;

   /* Parse descriptors and assign each interface a driver. */
   status = usb_device_parse_cfgdesc(pstack, index);
   if (status)
      usb_assert(0); /** @TODO handle error */

   /* Once that's done, set the configuration on the device. */
   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_SET_CONFIGURATION);
   USB_STDREQ_SET_wValue(  pstdreq, pdevice->cfg_value);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 0);

   usbhci_ctrlxfer_start(ppipe, pstdreq, NULL);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_UNLOCKING;

   status = USB_STATUS_OK;
   return status;
}

#if 0 /* This was a test */
case USB_DEV_STATE_TEST:
   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_INTERFACE )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_GET_DESCRIPTOR);
   USB_STDREQ_SET_wValue(  pstdreq, 0x22 << 8);
   USB_STDREQ_SET_wIndex(  pstdreq,  0);
   USB_STDREQ_SET_wLength( pstdreq, 40);

   usbhci_ctrlxfer_start(ppipe, pstdreq, pdevice->xfer_buffer);
   pdevice->state = USB_DEV_STATE_WAITING_ACK;
   pdevice->next_state = USB_DEV_STATE_CONFIGURED;
   break;
#endif

int _usbd_state_unlocking( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice =  &pstack->devices[index];

   /* This is needed to delay the unlocking of the default control endpoint. */
   pdevice->ticks_delay = pstack->ticks + 500;
   pdevice->state = USB_DEV_STATE_WAITING_DELAY;
   pdevice->next_state = USB_DEV_STATE_UNLOCKING2;

   return USB_STATUS_OK;
}

int _usbd_state_unlocking2( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdevice =  &pstack->devices[index];

   /*
    * This state is only needed to unlock the default endpoint once the stack is
    * done with the enumeration process. This cannot fail.
    */
   usb_device_unlock(pstack, index);
   pdevice->state = USB_DEV_STATE_CONFIGURED;

   return USB_STATUS_OK;
}

int _usbd_state_configured( usb_stack_t* pstack, uint8_t index )
{
   /* Nothing to do here... ? */
   return USB_STATUS_OK;
}

int _usbd_state_suspended( usb_stack_t* pstack, uint8_t index )
{
   /* @TODO implement the suspended state */
   return USB_STATUS_OK;
}


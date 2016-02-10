
/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USBD USB Driver
 * @{
 */

/*==================[inclusions]=============================================*/
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


/*==================[internal data declaration]==============================*/

/** @brief USBD state function type. */
typedef int (*_state_fn_t)( usb_stack_t* pstack, uint8_t index );


/*==================[internal functions declaration]=========================*/

/*
 * State functions must follow the usb_dev_state_t enumeration order.
 *
 * Following functions do not assert input parameters, this is because they must
 * not be called directly, rather through usbd_state_run() so there's no need.
 */
int _state_waiting_ack( usb_stack_t* pstack, uint8_t index );
int _state_waiting_delay( usb_stack_t* pstack, uint8_t index );
int _state_disconnected( usb_stack_t* pstack, uint8_t index );
int _state_attached( usb_stack_t* pstack, uint8_t index );
int _state_powered( usb_stack_t* pstack, uint8_t index );
int _state_reset( usb_stack_t* pstack, uint8_t index );
int _state_default( usb_stack_t* pstack, uint8_t index );
int _state_address_reset( usb_stack_t* pstack, uint8_t index );
int _state_address( usb_stack_t* pstack, uint8_t index );
int _state_configuring_pipes( usb_stack_t* pstack, uint8_t index );
int _state_dev_desc( usb_stack_t* pstack, uint8_t index );
int _state_dev_desc_len9( usb_stack_t* pstack, uint8_t index );
int _state_cfg_desc( usb_stack_t* pstack, uint8_t index );
int _state_unlocking( usb_stack_t* pstack, uint8_t index );
int _state_unlocking2( usb_stack_t* pstack, uint8_t index );
int _state_configured( usb_stack_t* pstack, uint8_t index );
int _state_suspended( usb_stack_t* pstack, uint8_t index );


/*==================[internal data definition]===============================*/

static _state_fn_t _state_fn[] =
{
   _state_waiting_ack,
   _state_waiting_delay,
   _state_disconnected,
   _state_attached,
   _state_powered,
   _state_reset,
   _state_default,
   _state_address_reset,
   _state_address,
   _state_configuring_pipes,
   _state_dev_desc,
   _state_dev_desc_len9,
   _state_cfg_desc,
   _state_unlocking,
   _state_unlocking2,
   _state_configured,
   _state_suspended,
};


/*==================[internal functions definition]==========================*/

int _state_waiting_ack( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev  = &pstack->devices[index];
   usb_pipe_t*   ppipe = &pdev->default_ep;

   int status = usbhci_xfer_status(pdev, ppipe);
   if (status == USB_STATUS_OK)
   {
      pdev->state = pdev->next_state;
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

int _state_waiting_delay( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev = &pstack->devices[index];

   if (((int32_t) pdev->ticks_delay) - ((int32_t) pstack->ticks) <= 0)
   {
      /* If delay is done, go to next state. */
      pdev->state = pdev->next_state;
   }
   return USB_STATUS_OK;
}

int _state_disconnected( usb_stack_t* pstack, uint8_t index )
{
   /* Waiting for connection, updated via usb_device_attach(). */
   return USB_STATUS_OK;
}

int _state_attached( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev = &pstack->devices[index];

   /* Device attached, wait for powerline to settle. */
   pdev->ticks_delay = pstack->ticks + 100;
   pdev->state       = USB_DEV_STATE_WAITING_DELAY;
   pdev->next_state  = USB_DEV_STATE_POWERED;

   return USB_STATUS_OK;
}

int _state_powered( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev = &pstack->devices[index];
#if (USB_MAX_HUBS > 0)
   /* We're accessing the root device or one with a valid parent HUB index. */
   usb_assert(index == 0 || pdev->parent_hub  < USB_MAX_HUBS);
   usb_assert(index == 0 || pdev->parent_port < USB_MAX_HUB_PORTS);
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
#if (USB_MAX_HUBS > 0)
      /*
       * If index isn't 0, then we're enumerating a device through a  HUB,  this
       * means we have to use HUB commands for reseting and such.
       */
      if (index > 0)
      {
         /* Start HUB port reset. */
         usb_hub_port_reset_start(pdev->parent_hub, pdev->parent_port);
         /* Reset duration is handled by the HUB, just wait for it to finish. */
      }
      else
#endif
      {
         /* Hardware reset, we wait the required 10~20 ms before proceeding. */
         usbhci_reset_start();
         //pdev->ticks_delay = pstack->ticks + 15;
         //pdev->state       = USB_DEV_STATE_WAITING_DELAY;
         //pdev->next_state  = USB_DEV_STATE_RESET;
      }

      pdev->state      = USB_DEV_STATE_RESET;
      pdev->next_state = USB_DEV_STATE_DEFAULT;
   }
   return USB_STATUS_OK;
}

int _state_reset( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev = &pstack->devices[index];
#if (USB_MAX_HUBS > 0)
   /* We're accessing the root device or one with a valid parent HUB index. */
   usb_assert(index == 0 || pdev->parent_hub  < USB_MAX_HUBS);
   usb_assert(index == 0 || pdev->parent_port < USB_MAX_HUB_PORTS);
#endif

#if (USB_MAX_HUBS > 0)
   if (index > 0)
   {
      /* Here we wait until the HUB releases the bus reset. */
      if (usb_hub_port_reset_status(pdev->parent_hub, pdev->parent_port)
            == USB_STATUS_BUSY)
      {
         pdev->state = USB_DEV_STATE_RESET;
      }
      else
      {
         pdev->state = USB_DEV_STATE_DEFAULT;
         pdev->state = pdev->next_state;
      }
   }
   else
#endif
   {
      if (usbhci_reset_stop() == USB_STATUS_BUSY)
      {
         /* Reset is still in progress, keep waiting. */
         pdev->state = USB_DEV_STATE_RESET;
         //pdev->ticks_delay = pstack->ticks + 5;
         //pdev->state       = USB_DEV_STATE_WAITING_DELAY;
         //pdev->next_state  = USB_DEV_STATE_RESET;
      }
      else
      {
         pdev->state = USB_DEV_STATE_DEFAULT;
         pdev->state = pdev->next_state;
      }
   }
   return USB_STATUS_OK;
}

int _state_default( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdev    = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdev->default_ep;
   uint8_t*      pstdreq =  pdev->stdreq;
#if (USB_MAX_HUBS > 0)
   /* We're accessing the root device or one with a valid parent HUB index. */
   usb_assert(index == 0 || pdev->parent_hub  < USB_MAX_HUBS);
   usb_assert(index == 0 || pdev->parent_port < USB_MAX_HUB_PORTS);
#endif

   /*
    * Get the device's speed and  create  the  0  address  control  pipes,  then
    * configure its new address and release the zero-address  communication  bit
    * so the host can setup other devices.
    */
#if (USB_MAX_HUBS > 0)
   if (index > 0)
   {
      /* Get speed of connected device from HUB's port. */
      pdev->speed = usb_hub_get_speed(pdev->parent_hub, pdev->parent_port);
   }
   else
#endif
   {
      /* Otherwise, get it directly from the HCI. */
      pdev->speed = usbhci_get_speed();
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
   {
      usb_assert(0); /** @TODO handle error */
   }
   ppipe->handle = (uint8_t) status;

   /*
    * Locking the default control pipe should never fail at this point, this  is
    * because the device interfaces have yet to be assigned to drivers.
    */
   status = usb_device_lock(pstack, index);
   usb_assert(status == USB_STATUS_OK);

   pdev->addr = 0;
   status = usbhci_pipe_configure(pdev, ppipe);
   if (status)
   {
      usb_assert(0); /** @TODO handle error */
   }

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

   pdev->default_ep.length = 8;
   pdev->default_ep.buffer = pdev->xfer_buffer;
   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state      = USB_DEV_STATE_WAITING_ACK;
   //pdev->next_state = USB_DEV_STATE_ADDRESS;
   pdev->next_state = USB_DEV_STATE_ADDRESS_RESET;

   status = USB_STATUS_OK;
   return status;
}

int _state_address_reset( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev    = &pstack->devices[index];

   /* Get MPS and reset device once again. */
   pdev->default_ep.mps = USB_STDDESC_DEV_GET_bMaxPacketSize0(pdev->xfer_buffer);

#if (USB_MAX_HUBS > 0)
   if (index > 0)
   {
      /* Start HUB port reset. */
      usb_hub_port_reset_start(pdev->parent_hub, pdev->parent_port);
      /* Reset duration is handled by the HUB, just wait for it to finish. */
   }
   else
#endif
   {
      /* Start hardware reset. */
      usbhci_reset_start();
   }
   pdev->state      = USB_DEV_STATE_RESET;
   pdev->next_state = USB_DEV_STATE_ADDRESS;
   return USB_STATUS_OK;
}

int _state_address( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdev    = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdev->default_ep;
   uint8_t*      pstdreq =  pdev->stdreq;

   /* Reconfigure default pipes to MPS and set a new address. */
   ppipe->type = USB_CTRL;
   ppipe->dir  = USB_DIR_TOK;
   status = usbhci_pipe_configure(pdev, ppipe);
   if (status)
   {
      usb_assert(0); /** @TODO handle error */
   }

   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_SET_ADDRESS);
   USB_STDREQ_SET_wValue(  pstdreq, index + 1);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 0);

   pdev->default_ep.buffer = NULL;
   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state      = USB_DEV_STATE_WAITING_ACK;
   pdev->next_state = USB_DEV_STATE_CONFIGURING_PIPES;

   status = USB_STATUS_OK;
   return status;
}

int _state_configuring_pipes( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdev    = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdev->default_ep;
   uint8_t*      pstdreq =  pdev->stdreq;

   /*
    * New address has been set, reconfigure default pipes and request the device
    * descriptor. Also, release lock on stack to allow others to enumerate.
    */
   pstack->status &= ~USB_STACK_STATUS_ZERO_ADDR;
   ppipe->dir      = USB_DIR_TOK;
   pdev->addr      = index + 1;
   status = usbhci_pipe_configure(pdev, ppipe);
   if (status)
   {
      usb_assert(0); /** @TODO handle error */
   }

   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_GET_DESCRIPTOR);
   USB_STDREQ_SET_wValue(  pstdreq, USB_STDDESC_DEVICE << 8);
   USB_STDREQ_SET_wIndex(  pstdreq,  0);
   USB_STDREQ_SET_wLength( pstdreq, 18);

   pdev->default_ep.buffer = pdev->xfer_buffer;
   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state      = USB_DEV_STATE_WAITING_ACK;
   pdev->next_state = USB_DEV_STATE_DEV_DESC;

   status = USB_STATUS_OK;
   return status;
}

int _state_dev_desc( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdev    = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdev->default_ep;
   uint8_t*      pstdreq =  pdev->stdreq;

   /* Parse the device descriptor and request the configuration one. */
   pdev->vendor_ID  = USB_STDDESC_DEV_GET_idVendor(pdev->xfer_buffer);
   pdev->product_ID = USB_STDDESC_DEV_GET_idProduct(pdev->xfer_buffer);

   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_GET_DESCRIPTOR);
   USB_STDREQ_SET_wValue(  pstdreq, USB_STDDESC_CONFIGURATION << 8);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 9);

   pdev->default_ep.buffer = pdev->xfer_buffer;
   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state      = USB_DEV_STATE_WAITING_ACK;
   pdev->next_state = USB_DEV_STATE_CFG_DESC_LEN9;

   status = USB_STATUS_OK;
   return status;
}

int _state_dev_desc_len9( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   uint16_t      aux;
   usb_device_t* pdev    = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdev->default_ep;
   uint8_t*      pstdreq =  pdev->stdreq;

   /* Get the configuration descriptor length and request it again. */
   if (USB_STDDESC_CFG_GET_wTotalLength(pdev->xfer_buffer) > 256)
   {
      usb_assert(0); /** @TODO handle error */
   }

   aux = USB_STDDESC_CFG_GET_wTotalLength(pdev->xfer_buffer);
   USB_STDREQ_SET_wLength(pstdreq, aux);
   pdev->default_ep.length = aux;
   pdev->default_ep.buffer = pdev->xfer_buffer;

   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state      = USB_DEV_STATE_WAITING_ACK;
   pdev->next_state = USB_DEV_STATE_CFG_DESC;

   status = USB_STATUS_OK;
   return status;
}

int _state_cfg_desc( usb_stack_t* pstack, uint8_t index )
{
   int           status;
   usb_device_t* pdev    = &pstack->devices[index];
   usb_pipe_t*   ppipe   = &pdev->default_ep;
   uint8_t*      pstdreq =  pdev->stdreq;

   /* Parse descriptors and assign each interface a driver. */
   status = usb_device_parse_cfgdesc(pstack, index);
   if (status)
   {
      usb_assert(0); /** @TODO handle error */
   }

   /* Once that's done, set the configuration on the device. */
   USB_STDREQ_SET_bmRequestType( pstdreq, USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_STD,
         USB_STDREQ_RECIP_DEV )
   );
   USB_STDREQ_SET_bRequest(pstdreq, USB_STDREQ_SET_CONFIGURATION);
   USB_STDREQ_SET_wValue(  pstdreq, pdev->cfg_value);
   USB_STDREQ_SET_wIndex(  pstdreq, 0);
   USB_STDREQ_SET_wLength( pstdreq, 0);

   pdev->default_ep.buffer = NULL;
   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state      = USB_DEV_STATE_WAITING_ACK;
   pdev->next_state = USB_DEV_STATE_UNLOCKING;

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

   pdev->default_ep.buffer = NULL;
   usbhci_ctrlxfer_start(pdev, ppipe);
   pdev->state = USB_DEV_STATE_WAITING_ACK;
   pdev->next_state = USB_DEV_STATE_CONFIGURED;
   break;
#endif

int _state_unlocking( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev = &pstack->devices[index];

   /* This is needed to delay the unlocking of the default control endpoint. */
   pdev->ticks_delay = pstack->ticks + 500;
   pdev->state       = USB_DEV_STATE_WAITING_DELAY;
   pdev->next_state  = USB_DEV_STATE_UNLOCKING2;

   return USB_STATUS_OK;
}

int _state_unlocking2( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev = &pstack->devices[index];

   /*
    * This state is only needed to unlock the default endpoint once the stack is
    * done with the enumeration process. This cannot fail.
    */
   usb_device_unlock(pstack, index);
   pdev->state = USB_DEV_STATE_CONFIGURED;

   return USB_STATUS_OK;
}

int _state_configured( usb_stack_t* pstack, uint8_t index )
{
   /* Nothing to do here... ? */
   return USB_STATUS_OK;
}

int _state_suspended( usb_stack_t* pstack, uint8_t index )
{
   /* @TODO implement the suspended state */
   return USB_STATUS_OK;
}


/*==================[external functions definition]==========================*/

int usbd_state_run( usb_stack_t* pstack, uint8_t index )
{
   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   return _state_fn[pstack->devices[index].state](pstack, index);
}

/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


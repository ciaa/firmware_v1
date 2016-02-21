
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
#include "usb_devices_cfg.h"
#include "usbhci.h"
#include "usb_std.h"
#include "usb_desc.h"
#include "usbd.h"
#include "usbd_states.h"

#include "drivers/usb_drivers.h"
#include "drivers/usb_hub.h"


/*==================[internal functions declaration]=========================*/
int16_t _devidx_from_hub_port(
      usb_stack_t* pstack,
      uint8_t hub_idx,
      uint8_t port
);

int _irp_status(
      usb_stack_t* pstack,
      uint16_t     id,
      uint8_t      index,
      uint8_t      is_ctrl
);


/*==================[internal functions definition]==========================*/
int16_t _devidx_from_hub_port(
      usb_stack_t* pstack,
      uint8_t hub_idx,
      uint8_t port
)
{
   int16_t       ret;
   uint8_t       i;
   usb_device_t* pdev;

   usb_assert(pstack!= NULL);
   usb_assert(hub_idx < USB_MAX_HUBS      || hub_idx == USB_DEV_PARENT_ROOT);
   usb_assert(port    < USB_MAX_HUB_PORTS || port    == USB_DEV_PARENT_ROOT);

   /* Search for matching device. */
   for (ret = -1, i = 0; i < USB_MAX_DEVICES && ret == -1; ++i)
   {
      pdev = &pstack->devices[i];
      if (pdev->parent_hub == hub_idx && pdev->parent_port == port)
      {
         ret = i;
      }
   }
   return ret;
}

int _irp_status(
      usb_stack_t* pstack,
      uint16_t     id,
      uint8_t      index,
      uint8_t      is_ctrl
)
{
   int           status;
   usb_device_t* pdev;
   void*         ppipe;
   uint8_t*      pretries;

   pdev  = &pstack->devices[USB_ID_TO_DEV(id)];
   if (is_ctrl)
   {
      ppipe    = &pstack->def_ep[index];
      pretries = &(((usb_msg_pipe_t*) ppipe)->retries);
      status   = usbhci_ctrlxfer_status(pdev, (usb_msg_pipe_t*) ppipe);
   }
   else
   {
      ppipe    = &pdev->interfaces[USB_ID_TO_IFACE(id)].endpoints[index];
      pretries = &(((usb_pipe_t*) ppipe)->retries);
      status   = usbhci_xfer_status(pdev, (usb_pipe_t*) ppipe);
   }

   if (status == USB_STATUS_EP_STALLED)
   {
      /* If the endpoint was stalled, cancel the rest of the transfer. */
      status = (is_ctrl) ? usbhci_ctrlxfer_cancel(pdev, (usb_msg_pipe_t*) ppipe)
                         : usbhci_xfer_cancel(pdev, (usb_pipe_t*) ppipe);
      if (*pretries < USB_MAX_XFER_RETRIES)
      {
         /* And retry if under 3 strikes. */
         status = (is_ctrl) ? usbhci_ctrlxfer_start(pdev,(usb_msg_pipe_t*)ppipe)
                            : usbhci_xfer_start(pdev, (usb_pipe_t*) ppipe);
         if (status != USB_STATUS_OK)
         {
            usb_assert(0); /** @TODO handle error */
         }
         else
         {
            status = USB_STATUS_XFER_WAIT;
         }
      }
      else
      {
         status = USB_STATUS_OK;
      }
   }
   else if (status == USB_STATUS_OK)
   {
      *pretries = 0;
   }
   return status;
}


/*==================[external functions definition]==========================*/

int usb_init( usb_stack_t* pstack )
{
   uint8_t i;
   int16_t handle;

   usb_assert(pstack != NULL);

   usbhci_init();

   pstack->state     = USB_HOST_STATE_IDLE;
   pstack->n_devices = 0;
   pstack->status    = 0;
   pstack->ticks     = 0;

   for (i = 0; i < USB_MAX_DEVICES; ++i)
   {
      usb_device_init(pstack, i);
   }

   for (i = 0; i < USB_N_CTRL_ENDPOINTS; ++i)
   {
      handle = usbhci_pipe_alloc(USB_CTRL);
      if (handle < 0)
      {
         while (1)
         {
            /*
             * If you get  trapped  here,  that  means  the  number  of  control
             * endpoints does not correctly match the ones made available to  us
             * by the hardware.  You should check that in  the  HCI  driver  and
             * correct the number specified for USB_N_CTRL_ENDPOINTS.
             */
         }
      }
      pstack->def_ep[i].handle = (uint8_t) handle;
      pstack->def_ep[i].lock   = 0;
   }

#if (USB_MAX_HUBS > 0)
   usb_hub_init(); /* Initialize HUB driver stack. */
#endif

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
   {
      if (usb_device_is_active(pstack, i))
      {
         usb_device_release(pstack, i);
      }
   }

   return USB_STATUS_OK;
}

uint16_t usb_systick_inc( usb_stack_t* pstack, uint16_t inc )
{
   usb_assert(pstack != NULL);
   return (pstack->ticks += inc);
}

uint16_t usb_systick( usb_stack_t* pstack )
{
   usb_assert(pstack != NULL);
   return pstack->ticks;
}

int usb_run( usb_stack_t* pstack )
{
   int     status;
   uint8_t addr;

   usb_assert(pstack != NULL);

   switch (pstack->state)
   {
      case USB_HOST_STATE_IDLE:
         /*
          * Do nothing until a device is  connected  to  the  USB  hardware,  it
          * could be either a regular device or a HUB,  the  root  HUB  in  this
          * case.
          */
         if (usbhci_is_connected())
         {
            /*
             * On connection, change root device's state to attached  and  start
             * running the stack. The new address must always be 0 for the first
             * device, this can never fail because there are  NO  other  devices
             * connected.
             */
            addr = usb_device_attach(
                  pstack,
                  USB_DEV_PARENT_ROOT,
                  USB_DEV_PARENT_ROOT
            );
            usb_assert(addr == 0);
            pstack->state = USB_HOST_STATE_RUNNING;
         }
         break;

      case USB_HOST_STATE_RUNNING:
         if (pstack->n_devices == 0)
         {
            /* If there are no devices connected, go back to _IDLE. */
            pstack->state = USB_HOST_STATE_IDLE;
         }
         else
         {
            /* Also check for root HUB/device disconnection. */
            if (!usbhci_is_connected())
            {
               /* Device is no longer connected, release everything. */
               status = usb_device_release(pstack, 0);
               if (status != USB_STATUS_OK)
               {
                  usb_assert(0); /** @TODO handle error */
               }
               pstack->state = USB_HOST_STATE_IDLE;
            }
            else
            {
               /*
                * Otherwise, loop through all active devices,  update  them  and
                * handle HUBs (HUBs are the only devices owned by the stack).
                */
               status = usb_stack_update_devices(pstack);
               if (status != USB_STATUS_OK)
               {
                  usb_assert(0); /** @TODO handle error */
               }

#if (USB_MAX_HUBS > 0)
               status = usb_hub_update();
               if (status != USB_STATUS_OK)
               {
                  usb_assert(0); /** @TODO handle error */
               }
#endif
            }
         }
         break;

      case USB_HOST_STATE_SUSPENDED:
         break;

      default:
         pstack->state = USB_HOST_STATE_IDLE;
         break;
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
   int           status;
   usb_device_t* pdev;
   usb_pipe_t*   ppipe;

   /* Validate input parameters. */
   if (  pstack == NULL ||
         buffer == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 ||
         pipe > USB_MAX_ENDPOINTS-1 ||
         len == 0 )
   {
      status = USB_STATUS_INV_PARAM;
   }
   else
   {
      pdev = &pstack->devices[USB_ID_TO_DEV(device_id)];
      if (!(pdev->status & USB_DEV_STATUS_INIT))
      {
         /* If device hasn't finished initialization don't accept IRPs. */
         status = USB_STATUS_BUSY;
      }
      else
      {
         ppipe = &pdev->interfaces[USB_ID_TO_IFACE(device_id)].endpoints[pipe];
         ppipe->buffer  = buffer;
         ppipe->length  = len;
         ppipe->retries = 0;
         status = usbhci_xfer_start(pdev, ppipe);
      }
   }
   return status;
}

int usb_ctrlirp_bypass(
      usb_stack_t*        pstack,
      uint16_t*           ticket,
      uint16_t            device_id,
      const usb_stdreq_t* pstdreq,
      uint8_t*            buffer
)
{
   int             status;
   uint8_t         index;
   usb_device_t*   pdev;
   usb_msg_pipe_t* pmsg;

   /* Validate input parameters. */
   if (  pstack  == NULL ||
         ticket  == NULL ||
         pstdreq == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 )
   {
      status = USB_STATUS_INV_PARAM;
   }
   else if (usb_get_ctrl_pipe(pstack, &index) != USB_STATUS_OK)
   {
      /* Check whether a control endpoint is available. */
      status = USB_STATUS_BUSY;
   }
   else
   {
      /* Pipe-lock in place, now reconfigure the pipe accordingly. */
      pdev    = &pstack->devices[USB_ID_TO_DEV(device_id)];
      pmsg    = &pstack->def_ep[index];
      *ticket = USB_IDX_TO_TICKET(index);
      status  = usbhci_msg_pipe_configure(pdev, pmsg);
      if (status != USB_STATUS_OK)
      {
         usb_assert(0); /** @TODO handle error */
      }
      else
      {
         /* Fill the request buffer with the data in USB-endianness order. */
         USB_STDREQ_SET_bmRequestType(pmsg->stdreq, pstdreq->bmRequestType);
         USB_STDREQ_SET_bRequest(     pmsg->stdreq, pstdreq->bRequest);
         USB_STDREQ_SET_wValue(       pmsg->stdreq, pstdreq->wValue);
         USB_STDREQ_SET_wIndex(       pmsg->stdreq, pstdreq->wIndex);
         USB_STDREQ_SET_wLength(      pmsg->stdreq, pstdreq->wLength);

         /* Device available, start control transfer. */
         pmsg->buffer  = buffer;
         pmsg->retries = 0;
         status = usbhci_ctrlxfer_start(pdev, pmsg);
      }
   }
   return status;
}

int usb_ctrlirp(
      usb_stack_t*        pstack,
      uint16_t*           ticket,
      uint16_t            device_id,
      const usb_stdreq_t* pstdreq,
      uint8_t*            buffer
)
{
   int status;

   /* Validate input parameters. */
   if (  pstack  == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 )
   {
      status = USB_STATUS_INV_PARAM;
   }
   else if (!(pstack->devices[USB_ID_TO_DEV(device_id)].status
               & USB_DEV_STATUS_INIT))
   {
      /* If device hasn't finished initialization don't accept IRPs. */
      status = USB_STATUS_BUSY;
   }
   else
   {
      status = usb_ctrlirp_bypass(pstack, ticket, device_id, pstdreq, buffer);
   }
   return status;
}

int usb_irp_status( usb_stack_t* pstack, uint16_t device_id, uint16_t irp_id )
{
   int status;

   if (  pstack == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 )
   {
      /* Invalid input parameters. */
      status = USB_STATUS_INV_PARAM;
   }
   else if ( USB_IRP_ID_IS_MSG(irp_id) &&
             USB_TICKET_TO_IDX(irp_id) < USB_N_CTRL_ENDPOINTS )
   {
      /* Message pipe, these have a slightly different interface. */
      status = _irp_status(pstack, device_id, USB_TICKET_TO_IDX(irp_id), 1);
      if (status != USB_STATUS_XFER_WAIT)
      {
         /* Release the lock on the message pipe. This cannot fail. */
         usb_unlock_pipe(pstack, USB_TICKET_TO_IDX(irp_id));
      }
   }
   else if (!USB_IRP_ID_IS_MSG(irp_id) && irp_id < USB_MAX_ENDPOINTS)
   {
      /* Streaming pipe. */
      status = _irp_status(pstack, device_id, irp_id, 0);
   }
   else
   {
      status = USB_STATUS_INV_PARAM;
   }
   return status;
}

int usb_irp_cancel( usb_stack_t* pstack, uint16_t device_id, uint16_t irp_id )
{
   int status;

   /* Validate input parameters. */
   if (  pstack == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 )
   {
      status = USB_STATUS_INV_PARAM;
   }
   else if ( USB_IRP_ID_IS_MSG(irp_id) &&
             USB_TICKET_TO_IDX(irp_id) < USB_N_CTRL_ENDPOINTS )
   {
      /* Cancelling a control transfer. */
      usbhci_ctrlxfer_cancel(
            &pstack->devices[USB_ID_TO_DEV(device_id)],
            &pstack->def_ep[USB_TICKET_TO_IDX(irp_id)]
      );
      status = USB_STATUS_OK;
   }
   else if (!USB_IRP_ID_IS_MSG(irp_id) && irp_id < USB_MAX_ENDPOINTS)
   {
      /* ... a streaming endpoint/pipe transfer. */
      usbhci_xfer_cancel(
            &pstack->devices[USB_ID_TO_DEV(device_id)],
            &pstack->devices[USB_ID_TO_DEV(device_id)]
               .interfaces[USB_ID_TO_IFACE(device_id)]
               .endpoints[(uint8_t)irp_id]
      );
      status = USB_STATUS_OK;
   }
   else
   {
      status = USB_STATUS_INV_PARAM;
   }
   return status;
}


/******************************************************************************/

int usb_device_init( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t* pdev;

   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);

   pdev = &pstack->devices[index];
   pdev->state         = USB_DEV_STATE_DISCONNECTED;
   pdev->next_state    = USB_DEV_STATE_DISCONNECTED;
   pdev->status        = 0;
   pdev->ticket        = 0;
   pdev->speed         = USB_SPD_INV;
   pdev->addr          = USB_DEV_DEFAULT_ADDR;
   pdev->mps           = 0;
   pdev->vendor_ID     = 0;
   pdev->product_ID    = 0;
   pdev->ticks_delay   = pstack->ticks;
#if (USB_MAX_HUBS > 0)
   pdev->parent_hub    = USB_DEV_PARENT_ROOT;
   pdev->parent_port   = USB_DEV_PARENT_ROOT;
#endif
   pdev->cte_index     = USB_STACK_INVALID_IDX;
   pdev->max_power     = 0;
   pdev->cfg_value     = 0;

   return USB_STATUS_OK;
}

int usb_device_release( usb_stack_t* pstack, uint8_t index )
{
   uint8_t          i, j;
   uint8_t          n_elems;
   usb_device_t*    pdev;
   usb_interface_t* piface;

   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);

   pdev = &pstack->devices[index];

   /**
    * @TODO write this function!
    *
    * Remember to release downstream devices if this is a HUB!
    * Don't forget to skip i=0 'cause that's the root HUB/device, it should  pop
    * up with the assertion in any case though.
    *
    * Something like follows...
    */
#if 0//(USB_MAX_HUBS > 0)
    usb_device_t dev_i;

   for (i = 1; i < pstack.n_devices; ++i) /* Note the i=1 to skip root. */
   {
      dev_i = pstack.devices[i];
      usb_assert(dev_i.parent_hub < USB_MAX_DEVICES);
      if (&pstack.devices[dev_i.parent_hub] == pdev)
         usb_device_release(dev_i);
   }
#endif

   if (pdev->cte_index != USB_STACK_INVALID_IDX)
   {
      n_elems = USB_GET_IFACES_N(pdev->cte_index);
      for (i = 0; i < n_elems; ++i)
      {
         piface = &pdev->interfaces[i];
         if (piface->driver_handle != USB_IFACE_NO_DRIVER)
         {
            usb_drivers_remove(
                  pstack,
                  USB_TO_ID(index, i),
                  piface->driver_handle );
            piface->driver_handle = USB_IFACE_NO_DRIVER;
         }
         for (j = 0; j < USB_GET_EPS_N(pdev->cte_index, i); ++j)
         {
            usb_pipe_remove(&piface->endpoints[j]);
         }
         piface->class       =   0;
         piface->subclass    =   0;
         piface->protocol    = 255; /* Unsupported protocol number. */
      }
   }

   /* Finish by leaving the device in its default state. */
   usb_device_init(pstack, index);

   pstack->n_devices--;
   return USB_STATUS_OK;
}

int usb_release_from_port( usb_stack_t* pstack, uint8_t hub_idx, uint8_t port )
{
   int16_t index;
   usb_assert((index = _devidx_from_hub_port(pstack, hub_idx, port)) >= 0);
   return usb_device_release(pstack, (uint8_t) index);
}

int usb_pipe_remove( usb_pipe_t* ppipe )
{
   int status;
   if ((status = usbhci_pipe_dealloc(ppipe)) == USB_STATUS_OK)
   {
      ppipe->handle   = (uint8_t) -1; /* Something invalid. */
      ppipe->number   = 0;
      ppipe->type     = USB_CTRL;
      ppipe->dir      = (uint8_t) -1; /* Something invalid. */
      ppipe->mps      = 0;
      ppipe->interval = (uint8_t) -1; /* Something invalid. */
      ppipe->length   = 0;
      ppipe->buffer   = NULL;
   }
   return status;
}

int usb_device_is_active( usb_stack_t* pstack, uint8_t index )
{
   int ret;
   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   ret = 0;
   if (pstack->devices[index].status & USB_DEV_STATUS_ACTIVE)
   {
      ret = 1;
   }
   return ret;
}

int usb_get_ctrl_pipe( usb_stack_t* pstack, uint8_t* index )
{
   uint8_t i;
   int     ret;

   usb_assert(pstack != NULL);
   usb_assert(index  != NULL);

   /* Search for an unlocked control pipe. */
   ret = USB_STATUS_BUSY;
   for (i = 0; i < USB_N_CTRL_ENDPOINTS && ret == USB_STATUS_BUSY; ++i)
   {
/** @TODO take into consideration multi-threaded systems here, use some kind of semaphore or something... */
      if (pstack->def_ep[i].lock == 0)
      {
         /* Endpoint is free, lock it and return. */
         pstack->def_ep[i].lock = 1;
         *index = i;
         ret = USB_STATUS_OK;
      }
   }
   return ret;
}

int usb_unlock_pipe( usb_stack_t* pstack, uint8_t index )
{
   usb_assert(pstack != NULL);
   usb_assert(index < USB_N_CTRL_ENDPOINTS);
/** @TODO I don't think multi-threaded systems will affect this function... check! */
   pstack->def_ep[index].lock = 0;
   return USB_STATUS_OK;
}

int16_t usb_device_attach(
      usb_stack_t* pstack,
      uint8_t      parent_hub,
      uint8_t      parent_port
)
{
   int16_t addr;

   usb_device_t* pdev;
   usb_assert(pstack != NULL);

   addr = usb_stack_new_addr(pstack);
   if (addr >= 0)
   {
      pdev = &pstack->devices[(uint8_t) addr];
      pstack->n_devices += 1;
      pdev->state        = USB_DEV_STATE_ATTACHED;
      pdev->status      |= USB_DEV_STATUS_ACTIVE;
#if (USB_MAX_HUBS > 0)
      pdev->parent_hub   = parent_hub;
      pdev->parent_port  = parent_port;
#endif
      pdev->interfaces   = NULL; /* We request this when enumerating. */
      pdev->cte_index    = USB_STACK_INVALID_IDX;
   }
   return addr;
}


/******************************************************************************/
int usb_device_update( usb_stack_t* pstack, uint8_t index )
{
   return usbd_state_run(pstack, index);
}

int usb_device_parse_cfgdesc( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t*  pdev;
   const uint8_t* buff;
   const uint8_t* next_buff;
   uint8_t        n_ifaces;
   uint8_t        len;
   uint8_t        next_len;
   uint8_t        i;
   int            status;

   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   pdev = &pstack->devices[index];
   buff = pstack->xfer_buffer;
   len  = pstack->xfer_length;
   usb_assert(buff != NULL);
   usb_assert(len > USB_STDDESC_CFG_SIZE);

   /*
    * Use the number of interfacesand product/vendor ID to search for a matching
    * interface array.  The array list is the one generated with  the  specified
    * number of interfaces per device and endpoints per interface,  this  should
    * not fail but could if you plug in something outside what you specified  or
    * the configuration was incorrect.
    */
   n_ifaces = USB_STDDESC_CFG_GET_bNumInterfaces(buff);
   pdev->interfaces = usb_devices_cfg_get_ifaces(
         pdev->product_ID,
         pdev->vendor_ID,
         n_ifaces,
         &pdev->cte_index
   );
   if (pdev->interfaces == NULL)
   {
      /* A matching interface was not found. */
      usb_assert(0); /** @TODO handle error */
   }

   /* Get the configuration value. */
   pdev->cfg_value = USB_STDDESC_CFG_GET_bConfigurationValue(buff);

   /* Get bmAttributes: self-powered? and remote-wakeup? */
   if (USB_STDDESC_CFG_GET_bmAttributes(buff) & USB_STDDESC_CFG_SELF_POWERED)
   {
      pdev->status |= USB_DEV_STATUS_SELF_PWRD;
   }
   if (USB_STDDESC_CFG_GET_bmAttributes(buff) & USB_STDDESC_CFG_REMOTE_WAKEUP)
   {
      pdev->status |= USB_DEV_STATUS_REMOTE_WKUP;
   }

   /* Get maximum power consumption. */
   pdev->max_power = USB_STDDESC_CFG_GET_bMaxPower(buff);

   buff += USB_STDDESC_CFG_SIZE;
   len  -= USB_STDDESC_CFG_SIZE;
   next_len = len;
   for (i = 0; i < n_ifaces; ++i)
   {
      /*
       * Find the next descriptor before parsing so we can pass down the  actual
       * entire interface plus endpoints and such descriptors.
       */
      next_buff = buff;
      len = next_len;
      if (usb_goto_next_desc(
               &next_buff,
               &next_len,
               USB_STDDESC_INTERFACE,
               USB_STDDESC_IFACE_SIZE) )
      {
         /* If no next iface desc was found and ... */
         if (i+1 < n_ifaces)
         {
            /* ... this isn't the last iface, then something's wrong. */
            usb_assert(0); /** @TODO handle error */
         }
         else
         {
            /* Otherwise, it's alright so do nothing. */
            next_buff = NULL;
            next_len  = 0;
         }
      }

      len = len - next_len;
      status = usb_device_parse_ifacedesc(pstack, index, i, &buff, &len);
      if (status)
      {
         usb_assert(0); /** @TODO handle error */
      }
   }

   return USB_STATUS_OK;
}

int usb_device_parse_ifacedesc(
      usb_stack_t*    pstack,
      uint8_t         dev_idx,
      uint8_t         iface_idx,
      const uint8_t** pbuff,
      uint8_t*        plen
)
{
   usb_interface_t*  piface;
   usb_device_t*     pdev;
   uint8_t           n_eps;
   uint8_t           ep;
   int               driver_idx;
   int16_t           pipe_handle;
   int               ret;
   const uint8_t*    buff;
   uint8_t           len;

   usb_assert(pstack != NULL);
   usb_assert(pbuff  != NULL);
   usb_assert(*pbuff != NULL);
   usb_assert(plen   != NULL);
   usb_assert(dev_idx   < USB_MAX_DEVICES);
   usb_assert(iface_idx < USB_MAX_INTERFACES);

   buff   = *pbuff;
   len    = *plen;
   pdev   = &pstack->devices[dev_idx];
   piface = &pdev->interfaces[iface_idx];

   if (*plen < USB_STDDESC_IFACE_SIZE)
   {
      /* Validate buffers minimum length. */
      ret = USB_STATUS_INV_PARAM;
   }
   else if(USB_STDDESC_IFACE_GET_bDescriptorType(buff) != USB_STDDESC_INTERFACE)
   {
      /* Validate interface's descriptor type. */
      ret = USB_STATUS_INV_DESC;
   }
   else if (USB_STDDESC_IFACE_GET_bNumEndpoints(buff) > USB_MAX_ENDPOINTS)
   {
      /* Validate maximum number of endpoints. */
      ret = USB_STATUS_EP_AVAIL;
   }
   else
   {
      /* Everything's alright so far, go ahead and copy base data over. */
      piface->class       = USB_STDDESC_IFACE_GET_bInterfaceClass(buff);
      piface->subclass    = USB_STDDESC_IFACE_GET_bInterfaceSubClass(buff);
      piface->protocol    = USB_STDDESC_IFACE_GET_bInterfaceProtocol(buff);

      /* Check if the configured interface matches the number of endpoints. */
      n_eps = USB_STDDESC_IFACE_GET_bNumEndpoints(buff);
      if (n_eps != USB_GET_EPS_N(pdev->cte_index, iface_idx))
      {
         usb_assert(0); /** @TODO handle error */
      }

      /* Check whether there's a suitable driver for the given interface. */
      driver_idx = usb_drivers_probe(pdev, 0, buff, len);
      if (driver_idx < 0)
      {
         /*
          * If a driver couldn't be found, skip the rest of  the  initialization
          * process.  However, the interface will still be listed and accessible
          * through the USB stack, this way the user can get  information  about
          * it and find out why it wasn't assigned a driver.
          */
         piface->driver_handle = (usb_driver_handle_t) USB_IFACE_NO_DRIVER;
         ret = USB_STATUS_OK;
      }
      else
      {
         /* Driver found, assign it to interface and parse ep descriptors. */
         usb_assert(driver_idx < USB_MAX_DRIVERS);
         piface->driver_handle = (usb_driver_handle_t) driver_idx;

         for (ep = 0; ep < n_eps; ++ep)
         {
            /*
             * First, advance buffer till  next  endpoint  descriptor,  this  is
             * important because other descriptors might be in between  endpoint
             * ones and that is only known by the driver.
             */
            if (usb_goto_next_desc(
                     pbuff,
                     plen,
                     USB_STDDESC_ENDPOINT,
                     USB_STDDESC_EP_SIZE) )
            {
               usb_assert(0); /** @TODO handle error */
            }

            /* Get endpoint info and initialize it. */
            usb_device_parse_epdesc(&piface->endpoints[ep], *pbuff);

            /* Allocate physical endpoint to it ... */
            pipe_handle = usbhci_pipe_alloc(piface->endpoints[ep].type);
            if (pipe_handle < 0)
            {
               usb_assert(0); /** @TODO handle error */
            }
            piface->endpoints[ep].handle = (uint8_t) pipe_handle;

            /* ... and configure it. */
            if (usbhci_pipe_configure(pdev, &piface->endpoints[ep]))
            {
               usb_assert(0); /** @TODO handle error */
            }
         }

         /* Finally, register interface with driver. */
         ret = usb_drivers_assign(
               pstack,
               USB_TO_ID(dev_idx, iface_idx),
               buff,
               len,
               driver_idx
         );
         if (ret)
         {
            usb_assert(0); /** @TODO handle error */
         }
      }
   }
   return ret;
}

int usb_device_parse_epdesc( usb_pipe_t* ppipe, const uint8_t* buffer )
{
   usb_assert(ppipe  != NULL);
   usb_assert(buffer != NULL);

   ppipe->number   = USB_STDDESC_EP_GET_bEndpointAddress(buffer) & 0x7F;
   ppipe->type     = USB_STDDESC_EP_GET_bmAttributes(buffer) & USB_STDDESC_EP_ATTR_TYPE_MASK;
   ppipe->dir      =(USB_STDDESC_EP_GET_bEndpointAddress(buffer) & 0x80) ? USB_DIR_IN : USB_DIR_OUT;
   ppipe->mps      = USB_STDDESC_EP_GET_wMaxPacketSize(buffer) & 0x7FF;
   ppipe->interval = USB_STDDESC_EP_GET_bInterval(buffer);
   ppipe->length   = 0;
   ppipe->buffer   = NULL;

   return USB_STATUS_OK;
}


/******************************************************************************/
int16_t usb_stack_new_addr( usb_stack_t* pstack )
{
   int addr;
   usb_assert(pstack != NULL);
   for (addr = 0; addr < USB_MAX_DEVICES; ++addr)
   {
      if (!usb_device_is_active(pstack, addr))
      {
         break;
      }
   }
   if (addr >= USB_MAX_DEVICES)
   {
      /* No available address found. */
      addr = -1;
   }
   return addr;
}

int usb_stack_update_devices( usb_stack_t* pstack )
{
   uint8_t i;
   int     status;

   usb_assert(pstack != NULL);

   for (i = 0; i < USB_MAX_DEVICES; ++i)
   {
      if (usb_device_is_active(pstack, i))
      {
         status = usb_device_update(pstack, i);
         if (status != USB_STATUS_OK)
         {
            usb_assert(0); /** @TODO handle error */
         }
      }
   }
   return USB_STATUS_OK;
}

int16_t usb_pipe_get_interval( usb_stack_t* pstack, uint16_t id, uint8_t pipe )
{
   int16_t       ret;
   uint8_t       bInt;
   usb_device_t* pdev;
   usb_pipe_t*   ppipe;

   /* Validate input parameters. */
   if (  pstack == NULL ||
         USB_ID_TO_DEV(id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(id) > USB_MAX_INTERFACES-1 ||
         pipe > USB_MAX_ENDPOINTS-1 )
   {
      ret = -1;
   }
   else
   {
      pdev  = &pstack->devices[USB_ID_TO_DEV(id)];
      ppipe = &pdev->interfaces[USB_ID_TO_IFACE(id)].endpoints[pipe];
      bInt  = ppipe->interval;
      if (   ppipe->type == USB_ISO ||
            (ppipe->type == USB_INT && pdev->speed == USB_SPD_HS))
      {
         /*
          * ISO FS/HS (there are no ISO LS) or INT HS.
          * interval = 2**(bInterval-1) with bInterval = [1:16]
          */
         if (bInt < USB_ISO_INTHS_MIN_bInt || bInt > USB_ISO_INTHS_MAX_bInt)
         {
            ret = -2;
         }
         else
         {
            ret = (1 << (bInt-1));
         }
      }
      else if (ppipe->type == USB_INT && pdev->speed != USB_SPD_HS)
      {
         /* INT FS/LS: interval = bInterval with bInterval = [1:255] */
         if (bInt < 1)
         {
            ret = -2;
         }
         else
         {
            ret = bInt;
         }
      }
      else
      {
         /*
          * Here we should check for BULK HS, but since we don't  care  what  we
          * return for other cases (because they make now  sense)  we  can  just
          * return the same for all of them.
          */
         ret = bInt;
      }
   }
   if (ret >= 0)
   {
      if (pdev->speed == USB_SPD_HS)
      {
         /* Convert microframes into milliseconds */
         ret = (ret + 7) / 8;
      }
      /* Convert milliseconds into ticks. */
      ret = (ret + USB_TASK_TICKS-1) / USB_TASK_TICKS;
   }
   return ret;
}

uint8_t usb_device_get_addr( usb_stack_t* pstack, uint16_t id )
{
   uint8_t ret;
   /* Validate input parameters. */
   if ( pstack == NULL || USB_ID_TO_DEV(id) > USB_MAX_DEVICES-1)
   {
      ret = USB_STACK_INVALID_IDX;
   }
   else
   {
      ret = pstack->devices[USB_ID_TO_DEV(id)].addr;
   }
   return ret;
}


void usb_assert(int condition)
{
   if (!condition)
   {
usb_assert:
      while (1);
   }
}


/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


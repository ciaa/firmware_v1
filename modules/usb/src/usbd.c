
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


/*==================[external functions definition]==========================*/

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
   {
      usb_device_init(pstack, i);
   }

#if (USB_MAX_HUBS > 0)
   usb_hub_init();
   for (i = 0; i < USB_MAX_HUBS; ++i)
   {
      pstack->hubs[i] = USB_STACK_INVALID_HUB_IDX;
   }
   pstack->n_hubs = 0;
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
   int     addr;  /* Need whole range for return codes. */

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
             * This new address must always be 0 for the first device and it can
             * never fail, thus, an assert here is OK.
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
               usb_assert(0); /** @TODO handle error */
            pstack->state = USB_HOST_STATE_IDLE;
            break;
         }
         /*
          * Otherwise, loop through all active devices, update them  and  handle
          * HUBs (HUBs are the only devices owned by the stack).
          */
         status = usb_stack_update_devices(pstack);
         if (status != USB_STATUS_OK)
            usb_assert(0); /** @TODO handle error */

#if (USB_MAX_HUBS > 0)
         status = usb_hub_update();
         if (status != USB_STATUS_OK)
            usb_assert(0); /** @TODO handle error */
#endif

#if 0//(USB_MAX_HUBS > 0)
         status = usb_stack_handle_hubs(pstack);
         if (status != USB_STATUS_OK)
            usb_assert(0); /** @TODO handle error */
#endif

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
   /* Validate input parameters. */
   if (  pstack == NULL ||
         buffer == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 ||
         pipe > USB_MAX_ENDPOINTS-1 ||
         len == 0 )
   {
      return USB_STATUS_INV_PARAM;
   }
   else
   {
      return usbhci_xfer_start(
            &pstack->devices[USB_ID_TO_DEV(device_id)].
               interfaces[USB_ID_TO_IFACE(device_id)].
               endpoints[pipe],
            buffer,
            len );
   }
}

int usb_ctrlirp(
      usb_stack_t*        pstack,
      uint16_t            device_id,
      const usb_stdreq_t* pstdreq,
      uint8_t*            buffer
)
{
   int           status;
   usb_device_t* pdevice;

   /* Validate input parameters. */
   if (  pstack  == NULL ||
         pstdreq == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 )
   {
      return USB_STATUS_INV_PARAM;
   }

   /*
    * Check whether the  control  endpoint  is  available.   This  is  necessary
    * because a device has only ONE control endpoint but all of  its  interfaces
    * share it.
    *
    * @TODO
    * Keep in mind each interface will most likely be  handled  by  a  different
    * driver and each of them will probably be running concurrently.  This means
    * some sort of semaphore should be used here.
    */
   if (usb_device_lock(pstack, USB_ID_TO_DEV(device_id)) != USB_STATUS_OK)
   {
      status = USB_STATUS_BUSY;
   }
   else
   {
      /* Reconfigure pipe accordingly. */
      pdevice = &pstack->devices[USB_ID_TO_DEV(device_id)];
      status = usbhci_pipe_configure(
            &pdevice->default_ep,
            pdevice->addr,
            pdevice->speed );
      if (status)
      {
         usb_assert(0); /** @TODO handle error */
      }
      else
      {
         /* Fill the request buffer with the data in USB-endianness order. */
         USB_STDREQ_SET_bmRequestType(pdevice->stdreq, pstdreq->bmRequestType);
         USB_STDREQ_SET_bRequest(     pdevice->stdreq, pstdreq->bRequest);
         USB_STDREQ_SET_wValue(       pdevice->stdreq, pstdreq->wValue);
         USB_STDREQ_SET_wIndex(       pdevice->stdreq, pstdreq->wIndex);
         USB_STDREQ_SET_wLength(      pdevice->stdreq, pstdreq->wLength);

         /* Device available, start control transfer. */
         status = usbhci_ctrlxfer_start(
               &pdevice->default_ep,
               pdevice->stdreq,
               buffer );
      }
   }
   return status;
}

int usb_irp_status( usb_stack_t* pstack, uint16_t device_id, uint8_t pipe )
{
   int status;

   /* Validate input parameters. */
   if (  pstack == NULL ||
         USB_ID_TO_DEV(device_id) > USB_MAX_DEVICES-1 ||
         USB_ID_TO_IFACE(device_id) > USB_MAX_INTERFACES-1 )
   {
      return USB_STATUS_INV_PARAM;
   }

   if (pipe == USB_CTRL_PIPE_TOKEN)
   {
      /*
       * Control pipes need to be handled separately because there's only one per
       * device, instead of one per interface.
       */
      status = usbhci_xfer_status(
            &pstack->devices[USB_ID_TO_DEV(device_id)].default_ep );
      if (status != USB_STATUS_XFER_WAIT)
      {
         /* Release the lock on the control pipe. This cannot fail. */
         usb_device_unlock(pstack, USB_ID_TO_DEV(device_id));
      }
      if (status == USB_STATUS_EP_STALLED)
      {
         /* If the endpoint was stalled, cancel the rest of the transfer. */
         usbhci_xfer_cancel( /* ???? */
               &pstack->devices[USB_ID_TO_DEV(device_id)].default_ep);
      }
   }
   else
   {
      if (pipe > USB_MAX_ENDPOINTS-1)
      {
         status = USB_STATUS_INV_PARAM;
      }
      else
      {
         status = usbhci_xfer_status(
               &pstack->devices[USB_ID_TO_DEV(device_id)].
                  interfaces[USB_ID_TO_IFACE(device_id)].
                  endpoints[pipe] );
      }
   }
   return status;
}

int usb_irp_cancel( usb_stack_t* pstack, uint16_t device_id, uint8_t pipe )
{
   /** @TODO write this function */
   return USB_STATUS_OK;
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
   pdevice->parent_hub    = USB_DEV_PARENT_ROOT;
   pdevice->parent_port   = USB_DEV_PARENT_ROOT;
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
    * @TODO write this function!
    *
    * Remember to release downstream devices if this is a HUB!
    * Don't forget to skip i=0 'cause that's the root HUB/device, it should  pop
    * up with the assertion in any case though.
    *
    * Something like:
    *
#if (USB_MAX_HUBS > 0)
    usb_device_t dev_i;

   for (i = 1; i < pstack.n_devices; ++i) /* Note the i=1 to skip root. * /
   {
      dev_i = pstack.devices[i];
      usb_assert(dev_i.parent_hub < USB_MAX_DEVICES);
      if (&pstack.devices[dev_i.parent_hub] == pdevice)
         usb_device_release(dev_i);
   }
#endif
    */

   /** @TODO error check the following instructions. */
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
      {
         usbhci_pipe_dealloc(&piface->endpoints[j]);
      }
      piface->n_endpoints = 0;
      piface->class       = 0;
      piface->subclass    = 0;
      piface->protocol    = 0;
      pdevice->n_interfaces--;
   }
   usb_assert(pdevice->n_interfaces == 0);

   /* Finish by leaving the device in its default state. */
   usb_device_init(pstack, index);

   pstack->n_devices--;
   return USB_STATUS_OK;
}

int usb_device_is_active( usb_stack_t* pstack, uint8_t index )
{
   int ret;
   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   ret = 0;
   if (pstack->devices[index].status & USB_DEV_STATUS_ACTIVE)
      ret = 1;
   return ret;
}

int usb_device_lock( usb_stack_t* pstack, uint8_t index )
{
   int ret;
   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
/** @TODO take into consideration multi-threaded systems here, use some kind of semaphore or something... */
   if (pstack->devices[index].status & USB_DEV_STATUS_LOCKED)
   {
      ret = USB_STATUS_BUSY;
   }
   else
   {
      pstack->devices[index].status |= USB_DEV_STATUS_LOCKED;
      ret = USB_STATUS_OK;
   }
   return ret;
}

int usb_device_unlock( usb_stack_t* pstack, uint8_t index )
{
   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
/** @TODO I don't think multi-threaded systems will affect this function... check! */
   pstack->devices[index].status &= ~USB_DEV_STATUS_LOCKED;
   return USB_STATUS_OK;
}

void usb_device_attach(
      usb_stack_t*  pstack,
#if (USB_MAX_HUBS > 0)
      uint8_t       parent_hub,
      uint8_t       parent_port,
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

#if 0 /* (USB_MAX_HUBS > 0) might not be necessary anymore, check this! */
/** @FIXME: this function will be either re-written or replaced. */
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
int usb_device_update( usb_stack_t* pstack, uint8_t index )
{
   return usbd_state_run(pstack, index);
}

int usb_device_parse_cfgdesc( usb_stack_t* pstack, uint8_t index )
{
   usb_device_t*  pdevice;
   const uint8_t* buff;
   const uint8_t* next_buff;
   uint8_t        len;
   uint8_t        next_len;
   uint8_t        i;
   int            status;

   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   pdevice = &pstack->devices[index];
   buff = pdevice->xfer_buffer;
   len  = pdevice->xfer_length;
   usb_assert(buff != NULL);
   usb_assert(len > USB_STDDESC_CFG_SIZE);

   /* Get the number of interfaces. */
   pdevice->n_interfaces = USB_STDDESC_CFG_GET_bNumInterfaces(buff);
   if (pdevice->n_interfaces > USB_MAX_INTERFACES)
      usb_assert(0); /** @TODO handle error */

   /* Get the configuration value. */
   pdevice->cfg_value = USB_STDDESC_CFG_GET_bConfigurationValue(buff);

   buff += USB_STDDESC_CFG_SIZE;
   len  -= USB_STDDESC_CFG_SIZE;
   next_len = len;
   for (i = 0; i < pdevice->n_interfaces; ++i)
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
         /* If no next iface desc. was found and ... */
         if (i+1 < pdevice->n_interfaces)
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
      status = usb_device_parse_ifacedesc(
            pstack,
            USB_TO_ID(index, i),
            &buff,
            &len );
      if (status)
         usb_assert(0); /** @TODO handle error */
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
   uint8_t           ep;
   int               driver_idx;
   int               ret;
   const uint8_t*    buff;
   uint8_t           len;

   usb_assert(pstack != NULL);
   usb_assert(USB_ID_TO_DEV(id) < USB_MAX_DEVICES);
   usb_assert(USB_ID_TO_IFACE(id) < USB_MAX_INTERFACES);
   usb_assert(pbuff  != NULL);
   usb_assert(*pbuff != NULL);
   usb_assert(plen   != NULL);

   buff        = *pbuff;
   len         = *plen;
   pdevice     = &pstack->devices[USB_ID_TO_DEV(id)];
   piface      = &pdevice->interfaces[USB_ID_TO_IFACE(id)];

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
      piface->n_endpoints = USB_STDDESC_IFACE_GET_bNumEndpoints(buff);
      piface->class       = USB_STDDESC_IFACE_GET_bInterfaceClass(buff);
      piface->subclass    = USB_STDDESC_IFACE_GET_bInterfaceSubClass(buff);
      piface->protocol    = USB_STDDESC_IFACE_GET_bInterfaceProtocol(buff);

      /* Check whether there's a suitable driver for the given interface. */
      driver_idx = usb_drivers_probe(pdevice, 0, buff, len);
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
         /* Driver found, assign it to interface. */
         usb_assert(driver_idx < USB_MAX_DRIVERS);
         piface->driver_handle = (usb_driver_handle_t) driver_idx;

         /* Endpoints pipes allocated, go ahead and parse descriptors. */
         for (ep = 0; ep < piface->n_endpoints; ++ep)
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
            ret = usbhci_pipe_alloc(piface->endpoints[ep].type);
            if (ret < 0)
            {
               usb_assert(0); /** @TODO handle error */
            }
            //usb_assert(ret < USB_MAX_ENDPOINTS);
            piface->endpoints[ep].handle = (uint8_t) ret;

            /* ... and configure it. */
            if (usbhci_pipe_configure(
                  &piface->endpoints[ep],
                  pdevice->addr,
                  pdevice->speed) )
            {
               usb_assert(0); /** @TODO handle error */
            }
         }

         /* Finally, register interface with driver. */
         ret = usb_drivers_assign(pstack, id, buff, len, driver_idx);
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

   return USB_STATUS_OK;
}


/******************************************************************************/
int16_t usb_stack_new_addr( usb_stack_t* pstack )
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
         status = usb_device_update(pstack, i);
         if (status != USB_STATUS_OK)
            usb_assert(0); /** @TODO handle error */
      }
      if (looped_devs >= pstack->n_devices)
         break; /* Done with active devices. */
   }
   return USB_STATUS_OK;
}

#if 0//(USB_MAX_HUBS > 0)
int usb_stack_handle_hubs( usb_stack_t* pstack )
{
/** @TODO finish this precedure */
   uint8_t         hub_idx;
   usb_device_t*   pdevice;
   int16_t         addr;  /* Need whole range for return codes. */
   int16_t         index; /* IDEM. */
   uint8_t         i, j;
   uint8_t         port_address;

   usb_assert(pstack != NULL);

   /*
    * First, update the HUB driver.  This also takes care of HUBs initialization
    * routine, not just updating their state and status.
    */
   usb_hub_update(); /** @TODO check return status */

   /*
    * Second, loop  through  each  HUB  and  check  its  status  and  ports  for
    * [dis]connections and other device-related events.
    */
   for (i = 0; pstack->hubs[i] != USB_STACK_INVALID_HUB_IDX && i < USB_MAX_HUBS; ++i)
   {
      hub_idx = pstack->hubs[i];

      /* Loop through each HUB's port. */
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
                  usb_assert(0); /** @TODO handle error */
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
                  usb_assert(0); /** @TODO handle error */
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


/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/



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
   usb_hub_init(); /* Initialize HUB driver stack. */
   for (i = 0; i < USB_MAX_HUBS; ++i)
   {
      pstack->hubs[i] = USB_STACK_INVALID_HUB_IDX;
   }
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
            break;
         }
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
            break;
         }
         /*
          * Otherwise, loop through all active devices, update them  and  handle
          * HUBs (HUBs are the only devices owned by the stack).
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
   usb_device_t* pdev;

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
      pdev = &pstack->devices[USB_ID_TO_DEV(device_id)];
      status = usbhci_pipe_configure(&pdev->default_ep,pdev->addr, pdev->speed);
      if (status)
      {
         usb_assert(0); /** @TODO handle error */
      }
      else
      {
         /* Fill the request buffer with the data in USB-endianness order. */
         USB_STDREQ_SET_bmRequestType(pdev->stdreq, pstdreq->bmRequestType);
         USB_STDREQ_SET_bRequest(     pdev->stdreq, pstdreq->bRequest);
         USB_STDREQ_SET_wValue(       pdev->stdreq, pstdreq->wValue);
         USB_STDREQ_SET_wIndex(       pdev->stdreq, pstdreq->wIndex);
         USB_STDREQ_SET_wLength(      pdev->stdreq, pstdreq->wLength);

         /* Device available, start control transfer. */
         status = usbhci_ctrlxfer_start(&pdev->default_ep,pdev->stdreq, buffer);
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
   usb_device_t* pdev;

   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);

   pdev = &pstack->devices[index];
   pdev->state         = USB_DEV_STATE_DISCONNECTED;
   pdev->next_state    = USB_DEV_STATE_DISCONNECTED;
   pdev->status        = 0;
   pdev->speed         = USB_SPD_INV;
   pdev->addr          = USB_DEV_DEFAULT_ADDR;
   pdev->n_interfaces  = 0;
   pdev->vendor_ID     = 0;
   pdev->product_ID    = 0;
   pdev->ticks_delay   = pstack->ticks;
#if (USB_MAX_HUBS > 0)
   pdev->parent_hub    = USB_DEV_PARENT_ROOT;
   pdev->parent_port   = USB_DEV_PARENT_ROOT;
#endif

   return USB_STATUS_OK;
}

int usb_device_release( usb_stack_t* pstack, uint8_t index )
{
   uint8_t          i, j;
   uint8_t          n_ep;
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

   /** @TODO error check the following instructions. */
   for (i = 0; i < USB_MAX_INTERFACES; ++i)
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
      n_ep = piface->n_endpoints;
      for (j = 0; j < n_ep; ++j)
      {
         usbhci_pipe_dealloc(&piface->endpoints[j]);
      }
      piface->n_endpoints = 0;
      piface->class       = 0;
      piface->subclass    = 0;
      piface->protocol    = 0;
      pdev->n_interfaces--;
   }
   usb_assert(pdev->n_interfaces == 0);

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
   {
      ret = 1;
   }
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
   uint8_t        len;
   uint8_t        next_len;
   uint8_t        i;
   int            status;

   usb_assert(pstack != NULL);
   usb_assert(index < USB_MAX_DEVICES);
   pdev = &pstack->devices[index];
   buff = pdev->xfer_buffer;
   len  = pdev->xfer_length;
   usb_assert(buff != NULL);
   usb_assert(len > USB_STDDESC_CFG_SIZE);

   /* Get the number of interfaces. */
   pdev->n_interfaces = USB_STDDESC_CFG_GET_bNumInterfaces(buff);
   if (pdev->n_interfaces > USB_MAX_INTERFACES)
   {
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
   for (i = 0; i < pdev->n_interfaces; ++i)
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
         if (i+1 < pdev->n_interfaces)
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
      {
         usb_assert(0); /** @TODO handle error */
      }
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
   usb_device_t*     pdev;
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

   buff   = *pbuff;
   len    = *plen;
   pdev   = &pstack->devices[USB_ID_TO_DEV(id)];
   piface = &pdev->interfaces[USB_ID_TO_IFACE(id)];

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
                  pdev->addr,
                  pdev->speed) )
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


void usb_assert(int condition)
{
   if (!condition)
      while (1);
}


/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


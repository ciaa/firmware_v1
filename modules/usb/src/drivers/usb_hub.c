
/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>

#include "usb.h"
#include "usbd.h"
#include "usb_desc.h"
#include "drivers/usb_drivers.h"
#include "drivers/usb_hub.h"


/*==================[internal data declaration]==============================*/
static hub_stack_t _hub_stack; /* Maybe this shouldn't be static or at least not here like this... */


/*==================[internal functions declaration]=========================*/

/**
 * @brief De-initialize device.
 * @return Non-zero on error.
 */
static int _deinit_dev( uint8_t index );

/**
 * @brief Get next free HUB index and mark it as not-free if found.
 * @return Negative on error, HUB device's internal index otherwise.
 */
static int16_t _get_free_dev( void );

/**
 * @brief Update HUB state and status.
 * @return Non-zero on error... @TODO complete this!
 */
static int _update_dev( uint8_t index );

/**
 * @brief Validate first endpoint in interface's descriptor.
 *
 * Buffer should be pointed to somewhere \b before  the  endpoint's  descriptor,
 * it will be advanced to it on entry.
 * Remember that this endpoint must be an interrupt IN one.
 */
static int _validate_first_ep( const uint8_t** pbuffer, uint8_t* plen );

/**
 * @brief Parse HUB descriptor.
 *
 * The descriptor must have  been  requested  before  hand  and  stored  in  the
 * device's internal buffer: phub->buffer.
 *
 * @param phub  Pointer to HUB.
 * @retval USB_STATUS_OK          Descriptor parsed  correctly  and  information
 *                                stored in HUB.
 * @retval USB_STATUS_XFER_RETRY  Descriptor's length requested  was  lass  than
 *                                the actual  descriptor's  length.   Issue  the
 *                                transfer again,  the  correct  value  will  be
 *                                stored in phub->buffer_len.
 * @return Any other value on error.
 */
static int _parse_hub_desc( usb_hub_t* phub );


/*==================[internal functions definition]==========================*/

static int _deinit_dev( uint8_t index )
{
   usb_hub_t* pdev;
   uint8_t    i;
   usb_assert(index < USB_MAX_HUBS);

   pdev = &_hub_stack.hubs[index];
   pdev->status     = HUB_STATUS_FREE | HUB_STATUS_ENTRY;
   pdev->pstack     = NULL;
   pdev->id         = 0xFFFF;
   pdev->state      = HUB_STATE_IDLE;
   pdev->buffer_len = 0;
   pdev->n_ports    = 0;

   for (i = 0; i < USB_MAX_HUB_PORTS; ++i)
      pdev->port_status[i] = 0x00;

   return 0;
}

static int16_t _get_free_dev( void )
{
   int16_t i;
   for (i = 0; i < USB_MAX_HUBS; ++i)
   {
      /* Iterate until a free device is found. */
      if (_hub_stack.hubs[i].status & HUB_STATUS_FREE)
      {
         /* If it was found, mark it as not-free and return its index. */
         _hub_stack.hubs[i].status &= ~HUB_STATUS_FREE;
         break;
      }
   }
   if (i >= USB_MAX_HUBS)
   {
      /* No device was free, return with an error. */
      i = -1;
   }
   return i;
}

static int _update_dev( uint8_t index )
{
   usb_stdreq_t stdreq;
   usb_hub_t*   pdev;
   int          status = USB_STATUS_OK;

   usb_assert(index < USB_MAX_HUBS);
   pdev = &_hub_stack.hubs[index];

   switch (pdev->state)
   {
      case HUB_STATE_IDLE:
         /* Do nothing and wait for an interface to be assigned to this HUB. */
         if (!(pdev->status & HUB_STATUS_FREE))
         {
            pdev->state      = HUB_STATE_GET_DESC;
            pdev->status    |= HUB_STATUS_ENTRY;
            /*
             * Since we might need to retry the next request (because  we  don't
             * yet know it's actual size), this signals it's the first try.
             */
            pdev->buffer_len = 0;
         }
         break;

      case HUB_STATE_GET_DESC:
         if (pdev->status & HUB_STATUS_ENTRY)
         {
            /* Request HUB descriptor. */
            stdreq.bmRequestType = USB_STDREQ_REQTYPE(
                  USB_DIR_IN,
                  USB_STDREQ_TYPE_CLASS,
                  USB_STDREQ_RECIP_DEV );
            stdreq.bRequest      = HUB_CLASSREQ_GET_DESCRIPTOR;
            stdreq.wValue        = (USB_HUB_DESC_TYPE << 8);
            stdreq.wIndex        = 0;
            if (pdev->buffer_len == 0)
            {
               /* When trying for the first time, request the default size. */
               stdreq.wLength       = HUB_DESC_SIZE;
            }
            else
            {
               /* Otherwise, it's been read from the previous request. */
               stdreq.wLength       = pdev->buffer_len;
            }

            pdev->buffer_len = HUB_DESC_SIZE;
            status = usb_ctrlirp(
                  pdev->pstack,
                  pdev->id,
                  &stdreq,
                  pdev->buffer
            );
            if (status == USB_STATUS_BUSY)
            {
               /* This isn't an error, so notify update as OK. */
               status = USB_STATUS_OK;
               break; /* Try again later... */
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }
            pdev->status &= ~HUB_STATUS_ENTRY;
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
            {
               break; /* Keep waiting... */
            }
            if (status == USB_STATUS_EP_STALLED)
            {
               /* If endpoint was stalled, try again. */
               /** @TODO this should be notified and NOT be done indefinitely*/
               pdev->status |= HUB_STATUS_ENTRY;
               break;
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }

            /* Everything's alright, parse the descriptor. */
            status = _parse_hub_desc(pdev);
            if (status == USB_STATUS_XFER_RETRY)
            {
               /* Need to get the entire descriptor. Retry request. */
               pdev->status  |= HUB_STATUS_ENTRY;
            }
            else if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }
            else
            {
               /* Advance to next state. */
               pdev->state   = HUB_STATE_RUNNING;
               pdev->status |= HUB_STATUS_ENTRY;
            }
         }
         break;

      case HUB_STATE_RUNNING:
         break;

      default:
         pdev->state   = HUB_STATE_IDLE;
         pdev->status |= HUB_STATUS_ENTRY;
         status = USB_STATUS_OK;
         break;
   }

   return status;
}

static int _validate_first_ep(const uint8_t** pbuffer, uint8_t* plen )
{
   int status;

   usb_assert(pbuffer  != NULL);
   usb_assert(*pbuffer != NULL);
   usb_assert(plen     != NULL);

   if (usb_goto_next_desc(
         pbuffer,
         plen,
         USB_STDDESC_ENDPOINT,
         USB_STDDESC_EP_SIZE ))
   {
      /* No endpoint descriptor found in buffer. */
      status = USB_STATUS_DRIVER_FAIL;
   }
   else if (*plen < USB_STDDESC_EP_SIZE)
   {
      /* Endpoint descriptor found but buffer isn't long enough. */
      status = USB_STATUS_DRIVER_FAIL;
   }
   else
   {
      if (!(USB_STDDESC_EP_GET_bEndpointAddress(*pbuffer) & USB_DIR_MASK))
      {
         /* Direction wasn't IN. */
         status = USB_STATUS_DRIVER_FAIL;
      }
      else if ((USB_STDDESC_EP_GET_bmAttributes(*pbuffer) &
               USB_STDDESC_EP_ATTR_TYPE_MASK) != USB_INT)
      {
         /* Type wasn't interrupt. */
         status = USB_STATUS_DRIVER_FAIL;
      }
      else
      {
         status = USB_STATUS_OK;
      }
   }
   return status;
}

static int _parse_hub_desc( usb_hub_t* phub )
{
   uint8_t* buff;
   int      ret;
   uint8_t  actual_length;
   uint16_t wHubChars;

   usb_assert(phub != NULL);

   buff = phub->buffer;

   /* 1) Check the descriptor's type. */
   if (HUB_DESC_GET_bDescriptorType(buff) != USB_HUB_DESC_TYPE)
   {
      ret = USB_STATUS_INV_DESC;
   }
   /* 2) Get the number of ports. */
   else if ((phub->n_ports = HUB_DESC_GET_bNbrPorts(buff)) > USB_MAX_HUB_PORTS)
   {
      /** @TODO mark the HUB as unsupported, this is not an error. */
      usb_assert(0);
   }
   else if (phub->n_ports == 0)
   {
      /* There cannot be a HUB with 0 ports. */
      ret = USB_STATUS_INV_DESC;
   }
   else
   {
      /* 3) Calculate the actual descriptor length and verify. */
      actual_length = HUB_DESC_REAL_SIZE(phub->n_ports);
      if (actual_length > phub->buffer_len)
      {
         /*
          * If the requested size wasn't enough for the  entire  descriptor  and
          * the actual length still fits  inside  the  HUB  buffer,  request  it
          * again.
          */
         if (actual_length <= HUB_BUFF_LEN)
         {
            /* Actual length will be returned as the buffer length. */
            phub->buffer_len = actual_length;
            ret = USB_STATUS_XFER_RETRY;
         }
         else
         {
            /** @TODO notify this somewhere, it's not really an error... */
            ret = USB_STATUS_INV_DESC;
         }
      }
      else
      {
         /* 4) Get HUB's characteristics. */
         wHubChars = HUB_DESC_GET_wHubCharacteristics(buff);

         /* Test for individual power switching. */
         if (((wHubChars & HUB_LPSM_MASK) >> HUB_LPSM_SHIFT) == HUB_LPSM_INDIV)
            phub->status |=  HUB_STATUS_INDIV_POWER;
         else
            phub->status &= ~HUB_STATUS_INDIV_POWER;
         /* Test for compound device. */
         if (wHubChars & HUB_CDI)
            phub->status |=  HUB_STATUS_COMPOUND_DEV;
         else
            phub->status &= ~HUB_STATUS_COMPOUND_DEV;
         /* Test for individual over-current reports. */
         if (((wHubChars & HUB_OCPM_MASK) >> HUB_OCPM_SHIFT) == HUB_OCPM_INDIV)
            phub->status |=  HUB_STATUS_INDIV_OVC;
         else
            phub->status &= ~HUB_STATUS_INDIV_OVC;
         /* Test for port indicators support. */
         if (wHubChars & HUB_PIS)
            phub->status |=  HUB_STATUS_INDICATORS;
         else
            phub->status &= ~HUB_STATUS_INDICATORS;

         /* 5) Get power-on sequence duration. */
         phub->poweron_t = HUB_DESC_GET_bPwrOn2PwrGood(buff);
#if 0 /** @TODO Request the USB host for power consumption. */
         /* 6) Request HUB's power consumption requirements from Host. */
#endif
         ret = USB_STATUS_OK;
      }
   }

   return ret;
}


/*==================[external functions definition]==========================*/

int usb_hub_probe( const uint8_t* buffer, uint8_t length )
{
   int     status;

   usb_assert(buffer != NULL);

   /*
    * Validate the input parameters.  Buffer can't be NULL but length has to  be
    * long enough to hold, at least, the interface and endpoint descriptors.
    */
   if (length < (USB_STDDESC_IFACE_SIZE + USB_STDDESC_EP_SIZE))
   {
      return USB_STATUS_DRIVER_FAIL;
   }

   if (USB_STDDESC_IFACE_GET_bInterfaceClass(buffer) != USB_CLASS_HUB)
   {
      /* Invalid class ID, must be a HUB. */
      status = USB_STATUS_DRIVER_FAIL;
   }
   else if (USB_STDDESC_IFACE_GET_bNumEndpoints(buffer) != 1)
   {
      /* HUB can only ever have ONE endpoint. */
      status = USB_STATUS_DRIVER_FAIL;
   }
   else
   {
      /* Parse the mandatory endpoint, it must be an interrupt IN one. */
      status = _validate_first_ep(&buffer, &length);
   }
   return status;
}

int usb_hub_assign(
      usb_stack_t*   pstack,
      uint16_t       id,
      const uint8_t* buffer,
      uint8_t        length
)
{
   usb_hub_t* pdev;
   int        new_idx;
   int        ret;
   uint8_t    bLength;

   usb_assert(pstack != NULL);
   usb_assert(buffer != NULL);
   usb_assert(length > 0);

   ret = -1;
   new_idx = _get_free_dev();
   if (new_idx < 0)
   {
      /* No more room for HUBs... */
      ret = -1; /** @TODO: use an actual error code. */
   }
   else
   {
      pdev = &_hub_stack.hubs[new_idx];
      bLength = USB_STDDESC_IFACE_GET_bLength(buffer);

      /* Check the descriptor for the interface's descriptor length. */
      if (bLength != USB_STDDESC_IFACE_SIZE)
      {
         ret = -1;
      }
      /* And make sure the buffer is long enough. */
      else if (length < USB_STDDESC_IFACE_SIZE)
      {
         ret = -1;
      }
      else
      {
         /* Done with parsing, activate device and exit. */
         pdev->pstack   =  pstack;
         pdev->id       =  id;
         _hub_stack.n_hubs++;
         ret = 0;
      }
   }

   /* If there's been an error, de-initialize the device's state and status. */
   if (ret && new_idx >= 0)
   {
      _deinit_dev(new_idx);
   }

   return ret; /** @TODO: use an actual error code. */
}

int usb_hub_remove( usb_stack_t* pstack, uint16_t id )
{
   uint8_t    i;
   int        status;
   usb_hub_t* pdev;

   /* First, search for device in driver's structure. */
   status = 0;
   for (i = 0; i < USB_MAX_HUBS && !status; ++i)
   {
      pdev = &_hub_stack.hubs[i];
      if ((pdev->pstack == pstack) && (pdev->id == id))
      {
         status = 1; /* Found! */
      }
   }
   if (status) /* When found, remove it. */
   {
      status = _deinit_dev(i-1); /* -1 because of for's post-increment. */
   }
   else /* Otherwise, do nothing and return with an error. */
   {
      status = -1;
   }
   return status;
}

int usb_hub_init( void )
{
   uint8_t i;
   for (i = 0; i < USB_MAX_HUBS; ++i)
      _deinit_dev(i);
   _hub_stack.n_hubs = 0;
   return 0;
}

int usb_hub_update( void )
{
   uint8_t i;
   int     status = 0;
   for (i = 0; i < USB_MAX_HUBS && !status; ++i)
   {
      if (!(_hub_stack.hubs[i].status & HUB_STATUS_FREE))
      {
         status = _update_dev(i);
      }
   }
   return status;
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




#if 0
int usb_hub_is_active( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_is_connected( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_bind_to_dev( usb_hub_t* phub, uint8_t port, uint8_t addr )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
   usb_assert(addr < USB_MAX_DEVICES);
   usb_assert(addr > 0); /* Index 0 is reserved for the root hub/device,
                          it cannot be connected to a HUB's port */
}

int usb_hub_unbind( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_poweron( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_poweroff( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_get_bound_addr( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
   /** TODO: some kind of error check here, return negative if port is invalid */
   return phub->ports[port].address;
}

int usb_hub_reset( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
   /** TODO: implement this function, send reset over ctrl xfer (or is it int?) */
   return USB_STATUS_OK;
}
#endif



/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_DRV USB Drivers
 * @{
 * @addtogroup USB_HUB USB HUB
 * @{
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>

#include "usb.h"
#include "usbd.h"
#include "usb_desc.h"
#include "drivers/usb_drivers.h"
#include "drivers/usb_hub.h"


/*==================[internal data declaration]==============================*/

/** @brief HUB driver state function type. */
typedef int (*_state_fn_t)( usb_hub_t* pdev );


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
 *
 * @retval USB_STATUS_OK          Descriptor parsed  correctly  and  information
 *                                stored in HUB.
 * @retval USB_STATUS_XFER_RETRY  Descriptor's length requested  was  lass  than
 *                                the actual  descriptor's  length.   Issue  the
 *                                transfer again,  the  correct  value  will  be
 *                                stored in phub->buffer_len.
 * @return Any other value on error.
 */
static int _parse_hub_desc( usb_hub_t* phub );

/**
 * @brief Parse HUB status.
 *
 * The descriptor must have  been  requested  before  hand  and  stored  in  the
 * device's internal buffer: phub->buffer.
 *
 * @param phub  Pointer to HUB.
 *
 * @retval USB_STATUS_OK  Status parsed correctly and information stored in HUB.
 * @return Any other value on error.
 */
static int _parse_hub_status( usb_hub_t* phub );

/**
 * @brief Parse HUB port status.
 *
 * The descriptor must have  been  requested  before  hand  and  stored  in  the
 * device's internal buffer: phub->buffer.
 *
 * @param phub  Pointer to HUB.
 * @param port  Port index, starting from 0.
 *
 * @retval USB_STATUS_OK  Status parsed correctly and information stored in HUB.
 * @return Any other value on error.
 */
static int _parse_port_status( usb_hub_t* phub, uint8_t port );

/*
 * State functions must follow the usb_hub_state_t enumeration order.
 *
 * Following functions do not assert input parameters, this is because they must
 * not be called directly, rather through _update_dev() so there's no need.
 */
static int _state_idle( usb_hub_t* pdev );
static int _state_desc_get( usb_hub_t* pdev );
static int _state_desc_parse( usb_hub_t* pdev );
static int _state_hub_status_get( usb_hub_t* pdev );
static int _state_hub_status_parse( usb_hub_t* pdev );
static int _state_port_status_get( usb_hub_t* pdev );
static int _state_port_status_parse( usb_hub_t* pdev );
static int _state_running( usb_hub_t* pdev );

/* No assert, sets the REQUEST bit as well. */
static void _next_state( usb_hub_t* pdev, usb_hub_state_t next );

/* HUB control requests. */
static int _ctrl_request( usb_hub_t* pdev, usb_stdreq_t* preq ); /* Notifies both busy and waiting-for-transfer with USB_STATUS_XFER_WAIT. */
static int _ClearHubFeature( usb_hub_t* pdev, usb_hub_featsel_t feature );
static int _ClearPortFeature( usb_hub_t* pdev, uint8_t port, usb_hub_featsel_t feature );
static int _GetHubDescriptor( usb_hub_t* pdev, uint16_t len );
static int _GetHubStatus( usb_hub_t* pdev );
static int _GetPortStatus( usb_hub_t* pdev, uint8_t port );
static int _SetHubDescriptor( usb_hub_t* pdev );
static int _SetHubFeature( usb_hub_t* pdev, usb_hub_featsel_t feature );
static int _SetPortFeature( usb_hub_t* pdev, uint8_t port, usb_hub_featsel_t feature );


/*==================[internal data definition]===============================*/
static usb_hub_stack_t _hub_stack; /* Maybe this shouldn't be static or at least not here like this... */

static _state_fn_t _state_fn[] =
{
   _state_idle,
   _state_desc_get,
   _state_desc_parse,
   _state_hub_status_get,
   _state_hub_status_parse,
   _state_port_status_get,
   _state_port_status_parse,
   _state_running,
};


/*==================[internal functions definition]==========================*/

static int _deinit_dev( uint8_t index )
{
   usb_hub_t* pdev;
   uint8_t    i;
   usb_assert(index < USB_MAX_HUBS);

   pdev = &_hub_stack.hubs[index];
   pdev->status     = USB_HUB_STATUS_FREE;
   pdev->pstack     = NULL;
   pdev->id         = 0xFFFF;
   pdev->state      = USB_HUB_STATE_IDLE;
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
      if (_hub_stack.hubs[i].status & USB_HUB_STATUS_FREE)
      {
         /* If it was found, mark it as not-free and return its index. */
         _hub_stack.hubs[i].status &= ~USB_HUB_STATUS_FREE;
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
   int status;
   usb_hub_t* pdev;
   usb_assert(index < USB_MAX_HUBS);
   pdev = &_hub_stack.hubs[index];
   usb_assert(_state_fn[pdev->state] != NULL);
   return _state_fn[pdev->state](pdev);
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
   int      ret;
   uint16_t wHubChars;
   uint8_t* buff;
   uint8_t  actual_length;
   uint8_t  i;
   uint32_t reg_or = 0x00000000;

   usb_assert(phub != NULL);

   buff = phub->buffer;

   /* 1) Check the descriptor's type. */
   if (USB_HUB_DESC_GET_bDescriptorType(buff) != USB_HUB_DESC_TYPE)
   {
      ret = USB_STATUS_INV_DESC;
   }
   /* 2) Get the number of ports. */
   else if ((phub->n_ports = USB_HUB_DESC_GET_bNbrPorts(buff)) > USB_MAX_HUB_PORTS)
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
      actual_length = USB_HUB_DESC_REAL_SIZE(phub->n_ports);
      if (actual_length > phub->buffer_len)
      {
         /*
          * If the requested size wasn't enough for the  entire  descriptor  and
          * the actual length still fits  inside  the  HUB  buffer,  request  it
          * again.
          */
         if (actual_length <= USB_HUB_BUFF_LEN)
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
         wHubChars = USB_HUB_DESC_GET_wHubCharacteristics(buff);

         /* Test for individual power switching. */
         if (((wHubChars & USB_HUB_LPSM_MASK) >> USB_HUB_LPSM_SHIFT) == USB_HUB_LPSM_INDIV)
         {
            reg_or |=  USB_HUB_STATUS_INDIV_POWER;
         }

         /* Test for compound device. */
         if (wHubChars & USB_HUB_CDI)
         {
            reg_or |=  USB_HUB_STATUS_COMPOUND_DEV;
         }

         /* Test for individual over-current reports. */
         if (((wHubChars & USB_HUB_OCPM_MASK) >> USB_HUB_OCPM_SHIFT) == USB_HUB_OCPM_INDIV)
         {
            reg_or |=  USB_HUB_STATUS_INDIV_OVC;
         }
         else if (((wHubChars & USB_HUB_OCPM_MASK) >> USB_HUB_OCPM_SHIFT) == USB_HUB_OCPM_GLOBAL)
         {
            reg_or |=  USB_HUB_STATUS_GLOBAL_OVC;
         }

         /* Test for port indicators support. */
         if (wHubChars & USB_HUB_PIS)
         {
            reg_or |=  USB_HUB_STATUS_INDICATORS;
         }

         /* 5) Get power-on sequence duration. */
         phub->poweron_t = USB_HUB_DESC_GET_bPwrOn2PwrGood(buff);

         /* 6) Get controller's power requirements. */
         phub->power_req = USB_HUB_DESC_GET_bHubContrCurrent(buff);

#if 0 /** @TODO Request the USB host for power consumption. */
         /* 7) Request HUB's power consumption requirements from Host. */
#endif

         /* 8) Check whether each port is removable. */
         for (i = 0; i < phub->n_ports; ++i)
         {
            if (USB_HUB_DESC_GET_DeviceRemovable(buff, i))
            {
               phub->port_status[i] |= USB_HUB_PORT_REM;
            }
         }

         phub->status |= reg_or;

         ret = USB_STATUS_OK;
      }
   }

   return ret;
}

static int _parse_hub_status( usb_hub_t* phub )
{
   uint8_t* buff;

   usb_assert(phub != NULL);

   buff = phub->buffer;

   if (USB_HUB_STATUS_GET_wHubStatus(buff) & USB_HUB_STATUS_LPS)
   {
      phub->status |=  USB_HUB_STATUS_LPS_GOOD;
   }
   else
   {
      phub->status &= ~USB_HUB_STATUS_LPS_GOOD;
   }

   if (USB_HUB_STATUS_GET_wHubStatus(buff) & USB_HUB_STATUS_OVC)
   {
      phub->status |= ~USB_HUB_STATUS_OVC_ACTIVE;
   }
   else
   {
      phub->status &= ~USB_HUB_STATUS_OVC_ACTIVE;
   }

   return USB_STATUS_OK;
}

static int _parse_port_status( usb_hub_t* phub, uint8_t port )
{
   uint32_t wPortStatus;
   uint32_t port_status;

   usb_assert(phub != NULL);
   usb_assert(port  < phub->n_ports);

   port_status = 0;
   wPortStatus = USB_HUB_PORT_STATUS_GET_wPortStatus(phub->buffer);

   if (wPortStatus & USB_HUB_PORT_STATUS_CCS)
   {
      port_status |= USB_HUB_PORT_CONNECTION;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_PED)
   {
      port_status |= USB_HUB_PORT_ENABLED;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_SUS)
   {
      port_status |= USB_HUB_PORT_SUSPENDED;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_OVC)
   {
      port_status |= USB_HUB_PORT_OVC_ACTIVE;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_RST)
   {
      port_status |= USB_HUB_PORT_RESET_ACTIVE;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_PWR)
   {
      port_status |= USB_HUB_PORT_POWERED_STATE;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_LSP)
   {
      port_status |= USB_HUB_PORT_CONNECTION;
   }

   if (wPortStatus & USB_HUB_PORT_STATUS_HSP)
   {
      port_status |= USB_HUB_PORT_CONNECTION;
   }
   phub->port_status[port] = port_status;

   return USB_STATUS_OK;
}


static int _state_idle( usb_hub_t* pdev )
{
   if (!(pdev->status & USB_HUB_STATUS_FREE))
   {
      /*
       * Since we might need to retry the next request  (because  we  don't  yet
       * know it's actual size), this signals it is the first try.
       */
      pdev->buffer_len = 0;
      _next_state(pdev, USB_HUB_STATE_DESC);
   }
   return USB_STATUS_OK;
}

static int _state_desc_get( usb_hub_t* pdev )
{
   int status;

   /* Request HUB descriptor. */
   if (pdev->buffer_len == 0)
   {
      /* When trying for the first time, request the default size. */
      pdev->buffer_len = USB_HUB_DESC_SIZE;
   }

   status = _GetHubDescriptor(pdev, pdev->buffer_len);

   if (status == USB_STATUS_XFER_RETRY)
   {
      /* Descriptor was longer than expected, retry request. */
      _next_state(pdev, USB_HUB_STATE_DESC);
   }
   else if (status == USB_STATUS_XFER_WAIT)
   {
      /* Do nothing and keep waiting. */
      status = USB_STATUS_OK;
   }
   else if (status != USB_STATUS_OK)
   {
      usb_assert(0); /** @TODO: handle error */
   }
   else /* USB_STATUS_OK */
   {
      /* Advance to next state. */
      _next_state(pdev, USB_HUB_STATE_HUB_STATUS);
   }

   return status;
}

static int _state_desc_parse( usb_hub_t* pdev )
{
   return USB_STATUS_OK;
}

static int _state_hub_status_get( usb_hub_t* pdev )
{
   int          status;

   /* Request HUB status. */
   status = _GetHubStatus(pdev);

   if (status == USB_STATUS_XFER_WAIT)
   {
      /* Do nothing and keep waiting. */
      status = USB_STATUS_OK;
   }
   else if (status != USB_STATUS_OK)
   {
      usb_assert(0); /** @TODO: handle error */
   }
   else /* USB_STATUS_OK */
   {
      /* Start querying ports for their status. */
      pdev->current_port = 0;
      _next_state(pdev, USB_HUB_STATE_PORT_STATUS);
   }
   return status;
}

static int _state_hub_status_parse( usb_hub_t* pdev )
{
   return USB_STATUS_OK;
}

static int _state_port_status_get( usb_hub_t* pdev )
{
   int          status;

   /* Request HUB status. */
   status = _GetPortStatus(pdev, pdev->current_port);

   if (status == USB_STATUS_XFER_WAIT)
   {
      /* Do nothing and keep waiting. */
      status = USB_STATUS_OK;
   }
   else if (status != USB_STATUS_OK)
   {
      usb_assert(0); /** @TODO: handle error */
   }
   else
   {
      if (++pdev->current_port < pdev->n_ports)
      {
         /* Query next port status. */
         _next_state(pdev, USB_HUB_STATE_PORT_STATUS);
      }
      else
      {
         /* Done with all ports, now start powering them up. */
         //_next_state(pdev, USB_HUB_STATE_POWER_UP_REQ);
         _next_state(pdev, USB_HUB_STATE_RUNNING);
      }
   }
   return status;
}

static int _state_port_status_parse( usb_hub_t* pdev )
{
   return USB_STATUS_OK;
}

static int _state_running( usb_hub_t* pdev )
{
   return USB_STATUS_OK;
}

static void _next_state( usb_hub_t* pdev, usb_hub_state_t next )
{
   pdev->status |= USB_HUB_STATUS_REQUEST;
   pdev->state   = next;
}

/*==== Control requests ====*/

static int _ctrl_request( usb_hub_t* pdev, usb_stdreq_t* preq )
{
   int status;

   if (pdev->status & USB_HUB_STATUS_REQUEST)
   {
      status = usb_ctrlirp(pdev->pstack, pdev->id, preq, pdev->buffer);
      if (status == USB_STATUS_OK)
      {
         pdev->status &= ~USB_HUB_STATUS_REQUEST;
         status = USB_STATUS_XFER_WAIT;
      }
      else if (status == USB_STATUS_BUSY)
      {
         status = USB_STATUS_XFER_WAIT;
      }
   }
   else
   {
      /* Waiting for acknowledge... */
      status = usb_irp_status(pdev->pstack, pdev->id, USB_CTRL_PIPE_TOKEN);
   }
   return status;
}

static int _ClearHubFeature( usb_hub_t* pdev, usb_hub_featsel_t feature )
{
   usb_stdreq_t stdreq;
   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_DEV );
   stdreq.bRequest      = USB_HUB_CLASSREQ_CLEAR_FEATURE;
   stdreq.wValue        = feature;
   stdreq.wIndex        = 0;
   stdreq.wLength       = 0;

   return _ctrl_request(pdev, &stdreq);
}

static int _ClearPortFeature( usb_hub_t* pdev, uint8_t port, usb_hub_featsel_t feature )
{
   usb_stdreq_t stdreq;
   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_OTHER );
   stdreq.bRequest      = USB_HUB_CLASSREQ_CLEAR_FEATURE;
   stdreq.wValue        = feature;
   stdreq.wIndex        = port;
   stdreq.wLength       = 0;

   return _ctrl_request(pdev, &stdreq);
}

static int _GetHubDescriptor( usb_hub_t* pdev, uint16_t len )
{
   int          status;
   usb_stdreq_t stdreq;
   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_DEV );
   stdreq.bRequest      = USB_HUB_CLASSREQ_GET_DESCRIPTOR;
   stdreq.wValue        = (USB_HUB_DESC_TYPE << 8);
   stdreq.wIndex        = 0; /* ... or language ID? */
   stdreq.wLength       = len;

   status = _ctrl_request(pdev, &stdreq);
   if (status == USB_STATUS_OK)
   {
      status = _parse_hub_desc(pdev);
   }
   return status;
}

static int _GetHubStatus( usb_hub_t* pdev )
{
   int          status;
   usb_stdreq_t stdreq;

   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_DEV );
   stdreq.bRequest      = USB_HUB_CLASSREQ_GET_STATUS;
   stdreq.wValue        = 0;
   stdreq.wIndex        = 0;
   stdreq.wLength       = USB_HUB_STATUS_LENGTH;

   status = _ctrl_request(pdev, &stdreq);
   if (status == USB_STATUS_OK)
   {
      status = _parse_hub_status(pdev);
   }
   return status;
}

static int _GetPortStatus( usb_hub_t* pdev, uint8_t port )
{
   int          status;
   usb_stdreq_t stdreq;

   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_IN,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_OTHER );
   stdreq.bRequest      = USB_HUB_CLASSREQ_GET_STATUS;
   stdreq.wValue        = 0;
   stdreq.wIndex        = port+1;
   stdreq.wLength       = USB_HUB_PORT_STATUS_LENGTH;

   status = _ctrl_request(pdev, &stdreq);
   if (status == USB_STATUS_OK)
   {
      status = _parse_port_status(pdev, pdev->current_port);
   }
   return status;
}

/* Put new descriptor into pdev->buffer. */
static int _SetHubDescriptor( usb_hub_t* pdev )
{
   int          status;
   usb_stdreq_t stdreq;
   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_DEV );
   stdreq.bRequest      = USB_HUB_CLASSREQ_SET_DESCRIPTOR;
   stdreq.wValue        = (USB_HUB_DESC_TYPE << 8);
   stdreq.wIndex        = 0;
   stdreq.wLength       = pdev->buffer_len;

   return _ctrl_request(pdev, &stdreq);
}

static int _SetHubFeature( usb_hub_t* pdev, usb_hub_featsel_t feature )
{
   int          status;
   usb_stdreq_t stdreq;
   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_DEV );
   stdreq.bRequest      = USB_HUB_CLASSREQ_SET_FEATURE;
   stdreq.wValue        = feature;
   stdreq.wIndex        = 0;
   stdreq.wLength       = 0;

   return _ctrl_request(pdev, &stdreq);
}

static int _SetPortFeature( usb_hub_t* pdev, uint8_t port, usb_hub_featsel_t feature )
{
   int          status;
   usb_stdreq_t stdreq;
   stdreq.bmRequestType = USB_STDREQ_REQTYPE(
         USB_DIR_OUT,
         USB_STDREQ_TYPE_CLASS,
         USB_STDREQ_RECIP_OTHER );
   stdreq.bRequest      = USB_HUB_CLASSREQ_SET_FEATURE;
   stdreq.wValue        = feature;
   stdreq.wIndex        = port;
   stdreq.wLength       = 0;

   return _ctrl_request(pdev, &stdreq);
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
      if (!(_hub_stack.hubs[i].status & USB_HUB_STATUS_FREE))
      {
         status = _update_dev(i);
      }
   }
   return status;
}


usb_speed_t usb_hub_get_speed( uint8_t hub_idx, uint8_t port )
{
   uint32_t    port_status;
   usb_speed_t speed;

   usb_assert(hub_idx < USB_MAX_HUBS);
   usb_assert(  port  < USB_MAX_HUB_PORTS);

   port_status = _hub_stack.hubs[hub_idx].port_status[port];
   if ((port_status & USB_HUB_PORT_LOWSPEED) &&
         !(port_status & USB_HUB_PORT_HIGHSPEED))
   {
      speed = USB_SPD_LS;
   }
   else if (!(port_status & USB_HUB_PORT_LOWSPEED) &&
         (port_status & USB_HUB_PORT_HIGHSPEED))
   {
      speed = USB_SPD_HS;
   }
   else if (!(port_status & USB_HUB_PORT_LOWSPEED) &&
         !(port_status & USB_HUB_PORT_HIGHSPEED))
   {
      speed = USB_SPD_FS;
   }
   else
   {
      speed = USB_SPD_INV;
   }
   return speed;
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

/** @} USB_HUB */
/** @} USB_DRV */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


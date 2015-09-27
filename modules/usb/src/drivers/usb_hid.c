
/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "usb.h"
#include "usbd.h"
#include "usb_desc.h"
#include "drivers/usb_hid.h"


/*==================[internal data declaration]==============================*/
static hid_stack_t _hid_stack;


/*==================[internal functions declaration]=========================*/

/**
 * @brief De-initialize device.
 * @return Non-zero on error.
 */
static int _deinit_dev( uint8_t index );

/**
 * @brief Get next free HID index and mark it as not-free if found.
 * @return Negative on error, HID device's internal index otherwise.
 */
static int16_t _get_free_dev( void );

/**
 * @brief Update HID state and status.
 * @return Non-zero on error... @TODO complete this!
 */
static int _update_dev( uint8_t index );

/**
 * @brief Open specified HID device.
 *
 * Pretty much the same as the POSIX upper level implementation but with the type
 * of HID device already identified.
 *
 * @param str_num   Two-digit device index, as "01" in /dev/keyboard01.
 * @param flags     Same as POSIX's _open() flags parameter.
 * @param protocol  Target protocol, as HID_PROTO_KEYB in previous example.
 *
 * @return Negative on error, HID device's internal index otherwise.
 */
static int _open_dev( const char* str_num, int flags, hid_protocol_t protocol );

/**
 * @brief Validate first endpoint in interface's descriptor.
 *
 * Buffer should be pointed to somewhere \b before  the  endpoint's  descriptor,
 * it will be advanced to it on entry.
 * Remember that this endpoint must be an interrupt IN one.
 */
static int _validate_first_ep( const uint8_t** pbuffer, uint8_t* plen );

/**
 * @brief Validate optional endpoint in interface's descriptor.
 *
 * Buffer should be pointed to somewhere \b before  the  endpoint's  descriptor,
 * it will be advanced to it on entry.
 * Remember that this endpoint must be an interrupt OUT one.
 */
static int _validate_optional_ep( const uint8_t** pbuffer, uint8_t* plen );


/*==================[internal functions definition]==========================*/

static int _deinit_dev( uint8_t index )
{
   hid_dev_t* pdev;
   usb_assert(index < HID_MAX_DEVICES);

   pdev = &_hid_stack.devices[index];
   pdev->status     = HID_STATUS_FREE | HID_STATUS_ENTRY;
   pdev->pstack     = NULL;
   pdev->id         = 0xFFFF;
   pdev->protocol   = HID_PROTO_NONE;
   pdev->state      = HID_STATE_IDLE;
   pdev->report_len = 0;

   return 0;
}

static int16_t _get_free_dev( void )
{
   int16_t i;
   for (i = 0; i < HID_MAX_DEVICES; ++i)
   {
      /* Iterate until a free device is found. */
      if (_hid_stack.devices[i].status & HID_STATUS_FREE)
      {
         /* If it was found, mark it as not-free and return its index. */
         _hid_stack.devices[i].status &= ~HID_STATUS_FREE;
         break;
      }
   }
   if (i >= HID_MAX_DEVICES)
   {
      /* No device was free, return with an error. */
      i = -1;
   }
   return i;
}

static int _update_dev( uint8_t index )
{
   usb_stdreq_t stdreq;
   hid_dev_t*   pdev;
   int          status = USB_STATUS_OK;

   usb_assert(index < HID_MAX_DEVICES);
   pdev = &_hid_stack.devices[index];

   switch (pdev->state)
   {
      case HID_STATE_IDLE:
         /*
          * Do nothing and wait for an  interface  to  be  assigned  to  this
          * HID device. The state will be changed by the hid_assign() method.
          */
         if (!(pdev->status & HID_STATUS_FREE))
         {
            //pdev->state   = HID_STATE_SET_IDLE;
            pdev->state   = HID_STATE_INIT;
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
            {
               /* This isn't an error, so notify update as OK. */
               status = USB_STATUS_OK;
               break; /* Try again later... */
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }
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
            {
               break; /* Keep waiting... */
            }
            if (status == USB_STATUS_EP_STALLED)
            {
               /* If endpoint was stalled, try again. */
               pdev->status |= HID_STATUS_ENTRY;
               break;
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }

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
            {
               break; /* Try again later... */
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }
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
            {
               break;
            }
            if (status == USB_STATUS_EP_STALLED)
            {
               /* If endpoint was stalled, try again. */
               pdev->status |= HID_STATUS_ENTRY;
               break;
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }

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
            {
               break; /* Try again later... */
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }
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
            {
               break;
            }
            if (status != USB_STATUS_OK)
            {
               usb_assert(0); /** @TODO: handle error */
            }

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
         break;
   }

   return status;
}

static int _open_dev( const char* str_num, int flags, hid_protocol_t protocol )
{
   long       temp;
   int        ret;
   char*      pchar;
   uint8_t    target_num;
   uint8_t    current_num;
   uint8_t    index;
   hid_dev_t* pdev;

   usb_assert(str_num != NULL);

   /* First, obtain the devices intended index from the base filename. */
   temp = strtol(str_num, &pchar, 10);
   if (str_num == pchar || pchar > str_num+2 || *pchar != '\0')
   {
      ret = -1; /** @TODO: use an actual error code. */
   }
   else if (temp > HID_MAX_DEVICES)
   {
      ret = -1; /** @TODO: use an actual error code. */
   }
   else
   {
      /* Now search for a valid device with the specified index and protocol. */
      target_num = (uint8_t) temp;

      ret = 0; /* This will be temporarily used to terminate the loop. */
      current_num = 0;
      for (index = 0; index < HID_MAX_DEVICES && !ret; ++index)
      {
         pdev = &_hid_stack.devices[index];
         if (!(pdev->status & HID_STATUS_FREE) && (pdev->protocol == protocol))
         {
            if (current_num++ == target_num)
            {
               ret = 1;
            }
         }
      }
      if (!ret)
      {
         /* If ret is still 0, then no matching device was found. */
         ret = -1; /** @TODO: use an actual error code. */
      }
      else if (pdev->status & HID_STATUS_OPEN)
      {
         /* Device is already open. */
         ret = -1; /** @TODO: use an actual error code. */
      }
      else if (!(pdev->status & HID_STATUS_INIT))
      {
         /* Device is still in the initialization process. */
         ret = -1; /** @TODO: use an actual error code. */
      }
      else
      {
         /* Matching device was found, mark it as open and return its index. */
         pdev->status |= HID_STATUS_OPEN;
         ret = index-1; /* -1 because of for loop's post increment. */
      }
   }
   return ret;
}

static int _validate_first_ep(const uint8_t** pbuffer, uint8_t* plen )
{
   int            status;

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

static int _validate_optional_ep(const uint8_t** pbuffer, uint8_t* plen )
{
   int            status;

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
      if (USB_STDDESC_EP_GET_bEndpointAddress(*pbuffer) & USB_DIR_MASK)
      {
         /* Direction wasn't OUT. */
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


/*==================[external functions definition]==========================*/

int hid_probe( const uint8_t* buffer, uint8_t length )
{
/**
 * @FIXME an HID device can actually have  a  second  (optional)  interrupt  OUT
 * pipe to use by the host instead of the default control one... fix the  driver
 * to include this. This will also affect the initialization routine because the
 * report requesting will be done through this new pipe.
 */

   int     status;
   uint8_t numep;

   usb_assert(buffer != NULL);

   /*
    * First, validate the input parameters. Buffer can't be NULL but length  has
    * to be long enough to hold, at  least,  the  interface,  HID  and  endpoint
    * descriptors. This doesn't account for HIDs with the optional ep. though.
    */
   if (length < (USB_STDDESC_IFACE_SIZE
            + USB_STDDESC_EP_SIZE
            + HID_DESC_HID_SIZE) )
   {
      return USB_STATUS_DRIVER_FAIL;
   }

   numep = USB_STDDESC_IFACE_GET_bNumEndpoints(buffer);

   if (USB_STDDESC_IFACE_GET_bInterfaceClass(buffer) != USB_CLASS_HID)
   {
      /* Invalid class ID, must be an HID. */
      status = USB_STATUS_DRIVER_FAIL;
   }
   else if (numep > 2 || numep < 1)
   {
      /* An HID must have at least one endpoint and cannot have more than two.*/
      status = USB_STATUS_DRIVER_FAIL;
   }
   else
   {
      /* Parse the mandatory endpoint, it must be an interrupt IN one. */
      status = _validate_first_ep(&buffer, &length);
      if (status == USB_STATUS_OK && numep == 2)
      {
         /* Parse the optional endpoint, it must be an interrupt OUT one.*/
         status = _validate_optional_ep(&buffer, &length);
      }

   }
   return status;
}

int hid_assign(
      usb_stack_t*   pstack,
      uint16_t       id,
      const uint8_t* buffer,
      uint8_t        length
)
{
   int               new_idx;
   int               ret;
   hid_dev_t*        pdev;
   uint8_t           bLength;

   usb_assert(pstack != NULL);
   usb_assert(buffer != NULL);
   usb_assert(length > 0);

   ret = 0;
   new_idx = _get_free_dev();
   if (new_idx < 0)
   {
      /* No more room for HID devices... */
      ret = -1; /** @TODO: use an actual error code. */
   }

   if (!ret)
   {
      pdev = &_hid_stack.devices[new_idx];
      bLength = USB_STDDESC_IFACE_GET_bLength(buffer);

      /* 1) Check the buffer for the interface's descriptor length. */
      if (bLength != USB_STDDESC_IFACE_SIZE)
      {
         ret = -1;
      }
      else if (length < USB_STDDESC_IFACE_SIZE)
      {
         ret = -1;
      }
   }

   if (!ret)
   {
      /* 2) Get the protocol information (we already know it's an HID dev.). */
      pdev->protocol = USB_STDDESC_IFACE_GET_bInterfaceProtocol(buffer);

      /* 3) Find out whether it uses an interrupt OUT endpoint for control. */
      if (USB_STDDESC_IFACE_GET_bNumEndpoints(buffer) == 2)
      {
         pdev->status |= HID_STATUS_INTOUT;
      }
      else
      {
         pdev->status &= ~HID_STATUS_INTOUT;
      }

      /* 4) Right after the interface descriptor should be the HID one. */
      buffer += bLength;
      length -= bLength;
      bLength = HID_DESC_HID_GET_bLength(buffer);
      if (bLength < HID_DESC_HID_SIZE)
      {
         /* Validate descriptor's minimum length. */
         ret = -1;
      }
      else if (length < bLength)
      {
         /* Validate buffer's minimum length. */
         ret = -1;
      }
      /* 5) Check the number of descriptors entry, must be at least 1. */
      else if (HID_DESC_HID_GET_bNumDescriptors(buffer) < 1)
      {
         ret = -1;
      }
      /* 6) Check class descriptor and get country code. */
      else if (HID_DESC_HID_GET_bClassDescriptorType(buffer) != HID_DESC_TYPE_REPORT)
      {
         ret = -1;
      }
   }

   if (!ret)
   {
      pdev->ctry_code = HID_DESC_HID_GET_bCountryCode(buffer);

      /* 7) Get report's length, supported up to HID_BUFF_LEN bytes long ATM. */
      pdev->report_len = HID_DESC_HID_GET_wDescriptorLength(buffer);
      if (pdev->report_len > HID_BUFF_LEN)
      {
         ret = -1;
      }
      else
      {
         /**
          * @TODO there are  optional  class  descriptors  after  the  mandatory
          * report one, this should be read and dealt with... fix this!
          */

         /*
          * After  the  HID  desc.,  there  should  be  one  [or  two]  endpoint
          * descriptors, however, this has already been checked by  the  probing
          * method.
          *
          * Done with parsing, activate device and exit. The 
          */
         pdev->pstack   =  pstack;
         pdev->id       =  id;
         _hid_stack.n_devices++;
      }
   }

   /* If there's been an error, de-initialize the device's state and status. */
   if (ret && new_idx >= 0)
   {
      _deinit_dev(new_idx);
   }
   return ret; /** @TODO: use an actual error code. */
}

int hid_remove( usb_stack_t* pstack, uint16_t id )
{
   uint8_t    i;
   int        status;
   hid_dev_t* pdev;

   /* First, search for device in driver's structure. */
   status = 0;
   for (i = 0; i < HID_MAX_DEVICES && !status; ++i)
   {
      pdev = &_hid_stack.devices[i];
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

int hid_init( void )
{
   uint8_t i;
   for (i = 0; i < HID_MAX_DEVICES; ++i)
      _deinit_dev(i);
   _hid_stack.n_devices = 0;
   return 0;
}

int hid_update( void )
{
   uint8_t i;
   int     status = 0;
   for (i = 0; i < HID_MAX_DEVICES && !status; ++i)
   {
      if (!(_hid_stack.devices[i].status & HID_STATUS_FREE))
      {
         status = _update_dev(i);
      }
   }
   return status;
}

/* POSIX */

int hid_open(const char *pathname, int flags)
{
   int            ret;
   uint8_t        offset;
   hid_protocol_t proto;

   /** @TODO
    * Choose what pahtnames will represent HID devices, for now:
    * 'pathname' is expected to be either "/mouseXX" or "/keybXX".
    */
   /* Validate input parameters. */
   if (pathname == NULL)
   {
      return -1; /** @TODO: use an actual error code. */
   }

   /*
    * Everything below this point is just temporary [and not very well done]  to
    * test the HID stack.
    */

   ret = -1;
   if (!strncmp(pathname, "/mouse", 6)) /* Open mouse XX */
   {
      offset = 6;
      proto  = HID_PROTO_MOUSE;
      ret = 0;
   }
   else if (!strncmp(pathname, "/keyb", 5)) /* Open keyboard XX */
   {
      offset = 5;
      proto  = HID_PROTO_KEYB;
      ret = 0;
   }

   if (!ret)
   {
      ret = _open_dev(pathname+offset, flags, proto);
   }
   return ret;
}

int hid_close(int fd)
{
   int     ret;
   uint8_t index;

   /* Validate input parameters. */
   if (fd < 0 || fd > HID_MAX_DEVICES-1)
   {
      return -1; /** @TODO: use an actual error code. */
   }

   index = (uint8_t) fd;
   ret = -1;
   if (!(_hid_stack.devices[index].status & HID_STATUS_FREE) &&
        (_hid_stack.devices[index].status & HID_STATUS_OPEN) )
   {
      /* If device has an interface attached and has been opened, close it. */
      _hid_stack.devices[index].status &= ~HID_STATUS_OPEN;
      /** @TODO finish this procedure, probably call _deinit_dev()... */
      ret = 0;
   }
   return ret;
}

size_t hid_read(int fd, void *buf, size_t count)
{
   hid_dev_t* pdev;
   int        status;

   /* Validate input parameters. */
   if (  buf == NULL ||
         fd < 0 ||
         fd > HID_MAX_DEVICES-1 )
   {
      return -1;
   }

   if (count > 0)
   {
      pdev = &_hid_stack.devices[fd];
      if (!(status = usb_irp(pdev->pstack, pdev->id, 0, (uint8_t*) buf, count)))
      {
         /* Wait for transfer to complete. */
         do
         {
            status = usb_irp_status(pdev->pstack, pdev->id, 0);
         } while (status == USB_STATUS_XFER_WAIT);
      }
   }
   else
   {
      /* If count is 0 then there's nothing to do, just return. */
      status = USB_STATUS_OK;
   }
   /** @TODO should return number of bytes actually transfered. */
   return (status == USB_STATUS_OK) ? count : -1;
}

int hid_write(int fd, const void *buf, size_t count)
{
   if (  buf == NULL ||
         fd < 0 ||
         fd > HID_MAX_DEVICES-1 )
   {
      return -1;
   }

   if (count == 0)
      return 0; /* Check this! */
   return 0;
}


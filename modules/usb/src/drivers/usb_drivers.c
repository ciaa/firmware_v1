#include <stdio.h>
#include <stdint.h>
#include "usb.h"
#include "usbd.h"
#include "drivers/usb_drivers.h"

#if (USB_MAX_HUBS > 0)
#include "drivers/usb_hub.h"
#endif

#include "drivers/usb_hid.h"

static int _usb_drivers_check_ids( const usb_device_t* pdevice, uint8_t index );

static const usb_driver_t usb_drivers[USB_MAX_DRIVERS] =
{
#if (USB_MAX_HUBS > 0)
   {   /* HUB */
      USB_FORCE_PROBING_ID,
      USB_FORCE_PROBING_ID,
      hub_probe,
      hub_assign,
      hub_remove
   },
#endif
   {   /* HID */
      USB_FORCE_PROBING_ID,
      USB_FORCE_PROBING_ID,
      hid_probe,
      hid_assign,
      hid_remove
   },
};

int usb_drivers_probe(
      const usb_device_t* pdevice,
      usb_driver_handle_t offset,
      const uint8_t*      buffer,
      uint8_t             length
)
{
   int ret;
   usb_driver_handle_t i;

   usb_assert(pdevice != NULL);
   usb_assert(buffer != NULL);
   usb_assert(length > 0);

   ret = -1;
   if (offset < USB_MAX_DRIVERS)
   {
      for (i = offset; i < USB_MAX_DRIVERS && ret == -1; ++i)
      {
         /*
          * First, check if there's a driver assigned to the device's vendor and
          * product IDs, in which case no probing needs to be done and  we  will
          * assume the driver can handle whatever device has been attached.
          */
         if (_usb_drivers_check_ids(pdevice, i))
         {
            ret = i;
         }
         else if (usb_drivers[i].probe != NULL)
         {
            /* Otherwise, call the probing function. */
            if (!usb_drivers[i].probe(buffer, length))
               ret = i; /* Driver found! */
         }
      }
   }
   return ret;
}

int usb_drivers_assign(
      usb_stack_t*        pstack,
      uint16_t            id,
      const uint8_t*      buffer,
      uint8_t             length,
      usb_driver_handle_t handle
)
{
   int status = USB_STATUS_DRIVER_NA;

   usb_assert(pstack != NULL);
   usb_assert(USB_ID_TO_DEV(id) < USB_MAX_DEVICES);
   usb_assert(USB_ID_TO_IFACE(id) < USB_MAX_INTERFACES);
   usb_assert(handle < USB_MAX_DRIVERS);

   if (usb_drivers[handle].assign != NULL)
      status = usb_drivers[handle].assign(pstack, id, buffer, length);
   return status;
}

int usb_drivers_remove(
      usb_stack_t*        pstack,
      uint16_t            id,
      usb_driver_handle_t handle
)
{
   int status = USB_STATUS_DRIVER_NA;

   usb_assert(pstack != NULL);
   usb_assert(USB_ID_TO_DEV(id) < USB_MAX_DEVICES);
   usb_assert(USB_ID_TO_IFACE(id) < USB_MAX_INTERFACES);
   usb_assert(handle < USB_MAX_DRIVERS);

   if (usb_drivers[handle].assign != NULL)
      status = usb_drivers[handle].remove(pstack, id);
   return status;
}


/******************************************************************************/
/**
 * @brief Validate device's product and vendor ID against those targeted by  the
 * driver.  If either target is USB_FORCE_PROBING_ID, then the corresponding  ID
 * will not be validated and will pass regardless of its actual value.
 *
 * @param pdevice Pointer to USB device.
 * @param index   Index of HID device in internal structure.
 */
static int _usb_drivers_check_ids( const usb_device_t* pdevice, uint8_t index )
{
   uint8_t vid = 0;
   uint8_t pid = 0;

   usb_assert(pdevice != NULL);
   usb_assert(index < USB_MAX_DRIVERS);

   if (usb_drivers[index].vendor_ID  == pdevice->vendor_ID)
      vid = 1;
   if (usb_drivers[index].product_ID == pdevice->product_ID)
      pid = 1;
   if (usb_drivers[index].vendor_ID  == USB_FORCE_PROBING_ID)
      vid = 0;
   if (usb_drivers[index].product_ID == USB_FORCE_PROBING_ID)
      vid = 0;
   return (vid | pid);
}


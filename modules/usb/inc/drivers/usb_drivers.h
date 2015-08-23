#ifndef USB_DRIVERS_H
#define USB_DRIVERS_H
/**
* @addtogroup USB_DRIVERS
* @brief CIAA USB drivers access methods
*
* @{ */


#ifdef __cplusplus
extern "C" {
#endif


/* Inclusions */
#include <stdint.h>
#include "usb.h"


/**
 * @brief Driver identifier.
 *
 * This enumeration list \b must correspond to the local static drivers array in
 * usb_drivers.c, if one changes the other \b must be updated accordingly.
 *
 * Order here and in the aforementioned array will determine driver's  priority.
 * If a specific driver should be found  before  another,  place  it  above  the
 * latter one.  For instance, if a custom HID device with extra  functionalities
 * needs to be assigned a custom driver based on its  vendor  and  product  IDs,
 * place it above the USB_DRIVER_HID one.
 */
enum usb_driver_type_t
{
#if (USB_MAX_HUBS > 0)
   USB_DRIVER_HUB,   /**< HUB driver */
#endif
   USB_DRIVER_HID,   /**< HID driver */

   USB_DRIVER_LAST   /* For array size, ignore this. */
};

/**
 * @brief Total number of indexed drivers.
 *
 * This is given by the number of entries in the local static drivers array, the
 * user must not access this array directly unless a new driver is being added.
 * @see usb_driver_type_t
 */
#define USB_MAX_DRIVERS  USB_DRIVER_LAST


/**
 * @brief Probe all available drivers against the given interface.
 *
 * The interface should be passed on to this function through buffer, it  should
 * be pointed to the beginning of the interface descriptor. Its length should be
 * up to the end of  the  current  interface,  following  interface  descriptors
 * shouldn't be factored into the count.
 *
 * @param pdevice Pointer to the device owner of  the  interface  beign  probed,
 *                this will be tested  against  the  driver's  specified  vendor
 *                and/or product IDs.
 * @param offset  Driver offset, in case the probing should  start  this  number
 *                of drivers after the first in the array.
 * @param buffer  Buffer containing the entire interface descriptor.
 * @param length  Buffer's length.
 */
int usb_drivers_probe(
      const usb_device_t* pdevice,
      usb_driver_handle_t offset,
      const uint8_t*      buffer,
      uint8_t             length
);


int usb_drivers_assign(
      usb_stack_t*        pstack,
      uint16_t            id,
      const uint8_t*      buffer,
      uint8_t             length,
      usb_driver_handle_t handle
);


int usb_drivers_remove(
      usb_stack_t*        pstack,
      uint16_t            id,
      usb_driver_handle_t handle
);


#ifdef __cplusplus
}
#endif

/**  @} USB_DRIVERS */
#endif /* USB_DRIVERS_H */

#ifndef USB_DESC_H
#define USB_DESC_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_DESC Standard Descriptors
 * @brief USB standard descriptors header.
 *
 * This file provides macros for accessing data in USB standard descriptors plus
 * a method to advance the buffer up to a given descriptor type.
 *
 * @{
 */


/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "usb_std.h"


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif



/*==================[macros]=================================================*/

/**
 * @name USB device descriptor
 * @{
 */

/** @brief USB device descriptor's length. */
#define USB_STDDESC_DEV_SIZE   18

/** @brief Get USB device descriptor's bLength from buffer x */
#define USB_STDDESC_DEV_GET_bLength(x) \
   ((uint8_t ) (x)[0])
/** @brief Get USB device descriptor's bDescriptorType from buffer x */
#define USB_STDDESC_DEV_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
/** @brief Get USB device descriptor's bcdUSB from buffer x */
#define USB_STDDESC_DEV_GET_bcdUSB(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[3] << 8) | (x)[2]))
/** @brief Get USB device descriptor's bDeviceClass from buffer x */
#define USB_STDDESC_DEV_GET_bDeviceClass(x) \
   ((uint8_t ) (x)[4])
/** @brief Get USB device descriptor's bDeviceSubClass from buffer x */
#define USB_STDDESC_DEV_GET_bDeviceSubClass(x) \
   ((uint8_t ) (x)[5])
/** @brief Get USB device descriptor's bDeviceProtocol from buffer x */
#define USB_STDDESC_DEV_GET_bDeviceProtocol(x) \
   ((uint8_t ) (x)[6])
/** @brief Get USB device descriptor's bMaxPacketSize0 from buffer x */
#define USB_STDDESC_DEV_GET_bMaxPacketSize0(x) \
   ((uint8_t ) (x)[7])
/** @brief Get USB device descriptor's idVendor from buffer x */
#define USB_STDDESC_DEV_GET_idVendor(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[9] << 8) | (x)[8]))
/** @brief Get USB device descriptor's idProduct from buffer x */
#define USB_STDDESC_DEV_GET_idProduct(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[11] << 8) | (x)[10]))
/** @brief Get USB device descriptor's bcdDevice from buffer x */
#define USB_STDDESC_DEV_GET_bcdDevice(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[13] << 8) | (x)[12]))
/** @brief Get USB device descriptor's iManufacturer from buffer x */
#define USB_STDDESC_DEV_GET_iManufacturer(x) \
   ((uint8_t ) (x)[14])
/** @brief Get USB device descriptor's iProduct from buffer x */
#define USB_STDDESC_DEV_GET_iProduct(x) \
   ((uint8_t ) (x)[15])
/** @brief Get USB device descriptor's iSerialNumber from buffer x */
#define USB_STDDESC_DEV_GET_iSerialNumber(x) \
   ((uint8_t ) (x)[16])
/** @brief Get USB device descriptor's bNumConfigurations from buffer x */
#define USB_STDDESC_DEV_GET_bNumConfigurations(x) \
   ((uint8_t ) (x)[17])

/** @} USB device descriptor */


/**
 * @name USB configuration descriptor
 * @{
 */

/** @brief USB configuration descriptor's length. */
#define USB_STDDESC_CFG_SIZE    9

/** @brief Get USB configuration descriptor's bLength from buffer x. */
#define USB_STDDESC_CFG_GET_bLength(x) \
   ((uint8_t ) (x)[0])
/** @brief Get USB configuration descriptor's bDescriptorType from buffer x. */
#define USB_STDDESC_CFG_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
/** @brief Get USB configuration descriptor's wTotalLength from buffer x. */
#define USB_STDDESC_CFG_GET_wTotalLength(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[3] << 8) | (x)[2]))
/** @brief Get USB configuration descriptor's bNumInterfaces from buffer x. */
#define USB_STDDESC_CFG_GET_bNumInterfaces(x) \
   ((uint8_t ) (x)[4])
/** @brief Get USB configuration descriptor's bConfigurationValue from buffer x. */
#define USB_STDDESC_CFG_GET_bConfigurationValue(x) \
   ((uint8_t ) (x)[5])
/** @brief Get USB configuration descriptor's iConfiguration from buffer x. */
#define USB_STDDESC_CFG_GET_iConfiguration(x) \
   ((uint8_t ) (x)[6])
/** @brief Get USB configuration descriptor's bmAttributes from buffer x. */
#define USB_STDDESC_CFG_GET_bmAttributes(x) \
   ((uint8_t ) (x)[7])
/** @brief Get USB configuration descriptor's bMaxPower from buffer x. */
#define USB_STDDESC_CFG_GET_bMaxPower(x) \
   ((uint8_t ) (x)[8])

/** @brief bmAttributes self-powered device bit. */
#define USB_STDDESC_CFG_SELF_POWERED  (1 << 6)
/** @brief bmAttributes remote wakeup supported bit. */
#define USB_STDDESC_CFG_REMOTE_WAKEUP (1 << 5)

/** @} USB configuration descriptor */


/**
 * @name USB interface descriptor
 * @{
 */

/** @brief USB interface descriptor's length. */
#define USB_STDDESC_IFACE_SIZE  9

/** @brief Get USB interface descriptor's bLength from buffer x. */
#define USB_STDDESC_IFACE_GET_bLength(x) \
   ((uint8_t ) (x)[0])
/** @brief Get USB interface descriptor's bDescriptorType from buffer x. */
#define USB_STDDESC_IFACE_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
/** @brief Get USB interface descriptor's bInterfaceNumber from buffer x. */
#define USB_STDDESC_IFACE_GET_bInterfaceNumber(x) \
   ((uint8_t ) (x)[2])
/** @brief Get USB interface descriptor's bAlternateSetting from buffer x. */
#define USB_STDDESC_IFACE_GET_bAlternateSetting(x) \
   ((uint8_t ) (x)[3])
/** @brief Get USB interface descriptor's bNumEndpoints from buffer x. */
#define USB_STDDESC_IFACE_GET_bNumEndpoints(x) \
   ((uint8_t ) (x)[4])
/** @brief Get USB interface descriptor's bInterfaceClass from buffer x. */
#define USB_STDDESC_IFACE_GET_bInterfaceClass(x) \
   ((uint8_t ) (x)[5])
/** @brief Get USB interface descriptor's bInterfaceSubClass from buffer x. */
#define USB_STDDESC_IFACE_GET_bInterfaceSubClass(x) \
   ((uint8_t ) (x)[6])
/** @brief Get USB interface descriptor's bInterfaceProtocol from buffer x. */
#define USB_STDDESC_IFACE_GET_bInterfaceProtocol(x) \
   ((uint8_t ) (x)[7])
/** @brief Get USB interface descriptor's iInterface from buffer x. */
#define USB_STDDESC_IFACE_GET_iInterface(x) \
   ((uint8_t ) (x)[8])

/** @} USB interface descriptor */


/**
 * @name USB endpoint descriptor
 * @{
 */

/** @brief USB endpoint descriptor's length. */
#define USB_STDDESC_EP_SIZE            7
/** @brief USB endpoint descriptor's bmAttributes endpoint type mask. */
#define USB_STDDESC_EP_ATTR_TYPE_MASK  0x03

/** @brief Get USB endpoint descriptor's bLength from buffer x. */
#define USB_STDDESC_EP_GET_bLength(x) \
   ((uint8_t ) (x)[0])
/** @brief Get USB endpoint descriptor's bDescriptorType from buffer x. */
#define USB_STDDESC_EP_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
/** @brief Get USB endpoint descriptor's bEndpointAddress from buffer x. */
#define USB_STDDESC_EP_GET_bEndpointAddress(x) \
   ((uint8_t ) (x)[2])
/** @brief Get USB endpoint descriptor's bmAttributes from buffer x. */
#define USB_STDDESC_EP_GET_bmAttributes(x) \
   ((uint8_t ) (x)[3])
/** @brief Get USB endpoint descriptor's wMaxPacketSize from buffer x. */
#define USB_STDDESC_EP_GET_wMaxPacketSize(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[5] << 8) | (x)[4]))
/** @brief Get USB endpoint descriptor's bInterval from buffer x. */
#define USB_STDDESC_EP_GET_bInterval(x) \
   ((uint8_t ) (x)[6])

/** @} USB endpoint descriptor */


/*==================[typedef]================================================*/

/**
 * @name USB device descriptor
 * @{
 */
/**
 * @brief Standard USB device descriptor.
 *
 * Structure fields have been reordered to account for byte alignment.
 */
typedef struct _usb_desc_dev_t
{
   uint16_t bcdUSB;
   uint16_t idVendor;
   uint16_t idProduct;
   uint16_t bcdDevice;
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint8_t  bDeviceClass;
   uint8_t  bDeviceSubClass;
   uint8_t  bDeviceProtocol;
   uint8_t  bMaxPacketSize0;
   uint8_t  iManufacturer;
   uint8_t  iProduct;
   uint8_t  iSerialNumber;
   uint8_t  bNumConfigurations;
} usb_desc_dev_t;
/** @} USB device descriptor */

/**
 * @name USB configuration descriptor
 * @{
 */
/**
 * @brief Standard USB configuration descriptor.
 *
 * Structure fields have been reordered to account for byte alignment.
 */
typedef struct _usb_desc_cfg_t
{
   uint16_t wTotalLength;
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint8_t  bNumInterfaces;
   uint8_t  bConfigurationValue;
   uint8_t  iConfiguration;
   uint8_t  bmAttributes;
   uint8_t  bMaxPower;
} usb_desc_cfg_t;
/** @} USB configuration descriptor */

/**
 * @name USB interface descriptor
 * @{
 */
/**
 * @brief Standard USB interface descriptor.
 *
 * Structure fields have been reordered to account for byte alignment.
 */
typedef struct _usb_desc_iface_t
{
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint8_t  bInterfaceNumber;
   uint8_t  bAlternateSetting;
   uint8_t  bNumEndpoints;
   uint8_t  bInterfaceClass;
   uint8_t  bInterfaceSubClass;
   uint8_t  bInterfaceProtocol;
   uint8_t  iInterface;
} usb_desc_iface_t;
/** @} USB interface descriptor */

/**
 * @name USB endpoint descriptor
 * @{
 */
/**
 * @brief Standard USB endpoint descriptor.
 *
 * Structure fields have been reordered to account for byte alignment.
 */
typedef struct _usb_desc_ep_t
{
   uint16_t wMaxPacketSize;
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint8_t  bEndpointAddress;
   uint8_t  bmAttributes;
   uint8_t  bInterval;
} usb_desc_ep_t;
/** @} USB endpoint descriptor */


/*==================[external functions declaration]=========================*/

/**
 * @brief Advance buffer to the next given descriptor type.
 *
 * @warning Buffer \b must be positioned at the start of a descriptor.
 *
 * @param  pbuff     Pointer to buffer to advance, buffer will be modified.
 * @param  plen      Pointer to length of buffer, length will be modified.
 * @param  desc_type Advance to the next descriptor that matches this type.
 * @param  desc_size Desired descriptor should be this size in length.
 *
 * @return 0 on success, -1 otherwise.
 */
int usb_goto_next_desc(
      const uint8_t** pbuff,
      uint16_t*       plen,
      usb_stddesc_t   desc_type,
      uint16_t        desc_size
);

/**
 * @brief Advance buffer to the specified interface (and setting) descriptor.
 *
 * @warning Buffer \b must be positioned at the start of a descriptor.
 *
 * @param  pbuff     Pointer to buffer to advance, buffer will be modified.
 * @param  plen      Pointer to length of buffer, length will be modified.
 * @param  iface_idx Interface number to advance to.
 * @param  alt_set   Alternate setting of interface to advance to.
 *
 * @return 0 on success, -1 otherwise.
 */
int usb_goto_iface_desc(
      const uint8_t** pbuff,
      uint16_t*       plen,
      uint8_t         iface_idx,
      uint8_t         alt_set
);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif /* USB_DESC_H */


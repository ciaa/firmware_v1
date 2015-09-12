#ifndef USB_DESC_H
#define USB_DESC_H


#ifdef __cplusplus
extern "C" {
#endif


/* Inclusions */
#include <stdint.h>
#include <usb.h>


/** @brief Standard USB device descriptor.
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
#define USB_STDDESC_DEV_SIZE   18

#define USB_STDDESC_DEV_GET_bLength(x) \
   ((uint8_t ) (x)[0])
#define USB_STDDESC_DEV_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
#define USB_STDDESC_DEV_GET_bcdUSB(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[3] << 8) | (x)[2]))
#define USB_STDDESC_DEV_GET_bDeviceClass(x) \
   ((uint8_t ) (x)[4])
#define USB_STDDESC_DEV_GET_bDeviceSubClass(x) \
   ((uint8_t ) (x)[5])
#define USB_STDDESC_DEV_GET_bDeviceProtocol(x) \
   ((uint8_t ) (x)[6])
#define USB_STDDESC_DEV_GET_bMaxPacketSize0(x) \
   ((uint8_t ) (x)[7])
#define USB_STDDESC_DEV_GET_idVendor(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[9] << 8) | (x)[8]))
#define USB_STDDESC_DEV_GET_idProduct(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[11] << 8) | (x)[10]))
#define USB_STDDESC_DEV_GET_bcdDevice(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[13] << 8) | (x)[12]))
#define USB_STDDESC_DEV_GET_iManufacturer(x) \
   ((uint8_t ) (x)[14])
#define USB_STDDESC_DEV_GET_iProduct(x) \
   ((uint8_t ) (x)[15])
#define USB_STDDESC_DEV_GET_iSerialNumber(x) \
   ((uint8_t ) (x)[16])
#define USB_STDDESC_DEV_GET_bNumConfigurations(x) \
   ((uint8_t ) (x)[17])


/** @brief Standard USB configuration descriptor.
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
#define USB_STDDESC_CFG_SIZE    9

#define USB_STDDESC_CFG_GET_bLength(x) \
   ((uint8_t ) (x)[0])
#define USB_STDDESC_CFG_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
#define USB_STDDESC_CFG_GET_wTotalLength(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[3] << 8) | (x)[2]))
#define USB_STDDESC_CFG_GET_bNumInterfaces(x) \
   ((uint8_t ) (x)[4])
#define USB_STDDESC_CFG_GET_bConfigurationValue(x) \
   ((uint8_t ) (x)[5])
#define USB_STDDESC_CFG_GET_iConfiguration(x) \
   ((uint8_t ) (x)[6])
#define USB_STDDESC_CFG_GET_bmAttributes(x) \
   ((uint8_t ) (x)[7])
#define USB_STDDESC_CFG_GET_bMaxPower(x) \
   ((uint8_t ) (x)[8])


/** @brief Standard USB interface descriptor.
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
#define USB_STDDESC_IFACE_SIZE  9

#define USB_STDDESC_IFACE_GET_bLength(x) \
   ((uint8_t ) (x)[0])
#define USB_STDDESC_IFACE_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
#define USB_STDDESC_IFACE_GET_bInterfaceNumber(x) \
   ((uint8_t ) (x)[2])
#define USB_STDDESC_IFACE_GET_bAlternateSetting(x) \
   ((uint8_t ) (x)[3])
#define USB_STDDESC_IFACE_GET_bNumEndpoints(x) \
   ((uint8_t ) (x)[4])
#define USB_STDDESC_IFACE_GET_bInterfaceClass(x) \
   ((uint8_t ) (x)[5])
#define USB_STDDESC_IFACE_GET_bInterfaceSubClass(x) \
   ((uint8_t ) (x)[6])
#define USB_STDDESC_IFACE_GET_bInterfaceProtocol(x) \
   ((uint8_t ) (x)[7])
#define USB_STDDESC_IFACE_GET_iInterface(x) \
   ((uint8_t ) (x)[8])


/** @brief Standard USB endpoint descriptor.
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
#define USB_STDDESC_EP_SIZE            7
#define USB_STDDESC_EP_ATTR_TYPE_MASK  0x03

#define USB_STDDESC_EP_GET_bLength(x) \
   ((uint8_t ) (x)[0])
#define USB_STDDESC_EP_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
#define USB_STDDESC_EP_GET_bEndpointAddress(x) \
   ((uint8_t ) (x)[2])
#define USB_STDDESC_EP_GET_bmAttributes(x) \
   ((uint8_t ) (x)[3])
#define USB_STDDESC_EP_GET_wMaxPacketSize(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[5] << 8) | (x)[4]))
#define USB_STDDESC_EP_GET_bInterval(x) \
   ((uint8_t ) (x)[6])


/* Descriptor handling methods. */

int usb_goto_next_desc(
      const uint8_t** pbuff,
      uint8_t*        plen,
      usb_stddesc_t   ep_type,
      uint8_t         ep_size
);


#ifdef __cplusplus
}
#endif

#endif /* USB_DESC_H */

#ifndef USB_DESC_H
#define USB_DESC_H


#ifdef __cplusplus
extern "C" {
#endif


/* Inclusions */
#include <stdint.h>

/** @TODO comment this file and see what can be done about structure packing. */

typedef struct _usb_desc_dev_t
{
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint16_t bcdUSB;
   uint8_t  bDeviceClass;
   uint8_t  bDeviceSubClass;
   uint8_t  bDeviceProtocol;
   uint8_t  bMaxPacketSize0;
   uint16_t idVendor;
   uint16_t idProduct;
   uint16_t bcdDevice;
   uint8_t  iManufacturer;
   uint8_t  iProduct;
   uint8_t  iSerialNumber;
   uint8_t  bNumConfigurations;
} __attribute__ ((__packed__)) usb_desc_dev_t;
#define USB_STDDESC_DEV_SIZE   18

typedef struct _usb_desc_cfg_t
{
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint16_t wTotalLength;
   uint8_t  bNumInterfaces;
   uint8_t  bConfigurationValue;
   uint8_t  iConfiguration;
   uint8_t  bmAttributes;
   uint8_t  bMaxPower;
} __attribute__ ((__packed__)) usb_desc_cfg_t;
#define USB_STDDESC_CFG_SIZE    9

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
} __attribute__ ((__packed__)) usb_desc_iface_t;
#define USB_STDDESC_IFACE_SIZE  9

typedef struct _usb_desc_ep_t
{
   uint8_t  bLength;
   uint8_t  bDescriptorType;
   uint8_t  bEndpointAddress;
   uint8_t  bmAttributes;
   uint16_t wMaxPacketSize;
   uint8_t  bInterval;
} __attribute__ ((__packed__)) usb_desc_ep_t;
#define USB_STDDESC_EP_SIZE            7
#define USB_STDDESC_EP_ATTR_TYPE_MASK  0x03


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

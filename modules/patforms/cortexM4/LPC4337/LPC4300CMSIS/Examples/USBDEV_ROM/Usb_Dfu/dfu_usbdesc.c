/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.c
 * Purpose: USB Descriptors
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This software is supplied "AS IS" without any warranties, express,
 * implied or statutory, including but not limited to the implied
 * warranties of fitness for purpose, satisfactory quality and
 * noninfringement. Keil extends you a royalty-free right to reproduce
 * and distribute executable files created using this software for use
 * on NXP ARM microcontroller devices only. Nothing else gives
 * you the right to use this software.
 *
 * Copyright (c) 2005-2009 Keil Software.
 * Adaption to LPCxxxx, Copyright (c) 2009 NXP.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Changed string descriptor handling
 *                Changed HID Report Descriptor
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include <string.h>
#include "usbd/usbd.h"
#include "usbd/usbd_core.h"
#include "usbd/usbd_msc.h"
#include "usbd/usbd_dfu.h"
#include "config.h"

/* USB Standard Device Descriptor */
uint8_t USB_DeviceDescriptor[] =
{
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
  0x00,                              /* bDeviceClass */
  0x00,                              /* bDeviceSubClass */
  0x00,                              /* bDeviceProtocol */
  USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
  WBVAL(0x1FC9),                     /* idVendor */
  WBVAL(0x0106),                     /* idProduct */
  WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
  0x01,                              /* iManufacturer */
  0x02,                              /* iProduct */
  0x03,                              /* iSerialNumber */
  0x01                               /* bNumConfigurations */
};

/* USB FSConfiguration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
uint8_t USB_FsConfigDescriptor[] = {
/* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    1*USB_CONFIGUARTION_DESC_SIZE +
    1*USB_INTERFACE_DESC_SIZE     +
    DFU_FUNC_DESC_SIZE            
  ),
  0x01,                              /* bNumInterfaces */
  0x01,                              /* bConfigurationValue */
  0x00,                              /* iConfiguration */
  USB_CONFIG_SELF_POWERED,           /* bmAttributes */
  USB_CONFIG_POWER_MA(100),          /* bMaxPower */
/* Interface 0, Alternate Setting 0, DFU Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x00,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x00,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_APP,              /* bInterfaceClass */
  USB_DFU_SUBCLASS,                  /* bInterfaceSubClass */
  0x01,                               /* bInterfaceProtocol */
  0x04,                              /* iInterface */
/* DFU RunTime/DFU Mode Functional Descriptor */
  DFU_FUNC_DESC_SIZE,                /* bLength */
  USB_DFU_DESCRIPTOR_TYPE,           /* bDescriptorType */
  USB_DFU_CAN_DOWNLOAD | USB_DFU_CAN_UPLOAD | USB_DFU_MANIFEST_TOL | USB_DFU_WILL_DETACH, /* bmAttributes */
  WBVAL(0xFF00),                     /* wDetachTimeout */
  WBVAL(USB_DFU_XFER_SIZE),          /* wTransferSize */
  WBVAL(0x100),                      /* bcdDFUVersion */
/* Terminator */
  0                                  /* bLength */
};

/* USB HSConfiguration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
uint8_t USB_HsConfigDescriptor[] = {
  /* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    1*USB_CONFIGUARTION_DESC_SIZE +
    1*USB_INTERFACE_DESC_SIZE     +
    DFU_FUNC_DESC_SIZE            
  ),
  0x01,                              /* bNumInterfaces */
  0x01,                              /* bConfigurationValue */
  0x00,                              /* iConfiguration */
  USB_CONFIG_SELF_POWERED /*|*/       /* bmAttributes */
  /*USB_CONFIG_REMOTE_WAKEUP*/,
  USB_CONFIG_POWER_MA(100),          /* bMaxPower */
/* Interface 0, Alternate Setting 0, DFU Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x00,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x00,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_APP,              /* bInterfaceClass */
  USB_DFU_SUBCLASS,                  /* bInterfaceSubClass */
  0x01,                              /* bInterfaceProtocol */
  0x04,                              /* iInterface */
/* DFU RunTime/DFU Mode Functional Descriptor */
  DFU_FUNC_DESC_SIZE,                /* bLength */
  USB_DFU_DESCRIPTOR_TYPE,           /* bDescriptorType */
  USB_DFU_CAN_DOWNLOAD | USB_DFU_CAN_UPLOAD | USB_DFU_MANIFEST_TOL, /* bmAttributes */
  WBVAL(0xFF00),                     /* wDetachTimeout */
  WBVAL(USB_DFU_XFER_SIZE),          /* wTransferSize */
  WBVAL(0x100),                      /* bcdDFUVersion */
  /* Terminator */
  0                                  /* bLength */
};

/* USB String Descriptor (optional) */
uint8_t USB_StringDescriptor[] =
{
  /* Index 0x00: LANGID Codes */
  0x04,                              /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0409), /* US English */    /* wLANGID */
  /* Index 0x01: Manufacturer */
  (18*2 + 2),                        /* bLength (13 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'N', 0,
  'X', 0,
  'P', 0,
  ' ', 0,
  'S', 0,
  'e', 0,
  'm', 0,
  'i', 0,
  'c', 0,
  'o', 0,
  'n', 0,
  'd', 0,
  'u', 0,
  'c', 0,
  't', 0,
  'o', 0,
  'r', 0,
  's', 0,
  /* Index 0x02: Product */
  (14*2 + 2),                        /* bLength (13 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'L', 0,
  'P', 0,
  'C', 0,
  '1', 0,
  '8', 0,
  'x', 0,
  'x', 0,
  ' ', 0,
  'M', 0,
  'e', 0,
  'm', 0,
  'o', 0,
  'r', 0,
  'y', 0,
  /* Index 0x03: Serial Number */
  (13*2 + 2),                        /* bLength (13 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  'A', 0,
  'B', 0,
  'C', 0,
  'D', 0,
  '1', 0,
  '2', 0,
  '3', 0,
  '4', 0,
  '5', 0,
  '6', 0,
  '7', 0,
  '8', 0,
  '9', 0,
  /* Index 0x04: Interface 0, Alternate Setting 0 */
  (3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,       /* bDescriptorType */
  'D', 0,
  'F', 0,
  'U', 0,
  /* Index 0x05: Interface 1, Alternate Setting 0 */
  (6*2 + 2),                        /* bLength (13 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,       /* bDescriptorType */
  'M', 0,
  'e', 0,
  'm', 0,
  'o', 0,
  'r', 0,
  'y', 0,
};

/* USB Device Qualifier */
uint8_t USB_DeviceQualifier[] = {
  USB_DEVICE_QUALI_SIZE,              /* bLength */
  USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
  0x00,                              /* bDeviceClass */
  0x00,                              /* bDeviceSubClass */
  0x00,                              /* bDeviceProtocol */
  USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
  0x01,                              /* bNumOtherSpeedConfigurations */
  0x00                               /* bReserved */
};


void copy_descriptors(USB_CORE_DESCS_T* pDesc, uint32_t mem_base)
{
  uint8_t* dest = (uint8_t*)mem_base;

  /* set all descriptors */
  memcpy((void*)mem_base, USB_DeviceDescriptor, sizeof(USB_DeviceDescriptor));
  pDesc->device_desc = dest;
  dest += sizeof(USB_DeviceDescriptor);
  /* align to 4 byte boundary */
  while ((uint32_t)dest & 0x03) dest++;
  
  memcpy(dest, USB_FsConfigDescriptor, sizeof(USB_FsConfigDescriptor));
  pDesc->full_speed_desc = dest;
  dest += sizeof(USB_FsConfigDescriptor);
  /* align to 4 byte boundary */
  while ((uint32_t)dest & 0x03) dest++;

  memcpy(dest, USB_HsConfigDescriptor, sizeof(USB_HsConfigDescriptor));
  pDesc->high_speed_desc = dest;
  dest += sizeof(USB_HsConfigDescriptor);
  /* align to 4 byte boundary */
  while ((uint32_t)dest & 0x03) dest++;

  memcpy(dest, USB_StringDescriptor, sizeof(USB_StringDescriptor));
  pDesc->string_desc = dest;
  dest += sizeof(USB_StringDescriptor);
   /* align to 4 byte boundary */
  while ((uint32_t)dest & 0x03) dest++;

  memcpy(dest, USB_DeviceQualifier, sizeof(USB_DeviceQualifier));
  pDesc->device_qualifier = dest;
  dest += sizeof(USB_DeviceQualifier);
  /* align to 4 byte boundary */
  while ((uint32_t)dest & 0x03) dest++;
}





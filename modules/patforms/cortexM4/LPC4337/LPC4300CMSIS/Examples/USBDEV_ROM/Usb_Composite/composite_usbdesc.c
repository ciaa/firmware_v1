/**********************************************************************
* $Id$		composite_usbdesc.c			2011-06-02
*//**
* @file		composite_usbdesc.c
* @brief	USB composite example project.
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include <string.h>
#include "usbd/usbd.h"
#include "usbd/usbd_core.h"
#include "usbd/usbd_msc.h"
#include "usbd/usbd_dfu.h"
#include "usbd/usbd_hid.h"
#include "config.h"

/*------------------------------------------------------------------------------
  HID Report Descriptor
 *------------------------------------------------------------------------------*/
#define HID_INPUT_REPORT_BYTES       1              /* size of report in Bytes */
#define HID_OUTPUT_REPORT_BYTES      1              /* size of report in Bytes */
#define HID_FEATURE_REPORT_BYTES     1              /* size of report in Bytes */

const uint8_t HID_ReportDescriptor[] = {
  HID_UsagePageVendor( 0x00                     ),
  HID_Usage          ( 0x01                     ),
  HID_Collection     ( HID_Application          ),
    HID_LogicalMin   ( 0                        ),  /* value range: 0 - 0xFF */
    HID_LogicalMaxS  ( 0xFF                     ),
    HID_ReportSize   ( 8                        ),  /* 8 bits */
    HID_ReportCount  ( HID_INPUT_REPORT_BYTES   ),
    HID_Usage        ( 0x01                     ),
    HID_Input        ( HID_Data | HID_Variable | HID_Absolute ),
    HID_ReportCount  ( HID_OUTPUT_REPORT_BYTES  ),
    HID_Usage        ( 0x01                     ),
    HID_Output       ( HID_Data | HID_Variable | HID_Absolute ),
    HID_ReportCount  ( HID_FEATURE_REPORT_BYTES ),
    HID_Usage        ( 0x01                     ),
    HID_Feature      ( HID_Data | HID_Variable | HID_Absolute ),
  HID_EndCollection,
};
const uint16_t HID_ReportDescSize = sizeof(HID_ReportDescriptor);

/* USB Standard Device Descriptor */
uint8_t USB_DeviceDescriptor[] =
{
  USB_DEVICE_DESC_SIZE,              /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
  0x00,                              /* bDeviceClass */
  0x00,                              /* bDeviceSubClass */
  0x00,                              /* bDeviceProtocol */
  64,                                /* bMaxPacketSize0 */
  WBVAL(0x1FC9),                     /* idVendor */
  WBVAL(0x0114),                     /* idProduct */
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
    DFU_FUNC_DESC_SIZE            +
    1*USB_INTERFACE_DESC_SIZE     +
    2*USB_ENDPOINT_DESC_SIZE      +
    1*USB_INTERFACE_DESC_SIZE     +
    HID_DESC_SIZE                 +
    1*USB_ENDPOINT_DESC_SIZE
  ),
  0x03,                              /* bNumInterfaces */
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
/* Interface 1, Alternate Setting 0, MSC Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x01,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x02,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_STORAGE,          /* bInterfaceClass */
  MSC_SUBCLASS_SCSI,                 /* bInterfaceSubClass */
  MSC_PROTOCOL_BULK_ONLY,            /* bInterfaceProtocol */
  0x05,                              /* iInterface */
/* Bulk In Endpoint */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  MSC_EP_IN,                         /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(USB_FS_MAX_BULK_PACKET),     /* wMaxPacketSize */
  0,                                 /* bInterval */
/* Bulk Out Endpoint */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  MSC_EP_OUT,                        /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(USB_FS_MAX_BULK_PACKET),     /* wMaxPacketSize */
  0,                                 /* bInterval */
/* Interface 2, Alternate Setting 0, HID Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x02,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x01,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
  HID_SUBCLASS_NONE,                 /* bInterfaceSubClass */
  HID_PROTOCOL_NONE,                 /* bInterfaceProtocol */
  0x06,                              /* iInterface */
/* HID Class Descriptor */
/* HID_DESC_OFFSET = 0x0012 */
  HID_DESC_SIZE,                     /* bLength */
  HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
  WBVAL(0x0100), /* 1.00 */          /* bcdHID */
  0x00,                              /* bCountryCode */
  0x01,                              /* bNumDescriptors */
  HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(HID_RPRT_DESC_SZ),         /* wDescriptorLength */
/* Endpoint, HID Interrupt In */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  HID_EP_IN,                         /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
  WBVAL(0x0004),                     /* wMaxPacketSize */
  0x20,          /* 16ms */          /* bInterval */
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
    DFU_FUNC_DESC_SIZE            +
    1*USB_INTERFACE_DESC_SIZE     +
    2*USB_ENDPOINT_DESC_SIZE      +
    1*USB_INTERFACE_DESC_SIZE     +
    HID_DESC_SIZE                 +
    1*USB_ENDPOINT_DESC_SIZE
  ),
  0x03,                              /* bNumInterfaces */
  0x02,                              /* bConfigurationValue */
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
/* Interface 1, Alternate Setting 0, MSC Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x01,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x02,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_STORAGE,          /* bInterfaceClass */
  MSC_SUBCLASS_SCSI,                 /* bInterfaceSubClass */
  MSC_PROTOCOL_BULK_ONLY,            /* bInterfaceProtocol */
  0x05,                              /* iInterface */
  /* Bulk In Endpoint */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  MSC_EP_IN,                         /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(512),                        /* wMaxPacketSize */
  0,                                 /* bInterval */
  /* Bulk Out Endpoint */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  MSC_EP_OUT,                        /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,            /* bmAttributes */
  WBVAL(USB_HS_MAX_BULK_PACKET),     /* wMaxPacketSize */
  0,                                 /* bInterval */
/* Interface 2, Alternate Setting 0, HID Class */
  USB_INTERFACE_DESC_SIZE,           /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x02,                              /* bInterfaceNumber */
  0x00,                              /* bAlternateSetting */
  0x01,                              /* bNumEndpoints */
  USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
  HID_SUBCLASS_NONE,                 /* bInterfaceSubClass */
  HID_PROTOCOL_NONE,                 /* bInterfaceProtocol */
  0x06,                              /* iInterface */
/* HID Class Descriptor */
/* HID_DESC_OFFSET = 0x0012 */
  HID_DESC_SIZE,                     /* bLength */
  HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
  WBVAL(0x0100), /* 1.00 */          /* bcdHID */
  0x00,                              /* bCountryCode */
  0x01,                              /* bNumDescriptors */
  HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
  WBVAL(HID_RPRT_DESC_SZ),         /* wDescriptorLength */
/* Endpoint, HID Interrupt In */
  USB_ENDPOINT_DESC_SIZE,            /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
  HID_EP_IN,                         /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
  WBVAL(0x0004),                     /* wMaxPacketSize */
  0x10,          /* 16ms */          /* bInterval */
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
  /* Index 0x06: Interface 2, Alternate Setting 0 */
  (3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
  USB_STRING_DESCRIPTOR_TYPE,       /* bDescriptorType */
  'H', 0,
  'I', 0,
  'D', 0,
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

uint8_t USB_dfuConfigDescriptor[] = {
  /* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
  WBVAL(                             /* wTotalLength */
    1*USB_CONFIGUARTION_DESC_SIZE +
    1*USB_INTERFACE_DESC_SIZE     +
    DFU_FUNC_DESC_SIZE
  ),
  0x01,                              /* bNumInterfaces */
  0x03,                              /* bConfigurationValue */
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
  0x02,                              /* bInterfaceProtocol */
  0x00,                              /* iInterface */
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

void overwrt_dfu_only_desc(uint8_t* pFsDesc, uint8_t* pHsDesc)
{
  memcpy(pFsDesc, USB_dfuConfigDescriptor, sizeof(USB_dfuConfigDescriptor));
  memcpy(pHsDesc, USB_dfuConfigDescriptor, sizeof(USB_dfuConfigDescriptor));
}

uint32_t copy_hid_descriptors(uint8_t** pDesc, uint32_t* mem_base, uint32_t* mem_size)
{
  *pDesc = (uint8_t*)(*mem_base);
  memcpy(*pDesc, HID_ReportDescriptor, sizeof(HID_ReportDescriptor));
  *mem_base += sizeof(HID_ReportDescriptor);
  *mem_size -= sizeof(HID_ReportDescriptor);
  /* align to 4 byte boundary */
  while (*mem_base & 0x03) {
    *mem_base = *mem_base + 1;
    *mem_size = *mem_size - 1;
  }
  return  sizeof(HID_ReportDescriptor);
}


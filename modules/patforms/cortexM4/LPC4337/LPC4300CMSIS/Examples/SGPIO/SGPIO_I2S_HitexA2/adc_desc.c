/***********************************************************************
 * $Id: adc_def.c
 *
 * Project: USB audio device class demo
 *
 * Description: USB ADC example project.
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
#include "config.h"
#include "usbd_config.h"
#include "adc_user.h"


/* USB Standard Device Descriptor */
ALIGNED(4) uint8_t USB_DeviceDescriptor[] =
{
	USB_DEVICE_DESC_SIZE,              /* bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
	WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
	0x00,                              /* bDeviceClass */
	0x00,                              /* bDeviceSubClass */
	0x00,                              /* bDeviceProtocol */
	USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
	WBVAL(0x1FC9),                     /* idVendor */
	WBVAL(0x0001),                     /* idProduct */
	WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
	0x01,                              /* iManufacturer */
	0x02,                              /* iProduct */
	0x03,                              /* iSerialNumber */
	0x01                               /* bNumConfigurations */
};

/* USB FSConfiguration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
ALIGNED(4) uint8_t ADC_FsSyncConfigDescriptor[] = {
	/* Configuration 1 */
	USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
	WBVAL(                             /* wTotalLength */
	USB_CONFIGUARTION_DESC_SIZE         +
	USB_INTERFACE_DESC_SIZE             +
	AUDIO_CONTROL_INTERFACE_DESC_SZ(2)  +
	AUDIO_INPUT_TERMINAL_DESC_SIZE      +
	AUDIO_FEATURE_UNIT_DESC_SZ(2,1)     +
	AUDIO_OUTPUT_TERMINAL_DESC_SIZE     +

	AUDIO_INPUT_TERMINAL_DESC_SIZE      +
	AUDIO_OUTPUT_TERMINAL_DESC_SIZE     +

	USB_INTERFACE_DESC_SIZE             +
	USB_INTERFACE_DESC_SIZE             +
	AUDIO_STREAMING_INTERFACE_DESC_SIZE +
	AUDIO_FORMAT_TYPE_I_DESC_SZ(1)      +
	AUDIO_STANDARD_ENDPOINT_DESC_SIZE   +
	AUDIO_STREAMING_ENDPOINT_DESC_SIZE  +

	USB_INTERFACE_DESC_SIZE             +
	USB_INTERFACE_DESC_SIZE             +
	AUDIO_STREAMING_INTERFACE_DESC_SIZE +
	AUDIO_FORMAT_TYPE_I_DESC_SZ(1)      +
	AUDIO_STANDARD_ENDPOINT_DESC_SIZE   +
	AUDIO_STREAMING_ENDPOINT_DESC_SIZE  
	),
	0x03,                               /* bNumInterfaces */
	0x01,                              	/* bConfigurationValue */
	0x00,                              	/* iConfiguration */
	USB_CONFIG_SELF_POWERED,           	/* bmAttributes */
	USB_CONFIG_POWER_MA(100),          	/* bMaxPower */
	/* Interface 0, Alternate Setting 0, Audio Control */
	USB_INTERFACE_DESC_SIZE,            /* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	USB_ADC_CIF_NUM,                    /* bInterfaceNumber */
	0x00,                               /* bAlternateSetting */
	0x00,                               /* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,             /* bInterfaceClass */
	AUDIO_SUBCLASS_AUDIOCONTROL,        /* bInterfaceSubClass */
	AUDIO_PROTOCOL_UNDEFINED,           /* bInterfaceProtocol */
	0x00,                               /* iInterface */
	/* Audio Control Interface */
	AUDIO_CONTROL_INTERFACE_DESC_SZ(2), /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,    /* bDescriptorType */
	AUDIO_CONTROL_HEADER,               /* bDescriptorSubtype */
	WBVAL(0x0100), /* 1.00 */           /* bcdADC */
	WBVAL(                              /* wTotalLength */
	AUDIO_CONTROL_INTERFACE_DESC_SZ(2) +
	AUDIO_INPUT_TERMINAL_DESC_SIZE     +
	AUDIO_FEATURE_UNIT_DESC_SZ(2, 1)   +
	AUDIO_OUTPUT_TERMINAL_DESC_SIZE    +
	AUDIO_INPUT_TERMINAL_DESC_SIZE     +
	AUDIO_OUTPUT_TERMINAL_DESC_SIZE
	),
	0x02,                                 /* bInCollection */
	0x01,                                 /* baInterfaceNr(1) */
	0x02,                                 /* baInterfaceNr(2) */
	/* Audio Input Terminal */
	AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
	0x01,                                 /* bTerminalID */
	WBVAL(AUDIO_TERMINAL_USB_STREAMING),  /* wTerminalType */
	0x00,                                 /* bAssocTerminal */
	0x02,                                 /* bNrChannels */
	WBVAL(AUDIO_CHANNEL_L |
	AUDIO_CHANNEL_R),                     /* wChannelConfig */
	0x00,                                 /* iChannelNames */
	0x00,                                 /* iTerminal */
	/* Audio Feature Unit */
	AUDIO_FEATURE_UNIT_DESC_SZ(2, 1),     /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
	0x02,                                 /* bUnitID */
	0x01,                                 /* bSourceID */
	0x01,                                 /* bControlSize */
	AUDIO_CONTROL_MUTE |
	AUDIO_CONTROL_VOLUME,                 /* bmaControls(0) */
	0x00,                                 /* bmaControls(1) */
	0x00,                                 /* bmaControls(2) */
	0x00,                                 /* iTerminal */
	/* Audio Output Terminal */
	AUDIO_OUTPUT_TERMINAL_DESC_SIZE,      /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
	0x03,                                 /* bTerminalID */
	WBVAL(AUDIO_TERMINAL_SPEAKER),        /* wTerminalType */
	0x00,                                 /* bAssocTerminal */
	0x02,                                 /* bSourceID */
	0x00,                                 /* iTerminal */

	/* Audio Input Terminal -microphone */
	AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
	0x04,                                 /* bTerminalID */
	WBVAL(AUDIO_TERMINAL_MICROPHONE),     /* wTerminalType */
	0x00,                                 /* bAssocTerminal */
	0x02,                                 /* bNrChannels */
	WBVAL(AUDIO_CHANNEL_L |
	AUDIO_CHANNEL_R),                     /* wChannelConfig */
	0x00,                                 /* iChannelNames */
	0x00,                                 /* iTerminal */
	/* Audio Output Terminal -speaker*/
	AUDIO_OUTPUT_TERMINAL_DESC_SIZE,      /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
	0x05,                                 /* bTerminalID */
	WBVAL(AUDIO_TERMINAL_USB_STREAMING),  /* wTerminalType */
	0x00,                                 /* bAssocTerminal */
	0x04,                                 /* bSourceID */
	0x00,                                 /* iTerminal */

	/* Interface 1, Alternate Setting 0, Audio Streaming - Zero Bandwith */
	USB_INTERFACE_DESC_SIZE,              /* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
	USB_ADC_SIF1_NUM,                     /* bInterfaceNumber */
	0x00,                                 /* bAlternateSetting */
	0x00,                                 /* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
	AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
	AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
	0x00,                                 /* iInterface */
	/* Interface 1, Alternate Setting 1, Audio Streaming - Operational */
	USB_INTERFACE_DESC_SIZE,              /* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
	0x01,                                 /* bInterfaceNumber */
	0x01,                                 /* bAlternateSetting */
	0x01,                                 /* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
	AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
	AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
	0x00,                                 /* iInterface */
	/* Audio Streaming Interface */
	AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
	0x01,                                 /* bTerminalLink */
	0x01,                                 /* bDelay */
	WBVAL(AUDIO_FORMAT_PCM),              /* wFormatTag */
	/* Audio Type I Format */
	AUDIO_FORMAT_TYPE_I_DESC_SZ(1),       /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
	AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
	0x02,                                 /* bNrChannels */
	0x02,                                 /* bSubFrameSize */
	16,                                   /* bBitResolution */
	0x01,                                 /* bSamFreqType */
	B3VAL(DEF_SAMPLE_RATE),               /* tSamFreq */
	/* Endpoint - Standard Descriptor */
	AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
	USB_ADC_OUT_EP,                       /* bEndpointAddress */
	USB_ENDPOINT_SYNC_SYNCHRONOUS | 
	USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
	WBVAL(DEF_MAX_PKT_SZ),                /* wMaxPacketSize */
	0x01,                                 /* bInterval */
	0x00,                                 /* bRefresh */
	0x00,                                 /* bSynchAddress */
	/* Endpoint - Audio Streaming */
	AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
	AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
	AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
	AUDIO_CONTROL_SAMPLING_FREQ,          /* bmAttributes */
	0x00,                                 /* bLockDelayUnits */
	WBVAL(0x0000),                        /* wLockDelay */

	/* Interface 2, Alternate Setting 0, Audio Streaming - Zero Bandwith */
	USB_INTERFACE_DESC_SIZE,              /* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
	USB_ADC_SIF2_NUM,                     /* bInterfaceNumber */
	0x00,                                 /* bAlternateSetting */
	0x00,                                 /* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
	AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
	AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
	0x00,                                 /* iInterface */
	/* Interface 2, Alternate Setting 1, Audio Streaming - Operational */
	USB_INTERFACE_DESC_SIZE,              /* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
	0x02,                                 /* bInterfaceNumber */
	0x01,                                 /* bAlternateSetting */
	0x01,                                 /* bNumEndpoints */
	USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
	AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
	AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
	0x00,                                 /* iInterface */
	/* Audio Streaming Interface */
	AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
	0x05,                                 /* bTerminalLink */
	0x01,                                 /* bDelay */
	WBVAL(AUDIO_FORMAT_PCM),              /* wFormatTag */
	/* Audio Type I Format */
	AUDIO_FORMAT_TYPE_I_DESC_SZ(1),       /* bLength */
	AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
	AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
	AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
	0x02,                                 /* bNrChannels */
	0x02,                                 /* bSubFrameSize */
	16,                                   /* bBitResolution */
	0x01,                                 /* bSamFreqType */
	B3VAL(DEF_SAMPLE_RATE),               /* tSamFreq */
	/* Endpoint - Standard Descriptor */
	AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
	USB_ADC_IN_EP,                        /* bEndpointAddress */
	USB_ENDPOINT_SYNC_SYNCHRONOUS | 
	USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
	WBVAL(DEF_MAX_PKT_SZ),                /* wMaxPacketSize */
	0x01,                                 /* bInterval */
	0x00,                                 /* bRefresh */
	0x00,                                 /* bSynchAddress */
	/* Endpoint - Audio Streaming */
	AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
	AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
	AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
	AUDIO_CONTROL_SAMPLING_FREQ,          /* bmAttributes */
	0x00,                                 /* bLockDelayUnits */
	WBVAL(0x0000),                        /* wLockDelay */
	/* Terminator */
	0                                     /* bLength */
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t ADC_FsAsyncConfigDescriptor[] = {
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,          /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType */
    WBVAL(                                /* wTotalLength */
    USB_CONFIGUARTION_DESC_SIZE         +
    USB_INTERFACE_DESC_SIZE             +
    AUDIO_CONTROL_INTERFACE_DESC_SZ(2)  +
    AUDIO_INPUT_TERMINAL_DESC_SIZE      +
    AUDIO_FEATURE_UNIT_DESC_SZ(2,1)     +
    AUDIO_OUTPUT_TERMINAL_DESC_SIZE     +
    
    AUDIO_INPUT_TERMINAL_DESC_SIZE      +
    AUDIO_OUTPUT_TERMINAL_DESC_SIZE     +
    
    USB_INTERFACE_DESC_SIZE             +
    USB_INTERFACE_DESC_SIZE             +
    AUDIO_STREAMING_INTERFACE_DESC_SIZE +                  
    AUDIO_FORMAT_TYPE_I_DESC_SZ(1)      +
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE   +
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE  +
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE   +
    
    USB_INTERFACE_DESC_SIZE             +
    USB_INTERFACE_DESC_SIZE             +
    AUDIO_STREAMING_INTERFACE_DESC_SIZE +
    AUDIO_FORMAT_TYPE_I_DESC_SZ(1)      +
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE   +
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE  +
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE   
    
    ),
    0x03,                                 /* bNumInterfaces */
    0x01,                                 /* bConfigurationValue */
    0x00,                                 /* iConfiguration */
    USB_CONFIG_SELF_POWERED,              /* bmAttributes  */
    USB_CONFIG_POWER_MA(8),               /* bMaxPower */
    /* Interface 0, Alternate Setting 0, Audio Control */
    USB_INTERFACE_DESC_SIZE,              /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x00,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* Audio Control Interface */
    AUDIO_CONTROL_INTERFACE_DESC_SZ(2),   /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
    WBVAL(0x0100), /* 1.00 */             /* bcdADC */
    WBVAL(                                /* wTotalLength */
    AUDIO_CONTROL_INTERFACE_DESC_SZ(2) +
    AUDIO_INPUT_TERMINAL_DESC_SIZE     +
    AUDIO_FEATURE_UNIT_DESC_SZ(2,1)    +
    AUDIO_OUTPUT_TERMINAL_DESC_SIZE    +
    AUDIO_INPUT_TERMINAL_DESC_SIZE     +
    AUDIO_OUTPUT_TERMINAL_DESC_SIZE
    ),
    0x02,                                 /* bInCollection */
    0x01,                                 /* baInterfaceNr(1) */
    0x02,                                 /* baInterfaceNr(2) */
    /* Audio Input Terminal */
    AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
    0x01,                                 /* bTerminalID */
    WBVAL(AUDIO_TERMINAL_USB_STREAMING),  /* wTerminalType */
    0x00,                                 /* bAssocTerminal */
    0x02,                                 /* bNrChannels */
    WBVAL(AUDIO_CHANNEL_L | 
    AUDIO_CHANNEL_R),                     /* wChannelConfig */
    0x00,                                 /* iChannelNames */
    0x00,                                 /* iTerminal */
    /* Audio Feature Unit */
    AUDIO_FEATURE_UNIT_DESC_SZ(2,1),      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
    0x02,                                 /* bUnitID */
    0x01,                                 /* bSourceID */
    0x01,                                 /* bControlSize */
    AUDIO_CONTROL_MUTE |
    AUDIO_CONTROL_VOLUME,                 /* bmaControls(0) */
    0x00,                                 /* bmaControls(1) */
    0x00,                                 /* bmaControls(2) */
    0x00,                                 /* iTerminal */
    /* Audio Output Terminal */
    AUDIO_OUTPUT_TERMINAL_DESC_SIZE,      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
    0x03,                                 /* bTerminalID */
    WBVAL(AUDIO_TERMINAL_SPEAKER),        /* wTerminalType */
    0x00,                                 /* bAssocTerminal */
    0x02,                                 /* bSourceID */
    0x00,                                 /* iTerminal */
    
    /* Audio Input Terminal -microphone */
    AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
    0x04,                                 /* bTerminalID */
    WBVAL(AUDIO_TERMINAL_MICROPHONE),     /* wTerminalType */
    0x00,                                 /* bAssocTerminal */
    0x02,                                 /* bNrChannels */
    WBVAL(AUDIO_CHANNEL_L | 
    AUDIO_CHANNEL_R),                     /* wChannelConfig */
    0x00,                                 /* iChannelNames */
    0x00,                                 /* iTerminal */
    /* Audio Output Terminal -microphone*/
    AUDIO_OUTPUT_TERMINAL_DESC_SIZE,      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
    0x05,                                 /* bTerminalID */
    WBVAL(AUDIO_TERMINAL_USB_STREAMING),  /* wTerminalType */
    0x00,                                 /* bAssocTerminal */
    0x04,                                 /* bSourceID */
    0x00,                                 /* iTerminal */
    
    /* Interface 1, Alternate Setting 0, Audio Streaming - Zero Bandwith */
    USB_INTERFACE_DESC_SIZE,              /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* Interface 1, Alternate Setting 1, Audio Streaming - Operational */
    USB_INTERFACE_DESC_SIZE,              /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x01,                                 /* bAlternateSetting */
    0x02,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* Audio Streaming Interface */
    AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
    0x01,                                 /* bTerminalLink */
    0x01,                                 /* bDelay */
    WBVAL(AUDIO_FORMAT_PCM),              /* wFormatTag */
    /* Audio Type I Format */
    AUDIO_FORMAT_TYPE_I_DESC_SZ(1),       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
    0x02,                                 /* bNrChannels */
    0x02,                                 /* bSubFrameSize */
    16,                                   /* bBitResolution */
    0x01,                                 /* bSamFreqType */
    B3VAL(DEF_SAMPLE_RATE),               /* tSamFreq */
    /* Endpoint - Standard Descriptor */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    USB_ADC_OUT_EP,                       /* bEndpointAddress */
    USB_ENDPOINT_SYNC_ASYNCHRONOUS | 
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
    WBVAL((DEF_MAX_PKT_SZ + 4)),          /* wMaxPacketSize */
    0x01,                                 /* bInterval */
    0x00,                                 /* bRefresh */
    USB_ADC_OUTSYNC_EP,                   /* bSynchAddress */
    /* Endpoint - Audio Streaming */
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
    AUDIO_CONTROL_SAMPLING_FREQ,          /* bmAttributes */
    0x00,                                 /* bLockDelayUnits */
    WBVAL(0x0000),                        /* wLockDelay */
    /* Endpoint - Standard Descriptor for synch EP */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    USB_ADC_OUTSYNC_EP,                   /* bEndpointAddress */
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
    WBVAL(USB_ADC_SYNC_MAXP),             /* wMaxPacketSize */
    0x01,                                 /* bInterval */
    USB_ADC_SYNC_REFRESH,                 /* bRefresh */
    0x00,                                 /* bSynchAddress */
    
    /* Interface 2, Alternate Setting 0, Audio Streaming - Zero Bandwith */
    USB_INTERFACE_DESC_SIZE,              /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x02,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* Interface 2, Alternate Setting 1, Audio Streaming - Operational */
    USB_INTERFACE_DESC_SIZE,              /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x02,                                 /* bInterfaceNumber */
    0x01,                                 /* bAlternateSetting */
    0x01,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* Audio Streaming Interface */
    AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
    0x05,                                 /* bTerminalLink */
    0x01,                                 /* bDelay */
    WBVAL(AUDIO_FORMAT_PCM),              /* wFormatTag */
    /* Audio Type I Format */
    AUDIO_FORMAT_TYPE_I_DESC_SZ(1),       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
    0x02,                                 /* bNrChannels */
    0x02,                                 /* bSubFrameSize */
    16,                                   /* bBitResolution */
    0x01,                                 /* bSamFreqType */
    B3VAL(DEF_SAMPLE_RATE),               /* tSamFreq */
    /* Endpoint - Standard Descriptor */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    USB_ADC_IN_EP,                        /* bEndpointAddress */
    /*USB_ENDPOINT_SYNC_SYNCHRONOUS |*/ 
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
    WBVAL(DEF_MAX_PKT_SZ),                /* wMaxPacketSize */
    0x01,                                 /* bInterval */
    0x00,                                 /* bRefresh */
    USB_ADC_INSYNC_EP,                    /* bSynchAddress */
    /* Endpoint - Audio Streaming */
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
    AUDIO_CONTROL_SAMPLING_FREQ,          /* bmAttributes */
    0x00,                                 /* bLockDelayUnits */
    WBVAL(0x0000),                        /* wLockDelay */
    /* Endpoint - Standard Descriptor for synch EP */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    USB_ADC_INSYNC_EP,                    /* bEndpointAddress */
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */
    WBVAL(USB_ADC_SYNC_MAXP),             /* wMaxPacketSize */
    0x01,                                 /* bInterval */
    USB_ADC_SYNC_REFRESH,                 /* bRefresh */
    0x00,                                 /* bSynchAddress */
    
    /* Terminator */
    0                                     /* bLength */
};

/* USB String Descriptor (optional) */
ALIGNED(4) uint8_t USB_StringDescriptor[] =
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
	(20*2 + 2),                              /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	'N', 0,
	'X', 0,
	'P', 0,
	' ', 0,
	'R', 0,
	'e', 0,
	'f', 0,
	'e', 0,
	'r', 0,
	'e', 0,
	'n', 0,
	'c', 0,
	'e', 0,
	'S', 0,
	'p', 0,
	'e', 0,
	'a', 0,
	'k', 0,
	'e', 0,
	'r', 0,
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



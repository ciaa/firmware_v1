#ifndef __USB_STD_H__
#define __USB_STD_H__
/**
* @addtogroup USB_STD
* @brief CIAA USB Standard Requests
*
* @par Description
*
* @{ */

#include <stdint.h>


typedef struct _usb_stdreq_t
{
	uint8_t  bmRequestType;  /**< Characteristics of request.                 */
	uint8_t  bRequest;       /**< Specific request (see usb_stdreq_cmd_t).    */
	uint16_t wValue;         /**< Word-sized request-specific parameter.      */
	uint16_t wIndex;         /**< Word-sized request-specific parameter.      */
	uint16_t wLength;        /**< Number of bytes to transfer in Data Stage.  */
} usb_stdreq_t;

#define USB_STDREQ_DIR             0x80
#define USB_STDREQ_DIR_S           7
#define	USB_STDREQ_TYPE            0x60
#define	USB_STDREQ_TYPE_S          5
#define	USB_STDREQ_RECIP           0x1F
#define	USB_STDREQ_RECIP_S         0

#define USB_STDREQ_TYPE_STD        0
#define USB_STDREQ_TYPE_CLASS      1
#define USB_STDREQ_TYPE_VENDOR     2

#define	USB_STDREQ_RECIP_DEV       0
#define	USB_STDREQ_RECIP_INTERFACE 1
#define	USB_STDREQ_RECIP_ENDPOINT  2
#define	USB_STDREQ_RECIP_OTHER     3

#define	USB_STDREQ_REQTYPE(DIR, TYPE, RECIP) ( \
		((  DIR << USB_STDREQ_DIR_S  ) & USB_STDREQ_DIR  ) | \
		(( TYPE << USB_STDREQ_TYPE_S ) & USB_STDREQ_TYPE ) | \
		((RECIP << USB_STDREQ_RECIP_S) & USB_STDREQ_RECIP) )

typedef enum _usb_stdreq_cmd_t
{
	USB_STDREQ_GET_STATUS        =  0, /**< Get specified recipient's status. */
	USB_STDREQ_CLEAR_FEATURE     =  1, /**< Clear of disable a specific
	                                        feature.                          */
	USB_STDREQ_SET_FEATURE       =  3, /**< Set or enable a specific feature. */
	USB_STDREQ_SET_ADDRESS       =  5, /**< Set device's address for all
	                                        future accesses.                  */
	USB_STDREQ_GET_DESCRIPTOR    =  6, /**< Get specified descriptor if it
	                                        exists.                           */
	USB_STDREQ_SET_DESCRIPTOR    =  7, /**< [Optional] Update existing or add
	                                        new descriptors.                  */
	USB_STDREQ_GET_CONFIGURATION =  8, /**< Get current device's configuration
	                                        value.                            */
	USB_STDREQ_SET_CONFIGURATION =  9, /**< Set device configuration.         */
	USB_STDREQ_GET_INTERFACE     = 10, /**< Get selected alternate setting
	                                        for the specified interface.      */
	USB_STDREQ_SET_INTERFACE     = 11, /**< Select an alternate setting for
	                                        the specified interface.          */
	USB_STDREQ_SYNCH_FRAME       = 12, /**< Set and report an endpoint's sync
	                                        frame.                            */
} usb_stdreq_cmd_t;

typedef enum _usb_stddesc_t
{
	USB_STDDESC_DEVICE           = 1, /**< Device descriptor.                 */
	USB_STDDESC_CONFIGURATION    = 2, /**< Configuration descriptor.          */
	USB_STDDESC_STRING           = 3, /**< [Optional] String descriptor.      */
	USB_STDDESC_INTERFACE        = 4, /**< Interface descriptor.              */
	USB_STDDESC_ENDPOINT         = 5, /**< Endpoint descriptor.               */
	USB_STDDESC_DEVICE_QUALIFIER = 6, /**< Describes what information about
	                                       the device would change if it
                                           operated at another speed.         */
	USB_STDDESC_OTHER_SPEED_CFG  = 7, /**< Configuration descriptor if the
	                                       device were to operate at its other
	                                       possible speed.                    */
	USB_STDDESC_INTERFACE_POWER  = 8, /**< N/A.                               */
} usb_stddesc_t;

typedef enum _usb_stdfeat_sel_t
{
	USB_STDFEAT_SEL_DEVICE_REMOTE_WAKEUP  = 1,
	USB_STDFEAT_SEL_ENDPOINT_HALT         = 0,
	USB_STDFEAT_SEL_TEST_MODE             = 2,
} usb_stdfeat_sel_t;

typedef enum _usb_class_t
{
	USB_CLASS_NA             = 0x00, /**< (Device) Use class information in
	                                      the Interface Descriptors.          */
	USB_CLASS_AUDIO          = 0x01, /**< (Interface) Audio.                  */
	USB_CLASS_CDC_CTRL       = 0x02, /**< (Both) Communications and CDC
	                                      Control.                            */
	USB_CLASS_HID            = 0x03, /**< (Interface) HID (Human Interface
	                                      Device).                            */
	USB_CLASS_PHYSICAL       = 0x05, /**< (Interface) Physical.               */
	USB_CLASS_IMAGE          = 0x06, /**< (Interface) Image.                  */
	USB_CLASS_PRINTER        = 0x07, /**< (Interface) Printer.                */
	USB_CLASS_MASS_STORAGE   = 0x08, /**< (Interface) Mass Storage.           */
	USB_CLASS_HUB            = 0x09, /**< (Device) Hub.                       */
	USB_CLASS_CDC_DATA       = 0x0A, /**< (Interface) CDC-Data.               */
	USB_CLASS_SMART_CARD     = 0x0B, /**< (Interface) Smart Card.             */
	USB_CLASS_CONT_SCRTY     = 0x0D, /**< (Interface) Content Security.       */
	USB_CLASS_VIDEO          = 0x0E, /**< (Interface) Video.                  */
	USB_CLASS_HEALTHCARE     = 0x0F, /**< (Interface) Personal Healthcare.    */
	USB_CLASS_AU_VI_DEVICES  = 0x10, /**< (Interface) Audio/Video Devices.    */
	USB_CLASS_BILLBOARD      = 0x11, /**< (Device) Billboard Device Class.    */
	USB_CLASS_DIAGNOSTIC     = 0xDC, /**< (Both) Diagnostic Device.           */
	USB_CLASS_WIRELESS       = 0xE0, /**< (Interface) Wireless Controller.    */
	USB_CLASS_MISC           = 0xEF, /**< (Both) Miscellaneous.               */

	USB_CLASS_APP_SPECIFIC   = 0xFE, /**< (Interface) Application Specific.   */
	USB_CLASS_VENDOR_SPECIFI = 0xFF, /**< (Both) Vendor Specific.             */
} usb_class_t;


/**  @} USBD */
#endif  /* __USB_STD_H__ */

#ifndef USB_HID_H
#define USB_HID_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_DRV USB Drivers
 * @{
 * @addtogroup USB_HID Human Interface Device
 * @brief USB HID driver header
 *
 * @{
 */


/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "usb.h"


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[macros]=================================================*/

/** @brief Number of HID devices supported by the driver. */
#define  USB_HID_MAX_DEVICES  1


/**
 * @name HID descriptor
 * @{
 */

/** @brief HID descriptor's size. */
#define HID_DESC_HID_SIZE  9

/** @brief Get HID descriptor's bLength from buffer x. */
#define USB_HID_DESC_HID_GET_bLength(x) \
   ((uint8_t ) (x)[0])
/** @brief Get HID descriptor's bDescriptorType from buffer x. */
#define USB_HID_DESC_HID_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
/** @brief Get HID descriptor's bcdHID from buffer x. */
#define USB_HID_DESC_HID_GET_bcdHID(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[3] << 8) | (x)[2]))
/** @brief Get HID descriptor's bCountryCode from buffer x. */
#define USB_HID_DESC_HID_GET_bCountryCode(x) \
   ((uint8_t ) (x)[4])
/** @brief Get HID descriptor's bNumDescriptors from buffer x. */
#define USB_HID_DESC_HID_GET_bNumDescriptors(x) \
   ((uint8_t ) (x)[5])
/** @brief Get HID descriptor's bClassDescriptorType from buffer x. */
#define USB_HID_DESC_HID_GET_bClassDescriptorType(x) \
   ((uint8_t ) (x)[6])
/** @brief Get HID descriptor's wDescriptorLength from buffer x. */
#define USB_HID_DESC_HID_GET_wDescriptorLength(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[8] << 8) | (x)[7]))

/** @} HID descriptor */


/**
 * @name HID device
 * @{
 */

/** @brief HID transfers and requests' buffer maximum length. */
#define USB_HID_BUFF_LEN  128

/** @brief Whether device is free or currently in use. */
#define USB_HID_STATUS_FREE    (1 << 0)
/** @brief Whether device has already been initialized. */
#define USB_HID_STATUS_INIT    (1 << 1)
/** @brief Whether device has been opened by user code. */
#define USB_HID_STATUS_OPEN    (1 << 2)
/** @brief Whether device uses an extra interrupt OUT endpoint for control. */
#define USB_HID_STATUS_INTOUT  (1 << 3)
/** @brief Whether entry actions are to be executed.  */
#define USB_HID_STATUS_ENTRY   (1 << 4)

/** @} HID device */


/*==================[typedef]================================================*/


/** @brief HID protocol number. */
typedef enum _usb_hid_protocol_t
{
   USB_HID_PROTO_NONE  = 0,  /**< None.     */
   USB_HID_PROTO_KEYB  = 1,  /**< Keyboard. */
   USB_HID_PROTO_MOUSE = 2,  /**< Mouse.    */
} usb_hid_protocol_t;

/** @brief Type of HID descriptor. */
typedef enum _usb_hid_desc_type_t
{
   USB_HID_DESC_TYPE_HID      = 0x21,  /**< HID descriptor.      */
   USB_HID_DESC_TYPE_REPORT   = 0x22,  /**< Report descriptor.   */
   USB_HID_DESC_TYPE_PHYSICAL = 0x23,  /**< Physical descriptor. */
} usb_hid_desc_type_t;

/** @brief HID device's possible states. */
typedef enum _usb_hid_state_t
{
   USB_HID_STATE_IDLE,     /**< IDLE, waiting for interface assignment.   */
   USB_HID_STATE_SET_IDLE, /**< Send SET_IDLE request, is this necessary? */
   USB_HID_STATE_INIT,     /**< TODO */
   USB_HID_STATE_INIT2,    /**< TODO */
   USB_HID_STATE_RUNNING,  /**< TODO */
} usb_hid_state_t;

/**
 * @name HID descriptor
 * @{
 */
/**
 * @brief HID descriptor data structure.
 *
 * Structure fields have been reordered to account for byte alignment.
 *
 * @warning This is \b not the complete descriptor because  it  has  a  variable
 * number of members depending on the value of wDescriptorLength.
 */
typedef struct _usb_hid_desc_hid_t
{
   uint16_t bcdHID;               /**< HID Class specification release.       */
   uint16_t wDescriptorLength;    /**< Total size of the Report descriptor.   */
   uint8_t  bLength;              /**< Total size of HID descriptor.          */
   uint8_t  bDescriptorType;      /**< Type of HID descriptor.                */
   uint8_t  bCountryCode;         /**< Country code, see _usb_hid_ctrycode_t. */
   uint8_t  bNumDescriptors;      /**< Number of class descriptors.           */
   uint8_t  bClassDescriptorType; /**< Type of class descriptor.              */
} usb_hid_desc_hid_t;
/** @} HID descriptor */


/** @brief HID standard request types. */
enum
{
   USB_HID_REQ_GET_REPORT   = 0x01, /**< Get report request, mandatory!       */
   USB_HID_REQ_GET_IDLE     = 0x02, /**< Get IDLE status request.             */
   USB_HID_REQ_GET_PROTOCOL = 0x03, /**< Get active protocol request.         */
   USB_HID_REQ_SET_REPORT   = 0x09, /**< Set report request.                  */
   USB_HID_REQ_SET_IDLE     = 0x0A, /**< Set IDLE request.                    */
   USB_HID_REQ_SET_PROTOCOL = 0x0B, /**< Set new protocol request.            */
};

/** @brief Country code of localized hardware. */
typedef enum _usb_hid_ctrycode_t
{
   USB_HID_CTRYCODE_NOT_SUPPORTED       =  0,
   USB_HID_CTRYCODE_ARABIC              =  1,
   USB_HID_CTRYCODE_BELGIAN             =  2,
   USB_HID_CTRYCODE_CANADIAN_BILINGUAL  =  3,
   USB_HID_CTRYCODE_CANADIAN_FRENCH     =  4,
   USB_HID_CTRYCODE_CZECH_REPUBLIC      =  5,
   USB_HID_CTRYCODE_DANISH              =  6,
   USB_HID_CTRYCODE_FINNISH             =  7,
   USB_HID_CTRYCODE_FRENCH              =  8,
   USB_HID_CTRYCODE_GERMAN              =  9,
   USB_HID_CTRYCODE_GREEK               = 10,
   USB_HID_CTRYCODE_HEBREW              = 11,
   USB_HID_CTRYCODE_HUNGARY             = 12,
   USB_HID_CTRYCODE_INTERNATIONAL_ISO   = 13,
   USB_HID_CTRYCODE_ITALIAN             = 14,
   USB_HID_CTRYCODE_JAPAN_KATAKANA      = 15,
   USB_HID_CTRYCODE_KOREAN              = 16,
   USB_HID_CTRYCODE_LATIN_AMERICAN      = 17,
   USB_HID_CTRYCODE_NETHERLANDS_DUTCH   = 18,
   USB_HID_CTRYCODE_NORWEGIAN           = 19,
   USB_HID_CTRYCODE_PERSIAN_FARSI       = 20,
   USB_HID_CTRYCODE_POLAND              = 21,
   USB_HID_CTRYCODE_PORTUGUESE          = 22,
   USB_HID_CTRYCODE_RUSSIA              = 23,
   USB_HID_CTRYCODE_SLOVAKIA            = 24,
   USB_HID_CTRYCODE_SPANISH             = 25,
   USB_HID_CTRYCODE_SWEDISH             = 26,
   USB_HID_CTRYCODE_SWISS_FRENCH        = 27,
   USB_HID_CTRYCODE_SWISS_GERMAN        = 28,
   USB_HID_CTRYCODE_SWITZERLAND         = 29,
   USB_HID_CTRYCODE_TAIWAN              = 30,
   USB_HID_CTRYCODE_TURKISH_Q           = 31,
   USB_HID_CTRYCODE_UK                  = 32,
   USB_HID_CTRYCODE_US                  = 33,
   USB_HID_CTRYCODE_YUGOSLAVIA          = 34,
   USB_HID_CTRYCODE_TURKISH_F           = 35,
} usb_hid_ctrycode_t;

/**
 * @name HID device
 * @{
 */
/** @brief Basic HID device structure. */
typedef struct _usb_hid_dev_t
{
   usb_stack_t*       pstack;     /**< USB stack the device belongs to.       */
   usb_hid_state_t    state;      /**< Device's state.                        */
   usb_hid_protocol_t protocol;   /**< Device's protocol.                     */
   usb_hid_ctrycode_t ctry_code;  /**< Country code.                          */
   uint32_t           status;     /**< Status, see constant definitions.      */
   uint16_t           id;         /**< Device's ID within the USB stack.      */
   uint8_t            report[USB_HID_BUFF_LEN];/**< Report's buffer, capped at
                                                    USB_HID_BUFF_LEN.         */
   uint8_t            report_len; /**< Report's length, capped at
                                       SB_HID_BUFF_LEN.                       */
} usb_hid_dev_t;
/** @} HID device */

/**
 * @brief HID device's stack.
 *
 * This structure will hold all the HID devices information and current state.
 */
typedef struct _usb_hid_stack_t
{
   usb_hid_dev_t devices[USB_HID_MAX_DEVICES]; /**< HID devices.              */
   uint8_t   n_devices;     /**< Number of HID devices currently connected.   */
} usb_hid_stack_t;


/*==================[external functions declaration]=========================*/

/**
 * @brief Driver registration probing function.
 * 
 * HID interfaces must have one other interrupt IN endpoint,  mandatory,  and  a
 * second interrupt OUT one, optional. HID  descriptor  will  not  be  validated
 * until assignment.
 *
 * @param buffer  Buffer containing the entire interface descriptor.
 * @param len     Buffer's length.
 */
int usb_hid_probe( const uint8_t* buffer, uint8_t len );

/**
 * @brief Driver registration assignment function.
 *
 * This method assumes you previously probed the interface  with  usb_hid_probe,
 * his means it will \b not check whether it's the correct  descriptor  and  its
 * characteristics, only what wasn't validated before.
 *
 * @param buffer  Buffer containing the entire interface descriptor.
 * @param len     Buffer's length.
 */
int usb_hid_assign(
      usb_stack_t*   pstack,
      uint16_t       id,
      const uint8_t* buffer,
      uint8_t        length
);

/**
 * @brief Driver registration removal function.
 *
 * @param pstack Pointer to the USB stack this device belongs to.
 * @param id     Device's unique identifier within the specified USB stack.
 */
int usb_hid_remove( usb_stack_t* pstack, uint16_t id );


/******************************************************************************/

/* Public interface */

/**
 * @brief Initialize HID stack.
 * @return Non-zero on error.
 */
int usb_hid_init( void );

/**
 * @brief Update HID stack.
 * @return Non-zero on error.
 */
int usb_hid_update( void );


/* POSIX interface */

/**
 * @brief Open HID device pointed to by the given \b pathname.
 *
 * @param pathname  Desired device's path.
 * @param flags     Unused at the moment.
 *
 * @return Non negative integer on success.
 */
int usb_hid_open( const char *pathname, int flags );

/**
 * @brief Close HID device \b fd previously opened with usb_hid_open().
 * @return Non-zero on error.
 */
int usb_hid_close( int fd );

/**
 * @brief Attempt to read \b count bytes from HID device \b fd into \b buf.
 * @return Number of bytes successfully read.
 */
size_t usb_hid_read( int fd, void *buf, size_t count );

/**
 * @brief Attempt to write \b count bytes from \b buf into HID device \b fd.
 * @return Number of bytes successfully written.
 */
int usb_hid_write( int fd, const void *buf, size_t count );


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_HID */
/** @} USB_DRV */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif /* USB_DESC_H */


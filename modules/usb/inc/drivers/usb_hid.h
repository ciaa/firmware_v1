#ifndef USB_HID_H
#define USB_HID_H
/**
* @addtogroup USB_HID
* @brief CIAA HID driver
*
* @{ */


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "usb.h"


/** @brief Number of HID devices supported by the driver. */
#define  HID_MAX_DEVICES  1


/** @brief HID protocol number. */
typedef enum _hid_protocol_t
{
   HID_PROTO_NONE  = 0,  /**< None.     */
   HID_PROTO_KEYB  = 1,  /**< Keyboard. */
   HID_PROTO_MOUSE = 2,  /**< Mouse.    */
} hid_protocol_t;

/** @brief Type of HID descriptor. */
typedef enum _hid_desc_type_t
{
   HID_DESC_TYPE_HID      = 0x21,  /**< HID descriptor.      */
   HID_DESC_TYPE_REPORT   = 0x22,  /**< Report descriptor.   */
   HID_DESC_TYPE_PHYSICAL = 0x23,  /**< Physical descriptor. */
} hid_desc_type_t;

/** @brief HID device's possible states. */
typedef enum _hid_dev_state_t
{
   HID_STATE_IDLE,     /**< IDLE, waiting for interface assignment. */
   HID_STATE_SET_IDLE, /**< Send SET_IDLE request to device, is this necessary? */
   HID_STATE_INIT,     /**< TODO */
   HID_STATE_INIT2,    /**< TODO */
   HID_STATE_RUNNING,  /**< TODO */
} hid_state_t;

/**
 * @brief HID descriptor.
 *
 * Structure fields have been reordered to account for byte alignment.
 *
 * @warning This is \b not the complete descriptor because  it  has  a  variable
 * number of members depending on the value of wDescriptorLength.
 */
typedef struct _hid_desc_hid_t
{
   uint16_t bcdHID;               /**< HID Class specification release.       */
   uint16_t wDescriptorLength;    /**< Total size of the Report descriptor.   */
   uint8_t  bLength;              /**< Total size of HID descriptor.          */
   uint8_t  bDescriptorType;      /**< Type of HID descriptor.                */
   uint8_t  bCountryCode;         /**< Country code, see _hid_ctrycode_t.     */
   uint8_t  bNumDescriptors;      /**< Number of class descriptors.           */
   uint8_t  bClassDescriptorType; /**< Type of class descriptor.              */
} hid_desc_hid_t;

/** @brief HID descriptor's size. */
#define HID_DESC_HID_SIZE  9

#define HID_DESC_HID_GET_bLength(x) \
   ((uint8_t ) (x)[0])
#define HID_DESC_HID_GET_bDescriptorType(x) \
   ((uint8_t ) (x)[1])
#define HID_DESC_HID_GET_bcdHID(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[3] << 8) | (x)[2]))
#define HID_DESC_HID_GET_bCountryCode(x) \
   ((uint8_t ) (x)[4])
#define HID_DESC_HID_GET_bNumDescriptors(x) \
   ((uint8_t ) (x)[5])
#define HID_DESC_HID_GET_bClassDescriptorType(x) \
   ((uint8_t ) (x)[6])
#define HID_DESC_HID_GET_wDescriptorLength(x) \
   USB_ARCH_ENDIANNESS16((uint16_t) (((x)[8] << 8) | (x)[7]))


/** @brief HID standard request types. */
enum
{
   HID_REQ_GET_REPORT   = 0x01, /**< Get report request, this one's mandatory!*/
   HID_REQ_GET_IDLE     = 0x02, /**< Get IDLE status request.                 */
   HID_REQ_GET_PROTOCOL = 0x03, /**< Get active protocol request.             */
   HID_REQ_SET_REPORT   = 0x09, /**< Set report request.                      */
   HID_REQ_SET_IDLE     = 0x0A, /**< Set IDLE request.                        */
   HID_REQ_SET_PROTOCOL = 0x0B, /**< Set new protocol request.                */
};

/** @brief Country code of localized hardware. */
typedef enum _hid_ctrycode_t
{
   HID_CTRYCODE_NOT_SUPPORTED       =  0,
   HID_CTRYCODE_ARABIC              =  1,
   HID_CTRYCODE_BELGIAN             =  2,
   HID_CTRYCODE_CANADIAN_BILINGUAL  =  3,
   HID_CTRYCODE_CANADIAN_FRENCH     =  4,
   HID_CTRYCODE_CZECH_REPUBLIC      =  5,
   HID_CTRYCODE_DANISH              =  6,
   HID_CTRYCODE_FINNISH             =  7,
   HID_CTRYCODE_FRENCH              =  8,
   HID_CTRYCODE_GERMAN              =  9,
   HID_CTRYCODE_GREEK               = 10,
   HID_CTRYCODE_HEBREW              = 11,
   HID_CTRYCODE_HUNGARY             = 12,
   HID_CTRYCODE_INTERNATIONAL_ISO   = 13,
   HID_CTRYCODE_ITALIAN             = 14,
   HID_CTRYCODE_JAPAN_KATAKANA      = 15,
   HID_CTRYCODE_KOREAN              = 16,
   HID_CTRYCODE_LATIN_AMERICAN      = 17,
   HID_CTRYCODE_NETHERLANDS_DUTCH   = 18,
   HID_CTRYCODE_NORWEGIAN           = 19,
   HID_CTRYCODE_PERSIAN_FARSI       = 20,
   HID_CTRYCODE_POLAND              = 21,
   HID_CTRYCODE_PORTUGUESE          = 22,
   HID_CTRYCODE_RUSSIA              = 23,
   HID_CTRYCODE_SLOVAKIA            = 24,
   HID_CTRYCODE_SPANISH             = 25,
   HID_CTRYCODE_SWEDISH             = 26,
   HID_CTRYCODE_SWISS_FRENCH        = 27,
   HID_CTRYCODE_SWISS_GERMAN        = 28,
   HID_CTRYCODE_SWITZERLAND         = 29,
   HID_CTRYCODE_TAIWAN              = 30,
   HID_CTRYCODE_TURKISH_Q           = 31,
   HID_CTRYCODE_UK                  = 32,
   HID_CTRYCODE_US                  = 33,
   HID_CTRYCODE_YUGOSLAVIA          = 34,
   HID_CTRYCODE_TURKISH_F           = 35,
} hid_ctrycode_t;


/** @brief Basic HID device structure. */
typedef struct _hid_dev_t
{
   hid_state_t    state;      /**< Device's state.                            */
   uint32_t       status;     /**< Object's status, see constant definitions. */
   usb_stack_t*   pstack;     /**< USB stack the device belongs to.           */
   uint16_t       id;         /**< Device's ID within the USB stack.          */
   hid_protocol_t protocol;   /**< Device's protocol.                         */
   hid_ctrycode_t ctry_code;  /**< Country code.                              */
   uint8_t        report[256];/**< Report's buffer, capped at 256 bytes.      */
   uint8_t        report_len; /**< Report's length, capped at 256 bytes.      */
} hid_dev_t;

/** @brief Whether device is free or currently in use. */
#define HID_STATUS_FREE    (1 << 0)
/** @brief Whether device has already been initialized. */
#define HID_STATUS_INIT    (1 << 1)
/** @brief Whether device has been opened by user code. */
#define HID_STATUS_OPEN    (1 << 2)
/** @brief Whether device uses an extra interrupt OUT endpoint for control. */
#define HID_STATUS_INTOUT  (1 << 3)
/**< Whether entry actions are to be executed.  */
#define HID_STATUS_ENTRY   (1 << 4)

/**
 * @brief HID device's stack.
 *
 * This structure will hold all the HID devices information and current state.
 */
typedef struct _hid_stack_t
{
   hid_dev_t devices[HID_MAX_DEVICES]; /**< HID devices state and members.    */
   uint8_t   n_devices;     /**< Number of HID devices currently connected.   */
} hid_stack_t;


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
int hid_probe( const uint8_t* buffer, uint8_t len );

/**
 * @brief Driver registration assignment function.
 *
 * This method assumes you previously probed the interface with hid_probe,  this
 * means it will \b not check  whether  it's  the  correct  descriptor  and  its
 * characteristics, only what wasn't validated before.
 *
 * @param buffer  Buffer containing the entire interface descriptor.
 * @param len     Buffer's length.
 */
int hid_assign(
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
int hid_remove( usb_stack_t* pstack, uint16_t id );


/******************************************************************************/

/* Public interface */

/**
 * @brief Initialize HID stack.
 * @return Non-zero on error.
 */
int hid_init( void );

/**
 * @brief Update HID stack.
 * @return Non-zero on error.
 */
int hid_update( void );


/* POSIX interface */

/**
 * @brief Open HID device pointed to by the given \b pathname.
 *
 * @param pathname  Desired device's path.
 * @param flags     Unused at the moment.
 *
 * @return Non negative integer on success.
 */
int hid_open( const char *pathname, int flags );

/**
 * @brief Close HID device \b fd previously opened with hid_open().
 * @return Non-zero on error.
 */
int hid_close( int fd );

/**
 * @brief Attempt to read \b count bytes from HID device \b fd into \b buf.
 * @return Number of bytes successfully read.
 */
size_t hid_read( int fd, void *buf, size_t count );

/**
 * @brief Attempt to write \b count bytes from \b buf into HID device \b fd.
 * @return Number of bytes successfully written.
 */
int hid_write( int fd, const void *buf, size_t count );


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/**  @} USB_HID */
#endif /* USB_HID_H */

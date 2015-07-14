#ifndef __USB_HID_H__
#define __USB_HID_H__
/**
* @addtogroup USB_HID
* @brief CIAA HID driver
*
* @{ */

#include <stdint.h>

#include "usb.h"


#define	HID_MAX_DEVICES  1


typedef enum _hid_protocol_t
{
	HID_PROTO_NONE  = 0,
	HID_PROTO_KEYB  = 1,
	HID_PROTO_MOUSE = 2,
} hid_protocol_t;

typedef enum _hid_dev_state_t
{
	HID_STATE_IDLE,
	HID_STATE_INIT,
} hid_dev_state_t;


typedef struct _hid_dev_t
{
	uint32_t        status;  /**< Object's status, see constant definitions.  */
	hid_dev_state_t state;   /**< Device's state.                             */
	usb_stack_t*    pstack;  /**< USB stack the device belongs to.            */
	uint16_t        id;      /**< Device's ID within the USB stack.           */
	hid_protocol_t  protocol;
} hid_dev_t;

typedef struct _hid_stack_t
{
	hid_dev_t devices[HID_MAX_DEVICES];
	uint8_t   n_devices;
} hid_stack_t;


/** @brief Whether object is free or currently in use. */
#define	HID_STATUS_FREE    (1 << 0)
/**
 * @brief '1 if device communicates through an interrupt endpoint, '0 if it only
 * uses the default control one.
 */
#define	HID_STATUS_INT_EP  (1 << 1)
/** @brief Whether device has already been initialized. */
#define	HID_STATUS_INIT    (1 << 2)
/** @brief Whether device has been opened by user code. */
#define	HID_STATUS_OPEN    (1 << 3)


/**
 * @brief Driver registration probing function.
 */
int hid_probe( const uint8_t* buffer, const uint8_t len );
/**
 * @brief Driver registration assignment function.
 */
int hid_assign( usb_stack_t* pstack, const uint16_t id );
/**
 * @brief Driver registration removal function.
 */
int hid_remove( usb_stack_t* pstack, const uint16_t id );


/******************************************************************************/

int hid_init( void );

int hid_dev_deinit( uint8_t index );

int hid_get_free_dev( void );

int hid_update( void );

int hid_update_device( uint8_t index );

int hid_open_dev( const char* str_num, int flags, hid_protocol_t protocol );


/* POSIX */
/**
 * @FIXME the following POSIX interface is beign  thrown  together  for  testing
 * purposes at the moment.
 */

int hid_open(const char *pathname, int flags);

int hid_close(int fd);

size_t hid_read(int fd, void *buf, size_t count);

int hid_write(int fd, const void *buf, size_t count);


/**  @} USB_HID */
#endif /* __USB_HID_H__ */

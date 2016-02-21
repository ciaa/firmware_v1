#ifndef USB_H
#define USB_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_COMMON Common Types & Structures
 * @{
 * @brief USB common types and structures header
 *
 * At the moment, only the most important information is stored in memory, other
 * attributes needed will have to be requested from the device via IRPs.
 *
 * @warning
 * All devices use the same  structure  with  fixed  number  of  interfaces  and
 * endpoints per interface.  This is not the most efficient way of using  memory
 * since the "biggest"  device  will  make  all  the  others  potentially  waste
 * resources.  One  way  to  improve  this  would  be  generating  the  device's
 * structure with a script (like the php OSEK one) depending on  what  the  user
 *  needs.   This  is  also  particularly  important   regarding   the   control
 * transactions buffer's size, since it will need to be as large as the  largest
 * configuration descriptor among all devices (which can be quite so in  complex
 * devices such as cellphones).
 *
 * @{
 */


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "usb_cfg.h"
#include "usb_std.h"
#include "usb_devices_cfg.h"


/*==================[macros]=================================================*/

/**
 * @name Endianness adaptation.
 * @{
 *
 * @TODO Rename the following test macro (USE_BIG_ENDIAN) accordingly to  what's
 * used in the toolchain.
 */
#ifdef USE_BIG_ENDIAN

/**
 * @brief Macro for converting a 32bit word from USB to machine  and  vice-versa
 * endianness.
 * @warning Remember USB bus transfers are little-endian.
 */
# define USB_ARCH_ENDIANNESS32(x) ((((x) & 0xFF000000) >> 24) | \
                                   (((x) & 0x00FF0000) >>  8) | \
                                   (((x) & 0x0000FF00) <<  8) | \
                                   (((x) & 0x000000FF) << 24))
/**
 * @brief Macro for converting a 16bit word from USB to machine  and  vice-versa
 * endianness.
 * @warning Remember USB bus transfers are little-endian.
 */
# define USB_ARCH_ENDIANNESS16(x) ((((x) & 0xFF00) >> 8) | \
                                   (((x) & 0x00FF) << 8))
#else
# define USB_ARCH_ENDIANNESS32(x)    (x)
# define USB_ARCH_ENDIANNESS16(x)    (x)
#endif
/** @} Endianness adaptation. */


/** @brief Address mask.  */
#define USB_ADDR_MASK         0x7F
/** @brief Direction mask.  */
#define USB_DIR_MASK          0x80
/** @brief Direction shift.  */
#define USB_DIR_S             7


/** @brief Per device control transfer's buffer length. */
#define USB_XFER_BUFFER_LEN   256


/**
 * @brief Use this pipe value instead of  an  actual  one  when  requesting  the
 * status of a control transfer.
 */
#define  USB_CTRL_PIPE_TOKEN  ((uint8_t) -1)


/** @brief Used to identify an interface with no driver associated to it. */
#define USB_IFACE_NO_DRIVER ((usb_driver_handle_t)-1)


/** @brief Default device's address, invalid. */
#define USB_DEV_DEFAULT_ADDR ((uint8_t) -1)


/** @brief Minimum bInterval value for FS/HS ISO or HS INT. */
#define USB_ISO_INTHS_MIN_bInt  1
/** @brief Maximum bInterval value for FS/HS ISO or HS INT. */
#define USB_ISO_INTHS_MAX_bInt 16


/** @brief Maximum number of transfer retries before failing. */
#define USB_MAX_XFER_RETRIES    3


/** @brief Device active and connected. */
#define USB_DEV_STATUS_ACTIVE          (1 << 0)
/** @brief Device enumerated and initialized. */
#define USB_DEV_STATUS_INIT            (1 << 1)
/** @brief Device is self-powered. */
#define USB_DEV_STATUS_SELF_PWRD       (1 << 2)
/** @brief Device supports remote wakeup. */
#define USB_DEV_STATUS_REMOTE_WKUP     (1 << 3)
/**
 * @brief Whether the device has requested a control transfer, this will be used
 * entirely by the Host during initialization  so  we  know  no  more  than  one
 * request at a time will be active (no matter the number of interfaces).
 */
#define USB_DEV_STATUS_REQUEST         (1 << 4)
/** @brief Root HUB/device identifier.  */
#define USB_DEV_PARENT_ROOT            ((uint8_t) -1)


/** @brief Currently writing to a device on address 0. */
#define USB_STACK_STATUS_ZERO_ADDR  (1 << 0)
/** @brief Marks a HUB index as unused. */
#define USB_STACK_INVALID_IDX       USB_DEV_PARENT_ROOT


/**
 * @brief Use this driver/vendor ID when  the  driver  should  NOT  target  only
 * devices with the specified ID.  This means a driver can be  assigned  to  any
 * interface with the necessary requirements no matter the device's IDs.
 */
#define USB_FORCE_PROBING_ID  0xFFFF

/** @brief Convert a device ID into its device's index. */
#define USB_ID_TO_DEV(id)      ((uint8_t ) ((id) >> 8))

/** @brief Convert a device ID into its device's interface index. */
#define USB_ID_TO_IFACE(id)    ((uint8_t ) ((id) & 0xFF))

/** @brief Convert a device and interface indices into a device ID. */
#define USB_TO_ID(dev, iface)  ((uint16_t) (((dev) << 8) | (iface)))

/** @brief Convert message pipe's index to ticket, to return by _ctrlirp(). */
#define USB_IDX_TO_TICKET(idx) ((uint16_t) (((idx) + 1) << 8))

/** @brief Convert ticket back to message pipe's index, used by _irp_status().*/
#define USB_TICKET_TO_IDX(tkt) ((uint8_t)  (((tkt) >> 8) - 1))

/** @brief Check whether IRP ID corresponds to message pipe or otherwise. */
#define USB_IRP_ID_IS_MSG(id)  ((id) & 0xFF00)


/*==================[typedef]================================================*/

/**
 * @brief Status codes.
 *
 */
typedef enum _usb_status_t
{
   USB_STATUS_OK = 0,         /**< Success.                                   */
   USB_STATUS_INV_PARAM,      /**< Invalid argument/parameter to function.    */

   USB_STATUS_DEV_NOT_FOUND,  /**< Device not found in any HUB.               */
   USB_STATUS_DRIVER_NA,      /**< No driver found for device.                */
   USB_STATUS_EP_NA,          /**< No endpoint descriptor found.              */
   USB_STATUS_DRIVER_FAIL,    /**< Driver could not identify device.          */

   USB_STATUS_PIPE_CFG,       /**< Unable to setup pipe.                      */
   USB_STATUS_HCD_INIT,       /**< Cannot initialize Host Controller Driver.  */

   USB_STATUS_INV_DESC,       /**< Invalid descriptor.                        */
   USB_STATUS_EP_AVAIL,       /**< Not enough pipes available.                */

   USB_STATUS_EP_STALLED,
   USB_STATUS_DEV_UNREACHABLE,
   USB_STATUS_BUSY,           /**< Control endpoint is currently busy.        */

   USB_STATUS_XFER_ERR,       /**< Error during transfer.                     */
   USB_STATUS_XFER_WAIT,      /**< Transfer in progress.                      */
   USB_STATUS_XFER_RETRY,     /**< Retry last transfer.                       */
} usb_status_t;


/** @brief Device's speed identifier.  */
typedef enum _usb_speed_t
{
   USB_SPD_LS,  /**< Low-speed.     */
   USB_SPD_FS,  /**< Full-speed.    */
   USB_SPD_HS,  /**< High-speed.    */
   USB_SPD_INV  /**< Invalid/error. */
} usb_speed_t;


/** @brief Endpoint/Pipe's direction.  */
typedef enum _usb_dir_t
{
   USB_DIR_OUT = 0,  /**< Towards device from host.     */
   USB_DIR_IN  = 1,  /**< Towards host from device.     */
   USB_DIR_TOK = 2,  /**< Token: for control transfers. */
} usb_dir_t;


/** @brief Endpoint/Pipe's type.  */
typedef enum _usb_xfer_type_t
{
   USB_CTRL = 0,  /**< Control.        */
   USB_ISO  = 1,  /**< Isochronous.    */
   USB_BULK = 2,  /**< Bulk.           */
   USB_INT  = 3,  /**< Interrupt.      */
} usb_xfer_type_t;


/**
 * @brief Message pipe structure (for control endpoints).
 *
 * Pipe's direction is stored in the most significant bit of addr.
 *
 * Requested transfers will be retried up  to  3  times  whenever  an  error  is
 * encountered during transmission (or reception).  The retries counter is reset
 * once a successful transfer takes place.  If it fails 3 times in  a  row,  the
 * pipe will be placed in the STALL state  and  it  will  need  to  be  restored
 * manually by the user.
 */
typedef struct _usb_msg_pipe_t
{
   uint8_t*         buffer;       /**< Pointer to message buffer.             */
   uint8_t          stdreq[USB_STDREQ_SIZE]; /**< Standard request buffer.    */
   uint8_t          retries;      /**< Number of transfer retries so far.     */
   uint8_t          handle;       /**< Hardware pipe's handle.                */
   uint8_t          lock;         /**< Pipe's lock control.                   */
} usb_msg_pipe_t;

/**
 * @brief Stream pipe structure.
 *
 * Pipe's direction is stored in the most significant bit of addr.
 *
 * The interval represents every how many (micro)frames a transfer  is  expected
 * to happen.  For example, a period of  0  means  a  transfer  on  every  frame
 * whereas a period of 1 means a transfer on every other frame.
 *
 * Requested transfers will be retried up  to  3  times  whenever  an  error  is
 * encountered during transmission (or reception).  The retries counter is reset
 * once a successful transfer takes place.  If it fails 3 times in  a  row,  the
 * pipe will be placed in the STALL state  and  it  will  need  to  be  restored
 * manually by the user.
 * @TODO implement pipe STALL state and restoring method.
 */
typedef struct _usb_pipe_t
{
   usb_xfer_type_t  type;      /**< Type (bulk, int, iso).                    */
   usb_dir_t        dir;       /**< Endpoint's direction.                     */
   uint32_t         length;    /**< Transfer's buffer length.                 */
   uint8_t*         buffer;    /**< Pointer to transfer buffer.               */
   uint8_t          retries;   /**< Number of transfer retries so far.        */
   uint8_t          handle;    /**< Hardware pipe's handle.                   */
   uint8_t          number;    /**< Endpoint number (0 - 15).                 */
   uint8_t          mps;       /**< Maximum packet size.                      */
   uint8_t          interval;  /**< Frames between INT or ISO transfers.      */
} usb_pipe_t;


/** @brief USB driver's handle type. */
typedef uint8_t usb_driver_handle_t;

/**
 * @brief USB device structure.
 *
 * Endpoints are instantiated as pipes because, even though they are referred to
 * as endpoints on the device end, the  host  connects  to  them  through  local
 * pipes. Also, an interface cannot be created  without  its  associated  pipes,
 * thus, configuring them here is the best option.
 *
 * The driver bound to  the  interface  will  talk  to  the  device's  endpoints
 * directly, although this will be handled as needed internally.
 *
 * When the stack was unable to assign a driver to an interface, it will set the
 * driver_handle to USB_IFACE_NO_DRIVER.
 *
 * @warning The protocol member can only take values from 0 to 253, both 254 and
 * 255 are reserved for internal USB stack usage.
 */
typedef struct _usb_interface_t
{
   usb_pipe_t*const    endpoints;      /**< Array of endpoints.               */
   usb_driver_handle_t driver_handle;  /**< Interface's driver handle.        */
   uint8_t             class;
   uint8_t             subclass;
   uint8_t             protocol;
} usb_interface_t;


/**
 * @brief Device's possible states from the stacks perspective.
 *
 * This are all the possible states a device can be in according to the  USB 2.0
 * specification, plus the DISCONNECTED state to represent a device not  present
 * and a couple more used internally by the host for setup and enumeration.
 *
 * The enumeration and driver bonding process are handled from state POWERED  to
 * CONFIGURED.
 */
typedef enum _usb_dev_state_t
{
   USB_DEV_STATE_WAIT_DELAY,        /**< Waiting for a delay to expire.       */
   USB_DEV_STATE_DISCONNECTED,      /**< No device connected.                 */
   USB_DEV_STATE_ATTACHED,          /**< Attached, waiting for power.         */
   USB_DEV_STATE_POWERED,           /**< Powered, start reset and lock addr 0 */
   USB_DEV_STATE_RESET,             /**< Request a bus-reset.                 */
   USB_DEV_STATE_DEFAULT,           /**< Wait for the bus-reset to complete.  */
   USB_DEV_STATE_MPS,               /**< Request 8B of dev descriptor (MPS).  */
   USB_DEV_STATE_ADDRESS,           /**< Setup device's new address.          */
   USB_DEV_STATE_DEV_DESC,          /**< Request and parse device descriptor. */
   USB_DEV_STATE_CFG_DESC_LEN9,     /**< Request first 9B of cfg. descriptor. */
   USB_DEV_STATE_CFG_DESC,          /**< Request and parse full cfg. desc.    */
   USB_DEV_STATE_SET_CFG,           /**< Set configuration value.             */
   USB_DEV_STATE_UNLOCK,            /**< Unlock address 0.                    */
   USB_DEV_STATE_CONFIGURED,        /**< Configured, in stand by for transfers*/
   USB_DEV_STATE_SUSPENDED,         /**< Bus inactive, waiting for activity.  */
   USB_DEV_STATE_TEST,              /*   For testing purposes.                */
} usb_dev_state_t;

/**
 * @brief USB device structure.
 *
 * A device structure can have different statuses at a given time,  each  status
 * will be represented by a bit in the status member. As of  now,  there's  only
 * the 'connected' and the 'opened' status.
 *
 * 'connected' devices are being handled by drivers but no user code has claimed
 * them.
 *
 * 'opened' devices have been claimed by  user  code  and  cannot  be  otherwise
 * claimed by someone else.
 *
 * A single endpoint is used for control  pipes,  it  will  be  reconfigured  as
 * needed for every IN or OUT transaction.
 *
 * @warning The standard request buffer (stdreq) stores members data in the  USB
 * bus's byte ordering, that is, little-endian.
 *
 * @warning Control pipe's buffer can become quite  large  on  devices  with  an
 * extensive configuration descriptor, as it  is  now  this  will  affect  *all*
 * devices' structure.
 *
 * @warning The buffer's size is currently limited to 256  bytes,  however,  the
 * USB specification assigns it a 2-byte value, so this limit is  being  imposed
 * by the implementation.  On the other hand, 256 bytes should be plenty  enough
 * to handle quite some complex devices, even with multiple interfaces.
 *
 */
typedef struct _usb_device_t
{
   usb_dev_state_t  state;          /**< Device's current state.              */
   usb_dev_state_t  next_state;     /**< Device's state after a delay.        */
   usb_speed_t      speed;          /**< Speed (LS, FS or HS).                */
   usb_interface_t* interfaces;     /**< Array of interfaces.                 */
   uint32_t         status;         /**< Device status, see description.      */
   uint16_t         ticket;         /**< Ticket for following IRP messages.   */
   uint16_t         vendor_ID;      /**< Vendor ID.                           */
   uint16_t         product_ID;     /**< Product ID.                          */
   uint16_t         ticks_delay;    /**< Delay this number of ticks count.    */
#if (USB_MAX_HUBS > 0)
   uint8_t          parent_hub;     /**< Index of upstream HUB.               */
   uint8_t          parent_port;    /**< Index of upstream HUB.               */
#endif
   uint8_t          xfer_buffer[USB_XFER_BUFFER_LEN]; /**< Control buffer.    */
   uint8_t          xfer_length;    /**< Control buffer's length.             */
   uint8_t          cte_index;      /**< Device's index in usb_devices_cfg.   */
   uint8_t          max_power;      /**< Maximum consumption from the bus.    */
   uint8_t          addr;           /**< Device's address.                    */
   uint8_t          mps;            /**< Device's control EP max packet size. */
   uint8_t          cfg_value;      /**< Configuration value.                 */
} usb_device_t;

/**
 * @brief Host stack's possible states.
 *
 * This states are mostly circumstantial,  everything  from  powering  the  Vbus
 * lines to enumeration will be  handled  by  the  Host  through  each  device's
 * state-machine.
 */
typedef enum _usb_host_state_t
{
   USB_HOST_STATE_IDLE,      /**< Waiting for a device connection.            */
   USB_HOST_STATE_RUNNING,   /**< At least one device connected and running.  */
   USB_HOST_STATE_SUSPENDED  /**< Bus inactive, waiting for activity.         */
} usb_host_state_t;

/** @brief USB stack structure.  */
typedef struct _usb_stack_t
{
   uint32_t         status;     /**< Stack status, see description.           */
   uint16_t         ticks;      /**< 1-millisecond ticks count.               */
   usb_host_state_t state;      /**< Stack's current state.                   */
   usb_msg_pipe_t   def_ep[USB_N_CTRL_ENDPOINTS]; /**< Default control EPs.   */
   usb_device_t     devices[USB_MAX_DEVICES];     /**< Array of devices.      */
   uint8_t          n_devices;  /**< Number of connected devices.             */
} usb_stack_t;


/**
 * @name USB driver callbacks
 * @{
 */

/** @brief Driver probing function, see @usb_drivers_probe() for details. */
typedef int (*usb_fcn_probe_t)(const uint8_t* buffer, uint8_t len );
/** @brief Driver assignment function, see @usb_drivers_assign() for details. */
typedef int (*usb_fcn_assign_t)(
      usb_stack_t*   pstack,
      uint16_t       id,
      const uint8_t* buffer,
      uint8_t        length
);
/** @brief Driver removal function, see @usb_drivers_remove() for details. */
typedef int (*usb_fcn_remove_t)( usb_stack_t* pstack, uint16_t id );

/** @brief USB driver callbacks structure. */
typedef struct _usb_driver_t
{
   const uint16_t vendor_ID;
   /**< Only probe devices that match this vendor ID, 0xFFFF to force.        */

   const uint16_t product_ID;
   /**< Only probe devices that match this product ID, 0xFFFF to force.       */

   const usb_fcn_probe_t probe;
   /**< Probing function, to determine driver compatibility with interface.   */

   const usb_fcn_assign_t assign;
   /**< Assignment function to bind driver to interface.                      */

   const usb_fcn_remove_t remove;
   /**< Remove and unbind interface from driver.                              */
} usb_driver_t;

/** @} USB driver callbacks */


/*==================[external functions definition]==========================*/
/* This are configuration related functions. */

usb_interface_t* usb_devices_cfg_get_ifaces(
      uint16_t pid,
      uint16_t vid,
      uint8_t  n_ifaces,
      uint8_t* pindex
);


/* Check USB configurations. */
#include "usb_check.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_COMMON */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif  /* USB_H */


#ifndef __USBD_H__
#define __USBD_H__
/**
* @addtogroup USBD
* @brief CIAA USB Driver
*
* @warning USB devices' addresses are stored starting from  0  (zero),  this  is
* because internally they are represented by an index. This is  NOT  the  actual
* device's address in the USB BUS, that  one  is  the  index  @b  plus  @b  one.
* Remember that address 0 is reserved for setup and initialization.
*
*
* @par Description
*
* USB driver abstraction layer between the USB  Host  Controller  and  the  user
* software. Communication is handled through IRPs (I/O Request Packets)  to/from
* a previously claimed device's interface.
*
* As of now, only one IRP can be requested at a time per device  interface.  You
* should start transactions with usb_irp(), which is intentionally non-blocking,
* and then poll usb_irp_status() waiting for  it  to  either  fail  or  complete
* successfully.  Optionally,  you  can  cancel  an  ongoing   transaction   with
* usb_irp_cancel() as long as it hasn't been completed yet.
*
* @{ */

#include <stdint.h>
#include "usb.h"

/** FIXME: having this is a disaster, but LPCxpresso doesn't see it!!! */
#ifndef NULL
# define NULL ((void*)0)
#endif


/**
 * @brief Initialize the USB driver and hardware.
 * @param pstack Pointer to USB stack structure.
 */
int usb_init( usb_stack_t* pstack );

/**
 * @brief De-initialize the USB driver and hardware.
 * @param pstack Pointer to USB stack structure.
 */
int usb_deinit( usb_stack_t* pstack );

/**
 * @brief USB main routine, everything is processed in here.
 * @param pstack Pointer to USB stack structure.
 */
int usb_run( usb_stack_t* pstack );


/**
 * @brief Main transfers request method, I/O Request Packet.
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param pipe      Pipe number (given by  order  of  endpoints  in  interface),
 *                  transaction direction is determined by endpoint's direction.
 * @param buffer    Data buffer of 'len' length.
 * @param len       Length of data buffer.
 */
int usb_irp(
		usb_stack_t* pstack,
		uint16_t     device_id,
		uint8_t      pipe,
		uint8_t*     buffer,
		uint32_t     len
);


/**
 * @brief Control transfers request method.
 *
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param pstdreq   Standard request descriptor.
 * @param buffer    Data buffer of length  determined  by  descriptor,  NULL  if
 *                  unused.
 *
 * @retval USB_STATUS_OK   On success
 * @retval USB_STATUS_BUSY Control endpoint is currently busy, try again later.
 * @TODO complete the return values list.
 */
int usb_ctrlirp(
		usb_stack_t*        pstack,
		uint16_t            device_id,
		const usb_stdreq_t* pstdreq,
		uint8_t*            buffer
);

/**
 * @brief Get IRP's status.
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param pipe      Pipe number (given by order of endpoints in interface),  use
 *                  0 (zero) for default control pipe.
 */
int usb_irp_status( usb_stack_t* pstack, uint16_t device_id, uint8_t pipe );

/**
 * @brief Cancel ongoing IRP.
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param pipe      Pipe number (given by order of endpoints in interface).
 */
int usb_irp_cancel( usb_stack_t* pstack, uint16_t device_id, uint8_t pipe );

/**
 * @brief Increase internal tick counter by one and retrieve old value.
 * @param pstack    Pointer to USB stack structure.
 * @return Old internal tick count value.
 */
int16_t usb_systick_inc( usb_stack_t* pstack );



/**
 * TODO: whatever follows won't really be used from the outside, maybe it should be placed in some kind of private file or something...
 */

/** TODO: improve the assert function, right now it will lock in place for easy debugging. */
void usb_assert(int condition);


/**
 * @brief Relase device and deallocate its interfaces.
 * @param pdevice Pointer to USB device to release.
 */
/** TODO */


int usb_device_init( usb_stack_t* pstack, uint8_t index );

int usb_device_release( usb_stack_t* pstack, uint8_t index );

int usb_device_is_active( usb_stack_t* pstack, uint8_t index );

int usb_device_udpate( usb_stack_t* pstack, uint8_t index );

int usb_dealloc_endpoint( usb_stack_t* pstack, usb_pipe_t* ppipe );

void usb_device_attach(
		usb_stack_t*  pstack,
#if (USB_MAX_HUBS > 0)
		uint8_t       parent_hub,
		uint8_t       parent_port,
#endif
		usb_device_t* pdevice
);

#if (USB_MAX_HUBS > 0)
int usb_device_find(
		usb_stack_t* pstack,
		uint8_t      hub_index,
		uint8_t      port
);

int usb_stack_handle_hubs( usb_stack_t* pstack );
#endif

int usb_stack_new_addr( usb_stack_t* pstack );

int usb_stack_update_devices( usb_stack_t* pstack );


/* Descriptor parsing methods. */

int usb_device_parse_cfgdesc( usb_stack_t* pstack, uint8_t index );

int usb_device_parse_ifacedesc(
		usb_stack_t*    pstack,
		uint16_t        id,
		const uint8_t** pbuff,
		uint8_t*        plen
);

int usb_device_parse_epdesc( usb_pipe_t* ppipe, const uint8_t* buffer );


/**  @} USBD */
#endif  /* __USBD_H__ */

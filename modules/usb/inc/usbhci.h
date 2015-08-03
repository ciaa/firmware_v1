#ifndef USBHCI_H
#define USBHCI_H
/**
* @addtogroup USBHCI
* @brief CIAA USB Host Controller Interface
*
* @par Description
*
* Interface between the main USB driver and the hardware  Host  Controller.  The
* Host Controller methods are hardware dependent, so this interface is  intended
* to provide uniform accesses.
*
* Device- related  method  reference  whatever  device  is  currently  connected
* directly to the Host Controller's hardware. Other devices connected downstream
* through HUBs will not be accessible by this interface but through that one  of
* the  HUB  driver.  This  applies  to  usbhci_reset(),  usbhci_get_speed()  and
* usbhci_is_connected().
*
*
* @{ */

#include <stdint.h>
#include "usb.h"
#include "usb_std.h"


/**
 * @brief USB hardware initialization.
 * @return Zero on success, non-zero otherwise.
 */
int usbhci_init( void );

/**
 * @brief USB hardware de-initialization.
 * @return Zero on success, non-zero otherwise.
 */
int usbhci_deinit( void );

/**
 * @brief USB reset, block until done (10 ~ 20 ms).
 */
void usbhci_reset( void );

/**
 * @brief Get device's speed.
 */
usb_speed_t usbhci_get_speed( void );

/**
 * @brief Check for a device connection.
 *
 * This should be routinely  called  to  be  able  to  detect  when  the  device
 * disconnects.
 *
 * @return Non-zero if a device is connected, zero otherwise.
 */
int usbhci_is_connected( void );

/**
 * @brief Get next frame's number.
 */
uint32_t usbhci_get_frame_number( void );

/**
 * @brief Request new hardware pipe.
 *
 * Returns a valid pipe handle on success, a negative value otherwise.
 *
 * On success, this call should be  followed  by  usbhci_pipe_configure()  right
 * after to configure the pipe.
 *
 * @param type Type of endpoint to allocate,  not  all  type  of  endpoints  are
 *             available on every physical endpoint.
 * @return Zero on success, non-zero otherwise.
 */
int usbhci_pipe_alloc( usb_xfer_type_t type );

/**
 * @brief Release hardware pipe.
 * @param ppipe Pointer to pipe structure containing pipe's handle to release.
 */
int usbhci_pipe_dealloc( usb_pipe_t* ppipe );

/**
 * @brief Configure previously allocated hardware pipe.
 * @param ppipe  Pointer to pipe structure.
 * @param addr   Device's address.
 * @param speed  Device's speed.
 * @warning This might be merged into usbhci_pipe_alloc().
 */
int usbhci_pipe_configure( usb_pipe_t* ppipe, uint8_t addr, usb_speed_t speed );

/**
 * @brief Request USB transfer.
 * @param ppipe  Pointer to pipe structure.
 * @param buffer External buffer with enough storage space for length bytes.
 * @param length Buffer's length.
 * @todo Replace return info with retvals
 */
int usbhci_xfer_start(
   usb_pipe_t* ppipe,
   uint8_t*    buffer,
   uint32_t    length
);

/**
 * @brief Request USB control transfer.
 *
 * Control transfer have their own separate method because  they  are  the  only
 * ones with a standarized structure, that is, that  use  message  pipes.  Their
 * status should still be checked with the same usbhci_xfer_status() method.
 *
 * @param ppipe   Pointer to pipe structure.
 * @param pstdreq Standard request structure filled with intended request.
 * @param buffer  External buffer with enough storage space for length bytes.
 * @param length  Buffer's length.
 * @todo Replace  return info with retvals
 */
int usbhci_ctrlxfer_start(
   usb_pipe_t*         ppipe,
   const usb_stdreq_t* pstdreq,
   uint8_t*            buffer
);

/**
 * @brief Get USB transfer status.
 * @param ppipe   Pointer to pipe structure.
 *
 * @return Current pipe's transfer status.
 * @todo Replace return info with retvals
 */
int usbhci_xfer_status( usb_pipe_t* ppipe );

/**
 * @brief Cancel USB transfer.
 * @param ppipe   Pointer to pipe structure.
 *
 * @todo Replace return info with retvals
 */
int usbhci_xfer_cancel( usb_pipe_t* ppipe );


/**
* @name HCD Callbacks
* @brief Methods called directly by the Host Controller Driver on interrupts.
* @{ */
/**
 * @brief Callback for when a device connection is detected.
 *
 * @warning Keep in mind this connection is of a device directly to the hardware
 * controller, HUBs are handled through the stack and have nothing  to  do  with
 * this kind of events.
 */
void _usb_host_on_connection( uint8_t corenum );

/**
 * @brief Callback for when a device disconnection is detected.
 *
 * @warning Keep in mind this connection is of a device directly to the hardware
 * controller, HUBs are handled through the stack and have nothing  to  do  with
 * this kind of events.
 */
void _usb_host_on_disconnection( uint8_t corenum );
/** @} HCD Callbacks */


/**  @} USBHCI */
#endif  /* USBHCI_H */

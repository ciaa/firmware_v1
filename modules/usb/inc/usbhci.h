#ifndef USBHCI_H
#define USBHCI_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USBHCI Host Controller Interface
 * @brief USB Host Controller Interface header.
 *
 * @par Description
 *
 * Interface between the main USB driver and the hardware  Host  Controller.  The
 * Host Controller methods are hardware dependent, so this interface is  intended
 * to provide uniform accesses.
 *
 * Device-related  method  reference  whatever  device  is  currently  connected
 * directly  to  the  Host  Controller's  hardware.    Other  devices  connected
 * downstream through HUBs will not be accessible by this interface but  through
 * that one of the HUB  driver.   This  applies  to  all  three  usbhci_reset(),
 * usbhci_get_speed() and usbhci_is_connected().
 *
 *
 * @{
 */


/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "usb.h"
#include "usb_std.h"


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[external functions declaration]=========================*/

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
 * @brief Start USB bus reset.
 */
void usbhci_reset_start( void );

/**
 * @brief Stop USB bus reset.
 * @retval USB_STATUS_BUSY  Reset still in progress.
 * @retval USB_STATUS_OK    Reset stopped and done.
 */
int usbhci_reset_stop( void );

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
 * @param pdev  Pointer to device owner of the pipe.
 * @param ppipe Pointer to pipe structure.
 * @warning This might be merged into usbhci_pipe_alloc().
 */
int usbhci_pipe_configure( usb_device_t* pdev, usb_pipe_t* ppipe );

/**
 * @brief Request USB transfer.
 * @param pdev  Pointer to device owner of the pipe.
 * @param ppipe Pointer to pipe structure.
 * @todo Replace return info with retvals
 */
int usbhci_xfer_start( usb_device_t* pdev, usb_pipe_t* ppipe );

/**
 * @brief Request USB control transfer.
 *
 * Control transfer have their own separate method because  they  are  the  only
 * ones with a standarized structure, that is, that  use  message  pipes.  Their
 * status should still be checked with the same usbhci_xfer_status() method.
 *
 * @param pdev   Pointer to device owner of the pipe (with the stdreq buffer).
 * @param ppipe  Pointer to pipe structure.
 *
 * @todo Replace return info with retvals
 */
int usbhci_ctrlxfer_start( usb_device_t* pdev, usb_pipe_t* ppipe );

/**
 * @brief Get USB transfer status.
 * @param pdev  Pointer to device owner of the pipe.
 * @param ppipe Pointer to pipe structure.
 *
 * @return Current pipe's transfer status.
 * @todo Replace return info with retvals
 */
int usbhci_xfer_status( usb_device_t* pdev, usb_pipe_t* ppipe );

/**
 * @brief Cancel USB transfer.
 * @param pdev  Pointer to device owner of the pipe.
 * @param ppipe Pointer to pipe structure.
 *
 * @todo Replace return info with retvals
 */
int usbhci_xfer_cancel( usb_device_t* pdev, usb_pipe_t* ppipe );


/**
 * @name HCD Callbacks
 *
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


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USBHCI */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif  /* USBHCI_H */


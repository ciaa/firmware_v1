#ifndef USBD_H
#define USBD_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USBD USB Driver
 * @brief USB Driver header.
 *
 * @warning USB devices' addresses are stored starting from 0  (zero),  this  is
 * because internally they are represented by an index.  This is NOT the  actual
 * device's address in the USB BUS, that one  is  the  index  @b  plus  @b  one.
 * Remember that address 0 is reserved for setup and initialization.
 *
 *
 * @par Description
 *
 * USB driver abstraction layer between the USB Host  Controller  and  the  user
 * software. Communication is handled through IRPs (I/O Request Packets) to/from
 * a previously claimed device's interface.
 *
 * As of now, only one IRP can be requested at a time per device interface.  You
 *  should  start  transactions   with   usb_irp(),   which   is   intentionally
 * non-blocking, and then poll usb_irp_status() waiting for it to either fail or
 * complete successfully.  Optionally, you can  cancel  an  ongoing  transaction
 * with usb_irp_cancel() as long as it hasn't been completed yet.
 *
 * @{
 */


/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <usb.h>


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[external functions declaration]=========================*/

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
 * @brief Reset USB device associated to the given ID (as received when assigned
 * to a driver).
 *
 * This gives you the option to reset the device associated to the given  ID  in
 * case some error happens, such  as  receiving  invalid  information  from  it,
 * corrupt bus transactions,  trying  to  connect  a  device  the  stack  wasn't
 * configured for, etc.
 *
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 */
int usb_device_reset( usb_stack_t* pstack, uint16_t device_id );

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
 * @param ticket    Returns control transfer's ticket,  this is  then used  when
 *                  checking for the IRP status.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param pstdreq   Standard request descriptor.
 * @param buffer    Data buffer of length  determined  by  descriptor,  NULL  if
 *                  unused.
 *
 * @retval USB_STATUS_OK   On success
 * @retval USB_STATUS_BUSY Control endpoint is currently busy  or  device  isn't
 *                         done with the enumeration process, try again later.
 * @TODO complete the return values list.
 */
int usb_ctrlirp(
      usb_stack_t*        pstack,
      uint16_t*           ticket,
      uint16_t            device_id,
      const usb_stdreq_t* pstdreq,
      uint8_t*            buffer
);

/**
 * @brief Get IRP's status.
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param irp_id    When requesting the status of a control  transfer,  use  the
 *                  ticket returned by @ref  usb_ctrlirp(),  otherwise  use  the
 *                  device's pipe number (given by order of EPs in interface).
 */
int usb_irp_status( usb_stack_t* pstack, uint16_t device_id, uint16_t irp_id );

/**
 * @brief Cancel ongoing IRP.
 * @param pstack    Pointer to USB stack structure.
 * @param device_id Device  identifier,  coordinates  representing  device   and
 *                  interface.
 * @param irp_id    When cancelling a control transfer,  use the ticket returned
 *                  by @ref  usb_ctrlirp(),  otherwise  use  the  device's  pipe
 *                  number (given by order of EPs in interface).
 */
int usb_irp_cancel( usb_stack_t* pstack, uint16_t device_id, uint16_t irp_id );

/**
 * @brief Control transfer request method \b without checking for completion  of
 * the enumeration process.
 *
 * See @ref usb_ctrlirp() for parameters and return values.
 *
 * @warning Do \b NOT use this unless you really know what you are doing!
 */
int usb_ctrlirp_bypass(
      usb_stack_t*        pstack,
      uint16_t*           ticket,
      uint16_t            device_id,
      const usb_stdreq_t* pstdreq,
      uint8_t*            buffer
);

/**
 * @brief Increase internal tick counter by given amount and retrieve new value.
 * @param pstack    Pointer to USB stack structure.
 * @param inc       Increment value.
 * @return New internal tick count value.
 */
uint16_t usb_systick_inc( usb_stack_t* pstack, uint16_t inc );

/**
 * @brief Get current internal tick counter.
 */
uint16_t usb_systick( usb_stack_t* pstack );



/**
 * TODO: whatever follows won't really be used from the outside, maybe it should be placed in some kind of private file or something...
 */

/** TODO: improve the assert function, right now it will lock in place for easy debugging. */
void usb_assert(int condition);



int usb_device_init( usb_stack_t* pstack, uint8_t index );

int usb_device_release( usb_stack_t* pstack, uint8_t index );

int usb_release_from_port( usb_stack_t* pstack, uint8_t hub_idx, uint8_t port );

int usb_pipe_remove( usb_pipe_t* ppipe );

/* Non-zero if active. */
int usb_device_is_active( usb_stack_t* pstack, uint8_t index );

int usb_get_ctrl_pipe( usb_stack_t* pstack, uint8_t dev_idx, uint8_t* index );

int usb_unlock_pipe( usb_stack_t* pstack, uint8_t index );

int usb_device_update( usb_stack_t* pstack, uint8_t index );

int usb_dealloc_endpoint( usb_stack_t* pstack, usb_pipe_t* ppipe );

int16_t usb_device_attach(
      usb_stack_t* pstack,
      uint8_t      parent_hub,
      uint8_t      parent_port
);

int16_t usb_stack_new_addr( usb_stack_t* pstack );

int usb_stack_update_devices( usb_stack_t* pstack );

/**
 * @brief Get pipe's polling/NAK interval as specified by USB 2.0 section 9.6.6.
 *
 * @param pstack Pointer to USB stack structure.
 * @param id     Device identifier, coordinates representing device and iface.
 * @param pipe   Pipe number (given by order of endpoints in interface).
 *
 * @retval Polling interval in number of USB task ticks value for ISO/INT EPs.
 * @retval Maximum NAK rate for BULT EPs.
 * @retval -1 Invalid parameters.
 * @retval -2 Invalid bInterval value.
 *
 * Polling interval in number  of  USB  stack  ticks  value  (see  @ref
 *          USB_TASK_TICKS), -1 if functions parameters were invalid.
 */
int16_t usb_pipe_get_interval( usb_stack_t* pstack, uint16_t id, uint8_t pipe );

uint8_t usb_device_get_addr( usb_stack_t* pstack, uint16_t id );


/* Descriptor parsing methods. */

int usb_device_parse_cfgdesc( usb_stack_t* pstack, uint8_t index );

int usb_device_parse_ifacedesc(
      usb_stack_t*    pstack,
      uint8_t         dev_idx,
      uint8_t         iface_idx,
      const uint8_t** pbuff,
      uint8_t*        plen
);

int usb_device_parse_epdesc(
      usb_device_t*  pdev,
      uint8_t        iface,
      uint8_t        ep,
      const uint8_t* buffer
);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif  /* USBD_H */


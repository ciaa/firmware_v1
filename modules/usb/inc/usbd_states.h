#ifndef USBD_STATES_H
#define USBD_STATES_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USBD USB Driver
 * @{
 */

/*==================[inclusions]=============================================*/
#include "usb.h"


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[external functions declaration]=========================*/

/**
 * @brief USBD state update.
 * @param pstack Pointer to USB stack structure.
 * @param index  Index of device in the USB stack pstack.
 */
int usbd_state_run( usb_stack_t* pstack, uint8_t index );


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_DESC */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif


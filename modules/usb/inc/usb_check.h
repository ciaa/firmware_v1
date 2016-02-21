#ifndef USB_CHECK_H
#define USB_CHECK_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_CHECK Stack configuration checks
 * @brief Validate USB stack configuration values.
 *
 * This file validates user-defined configuration values in @ref usb_cfg.h.
 *
 * @{
 */


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


#if (USB_MAX_DEVICES < 1) || (USB_MAX_DEVICES > 127)
# error "USB_MAX_DEVICES must be greater than zero and less than 128."
#endif
#if (USB_MAX_INTERFACES < 1) || (USB_MAX_INTERFACES > 127)
# error "USB_MAX_INTERFACES must be greater than zero and less than 128."
#endif
#if (USB_MAX_ENDPOINTS < 1)
# error "USB_MAX_ENDPOINTS must be greater than zero."
#endif
#if (USB_MAX_HUBS < 0) || (USB_MAX_HUBS > 126)
# error "USB_MAX_HUBS must be greater than or equal to zero and less than 127."
#endif
#if (USB_MAX_HUB_PORTS < 1) || (USB_MAX_HUB_PORTS > 126)
/** @FIXME: fix the max. number of hub ports, I'm sure it's not 126 ! */
# error "USB_MAX_HUB_PORTS must be greater than zero and less than 127."
#endif
#if (USB_MAX_HUBS == 0) && (USB_MAX_DEVICES > 1)
# error "Cannot support more than one device without HUB support."
#endif


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_CHECK */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif /* USB_CHECK_H */


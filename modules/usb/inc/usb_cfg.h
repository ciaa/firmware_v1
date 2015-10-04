#ifndef USB_CFG_H
#define USB_CFG_H

/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USB_CFG Stack configuration
 * @brief USB Stack user-defined configuration values
 *
 * This file contains all the main  USB  Stack's  configuration  fields.   Users
 * should change configuration options in this file \b only.
 *
 * @{
 */


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif


/*==================[macros]=================================================*/

/** @brief Maximum number of devices.  */
#define USB_MAX_DEVICES       2

/** @brief Maximum number of interfaces per device.  */
#define USB_MAX_INTERFACES    1

/** @brief Maximum number of endpoints per interface (besides ep. 0).  */
#define USB_MAX_ENDPOINTS     2

/** @brief Maximum number of HUBs, 0 equals no HUB support.  */
#define USB_MAX_HUBS          1

/** @brief Maximum number of HUB ports per HUB.  */
#define USB_MAX_HUB_PORTS     4


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_CFG */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif /* USB_CFG_H */


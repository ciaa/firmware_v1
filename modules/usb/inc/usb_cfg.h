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

/** @brief Task interval in ms. */
#define USB_TASK_TICKS       50

/** @brief Maximum number of HUBs, 0 equals no HUB support.  */
#define USB_MAX_HUBS          2

/** @brief Maximum number of HUB ports per HUB.  */
#define USB_MAX_HUB_PORTS     7

/**
 * @brief Number of control endpoints supported by the specific hardware.
 *
 * @warning It is \b very important to match this correctly  to  the  underlying
 * hardware because, on initialization, the Host will request  as  many  control
 * endpoints as selected here from the low-level hardware  driver  (through  the
 * USB-HCI interface) and that \b cannot fail. You can, however, specify a lower
 * number in case those extra endpoints need to be used for other purposes.
 */
#define USB_N_CTRL_ENDPOINTS  2


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} USB_CFG */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/
#endif /* USB_CFG_H */


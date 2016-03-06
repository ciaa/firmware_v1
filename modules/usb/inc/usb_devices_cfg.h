#ifndef USB_DEVICES_CFG_T
#define USB_DEVICES_CFG_T

#include <stdio.h>

/**@brief Number of simulatneous devices supported. */
#define USB_MAX_DEVICES     4
/**@brief Number of simulatneous interfaces supported. */
#define USB_MAX_INTERFACES  7
/**@brief Number of simulatneous endpoints supported (endpoint 0 excluded). */
#define USB_MAX_ENDPOINTS   5

/** @brief Get number of interfaces of device 'dev'. */
#define USB_GET_IFACES_N(dev)    usb_cte_devices[dev].n_ifaces
/** @brief Get number of endpoints of interface 'if' of device 'dev'. */
#define USB_GET_EPS_N(dev, if)   usb_cte_devices[dev].ifaces[if].n_endpoints
/** @brief Get product ID of device 'dev'. */
#define USB_GET_PRODUCT_ID(dev)  usb_cte_devices[dev].product_id
/** @brief Get vendor ID of device 'dev'. */
#define USB_GET_VENDOR_ID(dev)   usb_cte_devices[dev].vendor_id
/** @brief Get length of configuration descriptor buffer of device 'dev'. */
#define USB_GET_CFG_LEN(dev)     usb_cte_devices[dev].cfg_len

/** @brief Default data for usb_interface_t initialization. */
#define USB_DEFAULT_IFACE_DATA(eps) {eps, USB_IFACE_NO_DRIVER, 0, 0, 255}

typedef struct _usb_cte_iface_t
{
   uint8_t n_endpoints;
} usb_cte_iface_t;

typedef struct _usb_cte_dev_t
{
   usb_cte_iface_t* ifaces;
   uint16_t         vendor_id;
   uint16_t         product_id;
   uint16_t         cfg_len;
   uint8_t          n_ifaces;
} usb_cte_dev_t;

/* This constant cotains the number of interfaces and endpoints per interface.*/
/* Access them with @ref USB_GET_IFACES_N() and @ref USB_GET_EPS_N() macros.  */
extern const usb_cte_dev_t usb_cte_devices[USB_MAX_DEVICES];

#endif /* USB_DEVICES_CFG_T */


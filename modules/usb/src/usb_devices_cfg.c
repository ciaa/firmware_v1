#include <stdio.h>
#include "usb.h"
#include "usb_devices_cfg.h"


/* Endpoints arrays. */
static usb_pipe_t  usb_eps_0_0[1];
static usb_pipe_t  usb_eps_1_0[1];
static usb_pipe_t  usb_eps_2_0[1];
static usb_pipe_t  usb_eps_2_2[1];
static usb_pipe_t  usb_eps_3_0[1];

/* Interfaces arrays. */
static usb_interface_t usb_ifaces_0[1] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_0_0),
};
static usb_interface_t usb_ifaces_1[1] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_1_0),
};
static usb_interface_t usb_ifaces_2[4] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_2_0),
   USB_DEFAULT_IFACE_DATA(NULL),
   USB_DEFAULT_IFACE_DATA(usb_eps_2_2),
   USB_DEFAULT_IFACE_DATA(NULL),
};
static usb_interface_t usb_ifaces_3[1] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_3_0),
};


/* Constants. */
static const usb_cte_iface_t usb_cte_ifaces_0[1] = { {1} };
static const usb_cte_iface_t usb_cte_ifaces_1[1] = { {1} };
static const usb_cte_iface_t usb_cte_ifaces_2[4] = { {1}, {0}, {1}, {0} };
static const usb_cte_iface_t usb_cte_ifaces_3[1] = { {1} };

const usb_cte_dev_t usb_cte_devices[USB_MAX_DEVICES] = {
    {0xFFFF, 0xFFFF, usb_cte_ifaces_0, 1},  /* HUB1   */
    {0xFFFF, 0xFFFF, usb_cte_ifaces_1, 1},  /* Mouse  */
    {0x046D, 0x0825, usb_cte_ifaces_2, 4},  /* Webcam */
    {0xFFFF, 0xFFFF, usb_cte_ifaces_3, 1},  /* HUB0   */
};

static usb_interface_t*const usb_ifaces[USB_MAX_DEVICES] = {
   usb_ifaces_0,
   usb_ifaces_1,
   usb_ifaces_2,
   usb_ifaces_3,
};


/* Interface pointer getter. */
usb_interface_t* usb_devices_cfg_get_ifaces(
      uint16_t pid,
      uint16_t vid,
      uint8_t  n_ifaces,
      uint8_t* pindex
)
{
   usb_interface_t* piface = NULL;
   uint8_t i;
   for (i = 0; i < USB_MAX_DEVICES && piface == NULL; ++i)
   {
      if ( (USB_GET_PRODUCT_ID(i) == pid || USB_GET_PRODUCT_ID(i) == USB_FORCE_PROBING_ID) &&
           (USB_GET_VENDOR_ID(i)  == vid || USB_GET_VENDOR_ID(i)  == USB_FORCE_PROBING_ID) &&
            USB_GET_IFACES_N(i)   == n_ifaces &&
            usb_ifaces[i][0].protocol == 255 )
      {
         piface = usb_ifaces[i];
         if (pindex != NULL)
         {
            *pindex = i;
         }
      }
   }
   return piface;
}


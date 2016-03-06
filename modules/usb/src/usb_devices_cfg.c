#include <stdio.h>
#include "usb.h"
#include "usb_devices_cfg.h"


/* Endpoints arrays. */
static usb_pipe_t  usb_eps_0_1[1];
static usb_pipe_t  usb_eps_0_3[1];
static usb_pipe_t  usb_eps_1_0[1];
static usb_pipe_t  usb_eps_2_0[1];
static usb_pipe_t  usb_eps_3_0[1];

/* Interfaces arrays. */
static usb_interface_t usb_ifaces_0[4] = {
   USB_DEFAULT_IFACE_DATA(NULL),
   USB_DEFAULT_IFACE_DATA(usb_eps_0_1),
   USB_DEFAULT_IFACE_DATA(NULL),
   USB_DEFAULT_IFACE_DATA(usb_eps_0_3),
};
static usb_interface_t usb_ifaces_1[1] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_1_0),
};
static usb_interface_t usb_ifaces_2[1] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_2_0),
};
static usb_interface_t usb_ifaces_3[1] = {
   USB_DEFAULT_IFACE_DATA(usb_eps_3_0),
};

/* Configuration buffers. */
static char usb_cfg_buffer_0[2466];
static char usb_cfg_buffer_1[  34];
static char usb_cfg_buffer_2[  41];
static char usb_cfg_buffer_3[  25];


/* Constants. */
static const usb_cte_iface_t usb_cte_ifaces_0[4] = { {0}, {1}, {0}, {1} };
static const usb_cte_iface_t usb_cte_ifaces_1[1] = { {1} };
static const usb_cte_iface_t usb_cte_ifaces_2[1] = { {1} };
static const usb_cte_iface_t usb_cte_ifaces_3[1] = { {1} };

const usb_cte_dev_t usb_cte_devices[USB_MAX_DEVICES] = {
    {usb_cte_ifaces_0, 0x046D, 0x0825, 2466, 4},  /* Webcam */
    {usb_cte_ifaces_1, 0x046D, 0xC247,   34, 1},  /* Mouse  */
    {usb_cte_ifaces_2, 0xFFFF, 0xFFFF,   41, 1},  /* HUB0   */
    {usb_cte_ifaces_3, 0xFFFF, 0xFFFF,   25, 1},  /* HUB1   */
};

/* Array of interfaces, these will be requested during enumeration. */
static usb_interface_t*const usb_ifaces[USB_MAX_DEVICES] = {
   usb_ifaces_0,
   usb_ifaces_1,
   usb_ifaces_2,
   usb_ifaces_3,
};

/* Array of config buffers, these will be requested alongside the interfaces. */
static char*const usb_cfg_buffers[USB_MAX_DEVICES] = {
   usb_cfg_buffer_0,
   usb_cfg_buffer_1,
   usb_cfg_buffer_2,
   usb_cfg_buffer_3,
};


/* Interface & configuration buffer pointers getter. */
int16_t usb_device_get_config(
      usb_interface_t** ppiface,
      char**            pbuffer,
      uint16_t          vid,
      uint16_t          pid,
      uint16_t          len,
      uint8_t           n_ifaces
)
{
   uint8_t i;
   int16_t ret = -1;

   for (i = 0; i < USB_MAX_DEVICES && ret == -1; ++i)
   {
      if ( (USB_GET_VENDOR_ID(i)  == vid || USB_GET_VENDOR_ID(i)  == USB_FORCE_PROBING_ID) &&
           (USB_GET_PRODUCT_ID(i) == pid || USB_GET_PRODUCT_ID(i) == USB_FORCE_PROBING_ID) &&
            USB_GET_IFACES_N(i)   == n_ifaces &&
            USB_GET_CFG_LEN(i)    >= len      &&
            usb_ifaces[i][0].protocol == 255 )
      {
         ret = (int16_t) i;
      }
   }
   if (ret >= 0)
   {
       /* Iface/buffer found. */
       if (ppiface != NULL)
       {
           *ppiface = usb_ifaces[(uint8_t)ret];
       }
       if (pbuffer != NULL)
       {
           *pbuffer = usb_cfg_buffers[(uint8_t)ret];
       }
   }
   return ret;
}


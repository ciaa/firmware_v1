#include <stdint.h>
#include "drivers/usb_drivers.h"
#include "usb_hub.h"


int usb_hub_update( usb_hub_t* phub )
{
   usb_assert(phub != NULL);
}

int usb_hub_is_active( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_is_connected( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_bind_to_dev( usb_hub_t* phub, uint8_t port, uint8_t addr )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
   usb_assert(addr < USB_MAX_DEVICES);
   usb_assert(addr > 0); /* Index 0 is reserved for the root hub/device,
                          it cannot be connected to a HUB's port */
}

int usb_hub_unbind( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_poweron( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_poweroff( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
}

int usb_hub_get_bound_addr( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
   /** TODO: some kind of error check here, return negative if port is invalid */
   return phub->ports[port].address;
}

int usb_hub_reset( usb_hub_t* phub, uint8_t port )
{
   usb_assert(phub != NULL);
   usb_assert(port < phub->n_ports);
   /** TODO: implement this function, send reset over ctrl xfer (or is it int?) */
   return USB_STATUS_OK;
}


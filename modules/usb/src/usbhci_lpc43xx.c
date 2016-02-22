/**
 * @addtogroup CIAA_Firmware CIAA Firmware
 * @{
 * @addtogroup USB USB Stack
 * @{
 * @addtogroup USBHCI Host Controller Interface
 * @{
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "os.h"

#include "usb.h"
#include "usbd.h"
#include "usbhci.h"
#if (USB_MAX_HUBS > 0)
#include "drivers/usb_hub.h"
#endif

#include "HAL/HAL.h"
#include "HCD/HCD.h"
#include "USBMode.h"


/*==================[macros and definitions]=================================*/
#define USB_CORENUM 0  /** FIXME: set this up and store it somewhere in the stack's structure? */

/** @brief HW pipe allocated. */
#define USBHCI_PIPE_INUSE (1 << 0)
/** @brief HW pipe opened and configured. */
#define USBHCI_PIPE_OPEN  (1 << 1)


/*==================[internal data declaration]==============================*/

/**
 * @brief Hardware pipes.
 * This is defined and declared here because it is  hardware-dependent,  placing
 * it in the header file would make no sense for other hw controllers.
 */
struct _usbhci_pipe_t
{
   uint32_t       handle; /**< This is actually a Pipe_Handle_T struct.       */
   uint8_t        status; /**< Hardware pipe status.                          */
   const uint8_t  types;  /**< Hardware pipe supported transfer types.        */
};


/** @brief USB Core connection status, updated in ISRs. */
static volatile uint8_t _connected[MAX_USB_CORE];


/*==================[internal functions declaration]=========================*/

/** @brief Get pipe's status from LPCOpen handle. */
static int _get_hcd_status( uint32_t handle );

/** @brief Speed type conversion, LPC to local stack. */
static usb_speed_t _from_lpc_speed(HCD_USB_SPEED speed);

/** @brief Speed type conversion, local stack to LPC. */
static HCD_USB_SPEED _to_lpc_speed(usb_speed_t speed);

/** @brief Transfer type conversion, local stack to LPC. */
static HCD_TRANSFER_TYPE _to_lpc_type( usb_xfer_type_t type );

/** @brief Transfer direction conversion, LPC to local stack. */
static HCD_TRANSFER_DIR _to_lpc_dir( usb_dir_t dir );


/*==================[internal data definition]===============================*/

/** @FIXME We are only using one for one (core 0). */
static struct _usbhci_pipe_t _pipe_handle[HCD_MAX_ENDPOINT] =
{
   {0, 0, 0x01},
   {0, 0, 0x01},
   {0, 0, 0x0E},
   {0, 0, 0x0E},
   {0, 0, 0x0E},
   {0, 0, 0x0E},
   {0, 0, 0x0E},
   {0, 0, 0x0E},
};


/*==================[internal functions definition]==========================*/

static int _get_hcd_status( uint32_t handle )
{
   int status;
   switch (HcdGetPipeStatus(handle))
   {
      case HCD_STATUS_OK:
      case HCD_STATUS_TRANSFER_COMPLETED:
         status = USB_STATUS_OK;
         break;

      case HCD_STATUS_TRANSFER_QUEUED:
         status = USB_STATUS_XFER_WAIT;
         break;

      case HCD_STATUS_TRANSFER_ERROR: /* check this */
      case HCD_STATUS_TRANSFER_CRC:
      case HCD_STATUS_TRANSFER_BitStuffing:
      case HCD_STATUS_TRANSFER_DataToggleMismatch:
      case HCD_STATUS_TRANSFER_PIDCheckFailure:
      case HCD_STATUS_TRANSFER_UnexpectedPID:
      case HCD_STATUS_TRANSFER_DataOverrun:
      case HCD_STATUS_TRANSFER_DataUnderrun:
      case HCD_STATUS_TRANSFER_CC_Reserved1:
      case HCD_STATUS_TRANSFER_CC_Reserved2:
      case HCD_STATUS_TRANSFER_BufferOverrun:
      case HCD_STATUS_TRANSFER_BufferUnderrun:
      case HCD_STATUS_TRANSFER_NotAccessed:
      case HCD_STATUS_TRANSFER_Reserved:
      default:
         status = USB_STATUS_XFER_ERR;
         break;

      case HCD_STATUS_TRANSFER_Stall:
         status = USB_STATUS_EP_STALLED;
         break;

      case HCD_STATUS_TRANSFER_DeviceNotResponding:
         status = USB_STATUS_DEV_UNREACHABLE;
         break;
   };
   return status;
}

static usb_speed_t _from_lpc_speed(HCD_USB_SPEED speed)
{
   usb_speed_t ret;
   switch (speed)
   {
      case FULL_SPEED:
         ret = USB_SPD_FS;
         break;
      case LOW_SPEED:
         ret = USB_SPD_LS;
         break;
      case HIGH_SPEED:
         ret = USB_SPD_HS;
         break;
      default:
         ret = USB_SPD_INV;
         break;
   }
   return ret;
}

static HCD_USB_SPEED _to_lpc_speed(usb_speed_t speed)
{
   HCD_USB_SPEED ret;
   switch (speed)
   {
      case USB_SPD_HS:
         ret = HIGH_SPEED;
         break;
      case USB_SPD_FS:
         ret = FULL_SPEED;
         break;
      default:
         ret = LOW_SPEED;
         break;
   }
   return ret;
}

static HCD_TRANSFER_TYPE _to_lpc_type( usb_xfer_type_t type )
{
   HCD_TRANSFER_TYPE ret;
   switch (type)
   {
      case USB_CTRL:
         ret = CONTROL_TRANSFER;
         break;
      case USB_INT:
         ret = INTERRUPT_TRANSFER;
         break;
      case USB_ISO:
         ret = ISOCHRONOUS_TRANSFER;
         break;
      case USB_BULK:
      default:
         ret = BULK_TRANSFER;
         break;
   }
   return ret;
}

static HCD_TRANSFER_DIR _to_lpc_dir( usb_dir_t dir )
{
   HCD_TRANSFER_DIR ret;
   switch (dir)
   {
      case USB_DIR_TOK:
         ret = SETUP_TRANSFER;
         break;
      case USB_DIR_OUT:
         ret = OUT_TRANSFER;
         break;
      case USB_DIR_IN:
      default:
         ret = IN_TRANSFER;
         break;
   };
   return ret;
}


/*==================[external functions definition]==========================*/

int usbhci_init( void )
{
   uint8_t i;
   int     ret;

   Chip_USB0_Init();
   HAL_USBInit(USB_CORENUM, USB_MODE_Host);

   /* Host */
   if (HcdInitDriver(USB_CORENUM) == HCD_STATUS_OK)
   {
      HAL_EnableUSBInterrupt(USB_CORENUM);
      HcdRhPortEnable(USB_CORENUM);

      /* Pipes */
      for (i = 0; i < HCD_MAX_ENDPOINT; ++i)
      {
         _pipe_handle[i].handle = 0;
         _pipe_handle[i].status = 0;
      }

      /* Core status */
      for (i = 0; i < MAX_USB_CORE; ++i)
         _connected[i] = 0;

      ret = USB_STATUS_OK;
   }
   else
   {
      HcdDeInitDriver(USB_CORENUM);
      ret = USB_STATUS_HCD_INIT;
   }

   return ret;
}

int usbhci_deinit( void )
{
   HcdRhPortDisable(USB_CORENUM);
   HcdDeInitDriver(USB_CORENUM);
   HAL_USBDeInit(USB_CORENUM, 2); /* 2 = Host, 1 = Device */
   return 0; /** @TODO: validate previous return code */
}

void usbhci_reset_start( void )
{
   HcdRhPortReset(USB_CORENUM);
}

int usbhci_reset_stop( void )
{
   return (HcdRhPortResetDone(USB_CORENUM)) ? USB_STATUS_OK : USB_STATUS_BUSY;
}

usb_speed_t usbhci_get_speed( void )
{
   usb_speed_t spd;
   HCD_USB_SPEED speed;
   if (HcdGetDeviceSpeed(USB_CORENUM, &speed) != HCD_STATUS_OK)
   {
      spd = USB_SPD_INV;
   }
   else
   {
      spd = _from_lpc_speed(speed);
   }
   return spd;
}

int usbhci_is_connected( void )
{
   return _connected[USB_CORENUM];
}

uint32_t usbhci_get_frame_number( void )
{
   return HcdGetFrameNumber(USB_CORENUM);
}

int16_t usbhci_pipe_alloc( usb_xfer_type_t type )
{
   uint8_t i;
   int16_t ret = -1;
   for (i = 0; i < HCD_MAX_ENDPOINT && ret == -1; ++i)
   {
      /* Check if pipe is currently being used. */
      if (!_pipe_handle[i].status & USBHCI_PIPE_INUSE)
      {
         if (_pipe_handle[i].types & (1 << type))
         {
            /* Requested type supported. */
            _pipe_handle[i].status |= USBHCI_PIPE_INUSE;
            ret = i;
         }
      }
   }
   return ret;
}

int usbhci_pipe_dealloc( usb_pipe_t* ppipe )
{
   int status;

   usb_assert(ppipe != NULL);
   usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);

   /* Close pipe if open. */
   status = USB_STATUS_OK;
   if (_pipe_handle[ppipe->handle].status & USBHCI_PIPE_OPEN)
   {
      if (HcdClosePipe(_pipe_handle[ppipe->handle].handle) != HCD_STATUS_OK)
      {
         /* Closing failed... */ /** @TODO: improve this */
         status = USB_STATUS_PIPE_CFG;
      }
   }
   if (status == USB_STATUS_OK)
   {
      _pipe_handle[ppipe->handle].status = 0;
   }
   return status;
}

int usbhci_pipe_configure( usb_device_t* pdev, usb_pipe_t* ppipe )
{
   struct _usbhci_pipe_t* phci_pipe;
   int     status;
   uint8_t hub;
   uint8_t port;

   usb_assert(pdev  != NULL);
   usb_assert(ppipe != NULL);
   usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);

   phci_pipe = &_pipe_handle[ppipe->handle];

   hub  = 0;
   port = 0;
#if (USB_MAX_HUBS > 0)
   if (pdev->speed != USB_SPD_HS && usbhci_get_speed() == USB_SPD_HS)
   {
      /* FS/LS pipes require the addr & port when connected through a HS one. */
      hub  = usb_hub_get_address(pdev->parent_hub);
      port = pdev->parent_port;
   }
#endif

   if (phci_pipe->status & USBHCI_PIPE_OPEN)
   {
      /* If pipe has already been opened, close it before reconfiguring. */
      HcdClosePipe(phci_pipe->handle);
   }
   /* Set pipe's status as closed. */
   phci_pipe->status &= ~USBHCI_PIPE_OPEN;

   /* Configure pipe. */
   status = HcdOpenPipe(
         USB_CORENUM,
         pdev->addr & USB_ADDR_MASK,
         _to_lpc_speed(pdev->speed),
         ppipe->number,
         _to_lpc_type(ppipe->type),
         _to_lpc_dir(ppipe->dir),
         ppipe->mps,
         ppipe->interval,
         1, /** @TODO Mult, for ISO @ HS with >1 transaction per uframe */
         hub,
         port,
         &phci_pipe->handle );

   if (status == HCD_STATUS_OK)
   {
      /* Once configuration is done, set status as open again. */
      phci_pipe->status |= USBHCI_PIPE_OPEN;
      status = USB_STATUS_OK;
   }
   return status;
}

int usbhci_msg_pipe_configure( usb_device_t* pdev, usb_msg_pipe_t* pmsg )
{
   struct _usbhci_pipe_t* phci_pipe;
   int     status;
   uint8_t hub;
   uint8_t port;

   usb_assert(pdev != NULL);
   usb_assert(pmsg != NULL);
   usb_assert(pmsg->handle < HCD_MAX_ENDPOINT);

   phci_pipe = &_pipe_handle[pmsg->handle];

   hub  = 0;
   port = 0;
#if (USB_MAX_HUBS > 0)
   if (pdev->speed != USB_SPD_HS && usbhci_get_speed() == USB_SPD_HS)
   {
      /* FS/LS pipes require the addr & port when connected through a HS one. */
      hub  = usb_hub_get_address(pdev->parent_hub);
      port = pdev->parent_port;
   }
#endif

   if (phci_pipe->status & USBHCI_PIPE_OPEN)
   {
      /* If pipe has already been opened, close it before reconfiguring. */
      HcdClosePipe(phci_pipe->handle);
   }
   /* Set pipe's status as closed. */
   phci_pipe->status &= ~USBHCI_PIPE_OPEN;

   /* Configure pipe. */
   status = HcdOpenPipe(
         USB_CORENUM,
         pdev->addr & USB_ADDR_MASK,
         _to_lpc_speed(pdev->speed),
         0,
         _to_lpc_type(USB_CTRL),
         _to_lpc_dir(USB_DIR_TOK),
         pdev->mps,
         1,
         1, /** @TODO Mult, for ISO @ HS with >1 transaction per uframe */
         hub,
         port,
         &phci_pipe->handle );

   if (status == HCD_STATUS_OK)
   {
      /* Once configuration is done, set status as open again. */
      phci_pipe->status |= USBHCI_PIPE_OPEN;
      status = USB_STATUS_OK;
   }
   return status;
}

int usbhci_xfer_start( usb_device_t* pdev, usb_pipe_t* ppipe )
{
   int ret;

   usb_assert(pdev  != NULL);
   usb_assert(ppipe != NULL);
   usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);

   ret = USB_STATUS_XFER_ERR;
   if ( !HcdDataTransfer(
            _pipe_handle[ppipe->handle].handle,
            ppipe->buffer,
            ppipe->length,
            NULL) )
   {
      ret = USB_STATUS_OK;
   }
   return ret;
}

int usbhci_ctrlxfer_start( usb_device_t* pdev, usb_msg_pipe_t* pmsg )
{
   int ret;

   usb_assert(pdev != NULL);
   usb_assert(pmsg != NULL);
   usb_assert( pmsg->buffer != NULL || (pmsg->buffer == NULL &&
            USB_STDREQ_GET_wLength(pmsg->stdreq) == 0) );
   usb_assert(pmsg->handle < HCD_MAX_ENDPOINT);

   ret = USB_STATUS_XFER_ERR;
   if ( !HcdControlTransfer(
            _pipe_handle[pmsg->handle].handle,
            pmsg->stdreq,
            pmsg->buffer) )
   {
      ret = USB_STATUS_OK;
   }
   return ret;
}

int usbhci_xfer_status( usb_device_t* pdev, usb_pipe_t* ppipe )
{
   usb_assert(pdev  != NULL);
   usb_assert(ppipe != NULL);
   usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);
   return _get_hcd_status(_pipe_handle[ppipe->handle].handle);
}

int usbhci_ctrlxfer_status( usb_device_t* pdev, usb_msg_pipe_t* pmsg )
{
   usb_assert(pdev != NULL);
   usb_assert(pmsg != NULL);
   usb_assert(pmsg->handle < HCD_MAX_ENDPOINT);
   return _get_hcd_status(_pipe_handle[pmsg->handle].handle);
}

int usbhci_xfer_cancel( usb_device_t* pdev, usb_pipe_t* ppipe )
{
   int ret;
   usb_assert(pdev  != NULL);
   usb_assert(ppipe != NULL);
   usb_assert(ppipe->handle < HCD_MAX_ENDPOINT);
   ret = USB_STATUS_PIPE_CFG;
   if (!HcdCancelTransfer(_pipe_handle[ppipe->handle].handle))
   {
      ret = usbhci_pipe_configure(pdev, ppipe);
   }
   return ret;
}

int usbhci_ctrlxfer_cancel( usb_device_t* pdev, usb_msg_pipe_t* pmsg )
{
   int ret;
   usb_assert(pdev != NULL);
   usb_assert(pmsg != NULL);
   usb_assert(pmsg->handle < HCD_MAX_ENDPOINT);
   ret = USB_STATUS_PIPE_CFG;
   if (!HcdCancelTransfer(_pipe_handle[pmsg->handle].handle))
   {
      ret = usbhci_msg_pipe_configure(pdev, pmsg);
   }
   return ret;
}

void _usb_host_on_connection( uint8_t corenum )
{
   usb_assert(corenum < MAX_USB_CORE);
   _connected[corenum] = 1;
}

void _usb_host_on_disconnection( uint8_t corenum )
{
   usb_assert(corenum < MAX_USB_CORE);
   _connected[corenum] = 0;
}

/* Interrupts */

ISR(USB0_IRQHandler)
{
   USB0_IRQHandler();
}

ISR(USB1_IRQHandler)
{
   USB1_IRQHandler();
}

/** @} USBHCI */
/** @} USB */
/** @} CIAA_Firmware */
/*==================[end of file]============================================*/


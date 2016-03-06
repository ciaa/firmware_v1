#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "usb_hid_task.h"

#include "drivers/usb_hid.h"


#define EDU_CIAA_LED_R 0x10
#define EDU_CIAA_LED_G 0x20
#define EDU_CIAA_LED_Y 0x08


/* Board LEDs. */
int fd_out;

/* HID */
int hid_fd = -1;


/*****************************************************************************/

void usb_hid_task_start( void )
{
   /* Setup board LED outputs. */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);
}

void usb_hid_task_loop( void )
{
   uint8_t buffer[5];
   uint8_t LED_outputs = 0x00;

   /* Open HID device. */
   if (hid_fd < 0)
   {
      /* Try with a mouse. */
      hid_fd = usb_hid_open("/mouse0", 0);
   }
   if (hid_fd < 0)
   {
      /* Try with a keyboard */
      hid_fd = usb_hid_open("/keyb0", 0);
   }
   if (hid_fd >= 0)
   {
      /* If something was opened successfully, read from it. */

      /* Since _hid_read() is blocking, we need to stop and restart the alarm.*/
      CancelAlarm(ActivateUSBHIDTask);
      usb_hid_read(hid_fd, buffer, 8);
      SetRelAlarm(ActivateUSBHIDTask, 50, 50);

      if (buffer[1] != 0x00)
         LED_outputs |= EDU_CIAA_LED_R;
      if (buffer[2] != 0x00)
         LED_outputs |= EDU_CIAA_LED_G;
      if (buffer[3] != 0x00)
         LED_outputs |= EDU_CIAA_LED_Y;

      ciaaPOSIX_write(fd_out, &LED_outputs, 1);
      //usb_hid_close(hid_fd);
   }
}


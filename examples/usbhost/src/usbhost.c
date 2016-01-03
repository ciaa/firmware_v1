#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaak.h"

#include "usb_cfg.h"
#include "usb_task.h"
#include "usb_hid_task.h"


int main( void )
{
   StartOS(AppMode1);
   return 0;
}

void ErrorHook( void )
{
   ciaaPOSIX_printf("ErrorHook was called.\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n",
         OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(),
         OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

TASK(InitTask)
{
   ciaak_start();

   /* Activate USB task and its alarm. */
   usb_task_start();
   SetRelAlarm(ActivateUSBStackTask, 100, USB_TASK_TICKS);

   /* Activate USB HID task and its alarm. */
   usb_hid_task_start();
   SetRelAlarm(ActivateUSBHIDTask, 120, 50);

   TerminateTask();
}

TASK(USBStackTask)
{
   usb_task_loop();
   TerminateTask();
}

TASK(USBHIDTask)
{
   usb_hid_task_loop();
   TerminateTask();
}


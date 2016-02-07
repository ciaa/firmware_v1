#include "ciaaPOSIX_stdio.h"
#include "chip.h"
#include "usb_task.h"

#include "usb.h"
#include "usbd.h"
#include "drivers/usb_hid.h"


/* USB stack. */
static usb_stack_t usb_stack;


/*****************************************************************************/

STATIC const PINMUX_GRP_T pinmuxing[] =
{
   /* USB1 V-BUS Enable pin */
   {0x2,  5, (SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | SCU_MODE_FUNC4)},
};

static void setup_hardware( void )
{
   /* Set pin muxing. */
   Chip_SCU_SetPinMuxing(pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

#if 1 // apparently needed...?
   /* Setup System core frequency to MAX_CLOCK_FREQ. */
   Chip_SetupCoreClock(CLKIN_CRYSTAL, MAX_CLOCK_FREQ, true);
   /* Setup system base clocks and initial states. */
   Chip_Clock_SetBaseClock(CLK_BASE_USB1, CLKIN_IDIVD, true, true);

   /* Reset and enable 32Khz oscillator. */
   LPC_CREG->CREG0 &= ~((1 << 3) | (1 << 2));
   LPC_CREG->CREG0 |=   (1 << 1) | (1 << 0);
#endif
}

void usb_task_start( void )
{
   /* Setup USB hardware. */
   setup_hardware();

   /* Initialize USB stack. */
   usb_init(&usb_stack);

   /* Initialize HID driver. */
   usb_hid_init();
}

void usb_task_loop( void )
{
   /* Update USB stack's system tick counter. */
   usb_systick_inc(&usb_stack, USB_TASK_TICKS);

   /* Update USB stack. */
   usb_run(&usb_stack);

   /* Update HID driver. */
   usb_hid_update();
}


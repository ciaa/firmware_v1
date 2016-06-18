#include "msp430.h"

/*
      static void __attribute__((naked, section(".crt_0001disable_watchdog"), used))
      disable_watchdog (void)
      {
         WDTCTL = WDTPW | WDTHOLD;
      }
*/
/*
volatile unsigned char i;

__attribute__((constructor))
static void myctor(void)
{
//   WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
i=0;
}

*/

/*
void myctor(void) {
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
}

void (*funcptr)(void)  __attribute__ ((section ( ".ctors"))) =myctor;
*/
/*
extern __bssstart;
volatile unsigned short caca;
void dummy_function()
{
     caca= __bssstart;
}*/ //esto funciona

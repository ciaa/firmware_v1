#include "msp430.h"
//#include <unistd.h>

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

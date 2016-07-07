//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.2|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************
#include <msp430.h>
#include <string.h>


 unsigned int i = 3;
 unsigned short a = 4;

int main(void)
{

	for(;;)
	{
	//	volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}

	return 0;
}

#include "msp430.h"

__attribute__((__constructor__))
void myctor()
{
  WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
  P1DIR |= 0x01;					// Set P1.0 to output direction
}


__attribute__((__interrupt_vec__(PORT1_VECTOR),naked))
void unused_irq()
{
    /* Do something. */;
}

/*
__attribute__( (__interrupt_vec__(PORT1_VECTOR)))
port1_isr (void)
{

}*/

__attribute__( (weak, alias("unused_irq"),__interrupt_vec__(PORT2_VECTOR)))
port2_isr (void)
{

}



/*
_system_pre_init()
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
}*/

extern unsigned char * 	__bssstart;
extern unsigned int 	__bsssize;
extern unsigned char * 	__datastart;
extern unsigned char * 	__romdatastart;
extern unsigned int 	__romdatacopysize;
/*
//ESCRIBIENDO EL VECTOR DE RESET //ESTA ES LA FORMA DE DEFINIR
__attribute__((__interrupt_vec__(RESET_VECTOR),naked)) //__attribute__((__interrupt_vec__(RESET_VECTOR)))
reset_vector (void)
{
	WDTCTL = WDTPW | WDTHOLD;		        // Stop watchdog timer
   __asm volatile ("mov.w #__stack,SP");  //INICIALIZO EL STACK POINTER

   unsigned short i;

   //inicializo todas las variables en cer0
   for(i=0;i<__bsssize;i++)
   {
	   __bssstart[i] = 0;
   }

   //inicializo todas las variables en ram con valores diferentes a cero

    for(i=0;i<__romdatacopysize;i++)
    {
    	__datastart[i] = __romdatastart[i];
    }


	__asm volatile ("jmp main "); //si hago un llamado, usa un call, y por consiguiente tamaño de stack desperdiciado.
	//main();
}
*/

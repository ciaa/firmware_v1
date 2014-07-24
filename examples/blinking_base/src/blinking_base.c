/*
===============================================================================
 Name        : ciaaBlink.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

//#include <cr_section_macros.h>

#include "chip.h"
#include "ciaaIO.h"

volatile int contms;

void SysTick_Handler(void)
{
	if(contms) contms--;
}

void pausems(int t)
{
	contms = t;
	while(contms) __WFE();
}

int main(void)
{
	volatile int i;

	/* FIXME: SystemCoreClock no se usa con LPC43xx,
	 * pero CGU_GetPCLKFrequency() no sé por qué genera
	 * cuelgues erráticos o hace que SysTick no interrumpa :(
	 */
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
    ciaaIOInit();

    while(1)
    {
    	for(i=0; i<8; i++)
    	{
			ciaaWriteOutput(i, 1);
			pausems(500);
			ciaaWriteOutput(i, 0);
    	}
    }
}

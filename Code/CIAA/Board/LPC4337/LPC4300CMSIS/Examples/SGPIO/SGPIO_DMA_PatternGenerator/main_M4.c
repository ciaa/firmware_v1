
#include "LPC43xx.h"
#include "main_M4.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"

uint32_t reg_address, a, b, c, d, data1, data2;
#define LED1_PORT 4
#define LED1_BIT 1

int main()
{		
	CGU_Init_M4SGPIO();					// set frequncy of M4 and SGPIO
	
	data1 = 0xF0F0F0F0;	  // 0xFFFF, 0xF0F0F0F0
	putdata(data1);						// Put signal pattern in memory location from 0x20000000

	copy_VectorTable();					// Copy vector table from 0x10000000 to 0x20008000 for M0
	LPC_CREG->M0APPMEMMAP = 0x20008000;	// Write vector table address into CREG M0 Memory mapping register
	LPC_RGU->RESET_CTRL1 = 0;			// clear the reset of M0APP
	
	
	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : USB0_IND1 LED
	GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), 1);

	while(1)
	{
		int x = 0, y = 0;
		GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));
		for (x = 0; x <= 1000000; x++);
		GPIO_SetValue(LED1_PORT,(1<<LED1_BIT));
		for (y = 0; y <= 1000000; y++);
	}	
}



void putdata(uint32_t data)
{
	int i = 0, j=0, n=0, k=0;
	a = data;

	DMA_MEM->EN_Slices = 0xFFFF;
	for (k = 0; k<16; k++)
	{
		DMA_MEM->REG_SSdata[k]= data;
	}
	for (i = 0; i<16; i++)
	{
		DMA_MEM->REG_SSdata_1[i]= 0xF0F0F0F0;		// 0xFFFFFFFF 0xAAAAAAAA 0xF0F0F0F0
	}
	for (j = 0; j<16; j++)
	{
		DMA_MEM->REG_SSdata_2[j]= 0xF0F0F0F0;		// 0
	}
	for (n = 0; n<16; n++)
	{
		DMA_MEM->REG_SSdata_3[n]= 0xF0F0F0F0;		// 0x0F0F0F0F
	}
} 



void CGU_Init_M4SGPIO(void)
{		
	CGU_SetXTALOSC(12000000);
	CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);
//	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_M4);
	
	/* 21*12 = 252 Mhz */
	CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL1);
	CGU_SetPLL1(21);
	CGU_EnableEntity(CGU_CLKSRC_PLL1, ENABLE);
	
	
	/* XTAL -> PLL1 -> IDIVA(2) -> M4, SGPIO */
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVA);
	CGU_EnableEntity(CGU_CLKSRC_IDIVA, ENABLE);
	CGU_SetDIV(CGU_CLKSRC_IDIVA, 2);
	

	while(LPC_CGU->PLL1_STAT == 0);		// lock indicator, wait until the clock is locked

	CGU_EntityConnect(CGU_CLKSRC_IDIVA, CGU_BASE_M4);		   
	CGU_EntityConnect(CGU_CLKSRC_IDIVA, CGU_BASE_PERIPH);
	CGU_UpdateClock();

	WaitMS(1);

	/* DIV = 1 */
	CGU_SetDIV(CGU_CLKSRC_IDIVA, 1);
	CGU_UpdateClock();
}



void WaitMS(int ms)
{
	uint32_t us = 0;
	us = ms * 1000;
	us *= 10;
	while(us--)
	{ d = us;}
}



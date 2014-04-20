
//void SGPIO_IRQHandler()
//{
//	uint32_t SrcAddr, DesAddr;	
////#ifdef MS
////	uint32_t *p=(uint32_t*) 0x40002120;
////#endif
//	LPC_SGPIO->CTR_STATUS_1 = 0xFFFFFFFF;
//	//data2 = 0xFFFF0F00;
//	//putdata(data2);	
//	SrcAddr = (uint32_t) & (DMA_MEM->REG_SSdata[0]);
//	DesAddr = (uint32_t) & (LPC_SGPIO->REG_SS[0]);
//  //__IO uint32_t  C1SRCADDR;                 /*!< (@ 0x40002120) DMA Channel Source Address Register */
//  //__IO uint32_t  C1DESTADDR;                /*!< (@ 0x40002124) DMA Channel Destination Address Register */
//  //__IO uint32_t  C1LLI;                     /*!< (@ 0x40002128) DMA Channel Linked List Item Register */
//  //__IO uint32_t  C1CONTROL;                 /*!< (@ 0x4000212C) DMA Channel Control Register */
//  //__IO uint32_t  C1CONFIG;                  /*!< (@ 0x40002130) DMA Channel Configuration Register */
////#ifdef MS
////    p[0]=SrcAddr;
////	p[1]=DesAddr;
////	p[3]= (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);
////	p[4]=1;
////#endif
//
//#define BLA
//#ifdef BLA
//	// Setup GPDMA channel 1 : REG_SS --------------------------------
//	LPC_GPDMA->C1SRCADDR = SrcAddr;
//	LPC_GPDMA->C1DESTADDR =	DesAddr;
////	LPC_GPDMA->C1LLI = 0;
//	LPC_GPDMA->C1CONTROL = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);
////	LPC_GPDMA->C1CONFIG = 0x0;
//	
//	// Enable GPDMA channel 1 
//	LPC_GPDMA->C1CONFIG = 1|(1<<14)|(1<<15); 
//	while(LPC_GPDMA->ENBLDCHNS != 0);		// wait until channels are all disabled		
////#endif
//}




//__irq void SGPIO_IRQHandler()
//{
//	if (LPC_GPDMA->C3SRCADDR == (uint32_t) &(DMA_MEM->REG_SSdata[0]))
//	{
//		LPC_GPDMA->C3CONFIG = 1|(1<<14)|(1<<15);
//		LPC_SGPIO->CTR_STATUS_1 = 0xFFFF;
//
//		LPC_GPDMA->C1SRCADDR = (uint32_t) & (DMA_MEM->REG_SSdata[0]);
//		LPC_GPDMA->C1DESTADDR =	(uint32_t) & (LPC_SGPIO->REG_SS[0]);
//		LPC_GPDMA->C1CONTROL = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);	
//	}else{
//		LPC_GPDMA->C1CONFIG = 1|(1<<14)|(1<<15);
//		LPC_SGPIO->CTR_STATUS_1 = 0xFFFF;
//
//		LPC_GPDMA->C3SRCADDR = (uint32_t) & (DMA_MEM->REG_SSdata[0]);
//		LPC_GPDMA->C3DESTADDR =	(uint32_t) & (LPC_SGPIO->REG_SS[0]);
//		LPC_GPDMA->C3CONTROL = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);
//	}		
//}




int x = 0;

uint32_t *p = (uint32_t*) 0x40002120, *q = (uint32_t*) 0x40002160; // p: channel 1, q: channel 3
void SGPIO_IRQHandler()
{
	uint32_t *swap;  
	if(x<10)
	{
		q[4] = 1|(1<<14)|(1<<15);
		LPC_SGPIO->CTR_STATUS_1 = 0xFFFF;
		
		p[0] =  ((uint32_t)&(DMA_MEM->REG_SSdata[0])+ x*64);
		p[1] = (uint32_t) & (LPC_SGPIO->REG_SS[0]);
		p[3] = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);
		swap = q;
		q = p;
		p = swap;	 
		x++;
	}	
}


//uint32_t *p = (uint32_t*) 0x40002120, *q = (uint32_t*) 0x40002160;		 // p: channel 1, q: channel 3
//void SGPIO_IRQHandler()
//{
//	uint32_t *swap;  
//
//	q[4] = 1|(1<<14)|(1<<15);
//	LPC_SGPIO->CTR_STATUS_1 = 0xFFFF;
//	
//	p[0] = (uint32_t) &(DMA_MEM->REG_SSdata[0]);
//	p[1] = (uint32_t) & (LPC_SGPIO->REG_SS[0]);
//	p[3] = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);	// 7 transfers, 32 bits 
//	swap = q;
//	q = p;
//	p = swap;	 
//		
//}

int x=0, y = 0;
uint32_t *p = (uint32_t*) 0x40002120, *q = (uint32_t*) 0x40002160;		 // p: channel 1, q: channel 3
void SGPIO_IRQHandler()
{
	uint32_t *swap;
	q[4] = 1|(1<<14)|(1<<15);
	LPC_SGPIO->CTR_STATUS_1 = 0xFFFF;
	y = x % 3; 
	switch (y)
	{
		case 0:
			p[0] = (uint32_t) &(DMA_MEM->REG_SSdata_1[0]);
			break;
		case 1:		
			p[0] = (uint32_t) &(DMA_MEM->REG_SSdata_2[0]);
			break;
		case 2:
			p[0] = (uint32_t) &(DMA_MEM->REG_SSdata_3[0]);
			break;
		default:
		break;	 
	}
	p[1] = (uint32_t) & (LPC_SGPIO->REG_SS[0]);
	p[3] = (0x10)|(0x4<<12)|(0x4<<15)|(0x2<<18)|(0x2<<21)|(1<<24)|(1<<26)|(1<<27)|(1<<29);	swap = q;
	q = p;
	p = swap;
	x++;	
}
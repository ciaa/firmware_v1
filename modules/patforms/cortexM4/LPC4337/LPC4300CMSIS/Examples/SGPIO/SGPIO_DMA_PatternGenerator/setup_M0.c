
#include "stdint.h"


extern void start_m0(void);
extern void SGPIO_IRQHandler(void);

void copy_VectorTable()
{
	int i;
	uint32_t *VectorTable_ptr_M4, *VectorTable_ptr_M0;
	VectorTable_ptr_M4 = ((uint32_t *) 0x10000000);
	VectorTable_ptr_M0 = ((uint32_t *) 0x20008000);
	for (i = 2; i<69; i++)
	{
		VectorTable_ptr_M0[i] = VectorTable_ptr_M4[i];
	}	
	VectorTable_ptr_M0[0] = 0x2000C000; //stack ptr
	VectorTable_ptr_M0[1] = ((uint32_t)start_m0);
	VectorTable_ptr_M0[35] = ((uint32_t)SGPIO_IRQHandler);
}

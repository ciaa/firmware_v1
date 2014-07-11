/*
 * ciaaInputs.c
 *
 *  Created on: Jun 12, 2014
 *      Author: pablo
 */

#include "ciaaIO.h"

void ciaaIOInit(void)
{
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Inputs  */
	Chip_SCU_PinMux(4,0,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO2[0]
	Chip_SCU_PinMux(4,1,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO2[1]
	Chip_SCU_PinMux(4,2,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO2[2]
	Chip_SCU_PinMux(4,3,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO2[3]
	Chip_SCU_PinMux(7,3,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO3[11]
	Chip_SCU_PinMux(7,4,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO3[12]
	Chip_SCU_PinMux(7,5,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO3[13]
	Chip_SCU_PinMux(7,6,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO3[14]

	Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,0xF, 0);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, 3, 0xF<<11, 0);

	/* MOSFETs */
	Chip_SCU_PinMux(4,8,MD_PUP,FUNC4); //GPIO5[12]
	Chip_SCU_PinMux(4,9,MD_PUP,FUNC4);//GPIO5[13]
	Chip_SCU_PinMux(4,10,MD_PUP,FUNC4);//GPIO5[14]
	Chip_SCU_PinMux(1,5,MD_PUP,FUNC0);//GPIO1[8]

	Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<12)|(1<<13)|(1<<14),1);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, 1,(1<<8),1);


	Chip_GPIO_SetValue(LPC_GPIO_PORT, 5,(1<<12)|(1<<13)|(1<<14));
	Chip_GPIO_SetValue(LPC_GPIO_PORT, 1,(1<<8));

	/* Relays */
	Chip_GPIO_SetDir(LPC_GPIO_PORT, 2,(1<<4)|(1<<5)|(1<<6),1);
	Chip_SCU_PinMux(2,1,MD_PUP,FUNC4);
	Chip_GPIO_SetDir(LPC_GPIO_PORT, 5,(1<<1),1);

	Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2,(1<<4)|(1<<5)|(1<<6));
	Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5,(1<<1));

	/* GPIOs */
	Chip_SCU_PinMux(6,1,MD_PUP|MD_EZI|MD_ZI,FUNC0);	//GPIO0/P6_1/GPIO3[0]
	Chip_SCU_PinMux(2,5,MD_PUP|MD_EZI|MD_ZI,FUNC4);	//GPIO1/P2_5/GPIO5[5]

}

uint32_t ciaaReadInput(uint32_t inputNumber)
{
	if(inputNumber > 7) return ~0;
	return ciaaDigitalInputs() & (1<<inputNumber);
}

uint32_t ciaaWriteOutput(uint32_t outputNumber, uint32_t value)
{
	switch(outputNumber)
	{
		case 0:
			if(value) Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<4);
			else Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<4);
			break;
		case 1:
			if(value) Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<5);
			else Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<5);
			break;
		case 2:
			if(value) Chip_GPIO_SetValue(LPC_GPIO_PORT, 2, 1<<6);
			else Chip_GPIO_ClearValue(LPC_GPIO_PORT, 2, 1<<6);
			break;
		case 3:
			if(value) Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<1);
			else Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<1);
			break;
		case 4:
			if(value) Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<12);
			else Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<12);
			break;
		case 5:
			if(value) Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<13);
			else Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<13);
			break;
		case 6:
			if(value) Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, 1<<14);
			else Chip_GPIO_SetValue(LPC_GPIO_PORT, 5, 1<<14);
			break;
		case 7:
			if(value) Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1, 1<<8);
			else Chip_GPIO_SetValue(LPC_GPIO_PORT, 1, 1<<8);
			break;
		default:
			return -1;
	}
	return 0;
}

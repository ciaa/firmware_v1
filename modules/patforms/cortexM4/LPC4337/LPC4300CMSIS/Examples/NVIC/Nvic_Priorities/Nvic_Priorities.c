/**********************************************************************
* $Id$		Nvic_Priorities.c			2011-06-02
*//**
* @file		Nvic_Priorities.c
* @brief	This example used to test NVIC Grouping Priority function
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "lpc43xx_libcfg.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_evrt.h"
#include "lpc43xx_dac.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup Nvic_Priorities	Nvic_Priorities
 * @ingroup NVIC_Examples
 * @{
 */

/************************** PRIVATE MACROS *************************/
//#define SAME_GROUP

#define LED_CONTROL_PORT_NUM			4
#define LED_CONTROL_BYTE_NUM			0
#define LED_CONTROL_ALL_CONFIG_OUTPUT	((1<<15)|(1<<1))
#define LED_CONTROL_CLEAR_ALL			((1<<15)|(1<<1))
#define OUTPUT							0x01
#define INPUT							0x00
#define DAC_LED_BIT_VALUE				(1<<15)
#define WAKEUP_LED_BIT_VALUE			(1<<1)


/************************** PRIVATE VARIABLES *************************/
uint16_t dac_cnt=0;
uint16_t dac_val=0;
Bool dac_led_state=FALSE;


/************************** PRIVATE FUNCTION *************************/
void DAC_IRQHandler(void);
void EVRT_IRQHandler(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		DAC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void DAC_IRQHandler (void)
{
	dac_cnt++;
	if(dac_cnt>1000)
	{
	 	dac_cnt = 0;
		if(dac_led_state)
		{
			dac_led_state = FALSE;
			GPIO_SetValue(LED_CONTROL_PORT_NUM,DAC_LED_BIT_VALUE);
		}
		else
		{
			dac_led_state = TRUE;
			GPIO_ClearValue(LED_CONTROL_PORT_NUM,DAC_LED_BIT_VALUE);
		}
	}
	dac_val++;
	if(dac_val>0x3FF) dac_val = 0;
	DAC_UpdateValue(LPC_DAC,dac_val);
}

/*********************************************************************//**
 * @brief		EVRT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EVRT_IRQHandler(void)
{
	uint8_t i;
	uint32_t j;

	if(EVRT_IsSourceInterrupting(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0))
	{
		EVRT_ClrPendIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0);
		for(i=0;i<5;i++)
		{
			GPIO_SetValue(LED_CONTROL_PORT_NUM,WAKEUP_LED_BIT_VALUE);
			for(j=0;j<300000;j++);
			GPIO_ClearValue(LED_CONTROL_PORT_NUM,WAKEUP_LED_BIT_VALUE);
			for(j=0;j<300000;j++);
		}
	}

}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main DAC program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	DAC_CONVERTER_CFG_Type DAC_ConverterConfigStruct;

	SystemInit();
	CGU_Init();

	// Configure LED control pins

	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// USB0_IND1 LED
	scu_pinmux(0x9 ,3 , MD_PDN, FUNC0); 	// USB1_IND1 LED

	GPIO_SetDir(LED_CONTROL_PORT_NUM, LED_CONTROL_ALL_CONFIG_OUTPUT, OUTPUT);
	// Configure EVRT to receive interrupt from WAKEUP0 pin
	EVRT_Init(LPC_EVENTROUTER);
	EVRT_ConfigIntSrcActiveType(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,EVRT_SRC_ACTIVE_RISING_EDGE);
	EVRT_SetUpIntSrc(LPC_EVENTROUTER,EVRT_SRC_WAKEUP0,ENABLE);
	// Configure DAC
	DAC_ConverterConfigStruct.CNT_ENA =SET;
	DAC_ConverterConfigStruct.DMA_ENA = RESET;
	DAC_Init(LPC_DAC);
	DAC_UpdateValue(LPC_DAC,dac_val);
	dac_cnt = 0;
	DAC_SetDMATimeOut(LPC_DAC,0xFFFF);
	DAC_ConfigDAConverterControl(LPC_DAC, &DAC_ConverterConfigStruct);


    /* Disable interrupt */
    NVIC_DisableIRQ(EVENTROUTER_IRQn);
    NVIC_DisableIRQ(DAC_IRQn);
#ifdef SAME_GROUP
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x01<<3)|0x07));
    NVIC_SetPriority(DAC_IRQn, ((0x01<<3)|0x02));
#else
    NVIC_SetPriority(EVENTROUTER_IRQn, ((0x00<<3)|0x01));
    NVIC_SetPriority(DAC_IRQn, ((0x01<<3)|0x01));
#endif
    /* Enable interrupt */
    NVIC_EnableIRQ(EVENTROUTER_IRQn);
    NVIC_EnableIRQ(DAC_IRQn);

	while(1);
}
/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
    return c_entry();
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/**
 * @}
 */

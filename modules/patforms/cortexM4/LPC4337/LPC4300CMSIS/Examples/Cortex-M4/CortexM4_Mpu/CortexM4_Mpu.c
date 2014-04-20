/**********************************************************************
* $Id$		CortexM3_Mpu.c		2011-06-02
*//**
* @file		CortexM3_Bitband.c
* @brief	This example used to test MPU function on Cortex-M3
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
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"
#include "debug_frmwrk.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup CortexM3_Mpu	CortexM3_Mpu
 * @ingroup Cortex_M3_Examples
 * @{
 */


 /************************** PRIVATE DEFINTIONS*************************/
/* Used LED */
#define LED1_BIT			1 //LEDUSB
#define LED1_PORT			4

/* MPU Attribute Register: Instruction Fetch Disable Bit */
#define MPU_ATTR_XN					(1<<28)
/* MPU Attribute Register: Access Permission Definition */
#define PRI_NOACCESS_USR_NOACCESS	(0x00)
#define PRI_RW_USR_NOACCESS			(0x01)
#define PRI_RW_USR_RO				(0x02)
#define PRI_RW_USR_RW				(0x03)
#define PRI_RO_USR_NOACCESS			(0x05)
#define PRI_RO_USR_RO				(0x06)
/* MPU Attribute Register: Access Permission Bit Field */
#define MPU_ATTR_AP(x)				((x&0x7)<<24)
/* MPU Attribute Register: TEX Bit Field */
#define MPU_ATTR_TEX(x)				((x&0x7)<<19)
/* MPU Attribute Register: Share Bit */
#define MPU_ATTR_SHARE				(1<<18)
/* MPU Attribute Register: Cache Bit */
#define MPU_ATTR_CACHE				(1<<17)
/* MPU Attribute Register: Buffer Bit */
#define MPU_ATTR_BUFFER				(1<<16)
/* MPU Attribute Register: Sub Region Bit Field */
#define MPU_ATTR_SRD(x)				((x&0xFF)<<8)
/* MPU Attribute Register: Region Size Bit Field */
#define MPU_ATTR_SIZE(x)			((x&0x1F)<<1)
/* MPU Attribute Register: Region Enable Bit */
#define MPU_ATTR_EN					(1)

/* Max Number Of Regions */
#define MPU_MAX_REGION				(8)


/************************** PRIVATE TYPES *************************/
typedef struct {
	uint32_t address;
	uint8_t size;
	uint8_t ap;
} MPU_REGION_INIT;

/************************** PRIVATE VARIABLES *************************/
uint8_t menu[]=
	"********************************************************************************\n\r"
	"Hello NXP Semiconductors \n\r"
	"MPU demo \n\r"
	"\t - MCU: lpc43xx \n\r"
	"\t - Core: ARM CORTEX-M3 \n\r"
	"\t - Communicate via: UART1 - 115200 bps \n\r"
	"Set up 8 region memory and try to access memory that don't allow to invoke\n\r"
	"Memory Management Handler\n\r"
	"********************************************************************************\n\r";
Bool Led_State = FALSE;

MPU_REGION_INIT lpc43xx_region[MPU_MAX_REGION] = {
	{0x10000000, 0x13, PRI_RW_USR_RW}, /*SRAM 1MB */
	{0x1A000000, 0x12, PRI_RW_USR_RW}, /*iFlash 512MB */
	{0x1B000000, 0x12, PRI_RW_USR_RW}, /*iFlash 512MB */
	{0x14000000, 0x13, PRI_RW_USR_RW}, /*SPIFI 1MB */
	{0x1C000000, 0x19, PRI_RW_USR_RW}, /*EXT MEM 64MB */
	{0x20000000, 0x19, PRI_NOACCESS_USR_NOACCESS}, /*AHB RAM 64MB */
	{0x28000000, 0x1A, PRI_RW_USR_RW}, /*DYCS0 128MB */
	{0x40000000, 0x19, PRI_RW_USR_NOACCESS}, /*AHB APB 64MB */
//	{0x60000000, 0x1C, PRI_RW_USR_RW}, /*DYCS2-3 512MB */
//	{0x80000000, 0x1A, PRI_RW_USR_NOACCESS}, /*SPIF data 128MB */
//	{0xE0000000, 0x13, PRI_RW_USR_NOACCESS}, /*ARM BUS 1MB */
};

/************************** PRIVATE FUNCTIONS *************************/
void MemManage_Handler(void);

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		Memory Management interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void MemManage_Handler(void)
{
	uint32_t i;
	//Blink LED1
	if(Led_State == FALSE)
	{
		GPIO_SetValue(LED1_PORT,(1<<LED1_BIT));
		Led_State = TRUE;
		for(i = 0;i<0x700000;i++);//delay
	}
	else
	{
		GPIO_ClearValue(LED1_PORT,(1<<LED1_BIT));
		Led_State = FALSE;
		for(i = 0;i<0x700000;i++);//delay
	}
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main MPU program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{
	uint32_t test;
	uint32_t i;

	SystemInit();
	CGU_Init();
	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();
	_DBG(menu);

	scu_pinmux(0x8 ,1 , MD_PDN, FUNC0); 	// P8.1 : USB0_IND1 LED

	//Turn off all LEDs
	GPIO_SetDir(LED1_PORT,(1<<LED1_BIT), 1);

	/*Set up MPU Regions */
	for(i=0;i< MPU_MAX_REGION;i++){
		MPU->RNR = i;//indicate MPU region 0
		MPU->RBAR = lpc43xx_region[i].address; // update the base address for the region 0
		MPU->RASR = MPU_ATTR_AP(lpc43xx_region[i].ap) | MPU_ATTR_SHARE | MPU_ATTR_CACHE | MPU_ATTR_BUFFER
								| MPU_ATTR_SIZE(lpc43xx_region[i].size) | MPU_ATTR_EN;
	}

	_DBG_("Setup MPU: \n\r"
		  "This provide 8 regions: \n\r"
		  "Region 0 - Local SRAM:       0x10000000 (1MB)\n\r"
		  "Region 1 - iFlash Memory A:  0x1A000000 (512MB)\n\r"
		  "Region 2 - iFlash Memory B:  0x1B000000 (512MB)\n\r"
		  "Region 3 - SPIFI Memory:     0x14000000 (1MB)\n\r"
		  "Region 4 - Static Memory:    0x1C000000 (64MB)\n\r"
		  "Region 5 - AHB RAM:          0x20000000 (64MB)\n\r"
		  "Region 6 - DYCS0:            0x28000000 (128MB)\n\r"
		  "Region 7 - AHB Peripheral:   0x40000000 (64MB)\n\r"
//		  "Region 5 - DYCS2 DYCS3:		0x60000000 (512MB)\n\r"
//		  "Region 6 - SPIF Data:		0x80000000 (128MB)\n\r"
//		  "Region 7 - ARM BUS:			0xE0000000 (1MB)\n\r"
		  "Region 5 can not access (just used for testing)");

	SCB->SHCSR |=(1<<16); //Enable Memory management fault
	MPU->CTRL =(1<<0); //Enable the MPU
	_DBG_("Enable MPU!");

	//try to access to this memory range
	_DBG_("Press '1' to try to read memory from region 1");
	while(_DG !='1');
	test = *((uint32_t *)0x10000000);
	_DBG_("Read successful!!!");

	_DBG_("Press '2' to try to read memory from region 5\n\r"
		  "Read memory at this region is not allow, LED D10 will blink...");
	while(_DG !='2');
	test = *((uint32_t *)0x20000000);

	while(test); /*anti auto remove from Compiler */
	return 1;
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

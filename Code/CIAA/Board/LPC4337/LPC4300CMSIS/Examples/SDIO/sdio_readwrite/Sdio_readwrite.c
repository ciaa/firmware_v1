/**********************************************************************
* $Id$		Sdio_readwrite.c		2012-08-15
*//**
* @file		Sdio_readwrite.c
* @brief	This example describes how to read and write from/to SD Card
* 			It also measures read and write speed.
* @version	1.0
* @date		08. Aug. 2012
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2012, NXP Semiconductor
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
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_timer.h"
#include "debug_frmwrk.h"
#include "lpc_sdmmc.h"
#include "lpc43xx_sdif.h"
#include "lpc43xx_sdmmc.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup SDIO_readwrite		SDIO_readwrite
 * @ingroup SDIO_Examples
 * @{
 */

/************************** PRIVATE DEFINITIONS ***********************/
/* CAUTION!!!!!!! READ THIS FIRST!!!!!!!
   Only enable write mode if you are using a SD/MMC card that can
   affort to lose data. This example actively writes to the card and
   will overwrite any data there corrupting files or file data. You
   may need to re-format your card after using this example with
   write enable. Use with care! */
#define ENABLE_WRITE_MODE

/* If you want to try multiple sector read/write capability, enable
   this define. Multiple read/write uses a different command vs the
   single sector read write for faster data transfer rates */
#define USE_MULTPLE_TRANSFER

/* Maximum multiple sector transfer size, 128 max limited by size
   of memory */
#define MULT_XFER_MAX_SECTORS 28

/* I've found it's easier to understand the pullup/pulldown defines
   seperate instead of using the combo MD_* macros */
#define MD_ENAB_PD (1<<3) /* Enable pull down resistor at pad */
#define MD_DIS_PU (1<<4) /* Disable pullup resistor at pad */

/* SD clock/data pins have fast slew rate, not glitch filter,
   buffered input, and no pulldown or pullup. Note that the board
   already has pullups on the necessary SD signals */
#define SDFASTINOUTPIN (MD_DIS_PU | MD_EZI | MD_EHS | MD_ZI)

/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
" SD/MMC read/write demo \n\r"
"\t - MCU: LPC18xx \n\r"
"\t - Core: ARM CORTEX-M3 \n\r"
"\t - Communicate via: UART0 - 115200 bps \n\r"
" Use SDIO to perform read and write into from/to Card\n\r"
"********************************************************************************\n\r";

#ifdef USE_MULTPLE_TRANSFER
static uint32_t lbuff[(MULT_XFER_MAX_SECTORS * MMC_SECTOR_SIZE) / sizeof(uint32_t)];
#else
static uint32_t lbuff[MMC_SECTOR_SIZE / sizeof(uint32_t)];
#endif
static volatile uint32_t u32Milliseconds;
static volatile uint32_t mci_status, Leddelay = 500;

/* Keeping errorc global makes it easier to see it in the debugger */
int32_t errorc;
uint32_t tstartr, tstopr, tstartw, tstopw;
struct _mci_card_struct sdcardinfo;
static volatile int32_t sdio_wait_exit = 0;

/************************** PRIVATE FUNCTION *************************/
void print_menu(void);
static int verify_data(int sectors, int initval, int inc);
static void prep_data(int initval, int inc);
/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void print_menu(void)
{
	_DBG(menu1);
}

/*******************************************************************************
* @brief		Wait for card program to finish
* @param[in]    None
* @return		None
*******************************************************************************/
 static void wait_for_program_finish(void)
{
	/* Poll card for program completion */
	while (sdmmc_get_state() != SDMMC_TRAN_ST);
}

 /*******************************************************************************
* @brief		System tick interrupt handler
* @param[in]    None
* @return		None
*******************************************************************************/
void SysTick_Handler (void)
{
	u32Milliseconds += 10;
}

/*******************************************************************************
* @brief		SDIO interrupt handler callback
* @param[in]    rinsts Optional input parameter
* @return		Return value is 0, not currently used
*******************************************************************************/
static uint32_t sdmmc_irq(uint32_t rinsts)
{
	/* Set wait exit flag to tell wait function we are ready. In an RTOS,
	   this would trigger wakeup of a thread waiting for the IRQ. */
	NVIC_DisableIRQ(SDIO_IRQn);
	sdio_wait_exit = 1;

	return 0;
}

/*********************************************************************//**
* @brief 		Sets up the SD event driven wakeup
* @param[in]	bits Status bits to poll for command completion
* @return 		None
**********************************************************************/
static void sdmmc_setup_wakeup(uint32_t bits)
{
    /* Wait for IRQ - for an RTOS, you would pend on an event here
	   with a IRQ based wakeup. */
	NVIC_ClearPendingIRQ(SDIO_IRQn);
	sdio_wait_exit = 0;
	LPC_SDMMC->INTMASK = bits;
	NVIC_EnableIRQ(SDIO_IRQn);
}

/*********************************************************************//**
* @brief 		A better wait callback for SDMMC driven by the IRQ flag
* @param[in]	bits Status bits to poll for command completion
* @return 		0 on success, or failure condition (-1)
**********************************************************************/
static uint32_t sdmmc_irq_driven_wait(uint32_t bits)
{
    uint32_t status;

	/* Wait for event, would be nice to have a timeout, but keep it
	   simple */
	while (sdio_wait_exit == 0);

	/* Get status and clear interrupts */
	status = LPC_SDMMC->RINTSTS;
	LPC_SDMMC->RINTSTS = status;
	LPC_SDMMC->INTMASK = 0;

	return status;
}

/*********************************************************************//**
* @brief 		Delay callback for timed SDIF/SDMMC functions
* @param[in]	time Number of milliSeconds to wait
* @return 		None
**********************************************************************/
void sdmmc_waitms(uint32_t time)
{
	/* In an RTOS, the thread would sleep allowing other threads to
	   run. For standalone operation, we just spin on a timer */
	TIM_Waitus(time * 1000);
}

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main SDIO program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int c_entry(void)
{

	uint32_t dms, sdio_clk;
	int32_t loops, blk;

	errorc = 1;

	SystemInit();
	CGU_Init();
	
	/* Setup muxing for UART interface */
	scu_pinmux(0x2 ,0 , MD_PDN, FUNC1); 	// PF.10 : UART0_TXD
	scu_pinmux(0x2 ,1 , MD_PLN|MD_EZI|MD_ZI, FUNC1); 	// PF.11 : UART0_RXD
	
	/* Initialize debug via UART1
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */
	debug_frmwrk_init();

	// print welcome screen
	print_menu();

	/* Configure the IO's for the LEDs */
		/* Setup muxing for SD interface */
	scu_pinmux(0xc ,9 , MD_DIS_PU, FUNC7);           /* Pc.9 SDIO power */       //checked
//	scu_pinmux(0xD ,1 , MD_DIS_PU, FUNC5);           /* Pc.9 SDIO power */


  scu_pinmux(0xc ,2 , MD_DIS_PU, FUNC7);           /* Pc.2 SDIO LED */				//not_found in user's manual, only there in schematic
	scu_pinmux(0xf ,10, MD_DIS_PU | MD_EZI, FUNC6);  /* Pf.10 SDIO WP */        //checked
//		scu_pinmux(0xd ,15, MD_DIS_PU | MD_EZI, FUNC5);  /* Pf.10 SDIO WP */        //checked
	scu_pinmux(0xc ,8 , MD_DIS_PU | MD_EZI, FUNC7);  /* Pc.8 SDIO CD */					//checked
	scu_pinmux(0xc ,7 , SDFASTINOUTPIN, FUNC7);      /* Pc.7 SDIO D3 */       //checked
	scu_pinmux(0xc ,6 , SDFASTINOUTPIN, FUNC7);      /* Pc.6 SDIO D2 */				//checked
	scu_pinmux(0xc ,5 , SDFASTINOUTPIN, FUNC7);      /* Pc.5 SDIO D1 */				 //checked
	scu_pinmux(0xc ,4 , SDFASTINOUTPIN, FUNC7);      /* Pc.4 SDIO D0 */       //checked
	scu_pinmux(0xc ,0 , MD_DIS_PU | MD_EHS, FUNC7);  /* Pc.0 SDIO clock */     //checked
	scu_pinmux(0xc ,10, SDFASTINOUTPIN, FUNC7);      /* Pc.10 SDIO command */ //checked
	

#if 0 /* Not used, SD interfaces bits D4 - D7 */
	scu_pinmux(0xc ,11 , SDFASTINOUTPIN, FUNC7);     /* Pc.7 SDIO D4 */
	scu_pinmux(0xc ,12 , SDFASTINOUTPIN, FUNC7);     /* Pc.7 SDIO D5 */
	scu_pinmux(0xc ,13 , SDFASTINOUTPIN, FUNC7);     /* Pc.7 SDIO D6 */
	scu_pinmux(0xc ,14 , SDFASTINOUTPIN, FUNC7);     /* Pc.7 SDIO D7 */
#endif

	LPC_SCU->SFSCLK_0 = 0x01;

	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_SDIO);

	/* Generate interrupt @ 100 Hz */
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 100);

	/* The SDIO driver needs to know the SDIO clock rate */
	sdio_clk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_SDIO);

	/* This init sdio with sdio_clk */
	sdif_init(sdio_clk, sdmmc_irq);

	/* Wait for a card to be inserted (note CD is not on the
	   SDMMC power rail and can be polled without enabling
	   SD slot power */
	while (!(sdif_card_ndetect()));
	lpc_printf("Card inserted...\r\n");

	/* Enable slot power, 0 to enable */
	sdif_power_onoff(0);

	/* Allow some time for the power supply to settle and the
	   card to fully seat in the slot */
	dms = u32Milliseconds + 250;
	while (dms > u32Milliseconds);

	/* Enumerate the card once detected. Note this function may
	   block for a little while. */
	if (!sdmmc_acquire(sdmmc_setup_wakeup, sdmmc_irq_driven_wait,
		sdmmc_waitms, &sdcardinfo)) {
		errorc = -1;
		goto error_exit;
	}

	/* Setup card specific callbacks - use driver functions, but these can
	   be changed to custom functions or unique functions per slot. These
	   aren't used in the drivers, so setup of these is optional, but they
	   are setup here to be complete. */
	sdcardinfo.sdck_det = sdif_card_ndetect;
	sdcardinfo.sdck_wp = sdif_card_wp_on;
	sdcardinfo.sd_setpow = sdif_power_onoff;
	sdcardinfo.sd_setrst = sdif_reset;

	if (sdcardinfo.sdck_wp())
		lpc_printf("Card is write protected, so write tests will be skipped\r\n");

	memset(lbuff, 0, sizeof(lbuff));
	if (sdmmc_read_blocks((void *) lbuff, 0, 0) == 0) {
		errorc = -2;
		goto error_exit;
	}
#ifdef ENABLE_WRITE_MODE
	lpc_printf("Press 1 to write data to sector 1 and verify:\r\n");
	while (_DG != '1');
	/* Write a single sector of data (512 bytes) */
	if (!sdcardinfo.sdck_wp()) {
		prep_data(0, 1);
		/* Warning: This may corrupt SD card data! */
		if (sdmmc_write_blocks((void *) lbuff, 1, 1) == 0) {
			errorc = -3;
			goto error_exit;
		}
		/* Wait for write to finish (at the card) */
		wait_for_program_finish();
	}
#endif
	memset(lbuff, 0, sizeof(lbuff));
	/* Read a single sector of data (512 bytes) */
	if (sdmmc_read_blocks((void *) lbuff, 1, 1) == 0) {
		errorc = -4;
		goto error_exit;
	}
#ifdef ENABLE_WRITE_MODE
	/* Verify data and halt if an error occurs */
	if (!sdcardinfo.sdck_wp()) {
		if (verify_data(1, 0, 1) == 0) {
			errorc = -5;
			goto error_exit;
		}
		lpc_printf("Verified!\r\n");
	}
#endif

#ifdef USE_MULTPLE_TRANSFER
#ifdef ENABLE_WRITE_MODE

	lpc_printf("Press 2 to write data in Multitransfer mode and then verify:\r\n");
	while (_DG != '2');

	/* Write data using multiple sector write */
	if (!sdcardinfo.sdck_wp()) {
		prep_data(0x10, 3);
		/* Warning: This may corrupt SD card data! */
		if (sdmmc_write_blocks((void *) lbuff, 1, MULT_XFER_MAX_SECTORS) == 0) {
			errorc = -6;
			goto error_exit;
		}
		wait_for_program_finish();
	}
#endif
	memset(lbuff, 0, sizeof(lbuff));
	/* Read data using multiple sector read */
	if (sdmmc_read_blocks((void *) lbuff, 1, MULT_XFER_MAX_SECTORS) == 0) {
		errorc = -7;
		goto error_exit;
	}

#ifdef ENABLE_WRITE_MODE
	/* Verify data and halt if an error occurs */
	if (!sdcardinfo.sdck_wp()) {
		if (verify_data(MULT_XFER_MAX_SECTORS, 0x10, 3) == 0) {
			errorc = -8;
			goto error_exit;
		}
		lpc_printf("Verified!\r\n");
	}
#endif

	lpc_printf("Press 3 to measure continuous read speed...\r\n");
	while (_DG != '3');
	tstartr = u32Milliseconds;
	loops = 1000;
	blk = 64;
	while (loops-- > 0) {
		if (sdmmc_read_blocks((void *) lbuff, blk, (blk + MULT_XFER_MAX_SECTORS - 1)) == 0) {
			errorc = -8;
			goto error_exit;
		}

		blk += MULT_XFER_MAX_SECTORS;
	}
	tstopr = u32Milliseconds;
	lpc_printf("read speed = %d kB/s\r\n", MULT_XFER_MAX_SECTORS*512*1000/(tstopr-tstartr));

#ifdef ENABLE_WRITE_MODE
	lpc_printf("Press 4 to measure continuous write speed...\r\n");
	while (_DG != '4');
	if (!sdcardinfo.sdck_wp()) {
		tstartw = u32Milliseconds;
		loops = 200;
		blk = 64;
		while (loops-- > 0) {
			if (sdmmc_write_blocks((void *) lbuff, blk, (blk + MULT_XFER_MAX_SECTORS - 1)) == 0) {
				errorc = -9;
				goto error_exit;
			}
			blk += MULT_XFER_MAX_SECTORS;
			wait_for_program_finish();
		}
		tstopw = u32Milliseconds;
	}
	lpc_printf("write speed = %d kB/s\r\n", MULT_XFER_MAX_SECTORS*512*200/(tstopw-tstartw));
#endif
#endif

	sdif_power_onoff(1); /* 1 to disable for Hitex board */
	sdif_deinit();
	while(1);

	return errorc;

error_exit:
	lpc_printf("Application failed.\r\n");
	while (1);
	return errorc;
}

/* Support required entry point for other toolchain */
int main (void)
{
	return c_entry();
}

/*******************************************************************************
* @brief		Generates a data pattern in a buffer
* @param[in]    initval Initial value
* @param[in]    inc Increment value
* @return		None
*******************************************************************************/
static void prep_data(int initval, int inc)
{
	int i;
	unsigned char *p = (unsigned char *) lbuff;

	for (i = 0; i < sizeof(lbuff); i++) {
		p[i] = ((unsigned char) initval) & 0xff;
		initval += inc;
	}
}

/*******************************************************************************
* @brief		Verifies a data pattern in a buffer
* @param[in]	sectors Numbers of sectors to check, 512 bytes per sector
* @param[in]    initval Initial value
* @param[in]    inc Increment value
* @return		0 if the verify fails, otherwise !0
*******************************************************************************/
static int verify_data(int sectors, int initval, int inc)
{
	int i = 0, passed = 1;
	uint8_t *cbuf = (uint8_t *) lbuff;
	uint8_t tab;

	while (i < (sectors * MMC_SECTOR_SIZE)) {
	tab = cbuf[i];
		if (cbuf[i] != (uint8_t) (initval & 0xff)){
			passed = 0;break;
			}
		initval += inc;
		i++;
	}
	tab = tab;
	return passed;
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

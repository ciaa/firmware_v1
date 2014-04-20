/***********************************************************************
 * $Id: main.c 6703 2011-06-20 11:55:38Z nlv10814 $
 *
 * Project: LPC18xx_43xx SDMMC access example
 *
 * Description: SDMMC access example project.
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

#include <string.h>
#include "LPC43xx.h"
#include "config.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "sdio.h"
#include "lpc_sdmmc.h"

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

//#define NGX_BOARD
#define HITEX_REVA_REWORK_BOARD

/* Maximum multiple sector transfer size, 128 max limited byy size
   of memory */
#define MULT_XFER_MAX_SECTORS 28

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

/**********************************************************************
 ** Function prototypes
 **********************************************************************/
static void BoardIOInit(void);
static int verify_data(int sectors, int initval, int inc);
static void prep_data(int initval, int inc);

/* Wait functions need to be defined outside the driver. They can be
   mapped to system wait functions */
#define TIMEUNIT 24
void timer_wait_us(void *t, volatile int us)
{
	us = us * TIMEUNIT;
	while (us > 0)
		us--;
}
void timer_wait_ms(void *t, volatile int ms)
{
	ms = ms * TIMEUNIT * 1000;
	while (ms > 0)
		ms--;
}

/**********************************************************************
 ** Function name: mci_wait_cb		
 **
 ** Description: Wait callback function for delays
 **						
 ** Parameters:	pdev : Pointer to card info structure
 **             bits: Bit mask against MCI status
 **
 ** Returned value:	Returns 1 on success, otherwise 0
 **********************************************************************/
uint32_t mci_wait_cb(MCI_CARD_INFO_T* pdev, uint32_t bits)
{
	/* This isn't the best example for an event driven mode. The wait
	   function is called when the IP is processing a command and/oo
	   data. Ideally, this should sleep until woken up by an interrupt
	   event (but that requires an OS). We'll just loop until the
	   interrupt tells us to exit. */
	mci_status = 0;
	while (mci_status == 0);

	/* Just return the SDIO controller status */
	return mci_status;
}

/**********************************************************************
 ** Function name: mci_irq_cb		
 **
 ** Description: IRQ callback function for events
 **						
 ** Parameters:	pdev : Pointer to card info structure
 **             irqstatus: MCI IRQ status
 **
 ** Returned value:	Returns 1 on success, otherwise 0
 **********************************************************************/
uint32_t mci_irq_cb(MCI_CARD_INFO_T* pdev, uint32_t irqstatus)
{
	mci_status = irqstatus;
	return 0;
}

/**********************************************************************
 ** Function name: wait_for_program_finish		
 **
 ** Description: Wait for card program to finish
 **						
 ** Parameters:	pdev : None
 **
 ** Returned value:	None
 **********************************************************************/
#if 0
 static void wait_for_program_finish(void)
{
	while (sdio_get_state() == SDMMC_PRG_ST);
	while (sdio_get_state() != SDMMC_TRAN_ST);
}
#endif

/**********************************************************************
 ** Function name: SysTick_Handler
 **
 ** Description: System tick interrupt handler
 **						
 ** Parameters: None	
 **
 ** Returned value: None
 **********************************************************************/
void usbd_msc_sdio_SysTick_Handler (void) 					
{
	static uint32_t msl = 0;

	u32Milliseconds += 10;
	if (u32Milliseconds > msl) {
		msl = u32Milliseconds + Leddelay;

		/* Toggle LED state */
#ifdef NGX_BOARD 
		LPC_GPIO_PORT->NOT[1] = (1UL << 11);
#else
		LPC_GPIO_PORT->NOT[4] = (1UL << 14);
		LPC_GPIO_PORT->NOT[5] = (1UL << 19);
#endif
	}
}

/* I've found it's easier to understand the pullup/pulldown defines
   seperate instead of using the combo MD_* macros */
#define MD_ENAB_PD (1<<3) /* Enable pull down resistor at pad */
#define MD_DIS_PU (1<<4) /* Disable pullup resistor at pad */

/* SD clock/data pins have fast slew rate, not glitch filter,
   buffered input, and no pulldown or pullup. Note that the board
   already has pullups on the necessary SD signals */
#define SDFASTINOUTPIN (MD_DIS_PU | MD_EZI | MD_EHS | MD_ZI)

/**********************************************************************
 ** Function name: BoardIOInit
 **
 ** Description: Initialize board pin muxing
 **						
 ** Parameters: None	
 **
 ** Returned value: None
 **********************************************************************/
static void BoardIOInit(void)
{

#ifdef NGX_BOARD
	/* Setup muxing for SD interface NGX*/
	scu_pinmux(0x1 ,11 , SDFASTINOUTPIN, FUNC7);      /* P1.11 SDIO D2 */
	scu_pinmux(0x1 ,10 , SDFASTINOUTPIN, FUNC7);      /* P1.10 SDIO D1 */
	scu_pinmux(0x1 ,9 , SDFASTINOUTPIN, FUNC7);      /* P1.9 SDIO D0 */
	scu_pinmux(0x1 ,6, SDFASTINOUTPIN, FUNC7);      /* P1.6 SDIO command */
	scu_pinmux(0x1 ,12 , SDFASTINOUTPIN, FUNC7);      /* P1.12 SDIO D3 */

	LPC_SCU->SFSCLK_0 = 0x04;						/*	CLK0 Enable*/
#else
	/* Setup muxing for SD interface */
	scu_pinmux(0xc ,2 , MD_DIS_PU, FUNC7);           /* Pc.2 SDIO LED */
	scu_pinmux(0xf ,10, MD_DIS_PU | MD_EZI, FUNC6);  /* Pf.10 SDIO WP */
	scu_pinmux(0xc ,8 , MD_DIS_PU | MD_EZI, FUNC7);  /* Pc.8 SDIO CD */
	scu_pinmux(0xc ,6 , SDFASTINOUTPIN, FUNC7);      /* Pc.6 SDIO D2 */
	scu_pinmux(0xc ,5 , SDFASTINOUTPIN, FUNC7);      /* Pc.5 SDIO D1 */
	scu_pinmux(0xc ,4 , SDFASTINOUTPIN, FUNC7);      /* Pc.4 SDIO D0 */
	scu_pinmux(0xc ,0 , MD_DIS_PU | MD_EHS, FUNC7);  /* Pc.0 SDIO clock */
	scu_pinmux(0xc ,10, SDFASTINOUTPIN, FUNC7);      /* Pc.10 SDIO command */
	scu_pinmux(0xc ,7 , SDFASTINOUTPIN, FUNC7);      /* Pc.7 SDIO D3 */

#ifdef HITEX_REVA_REWORK_BOARD
	scu_pinmux(0xc ,9 , MD_DIS_PU, FUNC7);           /* Pc.9 SDIO power */
#else
	scu_pinmux(0xd ,1 , MD_DIS_PU, FUNC5);           /* Pc.9 SDIO power */
	LPC_SCU->SFSCLK_2 = 0x04;						/*	CLK2 Enable*/
#endif

#endif

}

/**********************************************************************
 ** Function name: prep_data		
 **
 ** Description: Generates a data pattern in a buffer
 **						
 ** Parameters:	initval : Initial value
 **              inc : Increment value
 **
 ** Returned value:	None
 **********************************************************************/
static void prep_data(int initval, int inc)
{
	int i;
	unsigned char *p = (unsigned char *) lbuff;

	for (i = 0; i < sizeof(lbuff); i++) {
		p[i] = ((unsigned char) initval) & 0xff;
		initval += inc;
	}
}

/**********************************************************************
 ** Function name: verify_data		
 **
 ** Description: Verifies a data pattern in a buffer
 **						
 ** Parameters:	sectors: Numbers of sectors to check, 512 bytes per sector
 **             initval : Initial value
 **             inc : Increment value
 **
 ** Returned value:	0 if the verify fails, otherwise !0
 **********************************************************************/
static int verify_data(int sectors, int initval, int inc)
{
	int i = 0, passed = 1;
	uint8_t *cbuf = (uint8_t *) lbuff;

	while (i < (sectors * MMC_SECTOR_SIZE)) {
		if (cbuf[i] != (uint8_t) (initval & 0xff))
			passed = 0;
		initval += inc;
		i++;
	}

	return passed;
}

/**********************************************************************
 ** Function name: main		
 **
 ** Description: main application for SDIO access
 **						
 ** Parameters:	None
 **
 ** Returned value:	Returns 1 on success, otherwise <0
 **********************************************************************/

int usbd_msc_sdio_init (void) 
{
	uint32_t dms;

#ifdef USE_NXP_EVAL
	uint8_t u8Ledcntr = 0;
#endif

	errorc = 1;

	/* Configure the IO's for the LEDs */ 
	BoardIOInit();

	/* Setup SDIO clocking. This must be done prior to using any
	   functions of the SDIO driver. Use the CPU PLL divided
	   by 1 as the base clock. */
	CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVB);
	CGU_EnableEntity(CGU_CLKSRC_IDIVB, ENABLE);
	CGU_SetDIV(CGU_CLKSRC_IDIVB, 4);
	CGU_EntityConnect(CGU_CLKSRC_IDIVB, CGU_BASE_SDIO);
	CGU_UpdateClock();

	/* The SDIO driver needs to know the SDIO clock rate */
	sdio_clk_rate = CGU_GetPCLKFrequency(CGU_PERIPHERAL_SDIO);

#ifdef SDIO_USE_POLLING
	/* This variant will use polling and the timer_wait_* functions */
	sdio_init((void *) 0, (void *) 0);
#else
	/* This variant is (mostly) event driven */
	sdio_init(mci_wait_cb, mci_irq_cb);
#endif

#ifndef NGX_BOARD
	/* Wait for a card to be inserted (note CD is not on the
	   SDMMC power rail and can be polled without enabling
	   SD slot power */
	while (!(sdio_card_detect()));

	/* Enable slot power */
	sdio_power_off();
	sdio_power_on();
#endif

	/* Allow some time for the power supply to settle and the
	   card to fully seat in the slot */
//	dms = u32Milliseconds + 250;
//	while (dms > u32Milliseconds);

	/* Enumerate the card once detected. Note this function may
	   block for a little while. */
	if (!sdio_acquire()) {
		errorc = -1;
		goto error_exit;
	}

	/* Read sector 0. This sector usually contains the partition
	   table for the device, so it has at least some non-0 data */
	memset(lbuff, 0, sizeof(lbuff));
	if (sdio_read_blocks((void *) lbuff, 0, 0) == 0) {
		errorc = -2;
		goto error_exit;
	}
 

#if 0
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

	scu_pinmux(0x3 ,1 , MD_DIS_PU, FUNC4);
//    LPC_GPIO5->DIR |= (1UL << 8);            // GPIO3_6 Output
	scu_pinmux(0x3 ,2 , MD_DIS_PU, FUNC4);
//    LPC_GPIO5->DIR |= (1UL << 9);            // GPIO3_6 Output
	memset(lbuff, 'a', sizeof(lbuff));
	while(1)
	{
		 static int block = 10;
		 volatile int returnval;
//		LPC_GPIO5->NOT = (1UL << 8);
		
		if ((returnval = sdio_read_blocks((void *) lbuff, block, block + 19)) == 0) {
			errorc = -2;
			//goto error_exit;
		}
		if ((returnval = sdio_write_blocks((void *) lbuff, block, block + 19)) == 0) {
			errorc = -2;
			//goto error_exit;
		}
	   	block++;
	}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#endif




	return errorc;

#if 0

#ifdef ENABLE_WRITE_MODE
	/* Write a single sector of data (512 bytes) */
	if (!sdio_card_wp_on()) {
		prep_data(0, 1);
		/* Warning: This may corrupt SD card data! */
		if (sdio_write_blocks((void *) lbuff, 1, 1) == 0) {
			errorc = -3;
			goto error_exit;
		}
		/* Wait for write to finish (at the card) */
		wait_for_program_finish();
	}
#endif
	memset(lbuff, 0, sizeof(lbuff));
	/* Read a single sector of data (512 bytes) */
	if (sdio_read_blocks((void *) lbuff, 1, 1) == 0) {
		errorc = -4;
		goto error_exit;
	}

#ifdef ENABLE_WRITE_MODE
	/* Verify data and halt if an error occurs */
	if (!sdio_card_wp_on()) {
		if (verify_data(1, 0, 1) == 0) {
			errorc = -5;
			goto error_exit;
		}
	}
#endif

#ifdef USE_MULTPLE_TRANSFER
#ifdef ENABLE_WRITE_MODE
	/* Write data using multiple sector write */
	if (!sdio_card_wp_on()) {
		prep_data(0x10, 3);
		/* Warning: This may corrupt SD card data! */
		if (sdio_write_blocks((void *) lbuff, 1, MULT_XFER_MAX_SECTORS) == 0) {
			errorc = -6;
			goto error_exit;
		}
		wait_for_program_finish();
	}
#endif
	memset(lbuff, 0, sizeof(lbuff));
	/* Read data using multiple sector read */
	if (sdio_read_blocks((void *) lbuff, 1, MULT_XFER_MAX_SECTORS) == 0) {
		errorc = -7;
		goto error_exit;
	}

#ifdef ENABLE_WRITE_MODE
	/* Verify data and halt if an error occurs */
	if (!sdio_card_wp_on()) {
		if (verify_data(MULT_XFER_MAX_SECTORS, 0x10, 3) == 0) {
			errorc = -8;
			goto error_exit;
		}
	}
#endif

	{
		int loops, blk;

		/* Read a lot of data and time it */
		/* My old 2G SD card does this in 1.7s, about 8MB/s */
		tstartr = u32Milliseconds;
		loops = 1000;
		blk = 64;
		while (loops-- > 0) {
			if (sdio_read_blocks((void *) lbuff, blk, (blk + MULT_XFER_MAX_SECTORS - 1)) == 0) {
				errorc = -8;
				goto error_exit;
			}
	
			blk += MULT_XFER_MAX_SECTORS;
		}
		tstopr = u32Milliseconds;
	}

#ifdef ENABLE_WRITE_MODE
	if (!sdio_card_wp_on()) {
		/* Write a lot of data and time it */
		/* My old 2G SD card does this in 4s, about 3.5MB/s */
		tstartw = u32Milliseconds;
		loops = 1000;
		blk = 64;
		while (loops-- > 0) {
			if (sdio_write_blocks((void *) lbuff, blk, (blk + MULT_XFER_MAX_SECTORS - 1)) == 0) {
				errorc = -9;
				goto error_exit;
			}
			blk += MULT_XFER_MAX_SECTORS;
			wait_for_program_finish();
		}
		tstopw = u32Milliseconds;
	}
#endif
#endif

	sdio_power_onoff(0);
	sdio_deinit();
	Leddelay = 2000; /* Blink slower on success */
	while(1);

#endif // #if 0

error_exit:
	Leddelay = 100; /* Blink faster on error */
	while (1);
	return errorc;
}


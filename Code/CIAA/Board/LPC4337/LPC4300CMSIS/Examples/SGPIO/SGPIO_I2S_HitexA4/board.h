
/***********************************************************************
* $Id:: board.h
*
* Project: USB audio device class demo
*
* Description:
*     Header file containing various LPC18xx/43xx related defines
*
***********************************************************************
*   Copyright(C) 2012, NXP Semiconductor
*   All rights reserved.
*
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
***********************************************************************/
#ifndef __BOARD_H 
#define __BOARD_H

///////////////////////////////////////////////////////////////////////////
//	shared memory between CPUs
//

#define HACK_BUFFER		0x10090020

#define sGPIO_buffer(x)		( * (volatile int *) (HACK_BUFFER + (x<<2)))

#define outta_sGPIO_buffer	( * (volatile unsigned char *) (HACK_BUFFER-4))
#define into_sGPIO_buffer	( * (volatile unsigned char *) (HACK_BUFFER-8))



#include <stdint.h>
#include "usbd/error.h"

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _BIT
#define _BIT(n) (((uint32_t)(1ul)) << (n))
#endif

/* debug macros */
#if defined(__DEBUG_RAM)
#include <stdio.h>
#define dbg(format, arg...) printf("%s: " format, __FILE__ , ## arg)
#else
#define dbg(format, arg...) do {} while (0)
#endif

/* include chip headers & corresponding settings*/

/****************************************************************
LPC18xx boards
****************************************************************/ 
/* port to which UDA1380 is connected */
#define UDA1380_I2C_PORT  0


/* Board specifc SDK function prototypes. The implementation of following 
   functions exisit in individual board_xxxxx.c & sdk_lpcXXXX.c files.
*/

typedef struct {
  uint8_t slave_address;
  uint8_t sub_address; 
  uint16_t rsvrd0;
  uint16_t ms_timeout;
  uint16_t buf_sz; 
  uint8_t* buf;
} I2C_Xfer_t;


/* board initilisation function */
void sdk_board_init(unsigned char * name);
/* timer functions */
extern volatile uint32_t g_sys_ticks;
static __inline uint32_t ms_timer_count(void)
{
  return (g_sys_ticks);
}

/* LED & button functions */
uint8_t sdk_get_buttons(void);
void sdk_set_leds(uint8_t leds);
/* I2C functions */
ErrorCode_t sdk_i2c_init(uint8_t port, uint32_t rate_khz);
ErrorCode_t sdk_i2c_read(uint8_t port, I2C_Xfer_t* rd_cfg);
ErrorCode_t sdk_i2c_write(uint8_t port, I2C_Xfer_t* wr_cfg);

/* I2S functions */

/* timer SDK functions */
void sdk_busy_sleep(uint32_t msecs);

/* USB functions */


/* codec functions */
void sdk_uda1380_sysclk_init(void);

#ifdef __cplusplus
   }
#endif
 
#endif /* end __BOARD_H */
/******************************************************************************
**                            End Of File
******************************************************************************/

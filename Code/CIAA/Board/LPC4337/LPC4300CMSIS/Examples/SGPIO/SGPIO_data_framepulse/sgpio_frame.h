/***********************************************************************
 * $Id: sgpio_frame.h 3858 2010-07-15 15:33:12Z nxp28536 $
 *
 * Project: LPC43xx SGPIO emulation
 *
 * Description: function definition for the frame snc + data emulation
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
 **********************************************************************/
 #ifndef __SGPIO_FRAME_H__
#define __SGPIO_FRAME_H__

#include <stdint.h>
#include <stdbool.h>


#include "sgpio.h"

typedef uint32_t pulsePattern;


/* spi emulation API calls */

/* initialize the peripheral */
void SGPIO_frameInit(void);

/* open a simulated SPI interface */
void SGPIO_frameOpen(void);

/* close a simulated SPI interface */
void SGPIO_frameClose(void);

/* write one data item to the frame interface */
void SGPIO_frameWrite(uint32_t data);

/* check if a new data item can be written */
bool isFrameReady(void);


/* used internally to check configuration errors within the code */
emuStatus SGPIO_frameCheckErrors (void) __attribute__((always_inline));

/* modify sgpio register contents outside the static configuration */
/* this shall be replaced by internal functions for all modules */
emuStatus SGPIO_frameIoctl(const SGPIO_Slice sliceId, const sliceIoctl cmd, const void* param) __attribute__((always_inline));



#endif


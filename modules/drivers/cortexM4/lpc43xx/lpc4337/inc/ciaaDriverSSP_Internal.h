/* Copyright 2016, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2016, Marcos Ziegler <ziegler.marcos.b@gmail.com>
 * All rights reserved.
 *
 *    or
 *
 * Copyright 2016, ACSE & CADIEEL & Marcos Ziegler <ziegler.marcos.b@gmail.com
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _CIAADRIVERSSP_INTERNAL_H_
#define _CIAADRIVERSSP_INTERNAL_H_
/** \brief Internal Header file of UART Driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup SSP SSP Drivers
 ** @{ */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
/** \brief Contains info about a SSP peripheral */
typedef struct
{
   /* TODO: Add more fields? */
   LPC_SSP_T *pSSP; /* SSP peripheral base address */
   SSP_ConfigFormat ssp_format;
   uint32_t mode;
   /* TODO: Keep information of CS pins in structure? Takes memory. Maybe should define instead */
   uint8_t CS_port; /* CS pin port */
   uint8_t CS_pin;  /* CS pin number */
   /* TODO: Keep methods in structure as a C++ object? Not sure if its a good practice */
   uint16_t (*RWByte)(ciaaDriverSSP_SSPDevType *SSP_dev, uint8_t tx_data); /* Byte transfer function */
   void (*set_CS)(ciaaDriverSSP_SSPDevType *SSP_dev, uint8_t value); /* CS set function */
   /* It is sometimes needed to change the clock rate */
   void (*set_ClockRate)(ciaaDriverSSP_SSPDevType *SSP_dev, uint32_t clk_rate);
} ciaaDriverSSP_SSPDevType;

/*==================[external data declaration]==============================*/
/** \brief SSP 0 */
extern ciaaDriverSSP_SSPDevType ciaaDriverSSP_SSP0;

/** \brief SSP 1 */
extern ciaaDriverSSP_SSPDevType ciaaDriverSSP_SSP1;

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _CIAADRIVERSSP_INTERNAL_H_ */

/* Copyright 2014, 2015, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
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

/** \brief HisIO Example source file
 **
 ** This is a mini example how to use HISIO Module
 ** without rtos.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup HISIO HisIO Module Example
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * JMC          Juan Manuel Cruz
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151222 v0.0.1   MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "hisio_example.h"    /* <= own header */
#include "IODriver.h"         /* <= IO Driver header files */
#include "ciaaPOSIX_stdint.h" /* <= DataTypes */

#define CPU lpc4337

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns never
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   int counter = 0;
   int32_t loop;

   /* Initialize the Dio Driver. The name MyDioConfig is the
    * one given in the DIL Configuration File */
   Dio_InitSync(0);

   /* to perform a while 1 is NEVER recomended, but in this case to perform a
    * really simple example we used anyway. :( */
   while(1) {
      counter++;

      /* alternate the led to high and low */
      if (counter & 0x1)
      {
         /* set the Led1 pin to HIGH, this will really set the port to 0V since
          * the pin is configured as inverted in the DIL File */
         Dio_SetSync(LED1, IO_HIGH);
         Dio_SetSync(LED2, IO_HIGH);
         Dio_SetSync(LED3, IO_HIGH);
         Dio_SetSync(LED0_R, IO_HIGH);
         Dio_SetSync(LED0_G, IO_HIGH);
         Dio_SetSync(LED0_B, IO_HIGH);
      }
      else
      {
         Dio_SetSync(LED1, IO_LOW);
         Dio_SetSync(LED2, IO_LOW);
         Dio_SetSync(LED3, IO_LOW);
         Dio_SetSync(LED0_R, IO_LOW);
         Dio_SetSync(LED0_G, IO_LOW);
         Dio_SetSync(LED0_B, IO_LOW);
      }

      /* set the DataBus port to the value 0x3F */
/*      Dio_SetPortSync(DataBus, 0x3F);
*/
      /* set the DataBus port to 0xbb3F, the high part of the 16 bit por
       * is not changed due to the 0xFF mask */
/*      Dio_SetPortMaskedSync(DataBus, 0x3F, 0xFF);
*/
      /* DIO Notifications will not be supported in te first version */

      for(loop = 0; loop < 0x3FFFFF; loop++) {
         /* do nothing */
         /* again a good example how not to perform a dealy. this delay
          * will keep the cpu on his maximal speed and wasting energy... :( */
      }
   }


   return 0;
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


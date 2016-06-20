/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 * Copyright 2016, Alvaro Alonso Bivou <alonso.bivou@gmail.com>
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

/** \brief Simon Says game implementation for EDU-CIAA
 **
 ** This is the Simon Says game. The light sequence must be
 ** replicated by the user pressing the buttons.
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Games Simon example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * AA         Alvaro Alonso
 */

 /*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160611 v0.0.2   AA   random sequence option
 * 20160607 v0.0.1   AA   first functional version
 */

/*==================[inclusions]=============================================*/
#include "SimonSays.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t leds_fd;
/** \brief File descriptor for digital input ports
 *
 * Device path /dev/dio/in/0
 */
static int32_t buttons_fd;
/** \brief Current state of the States Machine
 * Initial State or state after reset is IDLE
 * IDLE: The machine is waiting for an input
 */
static states state = IDLE;
/** \brief Array of the sequence to be displayed by the leds **/
#if RANDOM_SEQUENCE
static uint8_t sequence[100];
#else
static const uint8_t sequence[]={1, 2, 0, 1, 3, 3, 1, 2, 1, 0, 1, 1, 0, 1, 2, 2, 3, 3, 1, 1, 2, 1, 1, 3, 3, 3, 1, 0, 2, 1, 0, 0, 3, 0, 2, 1, 3, 2, 0, 3, 2, 0, 2, 2, 0, 2, 2, 1, 0, 0, 3, 2, 2, 2, 2, 1, 2, 2, 2, 0, 2, 2, 3, 2, 2, 1, 3, 2, 3, 2, 3, 3, 1, 1, 2, 0, 0, 0, 2, 1, 1, 2, 1, 3, 2, 0, 3, 3, 3, 2, 3, 3, 0, 0, 0, 0, 2, 3, 0, 1};
#endif
/** \brief Initial level of the game **/
static uint8_t level=0;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(Normal);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode Normal.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();
   /* open CIAA digital outputs & inputs */
   leds_fd = ciaaPOSIX_open(OUTPUTS, ciaaPOSIX_O_RDWR);
   buttons_fd = ciaaPOSIX_open(INPUTS, ciaaPOSIX_O_RDONLY);
   /* activate periodic task:
    *  - OutputTask: it updates the game outputs.
    *  - InputTask: it updates the user inputs, starts immediately.
    */
   SetRelAlarm(ActivateOutputTask, START_DELAY_OUTPUT_MS, REFRESH_RATE_OUTPUT_MS);
   SetRelAlarm(ActivateInputTask, 0, REFRESH_RATE_INPUT_MS);
   /* terminate task */
   TerminateTask();
}

/** \brief Input Task
 *
 * This task is started automatically every time that the alarm
 * ActivateInputTask expires: every REFRESH_RATE_INPUT_MS.
 * This is a polling task for monitoring buttons states
 *
 */
TASK(InputTask)
{
   /* buffer variables for input and output */
   uint8_t inputs_now, button_pressed, outputs;
   /* inputs store from the previous execution of InputTask */
   static uint8_t inputs_old;
   /* current index of the sequence array*/
   static uint8_t index=0;

   /* refresh buttons states */
   (void)ciaaPOSIX_read(buttons_fd, &inputs_now, 1);
   /* inverse button logic */
   inputs_now = ~inputs_now;
   /* detect a change on buttons state */
   button_pressed = (~inputs_old) & (inputs_now);
   /* States Machine */
   switch(state)
      {
      /* IDLE State: it is waiting for an user inputs*/
      case IDLE:
         /* Buttons has change state*/
         if(button_pressed & ANY_BUTTON)
         {
            state = START;
#if RANDOM_SEQUENCE
            /* seed random number generator */
            srand(inputs_now);
            /* complete the sequence array with random numbers */
            for(index=0; index<sizeof(sequence); index++)
               sequence[index] = (uint8_t)(rand()%4);
            index=0;
#endif
         }
         break;
      /* Listen State: it checks if the user input is correct or not */
      case LISTEN:
         /* light led pressed */
         outputs = (inputs_now<<2);
         (void)ciaaPOSIX_write(leds_fd, &outputs, 1);
         /* Buttons has change state and is the correct button*/
         if(button_pressed & (BUTTON_<<sequence[index]))
         {
            state = (index < level) ? LISTEN : INCREASE;
            index++;
         }
         /* Buttons has change state but is NOT the correct button*/
         else if(button_pressed & ANY_BUTTON)
         {
            state = FAIL;
         }
         break;
      /* Increase State: it moves the game to the next level */
      case INCREASE:
         outputs = 0;
         index = 0;
         (void)ciaaPOSIX_write(leds_fd, &outputs, 1);
         state = SEQUENCE;
         level = (level < sizeof(sequence)) ? (level+1) : 0;
         break;
      default:
         break;
   }
   /* store the buttons state */
   inputs_old = inputs_now;
   /* terminate task */
   TerminateTask();
}

/** \brief Output Task
 *
 * This task is started automatically every time that the alarm
 * ActivateOutputTask expires: every REFRESH_RATE_OUTPUT_MS.
 * This is a refresh task for updating leds states
 *
 */
TASK(OutputTask)
{
   /* buffer variable for POXIS_write function */
   static uint8_t outputs=0x00;
   /* Current index of the sequence array*/
   static uint8_t index=0;

   /* States Machine */
   switch(state)
      {
      /* Start State: It flash all the leds to show the game has started */
      case START:
         state =  (index > 1) ? SEQUENCE : START;
         outputs = (state == SEQUENCE) ? 0 : outputs^ALL_LED;
         index = (state == SEQUENCE) ? 0 : (index+1);
         level = 0;
         break;
      /* Sequence State: Flash a single led that correspond according to
       *                 the sequence */
      case SEQUENCE:
         state =  (index > (level*2)) ? LISTEN : SEQUENCE;
         outputs = (state == LISTEN) ? 0 : outputs^(LED_<<sequence[index/2]);
         index = (state == LISTEN) ? 0 : (index+1);
         break;
      /* Fail State: It flash all the leds showing the level reached in the game */
      case FAIL:
         state =  (index > (level*2)) ? IDLE : FAIL;
         outputs = (state == IDLE) ? 0 : outputs^ALL_LED;
         index = (state == IDLE) ? 0 : (index+1);
         break;
      default:
         outputs=0x00;
         break;
   }
   /* update leds states */
   (void)ciaaPOSIX_write(leds_fd, &outputs, 1);
   /* terminate task */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

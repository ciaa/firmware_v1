/* Copyright 2014, Mariano Cerdeiro
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

/** \brief Blinking_echo example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Blinking Blinking_echo example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 * GMuro        Gustavo Muro
 * JPV          Juan Pablo Vecchio
 * JR           Joaquin Rodriguez
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141019 v0.0.2   JuCe add printf in each task,
 *                        remove trailing spaces
 * 20140731 v0.0.1   PR   first functional version
 * 20150703 v0.0.3   JPV  Drivers for GPIOs
 * 20150704 v0.0.4   JR   Keyboard drivers and first Functions
 */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "blinking.h"         /* <= own header */

/*==================[macros and definitions]=================================*/
#define ciaaPOSIX_IOCTL_GPIO_OUT 10
#define ciaaPOSIX_IOCTL_GPIO_IN 11
#define ciaaGPIO_0 (1<<0)
#define ciaaGPIO_1 (1<<1)
#define ciaaGPIO_2 (1<<2)
#define ciaaGPIO_3 (1<<3)
#define ciaaGPIO_4 (1<<4)
#define ciaaGPIO_5 (1<<5)
#define ciaaGPIO_6 (1<<6)
#define ciaaGPIO_7 (1<<7)
#define ciaaGPIO_8 (1<<8)


#define KEYB_LINE_CTRL   11
#define MAX_COLUMN       3

#define MATH_FUNCTIONS      1
#define NUMBER_FUNCTIONS    2
#define BASICOP_FUNCTIONS   3
#define SHIFT_FUNCTION      4
#define CHANGE_FUNCTION     5

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */

uint8_t change_keyb_func = NUMBER_FUNCTIONS;
int (*functions[4][4])(void);
static int32_t fd_out, fd_in, fd_gpio_out, fd_gpio_in,fd_keyb;
static uint32_t Periodic_Task_Counter;
static uint8 pulsador1=0;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/


/*==================[external functions definition]==========================*/
extern void ciaaKeyboard_MainTask(void);
void ciaaKeyboard_FunctionInit(uint8_t);
void ciaaKeyboard_MainTask(void);
int Empty_func(void);

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
   StartOS(AppMode1);

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
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");

   /* open CIAA GPIO inputs-outputs */

   fd_gpio_out = ciaaPOSIX_open("/dev/gpio/out/0", O_RDWR);

   fd_gpio_in = ciaaPOSIX_open("/dev/gpio/in/0", O_RDONLY);

   /* open CIAA digital inputs-outputs */

   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);

   /* open CIAA Keyboard from PONCHO  */

   fd_keyb = ciaaPOSIX_open("/dev/keyb/keyb0",O_RDWR);
   if(fd_keyb == (-1)){
      /* Error. Keyboard can not be opened */
      ciaaPOSIX_printf("Error creating OS Thread timer!\n");
      while(1);
   }

   /* activate periodic task:
    *  - for the first time after 350 ticks (350 ms)
    *  - and then every 250 ticks (250 ms)
    */

   Periodic_Task_Counter = 0;
   SetRelAlarm(ActivatePeriodicTask, 350, 250);
   SetRelAlarm(KeyboardLoop, 100, 250);

   /* terminate task */
   TerminateTask();
}


/** \brief Periodic Task
 *
 * This task is started automatically every time that the alarm
 * ActivatePeriodicTask expires.
 *
 */

TASK(PeriodicTask)
{
   uint8 outputs, inputs;
   uint16 gpoutputs;
   int32_t result;

   ciaaPOSIX_printf("Blinking\n");

   /* Secuencia Aleatoria */
   /*
   outputs = (1<<(rand()%6));
   ciaaPOSIX_write(fd_out, &outputs, 1);
   outputs = 0;
   */
   /***********************/

   /* Escribe las 9 GPIO Salidas de manera intermitente */
   /*
   ciaaPOSIX_read(fd_gpio_out, &gpoutputs, 1);
   gpoutputs ^= 0x1FF;
   ciaaPOSIX_write(fd_gpio_out, &gpoutputs, 1);
   */
   /*****************************************************/

   /* Uso ioclt para cambiar la GPIO 0 a entrada */
   result = ciaaPOSIX_ioctl(fd_gpio_out, ciaaPOSIX_IOCTL_GPIO_IN, ciaaGPIO_0);
   if((-1)==result){
      /* Error. GPIO can not be changed */
      ciaaPOSIX_printf("Error changing GPIO as input!\n");
      while(1);
   }

   /* Leo las entradas GPIO */
   /* Puenteando GPIO y GND devuelve 0, sino 1 */
   ciaaPOSIX_read(fd_gpio_in, &gpoutputs, 1);

   /* Leo las salidas GPIO */
   ciaaPOSIX_read(fd_gpio_out, &gpoutputs, 1);


   /* Lectura Pulsador 1 */

   ciaaPOSIX_read(fd_in, &inputs, 1);

   if(inputs == 1){
	   if(pulsador1==0){
		   pulsador1=1;
	   }else{
		   pulsador1=0;
	   }
   }

   /* Secuencia de los 6 leds */
   /* La inicio o pauso con el pulsador 1 */

   if(pulsador1 == 1){

	   ciaaPOSIX_read(fd_out, &outputs, 1);

	   outputs ^= 1<<Periodic_Task_Counter;

	   ciaaPOSIX_write(fd_out, &outputs, 1);

	   Periodic_Task_Counter++;
	   if(Periodic_Task_Counter>5){
		   Periodic_Task_Counter=0;
	   }
   }


   /* terminate task */
   TerminateTask();
}

/* Keyboard Task */

TASK(Keyboard_Handler){
      ciaaKeyboard_MainTask();
      TerminateTask();
}

void ciaaKeyboard_MainTask(void){

    static uint8_t Column, Row = 0;
    static uint8_t prev_col = 5;
    static uint8_t prev_row = 5;
    uint8_t err;

    if(0 != change_keyb_func){
    	ciaaKeyboard_FunctionInit(change_keyb_func);
    }

	err = ciaaPOSIX_ioctl(fd_keyb,KEYB_LINE_CTRL,Row);

	if(err == (-1)){
	   /* Error. ioctl function cannot be done */
	   ciaaPOSIX_printf("Error changing Active Line\n");
	   while(1);
	}


	err = ciaaPOSIX_read(fd_keyb,&Column,1);

	if(err == (-1)){
	   /* Error. read function cannot be done */
	   ciaaPOSIX_printf("Error reading Keyboard\n");
	   while(1);
	}

	if(0x0F != Column)
	{

	if((Row != prev_row) && (Column != prev_col))
	{
		err = functions[Row][Column]();
	    prev_row = Row;
	    prev_col = Column;
	}
	}

	if(MAX_COLUMN <= Row){
		Row = 0;
	}
	else{
		Row++;
	}
}

/** \brief Keyboard Functions Initialization
 *
 * This function executes reconfigurates the botton functions.
 *
 ** \param[in]  kind of function from the Keyboard. The options are:
 ** MATH_FUNCTIONS
 ** NUMBER_FUNCTIONS
 ** BASICOP_FUNCTIONS
 ** SHIFT_FUNCTION
 ** CHANGE_FUNCTION
  */

void ciaaKeyboard_FunctionInit(uint8_t change_keyb_func){

	switch(change_keyb_func){
	case CHANGE_FUNCTION:
	   functions[0][0] = Empty_func;
	   functions[0][1] = Empty_func;
	   functions[0][2] = Empty_func;
	   functions[0][3] = Empty_func;
	   functions[1][0] = Empty_func;
	   functions[1][1] = Empty_func;
	   functions[1][2] = Empty_func;
	   functions[1][3] = Empty_func;
	   functions[2][0] = Empty_func;
	   functions[2][1] = Empty_func;
	   functions[2][2] = Empty_func;
	   functions[2][3] = Empty_func;
	   functions[3][0] = Empty_func;
	   functions[3][1] = Empty_func;
	   functions[3][2] = Empty_func;
	   functions[3][3] = Empty_func;
	   break;
	case NUMBER_FUNCTIONS:
		   functions[0][0] = Empty_func;
		   functions[0][1] = Empty_func;
		   functions[0][2] = Empty_func;
		   functions[0][3] = Empty_func;
		   functions[1][0] = Empty_func;
		   functions[1][1] = Empty_func;
		   functions[1][2] = Empty_func;
		   functions[1][3] = Empty_func;
		   functions[2][0] = Empty_func;
		   functions[2][1] = Empty_func;
		   functions[2][2] = Empty_func;
		   functions[2][3] = Empty_func;
		   functions[3][0] = Empty_func;
		   functions[3][1] = Empty_func;
		   functions[3][2] = Empty_func;
		   functions[3][3] = Empty_func;
		   break;
	}
	change_keyb_func = 0;
return;
}

int Empty_func(void){
	change_keyb_func = 0;
	return 1;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


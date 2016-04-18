/* Copyright 2015, 2016 Diego Ezequiel Vommaro
 * Copyright 2015, ACSE & CADIEEL
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

/** \brief Real time control system example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup RTCS RTCS example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151101 v0.0.1 Dev  initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "ciaaLibs_Matrix.h"  /* <= matrix operations header */
#include "Rtcs.h"             /* <= real time control system header */
#include "rtcs_example.h"         /* <= own header */

/*==================[macros and definitions]=================================*/
#define INPUT_PLANT_QTY 1     /* <= quantity of plant inputs */
#define OUTPUT_PLANT_QTY 2    /* <= quantity of plant outputs */
#define STATE_VAR_QTY 2       /* <= quantity of plant state variables */
#define INITIAL_DELAY_MS 100         /* <= initial delay of the Control Task (It's optional) */
#define CONTROL_TASK_PERIOD_MS 100   /* <= control task period (It's inverse to control frequency) */
#define BUFFER_SIZE 20         /* <= Size of buffer to send by serial port */
#define ADJUST_CONSTANT 1000.0F     /* <= Adjust constant to send to PC as char data */
#define INACTIVE_POINTS 125   /* <= quantity of points in zero of the input step */
#define ACTIVE_POINTS 250     /* <= quantity of points in one of the input step */
#define TOTAL_POINTS 500      /* <= total points of the step */
#define KR 2.9755F            /* <= gain of the reference input */

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;

/** \brief File descriptor of the USB uart
 *
 * Device path /dev/serial/uart/1
 */
static int32_t fd_uart1;

/** \brief File descriptor of the RS232 uart
 *
 * Device path /dev/serial/uart/2
 */
static int32_t fd_uart2;

/** \brief Fundamental matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float fund_matrix_data[] = { 0.8625F, 258.4922F,
                                   -0.0009F,   0.6748F };
/** \brief Transition matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float tran_matrix_data[] = { 0.1375F,
                                    0.0009F };

/** \brief C matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float c_matrix_data[] = { 1.0F, 0.0F,
                                 0.0F, 1.0F };

/** \brief D matrix data of an electric plant
 **
 ** RLC circuit
 **/
static float d_matrix_data[] = { 0.0F, 0.0F };

/** \brief state vector data of an electric plant
 **
 ** RLC circuit
 **/
static float x_vector_data[] = { 0.0F, 0.0F };

/** \brief auxiliar state vector data of an electric plant
 **
 ** RLC circuit
 **/
static float x_aux_vector_data[] = { 0.0F, 0.0F };

/** \brief output vector data of an electric plant
 **
 ** RLC circuit
 **/
static float y_vector_data[] = { 0.0F, 0.0F };

/** \brief output vector data of an electric plant
 **
 ** RLC circuit
 **/
static float y_aux_vector_data[] = { 0.0F, 0.0F };

/** \brief control efforts vector data of an electric plant
 **
 ** RLC circuit
 **/
static float u_vector_data[] = { 0.0F };

/** \brief Fundamental matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t fund_matrix;

/** \brief Transition matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t tran_matrix;

/** \brief C matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t c_matrix;

/** \brief D matrix of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t d_matrix;

/** \brief state vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t x_vector;

/** \brief auxiliar state vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t x_aux_vector;

/** \brief output vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t y_vector;

/** \brief auxiliar output vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t y_aux_vector;

/** \brief control efforts vector of an electric plant
 **
 ** RLC circuit
 **/
static ciaaLibs_matrix_t u_vector;

/** \brief Input points of the control effort vector
 **
 ** RLC circuit
 **/
static float u_in[TOTAL_POINTS];

/** \brief Input points of the control reference vector
 **
 ** RLC circuit
 **/
static float r_in[TOTAL_POINTS];

/** \brief Index of the control reference signal
 **
 ** RLC circuit
 **/
static uint32_t index = 0;

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
 * This task started automatically in the application mode 1.
 * It initializes the needed hardware and initializes The Real Time Control System Module (rtcs)
 */
TASK(InitTask)
{
   uint32_t k;

   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   /* open UART connected to USB bridge (FT2232) */
   fd_uart1 = ciaaPOSIX_open("/dev/serial/uart/1", ciaaPOSIX_O_RDWR);

   /* open UART connected to RS232 connector */
   fd_uart2 = ciaaPOSIX_open("/dev/serial/uart/2", ciaaPOSIX_O_RDWR);

   /* change baud rate for uart usb */
   ciaaPOSIX_ioctl(fd_uart1, ciaaPOSIX_IOCTL_SET_BAUDRATE, (void *)ciaaBAUDRATE_115200);

   /* change FIFO TRIGGER LEVEL for uart usb */
   ciaaPOSIX_ioctl(fd_uart1, ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL, (void *)ciaaFIFO_TRIGGER_LEVEL3);

   /* matrix initialization to simulate the RLC plant */
   ciaaLibs_MatrixInit(&fund_matrix, STATE_VAR_QTY, STATE_VAR_QTY, CIAA_LIBS_FLOAT_32, fund_matrix_data);
   ciaaLibs_MatrixInit(&tran_matrix, STATE_VAR_QTY, INPUT_PLANT_QTY, CIAA_LIBS_FLOAT_32, tran_matrix_data);
   ciaaLibs_MatrixInit(&c_matrix, OUTPUT_PLANT_QTY, STATE_VAR_QTY, CIAA_LIBS_FLOAT_32, c_matrix_data);
   ciaaLibs_MatrixInit(&d_matrix, OUTPUT_PLANT_QTY, INPUT_PLANT_QTY, CIAA_LIBS_FLOAT_32, d_matrix_data);
   ciaaLibs_MatrixInit(&x_vector, STATE_VAR_QTY, 1, CIAA_LIBS_FLOAT_32, x_vector_data);
   ciaaLibs_MatrixInit(&x_aux_vector, STATE_VAR_QTY, 1, CIAA_LIBS_FLOAT_32, x_aux_vector_data);
   ciaaLibs_MatrixInit(&y_vector, OUTPUT_PLANT_QTY, 1, CIAA_LIBS_FLOAT_32, y_vector_data);
   ciaaLibs_MatrixInit(&y_aux_vector, OUTPUT_PLANT_QTY, 1, CIAA_LIBS_FLOAT_32, y_aux_vector_data);
   ciaaLibs_MatrixInit(&u_vector, INPUT_PLANT_QTY, 1, CIAA_LIBS_FLOAT_32, u_vector_data);

   /* control efforts and reference vectors initialization */
   for(k = 0; k < INACTIVE_POINTS; k++)
   {
      r_in[k] = 0.0F;
      u_in[k] = 0.0F;
   }
   for(k = INACTIVE_POINTS; k < (INACTIVE_POINTS + ACTIVE_POINTS); k++)
   {
      u_in[k] = 1.0F;
      r_in[k] = 1.0F * KR;
   }
   for(k = (INACTIVE_POINTS + ACTIVE_POINTS); k < TOTAL_POINTS; k++)
   {
      u_in[k] = 0.0F;
      r_in[k] = 0.0F;
   }

   /*Real tiem control system init*/
   Rtcs_Init();

   /* Activate Control task */
   SetRelAlarm(ActivateControlTask, INITIAL_DELAY_MS, CONTROL_TASK_PERIOD_MS);

   /* terminate task */
   TerminateTask();
}

/** \brief Control task
 *
 * This task controls the RLC plant wich is simulated here
 */
TASK(ControlTask)
{
   float data_to_send;
   int8_t buf[BUFFER_SIZE];

#ifndef OPEN_LOOP
   /* Start closed loop control section */

   /* Load measurement vector */
   Rtcs_InputY1_PlantRLC(&x_vector_data[0]);

   /* Load reference vector */
   Rtcs_InputRefX1_PlantRLC(&r_in[index]);

   /* Controller function of the RLC plant (it should be called every 1 milisecond) */
   Rtcs_PlantRLC_1ms();

   /* End closed loop control section */
#endif

   /* Start plant simulation section (delete this section when the plant is a real process) */

   /* Calculate the next point of the RLC plant simulation */
#ifdef OPEN_LOOP
   *u_vector_data = u_in[index];
#endif
   ciaaLibs_MatrixMul_float(&fund_matrix, &x_vector, &x_vector);
   ciaaLibs_MatrixMul_float(&tran_matrix, &u_vector, &x_aux_vector);
   ciaaLibs_MatrixAdd_float(&x_vector, &x_aux_vector, &x_vector);
   ciaaLibs_MatrixMul_float(&c_matrix, &x_vector, &y_vector);
   ciaaLibs_MatrixMul_float(&d_matrix, &u_vector, &y_aux_vector);
   ciaaLibs_MatrixAdd_float(&y_vector, &y_aux_vector, &y_vector);

   /* Send output of the RLC plant simulation to stdout (on x86 only) */
   ciaaPOSIX_printf("%fmV %fmA %d\n", x_vector_data[0]*1000, x_vector_data[1]*1000, index);

#if (ARCH == cortexM4)
   /* Send output of the RLC plant simulation to PC */
   data_to_send = (int32_t)(x_vector_data[0] * ADJUST_CONSTANT);
   MyItoa(data_to_send, buf);
   ciaaPOSIX_write(fd_uart1, buf, ciaaPOSIX_strlen((char *)buf));
   ciaaPOSIX_write(fd_uart1, "mV ", 3);
   data_to_send = (int32_t)(x_vector_data[1] * ADJUST_CONSTANT);
   MyItoa(data_to_send, buf);
   ciaaPOSIX_write(fd_uart1, buf, ciaaPOSIX_strlen((char *)buf));
   ciaaPOSIX_write(fd_uart1, "mA ", 3);
   MyItoa(index, buf);
   ciaaPOSIX_write(fd_uart1, buf, ciaaPOSIX_strlen((char *)buf));
   ciaaPOSIX_write(fd_uart1, "\n", 1);
#endif
   /* End plant simulation section */

   /* Increment reference signal index */
   index++;
   index %= ACTIVE_POINTS;
   
   /* terminate task */
   TerminateTask();
}

/** \brief Send Control Efforts to plant
 *
 * This function sends control efforts to plant.
 * In this case, it sends to the input vector (u_vector_data) of the simulated plant
 */
extern void SendControlEffort (float *data, uint16_t num_elements)
{
   float *u_vector_data_ptr = u_vector_data;

   while(num_elements > 0u)
   {
     *u_vector_data_ptr++ = *data++;
     num_elements--;
   }
}

/** \brief Convert one integer to characters to s string
 **
 ** This function converts one integer to characters.
 */
extern void MyItoa(int32_t num, int8_t *s)
{
   int32_t i, sign;

   /* record sign  */
   if ((sign = num) < 0)
      /* make num positive */
      num = -num;
   i = 0;
   /* generate digits in reverse order */
   do
   {
      /* get next digit */
      s[i++] = num % 10 + '0';
   }  
   /* delete it */
   while ((num /= 10) > 0);
   if (sign < 0)
   {
      s[i++] = '-';
   }
   s[i] = '\0';
   /* invert the order of characteres*/
   StringReverse(s);
}

/** \brief Invert the order in a string
 **
 ** This function inverts the order in a string.
 */
extern void StringReverse(int8_t *s)
{
   int32_t i, j;
   int8_t c;

   for (i = 0, j = ciaaPOSIX_strlen((char *)s)-1; i<j; i++, j--)
   {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
   }
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


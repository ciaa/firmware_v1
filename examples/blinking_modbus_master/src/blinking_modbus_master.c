/* Copyright 2014, Gustavo Muro
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

/** \brief Blinking Modbus example source file
 **
 ** This is a mini example of the CIAA Firmware
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Blinking_Modbus_master Blinking Modbus master example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * GMuro        Gustavo Muro
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141108 v0.0.1 GMuro   initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaModbus.h"
#include "ciaak.h"
#include "blinking_modbus_master.h"

/*==================[macros and definitions]=================================*/
#define CIAA_BLINKING_MODBUS_ID     2

#define CIAA_MODBUS_ADDRESS_INPUTS  0X0000
#define CIAA_MODBUS_ADDRESS_OUTPUS  0X0001

typedef enum
{
   CIAA_BLINKING_MOD_MAST_STATE_READING = 0,
   CIAA_BLINKING_MOD_MAST_STATE_WRITING,
   CIAA_BLINKING_MOD_MAST_STATE_COMPLETE,
}ciaaBlinkingModMast_stateEnum;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static int32_t hModbusMaster;
static int32_t hModbusAscii;
static int32_t hModbusGateway;

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;

/** \brief File descriptor for digital inputs ports
 *
 * Device path /dev/dio/in/0
 */
static int32_t fd_in;

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
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   int32_t fdSerialPort;

   /* init the ciaa kernel */
   ciaak_start();

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", ciaaPOSIX_O_RDWR);

   fdSerialPort = ciaaPOSIX_open("/dev/serial/uart/0", ciaaPOSIX_O_RDWR | ciaaPOSIX_O_NONBLOCK);

   /* Open Modbus Master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* Open Transport Modbus Ascii */
   hModbusAscii = ciaaModbus_transportOpen(
         fdSerialPort,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   /* Open Gateway Modbus */
   hModbusGateway = ciaaModbus_gatewayOpen();

   /* Add Modbus Slave to gateway */
   ciaaModbus_gatewayAddMaster(
         hModbusGateway,
         hModbusMaster);

   /* Add Modbus Transport to gateway */
   ciaaModbus_gatewayAddTransport(
         hModbusGateway,
         hModbusAscii);

   SetRelAlarm(ActivateModbusTask, 5, CIAA_MODBUS_TIME_BASE);

   SetRelAlarm(AlarmCallBackPollingSlave, 10, 100);

   /* end InitTask */
   TerminateTask();
}

/** \brief Modbus Task
 *
 * This task is activated by the Alarm ActivateModbusTask.
 */
TASK(ModbusMaster)
{
   ciaaModbus_gatewayMainTask(hModbusGateway);

   TerminateTask();
}

/** \brief CallBack Activate Polling Slave Task
 *
 * This function activate task PollingSlave.
 */
ALARMCALLBACK(CallBackActivatePollingSlave)
{
   TaskStateType state;

   GetTaskState(PollingSlave, &state);

   if (SUSPENDED == state)
   {
      ActivateTask(PollingSlave);
   }
}

/** \brief Polling Slave Task
 *
 * This task is activated by the CallBackActivatePollingSlave
 * and it self if communication pending.
 */
TASK(PollingSlave)
{
   ciaaBlinkingModMast_stateEnum stateModMast = CIAA_BLINKING_MOD_MAST_STATE_READING;
   int16_t hrValue;
   int8_t ret;

   do
   {
      switch (stateModMast)
      {
         /* reading inputs of CIAA slave modbus */
         default:
         case CIAA_BLINKING_MOD_MAST_STATE_READING:

            /* read inputs from ciaa modbus slave */
            ret = ciaaModbus_masterCmd0x03ReadHoldingRegisters(
                  hModbusMaster,
                  CIAA_MODBUS_ADDRESS_INPUTS,
                  1,
                  &hrValue,
                  CIAA_BLINKING_MODBUS_ID,
                  NULL);

            if (CIAA_MODBUS_E_NO_ERROR == ret)
            {
               /* write outputs */
               ciaaPOSIX_write(fd_out, &hrValue, 1);

               /* set next state */
               stateModMast = CIAA_BLINKING_MOD_MAST_STATE_WRITING;
            }
            break;

         /* writing inputs in to outputs of CIAA slave modbus */
         case CIAA_BLINKING_MOD_MAST_STATE_WRITING:
            /* read inputs */
            hrValue = 0;
            ciaaPOSIX_read(fd_in, &hrValue, 1);

            /* write in to ciaa slave */
            ret = ciaaModbus_masterCmd0x10WriteMultipleRegisters(
                  hModbusMaster,
                  CIAA_MODBUS_ADDRESS_OUTPUS,
                  1,
                  &hrValue,
                  CIAA_BLINKING_MODBUS_ID,
                  NULL);

            if (CIAA_MODBUS_E_NO_ERROR != ret)
            {
               /* do nothing, next state idle */
            }

            /* set next state */
            stateModMast = CIAA_BLINKING_MOD_MAST_STATE_COMPLETE;
            break;
      }

   }while (CIAA_BLINKING_MOD_MAST_STATE_COMPLETE > stateModMast);

   TerminateTask();
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


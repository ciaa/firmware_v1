/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Gustavo Muro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
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
/** \addtogroup Blinking_Modbus Blinking Modbus example source file
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
 * 20140805 v0.0.1   GMuro first functional version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaModbus.h"
#include "ciaak.h"
#include "blinking_modbus_slave.h"

/*==================[macros and definitions]=================================*/
#define CIAA_BLINKING_MODBUS_ID     2

#define CIAA_MODBUS_ADDRESS_INPUTS  0X0000
#define CIAA_MODBUS_ADDRESS_OUTPUS  0X0001

/*==================[internal data declaration]==============================*/

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

/*==================[internal functions declaration]=========================*/
static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       );

static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      );


/*==================[internal data definition]===============================*/
static int32_t hModbusSlave;
static int32_t hModbusAscii;
static int32_t hModbusGateway;

static const ciaaModbus_slaveCmd_type callbacksStruct =
{
   NULL,
   NULL,
   cmd0x03ReadHoldingReg,
   NULL,
   NULL,
   NULL,
   NULL,
   cmd0x10WriteMultipleReg,
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       )
{
   /* used to store holding register value */
   uint16_t hrValue;
   /* used to indicate quantity of registers processed */
   int16_t quantityRegProcessed;
/* used to indicate total of registers reads */
   int8_t ret = 0;
/* used to store input or outputs byte */
   uint8_t inOutValue;

   /* loop to read all registers indicated */
   do
   {
      /* select register address to be read */
      switch (start)
      {
         /* read inputs of CIAA */
         case CIAA_MODBUS_ADDRESS_INPUTS:
            ciaaPOSIX_read(fd_in, &inOutValue, 1);
            hrValue = inOutValue;
            ciaaModbus_writeInt(buf, hrValue);
            quantityRegProcessed = 1;
            break;

         /* read outputs of CIAA */
         case CIAA_MODBUS_ADDRESS_OUTPUS:
            ciaaPOSIX_read(fd_out, &inOutValue, 1);
            hrValue = inOutValue;
            ciaaModbus_writeInt(buf, hrValue);
            quantityRegProcessed = 1;
            break;

         /* wrong address */
         default:
            *exceptioncode = CIAA_MODBUS_E_WRONG_STR_ADDR;
            quantityRegProcessed = -1;
            break;
      }

      /* if quantityRegProcessed > 0, successful operation */
      if (quantityRegProcessed > 0)
      {
         /* update buffer pointer to next register */
         buf += (quantityRegProcessed*2);

         /* next address to be read */
         start += quantityRegProcessed;

         /* increment count of registers */
         ret += quantityRegProcessed;
      }
      else
      {
         /* an error occurred in reading */
         ret = -1;
      }
      /* repeat until:
      * - read total registers or
      * - error occurs
      */
   }while ((ret > 0) && (ret < quantity));

   return ret;
}

static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      )
{
   /* used to store holding register value */
   uint16_t hrValue;
   /* used to indicate quantity of registers processed */
   int16_t quantityRegProcessed;
   /* used to store input or outputs byte */
   uint8_t inOutValue;

   /* loop to write all registers indicated */
   do
   {
      /* select register address to be write */
      switch (start)
      {
         /* inputs can not be written! */
         case CIAA_MODBUS_ADDRESS_INPUTS:
            *exceptioncode = CIAA_MODBUS_E_FNC_ERROR;
            quantityRegProcessed = -1;
            break;

         /* write outputs */
         case CIAA_MODBUS_ADDRESS_OUTPUS:
            hrValue = ciaaModbus_readInt(buf);
            inOutValue = hrValue & 0xFF;
            ciaaPOSIX_write(fd_out, &inOutValue, 1);
            quantityRegProcessed = 1;
            break;

         /* wrong address */
         default:
            *exceptioncode = CIAA_MODBUS_E_WRONG_STR_ADDR;
            quantityRegProcessed = -1;
            break;
      }

      /* if quantityRegProcessed > 0, successful operation */
      if (quantityRegProcessed > 0)
      {
         /* update buffer pointer to next register */
         buf += (quantityRegProcessed*2);

         /* next address to be write */
         start += quantityRegProcessed;

         quantity -= quantityRegProcessed;
      }
      else
      {
         quantity = 0;
      }

      /* repeat until:
      * - read total registers or
      * - error occurs
      */
   }while (0 < quantity);

}


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

   /* Open Modbus Slave */
   hModbusSlave = ciaaModbus_slaveOpen(
         &callbacksStruct,
         CIAA_BLINKING_MODBUS_ID);

   /* Open Transport Modbus Ascii */
   hModbusAscii = ciaaModbus_transportOpen(
         fdSerialPort,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE);

   /* Open Gateway Modbus */
   hModbusGateway = ciaaModbus_gatewayOpen();

   /* Add Modbus Slave to gateway */
   ciaaModbus_gatewayAddSlave(
         hModbusGateway,
         hModbusSlave);

   /* Add Modbus Transport to gateway */
   ciaaModbus_gatewayAddTransport(
         hModbusGateway,
         hModbusAscii);

   SetRelAlarm(ActivateModbusTask, 100, 5);

   /* end InitTask */
   TerminateTask();
}

/** \brief Modbus Task
 *
 * This task is activated by the Alarm ActivateModbusTask.
 */
TASK(ModbusSlave)
{
   ciaaModbus_gatewayMainTask(hModbusGateway);

   TerminateTask();
}





/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


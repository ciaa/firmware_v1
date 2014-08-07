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
#include "ciaaModbusSlave.h"
#include "ciaak.h"
#include "blinking_modbus.h"

/*==================[macros and definitions]=================================*/
/** \brief Modbus Address for Input Register
 *
 * Input Register address: 0x0000
 */
#define MODBUS_ADDR_IR_INPUT_REG_VAL      0x0000

/** \brief Holding Registers Addresses
 *
 * Holding Register Inputs:   0x0000
 * Holding Register Outputs:  0x0001
 */
#define MODBUS_ADDR_HR_CIAA_INPUTS        0X0000
#define MODBUS_ADDR_HR_CIAA_OUTPUTS       0X0001

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Inputs registers variables
 *
 * Input Registers:
 * Accessed through:
 *             - 0x04 Read Input Register
 *   address      ---------------------
 *   0x0000      |   inputRegVal       |
 *                ---------------------
 */
static uint16_t inputRegVal;

/** \brief Holding Register Input
 *
 * Holding Register Inputs Variable
 * Accessed through:
 *             - 0x03 Read Holding Register
 *             - 0x17 Read/Write multiple Register
 */
static uint16_t hr_ciaaInputs;

/** \brief Holding Register Output
 *
 * Holding Register Output Variable
 * Accessed through:
 *             - 0x03 Read Holding Register
 *             - 0x06 Write single Register
 *             - 0x10 Write multiple Register
 *             - 0x17 Read/Write multiple Register
 */
static uint16_t hr_ciaaOutputs;

/** \brief File descriptor for digital input ports
 *
 * Device path /dev/dio/in/0
 */
static int32_t fd_in;

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
int main(void)
{
   StartOS(AppMode1);
   return 0;
}

void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

TASK(InitTask) {
   /* init the ciaa kernel */
   ciaak_start();

   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   ActivateTask(ModbusSlave);

   /* terminate task */
   TerminateTask();
}

TASK(Blinking)
{
   uint8_t uint8Data;

   /* blink */
   hr_ciaaOutputs ^= 0x10;

   /* update outputs */
   uint8Data = hr_ciaaOutputs;
   ciaaPOSIX_write(fd_out, &uint8Data, sizeof(uint8Data));

   /* read inputs */
   ciaaPOSIX_read(fd_in, &uint8Data, sizeof(uint8Data));
   //hr_ciaaInputs = uint8Data;
   hr_ciaaInputs ++;

   /* terminate task */
   TerminateTask();
}

TASK(ModbusSlave)
{
   /* initialize modbus slave */
   ciaaModbus_init("/dev/serial/uart/0");

   /* start modbus main task */
   ciaaModbus_slaveMainTask();

   /* terminate task */
   TerminateTask();
}


extern int8_t readInputRegisters(
      uint16_t startingAddress,
      uint16_t quantityOfInputRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      )
{
   int8_t ret;

   /* check if address and quantity of registers match */
   if ( (0x0000 == startingAddress) &&
        (0x01 == quantityOfInputRegisters) )
   {
      ciaaModbus_writeInt(&buf[0], inputRegVal);
      ret = 1;
   }
   else
   {
      *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
      ret = -1;
   }

   return ret;
}

extern int8_t writeSingleRegister(
      uint16_t registerAddress,
      uint16_t registerValue,
      uint8_t * exceptionCode
      )
{
   int8_t ret = 1;

   switch (registerAddress)
   {
      /* inputs can not be written! */
      case MODBUS_ADDR_HR_CIAA_INPUTS:
         *exceptionCode = CIAAMODBUS_E_FNC_ERROR;
         break;

      /* write outputs */
      case MODBUS_ADDR_HR_CIAA_OUTPUTS:
         hr_ciaaOutputs = registerValue;
         break;

      /* wrong address */
      default:
         *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
         ret = -1;
         break;
   }

   return ret;
}

extern int8_t readHoldingRegisters(
      uint16_t startingAddress,
      uint16_t quantityOfHoldingRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      )
{
   int8_t ret = 0;
   uint16_t countReg;

   do
   {
      switch (startingAddress)
      {
         /* read inputs of CIAA */
         case MODBUS_ADDR_HR_CIAA_INPUTS:
            ciaaModbus_writeInt(buf, hr_ciaaInputs);
            countReg = 1;
            hr_ciaaInputs ++;
            break;

         /* read outputs of CIAA */
         case MODBUS_ADDR_HR_CIAA_OUTPUTS:
            ciaaModbus_writeInt(buf, hr_ciaaOutputs);
            countReg = 1;
            break;

         /* wrong address */
         default:
            *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
            countReg = -1;
            break;
      }

      if (countReg > 0)
      {
         /* update pointer to buffer */
         buf += (countReg*2);
         /* next address */
         startingAddress += countReg;
         /* increment count of registers */
         ret += countReg;
      }
      else
      {
         ret = -1;
      }
   /* repeat until:
    * - read total registers or
    * - error occurs
    */
   }while ((ret > 0) && (ret < quantityOfHoldingRegisters));

   return ret;
}

extern int8_t writeMultipleRegisters(
      uint16_t startingAddress,
      uint16_t quantityOfRegisters,
      uint8_t * exceptionCode,
      uint8_t * buf
      )
{
   int8_t ret = 0;
   uint16_t countReg;

   do
   {
      switch (startingAddress)
      {
         /* inputs can not be written! */
         case MODBUS_ADDR_HR_CIAA_INPUTS:
            *exceptionCode = CIAAMODBUS_E_FNC_ERROR;
            countReg = -1;
            break;

         /* write outputs */
         case MODBUS_ADDR_HR_CIAA_OUTPUTS:
            hr_ciaaOutputs = ciaaModbus_readInt(buf);
            countReg = 1;
            break;

         /* wrong address */
         default:
            *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
            countReg = -1;
            break;
      }

      if (countReg > 0)
      {
         /* update pointer to buffer */
         buf += (countReg*2);
         /* next address */
         startingAddress += countReg;
         /* increment count of registers */
         ret += countReg;
      }
      else
      {
         ret = -1;
      }

   /* repeat until:
    * - read total registers or
    * - error occurs
    */
   }while ((ret > 0) && (ret < quantityOfRegisters));

   /* if success return 1 */
   if (ret > 1)
      ret = 1;

   return ret;
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


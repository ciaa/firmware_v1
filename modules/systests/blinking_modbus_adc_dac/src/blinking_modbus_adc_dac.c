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
#include "ciaaPOSIX_stdlib.h"
#include "ciaaModbusSlave.h"
#include "ciaak.h"            /* <= ciaa kernel header */
#include "blinking_modbus_adc_dac.h"

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

/** \brief ramp signal value
 */
static uint16_t ramp;

/** \brief DAC output value
 */
static uint16_t hr_ciaaDac;

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

/** \brief File descriptor for ADC
 *
 * Device path /dev/serial/aio/in/0
 */
static int32_t fd_adc;

/** \brief File descriptor for DAC
 *
 * Device path /dev/serial/aio/out/0
 */
static int32_t fd_dac;


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
   /* init the ciaa kernel */
   ciaak_start();

   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   /* open CIAA ADC */
   fd_adc = ciaaPOSIX_open("/dev/serial/aio/in/0", O_RDONLY);
   ciaaPOSIX_ioctl(fd_adc, ciaaPOSIX_IOCTL_SET_SAMPLE_RATE, 10000);
   ciaaPOSIX_ioctl(fd_adc, ciaaPOSIX_IOCTL_SET_CHANNEL, ciaaCHANNEL_3);

   /* open CIAA DAC */
   fd_dac = ciaaPOSIX_open("/dev/serial/aio/out/0", O_WRONLY);

   /* Activates the ModbusSlave task */
   ActivateTask(ModbusSlave);

   /* init DAC */
   ramp = 0;

   /* end InitTask */
   TerminateTask();
}

TASK(Analogic)
{
   uint8_t uint8Data;

   /* Test signal. */
   ramp += 16;
   if (ramp > 1023) ramp = 0;
   hr_ciaaDac = ramp;

   /* Read ADC. */
   if (ciaaPOSIX_read(fd_adc, &hr_ciaaDac, sizeof(hr_ciaaDac)) < 1)
   {
      hr_ciaaDac = ramp; /* If error, return test signal. */
   }
   else
   {
      hr_ciaaDac >>= 0; /* Signal gain. */
   }

   /* Write DAC */
   ciaaPOSIX_write(fd_dac, &hr_ciaaDac, sizeof(hr_ciaaDac));

   /* end of Blinking */
   TerminateTask();
}

/** \brief Blinking Task
 *
 * This task is activated by the Alarm BlinkingAlarm.
 * This task blinks the output 4, copies hr_ciaaOutputs
 * to the output bits 0..8.
 * Read the input bits 0..8 and copies the value to hr_ciaaInputs
 */
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
   hr_ciaaInputs = uint8Data;

   /* end of Blinking */
   TerminateTask();
}

/** \brief Modbus Slave Task
 *
 * This init serial port used in Modbus Slave and call
 * ciaaModbus_slaveMainTask. This function is a
 * forever loop that receive Modbus ASCII request, process
 * and send Modbus ASCII response.
 *
 */
TASK(ModbusSlave)
{
   /* initialize modbus slave */
   ciaaModbus_init("/dev/serial/uart/0");

   /* start modbus main task */
   ciaaModbus_slaveMainTask();

   /* ciaaModbus_slaveMainTask shall never returns. Unless
    * ciaaModbus_exit != 0 */
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
      /* write register in to buffer */
      ciaaModbus_writeInt(&buf[0], inputRegVal);

      /* return quantity of registers writes */
      ret = 1;
   }
   else
   {
      /* set exception code address wrong */
      *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;

      /* return -1 to indicate that an exception occurred */
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

   /* select register address to be write */
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
   /* used to indicate total of registers reads */
   int8_t ret = 0;
   /* used to indicate quantity of registers processed */
   uint16_t quantityRegProcessed;

   /* loop to read all registers indicated */
   do
   {
      /* select register address to be read */
      switch (startingAddress)
      {
         /* read inputs of CIAA */
         case MODBUS_ADDR_HR_CIAA_INPUTS:
            ciaaModbus_writeInt(buf, hr_ciaaInputs);
            quantityRegProcessed = 1;
            break;

         /* read outputs of CIAA */
         case MODBUS_ADDR_HR_CIAA_OUTPUTS:
            ciaaModbus_writeInt(buf, hr_ciaaOutputs);
            quantityRegProcessed = 1;
            break;

         /* wrong address */
         default:
            *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
            quantityRegProcessed = -1;
            break;
      }

      /* if quantityRegProcessed > 0, successful operation */
      if (quantityRegProcessed > 0)
      {
         /* update buffer pointer to next register */
         buf += (quantityRegProcessed*2);

         /* next address to be read */
         startingAddress += quantityRegProcessed;

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
   /* used to indicate total of registers writes */
     int8_t ret = 0;
   /* used to indicate quantity of registers processed */
   uint16_t quantityRegProcessed;

   /* loop to write all registers indicated */
   do
   {
      /* select register address to be write */
      switch (startingAddress)
      {
         /* inputs can not be written! */
         case MODBUS_ADDR_HR_CIAA_INPUTS:
            *exceptionCode = CIAAMODBUS_E_FNC_ERROR;
            quantityRegProcessed = -1;
            break;

         /* write outputs */
         case MODBUS_ADDR_HR_CIAA_OUTPUTS:
            hr_ciaaOutputs = ciaaModbus_readInt(buf);
            quantityRegProcessed = 1;
            break;

         /* wrong address */
         default:
            *exceptionCode = CIAAMODBUS_E_WRONG_STR_ADDR;
            quantityRegProcessed = -1;
            break;
      }

      /* if quantityRegProcessed > 0, successful operation */
      if (quantityRegProcessed > 0)
      {
         /* update buffer pointer to next register */
         buf += (quantityRegProcessed*2);

         /* next address to be write */
         startingAddress += quantityRegProcessed;

         /* increment count of registers */
         ret += quantityRegProcessed;
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


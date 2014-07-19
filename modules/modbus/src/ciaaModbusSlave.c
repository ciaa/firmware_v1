/* Copyright 2014, Mariano Cerdeiro
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

/** \brief This file implements the Modbus Slave main functionality
 **
 ** This file implements the main functionality of the Modbus
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Modbus CIAA Modbus
 ** @{ */

/*
 * Initials     Name
 * MaCe         Mariano Cerdeiro
 * GMuro        Gustavo Muro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140623 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbusSlave.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaModbus_ascii.h"

/*==================[macros and definitions]=================================*/
#define CIAAMODBUS_BUFFER_SIZE          CIAAMODBUS_ASCII_MAXLENGHT

/** \brief Read Integer from modbus
 **
 ** As described in modbus specification, the modbus uses a bigendian format to
 ** transmit integers. This function shall be used to access integers.
 **
 ** \param[in] add address of the first byte of the integer to be read.
 **
 **/
#define CIAAMODBUS_READ_INT(add) \
   ( (((uint8_t*)add)[0] << 8) | (((uint8_t*)add)[1]))

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief Device descriptor */
static int32_t ciaaModbus_device;

/** \brief Modbus slave buffer */
static uint8_t ciaaModbus_slaveBuf[CIAAMODBUS_BUFFER_SIZE];

/** \brief Flag used to indicate that the modbus mainfunction shall exit
 **/
static int8_t ciaaModbus_exit;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaModbus_init(char * dev)
{
   ciaaModbus_device = ciaaPOSIX_open(dev, O_RDWR);

   /* falg used to exit modbus function */
   ciaaModbus_exit = 0;

}

extern void ciaaModbus_deinit(void)
{
   if (0 == ciaaModbus_exit)
   {
      ciaaModbus_exit = 1;
   }
}

extern void ciaaModbus_slaveMainTask(void)
{
   int32_t read;

   do
   {
      do
      {
         /* read modbus request */
         read = ciaaModbus_ascii_read(ciaaModbus_device, ciaaModbus_slaveBuf);

         /* repeat while
          * id does not match */
      } while (CIAAMODBUS_SLAVE_ID != ciaaModbus_slaveBuf[0]);

      /* process command, do not pass the addres of the slave id */
      read = ciaaModbus_process(&ciaaModbus_slaveBuf[1], read-1);

      /* no check is done, the sw shall always provide an answer */
      /* write modbus answer */
      ciaaModbus_ascii_write(ciaaModbus_device, ciaaModbus_slaveBuf, read);

   } while(0 == ciaaModbus_exit);

   ciaaPOSIX_close(ciaaModbus_device);
} /* end ciaaModbus_slaveMainFunction */

extern int32_t ciaaModbus_process(uint8_t * buf, int32_t len)
{
   uint8_t function = buf[0];
   uint32_t ret;

   switch(function)
   {
#if (CIAAMODBUS_READ_INPUT_REGISTERS == CIAAMODBUS_EN)
      case CIAAMODBUS_FCN_READINPUTREGISTERS:
         ret = ciaaModbus_readInputRegisters(buf, len);
         break;
#endif /* #if (CIAAMODBUS_READ_INPUT_REGISTERS == CIAAMODBUS_EN) */

#if (CIAAMODBUS_WRITE_SINGLE_REGISTER == CIAAMODBUS_EN)
      case CIAAMODBUS_FCN_WRITESINGLEREGISTER:
         ret = ciaaModbus_writeSingleRegister(buf, len);
         break;
#endif /* #if (CIAAMODBUS_WRITE_SINGLE_REGISTER == CIAAMODBUS_EN) */

      default:
         /* set error code bit */
         buf[0] |= 0x80;

         /* set error code */
         buf[1] = CIAAMODBUS_E_FNC_NOT_SUPPORTED;

         ret = 2;
         break;
   }

   return ret;

}

#if (CIAAMODBUS_READ_INPUT_REGISTERS == CIAAMODBUS_EN)
int32_t ciaaModbus_readInputRegisters(uint8_t * buf, int32_t len)
{
   uint16_t quantityOfRegisters;
   uint16_t address;
   int32_t ret = -1;
   int32_t loopi;

   quantityOfRegisters = CIAAMODBUS_READ_INT(&buf[3]);

   /* check that quantity of registers is in range */
   if ( (0x007D < quantityOfRegisters) ||
      /* check if lenght is valid, this is not part of modbus see:
       * https://github.com/ciaa/Firmware/issues/68 */
        (CIAAMODBUS_MSG_READ_INPUT_REGISTERS_LENGTH != len) )
   {
      /* set error flag */
      buf[0] |= 0x80;

      /* report invalid quantity of registers */
      buf[1] = CIAAMODBUS_E_WRONG_REG_QTY;

      /* return 2 bytes */
      ret = 2;
   } else
   {
      /* get address */
      address = CIAAMODBUS_READ_INT(&buf[1]);

      /* search for user callback */
      loopi = 0;
      while (NULL != ciaaModbus_cmdLst0x04[loopi].fct)
      {
         /* check if address in range */
         if ( (address >= ciaaModbus_cmdLst0x04[loopi].range.minAdd) &&
              (address <= ciaaModbus_cmdLst0x04[loopi].range.maxAdd) )
         {
            ret = ciaaModbus_cmdLst0x04[loopi].fct(address,
                  quantityOfRegisters,
                  &buf[1],
                  &buf[2]);

            /* verify if read successful  */
            if (0 < ret)
            {
               /* complete quantity of bytes */
               buf[1] = ret * 2;
               /* return length buffer in bytes */
               ret = ret * 2 + 2;
            }
            else
            {
               /* return length buffer 2 bytes (error) response*/
               ret = 2;
            }
         }

         /* increment pointer */
         loopi++;
      }
   }

   return ret;
} /* end ciaaModbus_readInputRegisters */
#endif /* #if (CIAAMODBUS_READ_INPUT_REGISTERS == CIAAMODBUS_EN) */

#if (CIAAMODBUS_WRITE_SINGLE_REGISTER == CIAAMODBUS_EN)
int32_t ciaaModbus_writeSingleRegister(uint8_t * buf, int32_t len)
{
   int32_t ret = 0;

   return ret;
} /* end ciaaModbus_writeSingleRegister */
#endif /* #if (CIAAMODBUS_WRITE_SINGLE_REGISTER == CIAAMODBUS_EN) */


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


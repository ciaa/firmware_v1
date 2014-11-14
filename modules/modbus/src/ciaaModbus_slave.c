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

#include "ciaaPOSIX_stdlib.h"
#include "ciaaPOSIX_stdbool.h"
#include "ciaaModbus_slave.h"
#include "ciaaModbus_protocol.h"
#include "ciaaModbus_config.h"
#include "os.h"

/*==================[macros and definitions]=================================*/


typedef struct
{
   const ciaaModbus_slaveCmd_type *cmd;
   uint8_t *buf;
   uint32_t size;
   uint8_t id;
   bool inUse;
}ciaaModbus_slaveObj_type;


/*==================[internal data declaration]==============================*/

static ciaaModbus_slaveObj_type ciaaModbus_slaveObj[CIAA_MODBUS_TOTAL_SLAVES];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaModbus_slaveInit(void)
{
   int32_t loopi;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_SLAVES ; loopi++)
   {
      ciaaModbus_slaveObj[loopi].cmd = NULL;
      ciaaModbus_slaveObj[loopi].buf = NULL;
      ciaaModbus_slaveObj[loopi].size = 0;
      ciaaModbus_slaveObj[loopi].id = 0;
      ciaaModbus_slaveObj[loopi].inUse = false;
   }
}

extern int32_t ciaaModbus_slaveOpen(
      const ciaaModbus_slaveCmd_type *cmd,
      uint8_t id)
{
   int32_t hModbusSlave;

   /* check valid parameters */
   if ( (id < CIAA_MODBUS_SLAVE_MIN_ID_VALUE) ||
        (id > CIAA_MODBUS_SLAVE_MAX_ID_VALUE) ||
        (cmd == NULL) )
   {
      /* if invalid parameters, initialize handler with invalid value */
      hModbusSlave = -1;
   }
   else
   {
      /* if valid parameters, initialize handler with valid value */
      hModbusSlave = 0;

      /* enter critical section */
      GetResource(MODBUSR);

      /* search a Transport Object not in use */
      while ( (hModbusSlave < CIAA_MODBUS_TOTAL_SLAVES) &&
              (ciaaModbus_slaveObj[hModbusSlave].inUse == true) )
      {
         hModbusSlave++;
      }

      /* if object available, use it */
      if (hModbusSlave < CIAA_MODBUS_TOTAL_SLAVES)
      {
         /* set object in use */
         ciaaModbus_slaveObj[hModbusSlave].inUse = true;

         /* set cmd call backs struct */
         ciaaModbus_slaveObj[hModbusSlave].cmd = cmd;

         /* set id of slave */
         ciaaModbus_slaveObj[hModbusSlave].id = id;
      }
      else
      {
         /* if no object available, return invalid handler */
         hModbusSlave = -1;
      }

      /* exit critical section */
      ReleaseResource(MODBUSR);
   }

   return hModbusSlave;
}

extern void ciaaModbus_slaveTask(int32_t handler)
{
   ciaaModbus_slaveCmd_type const *cmd = ciaaModbus_slaveObj[handler].cmd;
   uint32_t ret = 0;
   uint8_t *buf = ciaaModbus_slaveObj[handler].buf;
   uint16_t quantity;
   uint16_t address;
   uint16_t byteCount;
   uint8_t exceptioncode = 0;
   uint8_t function = ciaaModbus_slaveObj[handler].buf[0];

   if (ciaaModbus_slaveObj[handler].size)
   {
      switch(function)
      {
         case CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS:
            if (cmd->cmd0x03ReadHoldingReg == NULL)
            {
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               quantity = ciaaModbus_readInt(&buf[3]);

               if ( (0x007D < quantity) || (1 > quantity) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  address = ciaaModbus_readInt(&buf[1]);
                  ret = cmd->cmd0x03ReadHoldingReg(address, quantity, &exceptioncode, &buf[2]);
                  /* report byte count */
                  buf[1] = ret * 2;
                  ret = 2 + buf[1];
               }
            }
            break;

         case CIAA_MODBUS_FCN_WRITE_MULTIPLE_REGISTERS:
            if (cmd->cmd0x10WriteMultipleReg == NULL)
            {
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               address = ciaaModbus_readInt(&buf[1]);
               quantity = ciaaModbus_readInt(&buf[3]);
               byteCount = quantity * 2;

               if ( (0x007B < quantity) || (1 > quantity)
                  || (byteCount != buf[5]) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  cmd->cmd0x10WriteMultipleReg(address, quantity, byteCount, &exceptioncode, &buf[6]);
                  ret = 5;
               }
            }
            break;

         default:
            exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            break;
      }
   }

   if (exceptioncode != 0)
   {
      /* set error flag */
      buf[0] |= 0x80;
      /* report exception code */
      buf[1] = exceptioncode;
      /* return 2 bytes */
      ret = 2;
   }

   ciaaModbus_slaveObj[handler].size = ret;
}


extern void ciaaModbus_slaveRecvMsg(
      int32_t handler,
      uint8_t *id,
      uint8_t *pdu,
      uint32_t *size)
{
   uint32_t loopi;

   *id = ciaaModbus_slaveObj[handler].id;

   for (loopi = 0 ; loopi < ciaaModbus_slaveObj[handler].size ; loopi++)
   {
      pdu[loopi] = ciaaModbus_slaveObj[handler].buf[loopi];
   }

   *size = ciaaModbus_slaveObj[handler].size;
}

extern void ciaaModbus_slaveSendMsg(
      int32_t handler,
      uint8_t id,
      uint8_t *pdu,
      uint32_t size)
{
   ciaaModbus_slaveObj[handler].buf = pdu;
   ciaaModbus_slaveObj[handler].size = size;
}

extern uint8_t ciaaModbus_slaveGetId(int32_t handler)
{
   return ciaaModbus_slaveObj[handler].id;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


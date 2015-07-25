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
 * 20150604 v0.8.1 GMuro   replace numbers for macros
 * 20141108 v0.0.2 GMuro   New Modbus implementation
 * 20140623 v0.0.1 MaCe    first functional version
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
   uint16_t quantityW;
   uint16_t address;
   uint16_t addressW;
   uint16_t byteCount;
   uint16_t value;
   uint8_t exceptioncode = 0;
   uint8_t function = ciaaModbus_slaveObj[handler].buf[0];

   if (ciaaModbus_slaveObj[handler].size)
   {
      switch(function)
      {
         case CIAA_MODBUS_FCN_READ_COILS:
            /* verify if function is supported by application */
            if (cmd->cmd0x01ReadCoils == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain quantity of coils from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* check correct range and pdu length */
               if ( (0x07D0 < quantity) || (1 > quantity) ||
                    (5 != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* obtain address of registers from buffer */
                  address = ciaaModbus_readInt(&buf[1]);

                  /* perform application function */
                  ret = cmd->cmd0x01ReadCoils(address, quantity, &exceptioncode, &buf[2]);

                  /* report byte count */
                  buf[1] = ret / 8;

                  if (ret % 8)
                  {
                     buf[1]++;
                  }

                  /* set length of message */
                  ret = 2 + buf[1];
               }
            }
            break;

         case CIAA_MODBUS_FCN_READ_DISCRETE_INPUTS:
            /* verify if function is supported by application */
            if (cmd->cmd0x02ReadDiscrteInputs == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain quantity of discrete inputs from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* check correct range and pdu length */
               if ( (0x07D0 < quantity) || (1 > quantity) ||
                    (5 != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* obtain address of registers from buffer */
                  address = ciaaModbus_readInt(&buf[1]);

                  /* perform application function */
                  ret = cmd->cmd0x02ReadDiscrteInputs(address, quantity, &exceptioncode, &buf[2]);

                  /* report byte count */
                  buf[1] = ret / 8;

                  if (ret % 8)
                  {
                     buf[1]++;
                  }

                  /* set length of message */
                  ret = 2 + buf[1];
               }
            }
            break;

         case CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS:
            /* verify if function is supported by application */
            if (cmd->cmd0x03ReadHoldingReg == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain quantity of registers from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* check correct range and pdu length */
               if ( (0x007D < quantity) || (1 > quantity) ||
                    (5 != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* obtain address of registers from buffer */
                  address = ciaaModbus_readInt(&buf[1]);

                  /* perform application function */
                  ret = cmd->cmd0x03ReadHoldingReg(address, quantity, &exceptioncode, &buf[2]);

                  /* report byte count */
                  buf[1] = ret * 2;

                  /* set length of message */
                  ret = 2 + buf[1];
               }
            }
            break;

         case CIAA_MODBUS_FCN_READ_INPUT_REGISTERS:
            /* verify if function is supported by application */
            if (cmd->cmd0x04ReadInputReg == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain quantity of registers from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* check correct range and pdu length */
               if ( (0x007D < quantity) || (1 > quantity) ||
                    (5 != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* obtain address of registers from buffer */
                  address = ciaaModbus_readInt(&buf[1]);

                  /* perform application function */
                  ret = cmd->cmd0x04ReadInputReg(address, quantity, &exceptioncode, &buf[2]);

                  /* report byte count */
                  buf[1] = ret * 2;

                  /* set length of message */
                  ret = 2 + buf[1];
               }
            }
            break;

         case CIAA_MODBUS_FCN_WRITE_SINGLE_COIL:
            /* verify if function is supported by application */
            if (cmd->cmd0x05WriteSingleCoil == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain output value from buffer */
               value = ciaaModbus_readInt(&buf[3]);

               /* check correct value and pdu length */
               if ( ((0x0000 != value) && (0xFF00 != value)) ||
                     (5 != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* obtain address of output from buffer */
                  address = ciaaModbus_readInt(&buf[1]);

                  /* perform application function */
                  cmd->cmd0x05WriteSingleCoil(address, &exceptioncode, &buf[3]);

                  /* set length of message */
                  ret = 5;
               }
            }
            break;

         case CIAA_MODBUS_FCN_WRITE_SINGLE_REGISTER:
            /* verify if function is supported by application */
            if (cmd->cmd0x06WriteSingleRegister == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* check correct pdu length */
               if (5 != ciaaModbus_slaveObj[handler].size)
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* obtain address of register from buffer */
                  address = ciaaModbus_readInt(&buf[1]);

                  /* obtain register value from buffer */
                  value = ciaaModbus_readInt(&buf[3]);

                  /* perform application function */
                  cmd->cmd0x06WriteSingleRegister(address, &exceptioncode, &buf[3]);

                  /* set length of message */
                  ret = 5;
               }
            }
            break;

         case CIAA_MODBUS_FCN_WRITE_MULTIPLE_COILS:
            /* verify if function is supported by application */
            if (cmd->cmd0x0FWriteMultipleCoils == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain address of registers from buffer */
               address = ciaaModbus_readInt(&buf[1]);

               /* obtain quantity of coils from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* calculate byte count to compare with received */
               byteCount = quantity / 8;
               if (quantity % 8)
               {
                  byteCount++;
               }

               /* check correct range and pdu length */
               if ( (0x07B0 < quantity) || (1 > quantity) ||
                    (byteCount != buf[5]) ||
                    ((6+byteCount) != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of coils */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* perform application function */
                  cmd->cmd0x0FWriteMultipleCoils(address, quantity, byteCount, &exceptioncode, &buf[6]);

                  /* set length of message */
                  ret = 5;
               }
            }
            break;

         case CIAA_MODBUS_FCN_WRITE_MULTIPLE_REGISTERS:
            /* verify if function is supported by application */
            if (cmd->cmd0x10WriteMultipleReg == NULL)
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain address of registers from buffer */
               address = ciaaModbus_readInt(&buf[1]);

               /* obtain quantity of registers from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* calculate byte count to compare with received */
               byteCount = quantity * 2;

               /* check correct range and pdu length */
               if ( (0x007B < quantity) || (1 > quantity) ||
                    (byteCount != buf[5]) ||
                    ((6+byteCount) != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* perform application function */
                  cmd->cmd0x10WriteMultipleReg(address, quantity, byteCount, &exceptioncode, &buf[6]);

                  /* set length of message */
                  ret = 5;
               }
            }
            break;

         case CIAA_MODBUS_FCN_READ_WRITE_MULTIPLE_REGISTERS:
            /* verify if function is supported by application */
            if ( (cmd->cmd0x03ReadHoldingReg == NULL) ||
                 (cmd->cmd0x10WriteMultipleReg == NULL) )
            {
               /* function not supported */
               exceptioncode = CIAA_MODBUS_E_FNC_NOT_SUPPORTED;
            }
            else
            {
               /* obtain read address of registers from buffer */
               address = ciaaModbus_readInt(&buf[1]);

               /* obtain read quantity of registers from buffer */
               quantity = ciaaModbus_readInt(&buf[3]);

               /* obtain write address of registers from buffer */
               addressW = ciaaModbus_readInt(&buf[5]);

               /* obtain write quantity of registers from buffer */
               quantityW = ciaaModbus_readInt(&buf[7]);

               /* calculate byte count to compare with received */
               byteCount = quantityW * 2;

               /* check correct range and pdu length */
               if ( (0x007D < quantity)  || (1 > quantity)  ||
                    (0x0079 < quantityW) || (1 > quantityW) ||
                    (byteCount != buf[9]) ||
                    ((10+byteCount) != ciaaModbus_slaveObj[handler].size) )
               {
                  /* report invalid quantity of registers */
                  exceptioncode = CIAA_MODBUS_E_WRONG_REG_QTY;
               }
               else
               {
                  /* perform application function */
                  cmd->cmd0x10WriteMultipleReg(addressW, quantityW, byteCount, &exceptioncode, &buf[10]);

                  /* checks if not error */
                  if (0 == exceptioncode)
                  {
                     /* perform application function */
                     ret = cmd->cmd0x03ReadHoldingReg(address, quantity, &exceptioncode, &buf[2]);

                     /* report byte count */
                     buf[1] = ret * 2;

                     /* set length of message */
                     ret = 2 + buf[1];
                  }
               }
            }
            break;

         default:
            /* function not supported with modbus slave module */
            /* TODO: provide callback to application callback */
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


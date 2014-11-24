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
 * GMuro        Gustavo Muro
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20141108 v0.0.1 GMuro   initial version
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdio.h"
#include "ciaaModbus_master.h"
#include "ciaaModbus_transport.h"
#include "ciaaPOSIX_stdbool.h"
#include "ciaaModbus_config.h"
#include "os.h"

#if CIAA_MODBUS_TOTAL_MASTERS >= 0

/*==================[macros and definitions]=================================*/

typedef struct
{
   modbusMaster_cbEndOfComm cbEndComm;
   TaskType taskID;
   int16_t *pData;
   uint16_t startAddressR;
   uint16_t startAddressW;
   uint16_t quantityR;
   uint16_t quantityW;
   uint8_t cmd;
   uint8_t slaveId;
   uint8_t exceptioncode;
   bool inUse;
}ciaaModbus_masterObj_type;


/*==================[internal data declaration]==============================*/
static ciaaModbus_masterObj_type ciaaModbus_masterObj[CIAA_MODBUS_TOTAL_MASTERS];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void ciaaModbus_masterInit(void)
{
   int32_t loopi;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_MASTERS ; loopi++)
   {
      ciaaModbus_masterObj[loopi].cmd = 0;
      ciaaModbus_masterObj[loopi].slaveId = 0;
      ciaaModbus_masterObj[loopi].inUse = false;
   }
}

extern int32_t ciaaModbus_masterOpen(void)
{
   int32_t hModbusMaster = 0;

   /* enter critical section */
   GetResource(MODBUSR);

   /* search a Master Object not in use */
   while ( (hModbusMaster < CIAA_MODBUS_TOTAL_MASTERS) &&
           (ciaaModbus_masterObj[hModbusMaster].inUse == true) )
   {
      hModbusMaster++;
   }

   /* if object available, use it */
   if (hModbusMaster < CIAA_MODBUS_TOTAL_MASTERS)
   {
      /* set object in use */
      ciaaModbus_masterObj[hModbusMaster].inUse = true;
      ciaaModbus_masterObj[hModbusMaster].cmd = 0x00;
   }
   else
   {
      /* if no object available, return invalid handler */
      hModbusMaster = -1;
   }

   /* exit critical section */
   ReleaseResource(MODBUSR);

   return hModbusMaster;
}

extern int8_t ciaaModbus_masterCmd0x03ReadHoldingReg(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      int16_t *hrValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfComm cbEndComm)
{
   int8_t ret;

   if (ciaaModbus_masterObj[hModbusMaster].cmd == 0)
   {
      ciaaModbus_masterObj[hModbusMaster].startAddressR = startAddress;
      ciaaModbus_masterObj[hModbusMaster].quantityR = quantity;
      ciaaModbus_masterObj[hModbusMaster].pData = hrValue;
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;
      ciaaModbus_masterObj[hModbusMaster].cmd = CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS;

      if (NULL == cbEndComm)
      {
         ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;
         GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);
         WaitEvent(MODBUSME);
         ClearEvent(MODBUSME);
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         ret = 0;
      }
   }
   else
   {
      ret = -1;
   }

   return ret;
}

#endif

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


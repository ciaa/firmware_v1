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
#include "ciaaModbus_config.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

#define CIAA_MODBUS_SLAVE_MIN_ID_VALUE    1
#define CIAA_MODBUS_SLAVE_MAX_ID_VALUE    247

typedef struct
{
   bool inUse;
   const ciaaModbus_slaveCmd_type *cmd;
   uint8_t id;
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
      ciaaModbus_slaveObj[loopi].inUse = false;
      ciaaModbus_slaveObj[loopi].cmd = NULL;
      ciaaModbus_slaveObj[loopi].id = 0;
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

extern void ciaaModbus_slaveTask(int32_t hModbusSlave)
{
/*  int32_t ret;
   uint32_t len;

   ret = ciaaModbus_transportRecv(
         handlerModTra,
         buffer
         &len);

   if (ret == CIAAMODBUS_TRASNPORT_RECV_COMPLETE)
   {
      if (buffer[0] == CIAA_BLINKING_MODBUS_ID)
      {
         len = ciaaModbus_slaveProcess(
               &buffer[1],
               &callbacksStruct);

         ciaaModbus_transportSend(
               handlerModTra,
               buffer,
               len+1);
      }
   }
   */
}


/** \brief Process modbus request
 **
 ** \param[inout] buf buffer with the modbus data
 ** \param[in] len length of the buffer
 ** \param[in] cmd pointer to struct call backs modbus function
 ** \return length of data on the buffer
 **/
extern int32_t ciaaModbus_slaveProcess(
      uint8_t * buf,
      const ciaaModbus_slaveCmd_type *cmd
      )
{
   return 0;
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


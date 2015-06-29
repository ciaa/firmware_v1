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

/** \brief This file implements the Modbus main functionality
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
 * 20150604 v0.8.1 GMuro   replace numbers for macros
 * 20141108 v0.0.1 GMuro   initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbus_config.h"
#include "ciaaModbus_transport.h"
#include "ciaaModbus_ascii.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_stdbool.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

#define CIAA_MODBUS_TOTAL_TRANSPORTS   (  CIAA_MODBUS_TOTAL_TRANSPORT_ASCII + \
                                          CIAA_MODBUS_TOTAL_TRANSPORT_RTU   + \
                                          CIAA_MODBUS_TOTAL_TRANSPORT_TCP )

/** \brief Default response timeout (milliseconds) */
#define CIAA_MODBUS_TRASNPORT_DEFAULT_TIMEOUT   300

/** \brief Transport Object type */
typedef struct
{
   int32_t hModbusLowLayer;            /** <- Handler of low layer transport */
   uint32_t respTimeout;               /** <- response timeout */
   ciaaModbus_transportModeEnum mode;  /** <- Transport Mode */
   bool inUse;                         /** <- Object in use */
}ciaaModbus_transportObjType;


/*==================[internal data declaration]==============================*/
/** \brief Array of Transport Object */
static ciaaModbus_transportObjType ciaaModbus_transportObj[CIAA_MODBUS_TOTAL_TRANSPORTS];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[external functions definition]==========================*/

extern void ciaaModbus_transportInit(void)
{
   int32_t loopi;

   /* initialize all Transport Objects */
   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_TRANSPORTS ; loopi++)
   {
      /* not in use */
      ciaaModbus_transportObj[loopi].inUse = false;

      /* invalid handler low layer transport */
      ciaaModbus_transportObj[loopi].hModbusLowLayer = -1;

      /* default mode: MODBUS ASCII SLAVE */
      ciaaModbus_transportObj[loopi].mode = CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE;
   }
}

extern int32_t ciaaModbus_transportOpen(
      int32_t fildes,
      ciaaModbus_transportModeEnum mode)
{
   int32_t hModbusTransport;
   int32_t hModbusLowLayer;

   /* check parameter mode */
   if ( (mode == CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER) ||
        (mode == CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE)  ||
        (mode == CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER)   ||
        (mode == CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE)    ||
        (mode == CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER)   ||
        (mode == CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE) )
   {
      /* if valid mode, initialize handler with valid value */
      hModbusTransport = 0;

      /* enter critical section */
      GetResource(MODBUSR);

      /* search a Transport Object not in use */
      while ( (hModbusTransport < CIAA_MODBUS_TOTAL_TRANSPORTS) &&
              (ciaaModbus_transportObj[hModbusTransport].inUse == true) )
      {
         hModbusTransport++;
      }

      /* if object available, use it */
      if (hModbusTransport < CIAA_MODBUS_TOTAL_TRANSPORTS)
      {
         switch (mode)
         {
            case CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER:
            case CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE:
               /* open modbus ascii transport */
               hModbusLowLayer = ciaaModbus_asciiOpen(fildes);
               break;

            case CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER:
            case CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE:
               /* ciaaModbus_transportObj[hModbusTransport].hModbusLowLayer =
               ciaaModbus_rtuOpen() */
               hModbusLowLayer = -1;
               break;

            case CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER:
            case CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE:
               /* ciaaModbus_transportObj[hModbusTransport].hModbusLowLayer =
               ciaaModbus_tcpOpen() */
               hModbusLowLayer = -1;
               break;
         }
         /* check if a valid low layer transport */
         if (hModbusLowLayer >= 0)
         {
            /* set low layer transpor */
            ciaaModbus_transportObj[hModbusTransport].hModbusLowLayer = hModbusLowLayer;

            /* set object in use */
            ciaaModbus_transportObj[hModbusTransport].inUse = true;

            /* set low layer mode */
            ciaaModbus_transportObj[hModbusTransport].mode = mode;

            /* Set default response timeout */
            ciaaModbus_transportObj[hModbusTransport].respTimeout = CIAA_MODBUS_TRASNPORT_DEFAULT_TIMEOUT;
         }
         else
         {
            /* if invalid low layer transport, return invalid handler */
            hModbusTransport = -1;
         }
      }
      else
      {
         /* if no object available, return invalid handler */
         hModbusTransport = -1;
      }

      /* exit critical section */
      ReleaseResource(MODBUSR);
   }
   else
   {
      /* if invalid mode, initialize handler with invalid value*/
      hModbusTransport = -1;
   }

   return hModbusTransport;
}


extern void ciaaModbus_transportTask(int32_t handler)
{
   switch (ciaaModbus_transportObj[handler].mode)
   {
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE:
         ciaaModbus_asciiTask(ciaaModbus_transportObj[handler].hModbusLowLayer);
         break;

      case CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE:
         /* ciaaModbus_rtuTask(); */
         break;

      case CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE:
         /* ciaaModbus_tcpTask() */
         break;
   }
}

extern void ciaaModbus_transportRecvMsg(
      int32_t handler,
      uint8_t *id,
      uint8_t *pdu,
      uint32_t *size)
{
   switch (ciaaModbus_transportObj[handler].mode)
   {
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE:
         ciaaModbus_asciiRecvMsg(
               ciaaModbus_transportObj[handler].hModbusLowLayer,
               id,
               pdu,
               size);
         break;

      case CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE:
         /* ciaaModbus_rtuRecvMsg(); */
         break;

      case CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE:
         /* ciaaModbus_tcpRecvMsg() */
         break;
   }
}

void ciaaModbus_transportSendMsg(
      int32_t handler,
      uint8_t id,
      uint8_t *pdu,
      uint32_t size)
{
   switch (ciaaModbus_transportObj[handler].mode)
   {
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE:
         ciaaModbus_asciiSendMsg(
               ciaaModbus_transportObj[handler].hModbusLowLayer,
               id,
               pdu,
               size);
         break;

      case CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE:
         /* ciaaModbus_rtuSendMsg(); */
         break;

      case CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER:
      case CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE:
         /* ciaaModbus_tcpSendMsg() */
         break;
   }
}

extern int8_t ciaaModbus_transportGetType(int32_t handler)
{
   int8_t ret = CIAAMODBUS_TRANSPORT_TYPE_INVALID;

   if (ciaaModbus_transportObj[handler].inUse == true)
   {
      switch (ciaaModbus_transportObj[handler].mode)
      {
         case CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER:
         case CIAAMODBUS_TRANSPORT_MODE_RTU_MASTER:
         case CIAAMODBUS_TRANSPORT_MODE_TCP_MASTER:
            ret = CIAAMODBUS_TRANSPORT_TYPE_MASTER;
            break;

         case CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE:
         case CIAAMODBUS_TRANSPORT_MODE_RTU_SLAVE:
         case CIAAMODBUS_TRANSPORT_MODE_TCP_SLAVE:
            ret = CIAAMODBUS_TRANSPORT_TYPE_SLAVE;
            break;

         default:
            ret = CIAAMODBUS_TRANSPORT_TYPE_INVALID;
            break;
      }
   }

   return ret;
}

extern void ciaaModbus_transportSetRespTimeout(int32_t handler, uint32_t timeout)
{
   ciaaModbus_transportObj[handler].respTimeout = timeout;
}

extern uint32_t ciaaModbus_transportGetRespTimeout(int32_t handler)
{
   return ciaaModbus_transportObj[handler].respTimeout;
}


/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


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
 * 20140623 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/

#include "ciaaModbus_gateway.h"
#include "ciaaModbus_transport.h"
#include "ciaaModbus_config.h"
#include "ciaaPOSIX_stdbool.h"
#include "os.h"


/*==================[macros and definitions]=================================*/

/** \brief Total servers by gateway */
#define CIAA_MODBUS_GATEWAY_TOTAL_SERVERS    2

/** \brief Total clients by gateway */
#define CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS    2

/** \brief state of client in gateway */
typedef enum
{
   CIAA_MODBUS_CLIENT_STATE_IDLE = 0,
   CIAA_MODBUS_CLIENT_STATE_ROUTING,
   CIAA_MODBUS_CLIENT_STATE_SEND_MSG_TO_SERVER,
   CIAA_MODBUS_CLIENT_STATE_WAITING_SERVER_RESPONSE,
}ciaaModbus_clientStateEnum;

/** \brief CIAA Modbus task module
 **
 ** This function perform task of module:
 **
 ** \param[in] handler handler in to module to perform task
 ** \return
 **/
typedef void (*ciaaModbus_taskType)(int32_t handler);

/** \brief Receive modbus message
 **
 ** This function tells the module that want to receive a message
 **
 ** \param[out] id identification number of modbus message
 ** \param[out] pdu buffer with stored pdu
 ** \param[out] size size of pdu. If no valid message received
 **             size must be less than 5
 ** \return
 **/
typedef void (*ciaaModbus_recvMsgType)(uint8_t *id, uint8_t *pdu, uint32_t *size);

/** \brief Send modbus message
 **
 ** This function tells the module that want to send a message
 **
 ** \param[in] id identification number of modbus message
 ** \param[in] pdu buffer with stored pdu
 ** \param[in] size size of pdu
 ** \return
 **/
typedef void (*ciaaModbus_sendMsgType)(uint8_t id, uint8_t *pdu, uint32_t size);

/** \brief Client Modbus type */
typedef struct
{
   bool inUse;                         /** <- Object in use                  */
   int32_t handler;                    /** <- handler of module (slave, master,
                                              transport)                     */
   ciaaModbus_clientStateEnum state;   /** <- State of client */
   uint8_t buffer[256];                /** <- buffer to store modbus message
                                              received TODO: use macro       */
   uint8_t id;                         /** <- id of message received         */
   uint32_t size;                      /** <- size of message received       */
   int32_t indexServer;                /** <- index server to send message   */
   ciaaModbus_taskType task;           /** <- function task of module (master,
                                              transport)                     */
   ciaaModbus_recvMsgType recvMsg;     /** <- function recvMsg of module
                                              (master, transport)            */
   ciaaModbus_sendMsgType sendMsg;     /** <- function sendMsg of module
                                              (master, transport)            */
}ciaaModbus_gatewayClientType;

/** \brief Server Modbus type */
typedef struct
{
   bool inUse;                         /** <- Object in use                  */
   int32_t handler;                    /** <- handler of module (slave,
                                              transport)                     */
   bool busy;                          /** <- indicate slave busy */
   uint8_t id;                         /** <- id of slave, 0 if it transport */
   ciaaModbus_taskType task;           /** <- function task of module (master,
                                              transport)                     */
   ciaaModbus_recvMsgType recvMsg;     /** <- function recvMsg of module
                                              (master, transport)            */
   ciaaModbus_sendMsgType sendMsg;     /** <- function sendMsg of module
                                              (master, transport)            */
}ciaaModbus_gatewayServerType;


/** \brief Gateway Object type */
typedef struct
{
   bool inUse;
   ciaaModbus_gatewayClientType client[CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS];
   ciaaModbus_gatewayServerType server[CIAA_MODBUS_GATEWAY_TOTAL_SERVERS];
}ciaaModbus_gatewayObjType;


/*==================[internal data declaration]==============================*/

/** \brief Array of Gateway Object */
static ciaaModbus_gatewayObjType ciaaModbus_gatewayObj[CIAA_MODBUS_TOTAL_GATEWAY];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/** \brief Process task of Modbus client
 **
 **
 ** \param[inout] client pointer to client to process
 ** \param[inout] server array of servers to send modbus messages
 ** \return 1  if task pending
 **         0  if done
 **         -1 if error occurs
 **/
static int8_t ciaaModbus_gatewayClientProcess(
      ciaaModbus_gatewayClientType *client,
      ciaaModbus_gatewayServerType *servers)
{
   int32_t loopi;
   int8_t ret = 0;

   if (client->inUse)
   {
      switch (client->state)
      {
         case CIAA_MODBUS_CLIENT_STATE_IDLE:
            client->task(client->handler);
            client->recvMsg(&client->id, client->buffer, &client->size);
            if (client->size >= CIAAMODBUS_MSG_MINLENGTH)
            {
               client->state = CIAA_MODBUS_CLIENT_STATE_ROUTING;
               ret = 1;
            }
            else
            {
               ret = 0;
            }
            break;

         case CIAA_MODBUS_CLIENT_STATE_ROUTING:
            for ( loopi = 0 ;
                  (loopi < CIAA_MODBUS_GATEWAY_TOTAL_SERVERS) && (ret == 0) ;
                  loopi++ )
            {
               if ( (servers[loopi].inUse) &&
                    (servers[loopi].id == client->id) )
               {
                  client->state = CIAA_MODBUS_CLIENT_STATE_SEND_MSG_TO_SERVER;
                  client->indexServer = loopi;
                  ret = 1;
               }
            }
            if (ret == 0)
            {
               client->state = CIAA_MODBUS_CLIENT_STATE_IDLE;
            }
            break;

         case CIAA_MODBUS_CLIENT_STATE_SEND_MSG_TO_SERVER:
            if (servers[client->indexServer].busy)
            {
               ret = 0;
            }
            else
            {
               servers[client->indexServer].busy = true;
               servers[client->indexServer].sendMsg(
                     client->id,
                     client->buffer,
                     client->size);
               client->state = CIAA_MODBUS_CLIENT_STATE_WAITING_SERVER_RESPONSE;
               ret = 1;
            }
            break;

         case CIAA_MODBUS_CLIENT_STATE_WAITING_SERVER_RESPONSE:
            servers[client->indexServer].task(
                  servers[client->indexServer].handler);

            servers[client->indexServer].recvMsg(
                  &client->id,
                  client->buffer,
                  &client->size);

            if (client->size >= CIAAMODBUS_MSG_MINLENGTH)
            {
               //client->state = CIAA_MODBUS_CLIENT_STATE_ROUTING;
               ret = 1;
            }
            else
            {
               ret = 0;
            }

            break;
      }
   }
   else
   {
      ret = 0;
   }

   return ret;
}

/*==================[external functions definition]==========================*/
extern void ciaaModbus_gatewayInit(void)
{
   int32_t loopi;

   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_GATEWAY ; loopi++)
   {
      ciaaModbus_gatewayObj[loopi].inUse = false;
      /* TODO: init other fields */
   }
}

extern int32_t ciaaModbus_gatewayOpen(void)
{
   int32_t hModbusGW = 0;

   /* enter critical section */
   GetResource(MODBUSR);

   /* search a Gateway Object not in use */
   while ( (hModbusGW < CIAA_MODBUS_TOTAL_GATEWAY) &&
           (ciaaModbus_gatewayObj[hModbusGW].inUse == true) )
   {
      /* next object */
      hModbusGW++;
   }

   /* if object available, use it */
   if (hModbusGW != CIAA_MODBUS_TOTAL_GATEWAY)
   {
      /* set object in use */
      ciaaModbus_gatewayObj[hModbusGW].inUse = true;
   }
   else
   {
      /* if no object available, return invalid handler */
      hModbusGW = -1;
   }

   /* exit critical section */
   ReleaseResource(MODBUSR);

   return hModbusGW;
}

extern int8_t ciaaModbus_gatewayAddSlave(
      int32_t hModbusGW,
      int32_t hModbusSlave)
{
   return 0;
}

extern int8_t ciaaModbus_gatewayAddMaster(
      int32_t hModbusGW,
      int32_t hModbusMaster)
{
   return 0;
}

extern int8_t ciaaModbus_gatewayAddTransport(
      int32_t hModbusGW,
      int32_t hModbusMaster)
{
   return 0;
}


extern void ciaaModbus_gatewayMainTask(
      int32_t hModbusGW)
{
   uint32_t loopi;
   int32_t ret;

   for (loopi = 0 ; loopi < CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS ; loopi++)
   {
      do
      {
         ret = ciaaModbus_gatewayClientProcess(
               &ciaaModbus_gatewayObj[hModbusGW].client[loopi],
               ciaaModbus_gatewayObj[hModbusGW].server);

      }while (ret > 0);
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


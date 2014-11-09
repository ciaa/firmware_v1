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
#include "ciaaModbus_slave.h"
#include "ciaaModbus_config.h"
#include "ciaaPOSIX_stdbool.h"
#include "os.h"


/*==================[macros and definitions]=================================*/

/** \brief Total servers by gateway */
#define CIAA_MODBUS_GATEWAY_TOTAL_SERVERS    2

/** \brief Total clients by gateway */
#define CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS    2

/** \brief Limit consecutive calls to ciaaModbus_gatewayClientProcess */
#define CIAA_MODBUS_GATEWAY_LIMIT_CALLS      5

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
 ** \param[in] handler handler in to module to recv msg
 ** \param[out] id identification number of modbus message
 ** \param[out] pdu buffer with stored pdu
 ** \param[out] size size of pdu. If no valid message received
 **             size must be less than 5
 ** \return
 **/
typedef void (*ciaaModbus_recvMsgType)(int32_t handler, uint8_t *id, uint8_t *pdu, uint32_t *size);

/** \brief Send modbus message
 **
 ** This function tells the module that want to send a message
 **
 ** \param[in] handler handler in to module to send msg
 ** \param[in] id identification number of modbus message
 ** \param[in] pdu buffer with stored pdu
 ** \param[in] size size of pdu
 ** \return
 **/
typedef void (*ciaaModbus_sendMsgType)(int32_t handler, uint8_t id, uint8_t *pdu, uint32_t size);

/** \brief Client Modbus type */
typedef struct
{
   int32_t handler;                    /** <- handler of module (slave, master,
                                              transport)                     */
   uint8_t buffer[256];                /** <- buffer to store modbus message
                                              received TODO: use macro       */
   uint32_t size;                      /** <- size of message received       */
   int32_t indexServer;                /** <- index server to send message   */
   ciaaModbus_taskType task;           /** <- function task of module (master,
                                              transport)                     */
   ciaaModbus_recvMsgType recvMsg;     /** <- function recvMsg of module
                                              (master, transport)            */
   ciaaModbus_sendMsgType sendMsg;     /** <- function sendMsg of module
                                              (master, transport)            */
   ciaaModbus_clientStateEnum state;   /** <- State of client */
   uint8_t id;                         /** <- id of message received         */
   bool inUse;                         /** <- Object in use                  */
}ciaaModbus_gatewayClientType;

/** \brief Server Modbus type */
typedef struct
{
   int32_t handler;                    /** <- handler of module (slave,
                                              transport)                     */
   ciaaModbus_taskType task;           /** <- function task of module (master,
                                              transport)                     */
   ciaaModbus_recvMsgType recvMsg;     /** <- function recvMsg of module
                                              (master, transport)            */
   ciaaModbus_sendMsgType sendMsg;     /** <- function sendMsg of module
                                              (master, transport)            */
   uint8_t id;                         /** <- id of slave, 0 if it transport */
   bool inUse;                         /** <- Object in use                  */
   bool busy;                          /** <- indicate slave busy */
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

   /* check if client in use */
   if (client->inUse)
   {
      /* state client */
      switch (client->state)
      {
         /* client idle state: waiting new message */
         case CIAA_MODBUS_CLIENT_STATE_IDLE:

            /* perform client task */
            client->task(client->handler);

            /* receive message */
            client->recvMsg(
                  client->handler,
                  &client->id,
                  client->buffer,
                  &client->size);

            /* check if a valid message received */
            if (client->size >= CIAAMODBUS_REQ_PDU_MINLENGTH)
            {
               /* if a valid message received, step next state */
               client->state = CIAA_MODBUS_CLIENT_STATE_ROUTING;

               /* indicate task pending */
               ret = 1;
            }
            else
            {
               /* no task pending */
               ret = 0;
            }
            break;

         /* routing message: search a server for the message received */
         case CIAA_MODBUS_CLIENT_STATE_ROUTING:

            /* search server */
            for ( loopi = 0 ;
                  (loopi < CIAA_MODBUS_GATEWAY_TOTAL_SERVERS) && (ret == 0) ;
                  loopi++ )
            {
               /* message id == server id? */
               if ( (servers[loopi].inUse) &&
                    (servers[loopi].id == client->id) )
               {
                  /* if equal, step next state */
                  client->state = CIAA_MODBUS_CLIENT_STATE_SEND_MSG_TO_SERVER;

                  /* save index server */
                  client->indexServer = loopi;

                  /* indicate task pending */
                  ret = 1;
               }
            }
            /* if no server found, goto idle state */
            if (ret == 0)
            {
               client->state = CIAA_MODBUS_CLIENT_STATE_IDLE;
            }
            break;

         /* send message to server: if server not busy, send message */
         case CIAA_MODBUS_CLIENT_STATE_SEND_MSG_TO_SERVER:

            /* check if server not busy */
            if (servers[client->indexServer].busy)
            {
               /* if busy, return no task pending */
               ret = 0;
            }
            else
            {
               /* set busy flag */
               servers[client->indexServer].busy = true;

               /* send message to server */
               servers[client->indexServer].sendMsg(
                     servers[client->indexServer].handler,
                     client->id,
                     client->buffer,
                     client->size);

               /* step next state */
               client->state = CIAA_MODBUS_CLIENT_STATE_WAITING_SERVER_RESPONSE;

               /* indicate task pending */
               ret = 1;
            }
            break;

         /* wait response from server: perform task and receive response */
         case CIAA_MODBUS_CLIENT_STATE_WAITING_SERVER_RESPONSE:

            /* perform server task */
            servers[client->indexServer].task(
                  servers[client->indexServer].handler);

            /* receive message */
            servers[client->indexServer].recvMsg(
                  servers[client->indexServer].handler,
                  &client->id,
                  client->buffer,
                  &client->size);

            /* check if a valid message received */
            if (client->size >= CIAAMODBUS_RSP_PDU_MINLENGTH)
            {
               /* if a valid message received, send to client */
               client->sendMsg(
                     client->handler,
                     client->id,
                     client->buffer,
                     client->size);

               /* reset busy flag */
               servers[client->indexServer].busy = false;

               /* step next state: idle */
               client->state = CIAA_MODBUS_CLIENT_STATE_IDLE;

               /* indicate task pending */
               ret = 1;
            }
            else
            {
               /* TODO: check timeout response */
               ret = 0;
            }
            break;
      }
   }
   else
   {
      /* if not in use, no have task pending */
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
   uint32_t loopi;

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

      /* set servers not in use */
      for (loopi = 0 ; loopi < CIAA_MODBUS_GATEWAY_TOTAL_SERVERS ; loopi++)
      {
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].inUse = false;
      }

      /* set clients not in use */
      for (loopi = 0 ; loopi < CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS ; loopi++)
      {
         ciaaModbus_gatewayObj[hModbusGW].client[loopi].inUse = false;
      }
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
   uint32_t loopi;
   int8_t ret = -1;

   /* enter critical section */
   GetResource(MODBUSR);

   for (loopi = 0 ; (loopi < CIAA_MODBUS_GATEWAY_TOTAL_SERVERS) && (ret != 0) ; loopi++)
   {
      if (ciaaModbus_gatewayObj[hModbusGW].server[loopi].inUse == false)
      {
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].inUse = true;
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].handler = hModbusSlave;
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].busy = false;
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].id = ciaaModbus_slaveGetId(hModbusSlave);
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].recvMsg = ciaaModbus_slaveRecvMsg;
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].sendMsg = ciaaModbus_slaveSendMsg;
         ciaaModbus_gatewayObj[hModbusGW].server[loopi].task = ciaaModbus_slaveTask;
         ret = 0;
      }
   }

   /* exit critical section */
   ReleaseResource(MODBUSR);

   return ret;
}

extern int8_t ciaaModbus_gatewayAddMaster(
      int32_t hModbusGW,
      int32_t hModbusMaster)
{
   return 0;
}

extern int8_t ciaaModbus_gatewayAddTransport(
      int32_t hModbusGW,
      int32_t hModbusTransport)
{
   uint32_t loopi;
   int8_t ret = -1;
   int8_t transportType;

   /* enter critical section */
   GetResource(MODBUSR);

   transportType = ciaaModbus_transportGetType(hModbusTransport);

   if (transportType == 0)
   {
      /* transport slave -> client */
      for (loopi = 0 ; (loopi < CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS) && (ret != 0) ; loopi++)
      {
         if (ciaaModbus_gatewayObj[hModbusGW].client[loopi].inUse == false)
         {
            ciaaModbus_gatewayObj[hModbusGW].client[loopi].inUse = true;
            ciaaModbus_gatewayObj[hModbusGW].client[loopi].handler = hModbusTransport;
            ciaaModbus_gatewayObj[hModbusGW].client[loopi].recvMsg = ciaaModbus_transportRecvMsg;
            ciaaModbus_gatewayObj[hModbusGW].client[loopi].sendMsg = ciaaModbus_transportSendMsg;
            ciaaModbus_gatewayObj[hModbusGW].client[loopi].task = ciaaModbus_transportTask;
            ret = 0;
         }
      }
   }
   else if (transportType == 1)
   {
          /* transport master -> server */
      for (loopi = 0 ; (loopi < CIAA_MODBUS_GATEWAY_TOTAL_SERVERS) && (ret != 0) ; loopi++)
      {
         if (ciaaModbus_gatewayObj[hModbusGW].server[loopi].inUse == false)
         {
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].inUse = true;
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].handler = hModbusTransport;
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].busy = false;
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].id = 0;
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].recvMsg = ciaaModbus_transportRecvMsg;
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].sendMsg = ciaaModbus_transportSendMsg;
            ciaaModbus_gatewayObj[hModbusGW].server[loopi].task = ciaaModbus_transportTask;
            ret = 0;
         }
      }
   }

   /* exit critical section */
   ReleaseResource(MODBUSR);

   return ret;
}


extern void ciaaModbus_gatewayMainTask(
      int32_t hModbusGW)
{
   uint32_t loopi;
   uint32_t countCall;
   int32_t ret;

   for (loopi = 0 ; loopi < CIAA_MODBUS_GATEWAY_TOTAL_CLIENTS ; loopi++)
   {
      countCall = 0;

      do
      {
         ret = ciaaModbus_gatewayClientProcess(
               &ciaaModbus_gatewayObj[hModbusGW].client[loopi],
               ciaaModbus_gatewayObj[hModbusGW].server);

         countCall++;

      }while ( (ret > 0) && (countCall < CIAA_MODBUS_GATEWAY_LIMIT_CALLS) );
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


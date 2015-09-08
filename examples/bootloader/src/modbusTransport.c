/* Copyright 2015, Daniel Cohen
 * Copyright 2015, Esteban Volentini
 * Copyright 2015, Matias Giori
 * Copyright 2015, Franco Salinas
 * Copyright 2014, Mariano Cerdeiro
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

/** \brief This file implements the Flash Update MODBUS transport layer
 **
 ** This file implements the Flash Update MODBUS transport layer.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DC           Daniel Cohen
 * EV           Esteban Volentini
 * MG           Matias Giori
 * FS           Franco Salinas
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150908 v0.0.1  FS  first initial version
 */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaak.h"
#include "ciaaModbus.h"
#include "ciaaLibs_CircBuf.h"
#include "ciaaPOSIX_assert.h"
#include "ciaaPOSIX_stdio.h"
#include "UPDT_protocol.h"
#include "UPDT_ITransport.h"
#include "modbusTransport.h"

/*==================[macros and definitions]=================================*/
#define CIAA_BOOTLOADER_MODBUS_FIRST_ADDRESS   0x00u
#define CIAA_BOOTLOADER_MODBUS_LAST_ADDRESS    0xF6u

#define CIAA_BOOTLOADER_MODBUS_ID              0x02u
/*==================[internal data declaration]==============================*/

/** Memory block for the circular buffer */
static uint8_t input_buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE * 2];
/** Memory block for the circular buffer */
static uint8_t output_buffer[UPDT_PROTOCOL_PACKET_MAX_SIZE * 2];
/** Input circular buffer structure */
static ciaaLibs_CircBufType input_cb;
/** Output circular buffer structure */
static ciaaLibs_CircBufType output_cb;
/** Size of the input buffer. Must be a power of two */
static size_t buffer_size;

/*==================[internal functions declaration]=========================*/
static uint32_t nextPowerOfTwo(uint32_t v)
{
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return v;
}

static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       );

static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      );

/*==================[internal data definition]===============================*/
static int32_t hModbusSlave;
static int32_t hModbusAscii;
static int32_t hModbusGateway;

static const ciaaModbus_slaveCmd_type callbacksStruct =
{
   NULL,
   NULL,
   cmd0x03ReadHoldingReg,
   NULL,
   NULL,
   NULL,
   NULL,
   cmd0x10WriteMultipleReg,
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
/** \brief Sends a packet.
 **
 ** \param serial Serial structure.
 ** \param data Data to send.
 ** \param size Number of bytes to send.
 ** \return Number of bytes sent. -1 on error.
 **/
ssize_t modbusSend(UPDT_ITransportType *modbus, const void *data, size_t size)
{
   ciaaPOSIX_assert(NULL != modbus);
   ciaaPOSIX_assert(size <= buffer_size);

   /** \todo Handle the situation when there is no space to write */
   return ciaaLibs_circBufPut(&output_cb, data, size);
}
/** \brief Receives a packet.
 **
 ** \param modbus Modbus structure.
 ** \param data Buffer to receive.
 ** \param size Number of bytes to receive.
 ** \return Number of bytes received. -1 on error.
 **/
ssize_t modbusRecv(UPDT_ITransportType *modbus, void *data, size_t size)
{
   ciaaPOSIX_assert(NULL != modbus);
   ciaaPOSIX_assert(size <= buffer_size);

   /** \todo Handle the situation when there is no data to read */
   return ciaaLibs_circBufGet(&input_cb, data, size);
}

static uint16_t cmd0x03ReadHoldingReg(
       uint16_t start,
       uint16_t quantity,
       uint8_t * exceptioncode,
       uint8_t * buf
       )
{
   /* used to indicate total of registers reads */
   int8_t ret = 0;

   /* if the starting address is not the first */
   if(CIAA_BOOTLOADER_MODBUS_FIRST_ADDRESS != start)
   {
      /* then an error has occurred */
      *exceptioncode = CIAA_MODBUS_E_WRONG_STR_ADDR;
      ret = -1;
   }
   else
   {
      /* else try to write the data into the buffer */

      /* if the data sent fits the buffer */
      if (quantity * 2 < CIAA_BOOTLOADER_MODBUS_LAST_ADDRESS)
      {
         /* then write it */
         ret = ciaaLibs_circBufGet(&output_cb, buf, quantity * 2);
      }
      else
      {
         /* else an error has occurred */
         *exceptioncode = CIAA_MODBUS_E_WRONG_STR_ADDR;
         ret = -1;
      }
   }

   return ret;
}

static void cmd0x10WriteMultipleReg(
      uint16_t start,
      uint16_t quantity,
      uint8_t bytecount,
      uint8_t * exceptioncode,
      uint8_t * buf
      )
{

   /* if the starting address is not the first */
   if(CIAA_BOOTLOADER_MODBUS_FIRST_ADDRESS != start)
   {
      /* then an error has occurred */
      *exceptioncode = CIAA_MODBUS_E_WRONG_STR_ADDR;
   }
   else
   {
      /* else try to write the data into the buffer */

      /* if the data sent fits the buffer */
      if (quantity * 2 < CIAA_BOOTLOADER_MODBUS_LAST_ADDRESS)
      {
         /* then write it */
         ciaaLibs_circBufPut(&input_cb, buf, quantity * 2);
      }
      else
      {
         /* else an error has occurred */
         *exceptioncode = CIAA_MODBUS_E_WRONG_STR_ADDR;
      }
   }
}
/*==================[external functions definition]==========================*/
int32_t UPDT_modbusInit(modbusTransportType *modbus)
{
   ciaaPOSIX_assert(NULL != modbus);

   /* open serial port */
   modbus->fd = ciaaPOSIX_open("/dev/serial/uart/0", O_RDWR | O_NONBLOCK);

   /* Open Modbus Slave */
   hModbusSlave = ciaaModbus_slaveOpen(
         &callbacksStruct,
         CIAA_BOOTLOADER_MODBUS_ID);

   /* Open Transport Modbus Ascii */
   hModbusAscii = ciaaModbus_transportOpen(
         modbus->fd,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_SLAVE);

   /* Open Gateway Modbus */
   hModbusGateway = ciaaModbus_gatewayOpen();

   /* Add Modbus Slave to gateway */
   ciaaModbus_gatewayAddSlave(
         hModbusGateway,
         hModbusSlave);

   /* Add Modbus Transport to gateway */
   ciaaModbus_gatewayAddTransport(
         hModbusGateway,
         hModbusAscii);

   /* calculate the buffer size */
   buffer_size = nextPowerOfTwo(UPDT_PROTOCOL_PACKET_MAX_SIZE);

   /* initialize the circular buffers */
   ciaaPOSIX_assert(-1 == ciaaLibs_circBufInit(&input_cb, input_buffer, buffer_size));
   ciaaPOSIX_assert(-1 == ciaaLibs_circBufInit(&output_cb, output_buffer, buffer_size));

   /* set up the callbacks */
   modbus->recv = modbusRecv;
   modbus->send = modbusSend;

   SetRelAlarm(ActivateModbusTask, 100, 5);

   return 0;
}
void UPDT_modbusClear(modbusTransportType *modbus)
{
   ciaaPOSIX_assert(NULL != modbus);
   ciaaPOSIX_assert(0 <= modbus->fd);

   ciaaPOSIX_close(modbus->fd);

   modbus->send = NULL;
   modbus->recv = NULL;
   modbus->fd = -1;
}

/** \brief Modbus Task
 *
 * This task is activated by the Alarm ActivateModbusTask.
 */
TASK(ModbusSlave)
{
   ciaaModbus_gatewayMainTask(hModbusGateway);

   TerminateTask();
}
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


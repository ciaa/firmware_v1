/* Copyright 2015, Gustavo Muro
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

/** \brief CIAA master test source file
 **
 ** This is a main file for ciaa master tester
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Continuous Integration
 ** @{ */
/** \addtogroup CIAA Tester source file
 ** @{ */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaModbus.h"
#include "ciaak.h"
#include "testmaster.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static int32_t hModbusAsciiUartPC;
static int32_t hModbusGateway;
static int32_t hModbusMaster;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void modbusComm_init(void)
{
   int32_t fdUartPC;

   /* open serial port connected to ioboard */
   fdUartPC = ciaaPOSIX_open("/dev/serial/uart/0", ciaaPOSIX_O_RDWR | ciaaPOSIX_O_NONBLOCK);

   /* Open Modbus Master */
   hModbusMaster = ciaaModbus_masterOpen();

   /* Open Transport Modbus Ascii (UART PC) */
   hModbusAsciiUartPC = ciaaModbus_transportOpen(
         fdUartPC,
         CIAAMODBUS_TRANSPORT_MODE_ASCII_MASTER);

   /* Open Gateway Modbus */
   hModbusGateway = ciaaModbus_gatewayOpen();

   /* Add Modbus Master to gateway */
   ciaaModbus_gatewayAddMaster(
         hModbusGateway,
         hModbusMaster);

   /* Add Modbus Transport to gateway */
   ciaaModbus_gatewayAddTransport(
         hModbusGateway,
         hModbusAsciiUartPC);
}

extern void modbusComm_task(void)
{
   ciaaModbus_gatewayMainTask(hModbusGateway);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


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
 * 20150604 v0.8.1 GMuro   replace numbers for macros
 * 20141108 v0.0.1 GMuro   initial version
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdio.h"
#include "ciaaModbus_master.h"
#include "ciaaModbus_transport.h"
#include "ciaaPOSIX_stdbool.h"
#include "ciaaModbus_config.h"
#include "os.h"

#if CIAA_MODBUS_TOTAL_MASTERS > 0

/*==================[macros and definitions]=================================*/

/** \brief Default timeout response (millisecond) */
#define CIAA_MODBUS_MASTER_DEFAULT_TIMEOUT      300

/** \brief Default retry communication */
#define CIAA_MODBUS_MASTER_DEFAULT_RETRY_COMM   3

/** \brief Object modbus master */
typedef struct
{
   modbusMaster_cbEndOfCommType cbEndComm; /** <- pointer to call back function */
   TaskType taskID;                    /** <- Task ID if blocking mode */
   int16_t *pDataR;                    /** <- pointer to store data read */
   int16_t *pDataW;                    /** <- pointer to data to write */
   int16_t dataW;                      /** <- data to write */
   uint16_t respTimeout;               /** <- timeout configured */
   uint16_t retryComm;                 /** <- total retry if no correct respond */
   uint16_t retryCount;                /** <- retry count */
   uint16_t startAddressR;             /** <- start address to read */
   uint16_t startAddressW;             /** <- start address to write */
   uint16_t quantityR;                 /** <- quantity of read */
   uint16_t quantityW;                 /** <- quantity of write */
   uint8_t cmd;                        /** <- function to excecute on slave */
   uint8_t slaveId;                    /** <- slave ID */
   uint8_t exceptioncode;              /** <- exception code */
   bool inUse;                         /** <- Object in use */
}ciaaModbus_masterObjType;


/*==================[internal data declaration]==============================*/
/** \brief Array of Masters Object */
static ciaaModbus_masterObjType ciaaModbus_masterObj[CIAA_MODBUS_TOTAL_MASTERS];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/** \brief Process data to function 0x01 or 0x02
 **
 ** This function process PDU response as request function 0x01 or 0x02
 **
 ** \param[in] handler handler modbus master
 ** \param[in] pdu pdu received
 ** \param[out] size size of pdu.
 ** \return CIAA_MODBUS_E_NO_ERROR correct response
 **         CIAA_MODBUS_E_PDU_RECEIVED_WRONG incorrect response
 **/
static uint8_t ciaaModbus_masterProcess0x01_0x02(uint32_t hModbusMaster, uint8_t *pdu, uint16_t size)
{
   uint16_t loopi;
   uint8_t ret;
   uint8_t *pData;

   pData = (uint8_t*)ciaaModbus_masterObj[hModbusMaster].pDataR;

   /* check if valid byte count and PDU size */
   if ( (pdu[1] + 2) == size )
   {
      /* if valid, copy data received */
      for (loopi = 0 ; loopi < pdu[1] ; loopi++)
      {
         /* copy data to holding registers address */
         pData[loopi] = pdu[2+loopi];
      }

      /* return no error */
      ret = CIAA_MODBUS_E_NO_ERROR;
   }
   else
   {
      /* return wrong pdu */
      ret = CIAA_MODBUS_E_PDU_RECEIVED_WRONG;
   }

   return ret;
}

/** \brief Process data to function 0x03 or 0x04 or 0x17
 **
 ** This function process PDU response as request function 0x03 or 0x04 or 0x17
 **
 ** \param[in] handler handler modbus master
 ** \param[in] pdu pdu received
 ** \param[out] size size of pdu.
 ** \return CIAA_MODBUS_E_NO_ERROR correct response
 **         CIAA_MODBUS_E_PDU_RECEIVED_WRONG incorrect response
 **/
static uint8_t ciaaModbus_masterProcess0x03_0x04_0x17(uint32_t hModbusMaster, uint8_t *pdu, uint16_t size)
{
   uint16_t loopi;
   uint8_t ret;

   /* check if valid byte count and PDU size */
   if ( (pdu[1] == (ciaaModbus_masterObj[hModbusMaster].quantityR * 2)) &&
        (size == (2 + ciaaModbus_masterObj[hModbusMaster].quantityR * 2) ) )
   {
      /* if valid, copy data received */
      for (loopi = 0 ; loopi < ciaaModbus_masterObj[hModbusMaster].quantityR ; loopi++)
      {
         /* copy data to holding registers address */
         ciaaModbus_masterObj[hModbusMaster].pDataR[loopi] = ciaaModbus_readInt(&pdu[2+loopi*2]);
      }

      /* return no error */
      ret = CIAA_MODBUS_E_NO_ERROR;
   }
   else
   {
      /* return wrong pdu */
      ret = CIAA_MODBUS_E_PDU_RECEIVED_WRONG;
   }

   return ret;
}

/** \brief Process data to function 0x05 or 0x06
 **
 ** This function process PDU response as request function 0x05 or 0x06
 **
 ** \param[in] handler handler modbus master
 ** \param[in] pdu pdu received
 ** \param[out] size size of pdu.
 ** \return CIAA_MODBUS_E_NO_ERROR correct response
 **         CIAA_MODBUS_E_PDU_RECEIVED_WRONG incorrect response
 **/
static uint8_t ciaaModbus_masterProcess0x05_0x06(uint32_t hModbusMaster, uint8_t *pdu, uint16_t size)
{
   uint8_t ret;

   if ( (ciaaModbus_readInt(&pdu[1]) == ciaaModbus_masterObj[hModbusMaster].startAddressW) &&
        (uint16_t)(ciaaModbus_readInt(&pdu[3]) == (uint16_t)ciaaModbus_masterObj[hModbusMaster].dataW) )
   {
      ret = CIAA_MODBUS_E_NO_ERROR;
   }
   else
   {
      /* set exception code CIAA_MODBUS_E_PDU_RECEIVED_WRONG */
      ret = CIAA_MODBUS_E_PDU_RECEIVED_WRONG;
   }

   return ret;
}

/** \brief Process data to function 0x0F or 0x10
 **
 ** This function process PDU response as request function 0x0F or 0x10
 **
 ** \param[in] handler handler modbus master
 ** \param[in] pdu pdu received
 ** \param[out] size size of pdu.
 ** \return CIAA_MODBUS_E_NO_ERROR correct response
 **         CIAA_MODBUS_E_PDU_RECEIVED_WRONG incorrect response
 **/
static uint8_t ciaaModbus_masterProcess0x0F_0x10(uint32_t hModbusMaster, uint8_t *pdu, uint16_t size)
{
   uint8_t ret;

   if ( (ciaaModbus_readInt(&pdu[1]) == ciaaModbus_masterObj[hModbusMaster].startAddressW) &&
        (ciaaModbus_readInt(&pdu[3]) == ciaaModbus_masterObj[hModbusMaster].quantityW) )
   {
      ret = CIAA_MODBUS_E_NO_ERROR;
   }
   else
   {
      /* set exception code CIAA_MODBUS_E_PDU_RECEIVED_WRONG */
      ret = CIAA_MODBUS_E_PDU_RECEIVED_WRONG;
   }

   return ret;
}

/** \brief Perform read operation in modbus master
 **
 ** This function start read operation and wait for
 ** end or return if callback not NULL
 **
 ** \param[in] handler handler modbus master
 ** \param[in] startAddress starting address to read
 ** \param[in] quantity of coils/register to read
 ** \param[out] pData pointer to store data reads
 ** \param[in] slaveId slave identification number
 ** \param[in] cbEndComm call back function
 **            if NULL pointer is passed, this function block until
 **            end of communication (using MODBUSE).
 **            if call back is passed, this function return and
 **            call back will be executed at end of communication
 ** \return -1 if master is busy or invalid id passed
 **         0 successful
 **         1 function not supported
 **         2 wrong starting address
 **         3 wrong quantity
 **         4 function error
 **         5 the slave not respond
 **         6 pdu received wrong
 **/
static int8_t ciaaModbus_masterRead(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      void *pData,
      uint8_t slaveId,
      uint8_t cmd,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   /* check if no command pending and valid slave id */
   if ( (ciaaModbus_masterObj[hModbusMaster].cmd == 0) &&
      (0 != slaveId) )
   {
      /* no exception code */
      ciaaModbus_masterObj[hModbusMaster].exceptioncode = 0;

      /* set start address */
      ciaaModbus_masterObj[hModbusMaster].startAddressR = startAddress;

      /* set quantity of registers */
      ciaaModbus_masterObj[hModbusMaster].quantityR = quantity;

      /* set pointer to store data read */
      ciaaModbus_masterObj[hModbusMaster].pDataR = pData;

      /* set slave id */
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;

      /* set retry count */
      ciaaModbus_masterObj[hModbusMaster].retryCount = ciaaModbus_masterObj[hModbusMaster].retryComm;

      /* set task id to set event if blocking operation */
      GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);

      /* set call back if non-blocking operation */
      ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;

      /* set command to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = cmd;

      /* if no callback wait event... */
      if (NULL == cbEndComm)
      {
         /* wait for event */
         WaitEvent(MODBUSE);

         ClearEvent(MODBUSE);

         /* return exception code */
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         /* if non-blocking, return 0 */
         ret = 0;
      }
   }
   else
   {
      /* return -1 if it was not possible execute the function */
      ret = -1;
   }

   return ret;
}


/*==================[external functions definition]==========================*/
extern void ciaaModbus_masterInit(void)
{
   int32_t loopi;

   /* initialize all Transport Objects */
   for (loopi = 0 ; loopi < CIAA_MODBUS_TOTAL_MASTERS ; loopi++)
   {
      /* no function to execute */
      ciaaModbus_masterObj[loopi].cmd = 0;

      /* invalid slave id */
      ciaaModbus_masterObj[loopi].slaveId = 0;

      /* not in use */
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

      /* no function to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = 0x00;

      /* set default timeout */
      ciaaModbus_masterObj[hModbusMaster].respTimeout = CIAA_MODBUS_MASTER_DEFAULT_TIMEOUT;

      /* set default retry comm */
      ciaaModbus_masterObj[hModbusMaster].retryComm = CIAA_MODBUS_MASTER_DEFAULT_RETRY_COMM;
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

extern int8_t ciaaModbus_masterCmd0x01ReadCoils(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      uint8_t *coilValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   ret = ciaaModbus_masterRead(
         hModbusMaster,
         startAddress,
         quantity,
         coilValue,
         slaveId,
         CIAA_MODBUS_FCN_READ_COILS,
         cbEndComm);

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x02ReadDiscreteInputs(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      uint8_t *discreteInputsValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   ret = ciaaModbus_masterRead(
         hModbusMaster,
         startAddress,
         quantity,
         discreteInputsValue,
         slaveId,
         CIAA_MODBUS_FCN_READ_DISCRETE_INPUTS,
         cbEndComm);

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x03ReadHoldingRegisters(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      int16_t *hrValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   ret = ciaaModbus_masterRead(
         hModbusMaster,
         startAddress,
         quantity,
         hrValue,
         slaveId,
         CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS,
         cbEndComm);

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x04ReadInputRegisters(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      int16_t *irValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   ret = ciaaModbus_masterRead(
         hModbusMaster,
         startAddress,
         quantity,
         irValue,
         slaveId,
         CIAA_MODBUS_FCN_READ_INPUT_REGISTERS,
         cbEndComm);

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x05WriteSingleCoil(
      int32_t hModbusMaster,
      uint16_t startAddress,
      bool value,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   /* check if no command pending and valid slave id */
   if ( (ciaaModbus_masterObj[hModbusMaster].cmd == 0) &&
      (0 != slaveId) )
   {
      /* no exception code */
      ciaaModbus_masterObj[hModbusMaster].exceptioncode = 0;

      /* set start address */
      ciaaModbus_masterObj[hModbusMaster].startAddressW = startAddress;

      /* set coil value */
      if (value)
      {
         ciaaModbus_masterObj[hModbusMaster].dataW = 0xFF00;
      }
      else
      {
         ciaaModbus_masterObj[hModbusMaster].dataW = 0x0000;
      }

      /* set slave id */
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;

      /* set retry count */
      ciaaModbus_masterObj[hModbusMaster].retryCount = ciaaModbus_masterObj[hModbusMaster].retryComm;

      /* set task id to set event if blocking operation */
      GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);

      /* set call back if non-blocking operation */
      ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;

      /* set command to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = CIAA_MODBUS_FCN_WRITE_SINGLE_COIL;

      /* if no callback wait event... */
      if (NULL == cbEndComm)
      {
         /* wait for event */
         WaitEvent(MODBUSE);

         ClearEvent(MODBUSE);

         /* return exception code */
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         /* if non-blocking, return 0 */
         ret = 0;
      }
   }
   else
   {
      /* return -1 if it was not possible execute the function */
      ret = -1;
   }

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x06WriteSingleRegister(
      int32_t hModbusMaster,
      uint16_t startAddress,
      int16_t value,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   /* check if no command pending and valid slave id */
   if ( (ciaaModbus_masterObj[hModbusMaster].cmd == 0) &&
      (0 != slaveId) )
   {
      /* no exception code */
      ciaaModbus_masterObj[hModbusMaster].exceptioncode = 0;

      /* set start address */
      ciaaModbus_masterObj[hModbusMaster].startAddressW = startAddress;

      /* set register value */
      ciaaModbus_masterObj[hModbusMaster].dataW = value;

      /* set slave id */
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;

      /* set retry count */
      ciaaModbus_masterObj[hModbusMaster].retryCount = ciaaModbus_masterObj[hModbusMaster].retryComm;

      /* set task id to set event if blocking operation */
      GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);

      /* set call back if non-blocking operation */
      ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;

      /* set command to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = CIAA_MODBUS_FCN_WRITE_SINGLE_REGISTER;

      /* if no callback wait event... */
      if (NULL == cbEndComm)
      {
         /* wait for event */
         WaitEvent(MODBUSE);

         ClearEvent(MODBUSE);

         /* return exception code */
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         /* if non-blocking, return 0 */
         ret = 0;
      }
   }
   else
   {
      /* return -1 if it was not possible execute the function */
      ret = -1;
   }

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x0FWriteMultipleCoils(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      uint8_t *coilsValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   /* check if no command pending and valid slave id */
   if ( (ciaaModbus_masterObj[hModbusMaster].cmd == 0) &&
      (0 != slaveId) )
   {
      /* no exception code */
      ciaaModbus_masterObj[hModbusMaster].exceptioncode = 0;

      /* set start address */
      ciaaModbus_masterObj[hModbusMaster].startAddressW = startAddress;

      /* set quantity of coils */
      ciaaModbus_masterObj[hModbusMaster].quantityW = quantity;

      /* set pointer to read data */
      ciaaModbus_masterObj[hModbusMaster].pDataW = (int16_t*)coilsValue;

      /* set slave id */
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;

      /* set retry count */
      ciaaModbus_masterObj[hModbusMaster].retryCount = ciaaModbus_masterObj[hModbusMaster].retryComm;

      /* set task id to set event if blocking operation */
      GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);

      /* set call back if non-blocking operation */
      ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;

      /* set command to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = CIAA_MODBUS_FCN_WRITE_MULTIPLE_COILS;

      /* if no callback wait event... */
      if (NULL == cbEndComm)
      {
         /* wait for event */
         WaitEvent(MODBUSE);

         ClearEvent(MODBUSE);

         /* return exception code */
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         /* if non-blocking, return 0 */
         ret = 0;
      }
   }
   else
   {
      /* return -1 if it was not possible execute the function */
      ret = -1;
   }

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x10WriteMultipleRegisters(
      int32_t hModbusMaster,
      uint16_t startAddress,
      uint16_t quantity,
      int16_t *hrValue,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   /* check if no command pending and valid slave id */
   if ( (ciaaModbus_masterObj[hModbusMaster].cmd == 0) &&
      (0 != slaveId) )
   {
      /* no exception code */
      ciaaModbus_masterObj[hModbusMaster].exceptioncode = 0;

      /* set start address */
      ciaaModbus_masterObj[hModbusMaster].startAddressW = startAddress;

      /* set quantity of registers */
      ciaaModbus_masterObj[hModbusMaster].quantityW = quantity;

      /* set pointer to store data read */
      ciaaModbus_masterObj[hModbusMaster].pDataW = hrValue;

      /* set slave id */
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;

      /* set retry count */
      ciaaModbus_masterObj[hModbusMaster].retryCount = ciaaModbus_masterObj[hModbusMaster].retryComm;

      /* set task id to set event if blocking operation */
      GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);

      /* set call back if non-blocking operation */
      ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;

      /* set command to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = CIAA_MODBUS_FCN_WRITE_MULTIPLE_REGISTERS;

      /* if no callback wait event... */
      if (NULL == cbEndComm)
      {
         /* wait for event */
         WaitEvent(MODBUSE);

         ClearEvent(MODBUSE);

         /* return exception code */
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         /* if non-blocking, return 0 */
         ret = 0;
      }
   }
   else
   {
      /* return -1 if it was not possible execute the function */
      ret = -1;
   }

   return ret;
}

extern int8_t ciaaModbus_masterCmd0x17ReadWriteMultipleRegisters(
      int32_t hModbusMaster,
      uint16_t startAddressR,
      uint16_t quantityR,
      int16_t *hrValueR,
      uint16_t startAddressW,
      uint16_t quantityW,
      int16_t *hrValueW,
      uint8_t slaveId,
      modbusMaster_cbEndOfCommType cbEndComm)
{
   int8_t ret;

   /* check if no command pending and valid slave id */
   if ( (ciaaModbus_masterObj[hModbusMaster].cmd == 0) &&
      (0 != slaveId) )
   {
      /* no exception code */
      ciaaModbus_masterObj[hModbusMaster].exceptioncode = 0;

      /* set start address */
      ciaaModbus_masterObj[hModbusMaster].startAddressR = startAddressR;
      ciaaModbus_masterObj[hModbusMaster].startAddressW = startAddressW;

      /* set quantity of registers */
      ciaaModbus_masterObj[hModbusMaster].quantityR = quantityR;
      ciaaModbus_masterObj[hModbusMaster].quantityW = quantityW;

      /* set pointer to store data read */
      ciaaModbus_masterObj[hModbusMaster].pDataR = hrValueR;
      ciaaModbus_masterObj[hModbusMaster].pDataW = hrValueW;

      /* set slave id */
      ciaaModbus_masterObj[hModbusMaster].slaveId = slaveId;

      /* set retry count */
      ciaaModbus_masterObj[hModbusMaster].retryCount = ciaaModbus_masterObj[hModbusMaster].retryComm;

      /* set task id to set event if blocking operation */
      GetTaskID(&ciaaModbus_masterObj[hModbusMaster].taskID);

      /* set call back if non-blocking operation */
      ciaaModbus_masterObj[hModbusMaster].cbEndComm = cbEndComm;

      /* set command to execute */
      ciaaModbus_masterObj[hModbusMaster].cmd = CIAA_MODBUS_FCN_READ_WRITE_MULTIPLE_REGISTERS;

      /* if no callback wait event... */
      if (NULL == cbEndComm)
      {
         /* wait for event */
         WaitEvent(MODBUSE);

         ClearEvent(MODBUSE);

         /* return exception code */
         ret = ciaaModbus_masterObj[hModbusMaster].exceptioncode;
      }
      else
      {
         /* if non-blocking, return 0 */
         ret = 0;
      }
   }
   else
   {
      /* return -1 if it was not possible execute the function */
      ret = -1;
   }

   return ret;
}

extern void ciaaModbus_masterTask(int32_t handler)
{
   /* is there a function to perform? */
   if (0x00 != ciaaModbus_masterObj[handler].cmd)
   {
      /* if retry count available, decrement it */
      if (ciaaModbus_masterObj[handler].retryCount > 0)
      {
         ciaaModbus_masterObj[handler].retryCount--;
      }
      /* if no retry count available, cancel */
      else
      {
         /* if no exception code, set CIAA_MODBUS_E_SLAVE_NOT_RESPOND */
         if (ciaaModbus_masterObj[handler].exceptioncode == CIAA_MODBUS_E_NO_ERROR)
         {
            ciaaModbus_masterObj[handler].exceptioncode = CIAA_MODBUS_E_SLAVE_NOT_RESPOND;
         }
         /* if no callback, set event */
         if (NULL == ciaaModbus_masterObj[handler].cbEndComm)
         {
            SetEvent(ciaaModbus_masterObj[handler].taskID, MODBUSE);
         }
         else
         {
            /* if callback set, perform it */
            ciaaModbus_masterObj[handler].cbEndComm(
                  ciaaModbus_masterObj[handler].slaveId,
                  ciaaModbus_masterObj[handler].cmd,
                  ciaaModbus_masterObj[handler].exceptioncode);
         }

         /* cancel function */
         ciaaModbus_masterObj[handler].cmd = 0;
      }
   }
}

extern void ciaaModbus_masterRecvMsg(
      int32_t handler,
      uint8_t *id,
      uint8_t *pdu,
      uint32_t *size)
{
   uint32_t loopi;

   /* is there function to perform? */
   if (0x00 != ciaaModbus_masterObj[handler].cmd)
   {
      /* set slave id */
      *id = ciaaModbus_masterObj[handler].slaveId;

      /* set function code */
      pdu[0] = ciaaModbus_masterObj[handler].cmd;

      /* according to function, make pdu */
      switch (pdu[0])
      {
         /* read coils */
         case CIAA_MODBUS_FCN_READ_COILS:
         /* read discrete inputs */
         case CIAA_MODBUS_FCN_READ_DISCRETE_INPUTS:
         /* read holding registers */
         case CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS:
         /* read input registers */
         case CIAA_MODBUS_FCN_READ_INPUT_REGISTERS:

            /* write in buffer: start address */
            ciaaModbus_writeInt(&pdu[1], ciaaModbus_masterObj[handler].startAddressR);

            /* write in buffer: quantity of registers */
            ciaaModbus_writeInt(&pdu[3], ciaaModbus_masterObj[handler].quantityR);

            /* lenght of pdu */
            *size = 5;

            break;

         /* write single coil */
         case CIAA_MODBUS_FCN_WRITE_SINGLE_COIL:
         /* write single coil */
         case CIAA_MODBUS_FCN_WRITE_SINGLE_REGISTER:

            /* write in buffer: start address */
            ciaaModbus_writeInt(&pdu[1], ciaaModbus_masterObj[handler].startAddressW);

            /* write in buffer: quantity of registers */
            ciaaModbus_writeInt(&pdu[3], ciaaModbus_masterObj[handler].dataW);

            /* lenght of pdu */
            *size = 5;

            break;

         /* write multiple coils */
         case CIAA_MODBUS_FCN_WRITE_MULTIPLE_COILS:

            /* write in buffer: start address */
            ciaaModbus_writeInt(&pdu[1], ciaaModbus_masterObj[handler].startAddressW);

            /* write in buffer: quantity of registers */
            ciaaModbus_writeInt(&pdu[3], ciaaModbus_masterObj[handler].quantityW);

            /* write in buffer: byte count */
            pdu[5] = ciaaModbus_masterObj[handler].quantityW / 8;
            if (ciaaModbus_masterObj[handler].quantityW % 8)
            {
               pdu[5]++;
            }

            for (loopi = 0 ; loopi < ciaaModbus_masterObj[handler].quantityW ; loopi++)
            {
               /* write in buffer: outputs value */
               pdu[6+loopi] = ((uint8_t*)ciaaModbus_masterObj[handler].pDataW)[loopi];
            }

            /* lenght of pdu */
            *size = 6 + pdu[5];

            break;

         /* write multiple registers */
         case CIAA_MODBUS_FCN_WRITE_MULTIPLE_REGISTERS:

            /* write in buffer: start address */
            ciaaModbus_writeInt(&pdu[1], ciaaModbus_masterObj[handler].startAddressW);

            /* write in buffer: quantity of registers */
            ciaaModbus_writeInt(&pdu[3], ciaaModbus_masterObj[handler].quantityW);

            /* write in buffer: byte count */
            pdu[5] = ciaaModbus_masterObj[handler].quantityW * 2;

            for (loopi = 0 ; loopi < ciaaModbus_masterObj[handler].quantityW ; loopi++)
            {
               /* write in buffer: register value */
               ciaaModbus_writeInt(&pdu[6+loopi*2], ciaaModbus_masterObj[handler].pDataW[loopi]);
            }

            /* lenght of pdu */
            *size = 6 + ciaaModbus_masterObj[handler].quantityW * 2;

            break;

         /* write multiple registers */
         case CIAA_MODBUS_FCN_READ_WRITE_MULTIPLE_REGISTERS:

            /* write in buffer: start address (read) */
            ciaaModbus_writeInt(&pdu[1], ciaaModbus_masterObj[handler].startAddressR);

            /* write in buffer: quantity of registers (read)*/
            ciaaModbus_writeInt(&pdu[3], ciaaModbus_masterObj[handler].quantityR);

            /* write in buffer: start address (write) */
            ciaaModbus_writeInt(&pdu[5], ciaaModbus_masterObj[handler].startAddressW);

            /* write in buffer: quantity of registers (write) */
            ciaaModbus_writeInt(&pdu[7], ciaaModbus_masterObj[handler].quantityW);

            /* write in buffer: byte count */
            pdu[9] = ciaaModbus_masterObj[handler].quantityW * 2;

            for (loopi = 0 ; loopi < ciaaModbus_masterObj[handler].quantityW ; loopi++)
            {
               /* write in buffer: register value */
               ciaaModbus_writeInt(&pdu[10+loopi*2], ciaaModbus_masterObj[handler].pDataW[loopi]);
            }

            /* lenght of pdu */
            *size = 10 + ciaaModbus_masterObj[handler].quantityW * 2;

            break;

         /* function not defined */
         default:

            /* lenght of pdu */
            *size = 0;
            break;
      }
   }
}

extern void ciaaModbus_masterSendMsg(
      int32_t handler,
      uint8_t id,
      uint8_t *pdu,
      uint32_t size)
{
   /* check slave id */
   if (id == ciaaModbus_masterObj[handler].slaveId)
   {
      /* if correct function code, process data */
      if (pdu[0] == ciaaModbus_masterObj[handler].cmd)
      {
         switch (pdu[0])
         {
            /* read coils */
            case CIAA_MODBUS_FCN_READ_COILS:
            /* read discrete inputs */
            case CIAA_MODBUS_FCN_READ_DISCRETE_INPUTS:
               ciaaModbus_masterObj[handler].exceptioncode = \
               ciaaModbus_masterProcess0x01_0x02(handler, pdu, size);
               break;

            /* read holding registers */
            case CIAA_MODBUS_FCN_READ_HOLDING_REGISTERS:
            /* read input registers */
            case CIAA_MODBUS_FCN_READ_INPUT_REGISTERS:
            /* read write multiple registers  */
            case CIAA_MODBUS_FCN_READ_WRITE_MULTIPLE_REGISTERS:
               ciaaModbus_masterObj[handler].exceptioncode = \
               ciaaModbus_masterProcess0x03_0x04_0x17(handler, pdu, size);
               break;

            /* write single coil */
            case CIAA_MODBUS_FCN_WRITE_SINGLE_COIL:
            /* write single register */
            case CIAA_MODBUS_FCN_WRITE_SINGLE_REGISTER:
               ciaaModbus_masterObj[handler].exceptioncode = \
                  ciaaModbus_masterProcess0x05_0x06(handler, pdu, size);
               break;

            /* write multiple coils */
            case CIAA_MODBUS_FCN_WRITE_MULTIPLE_COILS:
            /* write multiple registers */
            case CIAA_MODBUS_FCN_WRITE_MULTIPLE_REGISTERS:
               ciaaModbus_masterObj[handler].exceptioncode = \
               ciaaModbus_masterProcess0x0F_0x10(handler, pdu, size);
               break;
         }
      }
      /* if received error function */
      else if (pdu[0] == (ciaaModbus_masterObj[handler].cmd | 0x80) )
      {
         /* set exception code send from slave */
         ciaaModbus_masterObj[handler].exceptioncode = pdu[1];
      }
      /* if invalid function code */
      else
      {
         /* set exception code CIAA_MODBUS_E_PDU_RECEIVED_WRONG */
         ciaaModbus_masterObj[handler].exceptioncode = CIAA_MODBUS_E_PDU_RECEIVED_WRONG;
      }

      /* if no error */
      if (CIAA_MODBUS_E_NO_ERROR == ciaaModbus_masterObj[handler].exceptioncode)
      {
         /* if no callback, set event */
         if (NULL == ciaaModbus_masterObj[handler].cbEndComm)
         {
            SetEvent(ciaaModbus_masterObj[handler].taskID, MODBUSE);
         }
         else
         {
            /* if callback set, perform it */
            ciaaModbus_masterObj[handler].cbEndComm(
                  ciaaModbus_masterObj[handler].slaveId,
                  ciaaModbus_masterObj[handler].cmd,
                  ciaaModbus_masterObj[handler].exceptioncode);
         }
         /* finish function */
         ciaaModbus_masterObj[handler].cmd = 0x00;
      }
   }
}

extern uint32_t ciaaModbus_masterGetRespTimeout(int32_t handler)
{
   return ciaaModbus_masterObj[handler].respTimeout;
}

#endif /* #if CIAA_MODBUS_TOTAL_MASTERS > 0 */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


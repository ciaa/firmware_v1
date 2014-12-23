/* Copyright 2014, Mariano Cerdeiro
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

/** \brief This file implements the Modbus configuration
 **
 ** This file implements the configuration of the modbus
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
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140713 v0.0.1 initials initial
 */

/*==================[inclusions]=============================================*/
#include "ciaaModbus_Cfg.h"
#include "ciaaPOSIX_stdlib.h"

/*******************[cfg needed includes]*************************************/
/** This section shall contain the header files containing the decalartion of
 ** the referenced functions.
 **/
#include "blinking_modbus_adc_dac.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
#if (CIAAMODBUS_READ_HOLDING_REGISTERS == CIAAMODBUS_EN)
ciaaModbus_cmdLst0x03Type ciaaModbus_cmdLst0x03[] = {
   {
      {
         0x0000,
         0x0001
      },
      readHoldingRegisters
   },
   { { 0x0000, 0x0000 }, NULL /** <= last entry */ }
};
#endif /* #if (CIAAMODBUS_READ_HOLDING_REGISTERS == CIAAMODBUS_EN) */

#if (CIAAMODBUS_READ_INPUT_REGISTERS == CIAAMODBUS_EN)
ciaaModbus_cmdLst0x04Type ciaaModbus_cmdLst0x04[] = {
   {
      {
         0x0000,
         0x0000
      },
      readInputRegisters
   },
   { { 0x0000, 0x0000 }, NULL /** <= last entry */ }
};
#endif /* #if (CIAAMODBUS_READ_INPUT_REGISTERS == CIAAMODBUS_EN) */

#if (CIAAMODBUS_WRITE_SINGLE_REGISTER == CIAAMODBUS_EN)
ciaaModbus_cmdLst0x06Type ciaaModbus_cmdLst0x06[] = {
   {
      {
         0x0000,
         0x0001
      },
      writeSingleRegister
   },
   { { 0x0000, 0x0000 }, NULL /** <= last entry */ }
};
#endif /* #if (CIAAMODBUS_WRITE_SINGLE_REGISTER == CIAAMODBUS_EN) */

#if (CIAAMODBUS_WRITE_MULTIPLE_REGISTERS == CIAAMODBUS_EN)
ciaaModbus_cmdLst0x10Type ciaaModbus_cmdLst0x10[] = {
   {
      {
         0x0000,
         0x0001
      },
      writeMultipleRegisters
   },
   { { 0x0000, 0x0000 }, NULL /** <= last entry */ }
};
#endif /* #if (CIAAMODBUS_WRITE_MULTIPLE_REGISTERS == CIAAMODBUS_EN) */

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


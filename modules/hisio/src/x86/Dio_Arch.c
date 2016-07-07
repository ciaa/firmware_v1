/* Copyright 2015, Mariano Cerdeiro
 * Copyright 2016, Juan Cecconi
 * All rights reserved.
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

/** \brief IO DIO Driver Arch source file
 **
 ** This file contains is the Dio driver Arch x86
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup HISIO HisIO Module
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * JuCe         Juan Cecconi
 * JMC          Juan Manuel Cruz
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160111 v0.0.1 JuCe initial version
 */

/*==================[inclusions]=============================================*/
#include "IODriver_Base.h"
#include "Dio_Cfg.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern IO_ErrorType Dio_InitSync_Arch(void * address)
{
   return IO_E_OK;
}

extern IO_ValueType Dio_GetSync_Arch(IO_ChannelType channel)
{
   return IO_LOW;
}

extern void Dio_SetSync_Arch(IO_ChannelType channel, IO_ValueType value)
{

}

extern void Dio_ToggleSync_Arch(IO_ChannelType channel, IO_ValueType value)
{

}

extern IO_ValueType Dio_GetPortSync_Arch(IO_ChannelType channel)
{
   return 0xFF;
}

extern void Dio_SetPortSync_Arch(IO_ChannelType channel, IO_ValueType value)
{

}

extern void Dio_SetPortMaskedSync_Arch(IO_ChannelType channel, IO_ValueType value, IO_ValueType mask)
{

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

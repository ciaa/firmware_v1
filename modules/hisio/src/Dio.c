/* Copyright 2015, Mariano Cerdeiro
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

/** \brief IO DIO Driver source file
 **
 ** This file contains is the Dio driver
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
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151222 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "IODriver_Int.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
This function initialises the DIO driver. It uses the configured parameters of the IO configuration
translated from the OIL file.
Ref: API IO Driver v 2.1.3
     5.2.2.1
*/
extern IO_ErrorType Dio_InitSync(void * address)
{
   IO_ErrorType ret;

   ret = Dio_InitSync_Arch(address);
   return ret;
}

/*
This function reads the input or output pin specified by the channel descriptor and returns
the current level.
Ref: API IO Driver v 2.1.3
     5.2.2.5
*/
extern IO_ValueType Dio_GetSync(IO_ChannelType channel)
{
   return Dio_GetSync_Arch(channel);
}

/*
This function sets the  output pin specified by the channel descriptor.
Ref: API IO Driver v 2.1.3
     5.2.2.6
*/
extern void Dio_SetSync(IO_ChannelType channel, IO_ValueType value)
{
   Dio_SetSync_Arch(channel, value);
}

/*
This function toggles the state of the output pin specified by the channel descriptor.
   Ref: Not within the standard
*/
extern void Dio_ToggleSync(IO_ChannelType channel)
{
   Dio_ToggleSync_Arch(channel);
}

/*
This funtion reads DIO port specified by the port descriptor and returns the current pin levels of that port.
Ref: API IO Driver v 2.1.3
     5.2.2.7
*/
extern IO_ValueType Dio_GetPortSync(IO_ChannelType port)
{
   return Dio_GetPortSync_Arch(IO_ChannelType port);
}

/*
This funtion sets a complete DIO port in one step. Only pins configured as output will be set.
The port functions always use the configuration settings of the assigned channel
Ref: API IO Driver v 2.1.3
     5.2.2.8
*/
extern void Dio_SetPortSync(IO_ChannelType port, IO_ValueType value)
{
   Dio_SetPortSync_Arch( port, value);
}

/*
This funtion sets a complete DIO port in one step. Only pins specified in mask (configured as output) will be set.
The port functions always use the configuration settings of the assigned channel
Ref: API IO Driver v 2.1.3
     5.2.2.9
*/
extern void Dio_SetPortMaskedSync(IO_ChannelType port, IO_ValueType value, IO_ValueType mask)
{
   Dio_SetPortMaskedSync_Arch( port,  value,  mask);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

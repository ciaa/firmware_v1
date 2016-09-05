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
 ** This file contains is the Dio driver Arch CortexM4\lpc43xx\lpc4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup HISIO HisIO Module
 ** @{ */


/*==================[inclusions]=============================================*/
#include "IODriver_Base.h"
#include "Dio_Cfg.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern IO_ErrorType Dio_InitSync_Arch(void * address)
{
#if (DIO_PINS_COUNT != 0)
   uint16_t i;

   for(i=0 ; i < DIO_PINS_COUNT ; i++)
   {
      if(Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_INPUT)
      {
         Chip_SCU_PinMux(Dio_Config.Pins[i].Port, Dio_Config.Pins[i].Pin, SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS, Dio_Config.Pins[i].GPIO_Func); /* Set it as GPIO */
         Chip_GPIO_SetDir(LPC_GPIO_PORT, Dio_Config.Pins[i].GPIO_Port, (1 << Dio_Config.Pins[i].GPIO_Pin), 0); /* Set it as input */
      }
      else if(Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW)
      {
         Chip_SCU_PinMux(Dio_Config.Pins[i].Port, Dio_Config.Pins[i].Pin, SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS, Dio_Config.Pins[i].GPIO_Func);
         Chip_GPIO_SetDir(LPC_GPIO_PORT, Dio_Config.Pins[i].GPIO_Port, (1 << Dio_Config.Pins[i].GPIO_Pin), 1); /* Set it as output */
         Chip_GPIO_SetPinState(LPC_GPIO_PORT, Dio_Config.Pins[i].GPIO_Port, Dio_Config.Pins[i].GPIO_Pin, 0); /* Set it as low */
      }
      else if(Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH)
      {
         Chip_SCU_PinMux(Dio_Config.Pins[i].Port, Dio_Config.Pins[i].Pin, SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS, Dio_Config.Pins[i].GPIO_Func);
         Chip_GPIO_SetDir(LPC_GPIO_PORT, Dio_Config.Pins[i].GPIO_Port, (1 << Dio_Config.Pins[i].GPIO_Pin), 1); /* Set it as output */
         Chip_GPIO_SetPinState(LPC_GPIO_PORT, Dio_Config.Pins[i].GPIO_Port, Dio_Config.Pins[i].GPIO_Pin, 1); /* Set it as high */
      }
   }
#endif
   return IO_E_OK;
}

#if (DIO_PINS_COUNT != 0)
extern IO_ValueType Dio_GetSync_Arch(IO_ChannelType channel)
{
   IO_ValueType value;
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if(DIO_PINS_COUNT > channel)
#endif
   {
      value = (IO_ValueType) Chip_GPIO_GetPinState(LPC_GPIO_PORT, Dio_Config.Pins[channel].GPIO_Port, Dio_Config.Pins[channel].GPIO_Pin); /* Get this value */
      if(Dio_Config.Pins[channel].Flags & DIO_CONFIG_PIN_INVERTED)
      {
         value = !value;
      }
   }
   return value;
}
#endif

#if (DIO_PINS_COUNT != 0)
extern void Dio_SetSync_Arch(IO_ChannelType channel, IO_ValueType value)
{
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if(DIO_PINS_COUNT > channel && (Dio_Config.Pins[channel].Flags & (DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW | DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH)))
#endif
   {
      if(Dio_Config.Pins[channel].Flags & DIO_CONFIG_PIN_INVERTED)
      {
         value = !value;
      }
      Chip_GPIO_SetPinState(LPC_GPIO_PORT, Dio_Config.Pins[channel].GPIO_Port, Dio_Config.Pins[channel].GPIO_Pin, value); /* Set this value */
   }
}
#endif

#if (DIO_PINS_COUNT != 0)
extern void Dio_ToggleSync_Arch(IO_ChannelType channel, IO_ValueType value)
{
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if(DIO_PINS_COUNT > channel && (Dio_Config.Pins[channel].Flags & (DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW | DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH)))
#endif
   {
      Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, Dio_Config.Pins[channel].GPIO_Port, Dio_Config.Pins[channel].GPIO_Pin); /* Toggle previous value */
   }
}
#endif

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

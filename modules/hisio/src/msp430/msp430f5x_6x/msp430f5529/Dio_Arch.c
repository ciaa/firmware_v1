/* Copyright 2016, Franco Bucafusco
 *
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
#include "msp430.h"
#include "driverlib.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
This function initialises the DIO driver for MSP430.
*/
extern IO_ErrorType Dio_InitSync_Arch(void * address)
{
#if( DIO_PINS_COUNT != 0 )
   uint16_t i;

   for( i=0 ; i < DIO_PINS_COUNT ; i++ )
   {
      if( Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_INPUT )
      {
         if( Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_INPUT_PULLEDUP )
         {
             GPIO_setAsInputPinWithPullUpResistor( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
         }
         else
         if( Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_INPUT_PULLEDDOWN )
         {
             GPIO_setAsInputPinWithPullDownResistor( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
         }
         else
         {
            GPIO_setAsInputPin( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
         }

      }
      else if( Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW )
      {
         GPIO_setOutputLowOnPin( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
         GPIO_setAsOutputPin( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
      }
      else if( Dio_Config.Pins[i].Flags & DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH )
      {
         GPIO_setOutputHighOnPin( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
         GPIO_setAsOutputPin( Dio_Config.Pins[i].Port , Dio_Config.Pins[i].PinMask );
      }
   }

#endif

   return IO_E_OK;
}


#if (DIO_PINS_COUNT != 0)
/*
This function reads the input or output pin specified by the channel descriptor and returns
the current level. Implementation for MSP430.
*/
extern IO_ValueType Dio_GetSync_Arch(IO_ChannelType channel)
{
   IO_ValueType rv;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if( DIO_PINS_COUNT > channel )
#endif
   {
      rv = (IO_ValueType) GPIO_getInputPinValue( Dio_Config.Pins[channel].Port , Dio_Config.Pins[channel].PinMask ); /* Get this value */

      if( Dio_Config.Pins[channel].Flags & DIO_CONFIG_PIN_INVERTED)
      {
         rv = !rv;
      }
   }
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   else
   {
#ifdef HISIO_DIO_ENABLE_ERRORHOOK
      //call the error hook with IO_E_INVALID_CHANNEL_ID
#endif

      rv = IO_INVALID;
   }
#endif

   return rv;
}
#endif

#if (DIO_PINS_COUNT != 0)
/*
This function sets the  output pin specified by the channel descriptor.
Implementation for MSP430
Ref: API IO Driver v 2.1.3
     5.2.2.6
*/
extern void Dio_SetSync_Arch(IO_ChannelType channel, IO_ValueType value)
{
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if( DIO_PINS_COUNT > channel && (Dio_Config.Pins[channel].Flags & (DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW | DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH)) )
#endif
   {
      if(Dio_Config.Pins[channel].Flags & DIO_CONFIG_PIN_INVERTED)
      {
         value = !value;
      }

      if( value!=0)
      {
         GPIO_setOutputLowOnPin( Dio_Config.Pins[channel].Port , Dio_Config.Pins[channel].PinMask ) ;
      }
      else
      {
         GPIO_setOutputHighOnPin( Dio_Config.Pins[channel].Port , Dio_Config.Pins[channel].PinMask ) ;
      }
   }
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   else
   {
#ifdef HISIO_DIO_ENABLE_ERRORHOOK
      //call the error hook with IO_E_INVALID_VALUE or IO_E_INVALID_CHANNEL_ID
#endif
   }
#endif

}
#endif

#if (DIO_PINS_COUNT != 0)
/*
This function toggles the state of the output pin specified by the channel descriptor.
Implementation for MSP430
   Ref: Not within the standard
*/
extern void Dio_ToggleSync_Arch(IO_ChannelType channel )
{
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if(DIO_PINS_COUNT > channel && (Dio_Config.Pins[channel].Flags & (DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW | DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH)))
#endif
   {
      GPIO_toggleOutputOnPin( Dio_Config.Pins[channel].Port , Dio_Config.Pins[channel].PinMask );
   }
   else
   {
#ifdef HISIO_DIO_ENABLE_ERRORHOOK
      //call the error hook with   IO_E_INVALID_CHANNEL_ID
#endif
   }
}
#endif

/*
This funtion reads DIO port specified by the port descriptor and returns the current pin levels of that port.
Implementation for MSP430
Ref: API IO Driver v 2.1.3
     5.2.2.7
*/
extern IO_ValueType Dio_GetPortSync_Arch(IO_ChannelType port)
{
   IO_ValueType rv = 0;
   IO_ValueType rv_tmp;
   uint8_t channel;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   /* check if the requested port is configured */
   if( DIO_PORTS_COUNT>port )
#endif
   {
      for( channel=0; channel<DIO_PINS_COUNT; channel++ )
      {
         /* validate that the port requested is configured within any DIO channel*/
         if( Dio_Config.Ports[port].Port == Dio_Config.Pins[channel].Port )
         {
            rv_tmp = Dio_GetSync_Arch( channel );
            /* rv_tmp can be IO_HIGH or IO_LOW */

            if( IO_HIGH == rv_tmp )
            {
               rv |= Dio_Config.Pins[channel].PinMask;
            }
         }

#if( ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED )
         else
         {
#ifdef HISIO_DIO_ENABLE_ERRORHOOK
            //call the error hook with IO_E_INVALID_CHANNEL_ID
#endif
            rv = 0;  /* the standard doesn't say anything about the return value in case or error.*/
         }
#endif
      }
   }
   return rv;
}

extern void Dio_SetPortSync_Arch(IO_ChannelType port, IO_ValueType value)
{
   IO_ValueType rv;
   IO_ValueType rv_tmp;
   uint8_t channel;

#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   /* check if the requested port is configured */
   if( DIO_PORTS_COUNT>port )
#endif
   {
      for( channel=0; channel<DIO_PINS_COUNT; channel++ )
      {
         /* validate that the port requested is configured within any DIO channel*/
         if( Dio_Config.Ports[port].Port == Dio_Config.Pins[channel].Port  )
         {
            rv_tmp = value&Dio_Config.Pins[channel].PinMask;

            if( Dio_Config.Pins[channel].Flags & (DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW | DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH) )
            {
               Dio_SetSync_Arch( channel , rv_tmp?IO_HIGH:IO_LOW    );
            }
         }
      }
   }
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   else
   {
#ifdef HISIO_DIO_ENABLE_ERRORHOOK
      //call the error hook with IO_E_INVALID_CHANNEL_ID
#endif
      rv = 0;  /* the standard doesn't say anything about the return value in case or error.*/
   }
#endif
}

/*
This funtion sets a complete DIO port in one step. Only pins specified in mask (configured as output) will be set.
The port functions always use the configuration settings of the assigned channel
Implementation for MSP430
Ref: API IO Driver v 2.1.3
     5.2.2.9
*/
extern void Dio_SetPortMaskedSync_Arch(IO_ChannelType port, IO_ValueType value, IO_ValueType mask)
{
   Dio_SetPortSync_Arch( port , value&mask );
}

/*
support function for Dio_EnableNotification_Arch and Dio_DisableNotification_Arch
*/
extern void Dio_ChangeNotification_Arch( IO_ChannelType channel, IO_ValueType notifType , IO_ValueType enable )
{
#if (ERROR_CHECKING_TYPE == ERROR_CHECKING_EXTENDED)
   if(DIO_PINS_COUNT > channel && (Dio_Config.Pins[channel].Flags & (DIO_CONFIG_PIN_DIRECTION_INPUT | DIO_CONFIG_PIN_ENABLE_NOTIFICATION)))
#endif
   {
      if( notifType == IO_N_RISING_EDGE )
      {
         GPIO_selectInterruptEdge( Dio_Config.Pins[channel].Port ,  Dio_Config.Pins[channel].PinMask ,  GPIO_LOW_TO_HIGH_TRANSITION );
      }
      if( notifType == IO_N_FALLING_EDGE )
      {
         GPIO_selectInterruptEdge( Dio_Config.Pins[channel].Port ,  Dio_Config.Pins[channel].PinMask ,  GPIO_HIGH_TO_LOW_TRANSITION );
      }
      if( notifType == IO_N_ALL )
      {
         //msp430 hasn't the ability of detecting both directions
         //report the error code IO_E_INVALID_NOTIF_TYPE
         return;
      }

      if( enable )
      {
         GPIO_clearInterrupt( Dio_Config.Pins[channel].Port ,  Dio_Config.Pins[channel].PinMask );
         GPIO_enableInterrupt( Dio_Config.Pins[channel].Port , Dio_Config.Pins[channel].PinMask );
      }
      else
      {
         GPIO_disableInterrupt( Dio_Config.Pins[channel].Port , Dio_Config.Pins[channel].PinMask );
      }
   }
   else
   {
#ifdef HISIO_DIO_ENABLE_ERRORHOOK
      //call the error hook with   IO_E_INVALID_CHANNEL_ID
#endif
   }
}

/*
Implementation for MSP430
Ref: API IO Driver v 2.1.3
     5.2.3.1.1
*/
extern void Dio_EnableNotification_Arch( IO_ChannelType channel, IO_ValueType notifType )
{
   Dio_ChangeNotification_Arch( channel, notifType , 1 );
}

/*
Implementation for MSP430
Ref: API IO Driver v 2.1.3
     5.2.3.1.2
*/
extern void Dio_DisableNotification_Arch( IO_ChannelType channel, IO_ValueType notifType )
{
   Dio_ChangeNotification_Arch( channel, notifType , 0 );
}


/*
this function returns  the Dio_Config index (channel number), given the number of the port, and the number of the pin (0 to 7)
*/
extern unsigned char Dio_FindChannel_Arch( uint8_t  Port, uint8_t  PinNro  )
{
   unsigned char channel;
   for( channel=0 ; channel<DIO_PINS_COUNT ; channel++ )
   {
      if( Dio_Config.Pins[channel].Port == Port && Dio_Config.Pins[channel].PinMask == 1<<PinNro )
      {
         return channel;
      }
   }

   return channel; //it returns DIO_PINS_COUNT in case of error.
}

/*
returns the configured edge of the IRQ for a Port and pin nro
*/
extern IO_ValueType Dio_GetEdge_Arch( uint8_t  Port, uint8_t  PinNro  )
{
   if( GPIO_GetInterruptEdge( Port , PinNro ) == GPIO_HIGH_TO_LOW_TRANSITION )
   {
      return IO_N_FALLING_EDGE;
   }
   else
   {
      return IO_N_RISING_EDGE;
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2016, Franco Bucafusco
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

#ifndef DIO_CFG_ARCH_H
#define DIO_CFG_ARCH_H
/** \brief DIO Driver Configuration File Arch
 **
 ** This file contains the DIO Driver configuration Arch
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup HISIO HisIO Module
 ** @{ */


/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "msp430.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

<?php
   print "\n";
   print "/** \brief DIO Configuration\n";
   print " **\n";
   print " **/\n";

   #get global configurations for all the DIO drivers
   $DILS =  $this->config->getList("", "DIL");

   $dio_use_error_hook =  $this->config->getValue("/" . $DILS[0] , "ERRORHOOK");
   $dio_use_notificacion_callback = 0;

   if( $dio_use_error_hook== "TRUE")
   {
      print "#define HISIO_DIO_ERRORHOOK  HISIO_ENABLE\n\n";
   }
   else {
      print "#define HISIO_DIO_ERRORHOOK  HISIO_DISABLE\n\n";
   }

   #for each configuration
   $dios          = $this->config->getList( "/".$DILS[0] , "DIO" );
   $dios_count    = count($dios);

   print "/** \brief Dio Configurations count */\n";
   print "#define DIO_CONFIGS_COUNT " . $dios_count . "U\n\n";

   if( $dios_count == 0 )
   {
      print "#define DIO_PINS_COUNT    0U\n\n";
      print "#define DIO_PORTS_COUNT   0U\n\n";
   }
   else
   {
      print "/** \brief Dio Channel (pins) count */\n";

      foreach ($dios as $count=>$dio)
      {
         if ($count != 0)
         {
            $this->error("Maximal one configuration is supported.");
         }

         $pins =  $this->config->getList( "/".$DILS[0]."/".$dio , "PIN" );
         print "#define DIO_PINS_COUNT " . count($pins) . "U\n\n";

         print "/* CHANNEL SETTINGS FOR DIO CONFIGURATION ($dio)*/\n\n";

         foreach($pins as $count=>$pin)
         {
            $pin_port         = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "PORT");
            $pin_pin          = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "PIN");
            $pin_notification = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "NOTIFICATION");

            if( $pin_notification=="TRUE")
               $dio_use_notificacion_callback = $pin_notification;

            print "/** \brief Port: " . $pin_port . " Pin: " . $pin_pin . " called " . $pin . " */\n";
            print "#define " . $pin . " " . $count . "\n";
         }

         $ports =  $this->config->getList("/".$DILS[0]."/" . $dio, "PORT");
         print "\n";
         print "/** \brief Dio Ports count */\n";
         print "#define DIO_PORTS_COUNT " . count($ports) . "U\n\n";
      }
   }
?>

/*==================[typedef]================================================*/
typedef struct
{
   uint8_t  Port;    /* number of port */
   uint8_t  PinMask; /* mask within a port */
   uint16_t Flags;   /* Inverted, Direction, I/O, etc . Its defined as 16 bit value for aligment issues (faster and less code) */
} Dio_PinConfigType;

typedef struct
{
   uint8_t  Port;
   //uint8_t  Size; /* 8, 16, 32 */
   //uint16_t Mask;
   //uint32_t Flags; /* Inverted, Direction, I/O, etc */
} Dio_PortConfigType;

typedef struct
{
   Dio_PinConfigType    Pins[DIO_PINS_COUNT];
   Dio_PortConfigType   Ports[DIO_PORTS_COUNT];
} Dio_ConfigType;

/*==================[external data declaration]==============================*/
/** \brief Dio Config struct
 **
 ** Contents the Dio Config settings
 **/
<?php
foreach ($dios as $count=>$dio)
{
   print "/** \brief Configration of Dio Driver: " . $dio . " */\n";
   print "extern const Dio_ConfigType Dio_Config;\n";
}
?>

/*==================[external functions declaration]=========================*/
<?php
if( $dio_use_error_hook== "TRUE")
{
   /* */
   print "void Dio_ErrorHook(IO_DeviceType device, IO_ErrorType error);\n";
}


if( $dio_use_notificacion_callback== "TRUE")
{
   /* */
   print "void Dio_Notification(IO_ChannelType channel, IO_ValueType notifType);\n";
}
?>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef DIO_CFG_ARCH_H */

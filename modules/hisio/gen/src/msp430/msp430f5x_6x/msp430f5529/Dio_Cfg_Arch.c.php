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

/** \brief DIO Driver Configuration File
 **
 ** This file contains the DIO Driver configuration
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup HISIO HisIO Module
 ** @{ */


/*==================[inclusions]=============================================*/
#include "os.h"
#include "IODriver_Base.h"
#include "Dio_Cfg.h"
#include "Dio_Cfg_Arch.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data declaration]==============================*/

/*==================[external data definition]==============================*/

<?php

#require_once './modules/hisio/gen/helpers/cortexM4/lpc43xx/lpc4337/hisio.php';
#$MyHisio = new Hisio();
$this->loadHelper("modules/rtos/gen/ginc/Platform.php");
$intList = $this->helper->platform->getInterruptHandlerList();

#get global configurations for all the DIO drivers
$DILS =  $this->config->getList("", "DIL");
$dio_use_error_hook =  $this->config->getValue("/" . $DILS[0] , "ERRORHOOK");

if( $dio_use_error_hook== "TRUE")
{
   print "/* the user must provide the implementation of \n\n";
   print "   void Dio_ErrorHook(IO_DeviceType device, IO_ErrorType error)\n\n";
   print "   callback*/\n\n";
}

$dios =  $this->config->getList( "/".$DILS[0] , "DIO" );
$dios_count = count($dios);

foreach ($dios as $count=>$dio)
{
   if ($dios_count>1)
   {
      $this->log->error("Maximal one configuration is supported.");
   }

   $pins =  $this->config->getList("/".$DILS[0]."/" . $dio, "PIN");

   $port_array_wnotification = array(); #this array will hold the port number for the pins that has the NOTIFICATION property in TRUE

   print "const Dio_ConfigType Dio_Config = \n";
   print "{\n";
   print "\n   /* Channels (Pins) Settings */\n";
   print "   {\n";

   foreach($pins as $count=>$pin)
   {
      #print "pins " . $pin ."\n";
      $pin_port         = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "PORT");
      $pin_pin          = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "PIN");
      $pin_direction    = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "DIRECTION");
      $pin_notification = $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "NOTIFICATION");

      switch ($pin_direction)
      {
         case "IO_INPUT":
            $pin_flags = "DIO_CONFIG_PIN_DIRECTION_INPUT";
            break;
         case "IO_OUTPUT_INIT_LOW":
            $pin_flags = "DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW";
            break;
         case "IO_OUTPUT_INIT_HIGH":
            $pin_flags = "DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH";
            break;
         default:
           $this->log->error("The pin $pin hasn't a defined direction!");
            break;
      }

      $pin_inverted =  $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $pin, "INVERTED");

      switch ($pin_inverted)
      {
         case "TRUE":
            $pin_flags = $pin_flags . " | DIO_CONFIG_PIN_INVERTED";
            break;
         case "FALSE":
            break;
         default:
            $this->log->error("The pin $pin hasn't a defined 'inverted' configuration!");
            break;
      }

      switch ($pin_notification)
      {
         case "TRUE":
            if( $pin_direction=="IO_INPUT" )
            {
               $pin_flags = $pin_flags . " | DIO_CONFIG_PIN_ENABLE_NOTIFICATION";

               #we add the port to the array that later has to implement the gpio interrupt handler
               $port_array_wnotification[$pin] =   "PORT" . $pin_port ;

            #   print "port ".$pin_port. "\n";
            #   print "arr" . $port_array_wnotification[$pin_port] . "\n";
            }
            else
            {
               $this->log->error("The pin $pin could allow notifications if configured as input.");
            }
            break;
         case "FALSE":
            break;
         default:
            break;
      }

      print "      /** \brief Port: " . $pin_port . " Pin: " . $pin_pin . " , called " . $pin . " */\n";
      print "      { 0x" . $pin_port . " , 1<<" .$pin_pin . " , (" . $pin_flags . " )},\n";
   }


#print_r(array_values($port_array_wnotification));
   #removing duplicates (ie: PORT1, PORT1, PORT2 ---> PORT1, PORT2 )
   $port_array_wnotification=array_unique($port_array_wnotification);

   print "   },\n";

   $ports =  $this->config->getList("/".$DILS[0]."/" . $dio, "PORT");

   print "\n   /* Port Settings */\n";
   print "   {\n";
   foreach($ports as $count=>$port)
   {
      $port_port =  $this->config->getValue("/".$DILS[0]."/" . $dio . "/" . $port, "PORT");

      print "      /** \brief Port: " . $port_port . " , called " . $port . " */\n";
      print "      { " . $port_port . " },\n";
   }
   print "   }\n";


   //print ", 0 /* foo var */\n";
   print "};\n";

   print "\n";

   #CREATION OF ALL IRQ VECTORS FOR ALL IO NOTIFICATIONS
   foreach($port_array_wnotification as $channel=>$port)
   {
      #msp430 HAS ONLY GPIO INTERRUPT CAPABILITY IN PORT 1 AND 2.
      #TODO: validate this when analizing the NOTIFICATION property of each channel

      $key = array_search($port, $intList);

//      if( $port == "PORT1")
   //   {

 //print "".$key ."\n";

      $port_N = 0;

      if( $port=="PORT1")
      {
        $port_N = 1;
      }
      if( $port=="PORT2")
      {
        $port_N = 2;
      }

      if( $port_N > 0 )
      {
         print "ISR(".$port.")\n";//_VECTOR
         print "{\n";
         print "   uint16_t interrupt_source = GetPendingIRQ_Arch( ".$key." );\n";

         print "   uint8_t channel = Dio_FindChannel_Arch( ".$port_N." , interrupt_source  );\n";

         print "   Dio_Notification( channel ,  Dio_GetEdge_Arch( ".$port_N." , interrupt_source  ) );\n";
         print "}\n\n";
      }
      else
      {
         //TODO: this should be rised as an generation error.
         //print "#error MSP430 has only 2 IO port with interrupt capability. Remove NOTIFICATION=TRUE from Input PINS assigned to $port from DIL file";
         trigger_error("===== DIL ERROR: MSP430 has only 2 IO port with interrupt capability. Remove NOTIFICATION=TRUE from Input PINS assigned to ".$port." from DIL file\n", E_USER_ERROR);
      }
   }
}
?>

/*==================[external functions declaration]=========================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

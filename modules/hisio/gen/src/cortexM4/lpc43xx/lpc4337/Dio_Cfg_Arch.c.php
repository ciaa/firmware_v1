/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2016, Juan Cecconi
 * Copyright 2016, Juan Manuel Cruz
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

/*
 * Initials     Name
 * ---------------------------
 * JuCe         Juan Cecconi
 * JMC          Juan Manuel Cruz
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160110 v0.0.1 JuCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Dio_Cfg.h"
#include "Dio_Cfg_Arch.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data declaration]==============================*/

/*==================[external data definition]==============================*/

<?php

require_once './modules/hisio/gen/helpers/cortexM4/lpc43xx/lpc4337/hisio.php';
$MyHisio = new Hisio;

$dios = $config->getList("/DIL", "DIO");
foreach ($dios as $count=>$dio) {
   if ($count != 0) {
      $this->log->error("Maximal one configuration is supported.");
   }
   $pins = $config->getList("/DIL/" . $dio, "PIN");

   print "const Dio_ConfigType Dio_Config = {\n";
   print "{\n";
   foreach($pins as $count=>$pin) {
      $pin_port = $config->getValue("/DIL/" . $dio . "/" . $pin, "PORT");
      $pin_pin = $config->getValue("/DIL/" . $dio . "/" . $pin, "PIN");
      $GPIO = array_search("P" . $pin_port . "_" . $pin_pin, $MyHisio->dio_pins, FALSE);
      if ($GPIO == NULL) {
         $this->log->error("PIN " . $pin . " has a 'Port-Pin' pair that doesn't match a valid GPIO.");
      }
      sscanf($GPIO, "GPIO%d[%d], FUNC%d", $GPIO_Port, $GPIO_Pin, $GPIO_Func);

      $pin_direction = $config->getValue("/DIL/" . $dio . "/" . $pin, "DIRECTION");
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
      $pin_inverted = $config->getValue("/DIL/" . $dio . "/" . $pin, "INVERTED");
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
      print "/** \brief Port: " . $pin_port . " Pin: " . $pin_pin . " , " . $GPIO . " called " . $pin . " */\n";
      print "{ 0x" . $pin_port . ", " . $pin_pin . ", 0x" . $GPIO_Port . ", " . $GPIO_Pin . ", SCU_MODE_FUNC" . $GPIO_Func . ", (" . $pin_flags . ")},\n";
   }
   print "},\n";

   $ports = $config->getList("/DIL/" . $dio, "PORT");

   print "{\n";
   foreach($ports as $count=>$port) {
      $port_port = $config->getValue("/DIL/" . $dio . "/" . $port, "PORT");
      $port_mask = $config->getValue("/DIL/" . $dio . "/" . $port, "MASK");

      $port_size = $config->getValue("/DIL/" . $dio . "/" . $port, "SIZE");
      switch ($port_size)
      {
         case "IO_PORT_SIZE_8":
            $port_size = "8";
            $port_flags = "DIO_CONFIG_PORT_SIZE_8";
            break;         
         case "IO_PORT_SIZE_16":
            $port_size = "16";
            $port_flags = "DIO_CONFIG_PORT_SIZE_16";
            break;
         case "IO_PORT_SIZE_32":
            $port_size = "32";
            $port_flags = "DIO_CONFIG_PORT_SIZE_32";           
            break;
         default:
           $this->log->error("The port $port hasn't a defined size!");
            break;
      }
      $port_direction = $config->getValue("/DIL/" . $dio . "/" . $port, "DIRECTION");
      switch ($port_direction)
      {
         case "IO_INPUT":
            $port_flags = $port_flags . " | DIO_CONFIG_PORT_DIRECTION_INPUT";
            break;         
         case "IO_OUTPUT_INIT_LOW":
            $port_flags = $port_flags . " | DIO_CONFIG_PORT_DIRECTION_OUTPUT_INIT_LOW";
            break;
         case "IO_OUTPUT_INIT_HIGH":
            $port_flags = $port_flags . " | DIO_CONFIG_PORT_DIRECTION_OUTPUT_INIT_HIGH";
            break;
         default:
           $this->log->error("The port $port hasn't a defined direction!");
            break;
      }
      $port_inverted = $config->getValue("/DIL/" . $dio . "/" . $port, "INVERTED");
      switch ($port_inverted)
      {
         case "TRUE":
            $port_flags = $port_flags . " | DIO_CONFIG_PORT_INVERTED";
            break;
         case "FALSE":
            break;
         default:
            $this->log->error("The port $port hasn't a defined 'inverted' configuration!");
            break;
      } 
      print "/** \brief Port: " . $port_port . " , size: " . $port_size . " , maks: 0x" . $port_mask . " , called " . $port . " */\n";
      print "{ " . $port_port . ", " . $port_size . ", 0x" . $port_mask . ", ("  . $port_flags . ")},\n";
   }
   print "}\n";

   print ", 0 /* foo var */\n";   
   print "};\n";
}    
?>

/*==================[external functions declaration]=========================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


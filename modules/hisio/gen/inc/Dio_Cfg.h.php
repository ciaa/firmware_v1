/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

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

#ifndef DIO_CFG_H
#define DIO_CFG_H
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
 * MaCe         Mariano Cerdeiro
 * JuCe         Juan Cecconi 
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20151222 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
#define DIO_CONFIG_PIN_DIRECTION_UNUSED                   0x00000001UL
#define DIO_CONFIG_PIN_DIRECTION_INPUT                    0x00000002UL
#define DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_LOW          0x00000004UL
#define DIO_CONFIG_PIN_DIRECTION_OUTPUT_INIT_HIGH         0x00000008UL
#define DIO_CONFIG_PIN_INVERTED                           0x00000010UL

<?php
$dios = $config->getList("/DIL", "DIO");
foreach ($dios as $count=>$dio) {
   if ($count != 0) {
      $this->error("Maximal one configuration is supported.");
   }
?>
/** \brief DIO Configuration pointer
 **
 ** \remarks At the moment only one configuration is supported, so the
 **          parameter is set to null pointer.
 **
 **/
<?php
   $pins = $config->getList("/DIL/" . $dio, "PIN");

   foreach($pins as $count=>$pin) {
      $pin_port = $config->getValue("/DIL/" . $dio . "/" . $pin, "PORT");
      $pin_pin = $config->getValue("/DIL/" . $dio . "/" . $pin, "PIN");
      print "/** \brief Port: " . $pin_port . " Pin: " . $pin_pin . " called " . $pin . " */\n";
      print "#define " . $pin . " " . $count . "\n";
   }
   print "\n";   
   print "/** \brief Dio Pins count */\n";
   print "#define DIO_PINS_COUNT " . count($pins) . "U\n\n";

   $ports = $config->getList("/DIL/" . $dio, "PORT");

   foreach($ports as $count=>$port) {
      $port_port = $config->getValue("/DIL/" . $dio . "/" . $port, "PORT");
      print "/** \brief Port: " . $port_port. " called " . $port . " */\n";
      print "#define " . $port . " " . $count . "\n";
   }
}
?>

/*==================[typedef]================================================*/
typedef struct {
   uint8_t Port;
   uint8_t Pin;
   uint8_t GPIO_Port;
   uint8_t GPIO_Pin;   
   uint32_t Flags; /* Inverted, Direction, I/O, etc */
} Dio_PinConfigType;

typedef struct {
   Dio_PinConfigType Pins[DIO_PINS_COUNT];
   uint8_t foo;
} Dio_ConfigType;

/*==================[external data declaration]==============================*/
/** \brief Dio Config struct
 **
 ** Contents the Dio Config settings
 **/
<?php
foreach ($dios as $count=>$dio) {
   print "/** \brief Configration of Dio Driver: " . $dio . " */\n";
   print "extern const Dio_ConfigType Dio_Config;\n";
}
?>

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef DIO_CFG_H */


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
 * 20151222 v0.0.1 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdint.h"
#include "chip.h"

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
   print "/** \brief Dio Pins count */\n";
   $dios = $config->getList("/DIL", "DIO");
   if(count($dios) == 0){
         print "#define DIO_PINS_COUNT 0U\n\n";      
         print "#define DIO_PORTS_COUNT 0U\n\n";      
   }
   else{
      foreach ($dios as $count=>$dio) {
         if ($count != 0) {
            $this->error("Maximal one configuration is supported.");
         }
         $pins = $config->getList("/DIL/" . $dio, "PIN");
         print "#define DIO_PINS_COUNT " . count($pins) . "U\n\n";
        
         foreach($pins as $count=>$pin) {
            $pin_port = $config->getValue("/DIL/" . $dio . "/" . $pin, "PORT");
            $pin_pin = $config->getValue("/DIL/" . $dio . "/" . $pin, "PIN");
            print "/** \brief Port: " . $pin_port . " Pin: " . $pin_pin . " called " . $pin . " */\n";
            print "#define " . $pin . " " . $count . "\n";
         }
         
         $ports = $config->getList("/DIL/" . $dio, "PORT");
         print "\n";
         print "/** \brief Dio Ports count */\n";
         print "#define DIO_PORTS_COUNT " . count($ports) . "U\n\n";

         foreach($ports as $count=>$port) {
            $port_port = $config->getValue("/DIL/" . $dio . "/" . $port, "PORT");
            $port_size = $config->getValue("/DIL/" . $dio . "/" . $port, "SIZE");
            print "/** \brief Port: " . $port_port. " called " . $port . " */\n";
            print "#define " . $port . " " . $count . "\n";
         }
      }
   }
?>

/*==================[typedef]================================================*/
typedef struct {
   uint8_t Port;
   uint8_t Pin;
   uint8_t GPIO_Port;
   uint8_t GPIO_Pin;
   uint8_t GPIO_Func;
   uint32_t Flags; /* Inverted, Direction, I/O, etc */
} Dio_PinConfigType;

typedef struct {
   uint8_t Port;
   uint8_t Size; /* 8, 16, 32 */
   uint32_t Mask;
   uint32_t Flags; /* Inverted, Direction, I/O, etc */
} Dio_PortConfigType;

typedef struct {
   Dio_PinConfigType Pins[DIO_PINS_COUNT];
   Dio_PortConfigType Ports[DIO_PORTS_COUNT];
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
#endif /* #ifndef DIO_CFG_ARCH_H */


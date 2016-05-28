<?php
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

class FunctionalityBase {
   const DIO = 1;
   const ADC = 2;
   const DAC = 4;
   const PORT = 8;

   var $resources = array();

   protected $type = null;

   function __construct($type) {
      $this->type = $type;
   }

   function getType() {
      return $this->type;
   }

   function addResource($res) {
      $this->resources[] = $res;
   }

   function useResource($res) {
      foreach ($this->resources as $r) {
         if ($r == $res)
            return true;
      }
      return false;
   }
}

class PinBase {
   protected $port = null;
   protected $pin = null;

   function __contruct($port, $pin) {
      $this->port = $port;
      $this->pin = $pin;
   }

}

class HisioBase {
   var $elements = array();

   function __construct() {
      $this->init();
   }

   function add($element) {
      $this->elements[] = $element;
   }

   function getByTypeAndResource($type, $res) {
      foreach($this->elements as $el) {
         if ($el->getType() == $type) {
            if ($el->useResource($res))
               return $el;
         }
      }

      return null;
   }
}

class Pin extends PinBase {
}

class Port extends FunctionalityBase {
   var $port = null;

   function __construct($port, $func) {
      parent::__construct(Port::PORT);
      $this->port= $port;
      $this->func = $func;
   }
}

class Dio extends FunctionalityBase {
   var $gpio_port = null;
   var $gpio_pin = null;
   var $func = null;

   function __construct($gpio_port, $gpio_pin, $func) {
      parent::__construct(Dio::DIO);
      $this->gpio_port = $gpio_port;
      $this->gpio_pin = $gpio_pin;
      $this->func = $func;
   }

   function getGpioPort() {
      return $this->gpio_port;
   }

   function getGpioPin() {
      return $this->gpio_pin;
   }

   function getFunc() {
      return $this->func;
   }
}

class Hisio extends HisioBase {

   function init() {
      $GPIO0_0 = new Dio(0, 0, "FUNC0");
      $GPIO0_0->addResource(new Pin(0, 0));
      $this->add($GPIO0_0);

      $GPIO0_1 = new Dio(0, 1, "FUNC0");
      $GPIO0_1->addResource(new Pin(0, 1));
      $this->add($GPIO0_1);

      $GPIO0_2 = new Dio(0, 2, "FUNC0");
      $GPIO0_2->addResource(new Pin(1, 15));
      $this->add($GPIO0_2);

      $GPIO0 = new Port(0, "FUNC0");
      $GPIO0->addResource(new Pin(0,0));
      $GPIO0->addResource(new Pin(0,1));
      $GPIO0->addResource(new Pin(1,15));
      $this->add($GPIO0);
   }

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


<?php
/* Copyright 2015, Carlos Pantelides
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

/** \brief FreeOSEK Generator Oil Config Implementation Test file
 **
 ** This file implements the Oil Config Implementation Test
 **
 ** \file OilConfigTest.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*==================[inclusions]=============================================*/
require_once(dirname(__FILE__) . '/../../OilConfig.php');
/*==================[class definition]=======================================*/
/** \brief Oil Config Test Class Implementation
 **
 ** This class implements the Oil Config Test Class
 **
 **/
class OilConfigTest extends PHPUnit_Framework_TestCase
{
   private $config=array(
      array(
         "root" => "/OSEK/ExampleOS",
         "type" => "USERESSCHEDULER",
         "value"=> false
      ),
      array(
         "root" => "/OSEK/ExampleOS",
         "type" => "MEMMAP",
         "value"=> false
      ),
      array(
         "root" => "/OSEK",
         "type" => "RESOURCE",
         "value"=> "POSIX"
      ),
      array(
         "root" => "/OSEK",
         "type" => "COUNTER",
         "value"=> "HardwareCounter"
      )
   );
   public function getListProvider()
   {
      return array(
         array(array(), array("/OSEKO","*"),"//1"),
         array(array("POSIX","HardwareCounter"),array("/OSEK","*"),"//2"),
      );
   }

   /**
   * @dataProvider getListProvider
   *
   */
   public function testListValue($expected, $data, $msg)
   {
      $config = new OilConfig();
      $config->setConfig($this->config);
      $this->assertEquals($expected, $config->getList($data[0],$data[1]) ,$msg);
   }

   public function getValueProvider()
   {
      return array(
         array("POSIX",array("/OSEK","RESOURCE"),"//1"),
         array(false,array("/OSEK/ExampleOS","MEMMAP"),"//2"),
      );
   }

   /**
   * @dataProvider getValueProvider
   *
   */
   public function testGetValue($expected, $data, $msg)
   {
      $config = new OilConfig();
      $config->setConfig($this->config);
      $this->assertEquals($expected, $config->getValue($data[0],$data[1]) ,$msg);
   }
   public function getCountProvider()
   {
      return array(
         array(1,array("/OSEK","RESOURCE"),"//1"),
         array(2,array("/OSEK","*"),"//2"),
         array(2,array("/OSEK/ExampleOS","*"),"//3"),
      );
   }

   /**
   * @dataProvider getCountProvider
   *
   */
   public function testGetCount($expected, $data, $msg)
   {
      $config = new OilConfig();
      $config->setConfig($this->config);
      $this->assertEquals($expected, $config->getCount($data[0],$data[1]) ,$msg);
   }

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
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

/** \brief FreeOSEK Generator Oil Generator Implementation Test file
 **
 ** This file implements the Oil Generator Implementation Test
 **
 ** \file oilGeneratorTest.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*==================[inclusions]=============================================*/
require_once(dirname(__FILE__) . '/../../OilGenerator.php');
require_once(dirname(__FILE__) . '/../../StdoutWriter.php');
require_once(dirname(__FILE__) . '/../../FileWriter.php');

/*==================[class definition]=======================================*/
/** \brief Oil Generator Test Class Implementation
 **
 ** This class implements the Oil Generator Test Class
 **
 **/
class OilGeneratorTest extends PHPUnit_Framework_TestCase
{
   public function compareFilesProvider()
   {
      return array(
         array('fileA.txt','fileNotLikeA.txt', false, '//1'),
         array('fileA.txt','fileLikeA.txt', true, '//2'),
      );
   }


   /**
   * @dataProvider compareFilesProvider
   *
   */
   public function testCompareFiles($f1, $f2, $expected, $msg)
   {
      $f1=dirname(__FILE__). "/fixtures/$f1";
      $f2=dirname(__FILE__). "/fixtures/$f2";
      echo "checking diff $f1 $f2 for $expected\n";
      $writer = new StdoutWriter();
      $generator = new OilGenerator($writer);
      $this->assertEquals($expected, $generator->compareFiles($f1,$f2), $msg);

   }

   public function testWarning() {
      $writer = new FileWriter();
      $generator = new OilGenerator($writer);
;

   }

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
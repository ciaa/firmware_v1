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

/** \brief FreeOSEK FileWirter Implementation Test file
 **
 ** This file implements the FileWriter Implementation Test
 **
 ** \file FileWriterTest.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*==================[inclusions]=============================================*/
require_once(dirname(__FILE__) . '/../../FileWriter.php');
/*==================[class definition]=======================================*/
/** \brief FileWriter Test Class Implementation
 **
 ** This class implements the FileWriter Test Class
 **
 **/
class FileWriterTest extends PHPUnit_Framework_TestCase
{
public function OutputFileNameProvider()
   {
      return array(

         array(
            '/modules/out/inc/Os_Internal_Cfg.h',
            array(
               'template' => '/modules/gen/inc/Os_Internal_Cfg.h,php',
               'outdir' => '/modules/out',
               'relativeBase' => '/gen/'
            ),
            '// 1'),
         array(
            '/modules/out/inc/Os_Internal_Cfg.h',
            array(
               'template' => '/modules/tmp/inc/Os_Internal_Cfg.h,php',
               'outdir' => '/modules/out',
               'relativeBase' => '/tmp/'
            ),
            '// 2'),
         array(
            '/modules/out/inc/Os_Internal_Cfg.h',
            array(
               'template' => '/modules/templates/inc/Os_Internal_Cfg.h,php',
               'outdir' => '/modules/out',
               'relativeBase' => '/templates/'
            ),
            '// 3'),
         array(
            '/home/ciaa/Firmware/modules/rtos/generator/tests/ftest/tmp/inc/Os_Cfg.h',
            array(
               'template' => '/home/ciaa/Firmware/modules/rtos/generator/tests/ftest/fixtures/templates/inc/Os_Cfg.h.php',
               'outdir' => '/home/ciaa/Firmware/modules/rtos/generator/tests/ftest/tmp',
               'relativeBase' => '/templates/'
            ),
            '// 4'),
      );
   }
   /**
   * @dataProvider OutputFileNameProvider
   */
   public function testOutputFileName($expected, $data, $msg)
   {

      $fileWriter = new FileWriter();
      $got = $fileWriter->outputFileName($data['template'], $data['outdir'], $data['relativeBase']);
      $this->assertEquals($expected, $got ,$msg);

   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
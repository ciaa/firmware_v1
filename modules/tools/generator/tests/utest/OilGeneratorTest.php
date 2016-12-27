<?php
/* Copyright 2015, 2016 Carlos Pantelides
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
require_once(dirname(__FILE__) . '/../../NullWriter.php');
require_once(dirname(__FILE__) . '/../../FileWriter.php');
require_once(dirname(__FILE__) . '/../../NullWriter.php');
require_once(dirname(__FILE__) . '/../../BufferWriter.php');

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
      $writer = new NullWriter();
      $generator = new OilGenerator($writer);
      $this->assertEquals($expected, $generator->compareFiles($f1,$f2), $msg);

   }

//    implement soon or delete
//    public function testWarning() {
//       $writer = new FileWriter();
//       $generator = new OilGenerator($writer);
// ;
//
//    }


   public function testGetNames()
   {
      $generator = new OilGenerator(new NullWriter());
      $this->assertEquals(
         array("dummyHelper","DummyHelper"),
         $generator->getNames('modules/rtos/generator/tests/ftest/fixtures/gen/DummyHelper.php')
      );
   }

   public function testProcessArgs_Ok()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-c','mock.oil','-o','mockdir','-t','mocktemplate'));
   }

   public function testProcessArgs_MultiOk()
   {
      $og = new OilGenerator(new NullWriter());
      $expected = array(
         false,
         array(),
         array('mock.oil','mock2.oil'),
         'mockdir',
         array('mocktemplate','mocktemplate2'),
         '/gen/',
         array('helper1.php','helper2.php')
      );
      $args = $og->processArgs(array('-c','mock.oil','mock2.oil','-o','mockdir','-t','mocktemplate','mocktemplate2','-H','helper1.php','helper2.php'));
      $this->assertEquals($expected,$args);
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testProcessArgs_bad_multi_dir()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-c','mock.oil','-o','mockdir','mockdir2','-t','mocktemplate'));
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testProcessArgs_missing_config()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-o','mockdir','-t','mocktemplate'));
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testProcessArgs_missing_config_value()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-c','-o','mockdir','-t','mocktemplate'));
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testProcessArgs_missing_output_dir()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-c','mock.oil','-t','mocktemplate'));
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testProcessArgs_missing_helper_value()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-c','mock.oil','-o','mockdir','-H', '-t','mocktemplate'));
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testProcessArgs_missing_template()
   {
      $og = new OilGenerator(new NullWriter());
      $og->processArgs(array('-c','mock.oil','-o','mockdir','-t'));
   }

   /**
   * expectedException OilGeneratorException
   */
   public function testCheckFileOrFail_ok()
   {
      $og = new OilGenerator(new NullWriter());
      $dir = dirname(__FILE__). '/../../../../../out';
      $file = dirname(__FILE__). '/fixtures/fileA.txt';

      $configFiles = array($file);
      $baseOutDir = $dir;
      $templateFiles =  array($file);
      $helperFiles = array($file);

      $og->checkFilesOrFail($configFiles, $baseOutDir, $templateFiles, $helperFiles);
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testCheckFileOrFail_bad_config()
   {
      $og = new OilGenerator(new NullWriter());
      $dir = dirname(__FILE__). '/../../../../../out';
      $file = dirname(__FILE__). '/fixtures/fileA.txt';
      $fileNoExists = dirname(__FILE__). '/fixtures/noExists';

      $configFiles = array($fileNoExists);
      $baseOutDir = $dir;
      $templateFiles =  array($file);
      $helperFiles = array($file);

      $og->checkFilesOrFail($configFiles, $baseOutDir, $templateFiles, $helperFiles);
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testCheckFileOrFail_bad_outdir()
   {
      $og = new OilGenerator(new NullWriter());
      $dir = dirname(__FILE__). '/../../../../../out';
      $file = dirname(__FILE__). '/fixtures/fileA.txt';
      $dirNoExists = dirname(__FILE__). '/../../../../../noout';

      $configFiles = array($file);
      $baseOutDir = $dirNoExists;
      $templateFiles =  array($file);
      $helperFiles = array($file);

      $og->checkFilesOrFail($configFiles, $baseOutDir, $templateFiles, $helperFiles);
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testCheckFileOrFail_bad_template()
   {
      $og = new OilGenerator(new NullWriter());
      $dir = dirname(__FILE__). '/../../../../../out';
      $file = dirname(__FILE__). '/fixtures/fileA.txt';
      $fileNoExists = dirname(__FILE__). '/fixtures/noExists';

      $configFiles = array($file);
      $baseOutDir = $dir;
      $templateFiles =  array($fileNoExists);
      $helperFiles = array($file);

      $og->checkFilesOrFail($configFiles, $baseOutDir, $templateFiles, $helperFiles);
   }

   /**
   * @expectedException OilGeneratorException
   */
   public function testCheckFileOrFail_bad_helper()
   {
      $og = new OilGenerator(new NullWriter());
      $dir = dirname(__FILE__). '/../../../../../out';
      $file = dirname(__FILE__). '/fixtures/fileA.txt';
      $fileNoExists = dirname(__FILE__). '/fixtures/noExists';

      $configFiles = array($file);
      $baseOutDir = $dir;
      $templateFiles =  array($file);
      $helperFiles = array($fileNoExists);

      $og->checkFilesOrFail($configFiles, $baseOutDir, $templateFiles, $helperFiles);
   }

   public function testCheckFileNaming_NoWarnings()
   {
      $bw = new BufferWriter($buffer);
      $og = new OilGenerator($bw);

      $configFiles = array('c1.oil','c2.oilx','c3.OIL','c4.OILX');
      $templateFiles = array('t.h.php','t.c.php','t.C.PHP','t.H.PHP');
      $helperFiles = array('h1.php','h2.PHP');
      $og->checkFileNaming($configFiles, $templateFiles, $helperFiles);
      $this->assertEquals(0, count($bw->getBuffer()));
   }

   public function testCheckFileNaming_configWarnings()
   {
      $bw = new BufferWriter($buffer);
      $og = new OilGenerator($bw);

      $configFiles = array('c2.txt');
      $templateFiles = array();
      $helperFiles = array();
      $og->checkFileNaming($configFiles, $templateFiles, $helperFiles);
      $this->assertEquals(1, count($bw->getBuffer()));
   }

   public function testCheckFileNaming_templateWarnings()
   {
      $bw = new BufferWriter($buffer);
      $og = new OilGenerator($bw);

      $configFiles = array();
      $templateFiles = array('t.xxC.PHP');
      $helperFiles = array();
      $og->checkFileNaming($configFiles, $templateFiles, $helperFiles);
      $this->assertEquals(1, count($bw->getBuffer()));
   }

   public function testCheckFileNaming_helperWarnings()
   {
      $bw = new BufferWriter($buffer);
      $og = new OilGenerator($bw);

      $configFiles = array();
      $templateFiles = array();
      $helperFiles = array('h1.bat','h2.PHP');
      $og->checkFileNaming($configFiles, $templateFiles, $helperFiles);
      $this->assertEquals(1, count($bw->getBuffer()));
   }

   public function testCheckFileNaming_MultipleWarnings()
   {
      $bw = new BufferWriter($buffer);
      $og = new OilGenerator($bw);

      $configFiles = array('c1.txt');
      $templateFiles = array('t.xxh.php','t.c');
      $helperFiles = array('h1.txt','h2.txt','h3.c','h4.h');
      $og->checkFileNaming($configFiles, $templateFiles, $helperFiles);
      $this->assertEquals(7, count($bw->getBuffer()));
   }

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

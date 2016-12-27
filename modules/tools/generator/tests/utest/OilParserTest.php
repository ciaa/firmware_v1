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

/** \brief FreeOSEK Generator Oil Parser Implementation Test file
 **
 ** This file implements the Oil Parser Implementation Test
 **
 ** \file OilParserTest.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*==================[inclusions]=============================================*/
require_once(dirname(__FILE__) . '/../../OilParser.php');
/*==================[class definition]=======================================*/
/** \brief Oil Parser Test Class Implementation
 **
 ** This class implements the Oil Parser Test Class
 **
 **/
class OilParserTest extends PHPUnit_Framework_TestCase
{
   /**
   * @expectedException Exception
   * @todo expect proper exception
   */
   public function testLoadFile()
   {
       $parser = new OilParser();
       $parser->loadFile("");
   }

   public function removeCommentsProvider()
   {
      return array(
      // // style
         array(array(''),array("//"),'// 1'),
         array(array(''),array('// comment'),'// 2'),
         array(array('code '),array("code // comment"),'// 3'),
         array(array('code '),array("code // comment // comment"),'// 4'),

         // /**/ style
         array(array('code '),array('code /* comment */'),'/**/ 1'),
         array(array(' code'),array('/* comment */ code'),'/**/ 2'),
         array(array('code  code'),array('code /* comment */ code'),'/**/ 3'),

         // mixed style
         array(array('code '),array("code // comment /* comment"),'mixed 1'),

         // multiline
         array(array('code '),array('code /*'),'multiline 1'),
         array(array('code */'),array('code */'),'multiline 2'),
         array(array('code1','','','code2'),array('code1','/* comment1', ' comment2*/','code2'),'multiline 3')
      );
   }
   /**
   * @dataProvider removeCommentsProvider
   *
   */
   public function testRemoveComments($expected, $data, $msg)
   {
      $parser = new OilParser();
      $parser->loadArray($data);
      $this->assertEquals($expected, $parser->removeComments() ,$msg);
   }

   public function removeMultiBlankProvider()
   {
      return array(
         // spaces
         array(' a ','  a  ','spaces 1'),
         array(' a','  a','spaces 2'),
         array('a ','a  ','spaces 3'),
         array(' a b ','  a b  ','spaces 4'),
         array(' a b ','  a  b  ','spaces 5'),

         // tabs
         array(' a ',"\t\ta\t\t",'tabs 1'),
         array(' a',"\t\ta",'tabs 2'),
         array('a ',"a\t\t",'tabs 3'),
         array(' a b ',"\t\ta\tb\t\t",'tabs 4'),
         array(' a b ',"\t\ta\t\tb\t\t",'tabs 5'),

         // mixed
         array(' a ',"\t a\t ",'mixed 1'),
         array(' a'," \ta",'mixed 2'),
         array('a ',"a \t",'mixed 3'),
         array(' a b '," \ta b\t ",'mixed 4'),
         array(' a b ',"\t a \tb \t",'mixed 5'),

         // final
         array('a b c d e',"a\t \t \t  \tb \t c d\t\t   \t  e",'final'),
      );
   }

   /**
   * @dataProvider removeMultiBlankProvider
   *
   */
   public function testRemoveMultiBlank($expected, $data, $msg)
   {
      $parser = new OilParser();
      $this->assertEquals($expected, $parser->removeMultiBlank($data), $msg);
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
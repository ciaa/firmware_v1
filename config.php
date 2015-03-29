<?php
/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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

/** \brief FreeOSEK Generator Read Configuration Class
 **
 ** This file implements the Configuration Class of the Generator
 **
 ** \file config.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20080713 v0.1.0 MaCe       - initial version
 */

/*==================[inclusions]=============================================*/
require_once("oilParser.php");

/*==================[class definition]=======================================*/
/** \brief configClass Configuration Class
 **
 ** This class implements the generation configuration class of FreeOSEK
 **
 **/
class configClass {
   protected $config = array();

   function configClass()
   {
   }

   function parseOilFile($file)
   {
      $parser = new oilParserClass($file);

      $tmp = $parser->getOil();

      foreach ($tmp as $element)
      {
         $this->config[] = $element;
      }

   }

   function parseAutosarFile()
   {
   }

   function getValue($root, $type)
   {
      $ret = false;
      foreach ($this->config as $element)
      {
         if ( ($element["root"] == $root) &&
            ($element["type"] == $type) )
         {
            $value = $element["value"];
            if(strpos($value, "\"")!==false)
            {
               /* remove quotation marks */
               $value = substr($value, 1, -1);
            }
            return $value;
         }
      }
      return $ret;
   }

   function getBase($db, $root)
   {
      $ret = array();

      foreach($db as $el)
      {
         if ( $el["root"] == $root )
         {
            $ret[] = $el;
         }
      }

      return $ret;
   }


   function getCount($root, $type)
   {
      $ret = 0;

      foreach ($this->config as $element)
      {
         if ( ( ($element["root"] == $root) && ($element["type"] == $type) ) ||
              ( ($element["root"] == $root) && ($type == "*") ) )
         {
            $ret++;
         }
      }

      return $ret;
   }

   function getList($root, $type)
   {
      $ret = array();

      foreach ($this->config as $element)
      {
         if ( ( ($element["root"] == $root) && ($element["type"] == $type) ) ||
              ( ($element["root"] == $root) && ($type == "*") ) )
         {
            $ret[] = $element["value"];
         }
      }

      return $ret;
   }

   function listAll()
   {
      $ret = array();

      $ret = $this->listar($this->config);

      return $ret;
   }

   function foo() {}

   private function listar($dbase)
   {
      static $ret = array();
      static $inst = -1;

      $inst++;

      if ($inst == 0)
      {
         $dbase = $this->config;
      }

      var_dump($dbase);

      foreach ($dbase as $db)
      {
         $ret[] = $db["root"];
         if ( $db["cont"]!=NULL)
         {
            $this->listar($db);
         }
      }

      $inst--;

      return $ret;
   }

   private function getListIn($root, $dbase, $level)
   {
      $ret = array();

      foreach ($dbase as $db)
      {
         $ret[] = getListIn($root,$db, $level + 1);
      }

      return $ret;

   }

   function exist($root, $attr)
   {
      $attributes = $this->getAttributes($root);

      foreach ($attributes as $attribute)
      {
         if ($attribute == $attr)
         {
            return true;
         }
      }

      return false;

   }

   function getAttributes($root)
   {
      $ret = array();

      foreach ($this->config as $element)
      {
         if ( $element["root"] == $root )
         {
            $ret[] = $element["type"];
         }
      }

      return $ret;

   }

   function dump()
   {
      var_dump($this->config);
   }

}

$config = new configClass();

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
?>

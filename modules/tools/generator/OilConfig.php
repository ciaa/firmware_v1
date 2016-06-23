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
 ** \file OilConfig.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*==================[inclusions]=============================================*/
require_once("OilParser.php");

/*==================[class definition]=======================================*/
/** \brief OilConfig Configuration Class
 **
 ** This class implements the generation configuration class of FreeOSEK
 **
 **/
class OilConfig {
   protected $config = array();

   function OilConfig()
   {
   }

   function parseOilFile($file)
   {
      $parser = new OilParser();
      $parser->loadFile($file);
      $parser->parse();
      foreach ($parser->getOil() as $item) {
          $this->config[] = $item;
      }
   }

   function setConfig($config)
   {
      $this->config = $config;
   }

   function parseAutosarFile()
   {
   }

   function getValue($root, $type)
   {
      foreach ($this->config as $element)
      {
         if ( ($element["root"] == $root) && ($element["type"] == $type) )
         {
            return $element["value"];
         }
      }
      return false;
   }

   private function compare($element, $root, $type)
   {
      return ( $element['root']==$root &&
         ($element['type'] == $type || $type == '*')
      );
   }

   function getCount($root, $type)
   {
      $count = 0;

      foreach ($this->config as $element)
      {
         if ( $this->compare($element, $root, $type) )
         {
            $count++;
         }
      }

      return $count;
   }

   function getList($root, $type, $where = array() )
   {
      $list = array();

      foreach ($this->config as $element)
      {
         if ( $this->compare($element, $root, $type) )
         {
            if (empty($where)) {
               $list[] = $element["value"];
            } else {
              die();
            }
         }
      }

      return $list;
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

   /* convert config priority to real osek priority */
   function priority2osekPriority($tasks)
   {
      $priorities = array();
      foreach ($tasks as $task)
      {
         $priorities[] = $this->getValue("/OSEK/" . $task, "PRIORITY");
      }
      $priorities = remove_doubles($priorities);

      $priority = array();
      foreach ($priorities as $count=>$prio)
      {
         $priority[$prio] = $count;
      }
      arsort($priority);
      return $priority;
   }


   function dump()
   {
      var_dump($this->config);
   }

}


/** @} doxygen end group definition */
/** @} doxygen end group definition */

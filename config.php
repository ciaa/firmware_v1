<?php
/* Copyright 2008, 2009, Mariano Cerdeiro
 *
 * This file is part of FreeOSEK.
 *
 * FreeOSEK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Linking FreeOSEK statically or dynamically with other modules is making a
 * combined work based on FreeOSEK. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of FreeOSEK give
 * you permission to combine FreeOSEK program with free software programs or
 * libraries that are released under the GNU LGPL and with independent modules
 * that communicate with FreeOSEK solely through the FreeOSEK defined interface.
 * You may copy and distribute such a system following the terms of the GNU GPL
 * for FreeOSEK and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL requires
 * distribution of source code.
 *
 * Note that people who make modified versions of FreeOSEK are not obligated to
 * grant this special exception for their modified versions; it is their choice
 * whether to do so. The GNU General Public License gives permission to release
 * a modified version without this exception; this exception also makes it
 * possible to release a modified version which carries forward this exception.
 *
 * FreeOSEK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.
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
				return $element["value"];
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

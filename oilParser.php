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

/** \brief FreeOSEK Generator Oil Parser Implementation File
 **
 ** This file implements the Generator Oil Parser Class
 **
 ** \file oilParser.php
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

/*==================[class definition]=======================================*/
/** \brief Oil Parser Class Implementation
 **
 ** This class implements the Oil Parser Class
 **
 **/
class oilParserClass {
	protected $file;
	protected $lines;
	protected $line;
	protected $config = array();

	function removeComments()
	{
		/* todo removeall comments */
		for ($l = 0; $l < count($this->lines); $l++)
		{
			/* remove spaces and tabs at start and end of the line */
			$this->lines[$l] = trim($this->lines[$l]);

			do
			{
				/* replaces multiply spaces with only one */
				$this->lines[$l] = preg_replace('/ +/', " ", $this->lines[$l]);

				/* remove multpy tabs or tabs with one space */
				$this->lines[$l] = preg_replace('/\t/', " ", $this->lines[$l]);

			}
			while (  ( strlen($this->lines[$l]) > 1 )  &&
						( strpos($this->lines[$l], "  ") !== false) );

			/* remove spaces sides of = */
			$this->lines[$l] = str_replace(" = ", "=", $this->lines[$l]);

			/* remove space before ; */
			$this->lines[$l] = str_replace(" ;",";", $this->lines[$l]);

			/* remove ; */
			$this->lines[$l] = str_replace(";","", $this->lines[$l]);

			/* remove c++ comments */
			if ( strpos($this->lines[$l],"//") !== false )
			{
				$tmp = split("//", $this->lines[$l]);
				$this->lines[$l] = $tmp[0];
			}

			/* remove c comments */
			/* to do */
			#if ( strpos($this->lines[$l], "/*") !== false )
			#{
		   #	$tmp = array ( substr ($this->lines[$l],0, strpos($this->lines[$l], "/*")-1),
			#						substr ($this->lines[$l],strpos($this->lines[$l], "/*")+2), strlen($this->lines[$l]));
			#	print "t1:" . $tmp[0] . "t2:" . $tmp[1] . "\n";
			#	$this->lines[$l] = $tmp[0];
			#	if ( strpos($tmp[1], "*/") !== false )
			#	{
			#		$this->lines[$l] .= " " . substr($tmp[1], strpos("*/", $tmp[1])+2, strlen($tmp[1]));
			#	}
			#}
		}
	}

	function resetLine()
	{
		$this->line = 0;
	}

	function nextLine()
	{
		$this->line++;
	}

	function eof()
	{
		$ret = false;

		if ( $this->line == count ($this->lines) )
		{
			$ret = true;
		}

		return $ret;
	}

	function getDefinition()
	{
		$ret = array ( "", "", false);

		$tmp = $this->lines[$this->line];

		if  ( strpos($tmp, "{") !== false )
		{
			$ret[2] = true;
			$tmp = split("{",$tmp);
			$tmp = $tmp[0];
		}

      if ($tmp != "")
		{
		   if ( strpos($tmp, "=") !== false )
   		{
	   		$tmp = split("=", $tmp);
		   	$ret[0] = $tmp[0];
			   $ret[1] = $tmp[1];
   		}
	   	else
		   {
   			$tmp = split(" ", $tmp);
            $ret[0] = $tmp[0];
            $ret[1] = $tmp[1];
   		}
   	}
		return $ret;
	}

	function parser($root)
	{
		$config = array();
		$entry = array();
		static $inst = 0;
		$inst++;

		while( 	( ( $inst == 1 ) && ($this->eof() === false ) ) ||
					( ( $inst != 1 ) && (strpos($this->lines[$this->line],"}") === false  ) ) )
		{
			$def = $this->getDefinition();

			if ( $def[0] != "")
			{
				$entry["root"] = $root;
				$entry["type"] = trim($def[0]);
				$entry["value"] = trim($def[1]);
				if ( $def[2] === true )
				{
					$this->nextLine();
					if ( $def[0] != "AUTOSTART" )
					{
						$this->parser($root . "/" . trim($def[1]));
					}
					else
					{
						$this->parser($root);
					}
				}
				else
				{
					$this->nextLine();
				}
				$this->config[] = $entry;

			}
			else
			{
				$this->nextLine();
			}
		}
		if ( $inst != 1 )
		{
			$this->nextLine();
		}

		$inst--;
	}

	function oilParserClass($file)
	{
		if (file_exists($file) == false) {
			error('Configuration file ' . $file . ' not found.');
		}

		$this->file = $file;
		$this->lines = file($file);

		$entry = array();

		$this->removeComments();

		$this->resetLine();

		#$entry["root"] = "/OSEK";
		#$entry["type"] = "OSEK";
		#$entry["name"] = "";

		#$this->config[] = $entry;

		#$this->parser("/OSEK");
		$this->parser("");
	}

	function getOil()
	{
		return $this->config;
	}

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
?>


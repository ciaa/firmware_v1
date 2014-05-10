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

/** \brief FreeOSEK Generator
 **
 ** This file implements the FreeOSEK Generator
 **
 ** \file generator.php
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
 * v0.1.0 20080812 MaCe initial version
 */

/*==================[inclusions]=============================================*/
require_once("config.php");

/*=================[user functions]============================================*/
$errors = 0;
$warnings = 0;
$generating = false;
$runagain = false;

/** \brief Compare Files Function
 **/
function comfiles($f)
{
	$ret = false;

	if( file_exists($f[0]) && file_exists($f[1]) )
	{
		$f1 = file($f[0]);
		$f2 = file($f[1]);
		#info("File: " . $f[0] . ", " . $f[1]);

		if(count($f1)==count($f2))
		{
			$loopi = 0;
			for (; ($loopi < count($f1)) && ($f1[$loopi]==$f2[$loopi]); $loopi++)
			{
				/* nothing to do */
			}
         if( ($loopi == count($f1)) )
			{
				$ret = true;
			}
		}
	}
	else
	{
#		info("one file doesnt exist!!!!!!!!!!!!!!!!!!!!");
	}

	return $ret;

}

/** \brief Info Generator Function
 **
 ** This function shall be used to report generation information to the user.
 ** Please don't use this function for report warnings or errors.
 **
 ** \param[in] msg string containing the information message to be reported
 **/
function info($msg)
{
	global $verbose;
	global $generating;
	if($generating == true)
	   ob_end_flush();
	if ($verbose)
	{
		print "INFO: " . $msg . "\n";
	}
	if($generating == true)
	ob_start('ob_file_callback');
}

/** \brief Warning Generator Function
 **
 ** This function shall be used to report warnings information to the user.
 ** Don't use this function to report information or errors.
 **
 ** \param[in] msg string containing the warning message to be reported.
 **/
function warning($msg)
{
	ob_end_flush();
	print "WARNING: " . $msg . "\n";
	global $warnings;
	$warnings++;
	ob_start('ob_file_callback');
}

/** \brief Error Generator Function
 **
 ** This function shall be used to report error information to the user.
 ** The generation process will continues to provide all error to the user.
 ** If you wan to report an error and to abort the generation use the
 ** abort function.
 ** Don't use this function to report information or warnings.
 **
 ** \param[in] msg string containing the error message to be reported.
 **/
function error($msg)
{
	global $error;
	ob_end_flush();
	print "ERROR: " . $msg . "\n";
	global $errors;
	$errors++;
	$error = true;
	ob_start('ob_file_callback');
}

/** \brief Abort Generator Function
 **
 ** This function shall be used to report an error and abort the generation.
 ** Don't use this function to report information or warnings.
 **
 ** \param[in] msg string containing the error message to be reported.
 **/
function halt($msg)
{
	ob_end_flush();
	error($msg);
	exit(1);
}

/*=================[end of user functions]=====================================*/

/*=================[global variables]==========================================*/
$verbose = false;
$path = "";

function ob_file_callback($buffer)
{
	global $ob_file;
	fwrite($ob_file,$buffer);
}

function printCmdLine()
{
	print "INFO: the generator was called as follow:\nINFO: ";
	foreach ($_SERVER['argv'] as $arg)
	{
		print "$arg ";
	}
	print "\n";
}

$args = $_SERVER['argv'];
$path = array_shift($args);

$path = substr($path,0, strlen($path)-strlen("/generator.php"));

print "FreeOSEK Generator - Copyright 2008, 2009, Mariano Cerdeiro - http://opensek.sf.net\n\n";

foreach ($args as $arg)
{
	switch ($arg)
	{
		case "--cmdline":
			printCmdLine();
			break;
		case "-l":
			print "INFO: ------ LICENSE START ------\n";
			print "INFO: FreeOSEK Generator is part of OpenOSEK.\nINFO: \n";
			print "INFO: FreeOSEK is free software: you can redistribute it and/or modify\n";
			print "INFO: it under the terms of the GNU General Public License as published by\n";
			print "INFO: the Free Software Foundation, either version 3 of the License, or\n";
			print "INFO: (at your option) any later version.\nINFO: \n";
			print "INFO: Linking FreeOSEK statically or dynamically with other modules is making a\n";
			print "INFO: combined work based on FreeOSEK. Thus, the terms and conditions of the GNU\n";
			print "INFO: General Public License cover the whole combination.\nINFO:\n";
			print "INFO: In addition, as a special exception, the copyright holders of FreeOSEK give\n";
			print "INFO: you permission to combine FreeOSEK program with free software programs or\n";
			print "INFO: libraries that are released under the GNU LGPL and with independent modules\n";
			print "INFO: that communicate with FreeOSEK solely through the FreeOSEK defined interface. \n";
			print "INFO: You may copy and distribute such a system following the terms of the GNU GPL\n";
			print "INFO: for FreeOSEK and the licenses of the other code concerned, provided that you\n";
			print "INFO: include the source code of that other code when and as the GNU GPL requires\n";
			print "INFO: distribution of source code.\nINFO:\n";
			print "INFO: Note that people who make modified versions of FreeOSEK are not obligated to\n";
			print "INFO: grant this special exception for their modified versions; it is their choice\n";
			print "INFO: whether to do so. The GNU General Public License gives permission to release\n";
			print "INFO: a modified version without this exception; this exception also makes it\n";
			print "INFO: possible to release a modified version which carries forward this exception.\nINFO:\n";
			print "INFO: FreeOSEK is distributed in the hope that it will be useful,\n";
			print "INFO: but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
			print "INFO: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
			print "INFO: GNU General Public License for more details.\n";
			print "INFO: You should have received a copy of the GNU General Public License\n";
			print "INFO: along with FreeOSEK. If not, see <http://www.gnu.org/licenses/>.\n";
			print "INFO: ------- LICENSE END -------\n";
			break;
		case "-h":
		case "--help":
			print "php generator.php [-l] [-h] [--cmdline] -c <CONFIG_1> [<CONFIG_2>] -o <OUTPUTDIR> -f <GENFILE_1> [<GENFILE_2>]\n";
			print "      -c   indicate the configuration input files\n";
			print	"      -o   output directory\n";
			print "      -f   indicate the files to be generated\n";
			print "   optional parameters:\n";
			print "      -h   display this help\n";
			print "      -l   displays a short license overview\n";
			print "      --cmdline print the command line\n";
			exit();
			break;
		case "-v":
			$verbose = true;
			break;
		case "-c":
		case "-o":
		case "-f":
			$oldarg = $arg;
			break;
		default:
			switch($oldarg)
				{
					case "-c":
						/* add a config file */
						$configfiles[] = $arg;
						break;
					case "-o":
						/* add an output dir */
						$outputdir[]= $arg;
						break;
					case "-f":
						/* add generated file */
						$genfiles[] = $arg;
						break;
					default:
						halt("invalid argument: " . $arg);
						break;
				}
			break;
	}
}

if (count($configfiles)==0)
{
	halt("at least one config file shall be provided");
}

if (count($outputdir)!=1)
{
	halt("exactly one output directory shall be provided");
}

if (count($genfiles)==0)
{
	halt("at least one file to be generated shall be provided");
}

if ($verbose)
{
	info("list of configuration files:");
	$count = 1;
	foreach ($configfiles as $file)
	{
		info("configuration file " . $count++ . ": " . $file);
	}

	info("list of files to be generated:");
	$count = 1;
	foreach ($genfiles as $file)
	{
		info("generated file " . $count++ . ": " . $file);
	}

	info("output directory: " . $outputdir[0]);
}

foreach ($configfiles as $file)
{
	info("reading " . $file);
	$config->parseOilFile($file);
}

foreach ($genfiles as $file)
{
	$exits = false;
	$outfile = $file;
	$outfile = substr($outfile, 0, strlen($outfile)-4);
	//print "info aca: pos: ". strpos($outile, "a",1);
	#while(strpos($outfile,"gen")!==FALSE)
	{
		#print "si: $outfile - ";
		$outfile = substr($outfile, strpos($outfile, "gen")+3);
		#print "$outfile\n";
	}
   $outfile = $outputdir[0] . $outfile;
	info("generating ". $file . " to " . $outfile);
	global $ob_file;
	if(!file_exists(dirname($outfile)))
	{
		mkdir(dirname($outfile), 0777, TRUE);
	}
	if(file_exists($outfile))
	{
		$exits = true;
		if(file_exists($outfile . ".old"))
		{
			unlink($outfile . ".old");
		}
		rename($outfile, $outfile . ".old");
	}
	$ob_file = fopen($outfile, "w");
	ob_start('ob_file_callback');
	$generating=true;
	require_once($file);
	$generating=false;
	ob_end_flush();
	fclose($ob_file);
	if(comfiles(array($outfile, $outfile . ".old")) == false)
	{
		if(substr($file, strlen($file) - strlen(".mak.php") == ".mak.php"))
		{
			$runagain = true;
		}
	}

}

info("Generation Finished with WARNINGS: $warnings and ERRORS: $errors");
if ($errors > 0)
{
	exit(1);
}
elseif($runagain == true)
{
	info("a makefile was generated, generation process will be executed again");
	system("make generate");
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
?>

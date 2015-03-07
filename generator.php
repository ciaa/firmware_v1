<?php
/* Copyright 2008, 2009, 2015 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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
      #info("one file doesnt exist!!!!!!!!!!!!!!!!!!!!");
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

print "ciaaFirmware RTOS Generator - Copyright 2008, 2009, 2015 Mariano Cerdeiro\n";
print "                              Copyright 2014, ACSE & CADIEEL\n";
print "         ACSE : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/\n";
print "         CADIEEL: http://www.cadieel.org.ar\n";
print "         All rights reserved.\n\n";

foreach ($args as $arg)
{
   switch ($arg)
   {
   case "--cmdline":
      printCmdLine();
      break;
   case "-l":
      print "INFO: ------ LICENSE START ------\n";
      print "INFO: This file is part of CIAA Firmware.\n";
      print "INFO: Redistribution and use in source and binary forms, with or without\n";
      print "INFO: modification, are permitted provided that the following conditions are met:\n";
      print "INFO: 1. Redistributions of source code must retain the above copyright notice,\n";
      print "INFO: this list of conditions and the following disclaimer.\n";
      print "INFO: 2. Redistributions in binary form must reproduce the above copyright notice,\n";
      print "INFO: this list of conditions and the following disclaimer in the documentation\n";
      print "INFO: and/or other materials provided with the distribution.\n";
      print "INFO: 3. Neither the name of the copyright holder nor the names of its\n";
      print "INFO: contributors may be used to endorse or promote products derived from this\n";
      print "INFO: software without specific prior written permission.\n";
      print "INFO: THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n";
      print "INFO: AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n";
      print "INFO: IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n";
      print "INFO: ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n";
      print "INFO: LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n";
      print "INFO: CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n";
      print "INFO: SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n";
      print "INFO: INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n";
      print "INFO: CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n";
      print "INFO: ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n";
      print "INFO: POSSIBILITY OF SUCH DAMAGE.\n";
      print "INFO: ------- LICENSE END -------\n";
      break;
   case "-h":
   case "--help":
      print "php generator.php [-l] [-h] [--cmdline] [-Ddef[=definition]] -c <CONFIG_1> [<CONFIG_2>] -o <OUTPUTDIR> -f <GENFILE_1> [<GENFILE_2>]\n";
      print "      -c   indicate the configuration input files\n";
      print "      -o   output directory\n";
      print "      -f   indicate the files to be generated\n";
      print "   optional parameters:\n";
      print "      -h   display this help\n";
      print "      -l   displays a short license overview\n";
      print "      -D   defines\n";
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
      if (empty($oldarg))
      {
         if (0 == strpos("-D", $arg))
         {
            $tmp = explode("=", substr($arg, 2));
            $definition[$tmp[0]] = $tmp[1];
         }
      } else {
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
      if(substr($file, strlen($file) - strlen(".mak.php") ) == ".mak.php")
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

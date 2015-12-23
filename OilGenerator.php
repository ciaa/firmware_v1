<?php
/* Copyright 2008, 2009, 2015 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
 * Copyright 2015, Carlos Pantelides
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
 ** \file OilGenerator.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */

/*==================[inclusions]=============================================*/
require_once("OilConfig.php");
require_once("Log.php");
/*=================[user functions]============================================*/

class OilGenerator
{
   private $verbose = false;
   private $path = "";
   private $log;
   private $writer;

   /** \brief Compare Files Function
   **/
   function compareFiles($filename1,$filename2)
   {
      if( file_exists($filename1) && file_exists($filename2) )
      {
         return (file($filename1) == file($filename2));
      }

      return false;

   }

   /*=================[end of user functions]=====================================*/

   /*=================[global variables]==========================================*/


   function printCmdLine()
   {
      print "INFO: the generator was called as follow:\nINFO: ";
      foreach ($_SERVER['argv'] as $arg)
      {
         print "$arg ";
      }
      print "\n";
   }

   function printInfo()
   {
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
   }

   function printHelp()
   {
      print "php generator.php [-l] [-h] [--cmdline] [-Ddef[=definition]] -c <CONFIG_1> [<CONFIG_2>] -o <OUTPUTDIR> -t <GENFILE_1> [<GENFILE_2>]\n";
      print "      -c   indicate the configuration input files\n";
      print "      -o   output directory\n";
      print "      -t   indicates the templates to be processed\n";
      print "      -b   relative base path (default \"/templates/\")\n";
      print "   optional parameters:\n";
      print "      -h   display this help\n";
      print "      -l   displays a short license overview\n";
      print "      -D   defines\n";
      print "      --cmdline print the command line\n";
   }

   function processArgs($args)
   {
      $configFiles= array();
      global $definitions;
      $definitions=array();
      $baseOutDir=array();
      $templateFiles=array();
      $directorySeparator=array();

      foreach ($args as $arg)
      {
         switch ($arg)
         {
         case "--cmdline":
            $this->printCmdLine();
            break;
         case "-l":
            $this->printInfo();
            break;
         case "-h":
         case "--help":
            $this->printHelp();
            exit();
            break;
         case "-v":
            $this->verbose = true;
            break;
         case "-c":
         case "-o":
         case "-t":
         case "-b":
            $oldarg = $arg;
            break;
         default:
            if (empty($oldarg))
            {
               if (0 == strpos("-D", $arg))
               {
                  $tmp = explode("=", substr($arg, 2));
                  $definitions[$tmp[0]] = $tmp[1];
               }
            } else {
               switch($oldarg)
               {
               case "-c":
                  /* add a config file */
                  $configFiles[] = $arg;
                  break;
               case "-o":
                  /* add an output dir */
                  $baseOutDir[]= $arg;
                  break;
               case "-t":
                  /* add generated file */
                  $templateFiles[] = $arg;
                  break;
               case "-b":
                  /* add path delimiter */
                  $directorySeparator[] = $arg;
                  break;
               default:
                  $this->log->halt("invalid argument: " . $arg);
                  exit(1);
                  break;
               }
            }
            break;
         }
      }

      if (count($configFiles)==0)
      {
         $this->log->halt("at least one config file shall be provided");
         exit(1);
      }

      if (count($baseOutDir)!=1)
      {
         $this->log->halt("exactly one output directory shall be provided");
         exit(1);
      }

      if (count($templateFiles)==0)
      {
         $this->log->halt("at least one tempalte file shall be provided");
         exit(1);
      }

      if (count($directorySeparator)>1)
      {
         $this->log->halt("no more than one path delimiter shall be provided");
         exit(1);
      }

      if (count($directorySeparator == 0 ))
      {
         $directorySeparator[]="/gen/"; #TODO: use /templates/
      }

      return array($this->verbose, $definitions, $configFiles, $baseOutDir[0], $templateFiles,$directorySeparator[0]);
   }

   public function checkFiles( $configFiles, $baseOutDir, $templateFiles)
   {
      $ok = true;
      foreach ($configFiles as $file)
      {
         if ( !file_exists($file))
         {
            $this->log->error("Configuration file $file not found");
            $ok = false;
         }
      }

      foreach ($templateFiles as $file)
      {
         if(!file_exists($file))
         {
            $this->log->error("Template $file does not exists");
            $ok = false;
         }
      }

      if ( ! is_dir($baseOutDir) || ! is_writeable($baseOutDir) )
      {
         $this->log->error("Directory $baseOutDir not writeable");
         $ok = false;
      }
      return $ok;
   }

   function isMak($outfile, $runagain)
   {
      if ($runagain)
      {
         if($this->compareFiles($outfile, $outfile . ".old") === false)
         {
            if(substr($file, strlen($file) - strlen(".mak.php") ) == ".mak.php")
            {
               $runagain = true;
            }
         }
      }
      return $runagain;
   }

   public function OilGenerator($writer)
   {
      $this->writer = $writer;
      $this->log = new Log($writer);
      $this->writer->setLog($this->log);

   }

   public function run($args)
   {

      $path = dirname(array_shift($args));

      list($verbose, $definitions, $configFiles, $baseOutDir, $templateFiles,$directorySeparator)= $this->processArgs($args);

      $this->log->setVerbose($verbose);
      if ($verbose)
      {
         print "ciaaFirmware RTOS Generator - Copyright 2008, 2009, 2015 Mariano Cerdeiro\n";
         print "                              Copyright 2014, ACSE & CADIEEL\n";
         print "         ACSE : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/\n";
         print "         CADIEEL: http://www.cadieel.org.ar\n";
         print "         All rights reserved.\n\n";
      }

      if ( ! $this->checkFiles($configFiles, $baseOutDir , $templateFiles) )
      {
         $this->log->halt("Missing files");
         exit(1);
      }
      if ($this->verbose)
      {
         $this->log->info("list of configuration files:");
         $count = 1;
         foreach ($configFiles as $file)
         {
            $this->log->info("configuration file " . $count++ . ": " . $file);
         }

         $this->log->info("list of templates to be processed:");
         $count = 1;
         foreach ($templateFiles as $file)
         {
            $this->log->info("template file " . $count++ . ": " . $file);
         }

         $this->log->info("output directory: " . $baseOutDir);
      }

      global $config;
      $config = new OilConfig();
      $runagain = false;
      foreach ($configFiles as $file)
      {
         $this->log->info("reading " . $file);
         $config->parseOilFile($file);
      }

      foreach ($templateFiles as $file)
      {
         if(!file_exists($file))
         {
            $this->log->error("Template $file does not exists");
         }
         else
         {
            $outfile = $this->writer->open($file,$baseOutDir,$directorySeparator);
            $this->writer->start();
            require_once($file);
            $this->writer->close();
            $runagain = $this->isMak($outfile, $runagain);
        }
      }

      $this->log->info($this->log->getReport());

      if ($this->log->getErrors() > 0)
      {
         exit(1);
      }
      if($runagain == true)
      {
         $this->log->info("a makefile was generated, generation process will be executed again");
         system("make generate");
      }
   }
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */

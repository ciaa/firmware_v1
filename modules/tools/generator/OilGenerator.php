<?php
/* Copyright 2008, 2009, 2015 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
 * Copyright 2015, 2016 Carlos Pantelides
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
require_once("OilGeneratorException.php");
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

   function printCmdLine()
   {
      $this->writer->printMsg("INFO: the generator was called as follow:\nINFO: ");
      foreach ($_SERVER['argv'] as $arg)
      {
         $this->writer->printMsg( "$arg ");
      }
      $this->writer->printMsg( "\n");
   }

   function printInfo()
   {
      $this->writer->printMsg( "INFO: ------ LICENSE START ------\n");
      $this->writer->printMsg( "INFO: This file is part of CIAA Firmware.\n");
      $this->writer->printMsg( "INFO: Redistribution and use in source and binary forms, with or without\n");
      $this->writer->printMsg( "INFO: modification, are permitted provided that the following conditions are met:\n");
      $this->writer->printMsg( "INFO: 1. Redistributions of source code must retain the above copyright notice,\n");
      $this->writer->printMsg( "INFO: this list of conditions and the following disclaimer.\n");
      $this->writer->printMsg( "INFO: 2. Redistributions in binary form must reproduce the above copyright notice,\n");
      $this->writer->printMsg( "INFO: this list of conditions and the following disclaimer in the documentation\n");
      $this->writer->printMsg( "INFO: and/or other materials provided with the distribution.\n");
      $this->writer->printMsg( "INFO: 3. Neither the name of the copyright holder nor the names of its\n");
      $this->writer->printMsg( "INFO: contributors may be used to endorse or promote products derived from this\n");
      $this->writer->printMsg( "INFO: software without specific prior written permission.\n");
      $this->writer->printMsg( "INFO: THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n");
      $this->writer->printMsg( "INFO: AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
      $this->writer->printMsg( "INFO: IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n");
      $this->writer->printMsg( "INFO: ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE\n");
      $this->writer->printMsg( "INFO: LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR\n");
      $this->writer->printMsg( "INFO: CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\n");
      $this->writer->printMsg( "INFO: SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\n");
      $this->writer->printMsg( "INFO: INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n");
      $this->writer->printMsg( "INFO: CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)\n");
      $this->writer->printMsg( "INFO: ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n");
      $this->writer->printMsg( "INFO: POSSIBILITY OF SUCH DAMAGE.\n");
      $this->writer->printMsg( "INFO: ------- LICENSE END -------\n");
   }

   function printHelp()
   {
      $this->writer->printMsg( "php generator.php [-l] [-h] [--cmdline] [-b PATH ] [-Ddef[=definition]] -c <CONFIG_1> [<CONFIG_N>] -o <OUTPUTDIR> -t <TEMPLATE_1> [<TEMPLATE_N>] [ -H <HELPER_1> [HELPER_N]]>\n");
      $this->writer->printMsg( "      -c   indicate the configuration input files\n");
      $this->writer->printMsg( "      -o   output directory\n");
      $this->writer->printMsg( "      -t   indicates the templates to be processed\n");
      $this->writer->printMsg( "   optional parameters:\n");
      $this->writer->printMsg( "      -b   relative base path (default \"/templates/\")\n");
      $this->writer->printMsg( "      -H   load helpers\n");
      $this->writer->printMsg( "      -h   display this help\n");
      $this->writer->printMsg( "      -l   displays a short license overview\n");
      $this->writer->printMsg( "      -D   defines\n");
      $this->writer->printMsg( "      --cmdline print the command line\n");
   }

   function processArgs($args)
   {
      $configFiles= array();
      $definitions=array();
      $baseOutDir=array();
      $templateFiles=array();
      $directorySeparator=array();
      $helperFiles=array();
      $atLeastOneHelper = false;

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
         case "-H":
            $atLeastOneHelper = true;
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
               case "-H":
                  /* add a helper to load */
                  $helperFiles[] = $arg;
                  break;
               case "-c":
                  /* add a config file */
                  $configFiles[] = $arg;
                  break;
               case "-o":
                  /* add an output dir */
                  $baseOutDir[]= $arg;
                  break;
               case "-t":
                  /* add template file */
                  $templateFiles[] = $arg;
                  break;
               case "-b":
                  /* add path delimiter */
                  $directorySeparator[] = $arg;
                  break;
               default:
                  throw new OilGeneratorException('invalid argument: '. $arg, 1);
                  break;
               }
            }
            break;
         }
      }

      if ($atLeastOneHelper && count($helperFiles) == 0)
      {
         throw new OilGeneratorException('at least one helper file shall be provided', 8);
      }

      if (count($configFiles)==0)
      {
         throw new OilGeneratorException('at least one config file shall be provided', 2);
      }

      if (count($baseOutDir)!=1)
      {
         throw new OilGeneratorException('exactly one output directory shall be provided', 3);
      }

      if (count($templateFiles)==0)
      {
         throw new OilGeneratorException('at least one template file shall be provided', 4);
      }

      if (count($directorySeparator)>1)
      {
         throw new OilGeneratorException('no more than one path delimiter shall be provided', 5);
      }

      if (count($directorySeparator == 0 ))
      {
         $directorySeparator[]="/gen/"; #TODO: use /templates/
      }

      return array($this->verbose, $definitions, $configFiles, $baseOutDir[0], $templateFiles,$directorySeparator[0], $helperFiles);
   }

   public function checkFileNaming($configFiles, $templateFiles, $helperFiles)
   {
      foreach($configFiles as $pos=>$file)
      {
         if (!  preg_match ( '/.*\.oilx{0,1}$/i' , $file))
         {
            $this->log->warning("The config file $file should be .oil or .oilx");
         }
      }
      foreach($templateFiles as $pos=>$file)
      {
         if (!  preg_match ( '/.*\.[hc]\.php$/i' , $file))
         {
            $this->log->warning("The template file $file should be .h.php or .c.php");
         }
      }
      foreach($helperFiles as $pos=>$file)
      {
         if (!  preg_match ( '/.*\.php$/i' , $file))
         {
            $this->log->warning("The helper file $file should be .php");
         }
      }
   }

   public function checkFilesOrFail( $configFiles, $baseOutDir, $templateFiles, $helperFiles)
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
            $this->log->error("Template $file does not exist");
            $ok = false;
         }
      }

      if ( ! is_dir($baseOutDir) || ! is_writeable($baseOutDir) )
      {
         $this->log->error("Directory $baseOutDir not writeable");
         $ok = false;
      }

      foreach ($helperFiles as $file)
      {
         if(!file_exists($file))
         {
            $this->log->error("Helper $file does not exist");
            $ok = false;
         }
      }

      if (! $ok) {
         throw new OilGeneratorException('Missing files', 6);
      }
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

   public function getNames($file)
   {
      $className=basename($file,'.php');
      $name = lcfirst($className);
      return array($name,$className);
   }

   public function loadHelper($file)
   {
      list($helperName,$helperClassName)=$this->getNames($file);

      $this->log->info("   helper file       : $file");
      $this->log->info("   helper name       : $helperName");
      $this->log->info("   helper class name : $helperClassName");

      if ( class_exists($helperClassName))
      {
         $this->log->warning("   class '$helperClassName' already exists, not loading again");
      }
      else
      {
         $this->log->info("   loading...");

         if (! @include_once($file) )
         {
            throw new OilGeneratorException("Helper '$file' not found", 9);
         }

         if (! class_exists($helperClassName))
         {
            throw new OilGeneratorException("Helper '$file' does not define class '$helperClassName'", 11);
         }

         if ( ! is_subclass_of($helperClassName, 'Helper'))
         {
            throw new OilGeneratorException("Helper '$file'  define a class '$helperClassName' that does not extend class Helper", 122);

         }
      }
      if (! isset($this->helper)) {
         $this->helper = new stdClass();
      }
      if (! isset($this->helper->$helperName))
      {
         $this->helper->$helperName = new $helperClassName($this->config,$this->definitions,$this->log);
      }
   }

   public function showFeedback($configFiles, $baseOutDir, $templateFiles, $helperFiles) {
      if ($this->verbose)
      {
         $this->log->info("list of configuration files:");
         $count = 1;
         foreach ($configFiles as $file)
         {
            $this->log->info("   configuration file " . $count++ . ": " . $file);
         }

         $this->log->info("list of templates to be processed:");
         $count = 1;
         foreach ($templateFiles as $file)
         {
            $this->log->info("   template file " . $count++ . ": " . $file);
         }

         $this->log->info("list of helpers to be loaded:");
         $count = 1;
         foreach ($helperFiles as $file)
         {
            $this->log->info("   helper file " . $count++ . ": " . $file);
         }

         $this->log->info("output directory: " . $baseOutDir);
      }
   }

   public function showHeader($verbose)
   {
      if ($verbose)
      {
         $this->writer->printMsg( "ciaaFirmware RTOS Generator - Copyright 2008, 2009, 2015 Mariano Cerdeiro\n");
         $this->writer->printMsg( "                              Copyright 2014, ACSE & CADIEEL\n");
         $this->writer->printMsg( "         ACSE : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/\n");
         $this->writer->printMsg( "         CADIEEL: http://www.cadieel.org.ar\n");
         $this->writer->printMsg( "         All rights reserved.\n\n");
      }
   }

   public function parseOilFiles($configFiles)
   {
      foreach ($configFiles as $file)
      {
         $this->log->info("reading " . $file);
         $this->config->parseOilFile($file);
      }
   }

   public function loadHelpers($helperFiles)
   {
      foreach ($helperFiles as $file)
      {
         $this->log->info("loading " . $file);
         $this->loadHelper($file);
      }
   }

   public function renderTemplates($templateFiles, $baseOutDir, $directorySeparator, $runagain)
   {
      foreach ($templateFiles as $file)
      {
         if(!file_exists($file))
         {
            $this->log->error("Template $file does not exist");
         }
         else
         {
            $outfile = $this->writer->open($file, $baseOutDir, $directorySeparator);
            $this->writer->start();
            $clone = clone($this);
            $clone->isolatedInclude($file);
            $this->writer->close();
            $runagain = $this->isMak($outfile, $runagain);
         }
      }
      return $runagain;
   }

   public function run($args)
   {
      try {
         $path = dirname(array_shift($args));

         list($verbose, $this->definitions, $configFiles, $baseOutDir, $templateFiles,$directorySeparator, $helperFiles)= $this->processArgs($args);

         $this->log->setVerbose($verbose);

         $this->showHeader($verbose);

         $this->checkFileNaming($configFiles, $templateFiles, $helperFiles);

         $this->checkFilesOrFail($configFiles, $baseOutDir , $templateFiles, $helperFiles);

         $this->showFeedback($configFiles, $baseOutDir, $templateFiles, $helperFiles);

         $this->config = new OilConfig();

         $runagain = false;

         $this->parseOilFiles($configFiles);

         $this->loadHelpers($helperFiles);

         //TODO. I (CFP) am sure that it should be $runagain = $runagain || ....

         $runagain = $this->renderTemplates($templateFiles, $baseOutDir, $directorySeparator, $runagain);

         $this->log->info($this->log->getReport());

         if ($this->log->getErrors() > 0)
         {
            throw new OilGeneratorException('please report this error', 7);
         }

         if($runagain == true)
         {
            $this->log->info("a makefile was generated, generation process will be executed again");
            system("make generate");
         }
      } catch (OilGeneratorException $e) {
         $this->log->error($e->getMessage());
         exit($e->getCode());
      }
   }

   public function isolatedInclude($file) {
      require_once($file);
   }
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */

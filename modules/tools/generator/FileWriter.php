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

/** \brief FreeOSEK Generator
 **
 ** This file implements a File Writer utility
 **
 ** \file FileWriter.php
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup Generator
 ** @{ */
require_once("OutputWriter.php");

class FileWriter extends OutputWriter
{

   private $ob_file;

   public function printMsg($msg)
   {
      print $msg;
   }

   public function outputFileName($file,$baseOutDir,$directorySeparator)
   {
      $outfile = substr($file, 0, strlen($file)-4);
      $outfile = substr($outfile, strpos($outfile, $directorySeparator)+strlen($directorySeparator) -1);
      $outfile = $baseOutDir . $outfile;
      return $outfile;
   }

   public function open($file,$baseOutDir,$directorySeparator)
   {
      $outfile = $this->outputFileName($file,$baseOutDir,$directorySeparator);

      $this->log->info("buffering ". $file . " to " . $outfile);

      if(!file_exists(dirname($outfile)))
      {
         mkdir(dirname($outfile), 0777, TRUE);
      }
      if(file_exists($outfile))
      {
         $exists = true;
         if(file_exists($outfile . ".old"))
         {
            unlink($outfile . ".old");
         }
         rename($outfile, $outfile . ".old");
      }
      $this->ob_file = fopen($outfile, "w");
      return $outfile;
   }

   public function close()
   {
      $this->buffering=false;
      $this->flush();
      fclose($this->ob_file);
   }

   public function ob_file_callback($buffer)
   {
      fwrite($this->ob_file,$buffer);
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

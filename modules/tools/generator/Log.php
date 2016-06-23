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
 ** This file a Log utility for FreeOSEK Generator
 **
 ** \file Log.php
 **
 **/
class Log
{
   private $errors = 0;
   private $warnings = 0;
   private $writer;
   private $verbose = false;

   public function Log($writer)
   {
      $this->writer = $writer;
   }

   public function setVerbose($verbose = true)
   {
      $this->verbose = $verbose;
   }

   public function getErrors()
   {
      return $this->errors;
   }

   public function getWarnings()
   {
      return $this->warnings;
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
      $this->writer->pause();

      if ($this->verbose)
      {
         $this->writer->printMsg("INFO: " . $msg . "\n");
      }
      $this->writer->resume();

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
      $this->writer->pause();
      $this->writer->printMsg("WARNING: " . $msg . "\n");
      $this->warnings++;
      $this->writer->resume();

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
      $this->writer->pause();
      $this->writer->printMsg("ERROR: " . $msg . "\n");
      $this->errors++;
      $this->writer->resume();

   }

   public function getReport()
   {
      return "Generation Finished with WARNINGS: " .$this->warnings . " and ERRORS: " . $this->errors;
   }

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */

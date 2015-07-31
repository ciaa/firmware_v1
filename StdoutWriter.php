<?php

require_once("OutputWriter.php");

class StdoutWriter extends OutputWriter
{

   public function open($file,$baseOutDir,$pathDelimiter)
   {
      $this->log->info("generating ". $file . " to STDOUT");
      return "STDOUT";
   }

   public function close()
   {
      $this->buffering=false;
      ob_end_flush();
   }
   public function ob_file_callback($buffer)
   {
      print $buffer;
   }

}

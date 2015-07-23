<?php

require_once("OutputWriter.php");

class FileWriter extends OutputWriter
{

   private $ob_file;

   public function outputFileName($file,$baseOutDir,$pathDelimiter)
   {
      $outfile = substr($file, 0, strlen($file)-4);
      $outfile = substr($outfile, strpos($outfile, $pathDelimiter)+strlen($pathDelimiter) -1);
      $outfile = $baseOutDir . $outfile;
      return $outfile;
   }
   
   public function open($file,$baseOutDir,$pathDelimiter, $caller)
   {
      $outfile = $this->outputFileName($file,$baseOutDir,$pathDelimiter);
      
      $caller->info("buffering ". $file . " to " . $outfile);
      
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
      ob_end_flush();
      fclose($this->ob_file);
   }

   public function ob_file_callback($buffer)
   {  
      fwrite($this->ob_file,$buffer);
   }
}

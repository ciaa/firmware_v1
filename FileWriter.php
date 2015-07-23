<?php

require_once("OutputWriter.php");

class FileWriter extends OutputWriter
{

   private $ob_file;
   private $buffering = false;

   public function open($file,$baseOutDir,$pathDelimiter, $caller)
   {
      $outfile = $caller->outputFileName($file,$baseOutDir,$pathDelimiter);
      
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
   
   function start()
   {
      ob_start(array($this, 'ob_file_callback'));
      $this->buffering = true;
   }
   
   function pause()
   {
      if($this->buffering == true)
         ob_end_flush();
   }
   
   function resume()
   {
      if($this->buffering == true)
         $this->start();
   }
   
   public function close()
   {
      $this->buffering=false;
      ob_end_flush();
      fclose($this->ob_file);
   }

   function ob_file_callback($buffer)
   {  
      fwrite($this->ob_file,$buffer);
   }
}

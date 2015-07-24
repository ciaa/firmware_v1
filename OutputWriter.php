<?php

abstract class OutputWriter
{
   protected $buffering = false;
   
   protected $log = null;

   abstract function close();
   
   abstract function ob_file_callback($buffer);
   
   public function start()
   {
      ob_start(array($this, 'ob_file_callback'));
      $this->buffering = true;
   }
   
   public function pause()
   {
      if($this->buffering == true)
         ob_end_flush();
   }
   
   public function resume()
   {
      if($this->buffering == true)
         $this->start();
   }
   
   public function setLog($log)
   {
      $this->log = $log;
   }
}

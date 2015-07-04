 /********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2008, 2009 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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

/** \brief FreeOSEK Os Generated Internal Achitecture Configuration Implementation File
 **
 ** \file Os_Internal_Arch_Cfg.c
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20080713 v0.1.0 MaCe initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
<?php
foreach ($tasks as $task)
{
   print "/** \brief $task context */\n";
   print "TaskContextType TaskContext" . $task . ";\n";
}
print "\n";

?>

InterruptType InterruptTable[INTERRUPTS_COUNT] =
{
<?php
$intnames = $config->getList("/OSEK","ISR");
for ($loopi = 0; $loopi < 32; $loopi++)
{
   if ($loopi<8)
   {
      switch($loopi)
      {
         case 0:
         case 1:
         case 2:
         case 3:
            print "   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt $loopi */\n";
            break;
         case 4:
            print "   OSEK_ISR_HWTimer0, /* HW Timer 0 Interrupt handler */\n";
            break;
         case 5:
            print "   OSEK_ISR_HWTimer1, /* HW Timer 1 Interrupt handler */\n";
            break;
         case 6:
         case 7:
            print "   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt $loopi */\n";
            break;
      }
   }
   else
   {
      $flag = false;
      foreach ($intnames as $int)
      {
         $inttype = $config->getValue("/OSEK/" . $int,"INTERRUPT");
         $intcat = $config->getValue("/OSEK/" . $int,"CATEGORY");
         switch($inttype)
         {
            case "GPIO0":
               if($loopi == 8)
               {
                  if($intcat == "1")
                  {
                     print "   OSEK_ISR_$int, /* interrupt handler $loopi */\n";
                     $flag = true;
                  }
                  elseif($intcat == "2")
                  {
                     print "   OSEK_ISR2_$int, /* interrupt handler $loopi */\n";
                     $flag = true;
                  }
                  else
                  {
                     error("Interrupt $int type $inttype has an invalid category $intcat");
                  }
               }
               break;
            case "GPIO1":
               if($loopi == 9)
               {
                  if($intcat == "1")
                  {
                     print "   OSEK_ISR_$int, /* interrupt handler $loopi */\n";
                     $flag = true;
                  }
                  elseif($intcat == "2")
                  {
                     print "   OSEK_ISR2_$int, /* interrupt handler $loopi */\n";
                     $flag = true;
                  }
                  else
                  {
                     error("Interrupt $int type $inttype has an invalid category $intcat");
                  }
               }
               break;
         }
      }
      if ($flag == false)
      {
         print "   OSEK_ISR_NoHandler, /* no interrupt handler for interrupt $loopi */\n";
      }
   }
}
?>
};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


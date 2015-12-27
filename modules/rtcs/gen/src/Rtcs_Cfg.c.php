/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2015, Diego Ezequiel Vommaro
 * Copyright 2015, ACSE & CADIEEL
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

/** \brief RTCS Generated Configuration Implementation File
 **
 ** \file Rtcs_Cfg.c
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementation
 ** @{ */

/*==================[inclusions]=============================================*/
#include "Rtcs_Internal.h"
<?php
$pdefs = $config->getList("/RTCS","INCLUDE_FILE");
$count = 1;
foreach ($pdefs as $pdef)
{
   print "#include \"" . $pdef . "\"\n";
   $count++;
}

?>

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
<?php
/* get controllers */
$controllers = $config->getList("/RTCS","StateFeedback");
?>
<?php
$count = 0;
foreach ($controllers as $controller)
{
?>
/* Public function that executes the controller of the <?=$controller;?> system */
extern void Rtcs_<?=$controller;?>_<?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>ms (void)
{
   if(Rtcs_state == ACTIVE)
   {
      Rtcs_StateFeedbackRun(Rtcs_controllers_list[<?=$count;?>]->data);
   }
}

<?php
$count++;
}
?>
<?php
$count = 0;
foreach ($controllers as $controller)
{
?>
<?php
$system_type = $config->getValue("/RTCS/" . $controller, "SYSTEM_TYPE");
if ($system_type == "CONTROL_SYSTEM"): ?>
<?php
$subcount = 0;
$subcount_max = $config->getValue("/RTCS/" . $controller, "X_SIZE");
while ($subcount < $subcount_max)
{
?>
/* Input function that loads the controller reference data of the <?=$controller;?> system */
extern void Rtcs_InputRefX<?=$subcount + 1;?>_<?=$controller;?> (float *data)
{
   Rtcs_statefeedback_data_t *controller_ptr = Rtcs_controllers_list[<?=$count;?>]->data;
   controller_ptr->r[<?=$subcount;?>] = *data;
}

<?php
$subcount++;
}
?>
<?php endif ?>
<?php
$subcount = 0;
$subcount_max = $config->getValue("/RTCS/" . $controller, "Y_SIZE");
while ($subcount < $subcount_max)
{
?>
/* Input function that loads the output data of the <?=$controller;?> system */
extern void Rtcs_InputY<?=$subcount + 1;?>_<?=$controller;?> (float *data)
{
   Rtcs_statefeedback_data_t *controller_ptr = Rtcs_controllers_list[<?=$count;?>]->data;
   controller_ptr->y[<?=$subcount;?>] = *data;
}

<?php
$subcount++;
}
?>
<?php
$count++;
}
?>
<?php
$count = 0;
foreach ($controllers as $controller)
{
?>
/* Public function that executes the "worst case" of the controller of the <?=$controller;?> system */
extern void Rtcs_WorstCase_<?=$controller;?>_<?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>ms (void)
{
   if(Rtcs_state == ACTIVE)
   {
      Rtcs_StateFeedbackWorstRun(Rtcs_controllers_list[<?=$count;?>]->data);
   }
}

<?php
$count++;
}?>
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

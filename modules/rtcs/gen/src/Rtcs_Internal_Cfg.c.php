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
 ** \file Rtcs_Internal_Cfg.c
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementation
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * DeV          Diego Ezequiel Vommaro
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20150727 v0.1.0 DeV  initial version
 */

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
<?php
/* get controllers */
$controllers = $config->getList("/RTCS","StateFeedback");
?>
<?php
$count = 1;
foreach ($controllers as $controller)
{
?>

/* Data definition of the closed loop system named "<?=$controller;?>" */
<?php
  print "/* SYSTEM_TYPE = " . $config->getValue("/RTCS/" . $controller, "SYSTEM_TYPE") . " - OBSERVER_TYPE = " . $config->getValue("/RTCS/" . $controller, "OBSERVER_TYPE") . " */\n";

$system_type = $config->getValue("/RTCS/" . $controller, "SYSTEM_TYPE");
$observer_type = $config->getValue("/RTCS/" . $controller, "OBSERVER_TYPE");

if ($system_type == "CONTROL_SYSTEM" && $observer_type == "NONE"): ?>
float r_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float x_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float e_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float y_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float u_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>];
float k_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "K_MATRIX")?>};
Rtcs_ext_matrix_t matrix_array_<?=$count;?>[6];
Rtcs_statefeedback_data_t controller_<?=$count;?> = {CONTROL_SYSTEM, NONE, <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, r_data_<?=$count?>, x_data_<?=$count?>, 0, e_data_<?=$count;?>, u_data_<?=$count;?>, y_data_<?=$count;?>, k_data_<?=$count;?>, 0, 0, 0, 0, 0, 0, 0, &matrix_array_<?=$count;?>[0], &matrix_array_<?=$count?>[1], 0, &matrix_array_<?=$count?>[2], &matrix_array_<?=$count?>[3], &matrix_array_<?=$count?>[4], &matrix_array_<?=$count?>[5], 0, 0, 0, 0, 0, 0, 0, <?=$config->getValue("/RTCS/" . $controller, "SEND_FUNCTION")?>, Rtcs_ControlSystemEffort, Rtcs_NoneObserver};
<?php endif ?>
<?php if ($system_type == "CONTROL_SYSTEM" && $observer_type == "FULL"): ?>
float r_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float x_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float xo_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float e_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float u_y_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?> + <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>];
float k_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "K_MATRIX")?>};
float mf_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "FUND_MATRIX")?>};
float mt_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "TRAN_MATRIX")?>};
Rtcs_ext_matrix_t matrix_array_<?=$count;?>[10];
Rtcs_statefeedback_data_t controller_<?=$count;?> = {CONTROL_SYSTEM, FULL, <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, r_data_<?=$count?>, x_data_<?=$count?>, xo_data_<?=$count;?>, e_data_<?=$count;?>, u_y_data_<?=$count;?>, &(u_y_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>]),k_data_<?=$count;?>, u_y_data_<?=$count;?>, 0, 0, 0, mf_data_<?=$count;?>, mt_data_<?=$count;?>, 0, &matrix_array_<?=$count;?>[0], &matrix_array_<?=$count?>[1], &matrix_array_<?=$count?>[2], &matrix_array_<?=$count?>[3], &matrix_array_<?=$count?>[4], &matrix_array_<?=$count?>[5], &matrix_array_<?=$count?>[6], &matrix_array_<?=$count?>[7], 0, 0, 0, &matrix_array_<?=$count?>[8], &matrix_array_<?=$count?>[9], 0, <?=$config->getValue("/RTCS/" . $controller, "SEND_FUNCTION")?>, Rtcs_ControlSystemEffort, Rtcs_FullObserver};
<?php endif ?>
<?php if ($system_type == "CONTROL_SYSTEM" && $observer_type == "REDUCED"): ?>
float r_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float x_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float xo_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float e_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float y_u_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?> + <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>];
float k_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "K_MATRIX")?>};
float l_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?> - <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "L_MATRIX")?>};
float mf_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "FUND_MATRIX")?>};
float mt_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "TRAN_MATRIX")?>};
float xo_aux_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?> - <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>];
Rtcs_ext_matrix_t matrix_array_<?=$count;?>[12];
Rtcs_statefeedback_data_t controller_<?=$count;?> = {CONTROL_SYSTEM, REDUCED, <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>, r_data_<?=$count?>, x_data_<?=$count?>, xo_data_<?=$count;?>, e_data_<?=$count;?>, &(y_u_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>]), y_u_data_<?=$count;?>,k_data_<?=$count;?>, y_u_data_<?=$count;?>, 0, 0, l_data_<?=$count;?>, mf_data_<?=$count;?>, mt_data_<?=$count;?>, xo_aux_data_<?=$count;?>, &matrix_array_<?=$count;?>[0], &matrix_array_<?=$count?>[1], &matrix_array_<?=$count?>[2], &matrix_array_<?=$count?>[3], &matrix_array_<?=$count?>[4], &matrix_array_<?=$count?>[5], &matrix_array_<?=$count?>[6], &matrix_array_<?=$count?>[7], 0, 0, &matrix_array_<?=$count?>[8], &matrix_array_<?=$count?>[9], &matrix_array_<?=$count?>[10], &matrix_array_<?=$count?>[11], <?=$config->getValue("/RTCS/" . $controller, "SEND_FUNCTION")?>, Rtcs_ControlSystemEffort, Rtcs_ReducedObserver};
<?php endif ?>
<?php if ($system_type == "REGULATOR" && $observer_type == "NONE"): ?>
float x_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float y_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float u_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>];
float k_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "K_MATRIX")?>};
Rtcs_ext_matrix_t matrix_array_<?=$count;?>[4];
Rtcs_statefeedback_data_t controller_<?=$count;?> = {REGULATOR, NONE, <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>, 0, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, 0, <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, 0, x_data_<?=$count?>, 0, 0, u_data_<?=$count;?>, y_data_<?=$count;?>, k_data_<?=$count;?>, 0, 0, 0, 0, 0, 0, 0, 0, &matrix_array_<?=$count?>[0], 0, 0, &matrix_array_<?=$count?>[1], &matrix_array_<?=$count?>[2], &matrix_array_<?=$count?>[3], 0, 0, 0, 0, 0, 0, 0, <?=$config->getValue("/RTCS/" . $controller, "SEND_FUNCTION")?>, Rtcs_RegulatorControlEffort, Rtcs_NoneObserver};
<?php endif ?>
<?php if ($system_type == "REGULATOR" && $observer_type == "FULL"): ?>
float x_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float xo_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float u_y_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?> + <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>];
float k_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "K_MATRIX")?>};
float mf_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "FUND_MATRIX")?>};
float mt_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "TRAN_MATRIX")?>};
Rtcs_ext_matrix_t matrix_array_<?=$count;?>[8];
Rtcs_statefeedback_data_t controller_<?=$count;?> = {REGULATOR, FULL, <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>, 0, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, 0, <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, 0, x_data_<?=$count?>, xo_data_<?=$count;?>, 0, u_y_data_<?=$count;?>, &(u_y_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>]),k_data_<?=$count;?>, u_y_data_<?=$count;?>, 0, 0, 0, mf_data_<?=$count;?>, mt_data_<?=$count;?>, 0, 0, &matrix_array_<?=$count?>[0], &matrix_array_<?=$count?>[1], 0, &matrix_array_<?=$count?>[2], &matrix_array_<?=$count?>[3], &matrix_array_<?=$count?>[4], &matrix_array_<?=$count?>[5], 0, 0, 0, &matrix_array_<?=$count?>[6], &matrix_array_<?=$count?>[7], 0, <?=$config->getValue("/RTCS/" . $controller, "SEND_FUNCTION")?>, Rtcs_RegulatorControlEffort, Rtcs_FullObserver};
<?php endif ?>
<?php if ($system_type == "REGULATOR" && $observer_type == "REDUCED"): ?>
float x_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float xo_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>];
float y_u_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?> + <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>];
float k_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "K_MATRIX")?>};
float l_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?> - <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>] = {<?=$config->getValue("/RTCS/" . $controller, "L_MATRIX")?>};
float mf_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "FUND_MATRIX")?>};
float mt_data_<?=$count;?>[] = {<?=$config->getValue("/RTCS/" . $controller, "TRAN_MATRIX")?>};
float xo_aux_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?> - <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>];
Rtcs_ext_matrix_t matrix_array_<?=$count;?>[10];
Rtcs_statefeedback_data_t controller_<?=$count;?> = {REGULATOR, REDUCED, <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>, 0, <?=$config->getValue("/RTCS/" . $controller, "X_SIZE")?>, 0, <?=$config->getValue("/RTCS/" . $controller, "U_SIZE")?>, <?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>, 0, x_data_<?=$count?>, xo_data_<?=$count;?>, 0, &(y_u_data_<?=$count;?>[<?=$config->getValue("/RTCS/" . $controller, "Y_SIZE")?>]), y_u_data_<?=$count;?>,k_data_<?=$count;?>, y_u_data_<?=$count;?>, 0, 0, l_data_<?=$count;?>, mf_data_<?=$count;?>, mt_data_<?=$count;?>, xo_aux_data_<?=$count;?>, 0, &matrix_array_<?=$count?>[0], &matrix_array_<?=$count?>[1], 0, &matrix_array_<?=$count?>[2], &matrix_array_<?=$count?>[3], &matrix_array_<?=$count?>[4], &matrix_array_<?=$count?>[5], 0, 0, &matrix_array_<?=$count?>[6], &matrix_array_<?=$count?>[7], &matrix_array_<?=$count?>[8], &matrix_array_<?=$count?>[9], <?=$config->getValue("/RTCS/" . $controller, "SEND_FUNCTION")?>, Rtcs_RegulatorControlEffort, Rtcs_ReducedObserver};
<?php endif ?>
<?php
    $count++;
}

?>

/* Definition of the Generic Controller Data */
<?php
$count = 1;
foreach ($controllers as $controller)
{
?>Rtcs_generic_controller_t Rtcs_controllers_data_<?=$count;?> = {Rtcs_StateFeedbackFirstRun, &controller_<?=$count;?>};
<?php
$count++;
}?>

/* Definition of the Controllers List wich has <?=count($controllers);?> elements */
Rtcs_generic_controller_t *Rtcs_controllers_list[CONTROLLERS_LIST_SIZE] = {<?php
$count = 1;
foreach ($controllers as $controller)
{
?>&Rtcs_controllers_data_<?=$count;?>
<?php if ($count < count($controllers)): ?>
, <?php endif ?>
<?php
$count++;
}?>};

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

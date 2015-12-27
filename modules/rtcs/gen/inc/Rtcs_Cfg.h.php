/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2015, Diego Ezequiel Vommaro
 * Copyright 2015, ACSE & CADIEEL
 *    ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
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

#ifndef RTCS_CFG_H
#define RTCS_CFG_H

/** \brief RTCS Generated Configuration Implementation File
 **
 ** \file Rtcs_Cfg.h
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup RTCS RTCS Implementatio
 ** @{ */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
<?php
/* get controllers */
$controllers = $config->getList("/RTCS","StateFeedback");
?>
<?php
$count = 0;
foreach ($controllers as $controller)
{
?>
/** \brief User's function that should be called every <?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>ms
 **
 ** Public function that executes the controller of the <?=$controller;?> system
 **/
void Rtcs_<?=$controller;?>_<?=$config->getValue("/RTCS/" . $controller, "PERIOD")?>ms (void);

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
/** \brief User's funtion that loads reference data into controller data structure
 **
 ** Input function that loads the controller reference data of the <?=$controller;?> system
 **
 ** \param[in] data pointer to float data to load into controller data structure
 **/
extern void Rtcs_InputRefX<?=$subcount + 1;?>_<?=$controller;?> (float *data);

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
/** \brief User's funtion that loads output data of system into controller data structure
 **
 ** Input function that loads the output data of the <?=$controller;?> system
 **
 ** \param[in] data pointer to float data to load into controller data structure
 **/
extern void Rtcs_InputY<?=$subcount + 1;?>_<?=$controller;?> (float *data);

<?php
$subcount++;
}
?>
<?php
$count++;
}?>
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef RTCS_CFG_H_ */


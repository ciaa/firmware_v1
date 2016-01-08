<?php
/* Copyright 2015, Mariano Cerdeiro
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

/** \brief IO DIO Driver source file
 **
 ** This file contains is the Dio driver
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup HISIO HisIO Module
 ** @{ */

class Hisio extends Hisio_Base {

   init() {
      $dio_pins = array (
         /* port GPIO0 */
         "GPIO0[0]" => "P0_0",
         "GPIO0[1]" => "P0_1",
         "GPIO0[2]" => "P1_15",
         "GPIO0[3]" => "P1_16",
         "GPIO0[4]" => "P1_0",
         "GPIO0[5]" => "P6_6",
         "GPIO0[6]" => "P3_6",
         "GPIO0[7]" => "P2_7",
         "GPIO0[8]" => "P1_1",
         "GPIO0[9]" => "P1_2",
         "GPIO0[10]" => "P1_3",
         "GPIO0[11]" => "P1_4",
         "GPIO0[12]" => "P1_17",
         "GPIO0[13]" => "P1_18",
         "GPIO0[14]" => "P2_10",
         "GPIO0[15]" => "P1_20",
         /* port GPIO1 */
         "GPIO1[0]" => "P1_7",
         "GPIO1[1]" => "P1_8",
         "GPIO1[2]" => "P1_9",
         "GPIO1[3]" => "P1_10",
         "GPIO1[4]" => "P1_11",
         "GPIO1[5]" => "P1_12",
         "GPIO1[6]" => "P1_13",
         "GPIO1[7]" => "P1_14",
         "GPIO1[8]" => "P1_5",
         "GPIO1[9]" => "P1_6",
         "GPIO1[10]" => "P2_9",
         "GPIO1[11]" => "P2_11",
         "GPIO1[12]" => "P2_12",
         "GPIO1[13]" => "P2_13",
         "GPIO1[14]" => "P3_4",
         "GPIO1[15]" => "P3_5",
      );
      $this->addDioPins($dio_pins);

      $dio_ports = array (
         "GPIO0" => array (
            "P0_0",
            "P0_1",
            "P1_15",
            "P1_16",
            "P1_0",
            "P6_6",
            "P3_6",
            "P2_7",
            "P1_1",
            "P1_2",
            "P1_3",
            "P1_4",
            "P1_17",
            "P1_18",
            "P2_10",
            "P1_20"
         )
         "GPIO1" => array (
            "P1_7",
            "P1_8",
            "P1_9",
            "P1_10",
            "P1_11",
            "P1_12",
            "P1_13",
            "P1_14",
            "P1_5",
            "P1_6",
            "P2_9",
            "P2_11",
            "P2_12",
            "P2_13",
            "P3_4",
            "P3_5",
         )
      );
      $this->addDioPorts($dio_ports);

      $pwm_ports = array (
         "PWM0" => "P4_0",
         "PWM1" => "P5_5"
      );
      $this->addPwm($pwm_ports);

      $uart_ports = array (
         "U0" => array (
            "P2_0",  #TXD
            "P2_1",  #RXD
            "P2_2",  #UCLK
            "P2_6",  #DIR
            "P6_1",  #UCLK
            "P6_2",  #DIR
            "P6_4",  #TXD
            "P6_5",  #RXD
            "P9_5",  #TXD
            "P9_6",  #RXD
            "PF_8",  #UCLK
            "PF_9",  #DIR
            "PF_10", #TXD
            "PF_11", #RXD)
         )
      );


      $adc_ports = array (
         "ADC0_1" => "P4_1",
         "ADC0_0" => "P4_3",
         "ADC0_4" => "P7_4",
         "ADC0_3" => "P7_5",
      )
      $this->addAdc($adc_ports);
   }


}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
?>

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

class Hisio_Base {
}

class Hisio extends Hisio_Base {

   public $dio_pins;
   function __construct() {
      $this->init();
   }
   function init() {
      $this->dio_pins = array (
         /* port GPIO0 */
         "GPIO0[0],FUNC0" => "P0_0",
         "GPIO0[1],FUNC0" => "P0_1",
         "GPIO0[2],FUNC0" => "P1_15",
         "GPIO0[3],FUNC0" => "P1_16",
         "GPIO0[4],FUNC0" => "P1_0",
         "GPIO0[5],FUNC0" => "P6_6",
         "GPIO0[6],FUNC0" => "P3_6",
         "GPIO0[7],FUNC0" => "P2_7",
         "GPIO0[8],FUNC0" => "P1_1",
         "GPIO0[9],FUNC0" => "P1_2",
         "GPIO0[10],FUNC0" => "P1_3",
         "GPIO0[11],FUNC0" => "P1_4",
         "GPIO0[12],FUNC0" => "P1_17",
         "GPIO0[13],FUNC0" => "P1_18",
         "GPIO0[14],FUNC0" => "P2_10",
         "GPIO0[15],FUNC0" => "P1_20",
         /* port GPIO1 */
         "GPIO1[0],FUNC0" => "P1_7",
         "GPIO1[1],FUNC0" => "P1_8",
         "GPIO1[2],FUNC0" => "P1_9",
         "GPIO1[3],FUNC0" => "P1_10",
         "GPIO1[4],FUNC0" => "P1_11",
         "GPIO1[5],FUNC0" => "P1_12",
         "GPIO1[6],FUNC0" => "P1_13",
         "GPIO1[7],FUNC0" => "P1_14",
         "GPIO1[8],FUNC0" => "P1_5",
         "GPIO1[9],FUNC0" => "P1_6",
         "GPIO1[10],FUNC0" => "P2_9",
         "GPIO1[11],FUNC0" => "P2_11",
         "GPIO1[12],FUNC0" => "P2_12",
         "GPIO1[13],FUNC0" => "P2_13",
         "GPIO1[14],FUNC0" => "P3_4",
         "GPIO1[15],FUNC0" => "P3_5",
         /* port GPIO2 */
         "GPIO2[0],FUNC0" => "P4_0",
         "GPIO2[1],FUNC0" => "P4_1",
         "GPIO2[2],FUNC0" => "P4_2",
         "GPIO2[3],FUNC0" => "P4_3",
         "GPIO2[4],FUNC0" => "P4_4",
         "GPIO2[5],FUNC0" => "P4_5",
         "GPIO2[6],FUNC0" => "P4_6",        
         "GPIO2[7],FUNC0" => "P5_7",        
         "GPIO2[8],FUNC0" => "P6_12",        
         "GPIO2[9],FUNC0" => "P5_0",        
         "GPIO2[10],FUNC0" => "P5_1",        
         "GPIO2[11],FUNC0" => "P5_2",        
         "GPIO2[12],FUNC0" => "P5_3",        
         "GPIO2[13],FUNC0" => "P5_4",        
         "GPIO2[14],FUNC0" => "P5_5",        
         "GPIO2[15],FUNC0" => "P5_6",        
         /* port GPIO3 */
         "GPIO3[0],FUNC0" => "P6_1",
         "GPIO3[1],FUNC0" => "P6_2",
         "GPIO3[2],FUNC0" => "P6_3",
         "GPIO3[3],FUNC0" => "P6_4",
         "GPIO3[4],FUNC0" => "P6_5",
         "GPIO3[5],FUNC0" => "P6_9",
         "GPIO3[6],FUNC0" => "P6_10",
         "GPIO3[7],FUNC0" => "P6_11",
         "GPIO3[8],FUNC0" => "P7_0",
         "GPIO3[9],FUNC0" => "P7_1",
         "GPIO3[10],FUNC0" => "P7_2",
         "GPIO3[11],FUNC0" => "P7_3",
         "GPIO3[12],FUNC0" => "P7_4",
         "GPIO3[13],FUNC0" => "P7_5",
         "GPIO3[14],FUNC0" => "P7_6",
         "GPIO3[15],FUNC0" => "P7_7",
         /* port GPIO4 */
         "GPIO4[0],FUNC0" => "P8_0",
         "GPIO4[1],FUNC0" => "P8_1",
         "GPIO4[2],FUNC0" => "P8_2",
         "GPIO4[3],FUNC0" => "P8_3",
         "GPIO4[4],FUNC0" => "P8_4",
         "GPIO4[5],FUNC0" => "P8_5",
         "GPIO4[6],FUNC0" => "P8_6",
         "GPIO4[7],FUNC0" => "P8_7",
         "GPIO4[8],FUNC0" => "PA_1",
         "GPIO4[9],FUNC0" => "PA_2",
         "GPIO4[10],FUNC0" => "PA_3",
         "GPIO4[11],FUNC0" => "P9_6",
         "GPIO4[12],FUNC0" => "P9_0",
         "GPIO4[13],FUNC0" => "P9_1",
         "GPIO4[14],FUNC0" => "P9_2",
         "GPIO4[15],FUNC0" => "P9_3",
         /* port GPIO5 */
         "GPIO5[0],FUNC4" => "P2_0",
         "GPIO5[1],FUNC4" => "P2_1",
         "GPIO5[2],FUNC4" => "P2_2",
         "GPIO5[3],FUNC4" => "P2_3",
         "GPIO5[4],FUNC4" => "P2_4",
         "GPIO5[5],FUNC4" => "P2_5",
         "GPIO5[6],FUNC4" => "P2_6",
         "GPIO5[7],FUNC4" => "P2_8",
         "GPIO5[8],FUNC4" => "P3_1",
         "GPIO5[9],FUNC4" => "P3_2",
         "GPIO5[10],FUNC4" => "P3_7",
         "GPIO5[11],FUNC4" => "P3_8",
         "GPIO5[12],FUNC4" => "P4_8",
         "GPIO5[13],FUNC4" => "P4_9",
         "GPIO5[14],FUNC4" => "P4_10",
         "GPIO5[15],FUNC4" => "P6_7",
         "GPIO5[16],FUNC4" => "P6_8",
         "GPIO5[17],FUNC4" => "P9_4",
         "GPIO5[18],FUNC4" => "P9_5",
         "GPIO5[19],FUNC4" => "PA_4",
         "GPIO5[20],FUNC4" => "PB_0",
         "GPIO5[21],FUNC4" => "PB_1",
         "GPIO5[22],FUNC4" => "PB_2",
         "GPIO5[23],FUNC4" => "PB_3",
         "GPIO5[24],FUNC4" => "PB_4",
         "GPIO5[25],FUNC4" => "PB_5",
         "GPIO5[26],FUNC4" => "PB_6",
         /* port GPIO6 */
         "GPIO6[0],FUNC4" => "PC_1",
         "GPIO6[1],FUNC4" => "PC_2",
         "GPIO6[2],FUNC4" => "PC_3",
         "GPIO6[3],FUNC4" => "PC_4",
         "GPIO6[4],FUNC4" => "PC_5",
         "GPIO6[5],FUNC4" => "PC_6",
         "GPIO6[6],FUNC4" => "PC_7",
         "GPIO6[7],FUNC4" => "PC_8",
         "GPIO6[8],FUNC4" => "PC_9",
         "GPIO6[9],FUNC4" => "PC_10",
         "GPIO6[10],FUNC4" => "PC_11",
         "GPIO6[11],FUNC4" => "PC_12",
         "GPIO6[12],FUNC4" => "PC_13",
         "GPIO6[13],FUNC4" => "PC_14",
         "GPIO6[14],FUNC4" => "PD_0",
         "GPIO6[15],FUNC4" => "PD_1",
         "GPIO6[16],FUNC4" => "PD_2",
         "GPIO6[17],FUNC4" => "PD_3",
         "GPIO6[18],FUNC4" => "PD_4",
         "GPIO6[19],FUNC4" => "PD_5",
         "GPIO6[20],FUNC4" => "PD_6",
         "GPIO6[21],FUNC4" => "PD_7",
         "GPIO6[22],FUNC4" => "PD_8",
         "GPIO6[23],FUNC4" => "PD_9",
         "GPIO6[24],FUNC4" => "PD_10",
         "GPIO6[25],FUNC4" => "PD_11",
         "GPIO6[26],FUNC4" => "PD_12",
         "GPIO6[27],FUNC4" => "PD_13",
         "GPIO6[28],FUNC4" => "PD_14",
         "GPIO6[29],FUNC4" => "PD_15",
         "GPIO6[30],FUNC4" => "PD_16",
         /* port GPIO7 */
         "GPIO7[0],FUNC4" => "PE_0",
         "GPIO7[1],FUNC4" => "PE_1",
         "GPIO7[2],FUNC4" => "PE_2",
         "GPIO7[3],FUNC4" => "PE_3",
         "GPIO7[4],FUNC4" => "PE_4",
         "GPIO7[5],FUNC4" => "PE_5",
         "GPIO7[6],FUNC4" => "PE_6",
         "GPIO7[7],FUNC4" => "PE_7",
         "GPIO7[8],FUNC4" => "PE_8",
         "GPIO7[9],FUNC4" => "PE_9",
         "GPIO7[10],FUNC4" => "PE_0",
         "GPIO7[11],FUNC4" => "PE_1",
         "GPIO7[12],FUNC4" => "PE_2",
         "GPIO7[13],FUNC4" => "PE_3",
         "GPIO7[14],FUNC4" => "PE_4",
         "GPIO7[15],FUNC4" => "PE_15",
         "GPIO7[16],FUNC4" => "PF_1",
         "GPIO7[17],FUNC4" => "PF_2",
         "GPIO7[18],FUNC4" => "PF_3",
         "GPIO7[19],FUNC4" => "PF_5",
         "GPIO7[20],FUNC4" => "PF_6",
         "GPIO7[21],FUNC4" => "PF_7",
         "GPIO7[22],FUNC4" => "PF_8",
         "GPIO7[23],FUNC4" => "PF_9",
         "GPIO7[24],FUNC4" => "PF_10",
         "GPIO7[25],FUNC4" => "PF_11"
      );
/*      $this->addDioPins($dio_pins); */

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
         ),
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
            "P3_5"
         ),
         "GPIO2" => array (
            "P4_0",
            "P4_1",
            "P4_2",
            "P4_3",
            "P4_4",
            "P4_5",
            "P4_6",        
            "P5_7",        
            "P6_12",        
            "P5_0",        
            "P5_1",        
            "P5_2",        
            "P5_3",        
            "P5_4",        
            "P5_5",        
            "P5_6"      
         ),
         "GPIO3" => array (
            "P6_1",
            "P6_2",
            "P6_3",
            "P6_4",
            "P6_5",
            "P6_9",
            "P6_10",
            "P6_11",
            "P7_0",
            "P7_1",
            "P7_2",
            "P7_3",
            "P7_4",
            "P7_5",
            "P7_6",
            "P7_7"
         ),
         "GPIO4" => array (
            "P8_0",
            "P8_1",
            "P8_2",
            "P8_3",
            "P8_4",
            "P8_5",
            "P8_6",
            "P8_7",
            "PA_1",
            "PA_2",
            "PA_3",
            "P9_6",
            "P9_0",
            "P9_1",
            "P9_2",
            "P9_3"
         ),
         "GPIO5" => array (
            "P2_0",
            "P2_1",
            "P2_2",
            "P2_3",
            "P2_4",
            "P2_5",
            "P2_6",
            "P2_8",
            "P3_1",
            "P3_2",
            "P3_7",
            "P3_8",
            "P4_8",
            "P4_9",
            "P4_10",
            "P6_7",
            "P6_8",
            "P9_4",
            "P9_5",
            "PA_4",
            "PB_0",
            "PB_1",
            "PB_2",
            "PB_3",
            "PB_4",
            "PB_5",
            "PB_6"
         ),
         "GPIO6" => array (
            "PC_1",
            "PC_2",
            "PC_3",
            "PC_4",
            "PC_5",
            "PC_6",
            "PC_7",
            "PC_8",
            "PC_9",
            "PC_10",
            "PC_11",
            "PC_12",
            "PC_13",
            "PC_14",
            "PD_0",
            "PD_1",
            "PD_2",
            "PD_3",
            "PD_4",
            "PD_5",
            "PD_6",
            "PD_7",
            "PD_8",
            "PD_9",
            "PD_10",
            "PD_11",
            "PD_12",
            "PD_13",
            "PD_14",
            "PD_15",
            "PD_16"
         ),
         "GPIO7" => array (
            "PE_0",
            "PE_1",
            "PE_2",
            "PE_3",
            "PE_4",
            "PE_5",
            "PE_6",
            "PE_7",
            "PE_8",
            "PE_9",
            "PE_0",
            "PE_1",
            "PE_2",
            "PE_3",
            "PE_4",
            "PE_15",
            "PF_1",
            "PF_2",
            "PF_3",
            "PF_5",
            "PF_6",
            "PF_7",
            "PF_8",
            "PF_9",
            "PF_10",
            "PF_11"
         )
      );
/*      $this->addDioPorts($dio_ports); */

      $pwm_ports = array (
         "PWM0" => "P4_0",
         "PWM1" => "P5_5"
      );
 /*     $this->addPwm($pwm_ports); */

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
            "PF_11"  #RXD
         ),
         "U1" => array (
            "Algo",  #Algo...
         )
      );

      $adc_ports = array (
         "ADC0_1" => "P4_1",
         "ADC0_0" => "P4_3",
         "ADC0_4" => "P7_4",
         "ADC0_3" => "P7_5",
      );
/*      $this->addAdc($adc_ports); */
   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
?>

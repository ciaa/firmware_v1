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
 ** \file cortexM4/Os_Internal_Arch_Cfg.c
 ** \arch cortexM4
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
 * MaCe			 Mariano Cerdeiro
 * PR           Pablo Ridolfi
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * v0.1.1 20141115 PR   added LPC43xx interrupt sources, spelling mistake fixed
 * v0.1.0 20141115 MaCe	initial version
 */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/* ResetISR is defined in cr_startup_lpc43xx.c */
extern void ResetISR(void);

/** \brief External declaration for the pointer to the stack top from the Linker Script */
extern void _vStackTop(void);

/** \brief Handlers used by OSEK */
extern void SysTick_Handler(void);
extern void PendSV_Handler(void);

/*==================[internal functions definition]==========================*/

/* Default exception handlers. */
__attribute__ ((section(".after_vectors")))
void NMI_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors")))
void HardFault_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors")))
void MemManage_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors")))
void BusFault_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors")))
void UsageFault_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors")))
void SVC_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors")))
void DebugMon_Handler(void) {
    while (1) {
    }
}

/** \brief ISR default handler */
__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void) {
    while (1) {
    }
}

/*==================[external functions definition]==========================*/

<?php
/* Interrupt sources for LPC43xx. 
 * See externals/platforms/cortexM4/lpc43xx/src/cr_startup_lpc43xx.c.
 */
$intList = array (
   0 => "DAC",
   1 => "M0APP",
   2 => "DMA",
   3 => "RESERVED1",
   4 => "FLASH_EEPROM",
   5 => "ETH",
   6 => "SDIO",
   7 => "LCD",
   8 => "USB0",
   9 => "USB1",
   10 => "SCT",
   11 => "RIT",
   12 => "TIMER0",
   13 => "TIMER1",
   14 => "TIMER2",
   15 => "TIMER3",
   16 => "MCPWM",
   17 => "ADC0",
   18 => "I2C0",
   19 => "I2C1",
   20 => "SPI",
   21 => "ADC1",
   22 => "SSP0",
   23 => "SSP1",
   24 => "UART0",
   25 => "UART1",
   26 => "UART2",
   27 => "UART3",
   28 => "I2S0",
   29 => "I2S1",
   30 => "SPIFI",
   31 => "SGPIO",
   32 => "GPIO0",
   33 => "GPIO1",
   34 => "GPIO2",
   35 => "GPIO3",
   36 => "GPIO4",
   37 => "GPIO5",
   38 => "GPIO6",
   39 => "GPIO7",
   40 => "GINT0",
   41 => "GINT1",
   42 => "EVRT",
   43 => "CAN1",
   44 => "RESERVED6",
   45 => "ADCHS",
   46 => "ATIMER",
   47 => "RTC",
   48 => "RESERVED8",
   49 => "WDT",
   50 => "M0SUB",
   51 => "CAN0",
   52 => "QEI"
);

$MAX_INT_COUNT = 53;
?>
/** \brief LPC4337 Interrupt vector */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
   // Core Level - CM4
   &_vStackTop,                    // The initial stack pointer
   ResetISR,                       // The reset handler
   NMI_Handler,                    // The NMI handler
   HardFault_Handler,              // The hard fault handler
   MemManage_Handler,              // The MPU fault handler
   BusFault_Handler,               // The bus fault handler
   UsageFault_Handler,             // The usage fault handler
   0,                              // Reserved
   0,                              // Reserved
   0,                              // Reserved
   0,                              // Reserved
   SVC_Handler,                    // SVCall handler
   DebugMon_Handler,               // Debug monitor handler
   0,                              // Reserved
   PendSV_Handler,                 // The PendSV handler
   SysTick_Handler,                // The SysTick handler

   /* Chip Level - LPC43xx (M4 core) */
<?php

/* get ISRs defined by user application */
$intnames = $config->getList("/OSEK","ISR");

for($i=0; $i < $MAX_INT_COUNT; $i++)
{
   $src_found = 0;
   foreach ($intnames as $int)
   {
      $intcat = $config->getValue("/OSEK/" . $int,"CATEGORY");
      $source = $config->getValue("/OSEK/" . $int,"INTERRUPT");

      if(($intList[$i] == $source) && ($intcat == 2))
      {
         print "   OSEK_ISR2_$int, /* ISR for " . $intList[$i] . " */ \n";
         $src_found = 1;
      }
   }
   if($src_found == 0)
   {
      print "   IntDefaultHandler, /* ISR for " . $intList[$i] . " */ \n";
   }
}
?>
};

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


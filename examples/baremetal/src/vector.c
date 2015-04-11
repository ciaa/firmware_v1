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

/** \brief Vector Interrupt tables
 **
 ** This file contains the vector interrupt tables
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Vector Vector interrupt tables
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "baremetal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/
/** \brief Handler for NMI */
static void NMI_Handler(void);

/** \brief Handler for HardFault */
static void HardFault_Handler(void);

/** \brief Handler for Memory Manager */
static void MemManage_Handler(void);

/** \brief Handler for Bus Fault */
static void BusFault_Handler(void);

/** \brief Handler for Usage Fault */
static void UsageFault_Handler(void);

/** \brief Handler for SVC */
static void SVC_Handler(void);

/** \brief Handler for Debug Monitor */
static void DebugMon_Handler(void);

/** \brief System Tick Handler */
static void SysTick_Handler(void);

/** \brief Dummy empty ISR Handler */
static void ISR_NoHandler(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
#ifndef CPU
#error CPU shall be defined.
#endif

#if (CPU == mk60fx512vlq15)
/** \brief mk60fx512vlq15 Interrupt vector */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
   /* System ISRs */
   &__StackTop,                    /* The initial stack pointer  */
   Reset_Handler,                  /* The reset handler          */
   NMI_Handler,                    /* The NMI handler            */
   HardFault_Handler,              /* The hard fault handler     */
   MemManage_Handler,              /* The MPU fault handler      */
   BusFault_Handler,               /* The bus fault handler      */
   UsageFault_Handler,             /* The usage fault handler    */
   0,                              /* Reserved                   */
   0,                              /* Reserved                   */
   0,                              /* Reserved                   */
   0,                              /* Reserved                   */
   SVC_Handler,                    /* SVCall handler             */
   DebugMon_Handler,               /* Debug monitor handler      */
   0,                              /* Reserved                   */
   0,                              /* The PendSV handler         */
   SysTick_Handler,                /* The SysTick handler        */
   /*** User Interruptions ***/
   ISR_NoHandler,      /* 0x10 0x00000040 - No Handler set for ISR DMA0_DMA16 (IRQ 0) */
   ISR_NoHandler,      /* 0x11 0x00000044 - No Handler set for ISR DMA1_DMA17 (IRQ 1) */
   ISR_NoHandler,      /* 0x12 0x00000048 - No Handler set for ISR DMA2_DMA18 (IRQ 2) */
   ISR_NoHandler,      /* 0x13 0x0000004C - No Handler set for ISR DMA3_DMA19 (IRQ 3) */
   ISR_NoHandler,      /* 0x14 0x00000050 - No Handler set for ISR DMA4_DMA20 (IRQ 4) */
   ISR_NoHandler,      /* 0x15 0x00000054 - No Handler set for ISR DMA5_DMA21 (IRQ 5) */
   ISR_NoHandler,      /* 0x16 0x00000058 - No Handler set for ISR DMA6_DMA22 (IRQ 6) */
   ISR_NoHandler,      /* 0x17 0x0000005C - No Handler set for ISR DMA7_DMA23 (IRQ 7) */
   ISR_NoHandler,      /* 0x18 0x00000060 - No Handler set for ISR DMA8_DMA24 (IRQ 8) */
   ISR_NoHandler,      /* 0x19 0x00000064 - No Handler set for ISR DMA9_DMA25 (IRQ 9) */
   ISR_NoHandler,      /* 0x1a 0x00000068 - No Handler set for ISR DMA10_DMA26 (IRQ 10) */
   ISR_NoHandler,      /* 0x1b 0x0000006C - No Handler set for ISR DMA11_DMA27 (IRQ 11) */
   ISR_NoHandler,      /* 0x1c 0x00000070 - No Handler set for ISR DMA12_DMA28 (IRQ 12) */
   ISR_NoHandler,      /* 0x1d 0x00000074 - No Handler set for ISR DMA13_DMA29 (IRQ 13) */
   ISR_NoHandler,      /* 0x1e 0x00000078 - No Handler set for ISR DMA14_DMA30 (IRQ 14) */
   ISR_NoHandler,      /* 0x1f 0x0000007C - No Handler set for ISR DMA15_DMA31 (IRQ 15) */
   ISR_NoHandler,      /* 0x20 0x00000080 - No Handler set for ISR DMA_ERR (IRQ 16) */
   ISR_NoHandler,      /* 0x21 0x00000084 - No Handler set for ISR MCM (IRQ 17) */
   ISR_NoHandler,      /* 0x22 0x00000088 - No Handler set for ISR FTFE (IRQ 18) */
   ISR_NoHandler,      /* 0x23 0x0000008C - No Handler set for ISR Read_Collision (IRQ 19) */
   ISR_NoHandler,      /* 0x24 0x00000090 - No Handler set for ISR LVD_LVW (IRQ 20) */
   ISR_NoHandler,      /* 0x25 0x00000094 - No Handler set for ISR LLW (IRQ 21) */
   ISR_NoHandler,      /* 0x26 0x00000098 - No Handler set for ISR WDG (IRQ 22) */
   ISR_NoHandler,      /* 0x27 0x0000009C - No Handler set for ISR RNG (IRQ 23) */
   ISR_NoHandler,      /* 0x28 0x000000A0 - No Handler set for ISR I2C0 (IRQ 24) */
   ISR_NoHandler,      /* 0x29 0x000000A4 - No Handler set for ISR I2C1 (IRQ 25) */
   ISR_NoHandler,      /* 0x2a 0x000000A8 - No Handler set for ISR SPI0 (IRQ 26) */
   ISR_NoHandler,      /* 0x2b 0x000000AC - No Handler set for ISR SPI1 (IRQ 27) */
   ISR_NoHandler,      /* 0x2c 0x000000B0 - No Handler set for ISR SPI2 (IRQ 28) */
   ISR_NoHandler,      /* 0x2d 0x000000B4 - No Handler set for ISR CAN0_READ (IRQ 29) */
   ISR_NoHandler,      /* 0x2e 0x000000B8 - No Handler set for ISR CAN0_BOFF (IRQ 30) */
   ISR_NoHandler,      /* 0x2f 0x000000BC - No Handler set for ISR CAN0_ERR (IRQ 31) */
   ISR_NoHandler,      /* 0x30 0x000000C0 - No Handler set for ISR CAN0_TXW (IRQ 32) */
   ISR_NoHandler,      /* 0x31 0x000000C4 - No Handler set for ISR CAN0_RXW (IRQ 33) */
   ISR_NoHandler,      /* 0x32 0x000000C8 - No Handler set for ISR CAN0_WAKEUP (IRQ 34) */
   ISR_NoHandler,      /* 0x33 0x000000CC - No Handler set for ISR I2S0_TX (IRQ 35) */
   ISR_NoHandler,      /* 0x34 0x000000D0 - No Handler set for ISR I2S0_RR (IRQ 36) */
   ISR_NoHandler,      /* 0x35 0x000000D4 - No Handler set for ISR CAN1_READ (IRQ 37) */
   ISR_NoHandler,      /* 0x36 0x000000D8 - No Handler set for ISR CAN1_BOFF (IRQ 38) */
   ISR_NoHandler,      /* 0x37 0x000000DC - No Handler set for ISR CAN1_EERROR (IRQ 39) */
   ISR_NoHandler,      /* 0x38 0x000000E0 - No Handler set for ISR CAN1_TXW (IRQ 40) */
   ISR_NoHandler,      /* 0x39 0x000000E4 - No Handler set for ISR CAN1_RXW (IRQ 41) */
   ISR_NoHandler,      /* 0x3a 0x000000E8 - No Handler set for ISR CAN1_WAKEUP (IRQ 42) */
   ISR_NoHandler,      /* 0x3b 0x000000EC - No Handler set for ISR RES59 (IRQ 43) */
   ISR_NoHandler,      /* 0x3c 0x000000F0 - No Handler set for ISR UART0_LON (IRQ 44) */
   ISR_NoHandler,      /* 0x3d 0x000000F4 - No Handler set for ISR UART0 (IRQ 45) */
   ISR_NoHandler,      /* 0x3e 0x000000F8 - No Handler set for ISR UART0_ERR (IRQ 46) */
   ISR_NoHandler,      /* 0x3f 0x000000FC - No Handler set for ISR UART1 (IRQ 47) */
   ISR_NoHandler,      /* 0x40 0x00000100 - No Handler set for ISR UART1_ERR (IRQ 48) */
   ISR_NoHandler,      /* 0x41 0x00000104 - No Handler set for ISR UART2 (IRQ 49) */
   ISR_NoHandler,      /* 0x42 0x00000108 - No Handler set for ISR UART2_ERR (IRQ 50) */
   ISR_NoHandler,      /* 0x43 0x0000010C - No Handler set for ISR UART3 (IRQ 51) */
   ISR_NoHandler,      /* 0x44 0x00000110 - No Handler set for ISR UART3_ERR (IRQ 52) */
   ISR_NoHandler,      /* 0x45 0x00000114 - No Handler set for ISR UART4 (IRQ 53) */
   ISR_NoHandler,      /* 0x46 0x00000118 - No Handler set for ISR UART4_ERR (IRQ 54) */
   ISR_NoHandler,      /* 0x47 0x0000011C - No Handler set for ISR UART5 (IRQ 55) */
   ISR_NoHandler,      /* 0x48 0x00000120 - No Handler set for ISR UART5_ERR (IRQ 56) */
   ISR_NoHandler,      /* 0x49 0x00000124 - No Handler set for ISR ADC0 (IRQ 57) */
   ISR_NoHandler,      /* 0x4a 0x00000128 - No Handler set for ISR ADC1 (IRQ 58) */
   ISR_NoHandler,      /* 0x4b 0x0000012C - No Handler set for ISR CMP0 (IRQ 59) */
   ISR_NoHandler,      /* 0x4c 0x00000130 - No Handler set for ISR CMP1 (IRQ 60) */
   ISR_NoHandler,      /* 0x4d 0x00000134 - No Handler set for ISR CMP2 (IRQ 61) */
   ISR_NoHandler,      /* 0x4e 0x00000138 - No Handler set for ISR FTM0 (IRQ 62) */
   ISR_NoHandler,      /* 0x4f 0x0000013C - No Handler set for ISR FTM1 (IRQ 63) */
   ISR_NoHandler,      /* 0x50 0x00000140 - No Handler set for ISR FTM2 (IRQ 64) */
   ISR_NoHandler,      /* 0x51 0x00000144 - No Handler set for ISR CMT (IRQ 65) */
   ISR_NoHandler,      /* 0x52 0x00000148 - No Handler set for ISR RTC (IRQ 66) */
   ISR_NoHandler,      /* 0x53 0x0000014C - No Handler set for ISR RTC_SEC (IRQ 67) */
   ISR_NoHandler,      /* 0x54 0x00000150 - No Handler set for ISR PIT0 (IRQ 68) */
   ISR_NoHandler,      /* 0x55 0x00000154 - No Handler set for ISR PIT1 (IRQ 69) */
   ISR_NoHandler,      /* 0x56 0x00000158 - No Handler set for ISR PIT2 (IRQ 70) */
   ISR_NoHandler,      /* 0x57 0x0000015C - No Handler set for ISR PIT3 (IRQ 71) */
   ISR_NoHandler,      /* 0x58 0x00000160 - No Handler set for ISR PDB0 (IRQ 72) */
   ISR_NoHandler,      /* 0x59 0x00000164 - No Handler set for ISR USB0 (IRQ 73) */
   ISR_NoHandler,      /* 0x5a 0x00000168 - No Handler set for ISR USBDCD (IRQ 74) */
   ISR_NoHandler,      /* 0x5b 0x0000016C - No Handler set for ISR ENET_1588_Timer (IRQ 75) */
   ISR_NoHandler,      /* 0x5c 0x00000170 - No Handler set for ISR ENET_TX (IRQ 76) */
   ISR_NoHandler,      /* 0x5d 0x00000174 - No Handler set for ISR ENET_RX (IRQ 77) */
   ISR_NoHandler,      /* 0x5e 0x00000178 - No Handler set for ISR ENET_ERR (IRQ 78) */
   ISR_NoHandler,      /* 0x5f 0x0000017C - No Handler set for ISR RES95 (IRQ 79) */
   ISR_NoHandler,      /* 0x60 0x00000180 - No Handler set for ISR SDHC (IRQ 80) */
   ISR_NoHandler,      /* 0x61 0x00000184 - No Handler set for ISR DAC0 (IRQ 81) */
   ISR_NoHandler,      /* 0x62 0x00000188 - No Handler set for ISR DAC1 (IRQ 82) */
   ISR_NoHandler,      /* 0x63 0x0000018C - No Handler set for ISR TSI0 (IRQ 83) */
   ISR_NoHandler,      /* 0x64 0x00000190 - No Handler set for ISR LPTimer (IRQ 84) */
   ISR_NoHandler,      /* 0x65 0x00000194 - No Handler set for ISR RES102 (IRQ 85) */
   ISR_NoHandler,      /* 0x66 0x00000198 - No Handler set for ISR PORTA (IRQ 86) */
   ISR_NoHandler,      /* 0x67 0x0000019C - No Handler set for ISR PORTB (IRQ 87) */
   ISR_NoHandler,      /* 0x68 0x000001A0 - No Handler set for ISR PORTC (IRQ 88) */
   ISR_NoHandler,      /* 0x69 0x000001A4 - No Handler set for ISR PORTD (IRQ 89) */
   ISR_NoHandler,      /* 0x6a 0x000001A8 - No Handler set for ISR PORTE (IRQ 90) */
   ISR_NoHandler,      /* 0x6b 0x000001AC - No Handler set for ISR PORTF (IRQ 91) */
   ISR_NoHandler,      /* 0x6c 0x000001B0 - No Handler set for ISR RES109 (IRQ 92) */
   ISR_NoHandler,      /* 0x6d 0x000001B4 - No Handler set for ISR SWI (IRQ 93) */
   ISR_NoHandler,      /* 0x6e 0x000001B8 - No Handler set for ISR NFC (IRQ 94) */
   ISR_NoHandler,      /* 0x6f 0x000001BC - No Handler set for ISR USBHS (IRQ 95) */
   ISR_NoHandler,      /* 0x70 0x000001C0 - No Handler set for ISR RES113 (IRQ 96) */
   ISR_NoHandler,      /* 0x71 0x000001C4 - No Handler set for ISR CMP3 (IRQ 97) */
   ISR_NoHandler,      /* 0x72 0x000001C8 - No Handler set for ISR RES115 (IRQ 98) */
   ISR_NoHandler,      /* 0x73 0x000001CC - No Handler set for ISR RES116 (IRQ 99) */
   ISR_NoHandler,      /* 0x74 0x000001D0 - No Handler set for ISR FTM3 (IRQ 100) */
   ISR_NoHandler,      /* 0x75 0x000001D4 - No Handler set for ISR ADC2 (IRQ 101) */
   ISR_NoHandler,      /* 0x76 0x000001D8 - No Handler set for ISR ADC3 (IRQ 102) */
   ISR_NoHandler,      /* 0x77 0x000001DC - No Handler set for ISR I2S1_TX (IRQ 103) */
   ISR_NoHandler,      /* 0x78 0x000001E0 - No Handler set for ISR I2S1_RX (IRQ 104) */
};
#elif (CPU == lpc4337)
/** \brief LPC4337 Interrupt vector */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
   /* System ISRs */
   &_vStackTop,                    /* The initial stack pointer  */
   ResetISR,                       /* The reset handler          */
   NMI_Handler,                    /* The NMI handler            */
   HardFault_Handler,              /* The hard fault handler     */
   MemManage_Handler,              /* The MPU fault handler      */
   BusFault_Handler,               /* The bus fault handler      */
   UsageFault_Handler,             /* The usage fault handler    */
   0,                              /* Reserved                   */
   0,                              /* Reserved                   */
   0,                              /* Reserved                   */
   0,                              /* Reserved                   */
   SVC_Handler,                    /* SVCall handler             */
   DebugMon_Handler,               /* Debug monitor handler      */
   0,                              /* Reserved                   */
   0,                              /* The PendSV handler         */
   SysTick_Handler,                /* The SysTick handler        */
   /*** User Interruptions ***/
   ISR_NoHandler,      /* 0x10 0x00000040 - No Handler set for ISR DAC (IRQ 0) */
   ISR_NoHandler,      /* 0x11 0x00000044 - No Handler set for ISR M0APP (IRQ 1) */
   ISR_NoHandler,      /* 0x12 0x00000048 - No Handler set for ISR DMA (IRQ 2) */
   ISR_NoHandler,      /* 0x13 0x0000004C - No Handler set for ISR RES1 (IRQ 3) */
   ISR_NoHandler,      /* 0x14 0x00000050 - No Handler set for ISR FLASH_EEPROM (IRQ 4) */
   ISR_NoHandler,      /* 0x15 0x00000054 - No Handler set for ISR ETH (IRQ 5) */
   ISR_NoHandler,      /* 0x16 0x00000058 - No Handler set for ISR SDIO (IRQ 6) */
   ISR_NoHandler,      /* 0x17 0x0000005C - No Handler set for ISR LCD (IRQ 7) */
   ISR_NoHandler,      /* 0x18 0x00000060 - No Handler set for ISR USB0 (IRQ 8) */
   ISR_NoHandler,      /* 0x19 0x00000064 - No Handler set for ISR USB1 (IRQ 9) */
   ISR_NoHandler,      /* 0x1a 0x00000068 - No Handler set for ISR SCT (IRQ 10) */
   ISR_NoHandler,      /* 0x1b 0x0000006C - No Handler set for ISR RIT (IRQ 11) */
   ISR_NoHandler,      /* 0x1c 0x00000070 - No Handler set for ISR TIMER0 (IRQ 12) */
   ISR_NoHandler,      /* 0x1d 0x00000074 - No Handler set for ISR TIMER1 (IRQ 13) */
   ISR_NoHandler,      /* 0x1e 0x00000078 - No Handler set for ISR TIMER2 (IRQ 14) */
   ISR_NoHandler,      /* 0x1f 0x0000007C - No Handler set for ISR TIMER3 (IRQ 15) */
   ISR_NoHandler,      /* 0x20 0x00000080 - No Handler set for ISR MCPWM (IRQ 16) */
   ISR_NoHandler,      /* 0x21 0x00000084 - No Handler set for ISR ADC0 (IRQ 17) */
   ISR_NoHandler,      /* 0x22 0x00000088 - No Handler set for ISR I2C0 (IRQ 18) */
   ISR_NoHandler,      /* 0x23 0x0000008C - No Handler set for ISR I2C1 (IRQ 19) */
   ISR_NoHandler,      /* 0x24 0x00000090 - No Handler set for ISR SPI (IRQ 20) */
   ISR_NoHandler,      /* 0x25 0x00000094 - No Handler set for ISR ADC1 (IRQ 21) */
   ISR_NoHandler,      /* 0x26 0x00000098 - No Handler set for ISR SSP0 (IRQ 22) */
   ISR_NoHandler,      /* 0x27 0x0000009C - No Handler set for ISR SSP1 (IRQ 23) */
   ISR_NoHandler,      /* 0x28 0x000000A0 - No Handler set for ISR UART0 (IRQ 24) */
   ISR_NoHandler,      /* 0x29 0x000000A4 - No Handler set for ISR UART1 (IRQ 25) */
   ISR_NoHandler,      /* 0x2a 0x000000A8 - No Handler set for ISR UART2 (IRQ 26) */
   ISR_NoHandler,      /* 0x2b 0x000000AC - No Handler set for ISR UART3 (IRQ 27) */
   ISR_NoHandler,      /* 0x2c 0x000000B0 - No Handler set for ISR I2S0 (IRQ 28) */
   ISR_NoHandler,      /* 0x2d 0x000000B4 - No Handler set for ISR I2S1 (IRQ 29) */
   ISR_NoHandler,      /* 0x2e 0x000000B8 - No Handler set for ISR SPIFI (IRQ 30) */
   ISR_NoHandler,      /* 0x2f 0x000000BC - No Handler set for ISR SGPIO (IRQ 31) */
   ISR_NoHandler,      /* 0x30 0x000000C0 - No Handler set for ISR GPIO0 (IRQ 32) */
   ISR_NoHandler,      /* 0x31 0x000000C4 - No Handler set for ISR GPIO1 (IRQ 33) */
   ISR_NoHandler,      /* 0x32 0x000000C8 - No Handler set for ISR GPIO2 (IRQ 34) */
   ISR_NoHandler,      /* 0x33 0x000000CC - No Handler set for ISR GPIO3 (IRQ 35) */
   ISR_NoHandler,      /* 0x34 0x000000D0 - No Handler set for ISR GPIO4 (IRQ 36) */
   ISR_NoHandler,      /* 0x35 0x000000D4 - No Handler set for ISR GPIO5 (IRQ 37) */
   ISR_NoHandler,      /* 0x36 0x000000D8 - No Handler set for ISR GPIO6 (IRQ 38) */
   ISR_NoHandler,      /* 0x37 0x000000DC - No Handler set for ISR GPIO7 (IRQ 39) */
   ISR_NoHandler,      /* 0x38 0x000000E0 - No Handler set for ISR GINT0 (IRQ 40) */
   ISR_NoHandler,      /* 0x39 0x000000E4 - No Handler set for ISR GINT1 (IRQ 41) */
   ISR_NoHandler,      /* 0x3a 0x000000E8 - No Handler set for ISR EVRT (IRQ 42) */
   ISR_NoHandler,      /* 0x3b 0x000000EC - No Handler set for ISR CAN1 (IRQ 43) */
   ISR_NoHandler,      /* 0x3c 0x000000F0 - No Handler set for ISR RES6 (IRQ 44) */
   ISR_NoHandler,      /* 0x3d 0x000000F4 - No Handler set for ISR ADCHS (IRQ 45) */
   ISR_NoHandler,      /* 0x3e 0x000000F8 - No Handler set for ISR ATIMER (IRQ 46) */
   ISR_NoHandler,      /* 0x3f 0x000000FC - No Handler set for ISR RTC (IRQ 47) */
   ISR_NoHandler,      /* 0x40 0x00000100 - No Handler set for ISR RES8 (IRQ 48) */
   ISR_NoHandler,      /* 0x41 0x00000104 - No Handler set for ISR WDT (IRQ 49) */
   ISR_NoHandler,      /* 0x42 0x00000108 - No Handler set for ISR M0SUB (IRQ 50) */
   ISR_NoHandler,      /* 0x43 0x0000010C - No Handler set for ISR CAN0 (IRQ 51) */
   ISR_NoHandler,      /* 0x44 0x00000110 - No Handler set for ISR QEI (IRQ 52) */
};
#endif

/*==================[internal functions definition]==========================*/
/* Default exception handlers. */
__attribute__ ((section(".after_vectors")))
static void NMI_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void HardFault_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void MemManage_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void BusFault_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void UsageFault_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void SVC_Handler(void) {
   while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void DebugMon_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void ISR_NoHandler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors")))
static void SysTick_Handler(void) {
    while (1) {
    }
}

/*==================[external functions definition]==========================*/

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/


/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    K60P144M150SF3RM, Rev. 2, Dec 2011
**     Version:             rev. 1.6, 2013-06-24
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2013 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2011-08-24)
**         Initial version
**     - rev. 1.1 (2011-11-03)
**         Registers updated according to the new reference manual revision - Rev. 1, Oct 2011
**         Registers of the following modules have been updated - AXBS, CAN, I2S, MCG, MPU, NFC, RCM, RTC, SDHC, SIM, USBHS, WDOG
**         The following modules have been removed - DDR, DRY
**     - rev. 1.2 (2012-01-04)
**         Registers updated according to the new reference manual revision - Rev. 2, Dec 2011
**         EWM - INTEN bit in EWM_CTRL register has been added.
**         PDB - register PDB_PO0EN renamed to PRB_POEN.
**         PMC - BGEN bit in PMC_REGSC register has been removed.
**         SIM - several changes in SCGC registers. Bit USBHS in SOPT2 register removed.
**         UART - new bits RXOFE in regiter CFIFO and RXOF in register SFIFO.
**     - rev. 1.3 (2012-04-13)
**         Added new #define symbol MCU_MEM_MAP_VERSION_MINOR.
**         Added new #define symbols <peripheralType>_BASE_PTRS.
**     - rev. 1.4 (2012-10-19)
**         RTC - security related registers removed.
**     - rev. 1.5 (2013-04-05)
**         Changed start of doxygen comment.
**     - rev. 1.6 (2013-06-24)
**         MPU - missing region descriptor registers added.
**
** ###################################################################
*/

/*!
 * @file MK60F12
 * @version 1.6
 * @date 2013-06-24
 * @brief Device specific configuration file for MK60F12 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef SYSTEM_MK60F12_H_
#define SYSTEM_MK60F12_H_                        /**< Symbol preventing repeated inclusion */

#define DISABLE_WDOG    1

#ifndef CLOCK_SETUP
  #define CLOCK_SETUP                  1
#endif

/* MCG mode constants */

#define MCG_MODE_FEI                   0U
#define MCG_MODE_FBI                   1U
#define MCG_MODE_BLPI                  2U
#define MCG_MODE_FEE                   3U
#define MCG_MODE_FBE                   4U
#define MCG_MODE_BLPE                  5U
#define MCG_MODE_PBE                   6U
#define MCG_MODE_PEE                   7U

/* Predefined clock setups
   0 ... Multipurpose Clock Generator (MCG) in FLL Engaged Internal (FEI) mode
         Reference clock source for MCG module is the slow internal clock source 32.768kHz
         Core clock = 41.94MHz, BusClock = 41.94MHz
   1 ... Multipurpose Clock Generator (MCG) in PLL Engaged External (PEE) mode
         Reference clock source for MCG module is an external reference clock source 50MHz
         Core clock = 120MHz, BusClock = 60MHz
   2 ... Multipurpose Clock Generator (MCG) in Bypassed Low Power External (BLPE) mode
         Core clock/Bus clock derived directly from an external reference clock source 50MHz with no multiplication
         Core clock = 50MHz, BusClock = 50MHz
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/

/* Define clock source values */

#define CPU_XTAL_CLK_HZ                50000000u           /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_XTAL32k_CLK_HZ             32768u              /* Value of the external 32k crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ            32768u              /* Value of the slow internal oscillator clock frequency in Hz  */
#define CPU_INT_FAST_CLK_HZ            4000000u            /* Value of the fast internal oscillator clock frequency in Hz  */
#define CPU_INT_IRC_CLK_HZ             48000000u           /* Value of the 48M internal oscillator clock frequency in Hz  */

/* RTC oscillator setting */
/* RTC_CR: SC2P=0,SC4P=0,SC8P=0,SC16P=0,CLKO=1,OSCE=1,WPS=0,UM=0,SUP=0,WPE=0,SWR=0 */
#define SYSTEM_RTC_CR_VALUE            0x0300U             /* RTC_CR */

/* Low power mode enable */
/* SMC_PMPROT: AVLP=1,ALLS=1,AVLLS=1 */
#define SYSTEM_SMC_PMPROT_VALUE        0x2AU               /* SMC_PMPROT */

/* Internal reference clock trim */
/* #undef SLOW_TRIM_ADDRESS */                             /* Slow oscillator not trimmed. Commented out for MISRA compliance. */
/* #undef SLOW_FINE_TRIM_ADDRESS */                        /* Slow oscillator not trimmed. Commented out for MISRA compliance. */
/* #undef FAST_TRIM_ADDRESS */                             /* Fast oscillator not trimmed. Commented out for MISRA compliance. */
/* #undef FAST_FINE_TRIM_ADDRESS */                        /* Fast oscillator not trimmed. Commented out for MISRA compliance. */

#if (CLOCK_SETUP == 0)
    #define CPU_XTAL0_CLK_HZ                50000000u /* Value of the external crystal or oscillator clock frequency in Hz connected to System Oscillator 0 */
    #define CPU_XTAL1_CLK_HZ                8000000u  /* Value of the external crystal or oscillator clock frequency in Hz connected to System Oscillator 1 */
    #define CPU_XTAL32k_CLK_HZ              32768u    /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            41943040u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL0_CLK_HZ                50000000u /* Value of the external crystal or oscillator clock frequency in Hz connected to System Oscillator 0 */
    #define CPU_XTAL1_CLK_HZ                8000000u  /* Value of the external crystal or oscillator clock frequency in Hz connected to System Oscillator 1 */
    #define CPU_XTAL32k_CLK_HZ              32768u    /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            120000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL0_CLK_HZ                50000000u /* Value of the external crystal or oscillator clock frequency in Hz connected to System Oscillator 0 */
    #define CPU_XTAL1_CLK_HZ                8000000u  /* Value of the external crystal or oscillator clock frequency in Hz connected to System Oscillator 1 */
    #define CPU_XTAL32k_CLK_HZ              32768u    /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u    /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u  /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            50000000u  /* Default System clock value */
#endif /* (CLOCK_SETUP == 2) */


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif  /* #if !defined(SYSTEM_MK60F12_H_) */

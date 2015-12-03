//*****************************************************************************
// LPC43xx (Cortex M0 APP) Startup code for use with LPCXpresso IDE
//
// Version : 141204
//*****************************************************************************
//
// Copyright(C) NXP Semiconductors, 2013-2014
// All rights reserved.
//
// Software that is described herein is for illustrative purposes only
// which provides customers with programming information regarding the
// LPC products.  This software is supplied "AS IS" without any warranties of
// any kind, and NXP Semiconductors and its licensor disclaim any and
// all warranties, express or implied, including all implied warranties of
// merchantability, fitness for a particular purpose and non-infringement of
// intellectual property rights.  NXP Semiconductors assumes no responsibility
// or liability for the use of the software, conveys no license or rights under any
// patent, copyright, mask work right, or any other intellectual property rights in
// or to any products. NXP Semiconductors reserves the right to make changes
// in the software without notification. NXP Semiconductors also makes no
// representation or warranty that such application will be suitable for the
// specified use without further testing or modification.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation is hereby granted, under NXP Semiconductors' and its
// licensor's relevant copyrights in the software, without fee, provided that it
// is used in conjunction with NXP Semiconductors microcontrollers.  This
// copyright, permission, and disclaimer notice must appear in all copies of
// this code.
//*****************************************************************************
#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
  extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
#if defined (__USE_CMSIS) || defined (__USE_LPCOPEN)
// Declaration of external SystemInit function
extern void SystemInit(void);
#endif

#include "crp.h"
// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP static const unsigned int CRP_WORD = CRP_NO_CRP ;

/* Checksum calculation */
__attribute__ ((weak)) extern void __valid_user_code_checksum();

//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
//
//*****************************************************************************
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);

//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);
//*****************************************************************************
//
// External declaration for the pointer to the stack top from the Linker Script
//
//*****************************************************************************
extern void _vStackTop(void);

//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
//
// The vector table.
// This relies on the linker script to place at correct location in memory.
//
//*****************************************************************************
extern void (* const g_pfnVectors[])(void);
//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
  unsigned int *pulDest = (unsigned int*) start;
  unsigned int *pulSrc = (unsigned int*) romstart;
  unsigned int loop;
  for (loop = 0; loop < len; loop = loop + 4)
    *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors")))
void bss_init(unsigned int start, unsigned int len) {
  unsigned int *pulDest = (unsigned int*) start;
  unsigned int loop;
  for (loop = 0; loop < len; loop = loop + 4)
    *pulDest++ = 0;
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table". This table is
// created by the linker via the Code Red managed linker script mechanism. It
// contains the load address, execution address and length of each RW data
// section and the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//
//*****************************************************************************
void
ResetISR(void) {

  // ******************************
  // Modify CREG->M0APPMAP so that M0 looks in correct place
  // for its vector table when an exception is triggered.
  // Note that we do not use the CMSIS register access mechanism,
  // as there is no guarantee that the project has been configured
  // to use CMSIS.
  unsigned int *pCREG_M0APPMAP = (unsigned int *) 0x40043404;
  // CMSIS : CREG->M0APPMAP = <address of vector table>
  *pCREG_M0APPMAP = (unsigned int)g_pfnVectors;

    //
    // Copy the data sections from flash to SRAM.
    //
  unsigned int LoadAddr, ExeAddr, SectionLen;
  unsigned int *SectionTableAddr;

  // Load base address of Global Section Table
  SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
  while (SectionTableAddr < &__data_section_table_end) {
    LoadAddr = *SectionTableAddr++;
    ExeAddr = *SectionTableAddr++;
    SectionLen = *SectionTableAddr++;
    data_init(LoadAddr, ExeAddr, SectionLen);
  }
  // At this point, SectionTableAddr = &__bss_section_table;
  // Zero fill the bss segment
  while (SectionTableAddr < &__bss_section_table_end) {
    ExeAddr = *SectionTableAddr++;
    SectionLen = *SectionTableAddr++;
    bss_init(ExeAddr, SectionLen);
  }

// **********************************************************
// No need to call SystemInit() here, as master CM4 cpu will
// have done the main system set up before enabling CM0.
// **********************************************************

#if defined (__cplusplus)
  //
  // Call C++ library initialisation
  //
  __libc_init_array();
#endif

#if defined (__REDLIB__)
  // Call the Redlib library, which in turn calls main()
  __main() ;
#else
  main();
#endif

  //
  // main() shouldn't return, but if it does, we'll just enter an infinite loop
  //
  while (1) {
    ;
  }
}

//*****************************************************************************
// Default exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
#if defined (__USE_LPCOPEN)
void NMI_Handler(void)
#else
void M0_NMI_Handler(void)
#endif
{   while(1) { }
}

__attribute__ ((section(".after_vectors")))
#if defined (__USE_LPCOPEN)
void HardFault_Handler(void)
#else
void M0_HardFault_Handler(void)
#endif
{   while(1) { }
}

__attribute__ ((section(".after_vectors")))
#if defined (__USE_LPCOPEN)
void SVC_Handler(void)
#else
void M0_SVC_Handler(void)
#endif
{   while(1) { }
}

//*****************************************************************************
// LPC5410x Microcontroller Startup code for use with LPCXpresso IDE
//
// Version : 150819
//*****************************************************************************
//
// Copyright(C) NXP Semiconductors, 2014-15
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



//*****************************************************************************



#if !defined (DONT_ENABLE_SWVTRACECLK) && !defined (CORE_M0PLUS)
// Allow confirmation that SWV trace has been enabled
unsigned int __SWVtrace_Enabled;
#endif



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
//
// External declaration for the vector pointer table
//
//*****************************************************************************



extern void (* const g_pfnVectors[])(void);



//*****************************************************************************



#if defined (__cplusplus)
} // extern "C"
#endif



//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************



__attribute__ ((section(".after_vectors")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len)
{
   unsigned int *pulDest = (unsigned int*) start;
   unsigned int *pulSrc = (unsigned int*) romstart;
   unsigned int loop;

   for (loop = 0; loop < len; loop = loop + 4)
      *pulDest++ = *pulSrc++;
}


__attribute__ ((section(".after_vectors")))
void bss_init(unsigned int start, unsigned int len)
{
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
//*****************************************************************************



#if defined (__MULTICORE_MASTER)

//#define cpu_ctrl         0x40000300
//#define coproc_boot      0x40000304
//#define set coproc_stack 0x40000308

__attribute__ ((naked, section(".after_vectors.reset")))
void ResetISR(void) {
   asm volatile(
         "    .set   cpu_ctrl,       0x40000300\t\n"
         "    .set   coproc_boot,    0x40000304\t\n"
         "    .set   coproc_stack,   0x40000308\t\n"
         "    MOVS   R5, #1\t\n"
         "    LDR    R0, =0xE000ED00\t\n"
         "    LDR    R1, [R0]\t\n"           // READ CPUID register
         "    LDR    R2,=0x410CC601\t\n"     // CM0 R0p1 identifier
         "    EORS   R1,R1,R2\t\n"           // XOR to see if we are C0
         "    LDR    R3, =cpu_ctrl\t\n"      // get address of CPU_CTRL
         "    LDR    R1,[R3]\t\n"            // read cpu_ctrl reg into R1
         "    BEQ.N  cm0_boot\t\n"
         "cm4_boot:\t\n"
         "    LDR    R0,=coproc_boot\t\n"    // coproc boot address
         "    LDR    R0,[R0]\t\n"            // get address to branch to
         "    MOVS   R0,R0\t\n"              // Check if 0
         "    BEQ.N  check_master_m4\t\n"    // if zero in boot reg, we just branch to  real reset
         "    BX     R0\t\n"                 // otherwise, we branch to boot address
         "commonboot:\t\n"
         "    LDR    R0, =ResetISR2\t\n"     // Jump to 'real' reset handler
         "    BX     R0\t\n"
         "cm0_boot:\t\n"
         "    LDR    R0,=coproc_boot\t\n"    // coproc boot address
         "    LDR    R0,[R0]\t\n"            // get address to branch to
         "    MOVS   R0,R0\t\n"              // Check if 0
         "    BEQ.N  check_master_m0\t\n"    // if zero in boot reg, we just branch to  real reset
         "    LDR    R1,=coproc_stack\t\n"   // pickup coprocesor stackpointer (from syscon CPSTACK)
         "    LDR    R1,[R1]\t\n"
         "    MOV    SP,R1\t\n"
         "    BX     R0\t\n"                 // goto boot address
         "check_master_m0:\t\n"
         "    ANDS   R1,R1,R5\t\n"           // bit test bit0
         "    BEQ.N  commonboot\t\n"         // if we get 0, that means we are masters
         "    B.N    goto_sleep_pending_reset\t\n"   // Otherwise, there is no startup vector for slave, so we go to sleep
         "check_master_m4:\t\n"
         "    ANDS   R1,R1,R5\t\n"           // bit test bit0
         "    BNE.N  commonboot\t\n"         // if we get 1, that means we are masters
         "goto_sleep_pending_reset:\t\n"
         "    MOV     SP,R5\t\n"             // load 0x1 into SP so that any stacking (eg on NMI) will not cause us to wakeup
         // and write to uninitialised Stack area (instead it will LOCK us up before we cause damage)
         // this code should only be reached if debugger bypassed ROM or we changed master without giving
         // correct start address, the only way out of this is through a debugger change of SP and PC
         "sleepo:\t\n"
         "    WFI\t\n"                       // go to sleep
         "    B.N    sleepo\t\n"
   );
}


void ResetISR2(void)
{

#else


__attribute__ ((section(".after_vectors.reset")))
void ResetISR(void)
{
#endif

   // If this is not the CM0+ core...
#if !defined (CORE_M0PLUS)
   // If this is not a slave project...
#if !defined (__MULTICORE_M0SLAVE) && \
      !defined (__MULTICORE_M4SLAVE)
   // Optionally enable RAM banks that may be off by default at reset
#if !defined (DONT_ENABLE_DISABLED_RAMBANKS)
   volatile unsigned int *SYSCON_SYSAHBCLKCTRL0 = (unsigned int *) 0x400000c0;
   // Ensure that SRAM2(4) bit in SYSAHBCLKCTRL0 are set
   *SYSCON_SYSAHBCLKCTRL0 |= (1 << 4);
#endif
#endif
#endif

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

   // Optionally enable Cortex-M4 SWV trace (off by default at reset)
   // Note - your board support must also set up pinmuxing such that
   // SWO is output on GPIO PIO0-15 (FUNC2) or PIO1_1 (FUNC2).
#if !defined (DONT_ENABLE_SWVTRACECLK) && !defined (CORE_M0PLUS)
   volatile unsigned int *TRACECLKDIV = (unsigned int *) 0x400000E4;
   volatile unsigned int *SYSAHBCLKCTRLSET = (unsigned int *) 0x400000C8;
   volatile unsigned int *SYSAHBCLKCTRL = (unsigned int *) 0x400000C0;

   // Write 0x00000001 to TRACECLKDIV (0x400000E4) – Trace divider
   *TRACECLKDIV = 1;

   // Enable IOCON peripheral clock (for SWO on PIO0-15 or PIO1_1)
   // by setting bit13 via SYSAHBCLKCTRLSET[0] (0x400000C8)
   *SYSAHBCLKCTRLSET = 1 << 13; // 0x2000

   // Read  SYSAHBCLKCTRL[0] (0x400000C0) and check bit 13
   __SWVtrace_Enabled = ((*SYSAHBCLKCTRL & 1 << 13) == 1 << 13);
#endif

#if !defined (__USE_LPCOPEN)
   // LPCOpen init code deals with FP and VTOR initialization
#if defined (__VFP_FP__) && !defined (__SOFTFP__)
   /*
    * Code to enable the Cortex-M4 FPU only included
    * if appropriate build options have been selected.
    * Code taken from Section 7.1, Cortex-M4 TRM (DDI0439C)
    */
   // CPACR is located at address 0xE000ED88
   asm("LDR.W R0, =0xE000ED88");
   // Read CPACR
   asm("LDR R1, [R0]");
   // Set bits 20-23 to enable CP10 and CP11 coprocessors
   asm(" ORR R1, R1, #(0xF << 20)");
   // Write back the modified value to the CPACR
   asm("STR R1, [R0]");
#endif // (__VFP_FP__) && !(__SOFTFP__)

   // ******************************
   // Check to see if we are running the code from a non-zero
   // address (eg RAM, external flash), in which case we need
   // to modify the VTOR register to tell the CPU that the
   // vector table is located at a non-0x0 address.

   // Note that we do not use the CMSIS register access mechanism,
   // as there is no guarantee that the project has been configured
   // to use CMSIS.
   unsigned int * pSCB_VTOR = (unsigned int *) 0xE000ED08;
   if ((unsigned int *) g_pfnVectors != (unsigned int *) 0x00000000) {
      // CMSIS : SCB->VTOR = <address of vector table>
      *pSCB_VTOR = (unsigned int) g_pfnVectors;
   }
#endif

#if defined (__USE_CMSIS) || defined (__USE_LPCOPEN)
   SystemInit();
#endif

#if defined (__cplusplus)
   //
   // Call C++ library initialization
   //
   __libc_init_array();
#endif

#if defined (__REDLIB__)

   // Call the Redlib library, which in turn calls main()
   __main();

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




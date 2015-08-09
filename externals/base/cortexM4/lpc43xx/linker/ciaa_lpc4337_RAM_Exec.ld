/* Copyright 2015, Juan Cecconi (Numetron, UTN-FRBA)
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

INCLUDE "externals/base/cortexM4/lpc43xx/linker/ciaa_lpc4337_lib.ld"
INCLUDE "externals/base/cortexM4/lpc43xx/linker/ciaa_lpc4337_mem.ld"

ENTRY(ResetISR)

SECTIONS
{
   /* Main Text Flash Section */    
   .text_Flash : ALIGN(4)
   {
      FILL(0xFF);

      LONG(_vStackTop);
      LONG(ResetISR | 1);   /* 1 */
      LONG(0x0);            /* 2 */
      LONG(0x0);            /* 3 */        
      LONG(0x0);            /* 4 */
      LONG(0x0);            /* 5 */
      LONG(0x0);            /* 6 */
      LONG(0x0);            /* 7 */
      LONG(0x0);            /* 8 */
      LONG(0x0);            /* 9 */
      LONG(0x0);            /* 10 */
      LONG(0x0);            /* 11 */
      LONG(0x0);            /* 12 */
      LONG(0x0);            /* 13 */
      LONG(0x0);            /* 14 */
      LONG(0x0);            /* 15 */
      LONG(0x0);            /* 16 */
      LONG(0x0);            /* 17 */
      LONG(0x0);            /* 18 */        
      LONG(0x0);            /* 19 */
      LONG(0x0);            /* 20 */
      LONG(0x0);            /* 21 */
      LONG(0x0);            /* 22 */
      LONG(0x0);            /* 23 */
      LONG(0x0);            /* 24 */
      LONG(0x0);            /* 25 */
      LONG(0x0);            /* 26 */
      LONG(0x0);            /* 28 */
      LONG(0x0);            /* 29 */        
      LONG(0x0);            /* 30 */
      LONG(0x0);            /* 31 */
      LONG(0x0);            /* 32 */
      LONG(0x0);            /* 33 */
      LONG(0x0);            /* 34 */
      LONG(0x0);            /* 35 */
      LONG(0x0);            /* 36 */
      LONG(0x0);            /* 37 */
      LONG(0x0);            /* 38 */
      LONG(0x0);            /* 39 */        
      LONG(0x0);            /* 40 */
      LONG(0x0);            /* 41 */
      LONG(0x0);            /* 42 */
      LONG(0x0);            /* 43 */
      LONG(0x0);            /* 44 */
      LONG(0x0);            /* 45 */
      LONG(0x0);            /* 46 */
      LONG(0x0);            /* 47 */
      LONG(0x0);            /* 48 */
      LONG(0x0);            /* 49 */        
      LONG(0x0);            /* 50 */
      LONG(0x0);            /* 51 */
      LONG(0x0);            /* 52 */
      LONG(0x0);            /* 53 */
      LONG(0x0);            /* 54 */
      LONG(0x0);            /* 55 */
      LONG(0x0);            /* 56 */
      LONG(0x0);            /* 57 */
      LONG(0x0);            /* 58 */
      LONG(0x0);            /* 59 */        
      LONG(0x0);            /* 60 */
      LONG(0x0);            /* 61 */
      LONG(0x0);            /* 62 */
      LONG(0x0);            /* 63 */
      LONG(0x0);            /* 64 */
      LONG(0x0);            /* 65 */
      LONG(0x0);            /* 66 */
      LONG(0x0);            /* 67 */
      LONG(0x0);            /* 68 */
      LONG(0x0);            /* 69 */
      
      /* CRP (Code Read Protect) */
      . = 0x000002FC;
      PROVIDE(__CRP_WORD_START__ = .);
      KEEP(*(.crp));
      PROVIDE(__CRP_WORD_END__ = .);
      ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");
      /* End of CRP (Code Read Protect) */
                      
   } >MFlashA512
    
   /* Code and read-only/const data, address: 0x10080000 */
   .text_RAM : ALIGN(4)    
   {
      FILL(0x00);
      /* RAM Vector Table  must be at the beginning */
      g_pfnVectors = ABSOLUTE(.);
      KEEP(*(.isr_vector));

      /* StartUp Reset Code, must be always at the same address */
      *(.text.ResetISR);
      *(.after_vectors*);

      /* StartUp and App Code */
      *(.text*);
      
      /* StartUp and App read-oly/const data */
      *(.rodata .rodata.* .constdata .constdata.*);
      
   } > RamLoc40

   /* Initialized Data */
   /* Main DATA section for StartUp and App (RamLoc40) */
   .data_RAM : ALIGN(4)
   {
      FILL(0x00);
      _data = .;
      
      /* Global Section Table */
      __section_table_start = .;  
      __data_section_table = .;
      /* Empty data Table */
      __data_section_table_end = .;
  
      __bss_section_table = .;
      LONG(    ADDR(.bss));
      LONG(  SIZEOF(.bss));
      __bss_section_table_end = .;
        
      __section_table_end = .;
      /* End of Global Section Table */

      /* StartUp and App Initialized data */
      *(.data*);
      
      _edata = .;

   } > RamLoc40

   /* Main BSS Section, StartUp and App */
   .bss : ALIGN(4)
   {
      FILL(0x00);
      _bss = .;

      *(COMMON);
      *(.bss*);
      
      _ebss = .;
    
    } > RamLoc40
 
   PROVIDE(_pvHeapStart = .);
   PROVIDE(_vStackTop = __top_RamLoc40 - 0);
}

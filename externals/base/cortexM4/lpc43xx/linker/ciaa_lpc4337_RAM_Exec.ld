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

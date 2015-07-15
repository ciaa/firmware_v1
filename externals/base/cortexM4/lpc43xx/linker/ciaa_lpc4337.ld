/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-${year}
 * (c) NXP Semiconductors 2013-${year}
 * Generated linker script file for 2014
 * Created from LPC4337 (Sun May 11 09:57:34 ART 2014)
 * By generic_c.ld on LPCXpresso v7.1.1 [Build 125] [2014-04-01] 
 */


INCLUDE "externals/base/cortexM4/lpc43xx/linker/ciaa_lpc4337_lib.ld"
INCLUDE "externals/base/cortexM4/lpc43xx/linker/ciaa_lpc4337_mem.ld"

ENTRY(ResetISR)

SECTIONS
{

    /* MAIN TEXT SECTION */    
    .text1 : ALIGN(4)
    {
        FILL(0xff)
        LONG(_vStackTop | 1);
        LONG(ResetISR | 1);
/*
       __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))      
*/
        *(.after_vectors*)
        
        /* Code Read Protect data */
        . = 0x000002FC ;
        PROVIDE(__CRP_WORD_START__ = .) ;
        KEEP(*(.crp))
        PROVIDE(__CRP_WORD_END__ = .) ;
        ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");
        /* End of Code Read Protect */

        *(.text.Chip_Clock*)
        *(.text.Chip_System*)
        *(.text.Chip_Setup*)
    } >MFlashA512
    
    /* Code and read-only/const data, address: 0x10080000 */
    .text : ALIGN(4)    
    {
        g_pfnVectors = 1<<29 | ABSOLUTE(.) ;
        KEEP(*(.isr_vector))   
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > RamLoc40
    /* MAIN DATA SECTION */
    
    /* Initialized Data, address: 0x10080000 */
    /* Main DATA section (RamLoc40) */
    .data : ALIGN(4)
    {
       FILL(0x00)
       _data = . ;

       /* Global Section Table */
        __section_table_start = .;
        
        __data_section_table = .;
        /* Empty data Table */
        __data_section_table_end = .;
        
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        
        __section_table_end = . ;
        /* End of Global Section Table */

       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > RamLoc40

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
       FILL(0x00)    
        _bss = .;
        *(.bss*);
        . = ALIGN(4);
        _ebss = .;
        PROVIDE(end = .);
    } > RamLoc40
    
    PROVIDE(_pvHeapStart = .);
    PROVIDE(_vStackTop = __top_RamLoc40 - 0);
}

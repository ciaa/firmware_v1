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

        LONG(_vStackTop);
        LONG(ResetISR | 1);   /* 1 */
        LONG(0x0);            /* 2 */
        LONG(0x0);            /* 3 */        
        LONG(0x1);            /* 4 */
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
        g_pfnVectors = ABSOLUTE(.) ;
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

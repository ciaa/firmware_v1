/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2016
 * Generated linker script file for LPC54102J512
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.23
 * LPCXpresso v8.0.0 [Build 526] [2015-11-23]  on Feb 18, 2016 2:53:13 PM
 */

INCLUDE "externals/base/cortexM4/lpc5410x/linker/lpc54102_m4_mem.ld"
INCLUDE "externals/base/cortexM4/lpc5410x/linker/lpc54102_m4_lib.ld"

ENTRY(ResetISR)

SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ; 
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2_core_m0slave_text));
        LONG(    ADDR(.data_RAM2_core_m0slave_text));
        LONG(  SIZEOF(.data_RAM2_core_m0slave_text));
        LONG(LOADADDR(.data_RAM2_core_m0slave_ARM_extab));
        LONG(    ADDR(.data_RAM2_core_m0slave_ARM_extab));
        LONG(  SIZEOF(.data_RAM2_core_m0slave_ARM_extab));
        LONG(LOADADDR(.data_RAM2_core_m0slave_ARM_exidx));
        LONG(    ADDR(.data_RAM2_core_m0slave_ARM_exidx));
        LONG(  SIZEOF(.data_RAM2_core_m0slave_ARM_exidx));
        LONG(LOADADDR(.data_RAM2_core_m0slave_data));
        LONG(    ADDR(.data_RAM2_core_m0slave_data));
        LONG(  SIZEOF(.data_RAM2_core_m0slave_data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        __bss_section_table_end = .;
        __section_table_end = . ;
	    /* End of Global Section Table */

        *(.after_vectors*)
        
        /* Code Read Protect data */
        . = 0x000002FC ;
        PROVIDE(__CRP_WORD_START__ = .) ;
        KEEP(*(.crp))
        PROVIDE(__CRP_WORD_END__ = .) ;
        ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");
        /* End of Code Read Protect */        

    } >MFlash512

    .text : ALIGN(4)    
    {
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > MFlash512
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4) 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > MFlash512
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > MFlash512
    __exidx_end = .;

    _etext = .;
        
    /* DATA section for Ram1_32 */
  
    .data_RAM2_core_m0slave_text : SUBALIGN(4)
    {
       FILL(0xff)
       PROVIDE(__start_data_RAM2 = .) ;
       __core_m0slave_START__ = .; /* start of slave image */
       KEEP(*(.core_m0slave))
    } > Ram1_32 AT>MFlash512

    /* M0SLAVE extab and exidx sections */
    .data_RAM2_core_m0slave_ARM_extab . : SUBALIGN(4)
    {
        FILL(0xff)
        KEEP(*(.core_m0slave.ARM.extab))
    } > Ram1_32 AT>MFlash512 

    .data_RAM2_core_m0slave_ARM_exidx . : SUBALIGN(4)
    {
        FILL(0xff)
        KEEP(*(.core_m0slave.ARM.exidx))
    } > Ram1_32 AT>MFlash512 

    /* M0SLAVE data section */
    .data_RAM2_core_m0slave_data . : SUBALIGN(4)
    {
        FILL(0xff)
        KEEP(*(.core_m0slave.data_*)) KEEP(*(.core_m0slave.data))
        __core_m0slave_END__ = .; /* end of slave image */

        /* perform some simple sanity checks */
        /*
        ASSERT(!(__core_m0slave_START__ == __core_m0slave_END__), "No slave code for _core_m0slave");
        ASSERT( (ABSOLUTE(__core_m0slave_START__) == __vectors_start___core_m0slave), "M0SLAVE execute address differs from address provided in source image");
        */
    } > Ram1_32 AT>MFlash512 
    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        *(.ramfunc.$RAM2)
        *(.ramfunc.$Ram1_32)
        *(.data.$RAM2*)
        *(.data.$Ram1_32*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
     } > Ram1_32 AT>MFlash512

    /* DATA section for Ram2_8 */
  
    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$Ram2_8)
        *(.data.$RAM3*)
        *(.data.$Ram2_8*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
     } > Ram2_8 AT>MFlash512

    /* MAIN DATA SECTION */
    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > Ram0_64
    /* Main DATA section (Ram0_64) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(vtable)
       *(.ramfunc*)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > Ram0_64 AT>MFlash512
    /* BSS section for Ram1_32 */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       *(.bss.$RAM2*)
       *(.bss.$Ram1_32*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
    } > Ram1_32 
    /* BSS section for Ram2_8 */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       *(.bss.$RAM3*)
       *(.bss.$Ram2_8*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
    } > Ram2_8 
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > Ram0_64
    /* NOINIT section for Ram1_32 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM2*)
       *(.noinit.$Ram1_32*)
       . = ALIGN(4) ;
    } > Ram1_32 
    /* NOINIT section for Ram2_8 */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       *(.noinit.$RAM3*)
       *(.noinit.$Ram2_8*)
       . = ALIGN(4) ;
    } > Ram2_8 
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > Ram0_64

    PROVIDE(_pvHeapStart = DEFINED(__user_heap_base) ? __user_heap_base : .);
    PROVIDE(_vStackTop = DEFINED(__user_stack_top) ? __user_stack_top : __top_Ram0_64 - 0);

    /* ## Create checksum value (used in startup) ## */
    PROVIDE(__valid_user_code_checksum = 0 - 
                                         (_vStackTop 
                                         + (ResetISR + 1) 
                                         + (NMI_Handler + 1) 
                                         + (HardFault_Handler + 1) 
                                         + (( DEFINED(MemManage_Handler) ? MemManage_Handler : 0 ) + 1)   /* MemManage_Handler may not be defined */
                                         + (( DEFINED(BusFault_Handler) ? BusFault_Handler : 0 ) + 1)     /* BusFault_Handler may not be defined */
                                         + (( DEFINED(UsageFault_Handler) ? UsageFault_Handler : 0 ) + 1) /* UsageFault_Handler may not be defined */
                                         ) );
}
/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2016
 * Generated linker script file for LPC54102J512
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.23
 * LPCXpresso v8.0.0 [Build 526] [2015-11-23]  on Feb 18, 2016 2:53:13 PM
 */

MEMORY
{
  /* Define each memory region */
  MFlash512 (rx) : ORIGIN = 0x0, LENGTH = 0x80000 /* 512K bytes (alias Flash) */  
  Ram0_64 (rwx) : ORIGIN = 0x2000000, LENGTH = 0x10000 /* 64K bytes (alias RAM) */  
  Ram1_32 (rwx) : ORIGIN = 0x2010000, LENGTH = 0x8000 /* 32K bytes (alias RAM2) */  
  Ram2_8 (rwx) : ORIGIN = 0x3400000, LENGTH = 0x2000 /* 8K bytes (alias RAM3) */  
}

  /* Define a symbol for the top of each memory region */
  __base_MFlash512 = 0x0  ; /* MFlash512 */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_MFlash512 = 0x0 + 0x80000 ; /* 512K bytes */  
  __top_Flash = 0x0 + 0x80000 ; /* 512K bytes */  
  __base_Ram0_64 = 0x2000000  ; /* Ram0_64 */  
  __base_RAM = 0x2000000 ; /* RAM */  
  __top_Ram0_64 = 0x2000000 + 0x10000 ; /* 64K bytes */  
  __top_RAM = 0x2000000 + 0x10000 ; /* 64K bytes */  
  __base_Ram1_32 = 0x2010000  ; /* Ram1_32 */  
  __base_RAM2 = 0x2010000 ; /* RAM2 */  
  __top_Ram1_32 = 0x2010000 + 0x8000 ; /* 32K bytes */  
  __top_RAM2 = 0x2010000 + 0x8000 ; /* 32K bytes */  
  __base_Ram2_8 = 0x3400000  ; /* Ram2_8 */  
  __base_RAM3 = 0x3400000 ; /* RAM3 */  
  __top_Ram2_8 = 0x3400000 + 0x2000 ; /* 8K bytes */  
  __top_RAM3 = 0x3400000 + 0x2000 ; /* 8K bytes */  

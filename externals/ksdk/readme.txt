Release Name: Kinetis     Version: 1.0.0
Software Development Kit (SDK)

-----------------------------------------------------------------------------
Revisions
  -- KSDK_1.0.0 rev.1 (2014-09-10)
     - reduced copy of Freescale KSDK_1.0.0
	 - mcu´s and bsp´s not used in CIAA-FSL project were removed.
     - this copy mantain directory structure for easy update from official 
       Freescale distribution of KSDK.
    Author: Darío Baliña, dariosb@gmail.com, Delsat Group.

-----------------------------------------------------------------------------

Package - Kinetis SDK     Filename: Freescale_KSDK_1.0.0.exe or
                          Freescale_KSDK_1.0.0.bin

CMSIS Core header files   Description: Industry-standard header files
                          distributed by ARM for Cortex-M cores
                          Version: 3.3.0
                          Author:  ARM
                          License: Open Source - BSD-3-Clause
                          Format: source code
                          URL: http://silver.arm.com
                          Location: 
                          KSDK_1.0.0/platform/CMSIS/Include

mmCAU S/W Library         Description: S/W library that works with the
                          memory-mapped cryptographic acceleration
                          unit present on some Kinetis SoCs
                          Version: 1.2
                          Author: Freescale
                          License: Open Source - BSD-3-Clause
                          Format: source code
                          Location:
                          KSDK_1.0.0/platform/drivers/mmcau

Flash / NVM               Description: Software driver (SSD) for Flash
                          and EEPROM
                          Version: 1.01
                          Author: Freescale
                          License: Open Source - BSD-3-Clause
                          Format: source code
                          Location: 
                          KSDK_1.0.0/platform/drivers/flash

KSDK Peripheral Drivers   Description: Peripheral drivers are designed 
                          for the most common use cases identified for 
                          the underlying hardware block.
                          Version: 1.0.0
                          Author: Freescale
                          License: Open Source - BSD-3-Clause
                          Format: source code
                          Location: KSDK_1.0.0/platform/drivers 

KSDK H/W Abstraction      Description: The HAL abstracts the H/W register 
Layer (HAL)               access into a set of stateless functional 
                          primitives which provide the building blocks for 
                          peripheral drivers or applications.
                          Version: 1.0.0
                          Author: Freescale
                          License: Open Source - BSD-3-Clause
                          Format: source code
                          Location: KSDK_1.0.0/platform/hal

lwIP TCP/IP Stack         Description: A light-weight TCP/IP stack
                          Version: 1.4.1
                          Author: Swedish Institute of Computer Science 
                          License: Open Source - BSD-3-Clause
                          Format: source code
                          URL: http://savannah.nongnu.org/projects/lwip/
                          Location: KSDK_1.0.0/tcpip/lwip

FatFs                     Description: FatFs is a generic FAT file system 
                          module for small embedded systems. 
                          Version: R0.09b
                          Author:  Electronic Lives Mfg. by ChaN of Ageo city, 
                          Japan (hobbyist)
                          License: Open Source - FatFs License
                          Approved Open Source License: Yes
                          Format: source code
                          URL: http://elm-chan.org/fsw/ff/00index_e.html
                          Location: KSDK_1.0.0/filesystem/fatfs

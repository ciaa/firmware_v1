###############################################################################
#
# Copyright 2016, Franco Bucafusco (BuckLabs)
# Copyright 2014, Mariano Cerdeiro
# Copyright 2014, 2015, Juan Cecconi (UTN-FRBA)
# Copyright 2015, Esteban Volentini (LabMicro, UNT)
# Copyright 2014, ACSE & CADIEEL
#    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
#    CADIEEL: http://www.cadieel.org.ar
# All rights reserved
#
# This file is part of CIAA Firmware.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
###############################################################################
# TOOLCHAIN
CC = msp430-elf-gcc
LD = msp430-elf-gcc
AR = msp430-elf-ar
AS = msp430-elf-as
GDB = msp430-elf-gdb
NM = msp430-elf-nm

# COMPILER AND LINKER OPTIONS
#AFLAGS += -ggdb3 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb
CFLAGS += -c -Wall -g3  -gdwarf-2 -ggdb -mmcu=$(CPU) 
LFLAGS += $(LINKSCRIPT)  -mmcu=$(CPU)  
#-Map="$(TARGET_NAME).map" 

# Optional POST BUILD
# Generates a .bin image file
POST_BUILD += msp430-elf-objcopy -v -O binary $(LD_TARGET) $(TARGET_NAME).bin

# define linker extension
LD_EXTENSION                = out

# gdb Flag
#GDB_FLAGS 		   = $(LD_TARGET) -ex "target remote localhost:3333"

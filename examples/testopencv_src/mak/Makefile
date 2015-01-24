###############################################################################
#
# Copyright 2014, ACSE & CADIEEL
#    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
#    CADIEEL: http://www.cadieel.org.ar
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
# Project Name: used for Project Path and OSEK configuration file
project 			 	 = testopencv_src
# Project path
$(project)_PATH   	 = $(ROOT_DIR)$(DS)examples$(DS)$(project)
# source path
$(project)_SRC_PATH  += $($(project)_PATH)$(DS)src
# include path
INC_FILES			   += $($(project)_PATH)$(DS)inc
INC_FILES			   += $($(project)_PATH)$(DS)src$(DS)core$(DS)include
# library source files
SRC_FILES 	  	   += $(wildcard $($(project)_SRC_PATH)$(DS)*.c)
SRC_FILES 	  	   += $(wildcard $($(project)_SRC_PATH)$(DS)*.cpp)
SRC_FILES 	  	   += $(wildcard $($(project)_SRC_PATH)$(DS)core$(DS)src$(DS)*.cpp)
# configuration for OSEK-OS
OIL_FILES		   += $($(project)_PATH)$(DS)etc$(DS)$(project).oil

vpath %.cpp $($(project)_SRC_PATH)
vpath %.cpp $($(project)_SRC_PATH)$(DS)core$(DS)src

ifeq ($(ARCH),x86)
LFLAGS += -L./examples/testopencv_src/src -lzlib -I./examples/testopencv_src/inc -lm -ldl
CPROYECT = $(CPP)
else
LFLAGS += -L"/home/so/Facultad/tesisi/workspace/gcc-arm-none-eabi-4_8-2014q2/arm-none-eabi/lib" -lg -lnosys -lstdc++ -lsupc++ -lgloss-linux -lrdimon
CFLAGS += -D__arch_cortex__
CPP = arm-none-eabi-g++
CPROYECT = $(CPP)
endif

# Modules needed for this example
MODS ?= modules$(DS)posix           \
        modules$(DS)ciaak           \
        modules$(DS)config          \
        modules$(DS)bsp             \
        modules$(DS)platforms       \
        modules$(DS)rtos            \
        modules$(DS)libs


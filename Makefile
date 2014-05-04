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
# ARCH, CPUTYPE and CPU following are supported
# +--------------+---------------+---------------+
# |      ARCH    |    CPUTYPE    |      CPU      |
# +--------------+---------------+---------------+
# | cortexM4     | lpc4000       | lpc4337       |
# +--------------+---------------+---------------+
#
ARCH = cortexM4
CPUTYPE = lpc4000
CPU = lpc4337

# MODULES
#
# Available modules are:
# examples/blinking
# modules/posix
#
MODS += modules$(DS)posix      			\
		  modules$(DS)ciaak					\
		  modules$(DS)config					\
        modules$(DS)bsp						\
        modules$(DS)platforms 			\
        examples$(DS)blinking_make		

DS = /
######################## DO NOT CHANGE AFTER THIS LINE ########################
###############################################################################
# get root dir
ROOT_DIR := $(shell pwd)
# out dir
OUT_DIR	= $(ROOT_DIR)$(DS)out
# object dir
OBJ_DIR  = $(OUT_DIR)$(DS)obj
# lib dir
LIB_DIR	= $(OUT_DIR)$(DS)lib
# bin dir
BIN_DIR	= $(OUT_DIR)$(DS)bin

# include needed modules
include $(foreach module, $(MODS), $(module)$(DS)mak$(DS)Makefile)

.DEFAULT_GOAL := $(project)

# add include files
INCLUDE += $(foreach LIB, $(LIBS), $($(LIB)_INC_PATH))
CFLAGS  +=-ggdb -Wall -Werror
CFLAGS  += $(foreach inc, $(INCLUDE), -I$(inc))


# create list of object files
$(foreach LIB, $(LIBS), $(eval $(LIB)_OBJ_FILES = $($(LIB)_SRC_FILES:.c=.o)) )

	
#rule for library
define librule
$(LIB_DIR)$(DS)$(strip $(1)).a : $(2)
	@echo ===============================================================================
	@echo Creating library $(1)
	$(AR) -rcs -o $(LIB_DIR)$(DS)$(strip $(1)).a $(2)
	
info_$(strip $(1)) :
	@echo Info of Library $(1)
	@echo Path........: $($(strip $(1))_PATH)
	@echo Include path: $($(strip $(1))_INC_PATH)
	@echo Source path.: $($(strip $(1))_SRC_PATH)
	@echo Source files: $($(strip $(1))_SRC_FILES)

endef

OBJ_FILES = $(SRC_FILES:.c=.o)

# create rule for library
# lib.a : lib_OBJ_FILES.o
$(foreach LIB, $(LIBS), $(eval $(call librule, $(LIB), $($(LIB)_OBJ_FILES))) )

$(foreach LIB, $(LIBS), $(eval $(LIB) : $(LIB_DIR)$(DS)$(LIB).a ) )

# compile rule
%.o : %.c
	@echo ===============================================================================
	@echo Compiling $<
	$(CC) -c $(CFLAGS) $< -o $@

# link rule
          
$(project) : $(LIBS) $(OBJ_FILES)
	@echo ===============================================================================
	@echo Linking $(project)
	$(CC) $(OBJ_FILES) $(foreach lib, $(LIBS), $(LIB_DIR)$(DS)$(lib).a) -o $(BIN_DIR)$(DS)$(project).bin
#	$(LD) -lcrt1 -Map $(BIN_DIR)$(DS)$(project).map --library-path=$(LIB_DIR)$(DS) $(OBJ_FILES) $(foreach lib, $(LIB_DIR)$(DS)$(LIBS).a, $(lib)) -o $(BIN_DIR)$(DS)$(project).bin

help:
	@echo info.......: general information about the make environment
	@echo info_\<mod\>.: same as info but reporting information of a library

info:
	@echo modules....: $(MODS)
	@echo includes...: $(INCLUDE)
	@echo libraries..: $(LIBS)
	@echo source.....: $(SRC_FILES)

.PHONY: clean
clean:
	rm -rf $(LIB_DIR)$(DS)*
	rm -rf $(BIN_DIR)$(DS)*
	find -name "*.o" -exec rm {} \;

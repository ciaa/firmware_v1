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
######################## DO NOT CHANGE THIS FILE ##############################
###############################################################################
# You dont have to change this file, never.
#
# Copy Makefile.config to Makefile.mine and set the variables.
#
# Makefile.mine will be included by this root Makefile and your configurations
# in Makefile.mine will be ignored due to .gitignore.
#
# Please take into account to check and compare your Makefile.mine every time
# that Makefile.config is updated, you may have to adapt your Makefile.mine
# if Makefile.config is changed.
#
###############################################################################
-include Makefile.mine
###############################################################################
# ARCH, CPUTYPE and CPU following are supported
# +--------------+---------------+---------------+--------------+
# |      ARCH    |    CPUTYPE    |      CPU      | COMPILER     |
# +--------------+---------------+---------------+--------------+
# | posix        | posix32       |               | gcc          |
# |              | posix64       |               |              |
# +--------------+---------------+---------------+--------------+
# | cortexM4     | lpc4000       | lpc4337       |              |
# +--------------+---------------+---------------+--------------+
#
ARCH 				?= posix
CPUTYPE 			?= posix64
CPU 				?=
COMPILER			?= gcc

DS 					?= /
# MODULES
#
# Available modules are:
# examples/blinking
# modules/posix
#
MODS ?= modules$(DS)posix      			\
		modules$(DS)ciaak				\
		modules$(DS)config				\
        modules$(DS)bsp					\
        modules$(DS)platforms 			\
		modules$(DS)rtos				\
        examples$(DS)blinking_make

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
CFLAGS  += -ggdb -Wall -Werror
CFLAGS  += $(foreach inc, $(INCLUDE), -I$(inc))
CFLAGS  += -DCPU=$(CPU)


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

###################### START UNIT TEST PART OF MAKE FILE ######################
# Gets all Modules Names
DIRS := $(sort $(dir $(wildcard modules$(DS)*$(DS))))
ALL_MODS := $(subst modules, , $(DIRS))
ALL_MODS := $(subst $(DS), , $(ALL_MODS))

MOCKS_OUT_DIR = out$(DS)ceedling$(DS)mocks

FILES_TO_MOCK = $(foreach DIR, $(DIRS), $(wildcard $(DIR)inc$(DS)*.h))

FILES_MOCKED = $(foreach MOCKED, $(FILES_TO_MOCK), $(MOCKS_OUT_DIR)$(DS)mock_$(notdir $(MOCKED)))

# Define rule for testing
define tstrule
tmp = $(strip $(1))

tst_$(tmp):
	@echo ===============================================================================
	@echo Testing $(1)
endef

# Creates all tst_<MOD> rules
$(foreach TST, $(ALL_MODS), $(eval $(call tstrule, $(TST))))

mocks: $(FILES_MOCKED)
	@echo $(FILES_MOCKED)
	@echo ===============================================================================
	@echo Creating Mocks for $(FILES_TO_MOCK)

#	@echo Mocked files $(FILES_MOCKED)
#	@echo $(MOCKS_OUT_DIR)$(DS)mock_%.h : %.h
#	ruby externals/ceedling/vendor/cmock/lib/cmock.rb -omodules/tools/ceedling/project.yml modules/posix/inc/ciaaPOSIX_stdio.h

# Rule to create Mock Files
mock_%.h :
	@echo ===============================================================================
	@echo Craeting Mocks $@
	@echo Creating Mocks $(notdir $@)
	@echo Searching $(subst mock_,,$(notdir $@))
	@echo Searching in $(FILES_TO_MOCK)
	@echo Found $(foreach MOCKH, $(FILES_TO_MOCK), ifn$(findstring $(DS)$(subst mock_,,$(notdir $@)), $(MOCKH)))
	@echo Found $(foreach MOCKH, $(FILES_TO_MOCK), $(MOCKH))

#	@echo  $(findstring $(subst mock_,,$(notdir $@)), $(FILES_TO_MOCK))

###################### ENDS UNIT TEST PART OF MAKE FILE #######################

# Rule to compile
%.o : %.c
	@echo ===============================================================================
	@echo Compiling $<
	$(CC) -c $(CFLAGS) $< -o $@

# link rule

$(project) : $(LIBS) $(OBJ_FILES)
	@echo ===============================================================================
	@echo Linking $(project)
	$(CC) $(OBJ_FILES) $(foreach lib, $(LIBS), $(LIB_DIR)$(DS)$(lib).a) -o $(BIN_DIR)$(DS)$(project).bin $(LFLAGS)
#	$(LD) -lcrt1 -Map $(BIN_DIR)$(DS)$(project).map --library-path=$(LIB_DIR)$(DS) $(OBJ_FILES) $(foreach lib, $(LIB_DIR)$(DS)$(LIBS).a, $(lib)) -o $(BIN_DIR)$(DS)$(project).bin


###############################################################################
# generation
GENDIR			= out$(DS)gen
generate : $(OIL_FILES)
	php modules$(DS)rtos$(DS)generator$(DS)generator.php --cmdline -l -v -c \
		$(OIL_FILES) -f $(rtos_GEN_FILES) -o $(GENDIR)

###############################################################################
# doxygen
doxygen:
	@echo running doxygen
	doxygen modules/tools/doxygen/doxygen.cnf

###############################################################################
# help
help:
	@echo info.......: general information about the make environment
	@echo info_\<mod\>.: same as info but reporting information of a library
	@echo tst_\<mod\>..: runs the tests of the indicated module
	@echo generate...: generates the ciaaRTOS

###############################################################################
# information
info:
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               General Info                                                  |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo enable modules....: $(MODS)
	@echo libraries.........: $(LIBS)
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Compiler Info                                                 |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo Compiler..........: $(COMPILER)
	@echo CC................: $(CC)
	@echo AR................: $(AR)
	@echo LD................: $(LD)
	@echo Compile Flags.....: $(CFLAGS)
#	@echo source............: $(SRC_FILES)
#	@echo all modules.......: $(DIRS)
# @echo includes..........: $(INCLUDE)

.PHONY: clean
clean:
	rm -rf $(LIB_DIR)$(DS)*
	rm -rf $(BIN_DIR)$(DS)*
	rm -rf $(GENDIR)$(DS)*
	find -name "*.o" -exec rm {} \;

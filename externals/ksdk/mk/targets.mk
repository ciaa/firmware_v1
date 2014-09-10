#-------------------------------------------------------------------------------
# Copyright (c) 2012 Freescale Semiconductor, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# o Redistributions of source code must retain the above copyright notice, this list
#   of conditions and the following disclaimer.
#
# o Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
# o Neither the name of Freescale Semiconductor, Inc. nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Sources and objects
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Different targets will be placed in separate folders.
#
# Libs:     output/<Debug>
#           output/<Release>
# Lib objs: output/<Debug>/obj
#           output/<Release>/obj
#
# Apps:     output/<Sram_Debug>
#           output/<Sram_Rlease>
#           output/<Flash_Debug>
#           output/<Flash_Rlease>
# App objs: output/<Sram_Debug>/obj
#           output/<Sram_Rlease>/obj
#           output/<Flash_Debug>/obj
#           output/<Flash_Rlease>/obj
#-------------------------------------------------------------------------------

ifeq "$(build)" "debug"
DEBUG_OR_RELEASE := Debug
else
DEBUG_OR_RELEASE := Release
endif
ifneq "$(APP_NAME)" ""
ifeq "$(target)" "sram"
TARGET_OUTPUT_ROOT := $(OUTPUT_ROOT)/Sram_$(DEBUG_OR_RELEASE)
else
TARGET_OUTPUT_ROOT := $(OUTPUT_ROOT)/Flash_$(DEBUG_OR_RELEASE)
endif
else
TARGET_OUTPUT_ROOT := $(OUTPUT_ROOT)/$(DEBUG_OR_RELEASE)
endif

OBJS_ROOT = $(TARGET_OUTPUT_ROOT)/obj

# Strip sources.
SOURCES := $(strip $(SOURCES))

# Convert sources list to absolute paths and root-relative paths.
SOURCES_ABS := $(foreach s,$(SOURCES),$(abspath $(s)))
SOURCES_REL := $(subst $(SDK_ROOT)/,,$(SOURCES_ABS))

# Get a list of unique directories containing the source files.
SOURCE_DIRS_ABS := $(sort $(foreach f,$(SOURCES_ABS),$(dir $(f))))
SOURCE_DIRS_REL := $(subst $(SDK_ROOT)/,,$(SOURCE_DIRS_ABS))

OBJECTS_DIRS := $(addprefix $(OBJS_ROOT)/,$(SOURCE_DIRS_REL))

# Filter source files list into separate source types.
C_SOURCES = $(filter %.c,$(SOURCES_REL))
CXX_SOURCES = $(filter %.cpp,$(SOURCES_REL))
ASM_s_SOURCES = $(filter %.s,$(SOURCES_REL))
ASM_S_SOURCES = $(filter %.S,$(SOURCES_REL))

# Convert sources to objects.
OBJECTS_C := $(addprefix $(OBJS_ROOT)/,$(C_SOURCES:.c=.o))
OBJECTS_CXX := $(addprefix $(OBJS_ROOT)/,$(CXX_SOURCES:.cpp=.o))
OBJECTS_ASM := $(addprefix $(OBJS_ROOT)/,$(ASM_s_SOURCES:.s=.o))
OBJECTS_ASM_S := $(addprefix $(OBJS_ROOT)/,$(ASM_S_SOURCES:.S=.o))

# Complete list of all object files.
OBJECTS_ALL := $(sort $(OBJECTS_C) $(OBJECTS_CXX) $(OBJECTS_ASM) $(OBJECTS_ASM_S))

#-------------------------------------------------------------------------------
# The make target is lib or application.
#-------------------------------------------------------------------------------

# Construct full path name
ifneq "$(APP_NAME)" ""
MAKE_TARGET ?= $(TARGET_OUTPUT_ROOT)/$(APP_NAME).elf
else
MAKE_TARGET ?= $(TARGET_OUTPUT_ROOT)/$(LIB_NAME).a
endif

#-------------------------------------------------------------------------------
# Expand the include paths
#-------------------------------------------------------------------------------
INCLUDES := $(foreach includes, $(INCLUDES), -I$(includes))

#-------------------------------------------------------------------------------
# Default target
#-------------------------------------------------------------------------------

# Note that prerequisite order is important here. The subdirectories must be built first, or you
# may end up with files in the current directory not getting added to libraries. This would happen
# if subdirs modified the library file after local files were compiled but before they were added
# to the library.
.PHONY: all
all: $(MAKE_TARGET)

# Recipe to create the output object file directories.
$(OBJECTS_DIRS) :
	$(at)mkdir -p $@

# Object files depend on the directories where they will be created.
#
# The dirs are made order-only prerequisites (by being listed after the '|') so they won't cause
# the objects to be rebuilt, as the modification date on a directory changes whenver its contents
# change. This would cause the objects to always be rebuilt if the dirs were normal prerequisites.
$(OBJECTS_ALL): | $(OBJECTS_DIRS)

#-------------------------------------------------------------------------------
# Pattern rules for compilation
#-------------------------------------------------------------------------------
# We cd into the source directory before calling the appropriate compiler. This must be done
# on a single command line since make calls individual recipe lines in separate shells, so
# '&&' is used to chain the commands.
#
# Generate make dependencies while compiling using the -MMD option, which excludes system headers.
# If system headers are included, there are path problems on cygwin. The -MP option creates empty
# targets for each header file so that a rebuild will be forced if the file goes missing, but
# no error will occur.

# Compile C sources.
$(OBJS_ROOT)/%.o: $(SDK_ROOT)/%.c
	@$(call printmessage,c,Compiling, $(subst $(SDK_ROOT)/,,$<))
	$(at)$(CC) $(CFLAGS) $(SYSTEM_INC) $(INCLUDES) $(DEFINES) -MMD -MF $(basename $@).d -MP -o $@ -c $<

# Compile C++ sources.
$(OBJS_ROOT)/%.o: $(SDK_ROOT)/%.cpp
	@$(call printmessage,cxx,Compiling, $(subst $(SDK_ROOT)/,,$<))
	$(at)$(CXX) $(CXXFLAGS) $(SYSTEM_INC) $(INCLUDES) $(DEFINES) -MMD -MF $(basename $@).d -MP -o $@ -c $<

# For .S assembly files, first run through the C preprocessor then assemble.
$(OBJS_ROOT)/%.o: $(SDK_ROOT)/%.S
	@$(call printmessage,asm,Assembling, $(subst $(SDK_ROOT)/,,$<))
	$(at)$(CC) $(CFLAGS) -D__LANGUAGE_ASM__ $(INCLUDES) $(DEFINES) -MMD -MF $(basename $@).d -o $@ -c $<

# Assembler sources.
$(OBJS_ROOT)/%.o: $(SDK_ROOT)/%.s
	@$(call printmessage,asm,Assembling, $(subst $(SDK_ROOT)/,,$<))
	$(at)$(AS) $(ASFLAGS) $(INCLUDES) -MD $(basename $@).d -o $@ $<

#------------------------------------------------------------------------
# Build the tagrget
#------------------------------------------------------------------------
# If build a library
ifeq "$(APP_NAME)" ""
$(MAKE_TARGET): $(OBJECTS_ALL)
	@$(call printmessage,ar,Archiving, $(?F) in $(@F))
	$(at)mkdir -p $(dir $(@))
	@$(AR) $(ARFLAGS) $@ $?
	$(call POSTBUILD)
ifneq "$(FINAL_OUTPUT)" ""
	@cp $(MAKE_TARGET) $(FINAL_OUTPUT)
endif

# If build an application
else
app_bin = $(basename $(MAKE_TARGET)).bin
app_map = $(basename $(MAKE_TARGET)).map

# Link the application.
# Wrap the link objects in start/end group so that ld re-checks each
# file for dependencies.  Otherwise linking static libs can be a pain
# since order matters.
$(MAKE_TARGET): $(LIBRARIES) $(OBJECTS_ALL) $(LD_FILE_NAME)
	@$(call printmessage,link,Linking, $(APP_NAME))
	$(at)$(LD) $(LDFLAGS) \
          -T$(LD_FILE_NAME) \
          $(LDINC) \
          -Xlinker --start-group \
          $(OBJECTS_ALL) \
          -lc -lgcc -lm  \
          $(LINK_LIB_NAME) \
          $(LIBNOSYS)      \
          $(SYSOBJ)    \
          -Xlinker --end-group \
          -o $@ \
          -Xlinker -Map=$(app_map) \
          -Xlinker --defsym=__heap_size__=$(heap) \
          -Xlinker --defsym=__stack_size__=$(stack)
	$(at)$(OBJCOPY) --gap-fill 0x00 -I elf32-little -O binary $@ $(app_bin)
	@echo "Output ELF:" ; echo "  $(MAKE_TARGET)"
	@echo "Output binary:" ; echo "  $(app_bin)"

endif

#-------------------------------------------------------------------------------
# Clean
#-------------------------------------------------------------------------------
.PHONY: clean cleanall
.PHONY: $(LINK_LIB_NAME)
cleanall: clean $(LINK_LIB_NAME)
clean: 
	-rm -rf $(OBJECTS_ALL) $(OBJECTS_DIRS) $(MAKE_TARGET) $(app_bin) $(app_map)
	$(POSTCLEAN)
$(LINK_LIB_NAME):
	$(MAKE) -C $(SDK_ROOT)/lib/gcc/$(CHIP)/$@ clean;	\
	rm -rf $(SDK_ROOT)/lib/gcc/$(CHIP)/output/$(DEBUG_OR_RELEASE)/*

# Include dependency files.
-include $(OBJECTS_ALL:.o=.d)


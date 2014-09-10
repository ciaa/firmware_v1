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
# Compiler flags
#-------------------------------------------------------------------------------

# Enables all warnings
C_FLAGS_WARNINGS = -Wall

# Turn on all warnings.
COMMON_FLAGS += $(C_FLAGS_WARNINGS)

# Don't use common symbols.  This is usually done in kernels.  Makes
# code size slightly larger and increases performance.
COMMON_FLAGS += -fno-common

#Place each function or data item into its own section in the output file
COMMON_FLAGS += -ffunction-sections -fdata-sections

# Use a freestanding build environment.  Standard for kernels, implies
# std library may not exist.
COMMON_FLAGS += -ffreestanding -fno-builtin

# Set the C standard to C99 with GNU extensions.
# Use traditional GNU inline function semantics.
C99_FLAGS = -std=gnu99

# Generate code specifically for cortex-mx CPU.
# Use the ARM Procedure Call Standard.
ARM_FLAGS = -mcpu=$(CPU) -mthumb

# Use float-abi=softfp for soft floating point api with HW instructions.
# Alternatively, float-abi=hard for hw float instructions and pass float args in float regs.
ifeq "$(CPU)" "cortex-m4"
ifeq "$(CHOOSE_FLOAT)" "SOFT_FP"
ARM_FLAGS += -mfloat-abi=softfp -mfpu=fpv4-sp-d16
else ifeq "$(CHOOSE_FLOAT)" "HARD_FP"
ARM_FLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
else
endif
endif

CC_LIB_POST := $(shell $(CC) -print-multi-directory $(ARM_FLAGS))

# AS flags.
ASFLAGS := $(ARM_FLAGS)

# Debug setting
ifeq "$(DEBUG)" "1"
COMMON_FLAGS += -O0
COMMON_FLAGS += -g2 -gdwarf-2 -gstrict-dwarf
ASFLAGS += -g
DEFINES += -D_DEBUG=1
else
COMMON_FLAGS += -Os
endif

# Build common flags shared by C and C++.
COMMON_FLAGS += $(ARM_FLAGS) -mtune=$(CPU) -mapcs

# C flags. Set C99 mode.
CFLAGS += $(COMMON_FLAGS) $(C99_FLAGS) -nostdinc

# C++ flags. Disable exceptions and RTTI.
CXXFLAGS += $(COMMON_FLAGS) -fno-exceptions -fno-rtti -nostdinc -nostdinc++

# AR flags.
ARFLAGS := -rucs

# LD flags.
# Use newlib-nano. To disable it, specify USE_NANO=
ifeq "$(toolchain)" "armgcc"
USE_NANO = --specs=nano.specs
else ifeq "$(toolchain)" "kdsgcc"
USE_NANO = -nanolibc
endif
LDFLAGS := $(USE_NANO)

SYSOBJ := $(LIBGCC_LDPATH)/crti.o \
          $(LIBGCC_LDPATH)/crtn.o \
          $(LIBC_LDPATH)/crt0.o

ifeq "$(toolchain)" "armgcc"
LIBNOSYS := -lnosys
endif

ifeq "$(toolchain)" "kdsgcc"
DEFINES += -DKDS
endif

LDFLAGS += $(COMMON_FLAGS) -nostartfiles -nodefaultlibs -nostdlib -Xlinker --gc-sections -Xlinker -cref -Xlinker -static -Xlinker -z -Xlinker muldefs

#-------------------------------------------------------------------------------
# Include paths
#-------------------------------------------------------------------------------

# These include paths have to be quoted because they may contain spaces,
# particularly under cygwin.
LDINC += -L '$(LIBGCC_LDPATH)' -L '$(LIBC_LDPATH)'

# Indicate gcc and newlib std includes as -isystem so gcc tags and
# treats them as system directories.
SYSTEM_INC = \
    -isystem '$(CC_INCLUDE)' \
    -isystem '$(CC_INCLUDE_FIXED)' \
    -isystem '$(LIBC_INCLUDE)'

INCLUDES += \
    $(SDK_ROOT)/platform \
    $(SDK_ROOT)/platform/hal  \
    $(SDK_ROOT)/platform/drivers  \
    $(SDK_ROOT)/platform/utilities  \
    $(SDK_ROOT)/platform/CMSIS/Include  \
    $(SDK_ROOT)/platform/CMSIS/Include/device \
    $(SDK_ROOT)/boards


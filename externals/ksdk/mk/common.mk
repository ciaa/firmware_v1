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


# The rest of the paths are defined in a separate makefile for easy access.
include $(SDK_ROOT)/mk/paths.mk

#-------------------------------------------------------------------------------
# Utility
#-------------------------------------------------------------------------------

# Kludge to create a variable equal to a single space.
empty :=
space := $(empty) $(empty)

#-------------------------------------------------------------------------------
# OS
#-------------------------------------------------------------------------------

# Get the OS name. Known values are "Linux", "CYGWIN_NT-5.1", and "Darwin".
os_name := $(shell uname -s)

# Set to 1 if running on cygwin.
is_cygwin := $(and $(findstring CYGWIN,$(os_name)),1)

# Set to 1 if running on cygwin.
is_mingw := $(and $(findstring MINGW,$(os_name)),1)

# Set to 1 if running on redhat.
is_redhat := $(shell if [ -f /etc/redhat-release ]; then echo 1 ; fi)

# Disable parallel builds for cygwin since they hang.
ifeq "$(is_cygwin)" "1"
.NOTPARALLEL:
endif

ifeq "$(is_mingw)" "1"
.NOTPARALLEL:
endif

#-------------------------------------------------------------------------------
# Logging options
#-------------------------------------------------------------------------------

# Enable color output by default.
BUILD_SDK_COLOR ?= 1

# MAKE
MAKE := make
ifeq "$(is_cygwin)" "1"
MAKE := mingw32-make
endif

ifeq "$(is_mingw)" "1"
MAKE := mingw32-make
endif

# Normally, commands in recipes are prefixed with '@' so the command itself
# is not echoed by make. But if VERBOSE is defined (set to anything non-empty),
# then the '@' is removed from recipes. The 'at' variable is used to control
# this. Similarly, 'silent_make' is used to pass the -s option to child make
# invocations when not in VERBOSE mode.
ifeq "$(VERBOSE)" "1"
at :=
silent_make :=
else
at := @
silent_make := -s
endif

# These colors must be printed with the printf command. echo won't handle the
# escape sequences.
color_default = \033[00m
color_bold = \033[01m
color_red = \033[31m
color_green = \033[32m
color_yellow = \033[33m
color_blue = \033[34m
color_magenta = \033[35m
color_cyan = \033[36m
color_orange = \033[38;5;172m
color_light_blue = \033[38;5;039m
color_gray = \033[38;5;008m
color_purple = \033[38;5;097m

ifeq "$(BUILD_SDK_COLOR)" "1"
color_build := $(color_light_blue)
color_c := $(color_green)
color_cxx := $(color_green)
color_cpp := $(color_orange)
color_asm := $(color_magenta)
color_ar := $(color_yellow)
color_link := $(color_purple)
endif

# Used in printmessage if the color args are not present.
color_ :=

# Use in recipes to print color messages if printing to a terminal. If
# BUILD_SDK_COLOR is not set to 1, this reverts to a simple uncolorized printf.
# A newline is added to the end of the printed message.
#
# Arguments:
#  1 - name of the color variable (see above), minus the "color_" prefix
#  2 - first colorized part of the message
#  3 - first uncolorized part of the message
#  4 - color name for second colorized message
#  5 - second colorized message
#  6 - second uncolorized part of the message
#  7 - uncolorized prefix on the whole line; this is last because it is expected to be used rarely
#
# All arguments are optional.
#
# Use like:
#  $(call printmessage,cyan,Building, remainder of the message...)
ifeq "$(BUILD_SDK_COLOR)" "1"
define printmessage
if [ -t 1 ]; then printf "$(7)$(color_$(1))$(2)$(color_default)$(3)$(color_$(4))$(5)$(color_default)$(6)\n" ; \
else printf "$(7)$(2)$(3)$(5)$(6)\n" ; fi
endef
else
define printmessage
printf "$(7)$(2)$(3)$(5)$(6)\n" ; fi
endef
endif

#-------------------------------------------------------------------------------
# Compiler and tools
#-------------------------------------------------------------------------------

# Select whether the GCC in use is from the GNU ARM Embedded project, or Kinetis
# Design Studio. They require slightly different usage.
# To use KDS, please set toolchain := kdsgcc
# To use ARMGCC, please set toolchain := armgcc
toolchain ?= armgcc

# Get GCC installation path from system environment variable.
# Please set ARMGCC_DIR or KDSGCC_DIR.
ifeq "$(toolchain)" "armgcc"
ifeq "$(ARMGCC_DIR)" ""
$(error Please set the system environment variable ARMGCC_DIR)
else
GCC_TOOLCHAIN_DIR := $(ARMGCC_DIR)
endif
else ifeq "$(toolchain)" "kdsgcc"
ifeq "$(KDSGCC_DIR)" ""
$(error Please set the system environment variable KDSGCC_DIR)
else
GCC_TOOLCHAIN_DIR := $(KDSGCC_DIR)
endif
endif

# Set compiler version defaults.
CROSS_COMPILE = arm-none-eabi-

# Strip off the trailing '-', resulting in arm-none-eabi
CROSS_COMPILE_STRIP := $(CROSS_COMPILE:%-=%)

# Build tool names.
CC = $(GCC_TOOLCHAIN_DIR)/bin/$(CROSS_COMPILE)gcc
CXX = $(GCC_TOOLCHAIN_DIR)/bin/$(CROSS_COMPILE)g++
LD = $(GCC_TOOLCHAIN_DIR)/bin/$(CROSS_COMPILE)gcc
AS = $(GCC_TOOLCHAIN_DIR)/bin/$(CROSS_COMPILE)as
AR = $(GCC_TOOLCHAIN_DIR)/bin/$(CROSS_COMPILE)ar
OBJCOPY = $(GCC_TOOLCHAIN_DIR)/bin/$(CROSS_COMPILE)objcopy

CC_VERSION := $(shell $(CC) -dumpversion)

# Standard library include paths.
LIBGCC_LDPATH = $(GCC_TOOLCHAIN_DIR)/lib/gcc/$(CROSS_COMPILE_STRIP)/$(CC_VERSION)/$(CC_LIB_POST)
LIBC_LDPATH = $(GCC_TOOLCHAIN_DIR)/$(CROSS_COMPILE_STRIP)/lib/$(CC_LIB_POST)

# System header file include paths.
CC_INCLUDE = $(GCC_TOOLCHAIN_DIR)/lib/gcc/$(CROSS_COMPILE_STRIP)/$(CC_VERSION)/include
CC_INCLUDE_FIXED = $(GCC_TOOLCHAIN_DIR)/lib/gcc/$(CROSS_COMPILE_STRIP)/$(CC_VERSION)/include-fixed
LIBC_INCLUDE = $(GCC_TOOLCHAIN_DIR)/$(CROSS_COMPILE_STRIP)/include

#-------------------------------------------------------------------------------
# Target and board configuration
#-------------------------------------------------------------------------------

include $(SDK_ROOT)/mk/config.mk

#-------------------------------------------------------------------------------
# Compiler flags
#-------------------------------------------------------------------------------

include $(SDK_ROOT)/mk/flags.mk


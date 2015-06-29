###############################################################################
#
# Copyright 2014, Mariano Cerdeiro
# All rights reserved.
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
CC = mipsel-mcp-elf-gcc
LD = mipsel-mcp-elf-gcc
AR = mipsel-mcp-elf-ar
AS = mipsel-mcp-elf-as
GDB = mipsel-mcp-elf-gdb
NM = mipsel-mcp-elf-nm
# COMPILER AND LINKER OPTIONS
AFLAGS +=
# We do not recomend to use -O3, see https://github.com/ciaa/Firmware/issues/226
# (#226) for more information.
CFLAGS += -c -O2
LFLAGS +=

# Download firmware to the target, next command should be FW file
GDB_DOWNLOAD_TO_TARGET +=

# Optional POST BUILD
# Generates a .bin image file
POST_BUILD +=

# define linker extension
LD_EXTENSION                = axf

include $(base_PATH)$(DS)mak$(DS)$(ARCH)$(DS)$(COMPILER)$(DS)$(CPUTYPE)$(DS)Makefile


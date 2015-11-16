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
# library
LIBS               += base
# version
base_VERSION		  = 0.4.1
# library path
base_PATH      = $(ROOT_DIR)$(DS)modules$(DS)base
# library source path
base_SRC_PATH  = $(base_PATH)$(DS)src
# library include path
base_INC_PATH  = $(base_PATH)$(DS)inc                      \
   $(base_PATH)$(DS)$(ARCH)$(DS)inc                             \
   $(base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)inc              \
   $(base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)inc

# library source files
base_SRC_FILES = $(wildcard $(base_SRC_PATH)$(DS)*.c)
base_SRC_FILES += $(wildcard $(base_SRC_PATH)$(DS)$(ARCH)$(DS)*.c)
base_SRC_FILES += $(wildcard $(base_SRC_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)*.c)
base_SRC_FILES += $(wildcard $(base_SRC_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)*.c)

include $(base_PATH)$(DS)mak$(DS)$(ARCH)$(DS)$(COMPILER)$(DS)Makefile

# define default binary extension
BIN_EXT             ?= bin

# include external base makefile
include externals$(DS)base$(DS)mak$(DS)Makefile


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
# Project Name: based on Project Path and used to define OSEK configuration file
PROJECT_NAME               = $(lastword $(subst $(DS), , $(PROJECT_PATH)))
# Project path
# Defined $(PROJECT_PATH) in makefile.mine
# source path
$(PROJECT_NAME)_SRC_PATH  += $(PROJECT_PATH)$(DS)src
# include path
INC_FILES            += $(PROJECT_PATH)$(DS)inc
# library source files
SRC_FILES            += $(wildcard $($(PROJECT_NAME)_SRC_PATH)$(DS)*.c)
# configuration for OSEK-OS
OIL_FILES            += $(PROJECT_PATH)$(DS)etc$(DS)$(PROJECT_NAME).oil
# Modules needed for this example
MODS ?= modules$(DS)posix           \
        modules$(DS)ciaak           \
        modules$(DS)drivers         \
        modules$(DS)rtos            \
        modules$(DS)libs				\
		  modules$(DS)modbus


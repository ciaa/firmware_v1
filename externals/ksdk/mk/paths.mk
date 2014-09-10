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
# Root paths
#-------------------------------------------------------------------------------

# Build root directory paths.
SDK_LIB_ROOT = $(SDK_ROOT)/platform
APPS_ROOT = $(SDK_ROOT)/demos
BOARD_ROOT = $(SDK_ROOT)/boards/$(BOARD)
LD_FILE_ROOT = $(SDK_ROOT)/platform/linker/gcc
BOARD_COMMON_ROOT = $(SDK_ROOT)/boards/common
UCOSII_ROOT = $(SDK_ROOT)/rtos/uCOSII
UCOSIII_ROOT = $(SDK_ROOT)/rtos/uCOSIII
FREERTOS_ROOT = $(SDK_ROOT)/rtos/FreeRTOS
MQX_ROOT = $(SDK_ROOT)/rtos/mqx

#-------------------------------------------------------------------------------
# Output file paths
#-------------------------------------------------------------------------------

# Build output directory paths.
#
# All build products will be placed in the project's own dir
#
OUTPUT_ROOT = .


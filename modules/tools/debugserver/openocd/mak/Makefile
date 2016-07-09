###############################################################################
#
# Copyright 2014, Mariano Cerdeiro
# Copyright 2014, Juan Cecconi
#
# All Rights Reserved
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
# openocd path
OPENOCD_PATH 		   = $(ROOT_DIR)$(DS)modules$(DS)tools$(DS)debugserver$(DS)openocd

# openocd bin file
OPENOCD_BIN 		   = openocd

# openocd Flag
OPENOCD_FLAGS 		   = -f $(OPENOCD_CFG)

# openocd dependent makefile
include $(OPENOCD_PATH)$(DS)mak$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)Makefile

# openocd name
DEBUG_SERVER_NAME    = OpenOCD

# openocd debugserver command
DEBUG_SERVER_CMD     = $(OPENOCD_BIN) $(OPENOCD_FLAGS)

# openocd erase command
ifeq ($(words $(MAKE_ARGS)),0)
ERASE_CMD            = $(OPENOCD_BIN) $(OPENOCD_FLAGS) -c "init" -c "halt 0" -c "$(MASS_ERASE_COMMAND)" -c "shutdown"
MESSAGE	             = "(after downloading a new firmware please do a hardware reset!)"
else

ifeq ($(CPUTYPE),k60_120)
#not supported
else

ifeq ($(word 1, $(MAKE_ARGS)),FLASH)
ERASE_CMD            = $(OPENOCD_BIN) $(OPENOCD_FLAGS) -c "init" -c "halt 0" -c "$(FLASH_ERASE_COMMAND) $(TARGET_DOWNLOAD_FLASH_BANK) 0 last" -c "shutdown"
MESSAGE	            = "(after downloading a new firmware please do a hardware reset!)"
else

ifeq ($(word 1, $(MAKE_ARGS)),QSPI)
ERASE_CMD            = $(OPENOCD_BIN) $(OPENOCD_FLAGS) -c "init" -c "halt 0" -c "$(FLASH_ERASE_COMMAND) $(TARGET_DOWNLOAD_QSPI_BANK) 0 last" -c "shutdown"
MESSAGE	            = "(after downloading a new firmware please do a hardware reset!)"
else
#error
endif #ifeq ($(word 1, $(MAKE_ARGS)),QSPI)
endif #ifeq ($(word 1, $(MAKE_ARGS)),FLASH)
endif #ifeq ($(CPUTYPE),k60_120)
endif #($(words $(MAKE_ARGS)),0)

# openocd download command
ifeq ($(words $(MAKE_ARGS)),0)
DOWNLOAD_CMD 			= $(OPENOCD_BIN) $(OPENOCD_FLAGS) -c "init" -c "halt 0" -c "$(FLASH_WRITE_COMMAND) $(TARGET_NAME).$(TARGET_DOWNLOAD_EXTENSION) $(TARGET_DOWNLOAD_FLASH_BASE_ADDR) $(TARGET_DOWNLOAD_EXTENSION)" -c "reset run" -c "shutdown"
else

ifeq ($(words $(MAKE_ARGS)),1)
DOWNLOAD_CMD 			= $(OPENOCD_BIN) $(OPENOCD_FLAGS) -c "init" -c "halt 0" -c "$(FLASH_WRITE_COMMAND) $(word 1,$(MAKE_ARGS)) $(TARGET_DOWNLOAD_FLASH_BASE_ADDR) $(TARGET_DOWNLOAD_EXTENSION)" -c "reset run" -c "shutdown"
else
DOWNLOAD_CMD 			= $(OPENOCD_BIN) $(OPENOCD_FLAGS) -c "init" -c "halt 0" -c "$(FLASH_WRITE_COMMAND) $(word 1,$(MAKE_ARGS)) $(TARGET_DOWNLOAD_$(word 2,$(MAKE_ARGS))_BASE_ADDR) $(TARGET_DOWNLOAD_EXTENSION)" -c "reset run" -c "shutdown"
endif #($(words $(MAKE_ARGS)),1)

endif #($(words $(MAKE_ARGS)),0)

###############################################################################
#
# Copyright 2014, Mariano Cerdeiro, ACSE, CADIEEL
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
LIBS 				     += rtos
# library path
rtos_PATH 		   	  = $(ROOT_DIR)$(DS)modules$(DS)rtos
# library source path
rtos_SRC_PATH 	      = $(rtos_PATH)$(DS)src
# library include path
rtos_INC_PATH 	      = $(rtos_PATH)$(DS)inc								\
	$(rtos_PATH)$(DS)inc$(DS)$(ARCH)										\
	$(rtos_PATH)$(DS)inc$(DS)$(ARCH)$(DS)$(CPUTYPE)							\
	$(GENDIR)$(DS)inc														\
	$(GENDIR)$(DS)inc$(DS)$(ARCH)											\
	$(GENDIR)$(DS)inc$(DS)$(ARCH)$(DS)$(CPUTYPE)
# library source files
rtos_SRC_FILES 	      = $(wildcard $(rtos_SRC_PATH)$(DS)*.c)				\
						$(wildcard $(rtos_SRC_PATH)$(DS)$(ARCH)$(DS)*.c)	\
						$(wildcard $(rtos_SRC_PATH)$(DS)$(ARCH)$(DS)*.s)	\
						$(wildcard out$(DS)gen$(DS)src$(DS)*.c)				\
						$(wildcard out$(DS)gen$(DS)src$(DS)$(ARCH)$(DS)*.c)
# include needed makefiles depending on the architecture
-include modules$(DS)rtos$(DS)mak$(DS)$(ARCH)$(DS)Makefile
# files to be generated for ciaa RTOS OSEK
rtos_GEN_FILES += $(rtos_PATH)$(DS)gen$(DS)inc$(DS)Os_Internal_Cfg.h.php	\
	$(rtos_PATH)$(DS)gen$(DS)inc$(DS)Os_Cfg.h.php							\
	$(rtos_PATH)$(DS)gen$(DS)src$(DS)Os_Cfg.c.php							\
	$(rtos_PATH)$(DS)gen$(DS)src$(DS)Os_Internal_Cfg.c.php					\
	$(rtos_PATH)$(DS)gen$(DS)src$(DS)$(ARCH)$(DS)Os_Internal_Arch_Cfg.c.php \
	$(rtos_PATH)$(DS)gen$(DS)inc$(DS)$(ARCH)$(DS)Os_Internal_Arch_Cfg.h.php



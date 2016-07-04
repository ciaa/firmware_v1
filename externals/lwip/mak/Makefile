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
LIBS                += ext_lwip
# library path
ext_lwip_PATH 	     = $(ROOT_DIR)$(DS)externals$(DS)lwip
# library source path
ext_lwip_SRC_PATH   = $(ext_lwip_PATH)$(DS)lwip_src$(DS)src
   
# library include path
# LWIP
ext_lwip_INC_PATH   = $(ext_lwip_PATH)$(DS)lwip_src$(DS)src$(DS)include \
	$(ext_lwip_PATH)$(DS)lwip_src$(DS)src$(DS)include$(DS)ipv4
# IPv6 Not yet tested, not implemented
# ext_lwip_INC_PATH  += $(ext_lwip_PATH)$(DS)lwip_src$(DS)src$(DS)include$(DS)ipv6
# arch's Drivers
ext_lwip_INC_PATH   += $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)inc \
	$(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)inc \
	$(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)inc

# library source files
# LWIP
ext_lwip_SRC_FILES += $(wildcard $(ext_lwip_SRC_PATH)$(DS)*.c)	\
   $(wildcard $(ext_lwip_SRC_PATH)$(DS)api$(DS)*.c) \
   $(wildcard $(ext_lwip_SRC_PATH)$(DS)core$(DS)*.c)   \
   $(wildcard $(ext_lwip_SRC_PATH)$(DS)core$(DS)ipv4$(DS)*.c)   \
   $(wildcard $(ext_lwip_SRC_PATH)$(DS)core$(DS)snmp$(DS)*.c)   \
   $(wildcard $(ext_lwip_SRC_PATH)$(DS)netif$(DS)*.c)
# IPv6 Not yet tested, not implemented
# ext_lwip_SRC_FILES += $(wildcard $(ext_lwip_SRC_PATH)$(DS)core$(DS)ipv6$(DS)*.c)
# LWIP'  NON TESTED code
# ext_lwip_SRC_FILES += $(wildcard $(ext_lwip_SRC_PATH)$(DS)netif$(DS)ppp$(DS)*.c)
# arch's drivers
ext_lwip_SRC_FILES    += $(wildcard $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)src$(DS)*.c)
ext_lwip_SRC_FILES    += $(wildcard $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)src$(DS)arch$(DS)*.c)
ext_lwip_SRC_FILES    += $(wildcard $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)src$(DS)*.c)
ext_lwip_SRC_FILES    += $(wildcard $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)src$(DS)arch$(DS)*.c)
ext_lwip_SRC_FILES    += $(wildcard $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)src$(DS)*.c)
ext_lwip_SRC_FILES    += $(wildcard $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)src$(DS)arch$(DS)*.c)

# makefiles from arch
-include $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)mak$(DS)Makefile
-include $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)mak$(DS)Makefile
-include $(ext_lwip_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)mak$(DS)Makefile

# Enable this feature in the ciaak
CFLAGS += -DCIAA_CFG_NET_IP


###############################################################################
#
# Copyright 2008, 2009 Mariano Cerdeiro
# Copyright 2014, ACSE & CADIEEL
#      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
#      CADIEEL: http://www.cadieel.org.ar
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
# add library
#
OBJ += 	ctest_rst.o			\
			$(PROJECT).o

VPATH += FreeOSEK$(DIR)Os$(DIR)tst$(DIR)ctest$(DIR)src

INCLUDE += -IFreeOSEK$(DIR)Os$(DIR)tst$(DIR)ctest$(DIR)inc                    \
           -IFreeOSEK$(DIR)Os$(DIR)tst$(DIR)ctest$(DIR)inc$(DIR)$(ARCH)       \

CFGFILES += out$(DIR)gen$(DIR)etc$(DIR)$(PROJECT).oil									\
				FreeOSEK$(DIR)Os$(DIR)tst$(DIR)ctest$(DIR)etc$(DIR)OpenDRV.oil

GENFILES += FreeOSEK$(DIR)Os$(DIR)tst$(DIR)ctest$(DIR)gen$(DIR)inc$(DIR)ctest_cfg.h.php

CLEAN += ctestclean

CFLAGS += -D$(PROJECT)

ctestclean :

ctest :
	mkdir -p out$(DIR)doc$(DIR)ctest
	@echo GDB:$(GDB)> out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo BINDIR:$(BINDIR)>> out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo DIR:$(DIR)>> out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo ARCH:$(ARCH)>> out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo CPUTYPE:$(CPUTYPE)>> out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo CPU:$(CPU)>> out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo RES:out/doc/ctest/ctestresults.log>>out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo LOG:out/doc/ctest/ctest.log>>out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo LOGFULL:out/doc/ctest/ctestfull.log>>out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo TESTS:FreeOSEK/Os/tst/ctest/cfg/ctestcases.cfg>>out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	@echo TESTCASES:FreeOSEK/Os/tst/ctest/cfg/testcases.cfg>>out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf
	.$(DIR)FreeOSEK$(DIR)Os$(DIR)tst$(DIR)ctest$(DIR)bin$(DIR)ctest.pl -f out$(DIR)doc$(DIR)ctest$(DIR)ctest.cnf $(CTESTF)



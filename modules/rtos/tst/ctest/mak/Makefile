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



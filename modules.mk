#CuTest.o: CuTest.c
#	$(SH_COMPILE)
CuTest.lo: CuTest.c
	$(SH_COMPILE)
libmodcutest.la: CuTest.lo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  CuTest.lo
mod_cutest.la: libmodcutest.la mod_cutest.slo
	$(SH_LINK) libmodcutest.la -rpath $(libexecdir) -module -avoid-version  mod_cutest.lo libmodcutest.la 
DISTCLEAN_TARGETS = modules.mk
shared =  mod_cutest.la libmodcutest.la


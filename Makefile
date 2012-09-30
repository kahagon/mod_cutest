##
##  Makefile -- Build procedure for sample cutest Apache module
##  Autogenerated via ``apxs -n cutest -g''.
##

builddir=.
top_srcdir=/usr/share/apache2
top_builddir=/usr/share/apache2
include /usr/share/apache2/build/special.mk

#   the used tools
APXS=apxs
APACHECTL=apachectl

#   additional defines, includes and libraries
#DEFS=-Dmy_define=my_value
#INCLUDES=-Imy/include/dir
#LIBS=-Lmy/lib/dir -lmylib
INCLUDES=`pkg-config --cflags apr-1` `apreq2-config --includes`
LIBS = `pkg-config --libs apr-1` `apreq2-config --ldflags --includes` -lapreq2
CFLAGS = $(INCLUDES) $(LIBS)

#   the default target
all: local-shared-build

#   install the shared object file into Apache 
install: install-modules-yes
	install -d /usr/local/include/ /usr/local/lib/ /usr/local/bin/
	cp CuTest.h /usr/local/include/
	install -D modcutest-config.sh /usr/local/bin/modcutest-config
	install -D .libs/libmodcutest.so /usr/local/lib/
	ldconfig

#   cleanup
clean:
	-rm -f mod_cutest.o mod_cutest.lo mod_cutest.slo mod_cutest.la 

#   simple test
test: reload
	lynx -mime_header http://localhost/cutest

#   install and activate shared object by reloading Apache to
#   force a reload of the shared object file
reload: install restart

#   the general Apache start/restart/stop
#   procedures
start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop


AM_CPPFLAGS = -I @top_srcdir@/include

bin_PROGRAMS = sinsy.js

sinsy_js_SOURCES = sinsy.cpp

sinsy_js_LDADD = @top_srcdir@/lib/libSinsy.a

sinsy_js_LDFLAGS = -s "EXPORTED_FUNCTIONS=['_main', '_sinsy_main']" -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

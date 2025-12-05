
AM_CPPFLAGS = -I @top_srcdir@/include

bin_PROGRAMS = sinsy 

sinsy_SOURCES = sinsy.cpp

sinsy_LDADD = @top_srcdir@/lib/libSinsy.a

sinsy_LDFLAGS = -s "EXPORTED_FUNCTIONS=['_main', '_sinsy_main']" -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in 

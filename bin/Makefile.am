
AM_CPPFLAGS = -I @top_srcdir@/include

bin_PROGRAMS = sinsy.js

sinsy_js_SOURCES = sinsy.cpp

sinsy_js_LDADD = @top_srcdir@/lib/libSinsy.a

sinsy_js_LDFLAGS = -s "EXPORTED_FUNCTIONS=['_main', '_sinsy_main']" -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" --preload-file R:/sinsy/voices/nitech_jp_song070_f001.htsvoice --preload-file R:/sinsy/scores/Ave_Maria_D839_-_Schubert_-_Solo_Piano_Arrg.mxl

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = ncdump
man1_MANS = ncdump.1

INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF

ncdump_SOURCES = dumplib.c ncdump.c vardata.c
ncdump_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
ncdump_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a
ncdump_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check: ncdump ${srcdir}/test0.cdl
	../ncgen/ncgen -o test0.nc -n ${srcdir}/test0.cdl
	./ncdump test0.nc > test1.cdl
	../ncgen/ncgen -o test1.nc -n test1.cdl
	./ncdump -n test0 test1.nc > test2.cdl
	@cmp test1.cdl test2.cdl && echo "*** ncdump test successful ***" ;     \
	    rm -f test1.nc test1.cdl test2.cdl

bin_PROGRAMS = hdfimport

INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF

hdfimport_SOURCES = hdfimport.c
hdfimport_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hdfimport_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hdfimport_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check_PROGRAMS = hdfimporttest

hdfimporttest_SOURCES = hdfimporttest.c
hdfimporttest_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hdfimporttest_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hdfimporttest_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check: hdfimporttest
	sh testutil.sh

bin_PROGRAMS = hzip

INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port     \
         -I${top_srcdir}/mfhdf/hdiff    \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF

hzip_SOURCES = copy.c hdfzip.c hdfzip_main.c list.c parse.c table.c
hzip_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hzip_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hzip_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check_PROGRAMS = hziptst

hziptst_SOURCES = add.c copy.c hdfzip.c hdfziptst_main.c list.c parse.c     \
                  table.c verify.c ../hdiff/dumplib.c ../hdiff/hdiff.c      \
                  ../hdiff/hdiff_gattr.c ../hdiff/hdiff_grdata.c            \
                  ../hdiff/hdiff_misc.c ../hdiff/hdiff_sdattr.c             \
                  ../hdiff/hdiff_sddata.c ../hdiff/hdiff_vdata.c
hziptst_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hziptst_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hziptst_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

hzipck_SOURCES = hdfzipck.c
hzipck_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hzipck_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hzipck_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check: hziptst hzip
	sh ${srcdir}/testhzip.sh

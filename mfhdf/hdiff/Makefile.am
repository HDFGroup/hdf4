bin_PROGRAMS = hdiff

INCLUDES=-I${top_srcdir}/hdf/src        \
         -I${top_srcdir}/mfhdf/libsrc   \
         -I${top_srcdir}/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF

hdiff_SOURCES = dumplib.c hdiff.c hdiff_main.c hdiff_gattr.c hdiff_misc.c   \
                hdiff_sdattr.c hdiff_sddata.c hdiff_grdata.c hdiff_vdata.c
hdiff_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hdiff_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hdiff_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check_PROGRAMS = hdifftst

hdifftst_SOURCES = hdifftst.c
hdifftst_CPPFLAGS = ${INCLUDES} ${DEFINES} @CPPFLAGS@
hdifftst_LDADD = ../libsrc/libmfhdf.a ${top_builddir}/hdf/src/libdf.a @LIBS@
hdifftst_DEPENPENDENCIES = ${top_builddir}/hdf/src/libdf.a ../libsrc/libmfhdf.a

check: hdifftst
	sh ${srcdir}/testhdiff.sh
